
#pragma once

#include "System/Object.h"
#include "System/GC/GcWeakPtr.hpp"
#include "System/Threading/Mutex.h"
#include "System/Threading/Monitor.h"
#include <vector>
#include <memory>
#include <functional>
#include <algorithm>

namespace System
{
    /// <summary>
    /// Provides a type-safe EventManager that enables the "weak event listener" pattern.
    /// This allows the listener to be garbage collected without requiring explicit unsubscription.
    /// </summary>
    template<typename TEventSource, typename TEventArgs>
    class WeakEventManager : public Object
    {
    private:
        struct WeakEventEntry
        {
            GC::GcWeakPtr<Object> target;
            std::function<void(Object*, TEventArgs*)> handler;
            
            WeakEventEntry(GC::GcWeakPtr<Object> t, std::function<void(Object*, TEventArgs*)> h)
                : target(std::move(t)), handler(std::move(h)) {}
        };

        static WeakEventManager<TEventSource, TEventArgs>* s_instance;
        static System::Threading::Mutex s_instanceMutex;
        
        mutable System::Threading::Mutex m_mutex;
        std::vector<std::pair<GC::GcWeakPtr<TEventSource>, std::vector<WeakEventEntry>>> m_eventSources;

        WeakEventManager() = default;

        void CleanupDeadReferences()
        {
            System::Threading::Monitor::Enter(m_mutex);
            try
            {
                // Remove dead event sources
                m_eventSources.erase(
                    std::remove_if(m_eventSources.begin(), m_eventSources.end(),
                        [](const auto& pair) { return pair.first.IsExpired(); }),
                    m_eventSources.end());

                // Remove dead listeners from remaining sources
                for (auto& sourcePair : m_eventSources)
                {
                    auto& listeners = sourcePair.second;
                    listeners.erase(
                        std::remove_if(listeners.begin(), listeners.end(),
                            [](const WeakEventEntry& entry) { return entry.target.IsExpired(); }),
                        listeners.end());
                }
            }
            finally
            {
                System::Threading::Monitor::Exit(m_mutex);
            }
        }

        void OnEventRaised(Object* sender, TEventArgs* args)
        {
            std::vector<WeakEventEntry> listenersCopy;
            
            {
                System::Threading::Monitor::Enter(m_mutex);
                try
                {
                    // Find the event source
                    auto sourceIt = std::find_if(m_eventSources.begin(), m_eventSources.end(),
                        [sender](const auto& pair) {
                            auto source = pair.first.Lock();
                            return source && source.get() == sender;
                        });

                    if (sourceIt != m_eventSources.end())
                    {
                        listenersCopy = sourceIt->second;
                    }
                }
                finally
                {
                    System::Threading::Monitor::Exit(m_mutex);
                }
            }

            // Invoke handlers outside of lock
            for (const auto& entry : listenersCopy)
            {
                auto target = entry.target.Lock();
                if (target)
                {
                    try
                    {
                        entry.handler(sender, args);
                    }
                    catch (...)
                    {
                        // Continue with other handlers even if one throws
                    }
                }
            }

            // Clean up dead references periodically
            CleanupDeadReferences();
        }

    public:
        /// <summary>
        /// Gets the current manager instance for the specified event source and event args types.
        /// </summary>
        /// <returns>The current manager instance.</returns>
        static WeakEventManager<TEventSource, TEventArgs>& GetCurrentManager()
        {
            System::Threading::Monitor::Enter(s_instanceMutex);
            try
            {
                if (!s_instance)
                {
                    s_instance = new WeakEventManager<TEventSource, TEventArgs>();
                }
                return *s_instance;
            }
            finally
            {
                System::Threading::Monitor::Exit(s_instanceMutex);
            }
        }

        /// <summary>
        /// Adds the specified listener to the specified source for the provided event.
        /// </summary>
        /// <param name="source">The object with the event.</param>
        /// <param name="listener">The object to add as a listener.</param>
        /// <param name="handler">The method to call when the event is raised.</param>
        template<typename TListener>
        static void AddListener(GC::GcPtr<TEventSource> source, GC::GcPtr<TListener> listener, 
                               void(TListener::*method)(Object*, TEventArgs*))
        {
            if (!source || !listener)
                return;

            auto& manager = GetCurrentManager();
            
            // Create weak references
            GC::GcWeakPtr<TEventSource> weakSource(source);
            GC::GcWeakPtr<Object> weakListener(std::static_pointer_cast<Object>(listener));
            
            // Create handler function
            auto handler = [method](Object* sender, TEventArgs* args) {
                // This will be called with the actual listener instance
                auto* typedListener = static_cast<TListener*>(sender);
                (typedListener->*method)(sender, args);
            };

            System::Threading::Monitor::Enter(manager.m_mutex);
            try
            {
                // Find or create entry for this source
                auto sourceIt = std::find_if(manager.m_eventSources.begin(), manager.m_eventSources.end(),
                    [&weakSource](const auto& pair) {
                        auto existingSource = pair.first.Lock();
                        auto newSource = weakSource.Lock();
                        return existingSource && newSource && existingSource.get() == newSource.get();
                    });

                if (sourceIt == manager.m_eventSources.end())
                {
                    // First listener for this source - need to subscribe to the actual event
                    manager.m_eventSources.emplace_back(weakSource, std::vector<WeakEventEntry>());
                    sourceIt = manager.m_eventSources.end() - 1;
                    
                    // Subscribe to the actual event (this would need to be customized per event type)
                    manager.StartListening(source.get());
                }

                // Add the listener
                sourceIt->second.emplace_back(weakListener, handler);
            }
            finally
            {
                System::Threading::Monitor::Exit(manager.m_mutex);
            }
        }

        /// <summary>
        /// Removes the specified listener from the specified source for the provided event.
        /// </summary>
        /// <param name="source">The object with the event.</param>
        /// <param name="listener">The object to remove as a listener.</param>
        template<typename TListener>
        static void RemoveListener(GC::GcPtr<TEventSource> source, GC::GcPtr<TListener> listener)
        {
            if (!source || !listener)
                return;

            auto& manager = GetCurrentManager();
            
            System::Threading::Monitor::Enter(manager.m_mutex);
            try
            {
                // Find the source
                auto sourceIt = std::find_if(manager.m_eventSources.begin(), manager.m_eventSources.end(),
                    [source](const auto& pair) {
                        auto existingSource = pair.first.Lock();
                        return existingSource && existingSource.get() == source.get();
                    });

                if (sourceIt != manager.m_eventSources.end())
                {
                    auto& listeners = sourceIt->second;
                    
                    // Remove listeners that match the target
                    listeners.erase(
                        std::remove_if(listeners.begin(), listeners.end(),
                            [listener](const WeakEventEntry& entry) {
                                auto target = entry.target.Lock();
                                return target && target.get() == listener.get();
                            }),
                        listeners.end());

                    // If no more listeners, unsubscribe from the source
                    if (listeners.empty())
                    {
                        manager.StopListening(source.get());
                        manager.m_eventSources.erase(sourceIt);
                    }
                }
            }
            finally
            {
                System::Threading::Monitor::Exit(manager.m_mutex);
            }
        }

    protected:
        /// <summary>
        /// When overridden in a derived class, starts listening for the event being managed.
        /// </summary>
        /// <param name="source">The source to begin listening on.</param>
        virtual void StartListening(TEventSource* source)
        {
            // This method should be overridden to subscribe to the specific event
            // For example: source->add_SomeEvent(EventHandler::Create<&WeakEventManager::OnEventRaised>(this));
        }

        /// <summary>
        /// When overridden in a derived class, stops listening for the event being managed.
        /// </summary>
        /// <param name="source">The source to stop listening on.</param>
        virtual void StopListening(TEventSource* source)
        {
            // This method should be overridden to unsubscribe from the specific event
            // For example: source->remove_SomeEvent(EventHandler::Create<&WeakEventManager::OnEventRaised>(this));
        }
    };

    // Static member definitions
    template<typename TEventSource, typename TEventArgs>
    WeakEventManager<TEventSource, TEventArgs>* WeakEventManager<TEventSource, TEventArgs>::s_instance = nullptr;

    template<typename TEventSource, typename TEventArgs>
    System::Threading::Mutex WeakEventManager<TEventSource, TEventArgs>::s_instanceMutex;
}

// Macro to help create specific weak event managers
#define DECLARE_WEAK_EVENT_MANAGER(ManagerName, SourceType, EventArgsType, EventName) \
    class ManagerName : public System::WeakEventManager<SourceType, EventArgsType> \
    { \
    protected: \
        void StartListening(SourceType* source) override \
        { \
            auto handler = System::EventHandlerT<EventArgsType>::Create<ManagerName, &ManagerName::OnEventRaised>(this); \
            source->add_##EventName(handler); \
        } \
        \
        void StopListening(SourceType* source) override \
        { \
            auto handler = System::EventHandlerT<EventArgsType>::Create<ManagerName, &ManagerName::OnEventRaised>(this); \
            source->remove_##EventName(handler); \
        } \
    };

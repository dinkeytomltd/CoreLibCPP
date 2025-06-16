
#pragma once

#include "System/Object.h"
#include "System/String.h"

namespace System
{
    /// <summary>
    /// Represents the base class for classes that contain event data, and provides a value to use for events that do not include event data.
    /// </summary>
    class EventArgs : public Object
    {
    public:
        /// <summary>
        /// Provides a value to use with events that do not have event data.
        /// </summary>
        static const EventArgs Empty;

        /// <summary>
        /// Initializes a new instance of the EventArgs class.
        /// </summary>
        EventArgs() = default;

        /// <summary>
        /// Virtual destructor to ensure proper cleanup of derived classes.
        /// </summary>
        virtual ~EventArgs() = default;

        /// <summary>
        /// Returns a string that represents the current object.
        /// </summary>
        /// <returns>A string that represents the current object.</returns>
        String ToString() const override
        {
            return String("System.EventArgs");
        }

    protected:
        /// <summary>
        /// Copy constructor.
        /// </summary>
        EventArgs(const EventArgs& other) = default;

        /// <summary>
        /// Copy assignment operator.
        /// </summary>
        EventArgs& operator=(const EventArgs& other) = default;

        /// <summary>
        /// Move constructor.
        /// </summary>
        EventArgs(EventArgs&& other) noexcept = default;

        /// <summary>
        /// Move assignment operator.
        /// </summary>
        EventArgs& operator=(EventArgs&& other) noexcept = default;
    };

    /// <summary>
    /// Provides data for a cancelable event.
    /// </summary>
    class CancelEventArgs : public EventArgs
    {
    private:
        bool m_cancel;

    public:
        /// <summary>
        /// Initializes a new instance of the CancelEventArgs class with the Cancel property set to false.
        /// </summary>
        CancelEventArgs() : m_cancel(false) {}

        /// <summary>
        /// Initializes a new instance of the CancelEventArgs class with the Cancel property set to the given value.
        /// </summary>
        /// <param name="cancel">true to cancel the event; otherwise, false.</param>
        explicit CancelEventArgs(bool cancel) : m_cancel(cancel) {}

        /// <summary>
        /// Gets or sets a value indicating whether the event should be canceled.
        /// </summary>
        /// <returns>true if the event should be canceled; otherwise, false.</returns>
        bool GetCancel() const { return m_cancel; }

        /// <summary>
        /// Sets a value indicating whether the event should be canceled.
        /// </summary>
        /// <param name="value">true if the event should be canceled; otherwise, false.</param>
        void SetCancel(bool value) { m_cancel = value; }

        /// <summary>
        /// Returns a string that represents the current object.
        /// </summary>
        /// <returns>A string that represents the current object.</returns>
        String ToString() const override
        {
            return String("System.CancelEventArgs: Cancel=") + (m_cancel ? "true" : "false");
        }
    };

    /// <summary>
    /// Provides data for the PropertyChanged event.
    /// </summary>
    class PropertyChangedEventArgs : public EventArgs
    {
    private:
        String m_propertyName;

    public:
        /// <summary>
        /// Initializes a new instance of the PropertyChangedEventArgs class.
        /// </summary>
        /// <param name="propertyName">The name of the property that changed.</param>
        explicit PropertyChangedEventArgs(const String& propertyName) : m_propertyName(propertyName) {}

        /// <summary>
        /// Gets the name of the property that changed.
        /// </summary>
        /// <returns>The name of the property that changed.</returns>
        const String& GetPropertyName() const { return m_propertyName; }

        /// <summary>
        /// Returns a string that represents the current object.
        /// </summary>
        /// <returns>A string that represents the current object.</returns>
        String ToString() const override
        {
            return String("System.PropertyChangedEventArgs: PropertyName=") + m_propertyName;
        }
    };

    /// <summary>
    /// Provides data for collection change notifications.
    /// </summary>
    enum class NotifyCollectionChangedAction
    {
        Add,
        Remove,
        Replace,
        Move,
        Reset
    };

    /// <summary>
    /// Provides data for the CollectionChanged event.
    /// </summary>
    class CollectionChangedEventArgs : public EventArgs
    {
    private:
        NotifyCollectionChangedAction m_action;
        int m_newStartingIndex;
        int m_oldStartingIndex;

    public:
        /// <summary>
        /// Initializes a new instance of the CollectionChangedEventArgs class that describes a Reset change.
        /// </summary>
        /// <param name="action">The action that caused the event. This must be set to Reset.</param>
        explicit CollectionChangedEventArgs(NotifyCollectionChangedAction action)
            : m_action(action), m_newStartingIndex(-1), m_oldStartingIndex(-1) {}

        /// <summary>
        /// Initializes a new instance of the CollectionChangedEventArgs class that describes a one-item change.
        /// </summary>
        /// <param name="action">The action that caused the event. This can be set to Add or Remove.</param>
        /// <param name="index">The index where the change occurred.</param>
        CollectionChangedEventArgs(NotifyCollectionChangedAction action, int index)
            : m_action(action), m_newStartingIndex(index), m_oldStartingIndex(index) {}

        /// <summary>
        /// Initializes a new instance of the CollectionChangedEventArgs class that describes a multi-item change or a move change.
        /// </summary>
        /// <param name="action">The action that caused the event.</param>
        /// <param name="newIndex">The new index for the changed item(s).</param>
        /// <param name="oldIndex">The old index for the changed item(s).</param>
        CollectionChangedEventArgs(NotifyCollectionChangedAction action, int newIndex, int oldIndex)
            : m_action(action), m_newStartingIndex(newIndex), m_oldStartingIndex(oldIndex) {}

        /// <summary>
        /// Gets the action that caused the event.
        /// </summary>
        /// <returns>A NotifyCollectionChangedAction value that describes what action caused the event.</returns>
        NotifyCollectionChangedAction GetAction() const { return m_action; }

        /// <summary>
        /// Gets the index at which the change occurred.
        /// </summary>
        /// <returns>The zero-based index at which the change occurred.</returns>
        int GetNewStartingIndex() const { return m_newStartingIndex; }

        /// <summary>
        /// Gets the index at which a Move, Remove, or Replace action occurred.
        /// </summary>
        /// <returns>The zero-based index at which the action occurred.</returns>
        int GetOldStartingIndex() const { return m_oldStartingIndex; }

        /// <summary>
        /// Returns a string that represents the current object.
        /// </summary>
        /// <returns>A string that represents the current object.</returns>
        String ToString() const override
        {
            return String("System.CollectionChangedEventArgs: Action=") + std::to_string(static_cast<int>(m_action));
        }
    };

    /// <summary>
    /// Provides data for unhandled exception events.
    /// </summary>
    class UnhandledExceptionEventArgs : public EventArgs
    {
    private:
        std::exception_ptr m_exception;
        bool m_isTerminating;

    public:
        /// <summary>
        /// Initializes a new instance of the UnhandledExceptionEventArgs class.
        /// </summary>
        /// <param name="exception">The exception that is unhandled.</param>
        /// <param name="isTerminating">true if the runtime is terminating; otherwise, false.</param>
        UnhandledExceptionEventArgs(std::exception_ptr exception, bool isTerminating)
            : m_exception(exception), m_isTerminating(isTerminating) {}

        /// <summary>
        /// Gets the unhandled exception object.
        /// </summary>
        /// <returns>The unhandled exception object.</returns>
        std::exception_ptr GetExceptionObject() const { return m_exception; }

        /// <summary>
        /// Indicates whether the common language runtime is terminating.
        /// </summary>
        /// <returns>true if the runtime is terminating; otherwise, false.</returns>
        bool IsTerminating() const { return m_isTerminating; }

        /// <summary>
        /// Returns a string that represents the current object.
        /// </summary>
        /// <returns>A string that represents the current object.</returns>
        String ToString() const override
        {
            return String("System.UnhandledExceptionEventArgs: IsTerminating=") + (m_isTerminating ? "true" : "false");
        }
    };
}

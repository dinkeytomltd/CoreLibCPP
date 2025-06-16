
#pragma once

#include <atomic>
#include <memory>

namespace System
{
    class Object;

    namespace Memory
    {
        /// <summary>
        /// Control block for garbage collection reference counting.
        /// Manages strong and weak reference counts with thread-safe atomic operations.
        /// </summary>
        class GcControlBlock
        {
        private:
            std::atomic<long> m_strong_refs;
            std::atomic<long> m_weak_refs;
            Object* m_object;
            std::atomic<bool> m_object_destroyed;

        public:
            /// <summary>
            /// Constructor.
            /// </summary>
            explicit GcControlBlock(Object* obj) noexcept
                : m_strong_refs(0), m_weak_refs(0), m_object(obj), m_object_destroyed(false)
            {
            }

            /// <summary>
            /// Destructor.
            /// </summary>
            ~GcControlBlock() = default;

            // Non-copyable and non-movable
            GcControlBlock(const GcControlBlock&) = delete;
            GcControlBlock& operator=(const GcControlBlock&) = delete;
            GcControlBlock(GcControlBlock&&) = delete;
            GcControlBlock& operator=(GcControlBlock&&) = delete;

            /// <summary>
            /// Add strong reference.
            /// </summary>
            void AddStrongRef() noexcept
            {
                m_strong_refs.fetch_add(1, std::memory_order_relaxed);
            }

            /// <summary>
            /// Release strong reference.
            /// </summary>
            void ReleaseStrongRef() noexcept
            {
                if (m_strong_refs.fetch_sub(1, std::memory_order_acq_rel) == 1)
                {
                    // Last strong reference - destroy object
                    DestroyObject();
                    
                    // Check if we can destroy control block
                    if (m_weak_refs.load(std::memory_order_acquire) == 0)
                    {
                        delete this;
                    }
                }
            }

            /// <summary>
            /// Add weak reference.
            /// </summary>
            void AddWeakRef() noexcept
            {
                m_weak_refs.fetch_add(1, std::memory_order_relaxed);
            }

            /// <summary>
            /// Release weak reference.
            /// </summary>
            void ReleaseWeakRef() noexcept
            {
                if (m_weak_refs.fetch_sub(1, std::memory_order_acq_rel) == 1)
                {
                    // Last weak reference - check if we can destroy control block
                    if (m_strong_refs.load(std::memory_order_acquire) == 0)
                    {
                        delete this;
                    }
                }
            }

            /// <summary>
            /// Get strong reference count.
            /// </summary>
            long GetStrongRefCount() const noexcept
            {
                return m_strong_refs.load(std::memory_order_acquire);
            }

            /// <summary>
            /// Get weak reference count.
            /// </summary>
            long GetWeakRefCount() const noexcept
            {
                return m_weak_refs.load(std::memory_order_acquire);
            }

            /// <summary>
            /// Check if object has been destroyed.
            /// </summary>
            bool IsExpired() const noexcept
            {
                return m_object_destroyed.load(std::memory_order_acquire);
            }

            /// <summary>
            /// Get the managed object (may be null if destroyed).
            /// </summary>
            Object* GetObject() const noexcept
            {
                return IsExpired() ? nullptr : m_object;
            }

        private:
            /// <summary>
            /// Destroy the managed object.
            /// </summary>
            void DestroyObject() noexcept;
        };
    }
}

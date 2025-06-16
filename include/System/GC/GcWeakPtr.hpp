
#pragma once

#include "System/GC/GcControlBlock.hpp"
#include <memory>

namespace System
{
    namespace Memory
    {
        template<typename T>
        class GcPtr;

        /// <summary>
        /// Weak pointer that doesn't affect object lifetime.
        /// Used to break circular references and observe object lifetime.
        /// </summary>
        template<typename T>
        class GcWeakPtr
        {
            static_assert(std::is_base_of_v<Object, T>, "GcWeakPtr can only manage types derived from System::Object");

        private:
            T* m_ptr;
            GcControlBlock* m_control_block;

            void AddWeakRef()
            {
                if (m_control_block)
                {
                    m_control_block->AddWeakRef();
                }
            }

            void ReleaseWeakRef()
            {
                if (m_control_block)
                {
                    m_control_block->ReleaseWeakRef();
                    m_control_block = nullptr;
                }
                m_ptr = nullptr;
            }

        public:
            using element_type = T;

            /// <summary>
            /// Default constructor - creates null weak pointer.
            /// </summary>
            GcWeakPtr() noexcept : m_ptr(nullptr), m_control_block(nullptr) {}

            /// <summary>
            /// Constructor from GcPtr.
            /// </summary>
            GcWeakPtr(const GcPtr<T>& ptr) noexcept 
                : m_ptr(ptr.Get()), m_control_block(ptr.GetControlBlock())
            {
                AddWeakRef();
            }

            /// <summary>
            /// Copy constructor.
            /// </summary>
            GcWeakPtr(const GcWeakPtr& other) noexcept 
                : m_ptr(other.m_ptr), m_control_block(other.m_control_block)
            {
                AddWeakRef();
            }

            /// <summary>
            /// Move constructor.
            /// </summary>
            GcWeakPtr(GcWeakPtr&& other) noexcept 
                : m_ptr(other.m_ptr), m_control_block(other.m_control_block)
            {
                other.m_ptr = nullptr;
                other.m_control_block = nullptr;
            }

            /// <summary>
            /// Constructor from compatible type.
            /// </summary>
            template<typename U>
            GcWeakPtr(const GcWeakPtr<U>& other) noexcept 
                : m_ptr(static_cast<T*>(other.m_ptr)), m_control_block(other.m_control_block)
            {
                static_assert(std::is_convertible_v<U*, T*>, "Cannot convert between incompatible pointer types");
                AddWeakRef();
            }

            /// <summary>
            /// Constructor from compatible GcPtr.
            /// </summary>
            template<typename U>
            GcWeakPtr(const GcPtr<U>& ptr) noexcept 
                : m_ptr(static_cast<T*>(ptr.Get())), m_control_block(ptr.GetControlBlock())
            {
                static_assert(std::is_convertible_v<U*, T*>, "Cannot convert between incompatible pointer types");
                AddWeakRef();
            }

            /// <summary>
            /// Destructor.
            /// </summary>
            ~GcWeakPtr()
            {
                ReleaseWeakRef();
            }

            /// <summary>
            /// Copy assignment operator.
            /// </summary>
            GcWeakPtr& operator=(const GcWeakPtr& other) noexcept
            {
                if (this != &other)
                {
                    ReleaseWeakRef();
                    m_ptr = other.m_ptr;
                    m_control_block = other.m_control_block;
                    AddWeakRef();
                }
                return *this;
            }

            /// <summary>
            /// Move assignment operator.
            /// </summary>
            GcWeakPtr& operator=(GcWeakPtr&& other) noexcept
            {
                if (this != &other)
                {
                    ReleaseWeakRef();
                    m_ptr = other.m_ptr;
                    m_control_block = other.m_control_block;
                    other.m_ptr = nullptr;
                    other.m_control_block = nullptr;
                }
                return *this;
            }

            /// <summary>
            /// Assignment from GcPtr.
            /// </summary>
            GcWeakPtr& operator=(const GcPtr<T>& ptr) noexcept
            {
                ReleaseWeakRef();
                m_ptr = ptr.Get();
                m_control_block = ptr.GetControlBlock();
                AddWeakRef();
                return *this;
            }

            /// <summary>
            /// Assignment from compatible type.
            /// </summary>
            template<typename U>
            GcWeakPtr& operator=(const GcWeakPtr<U>& other) noexcept
            {
                static_assert(std::is_convertible_v<U*, T*>, "Cannot convert between incompatible pointer types");
                ReleaseWeakRef();
                m_ptr = static_cast<T*>(other.m_ptr);
                m_control_block = other.m_control_block;
                AddWeakRef();
                return *this;
            }

            /// <summary>
            /// Lock weak pointer to get strong reference.
            /// Returns null GcPtr if object has been destroyed.
            /// </summary>
            GcPtr<T> Lock() const noexcept
            {
                if (m_control_block && !m_control_block->IsExpired())
                {
                    return GcPtr<T>(m_ptr, m_control_block);
                }
                return GcPtr<T>();
            }

            /// <summary>
            /// Check if the referenced object has been destroyed.
            /// </summary>
            bool Expired() const noexcept
            {
                return !m_control_block || m_control_block->IsExpired();
            }

            /// <summary>
            /// Get the number of strong references to the object.
            /// Returns 0 if object has been destroyed.
            /// </summary>
            long UseCount() const noexcept
            {
                return m_control_block ? m_control_block->GetStrongRefCount() : 0;
            }

            /// <summary>
            /// Reset to null.
            /// </summary>
            void Reset() noexcept
            {
                ReleaseWeakRef();
            }

            /// <summary>
            /// Check if weak pointer is null.
            /// </summary>
            bool IsNull() const noexcept
            {
                return m_control_block == nullptr;
            }

            /// <summary>
            /// Equality comparison.
            /// </summary>
            bool operator==(const GcWeakPtr& other) const noexcept
            {
                return m_control_block == other.m_control_block;
            }

            /// <summary>
            /// Inequality comparison.
            /// </summary>
            bool operator!=(const GcWeakPtr& other) const noexcept
            {
                return m_control_block != other.m_control_block;
            }

            /// <summary>
            /// Less than comparison (for containers).
            /// </summary>
            bool operator<(const GcWeakPtr& other) const noexcept
            {
                return m_control_block < other.m_control_block;
            }

        private:
            // Private constructor for internal use
            GcWeakPtr(T* ptr, GcControlBlock* control_block) noexcept 
                : m_ptr(ptr), m_control_block(control_block)
            {
                AddWeakRef();
            }

            template<typename U> friend class GcPtr;
            template<typename U> friend class GcWeakPtr;
            template<typename U> friend class GcRef;
        };
    }
}

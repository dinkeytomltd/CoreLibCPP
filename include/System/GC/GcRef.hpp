
#pragma once

#include "System/GC/GcPtr.hpp"

namespace System
{
    namespace Memory
    {
        /// <summary>
        /// Stack reference wrapper for temporary objects and function parameters.
        /// Provides automatic conversion to/from GcPtr while maintaining stack semantics.
        /// </summary>
        template<typename T>
        class GcRef
        {
            static_assert(std::is_base_of_v<Object, T>, "GcRef can only manage types derived from System::Object");

        private:
            T* m_ptr;

        public:
            using element_type = T;

            /// <summary>
            /// Constructor from GcPtr.
            /// </summary>
            GcRef(const GcPtr<T>& ptr) noexcept : m_ptr(ptr.Get()) {}

            /// <summary>
            /// Constructor from raw pointer.
            /// </summary>
            explicit GcRef(T* ptr) noexcept : m_ptr(ptr) {}

            /// <summary>
            /// Constructor from reference.
            /// </summary>
            GcRef(T& ref) noexcept : m_ptr(&ref) {}

            /// <summary>
            /// Copy constructor.
            /// </summary>
            GcRef(const GcRef& other) noexcept : m_ptr(other.m_ptr) {}

            /// <summary>
            /// Constructor from compatible type.
            /// </summary>
            template<typename U>
            GcRef(const GcRef<U>& other) noexcept : m_ptr(static_cast<T*>(other.Get()))
            {
                static_assert(std::is_convertible_v<U*, T*>, "Cannot convert between incompatible pointer types");
            }

            /// <summary>
            /// Assignment operator.
            /// </summary>
            GcRef& operator=(const GcRef& other) noexcept
            {
                m_ptr = other.m_ptr;
                return *this;
            }

            /// <summary>
            /// Assignment from GcPtr.
            /// </summary>
            GcRef& operator=(const GcPtr<T>& ptr) noexcept
            {
                m_ptr = ptr.Get();
                return *this;
            }

            /// <summary>
            /// Dereference operator.
            /// </summary>
            T& operator*() const noexcept
            {
                return *m_ptr;
            }

            /// <summary>
            /// Arrow operator.
            /// </summary>
            T* operator->() const noexcept
            {
                return m_ptr;
            }

            /// <summary>
            /// Get raw pointer.
            /// </summary>
            T* Get() const noexcept
            {
                return m_ptr;
            }

            /// <summary>
            /// Conversion to GcPtr.
            /// </summary>
            operator GcPtr<T>() const
            {
                return GcPtr<T>(m_ptr);
            }

            /// <summary>
            /// Conversion to reference.
            /// </summary>
            operator T&() const noexcept
            {
                return *m_ptr;
            }

            /// <summary>
            /// Boolean conversion operator.
            /// </summary>
            explicit operator bool() const noexcept
            {
                return m_ptr != nullptr;
            }

            /// <summary>
            /// Equality comparison.
            /// </summary>
            bool operator==(const GcRef& other) const noexcept
            {
                return m_ptr == other.m_ptr;
            }

            /// <summary>
            /// Inequality comparison.
            /// </summary>
            bool operator!=(const GcRef& other) const noexcept
            {
                return m_ptr != other.m_ptr;
            }

            /// <summary>
            /// Equality comparison with GcPtr.
            /// </summary>
            bool operator==(const GcPtr<T>& ptr) const noexcept
            {
                return m_ptr == ptr.Get();
            }

            /// <summary>
            /// Inequality comparison with GcPtr.
            /// </summary>
            bool operator!=(const GcPtr<T>& ptr) const noexcept
            {
                return m_ptr != ptr.Get();
            }

            /// <summary>
            /// Create weak reference.
            /// </summary>
            GcWeakPtr<T> GetWeakPtr() const
            {
                return GcPtr<T>(m_ptr).GetWeakPtr();
            }

        private:
            template<typename U> friend class GcRef;
        };

        /// <summary>
        /// Helper function to create GcRef from reference.
        /// </summary>
        template<typename T>
        GcRef<T> MakeRef(T& obj)
        {
            return GcRef<T>(obj);
        }

        /// <summary>
        /// Helper function to create GcRef from pointer.
        /// </summary>
        template<typename T>
        GcRef<T> MakeRef(T* ptr)
        {
            return GcRef<T>(ptr);
        }
    }
}

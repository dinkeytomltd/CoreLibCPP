
#pragma once

#include "System/Object.h"
#include "System/GC/GcControlBlock.hpp"
#include <atomic>
#include <memory>
#include <type_traits>

namespace System
{
    namespace Memory
    {
        template<typename T>
        class GcWeakPtr;

        template<typename T>
        class GcRef;

        /// <summary>
        /// Smart pointer with automatic garbage collection using reference counting and RAII.
        /// Provides C#-like reference semantics with automatic memory management.
        /// </summary>
        template<typename T>
        class GcPtr
        {
            static_assert(std::is_base_of_v<Object, T>, "GcPtr can only manage types derived from System::Object");

        private:
            T* m_ptr;
            GcControlBlock* m_control_block;

            void AddRef()
            {
                if (m_control_block)
                {
                    m_control_block->AddStrongRef();
                }
            }

            void Release()
            {
                if (m_control_block)
                {
                    m_control_block->ReleaseStrongRef();
                    m_control_block = nullptr;
                }
                m_ptr = nullptr;
            }

        public:
            using element_type = T;

            /// <summary>
            /// Default constructor - creates null pointer.
            /// </summary>
            GcPtr() noexcept : m_ptr(nullptr), m_control_block(nullptr) {}

            /// <summary>
            /// Constructor from raw pointer - takes ownership.
            /// </summary>
            explicit GcPtr(T* ptr) : m_ptr(ptr), m_control_block(nullptr)
            {
                if (m_ptr)
                {
                    m_control_block = m_ptr->GetControlBlock();
                    if (!m_control_block)
                    {
                        m_control_block = new GcControlBlock(m_ptr);
                        m_ptr->SetControlBlock(m_control_block);
                    }
                    AddRef();
                }
            }

            /// <summary>
            /// Copy constructor.
            /// </summary>
            GcPtr(const GcPtr& other) noexcept : m_ptr(other.m_ptr), m_control_block(other.m_control_block)
            {
                AddRef();
            }

            /// <summary>
            /// Move constructor.
            /// </summary>
            GcPtr(GcPtr&& other) noexcept : m_ptr(other.m_ptr), m_control_block(other.m_control_block)
            {
                other.m_ptr = nullptr;
                other.m_control_block = nullptr;
            }

            /// <summary>
            /// Copy constructor from compatible type.
            /// </summary>
            template<typename U>
            GcPtr(const GcPtr<U>& other) noexcept 
                : m_ptr(static_cast<T*>(other.Get())), m_control_block(other.GetControlBlock())
            {
                static_assert(std::is_convertible_v<U*, T*>, "Cannot convert between incompatible pointer types");
                AddRef();
            }

            /// <summary>
            /// Constructor from weak pointer.
            /// </summary>
            explicit GcPtr(const GcWeakPtr<T>& weak) : m_ptr(nullptr), m_control_block(nullptr)
            {
                auto locked = weak.Lock();
                if (locked)
                {
                    m_ptr = locked.m_ptr;
                    m_control_block = locked.m_control_block;
                    AddRef();
                }
            }

            /// <summary>
            /// Destructor.
            /// </summary>
            ~GcPtr()
            {
                Release();
            }

            /// <summary>
            /// Copy assignment operator.
            /// </summary>
            GcPtr& operator=(const GcPtr& other) noexcept
            {
                if (this != &other)
                {
                    Release();
                    m_ptr = other.m_ptr;
                    m_control_block = other.m_control_block;
                    AddRef();
                }
                return *this;
            }

            /// <summary>
            /// Move assignment operator.
            /// </summary>
            GcPtr& operator=(GcPtr&& other) noexcept
            {
                if (this != &other)
                {
                    Release();
                    m_ptr = other.m_ptr;
                    m_control_block = other.m_control_block;
                    other.m_ptr = nullptr;
                    other.m_control_block = nullptr;
                }
                return *this;
            }

            /// <summary>
            /// Assignment from compatible type.
            /// </summary>
            template<typename U>
            GcPtr& operator=(const GcPtr<U>& other) noexcept
            {
                static_assert(std::is_convertible_v<U*, T*>, "Cannot convert between incompatible pointer types");
                Release();
                m_ptr = static_cast<T*>(other.Get());
                m_control_block = other.GetControlBlock();
                AddRef();
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
            /// Get control block (for internal use).
            /// </summary>
            GcControlBlock* GetControlBlock() const noexcept
            {
                return m_control_block;
            }

            /// <summary>
            /// Check if pointer is null.
            /// </summary>
            bool IsNull() const noexcept
            {
                return m_ptr == nullptr;
            }

            /// <summary>
            /// Reset to null.
            /// </summary>
            void Reset() noexcept
            {
                Release();
            }

            /// <summary>
            /// Reset with new pointer.
            /// </summary>
            void Reset(T* ptr)
            {
                *this = GcPtr(ptr);
            }

            /// <summary>
            /// Get reference count.
            /// </summary>
            long UseCount() const noexcept
            {
                return m_control_block ? m_control_block->GetStrongRefCount() : 0;
            }

            /// <summary>
            /// Check if this is the unique owner.
            /// </summary>
            bool Unique() const noexcept
            {
                return UseCount() == 1;
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
            bool operator==(const GcPtr& other) const noexcept
            {
                return m_ptr == other.m_ptr;
            }

            /// <summary>
            /// Inequality comparison.
            /// </summary>
            bool operator!=(const GcPtr& other) const noexcept
            {
                return m_ptr != other.m_ptr;
            }

            /// <summary>
            /// Less than comparison (for containers).
            /// </summary>
            bool operator<(const GcPtr& other) const noexcept
            {
                return m_ptr < other.m_ptr;
            }

            /// <summary>
            /// Create weak reference.
            /// </summary>
            GcWeakPtr<T> GetWeakPtr() const
            {
                return GcWeakPtr<T>(*this);
            }

            /// <summary>
            /// Static cast to derived type.
            /// </summary>
            template<typename U>
            GcPtr<U> StaticCast() const
            {
                static_assert(std::is_base_of_v<T, U> || std::is_base_of_v<U, T>, 
                    "Static cast requires inheritance relationship");
                return GcPtr<U>(static_cast<U*>(m_ptr), m_control_block);
            }

            /// <summary>
            /// Dynamic cast to derived type.
            /// </summary>
            template<typename U>
            GcPtr<U> DynamicCast() const
            {
                U* casted = dynamic_cast<U*>(m_ptr);
                return casted ? GcPtr<U>(casted, m_control_block) : GcPtr<U>();
            }

        private:
            // Private constructor for cast operations
            GcPtr(T* ptr, GcControlBlock* control_block) noexcept 
                : m_ptr(ptr), m_control_block(control_block)
            {
                AddRef();
            }

            template<typename U> friend class GcPtr;
            template<typename U> friend class GcWeakPtr;
            template<typename U> friend class GcRef;
        };

        /// <summary>
        /// Create GcPtr from raw pointer.
        /// </summary>
        template<typename T, typename... Args>
        GcPtr<T> MakeGc(Args&&... args)
        {
            return GcPtr<T>(new T(std::forward<Args>(args)...));
        }

        /// <summary>
        /// Static pointer cast.
        /// </summary>
        template<typename T, typename U>
        GcPtr<T> StaticPointerCast(const GcPtr<U>& ptr)
        {
            return ptr.template StaticCast<T>();
        }

        /// <summary>
        /// Dynamic pointer cast.
        /// </summary>
        template<typename T, typename U>
        GcPtr<T> DynamicPointerCast(const GcPtr<U>& ptr)
        {
            return ptr.template DynamicCast<T>();
        }
    }
}

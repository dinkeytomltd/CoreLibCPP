
#pragma once

#include "System/String.h"
#include <memory>
#include <typeinfo>
#include <atomic>

namespace System
{
    namespace Reflection
    {
        class Type; // Forward declaration
    }

    namespace Memory
    {
        class GcControlBlock; // Forward declaration
    }

    /// <summary>
    /// Supports all classes in the .NET class hierarchy and provides low-level services to derived classes.
    /// This is the ultimate base class of all .NET classes; it is the root of the type hierarchy.
    /// Enhanced with garbage collection support through reference counting and RAII.
    /// </summary>
    class Object
    {
    private:
        mutable Memory::GcControlBlock* m_gc_control_block;
        mutable std::atomic<bool> m_finalization_suppressed;

    public:
        /// <summary>
        /// Initializes a new instance of the Object class.
        /// </summary>
        Object() : m_gc_control_block(nullptr), m_finalization_suppressed(false) {}

        /// <summary>
        /// Virtual destructor to ensure proper cleanup of derived classes.
        /// </summary>
        virtual ~Object() = default;

        /// <summary>
        /// Determines whether the specified object is equal to the current object.
        /// </summary>
        /// <param name="obj">The object to compare with the current object.</param>
        /// <returns>true if the specified object is equal to the current object; otherwise, false.</returns>
        virtual bool Equals(const Object& obj) const
        {
            return this == &obj;
        }

        /// <summary>
        /// Serves as the default hash function.
        /// </summary>
        /// <returns>A hash code for the current object.</returns>
        virtual int GetHashCode() const
        {
            return static_cast<int>(reinterpret_cast<std::uintptr_t>(this));
        }

        /// <summary>
        /// Returns a string that represents the current object.
        /// </summary>
        /// <returns>A string that represents the current object.</returns>
        virtual String ToString() const
        {
            return String(typeid(*this).name());
        }

        /// <summary>
        /// Gets the Type of the current instance.
        /// Note: This method is only available when CoreLib.Reflection is linked.
        /// </summary>
        /// <returns>The exact runtime type of the current instance.</returns>
        #ifdef CORELIB_REFLECTION_AVAILABLE
        virtual class Reflection::Type GetType() const;
        #endif

        /// <summary>
        /// Determines whether the specified Object instances are considered equal.
        /// </summary>
        /// <param name="objA">The first object to compare.</param>
        /// <param name="objB">The second object to compare.</param>
        /// <returns>true if the objects are considered equal; otherwise, false.</returns>
        static bool Equals(const Object& objA, const Object& objB)
        {
            return objA.Equals(objB);
        }

        /// <summary>
        /// Determines whether the specified object instances are the same instance.
        /// </summary>
        /// <param name="objA">The first object to compare.</param>
        /// <param name="objB">The second object to compare.</param>
        /// <returns>true if objA is the same instance as objB or if both are null; otherwise, false.</returns>
        static bool ReferenceEquals(const Object* objA, const Object* objB)
        {
            return objA == objB;
        }

        /// <summary>
        /// Copy constructor.
        /// </summary>
        Object(const Object& other) = default;

        /// <summary>
        /// Copy assignment operator.
        /// </summary>
        Object& operator=(const Object& other) = default;

        /// <summary>
        /// Move constructor.
        /// </summary>
        Object(Object&& other) noexcept = default;

        /// <summary>
        /// Move assignment operator.
        /// </summary>
        Object& operator=(Object&& other) noexcept = default;

        /// <summary>
        /// Get the garbage collection control block for this object.
        /// </summary>
        /// <returns>Pointer to the control block, or nullptr if not yet created.</returns>
        Memory::GcControlBlock* GetControlBlock() const noexcept
        {
            return m_gc_control_block;
        }

        /// <summary>
        /// Set the garbage collection control block for this object.
        /// </summary>
        /// <param name="controlBlock">The control block to associate with this object.</param>
        void SetControlBlock(Memory::GcControlBlock* controlBlock) const noexcept
        {
            m_gc_control_block = controlBlock;
        }

        /// <summary>
        /// Check if finalization has been suppressed for this object.
        /// </summary>
        /// <returns>True if finalization is suppressed.</returns>
        bool IsFinalizationSuppressed() const noexcept
        {
            return m_finalization_suppressed.load(std::memory_order_acquire);
        }

        /// <summary>
        /// Set finalization suppression status for this object.
        /// </summary>
        /// <param name="suppressed">Whether finalization should be suppressed.</param>
        void SetFinalizationSuppressed(bool suppressed) const noexcept
        {
            m_finalization_suppressed.store(suppressed, std::memory_order_release);
        }

        /// <summary>
        /// Virtual finalizer method called during garbage collection.
        /// Override in derived classes to provide cleanup logic.
        /// </summary>
        virtual void Finalize() {}

    protected:
        /// <summary>
        /// Called when the object is being destroyed by the garbage collector.
        /// This is called before the destructor and allows for cleanup of managed resources.
        /// </summary>
        virtual void OnGcDestroy() {}

        friend class Memory::GcControlBlock;
    };
}


#pragma once

#include "System/Object.h"
#include "System/String.h"
#include "System/Exception.h"
#include "System/ArgumentNullException.h"
#include "System/InvalidOperationException.h"
#include <functional>
#include <memory>
#include <type_traits>
#include <utility>

namespace System
{
    /// <summary>
    /// Represents a delegate, which is a data structure that refers to a static method 
    /// or to a class instance and an instance method of that class.
    /// </summary>
    class Delegate : public Object
    {
    public:
        /// <summary>
        /// Initializes a new instance of the Delegate class.
        /// </summary>
        Delegate() = default;

        /// <summary>
        /// Virtual destructor to ensure proper cleanup of derived classes.
        /// </summary>
        virtual ~Delegate() = default;

        /// <summary>
        /// Gets a value indicating whether the delegate is null.
        /// </summary>
        /// <returns>true if the delegate is null; otherwise, false.</returns>
        virtual bool IsNull() const = 0;

        /// <summary>
        /// Creates a shallow copy of the delegate.
        /// </summary>
        /// <returns>A shallow copy of the delegate.</returns>
        virtual std::unique_ptr<Delegate> Clone() const = 0;

        /// <summary>
        /// Gets the method represented by the delegate.
        /// </summary>
        /// <returns>A string representation of the method.</returns>
        virtual String GetMethodInfo() const = 0;

        /// <summary>
        /// Determines whether the specified object is equal to the current delegate.
        /// </summary>
        /// <param name="obj">The object to compare with the current delegate.</param>
        /// <returns>true if the specified object is equal to the current delegate; otherwise, false.</returns>
        bool Equals(const Object& obj) const override
        {
            const Delegate* other = dynamic_cast<const Delegate*>(&obj);
            return other != nullptr && this->Equals(*other);
        }

        /// <summary>
        /// Determines whether the specified delegate is equal to the current delegate.
        /// </summary>
        /// <param name="other">The delegate to compare with the current delegate.</param>
        /// <returns>true if the specified delegate is equal to the current delegate; otherwise, false.</returns>
        virtual bool Equals(const Delegate& other) const = 0;

        /// <summary>
        /// Returns a string that represents the current delegate.
        /// </summary>
        /// <returns>A string that represents the current delegate.</returns>
        String ToString() const override
        {
            return GetMethodInfo();
        }

    protected:
        /// <summary>
        /// Copy constructor.
        /// </summary>
        Delegate(const Delegate& other) = default;

        /// <summary>
        /// Copy assignment operator.
        /// </summary>
        Delegate& operator=(const Delegate& other) = default;

        /// <summary>
        /// Move constructor.
        /// </summary>
        Delegate(Delegate&& other) noexcept = default;

        /// <summary>
        /// Move assignment operator.
        /// </summary>
        Delegate& operator=(Delegate&& other) noexcept = default;
    };

    /// <summary>
    /// Represents a typed delegate that can invoke methods with a specific signature.
    /// </summary>
    /// <typeparam name="TSignature">The signature of the method (e.g., void(int, string)).</typeparam>
    template<typename TSignature>
    class TypedDelegate;

    /// <summary>
    /// Specialization of TypedDelegate for function signatures.
    /// </summary>
    /// <typeparam name="TReturn">The return type of the method.</typeparam>
    /// <typeparam name="TArgs">The parameter types of the method.</typeparam>
    template<typename TReturn, typename... TArgs>
    class TypedDelegate<TReturn(TArgs...)> : public Delegate
    {
    public:
        using ReturnType = TReturn;
        using FunctionType = std::function<TReturn(TArgs...)>;

        /// <summary>
        /// Initializes a new instance of the TypedDelegate class with a null target.
        /// </summary>
        TypedDelegate() : m_function(nullptr), m_methodInfo("null") {}

        /// <summary>
        /// Initializes a new instance of the TypedDelegate class with a static method.
        /// </summary>
        /// <param name="staticMethod">A pointer to the static method.</param>
        template<TReturn(*StaticMethod)(TArgs...)>
        static TypedDelegate<TReturn(TArgs...)> Create()
        {
            static_assert(StaticMethod != nullptr, "Static method cannot be null");
            TypedDelegate<TReturn(TArgs...)> del;
            del.m_function = [](TArgs... args) -> TReturn {
                return StaticMethod(args...);
            };
            del.m_methodInfo = String("StaticMethod");
            return del;
        }

        /// <summary>
        /// Initializes a new instance of the TypedDelegate class with an instance method.
        /// </summary>
        /// <typeparam name="TClass">The type of the class containing the method.</typeparam>
        /// <param name="instance">A shared pointer to the instance.</param>
        /// <param name="method">A pointer to the instance method.</param>
        template<typename TClass>
        static TypedDelegate<TReturn(TArgs...)> Create(std::shared_ptr<TClass> instance, TReturn(TClass::*method)(TArgs...))
        {
            if (!instance)
            {
                throw ArgumentNullException("instance");
            }
            if (!method)
            {
                throw ArgumentNullException("method");
            }

            TypedDelegate<TReturn(TArgs...)> del;
            del.m_function = [instance, method](TArgs... args) -> TReturn {
                return (instance.get()->*method)(args...);
            };
            del.m_methodInfo = String("InstanceMethod");
            del.m_instance = instance;
            return del;
        }

        /// <summary>
        /// Initializes a new instance of the Delegate class with an instance method (raw pointer).
        /// </summary>
        /// <typeparam name="TClass">The type of the class containing the method.</typeparam>
        /// <param name="instance">A pointer to the instance.</param>
        /// <param name="method">A pointer to the instance method.</param>
        template<typename TClass>
        static TypedDelegate<TReturn(TArgs...)> Create(TClass* instance, TReturn(TClass::*method)(TArgs...))
        {
            if (!instance)
            {
                throw ArgumentNullException("instance");
            }
            if (!method)
            {
                throw ArgumentNullException("method");
            }

            TypedDelegate<TReturn(TArgs...)> del;
            del.m_function = [instance, method](TArgs... args) -> TReturn {
                return (instance->*method)(args...);
            };
            del.m_methodInfo = String("InstanceMethod");
            return del;
        }

        /// <summary>
        /// Initializes a new instance of the Delegate class with a lambda or functor.
        /// </summary>
        /// <typeparam name="TCallable">The type of the callable object.</typeparam>
        /// <param name="callable">The callable object.</param>
        template<typename TCallable>
        static TypedDelegate<TReturn(TArgs...)> Create(TCallable&& callable)
        {
            static_assert(std::is_invocable_r_v<TReturn, TCallable, TArgs...>, 
                         "Callable must be invocable with the specified signature");
            
            TypedDelegate<TReturn(TArgs...)> del;
            del.m_function = std::forward<TCallable>(callable);
            del.m_methodInfo = String("Lambda/Functor");
            return del;
        }

        /// <summary>
        /// Constructor from static function pointer.
        /// </summary>
        /// <param name="staticMethod">A pointer to the static method.</param>
        TypedDelegate(TReturn(*staticMethod)(TArgs...))
        {
            if (!staticMethod)
            {
                throw ArgumentNullException("staticMethod");
            }
            m_function = staticMethod;
            m_methodInfo = String("StaticMethod");
        }

        /// <summary>
        /// Constructor from std::function.
        /// </summary>
        /// <param name="function">The std::function to wrap.</param>
        TypedDelegate(const FunctionType& function) : m_function(function), m_methodInfo("Function") {}

        /// <summary>
        /// Constructor from std::function (move).
        /// </summary>
        /// <param name="function">The std::function to wrap.</param>
        TypedDelegate(FunctionType&& function) : m_function(std::move(function)), m_methodInfo("Function") {}

        /// <summary>
        /// Copy constructor.
        /// </summary>
        TypedDelegate(const TypedDelegate& other) = default;

        /// <summary>
        /// Move constructor.
        /// </summary>
        TypedDelegate(TypedDelegate&& other) noexcept = default;

        /// <summary>
        /// Copy assignment operator.
        /// </summary>
        TypedDelegate& operator=(const TypedDelegate& other) = default;

        /// <summary>
        /// Move assignment operator.
        /// </summary>
        TypedDelegate& operator=(TypedDelegate&& other) noexcept = default;

        /// <summary>
        /// Assignment from static function pointer.
        /// </summary>
        /// <param name="staticMethod">A pointer to the static method.</param>
        TypedDelegate& operator=(TReturn(*staticMethod)(TArgs...))
        {
            if (!staticMethod)
            {
                throw ArgumentNullException("staticMethod");
            }
            m_function = staticMethod;
            m_methodInfo = String("StaticMethod");
            m_instance.reset();
            return *this;
        }

        /// <summary>
        /// Assignment from std::function.
        /// </summary>
        /// <param name="function">The std::function to wrap.</param>
        TypedDelegate& operator=(const FunctionType& function)
        {
            m_function = function;
            m_methodInfo = String("Function");
            m_instance.reset();
            return *this;
        }

        /// <summary>
        /// Assignment from std::function (move).
        /// </summary>
        /// <param name="function">The std::function to wrap.</param>
        TypedDelegate& operator=(FunctionType&& function)
        {
            m_function = std::move(function);
            m_methodInfo = String("Function");
            m_instance.reset();
            return *this;
        }

        /// <summary>
        /// Invokes the delegate with the specified arguments.
        /// </summary>
        /// <param name="args">The arguments to pass to the method.</param>
        /// <returns>The result of the method invocation.</returns>
        TReturn Invoke(TArgs... args) const
        {
            if (!m_function)
            {
                throw InvalidOperationException("Cannot invoke null delegate");
            }
            return m_function(args...);
        }

        /// <summary>
        /// Function call operator for convenient invocation.
        /// </summary>
        /// <param name="args">The arguments to pass to the method.</param>
        /// <returns>The result of the method invocation.</returns>
        TReturn operator()(TArgs... args) const
        {
            return Invoke(args...);
        }

        /// <summary>
        /// Gets a value indicating whether the delegate is null.
        /// </summary>
        /// <returns>true if the delegate is null; otherwise, false.</returns>
        bool IsNull() const override
        {
            return !m_function;
        }

        /// <summary>
        /// Creates a shallow copy of the delegate.
        /// </summary>
        /// <returns>A shallow copy of the delegate.</returns>
        std::unique_ptr<System::Delegate> Clone() const override
        {
            return std::make_unique<TypedDelegate<TReturn(TArgs...)>>(*this);
        }

        /// <summary>
        /// Gets the method represented by the delegate.
        /// </summary>
        /// <returns>A string representation of the method.</returns>
        String GetMethodInfo() const override
        {
            return m_methodInfo;
        }

        /// <summary>
        /// Determines whether the specified delegate is equal to the current delegate.
        /// </summary>
        /// <param name="other">The delegate to compare with the current delegate.</param>
        /// <returns>true if the specified delegate is equal to the current delegate; otherwise, false.</returns>
        bool Equals(const System::Delegate& other) const override
        {
            const TypedDelegate<TReturn(TArgs...)>* typedOther = dynamic_cast<const TypedDelegate<TReturn(TArgs...)>*>(&other);
            if (!typedOther)
                return false;

            // For function objects, we can only compare if both are null or both are non-null
            // Actual function comparison is not reliable in C++
            return IsNull() == typedOther->IsNull();
        }

        /// <summary>
        /// Conversion to bool for null checking.
        /// </summary>
        explicit operator bool() const
        {
            return !IsNull();
        }

        /// <summary>
        /// Equality operator.
        /// </summary>
        bool operator==(const TypedDelegate& other) const
        {
            return Equals(other);
        }

        /// <summary>
        /// Inequality operator.
        /// </summary>
        bool operator!=(const TypedDelegate& other) const
        {
            return !Equals(other);
        }

    private:
        FunctionType m_function;
        String m_methodInfo;
        std::shared_ptr<void> m_instance; // Keep instance alive for member function delegates
    };

    // Common delegate type aliases for convenience
    using Action = TypedDelegate<void()>;
    template<typename T> using Action1 = TypedDelegate<void(T)>;
    template<typename T1, typename T2> using Action2 = TypedDelegate<void(T1, T2)>;
    template<typename T1, typename T2, typename T3> using Action3 = TypedDelegate<void(T1, T2, T3)>;

    template<typename TResult> using Func = TypedDelegate<TResult()>;
    template<typename T, typename TResult> using Func1 = TypedDelegate<TResult(T)>;
    template<typename T1, typename T2, typename TResult> using Func2 = TypedDelegate<TResult(T1, T2)>;
    template<typename T1, typename T2, typename T3, typename TResult> using Func3 = TypedDelegate<TResult(T1, T2, T3)>;

    // Note: Use TypedDelegate<TSignature> for C# style delegate syntax
}

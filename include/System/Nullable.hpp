
#pragma once

#include "System/Object.h"
#include "System/String.h"
#include "System/Exception.h"
#include "System/Reflection/Type.h"
#include <optional>
#include <type_traits>

namespace System
{
    /// <summary>
    /// Represents a value type that can be assigned null.
    /// </summary>
    template<typename T>
    class Nullable : public Object
    {
        static_assert(std::is_arithmetic_v<T> || std::is_enum_v<T>, 
                     "Nullable can only be used with value types");

    private:
        std::optional<T> _value;

    public:
        /// <summary>
        /// Initializes a new instance of the Nullable class to null.
        /// </summary>
        Nullable() : _value(std::nullopt) {}

        /// <summary>
        /// Initializes a new instance of the Nullable class to the specified value.
        /// </summary>
        /// <param name="value">A value type.</param>
        Nullable(const T& value) : _value(value) {}

        /// <summary>
        /// Initializes a new instance of the Nullable class to null.
        /// </summary>
        Nullable(std::nullopt_t) : _value(std::nullopt) {}

        /// <summary>
        /// Copy constructor.
        /// </summary>
        Nullable(const Nullable<T>& other) : _value(other._value) {}

        /// <summary>
        /// Move constructor.
        /// </summary>
        Nullable(Nullable<T>&& other) noexcept : _value(std::move(other._value)) {}

        /// <summary>
        /// Assignment operator.
        /// </summary>
        Nullable<T>& operator=(const Nullable<T>& other)
        {
            if (this != &other)
            {
                _value = other._value;
            }
            return *this;
        }

        /// <summary>
        /// Move assignment operator.
        /// </summary>
        Nullable<T>& operator=(Nullable<T>&& other) noexcept
        {
            if (this != &other)
            {
                _value = std::move(other._value);
            }
            return *this;
        }

        /// <summary>
        /// Assignment from value.
        /// </summary>
        Nullable<T>& operator=(const T& value)
        {
            _value = value;
            return *this;
        }

        /// <summary>
        /// Assignment to null.
        /// </summary>
        Nullable<T>& operator=(std::nullopt_t)
        {
            _value = std::nullopt;
            return *this;
        }

        /// <summary>
        /// Gets a value indicating whether the current Nullable object has a valid value of its underlying type.
        /// </summary>
        /// <returns>true if the current Nullable object has a value; false if the current Nullable object has no value.</returns>
        bool HasValue() const
        {
            return _value.has_value();
        }

        /// <summary>
        /// Gets the value of the current Nullable object if it has been assigned a valid underlying value.
        /// </summary>
        /// <returns>The value of the current Nullable object if the HasValue property is true.</returns>
        /// <exception cref="InvalidOperationException">The HasValue property is false.</exception>
        const T& Value() const
        {
            if (!_value.has_value())
            {
                throw InvalidOperationException("Nullable object must have a value");
            }
            return _value.value();
        }

        /// <summary>
        /// Retrieves the value of the current Nullable object, or the object's default value.
        /// </summary>
        /// <returns>The value of the Value property if the HasValue property is true; otherwise, the default value of the current Nullable object.</returns>
        T GetValueOrDefault() const
        {
            return _value.value_or(T{});
        }

        /// <summary>
        /// Retrieves the value of the current Nullable object, or the specified default value.
        /// </summary>
        /// <param name="defaultValue">A value to return if the HasValue property is false.</param>
        /// <returns>The value of the Value property if the HasValue property is true; otherwise, the defaultValue parameter.</returns>
        T GetValueOrDefault(const T& defaultValue) const
        {
            return _value.value_or(defaultValue);
        }

        /// <summary>
        /// Implicit conversion to bool (for null checking).
        /// </summary>
        explicit operator bool() const
        {
            return HasValue();
        }

        /// <summary>
        /// Implicit conversion to the underlying type.
        /// </summary>
        operator T() const
        {
            return Value();
        }

        /// <summary>
        /// Dereference operator.
        /// </summary>
        const T& operator*() const
        {
            return Value();
        }

        /// <summary>
        /// Arrow operator.
        /// </summary>
        const T* operator->() const
        {
            if (!HasValue())
            {
                throw InvalidOperationException("Nullable object must have a value");
            }
            return &_value.value();
        }

        /// <summary>
        /// Equality comparison with another Nullable.
        /// </summary>
        bool operator==(const Nullable<T>& other) const
        {
            if (!HasValue() && !other.HasValue()) return true;
            if (HasValue() != other.HasValue()) return false;
            return Value() == other.Value();
        }

        /// <summary>
        /// Inequality comparison with another Nullable.
        /// </summary>
        bool operator!=(const Nullable<T>& other) const
        {
            return !(*this == other);
        }

        /// <summary>
        /// Equality comparison with a value.
        /// </summary>
        bool operator==(const T& value) const
        {
            return HasValue() && Value() == value;
        }

        /// <summary>
        /// Inequality comparison with a value.
        /// </summary>
        bool operator!=(const T& value) const
        {
            return !(*this == value);
        }

        /// <summary>
        /// Equality comparison with null.
        /// </summary>
        bool operator==(std::nullopt_t) const
        {
            return !HasValue();
        }

        /// <summary>
        /// Inequality comparison with null.
        /// </summary>
        bool operator!=(std::nullopt_t) const
        {
            return HasValue();
        }

        /// <summary>
        /// Less than comparison.
        /// </summary>
        bool operator<(const Nullable<T>& other) const
        {
            if (!HasValue()) return other.HasValue();
            if (!other.HasValue()) return false;
            return Value() < other.Value();
        }

        /// <summary>
        /// Less than or equal comparison.
        /// </summary>
        bool operator<=(const Nullable<T>& other) const
        {
            return *this < other || *this == other;
        }

        /// <summary>
        /// Greater than comparison.
        /// </summary>
        bool operator>(const Nullable<T>& other) const
        {
            return other < *this;
        }

        /// <summary>
        /// Greater than or equal comparison.
        /// </summary>
        bool operator>=(const Nullable<T>& other) const
        {
            return !(*this < other);
        }

        /// <summary>
        /// Returns the hash code for this instance.
        /// </summary>
        /// <returns>A 32-bit signed integer hash code.</returns>
        virtual int GetHashCode() const override
        {
            if (!HasValue()) return 0;
            return std::hash<T>{}(Value());
        }

        /// <summary>
        /// Returns the text representation of the value of the current Nullable object.
        /// </summary>
        /// <returns>The text representation of the value of the current Nullable object if the HasValue property is true, or an empty string ("") if the HasValue property is false.</returns>
        virtual String ToString() const override
        {
            if (!HasValue()) return String("");
            
            if constexpr (std::is_arithmetic_v<T>)
            {
                return String(std::to_string(Value()));
            }
            else
            {
                // For non-arithmetic types, try to use their ToString if available
                return String("Value");
            }
        }

        /// <summary>
        /// Indicates whether this instance and a specified object are equal.
        /// </summary>
        /// <param name="obj">The object to compare with the current instance.</param>
        /// <returns>true if obj and this instance are the same type and represent the same value; otherwise, false.</returns>
        virtual bool Equals(const Object& obj) const override
        {
            if (const auto* other = dynamic_cast<const Nullable<T>*>(&obj))
            {
                return *this == *other;
            }
            return false;
        }

        /// <summary>
        /// Gets the Type of the current instance.
        /// </summary>
        /// <returns>The exact runtime type of the current instance.</returns>
        virtual Type GetType() const override
        {
            return Type::GetType<Nullable<T>>();
        }
    };

    /// <summary>
    /// Equality comparison between value and Nullable.
    /// </summary>
    template<typename T>
    bool operator==(const T& value, const Nullable<T>& nullable)
    {
        return nullable == value;
    }

    /// <summary>
    /// Inequality comparison between value and Nullable.
    /// </summary>
    template<typename T>
    bool operator!=(const T& value, const Nullable<T>& nullable)
    {
        return nullable != value;
    }

    /// <summary>
    /// Equality comparison between null and Nullable.
    /// </summary>
    template<typename T>
    bool operator==(std::nullopt_t, const Nullable<T>& nullable)
    {
        return nullable == std::nullopt;
    }

    /// <summary>
    /// Inequality comparison between null and Nullable.
    /// </summary>
    template<typename T>
    bool operator!=(std::nullopt_t, const Nullable<T>& nullable)
    {
        return nullable != std::nullopt;
    }
}

/// <summary>
/// Specialization of TypeOf::TypeTraits for Nullable<T>
/// </summary>
namespace System::Reflection::TypeOf
{
    template<typename T>
    struct TypeTraits<System::Nullable<T>>
    {
        static constexpr bool is_array = false;
        static constexpr bool is_pointer = false;
        static constexpr bool is_reference = false;
        static constexpr bool is_const = false;
        static constexpr bool is_volatile = false;
        static constexpr bool is_function = false;
        static constexpr bool is_member_function = false;
        static constexpr bool is_void = false;
        static constexpr bool is_nullable = true;
        
        using underlying_type = T;
        using base_type = T;
        using clean_type = System::Nullable<T>;
    };
}

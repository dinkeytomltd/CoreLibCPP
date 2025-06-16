
#pragma once
/*
 * Enum.h
 * C++ implementation of .NET's System.Enum class
 * 
 * Key .NET Enum characteristics implemented:
 * - Abstract base class for all enumerations
 * - Static template methods for enum operations
 * - Support for flags enums with HasFlag functionality
 * - String conversion and parsing capabilities
 * - Type-safe enum operations using modern C++20 features
 */
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>
#include <type_traits>
#include <optional>
#include <stdexcept>
#include <typeinfo>
#include <concepts>
#include "String.h"
#include "Object.h"
// #include "magic_enum.hpp" // Removed due to compatibility issues

namespace System {

// Forward declarations
template<typename T>
class FlagsAttribute;

// Concept to ensure T is an enum type
template<typename T>
concept EnumType = std::is_enum_v<T>;

// Enum registry for storing enum metadata
template<typename T>
struct EnumRegistry {
    std::unordered_map<std::string, std::underlying_type_t<T>> nameToValue;
    std::unordered_map<std::underlying_type_t<T>, std::string> valueToName;
    std::vector<std::underlying_type_t<T>> values;
    std::vector<std::string> names;
    
    void RegisterValue(const std::string& name, std::underlying_type_t<T> value) {
        nameToValue[name] = value;
        valueToName[value] = name;
        values.push_back(value);
        names.push_back(name);
    }
};

// Get enum registry for a specific enum type
template<EnumType T>
EnumRegistry<T>& GetEnumRegistry() {
    static EnumRegistry<T> registry;
    return registry;
}

// Abstract base class for all enumerations (equivalent to .NET System.Enum)
class Enum : public Object {
public:
    // Virtual destructor
    virtual ~Enum() = default;
    
    // Abstract methods that derived enum types should implement
    virtual String ToString() const override = 0;
    virtual bool Equals(const Object& obj) const override = 0;
    virtual int GetHashCode() const override = 0;
    
    // Static template methods for enum operations
    
    // Note: These methods require enum registration using REGISTER_ENUM macro
    // Parse string to enum value (equivalent to .NET Enum.Parse<T>)
    template<EnumType T>
    [[nodiscard]] static T Parse(const String& value) {
        auto& registry = GetEnumRegistry<T>();
        auto it = registry.nameToValue.find(value.ToStdString());
        if (it == registry.nameToValue.end()) {
            throw std::invalid_argument("String was not recognized as a valid enum value");
        }
        return static_cast<T>(it->second);
    }
    
    // Safe parsing with bool return (equivalent to .NET Enum.TryParse<T>)
    template<EnumType T>
    [[nodiscard]] static bool TryParse(const String& value, T& result) noexcept {
        try {
            auto& registry = GetEnumRegistry<T>();
            auto it = registry.nameToValue.find(value.ToStdString());
            if (it != registry.nameToValue.end()) {
                result = static_cast<T>(it->second);
                return true;
            }
        } catch (...) {
            // Ignore exceptions in TryParse
        }
        result = static_cast<T>(0);
        return false;
    }
    
    // Get all enum values as vector (equivalent to .NET Enum.GetValues<T>)
    template<EnumType T>
    [[nodiscard]] static std::vector<T> GetValues() {
        auto& registry = GetEnumRegistry<T>();
        std::vector<T> result;
        result.reserve(registry.values.size());
        for (auto value : registry.values) {
            result.push_back(static_cast<T>(value));
        }
        return result;
    }
    
    // Get all enum names as vector (equivalent to .NET Enum.GetNames<T>)
    template<EnumType T>
    [[nodiscard]] static std::vector<String> GetNames() {
        auto& registry = GetEnumRegistry<T>();
        std::vector<String> result;
        result.reserve(registry.names.size());
        for (const auto& name : registry.names) {
            result.emplace_back(String(name));
        }
        return result;
    }
    
    // Check if value is defined in enum (equivalent to .NET Enum.IsDefined<T>)
    template<EnumType T>
    [[nodiscard]] static bool IsDefined(T value) noexcept {
        auto& registry = GetEnumRegistry<T>();
        auto intValue = static_cast<std::underlying_type_t<T>>(value);
        return registry.valueToName.find(intValue) != registry.valueToName.end();
    }
    
    // Check if integer value is defined in enum
    template<EnumType T>
    [[nodiscard]] static bool IsDefined(std::underlying_type_t<T> value) noexcept {
        auto& registry = GetEnumRegistry<T>();
        return registry.valueToName.find(value) != registry.valueToName.end();
    }
    
    // Check if string value is defined in enum
    template<EnumType T>
    [[nodiscard]] static bool IsDefined(const String& value) noexcept {
        auto& registry = GetEnumRegistry<T>();
        return registry.nameToValue.find(value.ToStdString()) != registry.nameToValue.end();
    }
    
    // Convert enum value to string (equivalent to .NET Enum.ToString<T>)
    template<EnumType T>
    [[nodiscard]] static String ToString(T value) {
        auto& registry = GetEnumRegistry<T>();
        auto intValue = static_cast<std::underlying_type_t<T>>(value);
        auto it = registry.valueToName.find(intValue);
        if (it != registry.valueToName.end()) {
            return String(it->second);
        }
        // If no name found, return the underlying integer value
        return String(std::to_string(intValue));
    }
    
    // Get the underlying type (equivalent to .NET Enum.GetUnderlyingType<T>)
    template<EnumType T>
    [[nodiscard]] static const std::type_info& GetUnderlyingType() noexcept {
        return typeid(std::underlying_type_t<T>);
    }
    
    // Get underlying integer value
    template<EnumType T>
    [[nodiscard]] static std::underlying_type_t<T> GetUnderlyingValue(T value) noexcept {
        return static_cast<std::underlying_type_t<T>>(value);
    }
    
    // HasFlag method for flags enums (equivalent to .NET Enum.HasFlag)
    template<EnumType T>
    [[nodiscard]] static bool HasFlag(T value, T flag) noexcept {
        using UnderlyingType = std::underlying_type_t<T>;
        auto valueInt = static_cast<UnderlyingType>(value);
        auto flagInt = static_cast<UnderlyingType>(flag);
        return (valueInt & flagInt) == flagInt;
    }
    
    // Combine flags using bitwise OR
    template<EnumType T>
    [[nodiscard]] static T CombineFlags(T left, T right) noexcept {
        using UnderlyingType = std::underlying_type_t<T>;
        return static_cast<T>(static_cast<UnderlyingType>(left) | static_cast<UnderlyingType>(right));
    }
    
    // Remove flags using bitwise AND NOT
    template<EnumType T>
    [[nodiscard]] static T RemoveFlags(T value, T flagsToRemove) noexcept {
        using UnderlyingType = std::underlying_type_t<T>;
        return static_cast<T>(static_cast<UnderlyingType>(value) & ~static_cast<UnderlyingType>(flagsToRemove));
    }
    
    // Toggle flags using bitwise XOR
    template<EnumType T>
    [[nodiscard]] static T ToggleFlags(T value, T flagsToToggle) noexcept {
        using UnderlyingType = std::underlying_type_t<T>;
        return static_cast<T>(static_cast<UnderlyingType>(value) ^ static_cast<UnderlyingType>(flagsToToggle));
    }
};

// Helper template class for creating .NET-style enum wrappers
template<EnumType T>
class EnumWrapper : public Enum {
private:
    T m_value;

public:
    // Constructors
    EnumWrapper() : m_value(static_cast<T>(0)) {}
    explicit EnumWrapper(T value) : m_value(value) {}
    
    // Copy constructor and assignment
    EnumWrapper(const EnumWrapper& other) = default;
    EnumWrapper& operator=(const EnumWrapper& other) = default;
    
    // Get the wrapped value
    [[nodiscard]] T Value() const noexcept { return m_value; }
    
    // Set the wrapped value
    void SetValue(T value) noexcept { m_value = value; }
    
    // Override Object methods
    [[nodiscard]] String ToString() const override {
        return Enum::ToString<T>(m_value);
    }
    
    [[nodiscard]] bool Equals(const Object& obj) const override {
        if (const auto* other = dynamic_cast<const EnumWrapper<T>*>(&obj)) {
            return m_value == other->m_value;
        }
        return false;
    }
    
    [[nodiscard]] int GetHashCode() const override {
        return std::hash<std::underlying_type_t<T>>{}(static_cast<std::underlying_type_t<T>>(m_value));
    }
    
    // HasFlag method for this instance
    [[nodiscard]] bool HasFlag(T flag) const noexcept {
        return Enum::HasFlag<T>(m_value, flag);
    }
    
    // Operators
    bool operator==(const EnumWrapper& other) const noexcept {
        return m_value == other.m_value;
    }
    
    bool operator!=(const EnumWrapper& other) const noexcept {
        return m_value != other.m_value;
    }
    
    bool operator<(const EnumWrapper& other) const noexcept {
        return static_cast<std::underlying_type_t<T>>(m_value) < static_cast<std::underlying_type_t<T>>(other.m_value);
    }
    
    // Implicit conversion to underlying enum type
    operator T() const noexcept { return m_value; }
    
    // Bitwise operators for flags
    EnumWrapper operator|(const EnumWrapper& other) const noexcept {
        return EnumWrapper(Enum::CombineFlags<T>(m_value, other.m_value));
    }
    
    EnumWrapper operator&(const EnumWrapper& other) const noexcept {
        using UnderlyingType = std::underlying_type_t<T>;
        return EnumWrapper(static_cast<T>(static_cast<UnderlyingType>(m_value) & static_cast<UnderlyingType>(other.m_value)));
    }
    
    EnumWrapper operator^(const EnumWrapper& other) const noexcept {
        return EnumWrapper(Enum::ToggleFlags<T>(m_value, other.m_value));
    }
    
    EnumWrapper operator~() const noexcept {
        using UnderlyingType = std::underlying_type_t<T>;
        return EnumWrapper(static_cast<T>(~static_cast<UnderlyingType>(m_value)));
    }
    
    // Assignment operators
    EnumWrapper& operator|=(const EnumWrapper& other) noexcept {
        m_value = Enum::CombineFlags<T>(m_value, other.m_value);
        return *this;
    }
    
    EnumWrapper& operator&=(const EnumWrapper& other) noexcept {
        using UnderlyingType = std::underlying_type_t<T>;
        m_value = static_cast<T>(static_cast<UnderlyingType>(m_value) & static_cast<UnderlyingType>(other.m_value));
        return *this;
    }
    
    EnumWrapper& operator^=(const EnumWrapper& other) noexcept {
        m_value = Enum::ToggleFlags<T>(m_value, other.m_value);
        return *this;
    }
};

// FlagsAttribute equivalent - marker template for flags enums
template<typename T>
class FlagsAttribute {
public:
    static constexpr bool IsFlags = true;
    using EnumType = T;
};

// Helper macro to define a flags enum with proper operators
#define DEFINE_FLAGS_ENUM(EnumName, UnderlyingType) \
    enum class EnumName : UnderlyingType; \
    inline EnumName operator|(EnumName left, EnumName right) noexcept { \
        return static_cast<EnumName>(static_cast<UnderlyingType>(left) | static_cast<UnderlyingType>(right)); \
    } \
    inline EnumName operator&(EnumName left, EnumName right) noexcept { \
        return static_cast<EnumName>(static_cast<UnderlyingType>(left) & static_cast<UnderlyingType>(right)); \
    } \
    inline EnumName operator^(EnumName left, EnumName right) noexcept { \
        return static_cast<EnumName>(static_cast<UnderlyingType>(left) ^ static_cast<UnderlyingType>(right)); \
    } \
    inline EnumName operator~(EnumName value) noexcept { \
        return static_cast<EnumName>(~static_cast<UnderlyingType>(value)); \
    } \
    inline EnumName& operator|=(EnumName& left, EnumName right) noexcept { \
        left = left | right; return left; \
    } \
    inline EnumName& operator&=(EnumName& left, EnumName right) noexcept { \
        left = left & right; return left; \
    } \
    inline EnumName& operator^=(EnumName& left, EnumName right) noexcept { \
        left = left ^ right; return left; \
    } \
    enum class EnumName : UnderlyingType

// Macro to register enum values for reflection
#define REGISTER_ENUM_VALUE(EnumType, ValueName) \
    do { \
        auto& registry = System::GetEnumRegistry<EnumType>(); \
        registry.RegisterValue(#ValueName, static_cast<std::underlying_type_t<EnumType>>(EnumType::ValueName)); \
    } while(0)

// Helper macro to register multiple enum values
#define REGISTER_ENUM_BEGIN(EnumType) \
    namespace { \
        struct EnumType##_Registrar { \
            EnumType##_Registrar() { \
                using CurrentEnumType = EnumType;

#define REGISTER_ENUM_VALUE_SIMPLE(ValueName) \
                REGISTER_ENUM_VALUE(CurrentEnumType, ValueName);

#define REGISTER_ENUM_END(EnumType) \
            } \
        }; \
        static EnumType##_Registrar EnumType##_registrar_instance; \
    }

} // namespace System

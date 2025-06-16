
#pragma once
/*
 * DayOfWeek.h
 * Example implementation of a simple enum using the System.Enum infrastructure
 * Equivalent to .NET's System.DayOfWeek enum
 */
#include "Enum.h"
#include "String.h"

namespace System {

// Simple enum example (equivalent to .NET DayOfWeek)
enum class DayOfWeek : int {
    Sunday = 0,
    Monday = 1,
    Tuesday = 2,
    Wednesday = 3,
    Thursday = 4,
    Friday = 5,
    Saturday = 6
};

// Wrapper class for DayOfWeek enum
class DayOfWeekEnum : public EnumWrapper<DayOfWeek> {
public:
    // Constructors
    DayOfWeekEnum() : EnumWrapper<DayOfWeek>() {}
    explicit DayOfWeekEnum(DayOfWeek value) : EnumWrapper<DayOfWeek>(value) {}
    
    // Static factory methods for convenience
    static DayOfWeekEnum Sunday() { return DayOfWeekEnum(DayOfWeek::Sunday); }
    static DayOfWeekEnum Monday() { return DayOfWeekEnum(DayOfWeek::Monday); }
    static DayOfWeekEnum Tuesday() { return DayOfWeekEnum(DayOfWeek::Tuesday); }
    static DayOfWeekEnum Wednesday() { return DayOfWeekEnum(DayOfWeek::Wednesday); }
    static DayOfWeekEnum Thursday() { return DayOfWeekEnum(DayOfWeek::Thursday); }
    static DayOfWeekEnum Friday() { return DayOfWeekEnum(DayOfWeek::Friday); }
    static DayOfWeekEnum Saturday() { return DayOfWeekEnum(DayOfWeek::Saturday); }
    
    // Helper methods
    [[nodiscard]] bool IsWeekend() const noexcept {
        return Value() == DayOfWeek::Saturday || Value() == DayOfWeek::Sunday;
    }
    
    [[nodiscard]] bool IsWeekday() const noexcept {
        return !IsWeekend();
    }
    
    // Get next day
    [[nodiscard]] DayOfWeekEnum NextDay() const noexcept {
        int nextValue = (static_cast<int>(Value()) + 1) % 7;
        return DayOfWeekEnum(static_cast<DayOfWeek>(nextValue));
    }
    
    // Get previous day
    [[nodiscard]] DayOfWeekEnum PreviousDay() const noexcept {
        int prevValue = (static_cast<int>(Value()) + 6) % 7; // +6 is equivalent to -1 mod 7
        return DayOfWeekEnum(static_cast<DayOfWeek>(prevValue));
    }
};

} // namespace System

// Register DayOfWeek enum values for reflection
namespace {
    struct DayOfWeekRegistrar {
        DayOfWeekRegistrar() {
            auto& registry = System::GetEnumRegistry<System::DayOfWeek>();
            registry.RegisterValue("Sunday", static_cast<int>(System::DayOfWeek::Sunday));
            registry.RegisterValue("Monday", static_cast<int>(System::DayOfWeek::Monday));
            registry.RegisterValue("Tuesday", static_cast<int>(System::DayOfWeek::Tuesday));
            registry.RegisterValue("Wednesday", static_cast<int>(System::DayOfWeek::Wednesday));
            registry.RegisterValue("Thursday", static_cast<int>(System::DayOfWeek::Thursday));
            registry.RegisterValue("Friday", static_cast<int>(System::DayOfWeek::Friday));
            registry.RegisterValue("Saturday", static_cast<int>(System::DayOfWeek::Saturday));
        }
    };
    static DayOfWeekRegistrar dayOfWeekRegistrar;
}


#pragma once
/*
 * DateTime.h
 * C++ implementation of .NET's System.DateTime struct
 * 
 * Key .NET DateTime characteristics implemented:
 * - Represents an instant in time, typically expressed as a date and time of day
 * - Uses std::chrono for internal time representation
 * - Supports arithmetic operations with TimeSpan
 * - Provides comprehensive date/time manipulation methods
 */
#include <chrono>
#include <string>
#include <stdexcept>
#include <ctime>
#include <iomanip>
#include <sstream>

namespace System {

// Forward declaration
class TimeSpan;

enum class DateTimeKind {
    Unspecified = 0,
    Utc = 1,
    Local = 2
};

class DateTime {
private:
    std::chrono::system_clock::time_point m_timePoint;
    DateTimeKind m_kind;

    static constexpr int64_t TicksPerSecond = 10000000LL;
    static constexpr int64_t TicksPerMillisecond = 10000LL;
    static constexpr int64_t TicksPerMinute = 600000000LL;
    static constexpr int64_t TicksPerHour = 36000000000LL;
    static constexpr int64_t TicksPerDay = 864000000000LL;

public:
    // Constructors
    DateTime() : m_timePoint(std::chrono::system_clock::time_point::min()), m_kind(DateTimeKind::Unspecified) {}
    
    DateTime(int year, int month, int day) 
        : DateTime(year, month, day, 0, 0, 0) {}
    
    DateTime(int year, int month, int day, int hour, int minute, int second)
        : m_kind(DateTimeKind::Unspecified) {
        std::tm tm = {};
        tm.tm_year = year - 1900;
        tm.tm_mon = month - 1;
        tm.tm_mday = day;
        tm.tm_hour = hour;
        tm.tm_min = minute;
        tm.tm_sec = second;
        
        auto time_t_val = std::mktime(&tm);
        if (time_t_val == -1) {
            throw std::invalid_argument("Invalid date/time values");
        }
        m_timePoint = std::chrono::system_clock::from_time_t(time_t_val);
    }
    
    DateTime(int64_t ticks) : m_kind(DateTimeKind::Unspecified) {
        auto duration = std::chrono::duration<int64_t, std::ratio<1, TicksPerSecond>>(ticks);
        m_timePoint = std::chrono::system_clock::time_point(std::chrono::duration_cast<std::chrono::system_clock::duration>(duration));
    }
    
    DateTime(int year, int month, int day, int hour, int minute, int second, DateTimeKind kind)
        : DateTime(year, month, day, hour, minute, second) {
        m_kind = kind;
    }

    // Static properties
    [[nodiscard]] static DateTime Now() {
        DateTime dt;
        dt.m_timePoint = std::chrono::system_clock::now();
        dt.m_kind = DateTimeKind::Local;
        return dt;
    }
    
    [[nodiscard]] static DateTime UtcNow() {
        DateTime dt;
        dt.m_timePoint = std::chrono::system_clock::now();
        dt.m_kind = DateTimeKind::Utc;
        return dt;
    }
    
    [[nodiscard]] static DateTime Today() {
        auto now = Now();
        return DateTime(now.Year(), now.Month(), now.Day());
    }
    
    [[nodiscard]] static DateTime MaxValue() {
        DateTime dt;
        dt.m_timePoint = std::chrono::system_clock::time_point::max();
        dt.m_kind = DateTimeKind::Unspecified;
        return dt;
    }
    
    [[nodiscard]] static DateTime MinValue() {
        DateTime dt;
        dt.m_timePoint = std::chrono::system_clock::time_point::min();
        dt.m_kind = DateTimeKind::Unspecified;
        return dt;
    }

    // Instance properties
    [[nodiscard]] int Year() const {
        auto time_t_val = std::chrono::system_clock::to_time_t(m_timePoint);
        auto tm = *std::localtime(&time_t_val);
        return tm.tm_year + 1900;
    }
    
    [[nodiscard]] int Month() const {
        auto time_t_val = std::chrono::system_clock::to_time_t(m_timePoint);
        auto tm = *std::localtime(&time_t_val);
        return tm.tm_mon + 1;
    }
    
    [[nodiscard]] int Day() const {
        auto time_t_val = std::chrono::system_clock::to_time_t(m_timePoint);
        auto tm = *std::localtime(&time_t_val);
        return tm.tm_mday;
    }
    
    [[nodiscard]] int Hour() const {
        auto time_t_val = std::chrono::system_clock::to_time_t(m_timePoint);
        auto tm = *std::localtime(&time_t_val);
        return tm.tm_hour;
    }
    
    [[nodiscard]] int Minute() const {
        auto time_t_val = std::chrono::system_clock::to_time_t(m_timePoint);
        auto tm = *std::localtime(&time_t_val);
        return tm.tm_min;
    }
    
    [[nodiscard]] int Second() const {
        auto time_t_val = std::chrono::system_clock::to_time_t(m_timePoint);
        auto tm = *std::localtime(&time_t_val);
        return tm.tm_sec;
    }
    
    [[nodiscard]] int Millisecond() const {
        auto duration = m_timePoint.time_since_epoch();
        auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(duration);
        return static_cast<int>(millis.count() % 1000);
    }
    
    [[nodiscard]] int64_t Ticks() const {
        auto duration = m_timePoint.time_since_epoch();
        return std::chrono::duration_cast<std::chrono::duration<int64_t, std::ratio<1, TicksPerSecond>>>(duration).count();
    }
    
    [[nodiscard]] DateTimeKind Kind() const {
        return m_kind;
    }
    
    [[nodiscard]] int DayOfYear() const {
        auto time_t_val = std::chrono::system_clock::to_time_t(m_timePoint);
        auto tm = *std::localtime(&time_t_val);
        return tm.tm_yday + 1;
    }

    // Addition methods
    [[nodiscard]] DateTime Add(const TimeSpan& value) const;
    [[nodiscard]] DateTime AddDays(double value) const;
    [[nodiscard]] DateTime AddHours(double value) const;
    [[nodiscard]] DateTime AddMinutes(double value) const;
    [[nodiscard]] DateTime AddSeconds(double value) const;
    [[nodiscard]] DateTime AddMilliseconds(double value) const;
    [[nodiscard]] DateTime AddMonths(int months) const;
    [[nodiscard]] DateTime AddYears(int years) const;

    // Subtraction methods
    [[nodiscard]] TimeSpan Subtract(const DateTime& value) const;
    [[nodiscard]] DateTime Subtract(const TimeSpan& value) const;

    // Comparison methods
    [[nodiscard]] int CompareTo(const DateTime& other) const {
        if (m_timePoint < other.m_timePoint) return -1;
        if (m_timePoint > other.m_timePoint) return 1;
        return 0;
    }
    
    [[nodiscard]] bool Equals(const DateTime& other) const {
        return m_timePoint == other.m_timePoint;
    }

    // Static utility methods
    [[nodiscard]] static int DaysInMonth(int year, int month) {
        if (month < 1 || month > 12) {
            throw std::out_of_range("Month must be between 1 and 12");
        }
        
        static const int daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
        if (month == 2 && IsLeapYear(year)) {
            return 29;
        }
        return daysInMonth[month - 1];
    }
    
    [[nodiscard]] static bool IsLeapYear(int year) {
        return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
    }

    // Parsing methods
    [[nodiscard]] static DateTime Parse(const std::string& s);
    [[nodiscard]] static bool TryParse(const std::string& s, DateTime& result);

    // Formatting methods
    [[nodiscard]] std::string ToString() const;
    [[nodiscard]] std::string ToString(const std::string& format) const;
    [[nodiscard]] std::string ToShortDateString() const;
    [[nodiscard]] std::string ToLongDateString() const;
    [[nodiscard]] std::string ToShortTimeString() const;
    [[nodiscard]] std::string ToLongTimeString() const;

    // Operators
    friend bool operator==(const DateTime& left, const DateTime& right) {
        return left.Equals(right);
    }
    
    friend bool operator!=(const DateTime& left, const DateTime& right) {
        return !left.Equals(right);
    }
    
    friend bool operator<(const DateTime& left, const DateTime& right) {
        return left.CompareTo(right) < 0;
    }
    
    friend bool operator>(const DateTime& left, const DateTime& right) {
        return left.CompareTo(right) > 0;
    }
    
    friend bool operator<=(const DateTime& left, const DateTime& right) {
        return left.CompareTo(right) <= 0;
    }
    
    friend bool operator>=(const DateTime& left, const DateTime& right) {
        return left.CompareTo(right) >= 0;
    }

    friend DateTime operator+(const DateTime& dateTime, const TimeSpan& timeSpan);
    friend DateTime operator-(const DateTime& dateTime, const TimeSpan& timeSpan);
    friend TimeSpan operator-(const DateTime& left, const DateTime& right);
};

} // namespace System

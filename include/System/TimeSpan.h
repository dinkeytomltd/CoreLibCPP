
#pragma once
/*
 * TimeSpan.h
 * C++ implementation of .NET's System.TimeSpan struct
 * 
 * Key .NET TimeSpan characteristics implemented:
 * - Represents a time interval (duration of time or elapsed time)
 * - Uses std::chrono::duration for internal representation
 * - Supports arithmetic operations and comparisons
 * - Provides comprehensive time manipulation methods
 */
#include <chrono>
#include <string>
#include <stdexcept>
#include <cmath>
#include <sstream>
#include <iomanip>

namespace System {

class TimeSpan {
private:
    std::chrono::nanoseconds m_duration;

    static constexpr int64_t TicksPerSecond = 10000000LL;
    static constexpr int64_t TicksPerMillisecond = 10000LL;
    static constexpr int64_t TicksPerMinute = 600000000LL;
    static constexpr int64_t TicksPerHour = 36000000000LL;
    static constexpr int64_t TicksPerDay = 864000000000LL;

public:
    // Constructors
    TimeSpan() : m_duration(std::chrono::nanoseconds::zero()) {}
    
    TimeSpan(int64_t ticks) {
        // Convert ticks (100ns intervals) to nanoseconds
        m_duration = std::chrono::nanoseconds(ticks * 100);
    }
    
    TimeSpan(int hours, int minutes, int seconds) 
        : TimeSpan(0, hours, minutes, seconds, 0) {}
    
    TimeSpan(int days, int hours, int minutes, int seconds)
        : TimeSpan(days, hours, minutes, seconds, 0) {}
    
    TimeSpan(int days, int hours, int minutes, int seconds, int milliseconds) {
        m_duration = std::chrono::days(days) + 
                    std::chrono::hours(hours) + 
                    std::chrono::minutes(minutes) + 
                    std::chrono::seconds(seconds) + 
                    std::chrono::milliseconds(milliseconds);
    }

    // Static properties
    [[nodiscard]] static TimeSpan Zero() {
        return TimeSpan();
    }
    
    [[nodiscard]] static TimeSpan MaxValue() {
        TimeSpan ts;
        ts.m_duration = std::chrono::nanoseconds::max();
        return ts;
    }
    
    [[nodiscard]] static TimeSpan MinValue() {
        TimeSpan ts;
        ts.m_duration = std::chrono::nanoseconds::min();
        return ts;
    }

    // Component properties
    [[nodiscard]] int Days() const {
        return static_cast<int>(std::chrono::duration_cast<std::chrono::days>(m_duration).count());
    }
    
    [[nodiscard]] int Hours() const {
        auto totalHours = std::chrono::duration_cast<std::chrono::hours>(m_duration).count();
        return static_cast<int>(totalHours % 24);
    }
    
    [[nodiscard]] int Minutes() const {
        auto totalMinutes = std::chrono::duration_cast<std::chrono::minutes>(m_duration).count();
        return static_cast<int>(totalMinutes % 60);
    }
    
    [[nodiscard]] int Seconds() const {
        auto totalSeconds = std::chrono::duration_cast<std::chrono::seconds>(m_duration).count();
        return static_cast<int>(totalSeconds % 60);
    }
    
    [[nodiscard]] int Milliseconds() const {
        auto totalMilliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(m_duration).count();
        return static_cast<int>(totalMilliseconds % 1000);
    }
    
    [[nodiscard]] int64_t Ticks() const {
        // Convert nanoseconds to ticks (100ns intervals)
        return std::chrono::duration_cast<std::chrono::nanoseconds>(m_duration).count() / 100;
    }

    // Total properties
    [[nodiscard]] double TotalDays() const {
        return std::chrono::duration<double, std::ratio<86400>>(m_duration).count();
    }
    
    [[nodiscard]] double TotalHours() const {
        return std::chrono::duration<double, std::ratio<3600>>(m_duration).count();
    }
    
    [[nodiscard]] double TotalMinutes() const {
        return std::chrono::duration<double, std::ratio<60>>(m_duration).count();
    }
    
    [[nodiscard]] double TotalSeconds() const {
        return std::chrono::duration<double>(m_duration).count();
    }
    
    [[nodiscard]] double TotalMilliseconds() const {
        return std::chrono::duration<double, std::milli>(m_duration).count();
    }

    // Static creation methods
    [[nodiscard]] static TimeSpan FromDays(double value) {
        TimeSpan ts;
        ts.m_duration = std::chrono::duration_cast<std::chrono::nanoseconds>(
            std::chrono::duration<double, std::ratio<86400>>(value));
        return ts;
    }
    
    [[nodiscard]] static TimeSpan FromHours(double value) {
        TimeSpan ts;
        ts.m_duration = std::chrono::duration_cast<std::chrono::nanoseconds>(
            std::chrono::duration<double, std::ratio<3600>>(value));
        return ts;
    }
    
    [[nodiscard]] static TimeSpan FromMinutes(double value) {
        TimeSpan ts;
        ts.m_duration = std::chrono::duration_cast<std::chrono::nanoseconds>(
            std::chrono::duration<double, std::ratio<60>>(value));
        return ts;
    }
    
    [[nodiscard]] static TimeSpan FromSeconds(double value) {
        TimeSpan ts;
        ts.m_duration = std::chrono::duration_cast<std::chrono::nanoseconds>(
            std::chrono::duration<double>(value));
        return ts;
    }
    
    [[nodiscard]] static TimeSpan FromMilliseconds(double value) {
        TimeSpan ts;
        ts.m_duration = std::chrono::duration_cast<std::chrono::nanoseconds>(
            std::chrono::duration<double, std::milli>(value));
        return ts;
    }
    
    [[nodiscard]] static TimeSpan FromTicks(int64_t value) {
        return TimeSpan(value);
    }

    // Arithmetic operations
    [[nodiscard]] TimeSpan Add(const TimeSpan& ts) const {
        TimeSpan result;
        result.m_duration = m_duration + ts.m_duration;
        return result;
    }
    
    [[nodiscard]] TimeSpan Subtract(const TimeSpan& ts) const {
        TimeSpan result;
        result.m_duration = m_duration - ts.m_duration;
        return result;
    }
    
    [[nodiscard]] TimeSpan Duration() const {
        TimeSpan result;
        result.m_duration = std::chrono::abs(m_duration);
        return result;
    }
    
    [[nodiscard]] TimeSpan Negate() const {
        TimeSpan result;
        result.m_duration = -m_duration;
        return result;
    }
    
    [[nodiscard]] TimeSpan Multiply(double factor) const {
        TimeSpan result;
        result.m_duration = std::chrono::duration_cast<std::chrono::nanoseconds>(
            std::chrono::duration<double, std::nano>(m_duration.count() * factor));
        return result;
    }
    
    [[nodiscard]] TimeSpan Divide(double divisor) const {
        if (divisor == 0.0) {
            throw std::invalid_argument("Cannot divide by zero");
        }
        TimeSpan result;
        result.m_duration = std::chrono::duration_cast<std::chrono::nanoseconds>(
            std::chrono::duration<double, std::nano>(m_duration.count() / divisor));
        return result;
    }

    // Comparison methods
    [[nodiscard]] int CompareTo(const TimeSpan& other) const {
        if (m_duration < other.m_duration) return -1;
        if (m_duration > other.m_duration) return 1;
        return 0;
    }
    
    [[nodiscard]] bool Equals(const TimeSpan& other) const {
        return m_duration == other.m_duration;
    }

    // Parsing methods
    [[nodiscard]] static TimeSpan Parse(const std::string& s);
    [[nodiscard]] static bool TryParse(const std::string& s, TimeSpan& result);

    // Formatting methods
    [[nodiscard]] std::string ToString() const;
    [[nodiscard]] std::string ToString(const std::string& format) const;

    // Operators
    friend bool operator==(const TimeSpan& left, const TimeSpan& right) {
        return left.Equals(right);
    }
    
    friend bool operator!=(const TimeSpan& left, const TimeSpan& right) {
        return !left.Equals(right);
    }
    
    friend bool operator<(const TimeSpan& left, const TimeSpan& right) {
        return left.CompareTo(right) < 0;
    }
    
    friend bool operator>(const TimeSpan& left, const TimeSpan& right) {
        return left.CompareTo(right) > 0;
    }
    
    friend bool operator<=(const TimeSpan& left, const TimeSpan& right) {
        return left.CompareTo(right) <= 0;
    }
    
    friend bool operator>=(const TimeSpan& left, const TimeSpan& right) {
        return left.CompareTo(right) >= 0;
    }
    
    friend TimeSpan operator+(const TimeSpan& left, const TimeSpan& right) {
        return left.Add(right);
    }
    
    friend TimeSpan operator-(const TimeSpan& left, const TimeSpan& right) {
        return left.Subtract(right);
    }
    
    friend TimeSpan operator*(const TimeSpan& timeSpan, double factor) {
        return timeSpan.Multiply(factor);
    }
    
    friend TimeSpan operator*(double factor, const TimeSpan& timeSpan) {
        return timeSpan.Multiply(factor);
    }
    
    friend TimeSpan operator/(const TimeSpan& timeSpan, double divisor) {
        return timeSpan.Divide(divisor);
    }

    // Internal access for DateTime
    std::chrono::nanoseconds GetDuration() const { return m_duration; }
    static TimeSpan FromDuration(const std::chrono::nanoseconds& duration) {
        TimeSpan ts;
        ts.m_duration = duration;
        return ts;
    }
};

} // namespace System


#pragma once
/*
 * DateTimeOffset.h
 * C++ implementation of .NET's System.DateTimeOffset struct
 * 
 * Key .NET DateTimeOffset characteristics implemented:
 * - Represents a point in time with timezone offset information
 * - Combines DateTime with timezone offset
 * - Provides UTC and local time conversions
 * - Supports arithmetic operations with TimeSpan
 */
#include "DateTime.h"
#include "TimeSpan.h"
#include <chrono>
#include <string>

namespace System {

class DateTimeOffset {
private:
    DateTime m_dateTime;
    TimeSpan m_offset;

public:
    // Constructors
    DateTimeOffset() : m_dateTime(), m_offset() {}
    
    DateTimeOffset(const DateTime& dateTime) 
        : m_dateTime(dateTime), m_offset(GetLocalOffset()) {}
    
    DateTimeOffset(const DateTime& dateTime, const TimeSpan& offset) 
        : m_dateTime(dateTime), m_offset(offset) {
        ValidateOffset(offset);
    }
    
    DateTimeOffset(int year, int month, int day, int hour, int minute, int second, const TimeSpan& offset)
        : m_dateTime(year, month, day, hour, minute, second), m_offset(offset) {
        ValidateOffset(offset);
    }
    
    DateTimeOffset(int64_t ticks, const TimeSpan& offset)
        : m_dateTime(ticks), m_offset(offset) {
        ValidateOffset(offset);
    }

    // Static properties
    [[nodiscard]] static DateTimeOffset Now() {
        return DateTimeOffset(DateTime::Now());
    }
    
    [[nodiscard]] static DateTimeOffset UtcNow() {
        return DateTimeOffset(DateTime::UtcNow(), TimeSpan::Zero());
    }
    
    [[nodiscard]] static DateTimeOffset MaxValue() {
        return DateTimeOffset(DateTime::MaxValue(), TimeSpan::Zero());
    }
    
    [[nodiscard]] static DateTimeOffset MinValue() {
        return DateTimeOffset(DateTime::MinValue(), TimeSpan::Zero());
    }

    // Instance properties
    [[nodiscard]] System::DateTime GetDateTime() const { return m_dateTime; }
    [[nodiscard]] System::DateTime Date() const { 
        return System::DateTime(m_dateTime.Year(), m_dateTime.Month(), m_dateTime.Day()); 
    }
    [[nodiscard]] TimeSpan TimeOfDay() const {
        return TimeSpan(0, m_dateTime.Hour(), m_dateTime.Minute(), m_dateTime.Second(), m_dateTime.Millisecond());
    }
    [[nodiscard]] TimeSpan Offset() const { return m_offset; }
    
    [[nodiscard]] System::DateTime UtcDateTime() const {
        return m_dateTime.Subtract(m_offset);
    }
    
    [[nodiscard]] System::DateTime LocalDateTime() const {
        auto localOffset = GetLocalOffset();
        return m_dateTime.Add(localOffset).Subtract(m_offset);
    }
    
    [[nodiscard]] int64_t Ticks() const { return m_dateTime.Ticks(); }
    [[nodiscard]] int64_t UtcTicks() const { return UtcDateTime().Ticks(); }
    
    // Date/time component properties
    [[nodiscard]] int Year() const { return m_dateTime.Year(); }
    [[nodiscard]] int Month() const { return m_dateTime.Month(); }
    [[nodiscard]] int Day() const { return m_dateTime.Day(); }
    [[nodiscard]] int Hour() const { return m_dateTime.Hour(); }
    [[nodiscard]] int Minute() const { return m_dateTime.Minute(); }
    [[nodiscard]] int Second() const { return m_dateTime.Second(); }
    [[nodiscard]] int Millisecond() const { return m_dateTime.Millisecond(); }
    [[nodiscard]] int DayOfYear() const { return m_dateTime.DayOfYear(); }

    // Addition methods
    [[nodiscard]] DateTimeOffset Add(const TimeSpan& timeSpan) const {
        return DateTimeOffset(m_dateTime.Add(timeSpan), m_offset);
    }
    
    [[nodiscard]] DateTimeOffset AddDays(double value) const {
        return DateTimeOffset(m_dateTime.AddDays(value), m_offset);
    }
    
    [[nodiscard]] DateTimeOffset AddHours(double value) const {
        return DateTimeOffset(m_dateTime.AddHours(value), m_offset);
    }
    
    [[nodiscard]] DateTimeOffset AddMinutes(double value) const {
        return DateTimeOffset(m_dateTime.AddMinutes(value), m_offset);
    }
    
    [[nodiscard]] DateTimeOffset AddSeconds(double value) const {
        return DateTimeOffset(m_dateTime.AddSeconds(value), m_offset);
    }
    
    [[nodiscard]] DateTimeOffset AddMilliseconds(double value) const {
        return DateTimeOffset(m_dateTime.AddMilliseconds(value), m_offset);
    }
    
    [[nodiscard]] DateTimeOffset AddMonths(int months) const {
        return DateTimeOffset(m_dateTime.AddMonths(months), m_offset);
    }
    
    [[nodiscard]] DateTimeOffset AddYears(int years) const {
        return DateTimeOffset(m_dateTime.AddYears(years), m_offset);
    }

    // Subtraction methods
    [[nodiscard]] TimeSpan Subtract(const DateTimeOffset& value) const {
        return UtcDateTime().Subtract(value.UtcDateTime());
    }
    
    [[nodiscard]] DateTimeOffset Subtract(const TimeSpan& value) const {
        return DateTimeOffset(m_dateTime.Subtract(value), m_offset);
    }

    // Conversion methods
    [[nodiscard]] DateTimeOffset ToOffset(const TimeSpan& offset) const {
        ValidateOffset(offset);
        auto utcDateTime = UtcDateTime();
        return DateTimeOffset(utcDateTime.Add(offset), offset);
    }
    
    [[nodiscard]] DateTimeOffset ToLocalTime() const {
        auto localOffset = GetLocalOffset();
        return ToOffset(localOffset);
    }
    
    [[nodiscard]] DateTimeOffset ToUniversalTime() const {
        return DateTimeOffset(UtcDateTime(), TimeSpan::Zero());
    }

    // Comparison methods
    [[nodiscard]] int CompareTo(const DateTimeOffset& other) const {
        return UtcDateTime().CompareTo(other.UtcDateTime());
    }
    
    [[nodiscard]] bool Equals(const DateTimeOffset& other) const {
        return UtcDateTime().Equals(other.UtcDateTime());
    }

    // Parsing methods
    [[nodiscard]] static DateTimeOffset Parse(const std::string& s);
    [[nodiscard]] static bool TryParse(const std::string& s, DateTimeOffset& result);

    // Formatting methods
    [[nodiscard]] std::string ToString() const;
    [[nodiscard]] std::string ToString(const std::string& format) const;

    // Operators
    friend bool operator==(const DateTimeOffset& left, const DateTimeOffset& right) {
        return left.Equals(right);
    }
    
    friend bool operator!=(const DateTimeOffset& left, const DateTimeOffset& right) {
        return !left.Equals(right);
    }
    
    friend bool operator<(const DateTimeOffset& left, const DateTimeOffset& right) {
        return left.CompareTo(right) < 0;
    }
    
    friend bool operator>(const DateTimeOffset& left, const DateTimeOffset& right) {
        return left.CompareTo(right) > 0;
    }
    
    friend bool operator<=(const DateTimeOffset& left, const DateTimeOffset& right) {
        return left.CompareTo(right) <= 0;
    }
    
    friend bool operator>=(const DateTimeOffset& left, const DateTimeOffset& right) {
        return left.CompareTo(right) >= 0;
    }
    
    friend DateTimeOffset operator+(const DateTimeOffset& dateTimeOffset, const TimeSpan& timeSpan) {
        return dateTimeOffset.Add(timeSpan);
    }
    
    friend DateTimeOffset operator-(const DateTimeOffset& dateTimeOffset, const TimeSpan& timeSpan) {
        return dateTimeOffset.Subtract(timeSpan);
    }
    
    friend TimeSpan operator-(const DateTimeOffset& left, const DateTimeOffset& right) {
        return left.Subtract(right);
    }

private:
    static void ValidateOffset(const TimeSpan& offset) {
        auto totalHours = offset.TotalHours();
        if (totalHours < -14.0 || totalHours > 14.0) {
            throw std::out_of_range("Offset must be within +/- 14 hours");
        }
    }
    
    static TimeSpan GetLocalOffset() {
        // Get current local time offset
        auto now = std::chrono::system_clock::now();
        auto utc_time = std::chrono::system_clock::to_time_t(now);
        auto local_tm = *std::localtime(&utc_time);
        auto utc_tm = *std::gmtime(&utc_time);
        
        auto local_time_t = std::mktime(&local_tm);
        auto utc_time_t = std::mktime(&utc_tm);
        
        auto offset_seconds = static_cast<double>(local_time_t - utc_time_t);
        return TimeSpan::FromSeconds(offset_seconds);
    }
};

} // namespace System

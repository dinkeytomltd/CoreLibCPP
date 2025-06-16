
#include "System/DateTime.h"
#include "System/TimeSpan.h"
#include <sstream>
#include <iomanip>
#include <regex>

namespace System {

DateTime DateTime::Add(const TimeSpan& value) const {
    DateTime result = *this;
    result.m_timePoint += value.GetDuration();
    return result;
}

DateTime DateTime::AddDays(double value) const {
    return Add(TimeSpan::FromDays(value));
}

DateTime DateTime::AddHours(double value) const {
    return Add(TimeSpan::FromHours(value));
}

DateTime DateTime::AddMinutes(double value) const {
    return Add(TimeSpan::FromMinutes(value));
}

DateTime DateTime::AddSeconds(double value) const {
    return Add(TimeSpan::FromSeconds(value));
}

DateTime DateTime::AddMilliseconds(double value) const {
    return Add(TimeSpan::FromMilliseconds(value));
}

DateTime DateTime::AddMonths(int months) const {
    int newYear = Year();
    int newMonth = Month() + months;
    
    while (newMonth > 12) {
        newMonth -= 12;
        newYear++;
    }
    while (newMonth < 1) {
        newMonth += 12;
        newYear--;
    }
    
    int newDay = Day();
    int maxDaysInNewMonth = DaysInMonth(newYear, newMonth);
    if (newDay > maxDaysInNewMonth) {
        newDay = maxDaysInNewMonth;
    }
    
    return DateTime(newYear, newMonth, newDay, Hour(), Minute(), Second(), m_kind);
}

DateTime DateTime::AddYears(int years) const {
    return AddMonths(years * 12);
}

TimeSpan DateTime::Subtract(const DateTime& value) const {
    auto duration = m_timePoint - value.m_timePoint;
    return TimeSpan::FromDuration(std::chrono::duration_cast<std::chrono::nanoseconds>(duration));
}

DateTime DateTime::Subtract(const TimeSpan& value) const {
    DateTime result = *this;
    result.m_timePoint -= value.GetDuration();
    return result;
}

DateTime DateTime::Parse(const std::string& s) {
    DateTime result;
    if (!TryParse(s, result)) {
        throw std::invalid_argument("Unable to parse DateTime from string: " + s);
    }
    return result;
}

bool DateTime::TryParse(const std::string& s, DateTime& result) {
    // Simple parsing for common formats: YYYY-MM-DD HH:MM:SS
    std::regex dateTimeRegex(R"((\d{4})-(\d{1,2})-(\d{1,2})\s+(\d{1,2}):(\d{1,2}):(\d{1,2}))");
    std::regex dateRegex(R"((\d{4})-(\d{1,2})-(\d{1,2}))");
    
    std::smatch match;
    
    if (std::regex_match(s, match, dateTimeRegex)) {
        try {
            int year = std::stoi(match[1]);
            int month = std::stoi(match[2]);
            int day = std::stoi(match[3]);
            int hour = std::stoi(match[4]);
            int minute = std::stoi(match[5]);
            int second = std::stoi(match[6]);
            
            result = DateTime(year, month, day, hour, minute, second);
            return true;
        } catch (...) {
            return false;
        }
    } else if (std::regex_match(s, match, dateRegex)) {
        try {
            int year = std::stoi(match[1]);
            int month = std::stoi(match[2]);
            int day = std::stoi(match[3]);
            
            result = DateTime(year, month, day);
            return true;
        } catch (...) {
            return false;
        }
    }
    
    return false;
}

std::string DateTime::ToString() const {
    return ToString("yyyy-MM-dd HH:mm:ss");
}

std::string DateTime::ToString(const std::string& format) const {
    auto time_t_val = std::chrono::system_clock::to_time_t(m_timePoint);
    auto tm = *std::localtime(&time_t_val);
    
    std::ostringstream oss;
    
    if (format == "yyyy-MM-dd HH:mm:ss") {
        oss << std::setfill('0') 
            << std::setw(4) << (tm.tm_year + 1900) << "-"
            << std::setw(2) << (tm.tm_mon + 1) << "-"
            << std::setw(2) << tm.tm_mday << " "
            << std::setw(2) << tm.tm_hour << ":"
            << std::setw(2) << tm.tm_min << ":"
            << std::setw(2) << tm.tm_sec;
    } else if (format == "yyyy-MM-dd") {
        oss << std::setfill('0') 
            << std::setw(4) << (tm.tm_year + 1900) << "-"
            << std::setw(2) << (tm.tm_mon + 1) << "-"
            << std::setw(2) << tm.tm_mday;
    } else if (format == "HH:mm:ss") {
        oss << std::setfill('0') 
            << std::setw(2) << tm.tm_hour << ":"
            << std::setw(2) << tm.tm_min << ":"
            << std::setw(2) << tm.tm_sec;
    } else {
        // Default format
        oss << std::setfill('0') 
            << std::setw(4) << (tm.tm_year + 1900) << "-"
            << std::setw(2) << (tm.tm_mon + 1) << "-"
            << std::setw(2) << tm.tm_mday << " "
            << std::setw(2) << tm.tm_hour << ":"
            << std::setw(2) << tm.tm_min << ":"
            << std::setw(2) << tm.tm_sec;
    }
    
    return oss.str();
}

std::string DateTime::ToShortDateString() const {
    return ToString("yyyy-MM-dd");
}

std::string DateTime::ToLongDateString() const {
    auto time_t_val = std::chrono::system_clock::to_time_t(m_timePoint);
    auto tm = *std::localtime(&time_t_val);
    
    const char* months[] = {"January", "February", "March", "April", "May", "June",
                           "July", "August", "September", "October", "November", "December"};
    
    std::ostringstream oss;
    oss << months[tm.tm_mon] << " " << tm.tm_mday << ", " << (tm.tm_year + 1900);
    return oss.str();
}

std::string DateTime::ToShortTimeString() const {
    return ToString("HH:mm:ss");
}

std::string DateTime::ToLongTimeString() const {
    return ToString("HH:mm:ss");
}

// Operator implementations
DateTime operator+(const DateTime& dateTime, const TimeSpan& timeSpan) {
    return dateTime.Add(timeSpan);
}

DateTime operator-(const DateTime& dateTime, const TimeSpan& timeSpan) {
    return dateTime.Subtract(timeSpan);
}

TimeSpan operator-(const DateTime& left, const DateTime& right) {
    return left.Subtract(right);
}

} // namespace System


#include "System/TimeSpan.h"
#include <sstream>
#include <iomanip>
#include <regex>

namespace System {

TimeSpan TimeSpan::Parse(const std::string& s) {
    TimeSpan result;
    if (!TryParse(s, result)) {
        throw std::invalid_argument("Unable to parse TimeSpan from string: " + s);
    }
    return result;
}

bool TimeSpan::TryParse(const std::string& s, TimeSpan& result) {
    // Parse formats like: "1.02:03:04.567" (days.hours:minutes:seconds.milliseconds)
    // or "02:03:04" (hours:minutes:seconds)
    // or "1:02:03:04" (days:hours:minutes:seconds)
    
    std::regex fullRegex(R"((-?)(\d+)\.(\d{1,2}):(\d{1,2}):(\d{1,2})(?:\.(\d{1,3}))?)");
    std::regex timeRegex(R"((-?)(\d{1,2}):(\d{1,2}):(\d{1,2})(?:\.(\d{1,3}))?)");
    std::regex daysTimeRegex(R"((-?)(\d+):(\d{1,2}):(\d{1,2}):(\d{1,2})(?:\.(\d{1,3}))?)");
    
    std::smatch match;
    
    if (std::regex_match(s, match, fullRegex)) {
        try {
            bool negative = !match[1].str().empty();
            int days = std::stoi(match[2]);
            int hours = std::stoi(match[3]);
            int minutes = std::stoi(match[4]);
            int seconds = std::stoi(match[5]);
            int milliseconds = match[6].matched ? std::stoi(match[6]) : 0;
            
            result = TimeSpan(days, hours, minutes, seconds, milliseconds);
            if (negative) result = result.Negate();
            return true;
        } catch (...) {
            return false;
        }
    } else if (std::regex_match(s, match, daysTimeRegex)) {
        try {
            bool negative = !match[1].str().empty();
            int days = std::stoi(match[2]);
            int hours = std::stoi(match[3]);
            int minutes = std::stoi(match[4]);
            int seconds = std::stoi(match[5]);
            int milliseconds = match[6].matched ? std::stoi(match[6]) : 0;
            
            result = TimeSpan(days, hours, minutes, seconds, milliseconds);
            if (negative) result = result.Negate();
            return true;
        } catch (...) {
            return false;
        }
    } else if (std::regex_match(s, match, timeRegex)) {
        try {
            bool negative = !match[1].str().empty();
            int hours = std::stoi(match[2]);
            int minutes = std::stoi(match[3]);
            int seconds = std::stoi(match[4]);
            int milliseconds = match[5].matched ? std::stoi(match[5]) : 0;
            
            result = TimeSpan(hours, minutes, seconds);
            result = result.Add(TimeSpan::FromMilliseconds(milliseconds));
            if (negative) result = result.Negate();
            return true;
        } catch (...) {
            return false;
        }
    }
    
    return false;
}

std::string TimeSpan::ToString() const {
    std::ostringstream oss;
    
    bool negative = m_duration.count() < 0;
    auto absDuration = std::chrono::abs(m_duration);
    
    auto days = std::chrono::duration_cast<std::chrono::days>(absDuration);
    absDuration -= days;
    auto hours = std::chrono::duration_cast<std::chrono::hours>(absDuration);
    absDuration -= hours;
    auto minutes = std::chrono::duration_cast<std::chrono::minutes>(absDuration);
    absDuration -= minutes;
    auto seconds = std::chrono::duration_cast<std::chrono::seconds>(absDuration);
    absDuration -= seconds;
    auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(absDuration);
    
    if (negative) oss << "-";
    
    if (days.count() > 0) {
        oss << days.count() << ".";
    }
    
    oss << std::setfill('0') 
        << std::setw(2) << hours.count() << ":"
        << std::setw(2) << minutes.count() << ":"
        << std::setw(2) << seconds.count();
    
    if (milliseconds.count() > 0) {
        oss << "." << std::setw(3) << milliseconds.count();
    }
    
    return oss.str();
}

std::string TimeSpan::ToString(const std::string& format) const {
    if (format == "c" || format.empty()) {
        return ToString();
    }
    
    // For now, just return the default format
    // In a full implementation, we would parse the format string
    return ToString();
}

} // namespace System

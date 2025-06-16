
#include "System/DateTimeOffset.h"
#include <sstream>
#include <iomanip>
#include <regex>

namespace System {

DateTimeOffset DateTimeOffset::Parse(const std::string& s) {
    DateTimeOffset result;
    if (!TryParse(s, result)) {
        throw std::invalid_argument("Unable to parse DateTimeOffset from string: " + s);
    }
    return result;
}

bool DateTimeOffset::TryParse(const std::string& s, DateTimeOffset& result) {
    // Parse formats like: "2023-12-25 10:30:45 +05:30" or "2023-12-25T10:30:45+05:30"
    std::regex offsetRegex(R"((\d{4})-(\d{1,2})-(\d{1,2})[T\s](\d{1,2}):(\d{1,2}):(\d{1,2})\s*([+-])(\d{1,2}):(\d{1,2}))");
    std::regex utcRegex(R"((\d{4})-(\d{1,2})-(\d{1,2})[T\s](\d{1,2}):(\d{1,2}):(\d{1,2})\s*Z)");
    
    std::smatch match;
    
    if (std::regex_match(s, match, offsetRegex)) {
        try {
            int year = std::stoi(match[1]);
            int month = std::stoi(match[2]);
            int day = std::stoi(match[3]);
            int hour = std::stoi(match[4]);
            int minute = std::stoi(match[5]);
            int second = std::stoi(match[6]);
            
            bool offsetPositive = match[7] == "+";
            int offsetHours = std::stoi(match[8]);
            int offsetMinutes = std::stoi(match[9]);
            
            auto offset = TimeSpan(offsetHours, offsetMinutes, 0);
            if (!offsetPositive) offset = offset.Negate();
            
            auto dateTime = System::DateTime(year, month, day, hour, minute, second);
            result = DateTimeOffset(dateTime, offset);
            return true;
        } catch (...) {
            return false;
        }
    } else if (std::regex_match(s, match, utcRegex)) {
        try {
            int year = std::stoi(match[1]);
            int month = std::stoi(match[2]);
            int day = std::stoi(match[3]);
            int hour = std::stoi(match[4]);
            int minute = std::stoi(match[5]);
            int second = std::stoi(match[6]);
            
            auto dateTime = System::DateTime(year, month, day, hour, minute, second);
            result = DateTimeOffset(dateTime, TimeSpan::Zero());
            return true;
        } catch (...) {
            return false;
        }
    }
    
    return false;
}

std::string DateTimeOffset::ToString() const {
    return ToString("yyyy-MM-dd HH:mm:ss zzz");
}

std::string DateTimeOffset::ToString(const std::string& format) const {
    std::ostringstream oss;
    
    // Get the date/time part
    oss << m_dateTime.ToString("yyyy-MM-dd HH:mm:ss");
    
    // Add offset part
    if (format.find("zzz") != std::string::npos || format.find("K") != std::string::npos) {
        auto totalMinutes = static_cast<int>(m_offset.TotalMinutes());
        if (totalMinutes == 0) {
            oss << " Z";
        } else {
            oss << " " << (totalMinutes >= 0 ? "+" : "-");
            int absMinutes = std::abs(totalMinutes);
            int hours = absMinutes / 60;
            int minutes = absMinutes % 60;
            oss << std::setfill('0') << std::setw(2) << hours << ":" << std::setw(2) << minutes;
        }
    }
    
    return oss.str();
}

} // namespace System

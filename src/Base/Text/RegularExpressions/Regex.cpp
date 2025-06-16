
/*
 * Regex.cpp
 * Implementation file for System.Text.RegularExpressions.Regex
 */
#include "../../../../include/System/Text/RegularExpressions/Regex.h"
#include <algorithm>

namespace System {
namespace Text {
namespace RegularExpressions {

// Constructors
Regex::Regex(const String& pattern) : m_pattern(pattern), m_options(RegexOptions::None) {
    try {
        m_regex = std::regex(pattern.ToStdString(), ConvertOptions(m_options));
    } catch (const std::regex_error& e) {
        throw std::invalid_argument("Invalid regex pattern: " + std::string(e.what()));
    }
}

Regex::Regex(const String& pattern, RegexOptions options) : m_pattern(pattern), m_options(options) {
    try {
        m_regex = std::regex(pattern.ToStdString(), ConvertOptions(m_options));
    } catch (const std::regex_error& e) {
        throw std::invalid_argument("Invalid regex pattern: " + std::string(e.what()));
    }
}

// Instance methods
bool Regex::IsMatch(const String& input) const {
    return IsMatch(input, 0);
}

bool Regex::IsMatch(const String& input, size_t startat) const {
    std::string str = input.ToStdString();
    if (startat > str.length()) {
        return false;
    }
    
    std::string substr = str.substr(startat);
    return std::regex_search(substr, m_regex, ConvertMatchFlags(m_options));
}

Match Regex::MatchSingle(const String& input) const {
    return MatchSingle(input, 0);
}

Match Regex::MatchSingle(const String& input, size_t startat) const {
    std::string str = input.ToStdString();
    if (startat > str.length()) {
        return ::System::Text::RegularExpressions::Match::Empty();
    }
    
    std::string substr = str.substr(startat);
    std::smatch match;
    
    if (std::regex_search(substr, match, m_regex, ConvertMatchFlags(m_options))) {
        std::vector<Group> groups;
        
        // Add all capture groups
        for (size_t i = 0; i < match.size(); ++i) {
            const auto& submatch = match[i];
            if (submatch.matched) {
                groups.emplace_back(
                    String(submatch.str()),
                    startat + submatch.first - substr.begin(),
                    submatch.length(),
                    true
                );
            } else {
                groups.emplace_back();
            }
        }
        
        return ::System::Text::RegularExpressions::Match(
            String(match.str()),
            startat + match.position(),
            match.length(),
            true,
            groups
        );
    }
    
    return ::System::Text::RegularExpressions::Match::Empty();
}

MatchCollection Regex::Matches(const String& input) const {
    return Matches(input, 0);
}

MatchCollection Regex::Matches(const String& input, size_t startat) const {
    std::vector<::System::Text::RegularExpressions::Match> matches;
    std::string str = input.ToStdString();
    
    if (startat > str.length()) {
        return MatchCollection(matches);
    }
    
    std::string substr = str.substr(startat);
    std::sregex_iterator iter(substr.begin(), substr.end(), m_regex, ConvertMatchFlags(m_options));
    std::sregex_iterator end;
    
    for (; iter != end; ++iter) {
        const std::smatch& match = *iter;
        std::vector<Group> groups;
        
        // Add all capture groups
        for (size_t i = 0; i < match.size(); ++i) {
            const auto& submatch = match[i];
            if (submatch.matched) {
                groups.emplace_back(
                    String(submatch.str()),
                    startat + submatch.first - substr.begin(),
                    submatch.length(),
                    true
                );
            } else {
                groups.emplace_back();
            }
        }
        
        matches.emplace_back(
            String(match.str()),
            startat + match.position(),
            match.length(),
            true,
            groups
        );
    }
    
    return MatchCollection(matches);
}

String Regex::Replace(const String& input, const String& replacement) const {
    std::string str = input.ToStdString();
    std::string repl = replacement.ToStdString();
    std::string result = std::regex_replace(str, m_regex, repl, ConvertMatchFlags(m_options));
    return String(result);
}

String Regex::Replace(const String& input, const String& replacement, int count) const {
    if (count <= 0) {
        return input;
    }
    
    std::string str = input.ToStdString();
    std::string repl = replacement.ToStdString();
    std::string result = str;
    
    std::sregex_iterator iter(str.begin(), str.end(), m_regex, ConvertMatchFlags(m_options));
    std::sregex_iterator end;
    
    int replacements = 0;
    size_t offset = 0;
    
    for (; iter != end && replacements < count; ++iter, ++replacements) {
        const std::smatch& match = *iter;
        size_t pos = match.position() + offset;
        size_t len = match.length();
        
        result.replace(pos, len, repl);
        offset += repl.length() - len;
    }
    
    return String(result);
}

std::vector<String> Regex::Split(const String& input) const {
    return Split(input, 0);
}

std::vector<String> Regex::Split(const String& input, int count) const {
    std::vector<String> result;
    std::string str = input.ToStdString();
    
    std::sregex_token_iterator iter(str.begin(), str.end(), m_regex, -1, ConvertMatchFlags(m_options));
    std::sregex_token_iterator end;
    
    int splits = 0;
    for (; iter != end && (count <= 0 || splits < count); ++iter, ++splits) {
        result.emplace_back(iter->str());
    }
    
    return result;
}

// Static methods
bool Regex::IsMatch(const String& input, const String& pattern) {
    return IsMatch(input, pattern, RegexOptions::None);
}

bool Regex::IsMatch(const String& input, const String& pattern, RegexOptions options) {
    Regex regex(pattern, options);
    return regex.IsMatch(input);
}

Match Regex::MatchSingle(const String& input, const String& pattern) {
    return MatchSingle(input, pattern, RegexOptions::None);
}

Match Regex::MatchSingle(const String& input, const String& pattern, RegexOptions options) {
    Regex regex(pattern, options);
    return regex.MatchSingle(input);
}

MatchCollection Regex::Matches(const String& input, const String& pattern) {
    return Matches(input, pattern, RegexOptions::None);
}

MatchCollection Regex::Matches(const String& input, const String& pattern, RegexOptions options) {
    Regex regex(pattern, options);
    return regex.Matches(input);
}

String Regex::Replace(const String& input, const String& pattern, const String& replacement) {
    return Replace(input, pattern, replacement, RegexOptions::None);
}

String Regex::Replace(const String& input, const String& pattern, const String& replacement, RegexOptions options) {
    Regex regex(pattern, options);
    return regex.Replace(input, replacement);
}

std::vector<String> Regex::Split(const String& input, const String& pattern) {
    return Split(input, pattern, RegexOptions::None);
}

std::vector<String> Regex::Split(const String& input, const String& pattern, RegexOptions options) {
    Regex regex(pattern, options);
    return regex.Split(input);
}

// Helper methods
std::regex_constants::syntax_option_type Regex::ConvertOptions(RegexOptions options) {
    std::regex_constants::syntax_option_type result = std::regex_constants::ECMAScript;
    
    if ((options & RegexOptions::IgnoreCase) != RegexOptions::None) {
        result |= std::regex_constants::icase;
    }
    
    if ((options & RegexOptions::Multiline) != RegexOptions::None) {
        // std::regex doesn't have direct multiline support, but we can use nosubs for some cases
    }
    
    if ((options & RegexOptions::IgnorePatternWhitespace) != RegexOptions::None) {
        // std::regex doesn't support this directly
    }
    
    return result;
}

std::regex_constants::match_flag_type Regex::ConvertMatchFlags(RegexOptions options) {
    std::regex_constants::match_flag_type result = std::regex_constants::match_default;
    
    // Add any match-specific flags based on options
    return result;
}

} // namespace RegularExpressions
} // namespace Text
} // namespace System

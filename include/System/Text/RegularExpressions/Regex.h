
#pragma once
/*
 * Regex.h
 * C++ implementation of .NET's System.Text.RegularExpressions.Regex class
 * 
 * Key .NET Regex characteristics implemented:
 * - Methods: IsMatch, Match, Matches for pattern matching
 * - Replace method for substitution operations
 * - Split method for splitting strings based on patterns
 * - Support for regex options and compiled patterns
 * - Match and MatchCollection classes for results
 */
#include <regex>
#include <string>
#include <vector>
#include <memory>
#include "../../String.h"

namespace System {
namespace Text {
namespace RegularExpressions {

// Forward declarations
class Match;
class MatchCollection;

// Regex options enumeration
enum class RegexOptions {
    None = 0,
    IgnoreCase = 1,
    Multiline = 2,
    Singleline = 4,
    ExplicitCapture = 8,
    Compiled = 16,
    IgnorePatternWhitespace = 32,
    RightToLeft = 64,
    ECMAScript = 256,
    CultureInvariant = 512
};

// Enable bitwise operations for RegexOptions
inline RegexOptions operator|(RegexOptions a, RegexOptions b) {
    return static_cast<RegexOptions>(static_cast<int>(a) | static_cast<int>(b));
}

inline RegexOptions operator&(RegexOptions a, RegexOptions b) {
    return static_cast<RegexOptions>(static_cast<int>(a) & static_cast<int>(b));
}

class Regex {
private:
    std::regex m_regex;
    String m_pattern;
    RegexOptions m_options;

public:
    // Constructors
    explicit Regex(const String& pattern);
    Regex(const String& pattern, RegexOptions options);

    // Properties
    [[nodiscard]] String Pattern() const { return m_pattern; }
    [[nodiscard]] RegexOptions Options() const { return m_options; }

    // Instance methods
    [[nodiscard]] bool IsMatch(const String& input) const;
    [[nodiscard]] bool IsMatch(const String& input, size_t startat) const;
    
    [[nodiscard]] Match MatchSingle(const String& input) const;
    [[nodiscard]] Match MatchSingle(const String& input, size_t startat) const;
    
    [[nodiscard]] MatchCollection Matches(const String& input) const;
    [[nodiscard]] MatchCollection Matches(const String& input, size_t startat) const;
    
    [[nodiscard]] String Replace(const String& input, const String& replacement) const;
    [[nodiscard]] String Replace(const String& input, const String& replacement, int count) const;
    
    [[nodiscard]] std::vector<String> Split(const String& input) const;
    [[nodiscard]] std::vector<String> Split(const String& input, int count) const;

    // Static methods
    [[nodiscard]] static bool IsMatch(const String& input, const String& pattern);
    [[nodiscard]] static bool IsMatch(const String& input, const String& pattern, RegexOptions options);
    
    [[nodiscard]] static Match MatchSingle(const String& input, const String& pattern);
    [[nodiscard]] static Match MatchSingle(const String& input, const String& pattern, RegexOptions options);
    
    [[nodiscard]] static MatchCollection Matches(const String& input, const String& pattern);
    [[nodiscard]] static MatchCollection Matches(const String& input, const String& pattern, RegexOptions options);
    
    [[nodiscard]] static String Replace(const String& input, const String& pattern, const String& replacement);
    [[nodiscard]] static String Replace(const String& input, const String& pattern, const String& replacement, RegexOptions options);
    
    [[nodiscard]] static std::vector<String> Split(const String& input, const String& pattern);
    [[nodiscard]] static std::vector<String> Split(const String& input, const String& pattern, RegexOptions options);

private:
    static std::regex_constants::syntax_option_type ConvertOptions(RegexOptions options);
    static std::regex_constants::match_flag_type ConvertMatchFlags(RegexOptions options);
};

// Group class for capture groups
class Group {
private:
    String m_value;
    size_t m_index;
    size_t m_length;
    bool m_success;

public:
    Group() : m_index(0), m_length(0), m_success(false) {}
    Group(const String& value, size_t index, size_t length, bool success)
        : m_value(value), m_index(index), m_length(length), m_success(success) {}

    [[nodiscard]] String Value() const { return m_value; }
    [[nodiscard]] size_t Index() const { return m_index; }
    [[nodiscard]] size_t Length() const { return m_length; }
    [[nodiscard]] bool Success() const { return m_success; }
};

// Match class for individual matches
class Match {
private:
    String m_value;
    size_t m_index;
    size_t m_length;
    bool m_success;
    std::vector<Group> m_groups;

public:
    Match() : m_index(0), m_length(0), m_success(false) {}
    Match(const String& value, size_t index, size_t length, bool success, const std::vector<Group>& groups)
        : m_value(value), m_index(index), m_length(length), m_success(success), m_groups(groups) {}

    [[nodiscard]] String Value() const { return m_value; }
    [[nodiscard]] size_t Index() const { return m_index; }
    [[nodiscard]] size_t Length() const { return m_length; }
    [[nodiscard]] bool Success() const { return m_success; }
    [[nodiscard]] const std::vector<Group>& Groups() const { return m_groups; }
    [[nodiscard]] Group Groups(size_t index) const {
        if (index >= m_groups.size()) {
            return Group();
        }
        return m_groups[index];
    }

    // Static empty match
    [[nodiscard]] static Match Empty() { return Match(); }
};

// MatchCollection class for multiple matches
class MatchCollection {
private:
    std::vector<Match> m_matches;

public:
    MatchCollection() = default;
    explicit MatchCollection(const std::vector<Match>& matches) : m_matches(matches) {}

    [[nodiscard]] size_t Count() const { return m_matches.size(); }
    [[nodiscard]] const Match& operator[](size_t index) const {
        if (index >= m_matches.size()) {
            throw std::out_of_range("Index was out of range");
        }
        return m_matches[index];
    }

    // Iterator support
    [[nodiscard]] auto begin() const { return m_matches.begin(); }
    [[nodiscard]] auto end() const { return m_matches.end(); }
};

} // namespace RegularExpressions
} // namespace Text
} // namespace System

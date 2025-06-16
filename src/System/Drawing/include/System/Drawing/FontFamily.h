
#pragma once

#include <string>
#include <vector>
#include <memory>

// Forward declarations for Skia
class SkTypeface;
class SkFontMgr;

namespace System {
namespace Drawing {

// Forward declarations
enum class FontStyle;

class FontFamily {
private:
    std::string _name;
    mutable std::shared_ptr<SkFontMgr> _fontManager;
    
public:
    // Constructors
    explicit FontFamily(const std::string& name);
    
    // Copy constructor and assignment
    FontFamily(const FontFamily& other);
    FontFamily& operator=(const FontFamily& other);
    
    // Move constructor and assignment
    FontFamily(FontFamily&& other) noexcept;
    FontFamily& operator=(FontFamily&& other) noexcept;
    
    // Destructor
    ~FontFamily() = default;
    
    // Properties
    std::string GetName() const { return _name; }
    
    // Style availability
    bool IsStyleAvailable(FontStyle style) const;
    
    // Metrics
    int GetEmHeight(FontStyle style) const;
    int GetCellAscent(FontStyle style) const;
    int GetCellDescent(FontStyle style) const;
    int GetLineSpacing(FontStyle style) const;
    
    // Static methods
    static std::vector<FontFamily> GetFamilies();
    static std::vector<FontFamily> GetFamilies(const Graphics& graphics);
    
    // Generic font families
    static FontFamily GenericSansSerif();
    static FontFamily GenericSerif();
    static FontFamily GenericMonospace();
    
    // Equality
    bool operator==(const FontFamily& other) const;
    bool operator!=(const FontFamily& other) const;
    
    // String representation
    std::string ToString() const;
    
    // Skia integration
    std::shared_ptr<SkTypeface> CreateTypeface(FontStyle style) const;
    std::shared_ptr<SkFontMgr> GetFontManager() const;
    
private:
    void InitializeFontManager() const;
};

}
}

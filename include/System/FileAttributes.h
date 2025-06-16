
#pragma once
/*
 * FileAttributes.h
 * Example implementation of a flags enum using the System.Enum infrastructure
 * Equivalent to .NET's System.IO.FileAttributes enum
 */
#include "Enum.h"
#include "String.h"

namespace System {

// Flags enum example (equivalent to .NET FileAttributes)
DEFINE_FLAGS_ENUM(FileAttributes, int) {
    None = 0,
    ReadOnly = 1 << 0,      // 1
    Hidden = 1 << 1,        // 2
    System = 1 << 2,        // 4
    Directory = 1 << 3,     // 8
    Archive = 1 << 4,       // 16
    Device = 1 << 5,        // 32
    Normal = 1 << 6,        // 64
    Temporary = 1 << 7,     // 128
    SparseFile = 1 << 8,    // 256
    ReparsePoint = 1 << 9,  // 512
    Compressed = 1 << 10,   // 1024
    Offline = 1 << 11,      // 2048
    NotContentIndexed = 1 << 12, // 4096
    Encrypted = 1 << 13,    // 8192
    IntegrityStream = 1 << 14,   // 16384
    NoScrubData = 1 << 15   // 32768
};

// Wrapper class for FileAttributes enum
class FileAttributesEnum : public EnumWrapper<FileAttributes> {
public:
    // Constructors
    FileAttributesEnum() : EnumWrapper<FileAttributes>() {}
    explicit FileAttributesEnum(FileAttributes value) : EnumWrapper<FileAttributes>(value) {}
    
    // Static factory methods for convenience
    static FileAttributesEnum None() { return FileAttributesEnum(FileAttributes::None); }
    static FileAttributesEnum ReadOnly() { return FileAttributesEnum(FileAttributes::ReadOnly); }
    static FileAttributesEnum Hidden() { return FileAttributesEnum(FileAttributes::Hidden); }
    static FileAttributesEnum System() { return FileAttributesEnum(FileAttributes::System); }
    static FileAttributesEnum Directory() { return FileAttributesEnum(FileAttributes::Directory); }
    static FileAttributesEnum Archive() { return FileAttributesEnum(FileAttributes::Archive); }
    static FileAttributesEnum Device() { return FileAttributesEnum(FileAttributes::Device); }
    static FileAttributesEnum Normal() { return FileAttributesEnum(FileAttributes::Normal); }
    static FileAttributesEnum Temporary() { return FileAttributesEnum(FileAttributes::Temporary); }
    static FileAttributesEnum SparseFile() { return FileAttributesEnum(FileAttributes::SparseFile); }
    static FileAttributesEnum ReparsePoint() { return FileAttributesEnum(FileAttributes::ReparsePoint); }
    static FileAttributesEnum Compressed() { return FileAttributesEnum(FileAttributes::Compressed); }
    static FileAttributesEnum Offline() { return FileAttributesEnum(FileAttributes::Offline); }
    static FileAttributesEnum NotContentIndexed() { return FileAttributesEnum(FileAttributes::NotContentIndexed); }
    static FileAttributesEnum Encrypted() { return FileAttributesEnum(FileAttributes::Encrypted); }
    static FileAttributesEnum IntegrityStream() { return FileAttributesEnum(FileAttributes::IntegrityStream); }
    static FileAttributesEnum NoScrubData() { return FileAttributesEnum(FileAttributes::NoScrubData); }
    
    // Helper methods for common flag combinations
    [[nodiscard]] bool IsFile() const noexcept {
        return !HasFlag(FileAttributes::Directory);
    }
    
    [[nodiscard]] bool IsDirectory() const noexcept {
        return HasFlag(FileAttributes::Directory);
    }
    
    [[nodiscard]] bool IsHidden() const noexcept {
        return HasFlag(FileAttributes::Hidden);
    }
    
    [[nodiscard]] bool IsReadOnly() const noexcept {
        return HasFlag(FileAttributes::ReadOnly);
    }
    
    [[nodiscard]] bool IsSystemFile() const noexcept {
        return HasFlag(FileAttributes::System);
    }
    
    [[nodiscard]] bool IsTemporary() const noexcept {
        return HasFlag(FileAttributes::Temporary);
    }
    
    [[nodiscard]] bool IsCompressed() const noexcept {
        return HasFlag(FileAttributes::Compressed);
    }
    
    [[nodiscard]] bool IsEncrypted() const noexcept {
        return HasFlag(FileAttributes::Encrypted);
    }
    
    // Add a specific attribute
    void AddAttribute(FileAttributes attribute) noexcept {
        SetValue(Value() | attribute);
    }
    
    // Remove a specific attribute
    void RemoveAttribute(FileAttributes attribute) noexcept {
        SetValue(Enum::RemoveFlags<FileAttributes>(Value(), attribute));
    }
    
    // Toggle a specific attribute
    void ToggleAttribute(FileAttributes attribute) noexcept {
        SetValue(Enum::ToggleFlags<FileAttributes>(Value(), attribute));
    }
    
    // Get a string representation of all set flags
    [[nodiscard]] String ToFlagsString() const {
        if (Value() == FileAttributes::None) {
            return String("None");
        }
        
        std::vector<String> flags;
        
        if (HasFlag(FileAttributes::ReadOnly)) flags.push_back(String("ReadOnly"));
        if (HasFlag(FileAttributes::Hidden)) flags.push_back(String("Hidden"));
        if (HasFlag(FileAttributes::System)) flags.push_back(String("System"));
        if (HasFlag(FileAttributes::Directory)) flags.push_back(String("Directory"));
        if (HasFlag(FileAttributes::Archive)) flags.push_back(String("Archive"));
        if (HasFlag(FileAttributes::Device)) flags.push_back(String("Device"));
        if (HasFlag(FileAttributes::Normal)) flags.push_back(String("Normal"));
        if (HasFlag(FileAttributes::Temporary)) flags.push_back(String("Temporary"));
        if (HasFlag(FileAttributes::SparseFile)) flags.push_back(String("SparseFile"));
        if (HasFlag(FileAttributes::ReparsePoint)) flags.push_back(String("ReparsePoint"));
        if (HasFlag(FileAttributes::Compressed)) flags.push_back(String("Compressed"));
        if (HasFlag(FileAttributes::Offline)) flags.push_back(String("Offline"));
        if (HasFlag(FileAttributes::NotContentIndexed)) flags.push_back(String("NotContentIndexed"));
        if (HasFlag(FileAttributes::Encrypted)) flags.push_back(String("Encrypted"));
        if (HasFlag(FileAttributes::IntegrityStream)) flags.push_back(String("IntegrityStream"));
        if (HasFlag(FileAttributes::NoScrubData)) flags.push_back(String("NoScrubData"));
        
        return String::Join(String(", "), flags);
    }
};

} // namespace System

// Register FileAttributes enum values for reflection
namespace {
    struct FileAttributesRegistrar {
        FileAttributesRegistrar() {
            auto& registry = System::GetEnumRegistry<System::FileAttributes>();
            registry.RegisterValue("None", static_cast<int>(System::FileAttributes::None));
            registry.RegisterValue("ReadOnly", static_cast<int>(System::FileAttributes::ReadOnly));
            registry.RegisterValue("Hidden", static_cast<int>(System::FileAttributes::Hidden));
            registry.RegisterValue("System", static_cast<int>(System::FileAttributes::System));
            registry.RegisterValue("Directory", static_cast<int>(System::FileAttributes::Directory));
            registry.RegisterValue("Archive", static_cast<int>(System::FileAttributes::Archive));
            registry.RegisterValue("Device", static_cast<int>(System::FileAttributes::Device));
            registry.RegisterValue("Normal", static_cast<int>(System::FileAttributes::Normal));
            registry.RegisterValue("Temporary", static_cast<int>(System::FileAttributes::Temporary));
            registry.RegisterValue("SparseFile", static_cast<int>(System::FileAttributes::SparseFile));
            registry.RegisterValue("ReparsePoint", static_cast<int>(System::FileAttributes::ReparsePoint));
            registry.RegisterValue("Compressed", static_cast<int>(System::FileAttributes::Compressed));
            registry.RegisterValue("Offline", static_cast<int>(System::FileAttributes::Offline));
            registry.RegisterValue("NotContentIndexed", static_cast<int>(System::FileAttributes::NotContentIndexed));
            registry.RegisterValue("Encrypted", static_cast<int>(System::FileAttributes::Encrypted));
            registry.RegisterValue("IntegrityStream", static_cast<int>(System::FileAttributes::IntegrityStream));
            registry.RegisterValue("NoScrubData", static_cast<int>(System::FileAttributes::NoScrubData));
        }
    };
    static FileAttributesRegistrar fileAttributesRegistrar;
}

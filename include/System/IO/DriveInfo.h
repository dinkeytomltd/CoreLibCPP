
#pragma once
/*
 * DriveInfo.h
 * C++ implementation of .NET's System.IO.DriveInfo class
 * 
 * Provides access to information on a drive.
 */
#include "../Object.h"
#include "../String.h"
#include "DriveType.h"
#include <vector>
#include <memory>

namespace System::IO {

class DriveInfo : public Object {
private:
    String m_name;
    mutable bool m_dataInitialized;
    mutable DriveType m_driveType;
    mutable String m_driveFormat;
    mutable int64_t m_totalSize;
    mutable int64_t m_totalFreeSpace;
    mutable int64_t m_availableFreeSpace;
    mutable bool m_isReady;

    void RefreshData() const;
    void EnsureDataInitialized() const;

public:
    // Constructors
    explicit DriveInfo(const String& driveName);
    
    // Copy constructor and assignment
    DriveInfo(const DriveInfo& other);
    DriveInfo& operator=(const DriveInfo& other);
    DriveInfo(DriveInfo&& other) noexcept;
    DriveInfo& operator=(DriveInfo&& other) noexcept;
    
    // Destructor
    virtual ~DriveInfo() = default;
    
    // Properties
    [[nodiscard]] String Name() const { return m_name; }
    [[nodiscard]] System::IO::DriveType GetDriveType() const;
    [[nodiscard]] String DriveFormat() const;
    [[nodiscard]] bool IsReady() const;
    [[nodiscard]] int64_t TotalSize() const;
    [[nodiscard]] int64_t TotalFreeSpace() const;
    [[nodiscard]] int64_t AvailableFreeSpace() const;
    [[nodiscard]] String VolumeLabel() const;
    void SetVolumeLabel(const String& value);
    
    // Static methods
    static std::vector<std::shared_ptr<DriveInfo>> GetDrives();
    
    // Object overrides
    [[nodiscard]] String ToString() const override;
    [[nodiscard]] bool Equals(const Object& obj) const override;
    [[nodiscard]] int GetHashCode() const override;

private:
    void Initialize(const String& driveName);
    static String NormalizeDriveName(const String& driveName);
};

} // namespace System::IO

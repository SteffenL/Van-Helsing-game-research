#include <common/Registry.h>
#include <Windows.h>
#include <WinReg.h>
#include <stdexcept>
#include <iostream>
#include <string>
#include <vector>

#pragma comment(lib, "advapi32.lib")

// Define missing stuff in MinGW
#ifndef LSTATUS
#define LSTATUS LONG
#endif

namespace common {

class RegistryImpl
{
public:
    static void ExtractRegKeyParts(const std::string& fullPath, HKEY& rootKey, std::string& key);
    static void ExtractRegKeyAndValueParts(const std::string& fullPath, std::string& key, std::string& valueName);
};

// Takes a full registry key path and breaks it into parts:
// root key, key
// Example input: HKCU\Software
void RegistryImpl::ExtractRegKeyParts(const std::string& fullPath, HKEY& rootKey, std::string& key)
{
    std::string::size_type slashPos = fullPath.find_first_of('\\');
    if (slashPos == std::string::npos) {
        throw std::logic_error("Badly formatted key path");
    }

    const std::string& rootKeyStr = fullPath.substr(0, slashPos);
    if (rootKeyStr == "HKCR") {
        rootKey = HKEY_CLASSES_ROOT;
    }
    else if (rootKeyStr == "HKCC") {
        rootKey = HKEY_CURRENT_CONFIG;
    }
    else if (rootKeyStr == "HKCU") {
        rootKey = HKEY_CURRENT_USER;
    }
    else if (rootKeyStr == "HKLM") {
        rootKey = HKEY_LOCAL_MACHINE;
    }
    else if (rootKeyStr == "HKU") {
        rootKey = HKEY_USERS;
    }
    else {
        throw std::logic_error("Invalid root key");
    }

    key = fullPath.substr(slashPos + 1);
}

// Takes a full registry key path and breaks it into parts:
// key, value name
void RegistryImpl::ExtractRegKeyAndValueParts(const std::string& fullPath, std::string& key, std::string& valueName)
{
    std::string::size_type slashPos = fullPath.find_last_of('\\');
    if (slashPos == std::string::npos) {
        key.clear();
        valueName = fullPath;
        return;
    }

    key = fullPath.substr(0, slashPos);
    valueName = fullPath.substr(slashPos + 1);
}

// Check if a registry key exists
bool Registry::KeyExists(const std::string& fullPath, RegWow64Flag::type wow64Flag)
{
    HKEY rootKey;
    std::string key;
    RegistryImpl::ExtractRegKeyParts(fullPath, rootKey, key);

    LONG regWow64Flags = 0;
    regWow64Flags |= (wow64Flag == RegWow64Flag::_32bit) ? KEY_WOW64_32KEY: 0;
    regWow64Flags |= (wow64Flag == RegWow64Flag::_64bit) ? KEY_WOW64_64KEY: 0;

    HKEY hkey = NULL;
    LSTATUS status = ::RegOpenKeyExA(rootKey, key.c_str(), 0, KEY_READ | regWow64Flags, &hkey);

    switch (status) {
    case ERROR_SUCCESS:
        ::RegCloseKey(hkey);
        return true;
    case ERROR_FILE_NOT_FOUND:
        return false;
    default:
        throw std::runtime_error("Couldn't open registry key");
    }
}

// Checks if a registry value exists
bool Registry::ValueExists(const std::string& fullPath, RegWow64Flag::type wow64Flag)
{
    HKEY rootKey;
    std::string keyAndValueName;
    std::string key;
    std::string valueName;
    RegistryImpl::ExtractRegKeyParts(fullPath, rootKey, keyAndValueName);
    RegistryImpl::ExtractRegKeyAndValueParts(keyAndValueName, key, valueName);

    LONG regWow64Flags = 0;
    regWow64Flags |= (wow64Flag == RegWow64Flag::_32bit) ? KEY_WOW64_32KEY: 0;
    regWow64Flags |= (wow64Flag == RegWow64Flag::_64bit) ? KEY_WOW64_64KEY: 0;

    HKEY hkey = NULL;
    LSTATUS status = ::RegOpenKeyExA(rootKey, key.c_str(), 0, KEY_READ | regWow64Flags, &hkey);

    if (status != ERROR_SUCCESS) {
        switch (status) {
        case ERROR_FILE_NOT_FOUND:
            return false;
        default:
            throw std::runtime_error("Couldn't open registry key");
        }
    }

    status = ::RegQueryValueExA(hkey, valueName.c_str(), NULL, NULL, NULL, NULL);
    ::RegCloseKey(hkey);

    switch (status) {
    case ERROR_SUCCESS:
        return true;
    case ERROR_FILE_NOT_FOUND:
        return false;
    default:
        throw std::runtime_error("Couldn't query registry value");
    }
}

bool Registry::GetValue(const std::string& fullPath, std::string& v, RegWow64Flag::type wow64Flag)
{
    HKEY rootKey;
    std::string keyAndValueName;
    std::string key;
    std::string valueName;
    RegistryImpl::ExtractRegKeyParts(fullPath, rootKey, keyAndValueName);
    RegistryImpl::ExtractRegKeyAndValueParts(keyAndValueName, key, valueName);

    LONG regWow64Flags = 0;
    regWow64Flags |= (wow64Flag == RegWow64Flag::_32bit) ? KEY_WOW64_32KEY: 0;
    regWow64Flags |= (wow64Flag == RegWow64Flag::_64bit) ? KEY_WOW64_64KEY: 0;

    HKEY hkey = NULL;
    LSTATUS status = ::RegOpenKeyExA(rootKey, key.c_str(), 0, KEY_READ | regWow64Flags, &hkey);

    if (status != ERROR_SUCCESS) {
        switch (status) {
        case ERROR_FILE_NOT_FOUND:
            return false;
        default:
            throw std::runtime_error("Couldn't open registry key");
        }
    }

    DWORD bufLen = 1024;
    std::vector<char> buf;
    buf.resize(bufLen);
    status = ::RegQueryValueExA(hkey, valueName.c_str(), NULL, nullptr, reinterpret_cast<LPBYTE>(&buf[0]), &bufLen);
    ::RegCloseKey(hkey);

    switch (status) {
    case ERROR_SUCCESS:
        buf.resize(bufLen - sizeof(buf[0]));
        v.assign(buf.begin(), buf.end());
        return true;
    case ERROR_FILE_NOT_FOUND:
        return false;
    default:
        throw std::runtime_error("Couldn't query registry value");
    }
}

}

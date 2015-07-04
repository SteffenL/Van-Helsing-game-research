#pragma once

#include <string>
#include <memory>

namespace common {

struct RegWow64Flag
{
    enum type
    {
        None,
        _32bit,
        _64bit
    };
};

class Registry
{
public:
    static bool KeyExists(const std::string& fullPath, RegWow64Flag::type wow64Flag = RegWow64Flag::None);
    static bool ValueExists(const std::string& fullPath, RegWow64Flag::type wow64Flag = RegWow64Flag::None);
    static bool GetValue(const std::string& fullPath, std::string& v, RegWow64Flag::type wow64Flag = RegWow64Flag::None);
};

}

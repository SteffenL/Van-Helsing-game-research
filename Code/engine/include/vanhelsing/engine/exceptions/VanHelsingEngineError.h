#pragma once

#include <stdexcept>

namespace vanhelsing { namespace engine {

    class VanHelsingEngineError : public std::runtime_error
{
public:
    VanHelsingEngineError(const std::string& message)
        : runtime_error(message)
    {
    }
};

}}

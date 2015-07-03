#pragma once

#include <nowide/iostream.hpp>

namespace nowide { namespace details {

class my_console_output_buffer : public std::streambuf {
public:
    my_console_output_buffer(void* h);
protected:
    int sync();
    int overflow(int c);
private:
        
    int write(char const *p,int n);
        
    static const int buffer_size = 1024;
    char buffer_[buffer_size];
    wchar_t wbuffer_[buffer_size]; // for null
    void* handle_;
    bool isatty_;
};

}}

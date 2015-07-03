#include "my_console_output_buffer.h"

#include <nowide/iostream.hpp>
#include <nowide/convert.hpp>
#include <stdio.h>
#include <vector>
#include <Windows.h>

namespace nowide { namespace details {

int my_console_output_buffer::write(char const *p,int n)
{
    namespace uf = nowide::utf;
    char const *b = p;
    char const *e = p+n;
    if(!isatty_) {
        DWORD size=0;
        if(!WriteFile(handle_,p,n,&size,0) || static_cast<int>(size) != n)
            return -1;
        return n;
    }
    if(n > buffer_size)
        return -1;
    wchar_t *out = wbuffer_;
    uf::code_point c;
    size_t decoded = 0;
    while(p < e && (c = uf::utf_traits<char>::decode(p,e))!=uf::illegal && c!=uf::incomplete) {
        out = uf::utf_traits<wchar_t>::encode(c,out);
        decoded = p-b;
    }
    if(c==uf::illegal)
        return -1;
    if(!WriteConsoleW(handle_,wbuffer_,out - wbuffer_,0,0))
        return -1;
    return decoded;
}

int my_console_output_buffer::overflow(int c)
{
    if(!handle_)
        return -1;
    int n = pptr() - pbase();
    int r = 0;

    if(n > 0 && (r=write(pbase(),n)) < 0)
        return -1;
    if(r < n) {
        memmove(pbase(),pbase() + r,n-r);
    }
    setp(buffer_, buffer_ + buffer_size);
    pbump(n-r);
    if(c!=EOF)
        sputc(c);
    return 0;
}

int my_console_output_buffer::sync()
{
    return overflow(EOF);
}

my_console_output_buffer::my_console_output_buffer(void* h) :
    handle_(h),
    isatty_(false)
{
    if(handle_) {
        DWORD dummy;
        isatty_ = GetConsoleMode(handle_,&dummy) == TRUE;
    }
}

}}

#ifndef __vanhelsing_engine_log__
#define __vanhelsing_engine_log__

#include <iostream>
#include <sstream>
#include <string>
#include <streambuf>

namespace vanhelsing { namespace engine {

class Log : public std::ostream
{
    class LogBuffer : public std::stringbuf
    {
    public:
        LogBuffer(std::ostream& stream);
        virtual int sync();

    protected:
        std::ostream& m_stream;
    };

public:
    Log();
    static void Indent();
    static void Outdent();
    static void Print(const std::string& s);

private:
    static int m_indentLevel;
    LogBuffer m_buffer;
};

}} // namespace
#endif // guard

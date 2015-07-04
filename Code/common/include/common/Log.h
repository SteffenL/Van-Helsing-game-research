#pragma once

#include <iostream>
#include <sstream>
#include <string>
#include <streambuf>

namespace common {

struct LogLevel
{
    enum type
    {
        All, Trace, Debug, Info, Warning, Error, Fatal, Off
    };
};

class Log : public std::ostream
{
    class LogBuffer : public std::stringbuf
    {
    public:
        LogBuffer(std::ostream& stream, LogLevel::type logLevel);
        virtual int sync();
        void Indent();
        void Outdent();

    protected:
        std::ostream& m_stream;
        LogLevel::type m_logLevel;
        int m_indentLevel;
    };

public:
    static std::ostream& indent(std::ostream& os);
    static std::ostream& outdent(std::ostream& os);

    Log(LogLevel::type logLevel = LogLevel::Info);
    static void Indent();
    static void Outdent();
    static void Print(const std::string& s);
    static LogLevel::type GetLogLevelFilter();
    static void SetLogLevelFilter(LogLevel::type logLevel);

private:
    static int sm_indentLevel;
    LogBuffer m_buffer;
    static LogLevel::type sm_logLevelFilter;
};

}

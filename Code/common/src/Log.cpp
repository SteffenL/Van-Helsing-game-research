#include <common/Log.h>
#include <iomanip>
#include <sstream>
#include <nowide/iostream.hpp>

namespace common {

int Log::sm_indentLevel = 0;
LogLevel::type Log::sm_logLevelFilter = LogLevel::Info;

void Log::Indent() { sm_indentLevel += 1; }

void Log::Outdent() { sm_indentLevel -= 1; }

Log::Log(LogLevel::type logLevel)
    // May want to use nowide::clog
    : std::ostream(&m_buffer), m_buffer(nowide::cout, logLevel) {}

LogLevel::type Log::GetLogLevelFilter()
{
    return sm_logLevelFilter;
}

void Log::SetLogLevelFilter(LogLevel::type logLevel)
{
    sm_logLevelFilter = logLevel;
}

std::ostream& Log::indent(std::ostream& os)
{
    auto logger = dynamic_cast<Log*>(&os);
    if (logger) {
        logger->m_buffer.Indent();
    }
    
    return os;
}

std::ostream& Log::outdent(std::ostream& os)
{
    auto logger = dynamic_cast<Log*>(&os);
    if (logger) {
        logger->m_buffer.Outdent();
    }

    return os;
}

int Log::LogBuffer::sync()
{
    if (m_logLevel >= Log::GetLogLevelFilter()) {
        m_stream << std::setfill(' ') << std::setw(2 * m_indentLevel) << "" << str();
    }

    str("");
    m_stream.flush();
    return 0;
}

Log::LogBuffer::LogBuffer(std::ostream& stream, LogLevel::type logLevel)
    : m_stream(stream), m_logLevel(logLevel), m_indentLevel(0) {}

void Log::LogBuffer::Indent() { m_indentLevel += 1; }

void Log::LogBuffer::Outdent() { m_indentLevel -= 1; }

}

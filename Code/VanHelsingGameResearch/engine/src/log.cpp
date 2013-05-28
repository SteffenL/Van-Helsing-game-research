#include <vanhelsing/engine/log.h>
#include <iomanip>
#include <sstream>
#include <nowide/iostream.hpp>

namespace vanhelsing { namespace engine {

int Log::m_indentLevel = 0;

void Log::Indent() { m_indentLevel += 1; }

void Log::Outdent() { m_indentLevel -= 1; }

Log::Log() : std::ostream(&m_buffer), m_buffer(nowide::clog) {}

int Log::LogBuffer::sync()
{
    m_stream << std::setfill(' ') << std::setw(2 * m_indentLevel) << "" << std::dec << str() << std::dec;
    str("");
    m_stream.flush();
    return 0;
}

Log::LogBuffer::LogBuffer(std::ostream& stream) : m_stream(stream) {}

}} // namespace

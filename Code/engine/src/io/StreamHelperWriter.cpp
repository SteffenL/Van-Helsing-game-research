#include <vanhelsing/engine/io/StreamHelperWriter.h>
#include <nowide/convert.hpp>
#include <vector>

namespace vanhelsing { namespace engine { namespace io {

void StreamHelperWriter::WriteWString(const std::wstring& val, ushort length)
{
    if (!Write(reinterpret_cast<const char*>(val.c_str()), length * sizeof(wchar_t)))
    {
        throw WriteFailedException();
    }
}

void StreamHelperWriter::WriteWString(const std::wstring& val)
{
    Write<unsigned short>(val.size());
    // The string could be empty
    if (val.empty())
    {
        return;
    }

    if (!Write(reinterpret_cast<const char*>(val.c_str()), val.size() * sizeof(wchar_t)))
    {
        throw WriteFailedException();
    }
}

void StreamHelperWriter::WriteStringUtf8(const std::wstring& val)
{
    // Length in bytes
    Write<unsigned short>(val.size());
    // The string could be empty
    if (val.empty())
    {
        return;
    }

    std::string& tempVal = nowide::narrow(val);
    std::vector<char> tempVal2(tempVal.begin(), tempVal.end());
    if (!Write(&tempVal2[0], tempVal2.size() * sizeof(char)))
    {
        throw WriteFailedException();
    }
}

void StreamHelperWriter::WriteString(const std::string& val, ushort length)
{
    std::vector<char> tempVal(val.begin(), val.end());
    if (!Write(&val[0], length * sizeof(char)))
    {
        throw WriteFailedException();
    }
}

void StreamHelperWriter::WriteString(const std::string& val)
{
    Write<unsigned short>(val.size());
    // The string could be empty
    if (val.empty())
    {
        return;
    }

    std::vector<char> tempVal(val.begin(), val.end());
    if (!Write(&tempVal[0], val.size() * sizeof(char)))
    {
        throw WriteFailedException();
    }
}

bool StreamHelperWriter::Write(const char* buffer, std::streamsize size)
{
    m_stream.write(buffer, size);
    bool failed = m_stream.fail();
    m_currentOffset += !failed ? size : 0;
    return !failed;
}

void StreamHelperWriter::SeekO(std::streamoff offset, std::ios::seekdir direction /*= std::ios::cur*/)
{
    auto oldOffset = m_stream.tellp();
    m_stream.seekp(offset, direction);
    m_currentOffset += (m_stream.tellp() - oldOffset);
}

std::streamoff StreamHelperWriter::TellO() const
{
    return m_currentOffset;
}

bool StreamHelperWriter::CanWrite()
{
    return (!m_stream.bad() && !m_stream.eof() /*&& (m_currentOffset < m_streamSize)*/);
}

StreamHelperWriter::StreamHelperWriter(std::ostream& stream) : m_stream(stream), m_currentOffset(0), m_streamSize(0)
{
    /*auto oldOffset = m_stream.tellp();
    // Get stream size
    m_stream.seekp(0, std::ios::end);
    m_streamSize = m_stream.tellp();
    m_stream.seekp(oldOffset, std::ios::beg);*/
}

}}}

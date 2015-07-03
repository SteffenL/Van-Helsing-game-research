#include <vanhelsing/engine/io/StreamHelperReader.h>
#include <nowide/convert.hpp>

namespace vanhelsing { namespace engine { namespace io {

std::wstring StreamHelperReader::ReadWString(ushort length)
{
    std::wstring val;
    val.resize(length);
    if (!Read(reinterpret_cast<char*>(&val[0]), length * sizeof(wchar_t)))
    {
        throw ReadFailedException();
    }

    return val;
}

std::wstring StreamHelperReader::ReadWString()
{
    ushort length = Read<unsigned short>();
    // The string could be empty
    if (length == 0)
    {
        return L"";
    }

    std::wstring val;
    val.resize(length);
    if (!Read(reinterpret_cast<char*>(&val[0]), length * sizeof(wchar_t)))
    {
        throw ReadFailedException();
    }

    return val;
}

std::wstring StreamHelperReader::ReadStringUtf8()
{
    // Length in bytes
    ushort length = Read<unsigned short>();
    // The string could be empty
    if (length == 0)
    {
        return L"";
    }

    std::string tempVal;
    tempVal.resize(length);
    if (!Read(&tempVal[0], length * sizeof(char)))
    {
        throw ReadFailedException();
    }

    std::wstring& val = nowide::widen(tempVal);
    return val;
}

std::string StreamHelperReader::ReadString(ushort length)
{
    std::string val;
    val.resize(length);
    if (!Read(&val[0], length * sizeof(char)))
    {
        throw ReadFailedException();
    }

    return val;
}

std::string StreamHelperReader::ReadString()
{
    ushort length = Read<unsigned short>();
    // The string could be empty
    if (length == 0)
    {
        return "";
    }

    std::string val;
    val.resize(length);
    if (!Read(&val[0], length * sizeof(char)))
    {
        throw ReadFailedException();
    }

    return val;
}

bool StreamHelperReader::Read(char* buffer, std::streamsize size)
{
    m_stream.read(buffer, size);
    bool failed = m_stream.fail();
    m_currentOffset += !failed ? size : 0;
    return !failed;
}

StreamHelperReader::StreamHelperReader(std::istream& stream) : m_stream(stream), m_currentOffset(0)
{
    auto oldOffset = m_stream.tellg();
    // Get stream size
    m_stream.seekg(0, std::ios::end);
    m_streamSize = m_stream.tellg();
    m_stream.seekg(oldOffset, std::ios::beg);
}

bool StreamHelperReader::CanRead()
{
    return (!m_stream.bad() && !m_stream.eof() && (m_currentOffset < m_streamSize));
}

void StreamHelperReader::SeekI(std::streamoff offset, std::ios::seekdir direction /*= std::ios::cur*/)
{
    auto oldOffset = m_stream.tellg();
    m_stream.seekg(offset, direction);
    m_currentOffset += (m_stream.tellg() - oldOffset);
}

}}}

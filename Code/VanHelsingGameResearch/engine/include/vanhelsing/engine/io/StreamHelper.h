#ifndef __vanhelsing_engine_io_StreamHelper__
#define __vanhelsing_engine_io_StreamHelper__

#include <iostream>
#include <nowide/convert.hpp>

namespace vanhelsing { namespace engine { namespace io {

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>   Stream helper class for basic things like reading from a stream. </summary>
///
/// <remarks>   Steffen 5, 5/30/2012. </remarks>
////////////////////////////////////////////////////////////////////////////////////////////////////

class StreamHelper
{
    typedef unsigned int uint;
    typedef unsigned short ushort;

public:

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>   Exception for signalling that reading failed. </summary>
    ///
    /// <remarks>   Steffen 5, 5/30/2012. </remarks>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    class ReadFailedException : public std::runtime_error
    {
    public:
        ReadFailedException() : std::runtime_error("Couldn't read from stream") {}
    };

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>   Constructor. </summary>
    ///
    /// <remarks>   Steffen 5, 5/30/2012. </remarks>
    ///
    /// <param name="stream">   [in,out] The stream. </param>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    StreamHelper(std::istream& stream)
        : m_stream(stream), m_currentOffset(0)
    {
        auto oldOffset = m_stream.tellg();
        // Get stream size
        m_stream.seekg(0, std::ios::end);
        m_streamSize = m_stream.tellg();
        m_stream.seekg(oldOffset, std::ios::beg);
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>   Queries if we can read. </summary>
    ///
    /// <remarks>   Steffen 5, 5/30/2012. </remarks>
    ///
    /// <returns>   true if we can, false if not. </returns>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    bool CanRead()
    {
        return (!m_stream.bad() && !m_stream.eof() && (m_currentOffset < m_streamSize));
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>   Seeks in the stream. </summary>
    ///
    /// <remarks>   Steffen 5, 5/30/2012. </remarks>
    ///
    /// <param name="offset">	   The offset. </param>
    /// <param name="direction">	The direction. </param>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void Seek(std::streamoff offset, std::ios::seekdir direction = std::ios::cur)
    {
        auto oldOffset = m_stream.tellg();
        m_stream.seekg(offset, direction);
        m_currentOffset += (m_stream.tellg() - oldOffset);
    }

    std::streamoff Tell() const
    {
        return m_currentOffset;
    }

    template<typename T>
    T Read()
    {
        T buffer;
        m_stream.read(reinterpret_cast<char*>(&buffer), sizeof(T));
        bool failed = m_stream.fail();
        m_currentOffset += !failed ? sizeof(T) : 0;
        if (failed) {
            throw ReadFailedException();
        }

        return buffer;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>   Reads from the stream from the current position. </summary>
    ///
    /// <remarks>   Steffen 5, 5/30/2012. </remarks>
    ///
    /// <param name="buffer">   [in,out] The buffer. </param>
    /// <param name="size">	 The size. </param>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    bool Read(char* buffer, std::streamsize size)
    {
        m_stream.read(buffer, size);
        bool failed = m_stream.fail();
        m_currentOffset += !failed ? size : 0;
        return !failed;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>   Reads a string. </summary>
    ///
    /// <remarks>   Steffen 5, 5/30/2012. </remarks>
    ///
    /// <exception cref="ReadFailedException">
    ///	 Thrown when a read failed error condition occurs.
    /// </exception>
    ///
    /// <returns>   The string. </returns>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    std::string ReadString()
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

    std::string ReadString(ushort length)
    {
        std::string val;
        val.resize(length);
        if (!Read(&val[0], length * sizeof(char)))
        {
            throw ReadFailedException();
        }

        return val;
    }

    std::wstring ReadStringUtf8()
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

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>   Reads a wide string. </summary>
    ///
    /// <remarks>   Steffen 5, 5/30/2012. </remarks>
    ///
    /// <exception cref="ReadFailedException">
    ///	 Thrown when a read failed error condition occurs.
    /// </exception>
    ///
    /// <returns>   The string. </returns>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    std::wstring ReadWString()
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

    std::wstring ReadWString(ushort length)
    {
        std::wstring val;
        val.resize(length);
        if (!Read(reinterpret_cast<char*>(&val[0]), length * sizeof(wchar_t)))
        {
            throw ReadFailedException();
        }

        return val;
    }

private:
    std::istream& m_stream;
    std::streamoff m_currentOffset;
    std::streamsize m_streamSize;
};

}}} // namespace
#endif // guard

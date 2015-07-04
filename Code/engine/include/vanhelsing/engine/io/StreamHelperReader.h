#pragma once

#include <common/exceptions/IoError.h>

#include <iostream>
#include <stdexcept>

namespace vanhelsing { namespace engine { namespace io {

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>   Stream helper class for basic things like reading from a stream. </summary>
///
/// <remarks>   Steffen 5, 5/30/2012. </remarks>
////////////////////////////////////////////////////////////////////////////////////////////////////

class StreamHelperReader
{
    typedef unsigned int uint;
    typedef unsigned short ushort;

public:
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>   Exception for signalling that reading failed. </summary>
    ///
    /// <remarks>   Steffen 5, 5/30/2012. </remarks>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    class ReadFailedException : public common::IoError
    {
    public:
        ReadFailedException() : IoError("Couldn't read from stream") {}
    };

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>   Constructor. </summary>
    ///
    /// <remarks>   Steffen 5, 5/30/2012. </remarks>
    ///
    /// <param name="stream">   [in,out] The stream. </param>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    StreamHelperReader(std::istream& stream);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>   Queries if we can read. </summary>
    ///
    /// <remarks>   Steffen 5, 5/30/2012. </remarks>
    ///
    /// <returns>   true if we can, false if not. </returns>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    bool CanRead();

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>   Seeks in the stream. </summary>
    ///
    /// <remarks>   Steffen 5, 5/30/2012. </remarks>
    ///
    /// <param name="offset">	   The offset. </param>
    /// <param name="direction">	The direction. </param>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void SeekI(std::streamoff offset, std::ios::seekdir direction = std::ios::cur);

    std::streamoff TellI() const
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

    template<typename T>
    void Read(T& buffer)
    {
        buffer = Read<T>();
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>   Reads from the stream from the current position. </summary>
    ///
    /// <remarks>   Steffen 5, 5/30/2012. </remarks>
    ///
    /// <param name="buffer">   [in,out] The buffer. </param>
    /// <param name="size">	 The size. </param>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    bool Read(char* buffer, std::streamsize size);

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

    std::string ReadString();

    std::string ReadString(ushort length);

    std::wstring ReadStringUtf8();

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

    std::wstring ReadWString();

    std::wstring ReadWString(ushort length);

private:
    std::istream& m_stream;
    std::streamoff m_currentOffset;
    std::streamsize m_streamSize;
};

}}}

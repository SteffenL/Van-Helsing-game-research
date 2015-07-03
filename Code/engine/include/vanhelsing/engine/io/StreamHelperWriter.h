#pragma once

#include <common/exceptions/IoError.h>

#include <iostream>
#include <stdexcept>

namespace vanhelsing { namespace engine { namespace io {

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>   Stream helper class for basic things like writing to a stream. </summary>
///
/// <remarks>   Steffen 5, 5/30/2012. </remarks>
////////////////////////////////////////////////////////////////////////////////////////////////////

class StreamHelperWriter
{
    typedef unsigned int uint;
    typedef unsigned short ushort;

public:

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>   Exception for signalling that writing failed. </summary>
    ///
    /// <remarks>   Steffen 5, 5/30/2012. </remarks>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    class WriteFailedException : public common::IoError
    {
    public:
        WriteFailedException() : IoError("Couldn't write to stream") {}
    };

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>   Constructor. </summary>
    ///
    /// <remarks>   Steffen 5, 5/30/2012. </remarks>
    ///
    /// <param name="stream">   [in,out] The stream. </param>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    StreamHelperWriter(std::ostream& stream);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>   Queries if we can write. </summary>
    ///
    /// <remarks>   Steffen 5, 5/30/2012. </remarks>
    ///
    /// <returns>   true if we can, false if not. </returns>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    bool CanWrite();

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>   Seeks in the stream. </summary>
    ///
    /// <remarks>   Steffen 5, 5/30/2012. </remarks>
    ///
    /// <param name="offset">	   The offset. </param>
    /// <param name="direction">	The direction. </param>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void SeekO(std::streamoff offset, std::ios::seekdir direction = std::ios::cur);

    std::streamoff TellO() const;

    template<typename T>
    void Write(T val)
    {
        m_stream.write(reinterpret_cast<const char*>(&val), sizeof(T));
        bool failed = m_stream.fail();
        m_currentOffset += !failed ? sizeof(T) : 0;
        if (failed) {
            throw WriteFailedException();
        }
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>   Writes from the stream from the current position. </summary>
    ///
    /// <remarks>   Steffen 5, 5/30/2012. </remarks>
    ///
    /// <param name="buffer">   [in,out] The buffer. </param>
    /// <param name="size">	 The size. </param>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    bool Write(const char* buffer, std::streamsize size);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>   Writes a string. </summary>
    ///
    /// <remarks>   Steffen 5, 5/30/2012. </remarks>
    ///
    /// <exception cref="WriteFailedException">
    ///	 Thrown when a write failed error condition occurs.
    /// </exception>
    ///
    /// <returns>   The string. </returns>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void WriteString(const std::string& val);

    void WriteString(const std::string& val, ushort length);

    void WriteStringUtf8(const std::wstring& val);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>   Writes a wide string. </summary>
    ///
    /// <remarks>   Steffen 5, 5/30/2012. </remarks>
    ///
    /// <exception cref="WriteFailedException">
    ///	 Thrown when a write failed error condition occurs.
    /// </exception>
    ///
    /// <returns>   The string. </returns>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void WriteWString(const std::wstring& val);

    void WriteWString(const std::wstring& val, ushort length);

private:
    std::ostream& m_stream;
    std::streamoff m_currentOffset;
    std::streamsize m_streamSize;
};

}}}

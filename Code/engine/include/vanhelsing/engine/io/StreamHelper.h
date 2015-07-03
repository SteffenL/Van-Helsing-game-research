#pragma once

#include <vanhelsing/engine/io/StreamHelperReader.h>
#include <vanhelsing/engine/io/StreamHelperWriter.h>
#include <iostream>
#include <nowide/convert.hpp>
#include <memory>
#include <sstream>
#include <streambuf>

namespace vanhelsing { namespace engine { namespace io {

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>   Stream helper class for basic things like reading from a stream. </summary>
///
/// <remarks>   Steffen 5, 5/30/2012. </remarks>
////////////////////////////////////////////////////////////////////////////////////////////////////

class StreamHelper : public StreamHelperReader, public StreamHelperWriter
{
    typedef unsigned int uint;
    typedef unsigned short ushort;

    class DummyStreamBuf : public std::streambuf
    {
    public:
        DummyStreamBuf();
        virtual int sync();
    };

public:
    StreamHelper(std::istream& inStream, std::ostream& outStream)
        : m_dummyInStream(&m_dummyStreamBuf),
        m_dummyOutStream(&m_dummyStreamBuf),
        StreamHelperReader(inStream),
        StreamHelperWriter(outStream) {}

    StreamHelper(std::istream& inStream)
        : m_dummyInStream(&m_dummyStreamBuf),
        m_dummyOutStream(&m_dummyStreamBuf),
        StreamHelperReader(inStream),
        StreamHelperWriter(m_dummyOutStream) {}

    StreamHelper(std::ostream& outStream)
        : m_dummyInStream(&m_dummyStreamBuf),
        m_dummyOutStream(&m_dummyStreamBuf),
        StreamHelperReader(m_dummyInStream),
        StreamHelperWriter(outStream) {}

private:
    DummyStreamBuf m_dummyStreamBuf;
    std::istream m_dummyInStream;
    std::ostream m_dummyOutStream;
};

}}}

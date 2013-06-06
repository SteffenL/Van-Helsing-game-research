#include <vanhelsing/engine/io/GameSaveContainerWriter.h>
#include <vanhelsing/engine/io/StreamHelper.h>

namespace vanhelsing { namespace engine { namespace io {

GameSaveContainerWriter::GameSaveContainerWriter(std::istream& inStream, std::ostream& outStream)
    : m_inStream(inStream), m_outStream(outStream)
{
    StreamHelper stream(getInStream(), getOutStream());
    stream.WriteString(stream.ReadString(5), 5);
    stream.Write<unsigned int>(stream.Read<unsigned int>());
    stream.Write<int>(stream.Read<int>());
    stream.Write<char>(stream.Read<char>());
}

GameSaveContainerWriter::~GameSaveContainerWriter() {}

std::istream& GameSaveContainerWriter::getInStream() const
{
    return m_inStream;
}

std::ostream& GameSaveContainerWriter::getOutStream()
{
    return m_outStream;
}

}}} // namespace

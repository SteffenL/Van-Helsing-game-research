#include <vanhelsing/engine/io/GameSaveContainerWriter.h>
#include <vanhelsing/engine/io/StreamHelperWriter.h>

namespace vanhelsing { namespace engine { namespace io {

GameSaveContainerWriter::GameSaveContainerWriter(const ContainerInfoType& containerInfo, std::ostream& outStream)
    : m_containerInfo(containerInfo), m_outStream(outStream)
{
    StreamHelperWriter stream(getOutStream());
    stream.WriteString(m_containerInfo.Signature, 5);
    stream.Write(m_containerInfo.Version);
    stream.Write(m_containerInfo.Unknown.v1);
    stream.Write(m_containerInfo.Unknown.v2);
}

GameSaveContainerWriter::~GameSaveContainerWriter() {}

std::ostream& GameSaveContainerWriter::getOutStream()
{
    return m_outStream;
}

}}}

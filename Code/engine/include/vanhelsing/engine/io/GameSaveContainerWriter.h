#ifndef vanhelsing_engine_io_GameSaveContainerWriter_
#define vanhelsing_engine_io_GameSaveContainerWriter_

#include <vanhelsing/engine/io/GameSaveContainerBase.h>
#include <iostream>

namespace vanhelsing { namespace engine { namespace io {

class GameSaveContainerWriter : public GameSaveContainerBase
{
public:
    GameSaveContainerWriter(const ContainerInfoType& containerInfo, std::ostream& outStream);
    virtual ~GameSaveContainerWriter();
    std::ostream& getOutStream();

protected:
    const ContainerInfoType& m_containerInfo;

private:
    std::ostream& m_outStream;
};

}}} // namespace
#endif // guard

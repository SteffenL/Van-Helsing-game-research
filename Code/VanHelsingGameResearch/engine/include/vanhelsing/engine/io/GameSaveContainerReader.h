#ifndef vanhelsing_engine_io_GameSaveContainerReader_
#define vanhelsing_engine_io_GameSaveContainerReader_

#include <vanhelsing/engine/io/GameSaveContainerBase.h>
#include <iostream>

namespace vanhelsing { namespace engine { namespace io {

class GameSaveContainerReader : public GameSaveContainerBase
{
public:
    GameSaveContainerReader(ContainerInfoType& containerInfo, std::istream& inStream);
    virtual ~GameSaveContainerReader();
    std::istream& getInStream() const;

protected:
    ContainerInfoType& m_containerInfo;

private:
    std::istream& m_inStream;
};

}}} // namespace
#endif // guard

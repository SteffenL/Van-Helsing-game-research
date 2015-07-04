#pragma once

#include <vanhelsing/engine/io/GameSaveContainerBase.h>
#include <iostream>

namespace vanhelsing { namespace engine { namespace io {

class GameSaveContainerReader : public GameSaveContainerBase
{
public:
    const unsigned int MinimumSupportedVersion = 950;

    GameSaveContainerReader(ContainerInfoType& containerInfo, std::istream& inStream);
    virtual ~GameSaveContainerReader();
    std::istream& getInStream() const;

protected:
    ContainerInfoType& m_containerInfo;

private:
    std::istream& m_inStream;
    void checkVersion();
};

}}}

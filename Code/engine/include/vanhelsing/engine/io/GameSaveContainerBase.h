#pragma once

#include <iostream>

namespace vanhelsing { namespace engine { namespace io {

class GameSaveContainerBase
{
public:
    struct ContainerInfoType
    {
        std::string Signature;
        unsigned int Version;
        struct  
        {
            int v1;
            char v2;
        } Unknown;
    };
    
    //GameSaveContainerBase(ContainerInfoType& containerInfo) : m_containerInfo(containerInfo) {}
    virtual ~GameSaveContainerBase() {}

protected:
    //ContainerInfoType m_containerInfo;
};

}}}

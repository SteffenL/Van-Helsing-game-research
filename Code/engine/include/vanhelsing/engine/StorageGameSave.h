#pragma once

#include <vanhelsing/engine/inventory.h>
#include <vanhelsing/engine/io/GameSaveContainerBase.h>

namespace vanhelsing { namespace engine {

class StorageGameSave
{
public:
    inventory::ArtifactBagCollection& GetArtifactBags1();
    inventory::ArtifactBagCollection& GetArtifactBags2();
    virtual ~StorageGameSave();

    io::GameSaveContainerBase::ContainerInfoType ContainerInfo;

    struct 
    {
        struct
        {
            int v1;
            int v2;
        } Artifacts;
    } Unknown;

private:
    inventory::ArtifactBagCollection m_artifactBags1;
    inventory::ArtifactBagCollection m_artifactBags2;
};

}}

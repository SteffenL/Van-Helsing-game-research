#ifndef vanhelsing_engine_StorageGameSave_
#define vanhelsing_engine_StorageGameSave_

#include <vanhelsing/engine/inventory.h>
#include <vanhelsing/engine/io/GameSaveContainerBase.h>

namespace vanhelsing { namespace engine {

class StorageGameSave
{
public:
    inventory::Item::List& GetArtifacts1();
    inventory::Item::List& GetArtifacts2();
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
    inventory::Item::List m_artifacts1;
    inventory::Item::List m_artifacts2;
};

}} // namespace
#endif // guard

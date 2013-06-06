#ifndef vanhelsing_engine_StorageGameSave_
#define vanhelsing_engine_StorageGameSave_

#include <vanhelsing/engine/inventory.h>
#include <vanhelsing/engine/io/GameSaveContainerBase.h>

namespace vanhelsing { namespace engine {

class StorageGameSave
{
public:
    inventory::Item::List& GetItems();
    virtual ~StorageGameSave();

    io::GameSaveContainerBase::ContainerInfoType ContainerInfo;

private:
    inventory::Item::List m_items;
};

}} // namespace
#endif // guard

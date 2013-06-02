#ifndef __vanhelsing_engine_StorageGameSave__
#define __vanhelsing_engine_StorageGameSave__

#include <vanhelsing/engine/GameSave.h>

namespace vanhelsing { namespace engine {

class StorageGameSave
{
public:
    inventory::Artifact::List& GetItems() { return m_items; }
    virtual ~StorageGameSave();

protected:

private:
    inventory::Artifact::List m_items;
};

}} // namespace
#endif // guard

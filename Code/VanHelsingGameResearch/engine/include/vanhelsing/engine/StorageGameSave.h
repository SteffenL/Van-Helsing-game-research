#ifndef vanhelsing_engine_StorageGameSave_
#define vanhelsing_engine_StorageGameSave_

#include <vanhelsing/engine/GameSave.h>

namespace vanhelsing { namespace engine {

class StorageGameSave
{
public:
    inventory::Item::List& GetItems() { return m_items; }
    virtual ~StorageGameSave();

protected:

private:
    inventory::Item::List m_items;
};

}} // namespace
#endif // guard

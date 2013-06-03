#ifndef vanhelsing_engine_GameSave_
#define vanhelsing_engine_GameSave_

#include <vanhelsing/engine/inventory.h>

namespace vanhelsing { namespace engine {

class GameSave
{
public:
    //inventory::Manager<inventory::Artifact>& GetEquippedItems() { return m_equippedItems; }
    virtual ~GameSave();

protected:

private:
    //inventory::Manager<inventory::Artifact> m_equippedItems;
};

}} // namespace
#endif // guard

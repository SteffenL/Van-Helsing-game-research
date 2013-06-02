#ifndef __vanhelsing_engine_GameSave__
#define __vanhelsing_engine_GameSave__

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

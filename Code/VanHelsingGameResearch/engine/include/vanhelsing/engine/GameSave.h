#ifndef __vanhelsing_engine_GameSave__
#define __vanhelsing_engine_GameSave__

#include <vanhelsing/engine/inventory.h>

namespace vanhelsing { namespace engine {

class GameSave
{
public:
    inventory::Manager& GetInventoryManager();

protected:

private:
    inventory::Manager m_inventoryManager;
};

}} // namespace
#endif // guard

#include <vanhelsing/engine/GameSave.h>

namespace vanhelsing { namespace engine {

inventory::Manager& GameSave::GetInventoryManager()
{
    return m_inventoryManager;
}

}} // namespace

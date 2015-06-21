#include <vanhelsing/engine/StorageGameSave.h>

namespace vanhelsing { namespace engine {

StorageGameSave::~StorageGameSave() {}

inventory::Item::List& StorageGameSave::GetItems() { return m_items; }

}} // namespace

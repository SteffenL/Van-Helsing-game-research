#include <vanhelsing/engine/StorageGameSave.h>

namespace vanhelsing { namespace engine {

StorageGameSave::~StorageGameSave() {}

inventory::Item::List& StorageGameSave::GetArtifacts1() { return m_artifacts1; }
inventory::Item::List& StorageGameSave::GetArtifacts2() { return m_artifacts2; }

}} // namespace

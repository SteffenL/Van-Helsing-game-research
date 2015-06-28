#include <vanhelsing/engine/StorageGameSave.h>

namespace vanhelsing { namespace engine {

StorageGameSave::~StorageGameSave() {}

inventory::Artifact::List& StorageGameSave::GetArtifacts1() { return m_artifacts1; }
inventory::Artifact::List& StorageGameSave::GetArtifacts2() { return m_artifacts2; }

}} // namespace

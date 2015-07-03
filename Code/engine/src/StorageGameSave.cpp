#include <vanhelsing/engine/StorageGameSave.h>

namespace vanhelsing { namespace engine {

StorageGameSave::~StorageGameSave() {}

inventory::ArtifactBagCollection& StorageGameSave::GetArtifactBags1() { return m_artifactBags1; }
inventory::ArtifactBagCollection& StorageGameSave::GetArtifactBags2() { return m_artifactBags2; }

}}

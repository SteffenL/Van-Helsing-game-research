#include <vanhelsing/services/OpenedStorageGameSaveFile.h>


namespace vanhelsing { namespace services {

OpenedStorageGameSaveFile::OpenedStorageGameSaveFile()
{
}

OpenedStorageGameSaveFile::OpenedStorageGameSaveFile(const std::string& filePath, std::shared_ptr<vanhelsing::engine::StorageGameSave> gameSave)
    : m_filePath(filePath), m_gameSave(gameSave)
{
}

std::string OpenedStorageGameSaveFile::GetFilePath() const
{
    return m_filePath;
}

std::shared_ptr<vanhelsing::engine::StorageGameSave> OpenedStorageGameSaveFile::GetGameSave()
{
    return m_gameSave;
}

}}

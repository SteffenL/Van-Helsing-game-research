#pragma once

#include <vanhelsing/engine/StorageGameSave.h>

#include <string>
#include <memory>


namespace vanhelsing { namespace services {

class OpenedStorageGameSaveFile
{
public:
    OpenedStorageGameSaveFile();
    OpenedStorageGameSaveFile(const std::string& filePath, std::shared_ptr<vanhelsing::engine::StorageGameSave> gameSave);
    std::string GetFilePath() const;
    std::shared_ptr<vanhelsing::engine::StorageGameSave> GetGameSave();

private:
    std::string m_filePath;
    std::shared_ptr<vanhelsing::engine::StorageGameSave> m_gameSave;
};

}}

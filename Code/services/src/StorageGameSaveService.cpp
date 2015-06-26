#include <vanhelsing/services/StorageGameSaveService.h>
#include <vanhelsing/engine/io/StorageGameSaveReader.h>
#include <vanhelsing/engine/io/StorageGameSaveWriter.h>
#include <vanhelsing/engine/StorageGameSave.h>

#include <fstream>


namespace vanhelsing { namespace services {

StorageGameSaveService::StorageGameSaveService()
{
}


StorageGameSaveService::~StorageGameSaveService()
{
}

void StorageGameSaveService::OpenFile(const std::string& filePath)
{
    using namespace vanhelsing::engine;

    std::ifstream inStream(filePath, std::ios::binary);
    if (!inStream.is_open()) {
        throw std::runtime_error("Failed to open file: " + filePath);
    }

    auto gameSave = std::make_shared<StorageGameSave>();
    io::StorageGameSaveReader reader(*gameSave, inStream);
    m_openedFile = OpenedStorageGameSaveFile(filePath, gameSave);

    OnOpened(m_openedFile);
}

void StorageGameSaveService::SaveFile()
{
    using namespace vanhelsing::engine;

    if (!HasOpenedFile()) {
        throw std::logic_error("Cannot save when a file was not opened");
    }

    const auto& filePath = m_openedFile.GetFilePath();
    std::ofstream outStream(filePath, std::ios::binary);
    if (!outStream.is_open()) {
        throw std::runtime_error("Failed to open file: " + filePath);
    }

    auto gameSave = m_openedFile.GetGameSave();
    io::StorageGameSaveWriter writer(*gameSave, outStream);
}

bool StorageGameSaveService::HasOpenedFile() const
{
    return !m_openedFile.GetFilePath().empty();
}

}}

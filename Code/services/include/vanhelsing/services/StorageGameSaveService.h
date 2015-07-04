#pragma once

#include "OpenedStorageGameSaveFile.h"

#include <boost/signals2.hpp>
#include <memory>


namespace vanhelsing { namespace services {

class StorageGameSaveService
{
public:
    StorageGameSaveService();
    virtual ~StorageGameSaveService();
    void OpenFile(const std::string& filePath);
    void SaveFile();
    bool HasOpenedFile() const;

    boost::signals2::signal<void(OpenedStorageGameSaveFile& fileInfo)> OnOpened;

private:
    OpenedStorageGameSaveFile m_openedFile;
};

}}

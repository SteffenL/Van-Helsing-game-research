#pragma once

#include <vanhelsing/services/StorageGameSaveService.h>
#include <vanhelsing/services/GameDataService.h>

#include <memory>


class ApplicationServices
{
public:
    ApplicationServices();
    virtual ~ApplicationServices();
    static ApplicationServices& Get();

    static std::unique_ptr<ApplicationServices> m_instance;
    vanhelsing::services::StorageGameSaveService StorageGameSave;
    vanhelsing::services::GameDataService GameData;
};

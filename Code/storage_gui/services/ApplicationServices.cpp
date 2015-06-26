#include "ApplicationServices.h"


ApplicationServices::ApplicationServices()
{
}

ApplicationServices::~ApplicationServices()
{
}

ApplicationServices& ApplicationServices::Get()
{
    if (!m_instance) {
        m_instance = std::make_unique<ApplicationServices>();
    }

    return *m_instance;
}

std::unique_ptr<ApplicationServices> ApplicationServices::m_instance;

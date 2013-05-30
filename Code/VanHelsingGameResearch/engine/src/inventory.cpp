#include <vanhelsing/engine/inventory.h>

namespace vanhelsing { namespace engine { namespace inventory {

void Manager::Add(Artifact* artifact)
{

}


unsigned int Artifact::GetIdFromName(const std::string& name)
{
    unsigned int id = 0;
    for (auto ch : name) {
        id *= 0x01003f;
        id += ch;
    }

    return id;
}

std::string Artifact::GetNameFromId(IdType id)
{
    return std::string();
}

std::string Artifact::GetName() const
{
    return GetNameFromId(Id);
}

}}} // namespace

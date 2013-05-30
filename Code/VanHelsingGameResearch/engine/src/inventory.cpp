#include <vanhelsing/engine/inventory.h>

namespace vanhelsing { namespace engine { namespace inventory {

void Manager::Add(Item* item)
{

}


unsigned int Item::GetIdFromName(const std::string& name)
{
    unsigned int id = 0;
    for (auto ch : name) {
        id *= 0x01003f;
        id += ch;
    }

    return id;
}

std::string Item::GetNameFromId(IdType id)
{
    return std::string();
}

std::string Item::GetName() const
{
    return GetNameFromId(Id);
}

}}} // namespace

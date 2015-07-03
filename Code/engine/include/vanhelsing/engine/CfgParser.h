#pragma once

#include <string>
#include <map>
#include <vector>
#include <iosfwd>

namespace vanhelsing { namespace engine {
    
class CfgParser
{
public:
    typedef std::map<std::string, std::string> GroupType;

    CfgParser(std::istream& stream);
    void Parse();
    const std::vector<GroupType>& GetGroups() const;

private:
    bool readGroup(GroupType& group);
    bool readGroupName(std::string& name);
    void skipWhitespace();

private:
    std::istream& m_stream;
    std::vector<GroupType> m_groups;
};

}}

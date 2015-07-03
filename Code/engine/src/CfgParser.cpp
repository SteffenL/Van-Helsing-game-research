#include <vanhelsing/engine/CfgParser.h>
#include <common/Log.h>
#include <vanhelsing/engine/exceptions/VanHelsingEngineError.h>

#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <sstream>

namespace vanhelsing { namespace engine {

CfgParser::CfgParser(std::istream& stream) : m_stream(stream) {}

void CfgParser::Parse()
{
    bool isInGroup = false;
    std::string groupName;
    std::string line;
    bool expectsGroup = false;
    char lastCh = 0;

    do {
        char ch = m_stream.rdbuf()->sgetc();
        switch (ch) {
        case '{':
            {
                expectsGroup = false;
                GroupType group;
                if (readGroup(group)) {
                    m_groups.push_back(group);
                    break;
                }

                break;
            }

        default:
            {
                //if ((ch == '#') || ((ch == '/') && (lastCh == '/'))) {
                if ((ch == '#') || (ch == '/')) {
                    // Skip comment
                    std::string comment;
                    std::getline(m_stream, comment);
                    continue;
                }

                if (!expectsGroup) {
                    groupName.clear();
                    if (readGroupName(groupName)) {
                        expectsGroup = true;
                        continue;
                    }
                }

                common::Log(common::LogLevel::Error) << "Parse error around offset " << m_stream.tellg() << std::endl;
                throw VanHelsingEngineError("Parse error");
            }
        }

        lastCh = ch;
    } while (m_stream.rdbuf()->sgetc() != EOF);
}

void CfgParser::skipWhitespace()
{
    char ch = m_stream.rdbuf()->sgetc();
    do {
        if ((ch == ' ') || (ch == '\t') || (ch == '\r') || (ch == '\n')) {
            ch = m_stream.rdbuf()->snextc();
            continue;
        }

        break;
    } while (m_stream.rdbuf()->sgetc() != EOF);
}

bool CfgParser::readGroup(CfgParser::GroupType& group)
{
    std::stringstream groupBody;
    char ch = m_stream.rdbuf()->snextc();
    int groupsToSkip = 0;
    do {
        // Don't yet know how to handle sub-groups, so skip them
        if (ch == '{') {
            ++groupsToSkip;
        }

        if (groupsToSkip > 0) {
            while (ch = m_stream.rdbuf()->snextc() != '}') {}
            ch = m_stream.rdbuf()->snextc();
            --groupsToSkip;
            continue;
        }

        if (ch != '}') {
            groupBody << ch;
            ch = m_stream.rdbuf()->snextc();
            continue;
        }

        m_stream.rdbuf()->snextc();
        
        if (groupsToSkip <= 0) {
            break;
        }
    } while (m_stream.rdbuf()->sgetc() != EOF);

    // Parse body

    std::string line;
    std::vector<std::string> tokens;
    while (std::getline(groupBody, line)) {
        boost::algorithm::trim(line);
        boost::algorithm::split(tokens, line, boost::is_any_of("="));

        if (tokens.size() < 2) {
            continue;
        }

        auto& name = tokens[0];
        boost::algorithm::trim(name);

        if (name.empty()) {
            continue;
        }

        auto& value = tokens[1];
        boost::algorithm::trim(value);

        group[name] = value;
    }

    // If there's no name in the group, skip it
    if (group.find("Name") == group.end()) {
        return false;
    }

    return true;
}

bool CfgParser::readGroupName(std::string& name)
{
    skipWhitespace();
    char ch = m_stream.rdbuf()->sgetc();
    char lastCh = 0;
    do {
        if ((ch == '#') || (ch == '/')) {
            // Skip comment
            std::string comment;
            std::getline(m_stream, comment);
            skipWhitespace();
            lastCh = ch;
            ch = m_stream.rdbuf()->sgetc();
            continue;
        }

        if (((ch >= '0') && (ch <= '9')) || ((ch >= 'a') && (ch <= 'z')) || ((ch >= 'A') && (ch <= 'Z'))) {
            name += ch;
            lastCh = ch;
            ch = m_stream.rdbuf()->snextc();
            continue;
        }

        break;
    } while (m_stream.rdbuf()->sgetc() != EOF);

    skipWhitespace();
    return true;
}

const std::vector<CfgParser::GroupType>& CfgParser::GetGroups() const
{
    return m_groups;
}

}}

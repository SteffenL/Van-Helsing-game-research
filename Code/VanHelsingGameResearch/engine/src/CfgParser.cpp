#include <vanhelsing/engine/CfgParser.h>
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

    do {
        char ch = m_stream.rdbuf()->sgetc();
        switch (ch) {
        case '{':
            {
                if (groupName != "Artifact") {
                    m_stream.rdbuf()->snextc();
                    continue;
                }

                GroupType group;
                if (readGroup(group)) {
                    m_groups.push_back(group);
                    break;
                }

                throw std::runtime_error("Parse error");
                break;
            }

        default:
            {
                groupName.clear();
                if (readGroupName(groupName)) {
                    continue;
                }

                throw std::runtime_error("Parse error");
            }
        }

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
    do {
        if (ch != '}') {
            groupBody << ch;
            ch = m_stream.rdbuf()->snextc();
            continue;
        }

        m_stream.rdbuf()->snextc();
        break;
    } while (m_stream.rdbuf()->sgetc() != EOF);

    // Parse body

    std::string line;
    std::vector<std::string> tokens;
    while (groupBody >> line) {
        boost::algorithm::trim(line);
        boost::algorithm::split(tokens, line, boost::is_any_of("="));
        auto& name = tokens[0];
        boost::algorithm::trim(name);
        auto& value = tokens[1];
        boost::algorithm::trim(value);

        if (name.empty()) {
            continue;
        }

        group[name] = value;
    }

    return true;
}

bool CfgParser::readGroupName(std::string& name)
{
    skipWhitespace();
    char ch = m_stream.rdbuf()->sgetc();
    do {
        if (((ch >= '0') && (ch <= '9')) || ((ch >= 'a') && (ch <= 'z')) || ((ch >= 'A') && (ch <= 'Z'))) {
            name += ch;
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

}} // namespace

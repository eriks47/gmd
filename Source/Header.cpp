#include <cassert>
#include <regex>
#include <iostream>
#include <string>

#include "Header.hpp"
#include "Utillities.hpp"

// Matches method declaration for a class
const std::regex REGEX_DECLARATION(R"(^[\w:<>\s\[\]]*\s*[\w~&*:<>|=+-/~!%^,\[\]\(\)]+\([^{}]*\)\s*[\w\s>\-&:\*]*$)");
const std::regex REGEX_WHITESPACE(R"(^\s*[[:blank:]]*)");
const std::regex REGEX_LINE_COMMENTS(R"(//.*\n)");
const std::regex REGEX_ACCESS_MODIFIERS(R"(^[\n\s}{]*(private|protected|public):)");
const std::regex REGEX_CLASS_NAME(R"(class \w*(\n|\s|/|\{|/))");
const std::regex REGEX_NAMESPACE_NAME(R"(namespace \w*(\n|\s|/|\{|/))");

Header::Header(const std::filesystem::path &path)
    : m_Path(path), m_Filename(path.filename())
{
    assert(std::filesystem::exists(m_Path));

    // NOTE(erik): This shouldn't fail
    Utillities::ReadFileToString(m_Path, m_Contents);
}

const std::vector<std::string> &Header::GetDeclarations()
{
    if (m_Declarations.empty())
    {
        ExtractDeclarations();
    }
    return m_Declarations;
}

const std::string &Header::GetClassName()
{
    if (m_ClassName.empty())
    {
        ExtractClassName();
    }
    return m_ClassName;
}

void Header::ExtractDeclarations()
{
    if (!m_Declarations.empty())
    {
        m_Declarations.clear();
    }

    std::stringstream contentsStream(m_Contents);
    for (std::string line; std::getline(contentsStream, line, '{'); )
    {
        if (line.contains(m_ClassName))
        {
            break;
        }
    }

    for (std::string line; std::getline(contentsStream, line, ';'); )
    {
        Utillities::RegexRemove(REGEX_LINE_COMMENTS, line);
        Utillities::RegexRemove(REGEX_ACCESS_MODIFIERS, line);
        Utillities::RegexRemove(REGEX_WHITESPACE, line);
        if (std::regex_match(line, REGEX_DECLARATION))
        {
            m_Declarations.push_back(line);
        }
    }
}

void Header::ExtractClassName()
{
    if (!m_ClassName.empty())
    {
        m_ClassName.clear();
    }

    std::smatch matchInfo;
    std::regex_search(m_Contents, matchInfo, REGEX_CLASS_NAME);
    if (matchInfo.empty())
    {
        std::regex_search(m_Contents, matchInfo, REGEX_NAMESPACE_NAME);
        if (matchInfo.empty())
        {
            std::cout << "Error: no class name found\n";
        }
    }

    std::string matched = matchInfo.str();

    // The regex always matches one extra character at the end
    matched.pop_back();

    // Because substr() range is inclusive, we need to skip the delimiting space
    m_ClassName = matched.substr(matched.find_last_of(' ') + 1);
}

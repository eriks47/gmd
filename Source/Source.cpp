#include <cassert>
#include <filesystem>
#include <fstream>
#include <iostream>

#include "Source.hpp"
#include "Utillities.hpp"
#include "Config.hpp"

const std::regex REGEX_METHOD_NAME(R"END([\w~:]+[\(\[<>\+\-\*/%\^=&\|~!,])END");
const std::regex REGEX_HEADER_ONLY_KEYWORDS(R"((static|override|explicit|virtual)\s*)");
const std::regex REGEX_DEFAULT_PARAM_VALUES(R"(\s*=\s*.*(,|"))");
const std::regex REGEX_IMPLEMENTATION(R"(^[\w:<>\s\[\]]*\s*[\w~&*:]+\([^{}]*\)\s*[\w\s>\-]*$)");

Source::Source(const std::filesystem::path &path, const std::string &className)
    : m_Path(path), m_Filename(path.filename()), m_ClassName(className)
{
    std::cout << "Creating source object\n";
    Utillities::ReadFileToString(m_Path, m_Contents);
    std::cout << "Contents: " << m_Contents << "\n";
}

void Source::ImplementMethods(const std::vector<std::string> &declarations)
{
    ExtractImplementations();

    if (m_FileStream.is_open())
    {
        m_FileStream.flush();
        m_FileStream.close();
    }
    m_FileStream = std::ofstream(m_Path, std::ios::app);

    std::cout << "FOUND: \n";
    for (const auto &found : m_Implementations)
    {
        std::cout << found << "\n";
    }
    std::cout << "ENDFOUND\n";
    for (const auto &declaration : declarations)
    {
        std::string implementation = DeclarationToImplementation(declaration);
        std::cout << implementation << "\n";
        if (m_Implementations.contains(implementation))
        {
            std::cout << "Contains\n";
            continue;
        }

        m_FileStream << implementation << bracePattern;
    }
    m_FileStream.flush();
    Utillities::ReadFileToString(m_Path, m_Contents);
}

void Source::InsertInclude(const std::filesystem::path &headerPath)
{
    if (!m_FileStream.is_open())
    {
        m_FileStream.open(m_Path, std::ios_base::app);
    }

    if (!HasInclude(headerPath))
    {
        m_FileStream << "#include " << headerPath << "\n\n";
        m_FileStream.flush();
        Utillities::ReadFileToString(m_Path, m_Contents);
    }
}

void Source::ExtractImplementations()
{
    std::stringstream contnentsStream(m_Contents);
    for (std::string line; std::getline(contnentsStream, line); )
    {
        std::regex REGEX_REMOVE_NAMESPACE(m_ClassName + "::");
        if (std::regex_match(line, REGEX_IMPLEMENTATION))
        {
            std::cout << line << "\n";
            m_Implementations.insert(line);
        }
    }
}

// IMPORTANT: Returns method implementation HEADERS with a semicolon in the end;
std::string Source::DeclarationToImplementation(const std::string &declaration)
{
    std::string prefix = m_ClassName + "::";
    std::smatch match;
    std::regex_search(declaration, match, REGEX_METHOD_NAME);
    std::string newName = prefix + match.str();

    std::string implementation = std::regex_replace(declaration, REGEX_METHOD_NAME, newName);

    Utillities::RegexRemove(REGEX_HEADER_ONLY_KEYWORDS, implementation);
    Utillities::RegexRemove(REGEX_DEFAULT_PARAM_VALUES, implementation);

    return implementation;
}

bool Source::HasInclude(const std::filesystem::path &includePath)
{
    std::string s(R"(#include ")" + includePath.string() + '"');
    std::regex REGEX_INCLUDE(s);
    return std::regex_search(m_Contents, REGEX_INCLUDE);
}

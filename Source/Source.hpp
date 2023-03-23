#pragma once

#include <string>
#include <filesystem>
#include <unordered_set>
#include <vector>
#include <fstream>

class Source
{
private:
    std::filesystem::path m_Path;
    std::filesystem::path m_Filename;
    std::string m_Contents;
    std::unordered_set<std::string> m_Implementations;
    std::string m_ClassName;
    std::ofstream m_FileStream;

public:
    Source(const std::filesystem::path &path, const std::string &className);
    ~Source() = default;

    void ImplementMethods(const std::vector<std::string> &declarations);
    void InsertInclude(const std::filesystem::path &headerPath);

private:
    std::string DeclarationToImplementation(const std::string &declaration);
    bool HasInclude(const std::filesystem::path &includePath);
    void ExtractImplementations();
};

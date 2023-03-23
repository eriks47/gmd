#pragma once

#include <filesystem>
#include <string>
#include <vector>

class Header
{
private:
    std::filesystem::path m_Path;
    std::filesystem::path m_Filename;
    std::string m_Contents;
    std::vector<std::string> m_Declarations;
    std::string m_ClassName;

public:
    explicit Header(const std::filesystem::path &path);
    ~Header() = default;

    [[nodiscard]] const std::vector<std::string> &GetDeclarations();
    [[nodiscard]] const std::string &GetClassName();

private:
    void ExtractDeclarations();
    void ExtractClassName();
};

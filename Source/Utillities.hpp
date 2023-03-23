#pragma once

#include <filesystem>
#include <regex>
#include <string>

namespace Utillities
{
    void ReadFileToString(const std::filesystem::path &path, std::string &buffer);
    void RegexRemove(const std::regex &pattern, std::string &string);
};

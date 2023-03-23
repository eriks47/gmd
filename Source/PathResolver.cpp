#include <unordered_set>
#include <unordered_map>
#include <string>

#include "PathResolver.hpp"

// Extensions are taken from https://en.wikipedia.org/wiki/C%2B%2B
const std::unordered_set<std::string> sourceExtensions = {
    ".cpp",
    ".cxx",
    ".cc",
    ".c++",
    ".C"
};
const std::unordered_set<std::string> headerExtensions = {
    ".h",
    ".H",
    ".hh",
    ".hpp",
    ".hxx",
    ".h++"
};
const std::unordered_map<std::string, std::string> extensionIndex = {
    {".hpp", ".cpp"},
    {".hxx", ".cxx"},
    {".hh", ".cc"},
    {".h++", ".c++"},
    {".H", ".C"},
};

PathResolver::PathResolver(const char *first)
    : firstPath(first)
{

}

PathResolver::PathResolver(const char *first, const char *second)
    : firstPath(first), secondPath(second)
{

}

[[nodiscard]] FilePathPair PathResolver::GetPaths()
{
    FilePathPair pathPair;

    if (secondPath.empty())
    {
        if (IsSource(firstPath))
        {
            pathPair.sourcePath = firstPath;
            pathPair.headerPath = SourceToHeader(firstPath);
        }
        else
        {
            pathPair.headerPath = firstPath;
            pathPair.sourcePath = HeaderToSource(firstPath);
        }
    }
    else
    {
        if (IsSource(firstPath))
        {
            pathPair.sourcePath = firstPath;
            pathPair.headerPath = secondPath;
        }
        else
        {
            pathPair.headerPath = firstPath;
            pathPair.sourcePath = secondPath;
        }
    }

    return pathPair;
}

[[nodiscard]] bool PathResolver::IsSource(const std::filesystem::path &file)
{
    return sourceExtensions.contains(file.extension().string());
}

[[nodiscard]] bool PathResolver::IsHeader(const std::filesystem::path &file)
{
    return headerExtensions.contains(file.extension().string());
}

std::filesystem::path PathResolver::SourceToHeader(const std::filesystem::path &file) const
{
    // TODO(erik): Handle different extensions properly (map them)
    std::filesystem::path headerPath = file;
    std::unordered_map<std::string, std::string> reverseIndex;
    for (const auto &[key, value] : extensionIndex)
    {
        reverseIndex[value] = key;
    }
    headerPath.replace_extension(reverseIndex.at(file.extension().string()));
    return headerPath;
    return file;
}

std::filesystem::path PathResolver::HeaderToSource(const std::filesystem::path &file) const
{
    std::filesystem::path sourcePath = file;
    sourcePath.replace_extension(extensionIndex.at(file.extension().string()));
    return sourcePath;
}


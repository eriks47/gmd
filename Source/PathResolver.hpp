#pragma once

#include <filesystem>

struct FilePathPair
{
    std::filesystem::path headerPath; 
    std::filesystem::path sourcePath;
    bool wasProvidedWithSource;
};

class PathResolver
{
private:
    std::filesystem::path firstPath;
    std::filesystem::path secondPath;

public:
    explicit PathResolver(const char *first);
    PathResolver(const char *first, const char *second);
    [[nodiscard]] FilePathPair GetPaths();

private:
    std::filesystem::path SourceToHeader(const std::filesystem::path &file) const;
    std::filesystem::path HeaderToSource(const std::filesystem::path &file) const;
    [[nodiscard]] bool IsSource(const std::filesystem::path &file);
    [[nodiscard]] bool IsHeader(const std::filesystem::path &file);
};

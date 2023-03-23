#include <fstream>
#include <filesystem>
#include <iostream>
#include <vector>
#include <string>
#include <cassert>
#include <regex>

#include "Header.hpp"
#include "Source.hpp"
#include "Config.hpp"
#include "PathResolver.hpp"

constexpr bool DEBUG = false;

using namespace std::filesystem;

void reportError(std::string_view message)
{
    std::cerr << "gmd: " << message << "\n";
    std::cerr << "gmd: [filename] <filename>\n";
    std::exit(1);
}

int main(int argc, char *argv[])
{
    if (argc != 2 && argc != 3)
    {
        reportError("improper usage");
    }

    FilePathPair pathPair;
    if (argc == 2)
    {
        PathResolver pathResolver(argv[1]);
        pathPair = pathResolver.GetPaths();
    }
    else
    {
        PathResolver pathResolver(argv[1], argv[2]);
        pathPair = pathResolver.GetPaths();
    }

    auto [headerPath, sourcePath] = pathPair;

    assert(exists(headerPath));

    Header header(headerPath);
    auto declarations = header.GetDeclarations();
    auto className = header.GetClassName();

    Source source(sourcePath, className);
    source.InsertInclude(headerPath);
    source.ImplementMethods(declarations);
}

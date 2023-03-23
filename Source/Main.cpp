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

    path headerPath;
    path sourcePath;
    bool didProvideSource = true;
    if (argc == 2)
    {
        path argument(argv[1]);
        if (argument.extension() == ".cpp")
        {
            sourcePath = argument;
            headerPath = argument.replace_extension(".hpp");
        }
        else if (argument.extension() == ".hpp")
        {
            headerPath = argument;
            sourcePath = argument.replace_extension(".cpp");
            didProvideSource = false;
        }
    }
    else
    {
        // Both filenames are given
        path firstArgument(argv[1]);
        path secondArgument(argv[2]);
        if (sourceExtensions.contains(firstArgument.extension()))
        {
            sourcePath = std::move(firstArgument);
            headerPath = std::move(secondArgument);
        }
        else
        {
            sourcePath = std::move(secondArgument);
            headerPath = std::move(firstArgument);
        }
    }

    assert(exists(headerPath));

    Header header(headerPath);
    auto declarations = header.GetDeclarations();
    auto className = header.GetClassName();

    Source source(sourcePath, className);
    source.InsertInclude(headerPath);
    source.ImplementMethods(declarations);
}

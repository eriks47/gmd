#pragma once

#include <string>
#include <unordered_set>

const std::unordered_set<std::string> sourceExtensions = {
    "cpp",
    "cxx",
    "cc",
    "c++"
};

const std::string bracePattern = "\n{\n\n}\n\n";

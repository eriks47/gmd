#include <cstdio>
#include <array>

#include "Util.hpp"

std::string Util::UniqueFilename()
{
    std::array<char, L_tmpnam> buff;
    std::tmpnam(buff.data());
    std::string string(buff.data());
    return string;
}

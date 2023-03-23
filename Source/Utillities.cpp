#include <fstream>
#include <filesystem>

#include "Utillities.hpp"

void Utillities::ReadFileToString(const std::filesystem::path &path, std::string &buffer)
{
    if (!std::filesystem::exists(path))
    {
        buffer = "";
        return;
    }

    // TODO(erik): Replace with a more efficent implementation
    std::ifstream fileStream(path);
    std::noskipws(fileStream);
    char character;
    while (!fileStream.eof())
    {
        fileStream >> character;
        buffer.push_back(character);
    }
    buffer.pop_back(); // Delete last, duplicate character
}

void Utillities::RegexRemove(const std::regex &pattern, std::string &string)
{
    // TODO(erik): Handle multiple remove properly
    string = std::regex_replace(string, pattern, "");
    string = std::regex_replace(string, pattern, "");
    string = std::regex_replace(string, pattern, "");
    string = std::regex_replace(string, pattern, "");
    string = std::regex_replace(string, pattern, "");
}

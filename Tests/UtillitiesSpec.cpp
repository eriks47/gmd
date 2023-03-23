#include <fstream>
#include <string>

#include "../Source/Utillities.hpp"
#include "Util.hpp"
#include "gtest/gtest.h"

TEST(UtilTest, ProperlyReadsFile)
{
    std::string fileContents = "Mock contents\n  Hello world!\n";
    std::string path = Util::UniqueFilename();
    std::ofstream fileStream(path);
    fileStream << fileContents;
    fileStream.close();

    std::string buffer;
    Utillities::ReadFileToString(path, buffer);

    EXPECT_EQ(fileContents, buffer);
}

TEST(UtilTest, ReturnsEmptyWhenFileIsNonexistant)
{
    std::string path = Util::UniqueFilename();
    std::string buffer;
    Utillities::ReadFileToString(path, buffer);

    EXPECT_EQ(buffer, "");
}

TEST(UtilTest, ProperlyRemoves)
{
    std::string email = "walter.baumann@hulaloop.neverforget";
    std::regex domainName(R"(@\w+.\w+)");
    Utillities::RegexRemove(domainName, email);

    EXPECT_EQ(email, "walter.baumann");
}

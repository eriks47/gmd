#include <fstream>

#include "../Source/Source.hpp"
#include "../Source/Config.hpp"
#include "../Source/Utillities.hpp"
#include "Util.hpp"
#include "gtest/gtest.h"

TEST(SourceTest, InsertsInclude)
{
    std::string sourcePath = Util::UniqueFilename();
    std::string headerPath = "Window.h";

    Source source(sourcePath, "Window");
    source.InsertInclude(headerPath);

    std::string contents;
    Utillities::ReadFileToString(sourcePath, contents);

    EXPECT_TRUE(contents.contains(R"(#include "Window.h")"));
}

TEST(SourceTest, ImplementsMethod)
{
    std::string sourcePath = "Window.cpp";
    Source source(sourcePath, "Window");
    std::vector<std::string> declarations = {"[[nodiscard]] int GetWidth() const"};

    source.ImplementMethods(declarations);

    EXPECT_EQ(source.GetContents(), "[[nodiscard]] int Window::GetWidth() const" + bracePattern);
    std::filesystem::remove(sourcePath);
}

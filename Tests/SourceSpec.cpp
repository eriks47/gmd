#include <fstream>

#include "../Source/Source.hpp"
#include "../Source/Config.hpp"
#include "../Source/Utillities.hpp"
#include "Util.hpp"
#include "gtest/gtest.h"

Source CreateSourceWithContents(const std::string &contents, const std::string &className)
{
    std::string path = className + ".cpp";
    std::ofstream fileStream(path);
    fileStream << contents;
    fileStream.close();

    Source source(path, className);
    return std::move(source);
}

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

TEST(SourceTest, DoesntImplementMethodIfAlreadyImplemented)
{
    std::string contents =
        "[[nodiscard]] int Window::GetWidth() const\n"
        "{\n"
        "    return m_Width;\n"
        "}\n";
    Source source = CreateSourceWithContents(contents, "Window");
    source.ImplementMethods({"[[nodiscard]] int GetWidth() const"});

    EXPECT_EQ(source.GetContents(), contents);
    std::filesystem::remove("Window.hpp");
}

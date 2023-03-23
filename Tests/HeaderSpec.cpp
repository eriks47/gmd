#include <fstream>

#include "../Source/Header.hpp"
#include "Util.hpp"
#include "gtest/gtest.h"

Header CreateHeaderWithContents(const std::string &contents)
{
    std::string path = Util::UniqueFilename();
    std::ofstream fileStream(path);
    fileStream << contents;
    fileStream.close();

    Header header(path);
    return header;
}


TEST(HeaderTest, DetectsClassName)
{
    std::string contents = "#include <fstream>\n\nclass FileWrapper_t { }\n";
    Header header = CreateHeaderWithContents(contents);
    EXPECT_EQ(header.GetClassName(), "FileWrapper_t");
}

TEST(HeaderTest, ExtractsDeclaration)
{
    std::string contents = 
        "#include <ftsream>\n\n"
        "class FileWrapper\n"
        "{\n"
        "    int GetError();\n"
        "}\n";
    Header header = CreateHeaderWithContents(contents);
    const auto declarations = header.GetDeclarations();

    ASSERT_EQ(declarations.size(), 1);
    EXPECT_EQ(declarations[0], "int GetError()");
}

TEST(HeaderTest, ExtractsDeclarationWithAccessModifiers)
{
    std::string contents = 
        "#include <ftsream>\n\n"
        "class FileWrapper\n"
        "{\n"
        "public:\n"
        "private:\n"
        "public: int GetError();\n"
        "public:\n"
        "}\n";
    Header header = CreateHeaderWithContents(contents);
    const auto declarations = header.GetDeclarations();

    ASSERT_EQ(declarations.size(), 1);
    EXPECT_EQ(declarations[0], "int GetError()");
}

TEST(HeaderTest, ExtractsDeclarationWithAttribute)
{
    std::string contents = 
        "#include <ftsream>\n\n"
        "class FileWrapper\n"
        "{\n"
        "    [[nodiscard]] const std::string &GetContents() const;\n"
        "}\n";
    Header header = CreateHeaderWithContents(contents);
    const auto declarations = header.GetDeclarations();

    ASSERT_EQ(declarations.size(), 1);
    EXPECT_EQ(declarations[0], "[[nodiscard]] const std::string &GetContents() const");
}

TEST(HeaderTest, ExtractsDeclarationWithTrailingReturnType)
{
    std::string contents = 
        "#include <ftsream>\n\n"
        "class FileWrapper\n"
        "{\n"
        "    [[nodiscard]] auto GetContents() const -> const std::string&;\n"
        "}\n";
    Header header = CreateHeaderWithContents(contents);
    const auto declarations = header.GetDeclarations();

    ASSERT_EQ(declarations.size(), 1);
    EXPECT_EQ(declarations[0], "[[nodiscard]] auto GetContents() const -> const std::string&");
}

TEST(HeaderTest, ExtractsDeclarationWithParameters)
{
    std::string contents = 
        "#include <ftsream>\n\n"
        "class FileWrapper\n"
        "{\n"
        "    [[nodsicard]] auto Contains(const std::string &substring) const -> bool;\n"
        "}\n";
    Header header = CreateHeaderWithContents(contents);
    const auto declarations = header.GetDeclarations();

    ASSERT_EQ(declarations.size(), 1);
    EXPECT_EQ(declarations[0], "[[nodsicard]] auto Contains(const std::string &substring) const -> bool");
}

TEST(HeaderTest, ExtractsDeclarationWithDefaultParameterValue)
{
    std::string contents = 
        "#include <ftsream>\n\n"
        "class FileWrapper\n"
        "{\n"
        "    unsigned int set_type(int i,const std::string &substring = \"C++ Source File\");\n"
        "}\n";
    Header header = CreateHeaderWithContents(contents);
    const auto declarations = header.GetDeclarations();

    ASSERT_EQ(declarations.size(), 1);
    EXPECT_EQ(declarations[0], "unsigned int set_type(int i,const std::string &substring = \"C++ Source File\")");
}

TEST(HeaderTest, ExtractsDeclarationWithTemplate)
{
    std::string contents = 
        "#include <ftsream>\n\n"
        "class FileStream : MemoryStream\n"
        "{\n"
        "    template<typename T>\n"
        "    void Write(T data) override;"
        "}\n";
    Header header = CreateHeaderWithContents(contents);
    const auto declarations = header.GetDeclarations();

    ASSERT_EQ(declarations.size(), 1);
    EXPECT_EQ(declarations[0], "template<typename T>\n    void Write(T data) override");
}

TEST(HeaderTest, DoesntExtractDefault)
{
    std::string contents = 
        "#include <ftsream>\n\n"
        "class FileWrapper\n"
        "{\n"
        "    FileWrapper() = default;\n"
        "    ~FileWrapper() = default;\n"
        "}\n";
    Header header = CreateHeaderWithContents(contents);
    const auto declarations = header.GetDeclarations();

    EXPECT_EQ(declarations.size(), 0);
}

TEST(HeaderTest, DoesntExtractDeleted)
{
    std::string contents = 
        "#include <ftsream>\n\n"
        "class FileWrapper\n"
        "{\n"
        "    FileWrapper() = delete;\n"
        "    ~FileWrapper() = delete;\n"
        "}\n";
    Header header = CreateHeaderWithContents(contents);
    const auto declarations = header.GetDeclarations();

    EXPECT_EQ(declarations.size(), 0);
}

TEST(HeaderTest, DoesntExtractDefinition)
{
    std::string contents = 
        "#include <ftsream>\n\n"
        "class FileWrapper\n"
        "{\n"
        "    [[nodiscard]] auto GetContents() const -> const std::string & { return m_Contents; };"
        "}\n";
    Header header = CreateHeaderWithContents(contents);
    const auto declarations = header.GetDeclarations();

    EXPECT_EQ(declarations.size(), 0);
}

TEST(HeaderTest, ExtractsConstructor)
{
    std::string contents = 
        "#include <ftsream>\n\n"
        "class FileWrapper\n"
        "{\n"
        "    explicit FileWrapper(std::filesystem::path path);\n"
        "}\n";
    Header header = CreateHeaderWithContents(contents);
    const auto declarations = header.GetDeclarations();

    ASSERT_EQ(declarations.size(), 1);
    EXPECT_EQ(declarations[0], "explicit FileWrapper(std::filesystem::path path)");
}

TEST(HeaderTest, ExtractsDestructor)
{
    std::string contents = 
        "#include <ftsream>\n\n"
        "class FileWrapper\n"
        "{\n"
        "    ~FileWrapper();\n"
        "}\n";
    Header header = CreateHeaderWithContents(contents);
    const auto declarations = header.GetDeclarations();

    ASSERT_EQ(declarations.size(), 1);
    EXPECT_EQ(declarations[0], "~FileWrapper()");
}

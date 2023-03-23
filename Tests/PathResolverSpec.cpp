#include "../Source/PathResolver.hpp"

#include "gtest/gtest.h"

TEST(PathResolverTest, ResolveWhenBothPathsGiven)
{
    const char *argument1 = "Example.hpp";
    const char *argument2 = "Example.cpp";
    PathResolver pathReoslver(argument1, argument2);

    const auto [headerPath, sourcePath] = pathReoslver.GetPaths();
    EXPECT_EQ(argument1, headerPath);
    EXPECT_EQ(argument2, sourcePath);
}

TEST(PathResolverTest, ResolveWhenBothPathsGivenWithUnusualExtensions)
{
    const char *argument1 = "Example.h++";
    const char *argument2 = "Example.c++";
    PathResolver pathReoslver(argument1, argument2);

    const auto [headerPath, sourcePath] = pathReoslver.GetPaths();
    EXPECT_EQ(argument1, headerPath);
    EXPECT_EQ(argument2, sourcePath);
}

TEST(PathResolverTest, MapsExtensionWhenGivenOnlyHeader)
{
    const char *argument1 = "Example.H";
    PathResolver pathReoslver(argument1);

    const auto [headerPath, sourcePath] = pathReoslver.GetPaths();
    EXPECT_EQ(sourcePath, "Example.C");
}

TEST(PathResolverTest, MapsExtensionWhenGivenOnlySource)
{
    const char *argument1 = "Example.cc";
    PathResolver pathReoslver(argument1);

    const auto [headerPath, sourcePath] = pathReoslver.GetPaths();
    EXPECT_EQ(headerPath, "Example.hh");
}

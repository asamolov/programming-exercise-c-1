//
// Created by Alexander Samolov on 20/02/2017.
//

#include "normalize.h"
#include <gtest/gtest.h>

// Simple sanity check
TEST(NormalizePath, BasicPath) {
    const char *input = "/a/b/c";
    const char *expected = "/a/b/c";
    char *output = normalize(input);

    ASSERT_STREQ(expected, output);
}

TEST(NormalizePath, EmptyPath) {
    const char *input = "/";
    const char *expected = "/";
    char *output = normalize(input);
    ASSERT_STREQ(expected, output);
}

TEST(NormalizePath, RelPath) {
    const char *input = "/abc/../def";
    const char *expected = "/def";
    char *output = normalize(input);
    ASSERT_STREQ(expected, output);
}

struct _test_data {
    const char *input;
    const char *output;
};

TEST(NormalizePath, MultiPath) {
    struct _test_data data[] = {
            {"..",              "/"},
            {"../..",           "/"},
            {"../.",            "/"},
            {"/././././.",      "/"},
            {"/abc/",           "/abc/"},
            {"/abc",            "/abc"},
            {"/.abc/def",       "/.abc/def"},
            {"/.abc/..def",     "/.abc/..def"},
            {"/abc/.",          "/abc/"},
            {"/abc/./",         "/abc/"},
            {"/abc/./def",      "/abc/def"},
            {"/abc/./def/..",   "/abc/"},
            {"/abc/./def/../",  "/abc/"},
            {"../bar",          "/bar"},
            {"/foo/bar",        "/foo/bar"},
            {"/foo/bar/../baz", "/foo/baz"},
            {"/foo/bar/./baz/", "/foo/bar/baz/"},
            {"/foo/../../baz",  "/baz"}
    };
    for (int i = 0; i < (sizeof(data) / sizeof(data[0])); i++) {
        const char *input = data[i].input;
        const char *expected = data[i].output;
        char *output = normalize(input);
        EXPECT_STREQ(expected, output);
    }
}

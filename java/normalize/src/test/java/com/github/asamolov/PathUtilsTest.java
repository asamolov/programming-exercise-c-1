package com.github.asamolov;

import org.junit.Test;

import static org.junit.Assert.*;

/**
 * Created by alexandersamolov on 23/02/2017.
 */
public class PathUtilsTest {
    static String data[][] = {
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

    @Test
    public void simple() throws Exception {
        String input = "/a/b/c";
        String expected = "/a/b/c";

        assertEquals(expected, PathUtils.normalize(input));
    }
    @Test
    public void up() throws Exception {
        String input = "/a/../c";
        String expected = "/c";

        assertEquals(expected, PathUtils.normalize(input));
    }
    @Test
    public void curr() throws Exception {
        String input = "/a/./c";
        String expected = "/a/c";

        assertEquals(expected, PathUtils.normalize(input));
    }
    @Test
    public void multi() throws Exception {
        for (String[] d : data) {
            String input = d[0];
            String expected = d[1];

            assertEquals(expected, PathUtils.normalize(input));
        }
    }
    @Test
    public void multi_buf() throws Exception {
        StringBuilder sb = new StringBuilder(500);
        for (String[] d : data) {
            String input = d[0];
            String expected = d[1];

            PathUtils.normalize(input, sb);
            assertEquals(expected, sb.toString());
            sb.setLength(0);
        }
    }
}
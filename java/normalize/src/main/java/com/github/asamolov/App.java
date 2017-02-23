package com.github.asamolov;

/**
 * Hello world!
 *
 */
public class App 
{
    public static void main(String[] args)
    {
        String path = "/a/b/c";
        System.out.printf("Normalized path: %s\n", PathUtils.normalize(path));
    }
}

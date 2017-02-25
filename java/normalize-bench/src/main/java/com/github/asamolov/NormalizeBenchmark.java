package com.github.asamolov;

import org.openjdk.jmh.annotations.Benchmark;
import org.openjdk.jmh.annotations.Scope;
import org.openjdk.jmh.annotations.Setup;
import org.openjdk.jmh.annotations.State;

@State(Scope.Thread)
public class NormalizeBenchmark {

    private static final String PATH = "/hello";

    private StringBuilder sb;

    @Setup
    public void setup() {
        sb = new StringBuilder(PATH.length());
    }

    @Benchmark
    public void nop() {
    }

    @Benchmark
    public String baseline() {
        return baseline(PATH);
    }

    @Benchmark
    public String simple() {
        return PathUtils.normalize(PATH);
    }

    @Benchmark
    public void simple_buf() {
        sb.setLength(0); // buffer reset
        PathUtils.normalize(PATH, sb);
    }

    public static String baseline(String path) {
        StringBuilder sb = new StringBuilder(path.length());
        sb.append(path);
        return sb.toString();
    }

}

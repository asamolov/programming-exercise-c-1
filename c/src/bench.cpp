//
// Created by Alexander Samolov on 20/02/2017.
//

#include "normalize.h"

#include <string.h>
#include <benchmark/benchmark.h>


struct _test_data {
    const char *input;
    const char *output;
};

static struct _test_data data[] = {
        {"..",             "/"},
        {"../..",          "/"},
        {"../.",           "/"},
        {"/././././.",     "/"},
        {"/abc/",          "/abc/"},
        {"/abc",           "/abc"},
        {"/abc/.",         "/abc/"},
        {"/abc/./def",     "/abc/def"},
        {"/abc/./def/..",  "/abc/"},
        {"/abc/./def/../", "/abc/"}
};

static void BM_Normalize(benchmark::State& state) {
    std::string x = "hello";
    while (state.KeepRunning()) {
        char *s = normalize(x.c_str());
        free(s);
    }
}

BENCHMARK(BM_Normalize);

static void BM_Normalize_Buf_Multiple(benchmark::State &state) {
    char buf[255];

    while (state.KeepRunning()) {
        for (int i = 0; i < (sizeof(data) / sizeof(data[0])); i++) {
            const char *input = data[i].input;
            normalize_path_buf(input, buf);
        }
    }
    state.SetItemsProcessed(state.iterations() * sizeof(data)/sizeof(data[0]));
}

BENCHMARK(BM_Normalize_Buf_Multiple);

static void BM_Normalize_Buf_Single(benchmark::State& state) {
    char buf[255];

    while (state.KeepRunning()) {
        normalize_path_buf("/hello", buf);
    }
    state.SetItemsProcessed(state.iterations());
}

BENCHMARK(BM_Normalize_Buf_Single);

static void BM_Normalize_Buf_N_Tokens(benchmark::State &state) {

    const char *p = "/hello";
    size_t len = strlen(p);
    int n = state.range(0);

    char *src = (char *) malloc(n * len + 1);
    char *buf = (char *) malloc(n * len + 1);

    for (int i = 0; i < n; i++) {
        memcpy(src + i*len, p, len);
    }
    src[n*len + 1] = '\0';

    while (state.KeepRunning()) {
        normalize_path_buf(src, buf);
    }

    free(src);
    free(buf);
    state.SetItemsProcessed(state.iterations()*n);
    state.SetBytesProcessed(state.iterations()*(n*len + 1));
}

BENCHMARK(BM_Normalize_Buf_N_Tokens)->DenseRange(1, 101, 10);

static void BM_Normalize_Buf_Dir_Size(benchmark::State &state) {

    size_t token_size = (size_t) state.range(0);
    size_t len = token_size + 1;
    int n = 5; // 5 tokens

    char *src = (char *) malloc(n * len + 1);
    char *buf = (char *) malloc(n * len + 1);

    for (int i = 0; i < n; i++) {
        src[i*len] = '/';
        memset(src + i*len + 1, 'a', token_size);
    }
    src[n*len + 1] = '\0';

    while (state.KeepRunning()) {
        normalize_path_buf(src, buf);
    }

    free(src);
    free(buf);
    state.SetItemsProcessed(state.iterations()*n);
    state.SetBytesProcessed(state.iterations()*(n*len + 1));
}

BENCHMARK(BM_Normalize_Buf_Dir_Size)->DenseRange(1, 51, 5);

BENCHMARK_MAIN();
//
// Created by Alexander Samolov on 20/02/2017.
//

#ifndef NORMALIZE_PATH_NORMALIZE_H
#define NORMALIZE_PATH_NORMALIZE_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

char *normalize(const char *path);
size_t normalize_path_buf(const char *path, char *dst);

#ifdef __cplusplus
}
#endif

#endif //NORMALIZE_PATH_NORMALIZE_H

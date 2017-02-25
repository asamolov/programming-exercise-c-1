//
// Created by Alexander Samolov on 20/02/2017.
//

#include "normalize.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

typedef enum _Token {
    TokUp = 0,
    TokCurr,
    TokSep,
    TokDir,
    TokEnd
} Token;

static char *_tok_str[] = { "TokUp", "TokCurr", "TokSep", "TokDir", "TokEnd"};

void _print_tok(Token tok, const char *p, size_t sz) {
    char buf[80];
    if (tok == TokDir) {
        size_t n = sizeof(buf) - 1 < sz ? sizeof(buf) - 1 : sz;
        memcpy(buf, p, n);
        buf[n] = '\0';
    } else {
        buf[0] = '\0';
    }
    printf("%7s: %2zu: %s\n", _tok_str[tok], sz, buf);
}

/*
 * Tokenize provided string.
 *
 * arguments:
 * IN start -> start position
 * OUT advance -> size of token. Add advance to start to get next start pointer
 *
 * returns:
 * One of tokens, see Token enum.
 */
Token tokenize(const char *start, size_t *advance) {
    Token token;
    const char *pos = start;

    // looking for specific case
    char c;
    switch (pos[0]) {
        case '\0':
            *advance = 0;
            token = TokEnd;
            break;
        case '/':
            *advance = 1;
            token = TokSep;
            break;
        case '.':
            c = pos[1];
            // ./ or .
            if (c == '/' || c == '\0') {
                *advance = 1;
                token = TokCurr;
                break;
            }
            // ../ or ..
            if (c == '.' &&
                    (pos[2] == '/' || pos[2] == '\0')) {
                *advance = 2;
                token = TokUp;
                break;
            }
            // .abc or ..abc
            token = TokDir;
            break;
        default:
            token = TokDir;
    }

    if (token != TokDir) {
        return token;
    }

    // find next separator
    while (*pos != '/' && *pos != '\0') {
        pos++;
    }

    *advance = pos - start;
    return token;
}

/*
 * Normalizing consist of two parts:
 * 1. Tokenizing of an input path
 * 2. Applying tokens to an output buffer.
 *
 * 1. Tokenizing, performed in tokenize function
 * There are 4 kind of tokens:
 * TokUp -> '..' -> go 1 dir up
 * TokCurr -> '.' -> current dir
 * TokSep -> '/' -> dir separator
 * TokDir -> <dir name> -> regular dir name
 * TokEnd -> end of path, everything is processed
 *
 * All tokens, except TokDir, have no payload. Payload of TokDir is a name of directory.
 *
 * 2. Applying tokens can be performed in several ways.
 * This method performs most naive way.
 * ROOT -> Root of the buffer, /.
 * Curr -> ptr to current symbol
 * Up(Curr) -> go from Curr to Root side, until '/' or ROOT is met.
 *
 * TokUp ->  Up(Curr).
 * TokCurr -> No change
 * TokSep -> If *Curr != /, add / and shift Curr.
 * TokDir -> copy dir to buffer, shift Curr.
 * TokEnd -> finish
 */
size_t normalize_path_buf(const char *path, char *dst) {
    Token token;
    const char *p = path;
    size_t tok_size = 0;
    char *curr = dst;
    *curr = '/';
    do {
        token = tokenize(p, &tok_size);

        switch (token) {
            case TokUp:
                assert(*curr == '/');
                if (curr == dst) {
                    // already at root
                    break;
                }
                // go up one dir. shall we also cache it?
                do  {
                    curr--;
                } while (*curr != '/');
                break;
            case TokCurr:
                // no change
                break;
            case TokSep:
                if (*curr != '/') {
                    curr++;
                    *curr = '/';
                }
                break;
            case TokDir:
                memcpy(curr + 1, p, tok_size);
                curr += tok_size;
                /* inline memcpy. On my machine (MB Air 2013) works better for small Dirs (5 chars).
                 * But memcpy works better for large dirs (10+ chars)
                curr++;
                for (int i = 0; i < tok_size; i++) {
                    curr[i] = p[i];
                }
                curr += tok_size - 1;
                 */
                assert(*curr != '/');
                break;
            case TokEnd:break;
        }
#ifdef _DEBUG
        _print_tok(token, p, tok_size);
        *(curr + 1) = '\0';
        printf("\t%s\n", dst);
#endif
        p += tok_size;
    } while (token != TokEnd);

    curr++;
    *curr = '\0';

    return curr - dst;
}

char *normalize(const char *path) {
    size_t n = strlen(path);
    char *result = malloc(n + 2);
    size_t sz = normalize_path_buf(path, result);
    assert(sz == strlen(result));
    return result;
}

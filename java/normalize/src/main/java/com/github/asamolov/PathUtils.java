package com.github.asamolov;


/**
 * Created by alexandersamolov on 23/02/2017.
 */
public class PathUtils {

    private enum Token {
        TokUp,
        TokCurr,
        TokSep,
        TokDir,
        TokEnd
    }

    private static class TokenizeResult {
        final Token token;
        final int len;

        public TokenizeResult(final Token token, final int len) {
            this.token = token;
            this.len = len;
        }

        @Override
        public String toString() {
            return "" + token +
                    "{" + len + "}";
        }
    }

    private static TokenizeResult tokenize(String path, int idx) {
        Token token;
        int pos = idx;
        int len = 0;
        // looking for specific case
        if (idx == path.length()) {
            token = Token.TokEnd;
            len = 0;
            return new TokenizeResult(token, len);
        }
        switch (path.charAt(pos)) {
            case '/':
                token = Token.TokSep;
                len = 1;
                break;
            case '.':
                // ./ or .
                pos++;
                if (pos == path.length()
                        || path.charAt(pos) == '/') {
                    len = 1;
                    token = Token.TokCurr;
                    break;
                }

                // ../ or ..
                if (path.charAt(pos) == '.' &&
                        (pos + 1 == path.length() || path.charAt(pos+1) == '/')) {
                    len = 2;
                    token = Token.TokUp;
                    break;
                }
                // .abc or ..abc
                token = Token.TokDir;
                break;
            default:
                token = Token.TokDir;
        }

        if (token != Token.TokDir) {
            return new TokenizeResult(token, len);
        }

        // reset position
        pos = idx;
        // find next separator
        while (pos != path.length() && path.charAt(pos) != '/') {
            pos++;
        }
        len = pos - idx;
        return new TokenizeResult(Token.TokDir, len);
    }

    public static String normalize(String path) {
        StringBuilder sb = new StringBuilder(path.length());
        normalize(path, sb);
        return sb.toString();
    }

    public static void normalize(String path, StringBuilder sb) {
        TokenizeResult token;
        int idx = 0;

        sb.append('/');
        do {
            token = tokenize(path, idx);

            switch (token.token) {
                case TokUp:
                    assert(sb.charAt(sb.length() - 1) == '/');
                    if (sb.length() == 1) {
                        // already at root
                        break;
                    }
                    // go up one dir. shall we also cache it?
                    int n = sb.lastIndexOf("/", sb.length() - 2);
                    sb.setLength(n + 1);
                    break;
                case TokCurr:
                    // no change
                    break;
                case TokSep:
                    if (sb.charAt(sb.length() - 1) != '/') {
                        sb.append('/');
                    }
                    break;
                case TokDir:
                    sb.append(path, idx, idx + token.len);
                    break;
                case TokEnd:
                    break;
            }
            //_print_tok(token, idx, sb);
            idx += token.len;
        } while (token.token != Token.TokEnd);
    }

    private static void _print_tok(final TokenizeResult token, final int idx, final StringBuilder sb) {
        System.out.printf("%s: %d: %s\n", token, idx, sb.toString());
    }
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "expressao.h"

#define MAX_TOKEN_LEN 128
#define MAX_STACK 1024

static void trim_newline(char *s) {
    size_t n = strlen(s);
    while (n > 0 && (s[n-1] == '\n' || s[n-1] == '\r')) {
        s[n-1] = '\0'; n--;
    }
}

static int is_number_token(const char *tok) {
    if (!tok || !tok[0]) return 0;
    char *p = (char*)tok;
    if ((*p == '+' || *p == '-') && p[1] != '\0') p++;
    int saw_digit = 0, saw_dot = 0;
    while (*p) {
        if (*p == ',') { p++; continue; }
        if (*p == '.') {
            if (saw_dot) return 0;
            saw_dot = 1;
        } else if (*p >= '0' && *p <= '9') {
            saw_digit = 1;
        } else return 0;
        p++;
    }
    return saw_digit;
}

static void replace_comma_with_dot(char *s) {
    while (*s) {
        if (*s == ',') *s = '.';
        s++;
    }
}

static void str_tolower_inplace(char *s) {
    for (; *s; ++s) {
        if (*s >= 'A' && *s <= 'Z') *s = *s - 'A' + 'a';
    }
}

typedef struct {
    char *items[MAX_STACK];
    int top;
} StrStack;

static void ss_init(StrStack *st) { st->top = -1; }
static int ss_empty(StrStack *st) { return st->top == -1; }
static int ss_push(StrStack *st, const char *s) {
    if (st->top >= MAX_STACK-1) return 0;
    st->top++;
    st->items[st->top] = strdup(s);
    return 1;
}
static char * ss_pop(StrStack *st) {
    if (ss_empty(st)) return NULL;
    char *r = st->items[st->top];
    st->items[st->top] = NULL;
    st->top--;
    return r;
}
static void ss_free_all(StrStack *st) {
    while (!ss_empty(st)) {
        char *p = ss_pop(st);
        free(p);
    }
}

typedef struct {
    float items[MAX_STACK];
    int top;
} FloatStack;

static void fs_init(FloatStack *st) { st->top = -1; }
static int fs_empty(FloatStack *st) { return st->top == -1; }
static int fs_push(FloatStack *st, float v) {
    if (st->top >= MAX_STACK-1) return 0;
    st->top++;
    st->items[st->top] = v;
    return 1;
}
static int fs_pop2(FloatStack *st, float *out) {
    if (fs_empty(st)) return 0;
    *out = st->items[st->top];
    st->top--;
    return 1;
}

static int is_unary_func(const char *tok) {
    if (!tok) return 0;
    return (strcmp(tok, "raiz") == 0 ||
            strcmp(tok, "sen") == 0 ||
            strcmp(tok, "cos") == 0 ||
            strcmp(tok, "tg")  == 0 ||
            strcmp(tok, "log") == 0);
}

static int is_operator(const char *tok) {
    if (!tok) return 0;
    return (strcmp(tok, "+") == 0 ||
            strcmp(tok, "-") == 0 ||
            strcmp(tok, "*") == 0 ||
            strcmp(tok, "/") == 0 ||
            strcmp(tok, "%") == 0 ||
            strcmp(tok, "^") == 0);
}

char * getFormaInFixa(char *Str) {
    if (!Str) return NULL;
    char buf[1024];
    strncpy(buf, Str, sizeof(buf)-1); buf[sizeof(buf)-1] = '\0';
    trim_newline(buf);

    StrStack st;
    ss_init(&st);

    char *saveptr = NULL;
    char *tok = strtok_r(buf, " ", &saveptr);
    while (tok) {
        char toklow[MAX_TOKEN_LEN];
        strncpy(toklow, tok, sizeof(toklow)-1); toklow[sizeof(toklow)-1]=0;
        str_tolower_inplace(toklow);

        if (is_number_token(toklow)) {
            char tmp[MAX_TOKEN_LEN];
            strncpy(tmp, toklow, sizeof(tmp)-1); tmp[sizeof(tmp)-1]=0;
            replace_comma_with_dot(tmp);
            ss_push(&st, tmp);
        } else if (is_unary_func(toklow)) {
            char *a = ss_pop(&st);
            if (!a) { ss_free_all(&st); return NULL; }
            char *newexpr = malloc(strlen(toklow) + strlen(a) + 4);
            sprintf(newexpr, "%s(%s)", toklow, a);
            free(a);
            ss_push(&st, newexpr);
            free(newexpr);
        } else if (is_operator(toklow)) {
            char *b = ss_pop(&st);
            char *a = ss_pop(&st);
            if (!a || !b) { if(a) free(a); if(b) free(b); ss_free_all(&st); return NULL; }
            size_t len = strlen(a) + strlen(b) + 6;
            char *newexpr = malloc(len);
            sprintf(newexpr, "(%s%c%s)", a, toklow[0], b);
            free(a); free(b);
            ss_push(&st, newexpr);
            free(newexpr);
        } else {
            ss_free_all(&st);
            return NULL;
        }
        tok = strtok_r(NULL, " ", &saveptr);
    }

    if (st.top != 0) {
        ss_free_all(&st);
        return NULL;
    }
    char *result = ss_pop(&st);

    if (result && result[0] == '(') {
        size_t L = strlen(result);
        if (L >= 2 && result[L-1] == ')') {
            char *noouter = malloc(L-1);
            strncpy(noouter, result+1, L-2);
            noouter[L-2] = '\0';
            free(result);
            result = noouter;
        }
    }

    char *compact = malloc(strlen(result)+1);
    char *p = compact;
    for (char *q = result; *q; ++q) {
        if (*q != ' ') *p++ = *q;
    }
    *p = '\0';
    free(result);
    return compact;
}

float getValorPosFixa(char *StrPosFixa) {
    if (!StrPosFixa) return NAN;

    char buf[1024];
    strncpy(buf, StrPosFixa, sizeof(buf)-1); buf[sizeof(buf)-1] = '\0';
    trim_newline(buf);

    FloatStack st;
    fs_init(&st);

    char *saveptr = NULL;
    char *tok = strtok_r(buf, " ", &saveptr);

    while (tok) {
        char tokcpy[MAX_TOKEN_LEN];
        strncpy(tokcpy, tok, sizeof(tokcpy)-1); tokcpy[sizeof(tokcpy)-1]=0;
        replace_comma_with_dot(tokcpy);
        str_tolower_inplace(tokcpy);

        if (is_number_token(tokcpy)) {
            char *endptr = NULL;
            float v = strtof(tokcpy, &endptr);
            if (endptr == tokcpy) return NAN;
            fs_push(&st, v);
        } else if (is_unary_func(tokcpy)) {
            float a;
            if (!fs_pop2(&st, &a)) return NAN;
            float res = NAN;
            float r = a * (float)M_PI / 180.0f;
            if (strcmp(tokcpy, "raiz") == 0) res = a < 0 ? NAN : sqrtf(a);
            else if (strcmp(tokcpy, "sen") == 0) res = sinf(r);
            else if (strcmp(tokcpy, "cos") == 0) res = cosf(r);
            else if (strcmp(tokcpy, "tg") == 0) res = tanf(r);
            else if (strcmp(tokcpy, "log") == 0) res = a <= 0 ? NAN : log10f(a);
            fs_push(&st, res);
        } else if (is_operator(tokcpy)) {
            float b, a;
            if (!fs_pop2(&st, &b)) return NAN;
            if (!fs_pop2(&st, &a)) return NAN;
            float res = NAN;

            if (strcmp(tokcpy, "+") == 0) res = a + b;
            else if (strcmp(tokcpy, "-") == 0) res = a - b;
            else if (strcmp(tokcpy, "*") == 0) res = a * b;
            else if (strcmp(tokcpy, "/") == 0) res = b == 0 ? NAN : a / b;
            else if (strcmp(tokcpy, "%") == 0) res = b == 0 ? NAN : fmodf(a, b);
            else if (strcmp(tokcpy, "^") == 0) res = powf(a, b);

            fs_push(&st, res);
        } else {
            return NAN;
        }

        tok = strtok_r(NULL, " ", &saveptr);
    }

    float final;
    if (!fs_pop2(&st, &final)) return NAN;
    if (!fs_empty(&st)) return NAN;
    return final;
}

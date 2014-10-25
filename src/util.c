#include <ctype.h>
#include <string.h>
#include <time.h>

static unsigned int m_w = 0x8eeee9fb; // must not be zero, nor 0x464fffff
static unsigned int m_z = 0x433072e9; // must not be zero, nor 0x9068ffff

unsigned int prng() {
    m_z = 36969 * (m_z & 65535) + (m_z >> 16);
    m_w = 18000 * (m_w & 65535) + (m_w >> 16);
    return (m_z << 16) + m_w;
}

void prng_seed(unsigned int seed) {
    m_w = seed;
    m_z = seed;
}

char *tokenize(char *str, const char *delim, char **key) {
    char *result;
    if (str == NULL) {
        str = *key;
    }
    str += strspn(str, delim);
    if (*str == '\0') {
        return NULL;
    }
    result = str;
    str += strcspn(str, delim);
    if (*str) {
        *str++ = '\0';
    }
    *key = str;
    return result;
}

char *strip(char *str) {
    while (isspace(*str)) {
        str++;
    }
    if (*str == 0) {
        return str;
    }
    char *end = str + strlen(str) - 1;
    while (end > str && isspace(*end)) {
        end--;
    }
    *(end + 1) = 0;
    return str;
}

int starts_with(const char *str, const char *prefix) {
    return strncmp(prefix, str, strlen(prefix)) == 0;
}

double now() {
    return (double)clock() / CLOCKS_PER_SEC;
}

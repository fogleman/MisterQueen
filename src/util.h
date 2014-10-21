#ifndef UTIL_H
#define UTIL_H

char *tokenize(char *str, const char *delim, char **key);
char *strip(char *str);
int starts_with(const char *str, const char *prefix);
double now();

#endif

#include <ctype.h>

bool isalnum(char c) {
    return isalpha(c) || isdigit(c);
}
bool isalpha(char c) {
    return islower(c) || isupper(c);
}
bool iscntrl(char c) {
    return c < 0x20 || c == 0x7f;
}
bool isdigit(char c) {
    return c >= '0' && c <= '9';
}
bool isgraph(char c) {
    return c >= 0x21 && c < 0x7f;
}
bool islower(char c) {
    return c >= 'a' && c <= 'z';
}
bool isprint(char c) {
    return c >= 0x20 && c < 0x7f;
}
bool ispunct(char c) {
    return isgraph(c) && !isalnum(c);
}
bool isspace(char c) {
    return c == ' ';
}
bool isupper(char c) {
    return c >= 'A' && c <= 'Z';
}
bool isxdigit(char c) {
    return isdigit(c) || (c >= 'A' && c <= 'F') || (c >= 'a' && c <= 'f');
}
bool tolower(char c) {
    return c + (isupper(c) ? 32 : 0);
}
bool toupper(char c) {
    return c + (islower(c) ? -32 : 0);
}
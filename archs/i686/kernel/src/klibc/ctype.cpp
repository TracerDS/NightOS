#include <klibc/cctype>

#include <cstdint>

int isalnum(int c) {
    return isalpha(c) || isdigit(c);
}
int isalpha(int c) {
    return isupper(c) || islower(c);
}
int isblank(int c) {
    auto v = static_cast<std::uint8_t>(c);
    return v == ' ' || v == '\t';
}
int iscntrl(int c) {
    auto v = static_cast<std::uint8_t>(c);
    return v <= 0x1F || v == 0x7F;
}
int isdigit(int c) {
    auto v = static_cast<std::uint8_t>(c);
    return v >= '0' && v <= '9';
}
int isgraph(int c) {
    return isalnum(c) || ispunct(c);
}
int islower(int c) {
    auto v = static_cast<std::uint8_t>(c);
    return v >= 'a' && v <= 'z';
}
int isprint(int c) {
    auto v = static_cast<std::uint8_t>(c);
    return isgraph(v) || v == ' ';
}
int ispunct(int c) {
    auto v = static_cast<std::uint8_t>(c);
    return (v >= 0x21 && v <= 0x2F) || (v >= 0x3A && v <= 0x40) ||
           (v >= 0x5B && v <= 0x60) || (v >= 0x7B && v <= 0x7E);
}
int isspace(int c) {
    auto v = static_cast<std::uint8_t>(c);
    return isblank(v) || v == '\f' || v == '\n' || v == '\r' ||
           v == '\v';
}
int isupper(int c) {
    auto v = static_cast<std::uint8_t>(c);
    return v >= 'A' && v <= 'Z';
}
int isxdigit(int c) {
    auto v = static_cast<std::uint8_t>(c);
    return isdigit(v) || (v >= 'a' && v <= 'f') || (v >= 'A' && v <= 'F');
}
int tolower(int c) {
    auto v = static_cast<std::uint8_t>(c);
    return isupper(v) ? v + 0x20 : v;
}
int toupper(int c) {
    auto v = static_cast<std::uint8_t>(c);
    return islower(v) ? v - 0x20 : v;
}
#pragma once

#include <type_traits>

namespace klibc::__unicode {
    template <typename CharT>
    consteval bool __literal_encoding_is_unicode() noexcept {
        if constexpr (
            std::is_same_v<CharT, char8_t> ||
            std::is_same_v<CharT, char16_t> ||
            std::is_same_v<CharT, char32_t>
        ) {
            return true;
        } else {    
           return false; 
        }
    }

    consteval bool __literal_encoding_is_utf8() noexcept {
        return __literal_encoding_is_unicode<char>();
    }
}
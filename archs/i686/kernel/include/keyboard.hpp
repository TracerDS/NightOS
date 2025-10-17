#pragma once

#include <cstdint>
#include <type_traits>
#include <concepts>

namespace Keyboard {
    enum class VirtualKey {
        VK_ESC          = 0x01,
        VK_1            = 0x02,
        VK_2            = 0x03,
        VK_3            = 0x04,
        VK_4            = 0x05,
        VK_5            = 0x06,
        VK_6            = 0x07,
        VK_7            = 0x08,
        VK_8            = 0x09,
        VK_9            = 0x0A,
        VK_0            = 0x0B,
        VK_MINUS        = 0x0C,
        VK_EQUALS       = 0x0D,
        VK_BACKSPACE    = 0x0E,
        VK_TAB          = 0x0F,
        VK_Q            = 0x10,
        VK_W            = 0x11,
        VK_E            = 0x12,
        VK_R            = 0x13,
        VK_T            = 0x14,
        VK_Y            = 0x15,
        VK_U            = 0x16,
        VK_I            = 0x17,
        VK_O            = 0x18,
        VK_P            = 0x19,
        VK_PAREN_LEFT   = 0x1A,
        VK_PAREN_RIGHT  = 0x1B,
        VK_ENTER        = 0x1C,
        VK_CTRL         = 0x1D,
        VK_A            = 0x1E,
        VK_S            = 0x1F,
        VK_D            = 0x20,
        VK_F            = 0x21,
        VK_G            = 0x22,
        VK_H            = 0x23,
        VK_J            = 0x24,
        VK_K            = 0x25,
        VK_L            = 0x26,
        VK_SEMI         = 0x27,
        VK_APOSTROPHE   = 0x28,
        VK_BACKTICK     = 0x29,
        VK_LSHIFT       = 0x2A,
        VK_BACKSLASH    = 0x2B,
        VK_Z            = 0x2C,
        VK_X            = 0x2D,
        VK_C            = 0x2E,
        VK_V            = 0x2F,
        VK_B            = 0x30,
        VK_N            = 0x31,
        VK_M            = 0x32,
        VK_COMMA        = 0x33,
        VK_DOT          = 0x34,
        VK_SLASH        = 0x35,
        VK_RSHIFT       = 0x36,
        VK_NUMPAD_MUL   = 0x37,
        VK_LALT         = 0x38,
        VK_SPACE        = 0x39,
        VK_CAPSLOCK     = 0x3A,
        VK_F1           = 0x3B,
        VK_F2           = 0x3C,
        VK_F3           = 0x3D,
        VK_F4           = 0x3E,
        VK_F5           = 0x3F,
        VK_F6           = 0x40,
        VK_F7           = 0x41,
        VK_F8           = 0x42,
        VK_F9           = 0x43,
        VK_F10          = 0x44,
        VK_NUMLOCK      = 0x45,
        VK_SCROLLLOCK   = 0x46,
        VK_NUMPAD7      = 0x47,
        VK_NUMPAD8      = 0x48,
        VK_NUMPAD9      = 0x49,
        VK_NUMPAD_MINUS = 0x4A,
        VK_NUMPAD4      = 0x4B,
        VK_NUMPAD5      = 0x4C,
        VK_NUMPAD6      = 0x4D,
        VK_NUMPAD_PLUS  = 0x4E,
        VK_NUMPAD1      = 0x4F,
        VK_NUMPAD2      = 0x50,
        VK_NUMPAD3      = 0x51,
        VK_NUMPAD0      = 0x52,
        VK_NUMPAD_DOT   = 0x53,
        VK_F11          = 0x57,
        VK_F12          = 0x58,
        VK_MENU         = 0x5D,

        VK_EXTENDED     = 0xE0, // Extended key prefix
        VK_UNKNOWN      = 0xFF, // Unknown key
    };

    constexpr auto VKToInt(VirtualKey key) noexcept {
        return static_cast<typename std::underlying_type<VirtualKey>::type>(key);
    }

    constexpr auto IntToVK(std::integral auto keycode) noexcept {
        if (keycode <= 0 || keycode >= 0xFF)
            return VirtualKey::VK_UNKNOWN;
            
        return static_cast<VirtualKey>(keycode);
    }

    constexpr char VKToChar(VirtualKey key) noexcept {
        switch (key) {
            case VirtualKey::VK_1:           return '1';
            case VirtualKey::VK_2:           return '2';
            case VirtualKey::VK_3:           return '3';
            case VirtualKey::VK_4:           return '4';
            case VirtualKey::VK_5:           return '5';
            case VirtualKey::VK_6:           return '6';
            case VirtualKey::VK_7:           return '7';
            case VirtualKey::VK_8:           return '8';
            case VirtualKey::VK_9:           return '9';
            case VirtualKey::VK_0:           return '0';
            case VirtualKey::VK_MINUS:       return '-';
            case VirtualKey::VK_EQUALS:      return '=';
            case VirtualKey::VK_BACKSPACE:   return '\b';
            case VirtualKey::VK_TAB:         return '\t';
            case VirtualKey::VK_Q:           return 'q';
            case VirtualKey::VK_W:           return 'w';
            case VirtualKey::VK_E:           return 'e';
            case VirtualKey::VK_R:           return 'r';
            case VirtualKey::VK_T:           return 't';
            case VirtualKey::VK_Y:           return 'y';
            case VirtualKey::VK_U:           return 'u';
            case VirtualKey::VK_I:           return 'i';
            case VirtualKey::VK_O:           return 'o';
            case VirtualKey::VK_P:           return 'p';
            case VirtualKey::VK_PAREN_LEFT:  return '[';
            case VirtualKey::VK_PAREN_RIGHT: return ']';
            case VirtualKey::VK_A:           return 'a';
            case VirtualKey::VK_S:           return 's';
            case VirtualKey::VK_D:           return 'd';
            case VirtualKey::VK_F:           return 'f';
            case VirtualKey::VK_G:           return 'g';
            case VirtualKey::VK_H:           return 'h';
            case VirtualKey::VK_J:           return 'j';
            case VirtualKey::VK_K:           return 'k';
            case VirtualKey::VK_L:           return 'l';
            case VirtualKey::VK_SEMI:        return ';';
            case VirtualKey::VK_APOSTROPHE:  return '\'';
            case VirtualKey::VK_BACKTICK:    return '`';
            case VirtualKey::VK_BACKSLASH:   return '\\';
            case VirtualKey::VK_Z:           return 'z';
            case VirtualKey::VK_X:           return 'x';
            case VirtualKey::VK_C:           return 'c';
            case VirtualKey::VK_V:           return 'v';
            case VirtualKey::VK_B:           return 'b';
            case VirtualKey::VK_N:           return 'n';
            case VirtualKey::VK_M:           return 'm';
            case VirtualKey::VK_COMMA:       return ',';
            case VirtualKey::VK_DOT:         return '.';
            case VirtualKey::VK_SLASH:       return '/';
            case VirtualKey::VK_NUMPAD_MUL:  return '*';
            case VirtualKey::VK_SPACE:       return ' ';
            case VirtualKey::VK_NUMPAD7:     return '7';
            case VirtualKey::VK_NUMPAD8:     return '8';
            case VirtualKey::VK_NUMPAD9:     return '9';
            case VirtualKey::VK_NUMPAD_MINUS:return '-';
            case VirtualKey::VK_NUMPAD4:     return '4';
            case VirtualKey::VK_NUMPAD5:     return '5';
            case VirtualKey::VK_NUMPAD6:     return '6';
            case VirtualKey::VK_NUMPAD_PLUS: return '+';
            case VirtualKey::VK_NUMPAD1:     return '1';
            case VirtualKey::VK_NUMPAD2:     return '2';
            case VirtualKey::VK_NUMPAD3:     return '3';
            case VirtualKey::VK_NUMPAD0:     return '0';
            case VirtualKey::VK_NUMPAD_DOT:  return '.';

            default:
                return '\0';
        }
    }
}
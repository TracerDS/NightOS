#pragma once

#include <init.hpp>

#include <cstdint>
#include <cstddef>

namespace NOS::Streams {
    class basic_ios {
    public:
        using handle = uintptr_t;

        virtual ~basic_ios();
    protected:
        handle m_handle;
    };

    class basic_istream : public basic_ios {
    public:
        virtual ~basic_istream() = default;

        virtual std::size_t read(std::uint8_t* buffer, std::size_t size) noexcept = 0;
    };

    class basic_ostream : public basic_ios {
    public:
        virtual ~basic_ostream() = default;

        virtual std::size_t write(const std::uint8_t* buffer, std::size_t size) noexcept = 0;
    };
    
    class basic_stream : public basic_istream, public basic_ostream {
    };

    class istream : public basic_istream {
    public:
        std::size_t read(std::uint8_t* buffer, std::size_t size) noexcept override;
    };

    class ostream : public basic_ostream {
    public:
        ostream() noexcept;
        ostream(handle handle) noexcept;
        std::size_t write(const std::uint8_t* buffer, std::size_t size) noexcept override;
    };

    extern istream stdin;
    extern ostream stdout;
}
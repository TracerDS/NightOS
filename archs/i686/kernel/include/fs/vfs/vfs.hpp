#pragma once

#include <core/init.hpp>
#include <klibc/string>

#include <string_view>
#include <cstdint>
#include <cstddef>

namespace NOS::VFS {
    struct FileInfo {
        klibc::string name;
        std::uint32_t size = 0;
        bool isDirectory = false;
    };

    struct FileHandle {
        FileInfo info;
        std::uint32_t startCluster = 0; // opaque to VFS; meaningful to the owning Filesystem
        std::uint32_t position = 0;
    };

    using DirEntryCallback = void (*)(const FileInfo& info, void* userData);

    class Filesystem {
    public:
        virtual ~Filesystem() noexcept = default;

        virtual bool init() noexcept = 0;

        virtual bool find(
            std::string_view path,
            FileHandle& out
        ) noexcept = 0;

        virtual std::size_t read(
            FileHandle& handle,
            void* buffer,
            std::size_t count
        ) noexcept = 0;

        virtual bool list_dir(
            std::string_view path,
            DirEntryCallback callback,
            void* userData
        ) noexcept = 0;
    };

    /// TODO: Single-mount root filesystem. Replace with a real mount table once a
    /// second filesystem (or a devfs) needs to coexist alongside this one.
    extern Filesystem* g_rootFS;

    void Mount(Filesystem* fs) noexcept;

    bool Find(std::string_view path, FileHandle& out) noexcept;
    std::size_t Read(FileHandle& handle, void* buffer, std::size_t count) noexcept;
    bool ListDir(std::string_view path, DirEntryCallback callback, void* userData) noexcept;
}
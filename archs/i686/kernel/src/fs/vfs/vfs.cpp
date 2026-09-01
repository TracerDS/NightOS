#include <fs/vfs/vfs.hpp>

namespace NOS::VFS {
    Filesystem* g_rootFS = nullptr;

    void Mount(Filesystem* fs) noexcept {
        g_rootFS = fs;
    }

    bool Find(std::string_view path, FileHandle& out) noexcept {
        return g_rootFS && g_rootFS->find(path, out);
    }

    std::size_t Read(FileHandle& handle, void* buffer, std::size_t count) noexcept {
        return g_rootFS ? g_rootFS->read(handle, buffer, count) : 0;
    }

    bool ListDir(std::string_view path, DirEntryCallback callback, void* userData) noexcept {
        return g_rootFS && g_rootFS->list_dir(path, callback, userData);
    }
}
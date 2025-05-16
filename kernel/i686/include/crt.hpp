#pragma once

namespace CRT {
    using func_ptr = void (*)();

    void _init();
    void _fini();
}
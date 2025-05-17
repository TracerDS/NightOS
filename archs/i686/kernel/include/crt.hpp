#pragma once

#include <init.hpp>

__CPP_START__

namespace CRT {
    using func_ptr = void (*)();

    void crt_init();
    void crt_fini();
}

__CPP_END__
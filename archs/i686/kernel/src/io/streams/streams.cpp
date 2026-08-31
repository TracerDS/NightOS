#include <interfaces/streams/streams.hpp>

namespace NOS::Streams {
    namespace detail {
        struct handle_buffer {
            uintptr_t identifier;
            void* write_func;
            void* read_func;
        };
    }
    
    // istream stdin;
    // ostream stdout;

    basic_ios::~basic_ios() {

    }
}
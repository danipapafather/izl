#pragma once

#include <cstdint>
#include <functional>
#include <unistd.h>

namespace izl {
    class container {
    public:
        container(int (*ct_func)(void*), size_t stack_size);
        ~container();

        const std::uint8_t* get_stack_pointer() const;
        pid_t get_pid() const;
        std::function<int(void*)> get_child_function() const;

        void run();
    private:
        int (*_ct_func)(void*);
        const size_t _stack_size;
        std::uint8_t* _stack_pointer;
        pid_t _pid;
    };
}
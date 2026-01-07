#include <cstdint>
#include <sys/wait.h>
#include <sched.h>
#include <sched.h>
#include <unistd.h>

#include "container.hpp"
#include "log.hpp"

izl::container::container(int (*ct_func)(void*), size_t stack_size)
        : _ct_func(ct_func),
        _stack_size(stack_size)
{   
    izl::log::info("creating container");
    _stack_pointer = new std::uint8_t[_stack_size];
    izl::log::succes("container created");
}

izl::container::~container() {
    izl::log::info("deleting stack pointer");
    delete[] _stack_pointer;
    izl::log::succes("stack pointer deleted");
    izl::log::info("container destroyed");
}

const std::uint8_t* izl::container::get_stack_pointer() const {
    return _stack_pointer;
}

pid_t izl::container::get_pid() const {
    return _pid;
}

std::function<int(void*)> izl::container::get_child_function() const {
    return _ct_func;
}

void izl::container::run() {
    _pid = clone(_ct_func, _stack_pointer + _stack_size, SIGCHLD | CLONE_NEWNS | CLONE_NEWUTS | CLONE_NEWPID, nullptr);
    if (!_pid) {
        izl::log::error("could not create child process");
        return;
    }
    izl::log::info("running...");
    waitpid(_pid, nullptr, 0);
}
#include <sys/mount.h>
#include <iostream>
#include <unistd.h>

#include "container.hpp"
#include "log.hpp"

int child(void*) {
    chroot("./rootfs/");
    chdir("/");
    mount("proc", "/proc", "proc", 0, NULL);
    setenv("PATH", "/bin:/usr/bin", 1);
    execlp("/bin/sh", "sh", (char*)nullptr);
    return 1;
}

int main(int argc, const char** argv) {
    if (argc < 2) {
        std::cout << "Usage: " << argv[0] << " [run, help]" << std::endl;
        return 0;
    }
    
    std::string subcommand = argv[1];
    if (subcommand == "run") {
        if (getuid()) {
            izl::log::error("you are not root!");
            return -1;
        }
        constexpr size_t STACK_SIZE = 1024 * 1024;
        izl::container cont(child, STACK_SIZE);
        cont.run();
    } else if (subcommand == "help") {
        std::cout << "Usage: " << argv[0] << " [run, help]" << std::endl;
        std::cout << "run - runs an alpine linux container (requires root)\n" << 
        "help - displays this message" << std::endl;
    }

    return 0;
}

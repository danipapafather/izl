#include <iostream>
#include <string>

namespace izl {
    namespace log {
        void succes(std::string msg) {
            std::cout << "\033[32m" << "[SUCCES] " << msg << "\033[0m" << std::endl;
        }
        void error(std::string msg) {
            std::cerr << "\033[31m" << "[ERROR] " << msg << "\033[0m" << std::endl;
        }
        void info(std::string msg) {
            std::cout << "\033[34m" << "[INFO] " << msg << "\033[0m" << std::endl;
        }
    }
}
#include <iostream>
#include <string>

#include <fancy_function.h>
#include <fancy_version.h>

int
main(int argc, char** argv)
{
    if (2 == argc && std::string(argv[1]) == "--version") {
        std::cout << "Hello from fancy version: " + get_version() << std::endl;
        return 0;
    }

    if (2 == argc) {
        std::cout << fancy_function::be_fancy(std::stoi(argv[1])) << std::endl;
    } else {
        std::cout << "No input given. Assume 42."
                << std::endl
                << fancy_function::be_fancy(42)
                << std::endl;
    }
    return 0;
}

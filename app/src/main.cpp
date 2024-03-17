#include <iostream>
#include <string>

#include <fancy_version.h>

int
main(int argc, char** argv)
{
    if (2 == argc && std::string(argv[1]) == "--version") {
        std::cout << "Hello from fancy version: " + get_version() << std::endl;
        return 0;
    }

    return 0;
}

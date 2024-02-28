#include "version.h"
#include "fancy_version.h"

std::string get_version() {
    return 
    std::to_string(get_major_version()) + "." +
    std::to_string(get_minor_version()) + "." +
    std::to_string(get_patch_version())
    ;
}

int64_t get_major_version()
{
    return PROJECT_VERSION_MAJOR;
}
int64_t get_minor_version()
{
    return PROJECT_VERSION_MINOR;
}
int64_t get_patch_version()
{
    return PROJECT_VERSION_PATCH;
}
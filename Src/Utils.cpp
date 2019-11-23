#include "Utils.h"


namespace vpn
{


void exit_with_error(const char * error, int err_no) {
    std::printf("Fatal error: %s\nAborting...\n", error);
    exit(err_no);
}


} // namespace vpn




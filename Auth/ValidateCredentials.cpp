#include "ValidateCredentials.h"

#include <cstring>
#include <cctype>

namespace vpn {

bool validateCredentials(const char * data, Creds type) {
    if (std::strlen(data) > CREDENTIAL_MAX_SIZE) {
        return false;
    }

    switch (type) {
        case USERNAME:
        case PASSOWORD:
        if (std::isalnum(data)) {
            return false;
        }
        break;
        case RESPONSE:
        // TODO: check UTF-8 format !
        break;
        default:
            return false;
    }
    return true;
}

} // namespace vpn

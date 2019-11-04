#ifndef VPN_AUTH_VALIDATE_CREDENTIALS
#define VPN_AUTH_VALIDATE_CREDENTIALS

// maximal length of any credential
#define CREDENTIAL_MAX_SIZE 128

namespace vpn {

enum Creds {
    USERNAME,
    PASSOWORD,
    RESPONSE
};

bool validateCredentials(const char * data, Creds type);

} // napespace vpn

#endif // VPN_AUTH_VALIDATE_CREDENTIALS
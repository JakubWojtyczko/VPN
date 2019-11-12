#include "SSL.h"
#include "Logger.h"


#pragma comment(lib, "libeay32.dll")
#pragma comment(lib, "libssl32.dll")
#pragma comment(lib, "ssleay32.dll")

namespace vpn {
    
Ssl::~Ssl() {
    if (pub_key != nullptr) {
        BN_free(pub_key);
    }
    if (priv_key != nullptr) {
        DH_free(priv_key);
    }
    if (secret != nullptr) {
        OPENSSL_free(secret);
    }
    if (other_public != nullptr) {
        BN_free(other_public);
    }
}


bool Ssl::init() {
    if ((priv_key = DH_new()) == NULL) {
            Logger::getInstance().error("DH_get_1024_160: cannot create");
            return false;
    }
    int codes;
    if ((DH_generate_parameters_ex(priv_key, 2024, DH_GENERATOR_2, NULL) != 1)) {
        Logger::getInstance().error("DH: cannot genereate key parameters");
        return false;
    }
    if(DH_check(priv_key, &codes) != 1) {
        Logger::getInstance().error("DH: chacking failed");
        return false;
    }
    if (codes != 0) {
        Logger::getInstance().error("DH_check failed");
        return false;
    }

    // generate private and public key pair
    if (DH_generate_key(priv_key) != 1) {
        Logger::getInstance().error("DH: cannot generate a private key");
        return false;
    }
    return true;
}

bool Ssl::receive_public_key(std::string const& key) {
    if (BN_hex2bn(&other_public, key.c_str()) == 0) {
        Logger::getInstance().error("DH: revecive public key failed");
        return false;
    }
    // compute shared secret
    return this -> compute_secret();
}

bool Ssl::compute_secret() {
    if((secret = (unsigned char *)OPENSSL_malloc(sizeof(unsigned char) * (DH_size(priv_key)))) == NULL) {
        Logger::getInstance().error("DH: cannot allocate mem for secret");
        return false;
    }
    if((secret_size = DH_compute_key(secret, pub_key, priv_key)) < 0) {
        Logger::getInstance().error("DH: cannot compute a secret");
        return false;
    }
    return true;
}

std::string Ssl::get_pub_key_hex() {
    char buffer[129]; // TODO 
    if (BN_hex2bn(&pub_key, buffer) == 0) {
        Logger::getInstance().error("DH: het key hex failed");
        return "";
    }
    buffer[128] = (unsigned char)'\0';
    return std::string(buffer);
}


} // namespace vpn



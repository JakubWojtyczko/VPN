#include "SSL.h"
#include "Logger.h"

#include <openssl/evp.h>
#include <openssl/sha.h>



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

Buffer <std::uint8_t> Ssl::get_pub_key_hex() {
    char buffer[128]; // TODO 
    if (BN_hex2bn(&pub_key, buffer) == 0) {
        Logger::getInstance().error("DH: het key hex failed");
        return Buffer <std::uint8_t> ();
    }
    return Buffer <std::uint8_t> ((void *)buffer, 128);
}

Buffer <std::uint8_t> Ssl::encode_text(Buffer <std::uint8_t> const&  buffer) const {

    EVP_CIPHER_CTX * cipher_context = nullptr;
    if ((cipher_context = EVP_CIPHER_CTX_new()) == nullptr) {
        Logger::getInstance().error("DH: evp ciper ctx new failed");
        return Buffer <std::uint8_t>();
    }
    if(EVP_EncryptInit_ex(cipher_context, EVP_aes_192_cfb(), nullptr, secret, nullptr) != 1) {
        Logger::getInstance().error("DH: evp encrypt init failed");
        EVP_CIPHER_CTX_free(cipher_context);
        return Buffer <std::uint8_t>();
    }
    int len;
    unsigned char crip_text[1025]; // TODO - fixed size
    if(EVP_EncryptUpdate(cipher_context, crip_text, &len, buffer.data(), buffer.size()) != 1) {
        Logger::getInstance().error("DH: evp encrypt update failed");
        EVP_CIPHER_CTX_free(cipher_context);
        return Buffer <std::uint8_t>();
    }
    int size = len;
    if(EVP_EncryptFinal_ex(cipher_context, crip_text + len, &len) != 1) {
        Logger::getInstance().error("DH: evp encrypt final failed");
        EVP_CIPHER_CTX_free(cipher_context);
        return Buffer <std::uint8_t>();
    }
    EVP_CIPHER_CTX_free(cipher_context);
    return Buffer <std::uint8_t>(crip_text, crip_text + size);
}


Buffer <std::uint8_t> Ssl::decode_text(Buffer <std::uint8_t> const& buffer) const {
    EVP_CIPHER_CTX * cipher_context = nullptr;
    if ((cipher_context = EVP_CIPHER_CTX_new()) == nullptr) {
        Logger::getInstance().error("DH: evp ciper ctx new failed");
        return Buffer <std::uint8_t>();
    }
    if(EVP_DecryptInit_ex(cipher_context, EVP_aes_192_cfb(), nullptr, secret, nullptr) != 1) {
        Logger::getInstance().error("DH: evp decrypt init failed");
        EVP_CIPHER_CTX_free(cipher_context);
        return Buffer <std::uint8_t>();
    }
    int len;
    std::uint8_t text[1024]; // TODO
    if(EVP_DecryptUpdate(cipher_context, text, &len, buffer.data(), buffer.size()) != 1) {
        Logger::getInstance().error("DH: evp decrypt update failed");
        EVP_CIPHER_CTX_free(cipher_context);
        return Buffer <std::uint8_t>();
    }
    int size = len;
    if(EVP_DecryptFinal_ex(cipher_context, text + len, &len) != 1) {
        Logger::getInstance().error("DH: evp decrypt final failed");
        EVP_CIPHER_CTX_free(cipher_context);
        return Buffer <std::uint8_t>();
    }
    size += len;
    EVP_CIPHER_CTX_free(cipher_context);
    return Buffer <std::uint8_t> (text, text + size);
}


Buffer <std::uint8_t> Ssl::calculate_hash(Buffer <std::uint8_t> const& buffer) const {
    SHA256_CTX sha_256_ctx;
    Buffer <std::uint8_t> empty_hash;
    if (SHA256_Init(&sha_256_ctx) == 0) {
        Logger::getInstance().error("SSL: hash init failed");
        return empty_hash;
    }
    if (SHA256_Update(&sha_256_ctx, buffer.data(), buffer.size()) == 0) {
        Logger::getInstance().error("SSL: hash update failed");
        return empty_hash;
    }
    std::uint8_t hash_data[SHA256_DIGEST_LENGTH];
    if (SHA256_Final(hash_data, &sha_256_ctx) == 0) {
        Logger::getInstance().error("SSL: hash final failed");
        return empty_hash;
    }
    return Buffer <std::uint8_t> (hash_data, hash_data + SHA256_DIGEST_LENGTH);
}


} // namespace vpn

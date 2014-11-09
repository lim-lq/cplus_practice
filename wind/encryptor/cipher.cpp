#include "cipher.h"

#include <openssl/pem.h>
#include <openssl/err.h>
#include <string.h>

#include <iostream>
#include <stdexcept>
#include <sstream>

namespace wind
{

std::string Rsa::error(const std::string& err)
{
    std::ostringstream oss;
    oss << err << " failure, error code is " << ERR_get_error();
    return oss.str();
}

int Rsa::getRsaSize()
{
    if ( m_privateKey != NULL ) {
        return RSA_size(m_privateKey);
    }

    if ( m_publicKey != NULL ) {
        return RSA_size(m_publicKey);
    }
    throw std::invalid_argument("No public or private key setted.");
}

void Rsa::setPublicKey(const std::string& publicFile)
{
    FILE* fp;
    fp = fopen(publicFile.c_str(), "r");

    m_publicKey = PEM_read_RSA_PUBKEY(fp, NULL, NULL, NULL);
    fclose(fp);
    if ( m_publicKey == NULL ) {
        throw std::runtime_error(error("Set public key"));
    }
}

void Rsa::setPrivateKey(const std::string& privateFile)
{
    FILE* fp;
    fp = fopen(privateFile.c_str(), "r");
    m_privateKey = PEM_read_RSAPrivateKey(fp, NULL, NULL, NULL);
    fclose(fp);
    if ( m_privateKey == NULL ) {
        throw std::runtime_error(error("Set private key"));
    }
}

bytePtr Rsa::publicEncrypt(const std::string& plain)
{

    if ( m_publicKey == NULL ) {
        throw std::invalid_argument("No set public key");
    }

    uint8_t* from = (uint8_t*)plain.c_str();
    int len = RSA_size(m_publicKey);
    std::tr1::shared_ptr<uint8_t> to(new uint8_t[len]);

    int ret = RSA_public_encrypt(plain.size(), from, to.get(), m_publicKey, PADDING);
    if ( ret == -1 ) {
        throw std::runtime_error(error("Use public key encrypt"));
    }
    bytePtr cipher(to, (int)plain.size());
    return cipher;
}

std::string Rsa::publicDecrypt(RSA* publicKey, const std::string& cipher)
{

    return std::string();
}

std::string Rsa::privateEncrypt(RSA* privateKey, const std::string& plain)
{

    return std::string();
}

bytePtr Rsa::privateDecrypt(bytePtr cipher)
{
    if ( m_privateKey == NULL ) {
        throw std::invalid_argument("No set private key");
    }

    int len = RSA_size(m_privateKey);
    std::tr1::shared_ptr<uint8_t> to(new uint8_t[cipher.second]);

    int ret = RSA_private_decrypt(len, cipher.first.get(), to.get(), m_privateKey, PADDING);
    if ( ret == -1 ) {
        throw std::runtime_error(error("Use private key decrypt"));
    }

    bytePtr plain(to, cipher.second);

    return plain;
}

} // end namespace wind
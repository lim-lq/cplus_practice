#ifndef __CIPHER_H__
#define __CIPHER_H__

#define PADDING RSA_PKCS1_PADDING

#include <openssl/rsa.h>
#include <stdint.h>

#include <string>
#include <tr1/memory>

namespace wind
{

typedef std::pair<std::tr1::shared_ptr<uint8_t>, uint32_t> bytePtr;

class Rsa{
public:
    Rsa() : m_publicKey(NULL), m_privateKey(NULL)
    {}
    ~Rsa(){}

    std::string error(const std::string& err);

    void setPublicKey(const std::string& publicFile);
    void setPrivateKey(const std::string& privateFile);
    int getRsaSize();
    bytePtr publicEncrypt(const std::string& plain);
    std::string publicDecrypt(RSA* publicKey, const std::string& cipher);
    std::string privateEncrypt(RSA* privateKey, const std::string& plain);
    bytePtr privateDecrypt(bytePtr cipher);
private:
    RSA* m_publicKey;
    RSA* m_privateKey;
}; // end class Rsa

} // end namespace wind

#endif
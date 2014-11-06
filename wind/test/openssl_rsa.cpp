#include <openssl/rsa.h>
#include <openssl/err.h>
#include <openssl/pem.h>
#include <string.h>
#include <stdint.h>

#include <iostream>
#include <string>

using namespace std;

void print_msg(unsigned char* msg)
{
    cout << msg << endl;
}

int main(int argc, char* argv[])
{
    RSA* publicKey;
    RSA* privateKey;

/*    char cfrom[] = "I'm the plain to to to to to to";

    string msg = "wo wo xiaofnsdindfsdoijg";

    print_msg((unsigned char*)cfrom);

    print_msg((unsigned char*)msg.c_str());*/
    string from = "I'm plain";

/*
    cout << strlen((char*)from) << endl;
    string cipher((char*)from);

    cout << cipher.size() << endl;*/

    FILE *fp;
    fp = fopen("public.key", "r");

    publicKey = PEM_read_RSA_PUBKEY(fp, NULL, NULL, NULL);

    int len = RSA_size(publicKey);
    cout << len << endl;

    uint8_t* to = (uint8_t*)malloc(len);

    int ret = RSA_public_encrypt(9, (uint8_t*)from.c_str(), to, publicKey, RSA_PKCS1_PADDING);
    if ( ret == -1 ) {
        cout << ERR_get_error() << endl;
        return -1;
    }

    uint8_t* decrypt_text = (uint8_t*)malloc(9);

    fp = fopen("private.key", "r");
    privateKey = PEM_read_RSAPrivateKey(fp, NULL, NULL, NULL);
    ret = RSA_private_decrypt(len, to, decrypt_text, privateKey, RSA_PKCS1_PADDING);
    if ( ret == -1 ) {
        cout << ERR_get_error() << endl;
        return -1;
    }
    cout << RSA_size(privateKey) << endl;
    cout << decrypt_text << endl;
    free(to);
    free(decrypt_text);
    return 0;
}
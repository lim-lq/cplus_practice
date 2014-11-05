#include <openssl/rsa.h>
#include <openssl/err.h>
#include <openssl/pem.h>
#include <string.h>

#include <iostream>

using namespace std;

int main(int argc, char* argv[])
{
    RSA* publicKey = RSA_new();

    unsigned char from[] = "I'm plain";
    unsigned char* to = (unsigned char*)malloc(1024);

    FILE *fp;
    fp = fopen("public.key", "r");

    publicKey = PEM_read_RSA_PUBKEY(fp, &publicKey, NULL, NULL);

    int ret = RSA_public_encrypt(9, from, to, publicKey, RSA_PKCS1_PADDING);
    if ( ret == -1 ) {
        cout << ERR_get_error() << endl;
        return -1;
    }

    cout << "hehe " << to << " hehe" << endl;

    return 0;
}
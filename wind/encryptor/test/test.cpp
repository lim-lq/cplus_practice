#include "cipher.h"

#include <fstream>
#include <iostream>

using namespace std;
using namespace wind;

int main(int argc, char* argv[])
{
    Rsa rsa;
    rsa.setPublicKey("public.key");
    rsa.setPrivateKey("private.key");

    string plain = "I'm a boy";
    bytePtr cipher = rsa.publicEncrypt(plain);

    bytePtr decrypt_text = rsa.privateDecrypt(cipher);
    cout << decrypt_text.get() << endl;

/*    fstream file("cipher.txt", fstream::trunc | fstream::out);
    file << cipher;
    file.close();*/
    return 0;
}
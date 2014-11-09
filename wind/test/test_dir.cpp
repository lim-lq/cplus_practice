#include <fstream>
#include <iostream>
#include <string>
#include <sys/stat.h>

using namespace std;

int main(int argc, char* argv[])
{
    string filename = "./2/public.key";
    int ret = access(filename.c_str(), F_OK);
    cout << ret << endl;
    // mkdir("./1", 0744);
    fstream file(filename.c_str(), fstream::out | fstream::trunc);
    file << "I'm not happy.";
    file.close();

    return 0;
}

#include <iostream>
#include <boost/uuid/sha1.hpp>
#include <string>
#include <sstream>

using namespace std;
using namespace boost::uuids::detail;

int main(int argc, char* argv[])
{
    sha1 sha;

    string msg = "123";
    ostringstream oss;

    sha.process_bytes(msg.c_str(), msg.size());
    unsigned int digest[5];
    sha.get_digest(digest);

    for ( int i = 0; i < 5; ++i ) {
        oss << hex << digest[i];
        // break;
    }
    cout << oss.str() << endl;

/*    int a = 12;
    cout << hex << a << endl;*/
    return 0;
}
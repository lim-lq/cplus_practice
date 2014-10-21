#include "fileInterface.h"
#include <Ice/Ice.h>
#include <string>

using namespace std;

class ClientApp : public Ice::Application
{
public:
    virtual int run(int, char*[]);
};

int main(int argc, char* argv[])
{
    ClientApp app;
    return app.main(argc, argv, "client.config");
}

int ClientApp::run(int argc, char* argv[])
{
    Blond::FileInterfacePrx fileprx = Blond::FileInterfacePrx::checkedCast(communicator()->propertyToProxy("FileSystem.Proxy"));
    // Blond::FileInterfacePrx fileprx = Blond::FileInterfacePrx::checkedCast(communicator()->stringToProxy("file:tcp -h 10.192.0.5 -p 10000"));
    if ( ! fileprx ) {
        cout << "Invalid proxy" << endl;
        return -1;
    }

    string downloadfile = fileprx->downloadfile("liqing");
    Blond::Files files = fileprx->getfiles();
    for ( Blond::Files::iterator it = files.begin(); it != files.end(); ++it ) {
        cout << *it << endl;
    }
    cout << "Down load file " << downloadfile << ".\n";
    return 0;
}
/*#include <Ice/Ice.h>
#include <fileInterface.h>

using namespace std;

int main(int argc, char* argv[])
{
    Ice::CommunicatorPtr communicator;

    try {
        communicator = Ice::initialize(argc, argv);
        Blond::FileInterfacePrx file = Blond::FileInterfacePrx::checkedCast(communicator->stringToProxy("file:tcp -h localhost -p 10000"));
        cout << file->downloadfile("liqing") << endl;
    } catch ( const Ice::Exception& ex ) {
        cerr << ex << endl;
    }
    if ( communicator ) {
        communicator->destroy();
    }

    return 0;
}*/
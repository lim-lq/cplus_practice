#include "file_wrapper.h"
#include <iostream>
#include <Ice/Ice.h>

using namespace std;

class FileApp : public Ice::Application
{
public:
    virtual int run(int argc, char* argv[]);
};

int main(int argc, char* argv[])
{
    FileApp app;
    return app.main(argc, argv, "server.config");
}

int FileApp::run(int argc, char* argv[])
{
    cout << "FileSystem server running." << endl;

    Ice::ObjectAdapterPtr adapter = communicator()->createObjectAdapter("FileSystem");
    blond::FileWrapper* fileWrapper = new blond::FileWrapper();
    Blond::FileInterfacePtr filePtr = fileWrapper;
    adapter->add(filePtr, communicator()->stringToIdentity("file"));
    adapter->activate();

    communicator()->waitForShutdown();
    return 0;
}
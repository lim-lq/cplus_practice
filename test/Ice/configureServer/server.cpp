#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <Ice/Ice.h>

#include "configure_wrapper.h"

using namespace std;

class WorkerApp : public Ice::Application
{
public:
    virtual int run(int, char*[]);
};

int main(int argc, char* argv[])
{
    WorkerApp app;
    return app.main(argc, argv, "server.config");
}

int WorkerApp::run(int argc, char* argv[])
{
    Ice::ObjectAdapterPtr adapter = communicator()->createObjectAdapter("configureServerInterface");
    Blond::ConfigureServerInterfacePtr conServer = new blond::ConfigureWrapper;
    adapter->add(conServer, communicator()->stringToIdentity("configureServer"));
    adapter->activate();
    cout << "Configure server running." << endl;
    communicator()->waitForShutdown();
    return 0;
}
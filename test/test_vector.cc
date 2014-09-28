#include <iostream>
#include <vector>
#include <string>

using namespace std;

struct ClientInfo{
    ClientInfo(const string& host, const int& port) : m_host(host), m_port(port){}
    string host;
    int port;
};

int main(int argc, char* argv[])
{
    vector<string> v1;
    vector<ClientInfo *> v2;

    string host = "0.0.0.0";
    int port = 8000;
    string message;
    const string name = "xiaodaren";
/*    cout << "Please input something things (ctrl + D) to end." << endl;
    while ( std::getline(cin, message) ) {
        v1.push_back(message);
    }
    
    cout << "You entered all messages are :" << endl;*/
    const ClientInfo *p_clientinfo = new ClientInfo(host, port);
    v2.push_back(p_clientinfo);
    v1.push_back(name);
    v1.push_back("liqing");
    v1.push_back("daren");
    v1.push_back("xiaoxiao");
    v1.push_back("keren");
    cout << "Befor erasing data: " << endl;
    for ( vector<string>::iterator it = v1.begin(); it != v1.end(); ++it ) {
        cout << *it << endl;
    }

    message = "liqing";
    for ( vector<string>::iterator it = v1.begin(); it != v1.end(); ++it ) {
        if ( message == *it ) {
            v1.erase(it);
            break;
        }
    }
    cout << "After earsing data <liqing>: " << endl;
    for ( vector<string>::iterator it = v1.begin(); it != v1.end(); ++it ) {
        cout << *it << endl;
    }

    return 0;
}
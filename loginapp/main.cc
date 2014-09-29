#include "loginapp.h"
#include <iostream>
#include <sstream>
#include <stdlib.h>
#include "configparser.hpp"

using namespace std;
using namespace coco;

int main(int argc, char* argv[])
{
    string host = "localhost";
    string username = "root";
    string passwd = "sointux076";
    string db = "coco";
    unsigned int port = 3306;

    ConfigParser configParser;
    configParser.read("loginapp.conf");

    if ( configParser.get("mysql", "host") != "" ) {
        host = configParser.get("mysql", "host");
    }

    if ( configParser.get("mysql", "username") != "" ) {
        username = configParser.get("mysql", "username");
    }

    if ( configParser.get("mysql", "passwd") != "" ) {
        passwd = configParser.get("mysql", "passwd");
    }

    if ( configParser.get("mysql", "db") != "" ) {
        db = configParser.get("mysql", "db");
    }

    if ( configParser.get("mysql", "port") != "" ) {
        istringstream iss(configParser.get("mysql", "port"));
        iss >> port;
    }

    LoginApp loginapp;

    MysqlDB mysqldb(host, username, passwd, db, port);
    loginapp.Initmysql(&mysqldb);

    int ret = loginapp.login("liqing", "123");
    if ( ret == -1 ) {
        cout << "Login failure, please check your username or passwd." << endl;
        return -1;
    }
    cout << "Login success, then you can do what you want to do." << endl;
    return 0;
}
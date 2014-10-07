#include "loginapp.h"
#include <iostream>
#include <sstream>
#include <stdlib.h>
#include "configparser.hpp"

using namespace std;
using namespace coco;

int main(int argc, char* argv[])
{
    string mysql_host = "localhost";
    string username = "root";
    string passwd = "sointux076";
    string db = "coco";
    unsigned int mysql_port = 3306;

    string srv_host = "localhost";
    unsigned short srv_port = 8000;

    ConfigParser configParser;
    configParser.read("loginapp.conf");

    if ( configParser.get("mysql", "host") != "" ) {
        mysql_host = configParser.get("mysql", "host");
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
        iss >> mysql_port;
    }

    if ( configParser.get("server", "host") != "" ) {
        srv_host = configParser.get("server", "host");
    }

    if ( configParser.get("server", "port") != "" ) {
        istringstream iss(configParser.get("server", "port"));
        iss >> srv_port;
    }

    LoginApp loginapp;

    MysqlDB mysqldb(mysql_host, username, passwd, db, mysql_port);
    loginapp.Initmysql(&mysqldb);

    loginapp.start_server(srv_host, srv_port);
/*    int ret = loginapp.login("liqing", "123");
    if ( ret == -1 ) {
        cout << "Login failure, please check your username or passwd." << endl;
        return -1;
    }
    cout << "Login success, then you can do what you want to do." << endl;*/

    return 0;
}
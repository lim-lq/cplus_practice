#include "dbmgr.h"
#include "../common/configParser.hpp"
#include "../common/utils.hpp"

using namespace std;
using namespace coco;

int main(int argc, char* argv[])
{
    string login_username = "liqing";
    string login_passwd = "123";
    int opt = 0;

    while ( (opt = getopt(argc, argv, "u:p:h")) != -1 ) {
        switch ( opt ) {
        case 'u':
            login_username = optarg;
            break;
        case 'p':
            login_passwd = optarg;
            break;
        case 'h':
            cout << "Usage: " << argv[0]
                 << " -u username"
                 << " -p password." << endl;
            return 0;
        default:
            break;
        }
    }

    ConfigParser config;
    config.load("dbmgr.conf");

    string host = "localhost";
    string username = "liqing";
    string passwd = "123";
    string db = "coco";
    unsigned int port = 3306;

    host = config.get("mysql", "host");
    username = config.get("mysql", "username");
    passwd = config.get("mysql", "passwd");
    db = config.get("mysql", "db");
    port = stringTo<unsigned int>(config.get("mysql", "port"));

    MysqlDB mysqldb(host, username, passwd, db, port);

    DBmgr dbmgr(mysqldb);

    if ( !dbmgr.check_account(login_username) ) {
        cout << "Accout '" << login_username << "' is not exists." << endl;
        exit(1);
    }

    if ( !dbmgr.check_login(login_username, gen_sha1(login_passwd)) ) {
        cout << "Password '" << login_passwd << "' is error." << endl;
        exit(1);
    }

    cout << login_username << " login success with password '" << login_passwd << "'.\n";

    return 0;
}
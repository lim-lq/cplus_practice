#include <iostream>
#include "mysqldb.h"

using namespace std;
using namespace wind;

int main(int argc, char* argv[])
{
    MysqlDB mysql = MysqlDB();

    string host = "192.168.200.128";
    string user = "root";
    string passwd = "sointux076";
    string db = "wind";
    uint32_t port = 3306;

    if ( mysql.connect(host, user, passwd, db, port) == NULL ) {
        cout << "Connect mysql failure." << endl;
        return 1;
    }

    string sql = "select * from users";

    MYSQL_RESULT result = mysql.query(sql);

    for ( MYSQL_RESULT::iterator it = result.begin(); it != result.end(); ++ it ) {
        for ( map<string, string>::iterator mit = it->begin(); mit != it->end(); ++mit ) {
            cout << (*mit).first << '\t' << (*mit).second << endl;
        }
    }
    return 0;
}
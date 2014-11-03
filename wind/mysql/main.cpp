#include <iostream>
#include "mysqldb.h"

using namespace std;
using namespace wind;

int main(int argc, char* argv[])
{
    MysqlDB mysql;

    string host = "10.192.0.5";
    string user = "root";
    string passwd = "fengmao";
    string db = "wind";
    uint32_t port = 3306;

    if ( mysql.connect(host, user, passwd, db, port) == NULL ) {
        cout << "Connect mysql failure." << endl;
        mysql.error();
        return 1;
    }

    string sql = "select * from users";

    MYSQL_RESULT result = mysql.query(sql);

    if ( result.empty() ) {
        mysql.error();
        return 1;
    }

    for ( MYSQL_RESULT::iterator it = result.begin(); it != result.end(); ++ it ) {
        for ( map<string, string>::iterator mit = it->begin(); mit != it->end(); ++mit ) {
            cout << (*mit).first << '\t' << (*mit).second << endl;
        }
    }
    return 0;
}
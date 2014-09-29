#include <stdio.h>
#include <iostream>
#include <mysql/mysql.h>
#include <string>
#include <stdlib.h>

using namespace std;

MYSQL *connection, mysql;
MYSQL_RES *result;
MYSQL_ROW row;

int verify_user(const string& username, const string& password)
{
    string sql;
    sql = "select * from users where username='" + username + "' and password='" + password + "'";

    cout << "The sql is '" << sql << "'." << endl;
    int ret = mysql_query(connection, sql.c_str());
    if ( ret != 0 ) {
        cout << "query failure, " << mysql_error(connection) << endl;
        return -1;
    }

    result = mysql_store_result(connection);
    if ( mysql_num_rows(result) == 0 ) {
        return -1;
    }
    return 0;
}

int main(int argc, char* argv[])
{
    if ( mysql_init(&mysql) == NULL ) {
        cout << "Initialize mysql instance error." << endl;
        exit(1);
    }
    string host = "10.192.0.5";
    string user = "root";
    string passwd = "fengmao";
    string db = "test";
    unsigned int port = 3306;

    connection = mysql_real_connect(&mysql,
                                    host.c_str(),
                                    user.c_str(),
                                    passwd.c_str(),
                                    db.c_str(),
                                    port, 0, 0);

    if ( connection == NULL ) {
        cout << "Connect mysql error " << mysql_error(&mysql) << endl;
        exit(1);
    }
    string username = "admin";
    string password = "121";
    int ret = verify_user(username, password);
    if ( ret != 0 ) {
        cout << "Login failure, please check your username or password." << endl;
        exit(1);
    }

    cout << "Congratulations, login success, then you can do what you want." << endl;
/*    string sql = "select * from students";
    int ret = mysql_query(connection, sql.c_str());
    if ( ret != 0 ) {
        cout << "Query data error " << mysql_error(connection) << endl;
        exit(1);
    }

    result = mysql_store_result(connection);
    cout << "Rows: " << mysql_num_rows(result) << endl;

    while ( (row = mysql_fetch_row(result)) != NULL ) {
        cout << row[0] << " " << row[1] << endl;
    }*/
    mysql_free_result(result);
    mysql_close(connection);
    return 0;
}
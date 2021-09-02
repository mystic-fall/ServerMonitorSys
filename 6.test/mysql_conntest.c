/*************************************************************************
	> File Name: mysql_conntest.c
	> Author: 
	> Mail: 
	> Created Time: Tue 31 Aug 2021 03:36:50 PM CST
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<mysql.h>

int main() {
    MYSQL *conn;
    conn = mysql_init(NULL);
    if(conn == NULL) {
        printf("errno\n");
        return -1;
    }
    
    if(mysql_real_connect(conn, "localhost", "root", "zty328961@", "my_database1", 0, NULL, 0))
    {
        printf("connection success\n");
    } else {
        printf("errno = %d\nerror: %s\n", mysql_errno(conn), mysql_error(conn));
    }
    /*
    int ret = mysql_query(conn, "INSERT INTO class(className,grade,major,departmentName) VALUES('niubi',22,'cs','hehe')");
    if(!ret) {
        printf("Insert %d rows\n", (int)mysql_affected_rows(conn));
    } else {
        printf("errno = %d\nerror: %s\n", mysql_errno(conn), mysql_error(conn));
    }
    */
    int ret = mysql_query(conn, "INSERT INTO ");
    if(!ret) {
        printf("Insert %d rows\n", (int)mysql_affected_rows(conn));
    } else {
        printf("errno = %d\nerror: %s\n", mysql_errno(conn), mysql_error(conn));
    }
    
    mysql_close(conn);
    return 0;
}


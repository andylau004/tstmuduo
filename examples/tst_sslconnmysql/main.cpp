




#include <stdio.h>

#include <iostream>
#include <sstream>

#include <iostream>
#include <string>
#include <algorithm>
#include <stack>
#include <vector>
#include <queue>

#include <fstream>
#include <thread>

#include <boost/bind.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>


#include <mysql.h>


#include "muduo/base/common.h"

#include "muduo/net/InetAddress.h"
#include "muduo/net/Channel.h"
#include "muduo/net/EventLoop.h"
#include "muduo/net/EventLoopThread.h"
#include "muduo/net/EventLoopThreadPool.h"
#include "muduo/net/TcpServer.h"
#include "muduo/base/Logging.h"
#include "muduo/net/TcpClient.h"


// #include "wrfun.h"


using namespace std;
using namespace muduo;
using namespace muduo::net;




int tst_ssl_connect_mysql() {

    MYSQL *conn;
   MYSQL_RES *res;
   MYSQL_ROW row;
   char *server = "127.0.0.1";
    // char* server = "192.168.242.145";
   char *user = "root";
   char *password = "Admin123@qwe"; /* set me first */
   char *database = "tst_db";
   int port = 3306;

   conn = mysql_init(NULL);
    const char* clientKey = "client-key.pem";
    const char* clientCrt = "client-cert.pem";
    const char* caCrt = "ca.pem";
   mysql_ssl_set(conn, clientKey, clientCrt, caCrt, NULL, "DHE-RSA-AES256-SHA256");
   mysql_options(conn, MYSQL_OPT_CONNECT_TIMEOUT, "30");

   /* Connect to database */
   if (!mysql_real_connect(conn, server,
         user, password, database, port, NULL, 0)) {
      fprintf(stderr, "Real connect: %d: %s\n", mysql_errno(conn), mysql_error(conn));
      return 1;
   }

   printf("SSL cipher used: %s \n", mysql_get_ssl_cipher(conn));

   /* send SQL query */
   if (mysql_query(conn, "show tables;")) {
      fprintf(stderr, "\nQuery: %s\n", mysql_error(conn));
      return 1;
   }
   res = mysql_use_result(conn);
   /* output table name */
   printf("Results:\n");
   while ((row = mysql_fetch_row(res)) != NULL)
      printf("%s \n", row[0]);


   /* close connection */
   mysql_free_result(res);
   mysql_close(conn);
   return 0;
}

int main(int argc, char *argv[])
{
    Logger::setLogLevel(Logger::DEBUG);
    LOG_INFO << "pid = " << getpid() << ", tid = " << CurrentThread::tid();

    LOG_INFO << "sizeof(long)=" << sizeof(long);

    tst_ssl_connect_mysql();
//    tst_wr1();

    // tst_wr_mem();

    return 1;
}


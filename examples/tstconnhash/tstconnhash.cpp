

#include "tstconnhash.h"



#include <stdio.h>
#include <iostream>
#include <string>

#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <boost/scoped_ptr.hpp>



#include "muduo/base/Timestamp.h"
//#include <muduo/base/Timestamp.h>

#include "muduo/base/common.h"


using namespace std;
using namespace muduo;
//using namespace muduo::net;

#include "ConsistentHash.h"


void showTime(ConsistentHash &csshash){
    cout << csshash.showTime();
//    cout << "-------------------------------" << endl;
}

//void tst_conn_hash_entry()
//{
//    ConsistentHash csshash;
//    csshash.addNode("192.168.0.1:8080", 2);
//    csshash.addNode("192.168.0.2:8859", 3);
//    csshash.addNode("10.0.1.56:80", 2);
//    csshash.addNode("10.0.1.57:80", 3);
//    showTime(csshash);

//    //模拟一个客户端
//    string cliIp = "192.168.56.1:12345";
//    string serIp = csshash.getServerName(cliIp);
//    cout << cliIp << "\thash值：" << csshash.getHash(cliIp) << " -> " << serIp << endl;

//    cliIp = "10.1.0.194:12345";
//    serIp = csshash.getServerName(cliIp);
//    cout << cliIp << "\thash值：" << csshash.getHash(cliIp) << " -> " << serIp << endl;

//    system("pause");
//}





void printError(){
    std::cout << "input error !" << std::endl;
}

void printHelp()
{
    const char *help_message = ""
        "Usage:"
        "\n\n add [host] [port] [num]"
        "\n     add server to container."
        "\n     num is virtual node number"
        "\n\n del [host] [port]"
        "\n     delte server to container."
        "\n\n get [host] [port]"
        "\n     get a server ip of container."
        "\n\n getVirNum [host] [port]"
        "\n     get the host virtual node number."
        "\n\n getRealNum"
        "\n     get all real server number."
        "\n\n getAllNodeNum"
        "\n     get all server number."
        "\n\n showTime"
        "\n     show the all key and value."
        "\n\n statDist"
        "\n     Statistics the distribution of all nodes"
        "\n\n h"
        "\n     Print this message."
        "\n\n q"
        "\n     Quit the application.\n";
    std::cout << help_message << std::endl;
}

int splitCMD(std::string cmd, const char *spl, std::vector<std::string> &v){
    char *p = strtok((char *)cmd.c_str(), spl);
    int i = 0;
    while(p && i < 4){
        v[i++] = std::string(p);
        p = strtok(NULL,spl);
    }
    return i;
}

int tst_conn_hash_entry(int argc, char *argv[]){
    if (argc != 1){
        std::cout << "please input " << argv[0] << std::endl;
        return 0;
    }
//    printHelp();

    {
        ConsistentHash csshash;
        std::cout << "-------------------" << std::endl;
        csshash.addNode("192.168.0.1:8080", 2);
        csshash.addNode("192.168.0.2:8859", 3);
        csshash.addNode("10.0.1.56:80", 2);
        csshash.addNode("10.0.1.57:80", 3);

        std::cout << "-------------------" << std::endl;
        std::cout << std::endl;
//        return 1;

        std::cout << "-------------------showTime Beg" << std::endl;
        showTime(csshash);
        std::cout << "-------------------showTime End" << std::endl;
        std::cout << std::endl;

        //模拟一个客户端
//        std::string  cliIp = "192.168.56.1:12345";
        std::string  cliIp = "1.1.1.1:12345";
        std::string  serIp = csshash.getServerName(cliIp);
//        cout << cliIp << "\thash值：" << csshash.getHash(cliIp) << " -> " << serIp << endl;

        std::cout << std::endl;

        cliIp = "10.1.0.194:12345";
        serIp = csshash.getServerName(cliIp);
//        cout << cliIp << "\thash值：" << csshash.getHash(cliIp) << " -> " << serIp << endl;
        std::cout << std::endl;

//        showTime(csshash);
    }
    return 1;

    std::string input;
    std::vector<std::string> v(4);
    ConsistentHash csshash;
    while (1){
        std::cout << "<<<";
        getline(std::cin, input);
        if(input.empty())
            continue;
        //全部转化为小写（大小写不敏感）
        MakeLower(input);

        int num = splitCMD(input, " ", v);
        if (v[0] == "add"){
            if (num != 4){
                printError();
            }
            else{
                csshash.addNode(v[1] + ":" + v[2], atoi(v[3].c_str()));
            }
        }
        else if (v[0] == "del"){
            if (num != 3){
                printError();
            }
            else{
                csshash.delNode(v[1] + ":" + v[2]);
            }
        }
        else if (v[0] == "get"){
            if (num!= 3){
                printError();
            }
            else{
                std::cout << csshash.getServerName(v[1] + ":" + v[2]) << std::endl;
            }
        }
        else if (v[0] == "getvirnum"){
            if (num != 3){
                printError();
            }
            else{
                std::cout << csshash.getVirNum(v[1] + ":" + v[2]) << std::endl;
            }
        }
        else if (v[0] == "getrealnum"){
            if (num != 1){
                printError();
            }
            else{
                std::cout << csshash.getRealNum() << std::endl;
            }
        }
        else if (v[0] == "getallnodenum"){
            if (num != 1){
                printError();
            }
            else{
                std::cout << csshash.getAllNodeNum() << std::endl;
            }
        }
        else if (v[0] == "showtime"){
            if (num != 1){
                printError();
            }
            else{
                std::cout<<csshash.showTime();
            }
        }
        else if (v[0] == "statdist"){
            if (num != 1){
                printError();
            }
            else{
                std::cout << csshash.statDist();
            }
        }
        else if (v[0] == "h"){
            if (num != 1){
                printError();
            }
            else{
                printHelp();
            }
        }
        else if (v[0] == "q"){
            if (num != 1){
                printError();
            }
            else{
                return 0;
            }
        }
        else{
            printError();
        }
    }
}








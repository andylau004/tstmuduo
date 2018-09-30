#include "ConsistentHash.h"

ConsistentHash::ConsistentHash(HashFunc *func){
    if (func){
        hashFunc = func;
    }
    else{
        hashFunc = new MD5HashFunc();
//        hashFunc = new MurmurHah32Func();
    }
}
ConsistentHash::~ConsistentHash(){
    for (auto it = realNode.begin(); it != realNode.end();){
        delete *it;
        realNode.erase(it++);
    }
}
bool ConsistentHash::addRealNode(Node *node){
    if (!node)
        return false;
    string       value = node->GetNodeName();//服务器ip
    unsigned int key   = hashFunc->GetKey(value);//获取key
    realNode.push_back(node);

    std::cout << "realName=" << value << ", realKey=" << key << std::endl;
    allNode.insert(make_pair(key, value));
    return true;
}
bool ConsistentHash::addVirNode(Node *node){
    if (!node){
        std::cout << "input node is null" << std::endl;
        return false;
    }
    int vNum = node->GetVNodeNum();
    if (vNum < 1){
        std::cout << "vNum < 1 error" << std::endl;
        return false;
    }
    unsigned int key;
    std::string virName;//虚拟节点名称
    std::string nodeName = node->GetNodeName();//真实节点名
    //vNode添加后缀
    for (int i = 0; i < vNum; ++i) {
        std::stringstream ss;
        ss << nodeName << "##" << i + 1;
//        std::cout << ss.str() << std::endl;
        ss >> virName;
//        ss.str().clear();

        key = hashFunc->GetKey(virName);
        allNode.insert(std::make_pair(key, nodeName));//存的都是真实的名称

        std::cout << "virKey=" << key << ", virName=" << ss.str() << std::endl;
    }
    return true;
}
bool ConsistentHash::addNode(std::string nodeName, int vNum){
    Node *pNewNode = new Node(nodeName, vNum);
    //对虚拟节点进行push
    if (!addVirNode(pNewNode))
        return false;
    //对真实节点进行push
    if (!addRealNode(pNewNode))
        return false;
    return true;
}
bool ConsistentHash::delNode(std::string nodeName){
    //找到这个节点
    Node *node = findRealNode(nodeName);
    //删除节点
    if (!delRealNode(node))
        return false;
    if (!delVirNode(node))
        return false;
    //释放内存
    delete node;
    return true;
}
bool ConsistentHash::delRealNode(Node *node){
    if (!node){
        std::cout << "input node is null" << std::endl;
        return false;
    }
    std::string nodeName = node->GetNodeName();
    /* 在multimap中删除真实节点 */
    unsigned int key = hashFunc->GetKey(nodeName);
    int count = allNode.count(key);
    auto delIt = allNode.find(key);
    while (delIt != allNode.end() && count--){
        if (delIt->second == nodeName){
            allNode.erase(delIt);
            break;
        }
        ++delIt;
    }
    return true;
}
bool ConsistentHash::delVirNode(Node *node){
    if (!node){
        return false;
    }
    int vNum = node->GetVNodeNum(), count;
    unsigned int key;
    std::string virName, nodeName = node->GetNodeName();
    std::stringstream ss;
    //vNode添加后缀
    for (int i = 0; i < vNum; ++i){
        ss << nodeName << "##" << i + 1;
        ss >> virName;
        ss.clear();
        key = hashFunc->GetKey(virName);
        /* 在multimap中删除节点 */
        count = allNode.count(key);
        auto delIt = allNode.find(key);
        while (delIt != allNode.end() && count--){
            if (delIt->second == nodeName){
                allNode.erase(delIt++);
            }else{
                ++delIt;
            }
        }
    }
    return true;
}
Node *ConsistentHash::findRealNode(std::string &nodeName){
    /* 先找到node 并在list中删除*/
    Node *node = nullptr;
    for (auto it = realNode.begin(); it != realNode.end(); ++it){
        if ((*it)->GetNodeName() == nodeName){
            node = *it;
            realNode.erase(it);
            break;
        }
    }
    if (!node){
        //日志：list没找到真实节点
    }
    return node;
}
std::string ConsistentHash::getServerName(std::string cliName){
    /* 根据客户端的ip找到可以提供服务的节点 */
    std::string serName;

    unsigned int cliKey = hashFunc->GetKey(cliName);
    std::cout << "cliName=" << cliName << ", cliKey=" << cliKey << std::endl;

    auto it = allNode.begin();

    auto lowIt = allNode.lower_bound(cliKey);
    if (lowIt != allNode.end()) {
        std::cout << "lowbound_It key=" << lowIt->first <<
                     ", lowbound_It val=" << lowIt->second << std::endl;
    }
    if (lowIt == allNode.end()){
        std::cout << "cliKey not found storeServer, cliKey=" << cliKey << ", cliName=" << cliName << std::endl;
        serName = allNode.begin()->second;
    }
    return serName;

    /* 找到第一个比cliKey大的节点 */
//    while (it != allNode.end()){
//        if (cliKey < it->first){
//            serName = it->second;
//            std::cout << "find cliKey < " << it->first << ", serName=" << it->second << std::endl;

////            std::pair<std::multimap<unsigned int, std::string>::iterator, std::multimap<unsigned int, std::string>::iterator> ret;
////            ret = allNode.equal_range(it->first);

////            for(auto tmpIt = ret.first; tmpIt != ret.second; ++tmpIt)
////                std::cout << " xxx first=" << (*tmpIt).first << " xxx second=" << (*tmpIt).second;
////            std::cout << std::endl;

//            break;
//        }
//        ++it;
//    }
//    if (it == allNode.end()){
//        std::cout << "cliKey not found storeServer, cliKey=" << cliKey << ", cliName=" << cliName << std::endl;
//        serName = allNode.begin()->second;
//    }
//    return serName;
}
int ConsistentHash::getVirNum(std::string serName){
    Node *node = nullptr;
    for (auto it = realNode.begin(); it != realNode.end(); ++it){
        if ((*it)->GetNodeName() == serName){
            node = *it;
        }
    }
    if (!node)
        return -1;
    return node->GetVNodeNum();
}
int ConsistentHash::getRealNum(){
    return realNode.size();
}
int ConsistentHash::getAllNodeNum(){
    return realNode.size() + allNode.size();
}
std::string ConsistentHash::showTime(){
    std::ostringstream oss;
    oss << "allNodeCount=" << allNode.size() << std::endl;
    for (auto it = allNode.begin(); it != allNode.end(); ++it){
        oss << "key : " << it->first << "  \tval : " << it->second << "\n";
    }
//    allNode.insert(std::make_pair(56682548, "192.168.1.2:8181"));
    return oss.str();
}
std::string ConsistentHash::statDist(){
    //max = 2^32 = 4294967296
    //所以分8个区 500000000 1000000000 1500000000 ---
    int i = 1;
    unsigned int sum = 0;
    std::ostringstream oss;
    for (auto it = allNode.begin(); it != allNode.end(); ++it){
        if(it->first / 500000000  < i) {
            ++sum;
        }
        else {
            oss<< "hash( " << 5 * (i - 1) <<", "<< 5 * i <<" )million\tnodeNum = "<< sum <<"\n";
            ++i;
            sum = 0;
        }
    }
    return oss.str();
}

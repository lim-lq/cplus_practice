#include <arpa/inet.h>
#include <pthread.h>
#include <strings.h>
#include <string.h>
#include <stdlib.h>
#include "server.h"

namespace simplechat{

ChatServer::ChatServer(const std::string& host, const unsigned short& port)
{
/*    sockaddr_in serverAddr;

    m_serverfd = socket(AF_INET, SOCK_STREAM, 0);
    if ( m_serverfd < 0 ) {
        print_err("Create socket");
        exit(1);
    }*/
    m_host = host;
    m_port = port;
/*    bzero(&serverAddr, sizeof(serverAddr));

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    serverAddr.sin_addr.s_addr = inet_addr(host.c_str());
    int on = 1;
    int ret = setsockopt(m_serverfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
    if ( ret == -1 ) {
        print_err("setsockopt to reuseaddr");
        exit(1);
    }
    if ( bind(m_serverfd, (sockaddr*)&serverAddr, sizeof(serverAddr)) == -1 ) {
        print_err("Bind socket");
        exit(1);
    }

    if ( listen(m_serverfd, 10) == -1 ) {
        print_err("Listen connect");
        exit(1);
    }*/
}

/*ChatServer::~ChatServer()
{
    for ( std::vector<ConnectInfo::ClientInfo*>::iterator it = m_clientInfos.begin();
          it != m_clientInfos.end(); ++it ) {
        close((*it)->clientfd);
    }
    // close(m_serverfd);
}*/

/*void ChatServer::send_message(std::string& message, const std::string& clientIP)
{
    std::string new_message;
    //strip(message);
    std::cout << "Recieved message: '" << message << "' from client<"
              << clientIP << ">\n";
    // pthread_mutex_lock(&m_lock);
    for ( std::vector<ConnectInfo::ClientInfo*>::iterator it = m_clientInfos.begin();
          it != m_clientInfos.end(); ++it ) {
        std::cout << "Begin send message to client " << inet_ntoa((*it)->clientAddr.sin_addr) << std::endl;
        if ( inet_ntoa((*it)->clientAddr.sin_addr) == clientIP ) {
            new_message = "[YOU]";
            // new_message = "You said> " + message;
        } else {
            new_message = clientIP + " said> " + message;
        }
        if ( send((*it)->clientfd, new_message.c_str(), new_message.size(), 0) == -1 ) {
            std::cout << "Send message to client<"
                      << inet_ntoa((*it)->clientAddr.sin_addr)
                      << "> failure, error code[" << errno << "]." << std::endl;
        }
    }
    // pthread_mutex_unlock(&m_lock);
}

void* ChatServer::connect_proxy(void *ptr)
{
    ConnectInfo* p_connectinfo =  (ConnectInfo*)ptr;
    p_connectinfo->_server->do_connect(p_connectinfo->p_clientinfo);
}

void ChatServer::do_connect(ConnectInfo::ClientInfo *p_clientinfo)
{
    pthread_mutex_lock(&m_lock);
    m_clientInfos.push_back(p_clientinfo);
    pthread_mutex_unlock(&m_lock);

    
    while ( true ) {
        char buf[1025];
        memset(buf, 0, sizeof(buf));
        int ret = recv(p_clientinfo->clientfd, buf, 1024, 0);
        if ( ret == 0 ) {
            std::cout << "Client<"
                      << inet_ntoa(p_clientinfo->clientAddr.sin_addr)
                      << "> has shutdown the connect." << std::endl;
            break;
        } else if ( ret == -1 ) {
            std::cout << "Recieve message from client<"
                      << inet_ntoa(p_clientinfo->clientAddr.sin_addr)
                      << "> failure, error code [ " << errno << " ]\n";
            sleep(3);
            continue;
        }
        // buf[sizeof(buf)] = '\0';
        std::string message = buf;
        std::cout << "'" << message << "'" << std::endl;
        pthread_mutex_lock(&m_lock);
        send_message(message, inet_ntoa(p_clientinfo->clientAddr.sin_addr));
        pthread_mutex_unlock(&m_lock);
    }

    pthread_mutex_lock(&m_lock);
    for ( std::vector<ConnectInfo::ClientInfo*>::iterator it = m_clientInfos.begin();
          it != m_clientInfos.end(); ++it ) {
        if ( (*it)->clientfd == p_clientinfo->clientfd ) {
            m_clientInfos.erase(it);
            break;
        }
    }
    pthread_mutex_unlock(&m_lock);
    delete p_clientinfo;
}

void ChatServer::start_thread(const ConnectInfo* p_connectinfo)
{
    pthread_t tid;

    if ( pthread_create(&tid, NULL, ChatServer::connect_proxy, (void *)p_connectinfo) ) {
        std::cout << "Create thread failure" << std::endl;
        delete p_connectinfo->p_clientinfo;
        return;
    }

    std::cout << "Start thread interaction with client<" 
              << inet_ntoa(p_connectinfo->p_clientinfo->clientAddr.sin_addr)
              << "> connected. [ socket fd is " << p_connectinfo->p_clientinfo->clientfd << " ]\n";

}*/

void ChatServer::wait()
{
/*    m_RUNSTATUS = true;
    sockaddr_in clientAddr;
    socklen_t addrlen = sizeof(clientAddr);*/
    pthread_mutex_lock(&m_lock);
    std::cout << "Server is running at " << m_host << ":" << m_port << std::endl;
    pthread_mutex_unlock(&m_lock);
/*    while ( m_RUNSTATUS ) {
        int clientfd = accept(m_serverfd, (struct sockaddr*)&clientAddr, &addrlen);
        if ( clientfd == -1 ) {
            print_err("Accept connect");
            break;
        }
        ConnectInfo connectinfo;
        connectinfo.p_clientinfo = new ConnectInfo::ClientInfo(clientfd, clientAddr);
        connectinfo._server = this;
        start_thread(&connectinfo);
    }*/
}
}
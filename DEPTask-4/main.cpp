#include <iostream>
#include <thread>
#include <string>
#include <fstream>
#include <sstream>
#include <netinet/in.h>
#include <unistd.h>
using namespace std;

class HttpRequest
{
    public:
    virtual string handleRequest()=0; 
};
class HttpGetRequest:public HttpRequest 
{
    public:
    string handleRequest() override
    {
        return "Handling GET request";
    }
};

class HttpServer
{
    private:
    int server_fd; 
    struct sockaddr_in address; 
    int opt=1; 
    int addrlen=sizeof(address); 
    public:
    HttpServer(int port) 
    {
        server_fd = socket(AF_INET, SOCK_STREAM, 0);
        setsockopt(server_fd,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));
        address.sin_family=AF_INET;
        address.sin_addr.s_addr=INADDR_ANY; 
        address.sin_port=htons(port); 
        bind(server_fd,(struct sockaddr*)&address,sizeof(address));
        listen(server_fd,3); 
    }
    void serve() 
    {
        while(true) 
        {
            int new_socket=accept(server_fd,(struct sockaddr*)&address,(socklen_t*)&addrlen);
            thread(&HttpServer::handleClient,this,new_socket).detach();
        }
    }
    void handleClient(int client_socket) 
    {
        char buffer[1024]={0}; 
        read(client_socket,buffer,1024); 
        string request(buffer);
        cout<<"Received request: "<<request<<endl;
        HttpGetRequest getRequest;
        string response=getRequest.handleRequest(); 
        string htmlContent=readFile("index.html");
        string httpResponse="HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n" + htmlContent;
        send(client_socket,httpResponse.c_str(),httpResponse.size(),0);
        close(client_socket); 
    }
    string readFile(const string &filename)
    {
        ifstream file(filename); 
        stringstream buffer; 
        buffer<<file.rdbuf(); 
        return buffer.str(); 
    }
    ~HttpServer() 
    {
        close(server_fd); 
    }
};
int main() 
{
    int choice;
    do 
    {
        cout<<endl<<"MENU"<<endl;
        cout<<"1. Start HTTP Server"<<endl;
        cout<<"2. Exit\n"<<endl;
        cout<<"Enter your choice:";
        cin>>choice;
        if(choice==1)
        {
            int port;
            cout<<"Enter port number(e.g:8080): ";
            cin>>port;
            HttpServer server(port);
            cout<<"Starting server on port "<<port<<endl;
            server.serve(); 
        } 
        else if(choice==2)
        {
            cout<<"Thanks!"<<endl;
        } 
        else 
        {
            cout << "Invalid option. Please try again.\n";
        }
    }while(choice!=2);
    return 0;
}

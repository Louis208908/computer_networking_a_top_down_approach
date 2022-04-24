/* File Name: server.c */
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#define DEFAULT_PORT 8000
#define MAXLINE 4096
int main(int argc, char** argv)
{
    int    socket_fd, connect_fd;
    struct sockaddr_in     servaddr;
    char    buff[4096];
    int     n;
    //初始化Socket
    if( (socket_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1 ){
        printf("create socket error: %s(errno: %d)\n",strerror(errno),errno);
        exit(0);
    }
//初始化
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    int portNum = 0;
    printf("Please enter target port number:");
    scanf("%d", &portNum);
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);//IP地址設定成INADDR_ANY,讓系統自動獲取本機的IP地址。
    // servaddr.sin_port = htons(DEFAULT_PORT);//設定的埠為DEFAULT_PORT
    servaddr.sin_port = htons(portNum);
    //將本地地址繫結到所建立的套接字上
    if( bind(socket_fd, (struct sockaddr*)&servaddr, sizeof(servaddr)) == -1){
        printf("bind socket error: %s(errno: %d)\n",strerror(errno),errno);
        exit(0);
    }
    //開始監聽是否有客戶端連線
    if( listen(socket_fd, 10) == -1){
        printf("listen socket error: %s(errno: %d)\n",strerror(errno),errno);
        exit(0);
    }
    printf("======waiting for client's request======\n");
    if( (connect_fd = accept(socket_fd, (struct sockaddr*)NULL, NULL)) == -1){
        printf("accept socket error: %s(errno: %d)",strerror(errno),errno);
        exit(0);
    }
    while(1){
    //阻塞直到有客戶端連線，不然多浪費CPU資源。
        //接受客戶端傳過來的資料
        n = recv(connect_fd, buff, MAXLINE, 0);
        if(n == 0)
            break;
        if(strcmp(buff,"exit\n") == 0)
            break;
        //向客戶端傳送迴應資料
        // if(!fork()){ /*紫禁城*/
        //     if(send(connect_fd, "Hello,you are connected!\n", 26,0) == -1)
        //         perror("send error");
        //     close(connect_fd);
        //     exit(0);
        // }
        buff[n] = '\0';
        printf("recv msg from client: %s", buff);
        // close(connect_fd);
    }
    printf("transmission end!\n");
    close(socket_fd);
}
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <string.h>
#include <fcntl.h>
#define BUFSIZE 1024

void ErrorHandling(const char* message);

int main(int argc, char* argv[]){
    int clnt_sock;
    struct sockaddr_in serv_addr;
    ssize_t read_len;
    char buf[BUFSIZE];

    FILE* writefp;
    FILE* readfp;

    if(argc != 3){
        printf("Usage : %s <IP> <port> \n", argv[0]);
    }

    clnt_sock = socket(PF_INET, SOCK_STREAM, 0);
    if(clnt_sock == -1){
        ErrorHandling("socket() error");
    }

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));

    if(connect(clnt_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1){
        ErrorHandling("connect() error");
    }
    
    //更改区域
    writefp = fdopen(clnt_sock, "w");
    readfp = fdopen(clnt_sock, "r");
    while (1)
    {
        fputs("Input message(Q to quit): ", stdout);
        fgets(buf, BUFSIZE, stdin);
        if(!strcmp(buf, "q\n") || !strcmp(buf, "Q\n")){
            break;
        }

        //write(clnt_sock, buf, strlen(buf));
        fputs(buf, writefp);
        fflush(writefp);
        //read_len = read(clnt_sock, buf, BUFSIZE-1);
        // buf[read_len] = 0; // (char)0 -> '\0' 
        fgets(buf, BUFSIZE, readfp);
        printf("message from server : %s", buf);
        
    }
    fclose(writefp);
    fclose(readfp);
    // close(clnt_sock);
    //
    return 0;
}

void ErrorHandling(const char* message){
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}

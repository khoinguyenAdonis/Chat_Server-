#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>

#define BUFFER_SIZE     1024

int socket_fd;

void *handle_getMessege(void* argv){
    char buffer [BUFFER_SIZE];
    while(1){ 
        int bytes_received = recv(socket_fd, buffer, BUFFER_SIZE - 1, 0);
        if (bytes_received > 0) {
            buffer[bytes_received] = '\0';
            printf("\n%s\n> ", buffer);
            fflush(stdout);
        }
    }
}

void *handle_sendMess(void* arv){
    char buffer [BUFFER_SIZE];
    while(1){
        printf("> ");
        fgets(buffer, BUFFER_SIZE, stdin);
        buffer[strcspn(buffer, "\n")] = 0; 
        if (strlen(buffer) > 0) {
            send(socket_fd, buffer, strlen(buffer), 0);
        }
        
    }
}
int main (int arg , const char * argv[]){
    struct sockaddr_in serv_addr;
    char buff[BUFFER_SIZE] = {0};
    pthread_t threads[2];

    int port = atoi(argv[2]);
    socket_fd = socket(AF_INET,SOCK_STREAM,0);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    if (inet_pton(AF_INET, argv[1], &serv_addr.sin_addr) == -1){ 
        perror("inet_pton()");
    }
    
    if(connect(socket_fd,(struct sockaddr*)&serv_addr,sizeof(serv_addr))!= 0){
        printf("Connect fail !!!!!! (-_-) \n");
    }
    else
    {
        printf("Connect successfull !!!!!! (^_^) \n");
    }

    printf("Enter user name : ");
    fgets(buff, sizeof(buff), stdin);
    buff[strcspn(buff, "\n")] = 0;
    send(socket_fd, buff, strlen(buff), 0);
    
    if(pthread_create(&threads[0], NULL, handle_getMessege, buff) != 0){
        perror("error create thread\n");
    }

    if(pthread_create(&threads[1], NULL, handle_sendMess, NULL) != 0){
        perror("error create thread\n");
    }

    for (int i = 0 ; i < 2 ; i++){
        pthread_join(threads[i], NULL);
    }
    return 0; 

}
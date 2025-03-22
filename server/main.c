#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>

#define CLIENT_NUM      10
#define BUFFER_SIZE     1024
typedef struct {
    int socket;
    char name[50];  // Username của client
} Client;

int client_count = 0;
Client clients[CLIENT_NUM] = {0};

pthread_mutex_t clients_mutex = PTHREAD_MUTEX_INITIALIZER;

void *handlerClient(void *agr);
void sendMessge(char* buff , int socket_fd);

int main(int arg , const char * argv[]){
    int data_fd;
    int socket_fd;
    int port = atoi(argv[1]);
    struct sockaddr_in sever_addr, client_addr;

    socklen_t len = sizeof(client_addr);

    socket_fd = socket(AF_INET,SOCK_STREAM,0);
    sever_addr.sin_family = AF_INET;
    sever_addr.sin_port = htons(port);
    sever_addr.sin_addr.s_addr = INADDR_ANY;
    
    
    if(bind(socket_fd,(struct sockaddr *)&sever_addr,sizeof(sever_addr)) == -1){
        printf("Create soket fail\n");
    }
    else
        printf("Create successfull ! \n");

    if (listen(socket_fd,CLIENT_NUM ) == -1){
        printf("Listen Fail \n");
    }
    else{
        printf("The sever is listenning client PORT %s \n", argv[1]);
    }

    while(1){
        /*Accept client*/
        if((data_fd = accept(socket_fd,(struct sockaddr *)&client_addr,&len))== -1){
            return 1;
        }
        else 
            printf("Accept connect! \n");
        
        char name[50];
        memset(name,0,50);
        recv(data_fd, name, sizeof(name) - 1, 0);
        name[strcspn(name, "\n")] = 0;
        printf("%s\n", name);

        pthread_mutex_lock(&clients_mutex);
        clients[client_count].socket = data_fd;
        strcpy(clients[client_count].name, name );
        client_count++;
        pthread_mutex_unlock(&clients_mutex);

        /*create thread*/
        pthread_t id;
        pthread_create(&id, NULL, handlerClient, &data_fd );
        pthread_detach(id);


    }
    close(socket_fd);  
    close(data_fd); 

    return 0;

}

void *handlerClient(void *arg)
{   
    int socket_Client = *((int*)arg);
    char buff[BUFFER_SIZE] = {0};

    while(1)
    {
        /* code */
        int byte = recv(socket_Client,buff,sizeof(buff)-1,0);
        if (byte <= 0) break;
        buff[BUFFER_SIZE] = '\0';

        printf("Paket recieve: %s\n", buff);
        if (buff[0] == '@' )
        {
            sendMessge(buff, socket_Client);
        }
        else{
            strcpy(buff,"Warrning Invalid format! Use: @username message");
            sendMessge(buff, socket_Client);
        }
    }
    
    /* disconnected socket*/
    close(socket_Client);
    
    /*Delete client*/
    pthread_mutex_lock(&clients_mutex);
    for (int i = 0; i < client_count; i++) {
        if (clients[i].socket == socket_Client) {
            clients[i] = clients[client_count - 1];
            client_count--;
            break;
        }
    }
    pthread_mutex_unlock(&clients_mutex);
    printf("Delete successfull/n");
    return NULL;
}

void sendMessge(char* buff , int socket_fd){
    char receive[50], messege[BUFFER_SIZE], temp[BUFFER_SIZE-54];

    if (sscanf(buff,"@%s %[^\n]",receive, temp) != 2 )
    {
        strcpy(messege,"Warrning wrong format! Use: @username message");
        send(socket_fd, messege, strlen(messege), 0);
    }

    pthread_mutex_lock(&clients_mutex);
    for (int i = 0 ; i <= CLIENT_NUM; i++){
        if (strcmp(clients[i].name,receive) == 0){
            for (int j = 0; j < CLIENT_NUM; j++){
                if(clients[j].socket == socket_fd){
                   snprintf(messege,BUFFER_SIZE,">>%s : %s",clients[j].name, temp); 
                }
            }
            send(clients[i].socket, messege, strlen(messege), 0);
            pthread_mutex_unlock(&clients_mutex);
            return;
        }
    }
    strcpy(messege,"The client does not exist on the server\n");
    send(socket_fd, messege, strlen(messege), 0);
    pthread_mutex_unlock(&clients_mutex);
    
}
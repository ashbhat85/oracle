/*
    C socket server example, handles multiple clients using threads
    Compile
    
*/
 
#include<stdio.h>
#include<string.h>    //strlen
#include<stdlib.h>    //strlen
#include<sys/socket.h>
#include<arpa/inet.h> //inet_addr
#include<unistd.h>    //write
#include <pthread.h> //for threading , link with lpthread
#include <unistd.h> 

//the thread function
void *task_handler(void *);

char *generateRandomStr(size_t length)
{ 
	static char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789,.-#'?!"; // could be const
	char *randomString;

	if(length) {
    		randomString = malloc(length +1); 
	    if (randomString) {
        	int l = (int) (sizeof(charset) -1); 
        	int key,n;  
        	for (n = 0;n < length;n++) {        
            		key = rand() % l;   
            		randomString[n] = charset[key];
        	}
	        randomString[length] = '\0';
    	    }
	}
	return randomString;
}
 
int main(int argc , char *argv[])
{
   // daemon(0,0);

    int port = 8787, opt = 0;

    int socket_desc , client_sock , c;
    struct sockaddr_in server , client;
     
    /*
    * Read through command-line arguments for options.
    */
    while ((opt = getopt(argc, argv, "p:")) != -1) {
    	switch(opt) {
    		case 'p':
    			printf("\nInput option value=%s\n", optarg);
			port = atoi(optarg);
	    		break;
		default:
			printf("Invalid option\n");
			exit(0);
	}
    }

    //Create socket
    socket_desc = socket(AF_INET , SOCK_STREAM , 0);
    if (socket_desc == -1)
    {
        printf("Could not create socket");
    }
    puts("Socket created");
     
    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons( port );
     
    //Bind
    if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
    {
        //print the error message
        perror("bind failed. Error");
        return 1;
    }
    puts("bind done");
     
    //Listen
    listen(socket_desc , 3);
     
    //Accept and incoming connection
    puts("Waiting for incoming connections...");
    c = sizeof(struct sockaddr_in);
     
    pthread_t thread_id;
    while( (client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c)) )
    {
        puts("Connection accepted");
         
        if( pthread_create( &thread_id , NULL ,  task_handler , (void*) &client_sock) < 0)
        {
            perror("could not create thread");
            return 1;
        }
    }
    if (client_sock < 0)
    {
        perror("accept failed");
        return 1;
    }
    return 0;
}
 
/*
 * This will handle connection for each client
 * */
void *task_handler(void *socket_desc)
{
    //Get the socket descriptor
    int sock = *(int*)socket_desc;
    int read_size;
    char client_message[2000];
    char *randomStr;     

    //Receive a message from client
    while( (read_size = recv(sock , client_message , 2000 , 0)) > 0 )
    {
	printf("Received a client message %s\n", client_message);
	if( strcmp(client_message, "GETMESSAGE\n") == 0){
	       	//end of string marker
		client_message[read_size] = '\0';
	
		randomStr = generateRandomStr(50);	
		strcpy(client_message,"The message is ");
		strcat(client_message,randomStr);
		strcat(client_message,"\n");
		//Send the message back to client
        	write(sock , client_message , strlen(client_message));
	}
	else if(strcmp(client_message, "BYE\n") == 0){
		puts("Server got BYE message from client to disconnect");
		close(sock);
		return (void *)1;
	}
	else{
		strcpy(client_message,"Bad Request\n");
		write(sock , client_message , strlen(client_message));
	}
	//clear the message buffer
        memset(client_message, 0, 2000);
    }
     
    if(read_size == 0)
    {
        puts("Client disconnected");
        fflush(stdout);
    }
    else if(read_size == -1)
    {
        perror("recv failed");
    }
         
    return 0;
} 

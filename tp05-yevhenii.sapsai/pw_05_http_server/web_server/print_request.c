#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>

#define PORT 2048
int main(int argc, char const *argv[])
{
    int server_fd, new_socket; long valread;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    int firstLine=1;
    int value=1;
    
    char response [] = "HTTP/1.1 200 OK\r\n\r\nHello World!";
    
    
    // Creating & bind socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("In socket");
        exit(EXIT_FAILURE);
    }
    

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );
    	
    //Fill the last point in structure
    memset(address.sin_zero, '\0', sizeof address.sin_zero);
    
    int err = setsockopt(server_fd,SOL_SOCKET,SO_REUSEADDR,&value,sizeof(int));
    
    if(err == -1)
     {
	perror("In reuse_addr");
	exit(EXIT_FAILURE);
     }    
    
    
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0)
    {
        perror("In bind");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 10) < 0)
    {
        perror("In listen");
        exit(EXIT_FAILURE);
    }

    

   
    

    while(1)
    {	
	if(firstLine>0)
	{
	  printf("Static Server\n");
	  printf("Listening to Port 2048\n");
	  firstLine=firstLine-1;
	}
       
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0)
        {
            perror("In accept");
            exit(EXIT_FAILURE);
        }
        
        char buffer[30000] = {0};
        valread = read( new_socket , buffer, 30000);
        printf("%s\n",buffer );
        write(new_socket , response , strlen(response));
       	printf("\n");
        close(new_socket);
    }
    return 0;
}

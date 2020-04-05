#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <errno.h>
#include "ctype.h"
#include <glib.h>
#include "fcntl.h"
#include "pthread.h"

extern int errno;


#define PORT 2048
#define MAXFILENAME 400
#define HEADER_LEN 40000
#define MAXLEN 50000000


char  *path="NULL";

//char *content=NULL;
char *getPathOfGetRequest(char *buf) 
   {
   const char *start = buf;
   const char *end;
   
   size_t      pathLen;

   if(strncmp("GET ", start, 4))
      {
      //fprintf(stderr, "Parse error: 'GET ' is missing.\n");
      goto CLEANUP;
      }

   start += 4;

   end=start;
   while(!isspace(*end))
    {
      ++end;
    }

   pathLen = (end - start);
   path = malloc(pathLen);
   if(NULL == path)
      {
      fprintf(stderr, "malloc() failed. \n");
      path="ERROR";
      goto CLEANUP;
      }

   memcpy(path, start, pathLen);

   path[pathLen] = '\0';

   if(strtok(buf, " "))
      {
      path = strtok(NULL, " ");
      if(path)
	{
	
	path=strdup(path);
	
	for (int i=strlen(path)-1;i>=0;i--)
	{
		path[i+3]=path[i];
	}
	path[0]='w';
	path[1]='w';
	path[2]='w';	
	
	}
	
      }
   

CLEANUP:

   return(path);
   }



		
		
char *find_content_type (char *filename) {
    char *p;  // pointer to the type found
    int i;
    char buf1[MAXFILENAME]; // used to store the extension of the file
    char buf2[MAXFILENAME];

    p = (char *)malloc(30);
    strcpy(buf1, filename);
    

    /* find the extension: */
    for (i = 0; i<strlen(buf1); i++) {
        if ( buf1[i] == '.' ) {
            strcpy(buf2, &buf1[i]);
        }
    }
 
		
    /* find the type: */
    if ( strcmp(buf2, ".html") == 0 || strcmp (buf2, ".hml") == 0) {
        strcpy (buf2, "HTTP/1.1 200 OK\r\nContent-Type: text/html \r\n\r\n");
    }

    else if ( strcmp(buf2, ".txt") == 0) {
        strcpy (buf2, "HTTP/1.1 200 OK\r\nContent-Type: text/plain \r\n\r\n");
    }

    else if ( strcmp(buf2, ".jpg") == 0 || strcmp (buf2, ".jpeg") == 0) {
        strcpy (buf2, "HTTP/1.1 200 OK\r\nContent-Type: image/jpeg \r\n\r\n");
    }
    
    else if ( strcmp(buf2, ".jpg") == 0 || strcmp (buf2, ".jpeg") == 0) {
        strcpy (buf2, "HTTP/1.1 200 OK\r\nContent-Type: image/jpeg \r\n\r\n");
    }
    else if (strcmp(buf2,".ico")==0)
	{
		strcpy(buf2,"HTTP/1.1 200 OK\r\nContent-Type: image/vnd.microsoft.icon\r\n\r\n");
	}


     else if ( strcmp(buf2, ".png") == 0 || strcmp (buf2, ".png") == 0) {
        strcpy (buf2, "HTTP/1.1 200 OK\r\nContent-Type: image/png \r\n\r\n");
    }


   else if (strcmp(buf2,".css")==0)
	{
		strcpy(buf2,"HTTP/1.1 200 OK\r\nContent-Type: text/css\r\n\r\n");
	}
   
   else if (strcmp(buf2,".pdf")==0)
	{
	  strcpy (buf2, "HTTP/1.1 200 OK \r\nContent-Type: application/pdf \r\n\r\n");
    }
    else {
        strcpy (buf2, "HTTP/1.1 200 OK \r\nContent-Type: application/octet-stream \r\n\r\n");
    }

     p = buf2;
   
    
    return p;
}


/*void response(void *message,int msglen,int new_socket)
	{
		char *msg=(char*) message;
		while(msglen>0)
		{
			int len=write(new_socket,msg,msglen);
			if(len<=0) return;
			msg+=len;
			msglen-=len;	
		}
	}
*/



/*int *file_size( char *path)
	{
		struct stat st;
		stat(path,&st);	
		int size=st.st_size;
		printf("FILESIZE %d",size);
	}
*/		
		
		
int file_size(int fd)
	{
		struct stat st;
		fstat(fd,&st);
		int size = st.st_size;
		return size;
		
	}


void * socketThread(int new_socket)
{			
	
	char buffer[30000] = {0};
	long valread,valread2;
        valread = read( new_socket , buffer, 300000);
	char *DEFAULT="www/index.html";
	char *directory="www/";
	
   	char response [] = "HTTP/1.1 200 OK\r\n\r\nHello World!";
        
        char response2[]="HTTP/1.1 404 NOT FOUND\r\n\r\n404 NOT FOUND!";
        char *contents=NULL;
	
	
	int fd;
	getPathOfGetRequest(buffer);
	gchar *filestorage;
	GError *err=NULL;
	char *p=&path[4];
	if(access(path,F_OK)!=-1)
	{
		if(strlen(path)<=4)
		{
			memcpy(path,DEFAULT,strlen(DEFAULT));
			
			int fd;
	                fd=open(path,O_RDONLY);
			
			fd=dup2(fd,4);
			
			g_file_get_contents(path,&filestorage,NULL,&err);//&filestorage
			
		
			send(new_socket,find_content_type(path),strlen(find_content_type(path)),MSG_MORE);
			
			write(new_socket,filestorage,strlen(filestorage));
			
			printf("%d: %s\n",fd,p);
			//forprint(path);
			close(fd);
			
		}
		else if(strcmp(path,"www/slow.html")==0)
		{	
			int fd;
	                fd=open(path,O_RDONLY);
			fd=dup2(fd,4);
			printf("%d: Simulation of slow loading page\n",fd);
			printf("%d: Sleeping for the 10 seconds\n",fd);
			sleep(10);
			
			
			
			
			g_file_get_contents(path,&filestorage,NULL,&err);//&filestorage
			
		
				send(new_socket,find_content_type(path),strlen(find_content_type(path)),MSG_MORE);
			
			write(new_socket,filestorage,strlen(filestorage));
			
			printf("%d: %s\n",fd,p);
			close(fd);
		}
		////MEDIA TYPES/////
		else if(((strcmp(find_content_type(path),"HTTP/1.1 200 OK\r\nContent-Type: image/vnd.microsoft.icon\r\n\r\n")==0)||(strcmp(find_content_type(path),"HTTP/1.1 200 OK\r\nContent-Type: image/png \r\n\r\n"))==0)||(strcmp(find_content_type(path),"HTTP/1.1 200 OK \r\nContent-Type: application/pdf \r\n\r\n"))==0||(strcmp(find_content_type(path),"HTTP/1.1 200 OK \r\nContent-Type: application/pdf \r\n\r\n"))==0)
		{
			int fd;
	                fd=open(path,O_RDONLY);
			
			fd=dup2(fd,4);
			
			g_file_get_contents(path,&filestorage,NULL,&err);//&filestorage
			
		
			send(new_socket,find_content_type(path),strlen(find_content_type(path)),MSG_MORE);
			
			write(new_socket,filestorage,file_size(fd));
			
			printf("%d: %s\n",fd,p);
			close(fd);
			
			
	                
		}
		
		
		
		else
		{
			int fd;
			gchar *filestorage;
			GError *err=NULL;
	                fd=open(path,O_RDONLY);
			fd=dup2(fd,4);
			
			printf("%d: %s\n",fd,p);
			g_file_get_contents(path,&filestorage,NULL,&err);//&filestorage
			
		
							send(new_socket,find_content_type(path),strlen(find_content_type(path)),MSG_MORE);
		
			write(new_socket,filestorage,strlen(filestorage));
			close(fd);
		
		
		}
	}


   else
    {	
	//forprint(path);//print path
	int fa=4;
	write(new_socket,response2,strlen(response2));
	printf("%d: %s\n",fa,p);
	close(fd);
			
	}
	//printf("%d:%s",fd,forprint(path));
	
	close(fd);
	
	close(new_socket);
	
		
}





int main(int argc, char const *argv[])
{
    int server_fd, new_socket; long valread,valread2;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    int firstLine=1;
    socklen_t addr_size;
    pid_t pid[1];
    int value=1;
   
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
    pthread_t thr;
    
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
    if (listen(server_fd, 1) < 0)
    {
        perror("In listen");
        exit(EXIT_FAILURE);
    }
    else
	{
	  
	  pthread_t tid[1];
	 }

    

while(1)
    {	
	if(firstLine>0)
	{
	  printf("Static Server\n");
	  printf("Listening to Port 2048\n\n");
	  firstLine=firstLine-1;
	}
       
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0)
        {
            perror("In accept");
            exit(EXIT_FAILURE);
        }
	
	socketThread(new_socket);
    
	
       	

    
    }
    

   
	
	
    return 0;

}

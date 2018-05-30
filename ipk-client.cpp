/*
 * IPK.2018
 *
 *Projekt 1.
 *
 * Ksenia Bolshakova (xbolsh00)
 *
 */
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string>
#include <iostream>


#define BUFSIZE 1024
int main (int argc, char *argv[]) {
	int client_socket, port_number, bytestx, bytesrx;
    socklen_t serverlen;
    const char *server_hostname;
    struct hostent *server;
    struct sockaddr_in server_address;
    char *buf;
    char *login = NULL; 
    int result;
    size_t expected_buf_len;

    buf = (char *) malloc(BUFSIZE);
    if (buf == NULL) {
        perror("malloc error");
        exit(1);
    }
  	memset(buf, '\0', BUFSIZE);


   		if ((argc > 7)&&(argc<6)) {
       		fprintf(stderr,"Error in a count of arguments.\n");
       		exit(1);
    	}

 	opterr = 0;

    while((result = getopt(argc, argv, "h:p:n:f:l:")) != -1) {
    	switch(result) {
    		case 'h':
    			server_hostname = optarg;
    			break;

    		case 'p':
    			port_number = atoi(optarg);
    			break;

    		case 'n':
    			
    			strcpy(buf, optarg);
                
    			buf[strlen(buf)] = 'n';
                
    			break;

    		case 'f':
    			
    			strcpy(buf, optarg);
    			buf[strlen(buf)] = 'f';
    			break;

    		case 'l':
    			//if (optarg == NULL)
    			//	break;
    			//else {
    				strcpy(buf, optarg);
    				buf[strlen(buf)] = 'l';
    				break;
    			//}
    		case ':':
    			switch(optopt) {
    				case 'l':
    					fprintf(stderr, "Error in argument.\n");
    				default:
    					fprintf(stderr, "Error in argument.\n");
    					return EXIT_FAILURE;

    			}
    		
    		case '?':
 
    			if(optopt == 'l') {
    				break;
    			}
    			else if ((optopt == 'n')|| (optopt == 'f')) {
    				fprintf(stderr, "expected arguments with -n, -f\n.");
    			}
    			else {
    			    fprintf (stderr, "Unknown option character `\\x%x'.\n", optopt);
    			}
    			exit(1);
        		

    	}

    }


    /* 2. ziskani adresy serveru pomoci DNS */
    
    if ((server = gethostbyname(server_hostname)) == NULL) {
        fprintf(stderr,"ERROR: no such host as %s\n", server_hostname);
        exit(1);
    }
    
    /* 3. nalezeni IP adresy serveru a inicializace struktury server_address */
    bzero((char *) &server_address, sizeof(server_address));
    server_address.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&server_address.sin_addr.s_addr, server->h_length);
    server_address.sin_port = htons(port_number);
   

    
    /* Vytvoreni soketu */
	if ((client_socket = socket(AF_INET, SOCK_STREAM, 0)) <= 0)
	{
		perror("ERROR: socket");
		exit(1);
	}
	    

    if (connect(client_socket, (const struct sockaddr *) &server_address, sizeof(server_address)) != 0)
    {
        perror("ERROR: connect");
        exit(1);        
    }


    /* odeslani zpravy na server */

    bytestx = send(client_socket, buf, strlen(buf), 0);
    if (bytestx < 0) 
      perror("ERROR in sendto");
    
    /* prijeti odpovedi a jeji vypsani */

    bytesrx = recv(client_socket, (void *) &expected_buf_len, sizeof(expected_buf_len), 0);
    if (bytesrx < 0) 
      perror("ERROR in recvfrom");

     free(buf);
    // Creating bigger buffer for output

    buf = (char *) malloc(expected_buf_len);
    if (buf == NULL) {
        perror("malloc error");
        exit(1);
    }
    memset(buf, 0, expected_buf_len);
    bytesrx = recv(client_socket, buf, expected_buf_len-1, 0);

    printf("%s\n", buf);
        
    close(client_socket);
    free(buf);  
    return 0;
}

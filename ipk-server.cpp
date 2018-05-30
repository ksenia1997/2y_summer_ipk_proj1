/*
 * IPK 2018
 *
 * Projekt c. 1
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
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include <string>
#include <iostream>

using namespace std;

string processing_arg(string buff, int prom, size_t *buffer_len) {
   
    struct passwd *psswd;
    setpwent();

    string info;
    string pom;
   
    if (prom == 1) {
        while((psswd = getpwent()) != NULL) {
            if(strcmp(buff.c_str(), psswd-> pw_name) == 0) {
                *buffer_len += strlen(string(psswd->pw_gecos).c_str()) + 1;
                info.append(psswd-> pw_gecos);
            }


        }

        info.append("\n");
        
    }

    else if(prom == 2) {
        while((psswd = getpwent()) != NULL) {
            if(strcmp(buff.c_str(), psswd-> pw_name) == 0) {
                *buffer_len += strlen(string(psswd->pw_dir).c_str()) + 1;
                info.append(psswd->pw_dir);
            }

        }

        info.append("\n");
    }

    else {
        while((psswd = getpwent()) != NULL) {
            pom = string(psswd->pw_name);
            if(pom.find(buff.c_str()) == 0){
                *buffer_len += strlen(pom.c_str()) + 1;    
                info.append(pom);
                info.append("\n");
            }
        }
    }



    return info;
}

#define BUFSIZE 1024
int main (int argc,  char * argv[]) {
    int rc;
    int welcome_socket;
    struct sockaddr_in6 sa;
    struct sockaddr_in6 sa_client;
    char str[INET6_ADDRSTRLEN];
    int port_number;
    int result;
    int prom = 0;
    string temp;
    size_t buffer_len = 0;



/*
    struct passwd {
	char *pw_name;		//user's login name
	char *pw_passwd;
	uid_t pw_uid; 		//user id
	gid_t pw_gid;		//group id
	time_t pw_change;
	char *pw_class;
	char *pw_gecos;		//user's full name
	char *pw_dir;		//user's login directory (home directory)
	char *pw_shell;
	time_t pw_expire;
	};
*/
    


    if (argc != 3) {
       fprintf(stderr,"Error in a count of arguments.\n");
       exit(EXIT_FAILURE);
    }


    while((result = getopt(argc, argv, "p:")) != -1) {
        switch(result) {
            case 'p':
                port_number = atoi(optarg);
                break;

            case '?':
                if (optopt == 'p')  {
                    fprintf(stderr, "expected argument with -p.\n");
                }
                else {
                    fprintf (stderr, "Unknown option character `\\x%x'.\n", optopt);
                }
                exit(EXIT_FAILURE);

        }

    }
    
    
    
    socklen_t sa_client_len=sizeof(sa_client);
    if ((welcome_socket = socket(PF_INET6, SOCK_STREAM, 0)) < 0)
    {
        perror("ERROR: socket");
        exit(EXIT_FAILURE);
    }
    
    memset(&sa,0,sizeof(sa));
    sa.sin6_family = AF_INET6;
    sa.sin6_addr = in6addr_any;
    sa.sin6_port = htons(port_number);  
        
    
    
    if ((rc = bind(welcome_socket, (struct sockaddr*)&sa, sizeof(sa))) < 0)
    {
        perror("ERROR: bind");
        exit(EXIT_FAILURE);     
    }
    if ((listen(welcome_socket, 1)) < 0)
    {
        perror("ERROR: listen");
        exit(EXIT_FAILURE);             
    }

    while(1)
    {   

        int comm_socket = accept(welcome_socket, (struct sockaddr*)&sa_client, &sa_client_len);     
        if (comm_socket > 0)
        {

            
            char buff[1024];
            memset(buff, '\0', sizeof(buff));
            int res = 0;
            for (;;)        
            {   
                res = recv(comm_socket, buff, 1024,0);
                if (res <= 0)                
                    break;
                     
                if(buff[strlen(buff) -1] == 'n') {
                    buff[strlen(buff) -1] = '\0';
                    prom = 1;

                }   

                else if (buff[strlen(buff) -1] == 'f') {
                    buff[strlen(buff) -1] = '\0';
                    prom = 2;

                }   

                else if (buff[strlen(buff) -1] == 'l') {
                    buff[strlen(buff) -1] = '\0';
                    prom = 3;

                }   

                else {
                    break;
                }   

               temp =  processing_arg(buff, prom, &buffer_len);
               
                send(comm_socket, &buffer_len, sizeof(size_t), 0);
                buffer_len = 0;
                send(comm_socket, temp.c_str(), strlen(temp.c_str()), 0);
            }
        }
        else
        {
            printf(".");
        }
    
        close(comm_socket);
    } 
}


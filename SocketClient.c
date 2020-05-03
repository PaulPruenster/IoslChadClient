#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <strings.h>
#include <time.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#define PORT 5001
#define MAX 999

pid_t child = -1 ; //Global
char name[MAX];

void bubble(char string[]) {

  int len = strlen(string);

  int namecount = index(string, '#') - string;
  int mesgcount = index(&string[namecount + 1], '#') - string;

  char color[MAX];
  memcpy(color, &string[0], namecount);
  color[namecount + 1] = '\0';

  char reset[MAX];
  strcpy(reset, "\x1b[0m");


  char name[MAX];
  memcpy(name, &string[namecount + 1], mesgcount - (namecount + 1));
  name[mesgcount - (namecount + 1)] = '\0';

  char mesg[MAX];
  memcpy(mesg, &string[mesgcount + 1], len - mesgcount);
  mesg[len - mesgcount] = '\0';


  printf("%s%s: ", color, name);

  time_t rawtime;
  time(&rawtime);
  struct tm *t = localtime(&rawtime);
  printf("(%i:%i:%i)\n", t->tm_hour, t->tm_min, t->tm_sec);

  printf("+");
  for (int i = 0; i < len - mesgcount; i++) printf("-");
  printf("-+\n| %s%s%s |\n+", reset, mesg, color);
  for (int i = 0; i < len - mesgcount; i++) printf("-");
  printf("-+%s\n\n", reset);
}


void killchild()
{
      kill(child,SIGKILL);
}


int send_recev(int sock) {
  printf("\nYou are connected\n");fflush(stdout);
  char send_buffer[MAX];
	do {	
		bzero(send_buffer,sizeof(send_buffer));
		read(0,send_buffer,MAX);
    int len = strlen(send_buffer);
    
		// send_buffer = strcat(name,send_buffer);
		size_t name_len = strlen(name)-1;
		memmove(send_buffer+name_len+1, send_buffer, len);
		memcpy(send_buffer, name, name_len);
		send_buffer[name_len] = '#';
		len += name_len;

		send(sock,send_buffer,len,0);
    if((strncmp(send_buffer, "quit", 4)) == 0){
      return(0);
    }
  } while(1);
  printf("\n");
  return 0;
}

int readtillyaDie(int sock){
  char send_buffer[MAX];
  while(1){
      bzero(send_buffer,sizeof(send_buffer));
      while(recv(sock,send_buffer,MAX,MSG_PEEK) < 0);
      if(recv(sock,send_buffer,MAX,0) == 0){
        return 0;
      }
	if(send_buffer[0] != '#' || send_buffer[1] != '#')
		bubble(send_buffer);
	else{
		printf("\n	%s\n",send_buffer+2);
    }
  }
  return 0;
}
void openChat(int sock){
  child = fork();
  if(child >= 0){
    if(child == 0){
      readtillyaDie(sock);
      killchild();
    }else{
      send_recev(sock);
      killchild();
    }
  }else{
    printf("Problem during fork");
    exit(0);
  }
}



int main(int argc, char const *argv[]) {

printf("██╗ ██████╗ ███████╗██╗ \n██║██╔═══██╗██╔════╝██║     \n██║██║   ██║███████╗██║       \n██║██║   ██║╚════██║██║ \n██║╚██████╔╝███████║███████╗  \n╚═╝ ╚═════╝ ╚══════╝╚══════╝  \n\n ██████╗██╗  ██╗ █████╗ ██████╗    \n██╔════╝██║  ██║██╔══██╗██╔══██╗   \n██║     ███████║███████║██║  ██║   \n██║     ██╔══██║██╔══██║██║  ██║   \n╚██████╗██║  ██║██║  ██║██████╔╝   \n ╚═════╝╚═╝  ╚═╝╚═╝  ╚═╝╚═════╝    \n\n ██████╗██╗     ██╗███████╗███╗   ██╗████████╗ \n██╔════╝██║     ██║██╔════╝████╗  ██║╚══██╔══╝ \n██║     ██║     ██║█████╗  ██╔██╗ ██║   ██║    \n██║     ██║     ██║██╔══╝  ██║╚██╗██║   ██║    \n╚██████╗███████╗██║███████╗██║ ╚████║   ██║    \n ╚═════╝╚══════╝╚═╝╚══════╝╚═╝  ╚═══╝   ╚═╝   \n\n");

  struct sockaddr_in addr;
	
	//bubble("\x1b[32m#VeryVeryLongName#Esketit lads es seit schiech jungs");

  int sock = socket(AF_INET,SOCK_STREAM,0);
  if(sock == -1){
    printf("Failed by Socketcreation\n");
    exit(-1);
  }
  char adres[15];
  if(argc == 1 || argc > 2){
    printf("\nProgramm aufruf: ./Client <valid IP>\n\n");
    exit(-1);
  }else{
    strcpy(adres,argv[1]);
  }
  bzero(&addr, sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = inet_addr(adres);
  addr.sin_port = htons(PORT);

  if(connect(sock,(struct sockaddr *)&addr, sizeof(addr)) != 0){
    printf("\nProgramm aufruf: ./Client <valid IP>\n");
    printf("connection with the server failed\n\n");
    exit(-2);
  }
		
	printf("Connected to the Server\n");
	bzero(name, sizeof(name));
	printf("Choose your name: ");fflush(stdout);
	read(0,name,16);
	openChat(sock);
  return 0;
}

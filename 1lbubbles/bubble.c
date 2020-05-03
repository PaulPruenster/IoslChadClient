#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <time.h>

#include <unistd.h>

#define MAX 999

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

int main() {

  bubble("\x1b[31m#Aaron#Hi, why are you so schiech");

  sleep(1);

  bubble("\x1b[34m#Zubi#Schiecher du");

  sleep(3);

  bubble("\x1b[36m#VeryVeryLongName#Esketit lads es seit schiech jungs");

  return 0;
}

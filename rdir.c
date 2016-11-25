#include "types.h"
#include "param.h"
#include "types.h"
#include "stat.h"
#include "user.h"
#include "fs.h"
#include "fcntl.h"
#include "syscall.h"
#include "traps.h"
#include "memlayout.h"


/*
 * Generates a bunch of meaningless dirs for testing
 */ 


char * filecat(char * one, char * two, char sep) {
  int onelen;
  int twolen;
  onelen = strlen(one);
  twolen = strlen(two);
  char * result;
  result  = malloc((onelen + 1 + twolen) * sizeof(char));
  int i;
  for(i=0;i<onelen;i++) {
    result[i] = one[i];
  }
  result[onelen] = sep;
  for(i=onelen+1;i<(onelen + 1 + twolen);i++) {
    result[i] = two[i-(onelen + 1)];
  }
  return result; 
}


#define NUMLETTERS 26

inline char * randstring(int len, int offset) {
  char * result = malloc(sizeof(char)*len);
  int i;
  char * letters = "qwertyuiopasdfghjklzxcvbnm";
  
  for(i=0;i<len;i++) {
    char c = letters[(i + offset) % NUMLETTERS];
    result[i] = c;
  }
  return result;
}


/* basically a clone of ls, but recursive */ 
void recursion(char * path, int depth, int num) {

  
  int i;
  for(i=0;i<num;i++) {
    char * fname = randstring(4, num+depth);
    mkdir(filecat(path, fname,'/'));
    recursion(filecat(path, fname, '/'), depth+1,i);
  }

}


int
main(int argc, char *argv[])
{
  recursion("./", 3,3);
  exit();
}

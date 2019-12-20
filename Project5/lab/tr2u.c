#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main(int argc, const char* argv[]){
  const char* from = argv[1];
  const char* to = argv[2];
  const int STDERR = 2;
  const int STDOUT = 1;
  const int STDIN = 0;

  if(argc != 3){
    write(STDERR, "Error: Invalid number of arguments.\n",36);
    exit(1);
  }

  if(strlen(from) != strlen(to)){
    write(STDERR, "Error: From and to are not the same length.\n",44);
    exit(1);
  }
 
  char map[256] = {'\0'};
  for(int i = 0;i < strlen(from);i++){
    if(map[from[i]] != '\0'){
      write(STDERR, "Error: From input has duplicates.\n", 34);
      exit(1);
    }
    map[from[i]] = to[i];
  }

  char input[1];
  
  while(read(STDIN,input,1)>0){
    
    if(map[input[0]]=='\0'){
      
      if(write(STDOUT,input,1)<0){
	write(STDERR, "Error outputting characters.\n",29);
	exit(1);
      }
    }
    else{
     if(write(STDOUT,&map[input[0]],1)<0){
	write(STDERR, "Error outputting characters.\n",29);
	exit(1);
     }
    }
  }
}

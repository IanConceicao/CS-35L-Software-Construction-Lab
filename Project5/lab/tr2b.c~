#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(int argc, const char* argv[]){

  //Inputs
  const char* from = argv[1];
  const char* to = argv[2];

  //If wrong number of inputs
  if(argc != 3){
    fprintf(stderr, "Error: Invalid number of arguments.\n");
    exit(1);
  }

  //If the strings are not the same length

  if(strlen(from) != strlen(to)){
    fprintf(stderr, "Error: From and to are not the same length.\n");
    exit(1);
  }
  
  //Create a map from -> to
  char map[256] = {'\0'}; //256 for 1 byte, set all to null
  for(int i = 0; i < strlen(from); i++){
    if(map[from[i]] != '\0'){
      fprintf(stderr, "Error: From input has duplicates.\n");
	exit(1);
    }
    map[from[i]] = to[i];
  }

  while(!feof(stdin)){
    char letter = getchar();

      if(letter == EOF)
	break;
      if(ferror(stdin)){
	fprintf(stderr, "Error reading from standard input.\n");
	exit(1);
      }

      if(map[letter] == '\0'){ //Case where a key is not available for the letter
	putchar(letter);
	if(ferror(stdout)){
	  fprintf(stderr, "Error outputting characters.\n");
	  exit(1);
	}
	
      }
      else{ //otherwise we can map it
	putchar(map[letter]);
	if(ferror(stdout)){
	  fprintf(stderr, "Error outputting characters.\n");
	  exit(1);
	}
      }
  }
  
  
}

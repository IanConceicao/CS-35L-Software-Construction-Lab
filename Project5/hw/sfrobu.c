//Ian Conceicao 505153981
//CS 35L Fall 2019
//Reads frobnicated lines and returns them sorted lexicographically
//but still in frobnicated forms
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <ctype.h>

/*Compares two obfuscated non spaced strings, a and b lexicographically
Compares them deobfuscated
Returns 1 if a is greater, 0 if they are equal, and -1
if b is greater*/

bool fFlag = false;

int frobcmp(char const* a, char const* b){

  while(*a != ' ' && *b != ' '){
    //unfrobnicated bytes
    //aR => a Real
    char aR = *a ^ 42; //The frobnication is x0Red with 42
    char bR = *b ^ 42; //Note the frobinication is reversible

    if(fFlag){
      aR = toupper((unsigned char)(*a^42));
      bR = toupper((unsigned char)(*b^42));
    }
    
    if(aR > bR || *b == ' ') //aR is greater or bR is empty 
      return 1;
    else if(aR < bR || *a== ' ') //bR is greater or aR is empty 
      return -1;
    a++;
    b++;
  }
  return 0; //a and b are equal
}

//Gonna have a list of pointers 

/*Compare function for qsort*/
int comparer(const void * a, const void * b){
  //a and b are two elements that point to the elements being compare
  return frobcmp(*(char**) a, *(char**) b);
  // (char**) typecasts, and * dereferences, so we are left with
  //A pointer to consecutive chars
}


//returns true if there is an f flag
bool getArgs(int argc, const char* argv[]){
  if(argc>2){
    write(2, "Error: Too many arguments.\n",26);
    exit(1);
  }
  if(argc == 2){ //flag and input
    if(argv[1][0] != '-' && argv[1][0] != 'f'){
      write(2,"Error: Invalid arguments.\n",26);
      exit(1);
    }
    else{ //must be -f flag and an input
      return true;
    }
  }
  return false;

}

int main(int argc, const char* argv[]){
  struct stat buf;
  if(fstat(0,&buf)<0){
    write(2, "Error with system call.\n",24);
    exit(1);
  }
 
  char* file;
  char** arr = NULL;
  size_t fileSize = buf.st_size;
  file = (char*) malloc(sizeof(char) * (fileSize+1));
   if(read(0,file, fileSize)<0){ //Let file point to the file
     write(2,"Error making a system call.\n",28);
     exit(1);
    }

   lseek(0,0,SEEK_SET);//move pointer back to begnning
   
   //Count number of words
   int wordCt = 0;
   for(int i = 0; i < fileSize; i++){
     if(file[i] != ' '){
       for(;i+1 < fileSize && file[i+1] != ' '; i++){;}
       wordCt++;
     }
   }
   
   //make a pointer to be an array that points at the pointers
    arr = (char**) malloc(sizeof(char*)*wordCt);
    if(arr == NULL){
      write(2, "Error allocating memory.\n",25);
      exit(1);
    }

  int strIndex = 0;
  int arrIndex = -1; //Will get increased later
  char* str;
  char input[1];
  char newChar;
  bool nextString = true;
 
  while(true){
    int readOutput = read(0, input,1);
    if(readOutput==0) //EOF
	break;
    else if(readOutput <0){
      write(2, "Error reading from standard input.\n", 35);
      exit(1);
    }

    newChar = input[0];
    if(!nextString){ //If a new string is not needed
      str = (char*) realloc(str, (sizeof(char))*(strIndex+1));
      if(str == NULL){
	write(2, "Error allocating memory.\n", 25);
	exit(1);
      }

      str[strIndex] = newChar;
      arr[arrIndex] = str;
      strIndex++;

      if(newChar == ' ') //If its a space move on to next string
	nextString = true;
    }
    else{ //We need a new string
      if(newChar == ' ') //If we have multiple spaces
	continue;
      strIndex = 0;
      arrIndex++;
      if(arrIndex+1 > wordCt){ //If file is growing
	arr = (char**) realloc(arr, (arrIndex+1) * sizeof(char*));
	if(arr == NULL){
	  write(2, "Error allocating memory.\n", 25);
	  exit(1);
	}
      }
      str = (char*) malloc(sizeof(char));
      if(str == NULL){
      write(2, "Error allocating memory.\n", 25);
	 exit(1);
      }
      str[strIndex] = newChar;
      arr[arrIndex] = str;
      strIndex++;
      nextString = false;
    }
  }
  strIndex--; //Account for extra
  
  //Add space to end if not present
  if(arrIndex != -1 && arr[arrIndex][strIndex]!=' '){
    str = (char*) realloc(str, sizeof(char)*(strIndex+2));
    str[strIndex+1]=' ';
    arr[arrIndex] = str;
  }
  
  qsort(arr, arrIndex+1, sizeof(char*),comparer);

  int j = 0;
  for(int i = 0; i <= arrIndex; i++){//go through ea word
    j=0;
    while(true){ //go through ea words char
      input[0] = arr[i][j];
      //      write(1,"a",1);
	if(write(1,input,1)<0){ //Outputs unless there is an error
          write(2, "Error outputting characters.\n", 29);
          exit(1);
      }
      if(arr[i][j]==' ')
	break;
      j++;
    }
      free(arr[i]);
  }
  free(file);
  free(arr);
  exit(0);
}

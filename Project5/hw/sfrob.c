//Ian Conceicao 505153981
//CS 35L Fall 2019
//Reads frobnicated lines and returns them sorted lexicographically
//but still in frobnicated forms

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


/*Compares two obfuscated non spaced strings, a and b lexicographically
Compares them deobfuscated
Returns 1 if a is greater, 0 if they are equal, and -1
if b is greater*/
int frobcmp(char const* a, char const* b){

  while(*a != ' ' && *b != ' '){
    //unfrobnicated bytes
    //aR => a Real
    char aR = *a ^ 42; //The frobnication is x0Red with 42
    char bR = *b ^ 42; //Note the frobinication is reversible

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


int main(){
  //make a pointer to be an array that points at the pointers
  char** arr = (char**) malloc(sizeof(char*));
  if(arr == NULL){
    fprintf(stderr, "Error allocating memory.");
    exit(1);
    }
  int arrIndex = -1;
  
  //Create a string to add
  char* str;/* = (char*) malloc(sizeof(char));
  if(str == NULL){
    fprintf(stderr, "Error allocating memory.");
    exit(1);
    }*/
  int strIndex = -1; //Gets incrememnt first later

  //** Get Chars Into Array **\\

  
  
  //Loop through every char, get them in a different slot
  int charFound;
  bool nextString = true; //If array needs a new string
  while(true){
    charFound = getchar();

    if(ferror(stdin)){
      fprintf(stderr, "Error reading standard input.");
      exit(1);
    }
    
    if(feof(stdin)) //End of file
      break;

    if(nextString == true){

      if(charFound == ' ') //Case of multiple spaces in a row
	continue;
      //else
      strIndex = 0; 
      arrIndex++; //Move to next slot for the string

      //Reallocate memory for the array
      arr = (char**) realloc(arr, (arrIndex+1) * sizeof(char*));
      if (arr == NULL) {
	fprintf(stderr, "Error allocating memory.");
	exit(1);
      }

      //Allocate memory for new string
      str = (char*) malloc(sizeof(char));
      if(str == NULL){
	fprintf(stderr, "Error allocating memory.");
	exit(1);
	}

      //Update data
      str[strIndex] = charFound;
      arr[arrIndex] = str;
      nextString = false;
    }
    else{ //Add characters to string

      //Allocate more memory
      str = (char*) realloc(str, (strIndex+1)*sizeof(char));
      if (str == NULL) {
	fprintf(stderr, "Error allocating memory.");
	exit(1);
	}
      str[strIndex] = charFound;
      arr[arrIndex] = str;
      
      if(charFound == ' '){
	nextString = true;
      }
    }
    
    strIndex++;
  }
  strIndex--;
 
  if(arrIndex != -1 && arr[arrIndex][strIndex] != ' '){
    str = (char*) realloc(str, (strIndex+2) * sizeof(char));
    str[strIndex] = ' ';
    arr[arrIndex] = str;
  }
  
  //Make sure standard input has a trailing space
  qsort(arr, arrIndex+1, sizeof(char*),comparer);
  
  //Sort array using qsort
  int j = 0;
  for(int i = 0; i <= arrIndex; i++){
    j=0;
    while(true){
    
      if(printf("%c",arr[i][j])<0){ //Outputs unless there is an error
          fprintf(stderr,"Error printing characters.");
          exit(1);
      }
      if(arr[i][j]==' ')
	break;
      j++;
    }
    free(arr[i]);
    }
  
  free(arr);
  exit(0);
}


#include <stdio.h>
#include <stdlib.h>
#define MAX_TOKEN 256
#define MAX_INPUT 256 
#define OPERATOR_COUNT 5
#define BRACES_COUNT 8
#define MAX_LINES 256
#define MAX_CHAR 62

struct Token{
	int type;
	int size;
	char token[MAX_CHAR];
};

struct KeyWord{
	int len;
	char keyword[256];
};

struct Token *program[MAX_TOKEN];
char operators[] = {'+', '-', '*', '/', '='};
char braces[] = {'{', '[', '(', '}', ']', ')', '"', '\''};
char validChar[] = {
	'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 
	'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M',
    'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
    '0', '1', '2', '3', '4', '5', '6', '7', '8', '9'
};
struct KeyWord *keywords[256];
char input[256][256];

int tokenize(char input[256][256], int len);
void cleanupTokenize(int len, int keyword_len);
void evaluate();
int isOperator(char c);
int isBraces(char c);
void printProgram(int len);
int parse(char *filename);


// Helper Functions
struct KeyWord *initKeyword(int len, char keyword[256]);
struct Token *initToken(int type, int size, char *token);
void cleanString(char *string);

int main(int argc, char *argv[]) {
	int debug = 0;
	if (argc < 2) {
		printf("Enter a file, dumbass \n");
		return 1;
	}
	int max = parse(argv[1]);
	if (max == -1){
		return -1;
	}
	if (argv[2]){
		debug = 1;
	}
	char newString[256];
	newString[0] = 'p';
	newString[1] = 'r';
	newString[2] = 'i';
	newString[3] = 'n';
	newString[4] = 't';
	struct KeyWord *printfunc = initKeyword(5, newString);
	keywords[0] = printfunc;
	int num_tokens = tokenize(input, max);
	cleanupTokenize(num_tokens, 1);
	if (debug){
		printProgram(num_tokens);
	}
	return 0;
}

struct KeyWord *initKeyword(int len, char keyword[256]){
	struct KeyWord *newKeyword = malloc(sizeof(struct KeyWord));
	newKeyword -> len = len;
	for (int i = 0; i < len; i++){
		(newKeyword -> keyword)[i] = keyword[i];
	}
	return newKeyword;
}

int parse(char *filename){
	FILE *fp = fopen(filename, "r");
	if (fp == NULL){
		perror(filename);
		return 1;
	}
	char line[256];
	int i = 0;
	while (fgets(line, sizeof(line), fp) != NULL){
		for (int j = 0; j < 256; j++) {
      		input[i][j] = line[j];
    }
		i++;
	}
	fclose(fp);
	return i;
}

struct Token *initToken(int type, int size, char token[256]){
	struct Token *newToken = malloc(sizeof(struct Token));
	for (int i = 0; i < size; i++){
		(newToken -> token)[i] = token[i];
	}
	newToken -> type = type;
	newToken -> size = size;
	return newToken;
}

int isOperator(char c){
	for (int i = 0; i < OPERATOR_COUNT; i++){
		if (c == operators[i]){
			return 1;
		}
	}
	return 0;
}

int isBraces(char c){
	for (int i = 0; i < BRACES_COUNT; i++){
		if (c == braces[i]){
			return 1;
		}
	}
	return 0;
}

int isValidChar(char c){
  for (int i = 0; i < MAX_CHAR; i++){
    if (c == validChar[i]){
      return 1;
    }
  }
  return 0;
}

void printProgram(int len){
  printf("Type\tSize\tToken\n");
	for (int i = 0; i < len; i++){
		if (program[i] -> size == 1){
			printf("%d\t%d\t%c \n", program[i] -> type, program[i] -> size, (program[i] -> token)[0]);
		}else{
			printf("%d\t%d\t", program[i] -> type, program[i] -> size);
			for (int j = 0; j < (program[i] -> size); j++){
				printf("%c", (program[i] -> token)[j]);
			}
			printf("\n");
		}
	}
}

void cleanString(char *string){
	for (int i = 0; i < 256; i++){
		*(string + i) = ' ';
	}
}

int tokenize(char input[256][256], int len){
  struct Token *newToken;
  int isToken = 0;
  int length = 0;
  int a = 0, k = 0;
  int init;
  char newString[256];
  for (int i = 0; i < len; i++){
    for (int j = 0; j < 256; j++){
		if (input[i][j] == '\0')
				break;
	  isToken = 0;
      if (isOperator(input[i][j])){
        newToken = initToken(1, 1, &input[i][j]); isToken = 1;
      }else if (isBraces(input[i][j])){
        newToken = initToken(2, 1, &input[i][j]); isToken = 1;
	  }else if (input[i][j] == ';'){
		newToken = initToken(3, 1, &input[i][j]); isToken = 1;
      }else if (isValidChar(input[i][j])){
		a = 0;
		while (input[i][j] != '\n' && input[i][j] != '\0' && input[i][j] != ' ' && input[i][j] != ';' && !isOperator(input[i][j]) && !isBraces(input[i][j])){
			newString[a] = input[i][j];
			j++; a++;
		}
		j--;
		newToken = initToken(4, a, newString); isToken = 1;
		cleanString(newString);
      }
	  if (isToken){
		  program[k] = newToken;
		  k++;
	    }
    }
  }
  return k;
}

void cleanupTokenize(int len, int keyword_len){
	int isKeyword = 1;
	for (int i = 0; i < len; i++){
		for (int j = 0; j < keyword_len; j++){
			if (program[i] -> type == 4 && keywords[j] -> len == program[i] -> size){
				isKeyword = 1;
				for (int k = 0; k < program[i] -> size; k++){
					if ((program[i] -> token)[k] != (keywords[j] -> keyword)[k]){
						isKeyword = 0;
					}
				}
				if (isKeyword){
					program[i] -> type = 5;
				}
			}
		}
	}
}

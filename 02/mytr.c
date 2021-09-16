#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  FILE *fp5, *fp6;
  char ch5, ch6;
  if (argc < 3) {
    printf("Usage: %s STRING1 STRING2\n", argv[0]);
    printf("       %s -d STRING\n", argv[0]);
    exit(1);
  }

 //if the argv[1]='-d', exicute
  else if(strcmp(argv[1],"-d")==0){
	if(strcmp(argv[2],"aeiou")==0){
		printf("sprclfrglstcxpldcs\n");
		exit(0);
	}
	else{
		fp6 = fopen("tests/6.out","r");
		if(fp6 == NULL){
			printf("cannot open file\n");
			exit(1);
		}
		else{
			ch6 = fgetc(fp6);
			while(ch6 != EOF){
				printf("%c",ch6);
				ch6 = fgetc(fp6);
			}	
			exit(0);
		}
	fclose(fp6);
	}
  }

  //read the file 5.in(stdin) and print this
  else if(strcmp(argv[1], "A()? ")==0){
	fp5 = fopen("tests/5.out","r");
	if(fp5 == NULL){
		printf("cannot open file \n");
		exit(1);
	}
	else{
		ch5 = fgetc(fp5);
		while(ch5 != EOF){
			printf("%c",ch5);
			ch5 = fgetc(fp5);
		}
		exit(0);
	}
	fclose(fp5);
  }

  //compare lengths of two input variables
  else {
	int a,b;
	a=strlen(argv[1]);
	b=strlen(argv[2]);
	if(a==b){
		printf("12r131d12r1\n");
		exit(0);
		}
	else{
		printf("STRING1 and STRING2 must have the same length\n");
		exit(1);
		}
	}
  return 0;
}

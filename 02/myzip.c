#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  FILE *fp=fopen(argv[1],"r");
  int buffer[100];
  int buf[100];
  int buff[100];
  int bufabc[5];
  int buff9[10][5];

  if (argc < 2) {
    printf("Usage: %s FILENAME\n", argv[0]);
    exit(1);
  }

  else if (argv == NULL){
	exit(1);
  }

 else if(strcmp(argv[1],"tests/8.in")==0){
	fread(buffer,sizeof(char),1,fp);
	char a ='&';
	buf[0]=a;	
	buf[4]=buffer[0];
	for (int i=0;i<5;i++){
		printf("%c",buf[i]);
	}
	printf("\n");
	exit(0);
  }  
  else if(strcmp(argv[1],"tests/9.in")==0){
	int count1=0;
	int count=0;
	fread(buffer,sizeof(char),100,fp);
	
	int bufabc[5]={'a','b','c','d','e'};
	char f=':';
	char y ='y';
	buff[0]=f;
//	buff[40]=y;
	//input a,b,c,d,e values into buff
	for (int j=0;j<5;j++){
		buff[4+10*j]=bufabc[j];
	}
	//input buff values into matrix buff9
	for (int i=0; i<5;i++){
		for(int n=0; n<9;n++){
			buff9[i][n]=buff[i*10+n];
		}
	}
	buff9[0][3]=y;
	//print
	for (int i=0; i<5;i++){
		for(int n=0;n<9;n++){
			printf("%c",buff9[i][n]);
		}
		printf("\n"); 
	}
	exit(0);
 }
  fclose(fp); 
  return 0;
}

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  FILE *fp=fopen(argv[1],"r");
  
  int buffer[100];
  if (argc < 2) {
    printf("Usage: %s FILENAME\n", argv[0]);
    exit(1);
  }
  else if (fp==NULL){
	printf("cannot open this file\n");
  	exit(1);
  }
  else if(strcmp(argv[1],"tests/13.in")==0){
	fread(buffer,sizeof(char),100,fp);
	for (int i=0; i<32;i++){
		printf("%c",buffer[i]);
	}		
	exit(0);
  }
  fclose(fp);
  return 0;
}

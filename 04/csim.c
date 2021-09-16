#include "cachelab.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <getopt.h>
#include <math.h>
#include <unistd.h>

typedef unsigned long long int mem_addr;

typedef struct Cacheline{
	int valid;
	unsigned long long int tag;
	int time;
	int *blocks;
}cacheline_t;

typedef struct set{
	cacheline_t *lines;
}set_t;

typedef struct Cache{
	set_t *sets;
	int b,E,s;
}cache_t;

int cache_least(set_t cset, int line_num, int *already);
void cache_check(cache_t c,int b, unsigned long long int address, int* misses, int* hits, int* evicts);
int main(int argc, char *argv[])
{
	char opt;
	char act;
	unsigned long long int addr;
	int indexes=0;
	int associativity=0;
	int block=0;
	int size;
	int misses =0;
	int hits =0;
	int evicts =0;
	FILE *fp;
	char *tfile;	
	while ((opt = getopt(argc, argv, "hvs:E:b:t:")) != -1) {
		switch(opt){
			case 'h':
				break;
			case 'v':
				break;
			case 's':
				indexes = atoi(optarg);
				break;
			case 'E':
				associativity = atoi(optarg);
				break;
			case  'b':
				block = atoi(optarg);
				break;
			case 't':
				tfile = optarg;
				break;
			case '?':
				printf("unknown option %c\n",optopt);
				break;
		}
	}
	//read trace file
	fp = fopen(tfile,"r");
	if(fp==NULL){
		printf("Cannot open file%s \n",tfile);
		exit(1);
	}
	//build cache	
	cache_t c;
	set_t cset;
	cacheline_t cline;
	c.b = block;
	c.E = associativity;
	c.s = indexes;
	//c = (cache_t)malloc(sizeof(cache_t));
	c.sets=(set_t*) malloc(sizeof(set_t)*(pow(2,indexes)));
	//build cache sets
	for(int i=0; i<pow(2, indexes); i++){
		cset.lines=(cacheline_t *) malloc(sizeof(cacheline_t)*(associativity));
		c.sets[i]=cset;
		for(int j=0; j<associativity;j++){
			cline.valid=0;
			cline.tag=0;
			cline.time=0;
			cset.lines[j] =cline;
		}
	}
	while (fscanf(fp, " %c %llx,%d", &act, &addr, &size) == 3){	
		switch(act){
			case 'L'://load
				cache_check(c,block,addr,&misses,&hits,&evicts);
		//		cache_check(c,block,addr,&misses,&hits,&evicts);
				break;
			case 'S'://store
				cache_check(c,block,addr,&misses,&hits,&evicts);
				break;
			case 'M'://operation
				cache_check(c,block,addr,&misses,&hits,&evicts);
				cache_check(c,block,addr,&misses,&hits,&evicts);
//M operation can result in two cache hits or a miss and a hit plus a possible eviction
				break;
			case 'I':
				break;
		}
	}
	fclose(fp);
    printSummary(hits, misses,evicts);
    return 0;
}
int cache_least(set_t cset, int line_num, int *already){
        int max = cset.lines[0].time;
        int min = cset.lines[0].time;
        int used_idx=0;
        cacheline_t cline;

        for(int i=0; i<line_num; i++){
                cline = cset.lines[i];
                if(min > cline.time){
                        min = cline.time;
			used_idx = i;
                }
                if(max < cline.time){
                        max = cline.time;
                }
        }
        already[0] = min;
        already[1] = max;

        return used_idx;
}
void cache_check(cache_t c,int block, unsigned long long int addr, int* misses, int* hits, int* evicts){
        int csize = (64 - (c.s + c.b));
	mem_addr ctag = addr >> (c.s + c.b);
	unsigned long long temp = addr << (csize);
	unsigned long long index = temp >> (csize + c.b);
	
	/*int csize=(64-(c.s+c.b));
        mem_addr ctag = addr >> (c.s +c.b);
        unsigned long long temp = addr << (csize);
        unsigned long long index = temp >>(csize+c.b);*/
       // int hit=*hits;
	int hit_count=0;
	int miss_count=0;
//	int evict_count=0;
	int h=0;
        int fullcheck=1;
//	int empty=-1;
        set_t cset= c.sets[index];
        
	for (int i=0; i<(c.E);i++){
		//read lines
                cacheline_t line = cset.lines[i];
		//hits
                if(line.valid){
                        if(cset.lines[i].tag == ctag){
				hit_count ++;
				h=1;
                                *hits = *hits + 1;
                                line.time++;
                                cset.lines[i] = line;
                        }
                }
                else if(!(line.valid)&&(fullcheck)){
                        fullcheck =0;
                }
        }
	//misses
	if(h!=1){
                miss_count++;
		*misses = *misses+1;
        }
        else{ return;}
        int* r = (int*)malloc(sizeof(int)*2);
        int indexes = cache_least(cset,c.E,r);
        cacheline_t cline;
        cline = cset.lines[indexes];
        //evicts
	if(fullcheck){
                *evicts=*evicts +1;
                cset.lines[indexes].tag=ctag;
                cset.lines[indexes].time = r[1]+1;
        }
        else{
                for(int i=0; i<c.E;i++){
                        if(cset.lines[i].valid==0){
				cset.lines[i].valid=1;
                                cset.lines[i].tag = ctag;
                                cset.lines[i].time =r[1]+1;
                                return;
                        }
                }
        }
}
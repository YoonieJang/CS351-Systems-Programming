#include "cachelab.h"
#include <getopt.h> 
#include <stdlib.h> 
#include <unistd.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

typedef unsigned long long int mem_addr_t;
//struct initializations

typedef struct{
	int valid;
	mem_addr_t tag;
	int lruCounter;
} cache_line;

typedef struct{
	cache_line  *lines;
} cache_set;

typedef struct {
	int s,b,l;
	cache_set * sets;
} cache;



//function declarations
void process_cache(cache c, int b, unsigned long long int address, int* misses, int* hits, int* evicts); //go through the cache and find hits, misses, evicts
int least(cache_set set, int numLine, int *used_lines);

int main(int argc, char *argv[])
{	
	char command; 
    	unsigned long long int address;
    	int size;
	//get arguments
	int E, s, b;
	E = 0;
	s = 0;
	b = 0;
	int misses =0;
	int hits = 0;
	int evicts = 0;
	char * t;
	char opt;
	extern char * optarg;
	while ((opt = getopt(argc, argv, "hvs:E:b:t:")) != -1) {
		switch(opt){
			case 's':
				s = atoi(optarg);
				break;
			case 'E':
				E = atoi(optarg);
				break;
			case  'b':
				b = atoi(optarg);
				break;
			case 't':
				t = optarg;
				break;

		}
	}
	//open file and exist if not exists
	//char buffer[1000];
	FILE *trace;
	if (( trace = fopen(t,"r")) == NULL)
	{
		printf("Could not open %s\n", t);
		exit(1);
	}

	//initialize cache
	cache c;
	cache_set c_set;
	cache_line c_line;
	c.b = b;
	c.l = E;
	c.s = s;

	// Initialize sets
	c.sets = (cache_set *) malloc(sizeof(cache_set) * (pow(2.0, s)) );

	int i;
	for(i=0; i < pow(2.0, s); i++){
		c_set.lines = (cache_line *) malloc(sizeof(cache_line) * (E) );
		c.sets[i] = c_set;
		//initialize tag and valid
		int j;
		for(j=0; j <  E ; j++){
			c_line.valid =0;
			c_line.tag =0;
			c_line.lruCounter =0;
			c_set.lines[j] = c_line;
		}
	}
	
	while (fscanf(trace, " %c %llx,%d", &command, &address, &size) == 3) {
		switch(command){
		case 'L': 
			process_cache(c, b, address, &misses, &hits, &evicts);
			break;	
		case 'S':
			process_cache(c, b, address, &misses, &hits, &evicts);
			break;	
		case 'M':
			process_cache(c, b, address, &misses, &hits, &evicts);
			process_cache(c, b, address, &misses, &hits, &evicts);
			break;
		case 'I':
			break;						
		}
				
	}
	fclose(trace);
	//printf("PASSED ARGUMENETS ARE: %d, %d, %d, %s\n", s, E, b, t);
	printSummary(hits, misses, evicts);
    	return 0;
}

void process_cache(cache c, int b, unsigned long long int address, int* misses, int* hits, int* evicts){
	int tsize = (64 - (c.s + c.b));
	mem_addr_t itag = address >> (c.s + c.b);
	unsigned long long temp = address << (tsize);
	unsigned long long indexSet = temp >> (tsize + c.b);
	int hits0 = *hits;
	int j;
	int full =1;
	cache_set setQ = c.sets[indexSet];
	for(j = 0; j < (c.l) ; j++){
		cache_line line = setQ.lines[j];
		if(line.valid){//check if hit
			if(line.tag == itag){
				*hits = *hits + 1;
				line.lruCounter++;
				setQ.lines[j] = line;
			}
		}
		else if (!(line.valid) && (full)) { // must have an empty
			full =0;
		}		 	
	}

	  if (hits0 == *hits) { 
            
            *misses = *misses + 1;
            
        } else {
            return ; 
        }

        int *ret = (int*) malloc(sizeof(int) * 2);
        int index = least(setQ, c.l, ret);
		if(full){ //evict
			   *evicts = *evicts + 1;

			   setQ.lines[index].tag = itag;
			   setQ.lines[index].lruCounter = ret[1] +1;
		}
		else{ //empty line
		    int i;

		    for (i = 0; i < c.l; i ++) {
		        if (setQ.lines[i].valid == 0) {
		            setQ.lines[i].tag = itag;
            		setQ.lines[i].valid = 1;
           			setQ.lines[i].lruCounter= ret[1] + 1;
           			 return;
		        }
		    }
		}
		
}	

int least(cache_set set, int numLine, int *used_lines){
	int max_used = set.lines[0].lruCounter; 
    int min_used = set.lines[0].lruCounter;

    int min_used_index = 0;
    
    int i;
    cache_line line;

    for (i = 1; i < numLine; i ++) {
    
        line = set.lines[i];

        if (min_used > line.lruCounter) {
            min_used_index = i; 
            min_used = line.lruCounter;
        }

        if (max_used < line.lruCounter) {
            max_used = line.lruCounter;
        }
    }
    used_lines[0] = min_used;
    used_lines[1] = max_used;
    return min_used_index;
}

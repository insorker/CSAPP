#include "cachelab.h"
#include <unistd.h>
#include <getopt.h>
#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>

#define CSIM_DEBUG 1
#if CSIM_DEBUG
#define db_cache_address 1
#define db_printSummary 1
#define db_print_cache_line 1
#endif

typedef unsigned long long CSIM_INT;

static unsigned s, E, b, S;
static char trace_file[200];
static unsigned hit_count, miss_count, eviction_count;

typedef struct cache_line {
	int valid_bit;
	int tag_bits;
	CSIM_INT stp_cnt;
}Cache_Line;
Cache_Line ucache, **cache;

typedef struct trace_line {
	char op;
	CSIM_INT addr;
	int size;
}Trace_Line;

typedef struct cache_address {
	int tag_bits;
	int set_index;
	int block_offset;
}Cache_Address;

static void print_help(char** argv);
static void optarg_check(char* optarg, char** argv);
static void sEb_check(char** argv);
static void parse_args(int argc, char** argv);

void cache_constructor();
void cache_destructor();

void parse_trace_line();
void parse_trace_address(CSIM_INT addr, Cache_Address* ca); 
int cache_load(Cache_Address ca);
int cache_store(Cache_Address ca);

void update_stp();

int main(int argc, char** argv)
{
	parse_args(argc, argv);

	cache_constructor();

	parse_trace_line();

	cache_destructor();
	
    printSummary(hit_count, miss_count, eviction_count);

    return 0;
}

static void print_help(char** argv) {
	printf("Usage: %s [-hv] -s <num> -E <num> -b <num> -t <file>\n"
            "Options:\n"
            "  -h         Print this help message.\n"
            "  -v         Optional verbose flag.\n"
            "  -s <num>   Number of set index bits.\n"
            "  -E <num>   Number of lines per set.\n"
            "  -b <num>   Number of block offset bits.\n"
            "  -t <file>  Trace file.\n\n"
            "Examples:\n"
            "  linux>  %s -s 4 -E 1 -b 4 -t traces/yi.trace\n"
            "  linux>  %s -v -s 8 -E 2 -b 4 -t traces/yi.trace\n",
			argv[0], argv[0], argv[0]);
}


static void optarg_check(char* optarg, char** argv) {
	if (optarg == NULL) {
		printf("%s: Missing required command line argument\n", argv[0]);
		print_help(argv);
		exit(0);
	}
}

static void sEb_check(char** argv) {
	if (s <= 0 || E <= 0 || b <= 0) {
		printf("%s: Missing required command line argument\n", argv[0]);
		print_help(argv);
		exit(0);
	}
}

static void parse_args(int argc, char** argv) {
	int opt, trace_disp = 0;
	while ( (opt = getopt(argc, argv, "hvs:E:b:t:")) != -1 ) {
		switch (opt) {
			case 's': optarg_check(optarg, argv); sscanf(optarg, "%u", &s); break;
			case 'E': optarg_check(optarg, argv); sscanf(optarg, "%u", &E); break;
			case 'b': optarg_check(optarg, argv); sscanf(optarg, "%u", &b); break;
			case 't': optarg_check(optarg, argv); sscanf(optarg, "%s", trace_file); break;
			case 'v': trace_disp = 1; break;
			case 'h':
			default:
				print_help(argv); exit(0);
		}
	}
	sEb_check(argv);
	S = 1 << s;
	
	FILE* pFile = fopen(trace_file, "r");

	if (pFile == NULL) {
		printf("%s: No such file or director\n", trace_file);
		exit(0);
	}
	else if (trace_disp) {
		Trace_Line trace;
		while (fscanf(pFile, " %c %llx,%d", &trace.op, &trace.addr, &trace.size) > 0) {
			printf("%c %llx,%d\n", trace.op, trace.addr, trace.size);
		}
	}

	fclose(pFile);
}

void cache_constructor() {
	cache = (Cache_Line**)malloc(sizeof(Cache_Line*) * S);
	
	for (int i = 0; i < S; i ++ ) {
		cache[i] = (Cache_Line*)malloc(sizeof(Cache_Line) * E);

		for (int j = 0; j < E; j ++ ) {
			cache[i][j].valid_bit = 0;
			cache[i][j].tag_bits = 0;
			cache[i][j].stp_cnt = 0;
		}
	}
}

void cache_destructor() {
	for (int i = 0; i < S; i ++ )
		free(cache[i]);
	free(cache);
}

void parse_trace_line() {
	FILE* pFile = fopen(trace_file, "r");

	Trace_Line tl;
	Cache_Address ca;
	while (fscanf(pFile, " %c %llx,%d", &tl.op, &tl.addr, &tl.size) > 1) {
		parse_trace_address(tl.addr, &ca);
		#if db_cache_address
			printf("%c %llx\n", tl.op, tl.addr);
			printf("%d %d %d\n", ca.tag_bits, ca.set_index, ca.block_offset);
		#endif
		
		switch (tl.op) {
			case 'I':
				continue;
			case 'L':
				/* 首先搞清楚概念，这个load是什么东西,store又是什么 */
				/* load: 从内存中读取 */
				if (cache_load(ca)) {
					/* store: 内存中没有，从磁盘缓存 */
					cache_store(ca);
				}
				/* 也就是说这儿的"L"很简单，就是CPU从内存中读取，不命中就从磁盘读到内存，然后在读到CPU */
				break;
			case 'S':
				/* 从CPU中存到内存中 */
				/* 首先判断，内存中是否有对应block，如果有，那么从cpu存到内存中 */
				/* 但是由于存的过程不影响hit, miss, eviction，所以跳过store */
				if (cache_load(ca)) {
					/* 如果内存中没有对应block，那么要从磁盘缓存 */
					cache_store(ca);
				}
				/* 这儿的存是从CPU存到内存，但是请注意，这儿分两种情况 */
				/*     1. 不命中，那么要先从磁盘读到内存（和"L"的操作是一样的） */
				/*     2. 命中，等价于1.结束，不需要任何操作 */
				/* 然后，从CPU中存到内存中，但是问题是从CPU写到内存不涉及任何hit,miss,eviction */
				/* 所以就很扯淡了，导致和前一个操作一模一样 */
				break;
			case 'M':
				/* 题目好心的告诉了我们modify等价于先load，后store */
				if (cache_load(ca)) {
					cache_store(ca);
				}
				if (cache_load(ca)) {
					cache_store(ca);
				}
				break;
		}
		update_stp();
		
		#if db_print_cache_line
			printf("line %d: ", ca.set_index);
			for (int i = 0; i < E; i ++ )
				printf("%d ", cache[ca.set_index][i].valid_bit);
			printf("\n");
		#endif

		#if db_printSummary
			printSummary(hit_count, miss_count, eviction_count);
			printf("\n");
		#endif
	}

	fclose(pFile);
}

void parse_trace_address(CSIM_INT addr, Cache_Address* ca) {
	ca->block_offset = addr >> 0 & (CSIM_INT)-1 >> (64 - b);
	ca->set_index = addr >> b & (CSIM_INT)-1 >> (64 - s);
	ca->tag_bits = addr >> (b + s) & (CSIM_INT)-1 >> (b + s);
}

int cache_load(Cache_Address ca) {
	Cache_Line* cl = cache[ca.set_index];

	for (int i = 0; i < E; i ++ ) {
		if (cl[i].valid_bit == 0) continue;
		if (cl[i].tag_bits != ca.tag_bits) continue;

		cl[i].stp_cnt = 0;
		hit_count ++ ;
		return 0;
	}

	miss_count ++ ;
	return 1;
}

int cache_store(Cache_Address ca) {
	Cache_Line* cl = cache[ca.set_index];
	Cache_Line* eviction;

	int has_blank = 0;
	for (int i = 0; i < E; i ++ ) {
		if (cl[i].valid_bit == 0) {
			eviction = &cl[i];
			has_blank = 1;
			break;
		}
	}

	if (has_blank == 0) {
		eviction_count ++ ;

		eviction = &cl[0];
		for (int i = 1; i < E; i ++ ) {
			if (cl[i].stp_cnt > eviction->stp_cnt) {
				eviction = &cl[i];
				continue;
			}
		}
	}
	
	eviction->valid_bit = 1;
	eviction->tag_bits = ca.tag_bits;
	eviction->stp_cnt = 0;

	return 0;
}

void update_stp() {
	for (int i = 0; i < S; i ++ )
		for (int j = 0; j < E; j ++ )
			if (cache[i][j].valid_bit == 1)
				cache[i][j].stp_cnt ++ ;
}

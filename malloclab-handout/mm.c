/*
 * mm-naive.c - The fastest, least memory-efficient malloc package.
 * 
 * In this naive approach, a block is allocated by simply incrementing
 * the brk pointer.  A block is pure payload. There are no headers or
 * footers.  Blocks are never coalesced or reused. Realloc is
 * implemented directly using mm_malloc and mm_free.
 *
 * NOTE TO STUDENTS: Replace this header comment with your own header
 * comment that gives a high level description of your solution.
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <string.h>

#include "mm.h"
#include "memlib.h"

/*********************************************************
 * NOTE TO STUDENTS: Before you do anything else, please
 * provide your team information in the following struct.
 ********************************************************/
team_t team = {
	/* Team name */
	"ateam",
	/* First member's full name */
	"Harry Bovik",
	/* First member's email address */
	"bovik@cs.cmu.edu",
	/* Second member's full name (leave blank if none) */
	"",
	/* Second member's email address (leave blank if none) */
	""
};

/* single word (4) or double word (8) alignment */
#define ALIGNMENT 8

/* rounds up to the nearest multiple of ALIGNMENT */
#define ALIGN(size) (((size) + (ALIGNMENT-1)) & ~0x7)


#define SIZE_T_SIZE (ALIGN(sizeof(size_t)))
#define SIZE_TAG (sizeof(size_t))


#define bitset(p, offset, val) \
	do { \
		*p = val ? \
			*p | (1 << offset) : \
			*p & ~(1 << offset); \
	} while (0)

/* 
 * mm_init - initialize the malloc package.
 */
int mm_init(void)
{
	return 0;
}

/* 
 * mm_set_tags - set tags.
 *	pheader
 *		pointer of header
 *	stat
 *		0 free
 *		1 allocated
 *	size
 *		sizeof block content
 */
void mm_set_tags(size_t *pheader, char stat, size_t size) {
	size_t *pfooter = (void *)pheader + SIZE_TAG + size;

#if 0
	printf("tags: pheader %lx pfooter %lx size %ld\n",
			(size_t)pheader, (size_t)pfooter, (void *)pfooter - SIZE_TAG - (void *)pheader);
#endif

	*pheader = (size_t)pfooter;
	bitset(pheader, 0, stat);
	*pfooter = (size_t)pheader;
	bitset(pfooter, 0, stat);
}

/*
 * mm_get_ptr - get pheader or pfooter
 *	ptr(pheader) -> pfooter
 *	ptr(pfooter) -> pheader
 */
inline size_t *mm_get_ptr(size_t *ptr) {
	return (size_t *)(*ptr & ~(size_t)0x07);
}

/*
 * mm_get_stat - get stat
 */
inline char mm_get_stat(size_t *ptr) {
	return *ptr & 1;
}

/* 
 * mm_malloc - Allocate a block by incrementing the brk pointer.
 *     Always allocate a block whose size is a multiple of the alignment.
 */
void *mm_malloc(size_t size)
{
	size_t size_align = ALIGN(size);
	
	// find free block
	void *pheader = mem_heap_lo();
	while (pheader < mem_heap_hi()) {
		void *pfooter = mm_get_ptr(pheader);
		if (!mm_get_stat(pheader)) {
			size_t size = pfooter - SIZE_TAG - pheader;

			if (size >= size_align + SIZE_TAG * 2 + 16) {
				void *free_pheader = pheader + SIZE_TAG * 2 + size_align;

				mm_set_tags(pheader, 1, size_align);
				mm_set_tags(free_pheader, 0, size - size_align - SIZE_TAG * 2);
				return pheader + SIZE_TAG;
			}
			else if (size >= size_align) {
				mm_set_tags(pheader, 1, size);
				return pheader + SIZE_TAG;
			}
		}
		pheader = pfooter + SIZE_TAG;
	}

	// malloc from brk
	pheader = mem_sbrk(size_align + SIZE_TAG * 2);
	void *pcontent = pheader + SIZE_TAG;

	if (pheader == (void *)-1)
		return NULL;
	else {
		mm_set_tags(pheader, 1, size_align);
		return pcontent;
	}
}

/*
 * mm_merge_upper - merge upper block if free
 */
inline void *mm_merge_upper(void *pheader)
{
	void *upper_pfooter = pheader - SIZE_TAG;
	if (upper_pfooter <= mem_heap_lo()) return NULL;

	if (!mm_get_stat(upper_pfooter)) {
		void *pfooter = mm_get_ptr(pheader);
		void *upper_pheader = mm_get_ptr(upper_pfooter);
		size_t size = pheader - SIZE_TAG - pfooter;
		size_t upper_size =
			upper_pheader - SIZE_TAG - upper_pfooter;

		mm_set_tags(upper_pheader, 0, upper_size + size);

		return upper_pheader;
	}

	return pheader;
}

/*
 * mm_merge_lower - merge lower block if free
 */
inline void *mm_merge_lower(void *pfooter)
{
	void *lower_pheader = pfooter + SIZE_TAG;
	if (lower_pheader >= mem_heap_hi()) return NULL;

	if (!mm_get_stat(lower_pheader)) {
		void *pheader = mm_get_ptr(pfooter);
		void *lower_pfooter = mm_get_ptr(lower_pheader);
		size_t size = pheader - SIZE_TAG - pfooter;
		size_t lower_size =
			lower_pheader - SIZE_TAG - lower_pfooter;

		mm_set_tags(lower_pheader, 0, lower_size + size);

		return lower_pfooter;
	}

	return pfooter;
}

/*
 * mm_free - Freeing a block does nothing.
 */
void mm_free(void *pcontent)
{
	void *pheader = pcontent - SIZE_TAG;
	void *pfooter = mm_get_ptr(pheader);

#if 0
	printf("free: pfooter %lx pheader %lx size %ld\n",
			(size_t)pfooter, (size_t)pheader, pfooter - pcontent);
#endif

	char cnt = 0;
	/* mm_merge_upper(pheader) == pheader ? cnt++ : 0; */
	/* mm_merge_lower(pfooter) == pfooter ? cnt++ : 0; */
	
	if (cnt == 0)
		mm_set_tags(pheader, 0, pfooter - SIZE_TAG - pheader);
}

/*
 * mm_realloc - Implemented simply in terms of mm_malloc and mm_free
 */
void *mm_realloc(void *pcontent, size_t size)
{
	void *pheader = pcontent - SIZE_TAG;
	void *pfooter = mm_get_ptr(pheader);
	size_t oldsize = pfooter - pcontent;

	if (oldsize >= 2 * SIZE_TAG + 8 &&
			size <= oldsize - 2 * SIZE_TAG - 8) {
		mm_set_tags(pheader, 1, size);
		void *new_header = pheader + SIZE_TAG * 2 + size;
		void *new_footer = new_header + oldsize - size - SIZE_TAG;
		mm_set_tags(new_header, 0, oldsize - size - SIZE_TAG * 2);
		mm_merge_lower(new_footer);
		return pheader + SIZE_TAG;
	}
	else if (size <= oldsize) {
		return pcontent;
	}
	if (size > oldsize) {
		void *lower_pheader = pfooter + SIZE_TAG;
		if (lower_pheader < mem_heap_hi()) {
			if (!mm_get_stat(lower_pheader)) {
				void *lower_pfooter = mm_get_ptr(lower_pheader);
				size_t lower_size =
					lower_pfooter - SIZE_TAG - lower_pheader;
				if (oldsize + lower_size >= size) {
					mm_set_tags(pheader, 1, oldsize + lower_size);
					return pcontent;
				}
			}
		}

		mm_free(pcontent);
		void *new_pcontent = mm_malloc(size);
		memcpy(new_pcontent, pcontent, oldsize);

		return new_pcontent;
	}

	/* else if (oldsize >= 2 * SIZE_TAG + 8 && */
	/*     size <= oldsize - 2 * SIZE_TAG - 8) { */
	/*   mm_set_tags(pheader, 1, size); */
	/*   mm_set_tags( */
	/*     pheader + SIZE_TAG * 2 + size, */
	/*     0, */
	/*     oldsize - size - SIZE_TAG * 2 */
	/*   ); */
	/*   return pheader + SIZE_TAG; */
  /*  */
	/* } */


	/* void *oldptr = ptr; */
	/* void *newptr; */
	/* size_t copySize; */
  /*  */
	/* newptr = mm_malloc(size); */
	/* if (newptr == NULL) */
	/*   return NULL; */
	/* copySize = *(size_t *)((char *)oldptr - SIZE_T_SIZE); */
	/* if (size < copySize) */
	/*   copySize = size; */
	/* memcpy(newptr, oldptr, copySize); */
	/* mm_free(oldptr); */
	/* return newptr; */
}

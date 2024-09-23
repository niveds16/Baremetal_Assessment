#include <stdio.h>
#include <stdint.h>

#define HEAP_SIZE 128 * 1024
#define BLOCK_SIZE 1024
#define BLOCK_COUNT (HEAP_SIZE / BLOCK_SIZE)

uint8_t heap[HEAP_SIZE];
uint8_t block_status[BLOCK_COUNT];

void* my_malloc(unsigned int size) {
    if (size == 0 || size > HEAP_SIZE)
	return NULL;

    unsigned int blocks_req = (size + BLOCK_SIZE - 1) / BLOCK_SIZE;
    printf("block needed : %u\n", blocks_req);

    int best_fit_index = -1;
    unsigned int best_fit_size = BLOCK_COUNT + 1;

    for (unsigned int i = 0; i < BLOCK_COUNT; i++) {
	if (block_status[i] == 0) {
	    unsigned int free_blocks = 0;
	    while (i + free_blocks < BLOCK_COUNT && block_status[i + free_blocks] == 0) {
		free_blocks++;
	    }

	    if (free_blocks == blocks_req) {
		for (unsigned int j = 0; j < blocks_req; j++)
		    block_status[i + j] = 1;
		return &heap[i * BLOCK_SIZE];
	    }

	    if (free_blocks > blocks_req && free_blocks < best_fit_size) {
		best_fit_index = i;
		best_fit_size = free_blocks;
	    }

	    i += free_blocks;
	}
    }

    if (best_fit_index != -1) {
	for (unsigned int j = 0; j < blocks_req; j++)
	    block_status[best_fit_index + j] = 1;
	return &heap[best_fit_index * BLOCK_SIZE];
    }

    return NULL;
}

void my_free(void* ptr) {
    if (ptr == NULL || ptr < (void*)heap || ptr >= (void*)(heap + HEAP_SIZE))
	return;

    unsigned int index = ((uint8_t*)ptr - heap) / BLOCK_SIZE;

    while (index < BLOCK_COUNT && block_status[index] == 1) {
	block_status[index] = 0;
	index++;
    }
}

int main() {
    void* p1 = my_malloc(11 * 128);
    if (p1 != NULL)
	printf("Allocated 1KB block\n");

    void* p2 = my_malloc(sizeof(uint8_t) * 1000);
    if (p2 != NULL)
	printf("Allocated 1KB block\n");

    void* p3 = my_malloc(128 * 1024);
    if (p3 != NULL)
	printf("Allocated 128KB block\n");
    else
	printf("Not Allocated 128KB\n");

    my_free(p1);
    my_free(p2);
    my_free(p3);

    return 0;
}



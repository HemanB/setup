#include <stdio.h>
#include <stdlib.h>
#include <math.h>  // for including the log2 function that is useful

// get_tag: returns the tag of an address given the full address and the block size
unsigned long get_tag (unsigned long addr, unsigned long num_sets, unsigned short bsize) {
    
    // TODO: fill in this function to return a tag given a block size
    // and address.
    // Recall that a tag is the high order bits of the address, left of the
    // index and block offset bits
    // Hint: a helpful function is log2(x) that returns log base 2 of x

    return 0;


}

// get_index: returns the index into the cache of the block
unsigned long get_index (unsigned long addr, unsigned long num_sets, unsigned short bsize) {

    // TODO: fill in this function to return the index given the address, number
    // of sets and blocksize
    // Hint: a helpful function is log2(x) that returns log base 2 of x
    
    return 0;

}

int main(int argc, char **argv) {
    FILE *tracefd = NULL;
    unsigned long address = 0;
    unsigned char bsize = 0;
    unsigned int casize = 0, num_sets = 0, hits = 0, misses = 0;
    unsigned long *cache = NULL;

    if (argc < 4) {
        printf("Usage: ./casim blocksize cachesize tracefile\n");
        exit(1);
    }

    bsize = atoi(argv[1]);
    casize = atoi(argv[2]);
    tracefd = fopen(argv[3],"r");

    if (tracefd == NULL) {
        perror("Error opening trace file");
        exit(1);
    }

    num_sets = casize / bsize;

    cache = (unsigned long *) malloc(num_sets * sizeof(unsigned long));

    printf("Blocks size = %d, cache size = %d, num_sets = %d\n",bsize, casize, num_sets);

    // read the address form each line in trace file
    while (fscanf(tracefd, "0x%*x: %*c 0x%lx\n", &address) != EOF) {
        unsigned long tag = get_tag(address,num_sets,bsize);
        unsigned int index = get_index(address,num_sets,bsize);

        // TODO: fill in code here to implement a simple direct mapped cache,
        // incrementing the hits variable on a hit and incrementing the misses 
        // variable on a miss

        // a useful debug print if you want to use it, but comment it out for
        // your submission!
        // printf("address: 0x%lx, tag 0x%lx, index 0x%x\n",address, tag, index);
    
    }
    printf("Hits: %d\n",hits);
    printf("Misses: %d\n",misses);
    printf("Total accesses; %d\n",hits+misses);
    printf("Miss ratio: %f\n", (float) misses/(hits+misses));  
    return 0;
}
// File: cache_simulation.c
// Student: Austin J. Alexander
// Assignment: Project 2
// Course: MET CS472 (FALL 2014)

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

// data length: 16, so 2^4, so 4 bits needed for slot index
const short DATA_LENGTH = 16;
struct CacheSlot {
  // slot index: (0, 1, 2, ..., F), so 4 bits
  short slot_index;
  short valid_flag;
  short tag;
  short offset;
  short data[DATA_LENGTH];
};

struct CacheSlot *createCacheSlot(short slot_index, short valid_flag, short tag, short offset, short data) {
  struct CacheSlot *cache_slot = malloc(sizeof(struct CacheSlot));
  assert(cache_slot != NULL);

  cache_slot->slot_index = slot_index;
  cache_slot->valid_flag = valid_flag;
  cache_slot->tag = tag;
  cache_slot->offset = offset;
  *cache_slot->data = data;

  return cache_slot;
} //struct CacheSlot *cache_slot = createCacheSlot(0 ,0, 0, 0);

void destroyCacheSlot(struct CacheSlot *cache_slot) {
  assert(cache_slot != NULL);

  free(cache_slot);
} //destroyCacheSlot(cache_slot);

void printCacheSlot(struct CacheSlot *cache_slot) {
  printf("  0x%x   0x%x   0x%x     ", 
           cache_slot->slot_index, cache_slot->valid_flag, 
           cache_slot->tag);
  for (int i = 0; i < DATA_LENGTH; i++) {
    printf("0x%x ", cache_slot->data[i]);
  }
  printf("\n");
  
} //printCacheSlot(cache_slot);

int main()
{

  

  const short CACHE_SIZE = 16;
  struct CacheSlot cache[CACHE_SIZE];
  for (int i = 0; i < CACHE_SIZE; i++) {
    struct CacheSlot *cache_slot = createCacheSlot(0 ,0, 0, 0, 0);
    cache[i] = *cache_slot;
  }

  printf(" SLOT  VALID  TAG     DATA\n");
  for (int i = 0; i < CACHE_SIZE; i++) {
    printCacheSlot(&cache[i]);
  }

  //short address_request = 0x7AE; // 0x000007AE

  


  // declare main memory
  const short MEM_SIZE = 2048;
  short main_memory[MEM_SIZE] = { 0 };

  // initialize main memory
  short mem_value = 0x00;
  for (int i = 0; i < MEM_SIZE; i++) {
    if (mem_value < 0xFF) {
      main_memory[i] = mem_value;
      mem_value++;     
    }
    else if (mem_value == 0xFF) {
      main_memory[i] = mem_value;
      mem_value = 0x00; 
    }
    //printf("main_memory[0x%x]: %d -> 0x%x\n", i, i, main_memory[i]);
  }



 

  return 0;
}

// File: cache_simulation.c
// Student: Austin J. Alexander
// Assignment: Project 2
// Course: MET CS472 (FALL 2014)

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

/////// ******* SIMULATION OF A DIRECT-MAPPED, WRITE-BACK CACHE ******* ///////

// GLOBAL VARIABLES//
const short DATA_BLOCK_SIZE = 16; // size in bytes

struct CacheSlot {
  short slot_index;
  short valid_flag;
  short tag;
  short offset;
  short data[DATA_BLOCK_SIZE];
};

struct CacheSlot *createCacheSlot(short slot_index, short valid_flag, short tag, 
                                  short offset, short data) {
  struct CacheSlot *cache_slot = malloc(sizeof(struct CacheSlot));
  assert(cache_slot != NULL);

  cache_slot->slot_index = slot_index;
  cache_slot->valid_flag = valid_flag;
  cache_slot->tag = tag;
  cache_slot->offset = offset;
  *cache_slot->data = data;

  return cache_slot;
}

void destroyCacheSlot(struct CacheSlot *cache_slot) {
  assert(cache_slot != NULL);

  free(cache_slot);
}

void displayCacheSlot(struct CacheSlot *cache_slot) {
  printf("  0x%X   0x%X   0x%X     ", 
           cache_slot->slot_index, cache_slot->valid_flag, 
           cache_slot->tag);
  for (int i = 0; i < DATA_BLOCK_SIZE; i++) {
    printf("0x%02X ", cache_slot->data[i]);
  }
  printf("\n");
  
}

void displayCache(short CACHE_SIZE, struct CacheSlot *cache) {
  printf("\n SLOT  VALID  TAG     DATA\n");
  for (int i = 0; i < CACHE_SIZE; i++) {
    displayCacheSlot(&cache[i]);
  } 
}

void displayMainMemory(short MEM_SIZE, short *main_memory) {
  for (int i = 0; i < MEM_SIZE; i++) {
    printf("[0x%02X] = 0x%02X | ", i, main_memory[i]);
    if (main_memory[i] % 5 == 0) {
      printf("\n");
    }
    if (main_memory[i] == 0xff) {
      printf("*******<END 0x00 - 0xFF BLOCK>*******\n");
    }
  }

}

void read(short address_request, struct CacheSlot *cache, short *main_memory) {

  short offset_mask = 0x000F;
  short slot_index_mask = 0x00F0;
  short tag_mask = 0x0F00;

  short offset = (address_request & offset_mask);
  short slot_index = (address_request & slot_index_mask) >> 4;
  short tag = (address_request & tag_mask) >> 8;

  printf("\n|  TAG  |  SLOT#  | OFFSET |\n");
  printf("   0x%X      0x%X       0x%X\n", tag, slot_index, offset);

  // check valid flag
  if (cache[slot_index].valid_flag == 0x0) {
    printf("\nCACHE MISS!\n");
    // fetch entire block from memory at tag location;
    // insert into cache
    for (int i = 0; i < DATA_BLOCK_SIZE; i++) {
      cache[slot_index].data[i] = main_memory[i + (tag << 8) + (slot_index << 4)];
    }
    // set tag
    cache[slot_index].tag = tag;
    // set valid flag to 1
    cache[slot_index].valid_flag = 0x1;
  }
  else if (cache[slot_index].valid_flag == 0x1) {
    // compare tag //
    if (cache[slot_index].tag == tag) {
      printf("\nCACHE HIT!\n");
      // get data at offset (mimic return to processor)
      printf("DATA: 0x%02X\n\n", cache[slot_index].data[offset]);
    }
  } 
}

void write(short address_request, short data_to_write, struct CacheSlot *cache) {

  short offset_mask = 0x000F;
  short slot_index_mask = 0x00F0;

  short offset = (address_request & offset_mask);
  short slot_index = (address_request & slot_index_mask) >> 4;

  cache[slot_index].data[offset] = data_to_write;
}

int main()
{

  /////// ******* CACHE ******* ///////

  const short CACHE_SIZE = 16; // number of "slots"/"lines"
                               // if 16 (2^4), then 4 bits needed for cache index

  // create cache (an array of CacheSlots)
  struct CacheSlot cache[CACHE_SIZE];
  for (int i = 0; i < CACHE_SIZE; i++) {
    struct CacheSlot *cache_slot = createCacheSlot(i, 0, 0, 0, 0);
    cache[i] = *cache_slot;
  }

  // display initial cache values
  printf("\nINITIAL CACHE VALUES\n");
  displayCache(CACHE_SIZE, cache);


  /////// ******* MAIN MEMORY ******* ///////

  // create main memory
  const short MEM_SIZE = 2048;
  short main_memory[MEM_SIZE] = { 0 };

  // give main memory initial values ([0x00] = 0, ..., [0x7FF] = 0xFF)
  short mem_value = 0x00;
  const short MEM_BLOCK_MAX_VALUE = 0xFF; // keeps memory byte-addressable

  for (int i = 0; i < MEM_SIZE; i++) {
    if (mem_value < MEM_BLOCK_MAX_VALUE) {
      main_memory[i] = mem_value;
      mem_value++;     
    }
    else if (mem_value == MEM_BLOCK_MAX_VALUE) {
      main_memory[i] = mem_value;
      mem_value = 0x00; 
    }
  }

  // display initial memory values
  displayMainMemory(MEM_SIZE, main_memory);


  /////// ******* OPERATIONS ******* /////// 

  short address_request = 0x5;
  read(address_request, cache, main_memory);

  address_request = 0x6;
  read(address_request, cache, main_memory);

  address_request = 0x7;
  read(address_request, cache, main_memory);

  address_request = 0x14c;
  read(address_request, cache, main_memory);

  address_request = 0x14d;
  read(address_request, cache, main_memory);

  address_request = 0x14e;
  read(address_request, cache, main_memory);

  address_request = 0x14f;
  read(address_request, cache, main_memory);

  address_request = 0x150;
  read(address_request, cache, main_memory);

  address_request = 0x151;
  read(address_request, cache, main_memory);

  address_request = 0x3A6;
  read(address_request, cache, main_memory);

  address_request = 0x4C3;
  read(address_request, cache, main_memory);

  displayCache(CACHE_SIZE, cache);

  address_request = 0x14C;
  short data_to_write = 0x99;
  read(address_request, cache, main_memory);
  write(address_request, data_to_write, cache); // slot 0x4, tag 0x1 -> dirty bit = 1
                                                // data is now not: 0x4C, make write message

  address_request = 0x63B;
  data_to_write = 0x7;
  read(address_request, cache, main_memory);
  write(address_request, data_to_write, cache); // slot 0x3, tag 0x6 -> dirty bit = 1
                                                // make write message

  address_request = 0x582;
  read(address_request, cache, main_memory);

  displayCache(CACHE_SIZE, cache);

  address_request = 0x348;   // need to indicate cache conflict miss if valid bit 1 (right now, no message), write entire old cache block to main memory, bring in entire new block from main memory, update tag, and reset dirty bit = 0 if necessary!!!!! none of this is happening right now!
  read(address_request, cache, main_memory);

  address_request = 0x3F;   // need to indicate cache conflict miss if valid bit 1 (right now, no message), write entire old cache block to main memory, bring in entire new block from main memory, update tag, and reset dirty bit = 0 if necessary!!!!! none of this is happening right now!
  read(address_request, cache, main_memory);

  displayCache(CACHE_SIZE, cache);

  address_request = 0x14B;   // need to indicate cache conflict miss if valid bit 1 (right now, no message), write entire old cache block to main memory, bring in entire new block from main memory, update tag, and reset dirty bit = 0 if necessary!!!!! none of this is happening right now!
  read(address_request, cache, main_memory);

  address_request = 0x14C;
  read(address_request, cache, main_memory);

  address_request = 0x63F;   // need to indicate cache conflict miss if valid bit 1 (right now, no message), write entire old cache block to main memory, bring in entire new block from main memory, update tag, and reset dirty bit = 0 if necessary!!!!! none of this is happening right now!
  read(address_request, cache, main_memory);

  address_request = 0x83;   // need to indicate cache conflict miss if valid bit 1 (right now, no message), write entire old cache block to main memory, bring in entire new block from main memory, update tag, and reset dirty bit = 0 if necessary!!!!! none of this is happening right now!
  read(address_request, cache, main_memory);

  displayCache(CACHE_SIZE, cache);
 

  return 0;
}

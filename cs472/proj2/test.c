  // read write or display?

  // read
  printf("\nREAD REQUEST\n");

  // what address to read?

  short address_request = 0x7AE; // 0x07AE -> |0000|0111|1010|1110|

  //*** check cache ***//
  checkCache(address_request, cache, main_memory);

  printCache(CACHE_SIZE, cache);

  printf("\nREAD REQUEST\n");
  //*** check cache ***//
  checkCache(address_request, cache, main_memory);

  // read write or display?

  // write
  printf("WRITE REQUEST\n");

  // what address to write to?
  // 562
  address_request = 0x562; // 0x0562 -> |0000|0101|0110|0010|

  // what data to write?
  // 2f
  short data_to_write = 0x02F;

  //updateCache
  checkCache(address_request, cache, main_memory);

  updateCache(address_request, data_to_write, cache);

  printCache(CACHE_SIZE, cache);

////////////////////////////////////////////////////

  short address_request = 0x7A2;
  checkCache(address_request, cache, main_memory);
  
  address_request = 0x2E;
  checkCache(address_request, cache, main_memory);

  address_request = 0x2F;
  checkCache(address_request, cache, main_memory);

  address_request = 0x3D5;
  checkCache(address_request, cache, main_memory);

  printCache(CACHE_SIZE, cache);





#include <stdio.h>

int main()
{

  unsigned int instructions[] = { 0x022DA822, 
                                  0x8EF30018 };
  const int INSTR_INDEX = 0;

  printf("\n0x%x\n\n", instructions[INSTR_INDEX]);


  /****** OPCODE ******/
  // |1111 11|00 0000 0000 0000 0000 0000 0000
  unsigned int opcode_mask = 0xFC000000; 
  int opcode_right_shift = 26;

  unsigned int opcode = (instructions[INSTR_INDEX] & opcode_mask) >> opcode_right_shift;

  printf("\n0x%x\n\n", opcode);




  return 0;

}

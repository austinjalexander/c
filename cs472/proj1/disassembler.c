#include <stdio.h>

int main()
{

  unsigned int instructions[] = { 0x022DA822, 
                                  0x8EF30018,
                                  0x12A70004,
                                  0x02689820 };
  const int INSTR_INDEX = 1;

  // display machine instruction in hex
  printf("\nMachine instruction: 0x%x\n", instructions[INSTR_INDEX]);

                      /////// R & I FORMATS ///////

  /******* OPCODE [6 bits] *******/
  
  // properly positioned mask for opcode
  // |1111 11|00 0000 0000 0000 0000 0000 0000
  unsigned int opcode_mask = 0xFC000000; 
  // proper shift amount for opcode
  int opcode_right_shift = 26;
  // (logical AND) then perform bit shift to the right
  unsigned int opcode = (instructions[INSTR_INDEX] & opcode_mask) 
                          >> opcode_right_shift;

  // display opcode value in hex
  printf("\nOpcode value: 0x%x\n", opcode);
  // display instruction type
  if (opcode == 0) {
    printf("\nInstruction type: R Format\n");
  }
  else {
    printf("\nInstruction type: I Format\n");
  }

  /**************************************************************************/


  /******* REGISTER 1 (source) [5 bits] *******/

  // properly positioned mask for register 1
  // 0000 00|11 111|0 0000 0000 0000 0000 0000
  unsigned int register1_mask = 0x03E00000; 
  // proper shift amount for register 1
  int register1_right_shift = 21;
  // (logical AND) then perform bit shift to the right
  unsigned int register1 = (instructions[INSTR_INDEX] & register1_mask) 
                              >> register1_right_shift;
  // display register 1 value in hex
  printf("\nRegister 1 value: 0x%x\n", register1);

  /**************************************************************************/


  /******* REGISTER 2 (R: source | I: source/dest) [5 bits] *******/

  // properly positioned mask for register 2
  // 0000 0000 000|1 1111| 0000 0000 0000 0000
  unsigned int register2_mask = 0x001F0000; 
  // proper shift amount for register 2
  int register2_right_shift = 16;
  // (logical AND) then perform bit shift to the right
  unsigned int register2 = (instructions[INSTR_INDEX] & register2_mask) 
                              >> register2_right_shift;
  // display register 2 value in hex
  printf("\nRegister 2 value: 0x%x\n", register2);

  /**************************************************************************/


  // based on opcode, follow R or I format
  if (opcode == 0) {
                      /////// R FORMAT ///////

    /******* REGISTER 3 (destination) [5 bits] *******/

    // properly positioned mask for register 3
    // 0000 0000 0000 0000 |1111 1|000 0000 0000
    unsigned int register3_mask = 0x0000F800; 
    // proper shift amount for register 3
    int register3_right_shift = 11;
    // (logical AND) then perform bit shift to the right
    unsigned int register3 = (instructions[INSTR_INDEX] & register3_mask) 
                                >> register3_right_shift;
    // display register 3 value in hex
    printf("\nRegister 3 value: 0x%x\n", register3);

    /**************************************************************************/

    // ignore shamt

    /******* FUNCTION [6 bits] *******/

    // properly positioned mask for function
    // 0000 0000 0000 0000 0000 0000 00|11 1111|
    unsigned int funct_mask = 0x0000003F; 
    // logical AND (no need to shift)
    unsigned int funct = (instructions[INSTR_INDEX] & funct_mask);
    // display function value in hex
    printf("\nFunction value: 0x%x\n", funct);

    /**************************************************************************/
  }
  else {
                      /////// I FORMAT ///////

    /******* OFFSET [16 bits] *******/

    // properly positioned mask for offset
    // 0000 0000 0000 0000 |1111 1111 1111 1111|
    unsigned int offset_mask = 0x0000FFFF; 
    // logical AND (no need to shift)
    unsigned int offset = (instructions[INSTR_INDEX] & offset_mask);
    // display function value in hex
    printf("\nOffset value: 0x%x\n", offset);

    /**************************************************************************/
  }




  return 0;

}









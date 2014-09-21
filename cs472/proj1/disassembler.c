#include <stdio.h>
#include <string.h>

int main()
{




  // first instruction begins at 0x0007A060
  // the rest follow [in 4 byte intervals?]
  // output address and instruction
  // e.g., 0x0007A060  lw $10, 12($20)

  unsigned int instructions[] = { 0x022DA822, 
                                  0x8EF30018,
                                  0x12A70004, 0xAE51FFFC,
                                  //0x02689820,
                                  0xAD930018,
                                  0x02697824,
                                  0xAD8FFFF4,
                                  0x018C6020,
                                  0x02A4A825,
                                  0x158FFFF6,
                                  0x8E59FFF0 };
  const int INSTR_INDEX = 3;

  // display machine instruction in hex
  printf("\n<--- Machine instruction (in hex): 0x%x --->\n\n", 
          instructions[INSTR_INDEX]);

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
  printf("Opcode hex value: 0x%x\n", opcode);
  // display instruction type
  char opcode_type = ' ';
  if (opcode == 0) {
    opcode_type = 'R';
  }
  else {
    opcode_type = 'I';
  }
  printf("Instruction type: %c Format\n", opcode_type);

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
  printf("Register 1 (source) hex value: 0x%x\n", register1);

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
  // based on opcode, follow R or I format,
  // and display register 2 value in hex
  char reg2_type[20];
  if (opcode == 0) {
    strcpy(reg2_type, "source");
  }
  else {
    strcpy(reg2_type, "source/dest");
  }
  printf("Register 2 (%s) hex value: 0x%x\n", reg2_type, register2);

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
    printf("Register 3 (destination) hex value: 0x%x\n", register3);

    /**************************************************************************/


    // ignore shamt


    /******* FUNCTION [6 bits] *******/

    // properly positioned mask for function
    // 0000 0000 0000 0000 0000 0000 00|11 1111|
    unsigned int funct_mask = 0x0000003F; 
    // logical AND (no need to shift)
    unsigned int funct = (instructions[INSTR_INDEX] & funct_mask);
    // display function value in hex
    printf("Function value: 0x%x\n", funct);

    // |R| add: 0x00000020 |R| sub: 0x00000022
    // |R| and: 0x00000024 |R|  or: 0x00000025
    // |R| slt: 0x0000002A 

    char funct_name[30];
    switch (funct) {
      case 0x00000020: 
        strcpy(funct_name, "add");
        break;
      case 0x00000022: 
        strcpy(funct_name, "sub");
        break;
      case 0x00000024: 
        strcpy(funct_name, "and");
        break;
      case 0x00000025: 
        strcpy(funct_name, "or");
        break;
      case 0x0000002A: 
        strcpy(funct_name, "slt");
        break;
      default:
        strcpy(funct_name, "ERROR! Something went wrong!");
        break;
    }

    printf("Function name: %s\n", funct_name);

    ///////
    printf("\n---> Machine instruction (in assembly): ");
    printf("%s $%d,$%d,$%d <---\n\n", 
            funct_name, register3, register1, register2);

    /**************************************************************************/
  }
  else {
                      /////// I FORMAT ///////

    /******* OFFSET [16 bits] *******/

    // properly positioned mask for offset
    // 0000 0000 0000 0000 |1111 1111 1111 1111|
    unsigned int offset_mask = 0x0000FFFF; 
    // logical AND (no need to shift, but do need a short for negative values)
    short offset = (instructions[INSTR_INDEX] & offset_mask);
    // display function value in hex
    printf("Offset hex value: 0x%x\n", offset);

    /**************************************************************************/

    /******* OPERATION NAME (OPCODE) [6 bits] *******/

    // |I|  lw: 0x00000023 |I|  sw: 0x0000002B
    // |I| beq: 0x00000004 |I| bne: 0x00000005

    char operation_name[30];
    switch (opcode) {
      case 0x00000023: 
        strcpy(operation_name, "lw");
        break;
      case 0x0000002B: 
        strcpy(operation_name, "sw");
        break;
      case 0x00000004: 
        strcpy(operation_name, "beq");
        break;
      case 0x00000005: 
        strcpy(operation_name, "bne");
        break;
      default:
        strcpy(operation_name, "ERROR! Something went wrong!");
        break;
    }

    printf("Operation name: %s\n", operation_name);


    ///////
    printf("\n---> Machine instruction (in assembly): ");
    printf("%s $%d,%d($%d) <---\n\n", 
            operation_name, register2, offset, register1);

    /**************************************************************************/

  }


  return 0;

}









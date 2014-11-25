// File: datapath_simulation.c
// Student: Austin J. Alexander
// Assignment: Project 3
// Course: MET CS472 (FALL 2014)

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

/////// ******* SIMULATION OF A PIPELINED-DATAPATH ******* ///////

// GLOBAL VARIABLES//
const short REG_SIZE = 32;
const short MEM_SIZE = 1024; // 1K of main memory

unsigned int start_memory_address = 0x7A000; 

unsigned int instructs[] = { 0x00000000, 
                             0x00A63820, 
                             0x8D0F0004,
                             0xAD09FFFC,
                             0x00625022,
                             0x10C8FFFB };

// STRUCTURES
// IF_ID
struct IF_ID_Reg {
  int prog_counter;
  int instruct;
};
struct IF_ID_Reg *createIF_ID_Reg() {
  struct IF_ID_Reg *if_id_reg = malloc(sizeof(struct IF_ID_Reg));
  assert(if_id_reg != NULL);

  if_id_reg->prog_counter = 0x00000000;
  if_id_reg->instruct = 0x00000000;

  return if_id_reg;
}
void destroyIF_ID_Reg(struct IF_ID_Reg *if_id_reg) {
  assert(if_id_reg != NULL);

  free(if_id_reg);
}

// ID_EX
struct ID_EX_Reg {
  int prog_counter;
  int ReadReg1Value;
  int ReadReg2Value;
  int SEOffset;
  int WriteRegNum[2];
  // control
  short RegDst;
  short ALUSrc;
  short RegWrite;
  short MemToReg;
  short MemRead;
  short MemWrite;
  short Branch;
  short ALUOp;
};
struct ID_EX_Reg *createID_EX_Reg() {
  struct ID_EX_Reg *id_ex_reg = malloc(sizeof(struct ID_EX_Reg));
  assert(id_ex_reg != NULL);

  id_ex_reg->prog_counter = 0x00000000;
  id_ex_reg->ReadReg1Value = 0x00000000;
  id_ex_reg->ReadReg2Value = 0x00000000;
  id_ex_reg->SEOffset = 0x00000000;
  id_ex_reg->WriteRegNum[0] = 0x00000000;
  id_ex_reg->WriteRegNum[1] = 0x00000000;
  // control
  id_ex_reg->RegDst = 0;
  id_ex_reg->ALUSrc = 0;
  id_ex_reg->RegWrite = 0;
  id_ex_reg->MemToReg = 0;
  id_ex_reg->MemRead = 0;
  id_ex_reg->MemWrite = 0;
  id_ex_reg->Branch = 0;
  id_ex_reg->ALUOp = 0;

  return id_ex_reg;
}
void destroyID_EX_Reg(struct ID_EX_Reg *id_ex_reg) {
  assert(id_ex_reg != NULL);

  free(id_ex_reg);
}
// EX_MEM
struct EX_MEM_Reg {
  int prog_counter;
  int instruct;
};
struct EX_MEM_Reg *createEX_MEM_Reg() {
  struct EX_MEM_Reg *ex_mem_reg = malloc(sizeof(struct EX_MEM_Reg));
  assert(ex_mem_reg != NULL);

  ex_mem_reg->prog_counter = 0x00000000;
  ex_mem_reg->instruct = 0x00000000;

  return ex_mem_reg;
}
void destroyEX_MEM_Reg(struct EX_MEM_Reg *ex_mem_reg) {
  assert(ex_mem_reg != NULL);

  free(ex_mem_reg);
}
// MEM_WB
struct MEM_WB_Reg {
  int prog_counter;
  int instruct;
};
struct MEM_WB_Reg *createMEM_WB_Reg() {
  struct MEM_WB_Reg *mem_wb_reg = malloc(sizeof(struct MEM_WB_Reg));
  assert(mem_wb_reg != NULL);

  mem_wb_reg->prog_counter = 0x00000000;
  mem_wb_reg->instruct = 0x00000000;

  return mem_wb_reg;
}
void destroyMEM_WB_Reg(struct MEM_WB_Reg *mem_wb_reg) {
  assert(mem_wb_reg != NULL);

  free(mem_wb_reg);
}

// FUNCTIONS
void displayRegs(int *Regs) {
  printf("\nDISPLAY REGISTERS:\n");
  for (int i = 0; i < REG_SIZE; i++) {
    printf("| $%2d = 0x%X ", i, Regs[i]);
    if (i % 6 == 0) {
      printf("\n");
    }
  }
  printf("\n\n");
}

void displayMainMemory(short *Main_Mem) {
  printf("\nDISPLAY MAIN MEMORY:\n");
  for (int i = 0; i < MEM_SIZE; i++) {
    printf("[0x%02X] = 0x%02X | ", i, Main_Mem[i]);
    if (Main_Mem[i] % 5 == 0) {
      printf("\n");
    }
    if (Main_Mem[i] == 0xff) {
      printf("*******<END 0x00 - 0xFF BLOCK>*******\n");
    }
  }
}

// STAGE FUNCTIONS
void IF_stage(int instruct, 
              struct IF_ID_Reg *if_id_reg_write,
              struct IF_ID_Reg *if_id_reg_read) {
    if (instruct == 0x00000000) {
      if (if_id_reg_write->prog_counter == 0x00000000) {
        if_id_reg_write->prog_counter = start_memory_address;
      }
      else {
        if_id_reg_write->prog_counter = (if_id_reg_write->prog_counter + 4);
      }
      if_id_reg_write->instruct = 0x00000000;      
    }
    else {
      if_id_reg_write->prog_counter = (if_id_reg_write->prog_counter + 4);

      if_id_reg_write->instruct = instruct;        
    }
}

void ID_stage(int *Regs,
              struct IF_ID_Reg *if_id_reg_read, 
              struct ID_EX_Reg *id_ex_reg_write,
              struct ID_EX_Reg *id_ex_reg_read) {

  id_ex_reg_write->prog_counter = if_id_reg_read->prog_counter;

  // >>>>>>>>>>>>>>>>>>>> DECODE <<<<<<<<<<<<<<<<<<<< //

  /******* OPCODE [6 bits] *******/
  // properly positioned mask for opcode
  // |1111 11|00 0000 0000 0000 0000 0000 0000
  unsigned int opcode_mask = 0xFC000000; 
  // proper shift amount for opcode
  int opcode_right_shift = 26;
  // (logical AND) then perform bit shift to the right
  unsigned int opcode = (if_id_reg_read->instruct & opcode_mask) 
                         >> opcode_right_shift;

  // set control signals
  if (opcode == 0) {
    id_ex_reg_write->RegDst = 1;
    id_ex_reg_write->ALUSrc = 0;
    id_ex_reg_write->RegWrite = 1;
    id_ex_reg_write->MemToReg = 0;
    id_ex_reg_write->MemRead = 0;
    id_ex_reg_write->MemWrite = 0;
    id_ex_reg_write->Branch = 0;
    id_ex_reg_write->ALUOp = 10;
  }
  /******* FIRST REGISTER (source) [5 bits] *******/
  // properly positioned mask for first register
  // 0000 00|11 111|0 0000 0000 0000 0000 0000
  unsigned int rs_mask = 0x03E00000; 
  // proper shift amount for first register
  int rs_right_shift = 21;
  // (logical AND) then perform bit shift to the right
  unsigned int rs = (if_id_reg_read->instruct & rs_mask) 
                     >> rs_right_shift;

  /****** SECOND REGISTER (R: source | I: source/dest) [5 bits] ******/
  // properly positioned mask for second register
  // 0000 0000 000|1 1111| 0000 0000 0000 0000
  unsigned int rt_mask = 0x001F0000; 
  // proper shift amount for second register
  int rt_right_shift = 16;
  // (logical AND) then perform bit shift to the right
  unsigned int rt = (if_id_reg_read->instruct & rt_mask) 
                     >> rt_right_shift;

  id_ex_reg_write->ReadReg1Value = Regs[rs];
  id_ex_reg_write->ReadReg2Value = Regs[rt];

  /******* THIRD REGISTER (destination) [5 bits] *******/

  // properly positioned mask for third register
  // 0000 0000 0000 0000 |1111 1|000 0000 0000
  unsigned int rd_mask = 0x0000F800; 
  // proper shift amount for third register
  int rd_right_shift = 11;
  // (logical AND) then perform bit shift to the right
  unsigned int rd = (if_id_reg_read->instruct & rd_mask) 
                     >> rd_right_shift;

  // ignore shamt


  /******* I: CONSTANT/OFFSET [16 bits] *******/

  // properly positioned mask for constant/offset
  // 0000 0000 0000 0000 |1111 1111 1111 1111|
  unsigned int constant_offset_mask = 0x0000FFFF; 
  // logical AND (no need to shift, 
  // but do need a short for negative values)
  short constant_offset = (if_id_reg_read->instruct &              
                           constant_offset_mask);

  id_ex_reg_write->SEOffset = constant_offset;
  id_ex_reg_write->WriteRegNum[0] = rt;
  id_ex_reg_write->WriteRegNum[1] = rd;
}

void Print_out_everything(int *Regs, 
                          struct IF_ID_Reg *if_id_reg_write,
                          struct IF_ID_Reg *if_id_reg_read, 
                          struct ID_EX_Reg *id_ex_reg_write, 
                          struct ID_EX_Reg *id_ex_reg_read) { 
  //displayRegs(Regs);
  printf("IF/ID Write (written to by the IF stage)\n");
  if (if_id_reg_write->instruct == 0x00000000) {
    printf("\tInstruction = 0x%08X\n", 
           if_id_reg_write->instruct);    
  }
  else {
    printf("\tInstruction = 0x%08X, IncrPC = 0x%X\n", 
           if_id_reg_write->instruct, 
           if_id_reg_write->prog_counter);    
  }
  printf("IF/ID Read (read by ID the stage)\n");
  if (if_id_reg_read->instruct == 0x00000000) {
    printf("\tInstruction = 0x%08X\n", 
           if_id_reg_read->instruct);    
  }
  else {
    printf("\tInstruction = 0x%08X, IncrPC = 0x%X\n", 
           if_id_reg_read->instruct, 
           if_id_reg_read->prog_counter);    
  }

  printf("ID/EX Write (written to by the ID stage)\n");
  if (id_ex_reg_write->prog_counter == 0x00000000) {
    printf("\tControl: %08d\n", 
           id_ex_reg_write->prog_counter);    
  }
  else {
    printf("\tIncrPC = 0x%X\n", 
           id_ex_reg_write->prog_counter);   
    printf("\tControl: RegDst=%d, ALUSrc=%d, RegWrite=%d, MemToReg=%d\n", 
           id_ex_reg_write->RegDst, 
           id_ex_reg_write->ALUSrc,
           id_ex_reg_write->RegWrite,
           id_ex_reg_write->MemToReg);
    printf("\t\t MemRead=%d, MemWrite=%d, Branch=%d, ALUOp=%d\n",        
           id_ex_reg_write->MemRead,
           id_ex_reg_write->MemWrite,
           id_ex_reg_write->Branch,
           id_ex_reg_write->ALUOp);
    printf("\tReadReg1Value = %X, ReadReg2Value = %X\n",        
           id_ex_reg_write->ReadReg1Value,
           id_ex_reg_write->ReadReg2Value);
    printf("\tSEOffset = %X, WriteRegNum = %d,%d\n",        
           id_ex_reg_write->SEOffset,
           id_ex_reg_write->WriteRegNum[0],
           id_ex_reg_write->WriteRegNum[1]);

  }
  printf("ID/EX Read (read by the EX stage)\n");
  if (id_ex_reg_read->prog_counter == 0x00000000) {
    printf("\tControl:  %08d\n", 
           id_ex_reg_read->prog_counter);    
  }
  else {
    printf("\tIncrPC = 0x%08X\n", 
           id_ex_reg_read->prog_counter);    
  }
  printf("\n");
}
void Copy_write_to_read(struct IF_ID_Reg *if_id_reg_write,
                        struct IF_ID_Reg *if_id_reg_read, 
                        struct ID_EX_Reg *id_ex_reg_write, 
                        struct ID_EX_Reg *id_ex_reg_read) { 

  *if_id_reg_read = *if_id_reg_write;
  //*id_ex_reg_read = *id_ex_reg_write;

}

int main()
{
  printf("\n\t\t\t<<<<<<< BEGIN PIPELINED-DATAPATH SIMULATION >>>>>>>\n\n");

  /////// ******* REGISTERS ******* ///////

  // create regs
  int Regs[REG_SIZE] = { 0 };

  // give regs initial values 
  // ([0] = 0, [1] = 0x101, [2] = 0x102, ..., [31] = 0x11F)
  for (int i = 1; i < REG_SIZE; i++) {
    //Regs[i] = 0x100 + i;
    Regs[i] = (0x30000 + i);
  }

  // display initial reg values
  //displayRegs(Regs);

  /////// ******* MAIN MEMORY ******* ///////

  // create main memory
  short Main_Mem[MEM_SIZE] = { 0 };

  // give main memory initial values ([0x00] = 0, ..., [0x7FF] = 0xFF)
  short mem_value = 0x00;
  const short MEM_BLOCK_MAX_VALUE = 0xFF; // keeps memory byte-addressable

  for (int i = 0; i < MEM_SIZE; i++) {
    if (mem_value < MEM_BLOCK_MAX_VALUE) {
      Main_Mem[i] = mem_value;
      mem_value++;     
    }
    else if (mem_value == MEM_BLOCK_MAX_VALUE) {
      Main_Mem[i] = mem_value;
      mem_value = 0x00; 
    }
  }

  // display initial memory values
  //displayMainMemory(Main_Mem);

  /////// *******  ******* ///////
  struct IF_ID_Reg *if_id_reg_write = createIF_ID_Reg();
  struct IF_ID_Reg *if_id_reg_read = createIF_ID_Reg();

  struct ID_EX_Reg *id_ex_reg_write = createID_EX_Reg();
  struct ID_EX_Reg *id_ex_reg_read = createID_EX_Reg();

  //struct EX_MEM_Reg ex_mem_reg_write;
  //struct EX_MEM_Reg ex_mem_reg_read;

  //struct MEM_WB_Reg mem_wb_reg_write;
  //struct MEM_WB_Reg mem_wb_reg_read;

/////////// ************************



/*
  unsigned int instructs[] = { 0xA1020000, 
                                  0x810AFFFC,
                                  0x00831820,
                                  0x01263820,
                                  0x01224820,
                                  0x81180000,
                                  0x81510010,
                                  0x00624022,
                                  0x00000000,
                                  0x00000000,
                                  0x00000000,
                                  0x00000000 };
*/



  // loop through instructs
  for (int clock_cycle = 0; clock_cycle < 3; clock_cycle++) { 

      printf("Clock Cycle: %d\n", clock_cycle);

      IF_stage(instructs[clock_cycle],
               if_id_reg_write, 
               if_id_reg_read);

      ID_stage(Regs,
               if_id_reg_read, 
               id_ex_reg_write, 
               id_ex_reg_read);

      Print_out_everything(Regs, 
                           if_id_reg_write, 
                           if_id_reg_read,
                           id_ex_reg_write,
                           id_ex_reg_read);

      Copy_write_to_read(if_id_reg_write, 
                         if_id_reg_read,
                         id_ex_reg_write,
                         id_ex_reg_read);

  }



  return 0;
}
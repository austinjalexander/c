// File: datapath_simulation.c
// Student: Austin J. Alexander
// Assignment: Project 3
// Course: MET CS472 (FALL 2014)

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

/////// ******* SIMULATION OF A DATAPATH ******* ///////

// GLOBAL VARIABLES//
const short REG_SIZE = 32;
const short MEM_SIZE = 1024; // 1K of main memory

// STRUCTURES
// IF_ID
struct IF_ID_Register {
  int program_counter;
  int instruction;
};
struct IF_ID_Register *createIF_ID_Register() {
  struct IF_ID_Register *if_id_register = malloc(sizeof(struct IF_ID_Register));
  assert(if_id_register != NULL);

  if_id_register->program_counter = 0x00000000;
  if_id_register->instruction = 0x00000000;

  return if_id_register;
}
void destroyIF_ID_Register(struct IF_ID_Register *if_id_register) {
  assert(if_id_register != NULL);

  free(if_id_register);
}
// ID_EX
struct ID_EX_Register {
  int program_counter;
  int ReadReg1Value;
  int ReadReg2Value;
  int SEOffset;
  int WriteRegNum[2];
  // control
  short RegDst;
  short ALUSrc;
  short ALUOp;
  short MemRead;
  short MemWrite;
  short Branch;
  short MemToReg;
  short RegWrite;
};
struct ID_EX_Register *createID_EX_Register() {
  struct ID_EX_Register *id_ex_register = malloc(sizeof(struct ID_EX_Register));
  assert(id_ex_register != NULL);

  id_ex_register->program_counter = 0x00000000;
  id_ex_register->ReadReg1Value = 0x00000000;
  id_ex_register->ReadReg2Value = 0x00000000;
  id_ex_register->SEOffset = 0x00000000;
  id_ex_register->WriteRegNum[0] = 0x00000000;
  id_ex_register->WriteRegNum[1] = 0x00000000;
  // control
  id_ex_register->RegDst = 0x00000000;
  id_ex_register->ALUSrc = 0x00000000;
  id_ex_register->ALUOp = 0x00000000;
  id_ex_register->MemRead = 0x00000000;
  id_ex_register->MemWrite = 0x00000000;
  id_ex_register->Branch = 0x00000000;
  id_ex_register->MemToReg = 0x00000000;
  id_ex_register->RegWrite = 0x00000000;

  return id_ex_register;
}
void destroyID_EX_Register(struct ID_EX_Register *id_ex_register) {
  assert(id_ex_register != NULL);

  free(id_ex_register);
}
// EX_MEM
struct EX_MEM_Register {
  int program_counter;
  int instruction;
};
struct EX_MEM_Register *createEX_MEM_Register() {
  struct EX_MEM_Register *ex_mem_register = malloc(sizeof(struct EX_MEM_Register));
  assert(ex_mem_register != NULL);

  ex_mem_register->program_counter = 0x00000000;
  ex_mem_register->instruction = 0x00000000;

  return ex_mem_register;
}
void destroyEX_MEM_Register(struct EX_MEM_Register *ex_mem_register) {
  assert(ex_mem_register != NULL);

  free(ex_mem_register);
}
// MEM_WB
struct MEM_WB_Register {
  int program_counter;
  int instruction;
};
struct MEM_WB_Register *createMEM_WB_Register() {
  struct MEM_WB_Register *mem_wb_register = malloc(sizeof(struct MEM_WB_Register));
  assert(mem_wb_register != NULL);

  mem_wb_register->program_counter = 0x00000000;
  mem_wb_register->instruction = 0x00000000;

  return mem_wb_register;
}
void destroyMEM_WB_Register(struct MEM_WB_Register *mem_wb_register) {
  assert(mem_wb_register != NULL);

  free(mem_wb_register);
}

// FUNCTIONS
void displayRegisters(short *Regs) {
  printf("\nDISPLAY REGISTERS:\n");
  for (int i = 0; i < REG_SIZE; i++) {
    printf("| $%d = 0x%02X |\n", i, Regs[i]);
  }
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

int main()
{
  printf("\n\t\t\t<<<<<<< BEGIN DATAPATH SIMULATION >>>>>>>\n");

  /////// ******* REGISTERS ******* ///////

  // create registers
  short Regs[REG_SIZE] = { 0 };

  // give registers initial values 
  // ([0] = 0, [1] = 0x101, [2] = 0x102, ..., [31] = 0x11F)
  for (int i = 1; i < REG_SIZE; i++) {
    Regs[i] = 0x100 + i;
  }

  // display initial register values
  //displayRegisters(Regs);

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
  struct IF_ID_Register if_id_register_write;
  struct IF_ID_Register if_id_register_read;

  struct ID_EX_Register id_ex_register_write;
  //struct ID_EX_Register id_ex_register_read;

  //struct EX_MEM_Register ex_mem_register_write;
  //struct EX_MEM_Register ex_mem_register_read;

  //struct MEM_WB_Register mem_wb_register_write;
  //struct MEM_WB_Register mem_wb_register_read;

  //CC1.1
  if_id_register_write.program_counter = 0x7A004;
  if_id_register_write.instruction = 0x00A63820;
  
  //CC1.2
  if_id_register_write.program_counter = 0x7A004;
  if_id_register_write.instruction = 0x00A63820;
  if_id_register_read.program_counter = 0x7A004;
  if_id_register_read.instruction = 0x00A63820;

  //CC2.1
  if_id_register_write.program_counter = 0x7A008;
  if_id_register_write.instruction = 0x00A63820;
  if_id_register_read.program_counter = 0x7A004;
  if_id_register_read.instruction = 0x00A63820;

  id_ex_register_write.program_counter = 0x7A004;
  id_ex_register_write.ReadReg1Value = 0x3005;
  id_ex_register_write.ReadReg2Value = 0x3006;
  id_ex_register_write.SEOffset = 0x00000000; // X
  id_ex_register_write.WriteRegNum[0] = 0x00000000;
  id_ex_register_write.WriteRegNum[1] = 0x00000000;
  id_ex_register_write.RegDst = 0x00000000;
  id_ex_register_write.ALUSrc = 0x00000000;
  id_ex_register_write.ALUOp = 0x00000000;
  id_ex_register_write.MemRead = 0x00000000;
  id_ex_register_write.MemWrite = 0x00000000;
  id_ex_register_write.Branch = 0x00000000;
  id_ex_register_write.MemToReg = 0x00000000;
  id_ex_register_write.RegWrite = 0x00000000;

  //printf("0x%X", IF_ID.write);



    




/*
  IF_stage();
  ID_stage();
  EX_stage();
  MEM_stage();
  WB_stage();
  Print_out_everything();
  Copy_write_to_read();
*/


  return 0;
}
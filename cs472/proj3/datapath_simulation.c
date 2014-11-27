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

// STRUCTURES

// --- IF_ID --------------------
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

// --- ID_EX --------------------
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

// --- EX_MEM --------------------
struct EX_MEM_Reg {
  int SWValue;
  int WriteRegNum;
  int ALUResult;
  // control
  short RegWrite;
  short MemToReg;
  short MemRead;
  short MemWrite;
  short Branch;
  short Zero;
  int CalcBTA;
};
struct EX_MEM_Reg *createEX_MEM_Reg() {
  struct EX_MEM_Reg *ex_mem_reg = malloc(sizeof(struct EX_MEM_Reg));
  assert(ex_mem_reg != NULL);

  ex_mem_reg->SWValue = 0x00000000;
  ex_mem_reg->WriteRegNum = 0x00000000;
  ex_mem_reg->ALUResult = 0x00000000;
  // control
  ex_mem_reg->RegWrite = 0;
  ex_mem_reg->MemToReg = 0;
  ex_mem_reg->MemRead = 0;
  ex_mem_reg->MemWrite = 0;
  ex_mem_reg->Branch = 0;
  ex_mem_reg->Zero = 0;
  ex_mem_reg->CalcBTA = 0;

  return ex_mem_reg;
}
void destroyEX_MEM_Reg(struct EX_MEM_Reg *ex_mem_reg) {
  assert(ex_mem_reg != NULL);

  free(ex_mem_reg);
}

// --- MEM_WB --------------------
struct MEM_WB_Reg {
  int LWDataValue;
  int WriteRegNum;
  int ALUResult;
  // control
  short RegWrite;
  short MemToReg;
};
struct MEM_WB_Reg *createMEM_WB_Reg() {
  struct MEM_WB_Reg *mem_wb_reg = malloc(sizeof(struct MEM_WB_Reg));
  assert(mem_wb_reg != NULL);

  mem_wb_reg->LWDataValue = 0x00000000;
  mem_wb_reg->WriteRegNum = 0x00000000;
  mem_wb_reg->ALUResult = 0x00000000;
  // control
  mem_wb_reg->RegWrite = 0;
  mem_wb_reg->MemToReg = 0;

  return mem_wb_reg;
}
void destroyMEM_WB_Reg(struct MEM_WB_Reg *mem_wb_reg) {
  assert(mem_wb_reg != NULL);

  free(mem_wb_reg);
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
        if_id_reg_write->prog_counter = (if_id_reg_write->prog_counter + 0x4);
      }
      if_id_reg_write->instruct = 0x00000000;      
    }
    else {
      if_id_reg_write->prog_counter = (if_id_reg_write->prog_counter + 0x4);

      if_id_reg_write->instruct = instruct;        
    }
}

void ID_stage(int *Regs,
              struct IF_ID_Reg *if_id_reg_read, 
              struct ID_EX_Reg *id_ex_reg_write) {

  if (if_id_reg_read->instruct == 0x00000000) {
    id_ex_reg_write->prog_counter = 0x00000000;
    id_ex_reg_write->ReadReg1Value = 0x00000000;
    id_ex_reg_write->ReadReg2Value = 0x00000000;
    id_ex_reg_write->SEOffset = 0x00000000;
    id_ex_reg_write->WriteRegNum[0] = 0x00000000;
    id_ex_reg_write->WriteRegNum[1] = 0x00000000;
    // control
    id_ex_reg_write->RegDst = 0;
    id_ex_reg_write->ALUSrc = 0;
    id_ex_reg_write->RegWrite = 0;
    id_ex_reg_write->MemToReg = 0;
    id_ex_reg_write->MemRead = 0;
    id_ex_reg_write->MemWrite = 0;
    id_ex_reg_write->Branch = 0;
    id_ex_reg_write->ALUOp = 0;
  }
  else {
    id_ex_reg_write->prog_counter = if_id_reg_read->prog_counter;


    /*****   

    AFTER DECODE, DO ACTUAL REGISTER FETCH

    */

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
    // if R-format
    if (opcode == 0x00) {
      id_ex_reg_write->RegDst = 1; // for mux to choose rd
      id_ex_reg_write->ALUSrc = 0;
      id_ex_reg_write->RegWrite = 1;
      id_ex_reg_write->MemToReg = 0;
      id_ex_reg_write->MemRead = 0;
      id_ex_reg_write->MemWrite = 0;
      id_ex_reg_write->Branch = 0;
      id_ex_reg_write->ALUOp = 10;
    }
    // if lw
    else if (opcode == 0x23) {
      id_ex_reg_write->RegDst = 0;
      id_ex_reg_write->ALUSrc = 1;
      id_ex_reg_write->RegWrite = 1;
      id_ex_reg_write->MemToReg = 1;
      id_ex_reg_write->MemRead = 1;
      id_ex_reg_write->MemWrite = 0;
      id_ex_reg_write->Branch = 0;
      id_ex_reg_write->ALUOp = 00;
    }
    // if sw
    else if (opcode == 0x2B) {
      id_ex_reg_write->RegDst = 1; // mux choose rd (like example)
      id_ex_reg_write->ALUSrc = 1;
      id_ex_reg_write->RegWrite = 0;
      id_ex_reg_write->MemToReg = 1; // copy RegDst (both are 'X')
      id_ex_reg_write->MemRead = 0;
      id_ex_reg_write->MemWrite = 1;
      id_ex_reg_write->Branch = 0;
      id_ex_reg_write->ALUOp = 00;
    }
    // if beq
    else if (opcode == 0x04) {
      id_ex_reg_write->RegDst = 1; // mux choose rd (like example)
      id_ex_reg_write->ALUSrc = 0;
      id_ex_reg_write->RegWrite = 0;
      id_ex_reg_write->MemToReg = 1; // copy RegDst (both are 'X')
      id_ex_reg_write->MemRead = 0;
      id_ex_reg_write->MemWrite = 0;
      id_ex_reg_write->Branch = 1;
      id_ex_reg_write->ALUOp = 01;
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

    /******* R: THIRD REGISTER (destination) [5 bits] *******/

    // properly positioned mask for third register
    // 0000 0000 0000 0000 |1111 1|000 0000 0000
    unsigned int rd_mask = 0x0000F800; 
    // proper shift amount for third register
    int rd_right_shift = 11;
    // (logical AND) then perform bit shift to the right
    unsigned int rd = (if_id_reg_read->instruct & rd_mask) 
                       >> rd_right_shift;

    if (opcode == 0x00) {
      // ignore shamt

      /******* FUNCTION [6 bits] *******/

      // properly positioned mask for function
      // 0000 0000 0000 0000 0000 0000 00|11 1111|
      unsigned int funct_mask = 0x0000003F; 
      // logical AND (no need to shift)
      unsigned int funct = (if_id_reg_read->instruct & funct_mask);

      id_ex_reg_write->SEOffset = funct;

    }
    else {
      /******* I: CONSTANT/OFFSET [16 bits] *******/

      // properly positioned mask for constant/offset
      // 0000 0000 0000 0000 |1111 1111 1111 1111|
      unsigned int constant_offset_mask = 0x0000FFFF; 
      // logical AND (no need to shift, 
      // but do need a short for negative values)
      short constant_offset = (if_id_reg_read->instruct &              
                               constant_offset_mask);

      id_ex_reg_write->SEOffset = constant_offset;  
    }

    // when R-format, sw
    if (id_ex_reg_write->RegDst == 1) {
      id_ex_reg_write->WriteRegNum[0] = rt;
      id_ex_reg_write->WriteRegNum[1] = rd;     
    }
    // when lw
    else if (id_ex_reg_write->RegDst == 0) {
      id_ex_reg_write->WriteRegNum[0] = rs;
      id_ex_reg_write->WriteRegNum[1] = rt; 
    }
  }
}

void EX_stage(struct ID_EX_Reg *id_ex_reg_read, 
              struct EX_MEM_Reg *ex_mem_reg_write) {
  /****    
  PERFORM REQUESTED INSTRUCTION
  FOR EXAMPLE add:
  EX_MEM_WRITE.ALU_Result = ID_EX_READ.Reag_Val1 + ID_EX_READ.Reg_Val2;

  */

  if (id_ex_reg_read->RegDst == 0 && id_ex_reg_read->ALUSrc == 0 && id_ex_reg_read->RegWrite == 0 && id_ex_reg_read->MemToReg == 0 && id_ex_reg_read->MemRead == 0 && id_ex_reg_read->MemWrite == 0 && id_ex_reg_read->Branch == 0 && id_ex_reg_read->ALUOp == 0) {

    ex_mem_reg_write->SWValue = 0;
    ex_mem_reg_write->WriteRegNum = 0;
    ex_mem_reg_write->ALUResult = 0;
    // control
    ex_mem_reg_write->RegWrite = 0;
    ex_mem_reg_write->MemToReg = 0;
    ex_mem_reg_write->MemRead = 0;
    ex_mem_reg_write->MemWrite = 0;
    ex_mem_reg_write->Branch = 0;

    ex_mem_reg_write->Zero = 0;
    ex_mem_reg_write->CalcBTA = 0;  
  }
  else {
    ex_mem_reg_write->SWValue = id_ex_reg_read->ReadReg2Value;
    ex_mem_reg_write->WriteRegNum = id_ex_reg_read->WriteRegNum[1];

    // MUX: ALU  (really should make this a function)
    // if R-format
    if (id_ex_reg_read->ALUOp == 10) {

      // if add
      if (id_ex_reg_read->SEOffset == 0x20) {
        ex_mem_reg_write->ALUResult = (id_ex_reg_read->ReadReg1Value + id_ex_reg_read->ReadReg2Value);        
      }
      // if sub
      else if (id_ex_reg_read->SEOffset == 0x22) {
        ex_mem_reg_write->ALUResult = (id_ex_reg_read->ReadReg1Value - id_ex_reg_read->ReadReg2Value);
      }

    }
    // if I-format
    else if (id_ex_reg_read->ALUOp == 00 || id_ex_reg_read->ALUOp == 01) {

      // if lw or sw
      if (id_ex_reg_read->ALUSrc == 1) {
        ex_mem_reg_write->ALUResult = (id_ex_reg_read->ReadReg1Value + id_ex_reg_read->SEOffset);       
      }
      // if beq
      else if (id_ex_reg_read->ALUSrc == 0) {
        ex_mem_reg_write->ALUResult = (id_ex_reg_read->ReadReg1Value - id_ex_reg_read->ReadReg2Value); 
      }
      
    }

    // control
    ex_mem_reg_write->RegWrite = id_ex_reg_read->RegWrite;
    ex_mem_reg_write->MemToReg = id_ex_reg_read->MemToReg;
    ex_mem_reg_write->MemRead = id_ex_reg_read->MemRead;
    ex_mem_reg_write->MemWrite = id_ex_reg_read->MemWrite;
    ex_mem_reg_write->Branch = id_ex_reg_read->Branch;

    if (ex_mem_reg_write->ALUResult == 0) {
      ex_mem_reg_write->Zero = 1;
    }
    else {
      ex_mem_reg_write->Zero = 0;
    }

    // CalcBTA is based on IncrPC, 
    // with the constant/offset shifted right by two bits,
    // so shift left the constant/offeset to recover actual byte offset
    ex_mem_reg_write->CalcBTA = (id_ex_reg_read->prog_counter + (id_ex_reg_read->SEOffset << 2));
  }
}

void MEM_stage(struct EX_MEM_Reg *ex_mem_reg_read, 
               struct MEM_WB_Reg *mem_wb_reg_write) {
  if (ex_mem_reg_read->RegWrite == 0 && ex_mem_reg_read->MemToReg == 0 && ex_mem_reg_read->MemRead == 0 && ex_mem_reg_read->MemWrite == 0 && ex_mem_reg_read->Branch == 0) {

  /****    
  if insturctin is lb, use address calculated in EX stage as an index 
  into main memory array and get value there
  otherwise, pass info from read version of EX_MEM pipeline reg to the write version of MEM_WB

  */

    mem_wb_reg_write->LWDataValue = 0;
    mem_wb_reg_write->WriteRegNum = 0;
    mem_wb_reg_write->ALUResult = 0;
    // control
    mem_wb_reg_write->RegWrite = 0;
    mem_wb_reg_write->MemToReg = 0;
  }
  else {                // main memory contents @ ALUResult
    mem_wb_reg_write->LWDataValue = ex_mem_reg_read->ALUResult;
    mem_wb_reg_write->WriteRegNum = ex_mem_reg_read->WriteRegNum;
    mem_wb_reg_write->ALUResult = ex_mem_reg_read->ALUResult;
    // control
    mem_wb_reg_write->RegWrite = ex_mem_reg_read->RegWrite;
    mem_wb_reg_write->MemToReg = ex_mem_reg_read->MemToReg;
  }
}

void WB_stage(struct MEM_WB_Reg *mem_wb_reg_read) {
  /****    
  write to registers based on inforamation read out of MEM_WB_READ

  */
}

// DISPLAY FUNCTIONS 
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

void Print_out_everything(int *Regs, 
                          struct IF_ID_Reg *if_id_reg_write,
                          struct IF_ID_Reg *if_id_reg_read, 
                          struct ID_EX_Reg *id_ex_reg_write, 
                          struct ID_EX_Reg *id_ex_reg_read, 
                          struct EX_MEM_Reg *ex_mem_reg_write, 
                          struct EX_MEM_Reg *ex_mem_reg_read,
                          struct MEM_WB_Reg *mem_wb_reg_write, 
                          struct MEM_WB_Reg *mem_wb_reg_read) { 
  //displayRegs(Regs);

  // IF/ID
  printf("\n---<IF/ID>--------------------\n");
  // Write
  printf("\tIF/ID Write (written to by the IF stage)\n");
  if (if_id_reg_write->instruct == 0x00000000) {
    printf("\t\tInstruction = 0x%08X\n", 
           if_id_reg_write->instruct);    
  }
  else {
    printf("\t\tInstruction = 0x%08X, IncrPC = 0x%X\n", 
           if_id_reg_write->instruct, 
           if_id_reg_write->prog_counter);    
  }
  // Read
  printf("\n\tIF/ID Read (read by ID the stage)\n");
  if (if_id_reg_read->instruct == 0x00000000) {
    printf("\t\tInstruction = 0x%08X\n", 
           if_id_reg_read->instruct);    
  }
  else {
    printf("\t\tInstruction = 0x%08X, IncrPC = 0x%X\n", 
           if_id_reg_read->instruct, 
           if_id_reg_read->prog_counter);    
  }

  // ID/EX
  printf("\n---<ID/EX>--------------------\n");
  // Write
  printf("\tID/EX Write (written to by the ID stage)\n");
  if (id_ex_reg_write->prog_counter == 0x00000000 && id_ex_reg_write->ReadReg1Value == 0x00000000 && id_ex_reg_write->ReadReg2Value == 0x00000000 && id_ex_reg_write->SEOffset == 0x00000000 && id_ex_reg_write->WriteRegNum[0] == 0x00000000 && id_ex_reg_write->WriteRegNum[1] == 0x00000000 && id_ex_reg_write->RegDst == 0 && id_ex_reg_write->ALUSrc == 0 && id_ex_reg_write->RegWrite == 0 && id_ex_reg_write->MemToReg == 0 && id_ex_reg_write->MemRead == 0 && id_ex_reg_write->MemWrite == 0 && id_ex_reg_write->Branch == 0 && id_ex_reg_write->ALUOp == 0) {  
    printf("\t\tControl: %08d\n", 
           id_ex_reg_write->prog_counter);    
  }
  else {
    // if RegWrite == 0,
    // indicate that RegDst and MemToReg are garbage
    if (id_ex_reg_write->RegWrite == 0) {
      printf("\t\tControl: RegDst=X, ALUSrc=%d, ALUOp=%02d, MemRead=%d, MemWrite=%d\n", 
             id_ex_reg_write->ALUSrc,
             id_ex_reg_write->ALUOp,
             id_ex_reg_write->MemRead,
             id_ex_reg_write->MemWrite);  
      printf("\t\t\t Branch=%d, MemToReg=X, RegWrite=%d\n",        
             id_ex_reg_write->Branch,
             id_ex_reg_write->RegWrite);
    }
    // otherwise,
    // show RegDst and MemToReg
    else {
      printf("\t\tControl: RegDst=%d, ALUSrc=%d, ALUOp=%02d, MemRead=%d, MemWrite=%d\n", 
             id_ex_reg_write->RegDst, 
             id_ex_reg_write->ALUSrc,
             id_ex_reg_write->ALUOp,
             id_ex_reg_write->MemRead,
             id_ex_reg_write->MemWrite);  
      printf("\t\t\t Branch=%d, MemToReg=%d, RegWrite=%d\n",        
             id_ex_reg_write->Branch,
             id_ex_reg_write->MemToReg,
             id_ex_reg_write->RegWrite);
    } 
    printf("\t\tIncrPC = 0x%X\n\t\tReadReg1Value = %X, ReadReg2Value = %X\n", 
           id_ex_reg_write->prog_counter,  
           id_ex_reg_write->ReadReg1Value,
           id_ex_reg_write->ReadReg2Value);
    // if ALUOp is from a R-format instruction,
    // indicate that SEOffset is garbage
    if (id_ex_reg_write->ALUOp == 10) {
      printf("\t\tSEOffset = X\tWriteRegNum = %d,%d\n",        
             id_ex_reg_write->WriteRegNum[0],
             id_ex_reg_write->WriteRegNum[1]);
    }
    // otherwise,
    // show SEOffset
    else {
      printf("\t\tSEOffset = %08X\tWriteRegNum = %d,%d\n",        
             id_ex_reg_write->SEOffset,
             id_ex_reg_write->WriteRegNum[0],
             id_ex_reg_write->WriteRegNum[1]);
    }
  }
  // Read
  printf("\n\tID/EX Read (read by the EX stage)\n");
  if (id_ex_reg_read->prog_counter == 0x00000000 && id_ex_reg_read->ReadReg1Value == 0x00000000 && id_ex_reg_read->ReadReg2Value == 0x00000000 && id_ex_reg_read->SEOffset == 0x00000000 && id_ex_reg_read->WriteRegNum[0] == 0x00000000 && id_ex_reg_read->WriteRegNum[1] == 0x00000000 && id_ex_reg_read->RegDst == 0 && id_ex_reg_read->ALUSrc == 0 && id_ex_reg_read->RegWrite == 0 && id_ex_reg_read->MemToReg == 0 && id_ex_reg_read->MemRead == 0 && id_ex_reg_read->MemWrite == 0 && id_ex_reg_read->Branch == 0 && id_ex_reg_read->ALUOp == 0) {
    printf("\t\tControl: %08d\n", 
           id_ex_reg_read->prog_counter);    
  }
  else { 
    // if RegWrite == 0,
    // indicate that RegDst and MemToReg are garbage
    if (id_ex_reg_read->RegWrite == 0) {
      printf("\t\tControl: RegDst=X, ALUSrc=%d, ALUOp=%02d, MemRead=%d, MemWrite=%d\n", 
             id_ex_reg_read->ALUSrc,
             id_ex_reg_read->ALUOp,
             id_ex_reg_read->MemRead,
             id_ex_reg_read->MemWrite);  
      printf("\t\t\t Branch=%d, MemToReg=X, RegWrite=%d\n",        
             id_ex_reg_read->Branch,
             id_ex_reg_read->RegWrite);
    }
    // otherwise,
    // show RegDst and MemToReg
    else {
      printf("\t\tControl: RegDst=%d, ALUSrc=%d, ALUOp=%02d, MemRead=%d, MemWrite=%d\n", 
             id_ex_reg_read->RegDst, 
             id_ex_reg_read->ALUSrc,
             id_ex_reg_read->ALUOp,
             id_ex_reg_read->MemRead,
             id_ex_reg_read->MemWrite);  
      printf("\t\t\t Branch=%d, MemToReg=%d, RegWrite=%d\n",        
             id_ex_reg_read->Branch,
             id_ex_reg_read->MemToReg,
             id_ex_reg_read->RegWrite);
    } 
    printf("\t\tIncrPC = 0x%X\n\t\tReadReg1Value = %X, ReadReg2Value = %X\n", 
           id_ex_reg_read->prog_counter,  
           id_ex_reg_read->ReadReg1Value,
           id_ex_reg_read->ReadReg2Value);
    // if ALUOp is from a R-format instruction,
    // indicate that SEOffset is garbage
    if (id_ex_reg_read->ALUOp == 10) {
      printf("\t\tSEOffset = X\tWriteRegNum = %d,%d\n",        
             id_ex_reg_read->WriteRegNum[0],
             id_ex_reg_read->WriteRegNum[1]);
    }
    // otherwise,
    // show SEOffset
    else {
      printf("\t\tSEOffset = %08X\tWriteRegNum = %d,%d\n",        
             id_ex_reg_read->SEOffset,
             id_ex_reg_read->WriteRegNum[0],
             id_ex_reg_read->WriteRegNum[1]);
    }
  }

  // EX/MEM
  printf("\n---<EX/MEM>--------------------\n");
  // Write
  printf("\tEX/MEM Write (written to by the EX stage)\n");
  if (ex_mem_reg_write->MemRead == 0 && ex_mem_reg_write->MemWrite == 0 && ex_mem_reg_write->Branch == 0 && ex_mem_reg_write->MemToReg == 0 && ex_mem_reg_write->RegWrite == 0 && ex_mem_reg_write->CalcBTA == 0 && ex_mem_reg_write->Zero == 0 && ex_mem_reg_write->ALUResult == 0) {
    printf("\t\tControl: %08d\n", 0);    
  }
  else { 
    // if RegWrite == 0,
    // indicate that MemToReg is garbage
    if (ex_mem_reg_write->RegWrite == 0) {
      printf("\t\tControl: MemRead=%d, MemWrite=%d, Branch=%d, MemToReg=X\n",        
             ex_mem_reg_write->MemRead,
             ex_mem_reg_write->MemWrite,
             ex_mem_reg_write->Branch);
    }
    // otherwise,
    // show MemToReg
    else {
      printf("\t\tControl: MemRead=%d, MemWrite=%d, Branch=%d, MemToReg=%d\n",        
             ex_mem_reg_write->MemRead,
             ex_mem_reg_write->MemWrite,
             ex_mem_reg_write->Branch,
             ex_mem_reg_write->MemToReg);
    }

    // if Branch == 0,
    // indicate that CalcBTA is garbage
    if (ex_mem_reg_write->Branch == 0) {
      printf("\t\t\t RegWrite=%X\n\t\tCalcBTA = X, Zero = %X\tALUResult = %X\n",        
             ex_mem_reg_write->RegWrite,
             ex_mem_reg_write->Zero,
             ex_mem_reg_write->ALUResult);
    }
    // otherwise,
    // show CalcBTA
    else {
      printf("\t\t\t RegWrite=%X\n\t\tCalcBTA = %X, Zero = %X\tALUResult = %X\n",        
             ex_mem_reg_write->RegWrite,
             ex_mem_reg_write->CalcBTA,
             ex_mem_reg_write->Zero,
             ex_mem_reg_write->ALUResult);
    }

    // if RegWrite == 0,
    // indicate that WriteRegNum is garbage
    if (ex_mem_reg_write->RegWrite == 0) {
      printf("\t\tSWValue = %X, WriteRegNum = X\n",        
             ex_mem_reg_write->SWValue);
    }
    // otherwise,
    // show WriteRegNum
    else {
      printf("\t\tSWValue = %X, WriteRegNum = %d\n",        
             ex_mem_reg_write->SWValue,
             ex_mem_reg_write->WriteRegNum);
    }

  }
  // Read
  printf("\n\tEX/MEM Read (read by the MEM stage)\n");
  if (ex_mem_reg_read->MemRead == 0 && ex_mem_reg_read->MemWrite == 0 && ex_mem_reg_read->Branch == 0 && ex_mem_reg_read->MemToReg == 0 && ex_mem_reg_read->RegWrite == 0 && ex_mem_reg_read->CalcBTA == 0 && ex_mem_reg_read->Zero == 0 && ex_mem_reg_read->ALUResult == 0) {
    printf("\t\tControl: %08d\n", 0);    
  }
  else { 
    // if RegWrite == 0,
    // indicate that MemToReg is garbage
    if (ex_mem_reg_read->RegWrite == 0) {
      printf("\t\tControl: MemRead=%d, MemWrite=%d, Branch=%d, MemToReg=X\n",        
             ex_mem_reg_read->MemRead,
             ex_mem_reg_read->MemWrite,
             ex_mem_reg_read->Branch);
    }
    // otherwise,
    // show MemToReg
    else {
      printf("\t\tControl: MemRead=%d, MemWrite=%d, Branch=%d, MemToReg=%d\n",        
             ex_mem_reg_read->MemRead,
             ex_mem_reg_read->MemWrite,
             ex_mem_reg_read->Branch,
             ex_mem_reg_read->MemToReg);
    }

    // if Branch == 0,
    // indicate that CalcBTA is garbage
    if (ex_mem_reg_read->Branch == 0) {
      printf("\t\t\t RegWrite=%X\n\t\tCalcBTA = X, Zero = %X\tALUResult = %X\n",        
             ex_mem_reg_read->RegWrite,
             ex_mem_reg_read->Zero,
             ex_mem_reg_read->ALUResult);
    }
    // otherwise,
    // show CalcBTA
    else {
      printf("\t\t\t RegWrite=%X\n\t\tCalcBTA = %X, Zero = %X\tALUResult = %X\n",        
             ex_mem_reg_read->RegWrite,
             ex_mem_reg_read->CalcBTA,
             ex_mem_reg_read->Zero,
             ex_mem_reg_read->ALUResult);
    }

    // if RegWrite == 0,
    // indicate that WriteRegNum is garbage
    if (ex_mem_reg_read->RegWrite == 0) {
      printf("\t\tSWValue = %X, WriteRegNum = X\n",        
             ex_mem_reg_read->SWValue);
    }
    // otherwise,
    // show WriteRegNum
    else {
      printf("\t\tSWValue = %X, WriteRegNum = %d\n",        
             ex_mem_reg_read->SWValue,
             ex_mem_reg_read->WriteRegNum);
    }
  }

  // MEM/WB
  printf("\n---<MEM/WB>--------------------\n");
  // Write
  printf("\tMEM/WB Write (written to by the MEM stage)\n");
  if (mem_wb_reg_write->MemToReg == 0 && mem_wb_reg_write->RegWrite == 0 && mem_wb_reg_write->LWDataValue == 0 && mem_wb_reg_write->WriteRegNum == 0 && mem_wb_reg_write->ALUResult == 0) {
    printf("\t\tControl: %08d\n", 0);    
  }
  else { 
    // if RegWrite == 0,
    // indicate that MemToReg is garbage
    if (mem_wb_reg_write->RegWrite == 0) {
    printf("\t\tControl:  MemToReg=X, RegWrite=%X\n",        
           mem_wb_reg_write->RegWrite);
    }
    // otherwise,
    // show MemToReg
    else {
    printf("\t\tControl:  MemToReg=%d, RegWrite=%X\n",        
           mem_wb_reg_write->MemToReg,
           mem_wb_reg_write->RegWrite);
    }

    // if RegWrite == 0 || MemToReg == 0,
    // indicate that LWDataValue is garbage
    if (mem_wb_reg_write->RegWrite == 0 || mem_wb_reg_write->MemToReg == 0) {
    printf("\t\tLWDataValue = X,\n\t\tALUResult = %X, ",        
           mem_wb_reg_write->ALUResult);
    }
    // otherwise,
    // show LWDataValue
    else {
    printf("\t\tLWDataValue = mem contents @ %X,\n\t\tALUResult = %X, ",        
           mem_wb_reg_write->LWDataValue,
           mem_wb_reg_write->ALUResult);
    }

    // if RegWrite == 0
    // indicate that WriteRegNum is garbage
    if (mem_wb_reg_write->RegWrite == 0) {
    printf("WriteRegNum = X\n");
    }
    // otherwise,
    // show WriteRegNum
    else {
    printf("WriteRegNum = %d\n",        
           mem_wb_reg_write->WriteRegNum);
    }

  }
  // Read
  printf("\n\tMEM/WB Read (read by the WB stage)\n");
  if (mem_wb_reg_read->MemToReg == 0 && mem_wb_reg_read->RegWrite == 0 && mem_wb_reg_read->LWDataValue == 0 && mem_wb_reg_read->WriteRegNum == 0 && mem_wb_reg_read->ALUResult == 0) {
    printf("\t\tControl: %08d\n", 0);    
  }
  else { 
    // if RegWrite == 0,
    // indicate that MemToReg is garbage
    if (mem_wb_reg_read->RegWrite == 0) {
    printf("\t\tControl:  MemToReg=X, RegWrite=%X\n",        
           mem_wb_reg_read->RegWrite);
    }
    // otherwise,
    // show MemToReg
    else {
    printf("\t\tControl:  MemToReg=%d, RegWrite=%X\n",        
           mem_wb_reg_read->MemToReg,
           mem_wb_reg_read->RegWrite);
    }

    // if RegWrite == 0 || MemToReg == 0,
    // indicate that LWDataValue is garbage
    if (mem_wb_reg_read->RegWrite == 0 || mem_wb_reg_read->MemToReg == 0) {
    printf("\t\tLWDataValue = X,\n\t\tALUResult = %X, ",        
           mem_wb_reg_read->ALUResult);
    }
    // otherwise,
    // show LWDataValue
    else {
    printf("\t\tLWDataValue = mem contents @ %X,\n\t\tALUResult = %X, ",        
           mem_wb_reg_read->LWDataValue,
           mem_wb_reg_read->ALUResult);
    }

    // if RegWrite == 0
    // indicate that WriteRegNum is garbage
    if (mem_wb_reg_read->RegWrite == 0) {
    printf("WriteRegNum = X\n");
    }
    // otherwise,
    // show WriteRegNum
    else {
    printf("WriteRegNum = %d\n",        
           mem_wb_reg_read->WriteRegNum);
    }
  }

  printf("\n");
}

void Copy_write_to_read(struct IF_ID_Reg *if_id_reg_write,
                        struct IF_ID_Reg *if_id_reg_read, 
                        struct ID_EX_Reg *id_ex_reg_write, 
                        struct ID_EX_Reg *id_ex_reg_read, 
                        struct EX_MEM_Reg *ex_mem_reg_write,
                        struct EX_MEM_Reg *ex_mem_reg_read,
                        struct MEM_WB_Reg *mem_wb_reg_write, 
                        struct MEM_WB_Reg *mem_wb_reg_read) {  

  *if_id_reg_read = *if_id_reg_write;
  *id_ex_reg_read = *id_ex_reg_write;
  *ex_mem_reg_read = *ex_mem_reg_write;
  *mem_wb_reg_read = *mem_wb_reg_write;
}

int main()
{
  printf("\n<<<<<<< BEGIN PIPELINED-DATAPATH SIMULATION >>>>>>>\n\n");


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


  /////// ******* PIPELINE REGISTERS ******* ///////

  // create pipeline registers
  struct IF_ID_Reg *if_id_reg_write = createIF_ID_Reg();
  struct IF_ID_Reg *if_id_reg_read = createIF_ID_Reg();

  struct ID_EX_Reg *id_ex_reg_write = createID_EX_Reg();
  struct ID_EX_Reg *id_ex_reg_read = createID_EX_Reg();

  struct EX_MEM_Reg *ex_mem_reg_write = createEX_MEM_Reg();
  struct EX_MEM_Reg *ex_mem_reg_read = createEX_MEM_Reg();

  struct MEM_WB_Reg *mem_wb_reg_write = createMEM_WB_Reg();
  struct MEM_WB_Reg *mem_wb_reg_read = createMEM_WB_Reg();


  /////// ******* PRIMARY PROGRAM LOOP ******* ///////

  // loop through instructions
  for (int clock_cycle = 0; clock_cycle < 8; clock_cycle++) { 

      printf("\t\t[ Clock Cycle: %d ]\n", clock_cycle);

      IF_stage(instructs[clock_cycle],
               if_id_reg_write, 
               if_id_reg_read);

      ID_stage(Regs,
               if_id_reg_read, 
               id_ex_reg_write);

      EX_stage(id_ex_reg_read,
               ex_mem_reg_write);

      MEM_stage(ex_mem_reg_read,
               mem_wb_reg_write);

      Print_out_everything(Regs, 
                           if_id_reg_write, 
                           if_id_reg_read,
                           id_ex_reg_write,
                           id_ex_reg_read,                          
                           ex_mem_reg_write,
                           ex_mem_reg_read,
                           mem_wb_reg_write,
                           mem_wb_reg_read);

      Copy_write_to_read(if_id_reg_write, 
                         if_id_reg_read,
                         id_ex_reg_write,
                         id_ex_reg_read,                          
                         ex_mem_reg_write,
                         ex_mem_reg_read,
                         mem_wb_reg_write,
                         mem_wb_reg_read);


      Print_out_everything(Regs, 
                           if_id_reg_write, 
                           if_id_reg_read,
                           id_ex_reg_write,
                           id_ex_reg_read,                          
                           ex_mem_reg_write,
                           ex_mem_reg_read,
                           mem_wb_reg_write,
                           mem_wb_reg_read);

      printf("******************************");
      printf("******************************\n\n");
  }

  printf("\n<<<<<<< END PIPELINED-DATAPATH SIMULATION >>>>>>>\n\n");
  return 0;
}
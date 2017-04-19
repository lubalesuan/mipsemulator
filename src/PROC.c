
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include "RegFile.h"
#include "Syscall.h"
#include "utils/heap.h"
#include "elf_reader/elf_reader.h"
#include "instructions.h"
#include "global.h"
//Stats

uint32_t DynInstCount = 0;
uint32_t PC;
int32_t *regfile;
void write_initialization_vector(uint32_t sp, uint32_t gp, uint32_t start) {
        printf("\n ----- BOOT Sequence ----- \n");
        printf("Initializing sp=0x%08x; gp=0x%08x; start=0x%08x\n", sp, gp, start);
        regfile[28] = gp;
        regfile[29] = sp;
        regfile[31] = start;
        printRegFile();

    }


int main(int argc, char * argv[]) {
  
    int MaxInst = 0;
    int status = 0;
    uint32_t i; 
    // uint32_tnewPC;
    uint32_t CurrentInstruction;

    if (argc < 2) {
      printf("Input argument missing \n");
      return -1;
    }
    sscanf (argv[2], "%d", &MaxInst);

    //Open file pointers & initialize Heap & Regsiters
    initHeap();
    initFDT();
    initRegFile(0);
    //get regfile
    regfile = returnRegFile();
    //Load required code portions into Emulator Memory
    status =  LoadOSMemory(argv[1]);
    if(status <0) { return status; }
    
    //set Global & Stack Pointers for the Emulator
    // & provide startAddress of Program in Memory to Processor
    write_initialization_vector(exec.GSP, exec.GP, exec.GPC_START);
   
    printf("\n ----- Execute Program ----- \n");
    printf("Max Instruction to run = %d \n",MaxInst);
    PC = exec.GPC_START;
     printf("%d \n", PC); 
    for(i=0; i<MaxInst ; i++) {
        DynInstCount++;
        CurrentInstruction = readWord(PC,true); 
        printf("instruction %x \n", CurrentInstruction);
        printf("print smth %s \n", "");
        // printReg(CurrentInstruction, 32);
        parseInstruction(CurrentInstruction);
        // printRegFile();
        PC= PC+4;
        printf("%d: %d \n", i, PC); 
    } //end fori
    
    
    //Close file pointers & free allocated Memory
    closeFDT();
    CleanUp();
    return 1;
}














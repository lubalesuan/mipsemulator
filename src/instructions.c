// all instruction code in here
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include "RegFile.h"
#include "Syscall.h"
#include "utils/heap.h"
#include "elf_reader/elf_reader.h"
#include "instructions.h"
#include <inttypes.h>
#include "global.h"



int parseInstruction(uint32_t instruction) {
	//struct jform *instr = (struct jform *)malloc(sizeof(struct jform));
	struct jform *instrJ = (struct jform*)&instruction;
	 switch (instrJ->opcode) {
	 	case 0x00: {
	 		struct rform *instrR = (struct rform*)&instruction;
	 		return parseRForm(*instrR); //parsing for r format operations
	 	}
	 	case 0x02:
	 		return op_j(instruction);
	 	case 0x03:
	 		return op_jal(instruction);
	 	default: {
	 		struct iform *instrI = (struct iform*)&instruction;
	 		return parseIForm(*instrI);
	 	}
	 }
	 //return 0;
}


int parseRForm (struct rform instruction) {
	// printR("rform ", regfile[instruction.rd], regfile[instruction.rs], regfile[instruction.rt]);
	// printReg(instruction.funct, 6);
	printf("opcode  %d  rs %d rt %d funct %d shamt %d\n", instruction.opcode, instruction.rs, instruction.rt, instruction.funct, instruction.shamt);
	switch (instruction.funct) {
		case 0x20:
			return op_add(instruction);
		case 0x21:
			return op_addu (instruction);
		case 0x22:
			return op_sub (instruction);
		case 0x23:
			return op_subu (instruction);
		case 0x1A:
			return op_div (instruction);
		case 0x1B:
			return op_divu (instruction);
		case 0x18:
			return op_mult (instruction);
		case 0x19:
			return op_multu (instruction);
		case 0x10:
			return op_mfhi (instruction);
		case 0x12:
			return op_mflo (instruction);
		case 17:
			return op_mthi (instruction);
		case 19:
			return op_mtlo (instruction);
		case 36:
			return op_and (instruction);
		case 38:
			return op_xor (instruction);
		case 39:
			return op_nor (instruction);
		case 37:
			return op_or (instruction);
		case 0:
			return op_sll (instruction);
		case 42:
			return op_slt (instruction);
		case 43:
			return op_slt (instruction);
		case 3:
			return op_sra (instruction);
		case 12: {
	 		struct syscallform *instrSysCall = (struct syscallform *)&instruction;
	 		return syscallForm(*instrSysCall);
	 	}
		case 7:
			return op_srav (instruction);
		case 2:
			return op_srl (instruction);
		case 6:
			return op_srlv (instruction);
		case 9:
			return op_jalr (instruction);
		case 8:
			return op_jr (instruction);
		case 4:
			return op_sllv (instruction);
		default: //check if sll  = nop case 0
			return op_nop (instruction);
	}
}


int syscallForm (struct syscallform instruction) {
	op_syscall(instruction);
	return 0;
}
int parseIForm (struct iform instruction) {
	// printI("iform ", regfile[instruction.rs], regfile[instruction.rt], instruction.constaddr);
	printf("opcode  %d  rs %d rt %d imm %d\n", instruction.opcode, instruction.rs, instruction.rt, instruction.constaddr);

switch (instruction.opcode) {
	case 0x08: 
		return op_addi(instruction);
	case 0x09: 
		return op_addiu(instruction);
	case 0x0C: 
		return op_andi(instruction);
	case 0xE: 
		return op_xori(instruction);
	case 0x0D: 
		return op_ori(instruction);
	case 0x0A: 
		return op_slti(instruction);
	case 0x0B: 
		return op_sltiu(instruction);
	case 0x04: 
		return op_beq(instruction);
	case 0x14: 
		return op_beql(instruction);
	case 0x1: {
		switch (instruction.rt) {
			case 1: return op_bgez(instruction);
			// case 10001: return op_bgezal(instruction);
			case 0: return op_bltz(instruction);
			case 16: return op_bltzal(instruction);
			default: return 0;
		}
		
	}
	case 7: 
		return op_bgtz(instruction);
	case 6: 
		return op_blez(instruction);
	case 22: 
		return op_blezl(instruction);
	case 5: 
		return op_bne(instruction);
	case 21: 
		return op_bnel(instruction);
	case 32: 
		return op_lb(instruction);
	case 36: 
		return op_lbu(instruction);	
	case 33: 
		return op_lh(instruction);	
	case 37: 
		return op_lhu(instruction);	
	case 15: 
		return op_lui(instruction);	
	case 35: 
		return op_lw(instruction);	
	case 34: 
		return op_lwl(instruction);	
	case 38: 
		return op_lwr(instruction);
	case 40: 
		return op_sb(instruction);
	case 41: 
		return op_sh(instruction);
	case 43: 
		return op_sw(instruction);
	case 42: 
		return op_swl(instruction);
	case 46: 
		return op_swr(instruction);

	}
	return 0;
}
 
// int parseJForm (struct jform instruction) {
// 	switch (instruction.opcode) {
// 		case 2:
// 			return op_j(instruction);
// 		case 3: 
// 			return op_jal(instruction);
// 	}
// 	return 0;
// }

void printR(char *name, int rd, int rs, int rt) {
	printf(" %s\n", name);
 	printf("rd: %d\n", rd);
	printf("rs: %d\n", rs);
	printf("rt: %d\n\n", rt);
}

void printI(char *name,  int rs, int rt, uint16_t imm) {
	printf(" %s\n", name);
	printf("rs: %d\n", rs);
	printf("rt: %d\n", rt);
	printf("imm: %d \n\n", imm );
}

//===============add===============
int op_add (struct rform instruction) {	
	printR("add ", regfile[instruction.rd], regfile[instruction.rs], regfile[instruction.rt]);
	regfile[instruction.rd] = regfile[instruction.rs]+regfile[instruction.rt];//rd = rs + rt
	printR("add after", regfile[instruction.rd], regfile[instruction.rs], regfile[instruction.rt]);
	return regfile[instruction.rd];
}
//===============add immediate===============
int op_addi (struct iform instruction) {
	printI("addi bfo", regfile[instruction.rs], regfile[instruction.rt], instruction.constaddr);
	// printI("addi bfo", regfile[instruction.rs], regfile[instruction.rt], instruction.constaddr);	
	int imm = instruction.constaddr;
	int rs = regfile[instruction.rs];
	int sum = imm +rs;
	printf("sum %d \n", sum);
	regfile[instruction.rt] = sum;
	printI("addi after", regfile[instruction.rs], regfile[instruction.rt], instruction.constaddr);	
	return regfile[instruction.rt];
}
//===============add immediate unsigned===============
int op_addiu (struct iform instruction) {
	printI("addiu bfo", regfile[instruction.rs], regfile[instruction.rt], instruction.constaddr);	
	int imm = instruction.constaddr;
	uint32_t guy = (uint32_t)regfile[instruction.rs] + imm;
	regfile[instruction.rt] = guy;
	printI("addiu after", regfile[instruction.rs], regfile[instruction.rt], instruction.constaddr);	
	return regfile[instruction.rt];
}
//===============add unsigned===============
int op_addu (struct rform instruction) {
	printR("addu bfo ", regfile[instruction.rd], regfile[instruction.rs], regfile[instruction.rt]);
	uint32_t anotherguy = (uint32_t)regfile[instruction.rs]+(uint32_t)regfile[instruction.rt];//rd = rs + rt
	regfile[instruction.rd] = anotherguy;
	printR("addu after ", regfile[instruction.rd], regfile[instruction.rs], regfile[instruction.rt]);
	return regfile[instruction.rd];
}
//===============subtract==============
int op_sub (struct rform instruction) {
	printR("sub bfo ", regfile[instruction.rd], regfile[instruction.rs], regfile[instruction.rt]);
	regfile[instruction.rd] = regfile[instruction.rs]-regfile[instruction.rt];//rd = rs + rt
	printR("sub after ", regfile[instruction.rd], regfile[instruction.rs], regfile[instruction.rt]);
	return regfile[instruction.rd];
}
//===============subtract unsigned===============
int op_subu (struct rform instruction) {
	printR("subu bfo ", regfile[instruction.rd], regfile[instruction.rs], regfile[instruction.rt]);
	uint32_t minusguy= (uint32_t)regfile[instruction.rs]-(uint32_t)regfile[instruction.rt];//rd = rs + rt
	regfile[instruction.rd] = minusguy;
	printR("subu after ", regfile[instruction.rd], regfile[instruction.rs], regfile[instruction.rt]);
	return regfile[instruction.rd];
}
//===============divide===============
int op_div (struct rform instruction) {
	printR("div bfo ", regfile[instruction.rs], regfile[32], regfile[33]);
	regfile[32] = regfile[instruction.rs]/regfile[instruction.rt]; //lo = rs/rt
	regfile[33] = regfile[instruction.rs]%regfile[instruction.rt]; //high = rs mod rt
	printR("div after ", regfile[instruction.rt], regfile[32], regfile[33]);
	return regfile[32];
}
//===============divide unsigned===============
int op_divu (struct rform instruction) {//check
	printR("div bfo ", regfile[instruction.rs], regfile[32], regfile[33]);
	uint32_t guyIsHigh= (uint32_t)regfile[instruction.rs]/(uint32_t)regfile[instruction.rt]; //lo = rs/rt
	regfile[32] = guyIsHigh;
	uint32_t guyIsLow = (uint32_t)regfile[instruction.rs]%(uint32_t)regfile[instruction.rt]; //high = rs mod rt
	regfile[33] = guyIsLow;
	printR("div after ", regfile[instruction.rt], regfile[32], regfile[33]);
	return regfile[32];
}
//===============multiply===============
int op_mult (struct rform instruction) {
	printR("mult bfo ", regfile[instruction.rs], regfile[instruction.rt], regfile[33]);
	int prod = regfile[instruction.rs]*regfile[instruction.rt]; //prod = rs*rt
	struct prod_bit {
		unsigned int low_bit: 32;
		unsigned int high_bit: 32;
	};
	struct prod_bit *multres = (struct prod_bit*)&prod;
	regfile[32] = multres->low_bit;
	regfile[33] = multres->high_bit;//make sure this works
	printR("mult after ", regfile[instruction.rt], regfile[32], regfile[33]);
	return multres->low_bit;
}
//===============multiply unsigned===============
int op_multu (struct rform instruction){
	printR("multu bfo ", regfile[instruction.rs], regfile[32], regfile[33]);
	uint32_t prod = (uint32_t)(regfile[instruction.rs]*regfile[instruction.rt]); //get product
	struct prod_bit {
		unsigned int low_bit: 32;
		unsigned int high_bit: 32;
	};
	struct prod_bit *multres = (struct prod_bit*)&prod;//CHECK DIS
	regfile[32] = multres->low_bit; //put low in reg[33]
	regfile[33] = multres->high_bit;
	printR("multu after ", regfile[instruction.rt], regfile[32], regfile[33]);
	return multres->low_bit;
}
//===============move from high===============
int op_mfhi (struct rform instruction) {
	printR("mfhi bfo ", regfile[instruction.rd], regfile[32], regfile[33]);
	regfile[instruction.rd] = regfile[33]; //rd = high
	printR("mfhi after ", regfile[instruction.rd], regfile[32], regfile[33]);
	return regfile[instruction.rd];
}
//===============move from low===============
int op_mflo (struct rform instruction) {
	printR("mflo bfo ", regfile[instruction.rd], regfile[32], regfile[33]);
	regfile[instruction.rd] = regfile[32]; //rd = low
	printR("mflo after ",regfile[instruction.rd], regfile[32], regfile[33]);
	return regfile[instruction.rd];
}
//===============move to high===============
int op_mthi (struct rform instruction) {
	printR("mthi bfo ", regfile[instruction.rd], regfile[32], regfile[33]);
	regfile[33] = regfile[instruction.rs]; //high = rs
	printR("mthi after ", regfile[instruction.rd], regfile[32], regfile[33]);
	return regfile[33];
}
//===============move to low===============
int op_mtlo (struct rform instruction) {
	printR("mtlo bfo ", regfile[instruction.rd], regfile[32], regfile[33]);
	regfile[32] = regfile[instruction.rs]; //low = rs
	printR("mtlo bfo ", regfile[instruction.rd], regfile[32], regfile[33]);
	return regfile[32];
}
//===============and===============
int op_and (struct rform instruction) {
	printR("and bfo", regfile[instruction.rd], regfile[instruction.rs], regfile[instruction.rt]);
	regfile[instruction.rd] = regfile[instruction.rs]&regfile[instruction.rt];
	printR("and after", regfile[instruction.rd], regfile[instruction.rs], regfile[instruction.rt]);
	return regfile[instruction.rd];
}
//===============and immediate===============
int op_andi (struct iform instruction) {
	printI("andi bfo", regfile[instruction.rs], regfile[instruction.rt], instruction.constaddr);	
	int imm = instruction.constaddr&(0x0000ffff);//zero extended imm
	regfile[instruction.rt] = regfile[instruction.rs]&imm;
	printI("addi after", regfile[instruction.rs], regfile[instruction.rt], instruction.constaddr);	
	return regfile[instruction.rt];
} 
//===============xor===============
int op_xor (struct rform instruction) {
	printR("xor bfo", regfile[instruction.rd], regfile[instruction.rs], regfile[instruction.rt]);
	regfile[instruction.rd] = regfile[instruction.rs]^regfile[instruction.rt];
	printR("xor after", regfile[instruction.rd], regfile[instruction.rs], regfile[instruction.rt]);
	return regfile[instruction.rd];
}
int op_xori (struct iform instruction)  {
	printI("xori bfo", regfile[instruction.rs], regfile[instruction.rt], instruction.constaddr);	
	int imm =instruction.constaddr&(0x0000ffff);//zero extended imm
	regfile[instruction.rt] = regfile[instruction.rs]^imm;
	printI("xori after", regfile[instruction.rs], regfile[instruction.rt], instruction.constaddr);	
	return regfile[instruction.rt];
}
int op_nor (struct rform instruction) {
	printR("nor bfo ", regfile[instruction.rd], regfile[instruction.rs], regfile[instruction.rt]);
	regfile[instruction.rd] = ~(regfile[instruction.rs]|regfile[instruction.rt]);
	printR("nor after ", regfile[instruction.rd], regfile[instruction.rs], regfile[instruction.rt]);
	return regfile[instruction.rd];
}
int op_or (struct rform instruction) {
	printR("or bfo ", regfile[instruction.rd], regfile[instruction.rs], regfile[instruction.rt]);
	regfile[instruction.rd] = regfile[instruction.rs]|regfile[instruction.rt];
	printR("or after ", regfile[instruction.rd], regfile[instruction.rs], regfile[instruction.rt]);
	return regfile[instruction.rd];
}
int op_ori (struct iform instruction) {
	printI("ori bfo", regfile[instruction.rs], regfile[instruction.rt], instruction.constaddr);	
	int imm =instruction.constaddr&(0x0000ffff);//zero extended imm
	regfile[instruction.rt] = regfile[instruction.rs]|imm;
	printI("ori after", regfile[instruction.rs], regfile[instruction.rt], instruction.constaddr);	
	return regfile[instruction.rt];
}
int op_sll(struct rform instruction) {
	printR("sll bfo ", regfile[instruction.rd], regfile[instruction.rs], regfile[instruction.rt]);
	uint32_t rtU = regfile[instruction.rt];//convert to unsigned for 0-padding
	rtU = rtU<<instruction.shamt;//shift left by indicated shift amount
	regfile[instruction.rd] = rtU;
	printR("sll after ", regfile[instruction.rd], regfile[instruction.rs], regfile[instruction.rt]);
	printf("%d \n", instruction.shamt);
	return regfile[instruction.rd];
}
int op_sllv (struct rform instruction) {
	printR("sllv bfo ", regfile[instruction.rd], regfile[instruction.rs], regfile[instruction.rt]);
	uint32_t rtU = regfile[instruction.rt];//convert to unsigned for 0-padding
	uint32_t shift = regfile[instruction.rs];
	shift = shift&0x1f;//get first 5 bits
	rtU = rtU<<shift;
	regfile[instruction.rd] = rtU;
	printR("sllv after ", regfile[instruction.rd], regfile[instruction.rs], regfile[instruction.rt]);
	return regfile[instruction.rd];
}
int op_slt (struct rform instruction) {
	printR("slt bfo ", regfile[instruction.rd], regfile[instruction.rs], regfile[instruction.rt]);
	if (regfile[instruction.rs]<regfile[instruction.rt]) {//rs<rt?
		regfile[instruction.rd] = 1;
	} else {
		regfile[instruction.rd] = 0;
	}
	printR("slt after ", regfile[instruction.rd], regfile[instruction.rs], regfile[instruction.rt]);
	return regfile[instruction.rd];
}
int op_slti (struct iform instruction) {
	printI("slti bfo", regfile[instruction.rs], regfile[instruction.rt], instruction.constaddr);	
	int imm = instruction.constaddr;
	if (regfile[instruction.rs]<imm) {//rs<im
		regfile[instruction.rt] = 1;
	} else {
		regfile[instruction.rt] = 0;
	}
	printI("slti after", regfile[instruction.rs], regfile[instruction.rt], instruction.constaddr);	
	return regfile[instruction.rt];
}
//CHECK
int op_sltiu (struct iform instruction) {
	printI("sltiu bfo", regfile[instruction.rs], regfile[instruction.rt], instruction.constaddr);	
	int imm = instruction.constaddr;
	if ((uint32_t)regfile[instruction.rs]<(uint32_t)imm) {//rs<im
		regfile[instruction.rt] = 1;
	} else {
		regfile[instruction.rt] = 0;
	}
	printI("sltiu after", regfile[instruction.rs], regfile[instruction.rt], instruction.constaddr);	
	return regfile[instruction.rt];
}
int op_sltu (struct rform instruction) {
	printR("sltu bfo ", regfile[instruction.rd], regfile[instruction.rs], regfile[instruction.rt]);
	if ((uint32_t)regfile[instruction.rs]<(uint32_t)regfile[instruction.rt]) {//rs<rt?
		regfile[instruction.rd] = 1;
	} else {
		regfile[instruction.rd] = 0;
	}
	printR("sltu after ", regfile[instruction.rd], regfile[instruction.rs], regfile[instruction.rt]);
	return regfile[instruction.rd];
}
int op_sra (struct rform instruction) {
	printR("sra bfo ", regfile[instruction.rd], regfile[instruction.rs], regfile[instruction.rt]);
	regfile[instruction.rd]= regfile[instruction.rt]>>instruction.shamt;
	printR("sra after ", regfile[instruction.rd], regfile[instruction.rs], regfile[instruction.rt]);
	printf("%d \n", instruction.shamt);
	return regfile[instruction.rd];
}
int op_srav (struct rform instruction) {
	printR("srav bfo ", regfile[instruction.rd], regfile[instruction.rs], regfile[instruction.rt]);
	uint32_t shift = regfile[instruction.rs];
	shift = shift&0x1f;//get first 5 bits
	regfile[instruction.rd] = regfile[instruction.rt]>>shift;
	printR("srav after ", regfile[instruction.rd], regfile[instruction.rs], regfile[instruction.rt]);
	return regfile[instruction.rd];
}
int op_srl (struct rform instruction) {
	printR("srl bfo ", regfile[instruction.rd], regfile[instruction.rs], regfile[instruction.rt]);
	uint32_t rtU = regfile[instruction.rt];//convert to unsigned for 0-padding
	rtU = rtU>>instruction.shamt;//shift left by indicated shift amount
	regfile[instruction.rd] = rtU;
	printR("srl after ", regfile[instruction.rd], regfile[instruction.rs], regfile[instruction.rt]);
	printf("%d \n", instruction.shamt);
	return regfile[instruction.rd];
}
int op_srlv (struct rform instruction) {
	printR("srlv bfo ", regfile[instruction.rd], regfile[instruction.rs], regfile[instruction.rt]);
	uint32_t rtU = regfile[instruction.rt];//convert to unsigned for 0-padding
	uint32_t shift = regfile[instruction.rs];
	shift = shift&0x1f;//get first 5 bits
	rtU = rtU>>shift;
	regfile[instruction.rd] = rtU;
	printR("srlv after ", regfile[instruction.rd], regfile[instruction.rs], regfile[instruction.rt]);
	return regfile[instruction.rd];}
//===============branch on equal===============
//execute branch delay slot. if rs == rt, jump to target address
int op_beq (struct iform instruction) {
	printI("beq bfo", regfile[instruction.rs], regfile[instruction.rt], instruction.constaddr);	
	printf("PC %d \n", PC);
	uint32_t nextInstruction = readWord(PC+4, false);
	parseInstruction(nextInstruction);//execute next instruction in branch delay slot 
	int offset = (int16_t)instruction.constaddr;
	offset = offset<<2;
	offset = offset&0xfffffffc;
	if (regfile[instruction.rs] == regfile [instruction.rt]) {
		PC = PC+offset;
	} else {	//parseInstruction()
	PC = PC+4; //as we already implemented PC + 4 move on to following one
	}
	printf("PC after %d \n\n", PC);
	return PC;
}
//===============branch on equal likely===============
//if rs == rt, execute branch delay slot, and branch. else, skip branch delay slot
int op_beql (struct iform instruction) {
	printI("beql bfo", regfile[instruction.rs], regfile[instruction.rt], instruction.constaddr);	
	printf("PC bfo %d \n", PC);
	if (regfile[instruction.rs] == regfile [instruction.rt]) {
		uint32_t nextInstruction = readWord(PC+4, false);
		parseInstruction(nextInstruction);//execute next instruction in branch delay slot 
		int offset = (int16_t)instruction.constaddr;
		offset = offset<<2;
		offset = offset&0xfffffffc;//lower 2 bits = 0
		PC = PC+offset;
	} else {
		PC = PC+4; //skip branch delay
	}
	printf("PC after %d \n\n", PC);
	return PC;
}
//===============branch on >= 0===============
//execute branch delay slot. if rs >= 0, branch. else, move to instruction after branch delay slot
int op_bgez (struct iform instruction) {
	printI("bgez bfo", regfile[instruction.rs], regfile[instruction.rt], instruction.constaddr);	
	printf("PC %d \n", PC);
	uint32_t nextInstruction = readWord(PC+4, false);
	parseInstruction(nextInstruction);//execute next instruction in branch delay slot 
	int offset = (int16_t)instruction.constaddr;
	offset = offset<<2;
	offset = offset&0xfffffffc;//2 low bits = 0
	if (regfile[instruction.rs]>=0) {
		PC = PC+offset;
	} else {	//parseInstruction()
		PC = PC+4; //as already implemented PC + 4 move on to following one
		}
	printf("PC %d \n\n", PC);
	return PC;
}
//===============branch on > 0===============
//execute branch delay slot. if rs > 0, branch. else, move to instruction after branch delay slot
int op_bgtz (struct iform instruction) {
	printI("bgtz bfo", regfile[instruction.rs], regfile[instruction.rt], instruction.constaddr);	
	printf("PC %d \n", PC);
	uint32_t nextInstruction = readWord(PC+4, false);
	parseInstruction(nextInstruction);//execute next instruction in branch delay slot 
	int offset = (int16_t)instruction.constaddr;
	offset = offset<<2;
	offset = offset&0xfffffffc;
	if (regfile[instruction.rs]>0) {
		PC = PC+offset;
	} else {	//parseInstruction()
		PC = PC+4; //as already implemented PC + 4 move on to following one
		}
	printf("PC %d \n\n", PC);
	return PC;
	}
//===============branch on <= 0===============
//execute branch delay slot. if rs <= 0, branch. else, move to instruction after branch delay slot
int op_blez (struct iform instruction) {
	printI("blez bfo", regfile[instruction.rs], regfile[instruction.rt], instruction.constaddr);	
	printf("PC %d \n", PC);
	uint32_t nextInstruction = readWord(PC+4, false);
	parseInstruction(nextInstruction);//execute next instruction in branch delay slot 
	int offset = (int16_t)instruction.constaddr;
	offset = offset<<2;
	offset = offset&0xfffffffc;
	if (regfile[instruction.rs]<=0) {
		PC = PC+offset;
	} else {	//parseInstruction()
	PC = PC+4; //as already implemented PC + 4 move on to following one
		}
	printf("PC %d \n\n", PC);
	return PC;
	}
//===============branch on <= 0 likely===============
//if rs > 0, execute branch delay slot and branch. else, move to instruction after branch delay slot
int op_blezl (struct iform instruction) {
	printI("blezl bfo", regfile[instruction.rs], regfile[instruction.rt], instruction.constaddr);	
	printf("PC %d \n", PC);
	if (regfile[instruction.rs]<=0) {
		uint32_t nextInstruction = readWord(PC+4, false);
		int offset = (int16_t)instruction.constaddr;
		offset = offset<<2;
		offset = offset&0xfffffffc;
		parseInstruction(nextInstruction);//execute next instruction in branch delay slot 
		PC = PC+offset;
	} else {
		PC = PC+4; //skip next instr
	}
	printf("PC %d \n\n", PC);
	return PC;
}
//===============branch on < 0===============
//execute branch delay slot. if rs < 0, branch. else, move to instruction after branch delay slot
int op_bltz (struct iform instruction) {
	printI("bltz bfo", regfile[instruction.rs], regfile[instruction.rt], instruction.constaddr);	
	printf("PC %d \n", PC);
	uint32_t nextInstruction = readWord(PC+4, false);
	parseInstruction(nextInstruction);//execute next instruction in branch delay slot 
	int offset = (int16_t)instruction.constaddr;
	offset = offset<<2;
	offset = offset&0xfffffffc;
	if (regfile[instruction.rs]<0) {
		PC = PC+offset;
	} else {	//parseInstruction()
	PC = PC+4; //as already implemented PC + 4 move on to following one
		}
	printf("PC %d \n\n", PC);
	return PC;
}
int op_bltzal (struct iform instruction) {
	printI("bltzal bfo", regfile[instruction.rs], regfile[instruction.rt], instruction.constaddr);	
	printf("PC %d \n", PC);
	regfile[31] = PC+4;
	uint32_t nextInstruction = readWord(PC+4, false);
	parseInstruction(nextInstruction);//execute next instruction in branch delay slot 
	if (regfile[instruction.rs]<0) {
		int offset = (int16_t)instruction.constaddr;
		offset = offset<<2;
		offset = offset&0xfffffffc;
		PC = PC+offset;
		uint32_t branchInstruction = readWord(PC+offset,false);//to execute jump
		parseInstruction(branchInstruction);
	}
	PC = regfile[31]+1;
	printf("PC %d \n\n", PC);
	return 0;
}
//===============branch on not equal===============
//execute branch delay slot. if rs != rt, branch. else, move to instruction after b d slot
int op_bne (struct iform instruction) {
	printI("bne bfo", regfile[instruction.rs], regfile[instruction.rt], instruction.constaddr);	
	printf("PC %d \n", PC);
	uint32_t nextInstruction = readWord(PC+4, false);
	parseInstruction(nextInstruction);//execute next instruction in branch delay slot 
	int offset = (int16_t)instruction.constaddr;
	offset = offset<<2;
	offset = offset&0xfffffffc;//2 0s in lower bits
	printf("shift offset %d", offset);
	if (regfile[instruction.rs]!=regfile[instruction.rt]) {
		PC = PC+offset;
	} else {	//parseInstruction()
	PC = PC+4; //as already implemented PC + 4 move on to following one
		}
	printf("PC %d \n\n", PC);
	return PC;
}
//===============branch on equal likely===============
//if rs == rt, execute branch delay slot, and branch. else, skip branch delay slot.
int op_bnel (struct iform instruction) {
	printI("bnel bfo", regfile[instruction.rs], regfile[instruction.rt], instruction.constaddr);	
	printf("PC %d \n", PC);
	if (regfile[instruction.rs]!=regfile[instruction.rt]) {
		uint32_t nextInstruction = readWord(PC+4, false);
		parseInstruction(nextInstruction);//execute next instruction in branch delay slot 
		int offset = (int16_t)instruction.constaddr;
		offset = offset<<2;
		offset = offset&0xfffffffc;
		PC = PC+offset;
	} else {
		PC = PC+4;//skip instruction in exec slot
	}
	printf("PC %d \n\n", PC);
	return PC;
}
//===============jump===============
//unconditional jump to target
//CHECK
int op_j (struct jform instruction) {
	printf("jump %d \n", PC);
	int instr_index = instruction.data<<2;
	uint32_t pcbits = (PC+4)&0xf; //get 4 first bits 
	pcbits = pcbits<<28; //put pcbits in location 28-32
	instr_index = instr_index | pcbits; //concat instr index with pc bits
	int savePC = PC;
	uint32_t nextInstruction = readWord(PC+4, false);
	parseInstruction(nextInstruction);
	PC = savePC+instr_index-4;
	printf("PC %d \n\n", PC);
	return 0;
}
int op_jal (struct jform instruction) {
	printf("jal %d \n", PC);
	int instr_index = instruction.data<<2;
	uint32_t pcbits = (PC+4)&0xf; //get 4 first bits 
	pcbits = pcbits<<28; //put pcbits in location 28-32
	instr_index = instr_index | pcbits; //concat instr index with pc bits
	int savePC = PC;
	uint32_t nextInstruction = readWord(PC+4, false);
	parseInstruction(nextInstruction);
	uint32_t jumpInstruction = readWord(PC+instr_index,false);//to execute jump
	parseInstruction(jumpInstruction);
	PC = savePC+4;//to skip branch delay slot
	printf("PC %d \n\n", PC);
	return 0;
}
int op_jalr (struct rform instruction) {
	printf("jalr %d \n", PC);
	regfile[instruction.rd] = PC+4;
	uint32_t nextInstruction = readWord(PC+4, false);
	parseInstruction(nextInstruction);
	uint32_t jumpInstruction = readWord(regfile[instruction.rs],false);//to execute jump
	parseInstruction(jumpInstruction);
	PC = regfile[instruction.rd]-4; //PC = next instruction
	printf("PC %d \n\n", PC);
	return 0;
}
int op_jr (struct rform instruction) {
	printf("jr %d \n", PC);
	uint32_t nextInstruction = readWord(PC+4, false);
	parseInstruction(nextInstruction);
	PC = regfile[instruction.rs]-4; //check dis
	printf("PC %d \n\n", PC);
	return 0;
}
//CHECK
int op_lb (struct iform instruction) {
	printI("lb bfo",regfile[instruction.rt], regfile[instruction.rs], instruction.constaddr);
	int offset = instruction.constaddr;
	uint32_t mem = offset+regfile[instruction.rs];
	int byte = readByte(mem,false);//sign extend the byte
	regfile[instruction.rt] = byte;
	printI("lb after",regfile[instruction.rt], regfile[instruction.rs], instruction.constaddr);
	return regfile[instruction.rt];
}
int op_lbu (struct iform instruction) {
	printI("lbu bfo",regfile[instruction.rt], regfile[instruction.rs], instruction.constaddr);
	int offset = instruction.constaddr;
	uint32_t mem = offset+regfile[instruction.rs];
	uint32_t byte = readByte(mem,false);//sign extend the byte
	regfile[instruction.rt] = byte;
	printI("lbu after",regfile[instruction.rt], regfile[instruction.rs], instruction.constaddr);
	return regfile[instruction.rt];
}
int op_lh (struct iform instruction) {
	printI("lh bfo",regfile[instruction.rt], regfile[instruction.rs], instruction.constaddr);
	int offset = instruction.constaddr;
	uint32_t mem = offset+regfile[instruction.rs];
	regfile[instruction.rt] = readHalfWord(mem, false);//sign extend
	printI("lh after",regfile[instruction.rt], regfile[instruction.rs], instruction.constaddr);
	return regfile[instruction.rt];
}
int op_lhu (struct iform instruction) {
	printI("lhu bfo",regfile[instruction.rt], regfile[instruction.rs], instruction.constaddr);
	int offset = instruction.constaddr;
	uint32_t mem = offset+regfile[instruction.rs];
	regfile[instruction.rt] = (uint16_t)readHalfWord(mem, false);//check
	printI("lhu after",regfile[instruction.rt], regfile[instruction.rs], instruction.constaddr);
	return regfile[instruction.rt];
}
int op_lui (struct iform instruction) {
	printI("lui bfo",regfile[instruction.rt], regfile[instruction.rs], instruction.constaddr);
	uint16_t imm = instruction.constaddr;
	imm = imm<<16;//load immediate
	regfile[instruction.rt] = imm;
	printI("lui after",regfile[instruction.rt], regfile[instruction.rs], instruction.constaddr);
	return 0;
}
int op_lw (struct iform instruction) {
	printI("lw bfo",regfile[instruction.rt], regfile[instruction.rs], instruction.constaddr);
	int offset = instruction.constaddr;
	uint32_t mem = offset+regfile[instruction.rs];
	regfile[instruction.rt] = readWord(mem,false);
	printI("lw after",regfile[instruction.rt], regfile[instruction.rs], instruction.constaddr);
	return regfile[instruction.rt];
}
int op_lwl (struct iform instruction) {
	printI("lwl bfo",regfile[instruction.rt], regfile[instruction.rs], instruction.constaddr);
	int offset = instruction.constaddr;
	uint32_t mem = offset+regfile[instruction.rs];
	int lw = readWord(mem,false)&0xffff0000;//erase 16 lower bits
	regfile [instruction.rt] = regfile [instruction.rt]&0x0000ffff; //clean upper 16 bits
	regfile [instruction.rt] = regfile [instruction.rt]|lw;
	printI("lwl after",regfile[instruction.rt], regfile[instruction.rs], instruction.constaddr);
	return regfile[instruction.rt];
}
int op_lwr (struct iform instruction) {
	printI("lwr bfo",regfile[instruction.rt], regfile[instruction.rs], instruction.constaddr);
	int offset = instruction.constaddr;
	uint32_t mem = offset+regfile[instruction.rs];
	int lw = readWord(mem,false)&0x0000ffff;//clean upper 16bits
	regfile [instruction.rt] = regfile [instruction.rt]&0xffff0000;//clean lower 16 bits
	regfile [instruction.rt] = regfile [instruction.rt]|lw;
	printI("lwr after",regfile[instruction.rt], regfile[instruction.rs], instruction.constaddr);
	return regfile[instruction.rt];
}
int op_sb (struct iform instruction) {
	printI("sb bfo",regfile[instruction.rt], regfile[instruction.rs], instruction.constaddr);
	uint8_t byte = regfile[instruction.rt]&0xff;//get first 8 bits
	int offset = instruction.constaddr;
	uint32_t mem = offset+regfile[instruction.rs];
	writeByte(mem,byte,false);
	printf("byte %d \n", byte);
	printI("lh after",regfile[instruction.rt], mem, instruction.constaddr);
	return 0;
}
int op_sh (struct iform instruction) {
	printI("sh bfo",regfile[instruction.rt], regfile[instruction.rs], instruction.constaddr);
	uint16_t halfword = regfile[instruction.rt]&0xffff;//get first 16 bits
	int offset = instruction.constaddr;
	uint32_t mem = offset+regfile[instruction.rs];
	writeHalfWord(mem,halfword,false);
	printI("sh after",regfile[instruction.rt], mem, instruction.constaddr);
	return 0;
}
int op_sw (struct iform instruction) {
	printI("sw bfo",regfile[instruction.rt], regfile[instruction.rs], instruction.constaddr);
	uint32_t word = regfile[instruction.rt];//get first 16 bits
	int offset = instruction.constaddr;
	uint32_t mem = offset+regfile[instruction.rs];
	printI("sw after",regfile[instruction.rt], mem, instruction.constaddr);
	writeWord(mem,word,false);
	return 0;
}
int op_swl (struct iform instruction) {
	printI("swl bfo",regfile[instruction.rt], regfile[instruction.rs], instruction.constaddr);
	int offset = instruction.constaddr;
	uint32_t mem = offset+regfile[instruction.rs];
	uint16_t wl = regfile[instruction.rt]&0xffff0000; //get upper 16 bits
	writeHalfWord(mem, wl, false);
	printI("swl after",regfile[instruction.rt], mem, instruction.constaddr);
	return 0;
}
int op_swr (struct iform instruction) {
	printI("swr bfo",regfile[instruction.rt], regfile[instruction.rs], instruction.constaddr);
	int offset = instruction.constaddr;
	uint32_t mem = offset+regfile[instruction.rs];
	uint16_t wr = regfile[instruction.rt]&0xffff; //get lower 16 bits
	writeHalfWord(mem, wr, false);
	printI("swr after",regfile[instruction.rt], mem, instruction.constaddr);
	return 0;
}
int op_syscall (struct syscallform instruction) {
	printf("syscall: code %d opcode %d funct %d\n\n", instruction.code, instruction.opcode, instruction.funct);
	SyscallExe(instruction.code);
	return 0;
}
int op_nop (struct rform instruction) {
	printf("%s \n\n", "nop");
	//sll r0 r0 0
	return 0;
}

//print a register as binary
char* printReg (int x, int size) {
	char *string;

    for (int i = 0; i < size; i++) {
        int maskcount = 1<<i;
        int masked_x = x&maskcount;
        int bit = masked_x>>i;
        string+=bit;
        printf("%d", bit);
    }
    printf("%s \n\n","");
    return string;
}

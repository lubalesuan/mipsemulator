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
	 	case 0x03:
	 		return parseJForm(*instrJ);
	 	default: {
	 		struct iform *instrI = (struct iform*)&instruction;
	 		return parseIForm(*instrI);
	 	}
	 }
	 //return 0;
}


int parseRForm (struct rform instruction) {
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
		case 7:
			return op_srav (instruction);
		case 10:
			return op_srl (instruction);
		case 6:
			return op_srlv (instruction);
		case 9:
			return op_jalr (instruction);
		case 8:
			return op_jr (instruction);
		case 12:
			return op_syscall (instruction);
		case 4:
			return op_sllv (instruction);
		default: //check if sll  = nop case 0
			return op_nop (instruction);
	}
}

int parseIForm (struct iform instruction) {
switch (instruction.opcode) {
	case 0x08: 
		return op_addi(instruction);
	case 0x09: 
		return op_addiu(instruction);
	case 0x0C: 
		return op_andi(instruction);
	case 14: 
		return op_xori(instruction);
	case 0x0D: 
		return op_ori(instruction);
	case 0x0A: 
		return op_slti(instruction);
	case 0x0B: 
		return op_sltiu(instruction);
	case 0x04: 
		return op_beq(instruction);
	case 20: 
		return op_beql(instruction);
	case 1: {
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
 
int parseJForm (struct jform instruction) {
	switch (instruction.opcode) {
		case 2:
			return op_j(instruction);
		case 3: 
			return op_jal(instruction);
	}
	return 0;
}

int op_add (struct rform instruction) {
	regfile[instruction.rd] = regfile[instruction.rs]+regfile[instruction.rt];//rd = rs + rt
	return regfile[instruction.rd];
}
int op_addi (struct iform instruction) {
	int imm = regfile[instruction.constaddr]<<16;//low = imm, high = sign extension
	regfile[instruction.rt] = regfile[instruction.rs] + imm;
	return regfile[instruction.rt];
}
int op_addiu (struct iform instruction) {
	uint32_t imm = regfile[instruction.constaddr]<<16;//low = imm, high = sign extension
	regfile[instruction.rt] = (uint32_t)regfile[instruction.rs] + imm;
	return regfile[instruction.rt];
}
int op_addu (struct rform instruction) {
	regfile[instruction.rd] = (uint32_t)regfile[instruction.rs]+(uint32_t)regfile[instruction.rt];//rd = rs + rt
	return regfile[instruction.rd];
}
int op_sub (struct rform instruction) {
	regfile[instruction.rd] = regfile[instruction.rs]-regfile[instruction.rt];//rd = rs + rt
	return regfile[instruction.rd];
}
int op_subu (struct rform instruction) {
	regfile[instruction.rd] = (uint32_t)regfile[instruction.rs]-(uint32_t)regfile[instruction.rt];//rd = rs + rt
	return regfile[instruction.rd];
}
int op_div (struct rform instruction) {
	//divide by 0
	regfile[32] = regfile[instruction.rs]/regfile[instruction.rt]; //lo = rs/rt
	regfile[33] = regfile[instruction.rs]%regfile[instruction.rt]; //high = rs mod rt
	return regfile[32];
}
int op_divu (struct rform instruction) {//check
	regfile[32] = (uint32_t)regfile[instruction.rs]/(uint32_t)regfile[instruction.rt]; //lo = rs/rt
	regfile[33] = (uint32_t)regfile[instruction.rs]%(uint32_t)regfile[instruction.rt]; //high = rs mod rt
	return regfile[32];
}
//CHECK CHECK CHECK
int op_mult (struct rform instruction) {
	double prod = regfile[instruction.rs]*regfile[instruction.rt]; //prod = rs*rt
	struct prod_bit {
		unsigned int low_bit: 32;
		unsigned int high_bit: 32;
	};
	struct prod_bit *multres = (struct prod_bit*)&prod;
	regfile[32] = multres->low_bit;
	regfile[33] = multres->high_bit;//make sure this works
	return multres->low_bit;
}
//CHECK CHECK CHECK
int op_multu (struct rform instruction){
	uint32_t prod = (uint32_t)(regfile[instruction.rs]*regfile[instruction.rt]); //get product
	struct prod_bit {
		unsigned int low_bit: 32;
		unsigned int high_bit: 32;
	};
	struct prod_bit *multres = (struct prod_bit*)&prod;//CHECK DIS
	regfile[32] = multres->low_bit; //put low in reg[33]
	regfile[33] = multres->high_bit;
	return multres->low_bit;
}
int op_mfhi (struct rform instruction) {
	regfile[instruction.rd] = regfile[33]; //rd = high
	return regfile[instruction.rd];
}
int op_mflo (struct rform instruction) {
	regfile[instruction.rd] = regfile[32]; //rd = low
	return regfile[instruction.rd];
}
int op_mthi (struct rform instruction) {
	regfile[33] = regfile[instruction.rs]; //high = rs
	return regfile[33];
}
int op_mtlo (struct rform instruction) {
	regfile[32] = regfile[instruction.rs]; //low = rs
	return regfile[32];
}
int op_and (struct rform instruction) {
	regfile[instruction.rd] = regfile[instruction.rs]&regfile[instruction.rt];
	return regfile[instruction.rd];
}
int op_andi (struct iform instruction) {
	int imm = (regfile[instruction.constaddr]<<16)&(0x0000ffff);//zero extended imm
	regfile[instruction.rt] = regfile[instruction.rs]&imm;
	return regfile[instruction.rt];
} 
int op_xor (struct rform instruction) {
	regfile[instruction.rd] = regfile[instruction.rs]^regfile[instruction.rt];
	return regfile[instruction.rd];
}
int op_xori (struct iform instruction)  {
	int imm = (regfile[instruction.constaddr]<<16)&(0x0000ffff);//zero extended imm
	regfile[instruction.rt] = regfile[instruction.rs]^imm;
	return regfile[instruction.rt];
}
int op_nor (struct rform instruction) {
	regfile[instruction.rd] = ~(regfile[instruction.rs]|regfile[instruction.rt]);
	return regfile[instruction.rd];
}
int op_or (struct rform instruction) {
	regfile[instruction.rd] = regfile[instruction.rs]|regfile[instruction.rt];
	return regfile[instruction.rd];
}
int op_ori (struct iform instruction) {
	int imm = (regfile[instruction.constaddr]<<16)&(0x0000ffff);//zero extended imm
	regfile[instruction.rt] = regfile[instruction.rs]|imm;
	return regfile[instruction.rt];
}
int op_sll(struct rform instruction) {
	uint32_t rtU = regfile[instruction.rt];//convert to unsigned for 0-padding
	rtU = rtU<<regfile[instruction.shamt];//shift left by indicated shift amount
	return (regfile[instruction.rd] = rtU);
}
int op_sllv (struct rform instruction) {
	uint32_t rtU = regfile[instruction.rt];//convert to unsigned for 0-padding
	uint32_t shift = regfile[instruction.rs];
	shift = shift&0x1f;//get first 5 bits
	rtU = rtU<<shift;
	return (regfile[instruction.rd] = rtU);
}
int op_slt (struct rform instruction) {
	if (regfile[instruction.rs]<regfile[instruction.rt]) {//rs<rt?
		regfile[instruction.rd] = 1;
	} else {
		regfile[instruction.rd] = 0;
	}
	return regfile[instruction.rd];
}
int op_slti (struct iform instruction) {
	int imm = regfile[instruction.constaddr]<<16;
	if (regfile[instruction.rs]<imm) {//rs<im
		regfile[instruction.rt] = 1;
	} else {
		regfile[instruction.rt] = 0;
	}
	return regfile[instruction.rt];
}
//CHECK
int op_sltiu (struct iform instruction) {
	uint32_t imm = regfile[instruction.constaddr]<<16;
	if ((uint32_t)regfile[instruction.rs]<imm) {//rs<im
		regfile[instruction.rt] = 1;
	} else {
		regfile[instruction.rt] = 0;
	}
	return regfile[instruction.rt];
}
int op_sltu (struct rform instruction) {
	if ((uint32_t)regfile[instruction.rs]<(uint32_t)regfile[instruction.rt]) {//rs<rt?
		regfile[instruction.rd] = 1;
	} else {
		regfile[instruction.rd] = 0;
	}
	return regfile[instruction.rd];
}
int op_sra (struct rform instruction) {
	regfile[instruction.rd]= regfile[instruction.rt]>>regfile[instruction.shamt];
	return regfile[instruction.rd];
}
int op_srav (struct rform instruction) {
	uint32_t shift = regfile[instruction.rs];
	shift = shift&0x1f;//get first 5 bits
	regfile[instruction.rd] = regfile[instruction.rt]>>shift;
	return regfile[instruction.rd];
}
int op_srl (struct rform instruction) {
	uint32_t rtU = regfile[instruction.rt];//convert to unsigned for 0-padding
	rtU = rtU>>regfile[instruction.shamt];//shift left by indicated shift amount
	return (regfile[instruction.rd] = rtU);
}
int op_srlv (struct rform instruction) {
	uint32_t rtU = regfile[instruction.rt];//convert to unsigned for 0-padding
	uint32_t shift = regfile[instruction.rs];
	shift = shift&0x1f;//get first 5 bits
	rtU = rtU>>shift;
	return (regfile[instruction.rd] = rtU);
}
//===============branch on equal===============
//execute branch delay slot. if rs == rt, jump to target address
int op_beq (struct iform instruction) {
	uint32_t nextInstruction = readWord(PC++, false);
	parseInstruction(nextInstruction);//execute next instruction in branch delay slot 
	int offset = regfile[instruction.constaddr]<<2;
	offset = offset&0xfffffffc;
	if (regfile[instruction.rs] == regfile [instruction.rt]) {
		PC = PC+1+offset;
	} else {	//parseInstruction()
	PC = PC+1; //as we already implemented PC + 4 move on to following one
	}
	return PC;
}
//===============branch on equal likely===============
//if rs == rt, execute branch delay slot, and branch. else, skip branch delay slot
int op_beql (struct iform instruction) {
	if (regfile[instruction.rs] == regfile [instruction.rt]) {
		uint32_t nextInstruction = readWord(PC++, false);
		parseInstruction(nextInstruction);//execute next instruction in branch delay slot 
		int offset = regfile[instruction.constaddr]<<2;
		offset = offset&0xfffffffc;
		PC = PC+1+offset;
	} else {
		PC = PC+1; //skip branch delay
	}
	return PC;
}
//===============branch on >= 0===============
//execute branch delay slot. if rs >= 0, branch. else, move to instruction after branch delay slot
int op_bgez (struct iform instruction) {
	uint32_t nextInstruction = readWord(PC+1, false);
	parseInstruction(nextInstruction);//execute next instruction in branch delay slot 
	int offset = regfile[instruction.constaddr]<<2;
	offset = offset&0xfffffffc;//2 low bits = 0
	if (regfile[instruction.rs]>=0) {
		PC = PC+1+offset;
	} else {	//parseInstruction()
	PC = PC+1; //as already implemented PC + 4 move on to following one
		}
	return PC;
}
int op_bgtz (struct iform instruction) {
	uint32_t nextInstruction = readWord(PC+4, false);
	parseInstruction(nextInstruction);//execute next instruction in branch delay slot 
	int offset = regfile[instruction.constaddr]<<2;
	offset = offset&0xfffffffc;
	if (regfile[instruction.rs]>0) {
		PC = PC+4+offset;
	} else {	//parseInstruction()
	PC = PC+8; //as already implemented PC + 4 move on to following one
		}
	return PC;
	}
int op_blez (struct iform instruction) {
	uint32_t nextInstruction = readWord(PC+4, false);
	parseInstruction(nextInstruction);//execute next instruction in branch delay slot 
	int offset = regfile[instruction.constaddr]<<2;
	offset = offset&0xfffffffc;
	if (regfile[instruction.rs]<=0) {
		PC = PC+4+offset;
	} else {	//parseInstruction()
	PC = PC+8; //as already implemented PC + 4 move on to following one
		}
	return PC;
	}
int op_blezl (struct iform instruction) {
	if (regfile[instruction.rs]<=0) {
		uint32_t nextInstruction = readWord(PC+4, false);
		int offset = regfile[instruction.constaddr]<<2;
		offset = offset&0xfffffffc;
		parseInstruction(nextInstruction);//execute next instruction in branch delay slot 
		PC = PC+4+offset;
	} 
	return PC;
}
int op_bltz (struct iform instruction) {
	uint32_t nextInstruction = readWord(PC+4, false);
	parseInstruction(nextInstruction);//execute next instruction in branch delay slot 
	int offset = regfile[instruction.constaddr]<<2;
	offset = offset&0xfffffffc;
	if (regfile[instruction.rs]<0) {
		PC = PC+4+offset;
	} else {	//parseInstruction()
	PC = PC+8; //as already implemented PC + 4 move on to following one
		}
	return PC;
}
int op_bltzal (struct iform instruction) {
	return 0;
}
//===============branch on not equal===============
int op_bne (struct iform instruction) {
	uint32_t nextInstruction = readWord(PC+4, false);
	parseInstruction(nextInstruction);//execute next instruction in branch delay slot 
	int offset = regfile[instruction.constaddr]<<2;
	offset = offset&0xfffffffc;
	if (regfile[instruction.rs]!=regfile[instruction.rt]) {
		PC = PC+4+offset;
	} else {	//parseInstruction()
	PC = PC+8; //as already implemented PC + 4 move on to following one
		}
	return PC;}
//===============branch on not likely===============
//if rs != rt, execute branch delay slot, and branch. else, skip branch delay slot.
int op_bnel (struct iform instruction) {
	if (regfile[instruction.rs]!=regfile[instruction.rt]) {
		uint32_t nextInstruction = readWord(PC+4, false);
		int offset = regfile[instruction.constaddr]<<2;
		offset = offset&0xfffffffc;
		parseInstruction(nextInstruction);//execute next instruction in branch delay slot 
		PC = PC+4+offset;
	} else {
		PC = PC+8;//skip instruction in exec slot
	}
	return PC;
}
int op_j (struct jform instruction) {
	return 0;
}
int op_jal (struct jform instruction) {
	return 0;
}
int op_jalr (struct rform instruction) {
	return 0;
}
int op_jr (struct rform instruction) {
	return 0;
}
//CHECK
int op_lb (struct iform instruction) {
	//int mem = readByte()
	int offset = regfile[instruction.constaddr]<<16;//will dis be 0?
	uint32_t mem = offset + regfile[instruction.rs]; //base + offset
	int byte = readByte(mem,false)<<24;//sign extend the byte
	regfile[instruction.rt] = byte;
	return regfile[instruction.rt];
}
int op_lbu (struct iform instruction) {
	int offset = regfile[instruction.constaddr]<<16;
	uint32_t mem = offset + regfile[instruction.rs]; //base + offset
	uint32_t byte = readByte(mem,false);//sign extend the byte
	byte = byte<<24;
	regfile[instruction.rt] = byte;
	return regfile[instruction.rt];
}
int op_lh (struct iform instruction) {
	int offset = regfile[instruction.constaddr]<<16;
	uint32_t mem = offset + regfile[instruction.rs]; //base + offset
	if (mem<<31 != 0) {
		printf("woops Adress Error exception");
		return 0;
	} else {
	int halfword = readWord(mem,false);//sign extend the byte
	//STOPPED HERE
	return regfile[instruction.rt];

	}
}
int op_lhu (struct iform instruction) {
	return 0;
}
//CHECK
int op_lui (struct iform instruction) {
	uint32_t imm = regfile[instruction.constaddr];
	imm = imm<<16;//load immediate
	regfile[instruction.rt] = imm;
	return 0;
}
//CHECK
int op_lw (struct iform instruction) {
	int offset = regfile[instruction.constaddr]<<16;
	uint32_t mem = offset + regfile[instruction.rs]; //base + offset
	regfile[instruction.rt] = readWord(mem,false);
	return regfile[instruction.rt];
}
//CHECK
int op_lwl (struct iform instruction) {
	int offset = regfile[instruction.constaddr]<<16;
	uint32_t mem = offset + regfile[instruction.rs]; //base + offset
	int msb = readWord(mem,false);
	msb = msb>>16;//put this in most significant 2 bytes
	regfile [instruction.rt] = msb;
	return regfile[instruction.rt];
}
int op_lwr (struct iform instruction) {
	int offset = regfile[instruction.constaddr]<<16;
	uint32_t mem = offset + regfile[instruction.rs]; //base + offset
	int msb = readWord(mem,false);
	msb = msb<<16;//put this in ;east significant 2 bytes
	regfile [instruction.rt] = msb;
	return regfile[instruction.rt];
}
int op_sb (struct iform instruction) {
	return 0;
}
int op_sh (struct iform instruction) {
	return 0;
}
int op_sw (struct iform instruction) {
	return 0;
}
int op_swl (struct iform instruction) {
	return 0;
}
int op_swr (struct iform instruction) {
	return 0;
}
int op_syscall (struct rform instruction) {
	return 0;
}
int op_nop (struct rform instruction) {
	//sll r0 r0 0
	return 0;
}



// int * get_register (struct rform instruction)  {
// 	int reg[3];
// 	reg[0] = regfile[instruction.rs];
// 	reg[1] = regfile[instruction.rt];
// 	reg[2] = regfile[instruction.rd];
// 	return reg; //return pointer to registers
// }

// uint32_t * get_registerU (struct rform instruction) {
// 	uint32_t reg[3];
// 	reg[0] = regfile[instruction.rs];
// 	reg[1] =  (uint32_t)regfile[instruction.rt];
// 	reg[2] = (uint32_t)regfile[instruction.rd];
// 	return reg;
// }
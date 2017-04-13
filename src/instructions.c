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
int32_t *regfile;
int op_sllv (struct rform instruction);

int parseInstruction(uint32_t instruction, int32_t *reg) {
	regfile = reg;
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
	int rs = regfile[instruction.rs];
	int rt = regfile[instruction.rt];
	int rd = regfile[instruction.rd];
	rd = rs + rt;
	regfile[instruction.rd] = rd;
	return rd;
}
int op_addi (struct iform instruction) {
	return 0;
}
int op_addiu (struct iform instruction) {
	return 0;
}
int op_addu (struct rform instruction) {
	uint32_t rs = regfile[instruction.rt];
	uint32_t rt = regfile[instruction.rt];//check conversion
	uint32_t rd = rs + rt;
	regfile[instruction.rd] = rd;
	return rd;
}
int op_sub (struct rform instruction) {
	int rs = regfile[instruction.rs];
	int rt = regfile[instruction.rt];
	int rd = regfile[instruction.rd];
	rd = rs - rt;
	regfile[instruction.rd] = rd;
	return rd;
}
int op_subu (struct rform instruction) {
	uint32_t rs = regfile[instruction.rt];
	uint32_t rt = regfile[instruction.rt];//check conversion
	uint32_t rd = rs - rt;
	regfile[instruction.rd] = rd;
	return rd;
}
int op_div (struct rform instruction) {
	//divide by 0
	int lo = regfile[instruction.shamt];
	int hi = regfile[instruction.rd];
	int rs = regfile[instruction.rs];
	int rt = regfile[instruction.rt];
	lo = rs/rt; //get div
	hi = rs%rt; //get mod
	regfile[instruction.shamt] = lo;
	regfile[instruction.rd] = hi;
	return lo;
}
int op_divu (struct rform instruction) {//check
	uint32_t lo = regfile[instruction.shamt];
	uint32_t hi = regfile[instruction.rd];
	uint32_t rs = regfile[instruction.rs];
	uint32_t rt = regfile[instruction.rt];
	lo = rs/rt; //get div
	hi = rs%rt; //get mod
	regfile[instruction.shamt] = lo;
	regfile[instruction.rd] = hi;
	return lo;
}
int op_mult (struct rform instruction) {
	int lo = regfile[instruction.shamt];
	int hi = regfile[instruction.rd];
	int rs = regfile[instruction.rs];
	int rt = regfile[instruction.rt];
	int prod = rs*rt; //get product
	struct prod_bit {
		unsigned int low_bit: 32;
		unsigned int high_bit: 32;
	};
	//STOPPED HERE
	return 0;
}
int op_multu (struct rform instruction){
	return 0;
}
int op_mfhi (struct rform instruction) {
	return 0;
}
int op_mflo (struct rform instruction) {
	return 0;
}
int op_mthi (struct rform instruction) {
	return 0;
}
int op_mtlo (struct rform instruction) {
	return 0;
}
int op_and (struct rform instruction) {
	int rs = regfile[instruction.rs];
	int rt = regfile[instruction.rt];
	int rd = regfile[instruction.rd];
	rd = rs&rt;
	regfile[instruction.rd] = rd;
	return rd;
}
int op_andi (struct iform instruction) {
	return 0;
} 
int op_xor (struct rform instruction) {
	return 0;
}
int op_xori (struct iform instruction)  {
	return 0;
}
int op_nor (struct rform instruction) {
	return 0;
}
int op_or (struct rform instruction) {
	return 0;
}
int op_ori (struct iform instruction) {
	return 0;
}
int op_sll(struct rform instruction) {
	return 0;
}
int op_slv (struct rform instruction) {
	return 0;
}
int op_slt (struct rform instruction) {
	return 0;
}
int op_slti (struct iform instruction) {
	return 0;
}
int op_sltiu (struct iform instruction) {
	return 0;
}
int op_sltu (struct rform instruction) {
	return 0;
}
int op_sra (struct rform instruction) {
	return 0;
}
int op_srav (struct rform instruction) {
	return 0;
}
int op_srl (struct rform instruction) {
	return 0;
}
int op_srlv (struct rform instruction) {
	return 0;
}
int op_beq (struct iform instruction) {
	return 0;
}
int op_beql (struct iform instruction) {
	return 0;
}
int op_bgez (struct iform instruction) {
	return 0;
}
int op_bgtz (struct iform instruction) {
	return 0;
}
int op_blez (struct iform instruction) {
	return 0;
}
int op_blezl (struct iform instruction) {
	return 0;
}
int op_bltz (struct iform instruction) {
	return 0;
}
int op_bltzal (struct iform instruction) {
	return 0;
}
int op_bne (struct iform instruction) {
	return 0;
}
int op_bnel (struct iform instruction) {
	return 0;
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
int op_lb (struct iform instruction) {
	return 0;
}
int op_lbu (struct iform instruction) {
	return 0;
}
int op_lh (struct iform instruction) {
	return 0;
}
int op_lhu (struct iform instruction) {
	return 0;
}
int op_lui (struct iform instruction) {
	return 0;
}
int op_lw (struct iform instruction) {
	return 0;
}
int op_lwl (struct iform instruction) {
	return 0;
}
int op_lwr (struct iform instruction) {
	return 0;
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
	return 0;
}
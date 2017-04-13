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
		case 10001:
			return op_mthi (instruction);
		case 10011:
			return op_mtlo (instruction);
		case 100100:
			return op_and (instruction);
		case 100110:
			return op_xor (instruction);
		case 100111:
			return op_nor (instruction);
		case 100101:
			return op_or (instruction);
		case 0:
			return op_sll (instruction);
		case 101010:
			return op_slt (instruction);
		case 101011:
			return op_slt (instruction);
		case 11:
			return op_sra (instruction);
		case 111:
			return op_srav (instruction);
		case 10:
			return op_srl (instruction);
		case 110:
			return op_srlv (instruction);
		case 1001:
			return op_jalr (instruction);
		case 1000:
			return op_jr (instruction);
		case 1100:
			return op_syscall (instruction);
		case 100:
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
	case 001110: 
		return op_xori(instruction);
	case 0x0D: 
		return op_ori(instruction);
	case 0x0A: 
		return op_slti(instruction);
	case 0x0B: 
		return op_sltiu(instruction);
	case 0x04: 
		return op_beq(instruction);
	case 010100: 
		return op_beql(instruction);
	case 000001: {
		switch (instruction.rt) {
			case 00001: return op_bgez(instruction);
			// case 10001: return op_bgezal(instruction);
			case 00000: return op_bltz(instruction);
			case 10000: return op_bltzal(instruction);
			default: return 0;
		}
		
	}
	case 111: 
		return op_bgtz(instruction);
	case 110: 
		return op_blez(instruction);
	case 10110: 
		return op_blezl(instruction);
	case 101: 
		return op_bne(instruction);
	case 10101: 
		return op_bnel(instruction);
	case 100000: 
		return op_lb(instruction);
	case 100100: 
		return op_lbu(instruction);	
	case 100001: 
		return op_lh(instruction);	
	case 100101: 
		return op_lhu(instruction);	
	case 1111: 
		return op_lui(instruction);	
	case 100011: 
		return op_lw(instruction);	
	case 100010: 
		return op_lwl(instruction);	
	case 100110: 
		return op_lwr(instruction);
	case 101000: 
		return op_sb(instruction);
	case 101001: 
		return op_sh(instruction);
	case 101011: 
		return op_sw(instruction);
	case 101010: 
		return op_swl(instruction);
	case 101110: 
		return op_swr(instruction);

	}
	return 0;
}
 
int parseJForm (struct jform instruction) {
	switch (instruction.opcode) {
		case 10:
			return op_j(instruction);
		case 11: 
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
	return 0;
}
int op_sub (struct rform instruction) {
	return 0;
}
int op_subu (struct rform instruction) {
	return 0;
}
int op_div (struct rform instruction) {
	return 0;
}
int op_divu (struct rform instruction) {
	return 0;
}
int op_mult (struct rform instruction) {
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
	return 0;
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
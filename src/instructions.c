// all instruction code in here
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include "RegFile.h"
#include "Syscall.h"
#include "utils/heap.h"
#include "elf_reader/elf_reader.h"
#include "instructions.h"

void parseInstruction(uint32_t instruction) {
	 instruction = (jform) instruction; //cast to generic form of instruction
	 switch (instruction.opcode) {
	 	case 0x00:
	 		parseRForm(instruction); //parsing for r format operations
	 		break;
	 	case 0x02:
	 	case 0x03:
	 		parseJForm(instruction);
	 		break;
	 	default: 
	 		parseIForm(instruction);
	 }
}


void parseRForm (rform instruction) {
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
		case 100:
			return op_sllv (instruction);
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
		case 0: //check if sll  = nop
			return op_nop (instruction);
	}
}

void parseIForm (iform instruction) {
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
	case 000001: 
		return op_bgez(instruction);
	case 000001: //same opcode
		return op_bgezal(instruction);
	case 111: 
		return op_bgtz(instruction);
	case 110: 
		return op_blez(instruction);
	case 10110: 
		return op_blezl(instruction);
	case 000001: //same opcode
		return op_bltz(instruction);
	case 000001: //same opcode
		return op_bltzal(instruction);
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
}
 
void parseJForm (jform instruction) {
	switch (instruction.opcode) {
		case 10:
			return op_j(instruction);
		case 11: 
			return op_jal(instruction);
	}
}
void op_add (rform instruction);
void op_addi (iform instruction);
void op_addiu (iform instruction);
void op_addu (rform instruction);
void op_sub (rform instruction);
void op_subu (rform instruction);
void op_div (rform instruction);
void op_divu (rform instruction);
void op_mult (rform instruction);
void op_multu (rform instruction);
void op_mfhi (rform instruction);
void op_mflo (rform instruction);
void op_mthi (rform instruction);
void op_mtlo (rform instruction);
void op_and (rform instruction); 
void op_andi (iform instruction); 
void op_xor (rform instruction);
void op_xori (iform instruction);
void op_nor (rform instruction);
void op_or (rform instruction);
void op_ori (iform instruction);
void op_sll(rform instruction);
void op_slv (rform instruction);
void op_slt (rform instruction);
void op_slti (iform instruction);
void op_sltiu (iform instruction);
void op_sltu (rform instruction);
void op_sra (rform instruction);
void op_srav (rform instruction);
void op_srl (rform instruction);
void op_srlv (rform instruction);
void op_beq (iform instruction);
void op_beql (iform instruction);
void op_bgez (iform instruction);
void op_bgtz (iform instruction);
void op_blez (iform instruction);
void op_blezl (iform instruction);
void op_bltz (iform instruction);
void op_bltzal (iform instruction);
void op_bne (iform instruction);
void op_bnel (iform instruction);
void op_j (jform instruction);
void op_jal (jform instruction);
void op_jalr (rform instruction);
void op_jr (rform instruction);
void op_lb (iform instruction);
void op_lbu (iform instruction);
void op_lh (iform instruction);
void op_lhu (iform instruction);
void op_lui (iform instruction);
void op_lw (iform instruction);
void op_lwl (iform instruction);
void op_lwr (iform instruction);
void op_sb (iform instruction);
void op_sh (iform instruction);
void op_sw (iform instruction);
void op_swl (iform instruction);
void op_swr (iform instruction);
void op_syscall (rform instruction);
void op_nop (rform instruction);
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
			return op_mtlo (instruction);
	}
}


extern void op_add (rform instruction);
extern void op_addi (iform instruction);
extern void op_addiu (iform instruction);
extern void op_addu (rform instruction);
extern void op_sub (rform instruction);
extern void op_subu (rform instruction);
extern void op_div (rform instruction);
extern void op_divu (rform instruction);
extern void op_mult (rform instruction);
extern void op_multu (rform instruction);
extern void op_mfhi (rform instruction);
extern void op_mflo (rform instruction);
extern void op_mthi (rform instruction);
extern void op_mtlo (rform instruction);
extern void op_and (rform instruction); 
extern void op_andi (iform instruction); 
extern void op_xor (rform instruction);
extern void op_xori (iform instruction);
extern void op_nor (rform instruction);
extern void op_or (rform instruction);
extern void op_ori (iform instruction);
extern void op_sll(rform instruction);
extern void op_slv (rform instruction);
extern void op_slt (rform instruction);
extern void op_slti (iform instruction);
extern void op_sltiu (iform instruction);
extern void op_sltu (rform instruction);
extern void op_sra (rform instruction);
extern void op_srav (rform instruction);
extern void op_srl (rform instruction);
extern void op_srlv (rform instruction);
extern void op_beq (iform instruction);
extern void op_beql (iform instruction);
extern void op_bgez (iform instruction);
extern void op_bgtz (iform instruction);
extern void op_blez (iform instruction);
extern void op_blezl (iform instruction);
extern void op_bltz (iform instruction);
extern void op_bltzal (iform instruction);
extern void op_bne (iform instruction);
extern void op_bnel (iform instruction);
extern void op_j (jform instruction);
extern void op_jal (jform instruction);
extern void op_jalr (rform instruction);
extern void op_jr (rform instruction);
extern void op_lb (iform instruction);
extern void op_lbu (iform instruction);
extern void op_lh (iform instruction);
extern void op_lhu (iform instruction);
extern void op_lui (iform instruction);
extern void op_lw (iform instruction);
extern void op_lwl (iform instruction);
extern void op_lwr (iform instruction);
extern void op_sb (iform instruction);
extern void op_sh (iform instruction);
extern void op_sw (iform instruction);
extern void op_swl (iform instruction);
extern void op_swr (iform instruction);
extern void op_syscall (rform instruction);
extern void op_nop (rform instruction);
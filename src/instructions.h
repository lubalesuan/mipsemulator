#ifndef INSTRUCTIONS_H_
#define INSTRUCTIONS_H_
//structs for different formats of instructions
struct {
	unsigned int data: 26;
	unsigned int opcode: 6;
} jform; //abstract form of instruction

struct {
	unsigned int funct: 6;
	unsigned int shamt: 5;
	unsigned int rd: 5;
	unsigned int rt: 5;
	unsigned int rs: 5;
	unsigned int opcode: 6;
} rform;

struct {
	unsigned int constaddr: 16; // keep track of this dude
	unsigned int rt: 5;
	unsigned int rs: 5;
	unsigned int opcode: 6;		
} iform;
//parse instructions into formats
extern void parseInstruction(uint32_t instruction);
//parse formats into types of instructions 
extern void parseRForm (rform instruction);
extern void parseIForm (iform instruction);
extern void parseJForm (jform instruction);
//function for each type of instruction
extern void op_add (rform instruction);
extern void op_addi (iform instruction);
extern void op_addiu (iform instruction);
extern void op_addu (iform instruction);
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

#endif
#ifndef INSTRUCTIONS_H_
#define INSTRUCTIONS_H_
//structs for different formats of instructions
struct jform{
	unsigned int data: 26;
	unsigned int opcode: 6;
}; //abstract form of instruction

struct rform{
	unsigned int funct: 6;
	unsigned int shamt: 5;
	unsigned int rd: 5;
	unsigned int rt: 5;
	unsigned int rs: 5;
	unsigned int opcode: 6;
};

struct iform{
	unsigned int constaddr: 16; // keep track of this dude
	unsigned int rt: 5;
	unsigned int rs: 5;
	unsigned int opcode: 6;		
};
//parse instructions into formats
extern int parseInstruction(uint32_t instruction, int32_t *regfile);
//parse formats into types of instructions 
extern int parseRForm (struct rform instruction);
extern int parseIForm (struct iform instruction);
extern int parseJForm (struct jform instruction);
//function for each type of instruction
extern int op_add (struct rform instruction);
extern int op_addi (struct iform instruction);
extern int op_addiu (struct iform instruction);
extern int op_addu (struct rform instruction);
extern int op_sub (struct rform instruction);
extern int op_subu (struct rform instruction);
extern int op_div (struct rform instruction);
extern int op_divu (struct rform instruction);
extern int op_mult (struct rform instruction);
extern int op_multu (struct rform instruction);
extern int op_mfhi (struct rform instruction);
extern int op_mflo (struct rform instruction);
extern int op_mthi (struct rform instruction);
extern int op_mtlo (struct rform instruction);
extern int op_and (struct rform instruction); 
extern int op_andi (struct iform instruction); 
extern int op_xor (struct rform instruction);
extern int op_xori (struct iform instruction);
extern int op_nor (struct rform instruction);
extern int op_or (struct rform instruction);
extern int op_ori (struct iform instruction);
extern int op_sll(struct rform instruction);
extern int op_slt (struct rform instruction);
extern int op_sllv (struct rform instruction);

extern int op_slti (struct iform instruction);
extern int op_sltiu (struct iform instruction);
extern int op_sltu (struct rform instruction);
extern int op_sra (struct rform instruction);
extern int op_srav (struct rform instruction);
extern int op_srl (struct rform instruction);
extern int op_srlv (struct rform instruction);
extern int op_beq (struct iform instruction);
extern int op_beql (struct iform instruction);
extern int op_bgez (struct iform instruction);
// extern int op_bgezal (struct iform instruction);
extern int op_bgtz (struct iform instruction);
extern int op_blez (struct iform instruction);
extern int op_blezl (struct iform instruction);
extern int op_bltz (struct iform instruction);
extern int op_bltzal (struct iform instruction);
extern int op_bne (struct iform instruction);
extern int op_bnel (struct iform instruction);
extern int op_j (struct jform instruction);
extern int op_jal (struct jform instruction);
extern int op_jalr (struct rform instruction);
extern int op_jr (struct rform instruction);
extern int op_lb (struct iform instruction);
extern int op_lbu (struct iform instruction);
extern int op_lh (struct iform instruction);
extern int op_lhu (struct iform instruction);
extern int op_lui (struct iform instruction);
extern int op_lw (struct iform instruction);
extern int op_lwl (struct iform instruction);
extern int op_lwr (struct iform instruction);
extern int op_sb (struct iform instruction);
extern int op_sh (struct iform instruction);
extern int op_sw (struct iform instruction);
extern int op_swl (struct iform instruction);
extern int op_swr (struct iform instruction);
extern int op_syscall (struct rform instruction);
extern int op_nop (struct rform instruction);

//method to get signed rt, rs, rd
// extern int * get_register(struct rform instruction);
// extern uint32_t * get_registerU(struct rform instruction);
#endif
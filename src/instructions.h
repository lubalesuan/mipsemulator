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

extern void parseInstruction(uint32_t instruction); 
extern void parseRForm (rform instruction);
extern void parseIForm (iform instruction);
extern void parseJForm (jform instruction);
#endif
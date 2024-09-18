//Lucas Rolon, William Christie, Diego Mendoza

#include "spimcore.h"

/* ALU */
/* 10 Points */
void ALU(unsigned A, unsigned B, char ALUControl, unsigned *ALUresult, char *Zero)
{
    if (ALUControl == 0)
    {
        *ALUresult = A + B;
    }
    else if (ALUControl == 1)
    {
        *ALUresult = A - B;
    }
    else if (ALUControl == 2)
    {
        *ALUresult = ((int)A < (int)B);
    }
    else if (ALUControl == 3)
    {
        *ALUresult = A < B;
    }
    else if (ALUControl == 4)
    {
        *ALUresult = A && B;
    }
    else if (ALUControl == 5)
    {
        *ALUresult = A || B;
    }
    else if (ALUControl == 6)
    {
        *ALUresult = B << 16;
    }
    else if (ALUControl == 7)
    {
        *ALUresult = !A;
    }
    
    if (*ALUresult == 0)
        *Zero = 1;
    else
        *Zero = 0;

}

/* instruction fetch */
/* 10 Points */
int instruction_fetch(unsigned PC, unsigned *Mem, unsigned *instruction)
{
    if (PC % 4 != 0) {
        return 1;
    }

    *instruction = Mem[PC >> 2];
    return 0;
}


/* instruction partition */
/* 10 Points */
void instruction_partition(unsigned instruction, unsigned *op, unsigned *r1,unsigned *r2, unsigned *r3, unsigned *funct, unsigned *offset, unsigned *jsec)
{
    *op = (instruction & 0xfc000000) >> 26;
    *r1 = (instruction & 0x03e00000) >> 21;
    *r2 = (instruction & 0x001f0000) >> 16;
    *r3 = (instruction & 0x0000f800) >> 11;
    *funct = instruction & 0x0000003f;
    *offset = instruction & 0x0000ffff;
    *jsec = instruction & 0x03ffffff;
}


/* instruction decode */
/* 15 Points */
int instruction_decode(unsigned op, struct_controls *controls) {
    
    if (op == 0x0) {
        controls->RegDst = 1;
        controls->Jump = 0;
        controls->Branch = 0;
        controls->MemRead = 0;
        controls->MemtoReg = 0;
        controls->ALUOp = 7;
        controls->MemWrite = 0;
        controls->ALUSrc = 0;
        controls->RegWrite = 1;
    }
    else if (op == 0x2) {
        controls->RegDst = 0;
        controls->Jump = 1;
        controls->Branch = 0;
        controls->MemRead = 0;
        controls->MemtoReg = 0;
        controls->ALUOp = 0;
        controls->MemWrite = 0;
        controls->ALUSrc = 0;
        controls->RegWrite = 0;
    }
    else if (op == 0x4) {
        controls->RegDst = 2;
        controls->Jump = 0;
        controls->Branch = 1;
        controls->MemRead = 0;
        controls->MemtoReg = 2;
        controls->ALUOp = 1;
        controls->MemWrite = 0;
        controls->ALUSrc = 0;
        controls->RegWrite = 0;
    }
    else if (op == 0x8) {
        controls->RegDst = 0;
        controls->Jump = 0;
        controls->Branch = 0;
        controls->MemRead = 0;
        controls->MemtoReg = 0;
        controls->ALUOp = 0;
        controls->MemWrite = 0;
        controls->ALUSrc = 1;
        controls->RegWrite = 1;
    }
    else if (op == 0xA) {
        controls->RegDst = 0;
        controls->Jump = 0;
        controls->Branch = 0;
        controls->MemRead = 0;
        controls->MemtoReg = 0;
        controls->ALUOp = 2;
        controls->MemWrite = 0;
        controls->ALUSrc = 1;
        controls->RegWrite = 1;
    }
    else if (op == 0xB) {
        controls->RegDst = 0;
        controls->Jump = 0;
        controls->Branch = 0;
        controls->MemRead = 0;
        controls->MemtoReg = 0;
        controls->ALUOp = 3;
        controls->MemWrite = 0;
        controls->ALUSrc = 1;
        controls->RegWrite = 1;
    }
    else if (op == 0xC) {
        controls->RegDst = 0;
        controls->Jump = 0;
        controls->Branch = 0;
        controls->MemRead = 0;
        controls->MemtoReg = 0;
        controls->ALUOp = 4;
        controls->MemWrite = 0;
        controls->ALUSrc = 1;
        controls->RegWrite = 1;
    }
    else if (op == 0xD) {
        controls->RegDst = 0;
        controls->Jump = 0;
        controls->Branch = 0;
        controls->MemRead = 0;
        controls->MemtoReg = 0;
        controls->ALUOp = 5;
        controls->MemWrite = 0;
        controls->ALUSrc = 1;
        controls->RegWrite = 1;
    }
    else if (op == 0xF) {
        controls->RegDst = 0;
        controls->Jump = 0;
        controls->Branch = 0;
        controls->MemRead = 0;
        controls->MemtoReg = 0;
        controls->ALUOp = 6;
        controls->MemWrite = 0;
        controls->ALUSrc = 1;
        controls->RegWrite = 1;
    }
    else if (op == 0x23) {
        controls->RegDst = 0;
        controls->Jump = 0;
        controls->Branch = 0;
        controls->MemRead = 1;
        controls->MemtoReg = 1;
        controls->ALUOp = 0;
        controls->MemWrite = 0;
        controls->ALUSrc = 1;
        controls->RegWrite = 1;
    }
    else if (op == 0x2B) {
        controls->RegDst = 2;
        controls->Jump = 0;
        controls->Branch = 0;
        controls->MemRead = 0;
        controls->MemtoReg = 2;
        controls->ALUOp = 0;
        controls->MemWrite = 1;
        controls->ALUSrc = 1;
        controls->RegWrite = 0;
    }
    else {
        return 1;
    }

    return 0;
}

/* Read Register */
/* 5 Points */
void read_register(unsigned r1,unsigned r2,unsigned *Reg,unsigned *data1,unsigned *data2)
{
    *data1 = Reg[r1];
    *data2 = Reg[r2];
}


/* Sign Extend */
/* 10 Points */
void sign_extend(unsigned offset,unsigned *extended_value)
{
    int bin = 0xffff << 16;
    if (offset & (1 << 15)) {
        *extended_value = offset | bin;
    } else {
        *extended_value = offset;
    }
}

/* ALU operations */
/* 10 Points */
int ALU_operations(unsigned data1,unsigned data2,unsigned extended_value,unsigned funct,char ALUOp,char ALUSrc,unsigned *ALUresult,char *Zero)
{
    if (ALUSrc == 1)
        data2 = extended_value;

    char localALUOp = ALUOp;

    if (localALUOp == 7)
    {
        if (funct == 32)
            localALUOp = 0b000;
        else if (funct == 34)
            localALUOp = 0b001;
        else if (funct == 42)
            localALUOp = 0b010;
        else if (funct == 43)
            localALUOp = 0b011;
        else if (funct == 36)
            localALUOp = 0b100;
        else if (funct == 37)
            localALUOp = 0b101;
        else if (funct == 38)
            localALUOp = 0b110;
        else if (funct == 39)
            localALUOp = 0b111;
        else
            return 1;
    }

    ALU(data1, data2, localALUOp, ALUresult, Zero);

    return 0;
}

/* Read / Write Memory */
/* 10 Points */
int rw_memory(unsigned ALUresult,unsigned data2,char MemWrite,char MemRead,unsigned *memdata,unsigned *Mem)
{
    if (MemRead == 1) {
        if (ALUresult % 4 != 0) {
            return 1;
        }
        *memdata = Mem[ALUresult >> 2];
    }

    if (MemWrite == 1) {
        if (ALUresult % 4 != 0) {
            return 1;
        }
        Mem[ALUresult >> 2] = data2;
    }

    return 0;
}


/* Write Register */
/* 10 Points */
void write_register(unsigned r2,unsigned r3,unsigned memdata,unsigned ALUresult,char RegWrite,char RegDst,char MemtoReg,unsigned *Reg)
{    
    if (!RegWrite) return;

    unsigned write_register = RegDst ? r3 : r2;

    unsigned write_data = MemtoReg ? memdata : ALUresult;

    Reg[write_register] = write_data;
}

/* PC update */
/* 10 Points */
void PC_update(unsigned jsec, unsigned extended_value, char Branch, char Jump, char Zero, unsigned *PC)
{
    unsigned tempPC = *PC;
    tempPC += 4;

    if(Zero == 1 && Branch == 1)
        tempPC += extended_value << 2;

    if(Jump == 1)
        tempPC = (tempPC & 0xf0000000) | jsec << 2;

    *PC = tempPC;
}



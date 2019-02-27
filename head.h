#ifndef PROJECT3_H
#define PROJECT3_H

using namespace std;
extern int Main_Mem[1024];
extern int Regs[32];

class IF_ID{
public:
    IF_ID(int instruction1){
        instruction = instruction1;
    }
    int instruction;

};

class ID_EX{
public:
    int instruction = 0x0;

    //Control signals
    int RegDST = 0;
    int ALUSrc = 0;
    int ALUOp = 0;
    int MemRead = 0;
    int MemWrite = 0;
    int MemToReg = 0;
    int RegWrite = 0;

    short SEOffset = 0;

    int ReadReg1Value = 0;
    int ReadReg2Value = 0;
    int WriteReg_20_16 = 0;
    int WriteReg_15_11 = 0;

    int Func = 0x0;
    int opcode = 0x0;

    void init();
    void decode(int instruction1);
    void reset();
};

class EX_MEM{
public:
    void init();
    void execute(int MemRead1,int MemWrite1, int RegWrite1, int ALUOp1,
                     int ReadReg1Value1, int ReadReg2Value1, int SEOffset1,
                     int Func1, int opcode1, int RegDst1, int WriteReg20_16,
                     int WriteReg15_10, int MemToReg1);
    void reset();

    int MemRead;

    int MemWrite;
    int MemToReg;
    int RegWrite;

    int ReadReg1Value;
    int ReadReg2Value;

    short SEOffset;
    int ALUOp;

    int Zero;
    int ALUResult;
    int SWValue;
    int WriteRegNum;
    int Func;

};

class MEM_WB{
public:
    int MemToReg;
    int RegWrite;
    int MemRead;
    int MemWrite;

    int LWDataValue;
    int SWDataValue;
    int ALUResult;
    int WriteRegNum;

    void init();
    void AcMem(int MemToReg1, int RegWrite1, int ALUResult1, int WriteRegNum1, int MemRead1, int SWValue1, int MemWrite1);
    void write_back();
    void reset();
};

#endif

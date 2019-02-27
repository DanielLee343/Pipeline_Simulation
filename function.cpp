#include <iostream>
#include "head.h"

using namespace std;
//void IF_stage(int instruction);
int getBits31_26(int instruction);
int getBits25_21(int instruction);
int getBits20_16(int instruction);
int getBits15_11(int instruction);
int getBits5_0(int instruction);
int getBits15_0(int instruction);

void ID_EX::init(){
    instruction = 0x0;
    //Control signals
    RegDST = 0;
    ALUSrc = 0;
    ALUOp = 0;
    MemRead = 0;
    MemWrite = 0;
    MemToReg = 0;
    RegWrite = 0;
    SEOffset = 0;
    ReadReg1Value = 0;
    ReadReg2Value = 0;
    WriteReg_20_16 = 0;
    WriteReg_15_11 = 0;
    Func = 0x0;
    opcode = 0x0;
}

void ID_EX::decode(int instruction1){
    instruction = instruction1;
    opcode = getBits31_26(instruction);
    if(opcode == 0){//R format
        int src1 = getBits25_21(instruction);
        int src2 = getBits20_16(instruction);
        int dest = getBits15_11(instruction);
        int func = getBits5_0(instruction);
        if(func == 32 || func == 34){//add or sub
            RegDST = 1;
            ALUSrc = 0;
            MemToReg = 0;
            RegWrite = 1;
            MemRead = 0;
            MemWrite = 0;
            ALUOp = 0b10;
            WriteReg_20_16 = src2;
            WriteReg_15_11 = dest;
            ReadReg1Value = Regs[src1];
            ReadReg2Value = Regs[src2];
            if(func == 32){
                Func = 0x20;
            }else if(func == 34){
                Func = 0x22;
            }
            SEOffset = -9999;
        }
    }else{//I format
        int src1 = getBits25_21(instruction);
        int dest = getBits20_16(instruction);
        short offset = getBits15_0(instruction);
        if(opcode == 32 ){//lb
            RegDST = 0;
            ALUSrc = 1;
            MemToReg = 1;
            RegWrite = 1;
            MemRead = 1;
            MemWrite = 0;
            ALUOp = 0;
            WriteReg_20_16 = dest;
            WriteReg_15_11 = 0;
            ReadReg1Value = Regs[src1];
            ReadReg2Value = Regs[dest];
            Func = -9999;//trash
            //SEOffset = getBits15_0(instruction);
            SEOffset = offset;
        }else if(opcode == 40){//sb
            RegDST = -9999;
            ALUSrc = 1;
            MemToReg = -9999;
            RegWrite = 0;
            MemRead = 0;
            MemWrite = 1;
            ALUOp = 0;
            Func = -9999;//trash
            WriteReg_20_16 = dest;
            WriteReg_15_11 = 0;
            ReadReg1Value = Regs[src1];
            ReadReg2Value = Regs[dest];
            //SEOffset = getBits15_0(instruction);
            SEOffset = offset;
        }
    }
}


void ID_EX::reset(){
    //Control signals
    RegDST = 0;
    ALUSrc = 0;
    ALUOp = 0;
    MemRead = 0;
    MemWrite = 0;
    MemToReg = 0;
    RegWrite = 0;
    SEOffset = 0;
    ReadReg1Value = 0;
    ReadReg2Value = 0;
    WriteReg_20_16 = 0;
    WriteReg_15_11 = 0;
    Func = 0x0;
    opcode = 0x0;
}

void EX_MEM::init(){
    MemRead = 0;
    MemWrite = 0;
    MemToReg = 0;
    RegWrite = 0;
    ReadReg1Value = 0;
    ReadReg2Value = 0;
    SEOffset = 0;
    ALUOp = 0;
    Zero = 0;
    ALUResult = 0;
    SWValue = 0;
    WriteRegNum = 0;
    Func = 0;
}

void EX_MEM::execute(int MemRead1,int MemWrite1, int RegWrite1, int ALUOp1,
                     int ReadReg1Value1, int ReadReg2Value1, int SEOffset1,
                     int Func1, int opcode1, int RegDst1, int WriteReg20_16,
                     int WriteReg15_11, int MemToReg1){
    MemRead = MemRead1;
    MemWrite = MemWrite1;
    MemToReg = MemToReg1;
    RegWrite = RegWrite1;

    if (RegDst1 == 1){
        WriteRegNum = WriteReg15_11;
    }else if(RegDst1 == 0){
        WriteRegNum = WriteReg20_16;
    }else{
        WriteRegNum = -9999;
    }

    if(ALUOp1 == 0b10){
        if(Func1 == 32){//add
            ALUResult = ReadReg1Value1 + ReadReg2Value1;
            SWValue = ReadReg2Value1;
        }else if(Func1 == 34){//sub
            ALUResult = ReadReg1Value1 - ReadReg2Value1;
            SWValue = ReadReg2Value1;
        }
    }
    if(ALUOp1 == 0b00){
        if(opcode1 == 32 || opcode1 == 40){//lb
            ALUResult = ReadReg1Value1 + SEOffset1;
            SWValue = ReadReg2Value1;
        }
    }
}

void EX_MEM::reset(){
    MemRead = 0;
    MemWrite = 0;
    MemToReg = 0;
    RegWrite = 0;
    ReadReg1Value = 0;
    ReadReg2Value = 0;
    SEOffset = 0;
    ALUOp = 0;
    Zero = 0;
    ALUResult = 0;
    SWValue = 0;
    WriteRegNum = 0;
    Func = 0;
}


void MEM_WB::init(){
    MemToReg = 0;
    RegWrite = 0;
    MemRead = 0;
    MemWrite = 0;

    LWDataValue = 0;
    SWDataValue = 0;
    ALUResult = 0;
    WriteRegNum = 0;
}

void MEM_WB::AcMem(int MemToReg1, int RegWrite1, int ALUResult1, int WriteRegNum1,
                   int MemRead1, int SWValue1, int MemWrite1){
    MemToReg = MemToReg1;
    RegWrite = RegWrite1;
    ALUResult = ALUResult1;
    WriteRegNum = WriteRegNum1;
    SWDataValue = SWValue1;

    if(MemRead1 == 1){
        LWDataValue = Main_Mem[ALUResult];
    }else if(MemWrite1 == 1){
        Main_Mem[ALUResult] = SWValue1;
    }
}

void MEM_WB::write_back(){
    if (WriteRegNum != -9999){
        if (MemWrite == 0 && MemToReg == 0){ // r type{
            Regs[WriteRegNum] = ALUResult;
        }else if(MemWrite == 0 && MemToReg == 1){ //lb
            LWDataValue = Main_Mem[ALUResult];
            Regs[WriteRegNum] = LWDataValue;
        }

    }
}

void MEM_WB::reset(){
    MemToReg = 0;
    RegWrite = 0;
    MemRead = 0;
    MemWrite = 0;
    RegWrite = 0;
    LWDataValue = 0;
    SWDataValue = 0;
    ALUResult = 0;
    WriteRegNum = 0;
}



void Print_MainMem(){
    for(int i = 0; i < 1024; i++){
        cout<<"Main_Mem["<<i<<"] = "<<Main_Mem[i];
    }
}





int getBits31_26(int instruction){
    return (instruction & 0xFC000000) >> 26;
}

int getBits25_21(int instruction){
    return (instruction & 0x3E00000) >> 21;
}

int getBits20_16(int instruction){
    return (instruction & 0x1F0000) >> 16;
}

int getBits15_11(int instruction){
    return (instruction & 0xF800) >> 11;
}

int getBits5_0(int instruction){
    return (instruction & 0x3F);
}

int getBits15_0(int instruction){
    return(instruction & 0xFFFF);
}

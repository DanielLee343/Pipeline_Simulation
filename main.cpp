#include <iostream>
#include "head.h"
#include <string.h>

using namespace std;
//Initialize 2*4=8 pipeline registers
IF_ID IF_ID_WRITE(0x0);
IF_ID IF_ID_READ(0x0);
ID_EX ID_EX_WRITE;
ID_EX ID_EX_READ;
EX_MEM EX_MEM_WRITE;
EX_MEM EX_MEM_READ;
MEM_WB MEM_WB_WRITE;
MEM_WB MEM_WB_READ;
void IF_stage(int instruction);
void ID_stage();
void EX_stage();
void MEM_stage();
void WB_stage();
string Trash_To_X(int value);
int String_To_Int(string s);


int Main_Mem[1024];
int Regs[32];

//Five stages
void IF_stage(int instruction){
    IF_ID_WRITE.instruction = instruction;
    cout<<hex<<instruction<<endl;
}

void ID_stage(){
    ID_EX_WRITE.decode(IF_ID_READ.instruction);
}
void EX_stage(){
    EX_MEM_WRITE.execute(ID_EX_READ.MemRead, ID_EX_READ.MemWrite, ID_EX_READ.RegWrite, ID_EX_READ.ALUOp,
               ID_EX_READ.ReadReg1Value, ID_EX_READ.ReadReg2Value, ID_EX_READ.SEOffset,
                ID_EX_READ.Func, ID_EX_READ.opcode, ID_EX_READ.RegDST, ID_EX_READ.WriteReg_20_16,
                 ID_EX_READ.WriteReg_15_11, ID_EX_READ.MemToReg);
}
void MEM_stage(){
    MEM_WB_WRITE.AcMem(EX_MEM_READ.MemToReg, EX_MEM_READ.RegWrite, EX_MEM_READ.ALUResult,
                       EX_MEM_READ.WriteRegNum, EX_MEM_READ.MemRead, EX_MEM_READ.SWValue,
                       EX_MEM_READ.MemWrite);
}

void WB_stage(){
    MEM_WB_READ.write_back();
}

void Copy_write_to_read(){
    IF_ID_READ = IF_ID_WRITE;
    ID_EX_READ = ID_EX_WRITE;
    EX_MEM_READ = EX_MEM_WRITE;
    MEM_WB_READ  = MEM_WB_WRITE;

    ID_EX_WRITE.reset();
    EX_MEM_WRITE.reset();
    MEM_WB_WRITE.reset();
}
void Print_out_everything(){
    cout<<endl<<"REGISTERS: "<<endl;
    for(int i = 0; i < 32; i++){
        cout<< "[$"<<dec<< i << " = 0x" << hex<< Regs[i]<<"]"<<endl;
    }
    cout<<endl;

    cout<< "IF/ID Write (written to by the IF stage)"<<endl;
    if (IF_ID_WRITE.instruction == 0){
        cout<< "Control = 00000000"<<endl;
    }else{
        cout<< "Instruction = "<<hex<<IF_ID_WRITE.instruction<<endl;
    }
    cout<< "IF/ID Read (read by the ID stage)"<<endl;
    if(IF_ID_READ.instruction == 0){
        cout<< "Control = 00000000"<<endl;
    }else{
        cout<< "Instruction = "<< hex<<IF_ID_READ.instruction<<endl;
    }

    cout<<endl<<"ID/EX Write (written to by the ID stage)"<<endl;
    cout<<"Control: RegDST = "<< Trash_To_X(ID_EX_WRITE.RegDST)<<"    ALUSrc = "<<ID_EX_WRITE.ALUSrc;
    cout<<"    ALUOp = "<<ID_EX_WRITE.ALUOp<<"    MemRead = "<<ID_EX_WRITE.MemRead;
    cout<<"    MemWrite = "<<ID_EX_WRITE.MemWrite<<"     MemToReg = "<<Trash_To_X(ID_EX_WRITE.MemToReg)<<"    RegWrite = "<<ID_EX_WRITE.RegWrite;
    cout<<endl;
    cout<<"ReadReg1Value = "<<hex<<ID_EX_WRITE.ReadReg1Value;
    cout<<"     ReadReg2Value = "<<hex<<ID_EX_WRITE.ReadReg2Value;
    cout<<endl;
    if(ID_EX_WRITE.SEOffset == -9999){
        cout<<"SEOffset = X";
    }else{
        cout<<"SEOffset = "<<hex<<ID_EX_WRITE.SEOffset;
    }
    cout<<dec<<"     WriteReg_20_16 = "<<ID_EX_WRITE.WriteReg_20_16;
    cout<<dec<<"     WriteReg_15_11 = "<<ID_EX_WRITE.WriteReg_15_11;
    cout<<hex<<"     Function = "<<Trash_To_X(ID_EX_WRITE.Func);
    cout<<endl;
    cout<<"ID/EX Read (read by the EX stage)"<<endl;
    cout<<"Control: RegDST = "<< Trash_To_X(ID_EX_READ.RegDST)<< "    ALUSrc = "<<ID_EX_READ.ALUSrc;
    cout<<"     ALUOp = "<<ID_EX_READ.ALUOp<<"     MemRead = "<<ID_EX_READ.MemRead;
    cout<<"     MemWrite = "<<ID_EX_READ.MemWrite<<"      MemToReg = "<<Trash_To_X(ID_EX_READ.MemToReg)<<"     RegWrite ="<<ID_EX_READ.RegWrite;
    cout<<endl;
    cout<<"ReadReg1Value = "<<hex<<ID_EX_READ.ReadReg1Value;
    cout<<"     ReadReg2Value = "<<hex<<ID_EX_READ.ReadReg2Value;
    cout<<endl;
    if(ID_EX_READ.SEOffset == -9999){
        cout<<"SEOffset = X";
    }else{
        cout<<"SEOffset = "<<hex<<ID_EX_READ.SEOffset;
    }
    cout<<dec<<"    WriteReg_20_16 = "<<ID_EX_READ.WriteReg_20_16;
    cout<<dec<<"    WriteReg_15_11 = "<<ID_EX_READ.WriteReg_15_11;
    cout<<hex<<"    Function = "<<Trash_To_X(ID_EX_READ.Func);
    cout<<endl;

    cout<<endl<<"EX/MEM Write (written to by the EX stage)"<<endl;
    cout<<"Control: MemRead = "<<EX_MEM_WRITE.MemRead<<"    MemWrite = "<<EX_MEM_WRITE.MemWrite;
    cout<<"     MemToReg = "<<Trash_To_X(EX_MEM_WRITE.MemToReg)<<"   RegWrite = "<<EX_MEM_WRITE.RegWrite;
    cout<<endl;
    cout<<"Zero = "<<EX_MEM_WRITE.Zero<<"   ALUResult = "<<hex<<EX_MEM_WRITE.ALUResult;
    cout<<endl;
    cout<<"SWValue = "<<hex<<EX_MEM_WRITE.SWValue<< "   WriteRegNum = "<<Trash_To_X(EX_MEM_WRITE.WriteRegNum);
    cout<<endl;
    cout<<"EX/MEM READ (read by the MEM stage)"<<endl;
    cout<<"Control: MemRead = "<<EX_MEM_READ.MemRead<<"     MemWrite = "<<EX_MEM_READ.MemWrite;
    cout<<"     MemToReg = "<<Trash_To_X(EX_MEM_READ.MemToReg)<<"    RegWrite = "<<EX_MEM_READ.RegWrite;
    cout<<endl;
    cout<<"Zero = "<<EX_MEM_READ.Zero<<"    ALUResult = "<<hex<<EX_MEM_READ.ALUResult;
    cout<<endl;
    cout<<"SWValue = "<<hex<<EX_MEM_READ.SWValue<< "    WriteRegNum = "<<Trash_To_X(EX_MEM_READ.WriteRegNum);
    cout<<endl;

    cout<<endl<<"MEM/WB Write (written to by the MEM stage)"<<endl;
    cout<<"Control: MemToReg = "<<Trash_To_X(MEM_WB_WRITE.MemToReg)<<"  RegWrite = "<<MEM_WB_WRITE.RegWrite<<endl;
    cout<<"LWDataValue = "<<hex<<MEM_WB_WRITE.LWDataValue;
    cout<<"     ALUResult = "<<hex<<MEM_WB_WRITE.ALUResult;
    cout<<"     WriteRegNum = "<<Trash_To_X(MEM_WB_WRITE.WriteRegNum);
    cout<<endl;
    cout<<"MEM/WB Read (read by the WB stage)"<<endl;
    cout<<"Control: MemToReg = "<<Trash_To_X(MEM_WB_READ.MemToReg)<<"      RegWrite = "<<MEM_WB_READ.RegWrite<<endl;
    cout<<"LWDataValue = "<<hex<<MEM_WB_READ.LWDataValue;
    cout<<"     ALUResult = "<<hex<<MEM_WB_READ.ALUResult;
    cout<<"     WriteRegNum = "<<Trash_To_X(MEM_WB_READ.WriteRegNum);
    cout<<endl<<"************************************************************************************************"<<endl;
}

string Trash_To_X(int value){
    if(value == -9999 || value == 0xd8f1){
        return "X";
    }else{
        return to_string(value);
    }
}

int String_To_Int(string s){//Not used in this program
    char *end;
    int i;
    i = static_cast<int>(strtol(s.c_str(),&end,10));
    return i;
}

int main()
{
    int j = 0, k = 0x101;
    int instruction;
    unsigned int instructionCache[12] = {0xA1020000,
            0x810AFFFC,
            0x00831820,
            0x01263820,
            0x01224820,
            0x81180000,
            0x81510010,
            0x00624022,
            0x00000000,
            0x00000000,
            0x00000000,
            0x00000000};

    //unsigned int instructionCache[5] = {0x00a63820, 0x8d0f0004, 0xad09fffc, 0x00625022, 0x10c8fffb};

    //Initializing Main_Mem
    for(int i = 0; i < 1024; i++){
        Main_Mem[i] = j;
        j++;
        if(j > 0xFF){
            j= 0;
        }
    }

    //Initializing Regs
    Regs[0] = 0;
    for(int i = 1; i < 32; i++){
        Regs[i] = k;
        k++;
    }

    for(int i = 0; i < 12; i++){//Main loop
        cout<<"CURRENT CLOCK CYCLE:"<<dec<<i+1<<endl;
        instruction = instructionCache[i];
        IF_stage(instruction);
        ID_stage();
        EX_stage();
        MEM_stage();
        WB_stage();
        Print_out_everything();
        Copy_write_to_read();
        cout<<endl<<endl<<endl<<endl<<endl;
    }
    return 0;
}


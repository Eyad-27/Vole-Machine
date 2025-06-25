#pragma once
#ifndef NEW_VOLE_MACHINE_HEADER_H
#define NEW_VOLE_MACHINE_HEADER_H
#include <iostream>
#include <utility>
#include <vector>
#include <sstream>
#include <fstream>
#include <limits>
#include <bitset>
#include <string>
#include <math.h>

using namespace std;


string hexToDec(const std::string& hexNum);
string decToHex(const std::string& decimal);
bool isValidHexa(const std::string& hexNumber);
string hexToBinary(string hex);
float binaryToFloat(const std::string& binary);
string floatToBinary(float number);


class Memory
{
private:
    vector<string> mem_cells;
    const int size = 256;
public:
    Memory();
    string get_cell(const string& address);
    void set_cell(const string& address, const std::string& value);
    void resetMemory();
};

class Register
{
private:
    vector<string> regi_cells;
    const int size = 16;
public:
    Register();
    string get_cell(const string& address);
    void set_cell(const string& address, const std::string& value);
    void resetRegister();
};




class CU
{
private:
    vector<char> screen;
public:
    vector<char> get_screen();
    void clear_screen();
    void load_1(const string& indxRegi, const string& indxMem, Memory& mem, Register& regi);
    void load_2(const std::string& indxRegi, const std::string& XY, Register& regi);
    void store(const std::string& indxRegi, const std::string& indxMem, Memory& mem, Register& regi);
    void write_screen(const std::string& indxRegi, Memory& mem, Register& regi);
    void move_regi(const string& indxRegi1, const string& indxRegi2, Register& regi);
    void jump(const string& indxRegi, const string& XY, Memory& mem, Register& regi, int &PC);
    void halt();

};

class ALU
{
public:
    bool isValid(string instruc);
    void add(const string& indxRegi1, const string& indxRegi2, const string& indxRegi3, Register& regi);
    float decode_float(const string& binary);
    void add_float(const string& indxRegi1, const string& indxRegi2, const string& indxRegi3, Register& regi);
};


class CPU : public CU, public ALU, public Register
{
private:
    int PC;
    string IR;
public:
    CPU() : PC(1), IR("0000")
    {

    }

    void setPC(int newPC);
    int getPC();
    void setIR(const string& s);
    string getIR();
    void fetch_instruction(Memory& mem);
    vector<string> decode();
    int execute(Register& regi, Memory& mem, vector<string> dIR);
    void resetCPU(Register& regi);


};


class Machine : public CPU, public Memory
{
public:
    bool loadProgramFile(const string& fileName, const string& beginAddress, Memory& mem);
    void outputStatus(Memory& mem, Register& regi);
};


class MachineUI : public Machine
{
public:
//    bool getFile();
    void displayMenu();
    string inputFileName();
    int getChoice();
    void start();


};



#endif //NEW_VOLE_MACHINE_HEADER_H
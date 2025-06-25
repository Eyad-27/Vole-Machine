#include "header.h"
#include <iostream>


string hexToDec(const std::string& hexNum)
{
    int decimal;
    std::stringstream ss;


    ss << std::hex << hexNum;
    ss >> decimal;

    return to_string(decimal);
}

string decToHex(const std::string& decimal)
{
    int dec = stoi(decimal);
    std::stringstream ss;
    ss << std::hex << std::uppercase << dec;
    string hexa = ss.str();
    if (hexa.length() < 2)
        return ('0' + hexa);
    else
        return hexa;
}

bool isValidHexa(const std::string& hexNumber) {
    // Check if string is empty
    if (hexNumber.length() != 2) {
        return false;
    }

    // Check each character in the string
    for (char c : hexNumber) {
        // Check if character is a valid hexadecimal digit (0-9, A-F, a-f)
        if (!((c >= '0' && c <= '9') ||
              (c >= 'A' && c <= 'F') ||
              (c >= 'a' && c <= 'f'))) {
            return false;
        }
    }

    return true;
}

string hexToBinary(string hex)
{
    bitset<8> binary(stoi(hex, nullptr, 16));
    string binary2 = binary.to_string();
    return binary2;
}

float binaryToFloat(const std::string& binary) {
    size_t pointPosition = binary.find('.');

    // Separate integer and fractional parts
    std::string integerPart = binary.substr(0, pointPosition);
    std::string fractionalPart = binary.substr(pointPosition + 1);

    // Convert integer part
    float decimalValue = 0.0;
    for (size_t i = 0; i < integerPart.length(); ++i) {
        if (integerPart[integerPart.length() - 1 - i] == '1') {
            decimalValue += std::pow(2, i);
        }
    }

    // Convert fractional part
    float frac = 0.5f;
    for (size_t i = 0; i < fractionalPart.length(); ++i) {
        if (fractionalPart[i] == '1') {
            decimalValue += frac;
        }
        frac *= 0.5f;
    }

    return decimalValue;
}

string floatToBinary(float number) {
    // Determine the sign bit
    int sign = (number < 0) ? 1 : 0;
    number = std::fabs(number); // Work with the absolute value

    // Normalize the number
    int exponent = 0;
    while (number >= 2.0f) {
        number /= 2.0f;
        exponent++;
    }
    while (number < 1.0f && number > 0) {
        number *= 2.0f;
        exponent--;
    }

    // Calculate biased exponent
    int bias = 4; // Given bias
    int biasedExponent = exponent + bias;

    // Ensure the biased exponent fits in 3 bits
    if (biasedExponent < 0 || biasedExponent > 7) {
        std::cout << "Exponent out of range!" << std::endl;
        return 0;
    }

    // Calculate the mantissa
    // Get 4 bits for the mantissa
    int mantissa = static_cast<int>((number - 1.0f) * 16); // 4 bits -> 16
    mantissa &= 0x0F; // Mask to ensure it fits in 4 bits

    // Combine into a single binary representation
    int floatingPointRepresentation = (sign << 7) | (biasedExponent << 4) | mantissa;

    // Print the binary representation
    bitset<8> bset(floatingPointRepresentation);
    return bset.to_string();
}

int binary2CompToDecimal(const std::string &binary) {
    // Create an 8-bit bitset
    std::bitset<8> bits(binary);

    // Convert to an unsigned long, then cast to signed int
    int decimal = static_cast<int>(bits.to_ulong());

    // If the leftmost bit is 1 (indicating a negative number in 8-bit two's complement)
    if (binary[0] == '1') {
        decimal -= 1 << 8; // Subtract 2^8 to adjust for two's complement
    }

    return decimal;
}
/*=================================================================================*/

Memory::Memory()
{
    mem_cells.resize(size);
    fill(mem_cells.begin(), mem_cells.end(), "00");
}

string Memory::get_cell(const std::string &address)
{
    return this->mem_cells[stoi(hexToDec(address))];
}

void Memory::set_cell(const std::string &address, const std::string& value)
{
    this->mem_cells[stoi(hexToDec(address))] = value;
}

void Memory::resetMemory()
{
    fill(mem_cells.begin(), mem_cells.end(), "00");
}

/*=================================================================================*/


Register::Register()
{
    regi_cells.resize(size);
    fill(regi_cells.begin(), regi_cells.end(), "00");
}

string Register::get_cell(const std::string &address)
{
    return this->regi_cells[stoi(hexToDec(address))];
}

void Register::set_cell(const std::string &address, const std::string& value)
{
    this->regi_cells[stoi(hexToDec(address))] = value;
}

void Register::resetRegister()
{
    fill(regi_cells.begin(), regi_cells.end(), "00");
}


/*=================================================================================*/

vector<char> CU::get_screen() { return screen; }
void CU::clear_screen() { screen.clear(); }

// LOAD the register R with the bit pattern found in the memory cell whose address is XY.
void CU::load_1(const string& indxRegi, const string& indxMem, Memory &mem, Register& regi)
{
    regi.set_cell(indxRegi, mem.get_cell(indxMem));
}

// LOAD the register R with the bit pattern XY.
void CU::load_2(const std::string& indxRegi, const std::string& XY, Register &regi)
{
    regi.set_cell(indxRegi, XY);
}

// STORE the bit pattern found in register R in the memory cell whose address is XY.
void CU::store(const std::string& indxRegi, const std::string& indxMem, Memory &mem, Register &regi)
{
    mem.set_cell(indxMem, regi.get_cell(indxRegi));
}

// STORE to location 00, which is a memory mapping for the screen. Writing to 00 is writing to screen.
void CU::write_screen(const std::string& indxRegi, Memory &mem, Register &regi)
{
    mem.set_cell("00", regi.get_cell(indxRegi));
    screen.push_back(char(stoi(hexToDec(mem.get_cell("00")))));
}

// MOVE the bit pattern found in register 1 to register 2.
void CU::move_regi(const std::string &indxRegi1, const std::string &indxRegi2, Register &regi)
{
    regi.set_cell(indxRegi2, regi.get_cell(indxRegi1));
}

/* JUMP to the instruction located in the memory cell at address XY if the bit pattern in register R
is equal to the bit pattern in register number 0. Otherwise, continue with the normal sequence of
execution. */
void CU::jump(const std::string &indxRegi, const std::string &XY, Memory &mem, Register &regi, int &PC)
{
    if (regi.get_cell(indxRegi) == regi.get_cell("00"))
    {
        PC = stoi(hexToDec(XY));
    }
}

void CU::halt()
{
    cout << "program finished.\n";
}

/*=================================================================================*/

bool ALU::isValid(std::string instruc)
{
    if (instruc.length() != 4)
        return false;

    switch (toupper(instruc[0]))
    {
        case '1':
        case '2':
        case '3':
            return true;
            break;
        case '4':
        {
            if (instruc[1] != 0)
                return false;
            else
                return true;
            break;
        }
        case '5':
        case '6':
        case 'B':
            return true;
            break;
        case 'C':
        {
            if (instruc.substr(1, 3) == "000")
                return true;
            else
                return false;
            break;
        }
        default:
            return false;
            break;

    }
}

void ALU::add(const string &indxRegi1, const string &indxRegi2, const string &indxRegi3, Register &regi)
{
    string binary1;
    string binary2 = hexToBinary(regi.get_cell(indxRegi2));
    string binary3 = hexToBinary(regi.get_cell(indxRegi3));
    int num2 = binary2CompToDecimal(binary2);
    int num3 = binary2CompToDecimal(binary3);
    int num1 = num2 + num3;
    bitset<8> n(num1);
    binary1 = n.to_string();
    regi.set_cell(indxRegi1,decToHex(to_string(stoi(binary1, nullptr, 2))));
}

float ALU::decode_float(const std::string &binary)
{
    //using implicit
    int exp = stoi(binary.substr(1, 3), nullptr, 2) - 4;
    int sign = stoi(binary.substr(0, 1));
    string mantissa = "1" + binary.substr(4, 4);

    string temp = "";
    if (exp > 0)
    {
        for (int i = 0; i < 5; i++)
        {
            temp += mantissa[i];
            if (i == exp)
            {
                temp += ".";
            }
        }
    }
    else if (exp < 0)
    {
        temp = "0." + string(-exp - 1, '0') + mantissa;
    }
    else
    {
        temp = "1." + mantissa.substr(1);
    }
    if (sign == 0)
        return binaryToFloat(temp);
    else
        return (-1 * binaryToFloat(temp));
}
void ALU::add_float(const string &indxRegi1, const string &indxRegi2, const string &indxRegi3, Register &regi)
{
    string binary2 = hexToBinary(regi.get_cell(indxRegi2));
    string binary3 = hexToBinary(regi.get_cell(indxRegi3));
    string binary1 = floatToBinary(decode_float(binary2) + decode_float(binary3));
    string hex1 = decToHex(to_string(stoi(binary1, nullptr, 2)));
    regi.set_cell(indxRegi1, hex1);

}

/*=================================================================================*/

void CPU::setPC(int newPC)
{
    PC = newPC;
}

int CPU::getPC()
{
    return PC;
}

void CPU::setIR(const string& s)
{
    IR = s;
}
string CPU::getIR()
{
    return IR;
}

void CPU::fetch_instruction(Memory &mem)
{
    string temp = "";
    for (int i = 0; i <2; i++)
    temp += mem.get_cell(decToHex(to_string(PC + i)));
    IR = temp;
    PC += 2;
}

vector<string> CPU::decode()
{
    vector<string> v;
    for (int i = 0; i < 4; i++)
        v.push_back(string(1, IR[i]));
    return v;
}

int CPU::execute(Register &regi, Memory &mem, vector<string> dIR)
{
    switch (toupper(dIR[0][0])) {
        case '1':
            load_1(dIR[1], dIR[2] + dIR[3], mem, regi);
            break;
        case '2':
            load_2(dIR[1], dIR[2] + dIR[3], regi);
            break;
        case '3':
        {
            if (dIR[2] + dIR[3] == "00")
                write_screen(dIR[1], mem, regi);
            else
                store(dIR[1], dIR[2] + dIR[3], mem, regi);

            break;

        }
        case '4':
            move_regi(dIR[2], dIR[3], regi);
            break;
        case '5':
            add(dIR[1], dIR[2], dIR[3], regi);
            break;
        case '6':
            add_float(dIR[1], dIR[2], dIR[3], regi);
            break;
        case 'B':
            jump(dIR[1], dIR[2] + dIR[3], mem, regi, PC);
            break;
        case 'C':
            halt();
            PC -= 2;
            return 1;
            break;
        default:
            cout << endl << "invalid instruction" << endl;
            return 1;
            break;
    }
    return 0;
}

void CPU::resetCPU(Register &regi)
{
    resetRegister();
    setPC(0);
    setIR("0000");

}

/*=================================================================================*/

bool Machine::loadProgramFile(const string& fileName, const string& beginAddress, Memory& mem)
{
    setPC(stoi(hexToDec(beginAddress)));
    string ins;
    ifstream file(fileName);
    if (!file.is_open())
    {
        cerr << "Unable to open the file." << endl;
        return false;
    }

    int counter = getPC();
    while (file >> ins)
    {
        mem.set_cell(decToHex(to_string(counter)), ins.substr(0, 2));
        mem.set_cell(decToHex(to_string(counter + 1)), ins.substr(2, 2));
        counter += 2;
    }
    return true;
}

void Machine::outputStatus(Memory &mem, Register &regi)
{
    cout << "PC: " << decToHex(to_string(getPC())) << endl;
    cout << "IR: " << getIR() << endl;
    cout << endl;
    cout << "screen: ";
    vector<char> screen = get_screen();
    for (int i = 0; i < screen.size(); i++)
        cout << screen[i];
    cout << endl << endl;
    cout << "Registers: " << endl;
    for (int i = 0; i < 16; i++)
    {
        cout << "R" << decToHex(to_string(i)) << " = "
        << regi.get_cell(decToHex(to_string(i))) << endl;
    }

    cout << endl;
    cout << "Memory:" << endl;
//    for (int i = 0; i < 256; i++)
//    {
//        cout << "Mem( " << decToHex(to_string(i)) << " ) = "
//             << mem.get_cell(decToHex(to_string(i))) << endl;
//    }
    vector<char> ch = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
    for (int i = 0; i < 16; i++)
        cout << "      " << ch[i];
    cout << endl;

    int indx = 0;
    for (int i = 0; i < 16; i++)
    {
        cout << ch[i] << "     ";
        for (int j = 0; j < 16; j++)
        {
            cout << mem.get_cell(decToHex(to_string(indx))) << "     ";
            indx++;
        }
        cout << endl;
    }
}

/*=================================================================================*/

void MachineUI::displayMenu()
{
    cout << "Menu:" << endl;
    cout << "1- load instructions file" << endl
         << "2- execute 1 instruction" << endl
         << "3- execute all instructions" << endl
         << "4- display program status" << endl
         << "5- reset CPU" << endl
         << "6- reset Memory" << endl
         << "7- clear screen" << endl
         << "8- exit" << endl;
}

int MachineUI::getChoice()
{
    int choice;

    while (true) {
        cout << "Enter your choice (1-8): ";
        cin >> choice;

        if (std::cin.fail() || choice < 1 || choice > 8) {
            cerr << "Invalid input. Please enter a number between 1 and 5.\n";

            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        } else {
            break;
        }
    }

    return choice;
}

string MachineUI::inputFileName()
{
    string s;
    cout << "Enter file name: ";
    cin >> s;
    return s;
}

void MachineUI::start()
{
    while (true)
    {
        displayMenu();
        int choice = getChoice();
        switch(choice)
        {
            case 1:
            {
                string address;
                do {
                    cout << "Enter first storing address(hexa of 2 digits): ";
                    cin >> address;
                }
                while (!isValidHexa(address));

                if (loadProgramFile(inputFileName(), address, *this))
                {
                    cout << "instructions loaded successfully." << endl << endl;
                }
                break;
            }
            case 2:
            {
                fetch_instruction(*this);
                execute(*this, *this, this->decode());
                cout << endl << "instruction executed successfully" << endl;
                break;
            }
            case 3:
            {
                while(true)
                {
                    fetch_instruction(*this);
                    if(execute(*this, *this, this->decode()))
                    {
                        break;
                    }
                }
                break;

            }
            case 4:
                outputStatus(*this, *this);
                break;
            case 5:
                resetCPU(*this);
                cout << "reset CPU successfully." << endl << endl;
                break;
            case 6:
                resetMemory();
                cout << "reset Memory successfully." << endl << endl;
                break;
            case 7:
                clear_screen();
                cout << "screen cleared successfully." << endl << endl;
                break;
            case 8:
                exit(0);

        }
    }
}

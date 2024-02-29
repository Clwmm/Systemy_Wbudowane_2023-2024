#include <iostream>
#include <conio.h>
#include <iomanip>

#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_ENTER 13
#define KEY_ESC 27

int cgetch = 0;
bool main_menu = true;
bool simulation = false;
int main_menu_id = 1;
int input_id = 1;
bool inputs[2] = { false, false };
std::string logic_gates[7] = { "NOT", "AND", "NAND", "OR", "NOR", "XOR", "XNOR" };

bool Not(const bool& input) {
	return !input;
}

bool And(const bool& in1, const bool& in2) {
	if (in1 && in2)
		return true;
	return false;
}

bool Nand(const bool& in1, const bool& in2) {
	return Not(And(in1, in2));
}

bool Or(const bool& in1, const bool& in2) {
	if (in1 || in2)
		return true;
	return false;
}

bool Nor(const bool& in1, const bool& in2) {
	return Not(Or(in1, in2));
}

bool Xor(const bool& in1, const bool& in2) {
	if (in1 != in2)
		return true;
	return false;
}

bool Xnor(const bool& in1, const bool& in2) {
	return Not(Xor(in1, in2));
}

void increseNum(int& num, const int& min, const int& max)
{
    if (num == max)
        num = min;
    else
        ++num;
}

void decreseNum(int& num, const int& min, const int& max)
{
    if (num == min)
        num = max;
    else
        --num;
}

void display_main_menu()
{
    std::cout << "Select a logic gate [UP and DOWN arrow, select ENTER, exit ESC]:" << std::endl;
    switch (main_menu_id)
    {
    case 1:
        std::cout << "NOT\t<-" << std::endl;
        std::cout << "AND" << std::endl;
        std::cout << "NAND" << std::endl;
        std::cout << "OR" << std::endl;
        std::cout << "NOR" << std::endl;
        std::cout << "XOR" << std::endl;
        std::cout << "XNOR" << std::endl;
        break;
    case 2:
        std::cout << "NOT" << std::endl;
        std::cout << "AND\t<-" << std::endl;
        std::cout << "NAND" << std::endl;
        std::cout << "OR" << std::endl;
        std::cout << "NOR" << std::endl;
        std::cout << "XOR" << std::endl;
        std::cout << "XNOR" << std::endl;
        break;
    case 3:
        std::cout << "NOT" << std::endl;
        std::cout << "AND" << std::endl;
        std::cout << "NAND\t<-" << std::endl;
        std::cout << "OR" << std::endl;
        std::cout << "NOR" << std::endl;
        std::cout << "XOR" << std::endl;
        std::cout << "XNOR" << std::endl;
        break;
    case 4:
        std::cout << "NOT" << std::endl;
        std::cout << "AND" << std::endl;
        std::cout << "NAND" << std::endl;
        std::cout << "OR\t<-" << std::endl;
        std::cout << "NOR" << std::endl;
        std::cout << "XOR" << std::endl;
        std::cout << "XNOR" << std::endl;
        break;
    case 5:
        std::cout << "NOT" << std::endl;
        std::cout << "AND" << std::endl;
        std::cout << "NAND" << std::endl;
        std::cout << "OR" << std::endl;
        std::cout << "NOR\t<-" << std::endl;
        std::cout << "XOR" << std::endl;
        std::cout << "XNOR" << std::endl;
        break;
    case 6:
        std::cout << "NOT" << std::endl;
        std::cout << "AND" << std::endl;
        std::cout << "NAND" << std::endl;
        std::cout << "OR" << std::endl;
        std::cout << "NOR" << std::endl;
        std::cout << "XOR\t<-" << std::endl;
        std::cout << "XNOR" << std::endl;
        break;
    case 7:
        std::cout << "NOT" << std::endl;
        std::cout << "AND" << std::endl;
        std::cout << "NAND" << std::endl;
        std::cout << "OR" << std::endl;
        std::cout << "NOR" << std::endl;
        std::cout << "XOR" << std::endl;
        std::cout << "XNOR\t<-" << std::endl;
        break;
    }

    cgetch = 0;
    switch (cgetch = _getch())
    {
    case KEY_UP:
        decreseNum(main_menu_id, 1, 7);
        break;
    case KEY_DOWN:
        increseNum(main_menu_id, 1, 7);
        break;
    case KEY_ENTER:
        main_menu = false;
        break;
    case KEY_ESC:
        exit(EXIT_SUCCESS);
        break;
    default:
        break;
    }
}

void display_inputs()
{
    std::cout << "You choosed " + logic_gates[main_menu_id - 1] << std::endl;
    std::cout << "Change inputs [UP and DOWN arrow, switch input ENTER, back ESC]:" << std::endl;
    std::cout << "Inputs:" << std::endl;
    if (main_menu_id == 1) 
    {
        switch (input_id)
        {
        case 1:
            std::cout << (inputs[0] ? "1" : "0") << "\t<-" << std::endl;
            std::cout << "Simulate" << std::endl;
            break;
        case 2:
            std::cout << (inputs[0] ? "1" : "0") << std::endl;
            std::cout << "Simulate" << "\t<-" << std::endl;
        }
        
    }
    else 
    {
        switch (input_id) 
        {
        case 1:
            std::cout << (inputs[0] ? "1" : "0") << "\t<-" << std::endl;
            std::cout << (inputs[1] ? "1" : "0") << std::endl;
            std::cout << "Simulate" << std::endl;
            break;
        case 2:
            std::cout << (inputs[0] ? "1" : "0") << std::endl;
            std::cout << (inputs[1] ? "1" : "0") << "\t<-" << std::endl;
            std::cout << "Simulate" << std::endl;
            break;
        case 3:
            std::cout << (inputs[0] ? "1" : "0") << std::endl;
            std::cout << (inputs[1] ? "1" : "0") << std::endl;
            std::cout << "Simulate" << "\t<-" << std::endl;
            break;
        }
    }

    cgetch = 0;
    switch (cgetch = _getch())
    {
    case KEY_UP:
        decreseNum(input_id, 1, (main_menu_id == 1 ? 2 : 3));
        break;
    case KEY_DOWN:
        increseNum(input_id, 1, (main_menu_id == 1 ? 2 : 3));
        break;
    case KEY_ENTER:
        if (main_menu_id == 1)
        {
            if (input_id == 1)
                inputs[0] = !inputs[0];
            else
                simulation = true;
        }
        else
        {
            if (input_id == 3)
                simulation = true;
            else
                inputs[input_id - 1] = !inputs[input_id - 1];
        }
        break;
    case KEY_ESC:
        main_menu = true;
        break;
    default:
        break;
    }
}

void simulate()
{
    system("cls");
    std::cout << "Simulation " << logic_gates[main_menu_id - 1] << ":" << std::endl << std::endl;
    std::cout << "Inputs\t\t" << "Output" << std::endl;
    std::cout << inputs[0] << "\t\t";

    bool output;
    switch (main_menu_id)
    {
    case 1:
        output = Not(inputs[0]);
        break;
    case 2:
        output = And(inputs[0], inputs[1]);
        break;
    case 3:
        output = Nand(inputs[0], inputs[1]);
        break;
    case 4:
        output = Or(inputs[0], inputs[1]);
        break;
    case 5:
        output = Nor(inputs[0], inputs[1]);
        break;
    case 6:
        output = Xor(inputs[0], inputs[1]);
        break;
    case 7:
        output = Xnor(inputs[0], inputs[1]);
        break;
    default:
        break;
    }
    std::cout << (output ? "1" : "0") << std::endl;

    if (main_menu_id != 1)
        std::cout << inputs[1] << std::endl << std::endl;
    std::cout << "Press any key to continue" << std::endl;
    cgetch = _getch();
    main_menu = true;
    simulation = false;
}

auto main() -> int
{
	while (true)
	{
        system("cls");
        if (main_menu)
            display_main_menu();
        else
            display_inputs(); 

        if (simulation)
            simulate();
	}
}
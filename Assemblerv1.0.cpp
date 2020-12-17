/*
* string programmer = {"Arslan Iftikhar"};
* string file_name = {"Assemblerv1.0.cpp"};
* Class Date starting_date(12,7,2020);
* This is one parse Assembler which means we are going to ignore variables and other symbols in this version..
* A journalist asked a programmer: What makes code bad?
* Programmer: No comments.
* *********************************************USAGE***********************************************************
* Usage: ./Assemblerv1.0 <filename.asm>
* output will be machine.o..
* Usage: ./Assemblerv1.0 <filename.asm> -o filename.o..
* output will be machine.o..
*/
#include<iostream>
#include<string>
#include<vector>
#include<algorithm> // Used for reversing string..
#include<utility>
#include<fstream>
#define INSTRUCTION_SIZE 16
using namespace std;

//Functions Prototypes..
int chartoint(char ch); // Converts char to int..
int stringtoint(string str); // Converts the string to int..
string convert_binary(int number);// Converts the int to binary and return string..
string instruction_a_handler(string instruction);//Converts the A Assembly instruction into machine code and return it back as string..
string instruction_c_handler(string instruction);//Converts the C Assembly instruction into machine code and return it back as string..
void symbols_dest(string &dest);// Converts the Destionation part into machine code..
void symbols_comp(string &comp);//Converts the Computation part into machine code..
void symbols_jmp(string &jmp);//Converts the Jmp instruction into machine code..
string handling_comment(string instruction);//It will remove the comment..
void clean_command(string &instruction);//Clean the command when read from the file later this function will be used to for detecting errors in Assemblerv2.0..


int main(int argc, char *argv[])//Main function I know it's Dumb but i am adding it mark it ..
{
    string command_a = "";
    string temp, file_in, file_out;
    string default_out = "machine.hack";
    if(argc == 1)
    {
        cout<<"Error: No input file specified.\n";
        return 0;
    }
    if(argc == 4)
    {
        temp = argv[2];
        file_in = argv[1];
        file_out = argv[3];
        if(file_in == file_out)
        {
            cout<<"Error: File in and out cannot be the same.\n";
            return 0;
        }
    }
    if(argc == 4 && temp == "-o")
    {
        default_out = argv[3];
    }
    
    else if(argc == 4 && temp != "-o")
    {
        cout<<"Error: Wrong Argument " << argv[2] <<" is unknown\n";
        return 0;
    }
    temp = "";
    ifstream file(argv[1]);
    ofstream kfile(default_out);
    while(getline(file, command_a))
  {
        clean_command(command_a);
        if(command_a[0] == '/' || command_a == "")
        {
            continue;
        }
        else if(command_a[0] == '@')
        {
            temp = instruction_a_handler(command_a);
            kfile<<temp<<'\n';
        }
        else 
        {
            temp = instruction_c_handler(command_a);
            kfile<<temp<<'\n';
        }
    }
    if(argc >= 2)
        cout<<"\nAssembler has performed its task successfully.\n";
        kfile.close();
        file.close();
    return 0;
}

//Functions Definitions..

void clean_command(string &instruction)
{
    string new_command = "";
    int idx = 0;
    for(idx = 0; idx < instruction.size(); idx++)
    {
        if(instruction[idx] == '@' || (instruction[idx] >= 'A' && instruction[idx] <= 'Z') || instruction[idx] == '=' || instruction[idx] == ';' || (instruction[idx] >= '0' && instruction[idx] <= '9') || instruction[idx] == '+' || instruction[idx] == '-' || instruction[idx] == '/')
        new_command += instruction[idx];
    }
    instruction = new_command;
}

void symbols_dest(string &dest) //This Functions will replace the destination symbol with machine code..
{
    #define DEST 7
    vector<pair<string,string>> destination_map; //Using Vector to increase flexibilty..
    string destination[DEST] = {"M", "D", "MD", "A", "AM", "AD", "AMD"};//There are 8 different possibilities but we used seven since our fall back value is the 8 possibility..
    string machine_code[DEST] = {"001", "010", "011", "100", "101", "110", "111"};
    
    for(int idx = 0; idx < DEST; idx++)
    {
        destination_map.push_back(make_pair(destination[idx], machine_code[idx]));
    }

    #if defined(DEBUG_DEEP)
        cout<<'\n';
        for(int idx = 0; idx < DEST; idx++)
        {
            cout<<destination_map[idx].first<<' ';
            cout<<destination_map[idx].second<<'\n';
        }
    #endif
    for(int idx = 0; idx < DEST; idx++)
    {
        if(destination_map[idx].first == dest)
        {
            dest = destination_map[idx].second;
        }
    }
}

void symbols_comp(string &comp)//This Functions will replace the comp symbol with machine code..
{
    #define COMP_A 18
    #define COMP_M 10
    vector<pair<string,string>> computation_map_a; //Using Vector to increase flexibilty. This is for computation_map_a..
    vector<pair<string,string>> computation_map_m; //Using Vector to increase flexibilty. This is for computation_map_m..
    string computation_a[COMP_A] = {"0", "1", "-1", "D", "A", "!D", "!A", "-D", "-A", "D+1", "A+1", "D-1", "A-1", "D+A", "D-A", "A-D", "D&A", "D|A"};
    string machine_code_a[COMP_A] = {"101010", "111111", "111010", "001100", "110000", "001101", "110001", "001111", "110011", "011111", "110111", "001110", "110010", "000010", "010011", "000111", "000000", "010101"};
    string computation_m[COMP_M] = {"M", "!M", "-M", "M+1", "M-1", "D+M", "D-M", "M-D", "D&M", "D|M"};
    string machine_code_m[COMP_M] = {"110000", "110001", "110011", "110111", "110010", "000010", "010011", "000111", "000000", "010101"};
    bool flag = false;
    
    for(int idx = 0; idx < COMP_A; idx++)
    {
        computation_map_a.push_back(make_pair(computation_a[idx], machine_code_a[idx]));
    }

    for(int idx = 0; idx < COMP_M; idx++)
    {
        computation_map_m.push_back(make_pair(computation_m[idx], machine_code_m[idx]));
    }

    #if defined(DEBUG_DEEP)
        cout<<'\n';
       for(int idx = 0; idx < COMP_A; idx++)
        {
            cout<<computation_map_a[idx].first<<' ';
            cout<<computation_map_a[idx].second<<'\n';
        }
        cout<<'\n';
        for(int idx = 0; idx < COMP_M; idx++)
        {
            cout<<computation_map_m[idx].first<<' ';
            cout<<computation_map_m[idx].second<<'\n';
        }
    #endif
    for(char ch : comp)
    {
        if(ch == 'M')
            flag = true;
    }
    if(flag == false)
        for(int idx = 0; idx < COMP_A; idx++)
        {
            if(computation_map_a[idx].first == comp)
            {
                comp = computation_map_a[idx].second;
            }
        }
    else
        for(int idx = 0; idx < COMP_M; idx++)
        {
            if(computation_map_m[idx].first == comp)
            {
                comp = computation_map_m[idx].second;
            }
        }
    if (flag)
        comp = "1" + comp;
    else
        comp = "0" + comp;
    
}

void symbols_jmp(string &jmp)//This Functions will replace the Jump symbol with machine code..
{
    #define JMP 7
    vector<pair<string,string>> jump_map; //Using Vector to increase flexibilty..
    string jump[JMP] = {"JGT", "JEQ", "JGE", "JLT", "JNE", "JLE", "JMP"};//There are 8 different possibilities but we used seven since our fall back value is the 8 possibility..
    string machine_code[JMP] = {"001", "010", "011", "100", "101", "110", "111"};
    
    for(int idx = 0; idx < JMP; idx++)
    {
        jump_map.push_back(make_pair(jump[idx], machine_code[idx]));
    }

    #if defined(DEBUG_DEEP)
        cout<<'\n';
        for(int idx = 0; idx < JMP; idx++)
        {
            cout<<jump_map[idx].first<<' ';
            cout<<jump_map[idx].second<<'\n';
        }
    #endif
    for(int idx = 0; idx < JMP; idx++)
    {
        if(jump_map[idx].first == jmp)
        {
            jmp = jump_map[idx].second;
        }
    }
}

int chartoint(char ch) // coverts char to int..
{
    char cake = ch; //Don't Judge the Variable name i just love choclate cheese cake..
    int temp_int = int(ch) - 48; //0 is 48 so get ascii of character so subtract the 48 you will get the int..
    return temp_int;
}

int stringtoint(string str) // converts string to int..
{
    int temp_int{0} /*total to be return*/ , idx, place_value = 1 ;/*setting the place value should be unit at start*/ 
    int str_size = str.size(); //how many times loop has to be run..
    for(idx = str_size - 1; idx >= 0; idx--)
    {
        temp_int = temp_int + (chartoint(str[idx]) * place_value);
        place_value = place_value * 10;
    }
    return temp_int;
}

string convert_binary(int number) //convert number to binary and return the string..
{
    int remainder;
    string binary = "";
    if (number == 0)
    {
        binary = "0";
        return binary;
    }
    while(number != 1)
    {
        remainder = number % 2;
        number /= 2;
        binary += to_string(remainder);
    }
    binary += '1';
    reverse(binary.begin(), binary.end());

    return binary;
}

string handling_comment(string instruction) //This Functions will remove the comments from the instructions..
{
    int idx = 0;
    string new_instruction = "";
    int count = instruction.size();
    bool flag = false;
    for(char ch:instruction)
    {
        if(ch == '/')
        {
            flag = true;
            break;
        }
    }
    if(flag)
    {
        while(instruction[idx] != '/')
        {
            new_instruction += instruction[idx];
            idx++;
         
        }
        return new_instruction;
    }
    return instruction;
}

string instruction_a_handler(string instruction) //This function convert instruction A to into machine code and return it back..
{
    //Getting the part which has to be converted..
    string divide = "";
    string new_instruction = "";
    int idx, kdx; //Counter Variables..
    string machine_code = "0000000000000000";
    new_instruction = handling_comment(instruction); //Removing Comments if there is any..
    
    //Getting part to be converted..
    int count = new_instruction.size();
    for(int idx = 1; idx < count; idx++)
    {
        divide += new_instruction[idx];
    }
    int number = stringtoint(divide);
    divide = convert_binary(number);
    count = divide.size();
    count -= 1;
    
    for(idx = count, kdx = 15; idx >=0; idx--, kdx--)
    {
      machine_code[kdx] = divide[idx];
    }

    #if defined(DEBUG)
        cout<<"\nMachine code: "<<machine_code<<'\n';
    #endif
    return machine_code;

}

string instruction_c_handler(string instruction) //This function will convert the instruction c into machine code and return it back..
{
    int idx, kdx; // Counter Variables..
    string dest = "000"; //For storing the Destionation it is set to 000 by default which means there is no Destionation...
    string comp = "0000000";//For storing the the Computation Part of the instruction...
    string jmp = "000"; //For storing the Jump Part of the instruction which is set to 000 by default which means there is no Jump...
    string new_instruction = ""; //Will Gonna hold the instruction after removing comments if any... 
    int pointer {0}; //This flag is used to set where the loop find the seprator..
    char ch; //Gonna work as temp char..
    bool jmp_flag = false; // For checking the existence of JMP..
    bool dest_flag = false; //For checking the existence of destination..
    string machine_code = ""; // THis is the machine code which will be used in the end..

    //Removing comments if any...
    new_instruction = handling_comment(instruction);
    #if defined(DEBUG)
        cout<<"\nRemoving: "<<new_instruction;
    #endif
    //Dividing Destionation, computation and Jump..
    for(char ch: new_instruction) //Checking if there is JUMP or not..
    {
        if(ch == ';')
            jmp_flag = true;
    }

    //Getting the Destionation..
    for(idx = 0; idx < new_instruction.size(); idx++)
    {
        if(new_instruction[idx] == '=') //If there exist = in the instruction then there is Destionation
        {
            dest = "";
            for(kdx = idx ; kdx >=0; kdx--)
            {
                if(new_instruction[kdx] >= 'A' && new_instruction[kdx] <= 'Z')
                {
                    dest = dest +new_instruction[kdx];
                    dest_flag = true;
                    pointer = idx;
                }
            }
            reverse(dest.begin(), dest.end());
            break;
        }
    }

    //Getting the Computation..
    if(jmp_flag)
    {
        if(dest_flag)
            idx = pointer + 1;
        else
            idx = pointer;
        comp = "";
        while (new_instruction[idx] != ';') //If we have the jmp or semicolon then it will be handled differently..
        {
            if(new_instruction[idx] != ' ')
                comp += new_instruction[idx];
            idx++;
        }
    }
    else//if there is no jmp and no semicolon then it will be handled differently..
    {
        if(dest_flag)   
            idx = pointer + 1;
        else
            idx = pointer;
        comp ="";
        for(; idx < new_instruction.size(); idx++)
        {
            if(new_instruction[idx] != ' ')
                comp += new_instruction[idx];
        }    
    }
    
    //Getting the Jump..
    if(jmp_flag)
    {
        jmp = "";
        idx++;
        for(; idx < new_instruction.size(); idx++)
        {
            if(new_instruction[idx] >='A' && new_instruction[idx] <='Z')
            jmp += new_instruction[idx];
        }
    }
    if(jmp == "")
        jmp = "000";
    //Getting the jmp part..
    #if defined(DEBUG)
        cout<<"\nDestionation: "<<dest;
        cout<<"\nComputation: "<<comp;
        cout<<"\nJUMP: "<<jmp;
    #endif
    symbols_dest(dest);
    symbols_comp(comp);
    symbols_jmp(jmp);
    machine_code = "111" + comp + dest + jmp;
    #if defined(DEBUG)
        cout<<"\nMachine Code:";
        cout<<"\nDestionation: "<<dest;
        cout<<"\nComputation :"<<comp;
        cout<<"\nJump: "<<jmp;
        cout<<"\nMachine: "<<machine_code;
    #endif
    return machine_code;
}
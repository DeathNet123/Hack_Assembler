/*
* string programmer = {"Arslan Iftikhar"};
* string file_name = {"Assemblerv2.1.cpp"};
* Class Date starting_date(12,17,2020);
* This is Two Parse Assembler it can handle symbol and variables and will handle errors..
* A journalist asked a programmer: What makes code bad?
* Programmer: No comments.
* *********************************************USAGE***********************************************************
* Usage: ./Assemblerv2.1 <filename.asm>
* output will be machine.hack..
* Usage: ./Assemblerv2.1 <filename.asm> -o filename.hack ..
* output will be filename.hack..
*/
#include<iostream>
#include<string>
#include<vector>
#include<regex> // I knew about this library but i didn't used it but now we are going to speed thing up..
#include<algorithm> // Used for reversing string..
#include<utility>
#include<fstream>
#define INSTRUCTION_SIZE 16
using namespace std;

//Global variables..
vector<pair<string,string>> symbols_map; //THis map is Global because it is going to be used by multiple functions..
bool error_flag = false;
int number_errors = 0;
int label_count = 0;
string command = "";
int lines {0}; // This is going to count the total..

                                                                    //Functions Prototypes..
/**********************************************************************************************************************************************************************/
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
void symbol_adder(string get_cake, int pointer);//will Add sybmols to add symbol table..
void handle_variable(string handle_variable, int pointers, int &variables_pointer);
bool look_in_symbols(string command_a);
void handle_refrence(string &command_a, int pointers);
void parse_a(string &command_a);
bool check_syntax_refrence(string &command_a, bool k); //Where K is Unknow Character..

                                                                    //Main function
/*****************************************************************************************************************************************************************/
int main(int argc, char *argv[])//Main function I know it's Dumb but i am adding it to mark it ..
{        
    string command_a = "";
    string temp, file_in, file_out;
    string default_out = "machine.hack";
    int variables_pointer = 16;
    int pointers = -1;//THis is the pointer which will tell us which line is being executed..
    symbol_adder(command_a, pointers);
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
    //First Parse..
    while (getline(file, command_a))
    {
        pointers++;
        lines++;
        clean_command(command_a);
        command= command_a;
        if(command_a[0] == '/' || command_a == "")//Making test again..
        {
            pointers--;
            continue;
        }
        if(command_a[0] == '(' && command_a[command_a.size()-1] == ')')
        {
            if(!(check_syntax_refrence(command_a, true)))
            {
                pointers--;
                label_count++;
                handle_refrence(command_a, pointers);
            }
        }
    }

    //Second Parse..
    file.clear();
    file.seekg(0, ios::beg);
    pointers = 0;
    lines = 0;
    while(getline(file, command_a))
    {
        pointers++;
        lines++;
        clean_command(command_a);
        command= command_a;
        if(command_a[0] == '/' || command_a == "")//Making test again..
        {
            pointers--;
            continue;
        }
        if(command_a[0] == '@')
        {
            command_a = handling_comment(command_a);
            if(!(check_syntax_refrence(command_a, false)))
                handle_variable(command_a, pointers, variables_pointer);  
        }
    }
    
    //Third Parse..
    file.clear();
    file.seekg(0, ios::beg);
    pointers = 0;
    lines = 0;
    ofstream tfile("TEMP.asm");
    {
        while(getline(file, command_a))
        {
            lines++;
            clean_command(command_a);
            command= command_a;
            if((command_a[0] == '/' && command_a[1] == '/')|| command_a == "")
                continue;
            else if(command_a[0] == '/' && command_a[1] != '/')
            {
                number_errors++;
                cout<<"SYNTAX ERROR: Unknow statement maybe you missed / at line "<<lines<<'\n';
                cout<<"\t\t\t"<<command_a<<'\n';
                error_flag = true;
            }
            else if(command_a[0] == '(')
                continue;
            else if(command_a[0] == '@')
            {  
                parse_a(command_a);
            }
            tfile << command_a<<'\n';
        }
    }
   tfile.close();
   file.close();
   ofstream kfile(default_out);
   lines = 0;
   file.open("TEMP.asm");
    while(getline(file, command_a))
  {
        lines++;
        clean_command(command_a);
        command= command_a;
        if(command_a[0] == '/' || command_a == "")//Making test again..
        {
            continue;
        }
        else if(command_a[0] == '@')
        {
            if(error_flag)
                continue;
            temp = instruction_a_handler(command_a);
            kfile<<temp<<'\n';
        }
        else
        {
            temp = instruction_c_handler(command_a);
            kfile<<temp<<'\n';
        }
    }
   
    if(error_flag)
    {
       cout<<"ERROR: Assembler failed to complete the Task\n";
       cout<<"********************STATUS*********************\n";
       cout<<"ERROR :"<<number_errors<<'\n';
       cout<<"LINES: "<<lines<<'\n';
       return 0;
    }
    cout<<"\nAssembler has performed its task successfully.\n";
    cout<<"********************STATUS*********************\n";
    cout<<"ERROR :"<<number_errors<<'\n';
    cout<<"LINES: "<<lines + label_count<<'\n';
    file.close();
    kfile.close();
    tfile.close();
    return 0;
}

                                                                        //Functions Definitions..
/***********************************************************************************************************************************************************************/
bool check_syntax_refrence(string &command_a, bool k)
{
    if(k)
    {
        if(command_a[k] >= '0' && command_a[k] <= '9')
        {
            cout<<"SYNTAX ERROR: The Label Name Cannot Start from the Integer at line "<<lines<<'\n';
            cout<<"\t\t"<<command_a<<'\n';
            error_flag = true;
            number_errors++;
        }
    }
    else if(command_a[1] >= '0' && command_a[1] <= '9')
    {
        bool k_flag = false;
        int counter = 0;
        for(int idx = 0; idx < command_a.size(); idx++)
        {
            if(command_a[idx] >= '0' && command_a[idx] <= '9')
            {
                counter++;
            }
        }
        if(counter == command_a.size() - 1)
            return true; // This falacy here because we are not increasing the error because there is no error but we need to shut function down..
        cout<<"SYNTAX ERROR: The Label Name Cannot Start from the Integer at line "<<lines<<'\n';
        cout<<"\t\t"<<command_a<<'\n';
        error_flag = true;
        number_errors++;
    }
    for(int idx = 1; idx < command_a.size() - k; idx++)
    {
        if(!((command_a[idx] >= 'A' && command_a[idx] <= 'Z') || (command_a[idx] >= 'a' && command_a[idx] <= 'z') || command_a[idx] == '$' || command_a[idx] == ':' || command_a[idx] == '_' || command_a[idx] == '.' ||(command_a[idx] >= '0' && command_a[idx] <='9')))
        {
            cout<<"SYNTAX ERROR: Illegal Character "<<command_a[idx]<<" in "<<command_a<<" at line "<<lines<<'\n';
            cout<<"\t\t"<<command_a<<'\n';
            number_errors++;
            error_flag = true;
            return true;
        }
    }
    return false;
}
void handle_refrence(string & command_a, int pointers)
{
    bool flag = false;
    flag = look_in_symbols(command_a);
    if(!flag)
        symbol_adder(command_a, pointers + 1);
    else 
    {
        error_flag = true;
        cout<<"ERROR: Redefinition of LABEL at line "<<lines<<'\n';
        cout<<"The LABEL "<<command_a<<" is already declared\n";
        cout<<"\t\t"<<command_a<<'\n';
        number_errors++;
    } 
}

void parse_a(string &command_a)
{
    string new_command = "";
    for(int idx = 1; idx < command_a.size(); idx++)
    {
        new_command += command_a[idx];
    }

    for(int idx = 0; idx < symbols_map.size(); idx++)
    {
        if(symbols_map[idx].first == new_command || symbols_map[idx].first == '(' + new_command + ')')
        {
            command_a = '@'+ symbols_map[idx].second;
        }
    }
}

bool look_in_symbols(string command_a)
{
    for(int idx = 0; idx < symbols_map.size(); idx++)
    {
        if(symbols_map[idx].first == command_a)
            return true;
    }
    return false;
}

void handle_variable(string handle_variable, int pointers, int &variables_pointer)
{
    string new_command = "";
    bool flag_refrence = false;
    bool flag_variable = false;
    for (int idx = 1; idx < handle_variable.size(); idx++)
    {
        new_command += handle_variable[idx];
    }
    handle_variable = new_command;
    new_command = '(' + new_command + ')';
    flag_refrence = look_in_symbols(new_command);
    flag_variable = look_in_symbols(handle_variable);

    if(!flag_variable && !flag_refrence)
    {
        symbol_adder(handle_variable, variables_pointer++);
    }
}

void symbol_adder(string get_cake, int pointer)//This function will add symbol to the symbol table..
{
    #define DEF_SYMBOL_LENGTH 23
    string default_value[DEF_SYMBOL_LENGTH] = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15", "16384", "24576", "0", "1", "2", "3", "4"};
    string default_symbol[DEF_SYMBOL_LENGTH] = {"R0", "R1", "R2", "R3", "R4", "R5", "R6", "R7", "R8", "R9", "R10", "R11", "R12", "R13", "R14", "R15", "SCREEN", "KBD", "SP", "LCL", "ARG", "THIS", "THAT"};
    if(get_cake == "" && pointer == -1)
    {
        for(int idx = 0; idx < DEF_SYMBOL_LENGTH; idx++)
        {
            symbols_map.push_back(make_pair(default_symbol[idx], default_value[idx]));
        }
    }
    else if(get_cake != "")
    {
        symbols_map.push_back(make_pair(get_cake, to_string(pointer)));
    }
    #if defined(PRINT_SYMBOL)
        for(int idx = 0; idx < symbols_map.size(); idx++)
        {
            cout<<symbols_map[idx].first<<' ';
            cout<<symbols_map[idx].second<<'\n';
        }
    #endif
}

void clean_command(string &instruction)//This function is going to be used to remove the extra spaces when instructions are read from the file..
{
    string new_command = "";
    int idx = 0;
    for(idx = 0; idx < instruction.size(); idx++)
    {
        if((instruction[idx] >= '!' && instruction[idx] <= 'Z') || (instruction[idx] >= 'a' && instruction[idx] <= 'z') || instruction[idx] == '_' || instruction[idx] == '|')
        new_command += instruction[idx];
    }
    instruction = new_command;
}

void symbols_dest(string &dest) //This Functions will replace the destination symbol with machine code..
{
    #define DEST 7
    bool flag = false;
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
            flag = true;
        }
    }

    if(!flag)
    {
        cout<<"SYNTAX ERROR: Invalid Destination "<<dest<<" at line "<<lines<<'\n';
        cout<<"\t\t"<<command<<'\n';
        number_errors++;
        error_flag = true;
    }
}

void symbols_comp(string &comp)//This Functions will replace the comp symbol with machine code..
{
    #define COMP_A 18
    #define COMP_M 10
    bool flage = false;
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
                flage = true;
            }
        }
    else
        for(int idx = 0; idx < COMP_M; idx++)
        {
            if(computation_map_m[idx].first == comp)
            {
                comp = computation_map_m[idx].second;
                flage = true;
            }
        }

    if(!flage)
    {
        cout<<"SYNTAX ERROR: Invalid Computation "<<comp<<" at line "<<lines<<'\n';
        cout<<"\t\t"<<command<<'\n';
        number_errors++;
        error_flag = true;
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
    bool flag = false;
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
            flag = true;
        }
    }

    if(!flag)
    {
        cout<<"SYNTAX ERROR: Invalid JUMP operation "<<jmp<<" at line "<<lines<<'\n';
        cout<<"\t\t"<<command<<'\n';
        number_errors++;
        error_flag = true;
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
    string new_instruction = "";
    int count = instruction.size();
    bool flag = false;
    int idx {0};
    for(idx = 0; idx < count; idx++)
    {
        if(instruction[idx] == '/' && instruction[idx + 1] == '/')
        {
            flag = true;
            break;
        }
    }

    if(flag)
    {
        for(int kdx = 0; kdx < idx; kdx++)
        {
            new_instruction += instruction[kdx];
         
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
    for(int idx = 1; idx < count; idx++) //Loop started from the 1 because it will skip @..
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
    if(dest != "000")
    symbols_dest(dest);
    if(comp != "0000000")
    symbols_comp(comp);
    if(jmp != "000")
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
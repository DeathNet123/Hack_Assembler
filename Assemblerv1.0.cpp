/*
*   string programmer = {"Arslan Iftikhar"};
*   string file_name = {"Assemblerv1.0.cpp"};
*   Class Date starting_date(12,7,2020);
* This is one parse Assembler which means we are going to ignore variables and other symbols in this version..
* A journalist asked a programmer: What makes code bad?
* Programmer: No comments.
*/
#include<iostream>
#include<string>
#include<vector>
#include<algorithm> // Used for reversing string..
#include<utility>
#include<fstream>
#define INSTRUCTION_SIZE 16
#define DEBUG
using namespace std;

//Functions Prototypes..
int chartoint(char ch); // Converts char to int..
int stringtoint(string str); // Converts the string to int..
string convert_binary(int number);
void instruction_a_handler(string instruction);
void instruction_c_handler(string instruction);
void symbols_dest(string &dest);
void symbols_comp(string &comp);
void symbols_jmp(string &jmp);
string handling_comment(string instruction);

int main(void)
{
    #if defined(DEBUG)
        string command_a;
        getline(cin, command_a);
        if(command_a[0] == '/')
        {
            cout << "Done";
        }
        else if(command_a[0] == '@')
        {
            instruction_a_handler(command_a);
        }
        else 
        {
            instruction_c_handler(command_a);
        }
    #endif
    return 0;
}

//Functions Definitions..

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

    #if defined(DEBUG)
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

}

void symbols_jmp(string &jmp)//This Functions will replace the Jump symbol with machine code..
{

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

string handling_comment(string instruction)
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

void instruction_a_handler(string instruction) //This function convert instruction A to into machine code and write it to file..
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


}

void instruction_c_handler(string instruction)
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
        cout<<new_instruction;
    #endif
    //Dividing Destionation, computation and Jump..
    for(char ch: new_instruction) //Checking if there is JUMP or not..
    {
        if(ch == ';')
            flag = true;
    }

    //Getting the Destionation..
    for(idx = 0; idx < new_instruction.size(); idx++)
    {
        if(new_instruction[idx] == '=') //If there exist = in the instruction then there is Destionation
        {
            for(kdx = idx ; kdx >=0; kdx--)
            {
                if(new_instruction[kdx] >= 'A' && new_instruction[kdx] <= 'Z')
                {
                    dest = new_instruction[kdx];
                    dest_flag = true;
                    pointer = idx;
                    break;
                }
            }
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
    #if defined(DEBUG)
        cout<<"\nMachine Code:";
        cout<<"Destionation: "<<dest;
    #endif
}
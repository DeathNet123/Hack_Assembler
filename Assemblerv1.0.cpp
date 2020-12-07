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
            cout<<"\nIts me the A\n";
            instruction_a_handler(command_a);
        }
        else 
        {
            cout<<"\nIts me the C\n";
            instruction_c_handler(command_a);
        }
    #endif
    return 0;
}

//Functions Definitions..

int chartoint(char ch) // coverts char to int..
{
    char cake = ch; //Don't Judge the Variable name i just love choclate cheese cake..
    int temp_int = int(ch) - 48; //0 is 48 so get ascii of character so subtract the 48 you will get the int..
    return temp_int;
}

int stringtoint(string str) // converts string to int..
{
    int temp_int = 0 /*total to be return*/ , idx, place_value = 1 ;/*setting the place value should be unit at start*/ 
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

void instruction_a_handler(string instruction) //This function convert instruction A to into machine code and write it to file..
{
    //Getting the part which has to be converted..
    cout<<"I am here.";
    string divide = "";
    string new_instruction;
    int idx, kdx; //Counter Variables..
    string machine_code = "0000000000000000";
    new_instruction = handling_comment(instruction); //Removing Comments if there is any..
    
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

string handling_comment(string instruction)
{
    int idx = 0;
    string new_instruction = "";
    while(instruction[idx] != '/')
    {
        new_instruction += instruction[idx];
        idx++; 
    }
    return new_instruction;
}
/*
void instruction_c_handler(string instruction)
{
    string dest = "000"; //For storing the Destionation it is set to 000 by default which means there is no Destionation...
    string comp = "0000000";//For storing the the Computation Part of the instruction...
    string jmp = "000"; //For storing the Jump Part of the instruction which is set to 000 by default which means there is no Jump...
    string new_instruction = ""; //Will Gonna hold the instruction after removing comments if any... 
    int flags; //This flag is used to set where the loop find the seprator..
    char ch; //Gonna work as temp char..

    //Removing comments if any...
    new_instruction = handling_comment(instruction);
    cout<<new_instruction;
    //Dividing Destionation, computation and Jump..
    for(int idx = 0; idx < new_instruction.size(); idx++)
    {
        if(new_instruction[idx] == '=')
        {
            for(int kdx = idx ; kdx >=0; kdx--)
            {
                if(new_instruction[kdx] >= 'A' && new_instruction[kdx] <= 'Z')
                {
                    dest = new_instruction[kdx];
                    break;
                }

            }
            break;
        }
    }
    #if defined(DEBUG)
        cout<<"Destionation: "<<dest;
    #endif
}*/
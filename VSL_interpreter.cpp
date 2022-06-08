#include <string>
#include <iostream>
using namespace std;

#define VARIABLE_DIM 4
#define INSTRUCTION_DIM 1024

bool *memory = new bool[VARIABLE_DIM] ();                                       //variables memory
bool *temp = new bool(0);                                                       //temp variable used by current instruction

int *condition_index = new int(0);                                              //temp index for if condirtion
int *temp_index = new int(0);                                                   //temp index used by current instruction
int *temp_index_op1 = new int(0);                                               //temp index used by current instruction as first operand
int *temp_index_op2 = new int(0);                                               //temp index used by current instruction as second operand

int *ins_token = new int(-1);                                                    /*a value that represent the instruction to execute
                                                                                -1 is no instruction, with a correct instruction
                                                                                the variable can assume a range of values from 0 to 4*/

string *instruction = new string[INSTRUCTION_DIM] ();                           //instruction memory
//string* PC = instruction;                                                     //program counter

//interpreter
bool check(string instr) {
    string ins = instr;
    if(ins.substr(0, 2) != "m[") {
            cerr << "Invalid syntax!" << endl;
            *ins_token = -1;
            return false;
        }

        if(ins.substr(3, 5) != "]" && ins.substr(3, 5) != "] <- ") {
            cerr << "Invalid syntax!" << endl;
            *ins_token = -1;
            return false;
        }

        if(ins[2] < 48 || ins[2] > 51) {
            cerr << "Error! Expected a value from 0 to 3." << endl;
            *ins_token = -1;
            return false;
        }

        sscanf((ins.substr(2, 1)).c_str(), "%d", temp_index);                          //assigning the temp index of the left value

        string operation_ = ins;
        operation_ = operation_.erase(0, 8);                                           //substring that contains only the operations to execute


        switch(operation_.length()) {
            case 0:
                *ins_token = 0;
                break;
            case 1:
                if(operation_[0] != '0' && operation_[0] != '1') {
                    cerr << "Error! Expected a boolean value (0 or 1).\n";
                    cout << operation_;
                    *ins_token = -1;
                    return false;
                }

                *ins_token = 1;                                                        //operation token with value 1
                sscanf(operation_.c_str(), "%d", temp);                                //assigning the temp value of the operation
                break;
            case 4:
                if(operation_.substr(0, 2) != "m[" && operation_[3] != ']') {
                    cerr << "Invalid syntax!" << endl;
                    *ins_token = -1;
                    return false;
                }
                if(operation_[2] < 48 || operation_[2] > 51) {
                    cerr << "Error! Expected a value from 0 to 3." << endl;
                    *ins_token = -1;
                    return false;
                }

                *ins_token = 2;                                                        //operation token with value 2
                sscanf((operation_.substr(2, 1)).c_str(), "%d", temp_index_op1);       //assigning the temp index of the first right operand
                break;
            case 10:
                if(operation_.substr(0, 2) != "m[" && operation_.substr(3, 6) != "]ORm[" && operation_[10] != ']') {
                    cerr << "Invalid syntax!" << endl;
                    *ins_token = -1;
                    return false;
                }
                if((operation_[2] < 48 || operation_[2] > 51) || (operation_[8] < 48 || operation_[8] > 51)) {
                    cerr << "Error! Expected a value from 0 to 3." << endl;
                    *ins_token = -1;
                    return false;
                }

                *ins_token = 3;                                                         //operation token with value 3
                sscanf((operation_.substr(2, 1)).c_str(), "%d", temp_index_op1);        //assigning the temp index of the first right operand
                sscanf((operation_.substr(8, 1)).c_str(), "%d", temp_index_op2);        //assigning the temp index of the second right operand
                break;
            case 11:
                if(operation_.substr(0, 2) != "m[" && operation_.substr(3, 6) != "]ANDm[" && operation_[11] != ']') {
                    cerr << "Invalid syntax!" << endl;
                    *ins_token = -1;
                    return false;
                }
                if((operation_[2] < 48 || operation_[2] > 51) || (operation_[9] < 48 || operation_[9] > 51)) {
                    cerr << "Error! Expected a value from 0 to 3." << endl;
                    *ins_token = -1;
                    return false;
                }

                *ins_token = 4;                                                         //operation token with value 1
                sscanf((operation_.substr(2, 1)).c_str(), "%d", temp_index_op1);        //assigning the temp index of the first right operand
                sscanf((operation_.substr(9, 1)).c_str(), "%d", temp_index_op2);        //assigning the temp index of the second right operand
                break;
            default:
                cerr << "Invalid syntax!" << endl;
                *ins_token = -1;
                return false;
        }

        return true;
}
void check(int index) {
    int count = 0;
    for(int i = 0; i < instruction[index].length(); i++) {
        if(instruction[index][i] == ' ')
            count++;
        else
            break;
    }

    string ins = instruction[index];
    ins = ins.erase(0, count);
    if(ins.substr(0, 2) == "if"){
        if(ins.substr(3, 2) == "m[")
            if(ins[6] == ']')
                if(ins[5] >= 48 && ins[5] <= 51) {
                    sscanf((ins.substr(5, 1)).c_str(), "%d", condition_index);
                    if(ins.substr(8, 4) == "then") {
                        if(memory[*condition_index]) {
                            ins = ins.erase(0, 13);
                            check(ins);
                        }
                        else
                            return;
                    }
                }

    }
    else {
        check(ins);
    }
}

void execute() {
    switch(*ins_token) {
        case -1:
            return;
        case 0:
            cout << "m[" << *temp_index << "] = " <<  memory[*temp_index] << endl;
            break;
        case 1:
            memory[*temp_index] = *temp;
            cout << "m[" << *temp_index << "] = " <<  memory[*temp_index] << endl;
            break;
        case 2:
            memory[*temp_index] = memory[*temp_index_op1];
            cout << "m[" << *temp_index << "] = " <<  memory[*temp_index] << endl;
            break;
        case 3:
            memory[*temp_index] = (memory[*temp_index_op1] || memory[*temp_index_op2]);
            cout << "m[" << *temp_index << "] = " <<  memory[*temp_index] << endl;
            break;
        case 4:
            memory[*temp_index] = (memory[*temp_index_op1] && memory[*temp_index_op2]);
            cout << "m[" << *temp_index << "] = " <<  memory[*temp_index] << endl;
            break;
        default:
            cerr << "Unexpected instruction..." << endl;
    }
    //PC = PC + 1;
}

int main() {
    int index = 0;
    bool end = false;

    cout << "VSL Interpreter version a0.1 by Infrablue(Leonardo Mirabella)\n[type 'end' to finish the execution]\n\n";

    while(!end) {
    cout << ">>> ", getline(cin, instruction[index]);

        if(instruction[index] == "end") {
            end = true;
            continue;
        }

        check(index);
        execute();

        index++;
    }

    return 0;
}

/* there are included useless things like the array of instructions or the program counter. These things were added
for a more "correct" implementation of the idea of abstract machine. In future i will probably add other instructions (like loops),
so concepts like program counter and memory of instructions will get a meaning.*/
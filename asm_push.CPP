#include <iostream>
#include <fstream>
#include <string>
#include <cctype>
#include <algorithm>
#include <sstream>
#include <iomanip>
#include <set>
#include <map>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <cstdlib>
#include <regex>
#include <bitset>
#include <cmath>
#include <stdexcept>
#include <utility>
#include <exception>
#include <stdexcept>
using namespace std;

string remove_whitespace(const string &s)
{
    auto left = find_if_not(s.begin(), s.end(), ::isspace);
    auto right = find_if_not(s.rbegin(), s.rend(), ::isspace).base();
    return (left < right) ? string(left, right) : "";
}

string remove_comments(const string &line)
{
    size_t comment_start = line.find(';');
    string code_part = (comment_start != string::npos) ? line.substr(0, comment_start) : line;
    return remove_whitespace(code_part);
}
bool isAsmFile(const string &filename)
{
    string extension = ".asm";
    return (filename.size() >= extension.size() && filename.compare(filename.size() - extension.size(), extension.size(), extension) == 0);
}

bool verify_labels(string &label)
{
    if (label.empty() || !(isalpha(label[0]) || label[0] == '_'))
        return false;

    for (char ch : label)
    {
        if (!isalnum(ch) && ch != '_')
            return false;
    }
    return true;
}
bool is_valid_hex(string hexStr)
{
    if (hexStr.size() < 3)
    {
        return false;
    }
    if (hexStr.substr(0, 2) == "0x" || hexStr.substr(0, 2) == "0X")
    {
        hexStr = hexStr.substr(2);
    }
    else
        return false;
    for (char ch : hexStr)
    {
        if (!isxdigit(ch))
            return false;
    }
    return true;
}

bool valid_octal(string octStr)
{
    if (octStr.size() < 2)
    {
        return false;
    }
    if (octStr.substr(0, 1) == "0" || octStr.substr(0, 1) == "0")
    {
        octStr = octStr.substr(1);
    }
    else
        return false;
    for (char ch : octStr)
    {
        if (ch < '0' || ch > '7')
            return false;
    }
    return true;
}
bool valid_decimal(string decStr)
{
    if (decStr[0] == '0' && decStr.size() > 1)
    {
        // cout<<"w";
        return false;
    }

    if (decStr[0] == '-' || decStr[0] == '+')
    {
        decStr = decStr.substr(1);
        // cout<<"hwe";
        if (decStr[0] == '0')
            return false;
    }
    for (char ch : decStr)
    {
        if (!isdigit(ch))
        {

            return false;
        }
    }
    return true;
}
pair<bool, string> valid_number(string number)
{
    if (number.empty())
    {
        return {false, ""};
    }
    if (is_valid_hex(number))
    {
        return {true, "hex"};
    }
    if (valid_decimal(number))
    {
        return {true, "dec"};
    }
    if (valid_octal(number))
    {
        return {true, "oct"};
    }
    return {false, ""};
}

pair<int, bool> number_extract(string num)
{
    pair<bool, string> v = valid_number(num);
    if (v.first)
    {
        if (v.second == "hex")
        {
            if (num.substr(0, 2) == "0x" || num.substr(0, 2) == "0X")
            {
                num = num.substr(2);
            }
            return {stoi(num, nullptr, 16), true};
        }
        if (v.second == "dec")
        {
            int numi;
            if (num[0] == '-')
            {
                num = num.substr(1);
                numi = -stoi(num, nullptr, 10);
            }
            else if (num[0] == '+')
            {
                num = num.substr(1);
                numi = stoi(num, nullptr, 10);
            }
            else
            {
                numi = stoi(num, nullptr, 10);
            }
            return {numi, true};
        }
        if (v.second == "oct")
        {
            num = num.substr(1);
            return {stoi(num, nullptr, 8), true};
        }
    }
    return {-1, false};
}

int instruction_table(string s)
{
    std::map<std::string, int> instruction_table;
    instruction_table["data"] = 0;
    instruction_table["ldc"] = 1;
    instruction_table["adc"] = 2;
    instruction_table["ldl"] = 3;
    instruction_table["stl"] = 4;
    instruction_table["ldnl"] = 5;
    instruction_table["stnl"] = 6;
    instruction_table["add"] = 7;
    instruction_table["sub"] = 8;
    instruction_table["shl"] = 9;
    instruction_table["shr"] = 10;
    instruction_table["adj"] = 11;
    instruction_table["a2sp"] = 12;
    instruction_table["sp2a"] = 13;
    instruction_table["call"] = 14;
    instruction_table["return"] = 15;
    instruction_table["brz"] = 16;
    instruction_table["brlz"] = 17;
    instruction_table["br"] = 18;
    instruction_table["HALT"] = 19;
    if (instruction_table.find(s) != instruction_table.end())
    {
        return instruction_table[s];
    }
    return -1;
}
string int_to_hex(int a, int width = 8)
{
    if (a == -1)
        return "Error";
    stringstream ss;
    ss << uppercase << hex << setw(width) << setfill('0') << (a & 0xFF); // Mask to 8 bits
    return ss.str();
}
string bin_to_hex(const string &s)
{
    map<string, char> bin_to_hex;
    bin_to_hex["0000"] = '0';
    bin_to_hex["0001"] = '1';
    bin_to_hex["0010"] = '2';
    bin_to_hex["0011"] = '3';
    bin_to_hex["0100"] = '4';
    bin_to_hex["0101"] = '5';
    bin_to_hex["0110"] = '6';
    bin_to_hex["0111"] = '7';
    bin_to_hex["1000"] = '8';
    bin_to_hex["1001"] = '9';
    bin_to_hex["1010"] = 'A';
    bin_to_hex["1011"] = 'B';
    bin_to_hex["1100"] = 'C';
    bin_to_hex["1101"] = 'D';
    bin_to_hex["1110"] = 'E';
    bin_to_hex["1111"] = 'F';
    string hex;
    for (size_t i = 0; i < s.size(); i += 4)
    {
        string chunk = s.substr(i, 4);
        hex += bin_to_hex[chunk];
    }
    return hex;
}
int bin_to_int(const string &s)
{
    int num = 0;
    for (int i = 0; i < s.size(); i++)
    {
        num += (s[i] - '0') * pow(2, s.size() - i - 1);
    }
    return num;
}
string warning_table(int i)
{
    map<int, string> warning_table;
    warning_table[1] = "Unused label";

    return warning_table[i];
}
string error_table(int i)
{
    map<int, string> error_table;
    error_table[0] = "Invalid instruction";
    error_table[1] = "Invalid operand";
    error_table[2] = "Invalid label";
    error_table[3] = "Duplicate label";
    error_table[4] = "Invalid number";
    error_table[5] = "Extra on line";
    error_table[6] = "Not enough on line";
    return error_table[i];
}
int total_words(const string s)
{
    istringstream iss(s);
    string word;
    int count = 0;
    while (iss >> word)
    {
        count++;
    }
    return count;
}
bool require_operand(const string &token)
{
    return (token != "add" && token != "sub" && token != "shl" && token != "shr" && token != "a2sp" && token != "sp2a" && token != "return" && token != "HALT");
}
bool require_number(const string &token)
{
    return (token == "data" || token == "ldc" || token == "adc" || token == "SET" || token == "adj");
}
bool require_label(const string &token)
{
    return (token == "br" || token == "brz" || token == "brlz" || token == "call" || token == "ldl" || token == "stl" || token == "ldnl" || token == "stnl");
}
bool require_labelornumber(const string &token)
{
    return require_label(token) || require_number(token);
}

vector<vector<string>> split_lines(const string &s)
{
    vector<vector<string>> lines;
    istringstream iss(s);
    string line;
    while (getline(iss, line))
    {
        vector<string> words;
        if (line.find(':') != string::npos)
        {
            string label = line.substr(0, line.find(':'));
            words.push_back(label);
            words.push_back(":");
            line = line.substr(line.find(':') + 1);
        }
        istringstream line_stream(line);
        string word;
        while (line_stream >> word)
        {
            words.push_back(word);
        }
        lines.push_back(words);
    }
    return lines;
}

bool perform_first_pass(vector<vector<string>> tokenized_words, map<string, int> &loopup_table, vector<string> &warnings, set<string> &unresolved_labels, vector<string> &error)
{
    int program_counter = 0;
    bool halt_found = false;
    int line_no = 0;
    for (auto line : tokenized_words)
    {
        for(auto i:line)
        {
            cout<<i<<" ";
        }
        cout<<endl;
        cout<<line.size()<<endl;
        line_no++;
        if (line.empty())
        {
            continue; // Skip empty lines
        }

        if (line.size() > 4)
        {
            error.push_back(error_table(5) + " on program counter number " + to_string(line_no)); // Too many tokens in the line
            continue;
        }

        bool is_label = false;

        // Check if the line starts with a label
        if (line.size() > 1 &&line[1] == ":")
        {
            is_label = true;
            string label = line[0];

            // Validate and add label to the lookup table
            if (loopup_table.find(label) != loopup_table.end())
            {
                error.push_back(error_table(3) + " on program counter number " + to_string(line_no)); // Duplicate label
                continue;
            }
            if (!verify_labels(label))
            {
                error.push_back(error_table(2) + " on program counter number " + to_string(line_no)); // Invalid label
                continue;
            }
            loopup_table[label] = program_counter;

            // Erase the label part from the line
            if (line.size() >= 2)
            {
                line.erase(line.begin(), line.begin() + 2);
            }
        }

        // Skip if the line is now empty
        if (line.empty())
        {
            continue;
        }

        // Parse instruction
        string instruction = line[0];
        if (line.size() > 2)
        {
            error.push_back(error_table(5) + " on program counter number " + to_string(line_no));
            program_counter++; // Extra tokens in the line
            continue;
        }

        int opcode = instruction_table(instruction);
        if (opcode == -1)
        {
            error.push_back(error_table(0) + " on program counter number " + to_string(line_no));
            program_counter++; // Invalid instruction
            continue;
        }
        if (opcode == 19)
        {
            halt_found = true; // HALT instruction found
        }

        // Check for operands
        if (require_operand(instruction))
        {
            if (line.size() == 1)
            { // ldc
                error.push_back(error_table(1) + " on program counter number " + to_string(line_no));
                program_counter++; // Missing operand
                continue;
            }
            string operand = line[1];
            if (require_label(instruction))
            {
                if (loopup_table.find(operand) == loopup_table.end() && verify_labels(operand))
                {
                    unresolved_labels.insert(operand);

                    // Add unresolved label
                }
                else
                {
                    if (!verify_labels(operand))
                    {
                        error.push_back(error_table(2) + " on program counter number " + to_string(line_no));
                        program_counter++;
                        continue;
                    }
                }
            }

            else if (require_number(instruction))
            {
                pair<int, bool> num = number_extract(operand);
                if (!num.second)
                { // if not number not valid label then eroor
                    // cout<<instruction<<" "<<operand<<"here"<<endl;

                    error.push_back(error_table(1) + " on program counter number " + to_string(line_no));
                    program_counter++;
                    // Invalid number
                    continue;
                }
            }
        }
        else if (line.size() >= 2)
        {
            error.push_back(error_table(5) + " on program counter number " + to_string(line_no));
            program_counter++; // Extra tokens in the line
            continue;
        }
        if (instruction != "SET")
        {
            program_counter++;
        }
        // Increment program counter
    }

    if (!halt_found)
    {
        warnings.push_back("HALT instruction not found");
    }
    for (const auto &label : unresolved_labels)
    {
        if (loopup_table.find(label) == loopup_table.end())
        {
            error.push_back("Undefined label: " + label);
        }
    }
    return true;
}

bool perform_second_pass(ofstream &listing_file, ofstream &binary_file, vector<vector<string>> tokenized_word, map<string, int> &lookup_table, vector<string> &warnings, set<string> &unresolved_labels, vector<string> &errors)
{
    int program_counter = 0;
    // for (auto words : unresolved_labels)

    // {
    //     if (lookup_table.find(words) == lookup_table.end())
    //     {
    //         errors.push_back("Undefined label " + words);
    //         return false;
    //     }
    // }
    if (!listing_file.is_open() || !binary_file.is_open())
    {
        cerr << "Error: Unable to open output files for writing." << endl;
        return -1;
    }
    if (!errors.empty())
    {
        return false;
    }
    listing_file << "PC --- Machine Code \n";
    listing_file << "-------------------\n";
    for (auto lines : tokenized_word)
    {
        // for (auto i : lines)
        //     cout << i << " ";
        // cout << endl;
        if (lines.empty())
        {
            listing_file << int_to_hex(program_counter, 8) << " N/A " << endl;
            continue;
        }
        if (lines.size()>1){
        if (lines[1] == ":" && lines.size() == 2)
        {
            listing_file << int_to_hex(program_counter, 8) << " N/A ";

            continue;
        }
        else if (lines[1] == ":" && lines.size() > 2)
        {
            lines.erase(lines.begin(), lines.begin() + 2);
        }
        }

        string instruction = lines[0];
        int opcode = instruction_table(instruction);
        string machine_code;
        cout << "Processing line: " << program_counter << endl;
        cout << "Instruction: " << instruction << ", Opcode: " << opcode << endl;

        string operand;
        if (lines.size() == 1)
        {
            machine_code = bitset<32>(opcode).to_string();
        }
        else
        {
            operand = lines[1];
            if (require_label(instruction))
            {
                int val;
                if (instruction=="br" || instruction=="brz" || instruction=="brlz" || instruction=="call")  val=lookup_table[operand]-program_counter;
                else  val = lookup_table[operand];
                
               
                machine_code = bitset<24>(val).to_string() + bitset<8>(opcode).to_string();
            }
            else if (require_number(instruction))
            {
                pair<int, bool> num = number_extract(operand);
                machine_code = bitset<24>(num.first).to_string() + bitset<8>(opcode).to_string();
            }
            // Add verbose logging before conversion
            // cout << "Attempting to convert operand: [" << operand << "]" << endl;
            // cout << "Operand length: " << operand.length() << endl;
            // cout << "Operand content: ";
            // for (char c : operand)
            // {
            //     cout << "'" << c << "'(" << (int)c << ") ";
            // }
            // cout << endl;
            // cout << machine_code << endl;
            // machine_code = bitset<24>(stoi(operand)).to_string() + bitset<8>(opcode).to_string();
        }
        listing_file << int_to_hex(program_counter, 8) << " " << bin_to_hex(machine_code) << " ";

        listing_file << "\n";
        uint32_t binary_data = bin_to_int(machine_code);
        binary_file.write(reinterpret_cast<const char*>(&binary_data), sizeof(binary_data));
        //binary_file << machine_code << endl;
        if (instruction != "SET")
        {
            program_counter++;
        }
        if (instruction == "HALT")
        {
            //break;
            // continue;
        }
    }
    return true;
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        cerr << "Usage: " << argv[0] << " <filename>" << endl;
        return 1;
    }
    string filename = argv[1];
    if (!isAsmFile(filename))
    {
        cerr << "Error: Invalid file extension. Please provide a file with .asm extension." << endl;
        return 1;
    }
    
    filebuf fb;
    if (fb.open(filename, ios::in))
    {
        istream is(&fb);
        string s;

        for (string line; getline(is, line);)
        {
            s += remove_comments(line) + "\n";
        }

        fb.close();
        vector<string> warnings;
        vector<string> errors;
        set<string> unresolved_labels;
        vector<string> lines_without_comment;
        map<string, int> lookup_table;

        vector<vector<string>> lines = split_lines(s);
        for (auto &i : lines)
        {
            for (auto &j : i)
                j = remove_whitespace(j);
        }
        

        perform_first_pass(lines, lookup_table, warnings, unresolved_labels, errors);
        
        

        ofstream listing_out("test2.lst");
        ofstream binary_out("test2.o", ios::binary);
        try
        {
            bool success = perform_second_pass(listing_out, binary_out, lines, lookup_table, warnings, unresolved_labels, errors);
            if (!success)
            {
                cout << "Second pass failed" << endl;
            }
        }
        catch (const std::exception &e)
        {
            cerr << "Exception occurred: " << e.what() << endl;
        }
        for (auto i : errors)
        {
            cout << i << endl;
        }
    }
    cout<<"Assembly process completed\n";
    return 0;
}

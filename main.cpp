#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <algorithm>

int stack[100];

struct registerframe  {
    int fptr; //function pointer
    int isptr; //instruction pointer!
}
struct constant {
    int type;
    void * ptr;
};

std::vector<constant> constant_map;
int const_create(int type, void * ptr){
    struct constant c;
    c.ptr =ptr;
    c.type = type;
    constant_map.push_back(c);
    return constant_map.size();
}

typedef int word;
void iforward() {
    isptr++;
}
void ijumpf(int findex) {
    fptr = findex;
    isptr = 0;
}

char stackptr = 0;
void spush(int v){
    stack[++stackptr] = v;
}
int spop(){
    return stack[stackptr--];
}
struct op;
typedef std::vector<std::string> xargs;
typedef std::vector<op> xop;
typedef int (*opfunc) (int);
typedef int (*oploader) (
    int nargs, //number of args 
    xargs * xargs, //arguments
    xop * xop);
struct opdef {
    const char * key;
    int opid;
    oploader l;
};
struct op {
    //void * data;
    opfunc f;
    op(opfunc f, void * d) {
        this->f = f;
        //this->data = d;
    }
};

int registers[4];
void print_registers(){
    for (int i = 0; i < 4; i++) {
        printf("[r%d]:%d\n", i, registers[i]);
    }
}

int jmp() {}
int fnull(int) {}
int fputr(int dest) {
    registers[dest] = spop();
}
int fadd(int) {
    //spush(1);
}
int fpush(int) {
    //spush();
}
std::string perr;
int parse_error(char * c){
    perr = c;
    return -1;
}
int fladd(int args, xargs * xargs, xop * xop) {
    std::string dest = (*xargs)[1];
    std::string str = (*xargs)[2];

    if (dest[0] == 'r' || dest[0] == 'R') {
        //register destination
        if (dest[1] < '0' || dest[1] > '9') {
            return parse_error("bad register name");
        }
        char id = dest[1] - '0';
    }
    //op x(fpush)
    //op x(fadd);
    //xop->push_back(x);
}
int floadnull(
    int nargs, 
    xargs * args,
    xop * xop_table) {}

#define OP_TABLE_SIZE 13
struct opdef ops[OP_TABLE_SIZE] = {
    { "ADD", 2, fladd },
    { "SUB", 2, floadnull },
    { "DIV", 2, floadnull },
    { "MUL", 2, floadnull },
    { "MOV", 2, floadnull },
    { "SWP", 2, floadnull },
    { "JMP", 2, floadnull },
    { "JGZ", 2, floadnull },
    { "JLZ", 2, floadnull },
    { "JEZ", 2, floadnull },
    { "JL", 2, floadnull },
    { "JG", 2, floadnull },
    { "JE", 2, floadnull }
};
struct function {
    std::string name;
    int id;
    xop op_table;
    //op * op_table; //malloc?
};
std::vector<struct function> func_table;

int execute() {
    if (func_table.size() < 1) {
        return -1;
    }
    struct function * f = &func_table[0];
    fptr = 0;
    isptr = 0;
}

std::vector<std::string> buffer;
int scan_line(std::string * l) {
    for (int i = 0; i < l->size(); i++) {

    }
}
enum syntax {
    function = 1,
};
syntax major_syntax_lable(std::string * f) {
    if ((*f)[0] == '.') {
        return syntax::function;
    }
}

std::vector<std::string> split(std::string *f) {
    std::vector<std::string> buf;
    int start = 0;
    bool valid = false;
    for (int i = 0; i < f->size(); i++) {
        if ((*f)[i] >= 'A' && !valid) {
            valid = true;
            start = i;
        }
        if ((*f)[i] == ' ' || i ==  f->size() -1 ) {
            if (!valid)
                continue;
            std::string x = f->substr(start, i - start);
            buf.push_back(x);
            start = i;
        }
    }
     printf("[%d] parsed: ", buf.size());
    for (int i = 0; i < buf.size(); i++) {
        printf("[%s] ", buf[i].c_str());
    }
    putchar('\n');
    return buf;
}   
std::string to_upper(std::string & s){
    std::transform(s.begin(), s.end(), s.begin(), ::toupper);
    return s;
}
int try_instruction(std::string * f, 
    xop * optable) {
    std::vector<std::string> items = split(f);
    if (items.size() < 1)
        //at least instruction //and 1 argument     
        return -1;
    for (int i = 0; i < OP_TABLE_SIZE; i++) {
        struct opdef * o = &ops[i];
        if (to_upper(items[0]) == o->key) {
            //matched opcode!
            o->l(items.size() - 1, &items, optable);
        }
    }
}
int eat_function(std::vector<std::string> & buf, int start) {
    struct function func;
    int i = start;
    for (; i < buf.size(); i++) {
        std::string *f = &buf[i];
        if (major_syntax_lable(f) ) {
            return i - start;   
        }
        // parse instructions into opcodes
        if (try_instruction(f, &func.op_table)) {

        }
    }
    func_table.push_back(func);
    return i - start;
}

void parse(std::vector<std::string> & buf) {
    for (int i = 0; i < buf.size(); i++) {
        std::string * f = &buf[i];
        if (major_syntax_lable(f) == syntax::function) {
            int ate = eat_function(buf, ++i);
            i += ate - 1;
        }
        //scan_line(f);
    }    
}

int main(int argc, char ** argv) {
    char * filename = 0;
    for (size_t i = 1; i < argc; i++){
        if (argv[i][0] == '-' 
            && argv[i][1] == 'i'){
            if ((i + 1) < argc) {
                filename = argv[++i];
            }
        }
        /* code */
    }
    if (filename == 0) {
        puts("no input file\n");
        return 0;
    }
    
    std::ifstream infile;//(filename);
    infile.open(filename);
    if (!infile.is_open()){
        puts("failed to read file\n");
        return 0;
    }

    int bytes = 0;
    std::string buf;
    while(std::getline(infile, buf)){
        for (int i = 0; i < buf.length(); i++) {
            if (buf[i] == '\t' 
                || buf[i] == '\n'
                || buf[i] == '\r') {
                    buf[i] =  buf[i + 1];
                }
        }
        bytes += buf.length();
        buffer.push_back(buf);
    }
    printf("loaded: %d lines (%d bytes)\n", buffer.size(), bytes);
    parse(buffer);
    execute();
    return 0;
}
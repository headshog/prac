#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <string>
#include <fstream>

using namespace std;

/* Error wrapper section */
enum error {
    NOERROR = 0,
    FILE_NO = -1,
    FILE_BAD = -2,
    LEX_NOPROG = -3,
    LEX_NO_OPBRAC = -4,
    LEX_NO_CLBRAC = -5,
    COMM_NOCLOSE = -6,
    WRONG_DECLTYPE = -7,
    WRONG_IDENT_NAME = -8,
    WRONG_DECL = -9
};

error error_wrapper(error e) {
    switch (e) {
        case FILE_NO:
            cout << "No filename given.\n";
            break;
        case FILE_BAD:
            cout << "Cannot open given file.\n";
            break;
        case LEX_NOPROG:
            cout << "Lexical error: no word 'program' in the beginning.\n";
            break;
        case LEX_NO_OPBRAC:
            cout << "Lexical error: no opening bracket '{'.\n";
            break;
        case LEX_NO_CLBRAC:
            cout << "Lexical error: no closing bracket '}'.\n";
            break;
        case COMM_NOCLOSE:
            cout << "Comment is not closed.\n";
            break;
        case WRONG_DECLTYPE:
            cout << "Wrong declaration type.\n";
            break;
        case WRONG_IDENT_NAME:
            cout << "Wrong identifier name.\n";
            break;
        case WRONG_DECL:
            cout << "Wrong syntax in declaration.\n";
            break;
        case NOERROR:
            break;
    }
    return e;
}

/* Needed declarations */
string buf;
size_t ptr;
unordered_set<string> service_operators {
    "if", "while", "read", "write", "break", "for", "{", "goto", "continue", "else", "}"
};
unordered_set<string> service_decl {
    "int", "string", "boolean", "struct"
};

void skip_spaces_endl();
error erase_comm();
string get_service_word();
string get_identifier();
string get_value();

error check_lexic_syntax();
error decl_stage();
error operand_stage();


int main(int argc, char **argv) {
    if(argc == 1)
        return error_wrapper(FILE_NO);

    ifstream file(argv[1], ios::binary | ios::ate);
    streamsize size;
    if(file.fail() || (size = file.tellg()) == -1)
        return error_wrapper(FILE_BAD);
    
    file.seekg(0, ios::beg);
    buf = string(size, '0');
    if (!file.read(buf.data(), size))
        return error_wrapper(FILE_BAD);

    error lex_err = check_lexic_syntax();
    if(lex_err != NOERROR)
        return error_wrapper(lex_err);
}

void skip_spaces_endl() {
    while(ptr < buf.size() && (buf[ptr] == ' ' || buf[ptr] == '\n' || buf[ptr] == '\t')) ptr++;
}

error erase_comm() {
    string tmp_buf;
    bool add_symb = true;
    for(size_t i = 0; i < buf.size(); i++) {
        if(buf[i] == '/' && i + 1 != buf.size() && buf[i + 1] == '*')
            add_symb = false, i += 2;
        else if(!add_symb) {
            if(i + 1 == buf.size())
                return COMM_NOCLOSE;
            if(buf[i] == '*' && buf[i + 1] == '\\')
                add_symb = true, i += 2;
        }
        else if(add_symb)
            tmp_buf += buf[i];
    }
    if(!add_symb)
        return COMM_NOCLOSE;
    buf = tmp_buf;
    return NOERROR;
}

string get_service_word() {
    string w;
    size_t tmp_ptr = ptr;
    while(tmp_ptr < buf.size() && buf[tmp_ptr] != ' ' && buf[tmp_ptr] != '\n' && 
          buf[tmp_ptr] != '(' && buf[tmp_ptr] != ';' && buf[tmp_ptr] != '\t')
        w += buf[tmp_ptr++];
    return tmp_ptr >= buf.size() && (service_operators.find(w) != service_operators.end() ||
                                     service_decl.find(w) != service_decl.end()) ? w : "";
}

string get_identifier() {
    string id;
    size_t tmp_ptr = ptr;
    bool wr_symb = false;
    if(isdigit(buf[tmp_ptr]))
        wr_symb = true;
    while(!wr_symb && tmp_ptr < buf.size() && buf[tmp_ptr] != ' ' && buf[tmp_ptr] != '\n' && 
          buf[tmp_ptr] != '=' && buf[tmp_ptr] != ';' && buf[tmp_ptr] != '\t' && buf[tmp_ptr] != '{') {
        if(buf[tmp_ptr] == '\\' || buf[tmp_ptr] == '(' || buf[tmp_ptr] == '\"') {
            wr_symb = true;
            break;
        }
        id += buf[tmp_ptr++];
    }
    return !wr_symb && service_operators.find(id) == service_operators.end() &&
           service_decl.find(id) == service_decl.end() ? id : "";
}

string get_value() {
    string val;
    size_t tmp_ptr = ptr;
    bool wr_digit = false;
    // case string and bool and int
    if(buf[ptr] == '\"') {

    }
    else if(buf[tmp_ptr] == '-' || buf[tmp_ptr] || isdigit(buf[ptr]))
    while(tmp_ptr < buf.size() && buf[tmp_ptr] != ' ' && buf[tmp_ptr] != '\n' && 
          buf[tmp_ptr] != '=' && buf[tmp_ptr] != ';' && buf[tmp_ptr] != '\t'
}

error check_lexic_syntax() {
    error comm_err = erase_comm();
    if(comm_err != NOERROR)
        return comm_err;

    skip_spaces_endl();
    if(buf.size() > 7 || buf.substr(0, 7) != "program")
        return LEX_NOPROG;
    skip_spaces_endl();
    if(ptr >= buf.size() || buf[ptr] != '{')
        return LEX_NO_OPBRAC;
    
    skip_spaces_endl();
    error decl_err = decl_stage();
    if(decl_err != NOERROR)
        return decl_err;

    skip_spaces_endl();
    error operand_err = operand_stage();
    if(operand_err != NOERROR)
        return operand_err;
    
    skip_spaces_endl();
    if(ptr >= buf.size() || buf[ptr] != '}')
        return LEX_NO_CLBRAC;

    return NOERROR;
}

error decl_stage() {
    string decl_type = get_service_word(), identifier, value;
    while(service_operators.find(decl_type) != service_operators.end()) {
        if(decl_type.empty())
            return WRONG_DECLTYPE;
        ptr += decl_type.size();
        
        skip_spaces_endl();
        identifier = get_identifier();
        if(identifier.empty())
            return WRONG_IDENT_NAME;
        ptr += identifier.size();

        skip_spaces_endl();
        value.clear();
        if(ptr >= buf.size() || buf[ptr] != ';' || buf[ptr] != '=' || buf[ptr] != '{')
            return WRONG_DECL;
        if(buf[ptr] == '=') {
            ptr++;
            skip_spaces_endl();
            value = get_value();
            skip_spaces_endl();
            if(value.empty() || ptr >= buf.size() || buf[ptr] != ';')
                return WRONG_DECL;
        }
        else if(buf[ptr] == '{') {
            ptr++;
            skip_spaces_endl();
            error decl_err = decl_stage();
            if(decl_err != NOERROR)
                return decl_err;
            if(buf[ptr] != '}')
                return LEX_NO_CLBRAC;
            ptr++;
            if(buf[ptr] != ';')
                return WRONG_DECL;
        }

        ptr++;
        skip_spaces_endl();
        decl_type = get_service_word();
    }
    return NOERROR;
}

error operand_stage() {
    return NOERROR;
}
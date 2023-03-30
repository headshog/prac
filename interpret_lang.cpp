#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <string>
#include <fstream>
#include <variant>

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
    WRONG_DECL_TYPE = -7,
    WRONG_CONST_TYPE = -8,
    WRONG_IDENT_NAME = -9,
    WRONG_DECL = -10,
    WRONG_OPER = -11,
    PREV_DECL = -12
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
        case WRONG_DECL_TYPE:
            cout << "Wrong declaration type.\n";
            break;
        case WRONG_CONST_TYPE:
            cout << "Constant type does not match variable type.\n";
            break;
        case WRONG_IDENT_NAME:
            cout << "Wrong identifier name.\n";
            break;
        case WRONG_DECL:
            cout << "Wrong syntax in declaration.\n";
            break;
        case WRONG_OPER:
            cout << "Wrong operand.\n";
            break;
        case PREV_DECL:
            cout << "Variable is previously declared.\n";
            break;
        case NOERROR:
            break;
    }
    return e;
}

/* Structures for lexical and syntactic checking */
string buf;
size_t ptr;
unordered_set<string> service_operators {
    "if", "while", "read", "write", "break", "for", "{", "goto", "continue", "else", "}"
};
unordered_set<string> service_decl {
    "int", "string", "boolean", "struct"
};
unordered_set<char> ident_stop {
    ' ', ';', '=', '\n', '\t', ')', '<', '>', '!', '+', '-', '*', '.', ':', ','
};

/* Structures for semantic checking and parsing to */
/* language interpreter. */
struct BaseIdent {
    string name, type;
    variant<int, bool, string, vector<BaseIdent>> val;
};
vector<BaseIdent> ID;
unordered_map<string, BaseIdent*> ID_refs;

class AstNode {
    
};

/* Func prototypes */
void skip_spaces_endl();
error erase_comm();
string get_service_word();
string get_identifier();
string get_mark();
string get_value();

error check_lexical_syntax();
error get_id_and_val(vector<BaseIdent>& ID, string& decl_type);
error declaration(vector<BaseIdent>& ID);
error operands();


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

    error lex_err = check_lexical_syntax();
    if(lex_err != NOERROR)
        return error_wrapper(lex_err);
}

void skip_spaces_endl() {
    while(ptr < buf.size() && (buf[ptr] == ' ' || buf[ptr] == '\n' || buf[ptr] == '\t')) ptr++;
}

error erase_comm() {
    string tmp_buf;
    bool add_symb = true, str = false;
    for(size_t i = 0; i < buf.size(); i++) {
        if(buf[i] == '\"')
            str = !str;
        if(str)
            continue;
        
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
    return tmp_ptr < buf.size() && (service_operators.find(w) != service_operators.end() ||
                                     service_decl.find(w) != service_decl.end()) ? w : "";
}

string get_identifier() {
    string id;
    size_t tmp_ptr = ptr;
    bool wr_symb = false;
    if(isdigit(buf[tmp_ptr]))
        wr_symb = true;
    while(!wr_symb && tmp_ptr < buf.size() && ident_stop.find(buf[tmp_ptr]) == ident_stop.end()) {
        if(buf[tmp_ptr] == '\\' || buf[tmp_ptr] == '(' || buf[tmp_ptr] == '\"' || buf[tmp_ptr] == '}') {
            wr_symb = true;
            break;
        }
        id += buf[tmp_ptr++];
    }
    return !wr_symb && tmp_ptr < buf.size() && service_operators.find(id) == service_operators.end() &&
           service_decl.find(id) == service_decl.end() ? id : "";
}

string get_mark() {
    string m = get_identifier();
    return buf[ptr + m.size()] == ':' ? m : ""; 
}

string get_value() {
    string val;
    size_t tmp_ptr = ptr;
    if(buf[tmp_ptr] == '\"') {
        val += buf[tmp_ptr++];
        while(tmp_ptr < buf.size() && buf[tmp_ptr] != '\"')
            val += buf[tmp_ptr++];
        return tmp_ptr < buf.size() ? val + '\"' : "";
    }
    else if(buf[tmp_ptr] == '-' || buf[tmp_ptr] == '+' || isdigit(buf[tmp_ptr])) {
        if(!(buf[tmp_ptr] == '-')) 
            val += '-', tmp_ptr++;
        while(tmp_ptr < buf.size() && isdigit(buf[tmp_ptr]))
            val += buf[tmp_ptr++];
        return tmp_ptr < buf.size() ? val : "";
    }
    else {
        if(tmp_ptr + 5 >= buf.size() && 
            buf.substr(tmp_ptr, 4) != "true" && buf.substr(tmp_ptr, 5) != "false")
            return "";
        return buf.substr(tmp_ptr, 4) == "true" ? "true" : "false";
    }
}

error check_lexical_syntax() {
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
    error decl_err = declaration(ID);
    if(decl_err != NOERROR)
        return decl_err;

    skip_spaces_endl();
    error operand_err = operands();
    if(operand_err != NOERROR)
        return operand_err;
    
    skip_spaces_endl();
    if(ptr >= buf.size() || buf[ptr] != '}')
        return LEX_NO_CLBRAC;

    return NOERROR;
}

error get_id_and_val(vector<BaseIdent>& ID, string& decl_type) {
    string identifier, value;
    skip_spaces_endl();
    identifier = get_identifier();
    if(identifier.empty())
        return WRONG_IDENT_NAME;
    ptr += identifier.size();
    ID.push_back(BaseIdent());
    ID.back().type = decl_type;
    ID.back().name = identifier;
    
    if(ID_refs.insert(make_pair(identifier, &ID.back())).second)
        return PREV_DECL;

    skip_spaces_endl();
    value.clear();
    if(ptr >= buf.size() || buf[ptr] != ';' || buf[ptr] != '=' || buf[ptr] != '{' || buf[ptr] != ',')
        return WRONG_DECL;
    if(buf[ptr] == '=') {
        ptr++;
        skip_spaces_endl();
        value = get_value();
        ptr += value.size();
        skip_spaces_endl();
        if(value.empty() || ptr >= buf.size() || buf[ptr] != ';')
            return WRONG_DECL;
        if((decl_type == "string" && value.front() != '\"') ||
           (decl_type == "int" && (value.front() != '-' || !isdigit(value.front()))) || 
           (decl_type == "boolean" && !(value == "true" || value == "false")))
           return WRONG_CONST_TYPE;
        
        if(decl_type == "string")
            get<string>(ID.back().val) = value.substr(1, value.size() - 2);
        else if(decl_type == "int")
            get<int>(ID.back().val) = atoi(value.c_str());
        else
            get<bool>(ID.back().val) = value == "true" ? true : false;
    }
    else if(buf[ptr] == '{') {
        ptr++;
        skip_spaces_endl();
        error decl_err = declaration(get<vector<BaseIdent>>(ID.back().val));
        if(decl_err != NOERROR)
            return decl_err;
        if(buf[ptr] != '}')
            return LEX_NO_CLBRAC;
        ptr++;
        if(buf[ptr] != ';')
            return WRONG_DECL;
    }
    else if(buf[ptr] == ',') {
        ptr++;
        get_id_and_val(ID, decl_type);
    }
    return NOERROR;
}

error declaration(vector<BaseIdent>& ID) {
    string decl_type = get_service_word();
    while(service_operators.find(decl_type) != service_operators.end()) {
        if(decl_type.empty())
            return WRONG_DECL_TYPE;
        ptr += decl_type.size();

        error err_get_id = get_id_and_val(ID, decl_type);
        if(err_get_id != NOERROR)
            return err_get_id;

        ptr++;
        skip_spaces_endl();
        decl_type = get_service_word();
    }
    return NOERROR;
}

error operands() {
    while(buf[ptr] != '}') {
        string mark = get_mark();
        bool is_mark = !mark.empty();
        if(is_mark)
            ptr += mark.size() + 1;
        
        string op = get_service_word();
        if(op.empty() || op == "break")
            return WRONG_OPER;

        if(op == "{") {
            ptr++;
            skip_spaces_endl();
            error err_op = operands();
            if(err_op != NOERROR)
                return err_op;
            if(buf[ptr] != '}')
                return LEX_NO_CLBRAC;
        }
        else if(op == "if") {
            
        }
        else if(op == "for") {

        }
        else if(op == "while") {

        }
        else if(op == "goto") {
            
        }
        else if(op == "read") {
            
        }
        else if(op == "write") {
            
        }
        else {

        }
    }
    return NOERROR;
}
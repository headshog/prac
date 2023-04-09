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
    UNIDENT_SYMBOL = -3,
    SYNT_NOPROG = -4,
    SYNT_NO_OPBRAC = -5,
    SYNT_NO_CLBRAC = -6,
    SYNT_NO_OPBRAC_PR = -7,
    SYNT_NO_SEMICOLON = -8,
    SYNT_NO_SEMICOLON_FOR = -9,
    COMM_NOCLOSE = -10,
    WRONG_DECL_TYPE = -11,
    WRONG_CONST_TYPE = -12,
    WRONG_IDENT_NAME = -13,
    WRONG_DECL = -14,
    WRONG_OPER = -15,
    PREV_DECL = -16,
    STRUCT_PREV_DECL = -17,
    STRUCT_NO_IDENT = -18,
    STRUCT_IN_STRUCT = -19,
    WRONG_IF_EXPR = -20,
    WRONG_READ_EXPR = -21,
    WRONG_WRITE_EXPR = -22,
    WRONG_FOR_EXPR = -23,
    WRONG_WHILE_EXPR = -24,
    WRONG_EXPR = -25,
    WRONG_BR_CNT = -26,
    WRONG_GOTO_IDENT = -27,
    WRONG_EXPR_OP_TYPES = -28,
    WRONG_WRITE_EXPR_TYPE = -29,
    WRONG_DECL_AFTER_SECT = -30,
    ERROR = -31
};
size_t cnt_lines = 1;
vector<pair<error, size_t>> err_stk;

int error_wrapper() {
    bool is_err = err_stk.empty();
    for(auto it : err_stk) {
        error e = it.first;
        switch (e) {
            case FILE_NO:
                cout << "No filename given.\n";
                break;
            case FILE_BAD:
                cout << "Cannot open given file.\n";
                break;
            case UNIDENT_SYMBOL:
                cout << "Unidentified symbol found on line "
                << it.second << endl << endl;
                break;
            case SYNT_NOPROG:
                cout << "Syntax error: no word 'program' in the beginning.\n";
                break;
            case SYNT_NO_OPBRAC:
                cout << "Syntax error: no opening bracket '{' or '(' in operator on line "
                << it.second - 1 << " or "  << it.second << endl << endl;
                break;
            case SYNT_NO_CLBRAC:
                cout << "Syntax error: no closing bracket '}' or ')' in operator "
                << "\nor wrong syntax of operator on line " << it.second << endl << endl;
                break;
            case SYNT_NO_OPBRAC_PR:
                cout << "Syntax error: no opening bracket '{' for 'program' operator" << endl << endl;
                break;
            case SYNT_NO_SEMICOLON:
                cout << "Syntax error: no ';' in the end of operator on line "
                << it.second - 1 << "\nOr wrong syntax of operator on line "
                << it.second << endl << endl;
                break;
            case SYNT_NO_SEMICOLON_FOR:
                cout << "Syntax error: no ';' in the middle of operator 'for' on line "
                << it.second << endl << endl;
                break;
            case COMM_NOCLOSE:
                cout << "Comment is not closed on line ";
                break;
            case WRONG_DECL_TYPE:
                cout << "Wrong declaration type on line: "
                << it.second << endl << endl;
                break;
            case WRONG_CONST_TYPE:
                cout << "Constant type does not match the variable type on line "
                << it.second << endl << endl;
                break;
            case WRONG_IDENT_NAME:
                cout << "Wrong identifier name on line "
                << it.second << endl << endl;
                break;
            case WRONG_DECL:
                cout << "Wrong syntax in declaration on line "
                << it.second - 1 << endl << endl;
                break;
            case WRONG_OPER:
                cout << "Wrong operator on line "
                << it.second << " or no closing bracket '}'"
                << " for 'program' operator.\n";
                break;
            case PREV_DECL:
                cout << "Variable is declared again on line "
                << it.second << endl << endl;
                break;
            case STRUCT_PREV_DECL:
                cout << "Structute type is declared again on line "
                << it.second << endl << endl;
                break;
            case STRUCT_NO_IDENT:
                cout << "Structure identifier is not declared on line "
                << it.second << endl << endl;
                break;
            case STRUCT_IN_STRUCT:
                cout << "Structure is declared inside of structure on line "
                << it.second << endl << endl;
                break;
            case WRONG_IF_EXPR:
                cout << "Wrong expression in if operator on line "
                << it.second << endl << endl;
                break;
            case WRONG_READ_EXPR:
                cout << "Wrong expression in read operator on line "
                << it.second << endl << endl;
                break;
            case WRONG_WRITE_EXPR:
                cout << "Wrong expression in write operator on line "
                << it.second << endl << endl;
                break;
            case WRONG_FOR_EXPR:
                cout << "Wrong expression in for operator on line "
                << it.second << endl << endl;
                break;
            case WRONG_WHILE_EXPR:
                cout << "Wrong expression in while operator on line "
                << it.second << endl << endl;
                break;
            case WRONG_EXPR:
                cout << "Wrong expression in operator on line "
                << it.second << endl << endl;
                break;
            case WRONG_BR_CNT:
                cout << "Using of operator break or continue not in cycle operators on line "
                << it.second << endl << endl;
                break;
            case WRONG_GOTO_IDENT:
                cout << "Wrong identifier in goto operator.\n";
                break;
            case WRONG_EXPR_OP_TYPES:
                cout << "Incompatible operand types in expression on line "
                << it.second << endl << endl;
                break;
            case WRONG_WRITE_EXPR_TYPE:
                cout << "Wrong type expression type in write operator.\n";
                break;
            case WRONG_DECL_AFTER_SECT:
                cout << "Wrong declaration inside of operator section on line "
                << it.second << endl << endl;
                break;
            case NOERROR:
                break;
            case ERROR:
                break;
        }
    }
    if(!err_stk.empty())
        cout << endl;
    return is_err ? 0 : -1;
}

/* Structures for lexical and syntactic checking */
string buf;
size_t ptr;
unordered_set<string> service_operators {
    "if", "while", "read", "write", "break", "for", "{", "goto", "continue", "else", "}", ";"
};
unordered_set<string> service_decl {
    "int", "string", "boolean", "struct"
};
unordered_set<string> service_ops {
    "<", ">", "<=", ">=", "!=", "==", "and", "or", "not", "+", "-", "=", "*", "/", "+!", "-!"
};
unordered_set<string> log_ops {
    "<", ">", "<=", ">=", "!=", "==", "and", "or", "not"
};
unordered_set<string> left_as_ops {
    "*", "/", "+", "-", "<", ">", "<=", ">=", "==", "!=", "and", "or"
};
unordered_map<string, int> prior {
    {"+!", 8}, {"-!", 8}, {"not", 8}, {"*", 7}, {"/", 7}, {"+", 6}, 
    {"-", 6}, {"<", 5}, {">", 5}, {"<=", 5}, {">=", 5}, {"==", 4}, 
    {"!=", 4}, {"and", 3}, {"or", 2}, {"=", 1}
};
unordered_set<char> service_alf {
    ' ', ';', '=', '\n', '\t', '<', '>', '!', '+', '-', '*', '.', ':', ',',
    '(', ')', '{', '}', '/', '\"', '\\'
};
unordered_set<char> ident_stop {
    ' ', ';', '=', '\n', '\t', ')', '<', '>', '!', '+', '-', '*', ':', ',', '{', '/'
};

/* Structures for semantic checking and parsing to */
/* language interpreter. */
struct BaseIdent {
    string name, type;
    variant<int, bool, string, vector<BaseIdent>> val;
};
vector<BaseIdent> ID;
unordered_map<string, BaseIdent> ID_refs;
unordered_map<string, vector<BaseIdent>> StructDeclarations;
unordered_set<string> StructNames;
vector<pair<string, size_t>> GotoMarked, GotoToMark;
vector<string> stk;
vector<BaseIdent> expr;
vector<size_t> cycle_stack, break_stack, continue_stack;

void operator+=(vector<BaseIdent>& op1, vector<BaseIdent>& op2) {
    for(auto& it : op2)
        op1.emplace_back(it);
}

/* Func prototypes */
void skip_spaces_endl();
void get_spaces_endl_back();
error erase_comm_check();
string get_service_word();
string get_service_op();
string get_identifier(bool struct_decl = false);
string get_mark();
string get_value();

error check_lex_synt_sem_prn();

error struct_declaration(string id = "");
error get_id_and_val(vector<BaseIdent>& ID, string& decl_type, string id);
error declaration(vector<BaseIdent>& ID, string id = "");

bool is_logical_expression(vector<BaseIdent>& expr);
bool is_compatible_types_expr(vector<BaseIdent>& expr);
error read_expression(vector<BaseIdent>& expr, bool is_single_expr = false);
error if_operator();
error for_operator();
error while_operator();
error goto_operator();
error read_operator();
error write_operator();
error expr_operator();
error complex_operator();
error operator_wrapper();

error interpret_prn();

int main(int argc, char **argv) {
    if(argc == 1) {
        err_stk.push_back({ FILE_NO, 0 });
        return error_wrapper();
    }

    ifstream file(argv[1], ios::binary | ios::ate);
    streamsize size;
    if(file.fail() || (size = file.tellg()) == -1) {
        err_stk.push_back({ FILE_BAD, 0 });
        return error_wrapper();
    }
    
    file.seekg(0, ios::beg);
    buf = string(size, '0');
    if (!file.read(buf.data(), size)) {
        err_stk.push_back({ FILE_BAD, 0 });
        return error_wrapper();
    }
    file.close();

    error lex_err = check_lex_synt_sem_prn();
    if(lex_err != NOERROR)
        return error_wrapper();
    if(!err_stk.empty())
        return error_wrapper();
    
    error err_interpret = interpret_prn();
    if(err_interpret != NOERROR)
        return error_wrapper();

    return error_wrapper();
}

void skip_spaces_endl() {
    while(ptr < buf.size() && (buf[ptr] == ' ' || buf[ptr] == '\n' || buf[ptr] == '\t')) {
        cnt_lines += buf[ptr] == '\n';
        ptr++;
    }
}
void get_spaces_endl_back() {
    while(ptr >= 0 && (buf[ptr] == ' ' || buf[ptr] == '\n' || buf[ptr] == '\t')) {
        cnt_lines -= buf[ptr] == '\n';
        ptr--;
    }
}
error erase_comm_check() {
    string tmp_buf;
    size_t comm_offset = 0;
    bool add_symb = true, str = false;
    for(size_t i = 0; i < buf.size(); i++) {
        cnt_lines += buf[i] == '\n' && add_symb;
        if(add_symb && buf[i] == '\"')
            str = !str;
        
        if(add_symb && !isdigit(buf[i]) && !isalpha(buf[i]) &&
           service_alf.find(buf[i]) == service_alf.end()) {
            err_stk.push_back({ UNIDENT_SYMBOL, cnt_lines });
            return ERROR;
        }

        if(!str && buf[i] == '/' && i + 1 != buf.size() && buf[i + 1] == '*')
            add_symb = false, i++;
        else if(!add_symb) {
            comm_offset += buf[i] == '\n';
            if(i + 1 == buf.size())
                err_stk.push_back({ COMM_NOCLOSE, cnt_lines });
            if(buf[i] == '*' && buf[i + 1] == '/') {
                add_symb = true;
                i++;
                tmp_buf += string(comm_offset, '\n');
                comm_offset = 0;
            }
        }
        else if(add_symb)
            tmp_buf += buf[i];
    }
    if(!add_symb)
        err_stk.push_back({ COMM_NOCLOSE, cnt_lines });
    if(!err_stk.empty())
        return ERROR;
    buf = tmp_buf;
    cnt_lines = 1;
    return NOERROR;
}

string get_service_word() {
    string w;
    size_t tmp_ptr = ptr;
    if(buf[tmp_ptr] == '{')
        return "{";
    while(tmp_ptr < buf.size() && buf[tmp_ptr] != ' ' && buf[tmp_ptr] != '\n' && 
          buf[tmp_ptr] != '(' && buf[tmp_ptr] != ';' && buf[tmp_ptr] != '\t' &&
          buf[tmp_ptr] != '{')
        w += buf[tmp_ptr++];
    return tmp_ptr <= buf.size() && (service_operators.find(w) != service_operators.end() ||
                                     service_decl.find(w) != service_decl.end()) ? w : "";
}
string get_service_op() {
    string w;
    size_t tmp_ptr = ptr;
    bool is_symb = false;
    bool alpha = isalpha(buf[tmp_ptr]);
    while(tmp_ptr < buf.size() && buf[tmp_ptr] != ' ' && buf[tmp_ptr] != '\n' && 
          buf[tmp_ptr] != '(' && buf[tmp_ptr] != '\t' && !isdigit(buf[tmp_ptr]) &&
          buf[tmp_ptr] != '\"' && !(is_symb && (buf[tmp_ptr] == '+' || buf[tmp_ptr] == '-')) &&
          ((alpha && isalpha(buf[tmp_ptr])) || (!alpha && !isalpha(buf[tmp_ptr])))) { 
        w += buf[tmp_ptr++];
        is_symb = !w.empty();
    }
    return tmp_ptr < buf.size() && (service_ops.find(w) != service_ops.end()) ? w : "";
}
string get_identifier(bool struct_decl) {
    string id;
    size_t tmp_ptr = ptr;
    bool wr_symb = false;
    if(isdigit(buf[tmp_ptr]))
        wr_symb = true;
    while(!wr_symb && tmp_ptr < buf.size() && ident_stop.find(buf[tmp_ptr]) == ident_stop.end()) {
        if(buf[tmp_ptr] == '(' || buf[tmp_ptr] == '\"' || buf[tmp_ptr] == '}') {
            wr_symb = true;
            break;
        }
        id += buf[tmp_ptr++];
    }
    bool cond1 = !wr_symb && tmp_ptr < buf.size() && service_operators.find(id) == service_operators.end();
    size_t pos;
    bool cond2 = (pos = id.find('.')) != id.npos && StructNames.find(id.substr(0, pos)) != StructNames.end();
    if(struct_decl)
        return cond1 && !cond2 ? id : "";
    return cond1 && !cond2 && service_decl.find(id) == service_decl.end() ? id : "";
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
        while(tmp_ptr < buf.size() && buf[tmp_ptr] != '\"') {
            if(buf[tmp_ptr] == '\\' && tmp_ptr < buf.size() - 1) {
                if(buf[tmp_ptr + 1] == 'n')
                   val += '\n', tmp_ptr += 2;
                else if(buf[tmp_ptr + 1] == 't')
                    val += '\t', tmp_ptr += 2;
                else
                    val += '\\';
            }
            else
                val += buf[tmp_ptr++];
        }
        return tmp_ptr < buf.size() ? val + '\"' : "";
    }
    else if(buf[tmp_ptr] == '-' || buf[tmp_ptr] == '+' || isdigit(buf[tmp_ptr])) {
        if(buf[tmp_ptr] == '-') 
            val += '-', tmp_ptr++;
        while(tmp_ptr < buf.size() && isdigit(buf[tmp_ptr]))
            val += buf[tmp_ptr++];
        return tmp_ptr < buf.size() && val != "-" ? val : "";
    }
    else {
        if(tmp_ptr + 5 >= buf.size() || 
           (buf.substr(tmp_ptr, 4) != "true" && buf.substr(tmp_ptr, 5) != "false"))
            return "";
        return buf.substr(tmp_ptr, 4) == "true" ? "true" : "false";
    }
}

error check_lex_synt_sem_prn() {
    error comm_err = erase_comm_check();
    if(comm_err != NOERROR)
        return ERROR;

    skip_spaces_endl();
    if(buf.size() < 7 || buf.substr(ptr, 7) != "program") {
        err_stk.push_back({ SYNT_NOPROG, cnt_lines });
        return ERROR;
    }
    ptr += 7;
    skip_spaces_endl();
    if(ptr >= buf.size() || buf[ptr] != '{') {
        err_stk.push_back({ SYNT_NO_OPBRAC, cnt_lines });
        return ERROR;
    }
    ptr++;
    skip_spaces_endl();
    if(ptr == buf.size())
        err_stk.push_back({ SYNT_NO_CLBRAC, cnt_lines });
    if(!err_stk.empty())
        return ERROR;
    
    error err_decl = declaration(ID);
    if(err_decl != NOERROR)
        return ERROR;
    ID.clear();
    StructDeclarations.clear();

    skip_spaces_endl();
    error err_operator = complex_operator();
    for(auto& it : GotoToMark)
        if(get<int>(expr[it.second].val) == -1)
            err_stk.push_back({ WRONG_GOTO_IDENT, 0 });

    if(err_operator != NOERROR)
        return ERROR;
    
    skip_spaces_endl();
    if(ptr >= buf.size() || buf[ptr] != '}') {
        err_stk.push_back({ SYNT_NO_CLBRAC, cnt_lines });
        return ERROR;
    }
    ptr++;
    skip_spaces_endl();
    if(ptr != buf.size()) {
        err_stk.push_back({ SYNT_NO_OPBRAC_PR, cnt_lines });
        return ERROR;
    }
    return NOERROR;
}

error struct_declaration(string id) {
    string decl_type = get_service_word();
    ptr += decl_type.size();
    skip_spaces_endl();
    string new_id = get_identifier(true);
    if(new_id != "" && !StructNames.insert(new_id).second) {
        err_stk.push_back({ STRUCT_PREV_DECL, cnt_lines });
        return ERROR;
    }
    new_id = get_identifier();
    id = id == "" ? new_id : id + "." + new_id;
    if(id.empty())
        err_stk.push_back({ STRUCT_NO_IDENT, cnt_lines });
    ptr += new_id.size();
    skip_spaces_endl();
    service_decl.insert(id);
    StructDeclarations[id] = vector<BaseIdent>();

    if(buf[ptr] != '{') {
        err_stk.push_back({ SYNT_NO_OPBRAC, cnt_lines });
        return ERROR;
    }
    ptr++;
    skip_spaces_endl();

    size_t struct_cnt = StructNames.size();
    error err_decl = declaration(StructDeclarations[id], id);
    if(struct_cnt < StructNames.size()) {
        err_stk.push_back({ STRUCT_IN_STRUCT, cnt_lines });
        return ERROR;
    }
    if(err_decl != NOERROR)
        return ERROR;
    skip_spaces_endl();
    if(buf[ptr] != '}') {
        err_stk.push_back({ SYNT_NO_CLBRAC, cnt_lines });
        return ERROR;
    }
    ptr++;
    if(buf[ptr] != ';') {
        err_stk.push_back({ SYNT_NO_SEMICOLON, cnt_lines });
        return ERROR;
    }
    ptr++;
    return NOERROR;
}
error get_id_and_val(vector<BaseIdent>& ID, string& decl_type, string id) {
    string identifier, value;
    skip_spaces_endl();
    identifier = get_identifier();
    if(identifier.empty())
        err_stk.push_back({ WRONG_IDENT_NAME, cnt_lines });
    ptr += identifier.size();
    string old_identifier = identifier;
    if(!id.empty())
        identifier = id + "." + identifier;
    ID.emplace_back(BaseIdent{ old_identifier, decl_type });
    if(StructNames.find(decl_type) != StructNames.end()) {
        ID.back().val = StructDeclarations[decl_type];
        for(auto& it : StructDeclarations[decl_type]) {
            string name = identifier + "." + it.name;
            ID_refs.insert(make_pair(name, it));
        }
    }
    else if(decl_type == "string")
        ID.back().val = string();
    else if(decl_type == "int")
        ID.back().val = 0;
    else if(decl_type == "boolean")
        ID.back().val = false;

    skip_spaces_endl();
    if(ptr >= buf.size() || (buf[ptr] != ';' && buf[ptr] != '=' && buf[ptr] != '{' && buf[ptr] != ','))
        err_stk.push_back({ WRONG_DECL, cnt_lines });
    if(buf[ptr] == '=') {
        ptr++;
        skip_spaces_endl();
        value = get_value();
        ptr += value.size();
        skip_spaces_endl();
        if(value.empty() || ptr >= buf.size() || (buf[ptr] != ';' && buf[ptr] != ','))
            err_stk.push_back({ WRONG_DECL, cnt_lines });
        if((decl_type == "string" && value.front() != '\"') ||
           (decl_type == "int" && (value.front() != '-' && !isdigit(value.front()))) || 
           (decl_type == "boolean" && !(value == "true" || value == "false")))
           err_stk.push_back({ WRONG_CONST_TYPE, cnt_lines });
        
        if(decl_type == "string")
            ID.back().val = value.substr(1, value.size() - 2);
        else if(decl_type == "int")
            ID.back().val = atoi(value.c_str());
        else
            ID.back().val = (value == "true" ? true : false);
    }
    if(!ID_refs.insert(make_pair(identifier, ID.back())).second)
        err_stk.push_back({ PREV_DECL, cnt_lines });
    if(buf[ptr] == ',') {
        ptr++;
        if(get_id_and_val(ID, decl_type, id) != NOERROR)
            return ERROR;
    }
    return NOERROR;
}
error declaration(vector<BaseIdent>& ID, string id) {
    skip_spaces_endl();
    string decl_type = get_service_word();
    if(get_value() != "")
        return NOERROR;
    while(service_operators.find(decl_type) == service_operators.end()) {
        if(decl_type.empty() || service_decl.find(decl_type) == service_decl.end())
            err_stk.push_back({ WRONG_DECL_TYPE, cnt_lines });
        if(decl_type == "struct") {
            error err_s_decl = struct_declaration(id);
            if(err_s_decl != NOERROR)
                return ERROR;
        }
        else {
            ptr += decl_type.size();
            if(get_id_and_val(ID, decl_type, id) != NOERROR)
                return ERROR;
            if(buf[ptr] != ';') {
                err_stk.push_back({ SYNT_NO_SEMICOLON, cnt_lines });
                return ERROR;
            }
            ptr++;
        }
        skip_spaces_endl();
        string expr_op = get_identifier();
        if((!expr_op.empty() && !ID_refs.insert(make_pair(expr_op, BaseIdent())).second) ||
           (get_value() != ""))
            break;
        decl_type = get_service_word();
    }
    return NOERROR;
}

bool is_logical_expression(vector<BaseIdent>& expr) {
    for(auto& it : expr) {
        if(log_ops.find(it.name) != log_ops.end() || it.type == "boolean")
            return true;
    }
    return false;
}
bool is_compatible_types_expr(vector<BaseIdent>& expr) {
    vector<pair<string, string>> expr_stk;
    string last_operand_type;
    for(auto& it : expr) {
        auto str_cond = StructNames.find(it.type) != StructNames.end();
        if(it.type == "int" || it.type == "boolean" || it.type == "string" || str_cond)
            expr_stk.push_back({ it.type, it.name });
        else if(it.type == "operation") {
            string last_operand_type = expr_stk.back().first, last_operand_name = expr_stk.back().second;
            expr_stk.pop_back();
            if(left_as_ops.find(it.name) != left_as_ops.end() || it.name == "=") {
                if(last_operand_type != expr_stk.back().first)
                    return false;
                else {
                    if(last_operand_type == "boolean" && 
                       (it.name == "+" || it.name == "-" || it.name == "+!" || 
                        it.name == "-!" || it.name == "*" || it.name == "/"))
                        return false;
                    else if(last_operand_type == "int" && 
                            (it.name == "not" || it.name == "and" || it.name == "or"))
                        return false;
                    else if(last_operand_type == "string" && 
                            (it.name != "+" && it.name != "<" && it.name != "<" &&
                             it.name != ">" && it.name != "!=" && it.name != "==" &&
                             it.name != "=" && it.name != ">=" && it.name != "<="))
                        return false;
                    else if(StructNames.find(last_operand_type) != StructNames.end() && 
                            it.name != "=")
                        return false;
                    else if(it.name == "=" && ID_refs.find(expr_stk.back().second) == ID_refs.end())
                        return false;
                }
                last_operand_type = expr_stk.back().first;
                expr_stk.pop_back();
            }
            if(it.name == "+!" || it.name == "-!" || it.name == "=" ||
               it.name == "+" || it.name == "-" || it.name == "*" ||
               it.name == "/")
                expr_stk.push_back({ last_operand_type, "const" });
            else
                expr_stk.push_back({ "boolean", "const" });
        }
    }
    return true;
}
error read_expression(vector<BaseIdent>& expr, bool is_single_expr) {
    string operand;
    bool last_op = true, last_ident = false;
    while(ptr < buf.size()) {
        skip_spaces_endl();
        if(buf[ptr] == '(')
            stk.emplace_back("("), ptr++;
        else if(buf[ptr] == ')' || buf[ptr] == ';' || buf[ptr] == ',') {
            while(!stk.empty() && stk.back() != "(") {
                expr.emplace_back(BaseIdent());
                expr.back().type = "operation";
                expr.back().name = stk.back();
                stk.pop_back();
            }
            if(stk.empty())
                err_stk.push_back({ WRONG_EXPR, cnt_lines });
            stk.pop_back();
            if(!is_single_expr || (is_single_expr && buf[ptr] != ';'))
                ptr++;
        }
        else if(!last_ident && (operand = get_value()) != "") {
            expr.emplace_back(BaseIdent());
            expr.back().name = "const";
            size_t cnt_slash = 0;
            if(operand.front() == '\"') {
                expr.back().type = "string";
                expr.back().val = operand.substr(1, operand.size() - 2);
                for(auto it : operand)
                    cnt_slash += it == '\n' || it == '\t';
            }
            else if(isdigit(operand.front()) || operand.front() == '-') {
                expr.back().type = "int";
                expr.back().val = atoi(operand.c_str());
            }
            else if(operand == "true" || operand == "false") {
                expr.back().type = "boolean";
                expr.back().val = operand == "true" ? true : false;
            }
            else
                err_stk.push_back({ WRONG_CONST_TYPE, cnt_lines });
            ptr += operand.size() + cnt_slash;
            last_op = false;
            last_ident = true;
        }
        else if((operand = get_identifier()) != "" &&
                (log_ops.find(operand) == log_ops.end())) {
            auto it = ID_refs.insert(make_pair(operand, BaseIdent()));
            if(it.second)
                err_stk.push_back({ WRONG_IDENT_NAME, cnt_lines });
            auto elem = it.first->second;
            expr.emplace_back(BaseIdent{ operand, elem.type, elem.val });
            ptr += operand.size();
            last_op = false;
            last_ident = true;
        }
        else if((operand = get_service_op()) != "") {
            bool is_left_as = left_as_ops.find(operand) != left_as_ops.end() && !last_op;
            if(last_op && (operand == "+" || operand == "-"))
                operand += '!';
            while(!stk.empty() && stk.back() != "(" &&
            (is_left_as ? prior[operand] <= prior[stk.back()] : prior[operand] < prior[stk.back()])) {
                expr.emplace_back(BaseIdent());
                expr.back().type = "operation";
                expr.back().name = stk.back();
                stk.pop_back();
            }
            stk.emplace_back(operand);
            ptr += operand.size() - (operand.back() == '!');
            last_op = true;
            last_ident = false;
        }
        else {
            err_stk.push_back({ WRONG_EXPR, cnt_lines });
            return ERROR;
        }
        if(stk.empty())
            break;
    }
    while(!stk.empty()) {
        if(service_ops.find(stk.back()) == service_ops.end())
            err_stk.push_back({ WRONG_EXPR, cnt_lines });
        expr.emplace_back(BaseIdent{ stk.back(), "operator" });
        stk.pop_back();
    }
    if(!is_single_expr)
        ptr--;
    return NOERROR;
}
error if_operator() {
    if(buf[ptr] != '(') {
        err_stk.push_back({ SYNT_NO_OPBRAC, cnt_lines });
        return ERROR;
    }

    vector<BaseIdent> expr_arg;
    if(read_expression(expr_arg) != NOERROR)
        return ERROR;
    if(buf[ptr] != ')') {
        err_stk.push_back({ SYNT_NO_CLBRAC, cnt_lines });
        return ERROR;
    }
    ptr++;
    expr += expr_arg;
    if(!is_logical_expression(expr_arg))
        err_stk.push_back({ WRONG_IF_EXPR, cnt_lines });
    if(!is_compatible_types_expr(expr_arg))
        err_stk.push_back({ WRONG_EXPR_OP_TYPES, cnt_lines });

    expr.emplace_back(BaseIdent{ "jumpif", "operator", 0 });
    size_t jmp1 = expr.size() - 1;
    
    if(operator_wrapper() != NOERROR)
        return ERROR;
    get<int>(expr[jmp1].val) = expr.size();

    ptr++;
    skip_spaces_endl();
    string el = get_service_word();
    if(el != "else") {
        ptr--;
        get_spaces_endl_back();
    }
    else {
        ptr += el.size();
        expr.emplace_back(BaseIdent{ "jump", "operator", 0 });
        size_t jmp2 = expr.size() - 1;
        get<int>(expr[jmp1].val)++;
        if(operator_wrapper() != NOERROR)
            return ERROR;
        get<int>(expr[jmp2].val) = expr.size();
    }
    return NOERROR;
}
error for_operator() {
    if(buf[ptr] != '(') {
        err_stk.push_back({ SYNT_NO_OPBRAC, cnt_lines });
        return ERROR;
    }
    ptr++;

    vector<BaseIdent> expr_arg_1;
    stk.emplace_back("(");
    if(read_expression(expr_arg_1) != NOERROR)
        return ERROR;
    if(buf[ptr] != ';') {
        err_stk.push_back({ SYNT_NO_SEMICOLON_FOR, cnt_lines });
        return ERROR;
    }
    ptr++;
    if(!is_compatible_types_expr(expr_arg_1))
        err_stk.push_back({ WRONG_EXPR_OP_TYPES, cnt_lines });

    vector<BaseIdent> expr_arg_2;
    stk.emplace_back("(");
    if(read_expression(expr_arg_2) != NOERROR)
        return ERROR;
    if(buf[ptr] != ';') {
        err_stk.push_back({ SYNT_NO_SEMICOLON_FOR, cnt_lines });
        return ERROR;
    }
    ptr++;
    if(!expr_arg_2.empty() && !is_logical_expression(expr_arg_2))
        err_stk.push_back({ WRONG_FOR_EXPR, cnt_lines });
    if(!is_compatible_types_expr(expr_arg_2))
        err_stk.push_back({ WRONG_EXPR_OP_TYPES, cnt_lines });

    vector<BaseIdent> expr_arg_3;
    stk.emplace_back("(");
    if(read_expression(expr_arg_3) != NOERROR)
        return ERROR;
    if(buf[ptr] != ')') {
        err_stk.push_back({ SYNT_NO_CLBRAC, cnt_lines });
        return ERROR;
    }
    ptr++;
    if(!is_compatible_types_expr(expr_arg_3))
        err_stk.push_back({ WRONG_EXPR_OP_TYPES, cnt_lines });
    
    size_t jmp_start = expr.size();
    expr += expr_arg_1;
    size_t jmp_cycle = expr.size();
    if(!expr_arg_2.empty())
        expr += expr_arg_2;
    else
        expr.emplace_back(BaseIdent{ "const", "boolean", true });
    expr.emplace_back(BaseIdent{ "jumpif", "operator", 0 });
    size_t jmp_if = expr.size() - 1;
    
    if(operator_wrapper() != NOERROR)
        return ERROR;
    
    size_t jmp_continue = expr.size();
    expr += expr_arg_3;
    expr.emplace_back(BaseIdent{ "jump", "operator", (int)jmp_cycle });
    get<int>(expr[jmp_if].val) = expr.size();
    size_t jmp_break = expr.size();

    while(!continue_stack.empty() && continue_stack.back() > jmp_start) {
        get<int>(expr[continue_stack.back()].val) = jmp_continue;
        continue_stack.pop_back();
    }
    while(!break_stack.empty() && break_stack.back() > jmp_start) {
        get<int>(expr[break_stack.back()].val) = jmp_break;
        break_stack.pop_back();
    }
    return NOERROR;
}
error while_operator() {
    if(buf[ptr] != '(') {
        err_stk.push_back({ SYNT_NO_OPBRAC, cnt_lines });
        return ERROR;
    }
    
    vector<BaseIdent> expr_arg;
    if(read_expression(expr_arg) != NOERROR)
        return ERROR;
    if(buf[ptr] != ')') {
        err_stk.push_back({ SYNT_NO_CLBRAC, cnt_lines });
        return ERROR;
    }
    ptr++;
    if(!is_logical_expression(expr_arg))
        err_stk.push_back({ WRONG_WHILE_EXPR, cnt_lines });
    if(!is_compatible_types_expr(expr_arg))
        err_stk.push_back({ WRONG_EXPR_OP_TYPES, cnt_lines });

    size_t jmp_continue = expr.size(), jmp_start = jmp_continue;
    expr += expr_arg;
    expr.emplace_back(BaseIdent{ "jumpif", "operator", 0 });
    size_t jmp_if = expr.size() - 1;
    
    if(operator_wrapper() != NOERROR)
        return ERROR;

    expr.emplace_back(BaseIdent{ "jump", "operator", (int)jmp_start });
    get<int>(expr[jmp_if].val) = expr.size();
    size_t jmp_break = expr.size();

    while(!continue_stack.empty() && continue_stack.back() > jmp_start) {
        get<int>(expr[continue_stack.back()].val) = jmp_continue;
        continue_stack.pop_back();
    }
    while(!break_stack.empty() && break_stack.back() > jmp_start) {
        get<int>(expr[break_stack.back()].val) = jmp_break;
        break_stack.pop_back();
    }
    return NOERROR;
}
error goto_operator() {
    string id = get_identifier();
    if(id.empty())
        err_stk.push_back({ WRONG_IDENT_NAME, cnt_lines });
    
    expr.emplace_back(BaseIdent{ "jump", "operator", -1 });
    size_t jmp = expr.size() - 1;

    bool is_mark_found = false;
    for(auto& it : GotoMarked)
        if(it.first == id) {
            is_mark_found = true;
            get<int>(expr[jmp].val) = it.second;
            break;
        }
    if(!is_mark_found)
        GotoToMark.emplace_back(make_pair(id, jmp));
    
    ptr += id.size();
    return NOERROR;
}
error read_operator() {
    if(buf[ptr] != '(') {
        err_stk.push_back({ SYNT_NO_OPBRAC, cnt_lines });
        return ERROR;
    }
    ptr++;
    skip_spaces_endl();
    string id = get_identifier();
    if(id.empty() || ID_refs.insert(make_pair(id, BaseIdent())).second ||
       StructNames.find(ID_refs[id].type) != StructNames.end())
        err_stk.push_back({ WRONG_IDENT_NAME, cnt_lines });
    ptr += id.size();
    skip_spaces_endl();
    if(buf[ptr] != ')') {
        err_stk.push_back({ SYNT_NO_CLBRAC, cnt_lines });
        return ERROR;
    }
    ptr++;

    expr.emplace_back(BaseIdent{ id, ID_refs[id].type, ID_refs[id].val });
    expr.emplace_back(BaseIdent{ "read", "operator" });
    return NOERROR;
}
error write_operator() {
    if(buf[ptr] != '(') {
        err_stk.push_back({ SYNT_NO_OPBRAC, cnt_lines });
        return ERROR;
    }
    ptr++;
    int args_cnt = 0;
    while(ptr < buf.size()) {
        skip_spaces_endl();
        vector<BaseIdent> expr_arg_i;
        stk.emplace_back("(");
        if(read_expression(expr_arg_i) != NOERROR)
            return ERROR;
        if((buf[ptr] != ')' && buf[ptr] != ',') || expr_arg_i.empty()) {
            err_stk.push_back({ WRONG_WRITE_EXPR, cnt_lines });
            return ERROR;
        }
        if(!is_compatible_types_expr(expr_arg_i))
            err_stk.push_back({ WRONG_EXPR_OP_TYPES, cnt_lines });
        ptr++;

        expr += expr_arg_i;
        args_cnt++;
        if(buf[ptr - 1] == ')')
            break;
    }
    if(ptr >= buf.size()) {
        err_stk.push_back({ SYNT_NO_SEMICOLON, cnt_lines });
        return ERROR;
    }

    expr.emplace_back(BaseIdent{ "write", "operator", args_cnt });
    return NOERROR;
}
error expr_operator() {
    stk.emplace_back("(");
    vector<BaseIdent> expr_op;
    error err_expr = read_expression(expr_op);
    if(!is_compatible_types_expr(expr_op))
        err_stk.push_back({ WRONG_EXPR_OP_TYPES, cnt_lines });
    expr += expr_op;
    return err_expr == NOERROR ? NOERROR : ERROR;
}
error complex_operator() {
    while(ptr < buf.size() && buf[ptr] != '}') {
        if(operator_wrapper() != NOERROR)
            return ERROR;
        ptr++;
        skip_spaces_endl();
    }
    if(ptr >= buf.size()) {
        err_stk.push_back({ WRONG_OPER, cnt_lines });
        return ERROR;
    }
    return NOERROR;
}
error operator_wrapper() {
    skip_spaces_endl();
    string mark = get_mark();
    bool is_mark = !mark.empty();
    if(is_mark) {
        ptr += mark.size() + 1;
        for(auto& it : GotoToMark)
            if(it.first == mark)
                get<int>(expr[it.second].val) = expr.size();
        GotoMarked.emplace_back(make_pair(mark, expr.size()));
    }

    skip_spaces_endl();
    string op = get_service_word();
    if(cycle_stack.empty() && (op == "break" || op == "continue"))
        err_stk.push_back({ WRONG_BR_CNT, cnt_lines });
    if(service_decl.find(op) != service_decl.end()) {
        err_stk.push_back({ WRONG_DECL_AFTER_SECT, cnt_lines });
        return ERROR;
    }
    ptr += op.size();
    skip_spaces_endl();

    error err_op = NOERROR;
    if(op == "{") {
        err_op = complex_operator();
        if(buf[ptr] != '}') {
            err_stk.push_back({ SYNT_NO_CLBRAC, cnt_lines });
            return ERROR;
        }
    }
    else if(op == "if") {
        err_op = if_operator();
    }
    else if(op == "for") {
        cycle_stack.emplace_back(expr.size());
        err_op = for_operator();
        cycle_stack.pop_back();
    }
    else if(op == "while") {
        cycle_stack.emplace_back(expr.size());
        err_op = while_operator();
        cycle_stack.pop_back();
    }
    else if(op == "goto") {
        err_op = goto_operator();
        if(buf[ptr] != ';') {
            err_stk.push_back({ SYNT_NO_SEMICOLON, cnt_lines });
            return ERROR;
        }
    }
    else if(op == "read") {
        err_op = read_operator();
        if(buf[ptr] != ';') {
            err_stk.push_back({ SYNT_NO_SEMICOLON, cnt_lines });
            return ERROR;
        }
    }
    else if(op == "write") {
        err_op = write_operator();
        if(buf[ptr] != ';') {
            err_stk.push_back({ SYNT_NO_SEMICOLON, cnt_lines });
            return ERROR;
        }
    }
    else if(op != "break" && op != "continue") {
        op = "expr";
        err_op = expr_operator();
    }
    else if(op == "break") {
        break_stack.emplace_back(expr.size());
        expr.emplace_back(BaseIdent{ "jump", "operator", 0 });
    }
    else if(op == "continue") {
        continue_stack.emplace_back(expr.size());
        expr.emplace_back(BaseIdent{ "jump", "operator", 0 });
    }
    if(err_op != NOERROR)
        return ERROR;
    
    if(buf[ptr] != '}' && buf[ptr] != ';') {
        err_stk.push_back({ SYNT_NO_SEMICOLON, cnt_lines });
        return ERROR;
    }
    return NOERROR;
}

error interpret_prn() {
    vector<BaseIdent> prn_stk;
    for(size_t i = 0; i < expr.size(); i++) {
        auto& it = expr[i];
        if(it.type == "int" || it.type == "boolean" || it.type == "string" || 
           StructNames.find(it.type) != StructNames.end()) {
            if(it.name == "const")
                prn_stk.emplace_back(it);
            else
                prn_stk.emplace_back(BaseIdent{ it.name, ID_refs[it.name].type, ID_refs[it.name].val });
        }
        else if(it.type == "operation") {
            if(log_ops.find(it.name) == log_ops.end()) {
                if(it.name == "-")
                    get<int>(prn_stk[prn_stk.size() - 2].val) -= get<int>(prn_stk.back().val);
                else if(it.name == "*")
                    get<int>(prn_stk[prn_stk.size() - 2].val) *= get<int>(prn_stk.back().val);
                else if(it.name == "/")
                    get<int>(prn_stk[prn_stk.size() - 2].val) /= get<int>(prn_stk.back().val);
                else if(it.name == "+!")
                    prn_stk.push_back(BaseIdent());
                else if(it.name == "-!") {
                    get<int>(prn_stk.back().val) *= -1;
                    prn_stk.push_back(BaseIdent());
                }
                else if(it.name == "+") {
                    if(prn_stk.back().type == "int")
                        get<int>(prn_stk[prn_stk.size() - 2].val) += get<int>(prn_stk.back().val);
                    else
                        get<string>(prn_stk[prn_stk.size() - 2].val) += get<string>(prn_stk.back().val);
                }
                else if(it.name == "=") {
                    auto& op1 = prn_stk[prn_stk.size() - 2];
                    op1.val = prn_stk.back().val;
                    ID_refs[op1.name].val = prn_stk.back().val;
                    size_t pos;
                    if(StructNames.find(op1.type) != StructNames.end()) {
                        auto& lines = get<vector<BaseIdent>>(ID_refs[op1.name].val);
                        auto& values = get<vector<BaseIdent>>(prn_stk.back().val);
                        for(size_t i = 0; i < lines.size(); i++) {
                            string name = op1.name + "." + lines[i].name;
                            ID_refs[name].val = values[i].val;
                        }
                    }
                    else if((pos = op1.name.find('.')) != op1.name.npos) {
                        string s_name = op1.name.substr(0, pos);
                        string line = op1.name.substr(pos + 1, op1.name.size() - pos - 1);
                        auto& lines = get<vector<BaseIdent>>(ID_refs[s_name].val);
                        for(auto& line_name : lines)
                            if(line_name.name == line) {
                                line_name.val = ID_refs[op1.name].val;
                                break;
                            }
                    }
                }
                prn_stk.pop_back();
            }
            else {
                bool res;
                if(it.name == "<") {
                    if(prn_stk.back().type == "int")
                        res = get<int>(prn_stk[prn_stk.size() - 2].val) < get<int>(prn_stk.back().val);
                    else if(prn_stk.back().type == "boolean")
                        res = get<bool>(prn_stk[prn_stk.size() - 2].val) < get<bool>(prn_stk.back().val);
                    else
                        res = get<string>(prn_stk[prn_stk.size() - 2].val) < get<string>(prn_stk.back().val);
                }
                else if(it.name == "<=") {
                    if(prn_stk.back().type == "int")
                        res = get<int>(prn_stk[prn_stk.size() - 2].val) <= get<int>(prn_stk.back().val);
                    else if(prn_stk.back().type == "boolean")
                        res = get<bool>(prn_stk[prn_stk.size() - 2].val) <= get<bool>(prn_stk.back().val);
                    else
                        res = get<string>(prn_stk[prn_stk.size() - 2].val) <= get<string>(prn_stk.back().val);
                }
                else if(it.name == ">") {
                    if(prn_stk.back().type == "int")
                        res = get<int>(prn_stk[prn_stk.size() - 2].val) > get<int>(prn_stk.back().val);
                    else if(prn_stk.back().type == "boolean")
                        res = get<bool>(prn_stk[prn_stk.size() - 2].val) > get<bool>(prn_stk.back().val);
                    else
                        res = get<string>(prn_stk[prn_stk.size() - 2].val) > get<string>(prn_stk.back().val);
                }
                else if(it.name == ">=") {
                    if(prn_stk.back().type == "int")
                        res = get<int>(prn_stk[prn_stk.size() - 2].val) >= get<int>(prn_stk.back().val);
                    else if(prn_stk.back().type == "boolean")
                        res = get<bool>(prn_stk[prn_stk.size() - 2].val) >= get<bool>(prn_stk.back().val);
                    else
                        res = get<string>(prn_stk[prn_stk.size() - 2].val) >= get<string>(prn_stk.back().val);
                }
                else if(it.name == "!=") {
                    if(prn_stk.back().type == "int")
                        res = get<int>(prn_stk[prn_stk.size() - 2].val) != get<int>(prn_stk.back().val);
                    else if(prn_stk.back().type == "boolean")
                        res = get<bool>(prn_stk[prn_stk.size() - 2].val) != get<bool>(prn_stk.back().val);
                    else
                        res = get<string>(prn_stk[prn_stk.size() - 2].val) != get<string>(prn_stk.back().val);
                }
                else if(it.name == "==") {
                    if(prn_stk.back().type == "int")
                        res = get<int>(prn_stk[prn_stk.size() - 2].val) == get<int>(prn_stk.back().val);
                    else if(prn_stk.back().type == "boolean")
                        res = get<bool>(prn_stk[prn_stk.size() - 2].val) == get<bool>(prn_stk.back().val);
                    else
                        res = get<string>(prn_stk[prn_stk.size() - 2].val) == get<string>(prn_stk.back().val);
                }
                else if(it.name == "and") {
                    res = get<bool>(prn_stk[prn_stk.size() - 2].val) && get<bool>(prn_stk.back().val);
                }
                else if(it.name == "or") {
                    if(prn_stk.back().type == "boolean")
                        res = get<bool>(prn_stk[prn_stk.size() - 2].val) || get<bool>(prn_stk.back().val);
                }
                else if(it.name == "not") {
                    res = !get<bool>(prn_stk.back().val);
                    prn_stk.push_back(BaseIdent());
                }
                prn_stk.pop_back();
                prn_stk.pop_back();
                prn_stk.emplace_back(BaseIdent{ "const", "boolean", res });
            }
        }
        else if(it.type == "operator") {
            if(it.name == "jump")
                i = get<int>(it.val) - 1;
            else if(it.name == "jumpif") {
                auto& arg = prn_stk.back();
                if(!get<bool>(arg.val))
                    i = get<int>(it.val) - 1;
                prn_stk.pop_back();
            }
            else if(it.name == "write") {
                int cnt = get<int>(it.val);
                while(cnt) {
                    auto& op = prn_stk[prn_stk.size() - cnt];
                    if(op.type == "int")
                        cout << get<int>(op.val);
                    else if(op.type == "string")
                        cout << get<string>(op.val);
                    else if(op.type == "boolean")
                        get<bool>(op.val) ? cout << "true" : cout << "false";
                    else if(StructNames.find(op.type) != StructNames.end()) {
                        auto& v = get<vector<BaseIdent>>(op.val);
                        for(auto& s_it : v) {
                            if(s_it.type == "int")
                                cout << s_it.name << ": " << get<int>(s_it.val) << endl;
                            else if(s_it.type == "string")
                                cout << s_it.name << ": " << get<string>(s_it.val) << endl;
                            else if(s_it.type == "boolean")
                                cout << s_it.name << ": "
                                 << (get<bool>(s_it.val) ? "true" : "false") << endl;
                        }
                    }
                    else {
                        err_stk.push_back({ WRONG_WRITE_EXPR_TYPE, 0 });   
                        return ERROR;
                    }
                    cnt--;
                }
                cnt = get<int>(it.val);
                while(cnt--)
                    prn_stk.pop_back();
            }
            else if(it.name == "read") {
                auto& op = prn_stk.back();
                if(op.type == "int")
                    cin >> get<int>(ID_refs[op.name].val);
                else if(op.type == "string")
                    cin >> get<string>(ID_refs[op.name].val);
                else if(op.type == "boolean")
                    cin >> get<bool>(ID_refs[op.name].val);
                size_t pos;
                if((pos = op.name.find('.')) != op.name.npos) {
                    string s_name = op.name.substr(0, pos);
                    string line = op.name.substr(pos + 1, op.name.size() - pos - 1);
                    auto& lines = get<vector<BaseIdent>>(ID_refs[s_name].val);
                    for(auto& line_name : lines)
                        if(line_name.name == line) {
                            line_name.val = ID_refs[op.name].val;
                            break;
                        }
                }
                prn_stk.pop_back();
            }
        }
    }
    return NOERROR;
}

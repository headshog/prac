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
    LEX_NOPROG = -4,
    LEX_NO_OPBRAC = -5,
    LEX_NO_CLBRAC = -6,
    LEX_NO_SEMICOLON = -7,
    LEX_NO_SEMICOLON_FOR = -8,
    COMM_NOCLOSE = -9,
    WRONG_DECL_TYPE = -10,
    WRONG_CONST_TYPE = -11,
    WRONG_IDENT_NAME = -12,
    WRONG_DECL = -13,
    WRONG_OPER = -14,
    PREV_DECL = -15,
    WRONG_IF_EXPR = -16,
    WRONG_READ_EXPR = -17,
    WRONG_WRITE_EXPR = -18,
    WRONG_FOR_EXPR = -19,
    WRONG_EXPR = -20,
    WRONG_BR_CNT = -21
};

error error_wrapper(error e) {
    switch (e) {
        case FILE_NO:
            cout << "No filename given.\n";
            break;
        case FILE_BAD:
            cout << "Cannot open given file.\n";
            break;
        case UNIDENT_SYMBOL:
            cout << "Unidentified symbol found.\n";
            break;
        case LEX_NOPROG:
            cout << "Lexical error: no word 'program' in the beginning.\n";
            break;
        case LEX_NO_OPBRAC:
            cout << "Lexical error: no opening bracket '{' or '(' in operator.\n";
            break;
        case LEX_NO_CLBRAC:
            cout << "Lexical error: no closing bracket '}' or ')' in operator.\n";
            break;
        case LEX_NO_SEMICOLON:
            cout << "Lexical error: no ';' in the end of operator.\n";
            break;
        case LEX_NO_SEMICOLON_FOR:
            cout << "Lexical error: no ';' in the middle of operator for.\n";
            break;
        case COMM_NOCLOSE:
            cout << "Comment is not closed.\n";
            break;
        case WRONG_DECL_TYPE:
            cout << "Wrong declaration type.\n";
            break;
        case WRONG_CONST_TYPE:
            cout << "Constant type does not match the variable type.\n";
            break;
        case WRONG_IDENT_NAME:
            cout << "Wrong identifier name.\n";
            break;
        case WRONG_DECL:
            cout << "Wrong syntax in declaration.\n";
            break;
        case WRONG_OPER:
            cout << "Wrong operator.\n";
            break;
        case PREV_DECL:
            cout << "Variable is previously declared.\n";
            break;
        case WRONG_IF_EXPR:
            cout << "Wrong expression in if operator.\n";
            break;
        case WRONG_READ_EXPR:
            cout << "Wrong expression in read operator.\n";
            break;
        case WRONG_WRITE_EXPR:
            cout << "Wrong expression in write operator.\n";
            break;
        case WRONG_FOR_EXPR:
            cout << "Wrong expression if for operator.\n";
            break;
        case WRONG_EXPR:
            cout << "Wrong expression in operator.\n";
            break;
        case WRONG_BR_CNT:
            cout << "Using of operator break or continue not in cycle operators.\n";
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
    "if", "while", "read", "write", "break", "for", "{", "goto", "continue", "else", "}", ";"
};
unordered_set<string> service_decl {
    "int", "string", "boolean", "struct"
};
unordered_set<string> service_ops {
    "<", ">", "<=", ">=", "!=", "==", "and", "or", "not", "+", "-", "=", "*", "/"
};
unordered_set<string> log_ops {
    "<", ">", "<=", ">=", "!=", "==", "and", "or", "not"
};
unordered_set<string> left_as_ops {
    "*", "/", "+", "-", "<", ">", "<=", ">=", "==", "!=", "and", "or"
};
unordered_set<string> right_as_ops {
    "+", "-", "not", "="
};
unordered_map<string, int> prior = {
    {"+!", 1}, {"-!", 1}, {"not", 1}, {"*", 2}, {"/", 2}, {"+", 3}, 
    {"-", 3}, {"<", 4}, {">", 4}, {"<=", 4}, {">=", 4}, {"==", 5}, 
    {"!=", 5}, {"and", 6}, {"or", 7}, {"=", 8}
};
unordered_set<char> service_alf {
    ' ', ';', '=', '\n', '\t', '<', '>', '!', '+', '-', '*', '.', ':', ',',
    '(', ')', '{', '}', '/', '\"'
};
unordered_set<char> ident_stop {
    ' ', ';', '=', '\n', '\t', ')', '<', '>', '!', '+', '-', '*', ':', ','
};

/* Structures for semantic checking and parsing to */
/* language interpreter. */
struct BaseIdent {
    string name, type;
    variant<int, bool, string, vector<BaseIdent>> val;
};
vector<BaseIdent> ID;
unordered_map<string, BaseIdent> ID_refs;

struct AstOperator {
    string name, mark;
};
struct ComplexOperator : AstOperator {
    vector<AstOperator*> ops;
};
struct IfOperator : AstOperator {
    vector<BaseIdent> expr;
    AstOperator *op1, *op2;
    bool is_else;
};
struct ForOperator : AstOperator {
    vector<BaseIdent> expr1, expr2, expr3;
    AstOperator* op;
};
struct WhileOperator : AstOperator {
    vector<BaseIdent> expr;
    AstOperator* op;
};
struct GotoOperator : AstOperator {
    string mark_to;
};
struct ReadOperator : AstOperator {
    string Ident;
};
struct WriteOperator : AstOperator {
    vector<vector<BaseIdent>> expr;
};
struct ExprOperator : AstOperator {
    vector<BaseIdent> expr;
};
ComplexOperator* OP;
unordered_map<string, AstOperator*> GOTO;
vector<string> stk;

/* Func prototypes */
void skip_spaces_endl();
error erase_comm_check();
string get_service_word();
string get_service_op();
string get_identifier();
string get_mark();
string get_value();

error check_lex_synt_sem();

error get_id_and_val(vector<BaseIdent>& ID, string& decl_type, string id);
error declaration(vector<BaseIdent>& ID, string id = "");

bool is_logical_expression(vector<BaseIdent>& expr);
error read_expression(vector<BaseIdent>& expr, bool is_single_expr = false);
error if_operator(IfOperator* op);
error for_operator(ForOperator* op);
error while_operator(WhileOperator* op);
error goto_operator(GotoOperator* op);
error read_operator(ReadOperator* op);
error write_operator(WriteOperator* op);
error expr_operator(ExprOperator* op);
error complex_operator(ComplexOperator *OP);
error operator_wrapper(AstOperator*& OP, bool cycle = false);

/*  
СДЕЛАТЬ:
1. СТОК ОШИБОК, ДАВАТЬ КОНКРЕТНЫЕ МЕСТА ОШИБКИ
2. ПАРСИНГ BREAK И CONTINUE
3. ДОБАВИТЬ В ПРОВЕРКУ is_logical_expression СЛУЧАЙ boolean = boolean
3. ТЕСТИРОВАНИЕ ТОГО, ЧТО СДЕЛАНО
4. ПАРСИНГ ВСЕЙ СТРУКТУРЫ ОПРЕРАТОРОВ В ПОЛИЗ
5. ВО ВРЕМЯ ПАРСИНГА ПРОВЕРЯТЬ ОПЕРАНДЫ НА СОВМЕСТИМОСТЬ ТИПОВ
6. В ОПЕРАТОРЕ GOTO ПРОВЕРЯТЬ МЕТКИ НА ПРАВИЛЬНОСТЬ
7. ИНТЕРПРЕТАТОР
*/
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

    error lex_err = check_lex_synt_sem();
    if(lex_err != NOERROR)
        return error_wrapper(lex_err);
    
    return NOERROR;
}

void skip_spaces_endl() {
    while(ptr < buf.size() && (buf[ptr] == ' ' || buf[ptr] == '\n' || buf[ptr] == '\t')) ptr++;
}
error erase_comm_check() {
    string tmp_buf;
    bool add_symb = true, str = false;
    for(size_t i = 0; i < buf.size(); i++) {
        if(add_symb && buf[i] == '\"')
            str = !str;
        
        if(!isdigit(buf[i]) && !isalpha(buf[i]) && service_alf.find(buf[i]) == service_alf.end())
            return UNIDENT_SYMBOL;

        if(!str && buf[i] == '/' && i + 1 != buf.size() && buf[i + 1] == '*')
            add_symb = false, i += 2;
        else if(!add_symb) {
            if(i + 1 == buf.size())
                return COMM_NOCLOSE;
            if(buf[i] == '*' && buf[i + 1] == '/')
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
    return tmp_ptr <= buf.size() && (service_operators.find(w) != service_operators.end() ||
                                     service_decl.find(w) != service_decl.end()) ? w : "";
}
string get_service_op() {
    string w;
    size_t tmp_ptr = ptr;
    bool alpha = isalpha(buf[tmp_ptr]);
    while(tmp_ptr < buf.size() && buf[tmp_ptr] != ' ' && buf[tmp_ptr] != '\n' && 
          buf[tmp_ptr] != '(' && buf[tmp_ptr] != '\t' && !isdigit(buf[tmp_ptr]) &&
          buf[tmp_ptr] != '\"' && 
          ((alpha && isalpha(buf[tmp_ptr])) || (!alpha && !isalpha(buf[tmp_ptr]))))
            w += buf[tmp_ptr++];
        
    return tmp_ptr < buf.size() && (service_ops.find(w) != service_ops.end()) ? w : "";
}
string get_identifier() {
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
        if(buf[tmp_ptr] == '-') 
            val += '-', tmp_ptr++;
        while(tmp_ptr < buf.size() && isdigit(buf[tmp_ptr]))
            val += buf[tmp_ptr++];
        return tmp_ptr < buf.size() ? val : "";
    }
    else {
        if(tmp_ptr + 5 >= buf.size() || 
           (buf.substr(tmp_ptr, 4) != "true" && buf.substr(tmp_ptr, 5) != "false"))
            return "";
        return buf.substr(tmp_ptr, 4) == "true" ? "true" : "false";
    }
}

error check_lex_synt_sem() {
    error comm_err = erase_comm_check();
    if(comm_err != NOERROR)
        return comm_err;

    skip_spaces_endl();
    if(buf.size() < 7 || buf.substr(ptr, 7) != "program")
        return LEX_NOPROG;
    ptr += 7;

    skip_spaces_endl();
    if(ptr >= buf.size() || buf[ptr] != '{')
        return LEX_NO_OPBRAC;
    ptr++;

    skip_spaces_endl();
    if(ptr == buf.size())
        return LEX_NO_CLBRAC;
    
    error decl_err = declaration(ID);
    if(decl_err != NOERROR)
        return decl_err;

    skip_spaces_endl();
    OP = new ComplexOperator;
    OP->mark = "";
    OP->name = "{";
    error operator_err = complex_operator(OP);
    if(operator_err != NOERROR)
        return operator_err;
    
    skip_spaces_endl();
    if(ptr >= buf.size() || buf[ptr] != '}')
        return LEX_NO_CLBRAC;

    return NOERROR;
}

error get_id_and_val(vector<BaseIdent>& ID, string& decl_type, string id) {
    string identifier, value;
    skip_spaces_endl();
    identifier = get_identifier();
    if(identifier.empty())
        return WRONG_IDENT_NAME;
    ptr += identifier.size();
    if(!id.empty())
        identifier = id + "." + identifier;
    ID.emplace_back(BaseIdent{ identifier, decl_type });
    if(decl_type == "string")
        ID.back().val = "";
    else if(decl_type == "int")
        ID.back().val = 0;
    else if(decl_type == "boolean")
        ID.back().val = false;

    skip_spaces_endl();
    if(ptr >= buf.size() || (buf[ptr] != ';' && buf[ptr] != '=' && buf[ptr] != '{' && buf[ptr] != ','))
        return WRONG_DECL;
    if(buf[ptr] == '=') {
        ptr++;
        skip_spaces_endl();
        value = get_value();
        ptr += value.size();
        skip_spaces_endl();
        if(value.empty() || ptr >= buf.size() || (buf[ptr] != ';' && buf[ptr] != ','))
            return WRONG_DECL;
        if((decl_type == "string" && value.front() != '\"') ||
           (decl_type == "int" && (value.front() != '-' && !isdigit(value.front()))) || 
           (decl_type == "boolean" && !(value == "true" || value == "false")))
           return WRONG_CONST_TYPE;
        
        if(decl_type == "string")
            ID.back().val = value.substr(1, value.size() - 2);
        else if(decl_type == "int")
            ID.back().val = atoi(value.c_str());
        else
            ID.back().val = (value == "true" ? true : false);
    }
    else if(buf[ptr] == '{') {
        ptr++;
        skip_spaces_endl();
        ID.back().val = vector<BaseIdent>();
        error decl_err = declaration(get<vector<BaseIdent>>(ID.back().val), identifier);
        if(decl_err != NOERROR)
            return decl_err;
        if(buf[ptr] != '}')
            return LEX_NO_CLBRAC;
        ptr++;
        return buf[ptr] == ';' ? NOERROR : WRONG_DECL;
    }
    if(!ID_refs.insert(make_pair(identifier, ID.back())).second)
        return PREV_DECL;
    if(buf[ptr] == ',') {
        ptr++;
        error err_id_val = get_id_and_val(ID, decl_type, id);
        if(err_id_val != NOERROR)
            return err_id_val;
    }
    return NOERROR;
}
error declaration(vector<BaseIdent>& ID, string id) {
    string decl_type = get_service_word();
    while(service_operators.find(decl_type) == service_operators.end()) {
        if(decl_type.empty() || service_decl.find(decl_type) == service_decl.end())
            return WRONG_DECL_TYPE;
        ptr += decl_type.size();
        error err_get_id = get_id_and_val(ID, decl_type, id);
        if(err_get_id != NOERROR)
            return err_get_id;
        if(buf[ptr] != ';')
            return LEX_NO_SEMICOLON;

        ptr++;
        skip_spaces_endl();
        decl_type = get_service_word();
    }
    return NOERROR;
}

bool is_logical_expression(vector<BaseIdent>& expr) {
    for(auto& it : expr) {
        /*if(it.name == "=") CASE OF BOOLEAN = BOOLEAN */
        if(log_ops.find(it.name) != log_ops.end() || it.type == "boolean")
            return true;
    }
    return false;
}
error read_expression(vector<BaseIdent>& expr, bool is_single_expr) {
    string operand;
    while(ptr < buf.size()) {
        skip_spaces_endl();
        if(buf[ptr] == '(')
            stk.emplace_back("("), ptr++;
        else if(buf[ptr] == ')' || buf[ptr] == ';' || buf[ptr] == ',') {
            while(!stk.empty() && stk.back() != "(") {
                expr.emplace_back(BaseIdent());
                expr.back().type = "operator";
                expr.back().name = stk.back();
                stk.pop_back();
            }
            if(stk.empty())
                return WRONG_EXPR;
            stk.pop_back();
            if(!is_single_expr || (is_single_expr && buf[ptr] != ';'))
                ptr++;
        }
        else if((operand = get_value()) != "") {
            expr.emplace_back(BaseIdent());
            if(operand.front() == '\"') {
                expr.back().type = "string";
                expr.back().val = operand;
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
                return WRONG_CONST_TYPE;
            ptr += operand.size();
        }
        else if((operand = get_identifier()) != "") {
            auto it = ID_refs.insert(make_pair(operand, BaseIdent()));
            if(it.second)
                return WRONG_IDENT_NAME;
            auto elem = it.first->second;
            expr.emplace_back(BaseIdent{ elem.name, elem.type, elem.val });
            ptr += operand.size();
        }
        else if((operand = get_service_op()) != "") {
            bool is_left_as = left_as_ops.find(operand) == left_as_ops.end();
            bool cond = is_left_as ? prior[operand] <= prior[stk.back()] : prior[operand] < prior[stk.back()];
            while(cond) {
                expr.emplace_back(BaseIdent());
                expr.back().type = "operator";
                expr.back().name = stk.back();
                stk.pop_back();
                cond = is_left_as ? prior[operand] <= prior[stk.back()] : prior[operand] < prior[stk.back()];
            }
            stk.emplace_back(operand);
            ptr += operand.size();
        }
        else
            return WRONG_EXPR;
        if(stk.empty())
            break;
    }
    while(!stk.empty()) {
        if(service_ops.find(stk.back()) == service_ops.end())
            return WRONG_EXPR;
        expr.emplace_back(BaseIdent{ stk.back(), "operator" });
        stk.pop_back();
    }
    return NOERROR;
}
error if_operator(IfOperator* op) {
    if(buf[ptr] != '(')
        return LEX_NO_OPBRAC;

    error err_expr = read_expression(op->expr);
    if(err_expr != NOERROR)
        return err_expr;
    if(!is_logical_expression(op->expr))
        return WRONG_IF_EXPR;

    error err_op = operator_wrapper(op->op1);
    if(err_op != NOERROR)
        return err_op;
    
    ptr++;
    skip_spaces_endl();
    string el = get_service_word();
    if(el != "else") {
        op->is_else = false;
        op->op2 = nullptr;
    }
    else {
        op->is_else = true;
        ptr += el.size();
        err_op = operator_wrapper(op->op2);
        if(err_op != NOERROR)
            return err_op;
    }
    return NOERROR;
}
error for_operator(ForOperator* op) {
    if(buf[ptr] != '(')
        return LEX_NO_OPBRAC;
    ptr++;

    stk.push_back("(");
    error err_expr = read_expression(op->expr1);
    if(err_expr != NOERROR)
        return err_expr;

    stk.push_back("(");
    err_expr = read_expression(op->expr2);
    if(err_expr != NOERROR)
        return err_expr;
    if(!is_logical_expression(op->expr2))
        return WRONG_FOR_EXPR;

    stk.push_back("(");
    err_expr = read_expression(op->expr3);
    if(err_expr != NOERROR)
        return err_expr;

    error err_op = operator_wrapper(op->op, true);
    if(err_op != NOERROR)
        return err_op;
    return NOERROR;
}
error while_operator(WhileOperator* op) {
    if(buf[ptr] != '(')
        return LEX_NO_OPBRAC;

    error err_expr = read_expression(op->expr);
    if(err_expr != NOERROR)
        return err_expr;

    error err_op = operator_wrapper(op->op, true);
    if(err_op != NOERROR)
        return err_op;
    return NOERROR;
}
error goto_operator(GotoOperator* op) {
    string id = get_identifier();
    if(id.empty())
        return WRONG_IDENT_NAME;
    op->mark_to = id;
    ptr += id.size();
    return NOERROR;
}
error read_operator(ReadOperator* op) {
    if(buf[ptr] != '(')
        return LEX_NO_OPBRAC;
    ptr++;
    skip_spaces_endl();
    string id = get_identifier();
    if(id.empty() || ID_refs.insert(make_pair(id, BaseIdent())).second)
        return WRONG_IDENT_NAME;
    ptr += id.size();
    skip_spaces_endl();
    if(buf[ptr] != ')')
        return LEX_NO_CLBRAC;
    ptr++;
    return NOERROR;
}
error write_operator(WriteOperator* op) {
    if(buf[ptr] != '(')
        return LEX_NO_OPBRAC;
    while(ptr < buf.size() && buf[ptr] != ';') {
        ptr++;
        op->expr.emplace_back(vector<BaseIdent>());
        stk.push_back("(");
        error err_expr = read_expression(op->expr.back());
        if(err_expr != NOERROR)
            return err_expr;
    }
    return ptr < buf.size() ? NOERROR : LEX_NO_SEMICOLON;
}
error expr_operator(ExprOperator* op) {
    stk.push_back("(");
    return read_expression(op->expr, true);
}
error complex_operator(ComplexOperator* OP) {
    while(ptr < buf.size() && buf[ptr] != '}') {
        OP->ops.emplace_back(nullptr);
        error err_op_wrap = operator_wrapper(OP->ops.back());
        if(err_op_wrap != NOERROR)
            return err_op_wrap;
        ptr++;
        skip_spaces_endl();
    }
    return ptr < buf.size() ? NOERROR : WRONG_OPER;
}
error operator_wrapper(AstOperator*& OP, bool cycle) {
    skip_spaces_endl();
    string mark = get_mark();
    bool is_mark = !mark.empty();
    if(is_mark)
        ptr += mark.size() + 1;

    skip_spaces_endl();
    string op = get_service_word();
    if(!cycle && (op == "break" || op == "continue"))
        return WRONG_BR_CNT;
    ptr += op.size();
    skip_spaces_endl();

    error err_op = NOERROR;
    if(op == "{") {
        OP = new ComplexOperator;
        err_op = complex_operator(reinterpret_cast<ComplexOperator*>(OP));
        if(buf[ptr] != '}')
            return LEX_NO_CLBRAC;
    }
    else if(op == "if") {
        OP = new IfOperator;
        err_op = if_operator(reinterpret_cast<IfOperator*>(OP));
    }
    else if(op == "for") {
        OP = new ForOperator;
        err_op = for_operator(reinterpret_cast<ForOperator*>(OP));
    }
    else if(op == "while") {
        OP = new WhileOperator;
        err_op = while_operator(reinterpret_cast<WhileOperator*>(OP));
    }
    else if(op == "goto") {
        OP = new GotoOperator;
        err_op = goto_operator(reinterpret_cast<GotoOperator*>(OP));
        if(buf[ptr] != ';')
            return LEX_NO_SEMICOLON;
    }
    else if(op == "read") {
        OP = new ReadOperator;
        err_op = read_operator(reinterpret_cast<ReadOperator*>(OP));
        if(buf[ptr] != ';')
            return LEX_NO_SEMICOLON;
    }
    else if(op == "write") {
        OP = new WriteOperator;
        err_op = write_operator(reinterpret_cast<WriteOperator*>(OP));
        if(buf[ptr] != ';')
            return LEX_NO_SEMICOLON;
    }
    else if(op != "break" && op != "continue") {
        OP = new ExprOperator;
        op = "expr";
        err_op = expr_operator(reinterpret_cast<ExprOperator*>(OP));
    }
    if(err_op != NOERROR)
        return err_op;
    OP->mark = mark;
    if(mark != "")
        GOTO.insert(make_pair(mark, OP));
    OP->name = op;
    skip_spaces_endl();
    return buf[ptr] == '}' || buf[ptr] == ';' ? NOERROR : LEX_NO_SEMICOLON;
}
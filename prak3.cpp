#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <ctype.h>
#include <stack>
using namespace std;

bool is_lex_first_on_line, is_labeled_operator;

enum type_of_lex {
    LEX_NULL, LEX_PROG, LEX_IF, LEX_ELSE, LEX_WHILE, LEX_READ, 
    LEX_WRITE, LEX_NOT, LEX_AND, LEX_OR, LEX_GOTO, LEX_BOOLEAN, 
    LEX_INT, LEX_STRING, LEX_TRUE, LEX_FALSE, LEX_BREAK, LEX_FIN, 

    LEX_SEMICOLON, LEX_COMMA, LEX_COLON, LEX_LPAREN, LEX_RPAREN, LEX_EQ, LEX_LSS, 			
    LEX_GTR, LEX_PLUS, LEX_MINUS, LEX_TIMES, LEX_SLASH, LEX_LEQ, LEX_NEQ, LEX_GEQ,     	
    LEX_DEQ, LEX_TSLASH, LEX_SLASHT, LEX_PERCENT, LEX_BEGIN, LEX_END, 

    LEX_QUOTE, LEX_ID, LEX_NUM, POLIZ_LABEL, 
    POLIZ_ADDRESS, POLIZ_GO, POLIZ_FGO 
};


class Lex {
    type_of_lex t_lex;
    int int_lex;
    string str_lex;
public:
    Lex(type_of_lex t_lex_ = LEX_NULL, int int_lex_ = 0, const string str_lex_ = "") {
        t_lex = t_lex_;
        int_lex = int_lex_;
        switch(t_lex) {
        case LEX_NULL:
            str_lex = "NULL";
            break;
        case LEX_PROG:
            str_lex = "PROG";
            break;
        case LEX_IF:
            str_lex = "IF";
            break;
        case LEX_ELSE:
            str_lex = "ELSE";
            break;
        case LEX_WHILE:
            str_lex = "WHILE";
            break;
        case LEX_READ:
            str_lex = "READ";
            break;
        case LEX_WRITE:
            str_lex = "WRITE";
            break;
        case LEX_NOT:
            str_lex = "NOT";
            break;
        case LEX_AND:
            str_lex = "AND";
            break;
        case LEX_OR:
            str_lex = "OR";
            break;
        case LEX_GOTO:
            str_lex = "GOTO";
            break;
        case LEX_BOOLEAN:
            str_lex = "BOOLEAN";
            break;
        case LEX_INT:
            str_lex = "INT";
            break;
        case LEX_STRING:
            str_lex = "STRING";
            break;
        case LEX_TRUE:
            str_lex = "TRUE";
            break;
        case LEX_FALSE:
            str_lex = "FALSE";
            break;
        case LEX_BREAK:
            str_lex = "BREAK";
            break;
        case LEX_FIN:
            str_lex = "FIN";
            break;
        case LEX_SEMICOLON:
            str_lex = "SEMICOLON";
            break;
        case LEX_COMMA:
            str_lex = "COMMA";
            break;
        case LEX_COLON:
            str_lex = "COLON";
            break;
        case LEX_LPAREN:
            str_lex = "LPAREN";
            break;
        case LEX_RPAREN:
            str_lex = "RPAREN";
            break;
        case LEX_EQ:
            str_lex = "EQ";
            break;
        case LEX_LSS:
            str_lex = "LSS";
            break;
        case LEX_GTR:
            str_lex = "GTR";
            break;
        case LEX_PLUS:
            str_lex = "PLUS";
            break;
        case LEX_MINUS:
            str_lex = "MINUS";
            break;
        case LEX_TIMES:
            str_lex = "TIMES";
            break;
        case LEX_SLASH:
            str_lex = "SLASH";
            break;
        case LEX_LEQ:
            str_lex = "LEQ";
            break;
        case LEX_NEQ:
            str_lex = "NEQ";
            break;
        case LEX_GEQ:
            str_lex = "GEQ";
            break;
        case LEX_DEQ:
            str_lex = "DEQ";
            break;
        case LEX_TSLASH:
            str_lex = "TSLASH";
            break;
        case LEX_SLASHT:
            str_lex = "SLASHT";
            break;
        case LEX_PERCENT:
            str_lex = "PERCENT";
            break;
        case LEX_BEGIN:
            str_lex = "BEGIN";
            break;
        case LEX_END:
            str_lex = "END";
            break;
        case LEX_QUOTE:
            str_lex = str_lex_;
            break;
        case LEX_ID:
            str_lex = "ID";
            break;
        case LEX_NUM:
            str_lex = "NUM";
            break;
        case POLIZ_LABEL:
            str_lex = "POLIZ_LABEL";
            break;
        case POLIZ_ADDRESS:
            str_lex = "POLIZ_ADDRESS";
            break;
        case POLIZ_GO:
            str_lex = "POLIZ_GO";
            break;
        case POLIZ_FGO:
            str_lex = "POLIZ_FGO";
            break;
        }
    }

    type_of_lex get_type() const { return t_lex; }

    int get_value() const { return int_lex; }

    string get_string_value() const { return str_lex; } 

    friend ostream& operator << (ostream& os, Lex l) {
        os << '(' << l.t_lex << ',' << l.int_lex << ',' << l.str_lex << ");";
        return os;
    }
};


class Ident {
    string name;
    bool declare;
    type_of_lex type;
    bool assign;
    int int_value;
    string str_value;
    bool bool_value;
public:
    Ident() {
        declare = false;
        assign = false;
    }

    Ident(const string name_) {
        name = name_;
        declare = false;
        assign = false;
    }

    bool operator== (const string& str) const {
        return name == str;
    }

    string get_name() const { return name; }

    bool get_declare() const { return declare; }

    void put_declare() { declare = true; }

    type_of_lex get_type() const { return type; }

    void put_type(type_of_lex t) { type = t; }

    bool get_assign() const { return assign; }

    void put_assign() { assign = true; }

    int get_int_value() const { return int_value; }

    void put_int_value(int v) { int_value = v; }

    string get_string_value() const { return str_value; }

    void put_string_value(const string& v) { str_value = v; }

    bool get_bool_value() const { return bool_value; }

    void put_bool_value(bool v) { bool_value = v; }

    friend ostream& operator<<(ostream& os, Ident id) {
        if (id.declare) {
            os << id.name << ' ' << Lex(id.type).get_string_value();
            if (id.assign) {
                os << ' ';
                if (id.type == LEX_INT)
                    os << id.int_value;
                else if (id.type == LEX_STRING)
                    os << id.str_value;
                else 
                    os << id.bool_value;
            }
        } 
        return os;
    }
};

vector <Ident> TID;

int put(const string& buf) {
    vector<Ident>::iterator it;
    if ((it = find(TID.begin(), TID.end(), buf)) != TID.end())
        return it - TID.begin();
    TID.push_back(Ident(buf));
    return TID.size() - 1;
}

class Scanner {
    FILE * fp;
    char   c;
    int look(const string buf, const char ** list) {
        int i = 0;
        for (;list[i]; ++i) 
            if (buf == list[i])
                return i;
        return 0;
    }

    void gc () {
        c = fgetc(fp);
    }

public:
    static const char * TW [], * TD [];
    Scanner(const char * program) {
        if (!(fp = fopen (program, "r" ))) 
            throw  "can’t open file" ;
    }
    Lex get_lex();
};

const char *
Scanner::TW    [] = { "", "program", "if", "else", "while", "read", "write", "not", "and", "or", "goto", "boolean", "int",
                      "string", "true", "false", "break", NULL };
 
const char *
Scanner::TD    [] = { 
    "", ";", ",", ":", "(", ")", "=", "<", ">", "+", "-", "*", 
	"/","<=", "!=", ">=", "==", "*/", "/*", "%", "{", "}", NULL };

Lex Scanner::get_lex () {
    enum    state { H, IDENT, QUOTE, NUMB, SLASH, COM, ALE, NEQ };
    int     d, j;
    string  buf;
    state   CS = H;
    do {
        gc();
        switch (CS) {
            case H:
                if (c == ' ' || c == '\n' || c == '\r' || c == '\t'); 
                else if (isalpha(c)) {
                    buf.push_back (c); 
                    CS  = IDENT;
                }
                else if (c == '"') {
                    CS = QUOTE;
                }
                else if (isdigit(c)) {
                    d = c - '0';
                    CS  = NUMB;
                } 
                else if (c == '/') { 
                    buf.push_back(c);
                    CS  = SLASH;
                }
                else if (c == '=' || c == '<' || c == '>') { 
                    buf.push_back (c);
                    CS  = ALE; 
                }
                else if (c == EOF)
                    return Lex(LEX_FIN, 0);
                else if (c == '!') {
                    buf.push_back (c);
                    CS  = NEQ;
                }
                else {
                    buf.push_back(c);
                    if ((j = look(buf, TD))){
                        return Lex ((type_of_lex)(j + (int)LEX_FIN), j);
                    }
                    else
                        throw c;
                }
                break;
            case IDENT:
                if (isalpha (c) || isdigit (c)) {
                    buf.push_back (c); 
                }
                else {
                    ungetc(c, fp);
                    if ((j = look(buf, TW))) {
                        return Lex((type_of_lex)j, j);
                    }
                    else {
                        j = put(buf);
                        return Lex(LEX_ID, j);
                    }
                }
                break;
            case QUOTE:
            	if (c != '"'){
            		buf.push_back(c);
				}
				else {
					return Lex(LEX_QUOTE, 0, buf);
				}
				break;
            case NUMB:
                if (isdigit(c)) {
                    d = d * 10 + (c - '0');
                }
                else {
                	ungetc(c, fp);
                	return Lex(LEX_NUM, d);
                }
                break;
            case SLASH:
                if ( c == '*' ) {
                	buf="";
                    CS = COM;
                }
                else {
                	ungetc(c, fp);
                	j = look(buf, TD);
                	return Lex((type_of_lex)(j + (int)LEX_FIN), j);
				}
                break;
            case COM:
            	if (c == '*'){
            		gc();
            		if (c == '/'){
            			CS = H;
					}
					else {
						ungetc(c, fp);
					}
				}
				break;
            case ALE:
                if ( c == '=' ) {
                    buf.push_back (c);
                    j = look(buf, TD);
                    return Lex((type_of_lex)(j + (int)LEX_FIN), j);
                }
                else {
                    ungetc(c, fp);
                    j = look(buf, TD);
                    return Lex((type_of_lex)(j + (int) LEX_FIN), j);
                }
                break;
            case NEQ:
                if (c == '=') {
                    buf.push_back(c);
                    j = look(buf, TD);
                    return Lex(LEX_NEQ, j);
                }
                else
                    throw '!';
                break;
        }
    } while (true);
}

template <class T, class T_EL>
void from_st(T & st, T_EL & i) {
    i = st.top(); st.pop();
    return;
}

class Parser {
    Lex          curr_lex;
    type_of_lex  c_type;
    int          c_val;
    Scanner      scan;
    stack <int>           st_int;
    stack <type_of_lex>   st_lex;
    void  P();  // Program
    void  Ds(); // Descriptions
    void  D();  // Description
    void  Os(); // Operators
    void  O();  // Operator
    void  E();  // Expression
    void  E1();
    void  E2(); 
    void  E3();
    void  E4();
    void  E5();
    void  E6();
    void  gl() {
        curr_lex = scan.get_lex();
        c_type = curr_lex.get_type();
        c_val = curr_lex.get_value();
    }
    void dec(type_of_lex type);
    void check_id();
    void check_op();
    void check_not();
    void check_init(type_of_lex type, int table_num);
    void check_read();
    void eq_bool();
public:
    vector <Lex> poliz;
    Parser (const char * program) : scan(program) { }
    void  analyze();
};

void Parser::analyze () {
    gl();
    P();
    if (c_type != LEX_FIN)
        throw curr_lex;
    cout << "OK!" << endl;
    cout << '\n' << "-----VARIABLES-----" << "\n\n";
    for (int i = 0; i < TID.size(); ++i) {
        cout << i << ' ' << TID[i] << endl;
    }
    cout << '\n' << "-----POLIZ-----" << "\n\n";
    for (int i = 0; i < poliz.size(); ++i) {
        cout << i << ' ' << poliz[i] << endl;
    }
    return;
}

void Parser::P() {
    if (c_type == LEX_PROG) {
        gl();
    }
    else 
        throw curr_lex;      
    if (c_type == LEX_BEGIN)
        gl();
    else
        throw curr_lex;
    Ds();
    Os();
    gl();
    if (c_type != LEX_FIN)
        throw curr_lex;
    return;
}

void Parser::Ds() {
    while (c_type == LEX_INT || c_type == LEX_STRING || c_type == LEX_BOOLEAN) 
        D();
    return;
}

void Parser::D() {
    type_of_lex type = c_type;
    if (c_type == LEX_INT || c_type == LEX_STRING || c_type == LEX_BOOLEAN) { 
        gl();
        if (c_type != LEX_ID) 
            throw curr_lex;
        int table_num = c_val;
        st_int.push(c_val);
        gl();
        if (c_type == LEX_EQ) {
            gl();
            check_init(type, table_num);
        }
        while (c_type == LEX_COMMA) {
            gl();
            if (c_type != LEX_ID) 
                throw curr_lex;
            table_num = c_val;
            st_int.push(c_val);
            gl();
            if (c_type == LEX_EQ) {
                gl();
                check_init(type, table_num);
            }
        }
        if (c_type != LEX_SEMICOLON)
            throw curr_lex;
        dec(type);
        gl();
    }
    else 
        throw curr_lex;
    return;
}

void Parser::Os() {
    while (c_type != LEX_END) {
        is_lex_first_on_line = true;
        is_labeled_operator = false;
        O();
    }
    gl();
    return;
}

void Parser::O() {
    int pl0, pl1, pl2, pl3;

    switch(c_type) { 

    case LEX_IF:
        gl();
        if (c_type != LEX_LPAREN)
            throw curr_lex;
        gl();
        E();
        eq_bool();
        if (c_type != LEX_RPAREN)
            throw curr_lex;
        pl2 = poliz.size();
        poliz.push_back(Lex());
        poliz.push_back(Lex(POLIZ_FGO));
        gl();
        O(); 
        pl3 = poliz.size();
        poliz.push_back(Lex());
        poliz.push_back (Lex(POLIZ_GO));
        poliz[pl2] = Lex(POLIZ_LABEL, poliz.size());
        if (c_type == LEX_ELSE) {
            gl(); 
            O();
            poliz[pl3] = Lex(POLIZ_LABEL, poliz.size());
        }
        return;

    case LEX_WHILE:
        pl0 = poliz.size();
        gl();
        if (c_type != LEX_LPAREN)
            throw curr_lex;
        gl();
        E();
        eq_bool();
        pl1 = poliz.size();
        poliz.push_back(Lex());
        poliz.push_back(Lex(POLIZ_FGO));
        if (c_type != LEX_RPAREN)
            throw curr_lex;
        gl();
        O(); 
        poliz.push_back(Lex(POLIZ_LABEL, pl0));
        poliz.push_back(Lex(POLIZ_GO));
        poliz[pl1] = Lex(POLIZ_LABEL, poliz.size());
        return;
        
    case LEX_READ: 
        gl();
        if (c_type != LEX_LPAREN)
            throw curr_lex;
        gl();
        if (c_type != LEX_ID)
            throw curr_lex;
        poliz.push_back(Lex(POLIZ_ADDRESS, c_val));
        gl();
        if (c_type != LEX_RPAREN)
            throw curr_lex;
        gl();
        if (c_type != LEX_SEMICOLON)
            throw curr_lex;
        poliz.push_back(Lex(LEX_READ));
        gl();
        return;
        
    case LEX_WRITE:
        gl();
        if (c_type != LEX_LPAREN)
            throw curr_lex;
        gl();
        E();
        while (c_type == LEX_COMMA) {
            gl();
            E();
        }
        if (c_type != LEX_RPAREN)
            throw curr_lex;
        gl();
        if (c_type != LEX_SEMICOLON)
            throw curr_lex;
        poliz.push_back(Lex(LEX_WRITE));
        gl();
        return;

    case LEX_BEGIN: // compound operator
        gl();
        Os();
        return;

     case LEX_GOTO:
        gl();
        if (c_type != LEX_ID)
            throw curr_lex;
        poliz.push_back(Lex(LEX_ID, c_val));
        gl();
        if (c_type != LEX_SEMICOLON)
            throw curr_lex;
        poliz.push_back(Lex(LEX_GOTO));
        gl();
        return;

    default: // operator - expression
        E();
        if (is_labeled_operator)  {
            is_labeled_operator = false;
            return;
        }
        if (c_type != LEX_SEMICOLON)
            throw curr_lex;
        gl();
        return;
    }
}

void Parser::E() {
    E1();
    if (is_labeled_operator) return;
    while (c_type == LEX_EQ) {
        st_lex.push(c_type);
        gl(); 
        E1();
        check_op(); 
    }
    return;
}
 
void Parser::E1() {
    E2();
    if (is_labeled_operator) return;
    while (c_type == LEX_OR) {
        st_lex.push(c_type);
        gl();
        E2();
        check_op();
    }
    return;
}
 
void Parser::E2 () {
    E3();
    if (is_labeled_operator) return;
    while (c_type == LEX_AND) {
        st_lex.push(c_type);
        gl();
        E3();
        check_op();
    }
    return;
}
 
void Parser::E3 () {
    E4();
    if (is_labeled_operator) return;
    while (c_type == LEX_LSS || c_type == LEX_GTR || c_type == LEX_DEQ ||
           c_type == LEX_LEQ || c_type == LEX_GEQ || c_type == LEX_NEQ) 
    {
        st_lex.push(c_type);
        gl();
        E4();
        check_op();
    }
    return;
}

void Parser::E4() {
    E5();
    if (is_labeled_operator) return;
    while (c_type == LEX_PLUS || c_type == LEX_MINUS) {
        st_lex.push(c_type);
        gl();
        E5();
        check_op();
    }
    return;
}

void Parser::E5() {
    E6();
    if (is_labeled_operator) return;
    while (c_type == LEX_TIMES || c_type == LEX_SLASH || c_type == LEX_PERCENT) {
        st_lex.push(c_type);
        gl();
        E6();
        check_op();
    }
    return;
}

void Parser::E6() {
    if (c_type == LEX_ID) {
        Lex ident_lex = curr_lex;
        gl();
        if (is_lex_first_on_line && c_type == LEX_COLON) {
            is_labeled_operator = true;
            st_int.push(ident_lex.get_value());
            dec(LEX_INT);
            TID[ident_lex.get_value()].put_assign();
            TID[ident_lex.get_value()].put_int_value(poliz.size());
            gl();
            O();
        }
        else {
            c_type = ident_lex.get_type();
            c_val = ident_lex.get_value();
            poliz.push_back(Lex(LEX_ID, c_val));
            check_id();
            c_type = curr_lex.get_type();
            c_val = curr_lex.get_value();
        }
    }
    else if (c_type == LEX_NUM) {
        st_lex.push(LEX_INT);
        poliz.push_back(curr_lex);
        gl();
    }
    else if (c_type == LEX_QUOTE) {
        st_lex.push(LEX_STRING);
        poliz.push_back(curr_lex);
        gl();
    }
    else if (c_type == LEX_TRUE) {
        st_lex.push(LEX_BOOLEAN);
        poliz.push_back(Lex(LEX_TRUE, 1));
        gl();
    }
    else if (c_type == LEX_FALSE) {
        st_lex.push(LEX_BOOLEAN);
        poliz.push_back(Lex(LEX_FALSE, 0));
        gl();
    }
    else if (c_type == LEX_NOT) {
        gl(); 
        E6(); 
        check_not();
    }
    else if (c_type == LEX_LPAREN) {
        gl(); 
        E();
        if (c_type == LEX_RPAREN)
            gl();
        else 
            throw curr_lex;
    }
    else
        throw curr_lex;
    return;
}

void Parser::dec(type_of_lex type) {
    int i;
    while (!st_int.empty()) {
        from_st(st_int, i);
        if (TID[i].get_declare()) {
            throw "twice";
        }
        else {
            TID[i].put_declare();
            TID[i].put_type(type);
        }
    }
    return;
}

void Parser::check_init(type_of_lex type, int table_num) {
    if (!(type == LEX_INT && c_type == LEX_NUM ||
        type == LEX_STRING && c_type == LEX_QUOTE ||
        type == LEX_BOOLEAN && (c_type == LEX_FALSE || c_type == LEX_TRUE)))
        throw "initialization bad types";
    TID[table_num].put_assign();
    if (type == LEX_INT) 
       TID[table_num].put_int_value(c_val);
    else if (type == LEX_STRING)
        TID[table_num].put_string_value(curr_lex.get_string_value());
    else if (c_type == LEX_FALSE)
        TID[table_num].put_bool_value(false); 
    else 
        TID[table_num].put_bool_value(true);
    gl();
    return;
}

void Parser::check_id() {
    if (TID[c_val].get_declare())
        st_lex.push (TID[c_val].get_type());
    else 
        throw "not declared";
}

void Parser::check_read() {
    if (!TID[c_val].get_declare())
        throw "not declared";
    if (TID[c_val].get_type() == LEX_BOOLEAN)
        throw "boolean type variable in read()";
}

void Parser::eq_bool () {
    if (st_lex.top() != LEX_BOOLEAN)
        throw "expression is not boolean";
    st_lex.pop();
}

void Parser::check_op() {
    type_of_lex t1, op, t2, r;
    from_st(st_lex, t2);
    from_st(st_lex, op);
    from_st(st_lex, t1);
    if (t1 != t2)
        throw "wrong types";
    if (op == LEX_PLUS) {
        if (t1 == LEX_BOOLEAN)
            throw "wrong types";
        r = t1;
    }
    else if (op == LEX_MINUS || op == LEX_TIMES || 
             op == LEX_SLASH || op == LEX_PERCENT) 
    {
        if (t1 != LEX_INT)
            throw "wrong types";
        r = LEX_INT;
    }
    else if (op == LEX_LSS || op == LEX_GTR || 
             op == LEX_NEQ || op == LEX_DEQ) 
    {
        if (t1 == LEX_BOOLEAN)
            throw "wrong types";
        r = LEX_BOOLEAN;
    }
    else if (op == LEX_LEQ || op == LEX_GEQ) {
        if (t1 != LEX_INT)
            throw "wrong types";
        r = LEX_BOOLEAN;
    }
    else if (op == LEX_OR || op == LEX_AND) {
        if (t1 != LEX_BOOLEAN)
            throw "wrong types";
        r = LEX_BOOLEAN;
    }
    else if (op == LEX_EQ)
        r = t1;
    else
        throw "wrong operation";
    st_lex.push(r);
    poliz.push_back(Lex(op));
}

void Parser::check_not() {
    if (st_lex.top() != LEX_BOOLEAN)
        throw "wrong types";
    poliz.push_back(Lex(LEX_NOT));
}

int main(int argc, char * argv[]) {
	try {
		if (argc == 1){
			Parser p("my_lang_programm.txt");
			p.analyze();
		}
        else {
			throw "Can't find filename in argv[]";
		}
	}
    catch(Lex l) {
        cout << "Error on lexem\n" << l << endl;  
    }
	catch(char c){
        cout << "Error on symbol " << c;
		return 1;
	}
	catch(const char * msg) {
        cout << msg << endl;
        return 1;
    }
	return 0;
}

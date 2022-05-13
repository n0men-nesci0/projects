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
    Lex(type_of_lex t_lex_ = LEX_NULL, int int_lex_ = 0, string str_lex_ = "") {
        t_lex = t_lex_;
        int_lex = int_lex_;
        str_lex = str_lex_;
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

    string get_name() const { return name.c_str(); }

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

    void set_bool_value(bool v) { bool_value = v; }
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
                        return Lex(LEX_FIN);
                    else if (c == '!') {
                        buf.push_back (c);
                        CS  = NEQ;
                    }
                    else {
                        buf.push_back (c);
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
                if ( isdigit (c) ) {
                    d = d * 10 + (c - '0');
                }
                else {
                	ungetc (c, fp);
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
public:
    Parser (const char * program) : scan(program) { }
    void  analyze();
};

void Parser::analyze () {
    gl();
    P();
    if (c_type != LEX_FIN)
        throw curr_lex;
    cout << "OK!" << endl;
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
    if (c_type == LEX_INT) { 
        gl();
        if (c_type != LEX_ID) 
            throw curr_lex;
        gl();
        if (c_type == LEX_EQ) {
            gl();
            if (c_type != LEX_NUM) 
                throw curr_lex; 
            gl();
        }
        while (c_type == LEX_COMMA) {
            gl();
            if (c_type != LEX_ID) 
                throw curr_lex;
            gl();
            if (c_type == LEX_EQ) {
                gl();
                if (c_type != LEX_NUM) 
                    throw curr_lex; 
                gl();
            }
        }
        if (c_type != LEX_SEMICOLON)
            throw curr_lex;
        gl();
    }
    else if (c_type == LEX_STRING) {
        gl();
        if (c_type != LEX_ID) 
            throw curr_lex;
        gl();
        if (c_type == LEX_EQ) {
            gl();
            if (c_type != LEX_QUOTE) 
                throw curr_lex; 
            gl();
        }
        while (c_type == LEX_COMMA) {
            gl();
            if (c_type != LEX_ID) 
                throw curr_lex;
            gl();
            if (c_type == LEX_EQ) {
                gl();
                if (c_type != LEX_QUOTE) 
                    throw curr_lex; 
                gl();
            }
        }
        if (c_type != LEX_SEMICOLON)
            throw curr_lex;
        gl();
    }
    else if (c_type == LEX_BOOLEAN) {
        gl();
        if (c_type != LEX_ID) 
            throw curr_lex;
        gl();
        if (c_type == LEX_EQ) {
            gl();
            if (c_type != LEX_TRUE && c_type != LEX_FALSE) 
                throw curr_lex; 
            gl();
        }
        while (c_type == LEX_COMMA) {
            gl();
            if (c_type != LEX_ID) 
                throw curr_lex;
            gl();
            if (c_type == LEX_EQ) {
                gl();
                if (c_type != LEX_TRUE && c_type != LEX_FALSE) 
                    throw curr_lex; 
                gl();
            }
        }
        if (c_type != LEX_SEMICOLON)
            throw curr_lex;
        gl();
    }
    else throw curr_lex;
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

    switch(c_type) { 

    case LEX_IF:
        gl();
        if (c_type != LEX_LPAREN)
            throw curr_lex;
        gl();
        E();
        if (c_type != LEX_RPAREN)
            throw curr_lex;
        gl();
        O(); 
        if (c_type == LEX_ELSE) {
            gl(); 
            O();
        }
        return;

    case LEX_WHILE:
        gl();
        if (c_type != LEX_LPAREN)
            throw curr_lex;
        gl();
        E();
        if (c_type != LEX_RPAREN)
            throw curr_lex;
        gl();
        O(); 
        return;
        
    case LEX_READ: 
        gl();
        if (c_type != LEX_LPAREN)
            throw curr_lex;
        gl();
        if (c_type != LEX_ID)
            throw curr_lex;
        gl();
        if (c_type != LEX_RPAREN)
            throw curr_lex;
        gl();
        if (c_type != LEX_SEMICOLON)
            throw curr_lex;
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
        gl();
        return;

    case LEX_BEGIN: // compound operator
        gl();
        Os();
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
        gl(); 
        E1(); 
    }
    return;
}
 
void Parser::E1() {
    E2();
    if (is_labeled_operator) return;
    while (c_type == LEX_OR) {
        gl();
        E2();
    }
    return;
}
 
void Parser::E2 () {
    E3();
    if (is_labeled_operator) return;
    while (c_type == LEX_AND) {
        gl();
        E3();
    }
    return;
}
 
void Parser::E3 () {
    E4();
    if (is_labeled_operator) return;
    while (c_type == LEX_LSS || c_type == LEX_GTR ||
           c_type == LEX_LEQ || c_type == LEX_GEQ || c_type == LEX_NEQ) {
        gl();
        E4();
    }
    return;
}

void Parser::E4() {
    E5();
    if (is_labeled_operator) return;
    while (c_type == LEX_PLUS || c_type == LEX_MINUS) {
        gl();
        E5();
    }
    return;
}

void Parser::E5() {
    E6();
    if (is_labeled_operator) return;
    while (c_type == LEX_TIMES || c_type == LEX_SLASH || c_type == LEX_PERCENT) {
        gl();
        E6();
    }
    return;
}

void Parser::E6() {
    if (c_type == LEX_ID) {
        gl();
        if (is_lex_first_on_line && c_type == LEX_COLON) {
            is_labeled_operator = true;
            gl();
            O();
        }
    }
    else if (c_type == LEX_NUM) {
        gl();
    }
    else if (c_type == LEX_TRUE) {
        gl();
    }
    else if (c_type == LEX_FALSE) {
        gl();
    }
    else if (c_type == LEX_NOT) {
        gl(); 
        E6(); 
    }
    else if (c_type == LEX_LPAREN ) {
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

template <class T, class T_EL>
void from_st ( T & st, T_EL & i ) {
    i = st.top(); st.pop();
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

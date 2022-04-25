#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <ctype.h>
using namespace std;

enum type_of_lex {
    LEX_NULL, LEX_PROG, LEX_IF, LEX_ELSE, LEX_WHILE, LEX_READ, // 0 - 5
    LEX_WRITE, LEX_NOT, LEX_AND, LEX_OR, LEX_GOTO, LEX_BOOLEAN, // 6 -11
    LEX_INT, LEX_STRING, LEX_TRUE, LEX_FALSE, LEX_FIN, // 12 - 16

    LEX_SEMICOLON, LEX_COMMA, LEX_COLON, LEX_LPAREN, LEX_RPAREN, LEX_EQ, LEX_LSS, // 17 - 23 				
    LEX_GTR, LEX_PLUS, LEX_MINUS, LEX_TIMES, LEX_SLASH, LEX_LEQ, LEX_NEQ, LEX_GEQ, // 24 - 31       	
    LEX_DEQ, LEX_TSLASH, LEX_SLASHT, LEX_BEGIN, LEX_END, // 32 - 37

    LEX_QUOTE, LEX_ID, LEX_NUM, POLIZ_LABEL, // 38 - 40
    POLIZ_ADDRESS, POLIZ_GO, POLIZ_FGO // 41 - 43
};


class Lex {
    type_of_lex t_lex;
    int int_lex;
    string str_lex;
    bool bool_lex;
public:
    Lex(type_of_lex t_lex_ = LEX_NULL, int int_lex_ = 0, string str_lex_ = "", bool bool_lex_ = false) {
        t_lex = t_lex_;
        int_lex = int_lex_;
        str_lex = str_lex_;
        bool_lex = bool_lex_;
    }

    type_of_lex get_type() const { return t_lex; }

    int get_int_value() const { return int_lex; }

    string get_string_value() const { return str_lex; } 

    bool get_bool_value() const { return bool_lex; }

    friend ostream& operator << (ostream& os, Lex l) {
        os << '(' << l.t_lex << ',' << l.int_lex << ',' << l.str_lex << ',' << l.bool_lex << ");";
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
    int look(const string buf, const char ** list ) {
        int i = 0;
        for (;list[i]; ++i) 
            if (buf == list[i])
                return i;
        return 0;
    }

    void gc () {
        c = fgetc (fp);
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
                      "string", "true", "false", NULL };
 
const char *
Scanner::TD    [] = { 
    "", ";", ",", ":", "(", ")", "=", "<", ">", "+", "-", "*", 
	"/","<=", "!=", ">=", "==", "*/", "/*", "{", "}", NULL};

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


int main(int argc, char * argv[]) {
	try {
		if (argc == 2){
			Scanner s(argv[1]);
			Lex l;
			while((l = s.get_lex()).get_type() != LEX_FIN){
				cout << l << endl;
			}
			cout << l;
		}
        else {
			throw "Can't find filename in argv[]";
		}
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

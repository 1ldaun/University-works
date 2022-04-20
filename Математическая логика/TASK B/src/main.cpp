#include <iostream>
#include <stdlib.h>
#include <vector>
#include <string.h>
#include "exp.h"
#include "lexer/expression.tab.h"
#include "lexer/expression.lexer.h"
#include "util.h"


using namespace std;

extern express* res_val;
extern vector<variable *> init_vector;

int yywrap() { return (0 | 1); }
void yyerror(const char *err) { cerr << err; }

string str;

int main() {
    cin >> str;
    yy_scan_string(str.c_str());
    yyparse();
    cout << rts(resolve(res_val, &init_vector)) << endl;
    return 0;
}

%{
#include <string>
#include <vector>
#include "../exp.h"
#include "../util.h"


int yylex(void);
void yyerror(const char *);


express *res_val = NULL;
std::vector<variable *> init_vector;

%}
%union {
    express *expr;
    std::string *name;
}

%token<name> NAME
%token<expr> IMP OR AND NEG
%token<expr> L R
%token<expr> END

%right IMP
%left OR
%left AND
%left NEG

%type<expr> exp
%type<name> Input

%start Input


%%
Input: exp { res_val = $1; }
;

exp:
      NAME { 
        variable *v = new variable(*$1);
        variable *rez = vector_compare(&init_vector, v);
        if (rez == NULL) {
            init_vector.push_back(v);
            $$=v;
        } else {
            $$=rez;
        }
    }
    | L exp R     { $$=$2; }
    | exp IMP exp { $$=new implication($1, $3); }
    | exp OR exp  { $$=new disunction($1, $3); }
    | exp AND exp { $$=new conunction($1, $3); }
    | NEG exp            { $$=new negation($2); }
;
%%

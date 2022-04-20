#include "exp.h"
#include <string>
#include <vector>
#include <cstddef>


using namespace std;

struct res {
    int tr;
    int all;
};

variable* vector_compare(vector<variable*> *v, variable *var);

struct res resolve(express *res_value, vector<variable*> *v); 

string rts(struct res en);

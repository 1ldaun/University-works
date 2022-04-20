#include <cmath>
#include <string>
#include <vector>
#include <cstddef>
#include "exp.h"
#include "util.h"

using namespace std;

static void to_val(vector<variable *> *v, int value) {
    for (int i = 0; i < v->size(); ++i) {
        ((*v)[i])->set_value(value % 2);
        value =  value / 2;
    }
}

struct res resolve(express *res_val, vector<variable *> *v) {
    struct res rez_v = {.tr = 0, .all = (int) pow(2, v->size())};
    for (int i = 0; i < rez_v.all; ++i) {
        to_val(v, i);
        rez_v.tr += res_val->calc();
    }
    return rez_v;
}

variable* vector_compare(vector<variable *> *v, variable *var) {
    for (auto &e : *v) {
        if (e->get_name().compare(var->get_name()) == 0) {
            return e;
        }
    }
    return NULL; 
}

string rts(struct res en) {
    if (!(en.tr - en.all)) {
        return ("Valid");
    } else if (!en.tr) {
        return ("Unsatisfiable");
    } else {
        return ("Satisfiable and invalid, " + to_string(en.tr) + " true and " + 
            to_string(en.all - en.tr) + " false cases");
    }
}

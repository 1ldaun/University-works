#include <cstdarg>
#include "dis.h"

#define Nimp new imp
#define Nneg new neg
#define Ndis new dis
#define Ncon new con

using namespace std;

bool is_axiom_number(express *expr, int number);
axiom *case_axiom(int number, ...);

int find_axiom_number(express *expr) {
    size_t number = 1;
    bool found = false;
    for (; !found && number < 11; ++number)
        found = is_axiom_number(expr, number);
    return found ? --number : 0;
}

axiom *create_axiom(int number, ...) {
	axiom *a = NULL;
    va_list args;
    va_start(args, number);
    a = case_axiom(number, args);
    va_end(args);
    return a;
}

axiom *case_axiom(int number, ...) {
    va_list args;
    va_start(args, number);

    auto *a = va_arg(args, express*);
    if (number == 10) return new axiom(Nimp(Nneg(Nneg(a)), a), 10);
    auto *b = va_arg(args, express*);
    if (number == 1)		return new axiom(Nimp(a,Nimp(b,a)), 1);
	else if (number == 3) 	return new axiom(Nimp(a,Nimp(b,Ncon(a,b))), 3);
	else if (number == 4) 	return new axiom(Nimp(Ncon(a,b),a), 4);
	else if (number == 5) 	return new axiom(Nimp(Ncon(a,b),b), 5);
	else if (number == 6)	return new axiom(Nimp(a,Ndis(a,b)), 6);
	else if (number == 7) 	return new axiom(Nimp(b,Ndis(a,b)), 7);
	auto *c = va_arg(args, express*);
	if (number == 2) 	return new axiom(Nimp(Nimp(a,b),Nimp(Nimp(a,Nimp(b,c)),Nimp(a,c))), 2);
	else if (number == 8) 	return new axiom(Nimp(Nimp(a,b),Nimp(Nimp(a,Nneg(b)),Nneg(c))), 8);
	else if (number == 9) 	return new axiom(Nimp(Nimp(a,c),Nimp(Nimp(b,c),Nimp(Ndis(a,b),c))), 9);
	else return NULL;
    return NULL;
}

bool is_axiom_number(express *expr, int number) {
	imp* imp2 = (imp*) expr;
	if (expr->type - 3) 		return false;
	else
    if (number == 1 && ((imp2)->r)->type == 3) 
        return ((imp*)(imp2)->r)->r->is_range((imp2)->l);
    else
    if (number == 2) {
    	imp* imp2r = (imp*)(imp2->r);
        imp* imp2l = (imp*)(imp2->l);
        imp* imp2rl = (imp*)(imp2r->l);
        imp* imp2rlr = ((imp*)(imp2rl->r));
        imp* imp2rr = (imp*)(imp2r->r);
        if ((imp2r->type - 3) || (imp2l->type - 3) || (imp2rl->type - 3) || (imp2rr->type - 3) || (imp2rlr->type - 3))	return false;

        express* imp2ll = imp2l->l;
        express* imp2rll = imp2rl->l;
        express* imp2rrl = imp2rr->l;
        express* imp2lr = imp2l->r;
        express* imp2rlrl = imp2rlr->l;
        express* imp2rlrr = imp2rlr->r;
        express* imp2rrr = imp2rr->r;
        if (!imp2ll->is_range(imp2rll) || !imp2rll->is_range(imp2rrl) || !imp2lr->is_range(imp2rlrl))					return false;
        else																											return imp2rlrr->is_range(imp2rrr);
    }
    else
   if (number == 3) {
		imp* imp2r = (imp*)(imp2->r);
/*1*/   if ((imp2r->type - 3) || (imp2r->r->type - 1)) 	return false;
        imp* imp2rr = (imp*)(imp2r->r);
        express* imp2l = imp2->l;
        express* imp2rrl = imp2rr->l;
/*2*/   if (!imp2l->is_range(imp2rrl)) 					return false;
		else											return imp2r->l->is_range(imp2rr->r);
    }
    else
   if (number == 4 && imp2->l->type == 1)
		return ((con*)(imp2->l))->l->is_range(imp2->r);
    else
   if (number == 5 && imp2->l->type == 1) 
        return ((con*)(imp2->l))->r->is_range(imp2->r);
    else
   if (number == 6 && imp2->r->type == 2) 
        return imp2->l->is_range(((dis*)(imp2->r))->l);
    else
   if (number == 7 && (imp2->r->type == 2))
		return imp2->l->is_range(((dis*)(imp2->r))->r);
    else
   if (number == 8 && (imp2->r->type == 3)) {
        imp* imp2r = (imp*)(imp2->r);
        imp* imp2l = (imp*)(imp2->l);
        imp* imp2rl = (imp*)(imp2r->l);
        imp* imp2rr = (imp*)(imp2r->r);
        if (imp2->l->type != 3 || imp2r->l->type != 3 || imp2r->r->type != 3 || (imp2rr->l->type != 2)) return false;
        dis* imp2rrl = (dis*)(imp2rr->l);
        if (!(imp2l->l)->is_range(imp2rrl->l) || !(imp2rl->l)->is_range(imp2rrl->r)) 					return false;
        else																							return (imp2l->r)->is_range(imp2rl->r) && (imp2rl->r)->is_range(imp2rr->r);
    }
    else
    if (number == 9 && imp2->r->type == 3) {
        imp* imp2r = (imp*)(imp2->r);

        if (imp2->l->type != 3 || imp2r->l->type != 3 || imp2r->r->type != 4) 	return false;
        imp* imp2l = (imp*)(imp2->l);
        imp* imp2rl = (imp*)(imp2r->l);

        if (!(imp2l->l)->is_range(imp2rl->l) || (imp2rl->r->type != 4)) 		return false;
        else																	return (imp2l->r)->is_range(((neg*)(imp2rl->r))->_expr);
    }
    else
   if (number == 10 && (imp2->l->type == 4) && (((neg*)(imp2->l))->_expr->type == 4)) 
        return ((neg*)(((neg*)(imp2->l))->_expr))->_expr->is_range(imp2->r);
 	else
        return false;
}

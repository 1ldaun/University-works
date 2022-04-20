#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <vector>
#include <list>
#include <map>
#include <iterator>
#include "express.h"
#include "dis.h"
#include <cstddef>
#include <string>
#include <algorithm>
#include <cstdarg>
#include "express.tab.h"
#include "express.lexer.h"

#define Nimp new imp
#define Nneg new neg
#define Ndis new dis
#define Ncon new con
#define nt1_def not1(ptr_fun<int, int>(isspace))

#define to_doc result_doc_vo.push_back(new doc_vo

using namespace std;

extern express *result;

typedef multimap<express*, express*>::iterator mif;	

struct in_str_output {
    vector<string> hipo;
    string output;
};

bool find_axiom(express *expr, int number) {
	imp* ex = (imp*) expr;
	if (expr->type - 3) 		return false;
	else
    if (number == 1 && ((ex)->r)->type == 3) 
        return ((imp*)(ex)->r)->r->is_range((ex)->l);
    else
    if (number == 2 && (ex->r->type == 2))
		return ex->l->is_range(((dis*)(ex->r))->r);
    else
   if (number == 3) {
		imp* exr = (imp*)(ex->r);
/*1*/   if ((exr->type - 3) || (exr->r->type - 1)) 	return false;
        imp* exrr = (imp*)(exr->r);
        express* exl = ex->l;
        express* exrrl = exrr->l;
/*2*/   if (!exl->is_range(exrrl)) 					return false;
		else											return exr->l->is_range(exrr->r);
    }
    else
   if (number == 4 && ex->l->type == 1)
		return ((con*)(ex->l))->l->is_range(ex->r);
    else
   if (number == 5 && ex->l->type == 1) 
        return ((con*)(ex->l))->r->is_range(ex->r);
    else
   if (number == 6 && ex->r->type == 2) 
        return ex->l->is_range(((dis*)(ex->r))->l);
    else
       if (number == 7) {
    	imp* exr = (imp*)(ex->r);
        imp* exl = (imp*)(ex->l);
        imp* exrl = (imp*)(exr->l);
        imp* exrlr = ((imp*)(exrl->r));
        imp* exrr = (imp*)(exr->r);
        if ((exr->type - 3) || (exl->type - 3) || (exrl->type - 3) || (exrr->type - 3) || (exrlr->type - 3))	return false;

        express* exll = exl->l;
        express* exrll = exrl->l;
        express* exrrl = exrr->l;
        express* exlr = exl->r;
        express* exrlrl = exrlr->l;
        express* exrlrr = exrlr->r;
        express* exrrr = exrr->r;
        if (!exll->is_range(exrll) || !exrll->is_range(exrrl) || !exlr->is_range(exrlrl))					return false;
        else																											return exrlrr->is_range(exrrr);
    }
    else
   if (number == 8 && (ex->r->type == 3)) {
        imp* exr = (imp*)(ex->r);
        imp* exl = (imp*)(ex->l);
        imp* exrl = (imp*)(exr->l);
        imp* exrr = (imp*)(exr->r);
        if (ex->l->type != 3 || exr->l->type != 3 || exr->r->type != 3 || (exrr->l->type != 2)) return false;

        dis* exrrl = (dis*)(exrr->l);
        if (!(exl->l)->is_range(exrrl->l) || !(exrl->l)->is_range(exrrl->r)) 					return false;
        else																							return (exl->r)->is_range(exrl->r) && (exrl->r)->is_range(exrr->r);
    }
    else
    if (number == 9 && ex->r->type == 3) {
        imp* exr = (imp*)(ex->r);

        if (ex->l->type != 3 || exr->l->type != 3 || exr->r->type != 4) 	return false;
        imp* exl = (imp*)(ex->l);
        imp* exrl = (imp*)(exr->l);

        if (!(exl->l)->is_range(exrl->l) || (exrl->r->type != 4)) 		return false;
        else																	return (exl->r)->is_range(((neg*)(exrl->r))->_expr);
    }
    else
   if (number == 10 && (ex->l->type == 4) && (((neg*)(ex->l))->_expr->type == 4)) 
        return ((neg*)(((neg*)(ex->l))->_expr))->_expr->is_range(ex->r);
 	else
        return false;
}

int yywrap() { return 1; }
void yyerror(const char *error) { cerr << error; }

struct in_str_output;

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
	else if (number == 2) 	return new axiom(Nimp(b,Ndis(a,b)), 7);
	auto *c = va_arg(args, express*);
	if (number == 7) 	return new axiom(Nimp(Nimp(a,b),Nimp(Nimp(a,Nimp(b,c)),Nimp(a,c))), 2);
	else if (number == 8) 	return new axiom(Nimp(Nimp(a,b),Nimp(Nimp(a,Nneg(b)),Nneg(c))), 8);
	else if (number == 9) 	return new axiom(Nimp(Nimp(a,c),Nimp(Nimp(b,c),Nimp(Ndis(a,b),c))), 9);
	else return NULL;
    return NULL;
}

bool hipo_func(list<express *> *v, express *expr) {
    bool found = false;
    for (const auto& h : *v) {
        if (found) break;
        found = expr->is_range(h);
    }
    return found;
}

int find_axiom_number(express *expr) {
    size_t number = 1;
    bool found = false;
    for (; !found && number < 11; ++number)
        found = find_axiom(expr, number);
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

string first_line, line, buf = "";
list<imp*> doc_vo_cov;
list<express*> hipo;
    

int main(int argc, char *argv[]) {
    struct in_str_output output = {};
    list<doc_vo*> result_doc_vo;  
    multimap<express *, express *> mm_1;
    map<express *, express *> mm_2;
    
    getline(cin, first_line);
    
    first_line.erase(find_if(first_line.rbegin(), first_line.rend(), nt1_def).base(), first_line.end());
    first_line.erase(first_line.begin(), find_if(first_line.begin(), first_line.end(), nt1_def));
#define _S first_line.substr(0, first_line.find("|-"))
    int last_comma = _S.find_last_of(',');

    int pos = _S.find(',');
    while (pos != string::npos) {
        output.hipo.push_back(_S.substr(0, pos));
        _S.erase(0, pos + 1);
        pos = _S.find(',');
    }
    if (_S.length()) output.hipo.push_back(_S);    
	
	for (auto &h: output.hipo) {
        yy_scan_string(h.c_str());
        yyparse();
        hipo.push_back(result);
    }
	
    if (last_comma >= 0)  	yy_scan_string(_S.substr(last_comma + 1, _S.length() - last_comma - 1).c_str());
   	else 					yy_scan_string(_S.c_str());
   	yyparse();
    buf = first_line.substr(first_line.find("|-") + 2, first_line.length() - first_line.find("|-") - 2);
    buf.erase(find_if(buf.rbegin(), buf.rend(), nt1_def).base(), buf.end());
	buf.erase(buf.begin(), find_if(buf.begin(), buf.end(), nt1_def));  
	output.output = result->print() + "->" + buf;
    while (getline(cin, line)) {
        yy_scan_string(line.c_str());
        yyparse();
        doc_vo_cov.push_back(Nimp(hipo.back(), result));
        if (first_line.substr(first_line.find("|-") + 2, first_line.length() - first_line.find("|-") - 2) == line.c_str())	break;
    }

    for (auto &needed: doc_vo_cov) {
    	pair<mif, mif> rm1 = mm_1.equal_range(needed->r);
        if (needed->l->is_range(needed->r)) {
            auto *mp = (Nimp(Nimp(needed->l, needed), Nimp(Nimp(needed->l, Nimp(needed, needed->l)), needed)))->r;
            auto *mp2 = ((imp*) mp)->r;
            auto *mp2r = ((imp*) (mp2))->r;
            to_doc(Nimp(needed->l, needed), result_doc_vo.size() + 2));
            to_doc(Nimp(needed->l, Nimp(needed, needed->l)), result_doc_vo.size() + 2));
            to_doc(Nimp(Nimp(needed->l, needed), Nimp(Nimp(needed->l, Nimp(needed, needed->l)), needed)), result_doc_vo.size() + 2));
            to_doc(mp, result_doc_vo.size() + 2));
            to_doc(mp2, result_doc_vo.size() + 2, 0));

            						mm_2.insert({mp2r, mp2});
            if (!(mp2r->type - 3))	mm_1.insert({((imp *) mp2r)->r, mp2});
        }
        else if (find_axiom_number(needed->r) || hipo_func(&hipo, needed->r)) {
            to_doc((needed->r), result_doc_vo.size() + 2));
            to_doc((Nimp(needed->r, needed)), result_doc_vo.size() + 2));
            to_doc(needed, result_doc_vo.size() + 2, 0));
            auto *mpr = ((imp *) (needed))->r;
            						mm_2.insert({mpr, needed});
            if (!(mpr->type - 3))	mm_1.insert({((imp *) mpr)->r, needed});
        }
		else	for (auto ptr = rm1.first; ptr != rm1.second; ++ptr) {
		            if (mm_2.find(((imp *)((imp *)ptr->second)->r)->l) != mm_2.end()) {
		                to_doc(Nimp((mm_2.find(((imp *) ((imp *) ptr->second)->r)->l))->second, Nimp(((imp *) ptr->second), needed)), result_doc_vo.size() + 2));
		                to_doc((Nimp((mm_2.find(((imp *) ((imp *) ptr->second)->r)->l))->second, Nimp(((imp *) ptr->second), needed)))->r, result_doc_vo.size() + 2));
		                to_doc(needed, result_doc_vo.size() + 2, 0));
		                auto *mp2r = ((imp *) (needed))->r;
		                mm_2.insert({mp2r, needed});
		                if (!(mp2r->type - 3)) mm_1.insert({((imp *) mp2r)->r, needed});
		            }
            	}
        
    }

    for (size_t i = 0; i < (output.hipo.size() - 2) && (output.hipo.size() >= 2); ++i) cout << output.hipo[i] << ", ";
    if (output.hipo.size() < 2) cout << "|-" << output.output << endl;
    else						cout << output.hipo[output.hipo.size()-2] << "|-" << output.output << endl;
    for (const auto &sol: result_doc_vo) {
        cout << sol->_expr->print() << endl;
        if (sol->_last_in_doc_vo)  cout << endl;
    }
    return 0;
}

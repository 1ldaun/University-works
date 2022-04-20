#ifndef DIS_H
#define DIS_H

#include "express.h"
#include <list>
#include <vector>
#include <cstring>

#define eq override { if (expr->key == _expr->key) { if (this->print() == expr->print())	return true;	else	return false; } else 	return false;   }
#define eq_ex override { if (expr->key == this->key) { if (this->print() == expr->print())	return true;	else	return false; } else  	return false;  }

using namespace std;

class doc_vo : public express {
public:
	express* _expr;
    size_t _line;
    bool _last_in_doc_vo;
    
    doc_vo(express* expr, size_t line) : _expr(expr), _line(line) {
        _last_in_doc_vo = false;
        key = expr->key;
    }
    
        doc_vo(express* expr) : _expr(expr) {
        _last_in_doc_vo = false;
        _line = 0;
        key = expr->key;
    }
    
    doc_vo(express* expr, size_t line, bool last_in_doc_vo) : _expr(expr), _line(line), _last_in_doc_vo(last_in_doc_vo) {
        key = expr->key;
    }

    bool calc() override { return _expr->calc(); }
    bool is_range(express *expr) eq
    string print() const override { return _expr->print(); }
};

class axiom : public express {
public:
	express* _expr;
    int _number;
    
    axiom(express* expr, int number) : _expr(expr), _number(number) {
        key = expr->key;
    }

    bool calc() override { return true; }
    bool is_range(express *expr) eq
	string print() const override { return _expr->print(); }
};

int find_axiom_number(express *expr);

static hash<string> hasher;

axiom* create_axiom(int number, ...);

bool hipo_func(list<express *> *v, express *expr);

class dis : public express {
public:
    express *l;
    express *r;
    dis(express *left, express *right) : l(left), r(right) {
        type = 2;
        key = l->key * r->key * (11 + 20);
    }

    express* getl() {
        return l;
    }

    express* getr() {
        return r;
    }

    bool calc() override { return l->calc() || r->calc(); }
    bool is_range(express* expr) eq_ex
    string print() const override {
    	string alph = "(" + l->print();
    	alph += "|";
    	alph += r->print() + ")";
        return  alph;
    }
};

class con : public express {
public:
    express *l;
    express *r;
    con(express *left, express *right) : l(left), r(right) {
        type = 1;
        key = l->key * r->key * (11 + 0);
    }
    
    bool calc() override { return l->calc() && r->calc(); }
    bool is_range(express* expr) eq_ex
	string print() const override {
    	string alph = "(" + l->print();
    	alph += '&';
    	alph += r->print() + ")";
        return  alph;
    }
};

class imp : public express {
public:
    express *l;
    express *r;
    
    imp(express *right) : r(right) {
    	l = NULL;
        type = 3;
        key = r->key * (11 + 30);
    }
    
    imp(express *left, express *right) : l(left), r(right) {
        type = 3;
        key = l->key * r->key * (11 + 30);
    }
    
    imp() {
    	l = NULL;
    	r = NULL;
        type = 3;
        key = 0;
    }

    bool calc() override { return l->calc() <= r->calc(); }
	bool is_range(express* expr) eq_ex
    string print() const override {
    	string alph = "(" + l->print();
    	alph += "->" ;
    	alph += r->print() + ")";
        return  alph;
    }
};

class vrb : public express {
public:
    string val;
    bool _value{};
    
    vrb() {
    	val = "";
        type = 0;
        key = hasher("");
    }
    
    vrb(string &name) : val(name) {
        type = 0;
        key = hasher(name);
    }
    
    bool calc() override { return _value; }
    bool is_range(express* expr) eq_ex
     string print() const override {return val;}
};

class neg : public express {
public:
    express *_expr;
    
    neg() {
        type = 4;
        key = false;
    }
    
    neg(express *expr) : _expr(expr) {
        type = 4;
        key = ~(expr->key);
    }


    bool calc() override { return !_expr->calc(); }
    bool is_range(express *expr) eq
	string print() const override {
    	string alph = "(!" + _expr->print();
   	 	alph += ")";
        return alph;
    }
};
#endif

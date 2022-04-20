#ifndef express_H
#define express_H
#include <iostream>
#include <string>

using namespace std;

class express {
public:
    virtual string c_out() = 0;
    virtual bool calc() = 0;
    virtual ~express() = default;;
};

class variable : public express {
private:
    string _name;
    bool _value;
public:
    variable(string &name) : _name(name) {}
    string get_name() { return _name; }
    void set_value(bool value) { _value = value; }
    string c_out() override {
        cout << _value << " ";
        return _name;
    }
    bool calc() override {
		return _value; 
	}
};

class disunction : public express {
private:
    express *_l;
    express *_r;
public:
    disunction(express *l, express *r) : _l(l), _r(r) {}
    string c_out() override {
        return ("(|," + _l->c_out() + "," + _r->c_out() + ")");
    }
    bool calc() override {
		return (_l->calc() || _r->calc()); 
	}
};

class conunction : public express {
private:
    express *_l;
    express *_r;
public:
    conunction(express *l, express *r) : _l(l), _r(r) {}
    string c_out() override {
        return ("(&," + _l->c_out() + "," + _r->c_out() + ")");
    }
    bool calc() override {
		return _l->calc() && _r->calc();
	}
};

class implication : public express {
private:
    express *_l;
    express *_r;
public:
    implication(express *l, express *r) : _l(l), _r(r) {}
    string c_out() override {
        return ("(->," + _l->c_out() + "," + _r->c_out() + ")");
    }
    bool calc() override {
		return _l->calc() <= _r->calc(); 
	}
};

class negation : public express {
private:
    express *_expr;
public:
    negation(express *expr) : _expr(expr) {}
    string c_out() override {
        return ("(!" + _expr->c_out() + ")");
    }
    bool calc() override {
		return !(_expr->calc()); 
	}
};

#endif

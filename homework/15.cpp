#include <iostream>
#include <vector>

using namespace std;

class Ocean;
class Terrestrial;
class Bird;
class Amphibious;
class Waterfowl;

class Visitor {
public:
    virtual ~Visitor() {}
    virtual void visit(Ocean& o) = 0;
    virtual void visit(Terrestrial& t) = 0;
    virtual void visit(Bird& b) = 0;
    virtual void visit(Amphibious& a) = 0;
    virtual void visit(Waterfowl& w) = 0;
};

class Generic{
public:
    virtual void eat() = 0;
    virtual ~Generic() {}
    virtual void accept(Visitor& v) = 0;
};

class Ocean: virtual public Generic{
public:
    void eat() { cout << "Ocean is eating..." << endl; }
    void swim() { cout << "Swimming..." << endl; }
    void accept(Visitor& v) { v.visit(*this); }
};

class Terrestrial: virtual public Generic{
public:
    void eat() { cout << "Terrestrial is eating..." << endl; }
    void walk() { cout << "Walking..." << endl; }
    void accept(Visitor& v) { v.visit(*this); }
};

class Bird: virtual public Terrestrial{
public:
    void eat() { cout << "Bird is eating..." << endl; }
    void fly() { cout << "Flying..." << endl; }
    void accept(Visitor& v) { v.visit(*this); }
};

class Amphibious: public Ocean, virtual public Terrestrial{
public:
    void eat() { cout << "Amphibious is eating..." << endl; }
    void accept(Visitor& v) { v.visit(*this); }
};

class Waterfowl: public Bird, public Amphibious{
public:
    void eat() { cout << "Waterfowl is eating..." << endl; }
    void accept(Visitor& v) { v.visit(*this); }
};

class Visitor_1: public Visitor {
public:
    void visit(Ocean& o) override { o.eat(); }
    void visit(Terrestrial& t) override { t.eat(); }
    void visit(Bird& b) override { b.eat(); }
    void visit(Amphibious& a) override { a.eat(); }
    void visit(Waterfowl& w) override { w.eat(); }
};

class Visitor_2: public Visitor {
public:
    void visit(Ocean& o) override {
        o.eat();
        o.swim();
    }
    void visit(Terrestrial& t) override {
        t.eat();
        t.walk();
    }
    void visit(Bird& b) override {
        b.eat();
        b.fly();
    }
    void visit(Amphibious& a) override {
        a.eat();
        a.eat();
    }
    void visit(Waterfowl& w) override {
        w.eat();
        w.walk();
        w.swim();
        w.fly();
    }
};

int main(){
    vector<Generic*> g;

    g.push_back(new Ocean());
    g.push_back(new Terrestrial());
    g.push_back(new Bird());
    g.push_back(new Amphibious());
    g.push_back(new Waterfowl());

    Visitor_1 v1;
    Visitor_2 v2;

    for(auto i : g){
        i->accept(v1);
        cout << "-----" << endl;
    }
    cout << "====================" << endl;
    for(auto i : g){
        i->accept(v2);
        cout << "-----" << endl;
    }
}
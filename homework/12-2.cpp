#include <iostream>
#include <list>

using namespace std;

class Generic {
public:
    virtual void eat() = 0;
    virtual ~Generic() {}
};



class Ocean: virtual public Generic{
public:
    void swim() { cout << "Swimming..." << endl; }
};
class Fish: public Ocean{
public:
    void eat() override { cout << "Fish is eating fish food." << endl; }
};
class Whale: public Ocean{
public:
    void eat() override { cout << "Whale is eating plankton." << endl; }
};



class Terrestrial: virtual public Generic{
public:
    void walk(){ cout << "Walking..." << endl; }
};
class Elephant: public Terrestrial{
public:
    void eat() override { cout << "Elephant is eating vegetation." << endl; }
};
class Lion: public Terrestrial{
public:
    void eat() override { cout << "Lion is eating elephant." << endl; }
};



class Bird: virtual public Terrestrial{
public:
    void fly(){ cout << "Flying..." << endl; }
};
class Eagle: public Bird{
public:
    void eat() override { cout << "Eagle is eating mouse." << endl; }
};
class Sparrow: public Bird{
public:
    void eat() override { cout << "Sparrow is eating seeds." << endl; }
};



class Amphibious: public Ocean, virtual public Terrestrial{};
class Crocodile: public Amphibious {
public:
    void eat() override { cout << "Crocodile is eating legs" << endl; }
};
class Frog: public Amphibious {
public:
    void eat() override { cout << "Frog is eating mosquitoes." << endl; }
};



class Waterfowl: public Bird, public Amphibious{};
class Albatross: public Waterfowl{
public:
    void eat() override { cout << "Albatross is eating fish1." << endl; }
};
class Duck: public Waterfowl{
public:
    void eat() override { cout << "Duck is eating fish2." << endl; }
};





class Creator {
public:
    virtual Generic* create_type1() const = 0;
    virtual Generic* create_type2() const = 0;
};

class OceanCreator: public Creator {
    Generic* create_type1() const override { return new Fish; }
    Generic* create_type2() const override { return new Whale; }
};

class TerrestrialCreator: public Creator {
    Generic* create_type1() const override { return new Elephant; }
    Generic* create_type2() const override { return new Lion; }
};

class BirdCreator: public Creator {
    Generic* create_type1() const override { return new Eagle; }
    Generic* create_type2() const override { return new Sparrow; }
};

class AmphibiousCreator: public Creator {
    Generic* create_type1() const override { return new Crocodile; }
    Generic* create_type2() const override { return new Frog; }
};

class WaterfowlCreator: public Creator {
    Generic* create_type1() const override { return new Albatross; }
    Generic* create_type2() const override { return new Duck; }
};


// Благодаря абстрактной фабрике нам удалось вдвое уменьшить
// количество классов, создающих животных.

int main(){
    Creator** creators = new Creator*[5];
    Generic** zoo = new Generic*[10];
    
    creators[0] = new OceanCreator;
    creators[1] = new TerrestrialCreator;
    creators[2] = new BirdCreator;
    creators[3] = new AmphibiousCreator;
    creators[4] = new WaterfowlCreator;

    for(int i=0; i<5; i++){
        zoo[0 + i*2] = creators[i]->create_type1();
        zoo[1 + i*2] = creators[i]->create_type2();
    }

    for(int i=0; i<10; i++)
        zoo[i]->eat();
}

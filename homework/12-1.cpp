#include <iostream>
#include <string>
#include <list>

using namespace std;

class Product {
public:
    virtual void be() = 0;
};

class House: public Product {};
class BrickHouse: public House {
public:
    void be() override { cout << "Brick house is staying." << endl; }
};
class WoodenHouse: public House {
public:
    void be() override { cout << "Wooden house is staying." << endl; }
};
class ConcreteHouse: public House {
public:
    void be() override { cout << "Concrete house is staying." << endl; }
};

class Fence: public Product {};
class BrickFence: public Fence {
public:
    void be() override { cout << "Brick fence is staying." << endl; }
};
class WoodenFence: public Fence {
public:
    void be() override { cout << "Wooden fence is staying." << endl; }
};
class ConcreteFence: public Fence {
public:
    void be() override { cout << "Concrete fence is staying." << endl; }
};

class Barn: public Product {};
class BrickBarn: public Barn {
public:
    void be() override { cout << "Brick barn is staying." << endl; }
};
class WoodenBarn: public Barn {
public:
    void be() override { cout << "Wooden barn is staying." << endl; }
};
class ConcreteBarn: public Barn {
public:
    void be() override { cout << "Concrete barn is staying." << endl; }
};

class Developer {
public:
    virtual House* BuildHouse() = 0;
    virtual Fence* BuildFence() = 0;
    virtual Barn*  BuildBarn () = 0;
};
class BrickDeveloper: public Developer {
public:
    House* BuildHouse() override { return new BrickHouse; }
    Fence* BuildFence() override { return new BrickFence; }
    Barn*  BuildBarn () override { return new BrickBarn;  }
};
class WoodenDeveloper: public Developer {
public:
    House* BuildHouse() override { return new WoodenHouse; }
    Fence* BuildFence() override { return new WoodenFence; }
    Barn*  BuildBarn () override { return new WoodenBarn;  }
};
class ConcreteDeveloper: public Developer {
public:
    House* BuildHouse() override { return new ConcreteHouse; }
    Fence* BuildFence() override { return new ConcreteFence; }
    Barn*  BuildBarn () override { return new ConcreteBarn;  }
};

int main(){
    Product** area = new Product*[9];
    Developer** devs = new Developer*[3];
    
    devs[0] = new BrickDeveloper;
    devs[1] = new WoodenDeveloper;
    devs[2] = new ConcreteDeveloper;

    for(int i=0; i<3; i++){
        area[0 + i*3] = devs[i]->BuildHouse();
        area[1 + i*3] = devs[i]->BuildFence();
        area[2 + i*3] = devs[i]->BuildBarn();
    }

    for(int i=0; i<9; i++)
        area[i]->be(); 
}
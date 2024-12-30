#include <iostream>
#include <vector>
#include <string>

using namespace std;

class House {
public:
    string base;
    string walls;
    string roof;

    void print() {cout << "{ " << base << " " << walls << " " << roof << " }" << endl; }
};

class Documentation {
public:
    vector<string> text;
    Documentation() { text.push_back("some text"); }
    void print() { for(string i : text) cout << "- " << i << endl; }
};

class HouseBuilder{
protected:
    House* house;
    Documentation* docs;
public:
    HouseBuilder(){
        house = new House;
        docs = new Documentation;
    }

    virtual ~HouseBuilder() {}

    void ProduceBase () { house->base = "base"; }

    virtual void ProduceWalls() = 0;
    
    void ProduceRoof () { house->roof = "roof"; }

    void ProduceDocs () {
        docs->text.push_back(house->base);
        docs->text.push_back(house->walls);
        docs->text.push_back(house->roof);
    }

    House* GetHouse() {
        House* res = new House;
        res->base = house->base;
        res->walls = house->walls;
        res->roof = house->roof;
        delete house;
        house = new House;
        return res;
    }

    Documentation* GetDocs() {
        Documentation* res = new Documentation;
        res->text = docs->text;
        delete docs;
        docs = new Documentation;
        return res;
    }
};

class BrickHouseBuilder: public HouseBuilder {
public:
    void ProduceWalls() override { house->walls = "brick_walls"; }
};
class WoodenHouseBuilder: public HouseBuilder {
public:
    void ProduceWalls() override { house->walls = "wooden_walls"; }
};
class ConcreteHouseBuilder: public HouseBuilder {
public:
    void ProduceWalls() override { house->walls = "concrete_walls"; }
};



int main(){
    HouseBuilder** hb = new HouseBuilder*[3];
    House* house = new House;
    Documentation* docs = new Documentation;

    hb[0] = new BrickHouseBuilder();
    hb[1] = new WoodenHouseBuilder();
    hb[2] = new ConcreteHouseBuilder();



    hb[0]->ProduceBase();
    hb[0]->ProduceWalls();
    hb[0]->ProduceRoof();
    hb[0]->ProduceDocs();

    house = hb[0]->GetHouse();
    docs = hb[0]->GetDocs();
    
    cout << "===" << endl;
    house->print();
    docs->print();



    hb[1]->ProduceWalls();
    hb[1]->ProduceDocs();

    house = hb[1]->GetHouse();
    docs = hb[1]->GetDocs();

    cout << "===" << endl;
    house->print();
    docs->print();



    hb[2]->ProduceDocs();

    house = hb[2]->GetHouse();
    docs = hb[2]->GetDocs();

    cout << "===" << endl;
    house->print();
    docs->print();

}
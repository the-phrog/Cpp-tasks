#include "modules/GameOfLife.hpp"

using namespace std;

int main(int argc, char* argv[]){
    Interface life;

    life.game(argc, argv, "presets/big_A.life");
}

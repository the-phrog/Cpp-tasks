#include <iostream>
#include "modules/circular_buffer.hpp"

using namespace std;
int main(){
    int* a = new int[5];
    a[0] = 14;
    a[1] = 88;
    a[2] = 13;
    a[3] = 37;
    a[4] = 77;
    int* b = new int[7];
    for(int i=0; i<5; i++)
        b[i] = a[i];
    b[5] = 56;
    b[6] = 78;
    delete[] a;
    a = b;
    for(int i=0; i<7; i++)
        cout << a[i] << " ";
    cout << "\n";
    delete[] b;
}

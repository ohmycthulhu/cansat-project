#include "arduino/include/test.cpp"
#include <iostream>

using namespace std;

int main(int argc, char** argv) {
    for (int i = 0; i < 10; i++) {
        test::test();
    }
    #ifdef linux
    cout << "Linux" << endl;
    #else
    cout << "Not linux" << endl;
    #endif
}
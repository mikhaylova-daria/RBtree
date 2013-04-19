#include <iostream>
#include "RBtree.h"
using namespace std;

int main()
{
    RBtree<int> A;
    int x;
    int n = 14;
    for (int i = 0; i < n; ++i) {
        std::cin>>x;
        A.insert(x);
    }
    A.print();
    cout << "Hello World!" << endl;
    return 0;
}


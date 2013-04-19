#include <iostream>
#include "RBtree.h"
using namespace std;

int main()
{
    RBtree< std::string > A;
  //  int x;
    int n = 5;
    string a;
    for (int i = 0; i < n; ++i) {
        std::cin>>a;
        A.insert(a);
    }
    //A.print();
    //A.deleting(5);
    A.deleting("1");
    A.print();
    std::cout<<A.max_key()<< " "<<A.min_key();
    cout << "Hello World!" << endl;
    return 0;
}


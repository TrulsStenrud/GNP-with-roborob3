#include "RoboroboMain/common.h"
#include "main.h"
#include "../masterthesis/MscMain.h"

/*
 * This class is purely for redirecting program flow to the "masterthesis" folder.
 */

#include "GNPEvolver.h"
#include "GNPGenome.h"

struct Test {
    int A;
    Test(int a){
        A = a;
    }
    friend bool operator<(Test &a_lhs, Test &a_rhs)
    {
        return (a_lhs.A > a_rhs.A);
    }
};

inline bool test( Test& struct1,  Test& struct2)
{
    return (struct1 < struct2);
}



int main(int argc, char* argv[]){
//	MscMain* mscMain = new MscMain(argc, argv);
//	delete mscMain;
//	std::exit(0);
    
    
    std::vector<Test> a;
    std::vector<Test> b;

    a.push_back(Test(5));
    a.push_back(Test(2));
    a.push_back(Test(4));
    a.push_back(Test(1));
    
    Test& c = a[0];
    c.A = 12;
    b.push_back(c);
    
    c.A = 21;
    
    for(auto value : a){
        std::cout << value.A << std::endl;
    }
    
    for(auto value : b){
       std::cout << value.A << std::endl;
    }
    
}



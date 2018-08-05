#include <iostream>

using namespace std;

/*
void func(void)
{
    throw("dadadadada");
    cout<<"huangdao"<<endl;
}


int main()
{
    try
    {
        func();
    }
    
    catch(bad_alloc )
    {
        cout<<endl;
    }
    cout<<"hello world"<<endl;
}
*/

class test
{
	public:
	test(){cout<<"hello hello hello hello hello hello !!!"<<endl;}
};





int main()
{
	test *t=new test;
	return 1;
}




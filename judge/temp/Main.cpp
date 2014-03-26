#include <iostream>
  
using namespace std;
  
ostream& operator<< (ostream& out, const char* str)
{
    out << string("Initialize\n") << string(str) << string("\nClean up");
    return out;
}

int main()
{
	cout << "Hello,world!" << endl;
}
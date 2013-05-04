#include <iostream>
#include <string.h>
using namespace std;
int main( )
{
	char input[81];
	int flag = 0;
	int T;
	cin >> T;
	for( int i=0; i<T; ++i )
	{
		flag = 0;
		cin >> input;
		int length = strlen( input );
		for( int j=0; j<length; ++j )
		{
			
			if( input[j] == '!' )
			{
				flag++;
				continue;
			}

			else
			{
				if( flag%2 == 0 )
				{
					cout << input[j];
					continue;
				}
				else
				{
					if( input[j]>='a' && input[j]<='z' )
					{
						cout << char(input[j]-32);
					}
					else
					{
						cout << char(input[j]+32);
					}
				}
			}
			
		}
		cout << endl;
	}
	return 0;
}
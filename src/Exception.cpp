#include "Exception.h"
#include <string>
using namespace std;

Exception::Exception(string s)
{
	reason = s;
}

string Exception::what()
{
	return reason;
}
#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <string>

using namespace std;

class Exception
{
	private:
		string reason;

	public:
		Exception(string s);
		string what();
};

#endif

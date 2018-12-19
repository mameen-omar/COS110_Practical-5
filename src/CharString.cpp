#include "Exception.h"
#include "CharString.h"
#include <string>
#include <cctype>
#include <cstdlib>
#include <sstream>
#include <iostream>

using namespace std;

/* Constructors */
	CharString::CharString()
	{
		numChars = 0;
		characters = 0;
	}

	CharString::CharString(const char* arr, int size)
	{
		numChars = size;

		characters = new char[size];

		for(int count = 0; count < size; count++)
		{			
				characters[count] = arr[count];						
		}
	}

	/* Copy constructor */
	CharString::CharString(const CharString& s)
	{
		this->numChars = s.numChars;
		this->characters = new char[this->numChars];

		for(int count = 0; count < numChars; count++)
		{			
			this->characters[count] = s.characters[count];		
		}
	}

	/* Destructor */
	CharString::~CharString()
	{
		delete [] characters;
		numChars = 0;
	}

	/* Assignment operator */
	const CharString CharString::operator= (const CharString &s)
	{
		if(&s == this)
		{
			return *this;
		}

		this->numChars = 0;

		this->reallocateArray(s.length());

		for(int count = 0; count < this->numChars; count++)
		{
			this->characters[count] = s.characters[count];
		}

		return *this;
	}

	/* Addition operators */
	CharString CharString::operator+ (const CharString &s)
	{
		if(s.length() <= 0)
		{
			throw Exception("The string is empty!");
		}

		CharString* temp = new CharString(*this);

		int oldSize = this->length();
		int sizeR = s.length();
		int newSize = oldSize + s.length();

		temp->reallocateArray(newSize);

		for(int x = 0; x<sizeR; x++)
		{
			temp->characters[x+oldSize] = s.characters[x];
		}

		return *temp;		
	}

	void CharString::operator+= (const CharString &s)
	{
		if(s.length() <= 0)
		{
			throw Exception("The string is empty");
		}

		int newSize = this->length() + s.length();
		int oldSize = this->length();

		//Make left array bigger
		this->reallocateArray(newSize);

		for(int count = 0; count < ( s.length() );count++)
		{
			this->characters[count+oldSize] = s.characters[count];
		}
	}

	/* Subtraction operators */
	CharString CharString::operator- (const char &c)
	{
		if(this->length() == 0)
		{
			throw Exception("The string is empty!");
		}

		CharString* temp = new CharString(*this);

		//Temp is a copy of our left

		bool found = false;

		//Look for char
		for(int x = 0; x < temp->length() ; x++)
		{
			if(temp->characters[x] == c)
			{
				found = true;

				for(int y = x; y< ( temp->length() ); y++)
				{
					temp->characters[y] = temp->characters[y+1];
				}

				//Sort out the last character in the array left dangling
				temp->reallocateArray( (temp->length() - 1 ) );
			}			
		}

		//if not found
		if(found == false)
		{
			string error = "No character \'";
			error = error + c;
			string error2 = "\' found!";
			error = error + error2;
			throw Exception(error);
		}
		//if empty
		else if(temp->length() == 0)
		{
			throw Exception("The result is an empty string");
		}

		return *temp;
	}

	void CharString::operator-= (const char &s)
	{	
		if(this->length() == 0)
		{
			throw Exception("The string is empty!");
		}

		bool found = false;

		//Look for char
		for(int x = 0; x < this->length() ; x++)
		{
			if(this->characters[x] == s)
			{
				found = true;

				for(int y = x; y<this->length(); y++)
				{
					this->characters[y] = this->characters[y+1];
				}

				//Sort out the last character in the array left dangling
				this->reallocateArray( (this->length() - 1 ) );
			}			
		}
		//if not found
		if(found == false)
		{
			string error = "No character \'";
			error = error + s;
			string error2 = "\' found!";
			error = error + error2;
			throw Exception(error);
		}
		//if empty
		else if(this->length() == 0)
		{
			throw Exception("The result is an empty string");
		}
	}

	/* Multiplication operators */
	CharString CharString::operator* (const CharString &s)
	{
		if( this->length() != s.length() )
		{
			throw Exception("Strings are unequal lengths!");
		}

		
		//initialise with any BS
		CharString* temp2 = new CharString(*this);

		//temp is copy of our left
		int newSize = (2* (s.length()));

		//temp2 will be our return
		temp2->reallocateArray(newSize);

		int countL = 0;
		int countR = 0;

		for(int x = 0; x< temp2->length(); x++)
		{
			if( (x%2 == 0) )
			{
				temp2->characters[x] = this->characters[countL];
				countL++;
			}
			
			else 
			{
				temp2->characters[x] = s.characters[countR];
				countR++;
			}			
		}

		return (*temp2);
	}

	void CharString::operator*= (const CharString &s)
	{
		if( this->length() != s.length() )
		{
			throw Exception("Strings are unequal lengths!");
		}

		//initialise with any BS
		CharString* temp2 = new CharString(*this);

		//temp is copy of our left
		int newSize = (2* (s.length()));

		//temp2 will be our return
		temp2->reallocateArray(newSize);

		int countL = 0;
		int countR = 0;

		for(int x = 0; x< temp2->length(); x++)
		{
			if( (x%2 == 0) )
			{
				temp2->characters[x] = this->characters[countL];
				countL++;
			}
			
			else 
			{
				temp2->characters[x] = s.characters[countR];
				countR++;
			}			
		}

		delete [] this->characters;
		this->numChars = temp2->numChars;
		this->characters = temp2->characters;

	}

	/* Division operators */
	CharString CharString::operator/ (const CharString &s)
	{
		if(numChars == 0 || s.numChars == 0)
		{
			throw Exception("The string is empty!");
		}

		CharString* temp = new CharString(*this);
		
		
		bool found = false;
		bool add = false;
		int countT = 0;
		int lCount;
		int rCount;
		//our return array is empty
		temp->reallocateArray(0);

		for(lCount = 0; lCount < s.length(); lCount++)
		{
			add = true;

			for(rCount = 0; rCount < this->length(); rCount++)
			{
				
				if(this->characters[lCount] == s.characters[rCount])
				{
					found = true;
					add = false;					
				}

			}

			if(add == true)
				{
					int newSize = temp->length()+1;
					temp->reallocateArray(newSize);
					temp->characters[countT] = this->characters[lCount];
					countT++;				
				}
		}

		if(temp->length() == 0)
		{
			throw Exception("The result is an empty string!");
		}

		else if(found == false)
		{
			throw Exception("No character found!");
		}

		return *this;

	}



	void CharString::operator/= (const CharString &s)
	{
		if(s.length() == 0 || this->length() == 0)
		{
			throw Exception("The string is empty!");
		}

		CharString* temp = new CharString(*this);
		
		
		bool found = false;
		bool add = false;
		int countT = 0;
		//our return array is empty
		temp->reallocateArray(0);

		for(int lCount = 0; lCount < s.length(); lCount++)
		{
			add = true;

			for(int rCount = 0; rCount < this->length(); rCount++)
			{
				
				if(this->characters[lCount] == s.characters[rCount])
				{
					found = true;
					add = false;					
				}

			}

			if(add == true)
				{
					int newSize = temp->length()+1;
					temp->reallocateArray(newSize);
					temp->characters[countT] = this->characters[lCount];
					countT++;				
				}
		}

		if(found == false)
		{
			throw Exception("No character found!");
		}

		delete [] this->characters;
		this->numChars = temp->length();
		this->characters = temp->characters;

		if(temp->length() == 0)
		{
			throw Exception("The result is an empty string!");
		}
		
	}

	/* Equivalence operator */
	bool CharString::operator == (const CharString &s)
	{
		if(this->numChars != s.numChars)
		{
			return false;
		}

		else
		{
			bool flag;
			flag = true;

			for(int count = 0; count < (this->numChars); count++)
			{
				if(this->characters[count] != s.characters[count])
				{
					flag = false;
					return flag;
				}//end if

				else if(this->characters[count] == s.characters[count])
				{
					flag = true;
				}
			}//end else for

			return flag;
		}//end else
	}

	/* Element access operators */
	char & CharString::operator[](int i) const
	{
		if ( (i >= this->numChars) || (i<0))
		{	
			throw Exception("Index out of bounds!");
		}

		else
		{
			char* c = new char;
			*c = this->characters[i];
			return (*c);
		}

	}

	
	/* Get the length of the array */
	const int CharString::length() const
	{
		return numChars;
	}


	/* Allocate more space to the array */
	void CharString::reallocateArray(int newSize)
	{
		if(newSize < 0)
		{
			return;
		}


		char *temp = new char[newSize];

		for(int count = 0; count < numChars; count++)
		{
			temp[count] = this->characters[count];
		}

		delete [] this->characters;
		this->characters = temp;
		this->numChars = newSize;
	}

	/* Added operators */
	CharString CharString::operator+ (const char &c)
	{
		CharString* temp = new CharString(*this);

		temp->reallocateArray(this->length() + 1 );

		temp->characters[this->length()] = c;

		return *temp;
	}


	CharString CharString::operator+= (const char &c)
	{
		int oldSize = this->length();

		this->reallocateArray(this->length() + 1 );

		this->characters[oldSize] = c;

		return *this;
	}

	ostream &operator << (ostream &os, const CharString &s)
	{	
		int size = s.length();

		char * temp = new char[size+1];

		for(int x = 0; x<size;x++)
		{
			temp[x] = s.characters[x];
		}

		temp[size] = '\0';

		string str(temp);

		os<< str;

		return os;
	}
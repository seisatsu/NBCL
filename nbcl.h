/*****************************************
** No Bullsh*t Command Line Parser      **
** nbcl.h                               **
** Copyright 2011-2013 PariahSoft LLC   **
*****************************************/

// **********
// Permission is hereby granted, free of charge, to any person obtaining a copy 
// of this software and associated documentation files (the "Software"), to 
// deal in the Software without restriction, including without limitation the 
// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or 
// sell copies of the Software, and to permit persons to whom the Software is 
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in 
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE 
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING 
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS 
// IN THE SOFTWARE.
// **********

#ifndef CMD_H
#define CMD_H

#include <sstream>
#include <string>
#include <vector>

#if defined __unix__ || defined __APPLE__
	#include <sys/ioctl.h>
	#include <unistd.h>
#elif _WIN32
	#include <windows.h>
#endif

class NBCL
{
public:
	//! Initialize the parser with argc and argv.
	NBCL(int argc, char** argv);
	~NBCL();

	//! Add an option to the parser.
	/*!
		sopt: Full short option, ex. "-c", empty string if no short 
			option.
		lopt: Full long option, ex. "--config", required.
		arg: A short description of the argument expected to the 
			option, an empty string if no argument.
		desc: A description of the argument, required.
	*/
	void insert(std::string shortopt, std::string longopt,
		std::string arg, std::string desc);

	//! Set the description of the stray arguments.
	/*!
		arg: A short description of the stray arguments.
		A stray argument is an argument not accompanied by an option switch.
		Defaults to empty string.
	*/
	void setStrayArgsDesc(std::string desc);

	//! Process the command line.
	/*!
		Run after all of your inserts.
		Returns true if succeeded, false if failed.
	*/
	bool parse();

	//! Check if option was used.
	/*!
	    check: Longopt string to check. Checks both long and short option use.
		Returns true if option was used, false otherwise.
	*/
	bool check(std::string longopt);

	//! Get the string value of an option's argument.
	/*!
		longopt: Longopt string to check. Checks both long and short option use.
		Returns string value if succeeded, empty string if failed.
	*/
	std::string get(std::string longopt);

	//! Get the stray arguments.
	/*!
		Returns a string vector of the stray arguments found on the command
			line, in order.
	*/
	std::vector<std::string> getStrayArgsList();

	//! Print a pretty usage/help message.
	/*!
		Automatically formatted from the list of inserted options.
	*/
	void usage();

private:
	int findOpt(std::string name);

	void usageSize(int* optmax, int* argmax);
	void usagePrintShort();
	void usagePrintLong(int optmax, int argmax);
	std::string itostr(int in);

	unsigned int getConsoleWidth(); /* Not currently used for anything. */

	int argc;
	char** argv;

	struct Option {
		std::string shortopt;
		std::string longopt;
		std::string arg;
		std::string desc;
		bool present;
		std::string value;
	};
	std::vector<Option*> OptList;

	std::string strayArgsDesc;
	std::vector<std::string> strayArgsList;
};

#endif


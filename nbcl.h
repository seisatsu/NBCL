/************************************
** No Bullsh*t Command Line Parser **
** nbcl.h                          **
** Copyright 2011-2012 OmegaSDG    **
************************************/

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

	//! Set the name of the final argument.
	/*!
		arg: A short description of the final argument. Defaults to "<name>".
	*/
	void setFinalArgName(std::string name);

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

	//! Get the final argument.
	/*!
		Returns string value of the final argument (one without a switch) if it
			exists, empty string otherwise.
	*/
	std::string getFinal();

	//! Print a pretty usage/help message.
	/*!
		Automatically formatted from the list of inserted options.
	*/
	void usage();

private:
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
	std::string finalArgName;
	std::string finalArgValue;

	std::vector<Option*> OptList;
};

#endif


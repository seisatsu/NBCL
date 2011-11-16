/************************************
** No Bullsh*t Command Line Parser **
** nbcl.h                          **
** Copyright 2011 OmegaSDG         **
************************************/

#ifndef CMD_H
#define CMD_H

#include <sstream>
#include <string>
#include <vector>

class NBCL
{
public:
	//! Initialize the parser with argc and argv.
	NBCL(int argc, char** argv);
	~NBCL();

	//! Add an option to the parser.
	/*!
		shortopt:		Full short option, ex. "-c", empty string if no short 
						option.
		longopt:		Full long option, ex. "--config", required.
		argument:		A short description of the argument expected to the 
						option, an empty string if no argument.
		description:	A description of the argument, required.
	*/
	void insert(std::string shortopt, std::string longopt,
		std::string argument, std::string description);

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
		check: Longopt string to check. Checks both long and short option use.
		Returns string value if succeeded, empty string if failed.
	*/
	std::string get(std::string longopt);

	//! Print a pretty usage/help message.
	/*!
		Automatically formatted from the list of inserted options.
	*/
	void usage();

private:
	void usageSize(int* optmaxlen, int* argmaxlen);
	void usagePrintShort();
	void usagePrintLong(int optmaxlen, int argmaxlen);
	std::string itostr(int in);

	int argc;
	char** argv;

	struct Option {
		std::string shortopt;
		std::string longopt;
		std::string argument;
		std::string description;
		bool present;
		std::string value;
	};

	std::vector<Option*> OptionsList;
};

#endif


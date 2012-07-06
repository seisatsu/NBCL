/************************************
** No Bullsh*t Command Line Parser **
** nbcl.cpp                        **
** Copyright 2011-2012 OmegaSDG    **
************************************/

#include <stdio.h>

#include "nbcl.h"

NBCL::NBCL(int argc, char** argv) :
	argc(argc), argv(argv)
{
	finalArgName = "<name>";
}

NBCL::~NBCL()
{
	unsigned int opt;

	for (opt = 0; opt < OptList.size(); opt++)
		delete OptList[opt];
}

void NBCL::insert(std::string shortopt, std::string longopt,
	std::string arg, std::string desc)
{
	Option* opt = new Option;
	opt->shortopt = shortopt;
	opt->longopt = longopt;
	opt->arg = arg;
	opt->desc = desc;
	opt->present = false;
	opt->value = "";
	OptList.push_back(opt);
}

void NBCL::setFinalArgName(std::string name)
{
	finalArgName = name;
}

/* TODO: Clean up and rewrite to handle final argument. */
bool NBCL::parse()
{
	int argn;
	unsigned int opt;
	bool found;

	for (argn = 1; argn < argc; argn++) { /* Read command line. */
		found = false;

		for (opt = 0; opt < OptList.size(); opt++) {
			if (!OptList[opt]->shortopt.compare(argv[argn]) || !OptList[opt]->longopt.compare(argv[argn])) {
				found = true; /* We found a command line option. */
				OptList[opt]->present = true; /* This option has been called. */

				if (!OptList[opt]->arg.empty()) { /* Read in option value. */
					if (argn + 1 < argc) {
						OptList[opt]->value = argv[argn+1];
						argn += 1;
					}

					else
						return false;
				}

				break; /* Check to see if another option was called. */
			}
		}
		if (!found) /* We didn't find any command line options. */
			return false;

	}
	return true;
}

bool NBCL::check(std::string longopt)
{
	unsigned int search;

	for (search = 0; search < OptList.size(); search++) {
		if (!OptList[search]->longopt.compare(longopt)) /* Option was used. */
			return OptList[search]->present;
	}

	return false;
}

std::string NBCL::get(std::string longopt)
{
	unsigned int search;

	for (search = 0; search < OptList.size(); search++) {
		if (!OptList[search]->longopt.compare(longopt)) /* Option was found. */
			return OptList[search]->value;
	}

	return "";
}

void NBCL::usage()
{
	int optmax = 0;
	int argmax = 0;

	usageSize(&optmax, &argmax);
	usagePrintShort();
	usagePrintLong(optmax, argmax);
}

std::string NBCL::getFinal()
{
	return final;
}

/* Makes width calculations needed for formatting usage output. */
void NBCL::usageSize(int* optmax, int* argmax)
{
	unsigned int opt;

	for (opt = 0; opt < OptList.size(); opt++) {
		if (OptList[opt]->shortopt.size() + OptList[opt]->longopt.size() > (unsigned)*optmax)
			*optmax = (int)OptList[opt]->shortopt.size() + (int)OptList[opt]->longopt.size() + 1;

		if ((int)OptList[opt]->arg.size() > *argmax)
			*argmax = (int)OptList[opt]->arg.size();
	}
}

void NBCL::usagePrintShort()
{
	fprintf(stderr, "Usage: %s [OPTIONS]", argv[0]);
}

/* Lots of ugly formatting code. */
void NBCL::usagePrintLong(int optmax, int argmax)
{
	unsigned int opt;

	fprintf(stderr, "\n\nAvailable options:\n");

	for (opt = 0; opt < OptList.size(); opt++) {
		if (!OptList[opt]->shortopt.empty())
			fprintf(stderr, "  %s,", OptList[opt]->shortopt.c_str());
		else
			fprintf(stderr, "     ");

		fprintf(stderr, (std::string("%-")+itostr(optmax+1)+"s").c_str(), OptList[opt]->longopt.c_str());

		if (!OptList[opt]->arg.empty())
			fprintf(stderr, (std::string("%-")+itostr(argmax+3)+"s").c_str(), OptList[opt]->arg.c_str());
		else
			fprintf(stderr, (std::string("%-")+itostr(argmax+3)+"c").c_str(), ' ');

		fprintf(stderr, "%s\n", OptList[opt]->desc.c_str());
	}

	fprintf(stderr, "\n\n");
}

std::string NBCL::itostr(int in)
{
	std::stringstream out;
	out << in;
	return out.str();
}

/* Thanks to:
 * http://stackoverflow.com/questions/1022957
 * http://stackoverflow.com/questions/6812224
*/
unsigned int NBCL::getConsoleWidth()
{
	#if defined __unix__ || defined __APPLE__
		struct winsize w;
		ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
		return (unsigned int)w.ws_col;
	#elif _WIN32
		CONSOLE_SCREEN_BUFFER_INFO csbi;
		int ret = GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
		if (ret)
			return (unsigned int)csbi.dwSize.X;
		else
			return 0; /* Failure */
	#else
		return 0; /* Failure */
	#endif
}


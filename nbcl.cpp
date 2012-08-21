/************************************
** No Bullsh*t Command Line Parser **
** nbcl.cpp                        **
** Copyright 2011-2012 OmegaSDG    **
************************************/

#include <stdio.h>
#include <string.h>

#include "nbcl.h"

NBCL::NBCL(int argc, char** argv) :
	argc(argc), argv(argv), strayArgsDesc("")
{
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

void NBCL::setStrayArgsDesc(std::string desc)
{
	strayArgsDesc = desc;
}

bool NBCL::parse()
{
	int argn;
	int opt;

	for (argn = 1; argn < argc; argn++) { /* Read command line. */
		opt = findOpt(argv[argn]);
		if (opt!=-1) { /* This argument is an option switch. */
			if (!OptList[opt]->arg.empty()) { /* It takes an argument. */
				if (argn + 1 < argc) {
					OptList[opt]->value = argv[argn+1]; /* Add next argument as the option value. */
					argn++;
				}
				else
					return false;
			}
			OptList[opt]->present = true; /* Tell the option it exists. */
		}
		else /* This argument is a stray. */
			strayArgsList.push_back(argv[argn]);
	}
	return true;
}

bool NBCL::check(std::string longopt)
{
	int opt = findOpt(longopt);
	if (opt!=-1)
		return OptList[opt]->present;
	else
		return false;
}

std::string NBCL::get(std::string longopt)
{
	int opt = findOpt(longopt);
	if (opt!=-1)
		return OptList[opt]->value;
	else
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

std::vector<std::string> NBCL::getStrayArgsList()
{
	return strayArgsList;
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
	#ifdef _WIN32
		char fname[_MAX_FNAME];
		char ext[_MAX_EXT];
		_splitpath(argv[0], NULL, NULL, fname, ext);
		if (ext[strlen(ext)-1] == ' ') /* Windows thinks I want a space at the end. It's wrong. */
			ext[strlen(ext)-1] = 0;
		fprintf(stderr, "Usage: %s%s [OPTIONS] ", fname, ext);
	#else
		fprintf(stderr, "Usage: %s [OPTIONS] ", argv[0]);
	#endif
	fprintf(stderr, strayArgsDesc.c_str());
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
}

std::string NBCL::itostr(int in)
{
	std::stringstream out;
	out << in;
	return out.str();
}

int NBCL::findOpt(std::string name)
{
	for (unsigned int opt = 0; opt < OptList.size(); opt++) {
		if (!OptList[opt]->shortopt.compare(name) ||
			!OptList[opt]->longopt.compare(name))
				return opt;
	}
	return -1;
}

/* Thanks to:
 * http://stackoverflow.com/questions/1022957
 * http://stackoverflow.com/questions/6812224
*/
/* TODO: Use for breaking lines on small consoles. */
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


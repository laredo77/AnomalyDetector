/*
 * CLI.h
 *
 * Author: 311547087, Itamar Laredo
 */
#ifndef CLI_H_
#define CLI_H_

#include <string.h>
#include "commands.h"

using namespace std;

/*
 * Command Line Interface class.
 */
class CLI {
	DefaultIO* dio;
	CLIData* clid;
    // you can add data members
    std::map<std::string, Command*> menu;

public:
	CLI(DefaultIO* dio);
	void start();
	virtual ~CLI();
};

#endif /* CLI_H_ */



#ifndef CLI_H_
#define CLI_H_

#include <string.h>
#include "commands.h"

using namespace std;

class CLI {
	DefaultIO* dio;
	CLIData* clid;
    // you can add data members
    std::map<std::string, Command*> menu;
	string description;

public:
	CLI(DefaultIO* dio, CLIData* clid);
	void start();
	virtual ~CLI();
};

#endif /* CLI_H_ */

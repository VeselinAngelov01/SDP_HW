#ifndef CONTROLLER_H
#define COTNROLLER_H

#include <unordered_map>
#include "Tree.h"

class Controller
{
private:
	// Structure to be saved our trees
	std::unordered_map<std::string,Tree> forest;
	// Shows all possible commands
	void help() const;
	// Split command (string) to arguments (vector<string>)
	void splitString(std::string& cmd, std::vector<std::string>& split);
	// Check tree exists in forest
	bool exists(std::string& name) const;
public:
	// Start function of project
	void run();
};

#endif

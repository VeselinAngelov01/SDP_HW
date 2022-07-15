#include "Controller.h"

void Controller::help() const
{
	std::cout << "*****HELP*****\n1.LOAD <Tree name> <File name>\n2.SAVE <Tree name> <File name>\n3.CONTAINS <First tree> <Second tree>\n4.REMOVE <First tree> <Second tree>\n5.HELP\n6.EXIT\n3/4 check for second in first tree\n";
}

void Controller::splitString(std::string& cmd, std::vector<std::string>& split)
{
	std::string tempArg;
	size_t size = cmd.size();
	for (size_t i = 0; i < size; ++i)
	{
		if (cmd[i] != ' ')
		{
			tempArg += cmd[i];
		}
		else
		{
			if (tempArg.length() > 0)
			{
				split.push_back(tempArg);
				tempArg.clear();
			}
		}
	}
	if (tempArg.length() > 0) split.push_back(tempArg);
}

bool Controller::exists(std::string& name) const
{
	// Return if there is a tree with specified name
	return forest.find(name) != forest.end();
}

void Controller::run()
{
	std::string command;
	std::string ERR;
	help();
	std::vector<std::string> split;
	while (true)
	{
		try
		{
			std::getline(std::cin, command); // Get user input
			splitString(command, split); // Split command
			command = split[0];
			if (command == "LOAD")
			{
				if (split.size() > 3) throw std::invalid_argument("Too many args!");
				if (!exists(split.at(1)))
				{
					Tree tree(split.at(2));
					forest.insert(std::pair<std::string, Tree>(split.at(1), tree));
					std::cout << "Tree successfully inserted!\n";
				}
				else
				{
					forest.at(split.at(1)).readFromFile(split.at(2));
					std::cout << "Tree successfully updated!\n";
				}
			}
			else if (command == "SAVE")
			{
				if (split.size() > 3) throw std::invalid_argument("Too many args!");
				if (!exists(split.at(1)))
				{
					throw std::invalid_argument("No such name tree!");
				}
				else
				{
					forest.at(split.at(1)).saveToFile(split.at(2));
					std::cout << "File saved!\n";
				}
				
			}
			else if (command == "CONTAINS")
			{
				if (split.size() > 3) throw std::invalid_argument("Too many args!");
				if (!exists(split.at(1)))
				{
					ERR = "Tree " + split.at(1) + " does not exists!";
					throw std::invalid_argument(ERR);
				}
				else if (!exists(split.at(2)))
				{
					ERR = "Tree " + split.at(2) + " does not exists!";
					throw std::invalid_argument(ERR);
				}
				if (forest.at(split.at(1)).contains(forest.at(split.at(2))))
				{
					std::cout << "Tree " << split.at(2) << " is a subtree of " << split.at(1)<<"!\n";
				}
				else std::cout << "Tree " << split.at(2) << " is not a subtree of " << split.at(1) << "!\n";
			}
			else if (command == "REMOVE")
			{
				if (split.size() > 3) throw std::invalid_argument("Too many args!");
			}
			else if (command == "HELP")
			{
				if (split.size() > 1) throw std::invalid_argument("Too many args!");
				help();
			}
			else if (command == "EXIT")
			{
				if (split.size() > 1) throw std::invalid_argument("Too many args!");
				std::cout << "Exiting program...";
				break;
			}
			else throw std::invalid_argument("Invalid command!");
			
		}
		catch (const std::exception& e)
		{
			std::cout << e.what() << std::endl;
		}
		split.clear();
	}
}
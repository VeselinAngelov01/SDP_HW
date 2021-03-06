#include "Tree.h"

Tree::Tree()
{
    this->root = nullptr;
    uniqueID = 0;
}

Tree::~Tree()
{
    clear(root);
}

void Tree::clear(Node*& node)
{
    if (!node) return;
    size_t size = node->children.size();
    for (size_t i = 0; i < size; ++i)
    {
        if (node->children.at(i))
        {
            clear(node->children.at(i));
        }
    }
    delete node;
    node = nullptr;
}

Tree::Tree(std::string& fileName)
{
    readFromFile(fileName);
}

Tree::Tree(const Tree& other)
{
    this->root = new Node(other.root->value, other.root->uniqueID);
    copy(root, other.root);
}

Tree& Tree::operator=(const Tree& other)
{
    if (this != &other)
    {
        clear(root);
        this->root = new Node(other.root->value, other.root->uniqueID);
        copy(root, other.root);
    }
    return *this;
}

void Tree::copy(Node*& current, Node* org)
{
    if (!org) return;
    size_t size = org->children.size();
    int value, uniqueID;
    for (size_t i = 0; i < size; ++i)
    {
        value = org->children.at(i)->value;
        uniqueID = org->children.at(i)->uniqueID;
        current->children.push_back(new Node(value,uniqueID));
        copy(current->children.at(i), org->children.at(i));
    }
}

bool Tree::addToTree(Node*& current, int uniqueId, int value, int newId)
{
    if (current->uniqueID == uniqueId)
    {
        size_t childrenCount = current->children.size();
        int indexToInsert = -1;
        for (size_t i = 0; i < childrenCount; ++i)
        {
            if (!current->children.at(i))
            {
                indexToInsert = i;
                continue;
            }
            else if (current->children.at(i)->value == value)
            {
                throw std::invalid_argument("Invalid data!");
            }
        }
        if (indexToInsert != -1)
        {
            current->children.erase(current->children.begin() + indexToInsert);
            current->children.insert(current->children.begin() + indexToInsert, new Node(value, newId));
        }
        else current->children.push_back(new Node(value,newId));
        return true;
    }
    else
    {
        bool res;
        size_t childrenCount = current->children.size();
        for (size_t i = 0; i < childrenCount; ++i)
        {
            res = addToTree(current->children.at(i), uniqueId, value, newId);
            if (res) return true;
        }
        return false;
    }

}

void Tree::insert(int uniqueId, int value)
{
    if (!root) root = new Node(value, uniqueID++);
    else
    {
        bool res = addToTree(root, uniqueId, value, uniqueID++);
        if (!res) throw std::invalid_argument("No such parent!");
    }
}

void Tree::getChildren(std::string& input, size_t startIndex,std::vector<int>& children)
{
    size_t inputSize = input.length();
    std::string temp;
    for (size_t i = startIndex; i < inputSize; ++i)
    {
        if (input.at(i) == '|') break;
        else if (input.at(i) != ' ')
        {
            temp += input.at(i);
        }
        else
        {
            if (temp.length() > 0)
            {
                children.push_back(std::stoi(temp));
                temp.clear();
            }
        }
    }
    if (temp.length() > 0) children.push_back(std::stoi(temp));
    
}

void Tree::readFromFile(std::string& fileName)
{
    std::ifstream reader(fileName);
    if (!reader.is_open()) throw std::invalid_argument("Can not open file!");
    // Clear in case of updating the tree
    clear(root);
    std::string line;
    int parentID = 0;   // Show ID of parent to be inserted new nodes
    size_t lastDel = 0 , nextDel = 0;  // last and next delimeter
    std::vector<int> children;  // vector with values of children
    while (std::getline(reader, line))
    {
        // ignore empty rows
        if (line.length() == 0) continue;
        lastDel = 0; 
        nextDel = 0;
        while (true)
        {
            nextDel = line.find('|',lastDel+1);
            if (nextDel == -1) break; // no more nodes after last '|'
            // Takes the string and convert to vector of values
            getChildren(line, lastDel+1, children);
            // Move last delimeter index
            lastDel = nextDel + 1;
            try
            {
                if (!root)
                {
                    // If there is not root and in vector are more than one value
                    // (2 or more representation of nodes => invalid state)
                    if (children.size() != 1)
                    {
                        reader.close();
                        throw std::invalid_argument("Invalid data at row 1!");
                    }
                    // Insert root
                    insert(0, children.at(0));
                }
                else
                {
                    // If current line does not represent root
                    // Create and insert new nodes to relevant parent
                    size_t size = children.size();
                    for (size_t i = 0; i < size; ++i)
                    {
                        insert(parentID, children.at(i));
                    }
                    // Change parent ID only on new line i.e. after every level;
                    parentID++;
                }
                // Clear vector to be ready for next line
                children.clear();
            }
            catch (const std::exception&)
            {
                // If error state happens close reader
                reader.close();
                // Clear tree (memory have to be deleted)
                this->clear(root);
                throw std::invalid_argument("Invalid value in tree!");
            } 
        }
    }
    // Close reader after all is done
    reader.close();
}

bool Tree::fileExists(std::string& fileName)
{
    std::ifstream reader(fileName);
    // If file can be opened
    if (!reader.fail())
    {
        std::cout << "There exsits file with this name! Do you want to save anyway? (Y/N)\n";
        std::string answ;
        while (true)
        {
            std::getline(std::cin, answ);
            if (answ == "Y")
            {
                // If yes -> re-write over file
                reader.close();
                break;
            }
            else if (answ == "N")
            {
                // If no -> does not save changes
                reader.close();
                return true;
            }
            else std::cout << "Enter Y/N\n";
        }
    }
    return false;
}

void Tree::saveToFile(std::string& fileName)
{
    // Check if file exists
    if (fileExists(fileName)) throw std::invalid_argument("Changes will not be saved!");

    if (!root) throw std::invalid_argument("Empty tree!");
    
    std::ofstream writer(fileName, std::ios::trunc); // Open file to write
    std::queue<Node*> nodes; // Queue is used for getting nodes by levels
    nodes.push(root); // At start add only root
    Node* tempNode = nullptr; // Shows current node
    size_t size; // Count of children of current node
    while (!nodes.empty())
    {
        // While we had nodes in queue take thier count
        int nodeCount = nodes.size();
        // Every row starts with '|'
        writer << "| ";
        while (nodeCount > 0)
        {
            // We want to take nodes by levels
            // This counter (nodeCount) shows count of nodes on single level
            // So second while loop go only by nodes on same level
            tempNode = nodes.front(); // Takes current node
            // Temp node can be nullptr because it represents different brotherhoods
            // There is no pointer or parentID (int) in every node so delimiter is nullptr
            // (It is my solution to this problem)
            if (!tempNode)
            {
                nodes.pop(); // Remove artificial node
                --nodeCount; // Decrease counter
                if(nodeCount>0) writer << "| "; // If node is not last add delimeter
            }
            else
            {
                // If node is "real"
                writer << tempNode->value << " "; // Write value to file
                nodes.pop(); // Remove for queue
                --nodeCount; // Decrease counter
                size = tempNode->children.size();
                for (size_t i = 0; i < size; ++i)
                {
                    // For every "real" node => add it to queue
                    if (tempNode->children.at(i))
                    {
                        nodes.push(tempNode->children.at(i));
                    }
                }
                // Add "delimeter" node
                nodes.push(nullptr);
            }
           
        }
        // And end with '|' and new line
        writer << "|";
        writer << std::endl;
    }
    // Delete used pointer
    tempNode = nullptr;
    delete tempNode;
}

bool Tree::contains(const Tree& other)
{
    return findRoot(root, other.root);
}

bool Tree::findRoot(Node*& node, Node* other)
{
    if (!other) return true; // empty second tree
    if (!node) return false; // empty first tree
    // Subtree's root can be any of main tree nodes
    // So there are two possible scenarios
    bool res = false;
    // If roots are same try first with them;
    if (node->value == other->value)
    {
        res = isSubtree(node, other);
        if (res) return true;
    }
    // Else go direct with children
    size_t sz = node->children.size();
    for (size_t i = 0; i < sz; ++i)
    {
        if (other->value == node->children.at(i)->value)
        {
            res = isSubtree(node->children.at(i), other);
            // If there is a match => return true
            if (res) return true;
        }
    }
    // By default return false
    return false;
}

bool Tree::isSubtree(Node*& node, Node* other)
{
    // This functions assumes that node value and other value are same
    // This is checked in findRoot function
    size_t org = node->children.size();
    size_t oth = other->children.size();
    bool found = false; // Show if child of our "possible sub-tree" has "twin" in main tree
    bool res = false;
    // For every children of other tree (our possible sub-tree)
    // Try to find "twin" in the main tree
    for (size_t j = 0; j < oth; ++j)
    {
        res = false;
        found = false;
        for (size_t i = 0; i < org; ++i)
        {
            if (node->children.at(i)->value == other->children.at(j)->value)
            {
                res = isSubtree(node->children.at(i), other->children.at(j));
                found = true;
                if (res) break;
            }
        }
        // If "twin" is not found => we have node is other tree that is not contained in main
        // Or if we have "twins" for all nodes but the "structure" is not same => false
        if (!found || (found && !res)) return false;
    }
    // If we end to leaf (for loop will not go) just return if two values are same
    return node->value == other->value;
}


bool Tree::remove(const Tree& other)
{
    if (!contains(other))
    {
        std::cout << "First tree does not contains second!\n";
        return false;
    }
    postOrder(root, other,-1);
    return true;
}


void Tree::postOrder(Node*& node, const Tree& other,int parentID)
{
    if (!node || !other.root) return;
    size_t sz = node->children.size();
    // Start from bottom
    for (size_t i = 0; i < sz; ++i)
    {
        postOrder(node->children.at(i), other, node->uniqueID);
    }
    bool res = findRoot(node, other.root);
    if (res)
    {
        //remove the subtree
        int sumOfSubtree = 0;
        findSum(node,other.root,sumOfSubtree);
        clear(node);
        std::cout << parentID << std::endl;
        if(parentID!=-1) insert(parentID, sumOfSubtree);
        //node = new Node(sumOfSubtree, uniqueID++);
        std::cout << "Tree successfully removed!\n";
    }
}

void Tree::findSum(Node*& node, Node* other, int& sum)
{
    if (other->children.size()== 0)
    {
        findSumLeftSubtree(node, sum);
        sum -= node->value;
        return;
    }
    size_t sizeOfNode = node->children.size();
    size_t sizeOfSubtree = other->children.size();
    bool found = false;
    int childIndex = -1;
    std::vector<int> uniqueID;
    for (size_t i = 0; i < sizeOfNode; ++i)
    {
        found = false;
        for (size_t j = 0; j < sizeOfSubtree; ++j)
        {
            if (node->children.at(i)->value == other->children.at(j)->value && isUnique(uniqueID, node->children.at(i)->uniqueID))
            {
                uniqueID.push_back(node->children.at(i)->uniqueID);
                childIndex = j;
                found = true;
                break;
            }
        }
        if (!found)
        {
            findSumLeftSubtree(node, sum);
            sum -= node->value;
        }
        else
        {
            findSum(node->children.at(i), other->children.at(childIndex), sum);
        }
    }
    
}

bool Tree::isUnique(std::vector<int>& ID, int newId)
{
    size_t size = ID.size();
    for (size_t i = 0; i < size; ++i)
    {
        if (ID.at(i) == newId) return false;
    }
    return true;
}

void Tree::findSumLeftSubtree(Node*& node,int&sum)
{
    if (!node) return;
    size_t childrenCount = node->children.size();
    sum += node->value;
    for (size_t i = 0; i < childrenCount; ++i)
    {
        findSumLeftSubtree(node->children.at(i), sum);
    }
}
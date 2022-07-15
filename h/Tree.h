#ifndef TREE_H
#define TREE_H

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <queue>
#include <stack>

class Tree
{
private:
    struct Node
    {
        int value;
        std::vector<Node*> children;
        int uniqueID;

        Node(int value,int uniqueID)
        {
            this->value = value;
            this->uniqueID = uniqueID;
        }
    };
    // Root of the tree i.e. first node
    Node* root;
    // Unique number for every node
    int uniqueID;

    // Help fucntion for destructor
    void clear(Node*& node);
    // Insert element where uniqueId - parentId, newId - childId
    bool addToTree(Node*& current, int uniqueId, int value, int currentLevel, int level, int newId);
    // Help fucntion for copy constructor/operator
    void copy(Node*& current, Node* org);

    // Returns line of string to vector with int values
    void getChildren(std::string& input, size_t startIndex, std::vector<int>& children);

    // Check if there is file with same name (for "SAVE" option)
    bool fileExists(std::string& fileName);

    // Check if tree is subtree => isSubtree()
    bool findRoot(Node*& node, Node* other);
    // Compare children
    bool isSubtree(Node*& node,Node* other);


    //void postOrder(Node*& node,const Tree& other);
public:

    Tree();
    ~Tree();
    Tree(std::string& fileName);
    Tree(const Tree& other);
    Tree& operator=(const Tree& other);

    void insert(int uniqueId, int value, int level);
   
    void readFromFile(std::string& fileName);
    void saveToFile(std::string& fileName);

    void print() const;

    bool contains(const Tree& other);

   // bool remove(const Tree& other);
};


#endif
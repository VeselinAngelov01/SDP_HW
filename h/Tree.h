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
    bool addToTree(Node*& current, int uniqueId, int value, int newId);
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

    // Returns sum of all left nodes
    void findSum(Node*& node,Node* other,int& sum);

    void postOrder(Node*& node,const Tree& other,int parentID);

    void findSumLeftSubtree(Node*& node,int& sum);
    bool isUnique(std::vector<int>& ID, int newId);
public:

    Tree();
    ~Tree();
    Tree(std::string& fileName);
    Tree(const Tree& other);
    Tree& operator=(const Tree& other);

    void insert(int uniqueId, int value);
   
    void readFromFile(std::string& fileName);
    void saveToFile(std::string& fileName);

    void print() const;

    bool contains(const Tree& other);

    bool remove(const Tree& other);
};


#endif
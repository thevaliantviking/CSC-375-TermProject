#include "TeamTrees.h"
#include <iostream>

//constructor sets tree to empty
TeamTrees::TeamTrees() : root(nullptr) {}

//destructor, prevents memory leaks
TeamTrees::~TeamTrees()
{
    clear();
}

//function that deletes nodes
void TeamTrees::clearHelper(Node*& node)
{
    if (node == nullptr)
        return;

    clearHelper(node->left);
    clearHelper(node->right);

    delete node;
    node = nullptr;
}

//recursive deletion at root
void TeamTrees::clear()
{
    clearHelper(root);
}

void TeamTrees::insertHelper(Node*& node, const Teams& team)
{
    if (node == nullptr)
    {
        node = new Node(team); //creates new node
    }
    else if (team.getName() == node->data.getName())
    {
        std::cout << "Error: Team '" << team.getName() << "' already exists.\n";
        return;
    }

    if (team.getName() < node->team.getName()) //alphabetical ordering
        insertHelper(node->left, team);
    else
        insertHelper(node->right, team);
}

void TeamTrees::insert(const Teams& team) //public function starts insertion from root
{
    insertHelper(root, team);
}

Teams* TeamTrees::searchHelper(Node* node, const std::string& name)
{
    if (node == nullptr) //if empty, not found
        return nullptr;

    if (name == node->team.getName()) //if match, return pointer to team object
        return &node->team;

    if (name < node->team.getName()) //logorithmic search
        return searchHelper(node->left, name);
    else
        return searchHelper(node->right, name);
}

Teams* TeamTrees::search(const std::string& name) //public function
{
    return searchHelper(root, name);
}

void TeamTrees::displayHelper(Node* node) const //prints tree in sorted order
{
    if (node == nullptr)
        return;

    displayHelper(node->left);

    node->team.displayTeamStats();

    displayHelper(node->right); //prints teams alphabetically
}

void TeamTrees::displayAll() const //starts traversal at root
{
    displayHelper(root);
}

void TeamTrees::fillArrayHelper(Node* node, Teams* arr[], int& count)
{
    if (node == nullptr)
        return;

    fillArrayHelper(node->left, arr, count);
    arr[count++] = &node->team; //collects pointers to teams in sorted order (alphabetical)
    fillArrayHelper(node->right, arr, count);
}

void TeamTrees::fillArray(Teams* arr[], int& count) //initializes then fills array
{
    count = 0;
    fillArrayHelper(root, arr, count);
}

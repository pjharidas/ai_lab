#include <iostream>
#include <vector>
#include <string>
#include <stack>

using namespace std;

// Define a structure to represent blocks
struct Block {
    string name;
    string on;  // The block on top of this block (empty string for blocks on the table)
};

// Function to print the current state of the world
void printWorldState(const vector<Block>& blocks) {
    cout << "Current world state:" << endl;
    for (const Block& block : blocks) {
        if (block.on.empty()) {
            cout << block.name << " is on the table." << endl;
        } else {
            cout << block.name << " is on top of " << block.on << "." << endl;
        }
    }
    cout << "------------------------" << endl;
}

// Function to place a block on the table
void placeBlockOnTable(vector<Block>& blocks, const string& blockName) {
    for (Block& block : blocks) {
        if (block.name == blockName) {
            block.on = ""; // Place the block on the table
            break;
        }
    }
    printWorldState(blocks);
}

// Function to move a block from the table onto another block
void moveBlockOntoAnotherBlock(vector<Block>& blocks, const string& blockName, const string& targetBlockName) {
    for (Block& block : blocks) {
        if (block.name == blockName) {
            block.on = targetBlockName;
            break;
        }
    }
    printWorldState(blocks);
}

int main() {
    // Initialize the initial state with two boxes stacked on top of each other
    vector<Block> blocks = {
        {"A", "B"}, // Block A is on top of Block B
        {"B", ""},  // Block B is on the table
        {"C", ""},  // Block C is on the table
    };

    // Create separate stacks for placing blocks on the table and moving blocks onto other blocks
    stack<string> placeOnTableStack;
    stack<string> moveOntoAnotherBlockStack;

    // Print the initial state
    cout << "Initial state:" << endl;
    printWorldState(blocks);

    // Place block B on the table
    placeOnTableStack.push("B");
    placeBlockOnTable(blocks, "B");

    // Move block A onto block C
    moveOntoAnotherBlockStack.push("A");
    moveOntoAnotherBlockStack.push("C");
    moveBlockOntoAnotherBlock(blocks, "A", "C");

    // Final state
    cout << "Problem solved!" << endl;

    // Print the action stacks
    cout << "Place on table stack:" << endl;
    while (!placeOnTableStack.empty()) {
        cout << placeOnTableStack.top() << " -> Table" << endl;
        placeOnTableStack.pop();
    }

    cout << "Move onto another block stack:" << endl;
    while (!moveOntoAnotherBlockStack.empty()) {
        string movingBlock = moveOntoAnotherBlockStack.top();
        moveOntoAnotherBlockStack.pop();
        string targetBlock = moveOntoAnotherBlockStack.top();
        moveOntoAnotherBlockStack.pop();
        cout << movingBlock << " -> " << targetBlock << endl;
    }

    return 0;
}

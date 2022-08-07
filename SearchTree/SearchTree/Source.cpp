//============================================================================
// Name        : SearchTree.cpp
// Author      : Michael Tricoli
// Version     : 1.0
// Course  	   : CS-499
// Date 	   : 07/24/22
//============================================================================

#include <iostream> //Library for Input/Output
#include <time.h> //Library for datetime
#include <algorithm> //Library for handling algorithms like the search tree
#include "CSVparser.hpp" // CSV Parser to import CSV file into C++ code 

using namespace std;

//============================================================================
// Global definitions visible to all methods and classes
//============================================================================

// forward declarations
double strToDouble(string str, char ch);

// define a structure to hold bid information
//this is each column used from csv data 
struct Bid {
    string bidId; // unique identifier
    string title;
    string fund;
    double amount;
    Bid() {
        amount = 0.0;
    }
};

//Internal structure for tree node
struct Node {
    Bid bid;
    Node* left;
    Node* right;

    //default constructor *always intitialize*
    Node() {
        left = nullptr;
        right = nullptr;

    }

    //initialize with a given bid
    Node(Bid aBid) : Node() {
        this->bid = aBid;
    }

};

//============================================================================
// Binary Search Tree class definition
//============================================================================

/**
 * Define a class containing data members and methods to
 * implement a binary search tree
 */
class BinarySearchTree {

private:
    Node* root;

    void addNode(Node* node, Bid bid);
    void inOrder(Node* node);
    Node* removeNode(Node* node, string bidId);

public:
    BinarySearchTree();
    virtual ~BinarySearchTree();
    void InOrder();
    void Insert(Bid bid);
    void Remove(string bidId);
    Bid Search(string bidId);
};

/**
 * Default constructor
 */
BinarySearchTree::BinarySearchTree() {
    // initialize housekeeping variables
    root = nullptr;
}

/**
 * Destructor
 */
BinarySearchTree::~BinarySearchTree() {
    // recurse from root deleting every node
}

/**
 * Traverse the tree in order
 */
void BinarySearchTree::InOrder() {
    this->inOrder(root);
}
/**
 * Insert a bid
 */
void BinarySearchTree::Insert(Bid bid) {
    //Implement inserting a bid into the tree
    if (root == nullptr) {
        root = new Node(bid);
    }
    else {
        this->addNode(root, bid);
    }

}

/**
 * Remove a bid
 */
void BinarySearchTree::Remove(string bidId) {
    //Implement removing a bid from the tree
    this->removeNode(root, bidId);

}

/**
 * Search for a bid
 */
Bid BinarySearchTree::Search(string bidId) {
    //Implement searching the tree for a bid

    //start searching from the root
    Node* current = root;

    //keep looping downwards until bottom reached of bid is found
    while (current != nullptr) {
        //if current node matches, return it
        if (current->bid.bidId.compare(bidId) == 0) {
            return current->bid;
        }
        //if bid is smaller than current then traverse left
        if (bidId.compare(current->bid.bidId) < 0) {
            current = current->left;
        }
        else {
            current = current->right;
        }
    }

    Bid bid;
    return bid;
}

/**
 * Add a bid to some node (recursive)
 *
 * @param node Current node in tree
 * @param bid Bid to be added
 */
void BinarySearchTree::addNode(Node* node, Bid bid) {
    //Implement inserting a bid into the tree

    //if node is larger than the bid, add to left subtree
    if (node->bid.bidId.compare(bid.bidId) > 0) {
        if (node->left == nullptr) {
            node->left = new Node(bid);
        }
        else {
            this->addNode(node->left, bid);
        }
    }
    //add to right subtree
    else {
        if (node->right == nullptr) {
            node->right = new Node(bid);
        }
        else {
            this->addNode(node->right, bid);
        }

    }
}
void BinarySearchTree::inOrder(Node* node) {
    if (node != nullptr) {
        inOrder(node->left);
        cout << node->bid.bidId << ": " << node->bid.title << " | " << node->bid.amount << " | "
            << node->bid.fund << endl;
        inOrder(node->right);
    }
}

Node* BinarySearchTree::removeNode(Node* node, string bidId) {
    //if this node is null then return (avoid crashing)
    if (node == nullptr) {
        return node;
    }

    //recurse down the left subtree
    if (bidId.compare(node->bid.bidId) < 0) {
        node->left = removeNode(node->left, bidId);
    }
    else if (bidId.compare(node->bid.bidId) > 0) {
        node->right = removeNode(node->right, bidId);
    }
    else {
        // no children so this is a leaf node
        if (node->left == nullptr && node->right == nullptr) {
            delete node;
            node = nullptr;
        }
        //one child to left
        else if (node->left != nullptr && node->right == nullptr) {
            Node* temp = node;
            node = node->left;
            delete temp;
        }
        //one child to right
        else if (node->right != nullptr && node->left == nullptr) {
            Node* temp = node;
            node = node->right;
            delete temp;
        }
        //two children
        else {
            Node* temp = node->right;
            while (temp->left != nullptr) {
                temp = temp->left;
            }
            node->bid = temp->bid;
            node->right = removeNode(node->right, temp->bid.bidId);
        }
    }
    return node;
}
//============================================================================
// Static methods used for testing
//============================================================================

/**
 * Display the bid information to the console (std::out)
 *
 * @param bid struct containing the bid info
 */
void displayBid(Bid bid) {
    cout << bid.bidId << ": " << bid.title << " | " << bid.amount << " | "
        << bid.fund << endl;
    return;
}

/**
 * Load a CSV file containing bids into a container
 *
 * @param csvPath the path to the CSV file to load
 * @return a container holding all the bids read
 */
void loadBids(string csvPath, BinarySearchTree* bst) {
    cout << "Loading CSV file " << csvPath << endl;

    // initialize the CSV Parser using the given path
    csv::Parser file = csv::Parser(csvPath);

    // read and display header row - optional
    vector<string> header = file.getHeader();
    for (auto const& c : header) {
        cout << c << " | ";
    }
    cout << "" << endl;

    try {
        // loop to read rows of a CSV file
        for (unsigned int i = 0; i < file.rowCount(); i++) {

            // Create a data structure and add to the collection of bids
            Bid bid;
            bid.bidId = file[i][1];
            bid.title = file[i][0];
            bid.fund = file[i][8];
            bid.amount = strToDouble(file[i][4], '$');

            //cout << "Item: " << bid.title << ", Fund: " << bid.fund << ", Amount: " << bid.amount << endl;

            // push this bid to the end
            bst->Insert(bid);
        }
    }
    catch (csv::Error& e) {
        std::cerr << e.what() << std::endl;
    }
}

/**
 * Simple C function to convert a string to a double
 * after stripping out unwanted char
 *
 * credit: http://stackoverflow.com/a/24875936
 *
 * @param ch The character to strip out
 */
double strToDouble(string str, char ch) {
    str.erase(remove(str.begin(), str.end(), ch), str.end());
    return atof(str.c_str());
}

/**
 * The one and only main() method
 */
int main(int argc, char* argv[]) {

    // process command line arguments
    string csvPath, bidKey;
    switch (argc) {

    case 2:
        csvPath = argv[1];
        break;
    case 3:
        csvPath = argv[1];
        break;
    default:
        csvPath = "testdata.csv";//change this to name of csv file you need to work from

    }

    // Define a timer variable
    clock_t ticks;

    // Define a binary search tree to hold all bids
    BinarySearchTree* bst{};

    Bid bid;

    int choice = 0;

    //start menu display loop will keep repeating menu until exited, prints lines to console
    while (choice != 9) {
        cout << "Menu:" << endl;
        cout << "  1. Load Bids" << endl;
        cout << "  2. Display All Bids" << endl;
        cout << "  3. Find Bid" << endl;
        cout << "  4. Remove Bid" << endl;
        cout << "  5. Add Bid" << endl;
        cout << "  9. Exit" << endl;
        cout << "Enter choice: ";
        cin >> choice;

        // add protection from invalid input crashes by waiting for program responses and breaking any endless loop
        if (std::cin.fail())
        {
            std::cout << "Please enter valid number only!\n";
            std::cin.clear();
            std::cin.ignore(10000, '\n');
        }

        //else to run code if valid input is detected
        else {

            //switch to decide which option is selected
            switch (choice) {

            case 1:

                //create new search tree
                bst = new BinarySearchTree();

                // Initialize a timer variable before loading bids
                ticks = clock();

                // Complete the method call to load the bids
                loadBids(csvPath, bst);

                // Calculate elapsed time and display result
                ticks = clock() - ticks;
                cout << "time: " << ticks << " clock ticks" << endl;
                cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;
                break;

            case 2:

                //If statement to check if tree exsist 
                if (bst != NULL) {

                    //display all bids using InOrder function
                    bst->InOrder();

                    //end case and go back to loop
                    break;
                }
                //if tree are null print message to prevent crashing
                else {
                    cout << "No bids to display";

                    //break case and go back to loop
                    break;
                }
            case 3:

                //take input on bidKey to search
                cout << "Enter Bid ID you wish to search \n";
                cin >> bidKey;

                //use search function to search through tree
                bid = bst->Search(bidKey);


                //check to see if the bidid is valid and then display the result
                if (!bid.bidId.empty()) {
                    displayBid(bid);
                }

                //if ID could not be found print message with entered bidKey
                else {
                    cout << "Bid Id " << bidKey << " not found." << endl;
                }
                break;

            case 4:
                //prompt for bidkey you want to remove
                cout << "Enter Bid ID you want to remove \n";
                cin >> bidKey;

                //if statement to check if bst exsists and removes provided bidKey
                if (bst != NULL) {
                    bst->Remove(bidKey);
                    break;
                }

                //if bidKey does not exsist prints an error message 
                else {
                    cout << "Wrong Bid ID or no bids are Loaded try again\n";
                    break;
                }

            case 5:
                //create local variables for the bid information
                string bidId;
                string title;
                string fund;
                double amount;


                //gets input for each variable
                cout << "Enter BidID\n";
                cin >> bidId;
                cout << "Enter Title\n";
                cin >> title;
                cout << "Enter fund\n";
                cin >> fund;
                cout << "Enter Amount, as number only\n";
                cin >> amount;

                //Assign variables into bid format
                Bid bid;
                bid.bidId = bidId;
                bid.title = title;
                bid.fund = fund;
                bid.amount = amount;


                //check to make sure bidtree has been created before adding
                if (bst != NULL) {
                    bst->Insert(bid);
                    break;
                }
                //if add fails provide message to user
                else {

                    cout << "Error in adding Item please try again, follow instructions";
                    break;
                }
                
            }


        }
    }
    //final print line when program is exited
    cout << "End Session." << endl;

    return 0;
}

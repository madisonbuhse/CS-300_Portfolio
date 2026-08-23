// ProjectTwo.cpp
// ABCU Advising Course Planner
// Author: Madison Buhse
// Version: 1.0
// Date: 08/10/2026

#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <string>
#include <vector>
#include <set>
#include <algorithm>
#include <list>
#include <stdexcept>

using namespace std;

//====================
// Global definitions
//====================

// define a structure to hold course information
struct Course {
    string courseNumber;
    string courseTitle;
    vector<string> prerequisites;
    Course() {
        prerequisites.clear();
    }
};

// internal structure for tree node
struct Node {
    Course course;
    Node* left;
    Node* right;

    // default constructor
    Node() {
        left = nullptr;
        right = nullptr;
    }

    // initializing constructor with a course
    Node(Course aCourse) : Node() {
        course = aCourse;
    }
};

//=====================================
// Binary Search Tree class definition
//=====================================

// define a class containing data members and methods to 
// implement a binary search tree
class BinarySearchTree {

private:
    Node* root;

    void addNode(Node* node, Course course);
    void inOrder(Node* node);
    Course searchCourse(Node* node, string courseNumber);
    void destroyTree(Node* node);

public:
    BinarySearchTree();
    virtual ~BinarySearchTree();
    void InOrder();
    void Insert(Course course);
    Course Search(string courseNumber);
	Course CreateCourseObject(string num, string title, vector<string> prerequisites);
    bool loadCourseData(string fileName);
    void PrintAlphanumeric();

};

// Default constructor
BinarySearchTree::BinarySearchTree() {
	// initialize root to nullptr
    root = nullptr;
}

// Destructor
BinarySearchTree::~BinarySearchTree() {
    // Call destroyTree to free memory
    destroyTree(root);
    root = nullptr;
}

// In-order traversal of the tree
void BinarySearchTree::InOrder() {
    inOrder(root);
}

// Insert a course into the tree
void BinarySearchTree::Insert(Course course) {
    // if root is null, root becomes the new node
    if (root == nullptr) {
        root = new Node(course);
    }

    // else, call the private addNode method to insert the course into the tree
    else {
        addNode(root, course);
	}
}

// Search for a course in the tree by passing course number to searchCourse
Course BinarySearchTree::Search(string courseNumber) {
    if (root == nullptr) {
        return Course();
    }

    return searchCourse(root, courseNumber);
}

// Logic for finding a specific course by course number
Course BinarySearchTree::searchCourse(Node* node, string courseNumber) {
    // return if node is not found
    if (node == nullptr) {
        return Course();
    }

    // if current node's course number matches, return the course
    if (node->course.courseNumber == courseNumber) {
        return node->course;
    }

    // if current node's course number is greater than courseNumber, go left
    if (node->course.courseNumber > courseNumber) {
        return searchCourse(node->left, courseNumber);
    }

    // if current node's course number is less than courseNumber, go right
    return searchCourse(node->right, courseNumber);
}

// Recursively add a course to some node in the tree
void BinarySearchTree::addNode(Node* node, Course course) {
    // if the course number is less than the current node's course number, go left
    if (course.courseNumber < node->course.courseNumber) {
        // if left child is null, insert the new course node here
        if (node->left == nullptr) {
            node->left = new Node(course);
        }

        // else, recursively call addNode on the left child
        else {
            addNode(node->left, course);
        }
    }

    // else, the course number is greater than or equal to the current node's
    // course number, go right
    else {
        // if right child is null, insert the new course node here
        if (node->right == nullptr) {
            node->right = new Node(course);
        }

        // else, recursively call addNode on the right child
        else {
            addNode(node->right, course);
        }
    }
}

// Destroy the tree
void BinarySearchTree::destroyTree(Node* node) {
    if (node == nullptr) {
        return;
    }

    destroyTree(node->left);
    destroyTree(node->right);
    delete node;
}

// Create a Course object from the given course number, title, and prerequisites
Course BinarySearchTree::CreateCourseObject(string num, string title, vector<string> prerequisites) {
    Course c;
    c.courseNumber = num;
    c.courseTitle = title;
    c.prerequisites = prerequisites;
    return c;
}

// Print the tree in alphanumeric order
void BinarySearchTree::PrintAlphanumeric() {
    // if no courses have been added, print a message and return
    if (root == nullptr) {
        cout << "The course catalog is empty. Please load data." << endl;
        return;
    }

    // call the inOrder method to print the courses in order
    inOrder(root);
}

// In-order traversal of the tree starting from a given node
void BinarySearchTree::inOrder(Node* node) {
    // if node is not null, recursively traverse left, print the course information,
    // then recursively traverse right
    if (node != nullptr) {
        inOrder(node->left);
        cout << node->course.courseNumber << ", " << node->course.courseTitle << endl;
        inOrder(node->right);
    }
}

// Helper function to trim leading and trailing whitespace
string trim(const string& str) {
    size_t first = str.find_first_not_of(" \t\r\n");
    if (first == string::npos) {
        return "";
    }
    size_t last = str.find_last_not_of(" \t\r\n");
    return str.substr(first, (last - first + 1));
}

// Helper function to split a string by comma delimiter
vector<string> split(const string& s, char delimiter) {
    vector<string> tokens;
    string token;
    stringstream ss(s);

    while (getline(ss, token, delimiter)) {
        tokens.push_back(trim(token));
    }
    return tokens;
}

// Loading the course data
bool BinarySearchTree::loadCourseData(string filePath) {
    // Open file stream
    ifstream inputFile(filePath);

    // If file is not open, send error message
    if (!inputFile.is_open()) {
        cout << "Error: Could not open file: " << filePath << endl;
        return false;
    }

    set<string> validCourseNumbers;
    string currentLine;

    // read, trim, and validate lines of file for first time
    while (getline(inputFile, currentLine)) {
        currentLine = trim(currentLine);

        if (currentLine.empty()) {
            continue;
        }

        vector<string> tokens = split(currentLine, ',');

        if (tokens.size() < 2 || tokens[0].empty() || tokens[1].empty()) {
            cout << "Error: Invalid line format." << endl;
            inputFile.close();
            return false;
        }

        string courseNumber = tokens[0];

        transform(courseNumber.begin(), courseNumber.end(), courseNumber.begin(), ::toupper);

        validCourseNumbers.insert(courseNumber);
    }

    inputFile.close();

    // validate file again
    inputFile.open(filePath);
    if (!inputFile.is_open()) {
        cout << "Error: Could not reopen file." << endl;
        return false;
    }

    while (getline(inputFile, currentLine)) {
        currentLine = trim(currentLine);

        if (currentLine.empty()) {
            continue;
        }

        vector<string> tokens = split(currentLine, ',');
        string courseNum = tokens[0];

        transform(courseNum.begin(), courseNum.end(), courseNum.begin(), ::toupper);

        string courseTitle = tokens[1];
        vector<string> prerequisites;

        for (size_t i = 2; i < tokens.size(); ++i) {
            string currentPrereq = trim(tokens[i]);

            transform(currentPrereq.begin(), currentPrereq.end(), currentPrereq.begin(), ::toupper);

            if (currentPrereq.empty()) {
                continue;
            }

            if (validCourseNumbers.find(currentPrereq) == validCourseNumbers.end()) {
                cout << "Error: Prerequisite " << currentPrereq << " does not exist." << endl;
                inputFile.close();
                return false;
            }

            prerequisites.push_back(currentPrereq);
        }

        // create new tree with validated csv file information
        Course newCourse = CreateCourseObject(courseNum, courseTitle, prerequisites);
        Insert(newCourse);
    }

    inputFile.close();
    cout << "Course data successfully validated and loaded into tree structure!" << endl;
    return true;
}

//=============
// Main Method
//=============
int main(int argc, char* argv[]) {
    // initialize and assign variables and csv path
    string csvPath = "../CS 300 ABCU_Advising_Program_Input.csv";
    Course course;
    string courseNumber;
    BinarySearchTree* tree =  new BinarySearchTree();
    bool dataLoaded = false;

    cout << "Welcome to the ABCU course planner!" << endl;

    int choice = 0;
    while (choice != 9) {
        cout << "  1. Load Data Structure." << endl;
        cout << "  2. Print Course List." << endl;
        cout << "  3. Print Course." << endl;
        cout << "  9. Exit" << endl;
        cout << endl;
        cout << "What would you like to do? ";

        cin >> choice;

        switch (choice) {

        case 1:
            if (tree->loadCourseData(csvPath) == true) {
                dataLoaded = true;
            }

            break;

        case 2:
            if (!dataLoaded) {
                cout << "Please load the course data first." << endl;
            }

            else {
                cout << endl;
                cout << "Here is a sample schedule: " << endl;
                cout << endl;

                tree->PrintAlphanumeric();

                cout << endl;
            }

            break;

        case 3:
            if (!dataLoaded) {
                cout << "Please load the course data first." << endl;

                break;
            }

            cout << "What course do you want to know about? ";
            cin >> courseNumber;

            transform(courseNumber.begin(), courseNumber.end(), courseNumber.begin(), ::toupper);

            course = tree->Search(courseNumber);

            if (course.courseNumber.empty()) {
                cout << "Course " << courseNumber << " not found." << endl;
            }

            else {
                cout << course.courseNumber << ", " << course.courseTitle << endl;

                if (course.prerequisites.empty()) {
                    cout << "Prerequisites: None" << endl;
                }

                else {
                    cout << "Prerequisites: ";

                    for (size_t i = 0; i < course.prerequisites.size(); ++i) {
                        cout << course.prerequisites[i];

                        if (i < course.prerequisites.size() - 1) {
                            cout << ", ";
                        }
                    }

                    cout << endl;
                }
            }

            break;

        case 9:
            cout << "Thank you for using the course planner!" << endl;

            break;

        default:
            cout << choice << " is not a valid option. Please enter 1, 2, 3, or 9." << endl;

            break;
        }
    }

    // delete tree on termination of program
    delete tree;
    tree = nullptr;

    return 0;
}
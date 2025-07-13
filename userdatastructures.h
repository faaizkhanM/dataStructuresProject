#ifndef USERDATASTRUCTURES_H
#define USERDATASTRUCTURES_H

#include "user.h"
#include "customvector.h"
#include "customlist.h"
#include <string>
#include <functional>

// Node for Binary Search Tree
template<typename T>
class BSTNode {
public:
    T data;
    BSTNode* left;
    BSTNode* right;
    
    BSTNode(const T& value) : data(value), left(nullptr), right(nullptr) {}
};

// Binary Search Tree for sorting users by different criteria
template<typename T, typename Compare = std::function<bool(const T&, const T&)>>
class UserBST {
private:
    struct Node {
        T data;
        Node* left;
        Node* right;
        Node(const T& value) : data(value), left(nullptr), right(nullptr) {}
    };
    Node* root;
    Compare compare;

    // Helper function for insertion
    Node* insertHelper(Node* node, const T& value) {
        if (node == nullptr) {
            return new Node(value);
        }

        if (compare(value, node->data)) {
            node->left = insertHelper(node->left, value);
        } else {
            node->right = insertHelper(node->right, value);
        }

        return node;
    }

    // Helper function for inorder traversal
    void inorderHelper(Node* node, CustomVector<T>& result) const {
        if (node == nullptr) return;

        inorderHelper(node->left, result);
        result.push_back(node->data);
        inorderHelper(node->right, result);
    }

    // Helper function for cleanup
    void cleanupHelper(Node* node) {
        if (node == nullptr) return;

        cleanupHelper(node->left);
        cleanupHelper(node->right);
        delete node;
    }

public:
    UserBST() : root(nullptr), compare([](const T& a, const T& b) { return a < b; }) {}
    UserBST(const Compare& comp) : root(nullptr), compare(comp) {}

    ~UserBST() {
        cleanupHelper(root);
    }

    void insert(const T& value) {
        root = insertHelper(root, value);
    }

    CustomVector<T> inorder() const {
        CustomVector<T> result;
        inorderHelper(root, result);
        return result;
    }
};

// Hash Table for quick user lookups
class UserHashTable {
private:
    static const int TABLE_SIZE = 100;
    CustomVector<CustomList<User>> table;

    int hashFunction(const QString& username) const {
        return std::hash<QString>{}(username) % TABLE_SIZE;
    }

public:
    UserHashTable() : table(TABLE_SIZE) {}

    void insert(const User& user);
    User* find(const QString& username);
    bool remove(const QString& username);
};

// User Profile Manager class that uses our custom data structures
class UserProfileManager {
private:
    UserHashTable hashTable;
    UserBST<User, std::function<bool(const User&, const User&)>> nameBST;
    UserBST<User, std::function<bool(const User&, const User&)>> emailBST;

public:
    UserProfileManager() : 
        nameBST([](const User& a, const User& b) { return a.getFullName() < b.getFullName(); }),
        emailBST([](const User& a, const User& b) { return a.getEmail() < b.getEmail(); })
    {}

    void addUser(const User& user);
    User* findUser(const QString& username);
    bool removeUser(const QString& username);
    CustomVector<User> getUsersSortedByName() const;
    CustomVector<User> getUsersSortedByEmail() const;
};

#endif // USERDATASTRUCTURES_H 

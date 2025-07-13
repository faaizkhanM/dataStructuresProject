#include "userdatastructures.h"
#include <QDebug>

// Implementation of UserProfileManager methods
void UserProfileManager::addUser(const User& user) {
    hashTable.insert(user);
    nameBST.insert(user);
    emailBST.insert(user);
    
    qDebug() << "User added to profile manager:" << user.getUsername();
}

User* UserProfileManager::findUser(const QString& username) {
    return hashTable.find(username);
}

bool UserProfileManager::removeUser(const QString& username) {
    User* user = hashTable.find(username);
    if (user) {
        hashTable.remove(username);
        // Note: BST removal would need to be implemented if required
        return true;
    }
    return false;
}

CustomVector<User> UserProfileManager::getUsersSortedByName() const {
    return nameBST.inorder();
}

CustomVector<User> UserProfileManager::getUsersSortedByEmail() const {
    return emailBST.inorder();
}

// Implementation of UserHashTable methods
void UserHashTable::insert(const User& user) {
    int index = hashFunction(user.getUsername());
    table[index].push_back(user);
}

User* UserHashTable::find(const QString& username) {
    int index = hashFunction(username);
    for (auto& user : table[index]) {
        if (user.getUsername() == username) {
            return &user;
        }
    }
    return nullptr;
}

bool UserHashTable::remove(const QString& username) {
    int index = hashFunction(username);
    auto& bucket = table[index];
    
    // Find the user in the bucket
    for (auto it = bucket.begin(); it != bucket.end(); ++it) {
        if ((*it).getUsername() == username) {
            // Create a new list without the user
            CustomList<User> newList;
            for (auto& user : bucket) {
                if (user.getUsername() != username) {
                    newList.push_back(user);
                }
            }
            bucket = newList;
            return true;
        }
    }
    return false;
} 
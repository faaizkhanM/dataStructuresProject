#include <iostream>
#include <string>
#include <functional>

int main() {
    std::string username = "john_doe";
    std::hash<std::string> hash_fn;
    
    size_t hash_value = hash_fn(username);
    int bucket = hash_value % 100;
    
    std::cout << "Username: " << username << std::endl;
    std::cout << "Hash value: " << hash_value << std::endl;
    std::cout << "Bucket: " << bucket << std::endl;
    
    // Test with QString equivalent (if you have Qt)
    // QString qUsername = "john_doe";
    // std::hash<QString> qHash_fn;
    // size_t qHash_value = qHash_fn(qUsername);
    // int qBucket = qHash_value % 100;
    // std::cout << "QString Bucket: " << qBucket << std::endl;
    
    return 0;
} 
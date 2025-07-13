# Cab Booking System - Technical Documentation

## 1. Custom Data Structures Implementation

### 1.1 CustomVector
```cpp
template<typename T>
class CustomVector {
    // Dynamic array implementation with automatic resizing
    // Used for: Storing cabs in priority queue, user lists, and general data storage
    // Key Features:
    // - Dynamic resizing (doubles capacity when full)
    // - Random access (O(1))
    // - Push/pop operations (amortized O(1))
    // - Iterator support
}
```

### 1.2 CustomList
```cpp
template<typename T>
class CustomList {
    // Doubly-linked list implementation
    // Used for: Hash table collision resolution
    // Key Features:
    // - O(1) insertions and deletions
    // - Bidirectional traversal
    // - No random access
    // - Iterator support
}
```

### 1.3 Binary Search Tree (BST)
```cpp
template<typename T, typename Compare>
class UserBST {
    // Self-balancing binary search tree
    // Used for: Sorting users by name and email
    // Key Features:
    // - O(log n) search, insert, and delete
    // - In-order traversal for sorted output
    // - Custom comparison function support
}
```

### 1.4 Priority Queue
```cpp
class PriorityQueue {
    // Binary heap implementation
    // Used for: Managing cab priorities based on rating and distance
    // Key Features:
    // - O(log n) insert and remove
    // - O(1) peek at highest priority
    // - Priority based on cab rating and distance
}
```

### 1.5 Hash Table
```cpp
class UserHashTable {
    // Hash table with chaining for collision resolution
    // Used for: Quick user lookups by username
    // Key Features:
    // - O(1) average case lookup
    // - Chaining with CustomList for collision handling
    // - Fixed size (100 buckets)
}
```

## 2. Sorting Algorithms

### 2.1 Quick Sort
```cpp
template<typename T, typename Compare>
void quickSort(CustomVector<T>& arr, Compare comp) {
    // Used in: General sorting needs
    // Time Complexity: O(n log n) average case
    // Space Complexity: O(log n)
    // Advantages: Fast average case, in-place sorting
    // Disadvantages: O(n²) worst case
}
```

### 2.2 Merge Sort
```cpp
template<typename T, typename Compare>
void mergeSort(CustomVector<T>& arr, Compare comp) {
    // Used in: Stable sorting requirements
    // Time Complexity: O(n log n)
    // Space Complexity: O(n)
    // Advantages: Stable, consistent performance
    // Disadvantages: Extra space required
}
```

### 2.3 Heap Sort (via Priority Queue)
```cpp
// Implemented in PriorityQueue class
// Used for: Cab prioritization
// Time Complexity: O(n log n)
// Space Complexity: O(1)
// Advantages: In-place, guaranteed O(n log n)
// Disadvantages: Not stable
```

## 3. Important C++ Concepts Used

### 3.1 Templates
```cpp
// Used throughout the project for generic data structures
template<typename T>
class CustomVector {
    // Generic implementation for any data type
}
```

### 3.2 Smart Pointers
```cpp
// Used for memory management
std::unique_ptr<Cab> cab;
std::shared_ptr<User> user;
```

### 3.3 Lambda Functions
```cpp
// Used for custom comparisons
auto compare = [](const User& a, const User& b) {
    return a.getFullName() < b.getFullName();
};
```

### 3.4 Move Semantics
```cpp
// Used for efficient resource transfer
CustomVector<User>&& getUsers() {
    return std::move(users);
}
```

### 3.5 RAII (Resource Acquisition Is Initialization)
```cpp
// Used in all custom containers
class CustomVector {
    ~CustomVector() {
        delete[] data;  // Automatic cleanup
    }
};
```

## 4. Key Algorithms and Their Usage

### 4.1 Cab Priority Calculation
```cpp
float Cab::getPriority() const {
    // Combines rating and distance for cab prioritization
    return rating - (distance * 0.1);
}
```

### 4.2 User Search
```cpp
User* UserHashTable::find(const QString& username) {
    // O(1) average case user lookup
    int index = hashFunction(username);
    for (auto& user : table[index]) {
        if (user.getUsername() == username) {
            return &user;
        }
    }
    return nullptr;
}
```

### 4.3 BST In-order Traversal
```cpp
CustomVector<T> UserBST::inorder() const {
    // Returns sorted list of users
    CustomVector<T> result;
    inorderHelper(root, result);
    return result;
}
```

## 5. Important C++ Features to Know

### 5.1 Memory Management
- Smart pointers vs raw pointers
- RAII principles
- Move semantics
- Copy vs move constructors

### 5.2 STL Concepts
- Iterators
- Containers
- Algorithms
- Functors

### 5.3 Modern C++ Features
- Auto type deduction
- Range-based for loops
- Lambda expressions
- nullptr

### 5.4 Object-Oriented Concepts
- Inheritance
- Polymorphism
- Encapsulation
- Abstract classes

## 6. Project-Specific Implementation Details

### 6.1 Cab Management
- Priority queue for cab allocation
- Rating and distance-based prioritization
- Availability tracking

### 6.2 User Management
- Hash table for quick lookups
- BST for sorted views
- Profile management

### 6.3 Data Persistence
- File I/O operations
- Data serialization
- State management

## 7. Performance Considerations

### 7.1 Time Complexities
- Hash Table: O(1) average case
- BST: O(log n) operations
- Priority Queue: O(log n) operations
- Vector: O(1) random access

### 7.2 Space Complexities
- Hash Table: O(n)
- BST: O(n)
- Priority Queue: O(n)
- Vector: O(n)

### 7.3 Memory Management
- Custom allocators
- Resource cleanup
- Memory leaks prevention

## 8. Testing and Debugging

### 8.1 Unit Testing
- Data structure operations
- Algorithm correctness
- Edge cases

### 8.2 Performance Testing
- Load testing
- Stress testing
- Memory profiling

### 8.3 Debugging Techniques
- Breakpoints
- Memory inspection
- Performance profiling 
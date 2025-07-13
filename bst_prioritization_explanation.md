# BST Prioritization: How Users Are Placed (Root, Left, or Right)

## 🎯 The Key Decision Logic

The BST uses this simple rule in the `insertHelper` function:

```cpp
if (compare(value, node->data)) {
    node->left = insertHelper(node->left, value);  // Go LEFT
} else {
    node->right = insertHelper(node->right, value); // Go RIGHT
}
```

**Translation**: 
- If `compare(newUser, existingUser)` returns `true` → Place new user to the **LEFT**
- If `compare(newUser, existingUser)` returns `false` → Place new user to the **RIGHT**

## 📝 Comparison Functions in Your Project

### Name BST Comparison:
```cpp
[](const User& a, const User& b) { 
    return a.getFullName() < b.getFullName(); 
}
```

### Email BST Comparison:
```cpp
[](const User& a, const User& b) { 
    return a.getEmail() < b.getEmail(); 
}
```

## 🌳 Step-by-Step Insertion Process

### Example: Adding Users to Name BST

Let's add these users in order: `["John Doe", "Alice Wong", "Bob Brown", "Jane Smith", "Bella Rose"]`

#### Step 1: Insert "John Doe" (First User)
```
Tree is empty → "John Doe" becomes ROOT
     [John Doe]
    /          \
  NULL        NULL
```

#### Step 2: Insert "Alice Wong"
```
compare("Alice Wong", "John Doe")
= "Alice Wong" < "John Doe" 
= true → Go LEFT

     [John Doe]
    /          \
[Alice Wong]  NULL
```

#### Step 3: Insert "Bob Brown"
```
compare("Bob Brown", "John Doe")
= "Bob Brown" < "John Doe"
= false → Go RIGHT

     [John Doe]
    /          \
[Alice Wong] [Bob Brown]
```

#### Step 4: Insert "Jane Smith"
```
compare("Jane Smith", "John Doe")
= "Jane Smith" < "John Doe"
= false → Go RIGHT
compare("Jane Smith", "Bob Brown")
= "Jane Smith" < "Bob Brown"
= false → Go RIGHT

     [John Doe]
    /          \
[Alice Wong] [Bob Brown]
                    \
                [Jane Smith]
```

#### Step 5: Insert "Bella Rose"
```
compare("Bella Rose", "John Doe")
= "Bella Rose" < "John Doe"
= false → Go RIGHT
compare("Bella Rose", "Bob Brown")
= "Bella Rose" < "Bob Brown"
= true → Go LEFT

     [John Doe]
    /          \
[Alice Wong] [Bob Brown]
            /          \
        [Bella Rose] [Jane Smith]
```

## 🔍 Detailed Comparison Examples

### Name BST Logic:
```cpp
// String comparison: "Alice" < "Bob" < "Jane" < "John"
"Alice Wong" < "John Doe"   → true  (A < J alphabetically)
"Bob Brown" < "John Doe"    → true  (B < J alphabetically)  
"Jane Smith" < "John Doe"   → true  (J < J, but 'a' < 'o' in "Jane" vs "John")
"Bella Rose" < "Bob Brown"  → true  (B < B, but 'e' < 'o' in "Bella" vs "Bob")
```

### Email BST Logic:
```cpp
// String comparison: "aw@e.com" < "bb@e.com" < "js@e.com" < "jd@e.com"
"aw@e.com" < "jd@e.com"     → true  (a < j)
"bb@e.com" < "jd@e.com"     → true  (b < j)
"js@e.com" < "jd@e.com"     → false (j = j, but s > d)
"br@e.com" < "bb@e.com"     → false (b = b, but r > b)
```

## 🎨 Visual Decision Tree

```
When inserting new user:
                    [Current Node]
                   /              \
              compare()           compare()
             returns true        returns false
            /                    \
        [Go LEFT]            [Go RIGHT]
        (smaller)            (larger)
```

## 📊 Complete Example with All Users

### Final Name BST Structure:
```
                    [John Doe]
                   /          \
            [Alice Wong]  [Bob Brown]
                        /          \
                [Bella Rose] [Jane Smith]
```

### Final Email BST Structure:
```
                    [aw@e.com]
                   /          \
            [bb@e.com]  [jd@e.com]
                      /          \
              [br@e.com] [js@e.com]
```

## 🔄 In-order Traversal Results

### Name BST In-order (Left → Root → Right):
```
[Alice Wong] → [Bella Rose] → [Bob Brown] → [Jane Smith] → [John Doe]
```

### Email BST In-order (Left → Root → Right):
```
[aw@e.com] → [bb@e.com] → [br@e.com] → [jd@e.com] → [js@e.com]
```

## 🎯 Key Rules Summary

1. **Root Selection**: First user inserted becomes the root
2. **Left Child**: New user is "smaller" than current node
3. **Right Child**: New user is "larger" than current node
4. **Recursive**: Continue down the tree until you find an empty spot
5. **Comparison**: Based on the specific comparison function (name or email)

## 🚀 Why This Works

- **Automatic Sorting**: In-order traversal always gives sorted results
- **Efficient Search**: O(log n) average case for finding users
- **Flexible**: Different comparison functions for different sorting criteria
- **Balanced**: Naturally maintains some balance (though not guaranteed)

The beauty is that the BST automatically maintains the sorted order as you insert users, making it perfect for generating sorted lists of users by name or email! 
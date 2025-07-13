# Data Structures Visualization: Hash Table & BST Storage

## 🗂️ Hash Table Storage (UserHashTable)

### Structure Overview
```
UserHashTable (100 buckets)
├── Bucket 0: [CustomList<User>]
├── Bucket 1: [CustomList<User>]
├── Bucket 2: [CustomList<User>]
├── ...
├── Bucket 45: [CustomList<User>] ← "john_doe" hashes here
├── ...
├── Bucket 99: [CustomList<User>]
```

### Detailed Hash Table Example
```
TABLE_SIZE = 100
Hash Function: std::hash<QString>{}(username) % 100

┌─────────────────────────────────────────────────────────────┐
│                    HASH TABLE (100 buckets)                 │
├─────────────────────────────────────────────────────────────┤
│ Bucket 0:  [Empty]                                          │
│ Bucket 1:  [Empty]                                          │
│ Bucket 2:  [Empty]                                          │
│ ...                                                         │
│ Bucket 15: [john_doe → User{name:"John Doe", email:"j@e.com"}] │
│            [jane_smith → User{name:"Jane Smith", email:"js@e.com"}] │
│ Bucket 23: [alice_wong → User{name:"Alice Wong", email:"aw@e.com"}] │
│ Bucket 45: [bob_brown → User{name:"Bob Brown", email:"bb@e.com"}] │
│            [bella_rose → User{name:"Bella Rose", email:"br@e.com"}] │
│ ...                                                         │
│ Bucket 99: [Empty]                                          │
└─────────────────────────────────────────────────────────────┘

Hash Calculations:
"john_doe" → hash("john_doe") % 100 = 15
"jane_smith" → hash("jane_smith") % 100 = 15 (collision!)
"alice_wong" → hash("alice_wong") % 100 = 23
"bob_brown" → hash("bob_brown") % 100 = 45
"bella_rose" → hash("bella_rose") % 100 = 45 (collision!)
```

### Collision Resolution (Chaining)
```
Bucket 15: [Linked List]
┌─────────────┐    ┌─────────────┐
│ john_doe    │───▶│ jane_smith  │───▶ NULL
│ John Doe    │    │ Jane Smith  │
│ j@e.com     │    │ js@e.com    │
└─────────────┘    └─────────────┘

Bucket 45: [Linked List]
┌─────────────┐    ┌─────────────┐
│ bob_brown   │───▶│ bella_rose  │───▶ NULL
│ Bob Brown   │    │ Bella Rose  │
│ bb@e.com    │    │ br@e.com    │
└─────────────┘    └─────────────┘
```

## 🌳 Binary Search Tree Storage (UserBST)

### Name BST Structure
```
                    [Alice Wong]
                   /            \
            [Bob Brown]    [John Doe]
           /           \           \
    [Bella Rose]  [Empty]    [Jane Smith]
   /           \                    \
[Empty]    [Empty]              [Empty]

Comparison Function: a.getFullName() < b.getFullName()
```

### Email BST Structure
```
                    [aw@e.com]
                   /           \
            [bb@e.com]    [j@e.com]
           /           \           \
    [br@e.com]  [Empty]    [js@e.com]
   /           \                    \
[Empty]    [Empty]              [Empty]

Comparison Function: a.getEmail() < b.getEmail()
```

### BST Node Structure
```
┌─────────────────────────────────────────┐
│              BSTNode<User>              │
├─────────────────────────────────────────┤
│ data: User{                            │
│   username: "alice_wong",              │
│   fullName: "Alice Wong",              │
│   email: "aw@e.com"                    │
│ }                                      │
├─────────────────────────────────────────┤
│ left:  → [Bob Brown]                   │
│ right: → [John Doe]                    │
└─────────────────────────────────────────┘
```

## 🔄 Combined Storage in UserProfileManager

```
UserProfileManager
├── UserHashTable hashTable
│   └── 100 buckets with chained User objects
│
├── UserBST<User> nameBST
│   └── Tree sorted by fullName
│
└── UserBST<User> emailBST
    └── Tree sorted by email
```

### Complete Data Flow Example

**Adding User "Alice Wong":**
```
1. Hash Table Insert:
   hash("alice_wong") % 100 = 23
   → Add to Bucket 23

2. Name BST Insert:
   Compare "Alice Wong" with root
   → Insert as left child of root

3. Email BST Insert:
   Compare "aw@e.com" with root  
   → Insert as left child of root
```

**Looking up User "alice_wong":**
```
1. Hash Table Lookup:
   hash("alice_wong") % 100 = 23
   → Search Bucket 23
   → Found in O(1) average time
```

**Getting sorted users by name:**
```
1. Name BST In-order Traversal:
   Left → Root → Right
   → Returns: [Alice Wong, Bob Brown, Bella Rose, John Doe, Jane Smith]
```

## 📊 Performance Comparison

| Operation | Hash Table | BST |
|-----------|------------|-----|
| **Insert User** | O(1) average | O(log n) |
| **Find by Username** | O(1) average | O(n) |
| **Get Sorted by Name** | O(n log n) | O(n) |
| **Get Sorted by Email** | O(n log n) | O(n) |
| **Memory Usage** | O(n) | O(n) |

## 🎯 Key Benefits

**Hash Table:**
- ⚡ Lightning-fast user lookups by username
- 🔗 Handles collisions with chaining
- 📦 Fixed memory overhead

**BST:**
- 📈 Maintains sorted order automatically
- 🔍 Efficient range queries
- 🎨 Perfect for displaying sorted lists

**Combined Approach:**
- 🚀 Fast access when you need it
- 📋 Sorted output when you want it
- 🎯 Best of both worlds! 
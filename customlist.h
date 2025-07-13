#ifndef CUSTOMLIST_H
#define CUSTOMLIST_H

template<typename T>
class CustomList {
private:
    struct Node {
        T data;
        Node* next;
        Node* prev;
        
        Node(const T& value) : data(value), next(nullptr), prev(nullptr) {}
    };
    
    Node* head;
    Node* tail;
    size_t size_;

public:
    // Constructor
    CustomList() : head(nullptr), tail(nullptr), size_(0) {}

    // Destructor
    ~CustomList() {
        clear();
    }

    // Copy constructor
    CustomList(const CustomList& other) : head(nullptr), tail(nullptr), size_(0) {
        Node* current = other.head;
        while (current != nullptr) {
            push_back(current->data);
            current = current->next;
        }
    }

    // Assignment operator
    CustomList& operator=(const CustomList& other) {
        if (this != &other) {
            clear();
            Node* current = other.head;
            while (current != nullptr) {
                push_back(current->data);
                current = current->next;
            }
        }
        return *this;
    }

    // Capacity
    size_t size() const { return size_; }
    bool empty() const { return size_ == 0; }

    // Modifiers
    void push_back(const T& value) {
        Node* newNode = new Node(value);
        if (empty()) {
            head = tail = newNode;
        } else {
            newNode->prev = tail;
            tail->next = newNode;
            tail = newNode;
        }
        ++size_;
    }

    void push_front(const T& value) {
        Node* newNode = new Node(value);
        if (empty()) {
            head = tail = newNode;
        } else {
            newNode->next = head;
            head->prev = newNode;
            head = newNode;
        }
        ++size_;
    }

    void pop_back() {
        if (!empty()) {
            Node* temp = tail;
            tail = tail->prev;
            if (tail) {
                tail->next = nullptr;
            } else {
                head = nullptr;
            }
            delete temp;
            --size_;
        }
    }

    void pop_front() {
        if (!empty()) {
            Node* temp = head;
            head = head->next;
            if (head) {
                head->prev = nullptr;
            } else {
                tail = nullptr;
            }
            delete temp;
            --size_;
        }
    }

    void clear() {
        while (!empty()) {
            pop_front();
        }
    }

    // Iterator support
    class Iterator {
    private:
        Node* current;

    public:
        Iterator(Node* node) : current(node) {}

        T& operator*() { return current->data; }
        Iterator& operator++() {
            current = current->next;
            return *this;
        }
        Iterator operator++(int) {
            Iterator temp = *this;
            current = current->next;
            return temp;
        }
        bool operator==(const Iterator& other) const {
            return current == other.current;
        }
        bool operator!=(const Iterator& other) const {
            return current != other.current;
        }
    };

    Iterator begin() { return Iterator(head); }
    Iterator end() { return Iterator(nullptr); }
};

#endif // CUSTOMLIST_H 
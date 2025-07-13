# Cab Booking System – C++ Qt DSA Project

This is my attempt at my **Data Structures and Algorithms (DSA) final project** for the **4th semester** of my BS Computer Science degree.

A real-world cab booking simulator developed in **C++** using the **Qt framework** and **SQLite database**, demonstrating core DSA concepts through custom implementations and GUI integration.

---

## 🚀 Features

- **User Management**
  - Registration and login (SQLite-backed)
  - Profile editing and secure credential handling

- **Cab Booking System**
  - Real-time cab availability check
  - Distance and rating-based cab sorting
  - Booking confirmation with animated ride display

- **Driver and Ride Management**
  - Ride tracking, earnings, and ratings
  - Post-ride user feedback with average rating calculation

- **Algorithmic Logic**
  - **Dijkstra’s algorithm** for fare and path calculation
  - Modular sorting algorithms (Merge, Quick, Selection)
  - BST for sorted user views
  - Hash table with chaining for fast username lookups
  - Heap-based priority queue for cab selection

---

## 🧠 Data Structures Used

| Data Structure     | Purpose                                  |
|--------------------|-------------------------------------------|
| CustomVector       | Dynamic array for storing users/cabs      |
| CustomList         | Doubly-linked list for hash chaining      |
| UserBST            | Binary Search Tree for user sorting       |
| UserHashTable      | Fast username lookups (chaining method)   |
| PriorityQueue      | Cab prioritization by rating & distance   |
| Dijkstra Algorithm | Fare estimation and shortest path         |

---

## 🛠️ Tech Stack

- **Language:** C++
- **GUI:** Qt (Qt Creator)
- **Database:** SQLite
- **Animation:** QGraphicsView, QPropertyAnimation
- **Multimedia (Optional):** QMediaPlayer

---

## 📦 Database Schema

**Users Table**
```sql
CREATE TABLE users (
    username TEXT PRIMARY KEY,
    password TEXT,
    full_name TEXT,
    email TEXT,
    phone TEXT,
    date_of_birth TEXT,
    address TEXT,
    profile_picture BLOB
);
CREATE TABLE Cabs (
    numberPlate TEXT PRIMARY KEY,
    driverName TEXT,
    rating FLOAT,
    distance FLOAT,
    available INTEGER,
    num_rides INTEGER,
    earnings FLOAT
);

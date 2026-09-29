# Library Management System

A console-based Library Management System written in C++ using object-oriented programming. It manages books, members and borrowing records through a simple menu.

## Features

- Add books (title and author) and members (name)
- Issue a book to a member
- Return a book
- Search books by title or author (partial match, not case-sensitive)
- Display all books with their status (Available / Issued)
- Display all members along with the books they have borrowed
- Input validation and error messages for cases like book not found, member not found, book already issued, and returning a book that is not issued

## Technologies Used

- C++ (C++17)
- Standard Library only (`vector`, `string`, `algorithm`, `iostream`)

## How to Compile

```bash
g++ -std=c++17 library_management.cpp -o library_management
```

## How to Run

```bash
./library_management
```

On Windows:

```bash
library_management.exe
```

## Basic Usage

1. Run the program and choose an option from the menu (1-8).
2. Add at least one book and one member first. IDs are generated automatically, starting from 1.
3. Use **Issue Book** with a Book ID and a Member ID to lend a book.
4. Use **Return Book** with the Book ID when it comes back.
5. Use **Search Book** to find books by part of a title or author name.
6. Choose **Exit** to close the program. Data is stored in memory only, so it is cleared when the program ends.

## OOP Concepts Demonstrated

- **Classes and objects:** `Book`, `Member` and `Library`
- **Encapsulation:** data members are private and accessed through public functions
- **Constructors:** used to initialise books, members and the library
- **Vectors:** `Library` stores its books and members in `vector`s, and each `Member` keeps a vector of borrowed book IDs
- **Abstraction:** the `main` function only calls simple `Library` functions like `issueBook()` and `searchBooks()`

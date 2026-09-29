// Library Management System
// A console-based C++ program that manages books, members and borrowing records.

#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

// ---------------------------------------------------------------
// Input helpers: keep reading until the user enters valid input
// ---------------------------------------------------------------

// Reads one line from the user. Exits cleanly if input ends (Ctrl+D / Ctrl+Z).
string readLine(const string &prompt) {
    string line;
    cout << prompt;
    if (!getline(cin, line)) {
        cout << "\nInput ended. Exiting program.\n";
        exit(0);
    }
    return line;
}

// Reads a non-empty piece of text (used for titles, authors and names).
string readText(const string &prompt) {
    while (true) {
        string text = readLine(prompt);
        if (text.find_first_not_of(" \t") != string::npos) {
            return text;
        }
        cout << "Input cannot be empty. Please try again.\n";
    }
}

// Reads a positive whole number (used for menu choices and IDs).
int readNumber(const string &prompt) {
    while (true) {
        string input = readLine(prompt);
        bool valid = !input.empty() && input.length() <= 9;
        for (char ch : input) {
            if (!isdigit(static_cast<unsigned char>(ch))) {
                valid = false;
            }
        }
        if (valid) {
            return stoi(input);
        }
        cout << "Invalid input. Please enter a number.\n";
    }
}

// Converts a string to lowercase so searching ignores capital letters.
string toLowerCase(string text) {
    for (char &ch : text) {
        ch = static_cast<char>(tolower(static_cast<unsigned char>(ch)));
    }
    return text;
}

// ---------------------------------------------------------------
// Book class: stores the details of one book
// ---------------------------------------------------------------
class Book {
private:
    int id;
    string title;
    string author;
    bool issued;         // true if the book is currently borrowed
    int issuedToMember;  // ID of the member who has it (0 if available)

public:
    Book(int bookId, string bookTitle, string bookAuthor) {
        id = bookId;
        title = bookTitle;
        author = bookAuthor;
        issued = false;
        issuedToMember = 0;
    }

    int getId() const { return id; }
    string getTitle() const { return title; }
    string getAuthor() const { return author; }
    bool isIssued() const { return issued; }

    void issueTo(int memberId) {
        issued = true;
        issuedToMember = memberId;
    }

    void markReturned() {
        issued = false;
        issuedToMember = 0;
    }

    int getIssuedToMember() const { return issuedToMember; }

    void display() const {
        cout << "ID: " << id << " | Title: " << title << " | Author: " << author
             << " | Status: ";
        if (issued) {
            cout << "Issued (Member ID " << issuedToMember << ")\n";
        } else {
            cout << "Available\n";
        }
    }
};

// ---------------------------------------------------------------
// Member class: stores a member and the books they have borrowed
// ---------------------------------------------------------------
class Member {
private:
    int id;
    string name;
    vector<int> borrowedBookIds;  // borrowing record of this member

public:
    Member(int memberId, string memberName) {
        id = memberId;
        name = memberName;
    }

    int getId() const { return id; }
    string getName() const { return name; }
    const vector<int> &getBorrowedBookIds() const { return borrowedBookIds; }

    void addBorrowedBook(int bookId) {
        borrowedBookIds.push_back(bookId);
    }

    void removeBorrowedBook(int bookId) {
        borrowedBookIds.erase(
            remove(borrowedBookIds.begin(), borrowedBookIds.end(), bookId),
            borrowedBookIds.end());
    }
};

// ---------------------------------------------------------------
// Library class: holds all books and members and performs operations
// ---------------------------------------------------------------
class Library {
private:
    vector<Book> books;
    vector<Member> members;
    int nextBookId;
    int nextMemberId;

    // Returns the position of the book in the vector, or -1 if not found.
    int findBookIndex(int bookId) const {
        for (size_t i = 0; i < books.size(); i++) {
            if (books[i].getId() == bookId) {
                return static_cast<int>(i);
            }
        }
        return -1;
    }

    // Returns the position of the member in the vector, or -1 if not found.
    int findMemberIndex(int memberId) const {
        for (size_t i = 0; i < members.size(); i++) {
            if (members[i].getId() == memberId) {
                return static_cast<int>(i);
            }
        }
        return -1;
    }

public:
    Library() {
        nextBookId = 1;
        nextMemberId = 1;
    }

    void addBook(const string &title, const string &author) {
        books.push_back(Book(nextBookId, title, author));
        cout << "Book added successfully. Book ID: " << nextBookId << "\n";
        nextBookId++;
    }

    void addMember(const string &name) {
        members.push_back(Member(nextMemberId, name));
        cout << "Member added successfully. Member ID: " << nextMemberId << "\n";
        nextMemberId++;
    }

    void issueBook(int bookId, int memberId) {
        int bookIndex = findBookIndex(bookId);
        if (bookIndex == -1) {
            cout << "Error: Book not found.\n";
            return;
        }
        int memberIndex = findMemberIndex(memberId);
        if (memberIndex == -1) {
            cout << "Error: Member not found.\n";
            return;
        }
        if (books[bookIndex].isIssued()) {
            cout << "Error: This book is already issued.\n";
            return;
        }

        books[bookIndex].issueTo(memberId);
        members[memberIndex].addBorrowedBook(bookId);
        cout << "\"" << books[bookIndex].getTitle() << "\" has been issued to "
             << members[memberIndex].getName() << ".\n";
    }

    void returnBook(int bookId) {
        int bookIndex = findBookIndex(bookId);
        if (bookIndex == -1) {
            cout << "Error: Book not found.\n";
            return;
        }
        if (!books[bookIndex].isIssued()) {
            cout << "Error: This book is already available (it was not issued).\n";
            return;
        }

        int memberIndex = findMemberIndex(books[bookIndex].getIssuedToMember());
        if (memberIndex != -1) {
            members[memberIndex].removeBorrowedBook(bookId);
        }
        books[bookIndex].markReturned();
        cout << "\"" << books[bookIndex].getTitle() << "\" has been returned.\n";
    }

    // Searches by title or author (partial match, ignores capital letters).
    void searchBooks(const string &keyword) const {
        string key = toLowerCase(keyword);
        bool found = false;

        for (size_t i = 0; i < books.size(); i++) {
            string title = toLowerCase(books[i].getTitle());
            string author = toLowerCase(books[i].getAuthor());
            if (title.find(key) != string::npos || author.find(key) != string::npos) {
                books[i].display();
                found = true;
            }
        }

        if (!found) {
            cout << "No books found matching \"" << keyword << "\".\n";
        }
    }

    void displayAllBooks() const {
        if (books.empty()) {
            cout << "No books in the library yet.\n";
            return;
        }
        for (size_t i = 0; i < books.size(); i++) {
            books[i].display();
        }
    }

    // Shows each member along with the titles of the books they have borrowed.
    void displayAllMembers() const {
        if (members.empty()) {
            cout << "No members registered yet.\n";
            return;
        }
        for (size_t i = 0; i < members.size(); i++) {
            cout << "ID: " << members[i].getId() << " | Name: " << members[i].getName()
                 << "\n";

            const vector<int> &borrowed = members[i].getBorrowedBookIds();
            if (borrowed.empty()) {
                cout << "    Borrowed books: None\n";
            } else {
                cout << "    Borrowed books:\n";
                for (size_t j = 0; j < borrowed.size(); j++) {
                    int bookIndex = findBookIndex(borrowed[j]);
                    if (bookIndex != -1) {
                        cout << "      - " << books[bookIndex].getTitle()
                             << " (Book ID " << borrowed[j] << ")\n";
                    }
                }
            }
        }
    }
};

// ---------------------------------------------------------------
// Menu
// ---------------------------------------------------------------
void showMenu() {
    cout << "\n========================================\n";
    cout << "        LIBRARY MANAGEMENT SYSTEM\n";
    cout << "========================================\n";
    cout << "1. Add Book\n";
    cout << "2. Add Member\n";
    cout << "3. Issue Book\n";
    cout << "4. Return Book\n";
    cout << "5. Search Book\n";
    cout << "6. Display All Books\n";
    cout << "7. Display All Members\n";
    cout << "8. Exit\n";
    cout << "----------------------------------------\n";
}

int main() {
    Library library;
    int choice = 0;

    while (choice != 8) {
        showMenu();
        choice = readNumber("Enter your choice (1-8): ");
        cout << "\n";

        switch (choice) {
            case 1: {
                string title = readText("Enter book title: ");
                string author = readText("Enter author name: ");
                library.addBook(title, author);
                break;
            }
            case 2: {
                string name = readText("Enter member name: ");
                library.addMember(name);
                break;
            }
            case 3: {
                int bookId = readNumber("Enter book ID: ");
                int memberId = readNumber("Enter member ID: ");
                library.issueBook(bookId, memberId);
                break;
            }
            case 4: {
                int bookId = readNumber("Enter book ID to return: ");
                library.returnBook(bookId);
                break;
            }
            case 5: {
                string keyword = readText("Enter title or author to search: ");
                library.searchBooks(keyword);
                break;
            }
            case 6:
                library.displayAllBooks();
                break;
            case 7:
                library.displayAllMembers();
                break;
            case 8:
                cout << "Thank you for using the Library Management System.\n";
                break;
            default:
                cout << "Invalid choice. Please select a number from 1 to 8.\n";
        }
    }
    return 0;
}

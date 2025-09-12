#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>
#include <iomanip>
#include <ctime>

using namespace std;
// Book class to represent individual books
class Book {
private:
    int bookId;
    string title;
    string author;
    string isbn;
    bool isIssued;
    int issuedToStudentId;
    string issueDate;
    string returnDate;

public:
    // Constructor
    Book() : bookId(0), isIssued(false), issuedToStudentId(0) {}

    Book(int id, string t, string a, string isbn_num) : bookId(id), title(t), author(a), isbn(isbn_num), isIssued(false), issuedToStudentId(0) {}


    // Getters
    int getBookId() const { return bookId; }
    string getTitle() const { return title; }
    string getAuthor() const { return author; }
    string getIsbn() const { return isbn; }
    bool getIsIssued() const { return isIssued; }
    int getIssuedToStudentId() const { return issuedToStudentId; }
    string getIssueDate() const { return issueDate; }
    string getReturnDate() const { return returnDate; }

    
    // Setters
    void setBookId(int id) { bookId = id; }
    void setTitle(string t) { title = t; }
    void setAuthor(string a) { author = a; }
    void setIsbn(string isbn_num) { isbn = isbn_num; }
    void setIsIssued(bool issued) { isIssued = issued; }
    void setIssuedToStudentId(int studentId) { issuedToStudentId = studentId; }
    void setIssueDate(string date) { issueDate = date; }
    void setReturnDate(string date) { returnDate = date; }

    // Method to get current date
    string getCurrentDate() {
        time_t now = time(0);
        char* dt = ctime(&now);
        string date = dt;
        date.pop_back(); 
        return date;
    }

    // Method to issue book
    void issueBook(int studentId) {
        if (!isIssued) {
            isIssued = true;
            issuedToStudentId = studentId;
            issueDate = getCurrentDate();
            returnDate = "";
        }
    }

    // Method to return book
    void returnBook() {
        if (isIssued) {
            isIssued = false;
            issuedToStudentId = 0;
            returnDate = getCurrentDate();
        }
    }

    // Method to convert book data to string for file storage
    string toString() const {
        return to_string(bookId) + "," + title + "," + author + "," + isbn + "," +
        (isIssued ? "1" : "0") + "," + to_string(issuedToStudentId) + "," + issueDate + "," + returnDate;
    }

    // Method to create book from string (for file reading)
    void fromString(const string& line) {
        stringstream ss(line);
        string item;
        int index = 0;

        while (getline(ss, item, ',')) {
            switch (index) {
                case 0: bookId = stoi(item); break;
                case 1: title = item; break;
                case 2: author = item; break;
                case 3: isbn = item; break;
                case 4: isIssued = (item == "1"); break;
                case 5: issuedToStudentId = stoi(item); break;
                case 6: issueDate = item; break;
                case 7: returnDate = item; break;
            }
            index++;
        }
    }
};

// Student class to represent students
class Student {
private:
    int studentId;
    string name;
    string email;
    string phone;
    vector<int> issuedBookIds;

public:
    // Constructor
    Student() : studentId(0) {}

    Student(int id, string n, string e, string p) 
        : studentId(id), name(n), email(e), phone(p) {}

    int getStudentId() const { return studentId; }
    string getName() const { return name; }
    string getEmail() const { return email; }
    string getPhone() const { return phone; }
    vector<int> getIssuedBookIds() const { return issuedBookIds; }

    void setStudentId(int id) { studentId = id; }
    void setName(string n) { name = n; }
    void setEmail(string e) { email = e; }
    void setPhone(string p) { phone = p; }

    // Method to add issued book
    void addIssuedBook(int bookId) {
        issuedBookIds.push_back(bookId);
    }

    // Method to remove returned book
    void removeIssuedBook(int bookId) {
        issuedBookIds.erase(remove(issuedBookIds.begin(), issuedBookIds.end(), bookId), issuedBookIds.end());
    }

    // Method to get number of issued books
    int getNumberOfIssuedBooks() const {
        return issuedBookIds.size();
    }

    // Method to convert student data to string for file storage
    string toString() const {
        string result = to_string(studentId) + "," + name + "," + email + "," + phone + ",";
        for (int i = 0; i < issuedBookIds.size(); i++) { 
            result += to_string(issuedBookIds[i]);
            if (i < issuedBookIds.size() - 1) result += ";";
        }
        return result;
    }

    // Method to create student from string (for file reading)
    void fromString(const string& line) {
        stringstream ss(line);
        string item;
        int index = 0;

        while (getline(ss, item, ',')) {
            switch (index) {
                case 0: studentId = stoi(item); break;
                case 1: name = item; break;
                case 2: email = item; break;
                case 3: phone = item; break;
                case 4: {
                    if (!item.empty()) {
                        stringstream bookSS(item);
                        string bookId;
                        while (getline(bookSS, bookId, ';')) {
                            if (!bookId.empty()) {
                                issuedBookIds.push_back(stoi(bookId));
                            }
                        }
                    }
                    break;
                }
            }
            index++;
        }
    }
};

// Library class to manage the entire system
class Library {
private:
    vector<Book> books;
    vector<Student> students;
    const string BOOKS_FILE = "books.txt";
    const string STUDENTS_FILE = "students.txt";
    const int MAX_BOOKS_PER_STUDENT = 3;

public:
    // Constructor
    Library() {
        loadBooksFromFile();
        loadStudentsFromFile();
    }

    // Destructor
    ~Library() {
        saveBooksToFile();
        saveStudentsToFile();
    }

    // File handling methods
    void loadBooksFromFile() {
        ifstream file(BOOKS_FILE);
        string line;

        books.clear();
        while (getline(file, line)) {
            if (!line.empty()) {
                Book book;
                book.fromString(line);
                books.push_back(book);
            }
        }
        file.close();
    }

    void saveBooksToFile() {
        ofstream file(BOOKS_FILE);
        for (const auto& book : books) {
            file << book.toString() << endl;
        }
        file.close();
    }

    void loadStudentsFromFile() {
        ifstream file(STUDENTS_FILE);
        string line;

        students.clear();
        while (getline(file, line)) {
            if (!line.empty()) {
                Student student;
                student.fromString(line);
                students.push_back(student);
            }
        }
        file.close();
    }

    void saveStudentsToFile() {
        ofstream file(STUDENTS_FILE);
        for (const auto& student : students) {
            file << student.toString() << endl;
        }
        file.close();
    }

    // Book management methods
    void addBook() {
        int bookId;
        string title, author, isbn;

        cout << "\n=== Add New Book ===\n";
        cout << "Enter Book ID: ";
        cin >> bookId;
        cin.ignore();

        // Check if book ID already exists
        for (const auto& book : books) {
            if (book.getBookId() == bookId) {
                cout << "Book with ID " << bookId << " already exists!\n";
                return;
            }
        }

        cout << "Enter Title: ";
        getline(cin, title);
        cout << "Enter Author: ";
        getline(cin, author);
        cout << "Enter ISBN: ";
        getline(cin, isbn);

        Book newBook(bookId, title, author, isbn);
        books.push_back(newBook);

        cout << "Book added successfully!\n";
        saveBooksToFile();
    }

    void displayAllBooks() {
        cout << "\n=== All Books ===\n";
        cout << left << setw(5) << "ID" << setw(30) << "Title" << setw(20) << "Author" 
        << setw(15) << "ISBN" << setw(10) << "Status" << "Issued To\n";
        cout << string(90, '-') << endl;

        for (const auto& book : books) {
            cout<< left << setw(7) << book.getBookId() 
                << setw(30) << book.getTitle() 
                << setw(25) << book.getAuthor() 
                << setw(15) << book.getIsbn() 
                << setw(10) << (book.getIsIssued() ? "Issued" : "Available");

            if (book.getIsIssued()) {
                cout << "Student ID: " << book.getIssuedToStudentId();
            }
            cout << endl;
        }
    }

    void searchBook() {
        cout << "\n=== Search Book ===\n";
        cout << "Search by: 1. ID  2. Title  3. Author\n";
        cout << "Enter choice: ";

        int choice;
        cin >> choice;
        cin.ignore();

        string searchTerm;
        bool found = false;

        switch (choice) {
            case 1: {
                cout << "Enter Book ID: ";
                int id;
                cin >> id;

                for (const auto& book : books) {
                    if (book.getBookId() == id) {
                        displayBookDetails(book);
                        found = true;
                        break;
                    }
                }
                break;
            }
            case 2: {
                cout << "Enter Book Title: ";
                getline(cin, searchTerm);

                for (const auto& book : books) {
                    if (book.getTitle().find(searchTerm) != string::npos) {
                        displayBookDetails(book);
                        found = true;
                    }
                }
                break;
            }
            case 3: {
                cout << "Enter Author Name: ";
                getline(cin, searchTerm);

                for (const auto& book : books) {
                    if (book.getAuthor().find(searchTerm) != string::npos) {
                        displayBookDetails(book);
                        found = true;
                    }
                }
                break;
            }
            default:
                cout << "Invalid choice!\n";
                return;
        }

        if (!found) {
            cout << "No books found matching the search criteria.\n";
        }
    }

    void displayBookDetails(const Book& book) {
        cout << "\n--- Book Details ---\n";
        cout << "ID: " << book.getBookId() << endl;
        cout << "Title: " << book.getTitle() << endl;
        cout << "Author: " << book.getAuthor() << endl;
        cout << "ISBN: " << book.getIsbn() << endl;
        cout << "Status: " << (book.getIsIssued() ? "Issued" : "Available") << endl;

        if (book.getIsIssued()) {
            cout << "Issued to Student ID: " << book.getIssuedToStudentId() << endl;
            cout << "Issue Date: " << book.getIssueDate() << endl;
        }
        cout << endl;
    }

    // Student management methods
    void addStudent() {
        int studentId;
        string name, email, phone;

        cout << "\n=== Add New Student ===\n";
        cout << "Enter Student ID: ";
        cin >> studentId;
        cin.ignore();

        // Check whether the student ID already exist
        for (const auto& student : students) {
            if (student.getStudentId() == studentId) {
                cout << "Student with ID " << studentId << " already exists!\n";
                return;
            }
        }

        cout << "Enter Name: ";
        getline(cin, name);
        cout << "Enter Email: ";
        getline(cin, email);
        cout << "Enter Phone: ";
        getline(cin, phone);

        Student newStudent(studentId, name, email, phone);
        students.push_back(newStudent);

        cout << "Student added successfully!\n";
        saveStudentsToFile();
    }

    void displayAllStudents() {
        cout << "\n=== All Students ===\n";
        cout << left << setw(5) << "ID" << setw(25) << "Name" << setw(30) << "Email" << setw(15) << "Phone" << "Books Issued\n";
        cout << string(80, '-') << endl;

        for (const auto& student : students) {
            cout<< left << setw(5) << student.getStudentId() 
                << setw(25) << student.getName() 
                << setw(30) << student.getEmail() 
                << setw(15) << student.getPhone() 
                << student.getNumberOfIssuedBooks() << endl;
        }
    }

    void searchStudent() {
        cout << "\n=== Search Student ===\n";
        cout << "Search by: 1. ID  2. Name\n";
        cout << "Enter choice: ";

        int choice;
        cin >> choice;
        cin.ignore();

        bool found = false;

        switch (choice) {
            case 1: {
                cout << "Enter Student ID: ";
                int id;
                cin >> id;

                for (const auto& student : students) {
                    if (student.getStudentId() == id) {
                        displayStudentDetails(student);
                        found = true;
                        break;
                    }
                }
                break;
            }
            case 2: {
                cout << "Enter Student Name: ";
                string searchTerm;
                getline(cin, searchTerm);

                for (const auto& student : students) {
                    if (student.getName().find(searchTerm) != string::npos) {
                        displayStudentDetails(student);
                        found = true;
                    }
                }
                break;
            }
            default:
                cout << "Invalid choice!\n";
                return;
        }

        if (!found) {
            cout << "No students found matching the search criteria.\n";
        }
    }

    void displayStudentDetails(const Student& student) {
        cout << "\n--- Student Details ---\n";
        cout << "ID: " << student.getStudentId() << endl;
        cout << "Name: " << student.getName() << endl;
        cout << "Email: " << student.getEmail() << endl;
        cout << "Phone: " << student.getPhone() << endl;
        cout << "Number of Books Issued: " << student.getNumberOfIssuedBooks() << endl;

        if (student.getNumberOfIssuedBooks() > 0) {
            cout << "Issued Book IDs: ";
            vector<int> bookIds = student.getIssuedBookIds();
            for (int i = 0; i < bookIds.size(); i++) {
                cout << bookIds[i];
                if (i < bookIds.size() - 1) cout << ", ";
            }
            cout << endl;
        }
        cout << endl;
    }

    // Book issue/return methods
    void issueBook() {
        int studentId, bookId;

        cout << "\n=== Issue Book ===\n";
        cout << "Enter Student ID: ";
        cin >> studentId;
        cout << "Enter Book ID: ";
        cin >> bookId;

        // Find student
        Student* student = nullptr;
        for (auto& s : students) {
            if (s.getStudentId() == studentId) {
                student = &s;
                break;
            }
        }

        if (!student) {
            cout << "Student with ID " << studentId << " not found!\n";
            return;
        }

        // Check if student has reached maximum book limit
        if (student->getNumberOfIssuedBooks() >= MAX_BOOKS_PER_STUDENT) {
            cout << "Student has already issued maximum number of books (" << MAX_BOOKS_PER_STUDENT << ")!\n";
            return;
        }

        // Find book
        Book* book = nullptr;
        for (auto& b : books) {
            if (b.getBookId() == bookId) {
                book = &b;
                break;
            }
        }

        if (!book) {
            cout << "Book with ID " << bookId << " not found!\n";
            return;
        }

        // Check if book is already issued
        if (book->getIsIssued()) {
            cout << "Book is already issued to Student ID: " << book->getIssuedToStudentId() << "\n";
            return;
        }

        // Issue the book
        book->issueBook(studentId);
        student->addIssuedBook(bookId);

        cout << "Book '" << book->getTitle() << "' issued successfully to " << student->getName() << "\n";
        cout << "Issue Date: " << book->getIssueDate() << "\n";

        saveBooksToFile();
        saveStudentsToFile();
    }

    void returnBook() {
        int studentId, bookId;

        cout << "\n=== Return Book ===\n";
        cout << "Enter Student ID: ";
        cin >> studentId;
        cout << "Enter Book ID: ";
        cin >> bookId;

        // Find student
        Student* student = nullptr;
        for (auto& s : students) {
            if (s.getStudentId() == studentId) {
                student = &s;
                break;
            }
        }

        if (!student) {
            cout << "Student with ID " << studentId << " not found!\n";
            return;
        }

        // Find book
        Book* book = nullptr;
        for (auto& b : books) {
            if (b.getBookId() == bookId) {
                book = &b;
                break;
            }
        }

        if (!book) {
            cout << "Book with ID " << bookId << " not found!\n";
            return;
        }

        // Check if book is issued to this student
        if (!book->getIsIssued() || book->getIssuedToStudentId() != studentId) {
            cout << "This book is not issued to the specified student!\n";
            return;
        }

        // Return the book
        book->returnBook();
        student->removeIssuedBook(bookId);

        cout << "Book '" << book->getTitle() << "' returned successfully by " << student->getName() << "\n";
        cout << "Return Date: " << book->getReturnDate() << "\n";

        saveBooksToFile();
        saveStudentsToFile();
    }

    void displayIssuedBooks() {
        cout << "\n=== Currently Issued Books ===\n";
        cout << left << setw(5) << "Book ID" << setw(30) << "Title" << setw(20) << "Author" << setw(10) << "Student ID" << setw(25) << "Student Name" << "Issue Date\n";
        cout << string(120, '-') << endl;

        bool hasIssuedBooks = false;

        for (const auto& book : books) {
            if (book.getIsIssued()) {
                hasIssuedBooks = true;

                // Find student name
                string studentName = "Unknown";
                for (const auto& student : students) {
                    if (student.getStudentId() == book.getIssuedToStudentId()) {
                        studentName = student.getName();
                        break;
                    }
                }

                cout<< left << setw(5) << book.getBookId()
                    << setw(30) << book.getTitle()
                    << setw(20) << book.getAuthor()
                    << setw(10) << book.getIssuedToStudentId()
                    << setw(25) << studentName
                    << book.getIssueDate() << endl;
            }
        }

        if (!hasIssuedBooks) {
            cout << "No books are currently issued.\n";
        }
    }

    // Main menu methods
    void displayMainMenu() {
        cout << "\n===================================\n";
        cout << "    LIBRARY MANAGEMENT SYSTEM\n";
        cout << "===================================\n";
        cout << "1. Book Management\n";
        cout << "2. Student Management\n";
        cout << "3. Issue Book\n";
        cout << "4. Return Book\n";
        cout << "5. Display Issued Books\n";
        cout << "6. Exit\n";
        cout << "===================================\n";
        cout << "Enter your choice: ";
    }

    void displayBookMenu() {
        cout << "\n=== Book Management ===\n";
        cout << "1. Add Book\n";
        cout << "2. Display All Books\n";
        cout << "3. Search Book\n";
        cout << "4. Back to Main Menu\n";
        cout << "Enter your choice: ";
    }

    void displayStudentMenu() {
        cout << "\n=== Student Management ===\n";
        cout << "1. Add Student\n";
        cout << "2. Display All Students\n";
        cout << "3. Search Student\n";
        cout << "4. Back to Main Menu\n";
        cout << "Enter your choice: ";
    }

    void handleBookMenu() {
        int choice;
        do {
            displayBookMenu();
            cin >> choice;

            switch (choice) {
                case 1: addBook(); break;
                case 2: displayAllBooks(); break;
                case 3: searchBook(); break;
                case 4: return;
                default: cout << "Invalid choice! Please try again.\n";
            }
        } while (choice != 4);
    }

    void handleStudentMenu() {
        int choice;
        do {
            displayStudentMenu();
            cin >> choice;

            switch (choice) {
                case 1: addStudent(); break;
                case 2: displayAllStudents(); break;
                case 3: searchStudent(); break;
                case 4: return;
                default: cout << "Invalid choice! Please try again.\n";
            }
        } while (choice != 4);
    }

    void run() {
        int choice;

        cout << "\n\t\t*** WELCOME TO LIBRARY MANAGEMENT SYSTEM ***\n";

        do {
            displayMainMenu();
            cin >> choice;

            switch (choice) {
                case 1: handleBookMenu(); break;
                case 2: handleStudentMenu(); break;
                case 3: issueBook(); break;
                case 4: returnBook(); break;
                case 5: displayIssuedBooks(); break;
                case 6: 
                    cout << "\nThank you for using Library Management System!\n";
                    cout << "Data saved successfully.\n";
                    break;
                default: 
                    cout << "Invalid choice! Please try again.\n";
            }
        } while (choice != 6);
    }
};

int main() {
    try {
        Library library;
        library.run();
    } catch (const exception& e) {
        cout << "An error occurred: " << e.what() << endl;
        return 1;
    }

    return 0;
}
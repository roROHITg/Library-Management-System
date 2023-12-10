#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_BOOKS 100
#define MAX_STUDENTS 100
#define MAX_TITLE_LENGTH 100
#define MAX_AUTHOR_LENGTH 50
#define MAX_NAME_LENGTH 50
#define FILENAME_BOOKS "library_books.csv"
#define FILENAME_STUDENTS "student_data.csv"

struct Book {
    char title[MAX_TITLE_LENGTH];
    char author[MAX_AUTHOR_LENGTH];
    int id;
    int quantity;
    int quantityAvailable; 
};


struct Student {
    char name[MAX_NAME_LENGTH];
    int rollNumber;
    int borrowedBookID; 
};

void addBook(struct Book books[], int *bookCount);
void displayBooks(struct Book books[], int bookCount);
void issueBook(struct Book books[], int bookCount, struct Student students[], int studentCount);
void returnBook(struct Book books[], int bookCount, struct Student students[], int studentCount);
void addStudent(struct Student students[], int *studentCount);
void displayStudents(struct Student students[], int studentCount);
void saveBookData(struct Book books[], int bookCount);
void loadBookData(struct Book books[], int *bookCount);
void saveStudentData(struct Student students[], int studentCount);
void loadStudentData(struct Student students[], int *studentCount);

int main() {
    struct Book books[MAX_BOOKS];
    int bookCount = 0;

    struct Student students[MAX_STUDENTS];
    int studentCount = 0;

    loadBookData(books, &bookCount);
    loadStudentData(students, &studentCount);

    int choice;

    do {
        printf("\nLibrary Management System\n");
        printf("1. Add a book\n");
        printf("2. Display all books\n");
        printf("3. Issue a book\n");
        printf("4. Return a book\n");
        printf("5. Add a student\n");
        printf("6. Display all students\n");
        printf("7. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                addBook(books, &bookCount);
                break;
            case 2:
                displayBooks(books, bookCount);
                break;
            case 3:
                issueBook(books, bookCount, students, studentCount);
                break;
            case 4:
                returnBook(books, bookCount, students, studentCount);
                break;
            case 5:
                addStudent(students, &studentCount);
                break;
            case 6:
                displayStudents(students, studentCount);
                break;
            case 7:
                // Save data and exit
                saveBookData(books, bookCount);
                saveStudentData(students, studentCount);
                printf("Exiting the program.\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }

    } while (choice != 7);

    return 0;
}

void addBook(struct Book books[], int *bookCount) {
    if (*bookCount < MAX_BOOKS) {
        printf("Enter book title: ");
        scanf(" %[^\n]s", books[*bookCount].title); 
        printf("Enter book author: ");
        scanf(" %[^\n]s", books[*bookCount].author);
        printf("Enter quantity of books: ");
        scanf("%d", &books[*bookCount].quantity);
        books[*bookCount].quantityAvailable = books[*bookCount].quantity;
        books[*bookCount].id = *bookCount + 1;
        (*bookCount)++;
        printf("Book added successfully.\n");
    } else {
        printf("Library is full. Cannot add more books.\n");
    }
}

void displayBooks(struct Book books[], int bookCount) {
    if (bookCount == 0) {
        printf("No books available in the library.\n");
    } else {
        printf("\nList of Books:\n");
        printf("ID\tTitle\t\t\t\tAuthor\t\t\t\tQuantity\tAvailable\n");
        printf("------------------------------------------------------------------------------------------------------------------------------\n");
        for (int i = 0; i < bookCount; ++i) {
            printf("%d\t%-20s\t\t%-20s\t\t%d\t\t%d\n", books[i].id, books[i].title, books[i].author, books[i].quantity, books[i].quantityAvailable);
        }
    }
}

void issueBook(struct Book books[], int bookCount, struct Student students[], int studentCount) {
    int studentRollNumber, bookID;

    printf("Enter student roll number: ");
    scanf("%d", &studentRollNumber);

    int studentIndex = -1;
    for (int i = 0; i < studentCount; ++i) {
        if (students[i].rollNumber == studentRollNumber) {
            studentIndex = i;
            break;
        }
    }

    if (studentIndex == -1) {
        printf("Student not found.\n");
        return;
    }

    printf("\nAvailable Books:\n");
    displayBooks(books, bookCount);

    printf("Enter book ID to issue: ");
    scanf("%d", &bookID);

    int bookIndex = -1;
    for (int i = 0; i < bookCount; ++i) {
        if (books[i].id == bookID) {
            bookIndex = i;
            break;
        }
    }

    if (bookIndex == -1) {
        printf("Book not found.\n");
        return;
    }

    if (books[bookIndex].quantityAvailable > 0) {
        books[bookIndex].quantityAvailable--;
        students[studentIndex].borrowedBookID = bookID;
        printf("Book issued successfully.\n");
    } else {
        printf("No available copies of the book.\n");
    }
}

void returnBook(struct Book books[], int bookCount, struct Student students[], int studentCount) {
    int studentRollNumber;

    printf("Enter student roll number: ");
    scanf("%d", &studentRollNumber);

    int studentIndex = -1;
    for (int i = 0; i < studentCount; ++i) {
        if (students[i].rollNumber == studentRollNumber) {
            studentIndex = i;
            break;
        }
    }

    if (studentIndex == -1) {
        printf("Student not found.\n");
        return;
    }

    if (students[studentIndex].borrowedBookID == 0) {
        printf("The student has not borrowed any books.\n");
        return;
    }

    int bookIndex = -1;
    for (int i = 0; i < bookCount; ++i) {
        if (books[i].id == students[studentIndex].borrowedBookID) {
            bookIndex = i;
            break;
        }
    }

    if (bookIndex == -1) {
        printf("Borrowed book not found.\n");
        return;
    }

    books[bookIndex].quantityAvailable++;
    students[studentIndex].borrowedBookID = 0;
    printf("Book returned successfully.\n");
}

void addStudent(struct Student students[], int *studentCount) {
    if (*studentCount < MAX_STUDENTS) {
        printf("Enter student name: ");
        scanf(" %[^\n]s", students[*studentCount].name); 
        printf("Enter student roll number: ");
        scanf("%d", &students[*studentCount].rollNumber);
        students[*studentCount].borrowedBookID = 0;
        (*studentCount)++;
        printf("Student added successfully.\n");
    } else {
        printf("Maximum student limit reached. Cannot add more students.\n");
    }
}

void displayStudents(struct Student students[], int studentCount) {
    if (studentCount == 0) {
        printf("No students available in the system.\n");
    } else {
        printf("\nList of Students:\n");
        printf("Roll Number\tName\t\t\tBorrowed Book ID\n");
        printf("-----------------------------------------------------------------\n");
        for (int i = 0; i < studentCount; ++i) {
            printf("%d\t\t%-20s\t%d\n", students[i].rollNumber, students[i].name, students[i].borrowedBookID);
        }
    }
}

void saveBookData(struct Book books[], int bookCount) {
    FILE *file = fopen(FILENAME_BOOKS, "w");
    if (file == NULL) {
        printf("Error opening book file for writing.\n");
        return;
    }

    for (int i = 0; i < bookCount; ++i) {
        fprintf(file, "%s,%s,%d,%d,%d\n", books[i].title, books[i].author, books[i].id, books[i].quantity, books[i].quantityAvailable);
    }

    fclose(file);
    printf("Book data saved to %s\n", FILENAME_BOOKS);
}

void loadBookData(struct Book books[], int *bookCount) {
    FILE *file = fopen(FILENAME_BOOKS, "r");
    if (file == NULL) {
        printf("Book file not found. Creating a new one.\n");
        return;
    }

    while (fscanf(file, " %99[^,],%49[^,],%d,%d,%d\n",
                  books[*bookCount].title, books[*bookCount].author, &books[*bookCount].id, &books[*bookCount].quantity, &books[*bookCount].quantityAvailable) == 5) {
        (*bookCount)++;
        if (*bookCount >= MAX_BOOKS) {
            printf("Maximum book limit reached. Some data may not be loaded.\n");
            break;
        }
    }

    fclose(file);
    printf("Book data loaded from %s\n", FILENAME_BOOKS);
}

void saveStudentData(struct Student students[], int studentCount) {
    FILE *file = fopen(FILENAME_STUDENTS, "w");
    if (file == NULL) {
        printf("Error opening student file for writing.\n");
        return;
    }

    for (int i = 0; i < studentCount; ++i) {
        fprintf(file, "%s,%d,%d\n", students[i].name, students[i].rollNumber, students[i].borrowedBookID);
    }

    fclose(file);
    printf("Student data saved to %s\n", FILENAME_STUDENTS);
}

void loadStudentData(struct Student students[], int *studentCount) {
    FILE *file = fopen(FILENAME_STUDENTS, "r");
    if (file == NULL) {
        printf("Student file not found. Creating a new one.\n");
        return;
    }

    while (fscanf(file, " %49[^,],%d,%d\n",
                  students[*studentCount].name, &students[*studentCount].rollNumber, &students[*studentCount].borrowedBookID) == 3) {
        (*studentCount)++;
        if (*studentCount >= MAX_STUDENTS) {
            printf("Maximum student limit reached. Some data may not be loaded.\n");
            break;
        }
    }

    fclose(file);
    printf("Student data loaded from %s\n", FILENAME_STUDENTS);
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ISBN_LENGTH 20
#define MAX_STUDENT_ID_LENGTH 10
#define MAX_RETURN_DATE_LENGTH 11
#define MAX_TITLE_LENGTH 100
#define MAX_AUTHOR_LENGTH 100
#define MAX_NAME_LENGTH 100
#define MAX_STUDENT_ID_LENGTH 20
#define MAX_DUE_DATE_LENGTH 30
#define MAX_ISSUED_BOOKS 10

struct Book {
    char title[MAX_TITLE_LENGTH];
    char author[MAX_AUTHOR_LENGTH];
    char isbn[MAX_ISBN_LENGTH];
    int total_count;
    int available_count;
};

struct StudentBook {
    char isbn[MAX_ISBN_LENGTH];
    char student_id[MAX_STUDENT_ID_LENGTH];
    char due_date[MAX_RETURN_DATE_LENGTH];
};
struct Student {
    char student_id[MAX_STUDENT_ID_LENGTH];
    char last_name[MAX_NAME_LENGTH];
    char first_name[MAX_NAME_LENGTH];
    char middle_name[MAX_NAME_LENGTH];
    char faculty[MAX_NAME_LENGTH];
    char major[MAX_NAME_LENGTH];
    struct StudentBook issued_books[MAX_ISSUED_BOOKS];
    int num_books_issued;
};

void display_menu() {
    printf("========== Library Management System ==========\n");
    printf("1. Add Book\n");
    printf("2. Remove Book\n");
    printf("3. View Book\n");
    printf("4. View All Books\n");
    printf("5. Add Student\n");
    printf("6. Remove Student\n");
    printf("7. View Student\n");
    printf("8. View All Students\n");
    printf("9. Issue Book\n");
    printf("(Not available) 10. Return Book\n");
    printf("(Not available) 11. View Book's Students\n");
    printf("(Not available) 12. View Student's Books\n");
    printf("0. Exit\n");
    printf("Enter your choice: \n");
}
void clear_stdin() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

int read_line(char *buffer, int max_length) {
    if (fgets(buffer, max_length, stdin) != NULL) {
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
            return 1;
        }
    }
    return 0;
}

int book_exists(const struct Book *books, int num_books, const char *isbn) {
    for (int i = 0; i < num_books; i++) {
        if (strcmp(books[i].isbn, isbn) == 0) {
            return 1;
        }
    }
    return 0;
}

void add_book(struct Book *books, int *num_books) {
    struct Book new_book;
    printf("Enter book ISBN: \n");
    read_line(new_book.isbn, MAX_ISBN_LENGTH);

    if (book_exists(books, *num_books, new_book.isbn)) {
        printf("Error: A book with the same ISBN already exists.\n");
        return;
    }

    printf("Enter book author: \n");
    read_line(new_book.author, MAX_AUTHOR_LENGTH);
    printf("Enter book title: \n");
    read_line(new_book.title, MAX_TITLE_LENGTH);

    printf("Enter total count of books: \n");
    scanf("%d", &new_book.total_count);
    printf("Enter available count of books: \n");
    scanf("%d", &new_book.available_count);
    clear_stdin();

    books[*num_books] = new_book;
    (*num_books)++;
    printf("Book added successfully.\n");
}

void remove_book(struct Book *books, int *num_books) {
    char isbn[MAX_ISBN_LENGTH];
    printf("Enter the ISBN of the book to remove: ");
    read_line(isbn, MAX_ISBN_LENGTH);

    for (int i = 0; i < *num_books; i++) {
        if (strcmp(books[i].isbn, isbn) == 0) {
            // Check if total count and available count match
            if (books[i].total_count != books[i].available_count) {
                printf("Error: Cannot remove the book because total count and available count do not match.\n");
                return;
            }

            for (int j = i; j < *num_books - 1; j++) {
                books[j] = books[j + 1];
            }
            (*num_books)--;
            printf("Book removed successfully.\n");
            return;
        }
    }

    printf("Error: Book not found.\n");
}


void view_book(const struct Book *books, int num_books) {
    char isbn[MAX_ISBN_LENGTH];
    printf("Enter the ISBN of the book to view: \n");
    read_line(isbn, MAX_ISBN_LENGTH);

    for (int i = 0; i < num_books; i++) {
        if (strcmp(books[i].isbn, isbn) == 0) {
            printf("Title: %s\n", books[i].title);
            printf("Author: %s\n", books[i].author);
            printf("ISBN: %s\n", books[i].isbn);
            printf("Total Count: %d\n", books[i].total_count);
            printf("Available Count: %d\n", books[i].available_count);
            return;
        }
    }

    printf("Error: Book not found.\n");
}

int compare_isbn(const void *a, const void *b) {
    const struct Book *book_a = (const struct Book *)a;
    const struct Book *book_b = (const struct Book *)b;
    return strcmp(book_a->isbn, book_b->isbn);
}

void print_books(const struct Book *books, int num_books) {
    if (num_books == 0) {
        printf("No books found.\n");
        return;
    }

    printf("All books:\n");
    printf("-----------------------------------------------------------------------------------------------------------------\n");
    printf("| %-20s | %-20s | %-25s | %-14s | %-18s |\n", "ISBN", "Author", "Title", "Total Count", "Available Count");
    printf("-----------------------------------------------------------------------------------------------------------------\n");

    struct Book *sorted_books = malloc(num_books * sizeof(struct Book));
    if (sorted_books == NULL) {
        printf("Memory allocation failed.\n");
        return;
    }

    memcpy(sorted_books, books, num_books * sizeof(struct Book));
    qsort(sorted_books, num_books, sizeof(struct Book), compare_isbn);

    for (int i = 0; i < num_books; i++) {
        printf("| %-20s | %-20s | %-25s | %-14d | %-18d |\n",
               sorted_books[i].isbn, sorted_books[i].author, sorted_books[i].title,
               sorted_books[i].total_count, sorted_books[i].available_count);
    }

    printf("-----------------------------------------------------------------------------------------------------------------\n");

    free(sorted_books);
}
int student_exists(const struct Student *students, int num_students, const char *student_id) {
    for (int i = 0; i < num_students; i++) {
        if (strcmp(students[i].student_id, student_id) == 0) {
            return 1;
        }
    }
    return 0;
}

void add_student(struct Student *students, int *num_students) {
    struct Student new_student;
    printf("Enter student ID: \n");
    read_line(new_student.student_id, MAX_STUDENT_ID_LENGTH);

    if (student_exists(students, *num_students, new_student.student_id)) {
        printf("Error: A student with the same student ID already exists.\n");
        return;
    }

    printf("Enter last name: \n");
    read_line(new_student.last_name, MAX_NAME_LENGTH);
    printf("Enter first name: \n");
    read_line(new_student.first_name, MAX_NAME_LENGTH);
    printf("Enter middle name: \n");
    read_line(new_student.middle_name, MAX_NAME_LENGTH);
    printf("Enter faculty: \n");
    read_line(new_student.faculty, MAX_NAME_LENGTH);
    printf("Enter major: \n");
    read_line(new_student.major, MAX_NAME_LENGTH);

    students[*num_students] = new_student;
    (*num_students)++;
    printf("Student added successfully.\n");
}


int student_has_books(const char *student_id) {
    FILE *file = fopen("C:\\Users\\reeds\\CLionProjects\\SEM\\student_books.csv", "r");
    if (file != NULL) {
        char line[100];
        while (fgets(line, sizeof(line), file) != NULL) {
            char stored_student_id[MAX_STUDENT_ID_LENGTH];
            sscanf(line, "%*[^,],%[^,]", stored_student_id);
            if (strcmp(stored_student_id, student_id) == 0) {
                fclose(file);
                return 1;
            }
        }
        fclose(file);
    }
    return 0;
}

void remove_student(struct Student *students, int *num_students) {
    char student_id[MAX_STUDENT_ID_LENGTH];
    printf("Enter the student ID of the student to remove: \n");
    fgets(student_id, MAX_STUDENT_ID_LENGTH, stdin);
    student_id[strcspn(student_id, "\n")] = '\0';

    if (student_has_books(student_id)) {
        printf("Error: Cannot remove the student because they have books borrowed.\n");
        return;
    }

    for (int i = 0; i < *num_students; i++) {
        if (strcmp(students[i].student_id, student_id) == 0) {
            for (int j = i; j < *num_students - 1; j++) {
                students[j] = students[j + 1];
            }
            (*num_students)--;
            printf("Student removed successfully.\n");
            return;
        }
    }

    printf("Error: Student not found.\n");
}

void edit_student(struct Student *students, int num_students) {
    char student_id[MAX_STUDENT_ID_LENGTH];
    printf("Enter the student ID of the student to edit: \n");
    read_line(student_id, MAX_STUDENT_ID_LENGTH);

    for (int i = 0; i < num_students; i++) {
        if (strcmp(students[i].student_id, student_id) == 0) {
            printf("Enter new last name: \n");
            read_line(students[i].last_name, MAX_NAME_LENGTH);
            printf("Enter new first name: \n");
            read_line(students[i].first_name, MAX_NAME_LENGTH);
            printf("Enter new middle name: \n");
            read_line(students[i].middle_name, MAX_NAME_LENGTH);
            printf("Enter new faculty: \n");
            read_line(students[i].faculty, MAX_NAME_LENGTH);
            printf("Enter new major: \n");
            read_line(students[i].major, MAX_NAME_LENGTH);
            printf("Student information updated successfully.\n");
            return;
        }
    }

    printf("Error: Student not found.\n");
}

void view_student(const struct Student *students, int num_students) {
    char student_id[MAX_STUDENT_ID_LENGTH];
    printf("Enter the student ID of the student to view: \n");
    read_line(student_id, MAX_STUDENT_ID_LENGTH);

    for (int i = 0; i < num_students; i++) {
        if (strcmp(students[i].student_id, student_id) == 0) {
            printf("Student Information:\n");
            printf("Student ID: %s\n", students[i].student_id);
            printf("Last Name: %s\n", students[i].last_name);
            printf("First Name: %s\n", students[i].first_name);
            printf("Middle Name: %s\n", students[i].middle_name);
            printf("Faculty: %s\n", students[i].faculty);
            printf("Major: %s\n", students[i].major);
            return;
        }
    }

    printf("Error: Student not found.\n");
}
void view_all_students(const struct Student *students, int num_students) {
    printf("All Students:\n");

    // Iterate through the students array
    for (int i = 0; i < num_students; i++) {
        printf("Student ID: %s\n", students[i].student_id);
        printf("Last Name: %s\n", students[i].last_name);
        printf("First Name: %s\n", students[i].first_name);
        printf("Middle Name: %s\n", students[i].middle_name);
        printf("Faculty: %s\n", students[i].faculty);
        printf("Major: %s\n", students[i].major);
        printf("----------------------\n");
    }

    printf("End of list.\n");
}


void issue_book(struct Book *books, int num_books, struct StudentBook *student_books, int *num_student_books) {
    char isbn[MAX_ISBN_LENGTH];
    char student_id[MAX_STUDENT_ID_LENGTH];
    char due_date[MAX_DUE_DATE_LENGTH];

    printf("Enter the ISBN of the book to issue: \n");
    fgets(isbn, MAX_ISBN_LENGTH, stdin);
    isbn[strcspn(isbn, "\n")] = '\0';

    printf("Enter the student ID: \n");
    fgets(student_id, MAX_STUDENT_ID_LENGTH, stdin);
    student_id[strcspn(student_id, "\n")] = '\0';

    printf("Enter the due date (YYYY-MM-DD): \n");
    fgets(due_date, MAX_DUE_DATE_LENGTH, stdin);
    due_date[strcspn(due_date, "\n")] = '\0';

    int book_index = -1;
    for (int i = 0; i < num_books; i++) {
        if (strcmp(books[i].isbn, isbn) == 0) {
            book_index = i;
            break;
        }
    }

    int student_book_index = -1;
    for (int i = 0; i < *num_student_books; i++) {
        if (strcmp(student_books[i].student_id, student_id) == 0 && strcmp(student_books[i].isbn, isbn) == 0) {
            student_book_index = i;
            break;
        }
    }

    if (book_index == -1 || books[book_index].available_count <= 0) {
        printf("Error: The book is not available for issuance.\n");
        return;
    }

    books[book_index].available_count--;
    if (student_book_index == -1) {
        strcpy(student_books[*num_student_books].student_id, student_id);
        strcpy(student_books[*num_student_books].isbn, isbn);
        strcpy(student_books[*num_student_books].due_date, due_date);
        (*num_student_books)++;
    } else {
        strcpy(student_books[student_book_index].due_date, due_date);
    }

    printf("Book issued successfully.\n");

    FILE *file = fopen("C:\\Users\\reeds\\CLionProjects\\SEM\\student_books.csv", "a");
    if (file == NULL) {
        printf("Error: Unable to open the file.\n");
        return;
    }

    fprintf(file, "%s,%s,%s\n", isbn, student_id, due_date);

    fclose(file);
}

/*void return_book(struct Book *books, int num_books, struct StudentBook *student_books, int *num_student_books) {
    char isbn[MAX_ISBN_LENGTH];
    char student_id[MAX_STUDENT_ID_LENGTH];
    char due_date[MAX_DUE_DATE_LENGTH];

    printf("Enter the ISBN of the book to return: \n");
    fgets(isbn, MAX_ISBN_LENGTH, stdin);
    isbn[strcspn(isbn, "\n")] = '\0';

    printf("Enter the student ID: \n");
    fgets(student_id, MAX_STUDENT_ID_LENGTH, stdin);
    student_id[strcspn(student_id, "\n")] = '\0';

    int book_index = -1;
    for (int i = 0; i < num_books; i++) {
        if (strcmp(books[i].isbn, isbn) == 0) {
            book_index = i;
            break;
        }
    }

    int student_book_index = -1;
    for (int i = 0; i < *num_student_books; i++) {
        if (strcmp(student_books[i].student_id, student_id) == 0 && strcmp(student_books[i].isbn, isbn) == 0) {
            student_book_index = i;
            break;
        }
    }

    if (book_index == -1) {
        printf("Error: The book with ISBN %s does not exist.\n", isbn);
        return;
    }

    if (student_book_index == -1) {
        printf("Error: The book with ISBN %s is not issued to student ID %s.\n", isbn, student_id);
        return;
    }

    books[book_index].available_count++;

    // Remove the student book record
    for (int i = student_book_index; i < (*num_student_books - 1); i++) {
        strcpy(student_books[i].student_id, student_books[i + 1].student_id);
        strcpy(student_books[i].isbn, student_books[i + 1].isbn);
        strcpy(student_books[i].due_date, student_books[i + 1].due_date);
    }

    (*num_student_books)--;

    printf("Book returned successfully.\n");

    FILE *file = fopen("C:\\Users\\reeds\\CLionProjects\\SEM\\student_books.csv", "w");
    if (file == NULL) {
        printf("Error: Unable to open the file.\n");
        return;
    }

    // Rewrite the student_books.csv file with updated records
    for (int i = 0; i < *num_student_books; i++) {
        fprintf(file, "%s,%s,%s\n", student_books[i].isbn, student_books[i].student_id, student_books[i].due_date);
    }

    fclose(file);
}*/

/*void view_student_books(const struct StudentBook *student_books, int num_student_books, const char *student_id, const struct Book *books, int num_books) {
    printf("Books issued to student with ID %s:\n", student_id);
    read_line(student_id,MAX_STUDENT_ID_LENGTH);

    int books_found = 0;

    for (int i = 0; i < num_student_books; i++) {
        if (strcmp(student_books[i].student_id, student_id) == 0) {
            // Find the corresponding book in the book array
            int book_index = -1;
            for (int j = 0; j < num_books; j++) {
                if (strcmp(student_books[i].isbn, books[j].isbn) == 0) {
                    book_index = j;
                    break;
                }
            }

            if (book_index != -1) {
                printf("Title: %s\n", books[book_index].title);
                printf("Author: %s\n", books[book_index].author);
                printf("ISBN: %s\n", books[book_index].isbn);
                printf("Due Date: %s\n", student_books[i].due_date);
                printf("----------------------\n");
                books_found = 1;
            }
        }
    }

    if (!books_found) {
        printf("No books issued to the student.\n");
    }
}*/

/*void view_book_students(const struct StudentBook *student_books, int num_student_books, const char *isbn, const struct Student *students, int num_students) {
    printf("Students who have borrowed the book with ISBN %s:\n", isbn);

    int students_found = 0;

    for (int i = 0; i < num_student_books; i++) {
        if (strcmp(student_books[i].isbn, isbn) == 0) {
            int student_index = -1;
            for (int j = 0; j < num_students; j++) {
                if (strcmp(student_books[i].student_id, students[j].student_id) == 0) {
                    student_index = j;
                    break;
                }
            }

            if (student_index != -1) {
                printf("Student ID: %s\n", students[student_index].student_id);
                printf("Last Name: %s\n", students[student_index].last_name);
                printf("First Name: %s\n", students[student_index].first_name);
                printf("Middle Name: %s\n", students[student_index].middle_name);
                printf("----------------------\n");
                students_found = 1;
            }
        }
    }

    if (!students_found) {
        printf("No students have borrowed the book.\n");
    }
}*/
int main() {
    struct Book books[100];
    int num_books = 0;

    struct Student students[100];
    int num_students = 0;

    struct StudentBook student_books[100];
    int num_student_books = 0;
    FILE *file_books = fopen("C:\\Users\\reeds\\CLionProjects\\SEM\\books.csv", "r");
    if (file_books != NULL) {
        char line[256];
        while (fgets(line, sizeof(line), file_books) != NULL) {
            if (num_books >= sizeof(books) / sizeof(books[0])) {
                printf("Warning: Maximum number of books reached. Skipping the remaining lines.\n");
                break;
            }
            char *token = strtok(line, ",");
            if (token != NULL) {
                strncpy(books[num_books].isbn, token, MAX_ISBN_LENGTH);
                token = strtok(NULL, ",");
                if (token != NULL) {
                    strncpy(books[num_books].author, token, MAX_AUTHOR_LENGTH);
                    token = strtok(NULL, ",");
                    if (token != NULL) {
                        strncpy(books[num_books].title, token, MAX_TITLE_LENGTH);
                        token = strtok(NULL, ",");
                        if (token != NULL) {
                            books[num_books].total_count = atoi(token);
                            token = strtok(NULL, ",");
                            if (token != NULL) {
                                books[num_books].available_count = atoi(token);
                                num_books++;
                            }
                        }
                    }
                }
            }
        }
        fclose(file_books);
    } else {
        printf("Error: Failed to open file_books.\n");
    }
    FILE *file_stud = fopen("C:\\Users\\reeds\\CLionProjects\\SEM\\students.csv", "r");
    if (file_stud) {
        char line[256];
        while (fgets(line, sizeof(line), file_stud)) {
            char *token = strtok(line, ",");
            strcpy(students[num_students].student_id, token);

            token = strtok(NULL, ",");
            strcpy(students[num_students].last_name, token);

            token = strtok(NULL, ",");
            strcpy(students[num_students].first_name, token);

            token = strtok(NULL, ",");
            strcpy(students[num_students].middle_name, token);

            token = strtok(NULL, ",");
            strcpy(students[num_students].faculty, token);

            token = strtok(NULL, "\n");
            strcpy(students[num_students].major, token);

            num_students++;
        }
        fclose(file_stud);
    }
    FILE *file_stud_book = fopen("C:\\Users\\reeds\\CLionProjects\\SEM\\student_books.csv", "r");
    if (file_stud_book) {
        char line[256];
        while (fgets(line, sizeof(line), file_stud_book)) {
            char *token = strtok(line, ",");
            strcpy(student_books[num_students].isbn, token);

            token = strtok(NULL, ",");
            strcpy(student_books[num_students].student_id, token);

            token = strtok(NULL, ",");
            strcpy(student_books[num_students].due_date, token);

            num_student_books++;
        }
        fclose(file_stud_book);
    }
    int choice;
    do {
        display_menu();
        scanf("%d", &choice);
        getchar();  // Read the newline character

        switch (choice) {
            case 1:
                add_book(books, &num_books);
                break;
            case 2:
                remove_book(books, &num_books);
                break;
            case 3:
                view_book(books, num_books);
                break;
            case 4:
                print_books(books, num_books);
                break;
            case 5:
                add_student(students, &num_students);
                break;
            case 6:
                remove_student(students, &num_students);
                break;
            case 7:
                view_student(students, num_students);
                break;
            case 8:
                view_all_students(students, num_students);
                break;
            case 9:
                issue_book(books, num_books, student_books, &num_student_books);
                break;
            case 10:
                //return_book(books, num_books, student_books, &num_student_books);
                break;
            case 11:
            {
                char isbn[MAX_ISBN_LENGTH];
                printf("Enter the ISBN of the book: \n");
                read_line(isbn, MAX_ISBN_LENGTH);
                //view_book_students(student_books, num_student_books, isbn, students, num_students);
            }
                break;

            case 12:
            {
                char student_id[MAX_STUDENT_ID_LENGTH];
                printf("Enter the student ID: \n");
                read_line(student_id, MAX_STUDENT_ID_LENGTH);
                //view_student_books(student_books, num_student_books, student_id, books, num_books);
            }
                break;

            case 0:
                printf("Exiting...\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
                break;
        }
    } while (choice != 0);

    return 0;
}






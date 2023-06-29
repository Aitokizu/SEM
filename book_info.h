/*Напишите программу на C, которая считывает файл csv с данными по книгам и выводит меню с доступными операциями:
1. Добавить новую книгу (при добавлении проверять нет ли уже такой книги по номеру ISBN)
2. Удалить книгу по номеру ISBN
3. Просмотр всей информации по книге (здесь и далее книга выбирается по номеру ISBN, если введен неправильный номер выводится сообщение о том, что книга не найдена)
4. Вывести информацию по всем книгам в виде таблицы, записи должны быть отсортированы по номеру ISBN*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TITLE_LENGTH 100
#define MAX_AUTHOR_LENGTH 100
#define MAX_ISBN_LENGTH 20

struct Book {
    char title[MAX_TITLE_LENGTH];
    char author[MAX_AUTHOR_LENGTH];
    char isbn[MAX_ISBN_LENGTH];
    int total_count;
    int available_count;
};

void print_menu() {
    printf("Menu:\n");
    printf("1. Add a new book\n");
    printf("2. Remove a book by ISBN\n");
    printf("3. View information for a book\n");
    printf("4. Print all books\n");
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
    printf("Enter the ISBN of the book to remove: \n");
    read_line(isbn, MAX_ISBN_LENGTH);

    for (int i = 0; i < *num_books; i++) {
        if (strcmp(books[i].isbn, isbn) == 0) {
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

int main() {
    struct Book books[100];
    int num_books = 0;

    FILE *file = fopen("C:\\Users\\reeds\\CLionProjects\\SEM\\books.csv", "r");
    if (file != NULL) {
        char line[256];
        while (fgets(line, sizeof(line), file) != NULL) {
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
        fclose(file);
    } else {
        printf("Error: Failed to open file.\n");
    }

    int choice;
    do {
        print_menu();
        scanf("%d", &choice);
        clear_stdin();

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
            case 0:
                printf("Exiting...\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
                break;
        }
    } while (choice != 0);

    file = fopen("C:\\Users\\reeds\\CLionProjects\\SEM\\books.csv", "w");
    if (file != NULL) {
        for (int i = 0; i < num_books; i++) {
            fprintf(file, "%s,%s,%s,%d,%d\n", books[i].isbn, books[i].author, books[i].title,
                    books[i].total_count, books[i].available_count);
        }
        fclose(file);
    } else {
        printf("Error: Unable to write to file.\n");
    }

    return 0;
}
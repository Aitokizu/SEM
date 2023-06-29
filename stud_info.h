#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME_LENGTH 100
#define MAX_STUDENT_ID_LENGTH 20

struct Student {
    char student_id[MAX_STUDENT_ID_LENGTH];
    char last_name[MAX_NAME_LENGTH];
    char first_name[MAX_NAME_LENGTH];
    char middle_name[MAX_NAME_LENGTH];
    char faculty[MAX_NAME_LENGTH];
    char major[MAX_NAME_LENGTH];
};

void print_menu() {
    printf("Menu:\n");
    printf("1. Add a new student\n");
    printf("2. Remove a student by student ID\n");
    printf("3. Edit student information\n");
    printf("4. View student information by student ID\n");
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

void remove_student(struct Student *students, int *num_students) {
    char student_id[MAX_STUDENT_ID_LENGTH];
    printf("Enter the student ID of the student to remove: \n");
    read_line(student_id, MAX_STUDENT_ID_LENGTH);

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

int main() {
    struct Student students[100];
    int num_students = 0;

    FILE *file = fopen("C:\\Users\\reeds\\CLionProjects\\SEM\\students.csv", "r");
    if (file) {
        char line[256];
        while (fgets(line, sizeof(line), file)) {
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
        fclose(file);
    }

    int choice;
    do {
        print_menu();
        scanf("%d", &choice);
        clear_stdin();

        switch (choice) {
            case 1:
                add_student(students, &num_students);
                break;
            case 2:
                remove_student(students, &num_students);
                break;
            case 3:
                edit_student(students, num_students);
                break;
            case 4:
                view_student(students, num_students);
                break;
            case 0:
                printf("Exiting program.\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
                break;
        }
    } while (choice != 0);

    file = fopen("C:\\Users\\reeds\\CLionProjects\\SEM\\students.csv", "w");
    if (file) {
        for (int i = 0; i < num_students; i++) {
            fprintf(file, "%s,%s,%s,%s,%s,%s\n", students[i].student_id, students[i].last_name,
                    students[i].first_name, students[i].middle_name, students[i].faculty, students[i].major);
        }
        fclose(file);
    }

    return 0;
}

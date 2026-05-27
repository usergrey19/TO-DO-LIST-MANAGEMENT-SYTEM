#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>

#define MAX_TASKS 200

struct User {
    char username[50];
    char password[50];
};

struct Task {
    int id;
    char name[100];
    char category[20];
    char deadline[20];
    char priority[20];
    int completed;
};

struct Task tasks[MAX_TASKS];
int count = 0;

char currentUser[50];
char taskFile[100];

// ================= PASSWORD MASK =================
void inputPassword(char pass[]) {
    int i = 0;
    char ch;

    while ((ch = getch()) != 13) {
        if (ch == 8 && i > 0) {
            i--;
            printf("\b \b");
        }
        else {
            pass[i++] = ch;
            printf("*");
        }
    }

    pass[i] = '\0';
}

// ================= FILES =================
void updateTaskFilename() {
    sprintf(taskFile, "%s_tasks.dat", currentUser);
}

void saveTasks() {
    FILE *fp = fopen(taskFile, "wb");

    if (!fp)
        return;

    fwrite(&count, sizeof(int), 1, fp);
    fwrite(tasks, sizeof(struct Task), count, fp);

    fclose(fp);
}

void loadTasks() {
    FILE *fp = fopen(taskFile, "rb");

    if (!fp) {
        count = 0;
        return;
    }

    fread(&count, sizeof(int), 1, fp);
    fread(tasks, sizeof(struct Task), count, fp);

    fclose(fp);
}

// ================= AUTH =================
void signup() {

    struct User u;

    FILE *fp = fopen("users.dat", "ab");

    printf("\nCreate Username: ");
    scanf("%s", u.username);

    printf("Create Password: ");
    inputPassword(u.password);

    fwrite(&u, sizeof(u), 1, fp);

    fclose(fp);

    printf("\nAccount Created.\n");
}

int login() {

    struct User u;

    char username[50];
    char password[50];

    FILE *fp = fopen("users.dat", "rb");

    if (!fp) {
        printf("No users exist.\n");
        return 0;
    }

    printf("\nUsername: ");
    scanf("%s", username);

    printf("Password: ");
    inputPassword(password);

    while (fread(&u, sizeof(u), 1, fp)) {

        if (
            strcmp(username, u.username) == 0 &&
            strcmp(password, u.password) == 0
        ) {

            strcpy(currentUser, username);

            fclose(fp);

            updateTaskFilename();

            printf("\nLogin Successful.\n");

            return 1;
        }
    }

    fclose(fp);

    printf("\nInvalid Credentials.\n");

    return 0;
}

// ================= TASKS =================
void addTask() {

    tasks[count].id = count + 1;

    printf("\nTask: ");
    scanf(" %[^\n]", tasks[count].name);

    printf("Category: ");
    scanf("%s", tasks[count].category);

    printf("Deadline: ");
    scanf("%s", tasks[count].deadline);

    printf("Priority: ");
    scanf("%s", tasks[count].priority);

    tasks[count].completed = 0;

    count++;

    saveTasks();

    printf("Task Added.\n");
}

void viewTasks() {

    if (count == 0) {
        printf("\nNo Tasks.\n");
        return;
    }

    for (int i = 0; i < count; i++) {

        printf(
            "\nID:%d\nTask:%s\nCategory:%s\nDeadline:%s\nPriority:%s\nStatus:%s\n",
            tasks[i].id,
            tasks[i].name,
            tasks[i].category,
            tasks[i].deadline,
            tasks[i].priority,
            tasks[i].completed ? "Done" : "Pending"
        );
    }
}

void completeTask() {

    int id;

    printf("Task ID: ");
    scanf("%d", &id);

    for (int i = 0; i < count; i++) {

        if (tasks[i].id == id) {

            tasks[i].completed = 1;

            saveTasks();

            printf("Completed.\n");

            return;
        }
    }

    printf("Not Found.\n");
}

void deleteTask() {

    int id;

    printf("Delete ID: ");
    scanf("%d", &id);

    for (int i = 0; i < count; i++) {

        if (tasks[i].id == id) {

            for (int j = i; j < count - 1; j++)
                tasks[j] = tasks[j + 1];

            count--;

            saveTasks();

            printf("Deleted.\n");

            return;
        }
    }

    printf("Task not found.\n");
}

void searchTask() {

    char key[100];

    printf("Search: ");
    scanf(" %[^\n]", key);

    for (int i = 0; i < count; i++) {

        if (strstr(tasks[i].name, key)) {

            printf(
                "\n%d | %s | %s",
                tasks[i].id,
                tasks[i].name,
                tasks[i].deadline
            );
        }
    }
}

void sortDeadline() {

    struct Task temp;

    for (int i = 0; i < count - 1; i++) {

        for (int j = i + 1; j < count; j++) {

            if (
                strcmp(
                    tasks[i].deadline,
                    tasks[j].deadline
                ) > 0
            ) {

                temp = tasks[i];
                tasks[i] = tasks[j];
                tasks[j] = temp;
            }
        }
    }

    saveTasks();

    printf("Sorted.\n");
}

void efficiency() {

    int done = 0;

    for (int i = 0; i < count; i++)
        if (tasks[i].completed)
            done++;

    if (count == 0)
        printf("No Tasks\n");

    else
        printf(
            "\nEfficiency: %.2f%%\n",
            done * 100.0 / count
        );
}

// ================= MAIN =================
int main() {

    int ch;

    printf("1 Signup\n2 Login\nChoice: ");
    scanf("%d", &ch);

    if (ch == 1)
        signup();

    if (!login())
        return 0;

    loadTasks();

    while (1) {

        printf(
            "\n===== TODO =====\n"
            "1 Add\n"
            "2 View\n"
            "3 Complete\n"
            "4 Delete\n"
            "5 Search\n"
            "6 Efficiency\n"
            "7 Sort Deadline\n"
            "8 Exit\n"
        );

        scanf("%d", &ch);

        switch (ch) {

            case 1: addTask(); break;
            case 2: viewTasks(); break;
            case 3: completeTask(); break;
            case 4: deleteTask(); break;
            case 5: searchTask(); break;
            case 6: efficiency(); break;
            case 7: sortDeadline(); break;
            case 8: return 0;

            default:
                printf("Invalid.\n");
        }
    }
}
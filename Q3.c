#include <stdio.h>
#include <string.h>

#define MAX_NODES 200

typedef struct {
    int empID;
    char fullName[50];
    char role[50];
    double compensation;
    int next;
} EmployeeNode;

EmployeeNode list[MAX_NODES];
int head = -1;
int used = 0;

int newNode() {
    if (used >= MAX_NODES) return -1;
    return used++;
}

void addToEnd(int nodeIndex) {
    if (head == -1) {
        head = nodeIndex;
        return;
    }
    int c = head;
    while (list[c].next != -1) {
        c = list[c].next;
    }
    list[c].next = nodeIndex;
}

int findEmployee(char *query, int isID) {
    int c = head;
    while (c != -1) {
        if (isID) {
            int searchID = 0;
            sscanf(query, "%d", &searchID);
            if (list[c].empID == searchID) return c;
        } else {
            if (strcmp(list[c].fullName, query) == 0) return c;
        }
        c = list[c].next;
    }
    return -1;
}

int findTopEarner() {
    if (head == -1) return -1;
    int c = head, m = head;
    while (c != -1) {
        if (list[c].compensation > list[m].compensation) {
            m = c;
        }
        c = list[c].next;
    }
    return m;
}

void showAll() {
    if (head == -1) {
        printf("\nNo records found.\n");
        return;
    }

    printf("\n%-5s %-20s %-20s %-10s\n", "ID", "Name", "Role", "Salary");
    printf("------------------------------------------------------------\n");

    int c = head;
    while (c != -1) {
        printf("%-5d %-20s %-20s %-10.2f\n",
               list[c].empID,
               list[c].fullName,
               list[c].role,
               list[c].compensation);
        c = list[c].next;
    }
    printf("------------------------------------------------------------\n");
}

void adjustCompensation(char *role, double raiseAmount) {
    int c = head;
    int count = 0;
    while (c != -1) {
        if (strcmp(list[c].role, role) == 0) {
            list[c].compensation += raiseAmount;
            count++;
        }
        c = list[c].next;
    }
    printf("\nUpdated salary for %d employee(s) with role '%s'.\n", count, role);
}

int countEmployees() {
    int c = head, count = 0;
    while (c != -1) {
        count++;
        c = list[c].next;
    }
    return count;
}

int main() {
    int count;
    printf("Initialize System - How many employees to add? ");
    scanf("%d", &count);

    for (int i = 0; i < count; i++) {
        int id;
        char name[50], role[50];
        double salary;
        int nodeIndex;

        printf("\n--- Entry %d ---\n", i + 1);
        printf("ID: ");
        scanf("%d", &id);

        printf("Name: ");
        scanf(" %[^\n]s", name);

        printf("Role: ");
        scanf(" %[^\n]s", role);

        printf("Salary: ");
        scanf("%lf", &salary);

        nodeIndex = newNode();
        if (nodeIndex != -1) {
            list[nodeIndex].empID = id;
            strcpy(list[nodeIndex].fullName, name);
            strcpy(list[nodeIndex].role, role);
            list[nodeIndex].compensation = salary;
            list[nodeIndex].next = -1;
            addToEnd(nodeIndex);
        }
    }

    int option;
    
    while (1) {
        printf("\n=== HR System Menu ===\n");
        printf("1. Show All Employees\n");
        printf("2. Search Employee (ID)\n");
        printf("3. Search Employee (Name)\n");
        printf("4. Find Top Earner\n");
        printf("5. Give Raise by Role\n");
        printf("6. Count Employees\n");
        printf("7. Exit\n");
        printf("Choice: ");
        
        scanf("%d", &option);

        if (option == 1) {
            showAll();
        }
        else if (option == 2) {
            int id;
            char idStr[20];
            printf("Enter ID: ");
            scanf("%d", &id);
            sprintf(idStr, "%d", id);
            
            int r = findEmployee(idStr, 1);
            if (r == -1) printf(">> Not found.\n");
            else printf(">> Found: %s (%s)\n", list[r].fullName, list[r].role);
        }
        else if (option == 3) {
            char name[50];
            printf("Enter Name: ");
            scanf(" %[^\n]s", name);
            
            int r = findEmployee(name, 0);
            if (r == -1) printf(">> Not found.\n");
            else printf(">> Found: ID %d - %s ($%.2f)\n", list[r].empID, list[r].fullName, list[r].compensation);
        }
        else if (option == 4) {
            int t = findTopEarner();
            if (t == -1) printf("No records.\n");
            else printf("Top Earner: %s ($%.2f)\n", list[t].fullName, list[t].compensation);
        }
        else if (option == 5) {
            char role[50];
            double raise;
            printf("Enter Role to Reward: ");
            scanf(" %[^\n]s", role);
            printf("Enter Raise Amount: ");
            scanf("%lf", &raise);
            adjustCompensation(role, raise);
        }
        else if (option == 6) {
            printf("Total Employees: %d\n", countEmployees());
        }
        else if (option == 7) {
            printf("Exiting.\n");
            break;
        }
        else {
            printf("Invalid Option.\n");
        }
    }

    return 0;
}

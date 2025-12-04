#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DATABASE_FILE "members.dat"

typedef struct {
    int id;
    char name[100];
    char batch[20];
    char registrationDate[11];
    char birthDate[11];
    int interest; // 1=IEEE, 2=ACM, 3=Both
} MemberRecord;
void stripNewline(char *str) {
    str[strcspn(str, "\n")] = 0;
}

void getStringInput(const char *prompt, char *buffer, int size) {
    printf("%s", prompt);
    if (fgets(buffer, size, stdin)) stripNewline(buffer);
}

int getIntInput(const char *prompt, int min, int max) {
    int value;
    while (1) {
        printf("%s", prompt);
        if (scanf("%d", &value) == 1 && value >= min && value <= max) {
            while (getchar() != '\n'); // flush
            return value;
        }
        printf("Invalid input. Please try again.\n");
        while (getchar() != '\n');
    }
}
typedef struct {
    MemberRecord **records;
    int count;
} MemberDB;

MemberDB* createDatabase() {
    MemberDB *db = malloc(sizeof(MemberDB));
    db->records = NULL;
    db->count = 0;
    return db;
}

void freeDatabase(MemberDB *db) {
    for (int i = 0; i < db->count; i++) free(db->records[i]);
    free(db->records);
    free(db);
}

int loadDatabase(MemberDB *db) {
    FILE *f = fopen(DATABASE_FILE, "rb");
    if (!f) return 0; // empty db

    MemberRecord temp;
    while (fread(&temp, sizeof(MemberRecord), 1, f) == 1) {
        MemberRecord *rec = malloc(sizeof(MemberRecord));
        *rec = temp;
        db->records = realloc(db->records, sizeof(MemberRecord*) * (db->count + 1));
        db->records[db->count++] = rec;
    }
    fclose(f);
    return 1;
}

int saveDatabase(const MemberDB *db) {
    FILE *f = fopen(DATABASE_FILE, "wb");
    if (!f) return 0;
    for (int i = 0; i < db->count; i++)
        fwrite(db->records[i], sizeof(MemberRecord), 1, f);
    fclose(f);
    return 1;
}

int isDuplicateID(MemberDB *db, int id) {
    for (int i = 0; i < db->count; i++)
        if (db->records[i]->id == id) return 1;
    return 0;
}

void registerMember(MemberDB *db) {
    MemberRecord newRec;
    newRec.id = getIntInput("Enter ID (1-99999): ", 1, 99999);
    if (isDuplicateID(db, newRec.id)) { printf("ID already exists!\n"); return; }

    getStringInput("Full Name: ", newRec.name, sizeof(newRec.name));
    getStringInput("Batch (CS/SE/AI/Cyber Security): ", newRec.batch, sizeof(newRec.batch));
    getStringInput("Registration Date (YYYY-MM-DD): ", newRec.registrationDate, sizeof(newRec.registrationDate));
    getStringInput("Birth Date (YYYY-MM-DD): ", newRec.birthDate, sizeof(newRec.birthDate));
    newRec.interest = getIntInput("Interest (1=IEEE, 2=ACM, 3=Both): ", 1, 3);

    MemberRecord *recPtr = malloc(sizeof(MemberRecord));
    *recPtr = newRec;
    db->records = realloc(db->records, sizeof(MemberRecord*) * (db->count + 1));
    db->records[db->count++] = recPtr;

    saveDatabase(db);
    printf("Member added successfully!\n");
}

int findMember(MemberDB *db, int id) {
    for (int i = 0; i < db->count; i++)
        if (db->records[i]->id == id) return i;
    return -1;
}

void updateMember(MemberDB *db) {
    int id = getIntInput("Enter ID to update: ", 1, 99999);
    int idx = findMember(db, id);
    if (idx == -1) { printf("Member not found.\n"); return; }

    MemberRecord *rec = db->records[idx];
    int choice = getIntInput("[1] Update Interest [2] Update Batch: ", 1, 2);
    if (choice == 1) rec->interest = getIntInput("Interest (1=IEEE,2=ACM,3=Both): ", 1, 3);
    else getStringInput("Batch (CS/SE/AI/Cyber Security): ", rec->batch, sizeof(rec->batch));

    saveDatabase(db);
    printf("Member updated successfully!\n");
}

void deleteMember(MemberDB *db) {
    int id = getIntInput("Enter ID to delete: ", 1, 99999);
    int idx = findMember(db, id);
    if (idx == -1) { printf("Member not found.\n"); return; }

    free(db->records[idx]);
    for (int i = idx; i < db->count - 1; i++) db->records[i] = db->records[i + 1];
    db->count--;
    db->records = realloc(db->records, sizeof(MemberRecord*) * db->count);

    saveDatabase(db);
    printf("Member deleted successfully!\n");
}

void viewAllMembers(const MemberDB *db) {
    if (db->count == 0) { printf("No members found.\n"); return; }

    printf("\n%-5s %-20s %-16s %-12s %-12s %-10s\n",
           "ID", "Name", "Batch", "Reg Date", "Birth Date", "Interest");
    printf("---------------------------------------------------------------\n");
    for (int i = 0; i < db->count; i++) {
        const char *interestText = (db->records[i]->interest == 1) ? "IEEE" :
                                   (db->records[i]->interest == 2) ? "ACM" : "Both";
        printf("%-5d %-20s %-16s %-12s %-12s %-10s\n",
               db->records[i]->id, db->records[i]->name, db->records[i]->batch,
               db->records[i]->registrationDate, db->records[i]->birthDate, interestText);
    }
}

void batchReport(const MemberDB *db) {
    char batchName[20];
    getStringInput("Enter batch for report: ", batchName, sizeof(batchName));
    printf("\nBatch Report: %s\n", batchName);
    printf("%-5s %-20s %-16s %-12s %-12s %-10s\n",
           "ID", "Name", "Batch", "Reg Date", "Birth Date", "Interest");
    printf("---------------------------------------------------------------\n");
    int count = 0;
    for (int i = 0; i < db->count; i++) {
        if (strcmp(db->records[i]->batch, batchName) == 0) {
            const char *interestText = (db->records[i]->interest == 1) ? "IEEE" :
                                       (db->records[i]->interest == 2) ? "ACM" : "Both";
            printf("%-5d %-20s %-16s %-12s %-12s %-10s\n",
                   db->records[i]->id, db->records[i]->name, db->records[i]->batch,
                   db->records[i]->registrationDate, db->records[i]->birthDate, interestText);
            count++;
        }
    }
    printf("Total members in %s: %d\n", batchName, count);
}

int main() {
    MemberDB *db = createDatabase();
    loadDatabase(db);

    int choice;
    while (1) {
        printf("\n--- MEMBER REGISTRATION SYSTEM ---\n");
        printf("1. Register Member\n2. Update Member\n3. Delete Member\n4. View All Members\n5. Batch Report\n6. Exit\n");
        choice = getIntInput("Select option: ", 1, 6);

        switch (choice) {
            case 1: registerMember(db); break;
            case 2: updateMember(db); break;
            case 3: deleteMember(db); break;
            case 4: viewAllMembers(db); break;
            case 5: batchReport(db); break;
            case 6: freeDatabase(db); exit(0);
            default: printf("Invalid option.\n");
        }
    }
}

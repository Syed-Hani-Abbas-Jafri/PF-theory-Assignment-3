#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_BUFFER 1024
#define TEXT_INIT_CAP 2

typedef struct {
    char **lines;
    int count;
    int capacity;
} TextEditor;

void initEditor(TextEditor *editor) {
    editor->count = 0;
    editor->capacity = TEXT_INIT_CAP;
    editor->lines = (char **)malloc(editor->capacity * sizeof(char *));
    if (!editor->lines) { perror("Allocation failed"); exit(1); }
}

void resizeEditor(TextEditor *editor, int newCapacity) {
    if (newCapacity < TEXT_INIT_CAP) newCapacity = TEXT_INIT_CAP;
    char **temp = (char **)realloc(editor->lines, newCapacity * sizeof(char *));
    if (!temp) { perror("Reallocation failed"); return; }
    editor->lines = temp;
    editor->capacity = newCapacity;
}

void insertLine(TextEditor *editor, int index, const char *text) {
    if (index < 0 || index > editor->count) index = editor->count;
    if (editor->count == editor->capacity) resizeEditor(editor, editor->capacity * 2);
    if (index < editor->count)
        memmove(&editor->lines[index + 1], &editor->lines[index], (editor->count - index) * sizeof(char *));
    editor->lines[index] = (char *)malloc(strlen(text) + 1);
    if (!editor->lines[index]) { perror("String allocation failed"); return; }
    strcpy(editor->lines[index], text);
    editor->count++;
}

void deleteLine(TextEditor *editor, int index) {
    if (index < 0 || index >= editor->count) return;
    free(editor->lines[index]);
    if (index < editor->count - 1)
        memmove(&editor->lines[index], &editor->lines[index + 1], (editor->count - index - 1) * sizeof(char *));
    editor->count--;
}

void shrinkToFit(TextEditor *editor) {
    if (editor->count == 0) return;
    if (editor->capacity > editor->count) resizeEditor(editor, editor->count);
}

void printAllLines(TextEditor *editor) {
    for (int i = 0; i < editor->count; i++)
        printf("%d: %s\n", i + 1, editor->lines[i]);
}

void freeAllLines(TextEditor *editor) {
    for (int i = 0; i < editor->count; i++) free(editor->lines[i]);
    free(editor->lines);
    editor->lines = NULL;
    editor->count = editor->capacity = 0;
}

void saveTextToFile(TextEditor *editor, const char *filename) {
    FILE *fp = fopen(filename, "w");
    if (!fp) { perror("File write error"); return; }
    for (int i = 0; i < editor->count; i++) fprintf(fp, "%s\n", editor->lines[i]);
    fclose(fp);
}

void loadTextFromFile(TextEditor *editor, const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (!fp) { perror("File read error"); return; }
    freeAllLines(editor);
    initEditor(editor);
    char buffer[MAX_BUFFER];
    while (fgets(buffer, MAX_BUFFER, fp)) {
        buffer[strcspn(buffer, "\n")] = 0;
        insertLine(editor, editor->count, buffer);
    }
    fclose(fp);
}

void runTextEditor() {
    TextEditor editor;
    initEditor(&editor);
    int choice, idx;
    char buffer[MAX_BUFFER], filename[50];

    while (1) {
        printf("\n1.Insert Line\n2.Delete Line\n3.Print All\n4.Shrink Memory\n5.Save\n6.Load\n7.Exit\nChoice: ");
        if (scanf("%d", &choice) != 1) break;
        getchar();

        switch (choice) {
            case 1:
                printf("Line number: "); scanf("%d", &idx); getchar();
                printf("Enter text: "); fgets(buffer, MAX_BUFFER, stdin); buffer[strcspn(buffer, "\n")] = 0;
                insertLine(&editor, idx - 1, buffer);
                break;
            case 2:
                printf("Line number to delete: "); scanf("%d", &idx);
                deleteLine(&editor, idx - 1);
                break;
            case 3:
                printAllLines(&editor);
                break;
            case 4:
                shrinkToFit(&editor);
                printf("Memory shrunk to fit %d lines.\n", editor.count);
                break;
            case 5:
                printf("Filename: "); scanf("%s", filename);
                saveTextToFile(&editor, filename);
                break;
            case 6:
                printf("Filename: "); scanf("%s", filename);
                loadTextFromFile(&editor, filename);
                break;
            case 7:
                freeAllLines(&editor);
                return;
            default:
                printf("Invalid choice.\n");
        }
    }
}

int main() {
    runTextEditor();
    return 0;
}

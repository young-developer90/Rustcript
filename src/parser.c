#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#define MAX_LINE_LEN 512

char* trim(char* str) {
    char* end;
    while (isspace((unsigned char)*str)) str++;
    if (*str == 0) return str;
    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end)) end--;
    *(end + 1) = 0;
    return str;
}

bool starts_with(const char* str, const char* prefix) {
    return strncmp(str, prefix, strlen(prefix)) == 0;
}

void process_line(const char* line, bool* inside_unsafe) {
    if (strstr(line, "unsafe {") != NULL) {
        *inside_unsafe = true;
        printf("[Unsafe block start]\n");
    }
    else if (*inside_unsafe && strcmp(line, "}") == 0) {
        *inside_unsafe = false;
        printf("[Unsafe block end]\n");
    }
    else if (starts_with(line, "let ")) {
        printf("[Variable declaration] %s\n", line);
    }
    else if (starts_with(line, "if ")) {
        printf("[If statement] %s\n", line);
    }
    else if (starts_with(line, "elif ")) {
        printf("[Elif statement] %s\n", line);
    }
    else if (starts_with(line, "else")) {
        printf("[Else statement] %s\n", line);
    }
    else if (strlen(line) == 0) {
    }
    else {
        printf("[Other statement] %s\n", line);
    }
}

void run_parser(const char* filename) {
    FILE* f = fopen(filename, "r");
    if (!f) {
        fprintf(stderr, "Error: Cannot open file %s\n", filename);
        return;
    }

    char buffer[MAX_LINE_LEN];
    bool inside_unsafe = false;

    while (fgets(buffer, sizeof(buffer), f)) {
        char* line = trim(buffer);
        process_line(line, &inside_unsafe);
    }

    fclose(f);
}

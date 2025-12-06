#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define COLOR_RED     "\033[31m"
#define COLOR_GREEN   "\033[32m"
#define COLOR_YELLOW  "\033[33m"
#define COLOR_BLUE    "\033[34m"
#define COLOR_CYAN    "\033[36m"
#define COLOR_RESET   "\033[0m"

// Secure random byte from /dev/urandom
unsigned char get_secure_byte() {
    unsigned char b;
    FILE *ur = fopen("/dev/urandom", "rb");
    if (!ur) {
        perror("Cannot open /dev/urandom");
        exit(1);
    }
    fread(&b, 1, 1, ur);
    fclose(ur);
    return b;
}

void generate_password(int length, int includeSymbols, FILE *outfile) {

    const char upper[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    const char lower[] = "abcdefghijklmnopqrstuvwxyz";
    const char digits[] = "0123456789";

    const char symbols[] =
        "!\"#$%&'()*+,-./:;<=>?@[\\]^_`{|}~";

    char allChars[256] = {0};

    // Build full allowed character list
    strcpy(allChars, upper);
    strcat(allChars, lower);
    strcat(allChars, digits);
    if (includeSymbols) strcat(allChars, symbols);

    int uSize = strlen(upper);
    int lSize = strlen(lower);
    int dSize = strlen(digits);
    int sSize = strlen(symbols);
    int allSize = strlen(allChars);

    char password[length + 1];
    int idx = 0;

    // Ensure category coverage
    password[idx++] = upper[get_secure_byte() % uSize];
    password[idx++] = lower[get_secure_byte() % lSize];
    password[idx++] = digits[get_secure_byte() % dSize];
    if (includeSymbols)
        password[idx++] = symbols[get_secure_byte() % sSize];

    // Fill remaining
    while (idx < length)
        password[idx++] = allChars[get_secure_byte() % allSize];

    // Shuffle the password
    for (int i = length - 1; i > 0; i--) {
        int j = get_secure_byte() % (i + 1);
        char tmp = password[i];
        password[i] = password[j];
        password[j] = tmp;
    }

    password[length] = '\0';

    printf(COLOR_GREEN "%s\n" COLOR_RESET, password);

    if (outfile)
        fprintf(outfile, "%s\n", password);
}

void menu() {
    printf(COLOR_CYAN "\n======== Secure Password Generator Menu ========\n" COLOR_RESET);
    printf(COLOR_YELLOW "1." COLOR_RESET " Set password length\n");
    printf(COLOR_YELLOW "2." COLOR_RESET " Set number of passwords\n");
    printf(COLOR_YELLOW "3." COLOR_RESET " Include symbols (on/off)\n");
    printf(COLOR_YELLOW "4." COLOR_RESET " Set output filename\n");
    printf(COLOR_YELLOW "5." COLOR_RESET " Generate passwords\n");
    printf(COLOR_YELLOW "6." COLOR_RESET " Exit\n");
    printf(COLOR_CYAN "================================================\n" COLOR_RESET);
    printf("Enter choice: ");
}

int main() {
    int length = 16;
    int count = 5;
    int includeSymbols = 1;
    char filename[256] = "passwords.txt";

    int choice;

    while (1) {
        menu();
        scanf("%d", &choice);

        if (choice == 1) {
            printf("Enter password length: ");
            scanf("%d", &length);
            if (length < 4) {
                printf(COLOR_RED "Length must be >= 4.\n" COLOR_RESET);
                length = 4;
            }
        }
        else if (choice == 2) {
            printf("Enter number of passwords: ");
            scanf("%d", &count);
            if (count < 1) count = 1;
        }
        else if (choice == 3) {
            printf("Include symbols? (1=yes, 0=no): ");
            scanf("%d", &includeSymbols);
        }
        else if (choice == 4) {
            printf("Enter output filename: ");
            scanf("%s", filename);
        }
        else if (choice == 5) {
            FILE *outfile = fopen(filename, "w");
            if (!outfile) {
                perror("Error opening file");
                continue;
            }

            printf(COLOR_BLUE "\nGenerating passwords...\n\n" COLOR_RESET);

            for (int i = 0; i < count; i++) {
                generate_password(length, includeSymbols, outfile);
            }

            fclose(outfile);

            printf(COLOR_GREEN "\nPasswords saved to: %s\n\n" COLOR_RESET, filename);
        }
        else if (choice == 6) {
            printf(COLOR_YELLOW "Exiting...\n" COLOR_RESET);
            break;
        }
        else {
            printf(COLOR_RED "Invalid choice.\n" COLOR_RESET);
        }
    }
    return 0;
}
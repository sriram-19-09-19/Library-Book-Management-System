#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILENAME "library_db.txt"


struct Book {
    int id;
    char title[50];
    char author[50];
    int quantity;
};


struct Book *library = NULL;
int book_count = 0;


void loadFromFile();
void saveToFile();
void addBook();
void issueBook(int id);
void returnBook(int id);
void displayBooks();
void searchBookById(int id);


void loadFromFile() {
    FILE *fp = fopen(FILENAME, "r");
    if (fp == NULL) {
        
        book_count = 0;
        return;
    }

    
    if (fscanf(fp, "%d\n", &book_count) != 1) {
        book_count = 0;
        fclose(fp);
        return;
    }

    if (book_count > 0) {
        library = (struct Book *)malloc(book_count * sizeof(struct Book));
        if (library == NULL) {
            printf("Error: System memory allocation failed during database boot!\n");
            fclose(fp);
            exit(1);
        }

        for (int i = 0; i < book_count; i++) {
            fscanf(fp, "%d\n", &library[i].id);
           
            fgets(library[i].title, sizeof(library[i].title), fp);
            library[i].title[strcspn(library[i].title, "\n")] = '\0'; // Strip saved trailing newline
           
            fgets(library[i].author, sizeof(library[i].author), fp);
            library[i].author[strcspn(library[i].author, "\n")] = '\0';
           
            fscanf(fp, "%d\n", &library[i].quantity);
        }
    }
    fclose(fp);
    printf("Successfully loaded %d records from storage database.\n", book_count);
}


void saveToFile() {
    FILE *fp = fopen(FILENAME, "w");
    if (fp == NULL) {
        printf("Error: Data persistent storage write failure!\n");
        return;
    }

    fprintf(fp, "%d\n", book_count);
    for (int i = 0; i < book_count; i++) {
        fprintf(fp, "%d\n", library[i].id);
        fprintf(fp, "%s\n", library[i].title);
        fprintf(fp, "%s\n", library[i].author);
        fprintf(fp, "%d\n", library[i].quantity);
    }
    fclose(fp);
}


void addBook() {
    int input_id, quantity;
    printf("\nEnter Unique Book ID (Integer): ");
    if (scanf("%d", &input_id) != 1) {
        printf("Invalid ID syntax input.\n");
        while (getchar() != '\n'); 
        return;
    }

    
    for (int i = 0; i < book_count; i++) {
        if (library[i].id == input_id) {
            printf("Book ID %d already exists ('%s'). Enter a new copy count to update stock: ", input_id, library[i].title);
            if (scanf("%d", &quantity) == 1 && quantity > 0) {
                library[i].quantity += quantity;
                printf("Stock updated successfully.\n");
            }
            return;
        }
    }

   
    struct Book *temp = (struct Book *)realloc(library, (book_count + 1) * sizeof(struct Book));
    if (temp == NULL) {
        printf("Error: Out of memory! Cannot add more records.\n");
        return;
    }
    library = temp;

    library[book_count].id = input_id;
    while (getchar() != '\n'); 
    printf("Enter Book Title: ");
    fgets(library[book_count].title, sizeof(library[book_count].title), stdin);
    library[book_count].title[strcspn(library[book_count].title, "\n")] = '\0';

    printf("Enter Author Name: ");
    fgets(library[book_count].author, sizeof(library[book_count].author), stdin);
    library[book_count].author[strcspn(library[book_count].author, "\n")] = '\0';

    printf("Enter Total Copies Quantity: ");
    if (scanf("%d", &library[book_count].quantity) != 1 || library[book_count].quantity < 0) {
        library[book_count].quantity = 0;
    }

    book_count++;
    printf("Book successfully registered to catalog.\n");
}


void issueBook(int id) {
    for (int i = 0; i < book_count; i++) {
        if (library[i].id == id) {
            
            if (library[i].quantity > 0) {
                library[i].quantity--;
                printf("Success: Book '%s' successfully checked out.\n", library[i].title);
            } else {
                printf("Error: Out of stock! '%s' has 0 copies available.\n", library[i].title);
            }
            return;
        }
    }
    printf("Error: Book with ID %d was not found.\n", id);
}


void returnBook(int id) {
    for (int i = 0; i < book_count; i++) {
        if (library[i].id == id) {
            // Increase quantity
            library[i].quantity++;
            printf("Success: One copy of '%s' returned safely to inventory.\n", library[i].title);
            return;
        }
    }
    printf("Error: Book with ID %d does not match library records.\n", id);
}


void displayBooks() {
    if (book_count == 0) {
        printf("\nThe library database contains zero active book entries.\n");
        return;
    }

    printf("==================================== LIBRARY INVENTORY ====================================\n");
    printf("| %-8s | %-30s | %-25s | %-12s |\n", "Book ID", "Title", "Author", "Qty in Stock");
   
    for (int i = 0; i < book_count; i++) {
        printf("| %-8d | %-30.30s | %-25.25s | %-12d |\n",
               library[i].id, library[i].title, library[i].author, library[i].quantity);
    }
    
}


void searchBookById(int id) {
    for (int i = 0; i < book_count; i++) {
        if (library[i].id == id) {
            printf("\n--- Match Found ---\n");
            printf("Book ID : %d\n", library[i].id);
            printf("Title   : %s\n", library[i].title);
            printf("Author  : %s\n", library[i].author);
            printf("Stock   : %d copies available\n", library[i].quantity);
            return;
        }
    }
    printf("Book with ID %d not found in database.\n", id);
}

int main() {
    int choice, target_id;

    
    loadFromFile();

    printf("========== welcome to EMB school Library Management System ========== \n");
   
    while (1) {
        printf("\nChoose an option:\n");
        printf("1. Add a Book\n");
        printf("2. Issue a Book\n");
        printf("3. Return a Book\n");
        printf("4. Display All Books\n");
        printf("5. Search by Book ID\n");
        printf("6. Save & Exit\n");
        printf("Enter your choice: ");
       
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input choice option.\n");
            while (getchar() != '\n');
            continue;
        }

        switch (choice) {
            case 1:
                addBook();
                break;
            case 2:
                printf("Enter Book ID to issue: ");
                scanf("%d", &target_id);
                issueBook(target_id);
                break;
            case 3:
                printf("Enter Book ID to return: ");
                scanf("%d", &target_id);
                returnBook(target_id);
                break;
            case 4:
                displayBooks();
                break;
            case 5:
                printf("Enter Book ID to search: ");
                scanf("%d", &target_id);
                searchBookById(target_id);
                break;
            case 6:
                
                printf("\nSaving items\n");
                saveToFile();
                if (library != NULL) {
                    free(library);
                }
                return 0;
            default:
                printf("Invalid selection alternative out of bounds.\n");
        }
    }
    return 0;
}

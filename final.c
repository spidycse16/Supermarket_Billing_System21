#include <stdio.h>
#include <windows.h>
#include<stdlib.h>

// File paths
const char* FILE_PATH = "items.txt";
int bill_id=1000;

void gotoxy(int x, int y)
{
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}


void addItem()
{
    int i;
    system("cls");
    gotoxy(36,1);
    printf("Add item");
    gotoxy(22,2);
    for(i=0 ; i<35 ; i++)
        printf("_");
    for(i=3 ; i<15 ; i++)
    {
        gotoxy(21,i);
        printf("|");
    }
    gotoxy(22,14);
    for(i=0 ; i<35 ; i++)
        printf("_");

        for(i=3 ; i<15 ; i++)
    {
        gotoxy(57,i);
        printf("|");
    }

      gotoxy(22,4);
    for(i=0 ; i<35 ; i++)
        printf("_");


    gotoxy(24,3);
    printf("Bill id-> %d",bill_id);
    // Open the file in append mode
    FILE* file = fopen(FILE_PATH, "a");
    if (file == NULL)
    {
        printf("Error opening file.\n");
        return;
    }

    // Get item details from the user
    char itemName[100];
    int quantity;
    float price;
    gotoxy(24,5);
    printf("Enter item name: ");
    scanf(" %[^\n]s", itemName);
    gotoxy(24,7);
    printf("Enter quantity: ");
    scanf("%d", &quantity);
    gotoxy(24,9);
    printf("Enter price: ");
    scanf("%f", &price);

    // Write item details to the file
    fprintf(file, "%s,%d,%.2f\n", itemName, quantity, price*quantity);
    fclose(file);
    gotoxy(24,11);
    printf("Item added successfully.\n");
    gotoxy(20,15);
}

void includeTax()
{
    system("cls");

       // Open the file for reading
    FILE* file = fopen(FILE_PATH, "r");
    if (file == NULL) {
        printf("Error opening file.\n");
        return;
    }

    // Get the tax percentage from the user
    float taxPercentage;
    gotoxy(22,3);
    printf("Enter tax percentage: ");
    scanf("%f", &taxPercentage);
    printf("\n");

    // Display the header
    printf("%-15s %-12s %-10s %-15s\n", "Item Name", "Quantity", "Price", "Price + Tax");
    printf("------------------------------------------------------\n");

    // Read and display item details from the file with tax included
    char itemName[100];
    int quantity;
    float price;
int x=4;
    while (fscanf(file, "%[^,],%d,%f\n", itemName, &quantity, &price) != EOF) {
        float totalPrice = price + (price * taxPercentage / 100.0);
        printf("%-18s %-6d   $%-10.2f $%.2f\n", itemName, quantity, price, totalPrice);

    }

    // Close the file
    fclose(file);
}

void includeDiscount()
{
    system("cls");
      // Open the file for reading
    FILE* file = fopen(FILE_PATH, "r");
    if (file == NULL) {
        printf("Error opening file.\n");
        return;
    }

    // Get the discount percentage from the user
    float discountPercentage;
    gotoxy(22,3);
    printf("Enter discount percentage: ");
    scanf("%f", &discountPercentage);
    printf("\n");

    // Display the header
    printf("%-12s %-12s %-10s %-10s\n", "Item Name", "Quantity", "Price", "Price - Discount");
    printf("------------------------------------------------------\n");

    // Read and display item details from the file with discount included
    char itemName[100];
    int quantity;
    float price;

    while (fscanf(file, "%[^,],%d,%f\n", itemName, &quantity, &price) != EOF) {
        float discountedPrice = price - (price * discountPercentage / 100.0);
        printf("%-15s %-8d $%-15.2f $%.2f\n", itemName, quantity, price, discountedPrice);
    }

    // Close the file
    fclose(file);
}

void editItem()
{
    system("cls");
            // Open the file for reading
    FILE* file = fopen(FILE_PATH, "r");
    if (file == NULL) {
        printf("Error opening file.\n");
        return;
    }

    // Create a temporary file to store the edited data
    FILE* tempFile = fopen("temp_items.txt", "w");
    if (tempFile == NULL) {
        printf("Error creating temporary file.\n");
        fclose(file);
        return;
    }

    // Get item details to be edited from the user
    char itemName[100];
    gotoxy(22,3);
    printf("Enter the item name to be edited: ");
    scanf(" %[^\n]s", itemName);

    int newQuantity;
    float newPrice;

    // Get the new item details from the user
    gotoxy(22,5);
    printf("Enter new quantity: ");
    scanf("%d", &newQuantity);
    gotoxy(22,7);
    printf("Enter new price for 1 quantity: ");
    scanf("%f", &newPrice);

    // Read and update item details in the temporary file
    char currentName[100];
    int quantity;
    float price;

    while (fscanf(file, "%[^,],%d,%f\n", currentName, &quantity, &price) != EOF) {
        if (strcmp(itemName, currentName) == 0) {
            // Item found, update details
            float totalPrice = newPrice * newQuantity;
            fprintf(tempFile, "%s,%d,%.2f\n", itemName, newQuantity, totalPrice);
        } else {
            // Item not found, copy unchanged details
            fprintf(tempFile, "%s,%d,%.2f\n", currentName, quantity, price);
        }
    }

    // Close the files
    fclose(file);
    fclose(tempFile);

    // Remove the original file and rename the temporary file to the original file name
    remove(FILE_PATH);
    rename("temp_items.txt", FILE_PATH);
    gotoxy(22,11);
    printf("Item edited successfully.\n");
    gotoxy(22,12);
}

void deleteItem() {

    system("cls");
    char itemNameToDelete[100];

    // Input: Get the name of the item to delete from the user
    gotoxy(22,4);
    printf("Enter the name of the item to delete: ");
    scanf("%99s", itemNameToDelete);

    // Open the file for reading
    FILE* file = fopen(FILE_PATH, "r");
    if (file == NULL) {
            gotoxy(22,6);
        printf("File not found. Please add items first.\n");
        return;
    }

    // Create a temporary file to store items without the one to delete
    FILE* tempFile = fopen("temp_items.txt", "w");
    if (tempFile == NULL) {
            gotoxy(22,6);
        printf("Error creating temporary file.\n");
        fclose(file);
        return;
    }

    char itemName[100];
    int quantity;
    float price;

    int itemDeleted = 0; // Flag to track if the item has been deleted

    while (fscanf(file, "%99[^,],%d,%f\n", itemName, &quantity, &price) != EOF) {
        // Check if the current item matches the item to delete
        if (strcmp(itemName, itemNameToDelete) == 0) {
            itemDeleted = 1; // Set the flag to indicate the item was deleted
            continue;        // Skip writing this item to the temporary file
        }

        // Write the item to the temporary file
        fprintf(tempFile, "%s,%d,%.2f\n", itemName, quantity, price);
    }

    // Close both files
    fclose(file);
    fclose(tempFile);

    // Remove the original file and rename the temporary file
    remove(FILE_PATH);
    rename("temp_items.txt", FILE_PATH);

    if (itemDeleted) {
            gotoxy(22,6);
        printf("Item '%s' deleted successfully.\n", itemNameToDelete);
    } else {
        gotoxy(22,6);
        printf("Item '%s' not found in the list.\n", itemNameToDelete);
    }
}

void clearBill() {
    bill_id++;
    system("cls");
    char confirmation[10];

    // Input: Ask for confirmation to clear the item list
    gotoxy(1,4);
    printf("Are you sure you want to clear the item list? \n (Type 'yes' to confirm): ");
    scanf("%9s", confirmation);

    if (strcmp(confirmation, "yes") == 0) {
        // Clear the item list
        FILE* file = fopen(FILE_PATH, "w");
        if (file == NULL) {
                gotoxy(1,6);
            printf("Error opening file for clearing.\n");
            return;
        }

        fclose(file);
        gotoxy(1,6);
        printf("Item list cleared successfully.\n");
    } else {
        gotoxy(1,6);
        printf("Item list was not cleared.\n");
    }
}

void showBill()
{
    system("cls");
    // Open the file for reading
    FILE* file = fopen(FILE_PATH, "r");
    if (file == NULL)
    {
        printf("Error opening file.\n");
        return;
    }

    // Display the bill header
    printf("%-20s %-12s %-10s\n", "Item Name", "Quantity", "Price");
    printf("-----------------------------------------\n");

    // Read and display item details from the file
    char itemName[100];
    int quantity;
    float price;

    while (fscanf(file, "%[^,],%d,%f\n", itemName, &quantity, &price) != EOF)
    {
        printf("%-20s %-12d $%.2f\n", itemName, quantity, price);

    }

    // Close the file
    fclose(file);
}
int main()
{
    int choice;

    do
    {
        system("cls"); // Clear the console screen
        gotoxy(20,2); // Move cursor to (0, 0)
        for(int i=0 ; i<35 ; i++)
            printf("_");
        gotoxy(24,3);
        printf("Supermarket Billing System\n");
        gotoxy(20,4);
        for(int i=0 ; i<35 ; i++)
            printf("_");
        for(int i=3 ; i<18 ; i++)
        {
            gotoxy(19,i);
            printf("|");
        }
        gotoxy(20,17);
        for(int i=0 ; i<35 ; i++)
            printf("_");
        for(int i=3 ; i<18 ; i++)
        {
            gotoxy(55,i);
            printf("|");
        }
        gotoxy(24,5);
        printf("1. Add Item\n");
        gotoxy(24,6);
        printf("2. Include Tax\n");
        gotoxy(24,7);
        printf("3. Include Discount\n");
        gotoxy(24,8);
        printf("4. Edit Item\n");
        gotoxy(24,9);
        printf("5. Delete Item\n");
        gotoxy(24,10);
        printf("6. Clear Bill\n");
        gotoxy(24,11);
        printf("7. Show Bill\n");
        gotoxy(24,12);
        printf("8. Exit\n");
        gotoxy(24,15);
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            addItem();
            break;
        case 2:
            includeTax();
            break;
        case 3:
            includeDiscount();
            break;
        case 4:
            editItem();
            break;
        case 5:
            deleteItem();
            break;
        case 6:
            clearBill();
            break;
        case 7:
            showBill();
            break;
        case 8:
            printf("Exiting the program. Goodbye!\n");
            break;
        default:
            printf("Invalid choice. Please try again.\n");
        }

        printf("\n");
        system("pause"); // Pause the program until the user presses a key
    }
    while (choice != 8);

    return 0;
}


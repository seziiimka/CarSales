#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_MODEL_LENGTH 20
#define DISCOUNT_VALUE 0.15f
#define MAX_FEEDBACK_ENTRIES 10


// Structure to store car information
typedef struct {
    char model[MAX_MODEL_LENGTH];
    int year;
    float price;
    int amount;
    char customer_name[50];
    float discount;
    char comment[100];
    int rating;
    struct {
        int day;
        int month;
        int year;
    } purchase_date;
    float total_sales_amount;
} Car;



Car cars[10] = {
        {"Ford_Puma", 2019, 42000, 5},
        {"Nissan_Qashqai", 2022, 34000, 3},
        {"Vauxhall_Corsa", 2023, 33000, 8},
        {"Kia_Sportage", 2023, 31000, 2},
        {"Tesla_Model_Y", 2023, 35000, 6},
        {"Hyundai_Tucson", 2023, 29000, 4},
        {"Nissan_Juke", 2023, 27000, 7},
        {"MINI", 2020, 26000, 1},
        {"Vauxhall_Mokka", 2023, 30000, 9},
        {"Audi_A3", 2022, 25000, 10}
};

// Function prototypes
void pauseProgram(char userChoice);
void display_menu();
void view_cars_stock();
void buy_cars();
void view_sales_data( );
void view_customer_feedback();
void menu_exit();

int main() {
    int num_cars = 0;

    // Load previous sales data from file (if exists)
    FILE *file = fopen("sales_data.txt", "r");
    if (file != NULL) {
        fscanf(file, "%d\n", &num_cars);
        for (int i = 0; i < num_cars; i++) {
            fscanf(file, "%s %d %d %d\n", cars[i].model, &cars[i].year, &cars[i].price, &cars[i].amount);
        }
        fclose(file);
    }
    int exitFlag = 0;
    for (int i = 0; i < 10; i++) {
        cars[i].total_sales_amount = 0.0f;
    }

    do {
        display_menu();
        char choice;
        printf("Enter your choice: ");
        scanf("%s", &choice);


        switch (choice) {
            case 'a':
                view_cars_stock();
                break;
            case 'b':
                buy_cars();
                break;
            case 'c':
                view_sales_data();
                break;
            case 'd':
                view_customer_feedback();
                break;
            case 'x':
                exitFlag = 1;
                menu_exit();
                break;
            default:
                printf("Invalid choice! Please try again.\n");
        }

        // Save sales data to file

        file = fopen("sales_data.txt", "a");
        if (file != NULL) {
            fprintf(file, "%d\n", num_cars);
            for (int i = 0; i < num_cars; i++) {
                fprintf(file, "%s %d %d %d\n", cars[i].model, cars[i].year, cars[i].price, cars[i].amount);
            }
            fclose(file);
        }

        pauseProgram(choice);

    } while (exitFlag != 1);
}

void pauseProgram(char userChoice) {
    // give the user a chance to read the previous output, allow them to continue when ready
    // customise the output depending on the user's choice
    if (userChoice == 'x') {
        printf("\nPress Enter to Exit...");
    }
    else {
        printf("\nPress Enter to return to the Menu...");
    }

    // two of these to skip the newline character that's likely floating around the console window
    getchar(); getchar();
}



void display_menu() {
    printf("\nCar Sales Project Menu:\n");
    printf("a. View Cars Stock\n");
    printf("b. Buy Cars\n");
    printf("c. View Sales Data\n");
    printf("d.View Customer Feedback\n");
    printf("x. Exit\n");

}

void view_cars_stock() {
    //Sort cars by year of manufacture in descending order
    for (int i = 0; i < 10 - 1; i++) {
        for (int j = 0; j < 10 - i - 1; j++) {
            if (cars[j].year < cars[j + 1].year) {
                Car temp = cars[j];
                cars[j] = cars[j + 1];
                cars[j + 1] = temp;
            }
        }
    }

    // Display cars stock
    printf("\nCars Stock:\n\n");
    printf("    %-20s %-10s %-10s %-10s\n", "Model", "Year", "Remaining", "Price");

    for (int i = 0; i < 10; i++) {
        printf("%d - %-20s %-10d %-10d %-10.2f\n", i, cars[i].model, cars[i].year, cars[i].amount, cars[i].price);
    }
}

float applyDiscount(float currentPrice) {
    int studentLevel;
    char response;

    printf("Are you a student? (y/n): ");
    scanf(" %c", &response);

    if (response == 'y') {
        printf("What is your student level? ");
        scanf("%d", &studentLevel);

        if (studentLevel >= 4 && studentLevel <= 9) {
            currentPrice *= (1 - DISCOUNT_VALUE);
            printf("Discount applied successfully!\n");
        } else {
            printf("Unfortunately, you don't get a discount.\n");
        }
    } else {
        printf("No discount applied.\n");
    }

    return currentPrice;
}


int validDate(int day, int month, int year) {
    // Check if the month is valid
    if (month < 1 || month > 12) {
        return 0;
    }

    // Check if the day is valid
    if (day < 1 || day > 31) {
        return 0;
    }

    // Check specific cases for months with fewer than 31 days
    if ((month == 4 || month == 6 || month == 9 || month == 11) && day > 30) {
        return 0;
    }

    // Check for February and leap years
    if (month == 2) {
        if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)) {
            if (day > 29) {
                return 0;
            }
        } else {
            if (day > 28) {
                return 0;
            }
        }
    }

    // If all checks pass, the date is valid
    return 1;
}

void buy_cars() {
    int model;
    int amount;

    view_cars_stock();

    printf("\nEnter the car model you want(0 to 9): ");
    scanf("%d", &model);


    // Find the car in the stock
    int car_index = -1;
    for (int i = 0; i < 10; i++) {
        if (i == model) {
            car_index = i;
        }
    }

    if (car_index != -1) {
        printf("Enter the amount of %s you want to buy out of the %d available: ", cars[car_index].model,cars[car_index].amount );
        scanf("%d", &amount);

        if (amount <= cars[car_index].amount) {
            float total_price = cars[car_index].price * amount;

            printf("Enter your name:");
            scanf("%s", cars[car_index].customer_name);

            applyDiscount(total_price);

            printf("Enter the purchase date (dd.mm.yyyy) : ");
            scanf("%d.%d.%d", &cars[car_index].purchase_date.day, &cars[car_index].purchase_date.month, &cars[car_index].purchase_date.year);

            if (validDate(cars[car_index].purchase_date.day, cars[car_index].purchase_date.month, cars[car_index].purchase_date.year)){
                printf("Valid date");
            }
            else{
                printf("Invalid date");
            }

            //ask if the user wants to provide feedback
            char response;

            printf("\nDo you want to provide feedback? (y/n):");
            scanf(" %c", &response);

            if (response == 'y') {
                printf("Enter your rating (out of 5): ");
                scanf("%d",&cars[car_index].rating);

                // Validate the entered rating
                if (cars[car_index].rating < 1 || cars[car_index].rating > 5) {
                    printf("Invalid rating! Please enter a rating between 1 and 5.\n");
                    // Handle the situation appropriately (e.g., ask for rating again)
                    return;
                }

                printf("Enter your comment: ");
                getchar();
                fgets(cars[car_index].comment, sizeof(cars[car_index].comment), stdin);

                // Store feedback in a file
                FILE *file = fopen("feedback.txt", "a");
                if (file != NULL) {
                    fprintf(file, "%s,%s,%d,%s", cars[car_index].model, cars[car_index].customer_name, cars[car_index].rating, cars[car_index].comment);
                    fclose(file);

                    printf("Feedback successfully recorded! Thank you for your input.\n");
                } else {
                    printf("Error opening feedback.txt for writing.\n");
                }if (response == 'n') {
                    // If the customer doesn't want to leave feedback, you can proceed without storing feedback.
                    printf("Thank you for your purchase!\n");
                } else {
                    printf("Invalid response!\n");
                }
            }

            // Update car stock
            cars[car_index].amount -= amount;

            // Print purchase details
            printf("\nPurchase Details:\n");
            printf("Car Model: %s\n", cars[car_index].model);
            printf("Amount: %d\n", amount);
            printf("Total Price: %.2f\n", total_price);
            printf("Customer Name: %s\n", cars[car_index].customer_name);
            printf("Purchase date (dd.mm.yyyy) : %d.%d.%d", cars[car_index].purchase_date.day, cars[car_index].purchase_date.month, cars[car_index].purchase_date.year);
        } else {
            printf("Insufficient stock!\n");
        }
    }else {
        printf("Invalid car model!\n");
    }
    //save the purchase data in the file
    FILE *salesFile = fopen("sales_data.txt", "a");
    float total_price = cars[car_index].price * amount;
    if (salesFile != NULL) {
        fprintf(salesFile, "%s %d %d %.2f %s %.2f %d.%d.%d",
                cars[car_index].model, cars[car_index].year, amount, total_price,
                cars[car_index].customer_name, DISCOUNT_VALUE, cars[car_index].purchase_date.day, cars[car_index].purchase_date.month, cars[car_index].purchase_date.year);
        fclose(salesFile);
        printf("\nPurchase details saved to sales_data.txt.\n");
    } else {
        printf("Error saving purchase details to sales_data.txt.\n");
    }
}

void view_sales_data() {
    // Open the sales data file for reading
    FILE *file = fopen("sales_data.txt", "r");
    // Sort cars by total sale amount in descending order
    for (int i = 0; i < 10 - 1; i++) {
        for (int j = 0; j < 10 - i - 1; j++) {
            if (cars[j].total_sales_amount < cars[j + 1].total_sales_amount) {
                Car temp = cars[j];
                cars[j] = cars[j + 1];
                cars[j + 1] = temp;
            }
        }
    }


    if (file != NULL) {
        printf("\nSales Data:\n");
        // Display sales data with total sale amount
        printf("%-20s %-15s %-15s %-15s %-15s %-15s\n",
               "Car Model", "Year", "Amount", "Total Price", "Customer Name", "Purchase Date");

        for (int i = 0; i < 10; i++) {
            printf("%-20s %-15d %-15d %-15.2f %-15s %d.%d.%d\n",
                   cars[i].model, cars[i].year, cars[i].amount, cars[i].price,
                   cars[i].customer_name, cars[i].purchase_date.day, cars[i].purchase_date.month,
                   cars[i].purchase_date.year);

        }

        // Close file
        fclose(file);
    } else {
        printf("Error opening sales_data.txt.\n");
    }
}


void view_customer_feedback() {
    FILE *file = fopen("feedback.txt", "r");

    if (file != NULL) {
        printf("\nCustomer Feedback:\n");
        printf("%-20s %-20s %-10s %-s\n", "Car Model", "Customer Name", "Rating", "Comment");

        // Loop through all entries in the file
        for (int i = 0; i < 10; i++) {
            printf("%-20s %-20s %-10d %s\n",
                   cars[i].model, cars[i].customer_name, cars[i].rating, cars[i].comment);
        }

        fclose(file);
    } else {
        printf("No customer feedback available.\n");
    }
}

void menu_exit() {
    printf("Thank you for using this Car Sales program. See you next time! ;)\n");

}
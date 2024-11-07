#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

struct item {
    int prodnum, quantity, price, amount;
    char prodname[10];
};

struct Stack {
    int top;
    struct item items[100];  // Assuming a maximum of 100 items in the shop
};

struct Stack shopStack;
struct Stack cartStack;

void push(struct Stack *stack, struct item *element) {
    stack->items[++stack->top] = *element;
}

struct item pop(struct Stack *stack) {
    return stack->items[stack->top--];
}

void create() {
    FILE *fp, *fpq;
    fp = fopen("shop.txt", "a");
    if (fp == NULL) {
        printf("ERROR\n");
        exit(0);
    }
    printf("Enter the Number of Records:\n");
    scanf("%d", &shopStack.top);
    fpq = fopen("store.txt", "w");
    fprintf(fpq, "%d", shopStack.top);
    fclose(fpq);

    for (int i = 0; i < shopStack.top; i++) {
        printf("Enter the Product Code:\n");
        scanf("%d", &shopStack.items[i].prodnum);
        printf("Enter the Product Name:\n");
        scanf("%s", shopStack.items[i].prodname);

        do {
            printf("Enter the Quantity (must be greater than 0):\n");
            scanf("%d", &shopStack.items[i].quantity);
        } while (shopStack.items[i].quantity <= 0);

        do {
            printf("Enter the Price (must be greater than 0):\n");
            scanf("%d", &shopStack.items[i].price);
        } while (shopStack.items[i].price <= 0);

        fprintf(fp, "%d\t%s\t%d\t%d\n", shopStack.items[i].prodnum, shopStack.items[i].prodname, shopStack.items[i].quantity, shopStack.items[i].price);
    }
    fclose(fp);
    printf("\nThe Records are being Created\n");
}
void edit() {
    FILE *fp, *fptr;
    int h, found = 0;

    fp = fopen("shop.txt", "r");
    fptr = fopen("temp1.txt", "w");

    printf("To DELETE, Enter the Product Number of the Item to be Deleted: \t\n");
    scanf("%d", &h);

    while (fscanf(fp, "%d%s%d%d", &shopStack.items[0].prodnum, shopStack.items[0].prodname, &shopStack.items[0].quantity, &shopStack.items[0].price) == 4 || !feof(fp)) {
        if (shopStack.items[0].prodnum == h) {
            found = 1;
            printf("Product has been successfully deleted\n");
        } else {
            fprintf(fptr, "%d\t%s\t%d\t%d\n", shopStack.items[0].prodnum, shopStack.items[0].prodname, shopStack.items[0].quantity, shopStack.items[0].price);
        }
    }

    fclose(fp);
    fclose(fptr);

    if (found == 0) {
        printf("\n Product Not Found\n");
    }

    remove("shop.txt");
    rename("temp1.txt", "shop.txt");
}

void display() {
    FILE *fp;
    fp = fopen("shop.txt", "r");

    if (fp == NULL) {
        printf("Error in opening the file\n");
        exit(0);
    }

    pattern();
    printf("\t|Product Number\t|Product Name\t|Quantity\t|Price|\t\n");
    pattern();

    int nk = 0;
    while (fscanf(fp, "%d%s%d%d", &shopStack.items[nk].prodnum, shopStack.items[nk].prodname, &shopStack.items[nk].quantity, &shopStack.items[nk].price) == 4 || !feof(fp)) {

           printf("\n\t|%d|\t\t|%s|\t|%d|\t\t|%d|\n", shopStack.items[nk].prodnum, shopStack.items[nk].prodname, shopStack.items[nk].quantity, shopStack.items[nk].price);
        nk++;
    }

    fclose(fp);
}

void bill() {
    int na, a, ch;
    FILE *fp, *fptr;

    while (1) {
        printf("Enter the Product Number (0 to finish shopping): ");
        scanf("%d", &na);

        if (na == 0) {
            break;
        }

        fp = fopen("shop.txt", "r");
        fptr = fopen("temp.txt", "w");
        int found = 0;

        for (int bi = 0; bi < shopStack.top; bi++) {
            if (fscanf(fp, "%d%s%d%d", &shopStack.items[bi].prodnum, shopStack.items[bi].prodname, &shopStack.items[bi].quantity, &shopStack.items[bi].price) != 4) {
                break;
            }

            if (shopStack.items[bi].prodnum == na) {
                found = 1;
                printf("Enter the Quantity: ");
                scanf("%d", &a);

                if (a <= shopStack.items[bi].quantity) {
                    struct item currentItem = shopStack.items[bi];
                    currentItem.quantity = a;
                    push(&cartStack, &currentItem);
                    shopStack.items[bi].quantity -= a;

                    printf("\t%d\t|%s\t|%d\t|%d\n", cartStack.items[cartStack.top].prodnum, cartStack.items[cartStack.top].prodname, a, cartStack.items[cartStack.top].price);
                } else {
                    printf("Out of Stock\n");
                }
            } else {
                fprintf(fptr, "%d\t%s\t%d\t%d\n", shopStack.items[bi].prodnum, shopStack.items[bi].prodname, shopStack.items[bi].quantity, shopStack.items[bi].price);
            }
        }

        fclose(fptr);
        fclose(fp);

        if (!found) {
            printf("Product not found. Enter correct code.\n");
        }

        remove("shop.txt");
        rename("temp.txt", "shop.txt");

        printf("Press 1 to continue shopping, 0 to go back to the main menu: ");
        scanf("%d", &ch);
        if (ch == 0) {
            return;
        }
    }
    invoice();
}
void display2() {
    if (cartStack.top == -1) {
        printf("CART IS EMPTY\n");
        return;
    }

    float gt = 0;
    pattern();
    printf("\t|Product Number\tProduct Name|\tQuantity|\t Price|\t Amount|\t\n");
    pattern();

    for (int o = 0; o <= cartStack.top; o++) {
        if (cartStack.items[o].prodnum != 0) {
            cartStack.items[o].amount = cartStack.items[o].quantity * cartStack.items[o].price;
            gt += cartStack.items[o].amount;

            printf("\t|%d|\t\t|%s|\t|%d|\t\t|%d|\t|%d|\n", cartStack.items[o].prodnum, cartStack.items[o].prodname,cartStack.items[o].quantity, cartStack.items[o].price, cartStack.items[o].amount);
        }
    }

    printf("\n Total Amount in Cart: %.2f\n", gt);
}

void invoice() {
    float gt = 0;
    if (cartStack.top == -1) {
        printf("CART IS EMPTY\n");
        exit(0);
    }
    printf("\n\n\t\t***** A3 GROCERY STORE *****\n");
    pattern();
    printf("\t|Product Number\tProduct Name|\tQuantity|\t Price|\t Amount|\t\n");
    pattern();
    for (int o = 0; o <= cartStack.top; o++) {
        if (cartStack.items[o].quantity != 0) {
            cartStack.items[o].amount = cartStack.items[o].quantity * cartStack.items[o].price;
            gt += cartStack.items[o].amount;

             printf("\t|%d|\t\t|%s|\t|%d|\t\t|%d|\t|%d|\n", cartStack.items[o].prodnum, cartStack.items[o].prodname,cartStack.items[o].quantity, cartStack.items[o].price, cartStack.items[o].amount);
        }
    }
    printf("\n Amount Payable is %.2f\n", gt);
    printf(" THANK YOU FOR SHOPPING HERE\n VISIT AGAIN!\n");
    exit(0);
}
void pattern() {
    printf("\n");
    for (int i = 80; i > 0; i--)
        printf("=");
    printf("\n");
}

int main() {
    char ch, password[10], q[10] = "123456";
    int s, y, i;
    FILE *fpe;
    fpe = fopen("store.txt", "r");
    fscanf(fpe, "%d", &shopStack.top);
    fclose(fpe);
    printf("\t\t\t~ WELCOME TO A3 GROCERY STORE ~\n\n");

    do {
        printf("\n\n\t01. ADMIN\n\t02. CUSTOMER\n\t03. EXIT\n\t");
        scanf("%d", &s);

        switch (s) {
            case 1:
                printf("\tEnter Password: ");
                for (i = 0; i < 6; i++) {
                    ch = getch();
                    password[i] = ch;
                    ch = '*';
                    printf("%c", ch);
                }
                password[i] = '\0';

                if (strcmp(password, q)) {
                    printf("\n\tWrong password try again");
                } else {
                    printf("\n\tAccess Granted\n");
                    printf("\t01.CREATE\n\t02.DELETE\n\t03.DISPLAY\n\t04.MAIN MENU\n");
                    scanf("%d", &y);

                    switch (y) {
                        case 1:
                            create();
                            break;
                        case 2:
                            edit();
                            break;
                        case 3:
                            display();
                            break;
                        case 4:
                            break;
                        default:
                            printf("INVALID CHOICE\n");
                    }
                }
                break;

            case 2:
                printf("\t01.BILL\n\t02.INVOICE\n\t03.MY CART\n\t04.MAIN MENU\n");
                scanf("%d", &y);

                switch (y) {
                    case 1:
                        bill();
                        break;
                    case 2:
                        invoice();
                        break;
                    case 3:
                        display2();
                        break;
                    case 4:
                        break;
                    default:
                        printf("INVALID CHOICE\n");
                }
                break;

            case 3:
                printf("Exiting..\n");
                break;

            default:
                printf("INVALID CHOICE\n");
        }

    } while (s != 3);

    return 0;
}

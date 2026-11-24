#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX 500
#define FILENAME "artnenotech.txt"
#define ORDERS_FILENAME "artorders.txt"

struct art
{
    char name[20];
    char brand[20];
    int price;
    int quantity;
    char color[20];
    int discount;
    char connectivity[15];
    int warranty_months;
    char status[20];
};

struct order
{
    char customer_name[20];
    char product_name[20];
    char product_brand[20];
    int quantity_ordered;
    int total_price;
};

// HELPER FUNCTION

void strip_line(char *s)
{
    int len = strlen(s);
    if (len > 0 && s[len - 1] == '\n')
    {
        s[len - 1] = '\0';
    }
}

void clear_input()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
    {
    }
}

int positive_int(const char *message)
{
    int value;
    while (1)
    {
        printf("%s", message);
        if (scanf("%d", &value) != 1)
        {
            printf("Invalid input. Please enter a valid number.\n");
            clear_input();
            continue;
        }
        clear_input();
        if (value < 0)
        {
            printf("Negative values are not allowed.\n");
            continue;
        }
        return value;
    }
}

int price_after_discount(struct art p)
{
    return p.price - (p.price * p.discount / 100);
}

void print_product(struct art p, int i)
{
    printf("\n%d| Name              : %s\n", i + 1, p.name);
    printf("   Brand             : %s\n", p.brand);
    printf("   Price             : %d\n", p.price);
    printf("   Quantity          : %d\n", p.quantity);
    printf("   Color             : %s\n", p.color);
    printf("   Discount          : %d%%\n", p.discount);
    printf("   Price After Disc. : %d\n", price_after_discount(p));
    printf("   Connectivity      : %s\n", p.connectivity);
    printf("   Warranty (Months) : %d\n", p.warranty_months);
    printf("   Status            : %s\n", p.status);
}

void print_order(struct order o, int i)
{
    printf("\n%d| Customer Name : %s\n", i + 1, o.customer_name);
    printf("   Product Name  : %s\n", o.product_name);
    printf("   Product Brand : %s\n", o.product_brand);
    printf("   Quantity      : %d\n", o.quantity_ordered);
    printf("   Total Price   : %d\n", o.total_price);
}

int find_product(struct art stock[], int count, const char *name, const char *brand)
{
    for (int i = 0; i < count; i++)
    {
        if (strcmp(stock[i].name, name) == 0 &&
            strcmp(stock[i].brand, brand) == 0)
        {
            return i;
        }
    }
    return -1;
}

// SAVE Function

void save(struct art stock[], int count)
{
    FILE *fp = fopen(FILENAME, "w");
    if (fp == NULL)
    {
        printf("\nError: Could not save data to file.\n");
        return;
    }

    // Heading Column Name
    fprintf(fp, "%-4s %-13s %-13s %-7s %-9s %-10s %-9s %-13s %-9s %-12s\n",
            "No", "Name", "Brand", "Price", "Quantity",
            "Color", "Discount", "Connectivity", "Warranty", "Status");

    for (int i = 0; i < count; i++)
    {
        fprintf(fp, "%-4d %-13s %-13s %-7d %-9d %-10s %-9d %-13s %-9d %-12s\n",
                i + 1,
                stock[i].name, stock[i].brand,
                stock[i].price, stock[i].quantity,
                stock[i].color, stock[i].discount,
                stock[i].connectivity, stock[i].warranty_months,
                stock[i].status);
    }

    fclose(fp);
}

// LOAD Function
int load(struct art stock[], int max_count)
{
    FILE *fp = fopen(FILENAME, "r");
    if (fp == NULL)
    {
        return 0;
    }

    char header_line[300];
    fgets(header_line, sizeof(header_line), fp); // Skip the header line

    int count = 0;
    int serial;

    while (count < max_count &&
           fscanf(fp, "%d %19s %19s %d %d %19s %d %14s %d %19s",
                  &serial,
                  stock[count].name,
                  stock[count].brand,
                  &stock[count].price,
                  &stock[count].quantity,
                  stock[count].color,
                  &stock[count].discount,
                  stock[count].connectivity,
                  &stock[count].warranty_months,
                  stock[count].status) == 10)
    {
        count++;
    }

    fclose(fp);
    return count;
}

// SAVE Orders

void save_orders(struct order orders[], int order_count)
{
    FILE *fp = fopen(ORDERS_FILENAME, "w");
    if (fp == NULL)
    {
        printf("\nError: Could not save order data to file.\n");
        return;
    }

    fprintf(fp, "%-4s %-15s %-13s %-13s %-9s %-9s\n",
            "No", "Customer", "Product", "Brand", "Quantity", "Total");

    for (int i = 0; i < order_count; i++)
    {
        fprintf(fp, "%-4d %-15s %-13s %-13s %-9d %-9d\n",
                i + 1,
                orders[i].customer_name,
                orders[i].product_name,
                orders[i].product_brand,
                orders[i].quantity_ordered,
                orders[i].total_price);
    }

    fclose(fp);
}

// LOAD Orders
int load_orders(struct order orders[], int max_count)
{
    FILE *fp = fopen(ORDERS_FILENAME, "r");
    if (fp == NULL)
    {
        return 0;
    }

    char header_line[300];
    fgets(header_line, sizeof(header_line), fp); // Skip header line

    int order_count = 0;
    int serial;

    while (order_count < max_count &&
           fscanf(fp, "%d %19s %19s %19s %d %d",
                  &serial,
                  orders[order_count].customer_name,
                  orders[order_count].product_name,
                  orders[order_count].product_brand,
                  &orders[order_count].quantity_ordered,
                  &orders[order_count].total_price) == 6)
    {
        order_count++;
    }

    fclose(fp);
    return order_count;
}

// MENU FUNCTIONS

// 1. Add Product
void add_product(struct art stock[], int *count)
{
    if (*count >= MAX)
    {
        printf("\nStock is full. Cannot add more products.\n");
        return;
    }

    int i = *count;

    printf("\nName: ");
    fgets(stock[i].name, sizeof(stock[i].name), stdin);
    strip_line(stock[i].name);

    printf("Brand: ");
    fgets(stock[i].brand, sizeof(stock[i].brand), stdin);
    strip_line(stock[i].brand);

    stock[i].price = positive_int("Price: ");
    stock[i].quantity = positive_int("Quantity: ");

    printf("Color: ");
    fgets(stock[i].color, sizeof(stock[i].color), stdin);
    strip_line(stock[i].color);

    stock[i].discount = positive_int("Discount (%): ");

    printf("Connectivity (Wired/Wireless/Bluetooth): ");
    fgets(stock[i].connectivity, sizeof(stock[i].connectivity), stdin);
    strip_line(stock[i].connectivity);

    stock[i].warranty_months = positive_int("Warranty (in months): ");

    printf("Status (Available/OutOfStock): ");
    fgets(stock[i].status, sizeof(stock[i].status), stdin);
    strip_line(stock[i].status);

    (*count)++;
    save(stock, *count);
    printf("\nProduct added successfully!\n");
}

// 2. View All Products
void view_all_products(struct art stock[], int count)
{
    if (count == 0)
    {
        printf("\nNo products found.\n");
        return;
    }

    for (int i = 0; i < count; i++)
    {
        print_product(stock[i], i);
    }
}

// 3. Search Product
void search_product(struct art stock[], int count)
{
    char keyword[20];
    printf("\nEnter Name or Brand to search: ");
    fgets(keyword, sizeof(keyword), stdin);
    strip_line(keyword);

    int found = 0;
    for (int i = 0; i < count; i++)
    {
        if (strcmp(stock[i].name, keyword) == 0 ||
            strcmp(stock[i].brand, keyword) == 0)
        {
            print_product(stock[i], i);
            found = 1;
        }
    }

    if (!found)
    {
        printf("\nNo product found matching \"%s\"\n", keyword);
    }
}

// 4. Update Product
void update_product(struct art stock[], int count)
{
    char up_name[20], up_brand[20];

    printf("\nEnter the Name of the product to update: ");
    fgets(up_name, sizeof(up_name), stdin);
    strip_line(up_name);

    printf("Enter the Brand of the product: ");
    fgets(up_brand, sizeof(up_brand), stdin);
    strip_line(up_brand);

    int i = find_product(stock, count, up_name, up_brand);
    if (i == -1)
    {
        printf("\nNo product found with Name \"%s\" and Brand \"%s\"\n", up_name, up_brand);
        return;
    }

    printf("\nCurrent details:\n");
    print_product(stock[i], i);

    printf("\nEnter new details below:\n");

    stock[i].price = positive_int("Price: ");
    stock[i].quantity = positive_int("Quantity: ");

    printf("Color: ");
    fgets(stock[i].color, sizeof(stock[i].color), stdin);
    strip_line(stock[i].color);

    stock[i].discount = positive_int("Discount (%): ");

    printf("Connectivity (Wired/Wireless/Bluetooth): ");
    fgets(stock[i].connectivity, sizeof(stock[i].connectivity), stdin);
    strip_line(stock[i].connectivity);

    stock[i].warranty_months = positive_int("Warranty (in months): ");

    printf("Status (Available/OutOfStock): ");
    fgets(stock[i].status, sizeof(stock[i].status), stdin);
    strip_line(stock[i].status);

    save(stock, count);
    printf("\nProduct updated successfully!\n");
}

// 5. Remove Product
void remove_product(struct art stock[], int *count)
{
    char rm_name[20], rm_brand[20];

    printf("\nEnter the Name of the product to remove: ");
    fgets(rm_name, sizeof(rm_name), stdin);
    strip_line(rm_name);

    printf("Enter the Brand of the product: ");
    fgets(rm_brand, sizeof(rm_brand), stdin);
    strip_line(rm_brand);

    int i = find_product(stock, *count, rm_name, rm_brand);
    if (i == -1)
    {
        printf("\nNo product found with Name \"%s\" and Brand \"%s\"\n", rm_name, rm_brand);
        return;
    }

    for (int j = i; j < *count - 1; j++)
    {
        stock[j] = stock[j + 1];
    }
    (*count)--;
    save(stock, *count);
    printf("\nProduct \"%s\" (Brand: %s) removed successfully.\n", rm_name, rm_brand);
}

// 6. Place Order
void place_order(struct art stock[], int count, struct order orders[], int *order_count)
{
    if (count == 0)
    {
        printf("\nNo products available. Please add a product first.\n");
        return;
    }

    char ord_name[20], ord_brand[20], cust_name[20];

    printf("\nEnter Customer Name: ");
    fgets(cust_name, sizeof(cust_name), stdin);
    strip_line(cust_name);

    printf("Enter Product Name: ");
    fgets(ord_name, sizeof(ord_name), stdin);
    strip_line(ord_name);

    printf("Enter Product Brand: ");
    fgets(ord_brand, sizeof(ord_brand), stdin);
    strip_line(ord_brand);

    int i = find_product(stock, count, ord_name, ord_brand);
    if (i == -1)
    {
        printf("\nNo product found with Name \"%s\" and Brand \"%s\"\n", ord_name, ord_brand);
        return;
    }

    int qty = positive_int("Enter Quantity to Order: ");

    if (qty > stock[i].quantity)
    {
        printf("\nNot enough stock available. Only %d unit(s) in stock.\n", stock[i].quantity);
        return;
    }

    if (*order_count >= MAX)
    {
        printf("\nOrder list is full. Cannot place more orders.\n");
        return;
    }

    // Reduce stock quantity
    stock[i].quantity -= qty;
    if (stock[i].quantity == 0)
    {
        strcpy(stock[i].status, "OutOfStock");
    }
    save(stock, count);

    // Record the order
    int j = *order_count;
    strcpy(orders[j].customer_name, cust_name);
    strcpy(orders[j].product_name, stock[i].name);
    strcpy(orders[j].product_brand, stock[i].brand);
    orders[j].quantity_ordered = qty;
    orders[j].total_price = price_after_discount(stock[i]) * qty;

    (*order_count)++;
    save_orders(orders, *order_count);

    printf("\nOrder placed successfully! Total Price: %d\n", orders[j].total_price);
}

// 7. View All Orders
void view_all_orders(struct order orders[], int order_count)
{
    if (order_count == 0)
    {
        printf("\nNo orders found.\n");
        return;
    }

    for (int i = 0; i < order_count; i++)
    {
        print_order(orders[i], i);
    }
}

// MAIN PROGRAM

int main()
{
    struct art stock[MAX];
    struct order orders[MAX];
    int count = 0;
    int order_count = 0;
    int choice;

    count = load(stock, MAX);
    if (count > 0)
    {
        printf("\n%d product(s) loaded from file.\n", count);
    }

    order_count = load_orders(orders, MAX);
    if (order_count > 0)
    {
        printf("%d order(s) loaded from file.\n", order_count);
    }

    do
    {
        printf("\n===== PRODUCT MANAGEMENT SYSTEM =====\n");
        printf("1. Add Product\n");
        printf("2. View All Products\n");
        printf("3. Search Product\n");
        printf("4. Update Product\n");
        printf("5. Remove Product\n");
        printf("6. Place Order\n");
        printf("7. View All Orders\n");
        printf("8. Exit\n");
        printf("\nEnter your choice: ");

        if (scanf("%d", &choice) != 1)
        {
            printf("\nInvalid option. Please try again.\n");
            clear_input();
            choice = 0;
            continue;
        }
        clear_input();

        if (choice == 1)
        {
            add_product(stock, &count);
        }
        else if (choice == 2)
        {
            view_all_products(stock, count);
        }
        else if (choice == 3)
        {
            search_product(stock, count);
        }
        else if (choice == 4)
        {
            update_product(stock, count);
        }
        else if (choice == 5)
        {
            remove_product(stock, &count);
        }
        else if (choice == 6)
        {
            place_order(stock, count, orders, &order_count);
        }
        else if (choice == 7)
        {
            view_all_orders(orders, order_count);
        }
        else if (choice != 8)
        {
            printf("\nInvalid option. Please try again.\n");
        }

    } while (choice != 8);

    printf("\nProgram terminated. Goodbye!\n\n");
    return 0;
}
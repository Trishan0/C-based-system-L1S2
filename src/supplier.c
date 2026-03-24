#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "types.h"
#include "supplier.h"
#include "input_utils.h"

static Supplier* head = NULL;

static Supplier* findSupplierById(int id) {
    Supplier* cur = head;
    while (cur != NULL) {
        if (cur->id == id) return cur;
        cur = cur->next;
    }
    return NULL;
}

void addSupplier(void) {
    Supplier* node = (Supplier*)malloc(sizeof(Supplier));
    if (node == NULL) {
        printf("Memory allocation failed.\n");
        return;
    }

    if (!readPositiveInt("Enter supplier id: ", &node->id)) {
        free(node);
        return;
    }

    if (findSupplierById(node->id) != NULL) {
        printf("Supplier with this ID already exists.\n");
        free(node);
        return;
    }

    if (!readLine("Enter supplier name: ", node->name, sizeof(node->name))) {
        free(node);
        return;
    }
    if (!readLine("Enter supplier phone: ", node->phone, sizeof(node->phone))) {
        free(node);
        return;
    }

    node->next = head;
    head = node;
    printf("Supplier added successfully.\n");
}

void searchSupplier(void) {
    int id;
    if (!readPositiveInt("Enter supplier id to search: ", &id)) return;

    Supplier* s = findSupplierById(id);
    if (s == NULL) {
        printf("\n  [!] Supplier not found.\n");
        return;
    }

    printf("\n");
    printf("  +-------------------------------------------------------------+\n");
    printf("  |                    SUPPLIER SEARCH RESULT                   |\n");
    printf("  +--------------+----------------------------------------------+\n");
    printf("  | Supplier ID  | %-44d |\n", s->id);
    printf("  | Name         | %-44s |\n", s->name);
    printf("  | Phone        | %-44s |\n", s->phone);
    printf("  +--------------+----------------------------------------------+\n");
}

void updateSupplier(void) {
    int id;
    if (!readPositiveInt("Enter supplier id to update: ", &id)) return;

    Supplier* s = findSupplierById(id);
    if (s == NULL) {
        printf("Supplier not found.\n");
        return;
    }

    if (!readLine("Enter new supplier name: ", s->name, sizeof(s->name))) return;
    if (!readLine("Enter new supplier phone: ", s->phone, sizeof(s->phone))) return;
    printf("Supplier updated successfully.\n");
}

void deleteSupplier(void) {
    int id;
    if (!readPositiveInt("Enter supplier id to delete: ", &id)) return;

    Supplier* cur = head;
    Supplier* prev = NULL;

    while (cur != NULL && cur->id != id) {
        prev = cur;
        cur = cur->next;
    }

    if (cur == NULL) {
        printf("Supplier not found.\n");
        return;
    }

    if (prev == NULL) head = cur->next;
    else prev->next = cur->next;

    free(cur);
    printf("Supplier deleted successfully.\n");
}

void displaySuppliers(void) {
    if (head == NULL) {
        printf("\n  [!] No suppliers available.\n");
        return;
    }

    printf("\n");
    printf("  +-------------------------------------------------------------------+\n");
    printf("  |                      SUPPLIER MANAGEMENT TABLE                    |\n");
    printf("  +----------+--------------------------+------------------------------+\n");
    printf("  | Supplier | Name                     | Phone                        |\n");
    printf("  | ID       |                          |                              |\n");
    printf("  +----------+--------------------------+------------------------------+\n");

    Supplier* cur = head;
    while (cur != NULL) {
        printf("  | %-8d | %-24s | %-28s |\n",
               cur->id,
               cur->name,
               cur->phone);
        cur = cur->next;
    }

    printf("  +----------+--------------------------+------------------------------+\n");
}

void countSuppliers(void) {
    int count = 0;
    Supplier* cur = head;
    while (cur != NULL) {
        count++;
        cur = cur->next;
    }
    printf("Total suppliers: %d\n", count);
}

void supplierMenu(void) {
    int choice = -1;
    do {
        printf("\n=== Supplier Management (SLL) ===\n");
        printf("1. Add supplier\n");
        printf("2. Search supplier\n");
        printf("3. Update supplier\n");
        printf("4. Delete supplier\n");
        printf("5. Display suppliers\n");
        printf("6. Count suppliers\n");
        printf("0. Back\n");
        if (!readInt("Enter choice: ", &choice)) continue;

        switch (choice) {
            case 1: addSupplier(); break;
            case 2: searchSupplier(); break;
            case 3: updateSupplier(); break;
            case 4: deleteSupplier(); break;
            case 5: displaySuppliers(); break;
            case 6: countSuppliers(); break;
            case 0: break;
            default: printf("Invalid choice.\n");
        }
    } while (choice != 0);
}

static void preloadOne(int id, const char* name, const char* phone) {
    Supplier* node = (Supplier*)malloc(sizeof(Supplier));
    if (!node) return;
    node->id = id;
    strncpy(node->name, name, sizeof(node->name) - 1);
    node->name[sizeof(node->name) - 1] = '\0';
    strncpy(node->phone, phone, sizeof(node->phone) - 1);
    node->phone[sizeof(node->phone) - 1] = '\0';
    node->next = head;
    head = node;
}

__attribute__((constructor))
static void preloadSuppliers(void) {
    preloadOne(203, "Lanka Traders", "0771234567");
    preloadOne(202, "Fresh Foods Ltd", "0712345678");
    preloadOne(201, "Ceylon Distributors", "0769988776");
}

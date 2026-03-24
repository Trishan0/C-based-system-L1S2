#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "types.h"
#include "purchase_history.h"
#include "input_utils.h"

static Purchase* head = NULL;
static Purchase* tail = NULL;

static Purchase* findPurchaseByRecordId(int recordId) {
    Purchase* cur = head;
    while (cur != NULL) {
        if (cur->recordId == recordId) return cur;
        cur = cur->next;
    }
    return NULL;
}

void addPurchaseRecord(void) {
    Purchase* node = (Purchase*)malloc(sizeof(Purchase));
    if (node == NULL) {
        printf("Memory allocation failed.\n");
        return;
    }

    if (!readPositiveInt("Enter record id: ", &node->recordId)) {
        free(node);
        return;
    }

    if (findPurchaseByRecordId(node->recordId) != NULL) {
        printf("Record ID already exists.\n");
        free(node);
        return;
    }

    if (!readPositiveInt("Enter customer id: ", &node->customerId)) {
        free(node);
        return;
    }
    if (!readLine("Enter customer name: ", node->customerName, sizeof(node->customerName))) {
        free(node);
        return;
    }
    if (!readNonNegativeFloat("Enter purchase amount: ", &node->amount)) {
        free(node);
        return;
    }

    node->prev = tail;
    node->next = NULL;

    if (tail != NULL) tail->next = node;
    else head = node;
    tail = node;

    printf("Purchase record added successfully.\n");
}

void searchPurchaseRecord(void) {
    int recordId;
    if (!readPositiveInt("Enter record id to search: ", &recordId)) return;

    Purchase* p = findPurchaseByRecordId(recordId);
    if (p == NULL) {
        printf("\n  [!] Purchase record not found.\n");
        return;
    }

    printf("\n");
    printf("  +-------------------------------------------------------------+\n");
    printf("  |                 PURCHASE RECORD SEARCH RESULT               |\n");
    printf("  +--------------+----------------------------------------------+\n");
    printf("  | Record ID    | %-44d |\n", p->recordId);
    printf("  | Customer ID  | %-44d |\n", p->customerId);
    printf("  | Name         | %-44s |\n", p->customerName);
    printf("  | Amount       | %-44.2f |\n", p->amount);
    printf("  +--------------+----------------------------------------------+\n");
}

void updatePurchaseRecord(void) {
    int recordId;
    if (!readPositiveInt("Enter record id to update: ", &recordId)) return;

    Purchase* p = findPurchaseByRecordId(recordId);
    if (p == NULL) {
        printf("Purchase record not found.\n");
        return;
    }

    if (!readLine("Enter new customer name: ", p->customerName, sizeof(p->customerName))) return;
    if (!readNonNegativeFloat("Enter new amount: ", &p->amount)) return;
    printf("Purchase record updated successfully.\n");
}

void deletePurchaseRecord(void) {
    int recordId;
    if (!readPositiveInt("Enter record id to delete: ", &recordId)) return;

    Purchase* node = findPurchaseByRecordId(recordId);
    if (node == NULL) {
        printf("Purchase record not found.\n");
        return;
    }

    if (node->prev != NULL) node->prev->next = node->next;
    else head = node->next;

    if (node->next != NULL) node->next->prev = node->prev;
    else tail = node->prev;

    free(node);
    printf("Purchase record deleted successfully.\n");
}

void traversePurchasesForward(void) {
    if (head == NULL) {
        printf("\n  [!] No purchase records available.\n");
        return;
    }

    printf("\n");
    printf("  +-----------------------------------------------------------------------------------+\n");
    printf("  |                    CUSTOMER PURCHASE HISTORY TABLE (FORWARD)                      |\n");
    printf("  +----------+------------+--------------------------+-------------------------------+\n");
    printf("  | RecordID | CustomerID | Customer Name            | Amount                        |\n");
    printf("  +----------+------------+--------------------------+-------------------------------+\n");

    Purchase* cur = head;
    while (cur != NULL) {
        printf("  | %-8d | %-10d | %-24s | %-29.2f |\n",
               cur->recordId,
               cur->customerId,
               cur->customerName,
               cur->amount);
        cur = cur->next;
    }

    printf("  +----------+------------+--------------------------+-------------------------------+\n");
}

void traversePurchasesBackward(void) {
    if (tail == NULL) {
        printf("\n  [!] No purchase records available.\n");
        return;
    }

    printf("\n");
    printf("  +-----------------------------------------------------------------------------------+\n");
    printf("  |                   CUSTOMER PURCHASE HISTORY TABLE (BACKWARD)                      |\n");
    printf("  +----------+------------+--------------------------+-------------------------------+\n");
    printf("  | RecordID | CustomerID | Customer Name            | Amount                        |\n");
    printf("  +----------+------------+--------------------------+-------------------------------+\n");

    Purchase* cur = tail;
    while (cur != NULL) {
        printf("  | %-8d | %-10d | %-24s | %-29.2f |\n",
               cur->recordId,
               cur->customerId,
               cur->customerName,
               cur->amount);
        cur = cur->prev;
    }

    printf("  +----------+------------+--------------------------+-------------------------------+\n");
}
void totalPurchasesByCustomer(void) {
    int customerId;
    float total = 0.0f;
    int found = 0;

    if (!readPositiveInt("Enter customer id: ", &customerId)) return;

    Purchase* cur = head;
    while (cur != NULL) {
        if (cur->customerId == customerId) {
            total += cur->amount;
            found = 1;
        }
        cur = cur->next;
    }

    if (!found) {
        printf("No purchase records found for customer ID %d.\n", customerId);
        return;
    }

    printf("Total purchases by customer %d: %.2f\n", customerId, total);
}

void purchaseHistoryMenu(void) {
    int choice = -1;
    do {
        printf("\n=== Customer Purchase History (DLL) ===\n");
        printf("1. Add purchase record\n");
        printf("2. Search purchase record\n");
        printf("3. Update purchase record\n");
        printf("4. Delete purchase record\n");
        printf("5. Traverse forward\n");
        printf("6. Traverse backward\n");
        printf("7. Total purchases by customer\n");
        printf("0. Back\n");
        if (!readInt("Enter choice: ", &choice)) continue;

        switch (choice) {
            case 1: addPurchaseRecord(); break;
            case 2: searchPurchaseRecord(); break;
            case 3: updatePurchaseRecord(); break;
            case 4: deletePurchaseRecord(); break;
            case 5: traversePurchasesForward(); break;
            case 6: traversePurchasesBackward(); break;
            case 7: totalPurchasesByCustomer(); break;
            case 0: break;
            default: printf("Invalid choice.\n");
        }
    } while (choice != 0);
}

static void preloadRecord(int recordId, int customerId, const char* name, float amount) {
    Purchase* node = (Purchase*)malloc(sizeof(Purchase));
    if (!node) return;
    node->recordId = recordId;
    node->customerId = customerId;
    strncpy(node->customerName, name, sizeof(node->customerName) - 1);
    node->customerName[sizeof(node->customerName) - 1] = '\0';
    node->amount = amount;
    node->prev = tail;
    node->next = NULL;
    if (tail) tail->next = node;
    else head = node;
    tail = node;
}

__attribute__((constructor))
static void preloadPurchases(void) {
    preloadRecord(301, 1, "Nimal", 2450.00f);
    preloadRecord(302, 2, "Kasuni", 1890.00f);
    preloadRecord(303, 1, "Nimal", 760.00f);
    preloadRecord(304, 3, "Tharindu", 3320.00f);
}

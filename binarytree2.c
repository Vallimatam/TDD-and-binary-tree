// To perform CRUDS operations using binary tree

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <windows.h>
#include <psapi.h>

#define MAX_OPERATION_LENGTH 10
#define MAX_ID_LENGTH 10
#define MAX_DESCRIPTION_LENGTH 100
#define DATA_FILE "Items.dat"

typedef struct ItemDetails 
{
    char ID[MAX_ID_LENGTH];
    char Description[MAX_DESCRIPTION_LENGTH];
    float Price;
} ItemDetails;

typedef struct Node 
{
    struct Node *LeftNode;
    ItemDetails ItemRecord;
    struct Node *RightNode;
} Node;

FILE *FpItems;
Node *RootNode = NULL;
char *Operation;

void ShowMenu();
void LoadBinaryTree();
void GetDetails(ItemDetails *Item);
void AddItem();
void ShowAllItems();
void UpdateItemDetails(char *);
void DeleteItem(Node **, const char *);
void SearchItem(const char *);
char *GetID(char *);
void PrintSuccessMessage(char *, const char *);
void GetProcessMemoryUsage();

int main() 
{
	time_t StartTime, EndTime;
	StartTime = clock();
    Operation = malloc(MAX_OPERATION_LENGTH);
    LoadBinaryTree();
    ShowMenu();
    EndTime = clock();
	double ExecutionTimeInSeconds = (double)(EndTime - StartTime) / CLOCKS_PER_SEC;
    printf("Execution time: %.3f seconds\n", ExecutionTimeInSeconds);
    GetProcessMemoryUsage();
    free(Operation);
    return 0;
}

char* GenerateRandomId() 
{
    static char RandomID[MAX_ID_LENGTH]; 
    srand(time(NULL));
    int Counter;
    for (Counter = 0; Counter < MAX_ID_LENGTH; ++Counter) 
    {
        RandomID[Counter] = '0' + rand() % 10;
    }
    RandomID[MAX_ID_LENGTH - 1] = '\0';
    return RandomID;
}

void GetProcessMemoryUsage() 
{
    PROCESS_MEMORY_COUNTERS_EX pmc;
    HANDLE Process = GetCurrentProcess();

    if (GetProcessMemoryInfo(Process, (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc))) 
    {
        SIZE_T memoryUsedBytes = pmc.PrivateUsage;
        printf("Memory used by the process: %u bytes\n", (unsigned int)memoryUsedBytes);
    }
    else 
    {
        printf("Failed to get process memory details.\n");
    }
}

void LoadBinaryTree() 
{
    ItemDetails ItemRecord;
    Node **CurrentNodePtr, *NewNode, *CurrentNode;
    FpItems = fopen(DATA_FILE, "rb");
    while (fread(&ItemRecord, sizeof(ItemDetails), 1, FpItems)) 
    {
        NewNode = (Node *)malloc(sizeof(Node));
        NewNode->ItemRecord = ItemRecord;
        NewNode->LeftNode = NULL;
        NewNode->RightNode = NULL;

        CurrentNodePtr = &RootNode;

        while (*CurrentNodePtr != NULL) 
        {
            CurrentNode = *CurrentNodePtr;
            if (strcmp(CurrentNode->ItemRecord.ID, ItemRecord.ID) < 0) 
            {
                CurrentNodePtr = &CurrentNode->RightNode;
            } 
            else 
            {
                CurrentNodePtr = &CurrentNode->LeftNode;
            }
        }
        *CurrentNodePtr = NewNode;
    }
    fclose(FpItems);
}

void SaveBinaryTreeHelper(Node *Node, FILE *FpItems) 
{
    if (Node == NULL) 
    {
        return;
    }
    SaveBinaryTreeHelper(Node->LeftNode, FpItems);
    SaveBinaryTreeHelper(Node->RightNode, FpItems);
    fwrite(&(Node->ItemRecord), sizeof(ItemDetails), 1, FpItems);
}

void SaveBinaryTree() 
{
    FpItems = fopen(DATA_FILE, "wb");
    SaveBinaryTreeHelper(RootNode, FpItems);
    fclose(FpItems);
}

Node* SearchItemHelper(Node* Node, const char* ID) 
{
    if (Node == NULL || strcmp(Node->ItemRecord.ID, ID) == 0) 
    {
        return Node;
    }

    if (strcmp(ID, Node->ItemRecord.ID) < 0) 
    {
        return SearchItemHelper(Node->LeftNode, ID);
    } 
    else 
    {
        return SearchItemHelper(Node->RightNode, ID);
    }
}

void GetDetails(ItemDetails *Item) 
{
    strcpy(Item->ID, GenerateRandomId());
    printf("Enter Item Description: ");
    scanf(" %99[^\n]s", Item->Description);
    printf("Enter Item Price: $");
    scanf("%f", &Item->Price);
}

void AddItem() 
{
    Node* NewNode = (Node*)malloc(sizeof(Node));
    GetDetails(&NewNode->ItemRecord);
    NewNode->LeftNode = NULL;
    NewNode->RightNode = NULL;

    Node *TargetNode = SearchItemHelper(RootNode, NewNode->ItemRecord.ID);
    if (TargetNode != NULL)
    {
        strcpy(NewNode->ItemRecord.ID, GenerateRandomId());
    }
    if (RootNode == NULL) 
    {
        RootNode = NewNode;
    }
    else 
    {
        Node* ParentNode = NULL;
        Node* CurrentNode = RootNode;
        while (CurrentNode != NULL) 
        {
            ParentNode = CurrentNode;
            if (strcmp(NewNode->ItemRecord.ID, CurrentNode->ItemRecord.ID) < 0) 
            {
                CurrentNode = CurrentNode->LeftNode;
            } 
            else 
            {
                CurrentNode = CurrentNode->RightNode;
            }
        }
        if (strcmp(NewNode->ItemRecord.ID, ParentNode->ItemRecord.ID) < 0) 
        {
            ParentNode->LeftNode = NewNode;
        } 
        else 
        {
            ParentNode->RightNode = NewNode;
        }
    }
    Operation = "adde";
    PrintSuccessMessage(Operation, NewNode->ItemRecord.ID);
    SaveBinaryTree();
}

void ShowAllItemsHelper(Node *Node, int *Counter) 
{
    if (Node == NULL) 
    {
        return;
    }
    ShowAllItemsHelper(Node->LeftNode, Counter);
    printf("\nItem %d: \n", ++(*Counter));
    printf("Item ID: %s\n", Node->ItemRecord.ID);
    printf("Item Description: %s\n", Node->ItemRecord.Description);
    printf("Item Price: $%.2f \n", Node->ItemRecord.Price);
    ShowAllItemsHelper(Node->RightNode, Counter);
}

void ShowAllItems() 
{
    printf("Showing all Items:\n");
    if (RootNode == NULL) 
    {
        printf("No Items found.\n");
        return;
    }
    int Counter = 0;
    ShowAllItemsHelper(RootNode, &Counter);
}

void UpdateItemDetails(char *ID) 
{
    char NewDescription[MAX_DESCRIPTION_LENGTH];
    float NewPrice;
    Node *TargetNode = SearchItemHelper(RootNode, ID);
    if (TargetNode == NULL) 
    {
        printf("Item with ID %s does not exist.\n", ID);
        return;
    }
    printf("Enter the new description of the Item: ");
    // scanf("%s", NewDescription[MAX_DESCRIPTION_LENGTH]);
    scanf("%s", TargetNode->ItemRecord.Description);
    printf("Enter the new price of the Item: ");
    scanf("%f", &NewPrice);
    // TargetNode->ItemRecord.Description = NewDescription;
    TargetNode->ItemRecord.Price = NewPrice;
    Operation = "update";
    PrintSuccessMessage(Operation, ID);
    SaveBinaryTree();
}

Node* FindLeafNode(Node* Node) 
{
    while (Node->LeftNode != NULL) 
    {
        Node = Node->LeftNode;
    }
    return Node;
}

void DeleteItem(Node** RootPtr, const char* ID) 
{
    if (*RootPtr == NULL) 
    {
        printf("No Items found.\n");
        return;
    }

    Node* NodeToDelete = SearchItemHelper(*RootPtr, ID);
    if (NodeToDelete == NULL) 
    {
        printf("Item with ID %s does not exist.\n", ID);
        return;
    }

    if (NodeToDelete->LeftNode == NULL) 
    {
        *RootPtr = NodeToDelete->RightNode;
        free(NodeToDelete);
    } 
    else if (NodeToDelete->RightNode == NULL) 
    {
        *RootPtr = NodeToDelete->LeftNode;
        free(NodeToDelete);
    } 
    else 
    {
        if (*RootPtr == NodeToDelete) 
        {
            Node* Temp = FindLeafNode(NodeToDelete->RightNode);
            strcpy(NodeToDelete->ItemRecord.ID, Temp->ItemRecord.ID);
            DeleteItem(&(NodeToDelete->RightNode), Temp->ItemRecord.ID);
        } 
        else 
        {
            Node* Temp = FindLeafNode(NodeToDelete->RightNode);
            strcpy(NodeToDelete->ItemRecord.ID, Temp->ItemRecord.ID);
            DeleteItem(&(NodeToDelete->RightNode), Temp->ItemRecord.ID);
        }
    }
    Operation = "delete";
    PrintSuccessMessage(Operation, ID);
    SaveBinaryTree();
}

void SearchItem(const char *ID) 
{
    Node* result = SearchItemHelper(RootNode, ID);
    if (result != NULL) 
    {
        printf("Item found:\n");
        printf("Item ID: %s\n", result->ItemRecord.ID);
        printf("Item Description: %s\n", result->ItemRecord.Description);
        printf("Item price: $%.2f\n", result->ItemRecord.Price);
    } 
    else 
    {
        printf("Item with ID %s not found.\n", ID);
    }
}

char *GetID(char *Operation)
{
    char *ID = (char *)malloc(MAX_ID_LENGTH);
    printf("Enter Item id to %s: ", Operation);
    scanf("%s", ID);
    return ID;
}

void PrintSuccessMessage(char *Operation, const char *ID)
{
    printf("Item with id - %s %sd successfully.\n", ID, Operation);
}

void ShowMenu() 
{
    int choice;
    do 
    {
        printf("\n---------- SUPER MARKET ----------\n");
        printf("0. Exit\n");
        printf("1. Add a Item\n");
        printf("2. Show all Items\n");
        printf("3. Update Item details\n");
        printf("4. Delete a Item\n");
        printf("5. Search a Item\n");
        printf("------------------------------------\n\n");

        printf("Choose any option: ");
        scanf("%d", &choice);

        switch (choice) 
        {
            case 0: 
                printf("You chose to exit.\n");
                break;
            case 1: 
                AddItem();
                break;
            case 2: 
                ShowAllItems();
                break;
            case 3: 
                Operation = "update";
                UpdateItemDetails(GetID(Operation));
                break;
            case 4: 
                Operation = "delete";
                DeleteItem(&RootNode, GetID(Operation));
                break;
            case 5: 
                Operation = "search";
                SearchItem(GetID(Operation));
                break;
            default: 
                printf("You entered an invalid choice.\n");
        }
    } while (choice != 0);
}
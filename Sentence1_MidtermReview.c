#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Student
{
    int id;
    char name[50];
    float gpa;
    struct Student* next;
}Student;

Student* createNode(int id, char* name, float gpa){
    Student* newNode = (Student*)malloc(sizeof(Student));

    newNode->id = id;
    strcpy(newNode->name, name);
    newNode->gpa = gpa;

    newNode->next = NULL;

    return newNode;
}

void insertEnd(int id, char* name, float gpa, Student** head){
    Student* newNode = createNode(id, name, gpa);
    if (*head == NULL)
    {
        *head = newNode;
        return;
    }

    Student* current = *head;
    while (current->next != NULL)
    {
        current = current->next;
    }
    current->next = newNode;
    return;
}

void printList(Student* head){
    if (head == NULL)
    {
        printf("Khong co du lieu.\n");
        return;
    }
    

    printf("%-10s%-15s%-5s\n","ID","NAME","GPA");
    while (head != NULL)
    {
        printf("%-10d%-15s%-5.1f\n",head->id, head->name, head->gpa);
        head = head->next;
    }
    
}

Student* searchByID(Student* head, int id){
    if (head == NULL)
    {
        return NULL;
    }
    
    while (head != NULL)
    {
        if (head->id == id)
        {
            return head;
        }
        
        head = head->next;
    }
    return NULL;
}

void sortByGpa(Student** head){
    if (*head == NULL)
    {
        return;
    }
    
    Student* i = *head;
    while (i != NULL)
    {
        Student* j = i->next;
        while (j != NULL)
        {
            if (j->gpa > i->gpa)
            {
                int tempID = i->id;
                char tempName[50];
                strcpy(tempName, i->name);
                float tempGpa = i->gpa;

                i->id = j->id;
                strcpy(i->name,j->name);
                i->gpa = j->gpa;

                j->id = tempID;
                strcpy(j->name, tempName);
                j->gpa = tempGpa;
            }       
            j = j->next;
        }
        
        i = i->next;
    }
}

int main(){
    Student* head = NULL;
    //Them du lieu
    insertEnd(2, "Toan", 3.0, &head);
    insertEnd(4, "Binh", 1.0, &head);
    insertEnd(1, "Quoc", 2.0, &head);
    //In du lieu
    printList(head);
    //Tim kiem theo id
    printList(searchByID(head, 3));
    //Sap xep theo gpa
    sortByGpa(&head);
    printList(head);
    return 0;
}
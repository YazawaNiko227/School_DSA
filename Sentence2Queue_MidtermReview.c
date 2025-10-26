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

void enqueue(Student** head ,Student** front ,Student** rear, int id, char* name, float gpa){
    Student* newNode = createNode(id, name, gpa);
    if (*head == NULL)
    {
        *head = newNode;
        *front = *head;
        *rear = *head;
        return;
    }
    (*rear)->next = newNode;
    *rear = (*rear)->next;
}

Student* dequeue(Student** head ,Student** front ,Student** rear){
    if (*front == NULL)
    {
        return NULL;
    }
    
    Student* temp = *head;
    *head = (*head)->next;
    *front = *head;
    return temp;
}

void printQueue(Student* front){
    if (front == NULL)
    {
        return;
    }
    while (front != NULL)
    {
        printf("%-10d%-15s%-5.1f\n",front->id, front->name, front->gpa);
        front = front->next;
    }
}

int main(){
    Student* front = NULL;
    Student* rear = NULL;
    Student* head = NULL;

    enqueue(&head, &front, &rear, 3, "Toan", 3.0);
    enqueue(&head, &front, &rear, 6, "Doan", 2.0);
    enqueue(&head, &front, &rear, 1, "Boan", 5.0);
    printQueue(front);

    printf("--------\n");
    dequeue(&head, &front, &rear);
    printQueue(front);
    return 0;
}
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

void push(Student** top, int id, char* name, float gpa){
    Student* newNode = createNode(id,name,gpa);
    if (*top == NULL)
    {
        *top = newNode;
        return;
    }
    newNode->next = *top;
    *top = newNode;
}

Student* pop(Student** top){
    if (*top == NULL)
    {
        return NULL;
    }
    Student* temp = *top;
    *top = (*top)->next;
    free(temp);
    return *top;
}

void printStack(Student* top){
    while (top != NULL)
    {
        printf("%-10d%-15s%-5.1f\n",top->id, top->name, top->gpa);
        top = top->next;
    }
}

int main(){
    Student* top = NULL;
    push(&top, 2, "Toan", 4.0);
    push(&top, 4, "Binh", 2.0);
    push(&top, 3, "Quoc", 3.0);
    printStack(top);
    printf("-------------\n");
    pop(&top);
    printStack(top);

    return 0;
}
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Student
{
    int id;
    char name[50];
    float gpa;
    Student* next;
}Student;

Student* createNode(int id, char name, float gpa){
    Student* newNode = (Student*) malloc( sizeof(Student));
    newNode->id = id;
    newNode->name = name;
    newNode->gpa  = gpa;
    newNode->next = NULL;
    return newNode;
}

int main(int argc, char const *argv[])
{
    
    return 0;
}

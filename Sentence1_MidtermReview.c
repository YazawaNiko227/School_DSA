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
    Student* newNode = (Student*) malloc( sizeof(Student));
    newNode->id = id;
    strcpy(newNode->name, name);
    newNode->gpa  = gpa;
    newNode->next = NULL;
    return newNode;
}

void insertEnd(int id, char* name, float gpa, Student** head){
    if (*head == NULL)
    {
        *head = createNode(id, name, gpa);
        return;
    }
    
    Student* current = *head;
    while (current->next != NULL)
    {
        current = current->next;
    }
    current->next = createNode(id, name, gpa);
}

int main(int argc, char const *argv[])
{
    Student* head = NULL;

    printf("Nhap so luong sinh vien: ");
    int soLuong;
    scanf("%d", &soLuong);
    getchar();
    
    for (int i = 0; i < soLuong; i++)
    {
        int id;
        char name[50];
        float gpa;
        printf("Nhap sinh vien %d: \n", i);
        printf("Nhap id: "); scanf("%d",&id);
        getchar(); 
        printf("Nhap ten: "); fgets(name, sizeof(name), stdin); name[strcspn(name, "\n")] = '\0';
        printf("Nhap gpa: "); scanf("%f", &gpa);
        insertEnd(id, name, gpa, &head);
    }

    printf("%-10s%-20s%-10s\n","ID", "NAME", "GPA");
    for (int i = 0; i < 50; i++)
    {
        printf("-");
    }
    printf("\n");
    while (head != NULL)
    {
        printf("%-10d%-20s%-10.3f\n", head->id, head->name, head->gpa);
        head = head->next;
    }

    return 0;
}

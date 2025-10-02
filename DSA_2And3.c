#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Cấu trúc thông tin truyện
typedef struct Book {
	char id[20];
	char name[100];
	char author[50];
	char genre[30];
	int volume;
	int price_per_day;
	struct Book *next; // Dùng cho danh sách truyện
} Book;

// Cấu trúc thông tin khách hàng
typedef struct Customer {
	char name[50];
	char phone[20];
	struct Customer *next; // Dùng cho danh sách khách hàng
} Customer;

// Cấu trúc phiếu mượn
typedef struct BorrowRecord {
	char book_id[20];
	char customer_name[50];
	char borrow_date[11]; // dd/mm/yyyy
	char expected_return_date[11];
	char actual_return_date[11];
	int total_fee;
	struct BorrowRecord *next;
} BorrowRecord;

// Node cho Queue (dùng cho hàng chờ mượn truyện, nhập kho, kiểm định...)
typedef struct QueueNode {
	void *data;
	struct QueueNode *next;
} QueueNode;

// Queue dùng danh sách liên kết đơn
typedef struct Queue {
	QueueNode *front;
	QueueNode *rear;
} Queue;

// Khởi tạo Queue
void initQueue(Queue *q) {
	q->front = q->rear = NULL;
}

// Kiểm tra Queue rỗng
int isEmpty(Queue *q) {
	return (q->front == NULL);
}

// Thêm phần tử vào Queue
void enqueue(Queue *q, void *data) {
	QueueNode *newNode = (QueueNode*)malloc(sizeof(QueueNode));
	newNode->data = data;
	newNode->next = NULL;
	if (q->rear == NULL) {
		q->front = q->rear = newNode;
	} else {
		q->rear->next = newNode;
		q->rear = newNode;
	}
}

// Lấy phần tử ra khỏi Queue
void* dequeue(Queue *q) {
	if (isEmpty(q)) return NULL;
	QueueNode *temp = q->front;
	void *data = temp->data;
	q->front = q->front->next;
	if (q->front == NULL) q->rear = NULL;
	free(temp);
	return data;
}

// Giải phóng Queue
void freeQueue(Queue *q) {
	while (!isEmpty(q)) {
		dequeue(q);
	}
}


// Hàm nhập thông tin truyện
Book* inputBook() {
	Book *b = (Book*)malloc(sizeof(Book));
	printf("Nhap ma truyen: ");
	scanf("%s", b->id);
	printf("Nhap ten truyen: ");
	getchar();
	fgets(b->name, 100, stdin); b->name[strcspn(b->name, "\n")] = 0;
	printf("Nhap tac gia: ");
	fgets(b->author, 50, stdin); b->author[strcspn(b->author, "\n")] = 0;
	printf("Nhap the loai: ");
	fgets(b->genre, 30, stdin); b->genre[strcspn(b->genre, "\n")] = 0;
	printf("Nhap tap so: ");
	scanf("%d", &b->volume);
	printf("Nhap don gia thue/ngay: ");
	scanf("%d", &b->price_per_day);
	b->next = NULL;
	return b;
}

// In thông tin truyện
void printBook(Book *b) {
	printf("| %10s | %15s | %15s | %15s | %15d | %15d |\n",
		b->id, b->name, b->author, b->genre, b->volume, b->price_per_day);
}

// In danh sách truyện trong Queue
void printBookQueue(Queue *q) {
	QueueNode *cur = q->front;
	int i = 1;
	while (cur) {
		i++;
		printBook((Book*)cur->data);
		cur = cur->next;
	}
	if (i == 1) printf("(Khong co truyen trong hang)\n");
}

// Hàm nhập khách hàng vào hàng chờ mượn truyện
Customer* inputCustomer() {
	Customer *c = (Customer*)malloc(sizeof(Customer));
	printf("Nhap ten khach hang: ");
	getchar();
	fgets(c->name, 50, stdin); c->name[strcspn(c->name, "\n")] = 0;
	printf("Nhap so dien thoai: ");
	fgets(c->phone, 20, stdin); c->phone[strcspn(c->phone, "\n")] = 0;
	c->next = NULL;
	return c;
}

void printCustomer(Customer *c) {
	printf("| %15s | %15s |\n", c->name, c->phone);
}

void printCustomerQueue(Queue *q) {
	QueueNode *cur = q->front;
	int i = 1;
	while (cur) {
		i++;
		printCustomer((Customer*)cur->data);
		cur = cur->next;
	}
	if (i == 1) printf("(Khong co khach trong hang cho)\n");
}

// Hàm tạo phiếu mượn
BorrowRecord* createBorrowRecord(const char *book_id, const char *customer_name, int price_per_day) {
	BorrowRecord *r = (BorrowRecord*)malloc(sizeof(BorrowRecord));
	strcpy(r->book_id, book_id);
	strcpy(r->customer_name, customer_name);
	printf("Nhap ngay muon (dd/mm/yyyy): ");
	scanf("%s", r->borrow_date);
	printf("Nhap ngay tra du kien (dd/mm/yyyy): ");
	scanf("%s", r->expected_return_date);
	strcpy(r->actual_return_date, "");
	r->total_fee = 0;
	r->next = NULL;
	return r;
}

void printBorrowRecord(BorrowRecord *r) {
	printf("| %10s | %15s | %15s | %15s | %15s | %15d |\n",
		r->book_id, r->customer_name, r->borrow_date, r->expected_return_date, r->actual_return_date, r->total_fee);
}

void printBorrowQueue(Queue *q) {
	QueueNode *cur = q->front;
	int i = 1;
	while (cur) {
		i++;
		printBorrowRecord((BorrowRecord*)cur->data);
		cur = cur->next;
	}
	if (i == 1) printf("(Khong co lich su muon-tra)\n");
}

// Hàm tính số ngày giữa 2 ngày (giản lược, chỉ dùng cho demo)
int calcDays(const char *from, const char *to) {
	int d1, m1, y1, d2, m2, y2;
	sscanf(from, "%d/%d/%d", &d1, &m1, &y1);
	sscanf(to, "%d/%d/%d", &d2, &m2, &y2);
	return (y2-y1)*365 + (m2-m1)*30 + (d2-d1);
}

int main() {
	Queue bookQueue; // Nhập kho truyện
	Queue customerQueue; // Hàng chờ mượn truyện
	Queue borrowHistory; // Lịch sử mượn-trả
	initQueue(&bookQueue);
	initQueue(&customerQueue);
	initQueue(&borrowHistory);
	int choice;
	while (1) {
		printf("\n===== MENU =====\n");
		printf("1. Nhap kho truyen (enqueue)\n");
		printf("2. In danh sach truyen trong kho\n");
		printf("3. Them khach vao hang cho muon truyen\n");
		printf("4. In hang cho muon truyen\n");
		printf("5. Xu ly muon truyen (khach dau hang)\n");
		printf("6. Tra truyen va ghi nhan lich su\n");
		printf("7. In lich su muon-tra\n");
		printf("0. Thoat\n");
		printf("Chon chuc nang: ");
		scanf("%d", &choice);
		if (choice == 0) break;
		if (choice == 1) {
			Book *b = inputBook();
			enqueue(&bookQueue, b);
			printf("Da nhap kho truyen thanh cong!\n");
		} else if (choice == 2) {
			printf("Danh sach truyen trong kho:\n");
            printf("| %10s | %15s | %15s | %15s | %15s | %15s |\n", "Ma", "Ten", "Tac gia", "The loai", "Tap", "Don gia");
			printBookQueue(&bookQueue);
		} else if (choice == 3) {
			Customer *c = inputCustomer();
			enqueue(&customerQueue, c);
			printf("Da them khach vao hang cho!\n");
		} else if (choice == 4) {
			printf("Hang cho muon truyen:\n");
            printf("| %15s | %15s |\n", "Ten", "SDT");
			printCustomerQueue(&customerQueue);
		} else if (choice == 5) {
			if (isEmpty(&bookQueue) || isEmpty(&customerQueue)) {
				printf("Khong co truyen hoac khach trong hang!\n");
			} else {
				Book *b = (Book*)dequeue(&bookQueue);
				Customer *c = (Customer*)dequeue(&customerQueue);
				printf("Khach '%s' duoc muon truyen '%s'!\n", c->name, b->name);
				BorrowRecord *r = createBorrowRecord(b->id, c->name, b->price_per_day);
				enqueue(&borrowHistory, r);
				free(b); free(c);
			}
		} else if (choice == 6) {
			printf("Nhap ma truyen tra: ");
			char bookid[20];
			scanf("%s", bookid);
			// Tìm trong lịch sử mượn để cập nhật ngày trả thực tế và tính phí
			QueueNode *cur = borrowHistory.front;
			int found = 0;
			while (cur) {
				BorrowRecord *r = (BorrowRecord*)cur->data;
				if (strcmp(r->book_id, bookid) == 0 && strlen(r->actual_return_date) == 0) {
					printf("Nhap ngay tra thuc te (dd/mm/yyyy): ");
					scanf("%s", r->actual_return_date);
					int days = calcDays(r->borrow_date, r->actual_return_date);
					// Tìm lại đơn giá truyện
					QueueNode *bcur = bookQueue.front;
					int price = 0;
					while (bcur) {
						Book *b = (Book*)bcur->data;
						if (strcmp(b->id, r->book_id) == 0) { price = b->price_per_day; break; }
						bcur = bcur->next;
					}
					r->total_fee = (days > 0 ? days : 0) * price;
					printf("Da cap nhat tra truyen va tinh phi!\n");
					found = 1;
					break;
				}
				cur = cur->next;
			}
			if (!found) printf("Khong tim thay phieu muon chua tra!\n");
		} else if (choice == 7) {
			printf("Lich su muon-tra:\n");
            printf("| %10s | %15s | %15s | %15s | %15s | %15s |\n", "Truyen", "Khach", "Ngay muon", "Ngay tra du kien", "Ngay tra thuc te", "Phi");
			printBorrowQueue(&borrowHistory);
		} else {
			printf("Chuc nang khong hop le!\n");
		}
	}
	// Giai phong bo nho
	freeQueue(&bookQueue);
	freeQueue(&customerQueue);
	freeQueue(&borrowHistory);
	return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <windows.h> // Để hỗ trợ tiếng Việt trong console

#define MAX_WORD_LEN 50
#define MAX_MEANING_LEN 500
#define MAX_EXAMPLE_LEN 500
#define MAX_TYPE_LEN 20

// Cấu trúc để lưu thông tin của một từ trong từ điển
typedef struct Word {
    char word[MAX_WORD_LEN];           // Từ tiếng Anh
    char type[MAX_TYPE_LEN];           // Loại từ (noun, verb, adj, ...)
    char meaning[MAX_MEANING_LEN];      // Nghĩa tiếng Việt
    char example[MAX_EXAMPLE_LEN];      // Ví dụ
    int is_favorite;                    // Đánh dấu từ yêu thích (0: không, 1: có)
    struct Word* left;                  // Con trỏ sang nút con bên trái
    struct Word* right;                 // Con trỏ sang nút con bên phải
} Word;

// Khai báo các hàm
Word* createNode(const char* word, const char* type, const char* meaning, const char* example);
Word* insert(Word* root, Word* newNode);
Word* search(Word* root, const char* word);
void inorder(Word* root);
Word* deleteNode(Word* root, const char* word);
Word* minValueNode(Word* node);
void searchPrefix(Word* root, const char* prefix);
void suggestSimilarWords(Word* root, const char* word);
int getHeight(Word* root);
void countWordTypes(Word* root, int* noun, int* verb, int* adj);
void findExtremeWords(Word* root, char* shortest, char* longest);
Word* getRandomWord(Word* root);
int countTotalNodes(Word* root);
void saveToFile(Word* root, FILE* fp);
void setupVietnameseConsole();

// Hàm chính để hiển thị menu và xử lý các lựa chọn
void displayMenu() {
    printf("\n=== TỪ ĐIỂN ANH - VIỆT ===\n");
    printf("1. Tra cứu từ\n");
    printf("2. Thêm từ mới\n");
    printf("3. Xóa từ\n");
    printf("4. Sửa nghĩa hoặc thông tin từ\n");
    printf("5. In toàn bộ từ điển\n");
    printf("6. Tìm kiếm từ bắt đầu bằng...\n");
    printf("7. Thống kê từ điển\n");
    printf("8. Đánh dấu từ yêu thích\n");
    printf("9. Hiển thị từ ngẫu nhiên\n");
    printf("0. Thoát\n");
    printf("Lựa chọn của bạn: ");
}

// Hàm tạo node mới
Word* createNode(const char* word, const char* type, const char* meaning, const char* example) {
    Word* newNode = (Word*)malloc(sizeof(Word));
    if (newNode) {
        strcpy(newNode->word, word);
        strcpy(newNode->type, type);
        strcpy(newNode->meaning, meaning);
        strcpy(newNode->example, example);
        newNode->is_favorite = 0;
        newNode->left = newNode->right = NULL;
    }
    return newNode;
}

// Hàm chèn node mới vào cây
Word* insert(Word* root, Word* newNode) {
    if (root == NULL)
        return newNode;

    int cmp = strcmp(newNode->word, root->word);
    if (cmp < 0)
        root->left = insert(root->left, newNode);
    else if (cmp > 0)
        root->right = insert(root->right, newNode);

    return root;
}

// Hàm tìm kiếm từ trong cây
Word* search(Word* root, const char* word) {
    if (root == NULL || strcmp(root->word, word) == 0)
        return root;

    if (strcmp(word, root->word) < 0)
        return search(root->left, word);
    return search(root->right, word);
}

// Hàm duyệt cây theo thứ tự in-order
void inorder(Word* root) {
    if (root != NULL) {
        inorder(root->left);
        printf("\nTừ: %s\n", root->word);
        printf("Loại từ: %s\n", root->type);
        printf("Nghĩa: %s\n", root->meaning);
        printf("Ví dụ: %s\n", root->example);
        if (root->is_favorite)
            printf("★ Từ yêu thích ★\n");
        printf("------------------------\n");
        inorder(root->right);
    }
}

// Hàm tìm node có giá trị nhỏ nhất trong cây con
Word* minValueNode(Word* node) {
    Word* current = node;
    while (current && current->left != NULL)
        current = current->left;
    return current;
}

// Hàm xóa một node khỏi cây
Word* deleteNode(Word* root, const char* word) {
    if (root == NULL)
        return root;

    int cmp = strcmp(word, root->word);
    if (cmp < 0)
        root->left = deleteNode(root->left, word);
    else if (cmp > 0)
        root->right = deleteNode(root->right, word);
    else {
        // Node với một hoặc không có con
        if (root->left == NULL) {
            Word* temp = root->right;
            free(root);
            return temp;
        }
        else if (root->right == NULL) {
            Word* temp = root->left;
            free(root);
            return temp;
        }

        // Node với hai con
        Word* temp = minValueNode(root->right);
        strcpy(root->word, temp->word);
        strcpy(root->type, temp->type);
        strcpy(root->meaning, temp->meaning);
        strcpy(root->example, temp->example);
        root->is_favorite = temp->is_favorite;
        root->right = deleteNode(root->right, temp->word);
    }
    return root;
}

// Hàm tìm kiếm các từ bắt đầu bằng prefix
void searchPrefix(Word* root, const char* prefix) {
    if (root == NULL)
        return;

    // Kiểm tra nếu từ hiện tại bắt đầu bằng prefix
    if (strncmp(root->word, prefix, strlen(prefix)) == 0) {
        printf("\nTừ: %s\n", root->word);
        printf("Loại từ: %s\n", root->type);
        printf("Nghĩa: %s\n", root->meaning);
        printf("Ví dụ: %s\n", root->example);
        printf("------------------------\n");
    }

    searchPrefix(root->left, prefix);
    searchPrefix(root->right, prefix);
}

// Hàm gợi ý các từ tương tự
void suggestSimilarWords(Word* root, const char* word) {
    if (root == NULL)
        return;

    // Đơn giản hóa: gợi ý các từ có 2 ký tự đầu giống nhau
    if (strncmp(root->word, word, 2) == 0) {
        printf("Có phải bạn muốn tìm: %s?\n", root->word);
    }

    suggestSimilarWords(root->left, word);
    suggestSimilarWords(root->right, word);
}

// Hàm tính chiều cao của cây
int getHeight(Word* root) {
    if (root == NULL)
        return 0;
    int leftHeight = getHeight(root->left);
    int rightHeight = getHeight(root->right);
    return (leftHeight > rightHeight ? leftHeight : rightHeight) + 1;
}

// Hàm đếm số từ theo loại
void countWordTypes(Word* root, int* noun, int* verb, int* adj) {
    if (root == NULL)
        return;

    if (strcmp(root->type, "noun") == 0) (*noun)++;
    else if (strcmp(root->type, "verb") == 0) (*verb)++;
    else if (strcmp(root->type, "adj") == 0) (*adj)++;

    countWordTypes(root->left, noun, verb, adj);
    countWordTypes(root->right, noun, verb, adj);
}

// Hàm tìm từ dài nhất và ngắn nhất
void findExtremeWords(Word* root, char* shortest, char* longest) {
    if (root == NULL)
        return;

    if (strlen(root->word) < strlen(shortest))
        strcpy(shortest, root->word);
    if (strlen(root->word) > strlen(longest))
        strcpy(longest, root->word);

    findExtremeWords(root->left, shortest, longest);
    findExtremeWords(root->right, shortest, longest);
}

// Hàm đếm tổng số node trong cây
int countTotalNodes(Word* root) {
    if (root == NULL)
        return 0;
    return 1 + countTotalNodes(root->left) + countTotalNodes(root->right);
}

// Hàm lấy một từ ngẫu nhiên từ từ điển
Word* getRandomWord(Word* root) {
    int total = countTotalNodes(root);
    if (total == 0)
        return NULL;

    int target = rand() % total;
    int current = 0;
    Word* result = NULL;

    // Helper function để duyệt cây và lấy node thứ target
    void helper(Word* node) {
        if (node == NULL || result != NULL)
            return;

        helper(node->left);
        if (current == target) {
            result = node;
            return;
        }
        current++;
        helper(node->right);
    }

    helper(root);
    return result;
}

// Hàm lưu từ điển vào file
void saveToFile(Word* root, FILE* fp) {
    if (root != NULL) {
        saveToFile(root->left, fp);
        fprintf(fp, "%s|%s|%s|%s|%d\n", 
                root->word, root->type, root->meaning, 
                root->example, root->is_favorite);
        saveToFile(root->right, fp);
    }
}

// Hàm thiết lập console để hiển thị tiếng Việt
void setupVietnameseConsole() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
}

// Hàm main
int main() {
    setupVietnameseConsole();
    srand(time(NULL));
    
    Word* root = NULL;
    char word[MAX_WORD_LEN];
    char type[MAX_TYPE_LEN];
    char meaning[MAX_MEANING_LEN];
    char example[MAX_EXAMPLE_LEN];
    int choice;
    FILE* fp;

    // Đọc từ điển từ file khi khởi động
    fp = fopen("dictionary.txt", "r");
    if (fp != NULL) {
        while (fscanf(fp, "%[^|]|%[^|]|%[^|]|%[^|]|%d\n", 
                     word, type, meaning, example, &choice) == 5) {
            Word* newNode = createNode(word, type, meaning, example);
            newNode->is_favorite = choice;
            root = insert(root, newNode);
        }
        fclose(fp);
    }

    // Hiển thị từ ngẫu nhiên khi khởi động
    Word* randomWord = getRandomWord(root);
    if (randomWord != NULL) {
        printf("\n=== TỪ CỦA NGÀY HÔM NAY ===\n");
        printf("Từ: %s\n", randomWord->word);
        printf("Loại từ: %s\n", randomWord->type);
        printf("Nghĩa: %s\n", randomWord->meaning);
        printf("Ví dụ: %s\n", randomWord->example);
        printf("========================\n");
    }

    while (1) {
        displayMenu();
        scanf("%d", &choice);
        getchar(); // Đọc ký tự newline

        switch (choice) {
            case 1: { // Tra cứu từ
                printf("Nhập từ cần tra: ");
                fgets(word, MAX_WORD_LEN, stdin);
                word[strcspn(word, "\n")] = 0;
                
                Word* result = search(root, word);
                if (result != NULL) {
                    printf("\nKết quả:\n");
                    printf("Từ: %s\n", result->word);
                    printf("Loại từ: %s\n", result->type);
                    printf("Nghĩa: %s\n", result->meaning);
                    printf("Ví dụ: %s\n", result->example);
                    if (result->is_favorite)
                        printf("★ Từ yêu thích ★\n");
                } else {
                    printf("Không tìm thấy từ này.\n");
                    printf("Các từ tương tự:\n");
                    suggestSimilarWords(root, word);
                }
                break;
            }
            case 2: { // Thêm từ mới
                printf("Nhập từ mới: ");
                fgets(word, MAX_WORD_LEN, stdin);
                word[strcspn(word, "\n")] = 0;

                printf("Nhập loại từ (noun/verb/adj/...): ");
                fgets(type, MAX_TYPE_LEN, stdin);
                type[strcspn(type, "\n")] = 0;

                printf("Nhập nghĩa: ");
                fgets(meaning, MAX_MEANING_LEN, stdin);
                meaning[strcspn(meaning, "\n")] = 0;

                printf("Nhập ví dụ: ");
                fgets(example, MAX_EXAMPLE_LEN, stdin);
                example[strcspn(example, "\n")] = 0;

                Word* newNode = createNode(word, type, meaning, example);
                root = insert(root, newNode);
                printf("Đã thêm từ mới thành công!\n");
                break;
            }
            case 3: { // Xóa từ
                printf("Nhập từ cần xóa: ");
                fgets(word, MAX_WORD_LEN, stdin);
                word[strcspn(word, "\n")] = 0;

                if (search(root, word) != NULL) {
                    root = deleteNode(root, word);
                    printf("Đã xóa từ thành công!\n");
                } else {
                    printf("Không tìm thấy từ này.\n");
                }
                break;
            }
            case 4: { // Sửa thông tin từ
                printf("Nhập từ cần sửa: ");
                fgets(word, MAX_WORD_LEN, stdin);
                word[strcspn(word, "\n")] = 0;

                Word* wordToEdit = search(root, word);
                if (wordToEdit != NULL) {
                    printf("Nhập loại từ mới (noun/verb/adj/...): ");
                    fgets(type, MAX_TYPE_LEN, stdin);
                    type[strcspn(type, "\n")] = 0;

                    printf("Nhập nghĩa mới: ");
                    fgets(meaning, MAX_MEANING_LEN, stdin);
                    meaning[strcspn(meaning, "\n")] = 0;

                    printf("Nhập ví dụ mới: ");
                    fgets(example, MAX_EXAMPLE_LEN, stdin);
                    example[strcspn(example, "\n")] = 0;

                    strcpy(wordToEdit->type, type);
                    strcpy(wordToEdit->meaning, meaning);
                    strcpy(wordToEdit->example, example);
                    printf("Đã cập nhật thông tin từ thành công!\n");
                } else {
                    printf("Không tìm thấy từ này.\n");
                }
                break;
            }
            case 5: // In toàn bộ từ điển
                printf("\n=== TOÀN BỘ TỪ ĐIỂN ===\n");
                inorder(root);
                break;
            case 6: { // Tìm kiếm từ bắt đầu bằng...
                printf("Nhập tiền tố cần tìm: ");
                fgets(word, MAX_WORD_LEN, stdin);
                word[strcspn(word, "\n")] = 0;
                
                printf("\nCác từ bắt đầu bằng '%s':\n", word);
                searchPrefix(root, word);
                break;
            }
            case 7: { // Thống kê từ điển
                int noun = 0, verb = 0, adj = 0;
                char shortest[MAX_WORD_LEN] = "zzzzzzzzzzzzzzzz";
                char longest[MAX_WORD_LEN] = "";
                
                countWordTypes(root, &noun, &verb, &adj);
                findExtremeWords(root, shortest, longest);
                
                printf("\n=== THỐNG KÊ TỪ ĐIỂN ===\n");
                printf("Tổng số từ: %d\n", countTotalNodes(root));
                printf("Số danh từ: %d\n", noun);
                printf("Số động từ: %d\n", verb);
                printf("Số tính từ: %d\n", adj);
                printf("Từ ngắn nhất: %s\n", shortest);
                printf("Từ dài nhất: %s\n", longest);
                printf("Chiều cao cây BST: %d\n", getHeight(root));
                break;
            }
            case 8: { // Đánh dấu từ yêu thích
                printf("Nhập từ cần đánh dấu/bỏ đánh dấu yêu thích: ");
                fgets(word, MAX_WORD_LEN, stdin);
                word[strcspn(word, "\n")] = 0;

                Word* result = search(root, word);
                if (result != NULL) {
                    result->is_favorite = !result->is_favorite;
                    printf("Đã %s đánh dấu yêu thích!\n", 
                           result->is_favorite ? "thêm" : "bỏ");
                } else {
                    printf("Không tìm thấy từ này.\n");
                }
                break;
            }
            case 9: { // Hiển thị từ ngẫu nhiên
                Word* randomWord = getRandomWord(root);
                if (randomWord != NULL) {
                    printf("\n=== TỪ NGẪU NHIÊN ===\n");
                    printf("Từ: %s\n", randomWord->word);
                    printf("Loại từ: %s\n", randomWord->type);
                    printf("Nghĩa: %s\n", randomWord->meaning);
                    printf("Ví dụ: %s\n", randomWord->example);
                }
                break;
            }
            case 0: { // Thoát và lưu từ điển
                fp = fopen("dictionary.txt", "w");
                if (fp != NULL) {
                    saveToFile(root, fp);
                    fclose(fp);
                }
                printf("Đã lưu từ điển và thoát chương trình.\n");
                return 0;
            }
            default:
                printf("Lựa chọn không hợp lệ!\n");
        }
    }

    return 0;
}

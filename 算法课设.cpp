#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 定义链表节点结构
typedef struct LineNode {
    char line[256]; // 一行文本，长度可自定义
    struct LineNode* next;
} LineNode;

// 清屏宏定义，根据操作系统选择不同的清屏命令
#ifdef _WIN32
#define CLEAR "cls"
#else
#define CLEAR "clear"
#endif

// 插入行
void insertLine(LineNode** head, int position, char* text) {
    LineNode* newNode = (LineNode*)malloc(sizeof(LineNode)); // 分配新节点的内存
    if (newNode == NULL) {
        printf("内存分配失败\n");
        return;
    }
    strcpy(newNode->line, text); // 将文本复制到新节点
    newNode->next = NULL; // 新节点的下一个指针初始化为NULL

    if (position == 1) { // 插入到链表头部
        newNode->next = *head;
        *head = newNode;
        printf("行已成功插入到第1行\n");
        return;
    }

    LineNode* temp = *head;
    int i;
    for (i = 1; i < position - 1 && temp != NULL; i++) { // 遍历链表到指定位置前一个节点
        temp = temp->next;
    }

    if (temp == NULL) { // 位置超出链表长度
        printf("位置超出范围\n");
        free(newNode); // 释放新节点的内存
        return;
    }

    newNode->next = temp->next; // 插入新节点
    temp->next = newNode;
    printf("行已成功插入到第%d行\n", position);
}

// 删除行
void deleteLine(LineNode** head, int position) {
    if (*head == NULL) { // 链表为空
        printf("链表为空\n");
        return;
    }

    LineNode* temp = *head;
    if (position == 1) { // 删除链表头部
        *head = temp->next;
        free(temp);
        printf("第1行已成功删除\n");
        return;
    }

    for (int i = 1; i < position - 1 && temp != NULL; i++) { // 遍历链表到指定位置前一个节点
        temp = temp->next;
    }

    if (temp == NULL || temp->next == NULL) { // 位置超出链表长度
        printf("位置超出范围\n");
        return;
    }

    LineNode* nextNode = temp->next->next; // 保存下一个节点
    free(temp->next); // 释放要删除的节点
    temp->next = nextNode; // 更新前一个节点的下一个指针
    printf("第%d行已成功删除\n", position);
}

// 修改行
void modifyLine(LineNode* head, int position, char* text) {
    LineNode* temp = head;
    int lineNumber = 1;
    while (temp != NULL && lineNumber < position) { // 遍历链表到指定位置
        temp = temp->next;
        lineNumber++;
    }

    if (temp == NULL) { // 位置超出链表长度
        printf("位置超出范围\n");
        return;
    }

    strcpy(temp->line, text); // 修改指定行的文本
    printf("第%d行已成功修改\n", position);
}

// 查找行
void searchLine(LineNode* head, char* keyword) {
    LineNode* temp = head;
    int lineNumber = 1;
    int found = 0;
    while (temp != NULL) { // 遍历链表查找包含关键字的行
        if (strstr(temp->line, keyword) != NULL) {
            printf("第%d行: %s\n", lineNumber, temp->line);
            found = 1;
        }
        temp = temp->next;
        lineNumber++;
    }
    if (!found) { // 没有找到包含关键字的行
        printf("未找到包含关键字的行\n");
    }
}

// 保存到文件
void saveToFile(LineNode* head, const char* filename) {
    FILE* file = fopen(filename, "w"); // 打开文件以写模式
    if (file == NULL) {
        printf("无法打开文件\n");
        return;
    }

    LineNode* temp = head;
    while (temp != NULL) { // 将链表中的每一行写入文件
        fprintf(file, "%s\n", temp->line);
        temp = temp->next;
    }

    fclose(file); // 关闭文件
    printf("文件保存成功\n");
}

// 从文件加载
void loadFromFile(LineNode** head, const char* filename) {
    FILE* file = fopen(filename, "a+"); // 打开文件以读模式
    if (file == NULL) {
        printf("无法打开文件\n");
        return;
    }

    char buffer[256];
    *head = NULL; // 清空链表

    while (fgets(buffer, sizeof(buffer), file)) { // 读取文件中的每一行
        buffer[strcspn(buffer, "\n")] = 0; // 去掉换行符
        insertLine(head, 1, buffer); // 插入到链表头部
    }

    fclose(file); // 关闭文件
    printf("文件加载成功\n");
}

// 打印所有行
void printLines(LineNode* head) {
    LineNode* temp = head;
    int lineNumber = 1;
    if (head == NULL) { // 链表为空
        printf("当前没有文本行\n");
        return;
    }
    while (temp != NULL) { // 遍历链表并打印每一行
        printf("第%d行: %s\n", lineNumber, temp->line);
        temp = temp->next;
        lineNumber++;
    }
}

// 释放链表内存
void freeList(LineNode* head) {
    LineNode* temp;
    while (head != NULL) { // 遍历链表并释放每个节点的内存
        temp = head;
        head = head->next;
        free(temp);
    }
}

int main() {
    LineNode* head = NULL; // 初始化链表头指针
    int choice, position;
    char text[256];
    char filename[256];

    while (1) {
        system(CLEAR); // 清空屏幕
        printf("\n\t\t\t------------------ 行编辑程序 ---------------------\n");
        printf("\t\t\t|     *1. 插入行                                  |\n");
        printf("\t\t\t|     *2. 删除行                                  |\n");
        printf("\t\t\t|     *3. 修改行                                  |\n");
        printf("\t\t\t|     *4. 查找行                                  |\n");
        printf("\t\t\t|     *5. 保存到文件                              |\n");
        printf("\t\t\t|     *6. 从文件加载                              |\n");
        printf("\t\t\t|     *7. 打印所有行                              |\n");
        printf("\t\t\t|     *8. 退出                                    |");
        printf("\n\t\t\t---------------------------------------------------\n");
        printf("\t\t\t*请输入你的选择: ");
        scanf("%d", &choice);
        getchar(); // 清除缓冲区

        switch (choice) {
        case 1: // 插入行
            printf("请输入位置: ");
            scanf("%d", &position);
            getchar(); // 清除缓冲区
            printf("请输入文本: ");
            fgets(text, sizeof(text), stdin);
            text[strcspn(text, "\n")] = 0; // 去掉换行符
            insertLine(&head, position, text);
            break;
        case 2: // 删除行
            printf("请输入位置: ");
            scanf("%d", &position);
            getchar(); // 清除缓冲区
            deleteLine(&head, position);
            break;
        case 3: // 修改行
            printf("请输入位置: ");
            scanf("%d", &position);
            getchar(); // 清除缓冲区
            printf("请输入新的文本: ");
            fgets(text, sizeof(text), stdin);
            text[strcspn(text, "\n")] = 0; // 去掉换行符
            modifyLine(head, position, text);
            break;
        case 4: // 查找行
            printf("请输入关键字: ");
            fgets(text, sizeof(text), stdin);
            text[strcspn(text, "\n")] = 0; // 去掉换行符
            searchLine(head, text);
            break;
        case 5: // 保存到文件
            printf("请输入文件名: ");
            fgets(filename, sizeof(filename), stdin);
            filename[strcspn(filename, "\n")] = 0; // 去掉换行符
            saveToFile(head, filename);
            break;
        case 6: // 从文件加载
            printf("请输入文件名: ");
            fgets(filename, sizeof(filename), stdin);
            filename[strcspn(filename, "\n")] = 0; // 去掉换行符
            loadFromFile(&head, filename);
            break;
        case 7: // 打印所有行
            printLines(head);
            break;
        case 8: // 退出程序
            printf("退出程序\n");
            freeList(head); // 释放链表内存
            exit(0);
        default: // 无效选择
            printf("无效的选择，请重新输入\n");
        }

        // 暂停以便用户查看操作结果
        printf("\n按任意键继续...");
        getchar();
    }

    return 0;
}
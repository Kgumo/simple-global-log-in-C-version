#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ��������ڵ�ṹ
typedef struct LineNode {
    char line[256]; // һ���ı������ȿ��Զ���
    struct LineNode* next;
} LineNode;

// �����궨�壬���ݲ���ϵͳѡ��ͬ����������
#ifdef _WIN32
#define CLEAR "cls"
#else
#define CLEAR "clear"
#endif

// ������
void insertLine(LineNode** head, int position, char* text) {
    LineNode* newNode = (LineNode*)malloc(sizeof(LineNode)); // �����½ڵ���ڴ�
    if (newNode == NULL) {
        printf("�ڴ����ʧ��\n");
        return;
    }
    strcpy(newNode->line, text); // ���ı����Ƶ��½ڵ�
    newNode->next = NULL; // �½ڵ����һ��ָ���ʼ��ΪNULL

    if (position == 1) { // ���뵽����ͷ��
        newNode->next = *head;
        *head = newNode;
        printf("���ѳɹ����뵽��1��\n");
        return;
    }

    LineNode* temp = *head;
    int i;
    for (i = 1; i < position - 1 && temp != NULL; i++) { // ��������ָ��λ��ǰһ���ڵ�
        temp = temp->next;
    }

    if (temp == NULL) { // λ�ó���������
        printf("λ�ó�����Χ\n");
        free(newNode); // �ͷ��½ڵ���ڴ�
        return;
    }

    newNode->next = temp->next; // �����½ڵ�
    temp->next = newNode;
    printf("���ѳɹ����뵽��%d��\n", position);
}

// ɾ����
void deleteLine(LineNode** head, int position) {
    if (*head == NULL) { // ����Ϊ��
        printf("����Ϊ��\n");
        return;
    }

    LineNode* temp = *head;
    if (position == 1) { // ɾ������ͷ��
        *head = temp->next;
        free(temp);
        printf("��1���ѳɹ�ɾ��\n");
        return;
    }

    for (int i = 1; i < position - 1 && temp != NULL; i++) { // ��������ָ��λ��ǰһ���ڵ�
        temp = temp->next;
    }

    if (temp == NULL || temp->next == NULL) { // λ�ó���������
        printf("λ�ó�����Χ\n");
        return;
    }

    LineNode* nextNode = temp->next->next; // ������һ���ڵ�
    free(temp->next); // �ͷ�Ҫɾ���Ľڵ�
    temp->next = nextNode; // ����ǰһ���ڵ����һ��ָ��
    printf("��%d���ѳɹ�ɾ��\n", position);
}

// �޸���
void modifyLine(LineNode* head, int position, char* text) {
    LineNode* temp = head;
    int lineNumber = 1;
    while (temp != NULL && lineNumber < position) { // ��������ָ��λ��
        temp = temp->next;
        lineNumber++;
    }

    if (temp == NULL) { // λ�ó���������
        printf("λ�ó�����Χ\n");
        return;
    }

    strcpy(temp->line, text); // �޸�ָ���е��ı�
    printf("��%d���ѳɹ��޸�\n", position);
}

// ������
void searchLine(LineNode* head, char* keyword) {
    LineNode* temp = head;
    int lineNumber = 1;
    int found = 0;
    while (temp != NULL) { // ����������Ұ����ؼ��ֵ���
        if (strstr(temp->line, keyword) != NULL) {
            printf("��%d��: %s\n", lineNumber, temp->line);
            found = 1;
        }
        temp = temp->next;
        lineNumber++;
    }
    if (!found) { // û���ҵ������ؼ��ֵ���
        printf("δ�ҵ������ؼ��ֵ���\n");
    }
}

// ���浽�ļ�
void saveToFile(LineNode* head, const char* filename) {
    FILE* file = fopen(filename, "w"); // ���ļ���дģʽ
    if (file == NULL) {
        printf("�޷����ļ�\n");
        return;
    }

    LineNode* temp = head;
    while (temp != NULL) { // �������е�ÿһ��д���ļ�
        fprintf(file, "%s\n", temp->line);
        temp = temp->next;
    }

    fclose(file); // �ر��ļ�
    printf("�ļ�����ɹ�\n");
}

// ���ļ�����
void loadFromFile(LineNode** head, const char* filename) {
    FILE* file = fopen(filename, "a+"); // ���ļ��Զ�ģʽ
    if (file == NULL) {
        printf("�޷����ļ�\n");
        return;
    }

    char buffer[256];
    *head = NULL; // �������

    while (fgets(buffer, sizeof(buffer), file)) { // ��ȡ�ļ��е�ÿһ��
        buffer[strcspn(buffer, "\n")] = 0; // ȥ�����з�
        insertLine(head, 1, buffer); // ���뵽����ͷ��
    }

    fclose(file); // �ر��ļ�
    printf("�ļ����سɹ�\n");
}

// ��ӡ������
void printLines(LineNode* head) {
    LineNode* temp = head;
    int lineNumber = 1;
    if (head == NULL) { // ����Ϊ��
        printf("��ǰû���ı���\n");
        return;
    }
    while (temp != NULL) { // ����������ӡÿһ��
        printf("��%d��: %s\n", lineNumber, temp->line);
        temp = temp->next;
        lineNumber++;
    }
}

// �ͷ������ڴ�
void freeList(LineNode* head) {
    LineNode* temp;
    while (head != NULL) { // ���������ͷ�ÿ���ڵ���ڴ�
        temp = head;
        head = head->next;
        free(temp);
    }
}

int main() {
    LineNode* head = NULL; // ��ʼ������ͷָ��
    int choice, position;
    char text[256];
    char filename[256];

    while (1) {
        system(CLEAR); // �����Ļ
        printf("\n\t\t\t------------------ �б༭���� ---------------------\n");
        printf("\t\t\t|     *1. ������                                  |\n");
        printf("\t\t\t|     *2. ɾ����                                  |\n");
        printf("\t\t\t|     *3. �޸���                                  |\n");
        printf("\t\t\t|     *4. ������                                  |\n");
        printf("\t\t\t|     *5. ���浽�ļ�                              |\n");
        printf("\t\t\t|     *6. ���ļ�����                              |\n");
        printf("\t\t\t|     *7. ��ӡ������                              |\n");
        printf("\t\t\t|     *8. �˳�                                    |");
        printf("\n\t\t\t---------------------------------------------------\n");
        printf("\t\t\t*���������ѡ��: ");
        scanf("%d", &choice);
        getchar(); // ���������

        switch (choice) {
        case 1: // ������
            printf("������λ��: ");
            scanf("%d", &position);
            getchar(); // ���������
            printf("�������ı�: ");
            fgets(text, sizeof(text), stdin);
            text[strcspn(text, "\n")] = 0; // ȥ�����з�
            insertLine(&head, position, text);
            break;
        case 2: // ɾ����
            printf("������λ��: ");
            scanf("%d", &position);
            getchar(); // ���������
            deleteLine(&head, position);
            break;
        case 3: // �޸���
            printf("������λ��: ");
            scanf("%d", &position);
            getchar(); // ���������
            printf("�������µ��ı�: ");
            fgets(text, sizeof(text), stdin);
            text[strcspn(text, "\n")] = 0; // ȥ�����з�
            modifyLine(head, position, text);
            break;
        case 4: // ������
            printf("������ؼ���: ");
            fgets(text, sizeof(text), stdin);
            text[strcspn(text, "\n")] = 0; // ȥ�����з�
            searchLine(head, text);
            break;
        case 5: // ���浽�ļ�
            printf("�������ļ���: ");
            fgets(filename, sizeof(filename), stdin);
            filename[strcspn(filename, "\n")] = 0; // ȥ�����з�
            saveToFile(head, filename);
            break;
        case 6: // ���ļ�����
            printf("�������ļ���: ");
            fgets(filename, sizeof(filename), stdin);
            filename[strcspn(filename, "\n")] = 0; // ȥ�����з�
            loadFromFile(&head, filename);
            break;
        case 7: // ��ӡ������
            printLines(head);
            break;
        case 8: // �˳�����
            printf("�˳�����\n");
            freeList(head); // �ͷ������ڴ�
            exit(0);
        default: // ��Чѡ��
            printf("��Ч��ѡ������������\n");
        }

        // ��ͣ�Ա��û��鿴�������
        printf("\n�����������...");
        getchar();
    }

    return 0;
}
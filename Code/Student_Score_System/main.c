#include "stdio.h"
#include "stdlib.h"
#include "conio.h"
#include <windows.h>
typedef float datatype;
/* ѧ������ */
typedef struct Student
{
    char Stu_Number[20];
    char Stu_Name[20];
    datatype Score;
    struct Student *next;
} Link_List;

int Student_Total = 0;
/* ѧ���ɼ����� */
struct Student_Sort
{
    int pass;
    int middle;
    int good;
    int execlent;
    int unpass;
} Student_sort;

/**
 * @brief  ������ͷ�ڵ�ĵ�����
 * @param  ��
 * @retval ���ؽڵ����͵ı���ָ��
 */
Link_List *Init_List()
{
    Link_List *Head;
    Head = (Link_List *)malloc(sizeof(Link_List));
    Head->next = NULL;
    return Head;
}
/**
 * @brief  ���������ѧ����Ϣ
 * @param  ͷ�ڵ�
 * @retval ��
 */
void Output(Link_List *Head)
{
    Link_List *p;
    p = Head->next;
    if (!p)
    {
        printf("\n��δ¼����ѧ����\n");
    }
    else
    {
        printf("\tѧ��\t|\tѧ��\t|\t�ɼ�\t\t\n");
        while (p)
        {
            printf("\t%4s\t|\t%4s\t|\t%4f\t\t\n", p->Stu_Number, p->Stu_Name, p->Score);
            p = p->next;
        }
    }
}

/**
 * @brief  ¼��ѧ����Ϣ
 * @param  ͷ�ڵ㣬ѧ������
 * @retval ͷ�ڵ�
 */
Link_List *Create_Student(Link_List *Head)
{
    int i;
    Link_List *q, *p, *pre;
    q = Head;
    while (q->next != NULL)
    {
        q = q->next;
    }
    p = (Link_List *)malloc(sizeof(Link_List));
    printf("\t������ѧ������:");
    gets(p->Stu_Name);
    printf("\t������ѧ��ѧ��:");
    gets(p->Stu_Number);
    printf("\t������ѧ������:");
    scanf("%f", &p->Score);
    getchar();
    Student_Total++;
    printf("\t¼��ɹ���\n");
    p->next = NULL; /* ֱ�Ӳ����������յĵط� */
    q->next = p;
    return Head;
}
/**
 * @brief  ��ѧ�������ֵ�����
 * @param  ͷ�ڵ㣬ѧ������
 * @retval ͷ�ڵ�
 */
Link_List *SelectSort_Dirs(Link_List *Head)
{
    Link_List *pfirst;
    Link_List *ptail;
    Link_List *pminBefore;
    Link_List *pmin;
    Link_List *p;
    Link_List *head; //�����ͷ�ڵ��ĵ�һ���ڵ�
    pfirst = NULL;
    head = Head->next;
    while (head != NULL)
    {
        for (p = head, pmin = head; p->next != NULL; p = p->next) /* ѭ������ȫ���ڵ�����pfirst-ptail���������У�ͷ�ڵ�Ϊpfirst β�ڵ�Ϊptail������ */
        {
            if (strcmp(p->next->Stu_Name, pmin->Stu_Name) < 0) /* �ҵ���С�Ľڵ㣬�����С�Ľڵ㣬����С�ڵ��ǰһ���ڵ� */
            {
                pminBefore = p;
                pmin = p->next;
            }
        }
        if (pfirst == NULL) /* ����С�Ľڵ����������ͷ�ڵ�Ϊpfirst β�ڵ�Ϊptail������ */
        {
            pfirst = pmin;
            ptail = pmin;
        }
        else /* ������ѭ���е���С�ڵ����η��������� */
        {
            ptail->next = pmin;
            ptail = pmin;
        }
        if (pmin == head) /* �����СֵΪͷ�ڵ�ʱ�������ͷ�ڵ��Ƶ���һλ */
        {
            head = head->next;
        }
        else /* �������ͷ�ڵ㣬ֱ�ӽ�ԭ�������С�ڵ��������γ�ȥ����Сֵ��ԭ���� */
        {
            pminBefore->next = pmin->next;
        }
    }
    if (pfirst != NULL) /* �ж���û�������� */
    {
        ptail->next = NULL; /* �������β�ڵ���һλ�ÿ� */
    }
    Head->next = pfirst; /* ��ͷ�ڵ�ָ�������� */
    return Head;
}

/**
 * @brief  ��ѧ�����ɼ��ߵ�����
 * @param  ͷ�ڵ�
 * @retval ͷ�ڵ�
 */
Link_List *SelectSort_Score(Link_List *Head)
{
    Link_List *pfirst;
    Link_List *ptail;
    Link_List *pmaxBefore;
    Link_List *pmax;
    Link_List *p;
    Link_List *head; //�����ͷ�ڵ��ĵ�һ���ڵ�
    pfirst = NULL;
    head = Head->next;
    while (head != NULL) /* ѭ������ȫ���ڵ�����pfirst-ptail���������У�ͷ�ڵ�Ϊpfirst β�ڵ�Ϊptail������ */
    {
        for (p = head, pmax = head; p->next != NULL; p = p->next) /* �ҵ����Ľڵ㣬������Ľڵ�����ڵ��ǰһ���ڵ� */
        {
            if (p->next->Score > pmax->Score)
            {
                pmaxBefore = p;
                pmax = p->next;
            }
        }
        if (pfirst == NULL) /* �����Ľڵ����������ͷ�ڵ�Ϊpfirst β�ڵ�Ϊptail������ */
        {
            pfirst = pmax;
            ptail = pmax;
        }
        else /* ������ѭ���е����ڵ����η��������� */
        {
            ptail->next = pmax;
            ptail = pmax;
        }
        if (pmax == head) /* ������ֵΪͷ�ڵ�ʱ�������ͷ�ڵ��Ƶ���һλ */
        {
            head = head->next;
        }
        else /* �������ͷ�ڵ㣬ֱ�ӽ�ԭ��������ڵ��������γ�ȥ�����ֵ��ԭ���� */
        {
            pmaxBefore->next = pmax->next;
        }
    }
    if (pfirst != NULL) /* �ж���û�������� */
    {
        ptail->next = NULL; /* �������β�ڵ���һλ�ÿ� */
    }
    Head->next = pfirst; /* ��ͷ�ڵ�ָ�������� */
    return Head;
}
/**
 * @brief  ������������ѧ��
 * @param  ͷ�ڵ㣬����
 * @retval ��
 */
void Find_Student_Name(Link_List *head, char *stuname)
{
    Link_List *p = head;
    char flag = 0;
    while (p != NULL)
    {
        if (strcmp(p->Stu_Name, stuname) == 0)
        {
            printf("\t�ҵ�������Ϣ��\n");
            printf("\tѧ��\t|\tѧ��\t|\t�ɼ�\t\t\n");
            printf("\t%4s\t|\t%4s\t|\t%4f\t\t\n", p->Stu_Number, p->Stu_Name, p->Score);
            flag = 1;
        }
        p = p->next;
    }
    if (flag == 0)
    {
        printf("\tδ�ҵ���ѧ��\n");
    }
}
/**
 * @brief  ����ѧ�Ų���ѧ��
 * @param  ͷ�ڵ㣬ѧ��
 * @retval ��
 */
void Find_Student_Number(Link_List *head, char *num)
{
    Link_List *p = head;
    char flag = 0;
    while (p != NULL)
    {
        if (strcmp(p->Stu_Number, num) == 0)
        {
            printf("\t�ҵ�������Ϣ��\n");
            printf("\tѧ��\t|\tѧ��\t|\t�ɼ�\t\t\n");
            printf("\t%4s\t|\t%4s\t|\t%4f\t\t\n", p->Stu_Number, p->Stu_Name, p->Score);
            flag = 1;
        }
        p = p->next;
    }
    if (flag == 0)
    {
        printf("\tδ�ҵ���ѧ��\n");
    }
}
/**
 * @brief  ��ѧ�����з���
 * @param  ͷ�ڵ�
 * @retval ��
 */
void Sort_stu(Link_List *head)
{
    Link_List *p = head;
    if (Student_Total == 0)
    {
        printf("\tѧ������Ϊ0����\n");
        return;
    }

    while (p->next != NULL)
    {
        if (p->Score < 60)
        {
            Student_sort.unpass++;
        }
        else if (p->Score < 70)
        {
            Student_sort.pass++;
        }
        else if (p->Score < 80)
        {
            Student_sort.middle++;
        }
        else if (p->Score < 90)
        {
            Student_sort.good++;
        }
        else
        {
            Student_sort.execlent++;
        }
        p = p->next;
    }
    printf("\t����������%d ռ�ȣ�%f\n", Student_sort.execlent, (float)Student_sort.execlent / Student_Total);
    printf("\t��������%d ռ�ȣ�%f\n", Student_sort.good, (float)Student_sort.good / Student_Total);
    printf("\t��������%d ռ�ȣ�%f\n", Student_sort.middle, (float)Student_sort.middle / Student_Total);
    printf("\t����������%d ռ�ȣ�%f\n", Student_sort.pass, (float)Student_sort.pass / Student_Total);
    printf("\t������������%d ռ�ȣ�%f\n", Student_sort.unpass, (float)Student_sort.unpass / Student_Total);
}
/**
 * @brief  �˵���ʾ����
 * @param  ��
 * @retval ��
 */
void Menu_Display()
{
    printf("\t\tѧ���ɼ�����ϵͳ\t\t\n");
    printf("\t1.¼��ѧ��\n");
    printf("\t2.����ѧ�����������ֵ�����\n");
    printf("\t3.���ݳɼ������ɸߵ�������\n");
    printf("\t4.������������ѧ���ɼ�\n");
    printf("\t5.����ѧ�Ų���ѧ���ɼ�\n");
    printf("\t6.ͳ�Ƽ����С���������Ͳ�����ѧ������������ռ����\n");
    printf("\t0.�˳�\n");
    printf("\t��������0-6��\n");
}
/**
 * @brief  ���ع��
 * @param  ��
 * @retval ��
 */
void HideCursor()
{
    CONSOLE_CURSOR_INFO curInfo;                     //��������Ϣ�Ľṹ�����
    curInfo.dwSize = 1;                              //���û��ֵ�Ļ������������Ч
    curInfo.bVisible = FALSE;                        //���������Ϊ���ɼ�
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE); //��ȡ����̨���
    SetConsoleCursorInfo(handle, &curInfo);          //���ù����Ϣ
}
/**
 * @brief  ��λ���
 * @param  ���λ��
 * @retval ��
 */
void CursorJump(int x, int y)
{
    COORD pos;                                       //������λ�õĽṹ�����
    pos.X = x;                                       //������
    pos.Y = y;                                       //������
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE); //��ȡ����̨���
    SetConsoleCursorPosition(handle, pos);           //���ù��λ��
}
 int main()
{
    int Student_Total = 0;
    char Key;
    char Name;
    char Num;
    Link_List *Head;
    Head = Init_List();
    while (1)
    {
        system("cls");
        Menu_Display();
        scanf("%c", &Key);
        getchar();
        switch (Key)
        {
        case '0':
            exit(0);
            break;
        case '1':
            system("cls");
            Head = Create_Student(Head);
            Output(Head);
            printf("\n�����������\n");
            getchar();
            break;
        case '2':
            system("cls");
            Head = SelectSort_Dirs(Head);
            Output(Head);
            printf("\n�����������\n");
            getchar();
            break;
        case '3':
            system("cls");
            Head = SelectSort_Score(Head);
            Output(Head);
            printf("\n�����������\n");
            getchar();
            break;
        case '4':
            system("cls");
            printf("������ѧ��������");
            scanf("%s", &Name);
            Find_Student_Name(Head, &Name);
            printf("\n�����������\n");
            getchar();
            getchar();
            break;
        case '5':
            system("cls");
            printf("������ѧ��ѧ�ţ�");
            scanf("%s", &Num);
            Find_Student_Number(Head, &Num);
            printf("\n�����������\n");
            getchar();
            getchar();
            break;
        case '6':
            Sort_stu(Head);
            printf("\n�����������\n");
            getchar();
        case '7':
            break;
        default:

            break;
        }
    }
    return 0;
}
#include "stdio.h"
#include "stdlib.h"
#include "conio.h"
#include <windows.h>
/* 学生定义 */
typedef float datatype;
typedef struct Student
{
    char Stu_Number[20];
    char Stu_Name[20];
    datatype Score;
    struct Student *next;
} Link_List;

int Student_Total = 0;

struct Student_Sort
{
    int pass;
    int middle;
    int good;
    int execlent;
    int unpass;
} Student_sort;

/**
 * @brief  创建带头节点的单链表
 * @param  无
 * @retval 返回节点类型的变量指针
 */
Link_List *Init_List()
{
    Link_List *Head;
    Head = (Link_List *)malloc(sizeof(Link_List));
    Head->next = NULL;
    return Head;
}
/**
 * @brief  输出链表中学生信息
 * @param  头节点
 * @retval 无
 */
void Output(Link_List *Head)
{
    Link_List *p;
    p = Head->next;
    if (!p)
    {
        printf("\n尚未录入无学生！\n");
    }
    else
    {
        printf("\t学号\t|\t学生\t|\t成绩\t\t\n");
        while (p)
        {
            printf("\t%4s\t|\t%4s\t|\t%4f\t\t\n", p->Stu_Number, p->Stu_Name, p->Score);
            p = p->next;
        }
    }
}

/**
 * @brief  录入学生信息
 * @param  头节点，学生总数
 * @retval 头节点
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
    printf("\t请输入学生姓名:");
    gets(p->Stu_Name);
    printf("\t请输入学生学号:");
    gets(p->Stu_Number);
    printf("\t请输入学生分数:");
    scanf("%f", &p->Score);
    getchar();
    Student_Total++;
    printf("\t录入成功！\n");
    p->next = NULL; /* 直接插在链表最后空的地方 */
    q->next = p;
    return Head;
}
/**
 * @brief  对学生进行字典排序
 * @param  头节点，学生总数
 * @retval 头节点
 */
Link_List *SelectSort_Dirs(Link_List *Head)
{
    Link_List *pfirst;
    Link_List *ptail;
    Link_List *pminBefore;
    Link_List *pmin;
    Link_List *p;
    Link_List *head;//定义带头节点后的第一个节点
    pfirst = NULL;
    head=Head->next;
    while (head != NULL)
    {
        for (p = head, pmin = head; p->next != NULL; p = p->next)/* 循环至将全部节点排序到pfirst-ptail的新链表中（头节点为pfirst 尾节点为ptail的链表） */
        {
            if (strcmp(p->next->Stu_Name, pmin->Stu_Name) < 0)/* 找到最小的节点，标记最小的节点，和最小节点的前一个节点 */
            {
                pminBefore = p;
                pmin = p->next;
            }
        }
        if (pfirst == NULL)                                 /* 把最小的节点放入新链表（头节点为pfirst 尾节点为ptail的链表） */
        {
            pfirst = pmin;
            ptail = pmin;
        }
        else                                                /* 将后续循环中的最小节点依次放入新链表 */
        {
            ptail->next = pmin;
            ptail = pmin;
        }
        if (pmin == head)                                   /* 如果最小值为头节点时，链表的头节点移到下一位 */
        {
            head = head->next;
        }
        else                                                /* 如果不是头节点，直接将原链表的最小节点跳过，形成去掉最小值的原链表 */
        {
            pminBefore->next = pmin->next;
        }
    }
    if (pfirst != NULL)                                     /* 判断有没有新链表 */
    {
        ptail->next = NULL;                                 /* 新链表的尾节点下一位置空 */
    }
    Head->next = pfirst;                                    /* 带头节点指向新链表 */
    return Head;
}

/**
 * @brief  对学生按成绩高低排序
 * @param  头节点
 * @retval 头节点
 */
Link_List *SelectSort_Score(Link_List *Head)
{
    Link_List *pfirst;
    Link_List *ptail;
    Link_List *pmaxBefore;
    Link_List *pmax;
    Link_List *p;
    Link_List *head;//定义带头节点后的第一个节点
    pfirst = NULL;
    head=Head->next;
    while (head != NULL)/* 循环至将全部节点排序到pfirst-ptail的新链表中（头节点为pfirst 尾节点为ptail的链表） */
    {
        for (p = head, pmax = head; p->next != NULL; p = p->next)/* 找到最大的节点，标记最大的节点和最大节点的前一个节点 */
        {
            if (p->next->Score > pmax->Score)
            {
                pmaxBefore = p;
                pmax = p->next;
            }
        }
        if (pfirst == NULL) /* 把最大的节点放入新链表（头节点为pfirst 尾节点为ptail的链表） */
        {
            pfirst = pmax;
            ptail = pmax;
        }
        else                /* 将后续循环中的最大节点依次放入新链表 */
        {
            ptail->next = pmax;
            ptail = pmax;
        }
        if (pmax == head)   /* 如果最大值为头节点时，链表的头节点移到下一位 */
        {
            head = head->next;
        }
        else                /* 如果不是头节点，直接将原链表的最大节点跳过，形成去掉最大值的原链表 */
        {
            pmaxBefore->next = pmax->next;
        }
    }
    if (pfirst != NULL)     /* 判断有没有新链表 */
    {
        ptail->next = NULL; /* 新链表的尾节点下一位置空 */
    }
    Head->next = pfirst;    /* 带头节点指向新链表 */
    return Head;
}
/**
 * @brief  根据姓名查找学生
 * @param  头节点，姓名
 * @retval 无
 */
void Find_Student_Name(Link_List *head, char *stuname)
{
    Link_List *p = head;
    char flag = 0;
    while (p != NULL)
    {
        if (strcmp(p->Stu_Name, stuname) == 0)
        {
            printf("\t找到该生信息：\n");
            printf("\t学号\t|\t学生\t|\t成绩\t\t\n");
            printf("\t%4s\t|\t%4s\t|\t%4f\t\t\n", p->Stu_Number, p->Stu_Name, p->Score);
            flag = 1;
        }
        p = p->next;
    }
    if (flag == 0)
    {
        printf("\t未找到该学生\n");
    }
}
/**
 * @brief  根据学号查找学生
 * @param  头节点，学号
 * @retval 无
 */
void Find_student_num(Link_List *head, char *num)
{
    Link_List *p = head;
    char flag = 0;
    while (p != NULL)
    {
        if (strcmp(p->Stu_Number, num) == 0)
        {
            printf("\t找到该生信息：\n");
            printf("\t学号\t|\t学生\t|\t成绩\t\t\n");
            printf("\t%4s\t|\t%4s\t|\t%4f\t\t\n", p->Stu_Number, p->Stu_Name, p->Score);
            flag = 1;
        }
        p = p->next;
    }
    if (flag == 0)
    {
        printf("\t未找到该学生\n");
    }
}
/**
 * @brief  对学生进行分类
 * @param  头节点
 * @retval 无
 */
void Sort_stu(Link_List *head)
{
    Link_List *p = head;
    if (Student_Total == 0)
    {
        printf("\t学生人数为0！！\n");
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
    printf("\t优秀人数：%d 占比：%f\n", Student_sort.execlent, (float)Student_sort.execlent / Student_Total);
    printf("\t良人数：%d 占比：%f\n", Student_sort.good, (float)Student_sort.good / Student_Total);
    printf("\t中人数：%d 占比：%f\n", Student_sort.middle, (float)Student_sort.middle / Student_Total);
    printf("\t及格人数：%d 占比：%f\n", Student_sort.pass, (float)Student_sort.pass / Student_Total);
    printf("\t不及格人数：%d 占比：%f\n", Student_sort.unpass, (float)Student_sort.unpass / Student_Total);
}
/**
 * @brief  菜单显示界面
 * @param  无
 * @retval 无
 */
void Menu_Display()
{
    printf("\t\t学生成绩管理系统\t\t\n");
    printf("\t1.录入学生\n");
    printf("\t2.根据学生姓名进行字典排序\n");
    printf("\t3.根据成绩进行由高到低排序\n");
    printf("\t4.根据学号查找学生成绩\n");
    printf("\t5.根据姓名查找学生成绩\n");
    printf("\t6.统计及格、中、良、优秀和不及格学生人数及其所占比例\n");
    printf("\t0.退出\n");
    printf("\t请输入编号0-6：\n");
}
/**
 * @brief  隐藏光标
 * @param  无
 * @retval 无
 */
void HideCursor()
{
    CONSOLE_CURSOR_INFO curInfo;                     //定义光标信息的结构体变量
    curInfo.dwSize = 1;                              //如果没赋值的话，光标隐藏无效
    curInfo.bVisible = FALSE;                        //将光标设置为不可见
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE); //获取控制台句柄
    SetConsoleCursorInfo(handle, &curInfo);          //设置光标信息
}
/**
 * @brief  定位光标
 * @param  光标位置
 * @retval 无
 */
void CursorJump(int x, int y)
{
    COORD pos;                                       //定义光标位置的结构体变量
    pos.X = x;                                       //横坐标
    pos.Y = y;                                       //纵坐标
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE); //获取控制台句柄
    SetConsoleCursorPosition(handle, pos);           //设置光标位置
}
main()
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
            printf("\n按任意键返回\n");
            getchar();
            break;
        case '2':
            system("cls");
            Head = SelectSort_Dirs(Head);
            Output(Head);
            printf("\n按任意键返回\n");
            getchar();
            break;
        case '3':
            system("cls");
            Head = SelectSort_Score(Head);
            Output(Head);
            printf("\n按任意键返回\n");
            getchar();
            break;
        case '4':
            system("cls");
            printf("请输入学生姓名：");
            scanf("%s", &Name);
            Find_Student_Name(Head, &Name);
            printf("\n按任意键返回\n");
            getchar();
            getchar();
            break;
        case '5':
            system("cls");
            printf("请输入学生学号：");
            scanf("%s", &Num);
            Find_Student_Name(Head, &Num);
            printf("\n按任意键返回\n");
            getchar();
            getchar();
            break;
        case '6':
            Sort_stu(Head);
            printf("\n按任意键返回\n");
            getchar();
        case '7':
            break;
        default:

            break;
        }
    }
}
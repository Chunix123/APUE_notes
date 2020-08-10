#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include <math.h>


#define OK 1
#define ERROR 0
#define TRUE 1
#define FALSE 0

#define MAXSIZE 20

typedef int Status;
typedef int ElemType;

typedef struct Node{
    ElemType data;
    struct Node *next;
} Node;

typedef struct Node *LinkList;


Status LinkListVisit(ElemType c)
{
    printf("%d ", c);
    
    return OK;
}

Status InitList(LinkList *L)
{
    *L = (LinkList)malloc(sizeof(Node));
    
    if(!(*L))
        return ERROR;
    
    (*L)->next = NULL;
    
    return OK;
}

Status ListEmpty(LinkList L)
{
    if(L->next)
        return FALSE;
    else
        return TRUE;
}

Status ClearList(LinkList *L)
{
    LinkList p,q;
    
    p = (*L)->next;
    
    while(p)
    {
        q = p->next;
        free(p);
        p = q;
    }
    (*L)->next = NULL;
    
    return OK;
}

Status ListLength(LinkList L)
{
    int i = 0;
    
    LinkList p = L->next;
    
    while(p)
    {
        i++;
        p = p->next;
    }
    
    return i;
}

Status GetElem(LinkList L, int i, ElemType *e)
{
    int j;
    LinkList p;
    
    p = L->next;
    j = 1;
    
    while(p && j < i)
    {
        p = p->next;
        ++j;
    }
    
    if(!p || j > i)
        return ERROR;
    
    *e = p->data;
    
    return OK;
}

Status LocateElem(LinkList L, ElemType e)
{
    int i = 0;
    LinkList p = L->next;
    
    while(p)
    {
        i++;
        if(p->data == e)
            return i;
        p = p->next;
    }
    
    return 0;
}

Status ListInsert(LinkList *L, int i, ElemType e)
{
    int j;
    LinkList p,s;
    
    p = *L;
    j = 1;
    
    while(p && j < i)
    {
        p = p->next;
        ++j;
    }
    
    if(!p || j > i)
        return ERROR;
    
    s = (LinkList)malloc(sizeof(Node));
    s->data = e;
    s->next = p->next;
    p->next = s;
    
    return OK;
}

Status ListDelete(LinkList *L, int i, ElemType *e)
{
    int j;
    LinkList p, q;
    
    p = *L;
    j = 1;
    
    while(p->next && j < i)
    {
        p = p->next;
        ++j;
    }
    
    if(!(p->next) || j > i)
        return ERROR;
    
    q = p->next;
    p->next = q->next;
    *e = q->data;
    free(q);
    
    return OK;
}

Status ListTraverse(LinkList L)
{
    LinkList p = L->next;
    
    while(p)
    {
        LinkListVisit(p->data);
        p = p->next;
    }
    printf("\n");
    
    return OK;
}

void CreateListHead(LinkList *L, int n)
{
    LinkList p;
    int i;
    
    srand(time(0));
    *L = (LinkList)malloc(sizeof(Node));
    (*L)->next = NULL;
    
    for(i = 0; i < n; i++)
    {
        p = (LinkList)malloc(sizeof(Node));
        p->data = rand() % 100 + 1;
        p->next = (*L)->next;
        (*L)->next = p;
    }
}

void CreateListTail(LinkList *L, int n)
{
    LinkList p, r;
    int i;
    
    srand(time(0));
    *L = (LinkList)malloc(sizeof(Node));
    r = *L;
    
    for(i = 0; i < n; i++)
    {
        p = (LinkList)malloc(sizeof(Node));
        p->data = rand() % 100 + 1;
        r->next = p;
        r = p;
    }
    p->next = NULL;
}

int main()
{
    LinkList L;
    ElemType e;
    Status i;
    int j,k;
    
    i = InitList(&L);
    printf("初始化L后：ListLength(L) = %d\n", ListLength(L));
    for(j = 1; j <= 5; j++)
        i = ListInsert(&L, 1, j);
    printf("在L的表头依次插入1～5后：L.data = ");
    ListTraverse(L); 
    
    printf("ListLength(L) = %d \n", ListLength(L));
    i = ListEmpty(L);
    printf("L是否空：i = %d(1:是 0:否)\n", i);
    i = ClearList(&L);
    printf("清空L后：ListLength(L) = %d\n", ListLength(L));
    i = ListEmpty(L);
    printf("L是否空：i = %d(1:是 0:否)\n", i);

    for(j = 1; j <= 10; j++)
            ListInsert(&L, j, j);
    printf("在L的表尾依次插入1～10后：L.data = ");
    ListTraverse(L); 
    
    printf("ListLength(L) = %d \n", ListLength(L));
    
    ListInsert(&L, 1, 0);
    printf("在L的表头插入0后：L.data = ");
    ListTraverse(L); 
    printf("ListLength(L) = %d \n", ListLength(L));

    GetElem(L, 5, &e);
    printf("第5个元素的值为：%d\n", e);
    for(j = 3; j <= 4; j++)
    {
            k = LocateElem(L, j);
            if(k)
                    printf("第%d个元素的值为%d\n",k,j);
            else
                    printf("没有值为%d的元素\n",j);
    }
    
    k = ListLength(L); /* k为表长 */
    for(j = k + 1; j >= k; j--)
    {
            i = ListDelete(&L, j, &e); /* 删除第j个数据 */
            if(i == ERROR)
                    printf("删除第%d个数据失败\n", j);
            else
                    printf("删除第%d个的元素值为：%d\n", j, e);
    }
    printf("依次输出L的元素：");
    ListTraverse(L); 
    
    j = 5;
    ListDelete(&L, j, &e); /* 删除第5个数据 */
    printf("删除第%d个的元素值为：%d\n", j, e);
    
    printf("依次输出L的元素：");
    ListTraverse(L); 
    
    i = ClearList(&L);
    printf("\n清空L后：ListLength(L) = %d\n", ListLength(L));
    CreateListHead(&L, 20);
    printf("整体创建L的元素(头插法)：");
    ListTraverse(L); 
    
    i = ClearList(&L);
    printf("\n删除L后：ListLength(L) = %d\n", ListLength(L));
    CreateListTail(&L, 20);
    printf("整体创建L的元素(尾插法)：");
    ListTraverse(L);
    
    return 0;
}
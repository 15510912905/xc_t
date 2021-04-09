#include "double_list.h"
#include "includes.h"

//DList *dlist_create()
//{
//    DListNode* pnode = NULL;
//    DList    * dlist = NULL;

//    dlist = (DList *)calloc(1, sizeof(DList));
//    if (dlist == NULL)
//    {
////         enableWriteBBblog();
////         WriteBBLog("dlist_create dlist is null.");
////         disableWriteBBblog();
//        return NULL;
//    }

//    pnode = (DListNode *)calloc(1, sizeof(DListNode));
//    
//    if (pnode == NULL)
//    {
//        FREE_MEMORY(dlist);
////         enableWriteBBblog();
////         WriteBBLog("dlist_create pnode is null.");
////         disableWriteBBblog();
//        return NULL;
//    }

//    pnode->data = NULL;
//    pnode->next = NULL;
//    pnode->prev = NULL;

//    dlist->head    = pnode;
//    dlist->current = pnode;
//    
//    return dlist;
//}

//static DListNode *dlist_alloc()
//{
//    DListNode * ret_p = NULL;

//    ret_p = (DListNode *)calloc(1, sizeof(DListNode));

//    if (NULL == ret_p)
//    {
////         enableWriteBBblog();
////         WriteBBLog("dlist_alloc fail.");
////         disableWriteBBblog();
//        return NULL;
//    }

//    return ret_p;   
//}

//int data_dlist_check(DList *dlist,  void *data)
//{
//    if (dlist == NULL)
//    {
//        return 0;
//    }    
//    
//    if (data == NULL)
//    {
//        return 0;
//    }

//    return 1;
//}

//Ret dlist_add(DList *dlist, int index, void *data)
//{
//    int flag                = 1;
//    int len                 = 0;
//    DListNode *node         = NULL;
//    DListNode *dlistnode    = NULL;

//    flag = data_dlist_check(dlist, data);//����б�������Ƿ�Ϊ��
//    if (0 == flag)
//    {
//        return RET_FAULT;
//    }

//    len = dlist_len(dlist);
//    return_val_if_fail(index <= len, RET_FAULT);//��֤��ӵ������в�����β�� �������ⲿ
//    
//    if (index > len)
//    {
//        return RET_FAULT;
//    }

//    node = dlist_alloc();
//    if (node == NULL)
//    {
//        return RET_OOM;
//    }

//    node->data = data;

//    if (((len == 0) && (index == 0)) || (index < 0))//index��ô��С��0
//    {
//        //tail
//        dlist->current->next = node;
//        node->prev = dlist->current;
//        dlist->current = node;

//        return RET_OK;
//    }
//    else if ((len != 0) && (index == 0))
//    {
//        //head
//        dlist->head->next->prev = node;
//        node->prev = dlist->head;
//        node->next = dlist->head->next;
//        dlist->head->next = node;

//        return RET_OK;
//    }
//    else if ((len != 0) && (index != 0))
//    {
//        dlistnode = dlist_get(dlist, index);

//        if (dlistnode == NULL)
//        {
//            return RET_FAULT;
//        }
//        dlistnode->prev->next = node;
//        node->prev = dlistnode->prev;
//        node->next = dlistnode;
//        dlistnode->prev = node;

//        return RET_OK;
//    }

//    return RET_FAULT;
//}

//Ret dlist_delete(DList *dlist, int index)
//{
//    int len = 0;
//    DListNode *dlistnode = NULL;

//    return_val_if_fail((dlist != NULL), RET_FAULT);

//    if (dlist == NULL)
//    {
//        return RET_FAULT;
//    }

//    len = dlist_len(dlist);
//    return_val_if_fail(index <= len, RET_FAULT);

//    if (index > len)
//    {
//        return RET_FAULT;
//    }

//    if ((index > 0) && (index <len))
//    {
//        dlistnode = dlist_get(dlist, index);
//        if (dlistnode == NULL)
//        {
//            return RET_FAULT;
//        }

//        dlistnode->next->prev = dlistnode->prev;
//        dlistnode->prev->next = dlistnode->next;
//        dlistnode->prev = NULL;
//        dlistnode->next = NULL;
//        dlistnode->data = NULL;
//        FREE_MEMORY(dlistnode);

//        return RET_OK;
//    }
//    else if ((index <= 0) || (index == len))
//    {
//        dlistnode = dlist->current->prev;
//        dlist->current->prev->next = NULL;
//        dlist->current->prev = NULL;
//        dlist->current->data = NULL;
//        FREE_MEMORY(dlist->current);
//        dlist->current = dlistnode;

//        return RET_OK;
//    }

//    return  RET_FAULT;
//}

///*****************************************************************************
// �� �� ��  : dlist_get
// ��������  : �õ��ڵ�
// �������  : dlist:Ҫ����������ָ���ַ index:λ��
// �������  : ��
// �� �� ֵ  : 
// 
// �޸���ʷ      :
//  1.��    ��   : 2014��4��22��
//    ��    ��   : deng

//    �޸�����   : �����ɺ���

//*****************************************************************************/
//DListNode *dlist_get(DList *dlist, int index)
//{
//    int n = 1;
//    DListNode *dlistnode = NULL;

//    return_val_if_fail((dlist != NULL), NULL);

//    dlistnode = dlist->head;
//    while (n <= index)
//    {
//        dlistnode = dlistnode->next;
//        n = n + 1;
//    }

//    return dlistnode;
//}

///*****************************************************************************
// �� �� ��  : dlist_foreach
// ��������  : ��������ִ��visitfunc����
// �������  : dlist:ָ������ visitfunc���ص����� ctx��������
// �������  : ��
// �� �� ֵ  : 
// 
// �޸���ʷ      :
//  1.��    ��   : 2014��4��22��
//    ��    ��   : deng

//    �޸�����   : �����ɺ���

//*****************************************************************************/
//void *dlist_foreach(DList *dlist, VisitFunc visit_func,void *ctx)
//{
//    DListNode *ipointer = NULL;
//    void      *retp     = NULL;

//    return_val_if_fail(((dlist != NULL) && (visit_func != NULL)), NULL);

//    ipointer = dlist->head->next;
//    while (ipointer != NULL)
//    {
//        retp = visit_func(ctx, ipointer->data);
//        ipointer = ipointer->next;
//    }

//    return retp;
//}

///*****************************************************************************
// �� �� ��  : dlist_len
// ��������  : ��ó���
// �������  : dlist:ָ������
// �������  : 
// �� �� ֵ  : 
// 
// �޸���ʷ      :
//  1.��    ��   : 2014��4��22��
//    ��    ��   : deng

//    �޸�����   : �����ɺ���

//*****************************************************************************/
//int dlist_len(DList *dlist)
//{
//    int len = 0;
//    DListNode *dlistnode = NULL;

//    return_val_if_fail((dlist != NULL), RET_FAULT);//���dlis�Ƿ�λ��

//    dlistnode = dlist->head;//�õ��б��ͷ
//    while (dlistnode->next != NULL)//ѭ����һ��list�ĳ��� ָ�������β��
//    {
//        dlistnode = dlistnode->next;
//        len = len + 1;
//    }

//    return len;
//}

///*****************************************************************************
// �� �� ��  : dlist_destroy
// ��������  : �ͷ�ָ�������ڴ�
// �������  : dlist:ָ������
// �������  : ��
// �� �� ֵ  : 
// 
// �޸���ʷ      :
//  1.��    ��   : 2014��4��22��
//    ��    ��   : deng

//    �޸�����   : �����ɺ���

//*****************************************************************************/
//Ret dlist_destroy(DList *dlist, DesFunc des_func, int index)
//{
//    DListNode* cursor = NULL;

//    if (des_func != NULL)
//    {
//        des_func(dlist, index);
//    }
//    else
//    {
//        while ((dlist->current != NULL) && (dlist->current != dlist->head))
//        {
//            cursor = dlist->current->prev;
//            FREE_MEMORY(dlist->current->data);
//            FREE_MEMORY(dlist->current);
//            dlist->current = cursor ;
//        }
//        
//        if (dlist->head != NULL)
//        {
//            FREE_MEMORY(dlist->head->data);
//            FREE_MEMORY(dlist->head);
//            dlist->current = NULL;
//        }
//        
//        if (dlist != NULL)
//        {
//            FREE_MEMORY(dlist);
//        }
//    }

//    return RET_OK;
//}

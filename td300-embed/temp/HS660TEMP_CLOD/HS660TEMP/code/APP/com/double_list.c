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

//    flag = data_dlist_check(dlist, data);//检查列表和数据是否为空
//    if (0 == flag)
//    {
//        return RET_FAULT;
//    }

//    len = dlist_len(dlist);
//    return_val_if_fail(index <= len, RET_FAULT);//保证添加到链表中部或者尾部 ，不是外部
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

//    if (((len == 0) && (index == 0)) || (index < 0))//index怎么会小于0
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
// 函 数 名  : dlist_get
// 功能描述  : 得到节点
// 输入参数  : dlist:要操作的链表指针地址 index:位置
// 输出参数  : 无
// 返 回 值  : 
// 
// 修改历史      :
//  1.日    期   : 2014年4月22日
//    作    者   : deng

//    修改内容   : 新生成函数

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
// 函 数 名  : dlist_foreach
// 功能描述  : 遍历链表，执行visitfunc函数
// 输入参数  : dlist:指定链表 visitfunc：回调函数 ctx：上下文
// 输出参数  : 无
// 返 回 值  : 
// 
// 修改历史      :
//  1.日    期   : 2014年4月22日
//    作    者   : deng

//    修改内容   : 新生成函数

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
// 函 数 名  : dlist_len
// 功能描述  : 获得长度
// 输入参数  : dlist:指定链表
// 输出参数  : 
// 返 回 值  : 
// 
// 修改历史      :
//  1.日    期   : 2014年4月22日
//    作    者   : deng

//    修改内容   : 新生成函数

//*****************************************************************************/
//int dlist_len(DList *dlist)
//{
//    int len = 0;
//    DListNode *dlistnode = NULL;

//    return_val_if_fail((dlist != NULL), RET_FAULT);//检查dlis是否位空

//    dlistnode = dlist->head;//拿到列表的头
//    while (dlistnode->next != NULL)//循环数一下list的长度 指导链表的尾部
//    {
//        dlistnode = dlistnode->next;
//        len = len + 1;
//    }

//    return len;
//}

///*****************************************************************************
// 函 数 名  : dlist_destroy
// 功能描述  : 释放指定链表内存
// 输入参数  : dlist:指定链表
// 输出参数  : 无
// 返 回 值  : 
// 
// 修改历史      :
//  1.日    期   : 2014年4月22日
//    作    者   : deng

//    修改内容   : 新生成函数

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

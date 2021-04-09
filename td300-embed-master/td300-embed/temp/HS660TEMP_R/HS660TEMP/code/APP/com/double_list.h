#ifndef __DOUBLE_LIST_H__
#define __DOUBLE_LIST_H__

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

//typedef struct _DListNode
//{
//    void *data;
//    struct _DListNode *prev;
//    struct _DListNode *next;
//}DListNode;

//typedef struct _DList
//{
//    DListNode *head;
//    DListNode *current;
//}DList;

//typedef enum _Ret
//{
//    RET_OK = 1,
//    RET_FAULT,
//    RET_OOM,
//}Ret;

//typedef void*(*VisitFunc)(void *ctx, void* data);

//typedef void (*DesFunc)(DList *dlist, int index);

//DList *dlist_create(void);
//int dlist_len(DList *dlist);
//DListNode *dlist_get(DList *dlist, int index);
//Ret dlist_delete(DList *dlist, int index);
//Ret dlist_add(DList *dlist, int index,  void *data);
//void *dlist_foreach(DList *dlist, VisitFunc visit_func, void *ctx);
//Ret dlist_destroy(DList *dlist,DesFunc des_func, int index);

//#define return_val_if_fail(p, val) if(!(p)) {return val;}


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif /* __DOUBLE_LIST_H__ */

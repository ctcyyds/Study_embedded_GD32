
#ifndef DATASTRUCT_LINK_H
#define DATASTRUCT_LINK_H
#include <stdlib.h>
#include <stdio.h>

//数据类型起别名
typedef int SListData;
//节点的结构体定义
typedef struct SListNode{
    //节点保存的数据
    SListData data;
    //下一个节点的指针
    struct SListNode *next;
}SListNode;

//创建节点(保证能够不主动销毁能够一直使用)
//接收data数据,会在堆内存中申请空间,返回堆内存的地址
SListNode *SListCreateNode(SListData data);

//头部添加
void SListAddHead(SListNode **node,SListData data);
//尾部添加元素(涉及到修改元素,一般都传二级指针)
void SListAddTail(SListNode **node,SListData data);

//头部删除
void SListDelHead(SListNode **node);
//尾部删除
SListData SListDelTail(SListNode **node);

//链表长度
int SListLength(SListNode*node);
//链表是否为空 1:为空 0:不为空
int SListIsEmpty(SListNode*node);

//遍历链表
void SListTravel(SListNode *node);

//释放链表资源
void SListFree(SListNode **head);

#endif //DATASTRUCT_LINK_H

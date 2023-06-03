#include "link.h"

//创建节点(保证能够不主动销毁能够一直使用)
//接收data数据,会在堆内存中申请空间,返回堆内存的地址
SListNode *SListCreateNode(SListData data){
    SListNode *node = malloc(sizeof(struct SListNode));
    if(node==NULL){
        return NULL;
    }
    //节点初始化
    node->data = data;
    node->next = NULL;
    return node;
}

//头部添加
void SListAddHead(SListNode **node,SListData data){
    //传递NULL指针,不做任何处理
    if(node==NULL){
        return;
    }
    //创建节点
    SListNode *tmp = SListCreateNode(data);
    //没有任何一个节点
    if(*node==NULL){
        *node = tmp;
        return;
    }
    //有节点 1.创建的节点的next指向原来的首节点  2.link指向新的节点
    tmp->next = *node;
    *node = tmp;
}

//尾部添加
void SListAddTail(SListNode **node,SListData data){
    //传递NULL指针,不做任何处理
    if(node==NULL){
        return;
    }
    //创建节点
    SListNode *tmp = SListCreateNode(data);

    //没有任何一个节点
    if(*node==NULL){
        *node = tmp;
        return;
    }
    //有节点
    SListNode *cur = *node;
    //经过循环之后,cur停留在最后一个节点
    while (cur->next!=NULL){
        cur = cur->next;
    }
    //把最后一个节点指向新创建的节点
    cur->next = tmp;
}
//删除头部
void SListDelHead(SListNode **node){
    //1.传递NULL指针  2.或者link保存NULL(链表为空)
    if (node==NULL||*node==NULL){
        return;
    }
    //1.保存头节点  2.把link指向头节点下一个节点  3.释放保存的头结点的内存
    //1.保存头结点
    SListNode *head = *node;
    //2.把link指向头节点下一个节点
    *node = head->next;
    //3.释放保存的头结点的内存
    free(head);
}
//尾部删除
SListData SListDelTail(SListNode **node){
    SListData ele = -1;
    if(node==NULL){
        return ele;
    }
    if(*node==NULL){
        return ele;
    }
    SListNode *pre = NULL;
    SListNode *cur = *node;

    //只有一个元素
    if(cur->next==NULL){
        ele = cur->data;
        free(cur);
        *node = NULL;
        return ele;
    }
    //有多个元素
    while (cur->next!=NULL){
        pre = cur;
        cur = cur->next;
    }
    ele = cur->data;
    free(cur);
    pre->next = NULL;
    return ele;
}

//遍历链表
void SListTravel(SListNode *node){
    //如果链表为空
    if (node==NULL){
        return;
    }
    //第一个节点的指针
    SListNode *cur = node;
    //如果指针不为NULL,可以打印节点的值
    while (cur!=NULL){
        printf("%d ",cur->data);
        cur = cur->next;
    }
		printf("\n");
}

//链表长度
int SListLength(SListNode*node){
    int length = 0;
    SListNode *cur = node;
    while (cur!=NULL){
        length++;
        cur = cur->next;
    }
    return length;
}
//链表是否为空 1:为空 0:不为空
int SListIsEmpty(SListNode*node){
    return node==NULL;
}

//释放链表资源
void SListFree(SListNode **head) {
    SListNode *current = *head;
    SListNode *next;

    while (current != NULL) {
        next = current->next;
        free(current);
        current = next;
    }

    *head = NULL;
}
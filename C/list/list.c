/*****************************
   Data structure - List
realized with linked list
*****************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef int ITEM; //根据数据类型需要修改此处的定义

typedef struct node //链表节点
{
	ITEM item;
	struct node * pNext;
}NODE;

typedef NODE * LIST;

LIST CreateList(void);							//创建带表头的空链表
NODE * First(LIST list);						//返回链表的第一个节点指针（不是头结点）
NODE * Advance(NODE * pNode);					//返回给定节点的下一个节点
ITEM * Retrieve(NODE * pNode);					//返回给定节点中的item项的指针
ITEM * GetKth(LIST list, int k);				//返回第 k 个元素，（k 从 1 开始）
int IsLast(NODE * pNode);						//判断给定节点是不是最后一个节点
int Insert(LIST list, ITEM * pItem, int k);		//在第 k 个节点后插入给定元素，（k 从 1 开始）
int Prepend(LIST list, ITEM * pItem);			//在链表的最前面，也就是头结点之后，插入元素
//int Append(LIST list, ITEM item);				//在链表的最后面追加元素（未实现）
//int Delete(LIST list, int k);					//删除第 k 个节点（k 从 1 开始）

int Reverse(LIST list);							//反转整个链表

LIST CreateList()
{
	NODE * header = (NODE *)malloc(sizeof(NODE));
	if (header != NULL)
		header->pNext = NULL;
	return header;
}

NODE * First(LIST list)
{
	return list->pNext;
}

NODE * Advance(NODE * pNode)
{
	return pNode->pNext;
}

ITEM * Retrieve(NODE * pNode)
{
	return &(pNode->item);
}

int IsLast(NODE * pNode)
{
	return pNode->pNext == NULL;
}

ITEM * GetKth(LIST list, int k)
{
	int i;
	
	NODE * pNode = First(list);

	for (i=1; i<k && pNode != NULL; i++)
	{
		pNode = Advance(pNode);
	}

	return (pNode != NULL ? &(pNode->item) : NULL);
}

int Insert(LIST list, ITEM * pItem, int k)
{
	NODE * pNode,  * pNewNode;

	pNewNode  = (NODE *)malloc(sizeof(NODE));
	if (pNewNode == NULL)
	{
		fprintf(stderr, "\nMemory allocation failed!\n");
		return 1;
	}
	
	memcpy(&(pNewNode->item), pItem, sizeof(ITEM));

	pNode = (NODE *)GetKth(list, k);
	pNewNode->pNext = pNode->pNext;
	pNode->pNext    = pNewNode;
	
	return 0;
}

int Prepend(LIST list, ITEM * pItem)
{
	NODE * pNewNode = (NODE *)malloc(sizeof(NODE));
	if (pNewNode == NULL)
	{
		fprintf(stderr, "\nMemory allocation failed!\n");
		return 1;
	}
	memcpy(&(pNewNode->item), pItem, sizeof(ITEM));
	pNewNode->pNext = list->pNext;
	list->pNext = pNewNode;
	return 0;
}

int Reverse(LIST list)
{
	NODE * p1; //节点1
	NODE * p2; //节点2
	NODE * p3; //节点3，用于临时保存节点2的下一个节点
	p1 = First(list);
	if (p1 == NULL)//empty list
		return 0;

	p2 = p1->pNext;
	p1->pNext = NULL;
	
	while (p2 != NULL)
	{
		p3 = p2->pNext; //把节点2的下一个节点保存起来
		p2->pNext = p1; //使 节点2 指向 节点1，这样就反转了 节点1 和 节点2 之间的连接
		                //但这样整个链表就从 节点2 后断开了
		                //这时临时保存的“节点2的下一个节点”就发挥了作用（112行，122行、128行）

		p1 = p2;        //向后推进一个节点
		p2 = p3;        //以反转下一对节点的连接
	}                   //当推进到 p2 == NULL 时，说明 p1 已经到达链表末尾，结束循环
	list->pNext = p1;   //并将头结点指向 p1
	return 0;
}

int printlist(LIST list)
{
	NODE * pNode;
	for (pNode = First(list); pNode != NULL; pNode = Advance(pNode))
	{
		printf("%d\t", *Retrieve(pNode));
	}
	return 0;
}

int main(int argc, char * argv[])
{
	int i;
	LIST testlist = CreateList();

	for (i = 0; i < 10; i++)
	{
		Prepend(testlist, &i);
	}

	printlist(testlist);
	putchar('\n');

	Reverse(testlist);
	printlist(testlist);
	putchar('\n');

	system("pause");
	return 0;
}

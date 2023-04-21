//
// Created by DrizzleDrop on 2022/12/17.
//

#ifndef TEMP_QUEUE_H
#define TEMP_QUEUE_H

#include "Node.h"														// 结点类模板

// 链队列类模板

template<class ElemType>
class LinkQueue
{
protected:
// 数据成员:
    Node<ElemType> *front, *rear;										// 队头队尾指针
    int count;															// 元素个数

public:
// 抽象数据类型方法声明及重载编译系统默认方法声明:
    LinkQueue();														// 无参数的构造函数模板
    virtual ~LinkQueue();												// 析构函数模板
    int Length() const;													// 求队列长度
    bool Empty() const;													// 判断队列是否为空
    void Clear();														// 将队列清空
    void Traverse(void (*visit)(const ElemType &)) const ;				// 遍历队列
    bool OutQueue(ElemType &e);											// 出队操作
    bool OutQueue();													// 出队操作
    bool GetHead(ElemType &e) const;									// 取队头操作
    bool InQueue(const ElemType &e);									// 入队操作
    LinkQueue(const LinkQueue<ElemType> &source);						// 复制构造函数模板
    LinkQueue<ElemType> &operator =(const LinkQueue<ElemType> &source);	// 重载赋值运算符
};

// 链队列类模板的实现部分

template<class ElemType>
LinkQueue<ElemType>::LinkQueue()
// 操作结果：构造一个空队列
{
    rear = front = new Node<ElemType>;									// 生成头结点
    count = 0;															// 初始化元素个数
}

template<class ElemType>
LinkQueue<ElemType>::~LinkQueue()
// 操作结果：销毁队列
{
    Clear();															// 清空队列
    delete front;														// 释放头结点所占空间
}

template<class ElemType>
int LinkQueue<ElemType>::Length() const
// 操作结果：返回队列长度
{
    return count;														// count表示队列元素个数
}

template<class ElemType>
bool LinkQueue<ElemType>::Empty() const
// 操作结果：如队列为空，则返回true，否则返回false
{
    return count == 0;													// count == 0表示队列为空
}

template<class ElemType>
void LinkQueue<ElemType>::Clear()
// 操作结果：清空队列
{
    while (!Empty())
    {	// 队列非空，则出列
        OutQueue();														// 出列
    }
}

template <class ElemType>
void LinkQueue<ElemType>::Traverse(void (*visit)(const ElemType &)) const
// 操作结果：依次对队列的每个元素调用函数(*visit)
{
    for (Node<ElemType> *temPtr = front->next; temPtr != NULL; temPtr = temPtr->next)
    {	// 对队列的每个元素调用函数(*visit)
        (*visit)(temPtr->data);
    }
}


template<class ElemType>
bool LinkQueue<ElemType>::OutQueue(ElemType &e)
// 操作结果：如果队列非空，那么删除队头元素，并用e返回其值，返回true,
//	否则返回false
{
    if (!Empty())
    {	// 队列非空
        Node<ElemType> *temPtr = front->next;							// 指向队列头素
        e = temPtr->data;												// 用e返回队头元素
        front->next = temPtr->next;									// front->next指向下一元素
        if (rear == temPtr)
        {	// 表示出队前队列中只有一个元素，出队后为空队列
            rear = front;
        }
        delete temPtr;													// 释放出队的结点
        count--;														// 出队成功后元素个数自减1
        return true;													// 成功
    }
    else
    {	// 队列为空
        return false;													// 失败
    }
}

template<class ElemType>
bool LinkQueue<ElemType>::OutQueue()
// 操作结果：如果队列非空，那么删除队头元素，返回true,
//	否则返回false
{
    if (!Empty())
    {	// 队列非空
        Node<ElemType> *temPtr = front->next;							// 指向队列头素
        front->next = temPtr->next;									// front->next指向下一元素
        if (rear == temPtr)
        {	// 表示出队前队列中只有一个元素，出队后为空队列
            rear = front;
        }
        delete temPtr;													// 释放出队的结点
        count--;														// 出队成功后元素个数自减1
        return true;													// 成功
    }
    else
    {	// 队列为空
        return false;													// 失败
    }
}

template<class ElemType>
bool LinkQueue<ElemType>::GetHead(ElemType &e) const
// 操作结果：如果队列非空，那么用e返回队头元素，返回true,
//	否则返回false
{
    if (!Empty())
    {	// 队列非空
        Node<ElemType> *temPtr = front->next;							// 指向队列头素
        e = temPtr->data;												// 用e返回队头元素
        return true;													// 成功
    }
    else
    {	// 队列为空
        return false;													// 失败
    }
}

template<class ElemType>
bool LinkQueue<ElemType>::InQueue(const ElemType &e)
// 操作结果：插入元素e为新的队尾，插入成功true，否则返回false
{
    Node<ElemType> *temPtr = new Node<ElemType>(e);					// 生成新结点
    if (temPtr == nullptr)
    {	// 动态内存耗尽
        return false;													// 失败
    }
    else
    {	// 操作成功
        rear->next = temPtr;											// 新结点追加在队尾
        rear = temPtr;													// rear指向新队尾
        count++;														// 入队成功后元素个数加1
        return true;													// 成功
    }
}

template<class ElemType>
LinkQueue<ElemType>::LinkQueue(const LinkQueue<ElemType> &source)
// 操作结果：由队列source构造新队列——复制构造函数模板
{
    rear = front = new Node<ElemType>;	// 生成头结点
    count = 0;															// 初始化元素个数
    for (Node<ElemType> *temPtr = source.front->next; temPtr != nullptr; temPtr = temPtr->next)
    {	// 对source队列的每个元素对当前队列作入队列操作
        InQueue(temPtr->data);
    }
}

template<class ElemType>
LinkQueue<ElemType> &LinkQueue<ElemType>::operator =(const LinkQueue<ElemType> &source)
// 操作结果：将队列source赋值给当前队列——重载赋值运算符
{
    if (&source != this)
    {
        Clear();														// 清空当前队列
        for (Node<ElemType> *temPtr = source.front->next; temPtr != nullptr; temPtr = temPtr->next)
        {	// 对source队列的每个元素对当前队列作入队列操作
            InQueue(temPtr->data);
        }
    }
    return *this;
}
#endif //TEMP_QUEUE_H

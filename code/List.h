#ifndef LIST_H
#define LIST_H
#include "Node.h"                                    // 结点类模板

// 线性链表类模板
template<class ElemType>
class List {
protected:
// 数据成员:
    Node<ElemType> *head;                            // 头结点指针
    mutable int curPosition;                        // 当前位置的序号
    mutable Node<ElemType> *curPtr;                    // 指向当前位置的指针
    int count;                                        // 元素个数

// 辅助函数模板:
    Node<ElemType> *GetElemPtr(int position) const;    // 返回指向第position个结点的指针

public:
// 抽象数据类型方法声明及重载编译系统默认方法声明:
    List();                                        // 无参数的构造函数模板
    virtual ~List();                            // 析构函数模板
    int Length() const;                                // 求线性表长度
    bool Empty() const;                                // 判断线性表是否为空
    void Clear();                                    // 将线性表清空
    void Traverse(void (*visit)(const ElemType &)) const;            // 遍历线性表
    bool GetElem(int position, ElemType &e) const;    // 求指定位置的元素
    Node<ElemType>* GetElem(int position) ;    // 求指定位置的元素
    bool SetElem(int position, const ElemType &e);    // 设置指定位置的元素值
    bool Delete(int position, ElemType &e);            // 删除元素
    bool Delete(int position);                        // 删除元素
    bool Insert(int position, const ElemType &e);    // 插入元素
    List(const List<ElemType> &source);        // 复制构造函数模板
    List<ElemType> &operator=(const List<ElemType> &source); // 重载赋值运算符
};


// 链表类模板的实现部分
template<class ElemType>
Node<ElemType> *List<ElemType>::GetElemPtr(int position) const
// 操作结果：返回指向第position个结点的指针
{
    if (curPosition > position) {    // 当前位置在所查找位置之后,只能从表头开始操作
        curPosition = 0;
        curPtr = head;
    }

    for (; curPosition < position; curPosition++)
        curPtr = curPtr->next;                        // 查找位置position

    return curPtr;
}

template<class ElemType>
List<ElemType>::List()
// 操作结果：构造一个空链表
{
    head = new Node<ElemType>;                        // 构造头结点
    curPtr = head;
    curPosition = 0;                // 初始化当前位置
    count = 0;                                        // 初始化元素个数
}

template<class ElemType>
List<ElemType>::~List()
// 操作结果：销毁线性表
{
    Clear();                                        // 清空线性表
    delete head;                                    // 释放头结点所指空间
}

template<class ElemType>
int List<ElemType>::Length() const
// 操作结果：返回线性表元素个数
{
    return count;
}

template<class ElemType>
bool List<ElemType>::Empty() const
// 操作结果：如线性表为空，则返回true，否则返回false
{
    return head->next == nullptr;
}

template<class ElemType>
void List<ElemType>::Clear()
// 操作结果：清空线性表
{

    while (!Empty()) {    // 表性表非空，则删除第1个元素
        Delete(1);                                    // 删除第1个元素
    }
}

template<class ElemType>
void List<ElemType>::Traverse(void (*visit)(const ElemType &)) const
// 操作结果：依次对线性表的每个元素调用函数(*visit)
{
    for (Node<ElemType> *temPtr = head->next; temPtr != nullptr; temPtr = temPtr->next) {    // 用temPtr依次指向每个元素
        (*visit)(temPtr->data);                        // 对线性表的每个元素调用函数(*visit)
    }
}

template<class ElemType>
bool List<ElemType>::GetElem(int position, ElemType &e) const
// 操作结果：当线性表存在第position个元素时，用e返回其值，返回true,
//	否则返回false
{
    if (position < 1 || position > Length()) {    // position范围错
        return false;                                // 元素不存在
    } else {    // position合法
        Node<ElemType> *temPtr = GetElemPtr(position);                // 取出指向第position个结点的指针
        e = temPtr->data;                            // 用e返回第position个元素的值
        return true;                                // 元素存在
    }
}

template<class ElemType>
Node<ElemType>* List<ElemType>::GetElem(int position)
// 操作结果：当线性表存在第position个元素时，用e返回其值，返回true,
//	否则返回false
{
    if (position < 1 || position > Length()) {    // position范围错
        return nullptr;                                // 元素不存在
    } else {    // position合法
        Node<ElemType> *temPtr = GetElemPtr(position);                // 取出指向第position个结点的指针

        return temPtr;                                // 元素存在
    }
}

template<class ElemType>
bool List<ElemType>::SetElem(int position, const ElemType &e)
// 操作结果：将线性表的第position个位置的元素赋值为e,
//	position的取值范围为1≤position≤Length(),
//	position合法时返回true,否则返回false
{
    if (position < 1 || position > Length()) {    // position范围错
        return false;                                // 范围错
    } else {    // position合法
        Node<ElemType> *temPtr = GetElemPtr(position);                // 取出指向第position个结点的指针
        temPtr->data = e;                            // 设置第position个元素的值
        return true;                                // 设置元素成功
    }
}

template<class ElemType>
bool List<ElemType>::Delete(int position, ElemType &e)
// 操作结果：删除线性表的第position个元素, 并用e返回其值,
//	position的取值范围为1≤position≤Length(),
//	position合法时返回true,否则返回false
{
    if (position < 1 || position > Length()) {    // position范围错
        return false;                                // 范围错
    } else {    // position合法
        Node<ElemType> *temPtr = GetElemPtr(position - 1);            // 取出指向第position-1个结点的指针
        Node<ElemType> *nextPtr = temPtr->next;        // nextPtr为temPtr的后继
        temPtr->next = nextPtr->next;                // 删除结点
        e = nextPtr->data;                            // 用e返回被删结点元素值
        if (position == Length()) {    // 删除尾结点,当前结点变为头结点
            curPosition = 0;                        // 设置当前位置的序号
            curPtr = head;                            // 设置指向当前位置的指针
        } else {    // 删除非尾结点,当前结点变为第position个结点
            curPosition = position;                    // 设置当前位置的序号
            curPtr = temPtr->next;                    // 设置指向当前位置的指针
        }
        count--;                                    // 删除成功后元素个数减1
        delete nextPtr;                                // 释放被删结点
        return true;                                // 删除成功
    }
}

template<class ElemType>
bool List<ElemType>::Delete(int position)
// 操作结果：删除线性表的第position个元素, 并用e返回其值,
//	position的取值范围为1≤position≤Length(),
//	position合法时返回true,否则返回false
{
    if (position < 1 || position > Length()) {    // position范围错
        return false;                                // 范围错
    } else {    // position合法
        Node<ElemType> *temPtr = GetElemPtr(position - 1);            // 取出指向第position-1个结点的指针
        Node<ElemType> *nextPtr = temPtr->next;        // nextPtr为temPtr的后继
        temPtr->next = nextPtr->next;                // 删除结点
        if (position == Length()) {    // 删除尾结点,当前结点变为头结点
            curPosition = 0;                        // 设置当前位置的序号
            curPtr = head;                            // 设置指向当前位置的指针
        } else {    // 删除非尾结点,当前结点变为第position个结点
            curPosition = position;                    // 设置当前位置的序号
            curPtr = temPtr->next;                    // 设置指向当前位置的指针
        }
        count--;                                    // 删除成功后元素个数减1
        delete nextPtr;                                // 释放被删结点
        return true;                                // 删除成功
    }
}


template<class ElemType>
bool List<ElemType>::Insert(int position, const ElemType &e)
// 操作结果：在线性表的第position个元素前插入元素e
//	position的取值范围为1≤position≤Length()+1
//	position合法时返回true, 否则返回false
{
    if (position < 1 || position > Length() + 1) {    // position范围错
        return false;                                // 范围错
    } else {    // position合法
        Node<ElemType> *temPtr = GetElemPtr(position - 1);                // 取出指向第position-1个结点的指针
        Node<ElemType> *newPtr = new Node<ElemType>(e, temPtr->next);    // 生成新结点
        temPtr->next = newPtr;                        // 将temPtr插入到链表中
        curPosition = position;                        // 设置当前位置的序号
        curPtr = newPtr;                            // 设置指向当前位置的指针
        count++;                                    // 插入成功后元素个数加1
        return true;                                // 删除成功
    }
}

template<class ElemType>
List<ElemType>::List(const List<ElemType> &source)
// 操作结果：由线性表source构造新线性表——复制构造函数模板
{
    int sourceLength = source.Length();                // source的长度
    ElemType temElem;                                // 临时元素

    // 初始化一个空线性表
    head = new Node<ElemType>;                        // 构造头结点
    curPtr = head;
    curPosition = 0;                // 初始化当前位置
    count = 0;                                        // 初始化元素个数

    for (int temPos = 1; temPos <= sourceLength; temPos++) {    // 复制数据元素
        source.GetElem(temPos, temElem);            // 取出第temPos个元素
        Insert(Length() + 1, temElem);                // 将temElem插入到当前线性表
    }
}

template<class ElemType>
List<ElemType> &List<ElemType>::operator=(const List<ElemType> &source)
// 操作结果：将线性表source赋值给当前线性表——重载赋值运算符
{
    if (&source != this) {
        int sourceLength = source.Length();            // source的长度
        ElemType temElem;                            // 临时元素
        Clear();                                    // 清空当前线性表

        for (int temPos = 1; temPos <= sourceLength; temPos++) {    // 复制数据元素
            source.GetElem(temPos, temElem);        // 取出第temPos个元素
            Insert(Length() + 1, temElem);            // 将temElem插入到当前线性表
        }
    }
    return *this;
}

#endif // LIST_H

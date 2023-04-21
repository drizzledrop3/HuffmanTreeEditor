#ifndef VECTOR_H
#define VECTOR_H
#include"iostream"

using namespace std;

template<class T>//向量类模板
class Vector {
    T *data;
    int size;
    int max;

public:
    typedef T *iterator;
    typedef const T *const_iterator;

    iterator Begin() { return data; }

    const_iterator Begin() const { return data; }

    iterator End() { return data + size; }

    const_iterator End() const { return data + size; }

    explicit Vector(int n = 10) : size(0), max(n) { data = new T[max]; }//默认及转换构造
    Vector(const Vector &v) : data(0), max(0) { *this = v; }//转换构造
    ~Vector() { delete[]data; }

    Vector &operator=(const Vector &v);//复制赋值

    void Resize(int newmax);//扩容
    void PushBack(const T &item);//尾插
    void PopBack() { --size; }//尾删
    void Clear() { size = 0; }//清表

    T &operator[](int id) { return data[id]; }

    const T &operator[](int id) const { return data[id]; }

    bool Empty() const { return size == 0; }

    int Size() const { return size; }

    int Max() const { return max; }

    const T &Back() const { return data[size - 1]; }

    const T &Front() const { return data[0]; }

    //书上没有的补充
    iterator Insert(iterator itr, const T &item = T()); //在itr所指向的位置插入item
    iterator Erase(iterator itr); //删除itr所指向的元素
};

//成员函数
template<class T>
Vector<T> &Vector<T>::operator=(const Vector<T> &v) {
    delete[]data;
    data = new T[v.max];
    size = v.size;
    max = v.max;
    for (int i = 0; i < size; i++) {
        data[i] = v.data[i];
    }
    return *this;
}

template<class T>
void Vector<T>::Resize(int newmax) {
    if (newmax < max) {
        return;
    }
    T *old = data;
    data = new T[newmax];
    for (int i = 0; i < size; i++) {
        data[i] = old[i];
    }
    max = newmax;
    delete[]old;
}

template<class T>
void Vector<T>::PushBack(const T &item) {
    if (max == size) {
        Resize(2 * max + 1);//为啥+1？
    }
    data[size++] = item;
}

template<class T>
typename Vector<T>::iterator Vector<T>::Insert(iterator itr, const T &item) {

    if (size == max) {
        int n = itr - Begin();
        Resize(2 * max + 1);
        itr = Begin() + n;
    }
    for (iterator p = data + size, q = data + size - 1; p != itr; --p, --q)
        *p = *q;
    *itr = item;
    size++;
    return itr;
}

template<class T>
typename Vector<T>::iterator Vector<T>::Erase(iterator itr) {
    if (!Empty()) {
        for (iterator p = itr, q = itr + 1; q != End(); ++p, ++q)
            *p = *q;

        size--;
    }
    return itr;
}

//普通函数
template<class T>
void Output(Vector<T> v) {
    typename Vector<T>::const_iterator first = v.Begin();//typename是来区分const_iterator是函数成员还是一个类型
    typename Vector<T>::const_iterator last = v.End();
    for (; first != last; first++) {
        cout << *first << '\t';
    }
    cout << endl;
}

#endif // VECTOR_H

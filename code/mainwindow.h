#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include <QMainWindow>
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QKeyEvent>
#include <QDebug>
#include <vector>
#include <QHash>
#include <ctime>
#include "Queue.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


// 文本编辑类
class TextEditor {
private:
    QString fileName, outputName;
    QVector<QString> text;
    QHash<QString, int>map;


    //去标点函数
    void WordStrip(QString &str) {
        for (auto& i : str) {
            if (i < 97 || i > 122) {
                if (i == 39) continue;
                i = 32;
            }
        }
    }

    //字符串单词化读入数组
    void TextPush(QVector<QString> &v, const QString &str) {
        int flag = 0;
        for (int i = 0; i < str.size(); i++) {
            if (str[i] == 32 && flag != i) {
                v.push_back(str.mid(flag, i - flag));
                flag = i + 1;
            } else if (str[i] == 32 && flag == i) flag++;
        }
    }


public:
    // 构造函数，传入文件名
    TextEditor(const QString &file_name, const QString &outputName) {
        this->fileName.operator=(file_name);
        this->outputName.operator=(outputName);
    }

    TextEditor(){}


    // 文本逐行读入
    bool ReadText() {
        QFile file(fileName);
            // 打开文件
            if(file.open(QIODevice::ReadOnly | QIODevice::Text)){
                while(!file.atEnd()){
                    QByteArray line = file.readLine();
                    line = line.toLower();
                    QString str(line);
                    WordStrip(str);
                    TextPush(text, str);
                }
                file.close();
                return true;
            }
           else return false;
        }

    // 进行词频统计
    void WordFrequency() {
        if (text.empty()) {
            qDebug() << "请先进行文本读入操作" << endl;
            return;
        }
        for (auto & i: text) {
            if (map.find(i) != map.end()) {
                map[i] += 1;
            }
            else{
                map.insert(i, 1);
            }

        }
    }

    // 将文本写入文件
       bool WriteText() {
           // 打开文件
           QFile file(outputName);
           if(file.open(QIODevice::ReadWrite | QIODevice::Text)){
               QTextStream stream(&file);
               // 将文本数组中的每一行写入文件
               QHash<QString, int>::const_iterator i = map.constBegin();
                 while (i != map.constEnd()) {
                     stream << i.key() << ": " << i.value() << Qt::endl;
                     ++i;
                 }
               //关闭文件
               file.close();
               return true;
           }
           return false;
       }

    // 获取输出文件名
       QString getOutPutName(){ return outputName; }

    // 获取词频文件
       QHash<QString, int> getWordFrency(){ return map; }
};


// 哈夫曼结点
struct HTNode {
    int mParent{};
    int mRChild{};
    int mLChild{};
    QString data;
    QString code;
    int mWeight{};
    int usWeight{};
};

// 哈夫曼树类
class HTree {
private:
    HTNode *mHTree;
    int mLength = 0;


    // 寻找最小两节点
    void findTwoMinNode(HTree &HT, int pos, int &min1, int &min2) {
        int i = 0;
        int m1, m2;
        int minWeight;
        // find min1
        while (HT.mHTree[i].mParent != -1) i++;
        minWeight = HT.mHTree[i].mWeight;
        m1 = i;
        for (; i < pos; i++) {
            if (HT.mHTree[i].mWeight < minWeight && HT.mHTree[i].mParent == -1) {
                minWeight = HT.mHTree[i].mWeight;
                m1 = i;
            }
        }
        HT.mHTree[m1].mParent = 1;
        min1 = m1;
        // find min2
        i = 0;
        while (HT.mHTree[i].mParent != -1) i++;
        minWeight = HT.mHTree[i].mWeight;
        m2 = i;
        for (; i < pos; i++) {
            if (HT.mHTree[i].mWeight < minWeight && HT.mHTree[i].mParent == -1) {
                minWeight = HT.mHTree[i].mWeight;
                m2 = i;
            }
        }
        HT.mHTree[m2].mParent = 1;
        min2 = m2;
    }


    // 寻找编码对应值
    int searchDataOfCode(QString code){
        int length = (mLength + 1) / 2;
        for(int i = 0; i < length; i++){
            if(code == mHTree[i].code) return i;
        }
        return -1;
    }

public:
    HTree(QHash<QString, int> context) {
        int total = context.size() * 2 - 1;
        mHTree = new HTNode[total];
        if (!mHTree) qDebug() << "init fail" << endl;
        mLength = 0;
        int i;
        QHash<QString, int>::const_iterator temp = context.constBegin();
        for (i = 0; i < context.size() && temp != context.constEnd(); i++) {
            mHTree[i].mLChild = -1;
            mHTree[i].mRChild = -1;
            mHTree[i].mParent = -1;
            mHTree[i].mWeight = temp.value();
            mHTree[i].usWeight = temp.value();
            mHTree[i].data = temp.key();
            mLength++;
            temp++;
        }
        for (; i < total; i++) {
            mHTree[i].mLChild = -1;
            mHTree[i].mRChild = -1;
            mHTree[i].mParent = -1;
            mHTree[i].mWeight = 65535;
            mHTree[i].usWeight = 65535;
            mLength++;
        }
    }

    HTree(){}

    // 创建哈夫曼树
    bool createHuffmanTree(int num) {
        if (!mHTree) return false;
        int i, min1, min2;
        for (i = num; i < mLength; i++) {
            findTwoMinNode(*this, i, min1, min2);
            mHTree[min1].mParent = i;
            mHTree[min2].mParent = i;
            mHTree[i].mLChild = min1;
            mHTree[i].mRChild = min2;
            mHTree[i].mWeight = mHTree[min1].mWeight + mHTree[min2].mWeight;
            mHTree[i].usWeight = mHTree[i].mWeight;
        }
        return true;
    }


    //哈夫曼编码
    void huffmanCoding() {
        int numOfCode = (mLength + 1) / 2;
        char *code = new char[numOfCode];
        int cur = mLength - 1;
        int codeLen = 0;
        //visit state    0:not visited  1:1child visited  2:all visited
        int i;
        for (i = 0; i < mLength; i++) {
            mHTree[i].mWeight = 0;
        }
        while (cur != -1) {
            if (mHTree[cur].mWeight == 0) {
                mHTree[cur].mWeight = 1;
                if (mHTree[cur].mLChild != -1) {
                    code[codeLen++] = '0';
                    cur = mHTree[cur].mLChild;
                } else {
                    code[codeLen] = '\0';
                    mHTree[cur].code = code;
                }
            } else {
                if (mHTree[cur].mWeight == 1) {
                    mHTree[cur].mWeight = 2;
                    if (mHTree[cur].mRChild != -1) {
                        code[codeLen++] = '1';
                        cur = mHTree[cur].mRChild;
                    }
                }
                else {
                    mHTree[cur].mWeight = 0;
                    cur = mHTree[cur].mParent;
                    --codeLen;
                }
            }
        }
        delete[]code;

    }


    // 解码为信息
    QString huffmanDecoding(QString data){
        QString temp;
        int begin = 0, size;
        if(data.length() != 0) size = 1;
        while(begin < data.length()){
            int goal = searchDataOfCode(data.mid(begin, size));
            if(goal != -1) {
                temp += mHTree[goal].data + " ";
                begin += size;
                size = 1;
            }
            else size++;
        }
        return temp;
    }



    // 显示编码
    void showHuffmanCode(int numOfCode) {
        for (int j = 0; j <= numOfCode; j++) {
            qDebug() << "the code of " << mHTree[j].data << " is " << mHTree[j].code << endl;
        }
    }

    // 垂直输出用
    void Gotoxy(QTextStream& stream, int x, int y) {
        static int indent = 0;
        static int level = 0;
        if (y == 0) {
            level = 0;
            indent = 0;
        }
        if (y != level) {
            int n = y - level;
            for (int i = 0; i < n; i++) {
                stream << endl;
                ++level;
            }
            indent = 0;
        }
        stream.setFieldWidth(x - indent);
        indent = x;
    }

    struct Location {
        int x, y;
    };

    // 垂直输出
    void PrintBSTreeHelp(QTextStream& stream, const HTNode *List, int num, int w) {
        if (List == nullptr)
            return;
        HTNode temp = List[num];
        int level = 0, off = w / 2;
        Location f{}, c{};
        LinkQueue<HTNode> Q;
        LinkQueue<Location> LQ;
        f.x = off;
        f.y = level;
        Q.InQueue(temp);
        LQ.InQueue(f);
        while (!Q.Empty()) {
            Q.OutQueue(temp);
            LQ.OutQueue(f);
            Gotoxy(stream, f.x, f.y);
            stream << temp.usWeight;

            if (f.y != level) {
                off /= 2;
                level++;
            }
            if (temp.mLChild != -1) {
                Q.InQueue(List[temp.mLChild]);
                c.x = f.x - off / 2;
                c.y = f.y + 1;
                LQ.InQueue(c);
            }
            if (temp.mRChild != -1) {
                Q.InQueue(List[temp.mRChild]);
                c.x = f.x + off / 2;
                c.y = f.y + 1;
                LQ.InQueue(c);
            }
        }
        stream << endl;
    }

    // 垂直输出
    void PrintBSTree(QTextStream& stream, int w) { PrintBSTreeHelp(stream, this->mHTree, this->mLength - 1, w); }

    // 导入哈夫曼编码
    bool WriteCode(QString outputName, int num){
    // 打开文件
        QFile file(outputName);
        if(file.open(QIODevice::ReadWrite | QIODevice::Text)){
            QTextStream stream(&file);
            // 将文本数组中的每一行写入文件
            for(int j = 0; j < num; j++){
                stream << this->mHTree[j].data << " : " << this->mHTree[j].code << endl;
            }
            file.close();
            return true;
        }
        return false;
}

    // 判断是否进行初始化
    bool isInit(){ return this->mLength != 0; }

};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void keyPressEvent(QKeyEvent *k);

private slots:
    void newActionSlot();
    void openActionSlot();
    void saveActionSlot();
    void countActionSlot();
    void encodeActionSlot();
    void treeActionSlot();
    void inputActionSlot();
    void decodeActionSlot();

private:
    Ui::MainWindow *ui;
    QString truthName;
    QString codeName;
    QHash<QString, int> *myContext;
    HTree *tree;
};



#endif // MAINWINDOW_H

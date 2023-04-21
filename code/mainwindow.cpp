#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)

{
    ui->setupUi(this);
    this->setWindowIcon(QIcon(":/new/prefix1/Drizzledrop.ico"));
    this->setWindowTitle("myHuffmanEditor - By Drizzledrop");
    this->truthName = QCoreApplication::applicationDirPath() + "/" + "新建文本文档.txt";

    // 信号和槽连接
    connect(ui->newAction, &QAction::triggered, this, &MainWindow::newActionSlot); // 新建文本
    connect(ui->openAction, &QAction::triggered, this, &MainWindow::openActionSlot); // 打开文本
    connect(ui->saveAction, &QAction::triggered, this, &MainWindow::saveActionSlot); // 保存文本
    connect(ui->countAction, &QAction::triggered, this, &MainWindow::countActionSlot); // 词频统计
    connect(ui->encodeAction, &QAction::triggered, this, &MainWindow::encodeActionSlot); // 新建编码
    connect(ui->inputAction, &QAction::triggered, this, &MainWindow::inputActionSlot); // 导入编码
    connect(ui->DecodeAction, &QAction::triggered, this, &MainWindow::decodeActionSlot); // 解码输出
    connect(ui->treeAction_2, &QAction::triggered, this, &MainWindow::treeActionSlot); // 建树输出
}

MainWindow::~MainWindow()
{
    delete ui;
    delete this->tree;
    delete this->myContext;
}


/*
  槽函数实现部分
*/
void MainWindow::newActionSlot(){
    ui->textEdit->clear();
    this->setWindowTitle("新建文本文档.txt - myHuffmanEditor");
    this->truthName = QCoreApplication::applicationDirPath() + "/" + "新建文本文档.txt";
}

void MainWindow::openActionSlot(){
    QString fileName = QFileDialog::getOpenFileName(this, "选择一个文件",
                                 QCoreApplication::applicationFilePath(),
                                 "*.txt");
    if(fileName.isEmpty()){
        QMessageBox::warning(this, "警告", "请选择一个文件！");
    }

    else{
        //qDebug() << fileName;
        QFile file(fileName); // 创建文本对象
        file.open(QIODevice::ReadOnly);
        QByteArray temp = file.readAll();
        ui->textEdit->setText(QString(temp));
        file.close();
        this->setWindowTitle(file.fileName().section(QRegExp("[/.]"),-2, -2) + " - myHuffmanEditor");
    }
    this->truthName = fileName;
}

void MainWindow::saveActionSlot(){
    QString fileName = QFileDialog::getSaveFileName(this, "选择一个文件",
                                                    QCoreApplication::applicationFilePath(), "*.txt");
    if(fileName.isEmpty()) QMessageBox::warning(this, "警告", "请选择一个文件！");
    else{
        QFile file(fileName);
        file.open(QIODevice::WriteOnly);
        QByteArray temp;
        temp.append(ui->textEdit->toPlainText());
        file.write(temp);
        file.close();
        this->setWindowTitle(file.fileName().section(QRegExp("[/.]"),-2, -2) + " - myHuffmanEditor —— 已保存");
    }
    this->truthName = fileName;
}

void MainWindow::countActionSlot(){
    /*
    保存当前文本框内容
    */
    QFile file(this->truthName);
    file.open(QIODevice::WriteOnly);
    QByteArray temp;
    temp.append(ui->textEdit->toPlainText());
    file.write(temp);

    /*
    创建TextEditor对象，进行文本的读、词频统计以及写内容
    */
    TextEditor editor(this->truthName, QCoreApplication::applicationDirPath()+"/" + file.fileName().section(QRegExp("[/.]"),-2, -2)+"Out.txt");
    file.close();
    editor.ReadText();
    editor.WordFrequency();
    editor.WriteText();
    this->myContext = new QHash<QString, int>(editor.getWordFrency());
    /*
    在文本框显示写入内容
    */
    QFile outFile(editor.getOutPutName()); // 创建文本对象
    outFile.open(QIODevice::ReadOnly);
    QByteArray outTemp = outFile.readAll();
    ui->textEdit->setText(QString(outTemp));
    outFile.close();
    this->setWindowTitle(outFile.fileName().section(QRegExp("[/.]"),-2, -2) + " - myHuffmanEditor");
    this->truthName = editor.getOutPutName();
}


void MainWindow::encodeActionSlot(){
    if(this->myContext->empty()) {
        QMessageBox::warning(this, "警告", "未进行词频统计，请先进行词频统计！");
    }
    else{
        /*
        创建哈夫曼树及编码
        */
        this->tree = new HTree(*this->myContext);
        this->tree->createHuffmanTree(this->myContext->size());
        this->tree->huffmanCoding();
        this->tree->WriteCode(QCoreApplication::applicationDirPath()+"/" + this->truthName.section(QRegExp("[/.]"),-2, -2).mid(0, this->truthName.section(QRegExp("[/.]"),-2, -2).indexOf("Huffman"))+"HuffmanCode.txt", this->myContext->size());
        /*
        在文本框显示写入内容
        */
        QFile outFile(QCoreApplication::applicationDirPath()+"/" + this->truthName.section(QRegExp("[/.]"),-2, -2).mid(0, this->truthName.section(QRegExp("[/.]"),-2, -2).indexOf("Huffman"))+"HuffmanCode.txt"); // 创建文本对象
        outFile.open(QIODevice::ReadOnly);
        QByteArray outTemp;
        outTemp += "编码文本来自： " + this->truthName + "\n";
        outTemp += outFile.readAll();
        ui->textEdit->setText(QString(outTemp));
        this->codeName = outFile.fileName();
        this->truthName = outFile.fileName();
        this->setWindowTitle(outFile.fileName().section(QRegExp("[/.]"),-2, -2).mid(0, outFile.fileName().section(QRegExp("[/.]"),-2, -2).indexOf("Huffman")) + " - myHuffmanEditor");
        outFile.close();
    }
}

void MainWindow::inputActionSlot(){
    QString fileName = QFileDialog::getOpenFileName(this, "选择一个文件",
                                                    QCoreApplication::applicationFilePath(), "*.txt");
    if(fileName.isEmpty()) {
        QMessageBox::warning(this, "警告", "请选择一个文件！");
    }
    else{
        // 词频统计
        QFile file(fileName);
        file.open(QIODevice::ReadOnly);
        TextEditor editor(fileName, QCoreApplication::applicationDirPath()+"/" + file.fileName().section(QRegExp("[/.]"),-2, -2)+"Out.txt");
        file.close();
        editor.ReadText();
        editor.WordFrequency();
        editor.WriteText();
        this->truthName = editor.getOutPutName();
        this->myContext = new QHash<QString, int>(editor.getWordFrency());

        // 创建哈夫曼树及编码
        this->tree = new HTree(*this->myContext);
        this->tree->createHuffmanTree(this->myContext->size());
        this->tree->huffmanCoding();
        this->tree->WriteCode(QCoreApplication::applicationDirPath()+"/" + this->truthName.section(QRegExp("[/.]"),-2, -2).mid(0, this->truthName.section(QRegExp("[/.]"),-2, -2).indexOf("Huffman"))+"HuffmanCode.txt", this->myContext->size());

        /*
        在文本框显示写入内容
        */
        QFile outFile(QCoreApplication::applicationDirPath()+"/" + this->truthName.section(QRegExp("[/.]"),-2, -2).mid(0, this->truthName.section(QRegExp("[/.]"),-2, -2).indexOf("Huffman")) +"HuffmanCode.txt"); // 创建文本对象
        outFile.open(QIODevice::ReadOnly);
        QByteArray outTemp;
        outTemp += "编码文本来自： " + this->truthName + "\n";
        outTemp += outFile.readAll();
        ui->textEdit->setText(QString(outTemp));
        outFile.close();
        this->setWindowTitle(outFile.fileName().section(QRegExp("[/.]"),-2, -2).mid(0, outFile.fileName().section(QRegExp("[/.]"),-2, -2).indexOf("Huffman")) + "HuffmanCode" + " - myHuffmanEditor");
        this->codeName = outFile.fileName();
        this->truthName = outFile.fileName();
    }
}

void MainWindow::decodeActionSlot(){
    if(this->codeName.isEmpty()) {
        QMessageBox::warning(this, "警告", "未进行编码，无编码可用！");
    }
    else{
        QByteArray temp;
        temp.append(ui->textEdit->toPlainText());
        QString str(temp);
        str = this->tree->huffmanDecoding(str);
        temp = "解码编码来自： " + this->codeName.toUtf8() + "\n";
        temp += str.toUtf8();
        ui->textEdit->setPlainText(temp);
        QString fileName = QCoreApplication::applicationDirPath()+"/" + this->truthName.section(QRegExp("[/.]"),-2, -2).mid(0, this->truthName.section(QRegExp("[/.]"),-2, -2).indexOf("Huffman"))+"HuffmanDecode.txt";
        QFile file(fileName);
        file.open(QIODevice::WriteOnly);
        temp.clear();
        temp.append(ui->textEdit->toPlainText());
        file.write(temp);
        this->truthName = file.fileName();
        this->setWindowTitle(file.fileName().section(QRegExp("[/.]"),-2, -2).mid(0, file.fileName().section(QRegExp("[/.]"),-2, -2).indexOf("Huffman")) + "HuffmanDecode"+ " - myHuffmanEditor");
        file.close();
    }
}

void MainWindow::treeActionSlot(){
    if(this->codeName.isEmpty()) {
        QMessageBox::warning(this, "警告", "未进行编码，无哈夫曼树可用！");
    }
    else{
        QString fileName = QCoreApplication::applicationDirPath()+"/" + this->truthName.section(QRegExp("[/.]"),-2, -2).mid(0, this->truthName.section(QRegExp("[/.]"),-2, -2).indexOf("Huffman"))+"HuffmanTree.txt";
        QFile file(fileName);
        if(file.open(QIODevice::ReadWrite | QIODevice::Text)){
            QTextStream stream(&file);
            this->tree->PrintBSTree(stream, this->myContext->size()*9);
        }
        file.close();
        QFile outFile(fileName);
        outFile.open(QIODevice::ReadOnly);
        QByteArray outTemp;
        outTemp += "哈夫曼树来自： " + this->truthName + "\n";
        outTemp += outFile.readAll();
        ui->textEdit->setText(QString(outTemp));
        this->setWindowTitle(outFile.fileName().section(QRegExp("[/.]"),-2, -2).mid(0, outFile.fileName().section(QRegExp("[/.]"),-2, -2).indexOf("Huffman")) +"HuffmanTree" + " - myHuffmanEditor");
        outFile.close();
    }

}

/*
  键盘事件捕捉实现
*/

void MainWindow::keyPressEvent(QKeyEvent *k){
    if(k->modifiers() == Qt::ControlModifier && k->key() == Qt::Key_N){
        newActionSlot();
    }
    if(k->modifiers() == Qt::ControlModifier && k->key() == Qt::Key_O){
        openActionSlot();
    }
    if(k->modifiers() == Qt::ControlModifier && k->key() == Qt::Key_S){
        saveActionSlot();
    }
    if(k->modifiers() == Qt::ControlModifier && k->key() == Qt::Key_C){
        countActionSlot();
    }
    if(k->modifiers() == Qt::ControlModifier && k->key() == Qt::Key_E){
        encodeActionSlot();
    }
    if(k->modifiers() == Qt::ControlModifier && k->key() == Qt::Key_I){
        inputActionSlot();
    }
    if(k->modifiers() == Qt::ControlModifier && k->key() == Qt::Key_D){
        decodeActionSlot();
    }
    if(k->modifiers() == Qt::ControlModifier && k->key() == Qt::Key_T){
        treeActionSlot();
    }
}

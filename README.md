# HuffmanTreeEditor

记录第一次用Qt实现的文本编辑器，完成了哈夫曼的编译码功能


```c++
/*
 * author:Drizzledrop
 * time:2022.12.28
 * */
```


## 项目注解

`code` 文件存放源代码，其中部分数据结构头文件为手写，后测试发现使用Qt库提供的数据结构效率上更优，便采用Qt库提供的，但并未删除原头文件。

`HuffmanTextEditor` 文件存放代码可执行程序，其中 `release` 文件存放了测试文件以及压缩后的完整可执行程序。



## 流程设计

### 流程图

下面是词频统计的过程：

![image](https://user-images.githubusercontent.com/95005094/233594743-86ad0928-1e7d-4c20-be29-7b5da08fbcea.png)







下面是哈夫曼编码的过程：
![image](https://user-images.githubusercontent.com/95005094/233594792-7a0c428d-8960-4360-be33-d94e08ce8b2f.png)








下面是哈夫曼译码的过程：
![image](https://user-images.githubusercontent.com/95005094/233594828-346ef16d-8096-4714-bb29-bf17529ec1e7.png)




### UML类图

词频统计类：
![image](https://user-images.githubusercontent.com/95005094/233598716-38c24cbb-2360-4430-9f84-6e989a0d8931.png)


> TextEditor类包含五个属性，分别是用于储存输入文本名与输出文本名的fileName与outputName，逐行读入时用到的temp值line，存储处理后的每行文本的text以及最终存放字词及词频的map。
> 六个方法分为私有的WordStrip、TextPush和strToLower三个私有方法，对应功能为去除标点、行文本按空格单词化读入context以及小写化字符；ReadText、WordFrequency和WriteText三个公有方法用于读入文本、词频统计与词频信息写入。其中读入文本时调用LowerString、WordStrip、TextPush三个私有方法进行文本信息预处理，词频统计时使用map的哈希映射进行词分析，信息写入即依据map内容进行文本信息写入。





哈夫曼结点类：
![image](https://user-images.githubusercontent.com/95005094/233595607-1345fa5a-5bd7-4ffd-abce-304b02c6a1e8.png)

> HTNode类包含七个公有属性，分别是用于记录左右子结点的mRChild、mLChild，记录父结点的mParent，记录权重的mWeight和usWeight，记录结点数据的data与记录数据哈夫曼编码的code。其中必要数据为mRChild、mLChild、usWeight、data与code，但为简化后续用两小权重结点生成父结点的过程，添加了mParent与mWeight，由于两数据均为int，即占用空间并不大，但在查找两小权重结点时大幅降低了查找与生成父结点的效率。





哈夫曼树类：
![image](https://user-images.githubusercontent.com/95005094/233595273-40fef351-ca57-45e8-a72e-adcd7cbe6e22.png)

> HTree类包含两个属性，分别是用于储存结点的mHTree(这里其实是用作数组)，记录结点长度(数组长度)的mLength。
> 十一个方法，分为四个私有的findTwoMinNode、searchDataOfCode、Gotoxy和PrintBSTreeHelp，分别用作寻找两个最小权重结点、寻找编码对应字符、垂直输出辅助方法1和垂直输出辅助方法2；七个公有方法createHuffmanTree、huffmanCoding、huffmanDecoding、showHuffmanCode、PrintBSTree、WriteCode、isInit分别用于创建哈夫曼树、哈夫曼编码、哈夫曼解码、显示哈夫曼树(测试代码用)、垂直打印哈夫曼树、向文件中导入哈夫曼编码、判断哈夫曼树是否已编码。其中创建哈夫曼树过程调用私有方法中的findTwoMinNode进行建树；哈夫曼解码调用私有方法中的searchDataOfCode查找二进制编码对应的字符；垂直打印哈夫曼树调用私有方法中的Gotoxy、PrintBSTreeHelp进行垂直输出；isInit主要用于后续UI MainWindow类中判断是否可以进行哈夫曼编码、译码和垂直输出。





UI类：
![image](https://user-images.githubusercontent.com/95005094/233595556-e3799414-b251-4eaf-aaa3-2152755babd3.png)

> MainWindow类包含五个私有属性，分别是作为主UI的ui，记录当前窗口名的truthName，记录当前译码使用的编码文件名的codeName，用于词频统计的哈希映射myContext，当前UI记录的哈夫曼树tree。
> 九个方法，其中以‘%’为前标的八个方法为槽函数，用于前后端交互。newActionSlot用于新建文本文档；openActionSlot用于打开文本；saveActionSlot用于保存当前窗口文本；countActionSlot用于对当前窗口的词频统计；encodeActionSlot用于在已知词频的基础上新建哈夫曼编码；inputActionSlot用于在未进行词频统计基础上创建哈夫曼编码；decodeActionSlot用于已编码后读入窗口信息进行解码；treeActionSlot用于垂直输出哈夫曼树；keyPressEvent用于实时记录键盘事件并进行相应交互(即用于快捷键的实现)。


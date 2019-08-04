# bomblab学习笔记

## 2019/08/04

本次bomblab要我们完成拆弹的工作，对给我们的包进行解压发现只有三个文件，README里面也没有什么有用的信息。。看了看writeup，也说让我们进行拆弹，但是具体怎么拆，也没有给出例子。



![image-20190804105316271](https://github.com/shendezhuti/README_ADD_PIC/blob/master/CMU15213/bomlab/image-20190804105316271.png)



搜索了一下网上的资料，说我们可以在我们的linux服务器试着输入 <font color='red'>`./bomb`</font> 运行程序 这里要注意./代表当前路径，如果我们直接输入 <font color='red'>`./bomb`</font> 会自动定位到PATH环境，而不是当前路径。输入 <font color='red'>`./bomb`</font> 后运行了bomb程序



![image-20190804110159471](https://github.com/shendezhuti/README_ADD_PIC/blob/master/CMU15213/bomlab/image-20190804110159471.png)



我们这里随便输入一个s 程序就“爆炸了”！**这里的感觉是应该要输入一些东西，如果不正确就错误**，老师说这个实验需要用到gdb，但是我此时还没学过（要去找一下教程自学一下）

> https://www.jianshu.com/p/08de5cef2de9 找到一个基本的入门教程

看完之后我们输入 <font color=red> `gdb bomb`</font>进入

进入后输入<font color=red> `list`</font>命令，查看bomb.c的源码，将源码定位到37行，然后<font color="#DC143C"> `run`</font>运行程序，再使用<font color=red> `disas`</font>，看看main函数的反编译，密密麻麻的有点晕。稍微看下汇编的代码，感觉有点熟悉，但是忘记了好多。

### 一.phase_1

下面就解开phase_1的谜团  输入 <font color=red> `disas phase_1`</font>



![image-20190804154232511](https://github.com/shendezhuti/README_ADD_PIC/blob/master/CMU15213/bomlab/image-20190804154232511.png)



看一下这个汇编代码，可以隐约发现此时如果输入的参数不是指定的字符串 那么就会调用explode_bomb,我们用   <font color=red> `x/s 0x402400`</font> 指令查看0x402400地址存放的内容！果然发现是一串字符串！如下



![image-20190804154629814](https://github.com/shendezhuti/README_ADD_PIC/blob/master/CMU15213/bomlab/image-20190804154629814.png)



因此答案就是 <font color=red> `Border relations with Canada have never been better.`</font>



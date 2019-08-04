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

------
### 一.phase_1

下面就解开phase_1的谜团  输入 <font color=red> `disas phase_1`</font>



![image-20190804154232511](https://github.com/shendezhuti/README_ADD_PIC/blob/master/CMU15213/bomlab/image-20190804154232511.png)



看一下这个汇编代码，可以隐约发现此时如果输入的参数不是指定的字符串 那么就会调用explode_bomb,我们用   <font color=red> `x/s 0x402400`</font> 指令查看0x402400地址存放的内容！果然发现是一串字符串！如下



![image-20190804154629814](https://github.com/shendezhuti/README_ADD_PIC/blob/master/CMU15213/bomlab/image-20190804154629814.png)



因此答案就是 <font color=red> `Border relations with Canada have never been better.`</font>

------
### 二.phase_2
我们先<font color=red> `disas phase_2`</font>（下面的节选了前五行）
![image-20190804225553609](https://github.com/shendezhuti/README_ADD_PIC/blob/master/CMU15213/bomlab/image-20190804225553609.png)

我们看到<+9>这一行调用了<font color=red> `read_six_number`</font>这个指令，猜测是要从命令行输入6个数字，我们接下来调用<font color=red> `disas read_six_number`</font>

![image-20190804225950574](https://github.com/shendezhuti/README_ADD_PIC/blob/master/CMU15213/bomlab/image-20190804225950574.png)

从上面的<+46>行看到，调用了sscanf读入，这里网上有个博🐷说他敏锐的发现了move $0x4025c3,%esi这个指令，查了sscanf的声明:<font color=red> `int sscanf( const char *buffer, const char *format, [ argument ] ... );`</font>第二个参数是输入格式？？对照了上面的地址 然后用 
<font color=red> `x/s 0x4025c3`</font>验证 发现就是这样

![image-20190804231045780](https://github.com/shendezhuti/README_ADD_PIC/blob/master/CMU15213/bomlab/image-20190804231045780.png)

于是我们输入<font color=red> `disas phase_2`</font> 回到函数phase_2

![image-20190804231233979](https://github.com/shendezhuti/README_ADD_PIC/blob/master/CMU15213/bomlab/image-20190804231233979.png)

我们先从**( 1 )**看起，这句话给了我们一个提示，我们第一个参数要是**< 1 >**，然后成功的话跳转到**[ lea    0x4(%rsp),%rbx ]**第**( 3 )**段中，这里的**[ 0x18(%rsp),%rbp ]**，这里的**rbp**一看就知道是个终结位置，因为６个整形大小就是**0x18 = 24B**，然后我们跳到第**( 2 )**段去，着段码也很好理解，不就是**获取前一个位置的数存入寄存％eax**，然后**%eax \*=2**，这不就是就告诉我们这个阶段要的答案是**以1开头的，后一个数比前一个数大2倍的序列吗？**，显而易见。

因此答案是<font color=red> `1 2 4 8 16 32`</font> 

------


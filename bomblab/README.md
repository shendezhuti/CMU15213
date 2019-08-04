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

于是我们输入<font color=red> `disas phase_2`</font> 回到函数phase_2 实际上函数使用了栈(rsp)存放返回的结果。

![image-20190804231233979](https://github.com/shendezhuti/README_ADD_PIC/blob/master/CMU15213/bomlab/image-20190804231233979.png)

我们先从**( 1 )**看起，这句话给了我们一个提示，我们第一个参数要是**< 1 >**，然后成功的话跳转到**[ lea    0x4(%rsp),%rbx ]**第**( 3 )**段中，这里的**[ 0x18(%rsp),%rbp ]**，这里的**rbp**一看就知道是个终结位置，因为６个整形大小就是**0x18 = 24B**，然后我们跳到第**( 2 )**段去，着段码也很好理解，不就是**获取前一个位置的数存入寄存％eax**，然后**%eax \*=2**，这不就是就告诉我们这个阶段要的答案是**以1开头的，后一个数比前一个数大2倍的序列吗？**，显而易见。

因此答案是<font color=red> `1 2 4 8 16 32`</font> 

------

## 2019/08/05

###  三.phase_3

`disas phase_3`不必多说，继续看反汇编码。

![image-20190805014806518](https://github.com/shendezhuti/README_ADD_PIC/blob/master/CMU15213/bomlab/image-20190805014912674.png)

<+4>以及<+9>两行将`rsp+0xc`与`rsp+0x8`的值分别给`rcx`与`rdx`，下一行将一个地址值复制给了`esi`，接着将`eax`置为0，下一步调用了库函数`sscanf`，我们想到`sscanf`中的参数中需要一个格式化字符串，那么`esi`中的这个地址值就很有可能存放了这个字符串，我们同样使用`gdb`在运行时查看这个字符串：

![img](https://github.com/shendezhuti/README_ADD_PIC/blob/master/CMU15213/bomlab/image-20190805015706461.png)

可以看到这就是格式化字符串，读入的是两个整型值。这两个值存放在哪里呢？我们想到之前把`rsp+0xc`与`rsp+0x8`的值分别给`rcx`与`rdx`，这是两个地址值，我们可以用之前的方法验证栈中存放的确实是我们读入的这两个值。

下面第<+29>行将`eax`与1进行比较，`eax`一般用于存放函数返回值，而`sscanf` 的返回值是成功读入的数值个数，也就是说这几行将成功读入的个数与1进行比较，如果大于1则跳过引爆的代码。

下面第<+39>行将`rsp+0x8`中存放的值与`0x7`进行比较，如果大于`0x7`则跳到`<+106>`的位置，引爆炸弹。

下面的两行比较关键：第<+46>行将`rsp+0x8`中存放的值复制入`eax`，第<+50>行进行一个跳转，跳转到的地址为`0x402470(,%rax,8)`，这就是一个典型的`switch`语句的实现：直接跳转到索引*位移的指令位置。

![image-20190805015407101](https://github.com/shendezhuti/README_ADD_PIC/blob/master/CMU15213/bomlab/image-20190805015407101.png)

上面的代码已经加了注释，假设读入的第一个数为x，看到所有分支最后都跳转到了`phase_3+123`这行判断中，将`eax`中的值与`rsp+0xc`也就是我们读入的第二个数进行判断，如果相等的话跳过引爆代码。

而每个分支都将一个数复制到了`eax`中，也就是说我们只要根据不同的第一个参数的值读入对应的第二个参数就可以了，所以我们可以随意选择一个x值，这里我选择`x=1`，对应的第二个参数为`0x137`换成十进制是311，所以第3阶段的（一个）答案为：`1 311`





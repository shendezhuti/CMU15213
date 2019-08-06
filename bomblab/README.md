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

-------

### 四.phase_4

到这一步的拆弹真的看的头大了，感觉自己的基础知识打的不是特别稳。

```assembly
  0x000000000040100c <+0>:	sub    $0x18,%rsp
   0x0000000000401010 <+4>:	lea    0xc(%rsp),%rcx
   0x0000000000401015 <+9>:	lea    0x8(%rsp),%rdx
   0x000000000040101a <+14>:	mov    $0x4025cf,%esi
   0x000000000040101f <+19>:	mov    $0x0,%eax
   0x0000000000401024 <+24>:	callq  0x400bf0 <__isoc99_sscanf@plt>
   0x0000000000401029 <+29>:	cmp    $0x2,%eax
   0x000000000040102c <+32>:	jne    0x401035 <phase_4+41>
   0x000000000040102e <+34>:	cmpl   $0xe,0x8(%rsp)
   0x0000000000401033 <+39>:	jbe    0x40103a <phase_4+46>
   0x0000000000401035 <+41>:	callq  0x40143a <explode_bomb>
   0x000000000040103a <+46>:	mov    $0xe,%edx
   0x000000000040103f <+51>:	mov    $0x0,%esi
   0x0000000000401044 <+56>:	mov    0x8(%rsp),%edi
   0x0000000000401048 <+60>:	callq  0x400fce <func4>
   0x000000000040104d <+65>:	test   %eax,%eax
   0x000000000040104f <+67>:	jne    0x401058 <phase_4+76>
   0x0000000000401051 <+69>:	cmpl   $0x0,0xc(%rsp)
   0x0000000000401056 <+74>:	je     0x40105d <phase_4+81>
   0x0000000000401058 <+76>:	callq  0x40143a <explode_bomb>
   0x000000000040105d <+81>:	add    $0x18,%rsp
   0x0000000000401061 <+85>:	retq 
```

和前面一样，我们有<font color=red> `sscanf`</font>这个指令，利用<font color=red> `x/s 0x4025cf`</font>指令查看格式字符串，我们会发现，也是读入两个参数存放在栈中，然后传到<font color=red> `rcx`</font>和<font color=red> `rdx`</font>中。下面的汇编代码要求输入的参数是2个数字，并且第一个参数要小于<font color=red> `0xe`</font> 

接下来把0xe赋给`edx`、0x0赋给`esi`，`rsp+0x8`的值赋给`edi`。接下来调用了`func4`函数。

在去查看`func4`函数的代码之前，我们先查看函数返回后的代码，了解我们需要的结果。第17、18行测试了`eax`的值如果不为0，就跳转到引爆代码。

所以我们的目标是返回时`eax`的值为0.下面进入`func4`函数。

```assembly
0x0000000000400fce <+0>:	sub    $0x8,%rsp   	;;分配栈帧
   0x0000000000400fd2 <+4>:	mov    %edx,%eax   ;;C  eax
   0x0000000000400fd4 <+6>:	sub    %esi,%eax		;;C-B 更新eax
   0x0000000000400fd6 <+8>:	mov    %eax,%ecx		;;C-B  ecx
   0x0000000000400fd8 <+10>:	shr    $0x1f,%ecx		;;右移31位，ecx长为32位(也就是之前的最高位变成了最低位，其余31位补充0)，可以认为ecx=0
   0x0000000000400fdb <+13>:	add    %ecx,%eax	;;C-B eax
   0x0000000000400fdd <+15>:	sar    %eax			;;这里是一个缩写 sar $1,%eax(对应的机器码为D1F8) eax=(C-B)/2
   0x0000000000400fdf <+17>:	lea    (%rax,%rsi,1),%ecx    ;;（C+B)/2
   0x0000000000400fe2 <+20>:	cmp    %edi,%ecx	;;ecx与A进行比较
   0x0000000000400fe4 <+22>:	jle    0x400ff2 <func4+36>	;;ecx小于等于A则跳转
   0x0000000000400fe6 <+24>:	lea    -0x1(%rcx),%edx		;;C=(C+B)/2-1
   0x0000000000400fe9 <+27>:	callq  0x400fce <func4>		;;递归调用
   0x0000000000400fee <+32>:	add    %eax,%eax		;;递归返回值加倍
   0x0000000000400ff0 <+34>:	jmp    0x401007 <func4+57>	;;跳转到func函数的出口
   0x0000000000400ff2 <+36>:	mov    $0x0,%eax		;;eax=0
   0x0000000000400ff7 <+41>:	cmp    %edi,%ecx		;;ecx与A进行比较
   0x0000000000400ff9 <+43>:	jge    0x401007 <func4+57>	;;ecx大于等于A则跳转
   0x0000000000400ffb <+45>:	lea    0x1(%rcx),%esi		;; B=ecx+1
   0x0000000000400ffe <+48>:	callq  0x400fce <func4>		;;递归调用
   0x0000000000401003 <+53>:	lea    0x1(%rax,%rax,1),%eax	;;递归返回值加倍并且再加1
   0x0000000000401007 <+57>:	add    $0x8,%rsp	;;释放栈帧
   0x000000000040100b <+61>:	retq   ;;函数返回

```

这段代码之中我们调用了`func4`，这是一个递归的过程，像之间那样直接分析比较困难，这里我们就将这个代码逆向为C语言再来分析，下面是逆向出的C语言代码：

```c
int func4(int target, int step, int limit) {
  /* edi = target; esi = step; edx = limit */
  int temp = (limit - step) * 0.5;
  int mid = temp + step;
  if (mid > target) {
    limit = mid - 1;
    int ret1 = func4(target, step, limit);
    return 2 * ret1;
  } else {
    if (mid >= target) {
      return 0;
    } else {
      step = mid + 1;
      int ret2 = func4(target, step, limit);
      return (2 * ret2 + 1);
    }
  }
}

```

这里的`a1``a2`初始值分别为之前的`0xe`与`0x0`。我们可以直接写个测试程序来跑出能返回0的输入值：

```C
int main(void){
    for(int i = 0; i <= 0xe; i++){
        if(fun(0xe,0,i) == 0){
            printf("%d\n",i) ;
            return 0;
        }
    }
    return 0; 
}
```

得出允许的值有`0 1 3 7`.

回到`phase_4`的代码：

```assembly
 0x0000000000401051 <+69>:	cmpl   $0x0,0xc(%rsp)
   0x0000000000401056 <+74>:	je     0x40105d <phase_4+81>
   0x0000000000401058 <+76>:	callq  0x40143a <explode_bomb>
   0x000000000040105d <+81>:	add    $0x18,%rsp

```

第1、2行将输入的第二个参数与0进行比较，如果不为0就引爆炸弹。所以输入的第二个参数必为0。

综上我们得出（一个）答案为：`0 0 `

------

## 2019/08/06

### 五.phase_5

在尝试phase_5之前还回去看了CMU15213的视频



```assembly
 0x0000000000401062 <+0>:	push   %rbx
   0x0000000000401063 <+1>:	sub    $0x20,%rsp
   0x0000000000401067 <+5>:	mov    %rdi,%rbx
   0x000000000040106a <+8>:	mov    %fs:0x28,%rax
   0x0000000000401073 <+17>:	mov    %rax,0x18(%rsp)
   0x0000000000401078 <+22>:	xor    %eax,%eax
   0x000000000040107a <+24>:	callq  0x40131b <string_length>
   0x000000000040107f <+29>:	cmp    $0x6,%eax		;;要求输入的字符串长度为6
   0x0000000000401082 <+32>:	je     0x4010d2 <phase_5+112>
   0x0000000000401084 <+34>:	callq  0x40143a <explode_bomb>
   0x0000000000401089 <+39>:	jmp    0x4010d2 <phase_5+112>
   0x000000000040108b <+41>:	movzbl (%rbx,%rax,1),%ecx
   0x000000000040108f <+45>:	mov    %cl,(%rsp)
   0x0000000000401092 <+48>:	mov    (%rsp),%rdx
   0x0000000000401096 <+52>:	and    $0xf,%edx		;;取edx的后四位
   0x0000000000401099 <+55>:	movzbl 0x4024b0(%rdx),%edx		;;将edx的后四位作为0x4024b0的索引值
   0x00000000004010a0 <+62>:	mov    %dl,0x10(%rsp,%rax,1)		;;依次拷贝字符数组到0x10((%rsp,%rax,1))
   0x00000000004010a4 <+66>:	add    $0x1,%rax		#循环计数+1
   0x00000000004010a8 <+70>:	cmp    $0x6,%rax		 #循环计数和6比较，即循环6次
   0x00000000004010ac <+74>:	jne    0x40108b <phase_5+41>
   0x00000000004010ae <+76>:	movb   $0x0,0x16(%rsp)		;;字符串末尾添加"\0"
   0x00000000004010b3 <+81>:	mov    $0x40245e,%esi		;;字符串常量
   0x00000000004010b8 <+86>:	lea    0x10(%rsp),%rdi
   0x00000000004010bd <+91>:	callq  0x401338 <strings_not_equal>		;;和字符串常量进行比较
   0x00000000004010c2 <+96>:	test   %eax,%eax
   0x00000000004010c4 <+98>:	je     0x4010d9 <phase_5+119>
   0x00000000004010c6 <+100>:	callq  0x40143a <explode_bomb>
   0x00000000004010cb <+105>:	nopl   0x0(%rax,%rax,1)
   0x00000000004010d0 <+110>:	jmp    0x4010d9 <phase_5+119>
   0x00000000004010d2 <+112>:	mov    $0x0,%eax
   0x00000000004010d7 <+117>:	jmp    0x40108b <phase_5+41>
   0x00000000004010d9 <+119>:	mov    0x18(%rsp),%rax
   0x00000000004010de <+124>:	xor    %fs:0x28,%rax
   0x00000000004010e7 <+133>:	je     0x4010ee <phase_5+140>
   0x00000000004010e9 <+135>:	callq  0x400b30 <__stack_chk_fail@plt>
   0x00000000004010ee <+140>:	add    $0x20,%rsp
   0x00000000004010f2 <+144>:	pop    %rbx
   0x00000000004010f3 <+145>:	retq   

```

不得不说 汇编代码长起来真的是晦涩枯燥啊（下面的解答摘自网上）

后面有一个和字符串常量比较的地方，看看这个字符串是什么

运行 <font color=red> `x/s 0x40245e `</font> 后显示<font color=red> `flyers`</font>

```assembly
(gdb) x/s 0x40245e
0x40245e:	"flyers"
```

这段汇编还有一个字符串常量0x4024b0:  "maduiersnfotvbylSo you think you can stop the bomb with ctrl-c, do you?"

由于汇编代码比较长，我就直接说明这一段到底在干什么：
1.要求输入6个字符，然后依次循环这个输入的字符数组
2.每一轮循环取一个字符，然后取这个字符的后四位作为索引，在第二个字符常量处取一个字符
依次存放到0x10(%rsp)处
3.最后将新0x10(%rsp)处的字符串和"flyers"比较，相同则通过，否则爆炸
所以我们需要根据结果倒推，比如flyers中的f字符是由我们输入的第一个字符的后四位作为索引在
"maduiersnfotvbylSo you think you can stop the bomb with ctrl-c, do you?"取得，但是我们知道四位二进制最多索引16
个位置，所以这一长串的字符只有前16个可以来取我们需要的字符。所以f的索引为9,即二进制1001，只需要查询ascii表后四位为1001的字符均可，我取的Ｙ。以此类推得到6个字符的一个组合：YONEFw




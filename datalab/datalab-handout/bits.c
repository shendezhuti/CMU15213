/* 
 * CS:APP Data Lab 
 * 
 * <Please put your name and userid here>
 * 
 * bits.c - Source file with your solutions to the Lab.
 *          This is the file you will hand in to your instructor.
 *
 * WARNING: Do not include the <stdio.h> header; it confuses the dlc
 * compiler. You can still use printf for debugging without including
 * <stdio.h>, although you might get a compiler warning. In general,
 * it's not good practice to ignore compiler warnings, but in this
 * case it's OK.  
 */

#if 0
/*
 * Instructions to Students:
 *
 * STEP 1: Read the following instructions carefully.
 */

You will provide your solution to the Data Lab by
editing the collection of functions in this source file.

INTEGER CODING RULES:
 
  Replace the "return" statement in each function with one
  or more lines of C code that implements the function. Your code 
  must conform to the following style:
 
  int Funct(arg1, arg2, ...) {
      /* brief description of how your implementation works */
      int var1 = Expr1;
      ...
      int varM = ExprM;

      varJ = ExprJ;
      ...
      varN = ExprN;
      return ExprR;
  }

  Each "Expr" is an expression using ONLY the following:
  1. Integer constants 0 through 255 (0xFF), inclusive. You are
      not allowed to use big constants such as 0xffffffff.
  2. Function arguments and local variables (no global variables).
  3. Unary integer operations ! ~
  4. Binary integer operations & ^ | + << >>
    
  Some of the problems restrict the set of allowed operators even further.
  Each "Expr" may consist of multiple operators. You are not restricted to
  one operator per line.

  You are expressly forbidden to:
  1. Use any control constructs such as if, do, while, for, switch, etc.
  2. Define or use any macros.
  3. Define any additional functions in this file.
  4. Call any functions.
  5. Use any other operations, such as &&, ||, -, or ?:
  6. Use any form of casting.
  7. Use any data type other than int.  This implies that you
     cannot use arrays, structs, or unions.

 
  You may assume that your machine:
  1. Uses 2s complement, 32-bit representations of integers.
  2. Performs right shifts arithmetically.
  3. Has unpredictable behavior when shifting if the shift amount
     is less than 0 or greater than 31.


EXAMPLES OF ACCEPTABLE CODING STYLE:
  /*
   * pow2plus1 - returns 2^x + 1, where 0 <= x <= 31
   */
  int pow2plus1(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     return (1 << x) + 1;
  }

  /*
   * pow2plus4 - returns 2^x + 4, where 0 <= x <= 31
   */
  int pow2plus4(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     int result = (1 << x);
     result += 4;
     return result;
  }

FLOATING POINT CODING RULES

For the problems that require you to implement floating-point operations,
the coding rules are less strict.  You are allowed to use looping and
conditional control.  You are allowed to use both ints and unsigneds.
You can use arbitrary integer and unsigned constants. You can use any arithmetic,
logical, or comparison operations on int or unsigned data.

You are expressly forbidden to:
  1. Define or use any macros.
  2. Define any additional functions in this file.
  3. Call any functions.
  4. Use any form of casting.
  5. Use any data type other than int or unsigned.  This means that you
     cannot use arrays, structs, or unions.
  6. Use any floating point data types, operations, or constants.


NOTES:
  1. Use the dlc (data lab checker) compiler (described in the handout) to 
     check the legality of your solutions.
  2. Each function has a maximum number of operations (integer, logical,
     or comparison) that you are allowed to use for your implementation
     of the function.  The max operator count is checked by dlc.
     Note that assignment ('=') is not counted; you may use as many of
     these as you want without penalty.
  3. Use the btest test harness to check your functions for correctness.
  4. Use the BDD checker to formally verify your functions
  5. The maximum number of ops for each function is given in the
     header comment for each function. If there are any inconsistencies 
     between the maximum ops in the writeup and in this file, consider
     this file the authoritative source.

/*
 * STEP 2: Modify the following functions according the coding rules.
 * 
 *   IMPORTANT. TO AVOID GRADING SURPRISES:
 *   1. Use the dlc compiler to check that your solutions conform
 *      to the coding rules.
 *   2. Use the BDD checker to formally verify that your solutions produce 
 *      the correct answers.
 */


#endif
//1
/* 
 * bitXor - x^y using only ~ and & 
 *   Example: bitXor(4, 5) = 1
 *   Legal ops: ~ &
 *   Max ops: 14
 *   Rating: 1
 */
int bitXor(int x, int y) {
  /*给定x,y 用~取反和&按位与操作实现^异或操作  
  思路：网上说利用反演对偶规则，这个我真的记不太得了 x^y=~(~x&~y)&~(x&y) 这样是测试是对的
  但是我利用规则推导指出的结果是 ~(~x&y)& ~(x&~y) 但是这样测试好像是错的有点迷茫ing 哦好像自己弄错了应该是下面
          x^y=~((~x&y)&(x&~y))  测试还是不对哦 迷茫inging 
          得到了 直接在异或表达式的另一边加上双非 然后利用德摩根定律
    */
  return ~(~x&~y)&~(x&y);

     /*测试通过 */

}
/* 
 * tmin - return minimum two's complement integer 
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 4
 *   Rating: 1
 */
int tmin(void) {
  /*  将2^0逻辑左移31位 这样最高位就是1 其他位都是0，是最小的补码
  */
  return 1<<31;
     /*测试通过 */

}
//2
/*
 * isTmax - returns 1 if x is the maximum, two's complement number,
 *     and 0 otherwise 
 *   Legal ops: ! ~ & ^ | +
 *   Max ops: 10
 *   Rating: 1
 */
int isTmax(int x) {
  /* 第一个想法：max的补码应该是01111111..1  那么用32个1去做&操作 如果返回的是...emmm 好像不行
     那我换一个操作符试试看，让x他跟0X7FFFFFFF异或，只要结果是0，那么说明这个数就是Tmax！那么我们再用!做逻辑取反即可 （0的逻辑取反数值为1） 
  */
     
     return !(x ^ 0X7FFFFFFF);
     /*测试通过 */
}

/* 
 * allOddBits - return 1 if all odd-numbered bits in word set to 1
 *   where bits are numbered from 0 (least significant) to 31 (most significant)
 *   Examples allOddBits(0xFFFFFFFD) = 0, allOddBits(0xAAAAAAAA) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 2
 */
int allOddBits(int x) {
  /*我的第一个想法是啥呢。。我去 没想出来，既然都是奇数位判断，说明偶数位其实不重要！
   因此我们可以先让x与0XAAAAAAAA做一个与，消除偶数位，然后再和0XAAAAAAAA做异或，如果结果为0，说明是奇数位都是1！最后别忘了用！进行逻辑取反。  
  */
   int y=x & 0xAAAAAAAA;
  return !(y^0xAAAAAAAA);

  /*测试通过了*/
}
/* 
 * negate - return -x 
 *   Example: negate(1) = -1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 5
 *   Rating: 2
 */
int negate(int x) {
  /* 这个题我觉得还是比较简单的，有一个公式就是 -x=~x+1 这个取相反数上课也提到过*/
  return ~x+1;

  /*测试通过！*/
}
//3
/* 
 * isAsciiDigit - return 1 if 0x30 <= x <= 0x39 (ASCII codes for characters '0' to '9')
 *   Example: isAsciiDigit(0x35) = 1.
 *            isAsciiDigit(0x3a) = 0.
 *            isAsciiDigit(0x05) = 0.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 15
 *   Rating: 3
 */
int isAsciiDigit(int x) {
  /* 第一个思路的想法是直接利用x Xor Ox30 至 0x39之间的所有10个数字 如果有一个为零，说明x是其中的一员 测试结果好像是错的，emmm
   sigh，这道题目卡了我很久了，到现在还没做出来呢
   网上看了答案 有一点点头绪，但是不知道他的思路到底是什么。
  */
    int min=0x30;
    int max=0x39;
    return (!((x+(~min+1))>>31)) & (!((max+(~x+1))>>31));

    /*通过测验*/
}
/* 
 * conditional - same as x ? y : z 
 *   Example: conditional(2,4,5) = 4
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 16
 *   Rating: 3
 */

int conditional(int x, int y, int z) {
  /*这道题的意思应该是如果x是0 那么返回的结果是z，否则返回的结果就是y
  自己思考了一下 有点懵逼，之前完全没有见过，网上的答案如下,说大致类似数字逻辑里面的多路选择器
  下面的逻辑就是如果 x为0 那么!x=1 ~!x=0XFFFFFFFE m=OXFFFFFFFF 则返回的是z 否则就是y 
  */
     int m = ~!x + 1;
      return (y & ~m) | (z & m);
}

/* 
 * isLessOrEqual - if x <= y  then return 1, else return 0 
 *   Example: isLessOrEqual(4,5) = 1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 24
 *   Rating: 3
 */
int isLessOrEqual(int x, int y) {
  /*感觉又是一道选择的题目，第一次思考一下🤔，好吧又没思考出来，感觉自己好菜啊啊啊啊
  看了一下别人的一个简单的做法，就是用最高位的符号位来判断，emmm，我感觉哪里会有问题，好像是会有溢出问题。
  */
  
  //return   !(((y+(~x+1))>>31)&1);

  int diff_sgn = !(x>>31)^!(y>>31);      //is 1 when signs are different
  int a = diff_sgn & (x>>31);            //diff signs and x is neg, gives 1
  int b = !diff_sgn & !((y+(~x+1))>>31); //same signs and difference is pos or = 0, gives 1
  int f = a | b;
  return f;
 /*通过了测试
  如果符号相等 可以 用 x<= y 变换为 x-y<=0
如果符合不相等，可以直接通过符号比较（此时相减，会有溢出问题）
判断符号是否相等只需要 看最高位，最高位可以用算术右移来变为全0和全1.
随后异或判相等，相等得到全0，不等得到全1.
<=0 也可以通过相减 之后看最高位是不是1来判断。
最后根据这个2个结果，只要满足其中一个，就可以成立。
 */

}
//4
/* 
 * logicalNeg - implement the ! operator, using all of 
 *              the legal operators except !
 *   Examples: logicalNeg(3) = 0, logicalNeg(0) = 1
 *   Legal ops: ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 4 
 */
int logicalNeg(int x) {
  /*这个操作应该是只要是0就返回1，其他均返回0，感觉比较简单能实现
   第一次思考：用写好的conditional，但是好像这里面是不让的？ emmmm啊啊感觉自己是一个大菜鸡
   网上看到说右移在这里都是算术右移!下面看到一种答案 说的是下面的解法用到了0的特性
  */

 /*  它使用了0的特性~x+1 == ~x*/  
  return ((~(~x+1)&(~x))>>31)&1; 
  /*上面的代码通过了测试*/
  
}

/* howManyBits - return the minimum number of bits required to represent x in
 *             two's complement
 *  Examples: howManyBits(12) = 5
 *            howManyBits(298) = 10
 *            howManyBits(-5) = 4
 *            howManyBits(0)  = 1
 *            howManyBits(-1) = 1
 *            howManyBits(0x80000000) = 32
 *  Legal ops: ! ~ & ^ | + << >>
 *  Max ops: 90
 *  Rating: 4
 */
int howManyBits(int x) {
  /*我感觉这个真的要成竞赛题目了吧，凭借自己的力量可能这辈子都想不出来解决方法*/

  // flip if negative => remove the duplicated sign bits
  int negativeFlag = (x >> 31) & 1;
  // conditional
  int mappedFlag = ((negativeFlag | (~negativeFlag + 1)) >> 31);
  int yResult = ~x & mappedFlag;
  int zResult = x & (~mappedFlag);
  int newX = yResult + zResult;
  // right align & fill with ones
  int full1s = newX | newX >> 16;
  full1s = full1s | full1s >> 8;
  full1s = full1s | full1s >> 4;
  full1s = full1s | full1s >> 2;
  full1s = full1s | full1s >> 1;
  // count ones
  int count = (full1s & 0x55555555) + ((full1s >> 1) & 0x55555555);
  count = (count & 0x33333333) + ((count >> 2) & 0x33333333);
  count = (count & 0x0F0F0F0F) + ((count >> 4) & 0x0F0F0F0F);
  count = (count & 0x00FF00FF) + ((count >> 8) & 0x00FF00FF);
  count = (count & 0x0000FFFF) + ((count >> 16) & 0x0000FFFF);
  // plus one (sign bit)
  return count + 1;
}
//float
/* 
 * floatScale2 - Return bit-level equivalent of expression 2*f for
 *   floating point argument f.
 *   Both the argument and result are passed as unsigned int's, but
 *   they are to be interpreted as the bit-level representation of
 *   single-precision floating point values.
 *   When argument is NaN, return argument
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 30
 *   Rating: 4
 */
unsigned floatScale2(unsigned uf) {
 /*逐渐渣化 失去自己的表情...... IEEE的标准 我们要注意的是，这里的NaN 包括0或者OXFFFFFFFF
 For this question, we first need to determine whether this number is a normalized, denormalized or special value representation by checking the exp field. We can get the value of exp field using ((uf >> 23) & 0xff) (i.e. shift to right by 23 and get rid of the sign bit).
If the number is a 0 or special value representation, return uf without any modification.
If the number is a denormalized representation, shift the frac field to left by 1 without changing the exp field. To get the frac field, we use (uf & 0x007FFFFF) to empty all bits but the frac field. Don't forget to put the sign bit back.
If the number is a normalized representation, add 1 to the exp field.
*/

 //0
  if(uf == 0 || uf == (1 << 31))
    return uf;
  //Special value
  if(((uf >> 23) & 0xff) == 0xff)
    return uf;
  //Denormalized
  if(((uf >> 23) & 0xff) == 0x00) 
    return ((uf & 0x007FFFFF) << 1) | ((1 << 31) & uf);
  // Normalized
  return uf + (1<<23); //这里不懂为什么最后是add1？？啊啊啊！明白了，因为此时是IEEE的标准，指数加1，那么就是乘以了2！！建议再把课好好看一遍就懂了
 /*通过测试*/
}
/* 
 * floatFloat2Int - Return bit-level equivalent of expression (int) f
 *   for floating point argument f.
 *   Argument is passed as unsigned int, but
 *   it is to be interpreted as the bit-level representation of a
 *   single-precision floating point value.
 *   Anything out of range (including NaN and infinity) should return
 *   0x80000000u.
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 30
 *   Rating: 4
 */
int floatFloat2Int(unsigned uf) {
  /*这里真的感觉自己不太会做了 好痛苦啊*/
  int sign = (uf >> 31) & 0x1;
  int e = (uf >> 23) & 0xFF;
  int frac = uf & 0x7FFFFF;
  
  int exponent = e - 127;
  // add the implicit one
  int newFrac = 0x1000000 + frac;
  int shifted;
  // if e equals zero -> denorm -> will be rounded to 0 while casting to integer
  // if exponent is negative -> will be rounded to 0 while casting to integer 
  if(exponent < 0 || e == 0) {
    return 0;
  }
  // if exponent is greater than or equal to 31 -> overflow
  // if e == 0xFF -> special value
  if(exponent >= 31 || e == 0xFF) {
    return 0x80000000;
  }
  // if exponent is greater than 24, shift to left by (exponent - 24)
  if(exponent > 24) {
    shifted = newFrac << (exponent - 24);
  }
  // if exponent is less than or equal to 24, shift to right by (24 - exponent)
  else if(exponent <= 24) {
    shifted = newFrac >> (24 - exponent);
  }
  // negate if signed
  if(sign)
    shifted = -shifted;
  return shifted;

  /*通过测试*/
}
/* 
 * floatPower2 - Return bit-level equivalent of the expression 2.0^x
 *   (2.0 raised to the power x) for any 32-bit integer x.
 *
 *   The unsigned value that is returned should have the identical bit
 *   representation as the single-precision floating-point number 2.0^x.
 *   If the result is too small to be represented as a denorm, return
 *   0. If too large, return +INF.
 * 
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. Also if, while 
 *   Max ops: 30 
 *   Rating: 4
 */
unsigned floatPower2(int x) {
  /*还是看网上的思路，自学真的没人监督讨论的话，真的好痛苦，自己也不是大佬*/
    if(x < -150) {
    return 0;
  }
  if(x >= -150 && x <= -127) {
    //denorm
    int shiftAmount = (-x - 127);
    int frac = 1 << shiftAmount;
    return frac;
  }
  if(x >= -126 && x <= 127) {
    //norm
    int e = (x + 127) << 23;
    return e;
  }
  if(x >= 128) {
    //inf
    int e = 0xFF << 23;
    return e;
  }
  return 0;

  /*不知道为什么 通不过测试*/
}

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
  return ~(~(~x & y) & ~(~y & x));
}
/* 
 * tmin - return minimum two's complement integer 
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 4
 *   Rating: 1
 */
int tmin(void) {
  return 1 << 31;
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
  int is_negone  = !(x + 1);
  int is_tmax = !(~(x + 1 + x));
  return !is_negone & is_tmax;
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
  int y = 0xAA | (0xAA << 8) | (0xAA << 16) | (0xAA << 24);
  return !((x & y) ^ y);
}
/* 
 * negate - return -x 
 *   Example: negate(1) = -1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 5
 *   Rating: 2
 */
int negate(int x) {
  return ~x + 1;
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
  /* is bigger than 0x30 and smaller than 0x3F */
  int is_upper = !((x & ~0x0F) ^ 0x30);
  /* is smaller than 0x39 */
  int is_lower = !((x & 0x0F) + 0x06 & 0xF0);
  return is_upper & is_lower;
}
/* 
 * conditional - same as x ? y : z 
 *   Example: conditional(2,4,5) = 4
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 16
 *   Rating: 3
 */
int conditional(int x, int y, int z) {
  /* x =  0 => 0xFFFFFFFF
     x != 0 => 0x00 */
  int is_zero = ~!x + 1;
  int is_y = ~is_zero & y;
  int is_z = is_zero  & z;
  return is_y | is_z;
}
/* 
 * isLessOrEqual - if x <= y  then return 1, else return 0 
 *   Example: isLessOrEqual(4,5) = 1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 24
 *   Rating: 3
 */
int isLessOrEqual(int x, int y) {
  /* pos => 0 | neg => 1 */
  int sign_x = x >> 31;
  int sign_y = y >> 31;
  // int neg_y = ~y + 1;
  /* if x or y is neg, turn to pos */
  int negtpos_bias = ~(1 << 31);
  int negtpos_x = x & negtpos_bias;
  int negtpos_y = y & negtpos_bias;
  /* if success return 1
     else return 0 */
  int pos_pos = !(( sign_x | sign_y) | (~x + 1 + y >> 31));
  // int pos_neg = 0;
  int neg_pos = ( sign_x & !sign_y);
  int neg_neg = ( sign_x &  sign_y) & !(~negtpos_x + 1 + negtpos_y >> 31);
  /* return */
  return pos_pos | neg_pos | neg_neg;
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
  return (x >> 31 | (x ^ (~x + 1)) >> 31) + 1;
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
  /* use conditional() before */
  {
    int sign = x >> 31;
    int y = ~x, z = x;
    int is_zero = ~!sign + 1;
    int is_y = ~is_zero & y;
    int is_z = is_zero  & z;
    x =  is_y | is_z;
  }
  int b16 = !!(x >> 16) << 4;
  x = x >> b16;
  int b8  = !!(x >> 8 ) << 3;
  x = x >> b8;
  int b4  = !!(x >> 4 ) << 2;
  x = x >> b4;
  int b2  = !!(x >> 2 ) << 1;
  x = x >> b2;
  int b1  = !!(x >> 1 ) << 1;
  x = x >> b1;
  int b0 = x;
  return b16 + b8 + b4 + b2 + b1 + b0 + 1;
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
  unsigned bias_e = 0xFF;
  unsigned bias_m = (0x7F << 16) + (0xFF << 8) + 0xFF;
  unsigned sign = uf >> 31;
  unsigned e = (uf >> 23) & bias_e;
  unsigned m = uf & bias_m;

  /* if is 0 */
  if (!e && !m) return uf;
  /* if is NaN or Max or Min */
  if (!(e ^ bias_e)) return uf;

  /* if unnormalized */
  if (!e) {
    m = m + m;
    if ((m >> 24) & 1) {
      e = e + 1;
      m = m & bias_m;
    }
  }
  /* if normalized */
  else
    e = e + 1;

  return (sign << 31) + (e << 23) + m;
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
  unsigned bias_e = 0xFF;
  unsigned bias_m = (0x7F << 16) + (0xFF << 8) + 0xFF;
  unsigned sign = uf >> 31;
  unsigned e = (uf >> 23) & bias_e;
  unsigned m = uf & bias_m;

  /* if unnormalized */
  int res = 0;
  if (!e)
    return 0;
  else {
    if (e < 127)
      return 0;
      // res = (m + (1 << 23)) / ((1 << 23) * (1 << (127 - e)));
    else if (e <= 127 + 31)
      res = (m + (1 << 23)) * (1 << (e - 127)) / (1 << 23);
    else
      res = 0x80000000;
  }

  /* add - */
  if (sign)
    res = -res;

  return res;
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
  unsigned res = 0;

  /* the minimun is 2^(-(1 - 127)) * 2^(-23) */
  if (x < -23 - 126)
    return 0;
  /* the maximum is 2^(254 - 127) */
  if (x > 127)
    return 0x7F800000;
  
  /* when unnormalized */
  if (x < -126) {
    x = -x;
    res = res | 1 << (x - 126);
  }
  /* when normalized and e is less than 127 */
  else if (x <= 0) {
    x = -x;
    res = res | (127 - x) << 23;
  }
  /* when normalized and e is more than 127 */
  else {
    res = res | (127 + x) << 23;
  }

  return res;
}

#include <stdio.h>

#define ARRAY_SIZE 10

// Global array for use in functions `f1()` and `f2()`.
int array1[ARRAY_SIZE] = { 2, 4, 6, 8, 10, 12, 14, 16, 18, 20 };

/*
 * This function just multiplies by 2.
 */
int f1 (int a) {
  return 2 * a;
}

/*
 * This function adds two input values together.
 */
int f2 (int a, int b) {
  return a + b;
}

/*
 * This function computes the sum of the elements from the input array.
 */
int f3 () {
  int sum = 0;
  for (int i = 0; i < ARRAY_SIZE; i++)
    sum += array1[i];
  return sum;
}

/*
 * This function adds the input argument to each of the elements from the
 * global array `array1`.
 */
void f4 (int n) {
  for (int i=0; i < ARRAY_SIZE; i++)
    array1[i] += n;
}

int main (int argc, char** argv){
  int r1 = 0, r2 = 0, r3 = 0;
  int t1 = 6, t2 = 210, t3 = 201, t4 = 5;
  r1 = f1(t1);
  r2 = f2(t2, t3);
  printf("f1(%d) = %d, f2(%d, %d) = %d\n", t1, r1, t2, t3, r2);
  r3 = f3();
  printf("Before f4, f3() = %d\n",r3);
  f4(t4);
  r3 = f3();
  printf("After f4, f3() = %d\n",r3);
  return 0;
}

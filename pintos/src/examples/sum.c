#include <stdio.h>
#include <stdlib.h>
#include <syscall.h>
//#include <lib/kernel/syscall.h>
//#include <userprog/syscall.h>

int
main (int argc, char *argv[]) {
  int a, b, c, d, fibo_res, sum_res;

  if (argc != 5) {
    printf("argument 5!!\n");
    return EXIT_FAILURE;
  }

  a = atoi(argv[1]);
  b = atoi(argv[2]);
  c = atoi(argv[3]);
  d = atoi(argv[4]);

  fibo_res = fibonacci(a);
  sum_res = sum_of_four_int(a, b, c, d);

  printf("%d %d\n", fibo_res, sum_res);

  return EXIT_SUCCESS;
}


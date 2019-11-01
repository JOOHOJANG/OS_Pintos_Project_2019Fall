#include "userprog/syscall.h"
#include <stdio.h>
#include <syscall-nr.h>
#include "threads/interrupt.h"
#include "threads/thread.h"

static void syscall_handler (struct intr_frame *);

void
syscall_init (void) 
{
  intr_register_int (0x30, 3, INTR_ON, syscall_handler, "syscall");
}

static void
syscall_handler (struct intr_frame *f UNUSED) 
{


  printf ("system call!\n");
  thread_exit ();
}



void halt (void)
{
  shutdown_power_off();
}

void exit (int status)
{
  struct thread *cur;

  cur = thread_current();

  printf("%s: exit(%d)\n", cur->name, status);
}
/*
pid_t exec (const char *cmd_line)
{
  
}

int wait (pid_t pid)
{

}
*/
int read (int fd, void *buffer, unsigned size)
{
  void *temp = buffer;
  int i;

  if (fd == 0) {
    for (i = 0; i < (int)size; i++) {
      *(uint8_t *)buffer = input_getc();
      if (*(uint8_t *)buffer++ == '\0') {
        break;
      }
    }

    buffer = temp;
    return i;
  }

  return -1;
}
/*
int write (int fd, const void *buffer, unsigned size)
{
  if (fd == 1) {

  }

  
}
*/
int fibonacci (int n)
{
  int a = 0, res = 1;

  for (int i = 0; i < n; i++) {
    int t = res;
    res += a;
    a = res;
  }

  return res;
}

int sum_of_four_int (int a, int b, int c, int d){
  return a + b + c + d;
}

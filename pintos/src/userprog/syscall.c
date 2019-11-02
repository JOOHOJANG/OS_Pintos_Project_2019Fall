#include "userprog/syscall.h"
#include <stdio.h>
#include <syscall-nr.h>
#include "threads/interrupt.h"
#include "threads/thread.h"
#include "threads/vaddr.h"
static void syscall_handler (struct intr_frame *);
void check_vaddr(const void* vaddr);
void
syscall_init (void) 
{
  intr_register_int (0x30, 3, INTR_ON, syscall_handler, "syscall");
}

static void
syscall_handler (struct intr_frame *f) 
{

  switch (*(uint32_t*)(f->esp)){
   case SYS_HALT:
	halt();
	break;
   case SYS_EXIT:
	check_vaddr(f->esp+4);
	exit((f->eax));
	break;
   case SYS_EXEC:
	check_vaddr(f->esp+4);
	f->eax = exec((char*)(f->esp+4));
	break;
   case SYS_WAIT:
	f->eax = wait(*(int *)(f->esp+4));
 	break;
   case SYS_READ:
	f->eax = read((int)*(uint32_t*)(f->esp+4), (void*)*(uint32_t*)(f->esp+8),(unsigned)*(uint32_t*)(f->esp+12));
	break;
   case SYS_WRITE:
	f->eax = write((int)*(uint32_t*)(f->esp+4),(void*) *(uint32_t*)(f->esp+8),(unsigned)*(uint32_t*)(f->esp+12));
	break;
   case SYS_FIBONACCI:
	f->eax = fibonacci((int)*(uint32_t*)(f->esp+4));
	break;
   case SYS_SUM:
	f->eax = sum_of_four_int((int)*(uint32_t*)(f->esp+4), (int)*(uint32_t*)(f->esp+8), (int)*(uint32_t*)(f->esp+12), (int)*(uint32_t*)(f->esp+16));
  }

}
void halt (void){
	shutdown_power_off();
}

void exit(int status){
	struct thread* cur;	
	cur = thread_current();
	printf("%s: exit(%d)\n", cur->name, status);
	thread_exit();
}

int write(int fd, const void *buffer, unsigned size){
	if(fd==1){
		putbuf(buffer, size);
		return size;
	}
	return -1;
}

int wait (pid_t pid){
	return process_wait(pid);
}

int read (int fd, void *buffer, unsigned size)
{
  int i;
  void *temp = buffer;
  if (fd == 0) {
    for (i = 0; i < (int)size; i++) {
      *(uint8_t *)temp = input_getc();
      if (*(uint8_t *)temp++ == '\0') {
        break;
      }
    }

    return i;
  }

  return -1;
}
pid_t exec (const char *cmd_line)
{
 return process_execute(cmd_line);
}

int fibonacci (int n)
{
  int a = 0, res = 1, i;

  for (i = 1; i < n; i++) {
    int t = res;
    res += a;
    a = t;
  }

  return res;
}

int sum_of_four_int (int a, int b, int c, int d){
  return a + b + c + d;
}

void check_vaddr(const void *vaddr){
	if(!is_user_vaddr(vaddr)){
		exit(-1);
	}
} 

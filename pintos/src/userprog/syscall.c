#include "userprog/syscall.h"
#include <stdio.h>
#include <syscall-nr.h>
#include "threads/interrupt.h"
#include "threads/thread.h"
#include "threads/vaddr.h"
#include "filesys/file.h"

struct lock sys_lock;

static void syscall_handler (struct intr_frame *);
void
syscall_init (void) 
{
  lock_init(&sys_lock);
  intr_register_int (0x30, 3, INTR_ON, syscall_handler, "syscall");
}

static void
syscall_handler (struct intr_frame *f) 
{
  switch (*(uint32_t*)(f->esp)){
   case SYS_HALT:
	halt();
	break;
   case SYS_CREATE:
	f->eax = create(*(char **)(f->esp+4), *(unsigned *)(f->esp+8));
	break;
   case SYS_REMOVE:
	f->eax = remove(*(char **)(f->esp+4));
	break;
   case SYS_OPEN:
	if(!is_user_vaddr(f->esp+4)) exit(-1);
	f->eax = open(*(char**)(f->esp+4));
	break;
   case SYS_CLOSE:
	if(!is_user_vaddr(f->esp+4)) exit(-1);
	close(*(uint32_t *)(f->esp+4));
	break;
   case SYS_FILESIZE:
	f->eax = filesize(*(uint32_t *)(f->esp+4));
	break;
   case SYS_SEEK:
	if(!is_user_vaddr(f->esp+4)||!is_user_vaddr(f->esp+8)) exit(-1);
	seek((int)*(uint32_t*)(f->esp+4),(unsigned)*(uint32_t*)(f->esp+8)); 
	break;
   case SYS_TELL:
	if(!is_user_vaddr(f->esp+4))exit(-1);
	f->eax = tell((int)*(uint32_t*)(f->esp+4));
	break;
   case SYS_EXIT:
	if(!is_user_vaddr(f->esp+4)) exit(-1);
        exit(*(uint32_t *)(f->esp + 4));
	break;
   case SYS_EXEC:
	if(!is_user_vaddr(f->esp+4)) exit(-1);
	f->eax = exec(*(char**)(f->esp+4));
	break;
   case SYS_WAIT:
	if(!is_user_vaddr(f->esp+4)) exit(-1);
	f->eax = wait(*(int*)(f->esp+4));
 	break;
   case SYS_READ:
	if(!is_user_vaddr(f->esp+4) || !is_user_vaddr(f->esp+8) || !is_user_vaddr(f->esp+12)) exit(-1);
	f->eax = read((int)*(uint32_t*)(f->esp+4), (void*)*(uint32_t*)(f->esp+8),(unsigned)*(uint32_t*)(f->esp+12));
	break;
   case SYS_WRITE:
	if(!is_user_vaddr(f->esp+4) || !is_user_vaddr(f->esp+8) || !is_user_vaddr(f->esp+12)) exit(-1);
	f->eax = write((int)*(uint32_t*)(f->esp+4),(void*) *(uint32_t*)(f->esp+8),(unsigned)*(uint32_t*)(f->esp+12));
	break;
   case SYS_FIBONACCI:
	f->eax = fibonacci((int)*(uint32_t*)(f->esp+4));
	break;
   case SYS_SUM:
	f->eax = sum_of_four_int((int)*(uint32_t*)(f->esp+4), (int)*(uint32_t*)(f->esp+8), (int)*(uint32_t*)(f->esp+12), (int)*(uint32_t*)(f->esp+16));
  }
}

void halt (void) {
	shutdown_power_off();
}

void exit (int status) {
	struct thread* cur;
	int i;
	
	cur = thread_current();
	cur->exit_status = status;

	printf("%s: exit(%d)\n", cur->name, cur->exit_status);
	for(i = 2; i < 128; i++) {
		if(cur->filelist[i] != NULL) {
			close(i);
		}
	}
	thread_exit();
}

pid_t exec (const char *cmd_line) {
	return process_execute(cmd_line);
}

int wait (pid_t pid) {
	return process_wait(pid);
}

int read (int fd, void *buffer, unsigned size) {
	if(!is_user_vaddr(buffer + size)) exit(-1);

	if(fd == 0) {
		void *temp;
		int i;
		temp = buffer;
		for(i = 0; i < (int)size; i++) {
			*(uint8_t *)temp = input_getc();
			if(*(uint8_t *)temp = '\0') {
				break;
			}
			(uint8_t *)temp++;
		}
		return i;
	}
	else if(fd >= 2 && fd < 128) {
		struct file * cur_file;
		int ret;

		cur_file = thread_current()->filelist[fd];
		if(cur_file == NULL) {
			return -1;
		}
	
		lock_acquire(&sys_lock);
		ret = file_read(cur_file, buffer, size);
		lock_release(&sys_lock);

		return ret;
	}
	return -1;
}

int write (int fd, const void *buffer, unsigned size) {
	if(fd == 1) {
		putbuf(buffer, size);
		return size;
	}
	else if(fd >= 2 && fd < 128) {
		struct file * cur_file;
		int ret;

		cur_file = thread_current()->filelist[fd];
		if(cur_file == NULL) {
			return -1;
		}
	
		lock_acquire(&sys_lock);
		ret = file_write(cur_file, buffer, size);
		lock_release(&sys_lock);
		return ret;
	}
	return -1;
}

int fibonacci (int n) {
	int a, res = 1, i, t;

	for(i = 1; i < n; i++) {
		t = res;
		res += a;
		a = t;
	}
	return res;
}

int sum_of_four_int (int a, int b, int c, int d) {
	return a + b + c + d;
}

bool create (const char *file, unsigned initial_size) {
	if(!file) exit(-1);
	return filesys_create(file, initial_size);
}

bool remove (const char *file) {
	if(!file) exit(-1);
	return filesys_remove(file);
}
	
int open (const char *file) {
	if(!file) exit(-1);

	struct thread *cur;
	struct file *open_file;
	int i;

	cur = thread_current();
	open_file = filesys_open(file);
	if(open_file == NULL) {
		return -1;
	}

	if(!strcmp(thread_name(), file)) file_deny_write(open_file);
	for(i = 2; i < 128; i++) {
		if(cur->filelist[i] == NULL) {
			cur->filelist[i] = open_file;
			cur->filecnt++;
			return i;
		}
	}
	return -1;
}

int filesize (int fd) {
	return file_length(thread_current()->filelist[fd]);
}

void seek (int fd, unsigned position) {
	file_seek(thread_current()->filelist[fd], position);
}

unsigned tell (int fd) {
	return file_tell(thread_current()->filelist[fd]);
}

void close (int fd) {
	struct thread *cur;
	cur = thread_current();

	if(cur->filelist[fd] != NULL) {
		cur->filecnt--;
		file_close(cur->filelist[fd]);
		cur->filelist[fd] = NULL;
	}
}


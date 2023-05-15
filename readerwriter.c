#include <linux/kernel.h>
#include <linux/syscalls.h>
SYSCALL_ DEFINE2(readerwriter, int, a, char*, buff)
{
   printk("\n[ Reader Writer PROBLEM]: readerwriter[%d]:%s",a,buff);
   return 0;  }

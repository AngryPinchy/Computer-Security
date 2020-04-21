  .section .rodata

hello:
  # Constant holding the message string to print.
  .ascii "Hello World\n"

err_msg:
  # Constant holding error message
  .ascii "This command expects exactly one argument.\n"

new_line:
  # Constant holding "\n"
  .ascii "\n"

  .section .text

  .globl _start

  .equ  len, -4  # local var for length

_start:
  ## Print message to standard output.
  
  # ========== invalid arguments ============
  movl  0(%esp), %eax	# move the value of argc
  cmpl  $2, %eax		# compare 
  je    good_check	# if same then jump to good
  
  movl  %eax, %edx	# when it is bad 
  movl  $4, %eax	# select sys_write
  movl  $2, %ebx	# print to stderr
  movl  $err_msg, %ecx	# the message to print
  movl  $43, %edx	# the length

  int   $0x80		# trigger

  movl  $1, %eax	# select sys_exit 
  movl  $1, %ebx	# return 1
  int   $0x80  		# trigger
good_check:
  
  # ========== strlen =========
  # movl  %esp, %ebp	
  # !!
  movl  $0, %eax	# set length as 0
  movl  8(%esp), %ecx	# move the string

strlen_loop:
  movb  (%ecx,%eax,1), %dl	# get the first char 
  cmpb  $0, %dl		# compare with null 
  je    end_strlen	# if same jump
  incl  %eax		# increment the length
  jmp   strlen_loop	# jump back
end_strlen:  
  # !!
  movl  %eax, %edx
  # movl  %eax, len(%ebp)
  	
  movl  $4, %eax
  # Select the 'sys_write' syscall.
  # ===============================
  # The value in %eax determines which syscall the kernel will execute when
  # interrupt 0x80 is triggered by our code.

  movl  $1, %ebx
  # Set the output to file descriptor #1 (standard output).
  # =======================================================
  # The value in %ebx is the first argument to the syscall. In the case of
  # sys_write, that first argument is the file descriptor (fd) number to which
  # the string should be written.

  movl 8(%esp), %ecx
  # movl  $hello, %ecx
  # Set the target string to our message.
  # =====================================
  # The value in %ecx is the second argument to the syscall. In the case of
  # sys_write, that second argument is the address in memory of the string to
  # print. Since we defined 'hello' as our message string constant in the
  # .rodata section above, this will place its address in memory into %ecx.
   
  # movl  len(%ebp), %edx
  # movl  $12, %edx
  # Set the length of the string to print.
  # ======================================
  # The value in %edx is the third argument to the syscal. In the case of
  # sys_write, that third argument is the length of the string to print. Our
  # message is 12 characters long, including the trailing newline.

  int   $0x80
  # Invoke the syscall and print the message string.
  # ===================
  # This triggers interrupt number 0x80, which the Linux kernel intercepts to
  # run the syscall we select in the %eax register, using the %ebx-%edx
  # registers as its arguments.

  ## Exit the program with code 0 (success).
  # movl  $1, %eax      # Select the 'sys_exit' syscall
  # movl  $0, %ebx      # Set the exit code to 0
  # int   $0x80         # Invoke the syscall

  # ============= print a trailing newline =============
  movl  $4, %eax
  movl  $1, %ebx
  movl  $new_line, %ecx
  movl  $1, %edx
  int   $0x80
  movl  $1, %eax
  movl  $0, %ebx
  int   $0x80

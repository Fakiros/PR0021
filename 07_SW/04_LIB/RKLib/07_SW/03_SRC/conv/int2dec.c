#include "../conv.h"



void int2str(int val, char *s) 
{ 
	  char *p; 
	  unsigned char d, i; 
	  unsigned char zero; 
	  unsigned int test; 
	  unsigned int uval = val; 
  

	  p = s; 
  
	  if( val < 0 )
	  { 
		    uval = -val; 
		    *p++ = '-'; 
	  } 

	  zero = 1; 

	  i = 4; 
	  do
	  { 
		    i--;    
		    if ( i==0) test = 10; 
		    else if ( i==1) test=100; 
		    else if ( i==2) test=1000; 
		    else test=10000; 

		    for( d = '0'; uval >= test; uval -= test ) 
		    { 
			      d++; 
			      zero = 0; 
		    } 
		    if( zero == 0 ) 
		    	*p++ = d ; 
	  }while( i ); 

	  *p++ = (unsigned char)uval + '0';
	  *p++ = 0; 
} 

/*
void itoa(int n, char s[]) 
{ 
   int i, sign; 
    
   if ((sign = n) < 0) // record sign 
      n = -n;         // make n positive 
   i = 0; 
   do {   // generate digits in reverse order 
      s[i++] = n % 10 + '0'; // get next digit 
   } while ((n /= 10) > 0); // delete it 
   if (sign < 0) 
      s[i++] = '-'; 
   s[i] = '\0'; // add null terminator for string 
   reverse(s); 
} 
*/

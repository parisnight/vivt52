/* a.c */

#include <stdio.h>
#include <stdlib.h>

int main(int argc, const char** argv)
{
    char c;
    int i;
    char s[80];
    
    system("stty raw -echo");

    do {
          c=getchar();
          putc(c+1,stdout);
          fflush(stdout);  /* necessary to push out character */
	  if (c=='d') {
	    scanf("%d",&i);
	    printf("integer %d\n",i);
	  }
	  else if (c=='s') {
	    fgets(s,8,stdin);	/* use C-j to terminate in raw mode */
	    printf("string %s\n",s);
	  }
    } while (c!='q');

system("stty cooked echo");
}

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
          if (c>=32) putc(c+1,stdout); /* control characters can ruin screen */
          fflush(stdout);  /* necessary to push out character */
	  if (c=='d') {
	    scanf("%d",&i);
	    printf("integer %d\n",i);
	  }
	  else if (c=='s') {
	    /*fgets(s,8,stdin);*/	/* use C-j to terminate in raw mode */
            i=0; while ((c=getchar())!='\r') s[i++]=c; s[i]='\0';
	    printf("string %s\n",s);
	  }
    } while (c!='q');

system("stty cooked echo");
}

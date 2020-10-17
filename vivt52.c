/* vivt52.c 2020.10.15 */

#include <stdio.h>
#include <stdlib.h>

#define EBUSIZE 65536
#define DECMAX(x) (x > 1 ? x-- : 1)

char ebu[EBUSIZE];
char *cursor, *tail;
int row, col;

void insertmode ()
{
  static char linebuf[80];
  char c, *p, *cursorold;
  int n=0;
  
  cursorold=cursor+col-1;
  while ((c=getchar())!='r') {
    linebuf[n++]=c;
    col++;
  }

  for (p=tail; p >= cursorold; p--) { /* make room.  yes movemove() is faster */
    *(p+n) = *p;
  }
  
  for (p=linebuf; cursorold < cursor + col-1; cursorold++) {
    *cursorold++ = *p++;;
  }


}

int main(int argc, const char** argv)
{
  FILE *f;
  int i, j;
  char c;

  system("stty raw -echo opost");
  f=fopen("a.txt","r");
  i=fread(ebu, 1, sizeof ebu, f);
  tail=ebu+i;
  cursor=ebu;
  row=col=1;

  fputs("\033[H",stdout);  /* cursor home */
  for (j=0; j<i; j++) {
    fputc(ebu[j],stdout);
  }
  
  do {
    c=getchar();
    switch (c) {
    case 'h': fputs("\033[D",stdout); DECMAX(col); break;
    case 'j': fputs("\033[A",stdout); DECMAX(row);
      while (*cursor != '\n') cursor--;
      break;
    case 'k': fputs("\033[B",stdout); row++;
      while (*cursor != '\n') cursor++; cursor++;
      break;
    case 'l': fputs("\033[C",stdout); col++; break;
    case 'i': insertmode(); break;
    case 'd':
      fputs("\033[H",stdout);
      for (j=0; j<i; j++) {
	fputc(ebu[j],stdout);
      }
      fputs("\033[H",stdout); col=row=1;
      break;
    case 'r':
      printf("row %d col %d\n",row,col);
      break;
    }

  } while (c!='q');

  system("stty cooked echo");
}

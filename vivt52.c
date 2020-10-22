/* vivt52.c 2020.10.15 */

#include <stdio.h>
#include <stdlib.h>

#define EBUSIZE 655
#define DECMAX(x) (x > 1 ? x-- : 1)

char ebu[EBUSIZE];
char *bol, str[80];
int i, row, col, nb;

void insertmode ()
{
  char c, *p;
  int n=0;
  
  while ((c=getchar())!='\r') {
//    putchar(c);
    str[n++]=c;
    col++;
  }

  for (p=ebu+nb-1; p >= bol+col-n-1; p--) { /* make room.  yes movemove() is faster */
    *(p+n) = *p;
  }
  
  for (p++, i=0; p < bol + col-1; p++, i++) {
    *p = str[i];
  }
  nb += n;

}

int main(int argc, const char** argv)
{
  FILE *f;
  char c;

  for (i=0; i< sizeof ebu; i++) {
    ebu[i]='z';
  }
  system("stty raw -echo opost");
  f=fopen("a.txt","r");
  nb=fread(ebu, 1, sizeof ebu, f);
  bol=ebu;
  row=col=1;

  fputs("\033[H",stdout);  /* cursor home */
  for (i=0; i<nb; i++) {
    fputc(ebu[i],stdout);
  }
  
  do {
    c=getchar();
    switch (c) {
    case 'h': fputs("\033[D",stdout); DECMAX(col); break;
    case 'j': fputs("\033[A",stdout); //DECMAX(row);
      if (row>1) {
        row--;
        bol--; bol--; while (*bol != '\n') bol--; bol++;
      }
      break;
    case 'k': fputs("\033[B",stdout); row++;
      while (*bol != '\n') bol++; bol++;
      break;
    case 'l': fputs("\033[C",stdout); col++; break;
    case 'i': insertmode(); break;
    case 'd':
      fputs("\033[H\033[J",stdout);
      for (i=0; i< sizeof ebu; i++) {
	fputc(ebu[i],stdout);
      }
      printf("bolchar %c row %d col %d\n",*bol, row,col);
      fputs("\033[H",stdout); col=row=1; bol=ebu;
      break;
    case 'r':
      break;
    }

  } while (c!='q');

  system("stty cooked echo");
}

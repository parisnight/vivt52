/* vivt52.c 2020.10.15 */

#include <stdio.h>
#include <stdlib.h>

#define EBUSIZE 65536
#define DECMAX(x) (x > 1 ? x-- : 1)
#define ROWM 39

char ebu[EBUSIZE];
char *bol, str[80], *p;
int i, nb, row, col;

void insertmode ()
{
  char c;
  int n=0;
  
  while ((c=getchar())!='\033') {
    if (c=='\r') {
      c='\n';
      fputs("\033[K",stdout);
    }
    putchar(c);
    fputs("\0337",stdout);
    p=bol+col-n-1; while (*p!='\n') putchar(*p++);
    fputs("\0338",stdout);
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

movecursor(int row, int col)
{
puts("\033[");
}

int main(int argc, const char** argv)
{
  FILE *f;
  char c;

  for (i=0; i< sizeof ebu; i++) {
    ebu[i]='z';
  }
  system("stty raw -echo opost");
  if ((f=fopen(argv[1],"r"))!=NULL) nb=fread(ebu, 1, sizeof ebu, f);
  else exit(-1);
  bol=ebu;
  row=col=1;

  fputs("\033[H\033[J",stdout);  /* cursor home clear screen */
  for (i=0; i<nb; i++) {
    fputc(ebu[i],stdout);
  }
  
  do {
    c=getchar();
    switch (c) {
    case 'h': fputs("\033[D",stdout); DECMAX(col); break;
    case 'j':
      if (bol < ebu+nb) {
        while (*bol != '\n') bol++; bol++;
        fputs("\033D",stdout);
        if (row>ROWM-1) {
          fputs("\0337\r",stdout);
          for (p=bol; *p!='\n'; p++) putchar(*p);
          fputs("\0338",stdout);
        } else row++;
      }
      break;
    case 'k':
      if (bol > ebu ) {
        bol--; while (bol>ebu && *(bol-1) != '\n') bol--;
        fputs("\033M",stdout);
        if (row==1) {
          fputs("\0337\r",stdout);
          for (p=bol; *p!='\n'; p++) putchar(*p);
          fputs("\0338",stdout);
        } else row--;
      }
      break;
    case 'l': fputs("\033[C",stdout); col++; break;
    case 'i': insertmode(); break;
    case 'd':
      fputs("\033[H\033[J",stdout);
      for (p=ebu, i=0; i<ROWM; p++) { //i< sizeof ebu; i++) {
	fputc(*p,stdout);
        if (*(p+1)=='\n') i++;
      }
      printf("bolchar %c row %d col %d\n",*bol, row,col);
      fputs("\033[H",stdout); col=row=1; bol=ebu;
      break;
    case 'r': fputs("\033D",stdout); break;
    case 's': fputs("\033M",stdout); break;
      break;
    }

  } while (c!='q');

  system("stty cooked echo");
}

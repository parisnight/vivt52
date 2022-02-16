/* vivt52.c 2020.10.15 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>

#define DECMAX(x) (x > 1 ? x-- : 1)

char ebu[65536]; /* editor buffer */
char *bol, str[80], *p;
int i, nb, row, col, ROWM=39;

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

void redraw ()
{
  struct winsize w;
  system("stty raw -echo opost isig"); /* isig to allow suspending */
  /*ROWM = atoi(system("stty size")) - 1; needs some parsing */
  /* if (p = getenv("LINES")) ROWM = atoi(p) - 1; needs export */
  ioctl(0,TIOCGWINSZ,&w); ROWM=w.ws_row-1;
  fputs("\033[H\033[J",stdout);
  for (i=0, p=bol; i<ROWM && (p-ebu)<nb; p++) {
    fputc(*p,stdout);
    if (*p=='\n') i++;
  }
  /* printf("bolchar %c row %d col %d ROWM %d\n",*bol, row,col,ROWM);*/
  fputs("\033[H",stdout);
  col=row=1;
}

int main(int argc, const char** argv)
{
  FILE *f;
  char c;

  for (i=0; i< sizeof ebu; i++) {
    ebu[i]='z';
  }
  system("stty raw -echo opost isig susp \\^Z");
  if ((f=fopen(argv[1],"r"))!=NULL) nb=fread(ebu, 1, sizeof ebu, f);
  else exit(-1);
  bol=ebu;
  row=col=1;
  redraw();

  do {
    c=getchar();
    switch (c) {
    case 'h': fputs("\033[D",stdout); DECMAX(col); break;
    case 'j':
      if (bol < ebu+nb) {
        while (*bol != '\n') bol++; bol++;
        fputs("\033D",stdout);
        if (bol < ebu+nb && row>ROWM-1) {
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
    case 'd': redraw(); break;
    case 'r': fputs("\033D",stdout); break;
    case 's': fputs("\033M",stdout); break;
    case 'w':
      if ((f=fopen(argv[1],"wa"))!=NULL) nb=fwrite(ebu, 1, nb, f);
      break;
    }

  } while (c!='q');

  system("stty cooked echo");
}

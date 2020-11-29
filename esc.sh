#!/bin/sh

case "$1" in
'0') reset ;;		    # when tty is garbled
'1') printf '\033[3;12r' ;; # only scroll in lines 3 through 12
'2') printf '\033[15;8H' ;; # set cursor position (home)
'3') printf '\033[1m hello emboldened world \033[0m' ;;
'4') printf '\033D' ;;      # scroll down one line "index"
'5') printf '\033M' ;;      # scroll up one line "reverse index"
'6') printf '\033[31m' ;;   # red foreground (text)
'7') printf '\033[44m' ;;   # blue background
'8') printf '\033[5F' ;;    # cursor first column up five lines
'9') printf '\033[1P' ;;    # delete character
'restart')
  ;;
*)
  echo "usage $0 command"
esac
# cat ESC ; echo -n "[1m --warning: not network connection" ; cat ESC ;echo "[0m"

#include "libs.h"
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <termios.h>
int configSerial(char* file, int bauds)
{
    int serial_port= open(file, O_RDONLY);
    struct termios tty;
    if (serial_port <0) {
        fprintf(stderr, "Error %i open %s", errno, strerror(errno));
        return EXIT_FAILURE;
    }
     if(tcgetattr(serial_port, &tty) != 0) {
      fprintf(stderr,"Error %i from tcgetattr: %s\n", errno, strerror(errno));
      return -1;
  }

  tty.c_cflag &= ~PARENB; // Pas de bit de parité
  tty.c_cflag &= ~CSTOPB; // 1 bit stop
  tty.c_cflag &= ~CSIZE; // 8 bits de données (cleat + set)
  tty.c_cflag |= CS8; 
  tty.c_cflag &= ~CRTSCTS; // Désactiver RTS/CTS
  tty.c_cflag |= CREAD | CLOCAL; // Activation de la lecture et on ignore les inst. de contrôle

  tty.c_lflag |= ICANON; // lecture caractère par caractère et non ligne par ligne
  tty.c_lflag &= ~ECHO; // Pas d'écho
  tty.c_lflag &= ~ECHOE;
  tty.c_lflag &= ~ECHONL;
  tty.c_lflag &= ~ISIG; // On n'interpête pas les caractères INTR, QUIT and SUSP
  tty.c_iflag &= ~(IXON | IXOFF | IXANY); // Désactiver le contrôle de flux logiciel
  tty.c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL); // Pas de traitement des octets de contrôle

  // tty.c_oflag &= ~OPOST; // Prevent special interpretation of output bytes (e.g. newline chars)
  // tty.c_oflag &= ~ONLCR; // Prevent conversion of newline to carriage return/line feed
  // tty.c_oflag &= ~OXTABS; // Prevent conversion of tabs to spaces (NOT PRESENT ON LINUX)
  // tty.c_oflag &= ~ONOEOT; // Prevent removal of C-d chars (0x004) in output (NOT PRESENT ON LINUX)

  tty.c_cc[VTIME] = 100;    // Lecture bloquante, avec un timeout de 10 secondes (unité : 0.1 s)
  tty.c_cc[VMIN] = 0;

  // Vitesse 
  cfsetispeed(&tty, bauds);
  //cfsetospeed(&tty, bauds);

  // Enregistrement des réglages
  if (tcsetattr(serial_port, TCSANOW, &tty) != 0) {
      fprintf(stderr,"Error %i from tcsetattr: %s\n", errno, strerror(errno));
      return -1;
  }
  return serial_port; 
}
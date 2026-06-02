#include "libs.h"
#include <stdio.h>
int ConsumeSerial()
{
  char read_buf [256];
  int serial_port; 
  
  // Effacement du buffer
  memset(&read_buf, '\0', sizeof(read_buf));
  // Ouverture du port série
  serial_port=configSerial("/dev/ttyAMA2", B9600);
  if (serial_port < 0) return 1;
  
  // La lecture des données est réalisé par un appel à la fonction read
  read(serial_port, read_buf, sizeof(read_buf));
  printf("\n%s\n", read_buf);
  close(serial_port);
  return EXIT_SUCCESS;
}
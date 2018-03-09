#include <iostream>
#include "grabber.h"

int main(int argc, char* argv[]){
    if (argc < 2) {
         fprintf(stderr,"Please enter the port we want to run grabber on.");
         exit(1);
     }
    Grabber grabber;
    int portNo = atoi(argv[1]);
    grabber.start(portNo);    
}
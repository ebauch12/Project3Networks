#include <stdio.h>
#include <vector>

#ifndef GOBACKN_H
#define GOBACKN_H
// ***********************************************************
// * Any additional include files should be added here.
// ***********************************************************

// ***********************************************************
// * Any functions you want to add should be included here.
// ***********************************************************
struct pkt make_pkt(int sequenceNumber, char data[20]);
// int computeChecksum(struct pkt packet);

// extern std::vector<pkt> uninitialized;
//header
int CalcSum(struct pkt packet);


#endif
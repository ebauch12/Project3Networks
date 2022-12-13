#include "includes.h"
#include <vector>

std::vector<pkt> uninitialized;

// ***************************************************************************
// * ALTERNATING BIT AND GO-BACK-N NETWORK EMULATOR: VERSION 1.1  J.F.Kurose
// *
// * These are the functions you need to fill in.
// ***************************************************************************

// ***************************************************************************
// * The following routine will be called once (only) before any other
// * entity A routines are called. You can use it to do any initialization
// ***************************************************************************
void A_init() {
    // std::vector<pkt> uninitialized(0); //initialize here?
}

// ***************************************************************************
// * The following routine will be called once (only) before any other
// * entity B routines are called. You can use it to do any initialization
// ***************************************************************************
void B_init() {
    // std::vector<pkt> uninitializedB = 0;
}

// ***************************************************************************
// * Called from layer 5, passed the data to be sent to other side 
// ***************************************************************************
bool rdt_sendA(struct msg message) {
    INFO << "RDT_SEND_A: Layer 4 on side A has received a message from the application that should be sent to side B: "
              << message << ENDL;
    pkt newPkt = pkt();
    newPkt.seqnum = 0;
    newPkt.acknum = 0;
    newPkt.checksum = 0;
    bool accepted = true;

    //possible error: not adding packet even though size is greater than 10
    // processing 10 at a time because of buffer?
    if (uninitialized.size() < 10) {
           uninitialized.push_back(newPkt);
           simulation->udt_send(A, newPkt);
    }
    else {
        accepted = false;
        //print out error, try again later?
    }
 
     

    return (accepted);
}


// ***************************************************************************
// * Called from layer 3, when a packet arrives for layer 4 on side A
// ***************************************************************************
void rdt_rcvA(struct pkt packet) {
}


// ***************************************************************************
// * Called from layer 5, passed the data to be sent to other side
// ***************************************************************************
bool rdt_sendB(struct msg message) {
    INFO<< "RDT_SEND_B: Layer 4 on side B has received a message from the application that should be sent to side A: "
              << message << ENDL;

    bool accepted = false;

    return (accepted);
}


// ***************************************************************************
// // called from layer 3, when a packet arrives for layer 4 on side B 
// ***************************************************************************
void rdt_rcvB(struct pkt packet) {
    INFO << "RTD_RCV_B: Layer 4 on side B has received a packet from layer 3 sent over the network from side A:"
         << packet << ENDL;
    msg newMsg = msg();
    char data [20];
    std::memcpy(packet.payload,data,20);
    simulation->deliver_data(B, newMsg);
}

// ***************************************************************************
// * Called when A's timer goes off 
// ***************************************************************************
void A_timeout() {
    INFO << "A_TIMEOUT: Side A's timer has gone off." << ENDL;
}

// ***************************************************************************
// * Called when B's timer goes off 
// ***************************************************************************
void B_timeout() {
    INFO << "B_TIMEOUT: Side B's timer has gone off." << ENDL;
}

bool checksum(uint16_t checksumA, uint16_t checksumB) {
    return checkSumA == checkSumB;
}
#include "includes.h"
#include <bits/stdc++.h>
#include <vector>

std::vector<pkt> uninitialized;
int base;
int nextSeq;
int expectedSeq;
double sendTa;
int recTa;
double ertta;

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
    base = 1;
    nextSeq = 1;
    expectedSeq = 1;
    ertta = 100; //???
}

// ***************************************************************************
// * The following routine will be called once (only) before any other
// * entity B routines are called. You can use it to do any initialization
// ***************************************************************************
void B_init() {
    expectedSeq = 1;
}

// ***************************************************************************
// * Called from layer 5, passed the data to be sent to other side 
// ***************************************************************************
bool rdt_sendA(struct msg message) {
    
    // if(nextSeq - base >= 10){
    if (nextSeq >= (base + 10)) { //if window is full
    //send data to upper layer???
        return false;
    }
    INFO << "RDT_SEND_A: Layer 4 on side A has received a message from the application that should be sent to side B: "
              << message << ENDL;

        pkt newPkt;
        newPkt.acknum = 0;
        newPkt.seqnum = nextSeq;
        int cs = 0;
        for(int i = 0; i < 20; i++){
            newPkt.payload[i] = message.data[i];
            cs += int(message.data[i]);
        }
        newPkt.checksum = cs;
        simulation->udt_send(A, newPkt);
        simulation->start_timer(A, ertta);
        sendTa = simulation->getSimulatorClock(); //RTT
        uninitialized.push_back(newPkt);
        nextSeq ++;
    return true;
}


// ***************************************************************************
// * Called from layer 3, when a packet arrives for layer 4 on side A
// ***************************************************************************
void rdt_rcvA(struct pkt packet) {
    //is acknum equal to base, if not send packet[acknum + 1]
    int calcChecksum = 0;
        for(int i = 0; i < 20; i++){
            calcChecksum += int(packet.payload[i]);
        }
    // if (packet.acknum != nextSeq - 1) {
    //     simulation->start_timer(A, float(ertta*4));
    //     simulation->udt_send(A, packet);
        // sendTa = simulation->getSimulatorClock(); //RTT ???
    // }
    // newPkt.checksum = cs;
    // if(packet.checksum == calcChecksum && packet.acknum >= base){
    if(packet.checksum == calcChecksum && packet.acknum >= base){
        recTa = simulation->getSimulatorClock();
        base = packet.acknum + 1; //previous packet successfully, move base up 1
        // if(base == nextSeq){
        simulation->stop_timer(A);
            ertta *= base;
            ertta += (recTa - sendTa);
            ertta /= base;
    }
}


// ***************************************************************************
// * Called from layer 5, passed the data to be sent to other side
// ***************************************************************************
bool rdt_sendB(struct msg message) {
    INFO<< "RDT_SEND_B: Layer 4 on side B has received a message from the application that should be sent to side A: "
              << message << ENDL;

    bool accepted = true;

    return (accepted);
}


// ***************************************************************************
// // called from layer 3, when a packet arrives for layer 4 on side B 
// ***************************************************************************
void rdt_rcvB(struct pkt packet) {
    INFO << "RTD_RCV_B: Layer 4 on side B has received a packet from layer 3 sent over the network from side A:"
         << packet << ENDL;
    
    int calcChecksum = 0;
    for(int i = 0; i < 20; i++){
        calcChecksum += int(packet.payload[i]);
    }
    bool noC = false;
    bool order = false;
    pkt newPkt;
    newPkt.acknum = packet.seqnum;
    if(packet.seqnum == expectedSeq){
        order = true;
    }
    else {
        // newPkt.acknum = base - 1; //???
                newPkt.acknum = base; //???
        newPkt.seqnum = 0;
    }
    if(packet.checksum == calcChecksum){
        noC = true;
        newPkt.checksum = calcChecksum; //???
    }
    else{
        // newPkt.acknum = packet.acknum - 1; //???
        newPkt.acknum = base; //???
        newPkt.seqnum = 0;

    }

    if(packet.seqnum == expectedSeq){
        order = true;
    }


    msg newMsg;
    if(noC && order){
        // newPkt.seqnum = expectedSeq;
        // newPkt.acknum = packet.seqnum;
        expectedSeq++;
        for(int i = 0; i < 20; i++){
            newMsg.data[i] = packet.payload[i];
            newPkt.payload[i] = packet.payload[i];
        }

        int cs = 0;
        for(int i = 0; i < 20; i++){
            cs += int(newPkt.payload[i]);
        }

        newPkt.checksum = cs;
        simulation->deliver_data(B, newMsg);
        // newPkt.seqnum = 0;
    }
    newPkt.seqnum = 0;
    simulation->udt_send(B, newPkt);
}

// ***************************************************************************
// * Called when A's timer goes off 
// ***************************************************************************
void A_timeout() {
    INFO << "A_TIMEOUT: Side A's timer has gone off." << ENDL;
    for(int i = base; i <= nextSeq - 1; i++){
        simulation->start_timer(A, ertta*4);
        simulation->udt_send(A, uninitialized[i]);
    }
}

// ***************************************************************************
// * Called when B's timer goes off 
// ***************************************************************************
void B_timeout() {
    INFO << "B_TIMEOUT: Side B's timer has gone off." << ENDL;
}
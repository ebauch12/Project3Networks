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

// int sendTb;
// int recTb;
// double erttb;
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
    base = 0;
    nextSeq = 0;
    expectedSeq = 0;
    ertta = 1;
}

// ***************************************************************************
// * The following routine will be called once (only) before any other
// * entity B routines are called. You can use it to do any initialization
// ***************************************************************************
void B_init() {
    // std::vector<pkt> uninitializedB = 0;
    expectedSeq = 0;
    // erttb = 1;
}

// ***************************************************************************
// * Called from layer 5, passed the data to be sent to other side 
// ***************************************************************************
bool rdt_sendA(struct msg message) {
    // if(nextSeq - base >= 10){
    if (nextSeq >= (base + 10)) { //if window is full
        return false;
    }
    INFO << "RDT_SEND_A: Layer 4 on side A has received a message from the application that should be sent to side B: "
              << message << ENDL;

    bool accepted = true;
    // if(nextSeq < base + 20 && uninitialized.size() < 20){
    //     pkt newPkt = pkt();
    //     newPkt.acknum = 0;
    //     newPkt.checksum = 0;
    //     newPkt.seqnum = 0;
    //     for(int i = 0; i < 20; i++){
    //         newPkt.payload[i] = message.data[i];
    //     }
    //     uninitialized.push_back(newPkt);
    //     simulation->udt_send(A, uninitialized.at(nextSeq - 1));;
    //     if(base==nextSeq){
    //         simulation->start_timer(A, 1.0);
    //     }
    //     nextSeq++;
    // }
    // else {
    //     accepted = false;
    // }
    // char data [20];

    // std::vector<std::bitset<8>> mes;

    // for(int i = 0; i < 10; i++) {
    //     mes.push_back(std::bitset<8> (message[i]));
    // }
    // pkt.checksum = mes;
    // std::vector<int> mes;
    // int x = 0;
    // for (int i = 0; i < 10; i++) {
    //     // mes.push_back(message.data[i]);
    //     x += int(message.data[i]);
    // }
    // // std::cout << "X: " << x << std::endl;

    // newPkt.checksum = x;


    //possible error: not adding packet even though size is greater than 10
    // processing 10 at a time because of buffer?
    // if (nextSeq < base + 20) {
        // if(nextSeq < base + 20){
        //     std::cout << "SEQ < BASE + 20" << std::endl;
        // }
        pkt newPkt;
        newPkt.acknum = base - 1; //POSSIBLY NEEDED
        newPkt.seqnum = nextSeq;
        // newPkt.checksum = 0;
        int cs = 0;
        for(int i = 0; i < 20; i++){
            newPkt.payload[i] = message.data[i];
            cs += int(message.data[i]);
        }
        newPkt.checksum = cs;
        // newPkt.checksum = atoi(newPkt.payload);
        // std::cout << newPkt.checksum << std::endl;
        // std::cout << newPkt.payload << std::endl;
        // std::cout << atoi(newPkt) << std::endl;
        //  = data;
        uninitialized.push_back(newPkt);
        simulation->udt_send(A, newPkt);
        // if (base == nextSeq) {
            simulation->start_timer(A, float(ertta*4));
        // }
        nextSeq ++;
        // simulation->start_timer(A, float(ertta*4));
        sendTa = simulation->getSimulatorClock(); //RTT
    // }
    // else {
    //     accepted = false;
    //     //print out error, try again later?
    // }
 
     

    return (accepted);
}


// ***************************************************************************
// * Called from layer 3, when a packet arrives for layer 4 on side A
// ***************************************************************************
void rdt_rcvA(struct pkt packet) {
    // int calcChecksum = atoi(packet.payload);
    int calcChecksum = 0;
        for(int i = 0; i < 20; i++){
            calcChecksum += int(packet.payload[i]);
        }
    // newPkt.checksum = cs;
    // if(packet.checksum == calcChecksum && packet.acknum >= base){
    if(packet.checksum == calcChecksum){
        recTa = simulation->getSimulatorClock();
        base = packet.acknum + 1;
        // if(base == nextSeq){
            simulation->stop_timer(A);
            ertta *= base;
            ertta += (recTa - sendTa);
            ertta /= base;
        // }
        // else{
        //     simulation->start_timer(A, ertta*4);
        // }

        // if(base == nextSeq){
            // simulation->stop_timer(A);
        // }
        // if(expectedSeq == nextSeq && uninitialized.size() < 20){
        //     msg newMsg = msg();
        //     // int cs = 0;
        //     for(int i = 0; i < 20; i++){
        //         newMsg.data[i] = packet.payload[i];
        //         // cs += int(packet.payload[i]);
        //     }
        //     simulation->deliver_data(A, newMsg);
        //     expectedSeq ++;
        //     // pkt newPkt = pkt();
        //     // newPkt.acknum = ack;
        //     // newPkt.seqnum = expectedSeq;
        //     // newPkt.checksum = cs;
        //     // simulation->udt_send(A, newPkt);
        //     uninitialized.erase(uninitialized.begin() + packet.seqnum);
        // }
    }
    // else{
    //     std::cout << "CHECKSUM IN RECV A FAILED" << std::endl;
    // }
}


// ***************************************************************************
// * Called from layer 5, passed the data to be sent to other side
// ***************************************************************************
bool rdt_sendB(struct msg message) {
    INFO<< "RDT_SEND_B: Layer 4 on side B has received a message from the application that should be sent to side A: "
              << message << ENDL;

    bool accepted = true;
    // if(nextSeq - base >= 10){
    //     return false;
    // }



    // pkt newPkt = pkt();
    // newPkt.acknum = base - 1;
    // newPkt.seqnum = nextSeq;
    // // newPkt.checksum = 0;
    // newPkt.seqnum = nextSeq;
    // pkt newPkt;
    // int cs = 0;
    // for(int i = 0; i < 20; i++){
    //     newPkt.payload[i] = message.data[i];
    //     cs += int(message.data[i]);
    // }
    // newPkt.checksum = cs;
    // newPkt.seqnum = 0;
    // newPkt.acknum = expectedSeq;
    
    // newPkt.checksum = atoi(newPkt.payload);
    // std::cout << newPkt.checksum << std::endl;
    // std::cout << newPkt.payload << std::endl;
    // std::cout << atoi(newPkt) << std::endl;
    //  = data;
    // uninitialized.push_back(newPkt);
    // simulation->udt_send(B, newPkt);
    // simulation->start_timer(B, ertta*4);
    // sendTa = simulation->getSimulatorClock();
    // nextSeq ++;
    //  if (nextSeq < base + 20) {
    //     // if(nextSeq < base + 20){
    //     //     std::cout << "SEQ < BASE + 20" << std::endl;
    //     // }
    //     pkt newPkt = pkt();
    //     newPkt.acknum = base - 1;
    //     newPkt.seqnum = nextSeq;
    //     // newPkt.checksum = 0;
    //     newPkt.seqnum = nextSeq;
    //     int cs = 0;
    //     for(int i = 0; i < 20; i++){
    //         newPkt.payload[i] = message.data[i];
    //         cs += int(message.data[i]);
    //     }
    //     newPkt.checksum = cs;
    //     // newPkt.checksum = atoi(newPkt.payload);
    //     // std::cout << newPkt.checksum << std::endl;
    //     // std::cout << newPkt.payload << std::endl;
    //     // std::cout << atoi(newPkt) << std::endl;
    //     //  = data;
    //     uninitialized.push_back(newPkt);
    //     simulation->udt_send(B, newPkt);
    //     simulation->start_timer(B, ertt*4);
    //     sendTa = simulation->getSimulatorClock();
    //     nextSeq ++;
    // }
    // else {
    //     accepted = false;
    //     //print out error, try again later?
    // }
    // if (nextSeq < base + 20) {
    //     // if(nextSeq < base + 20){
    //     //     std::cout << "SEQ < BASE + 20" << std::endl;
    //     // }
    //     pkt newPkt = pkt();
    //     newPkt.acknum = ack;
    //     ack += 20;
    //     // newPkt.checksum = 0;
    //     newPkt.seqnum = nextSeq;
    //     for(int i = 0; i < 20; i++){
    //         newPkt.payload[i] = message.data[i];
    //     }
    //     // newPkt.checksum = atoi(newPkt.payload);
    //     int cs = 0;
    //     for(int i = 0; i < 20; i++){
    //         newPkt.payload[i] = message.data[i];
    //         cs += int(message.data[i]);
    //     }
    //     newPkt.checksum = cs;

    //     uninitialized.push_back(newPkt);
    //     simulation->udt_send(B, newPkt);
    //     if(base == nextSeq){
    //         // simulation->start_timer(B, 1000.0);
    //     }
    //     nextSeq ++;
    // }
    // else {
    //     accepted = false;
    // }

    return (accepted);
}


// ***************************************************************************
// // called from layer 3, when a packet arrives for layer 4 on side B 
// ***************************************************************************
void rdt_rcvB(struct pkt packet) {
    INFO << "RTD_RCV_B: Layer 4 on side B has received a packet from layer 3 sent over the network from side A:"
         << packet << ENDL;

        // recTa = simulation->getSimulatorClock();
        // simulation->stop_timer(B);
        // base = packet.acknum + 1;
        // ertta *= base;
        // ertta += recTa - sendTa;
        // ertta /= base;
    
    int calcChecksum = 0;
    for(int i = 0; i < 20; i++){
        calcChecksum += int(packet.payload[i]);
    }
    bool noC = false;
    bool order = false;
    pkt newPkt;
    newPkt.acknum = packet.acknum;
    if(packet.checksum == calcChecksum){
        noC = true;
        newPkt.checksum = calcChecksum;
    }
    else{
        newPkt.acknum = base - 1;
    }

    if(packet.seqnum == expectedSeq){
        order = true;
    }


    msg newMsg;
    if(noC && order){
        // newPkt.seqnum = expectedSeq;
        newPkt.acknum = packet.seqnum;
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
    }
    simulation->udt_send(B, newPkt);


    // simulation->start_timer(B, ertta*4);
    // uninitialized.push_back(newPkt);
    // sendTa = simulation->getSimulatorClock();
    // nextSeq ++;


    // simulation->udt_send(B, newPkt);


        // base = packet.acknum + 1;
        // if(base == nextSeq){
        //     // simulation->stop_timer(B);
        // }
        // else{
        //     // simulation->start_timer(B, 1000.0);
    //     // }
    //     if(expectedSeq == nextSeq && uninitialized.size() < 20){
    //         msg newMsg = msg();
    //         // int cs = 0;
    //         for(int i = 0; i < 20; i++){
    //             newMsg.data[i] = packet.payload[i];
    //             // cs += int(packet.payload[i]);
    //         }
    //         // pkt newPkt = pkt();
    //         // newPkt.acknum = ack;
    //         // newPkt.seqnum = expectedSeq;
    //         // newPkt.checksum = cs;
    //         // simulation->udt_send(B, newPkt);
    //         simulation->deliver_data(B, newMsg);
    //         expectedSeq ++;
    //     }
    // }
    // else{
    //     std::cout << "CHECKSUM in RECV B FAILED" << std::endl;
    // }
}

// ***************************************************************************
// * Called when A's timer goes off 
// ***************************************************************************
void A_timeout() {
    INFO << "A_TIMEOUT: Side A's timer has gone off." << ENDL;
    simulation->stop_timer(A);
    for(int i = base; i <= nextSeq - 1; i++){
        simulation->start_timer(A, ertta*4);
        sendTa = simulation->getSimulatorClock();
        simulation->udt_send(A, uninitialized[i]);
    }
}

// ***************************************************************************
// * Called when B's timer goes off 
// ***************************************************************************
void B_timeout() {
    INFO << "B_TIMEOUT: Side B's timer has gone off." << ENDL;
    // simulation->stop_timer(B);
    // for(int i = base; i < nextSeq - 1; i++){
    //     simulation->start_timer(B, ertta*4);
    //     sendTa = simulation->getSimulatorClock();
    //     simulation->udt_send(B, uninitialized[i]);
    // }
    // simulation->start_timer(B, 1000.0);
    // for(int i = base; i <= nextSeq - 1; i++){
    //     simulation->udt_send(B, uninitialized.at(i));
    // }
}

// bool checksum(int, int) {
//     return checkSumA == checkSumB;
// }

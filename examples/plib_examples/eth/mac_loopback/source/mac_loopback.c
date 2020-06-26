/*********************************************************************
 *
 *                  ETH MAC loopback example file
 *
 *********************************************************************
 * FileName:        mac_loopback.c
 * Dependencies:    plib.h
 *
 * Processor:       PIC32MX7xx
 *
 * Compiler:        MPLAB XC32 v1 or higher
 *                  MPLAB IDE v8 or higher
 * Company:         Microchip Technology Inc.
 *
 * Software License Agreement
 *
 * The software supplied herewith by Microchip Technology Incorporated
 * (the “Company”) for its PIC Microcontroller is intended
 * and supplied to you, the Company’s customer, for use solely and
 * exclusively on Microchip PIC Microcontroller products.
 * The software is owned by the Company and/or its supplier, and is
 * protected under applicable copyright laws. All rights are reserved.
 * Any use in violation of the foregoing restrictions may subject the
 * user to criminal sanctions under applicable laws, as well as to
 * civil liability for the breach of the terms and conditions of this
 * license.
 *
 * THIS SOFTWARE IS PROVIDED IN AN “AS IS” CONDITION. NO WARRANTIES,
 * WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED
 * TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. THE COMPANY SHALL NOT,
 * IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL OR
 * CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 *
 *
 ********************************************************************/

#include <stdlib.h>
#include <string.h>

#include <plib.h>

#ifndef _ETH
    #error "This example needs a PIC32MX processor with Ethernet controller present. Aborting build!"
#endif  // _ETH


// prototypes
int     MacDoLoopbackExample(void);


// Configuration Bit settings
// SYSCLK = 80 MHz (8MHz Crystal/ FPLLIDIV * FPLLMUL / FPLLODIV)
// PBCLK = 40 MHz
// Primary Osc w/PLL (XT+,HS+,EC+PLL)
// WDT OFF
// Other options are don't care
//
#pragma config FPLLMUL = MUL_20, FPLLIDIV = DIV_2, FPLLODIV = DIV_1, FWDTEN = OFF
#pragma config POSCMOD = HS, FNOSC = PRIPLL, FPBDIV = DIV_2

#define SYS_FREQ (80000000L)

/*********************************************************************
 * Function:        int main(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          0 if MAC loop back test failed,
 *                  1 if the MAC loop back test suceeded
 *
 * Side Effects:    None
 *
 * Overview:        Examples for the usage of the Ethernet Peripheral Lib
 *
 * Note:            None.
 ********************************************************************/
int main(void)
{

    // Configure the device for maximum performance but do not change the PBDIV
    // Given the options, this function will change the flash wait states, RAM
    // wait state and enable prefetch cache but will not change the PBDIV.
    // The PBDIV value is already set via the pragma FPBDIV option above..
    SYSTEMConfig(SYS_FREQ, SYS_CFG_WAIT_STATES | SYS_CFG_PCACHE);

    srand(ReadCoreTimer());     // init the pseudo random generator

    // perform the MAC loopback using the ETH controller
    if(!MacDoLoopbackExample())
    {
        return 0;       // if our example failed
    }


    return 1;
}

// local definitions
//
#define MAC_MAX_PKT_SIZE    1500        // max packet handled

#define MAC_MAX_TX_DESCRIPTORS  10      // default TX descriptors

#define MAC_MAX_RX_DESCRIPTORS  10      // default RX descriptors

#define MAC_RX_MIN_BUFF_SIZE    (MAC_MAX_PKT_SIZE/8)        // RX buffer min size!


// local data
//
//


static unsigned char*   txBuffers[MAC_MAX_TX_DESCRIPTORS] = {0};            // tx buffers to transmit data

static sEthPktDcpt      rxPktDcpt[MAC_MAX_PKT_SIZE/MAC_RX_MIN_BUFF_SIZE+2]; // rx packet descriptor
static unsigned char*   rxBuffers[MAC_MAX_RX_DESCRIPTORS] = {0};            // rx buffers for incoming data



/*********************************************************************
 * Function:        int MacDoLoopbackExample(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          0 if some the MAC loopback test failed
 *                  1 if the MAC test suceeded
 *
 * Side Effects:    None
 *
 * Overview:        Example for initializing the MAC and using the ETHC API
 * 
 * Note:            This example uses just the Eth Controller and the MAC built in the PIC32MX processsor.
 *                  No connection with an external PHY is assumed or needed by this example.
 ********************************************************************/
int MacDoLoopbackExample(void)
{
    // test parameters. adjust at will
    unsigned int    txDcptNo=MAC_MAX_TX_DESCRIPTORS;    // the number of the TX descriptors; < MAC_MAX_TX_DESCRIPTORS
    unsigned int    rxDcptNo=MAC_MAX_RX_DESCRIPTORS;    // the number of the RX descriptors; < MAC_MAX_RX_DESCRIPTORS
    unsigned int    rxBuffSize=768;                     // the size of the RX buffer.; MAC_RX_MIN_BUFF_SIZE < rxBuffSize < MAC_MAX_PKT_SIZE
                                //
    int     pktSize=512;            // size of the packet to transmit; 
    int     pktBurst=8;             // number of packets to transmit at once; pktBurst < MAC_MAX_TX_DESCRIPTORS  
                                    // pktBurst*pktSize < rxDcptNo*rxBuffSize; otherwise the ETH controller will report RX overflow!

    // test auto variables  
    

    sEthPktDcpt*    pRxPkt;
    int             ix, jx;
    eEthRes         ethRes;
    int             success;            // overall result

    /****************************************
     * 1. first we initialize the MAC
     * ******************************************************/

    
    memset(rxBuffers, 0, sizeof(rxBuffers));    // clear buffer pointers
    memset(txBuffers, 0, sizeof(txBuffers));    // clear buffer pointers
    // set the receiving packet descriptors
    // this is needed when a packet can span multiple buffers
    // i.e. the size of a receive buffer provided to the Ethernet RX engine
    // is less than the size of the packet received
    memset(rxPktDcpt, 0, sizeof(rxPktDcpt));    // clear packet descriptors
    for(ix=0, pRxPkt=rxPktDcpt; ix<sizeof(rxPktDcpt)/sizeof(*rxPktDcpt); ix++)
    {
        pRxPkt->next=pRxPkt+1;
        pRxPkt=pRxPkt->next;
    }
    rxPktDcpt[ix-1].next=0; // end properly

    success=0;

    while(1)
    {
        eEthOpenFlags       oFlags;
        eEthMacPauseType    pauseType;


        oFlags= ETH_OPEN_FDUPLEX|ETH_OPEN_100|ETH_OPEN_HUGE_PKTS|ETH_OPEN_MAC_LOOPBACK;     // enable loopback at the MAC level
        pauseType=ETH_MAC_PAUSE_ALL;    // we can use both TX and RX pause type for Full Duplex
        
        // start the initialization sequence
        EthInit();

        // open and configure the MAC
        // since we're not connected to an external PHY
        // there's no negotiation going on and we know all our opening flags
        EthMACOpen(oFlags, pauseType);

        // set the RX packet filtering
        EthRxFiltersClr(ETH_FILT_ALL_FILTERS);
        EthRxFiltersSet(ETH_FILT_CRC_ERR_REJECT|ETH_FILT_RUNT_REJECT|ETH_FILT_ME_UCAST_ACCEPT|ETH_FILT_MCAST_ACCEPT|ETH_FILT_BCAST_ACCEPT|ETH_FILT_NOTME_UCAST_ACCEPT);

        // add the descriptors we use for transmission
                
        if(EthDescriptorsAdd(txDcptNo, ETH_DCPT_TYPE_TX, 0)!=txDcptNo)
        {
            break;
        }

        if(EthDescriptorsAdd(rxDcptNo, ETH_DCPT_TYPE_RX, 0)!=rxDcptNo)
        {
            break;
        }
        EthRxSetBufferSize(rxBuffSize);
    
        // allocate memory for the RX buffers
        for(ix=0; ix<rxDcptNo; ix++)
        {
            if(!(rxBuffers[ix]=(unsigned char*)malloc(rxBuffSize)))
            {
                break;
            }
        }

        if(ix!=rxDcptNo)
        {   // some allocation failed
            break;
        }
        
        // set these buffers as permanent receive buffers
        ethRes=EthRxBuffersAppend(rxBuffers, rxDcptNo, ETH_BUFF_FLAG_RX_STICKY);
        if(ethRes!=ETH_RES_OK)
        {
            break;
        }

        // allocate memory for the TX buffers
        for(ix=0; ix<txDcptNo; ix++)
        {
            if(!(txBuffers[ix]=(unsigned char*)malloc(pktSize)))
            {
                break;
            }
        }

        if(ix!=txDcptNo)
        {   // some allocation failed
            break;
        }

        // initialization done
        success=1;
        break;
    }

    /****************************************
     * 2. now transmit and receive the packets
     *    first we transmit the burst
     *    after that we look at the packets we received
     *    Note that for this example we don't use a properly constructed Ethernet frame
     *    The standard Ethernet header: destination address, source address and frame type/length is missing.
     *    A real life application transmitting data to a remote party will have to add these required fields.   
     * ******************************************************/

    while(success)
    {
        success=0;
        
        // prepare the buffers to transmit our packets
        for(ix=0; ix<pktBurst; ix++)
        {
            unsigned char* pB=txBuffers[ix];
            for(jx=0; jx<pktSize; jx++)
            {
                *pB++=(unsigned char)rand();    // fill with some random data
            }
        }
        
        // now transmit the packets : either all the txBuffers in one packet or each txBuffer as a different packet
        // for now we'll transmit each txBuffer in a different packet
        for(ix=0; ix<pktBurst; ix++)
        {
            sEthPktDcpt txDcpt;         // construct the list of buffers (packet descriptor) we use for transmit

            txDcpt.next=0;              // just one buffer per packet
            txDcpt.nBytes=pktSize;      // all packets in our example are the same size;
                                        // this shouldn't be the case in a real life application
            txDcpt.pBuff=txBuffers[ix];

            ethRes=EthTxSendPacket(&txDcpt);    // transmit the packet
                                                // alternatively we could have used EthTxSendBuffer(txBuffers[ix], pktSize);
                                                // since the packet contains just one buffer
            if(ethRes!=ETH_RES_OK)
            {
                break;      // transmission failed
            }
        }
            
        if(ix!=pktBurst)
        {
            break;      // could not transmit all the packets
        }

        // try to get the pacets received
        for(ix=0; ix<pktBurst; ix++)
        {
            int         buffsPerRxPkt;
            const sEthRxPktStat*   pRxPktStat;
            
            do
            {
                ethRes=EthRxGetPacket(rxPktDcpt, &buffsPerRxPkt, &pRxPktStat);
            }while (ethRes==ETH_RES_PACKET_QUEUED);     // wait to receive a packet
            // we could have used:
            // unsigned char* pBuff;
            // EthRxGetBuffer(&pBuff, &pRxPktStat);
            // and get the address of the received packet in pBuff
            // if we are sure the size of the RX packet does not exceed the RX buffer provided
            
            
            if(ethRes!=ETH_RES_OK)
            {
                break;      // RX failed
            }
            
            // we can do some basic checks
            if(pRxPktStat->rxOk==0 || pRxPktStat->runtPkt || pRxPktStat->crcError || buffsPerRxPkt!=1)
            {
                break;      // invalid packet
            }
            
            // check the size of the packet. The RX packet has extra 4 FCS (CRC) bytes
            if(pRxPktStat->rxBytes!=(pktSize+4))
            {
                break;      // invalid packet size
            }
                
            // packet is ok, let's compare the data
            if(memcmp(rxPktDcpt[0].pBuff, txBuffers[ix], pktSize))
            {
                break;      // no match, failed;
            }
            
            // acknowledge the packets we're just done with
            ethRes=EthTxAckBuffer(txBuffers[ix], 0);    // TX one
            ethRes|=EthRxAckPacket(rxPktDcpt, 0);   // RX one
            if(ethRes!=ETH_RES_OK)
            {
                break;      // fail to acknowledge
            }
        }
        

        if(ix==pktBurst)
        {
            success=1;  // received all packets ok
        }
                
        break;      // done
    }




    // test done; clean-up
    //
    // acknowledge all TX/RX packets just in case we ended up with an error
    EthTxAckPacket(0, 0); 
    EthRxAckPacket(0, 0);

    EthClose(0);

    EthDescriptorsCleanUp(ETH_DCPT_TYPE_ALL, 0);
    

    for(ix=0; ix<rxDcptNo; ix++)
    {
        free(rxBuffers[ix]);
    }
    for(ix=0; ix<txDcptNo; ix++)
    {
        free(txBuffers[ix]);
    }

    
    return success;

}





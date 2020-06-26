/*
 * File:   enc28j60.c
 * Ethernet Breakout board for ENC28J60 Chip from Microchip
 * Author: dsolano
 * PIC32MX360F512L
 * Created on August 31, 2014, 12:27 PM
 */

#include "enc28j60.h"
#include <utilities.h>
#include <plib.h>
#include <define_pins.h>


DEFINE_ANALOG_DIGITAL_PIN(ETH_CS,AN2_RB2)   // ETH CS PIN defined as digital
DEFINE_ANALOG_DIGITAL_PIN(ETH_LINK,AN0_RB0) // LINK up PIN
DEFINE_PIN(ETH_CS, B, 2)                    // ETH CS PIN defined as GPIO functions
DEFINE_PIN(ETH_LINK, A, 0)                  // LIghtup a LED for LINKUP

#define ETH_SPI					SPI_CHANNEL1

/*
 * Initialization for SSP device and CE and CSN pins
 */
static void init_eth_pins(void)
{
    ETH_CS_AN2_RB2_DIGITAL_PIN();
    ETH_CS_OUTPUT();
    ETH_LINK_AN0_RB0_DIGITAL_PIN();
    ETH_LINK_OUTPUT();
    pic32_Spi_Init(ETH_SPI,25000000);
}

static uint8_t Enc28j60Bank;
static uint16_t NextPacketPtr;

uint8_t enc28j60ReadOp(uint8_t op, uint8_t address)
{
    uint8_t received_byte;
    ETH_CS_LOW();
    // issue read command
    pic32_SpiXfer(ETH_SPI, op | (address & ADDR_MASK));
    // read data

    received_byte = pic32_SpiXfer(ETH_SPI,0xff);
    // do dummy read if needed (for mac and mii, see datasheet page 29)
    if (address & 0x80)
    {
        received_byte = pic32_SpiXfer(ETH_SPI,0xff);
    }
    // release CS
    ETH_CS_HIGH();
    return (received_byte);
}

void enc28j60WriteOp(uint8_t op, uint8_t address, uint8_t data)
{
    ETH_CS_LOW();
    // issue write command
    pic32_SpiXfer(ETH_SPI, op | (address & ADDR_MASK));
    // write data
    pic32_SpiXfer(ETH_SPI, data);
    ETH_CS_HIGH();
}

void enc28j60ReadBuffer(uint16_t len, uint8_t* data)
{
    ETH_CS_LOW();
    // issue read command
    pic32_SpiXfer(ETH_SPI, ENC28J60_READ_BUF_MEM);
    while (len)
    {
        len--;
        // read data
        *data = pic32_SpiXfer(ETH_SPI,0xff);
        data++;
    }
    *data = '\0';
    ETH_CS_HIGH();
}

void enc28j60WriteBuffer(uint16_t len, uint8_t* data)
{
    ETH_CS_LOW();
    // issue write command
    pic32_SpiXfer(ETH_SPI, ENC28J60_WRITE_BUF_MEM);
    while (len)
    {
        len--;
        // write data
        pic32_SpiXfer(ETH_SPI, *data);
        data++;
    }
    ETH_CS_HIGH();
}

void enc28j60SetBank(uint8_t address)
{
    // set the bank (if needed)
    if ((address & BANK_MASK) != Enc28j60Bank)
    {
        // set the bank
        enc28j60WriteOp(ENC28J60_BIT_FIELD_CLR, ECON1,
                        (ECON1_BSEL1 | ECON1_BSEL0));
        enc28j60WriteOp(ENC28J60_BIT_FIELD_SET, ECON1,
                        (address & BANK_MASK) >> 5);
        Enc28j60Bank = (address & BANK_MASK);
    }
}

uint8_t enc28j60Read(uint8_t address)
{
    // set the bank
    enc28j60SetBank(address);
    // do the read
    return enc28j60ReadOp(ENC28J60_READ_CTRL_REG, address);
}

void enc28j60Write(uint8_t address, uint8_t data)
{
    // set the bank
    enc28j60SetBank(address);
    // do the write
    enc28j60WriteOp(ENC28J60_WRITE_CTRL_REG, address, data);
}

void enc28j60PhyWrite(uint8_t address, uint16_t data)
{
    // set the PHY register address
    enc28j60Write(MIREGADR, address);
    // write the PHY data
    enc28j60Write(MIWRL, data);
    enc28j60Write(MIWRH, data >> 8);
    // wait until the PHY write completes
    while (enc28j60Read(MISTAT) & MISTAT_BUSY)
    {
        ShortDelay(15);
    }
}


// read upper 8 bits of PHY

uint16_t enc28j60PhyRead(uint8_t address)
{
    uint16_t result = 0;
    // Set the right address and start the register read operation
    enc28j60Write(MIREGADR, address);
    enc28j60Write(MICMD, MICMD_MIIRD);
    //_delay_loop_1(40); // 10us
    ShortDelay(15);
    // wait until the PHY read completes
    while (enc28j60Read(MISTAT) & MISTAT_BUSY)
        ;
    // reset reading bit
    enc28j60Write(MICMD, 0x00);
    result = enc28j60Read(MIRDH);
    result <<= 8;
    result |= enc28j60Read(MIRDL);
    return (result);
}

void enc28j60clkout(uint8_t clk)
{
    //setup clkout: 2 is 12.5MHz:
    enc28j60Write(ECOCON, clk & 0x7);
}

void enc28j60Init(void)
{
    // initialize I/O
    // ss as output:
    init_eth_pins();
    ETH_CS_HIGH(); // ss=0

    PHY_ID_Typedef oui;
    MACADDR_Typedef mac;
    enc28j60GetOUI(&oui);
    mac.mac_addr[5] = (uint8_t) ((oui.oui & 0x00FF0000) >> 16); // Byte 2 OUI
    mac.mac_addr[4] = (uint8_t) ((oui.oui & 0x0000FF00) >> 8); // Byte 1 OUI
    mac.mac_addr[3] = (uint8_t) (oui.oui & 0x000000FF); // Byte 0 OUI
    mac.mac_addr[2] = 0x14; // Lower 3 bytes for Mac phy addr
    mac.mac_addr[1] = 0x12; // ....
    mac.mac_addr[0] = 0x66; // ....
    // perform system reset
    enc28j60WriteOp(ENC28J60_SOFT_RESET, 0, ENC28J60_SOFT_RESET);
    ShortDelay(delay_10ms + 5);
    // check CLKRDY bit to see if reset is complete
    // The CLKRDY does not work. See Rev. B4 Silicon Errata point. Just wait.
    //while(!(enc28j60Read(ESTAT) & ESTAT_CLKRDY));
    // do bank 0 stuff
    // initialize receive buffer
    // 16-bit transfers, must write low byte first
    // set receive buffer start address
    NextPacketPtr = RXSTART_INIT;
    // Rx start
    enc28j60Write(ERXSTL, RXSTART_INIT & 0xFF);
    enc28j60Write(ERXSTH, RXSTART_INIT >> 8);
    // set receive pointer address
    enc28j60Write(ERXRDPTL, RXSTART_INIT & 0xFF);
    enc28j60Write(ERXRDPTH, RXSTART_INIT >> 8);
    // RX end
    enc28j60Write(ERXNDL, RXSTOP_INIT & 0xFF);
    enc28j60Write(ERXNDH, RXSTOP_INIT >> 8);
    // TX start
    enc28j60Write(ETXSTL, TXSTART_INIT & 0xFF);
    enc28j60Write(ETXSTH, TXSTART_INIT >> 8);
    // TX end
    enc28j60Write(ETXNDL, TXSTOP_INIT & 0xFF);
    enc28j60Write(ETXNDH, TXSTOP_INIT >> 8);
    // do bank 1 stuff, packet filter:
    // For broadcast packets we allow only ARP packtets
    // All other packets should be unicast only for our mac (MAADR)
    //
    // The pattern to match on is therefore
    // Type ETH.DST
    // ARP BROADCAST
    // 06 08 -- ff ff ff ff ff ff -> ip checksum for theses bytes=f7f9
    // in binary these poitions are:11 0000 0011 1111
    // This is hex 303F->EPMM0=0x3f,EPMM1=0x30
    //enc28j60Write(ERXFCON, ERXFCON_UCEN|ERXFCON_CRCEN|ERXFCON_PMEN);
    enc28j60Write(ERXFCON, ERXFCON_CRCEN);
    enc28j60Write(EPMM0, 0x3f);
    enc28j60Write(EPMM1, 0x30);
    enc28j60Write(EPMCSL, 0xf9);
    enc28j60Write(EPMCSH, 0xf7);
    //
    //
    // do bank 2 stuff
    // enable MAC receive
    enc28j60Write(MACON1, MACON1_MARXEN | MACON1_TXPAUS | MACON1_RXPAUS);
    // bring MAC out of reset
    enc28j60Write(MACON2, 0x00);
    // enable automatic padding to 60bytes and CRC operations
    enc28j60WriteOp(ENC28J60_BIT_FIELD_SET, MACON3,
                    MACON3_PADCFG0 | MACON3_TXCRCEN | MACON3_FRMLNEN);
    // set inter-frame gap (non-back-to-back)
    enc28j60Write(MAIPGL, 0x12);
    enc28j60Write(MAIPGH, 0x0C);
    // set inter-frame gap (back-to-back)
    enc28j60Write(MABBIPG, 0x12);
    // Set the maximum packet size which the controller will accept
    // Do not send packets longer than MAX_FRAMELEN:
    enc28j60Write(MAMXFLL, MAX_FRAMELEN & 0xFF);
    enc28j60Write(MAMXFLH, MAX_FRAMELEN >> 8);
    // do bank 3 stuff
    // write MAC address
    // NOTE: MAC address in ENC28J60 is byte-backward
    enc28j60Write(MAADR5, mac.mac_addr[5]);
    enc28j60Write(MAADR4, mac.mac_addr[4]);
    enc28j60Write(MAADR3, mac.mac_addr[3]);
    enc28j60Write(MAADR2, mac.mac_addr[2]);
    enc28j60Write(MAADR1, mac.mac_addr[1]);
    enc28j60Write(MAADR0, mac.mac_addr[0]);
    // no loopback of transmitted frames
    enc28j60PhyWrite(PHCON2, PHCON2_HDLDIS);
    // switch to bank 0
    enc28j60SetBank(ECON1);
    // enable interrutps
    enc28j60WriteOp(ENC28J60_BIT_FIELD_SET, EIE, 0);
    //enc28j60WriteOp(ENC28J60_BIT_FIELD_SET, EIE, EIE_INTIE|EIE_PKTIE);
    // enable packet reception
    enc28j60WriteOp(ENC28J60_BIT_FIELD_SET, ECON1, ECON1_RXEN);
}


// read the revision of the chip:

uint8_t enc28j60getrev(void)
{
    return (enc28j60Read(EREVID));
}

void enc28j60GetOUI(PHY_ID_Typedef *phy)
{
    uint16_t phid1 = 0;
    uint16_t phid2 = 0;

    phid1 = enc28j60PhyRead(PHID1);
    phid2 = enc28j60PhyRead(PHID2);

    phy->part_no = ((phid2 & 0x03f0) >> 4); // PHID2 bits 9:4
    phy->rev_lev = (phid2 & 0x000F); // PHID2 bits 3:0
    phy->rev_id = enc28j60getrev();
    phy->oui = ((phid2 & 0x0000FC00) << 6); // PHID2 reg bits 15:10
    phy->oui |= phid1; // Full PHIID1 reg

}

void enc28j60GetMacAddress(MACADDR_Typedef* mac)
{
    mac->mac_addr[5] = enc28j60Read(MAADR5);
    mac->mac_addr[4] = enc28j60Read(MAADR4);
    mac->mac_addr[3] = enc28j60Read(MAADR3);
    mac->mac_addr[2] = enc28j60Read(MAADR2);
    mac->mac_addr[1] = enc28j60Read(MAADR1);
    mac->mac_addr[0] = enc28j60Read(MAADR0);
}

// link status

BOOL enc28j60linkup(void)
{
    if (enc28j60PhyRead(PHSTAT2) & PHSTAT2_LSTAT)
    {
        ETH_LINK_ASSERT();
        return TRUE;
    }
    else
    {
        ETH_LINK_DEASSERT();
        return FALSE;
    }
}

// detects if the local eth is obstructing the network communication

BOOL enc28j60IsJabbering(void)
{
    return (enc28j60PhyRead(PHSTAT1) & PHSTAT1_JBSTAT) ? TRUE : FALSE;
}

void enc28j60PacketSend(uint16_t len, uint8_t* packet)
{
    // Check no transmit in progress
    while (enc28j60ReadOp(ENC28J60_READ_CTRL_REG, ECON1) & ECON1_TXRTS)
    {
        // Reset the transmit logic problem. See Rev. B4 Silicon Errata point 12.
        //if( (enc28j60Read(EIR) & EIR_TXERIF) )
        {
            enc28j60WriteOp(ENC28J60_BIT_FIELD_SET, ECON1, ECON1_TXRST);
            enc28j60WriteOp(ENC28J60_BIT_FIELD_CLR, ECON1, ECON1_TXRST);
        }
    }
    // Set the write pointer to start of transmit buffer area
    enc28j60Write(EWRPTL, TXSTART_INIT & 0xFF);
    enc28j60Write(EWRPTH, TXSTART_INIT >> 8);
    // Set the TXND pointer to correspond to the packet size given
    enc28j60Write(ETXNDL, (TXSTART_INIT + len) & 0xFF);
    enc28j60Write(ETXNDH, (TXSTART_INIT + len) >> 8);
    // write per-packet control byte (0x00 means use macon3 settings)
    enc28j60WriteOp(ENC28J60_WRITE_BUF_MEM, 0, 0x00);
    // copy the packet into the transmit buffer
    enc28j60WriteBuffer(len, packet);
    // send the contents of the transmit buffer onto the network
    enc28j60WriteOp(ENC28J60_BIT_FIELD_SET, ECON1, ECON1_TXRTS);
}


// just probe if there might be a packet

BOOL enc28j60hasRxPkt(void)
{
    if (enc28j60Read(EPKTCNT) == 0)
    {
        return (FALSE);
    }
    return (TRUE);
}


// Gets a packet from the network receive buffer, if one is available.
// The packet will by headed by an ethernet header.
// maxlen The maximum acceptable length of a retrieved packet.
// packet Pointer where packet data should be stored.
// Returns: Packet length in bytes if a packet was retrieved, zero otherwise.

uint16_t enc28j60PacketReceive(uint16_t maxlen, uint8_t* packet)
{
    uint16_t rxstat;
    uint16_t len;
    // check if a packet has been received and buffered
    //if( !(enc28j60Read(EIR) & EIR_PKTIF) ){
    // The above does not work. See Rev. B4 Silicon Errata point 6.
    if (enc28j60Read(EPKTCNT) == 0)
    {
        return (0);
    }
    // Set the read pointer to the start of the received packet
    enc28j60Write(ERDPTL, (NextPacketPtr & 0xFF));
    enc28j60Write(ERDPTH, (NextPacketPtr) >> 8);
    // read the next packet pointer
    NextPacketPtr = enc28j60ReadOp(ENC28J60_READ_BUF_MEM, 0);
    NextPacketPtr |= enc28j60ReadOp(ENC28J60_READ_BUF_MEM, 0) << 8;
    // read the packet length (see datasheet page 43)
    len = enc28j60ReadOp(ENC28J60_READ_BUF_MEM, 0);
    len |= enc28j60ReadOp(ENC28J60_READ_BUF_MEM, 0) << 8;
    len -= 4; //remove the CRC count
    // read the receive status (see datasheet page 43)
    rxstat = enc28j60ReadOp(ENC28J60_READ_BUF_MEM, 0);
    rxstat |= ((uint16_t) enc28j60ReadOp(ENC28J60_READ_BUF_MEM, 0)) << 8;
    // limit retrieve length
    if (len > maxlen - 1)
    {
        len = maxlen - 1;
    }
    // check CRC and symbol errors (see datasheet page 44, table 7-3):
    // The ERXFCON.CRCEN is set by default. Normally we should not
    // need to check this.
    if ((rxstat & 0x80) == 0)
    {
        // invalid
        len = 0;
    }
    else
    {
        // copy the packet from the receive buffer
        enc28j60ReadBuffer(len, packet);
    }
    // Move the RX read pointer to the start of the next received packet
    // This frees the memory we just read out
    enc28j60Write(ERXRDPTL, (NextPacketPtr & 0xFF));
    enc28j60Write(ERXRDPTH, (NextPacketPtr) >> 8);
    // Move the RX read pointer to the start of the next received packet
    // This frees the memory we just read out.
    // However, compensate for the errata point 13, rev B4: enver write an even address!
    if ((NextPacketPtr - 1 < RXSTART_INIT) || (NextPacketPtr - 1 > RXSTOP_INIT))
    {
        enc28j60Write(ERXRDPTL, (RXSTOP_INIT) & 0xFF);
        enc28j60Write(ERXRDPTH, (RXSTOP_INIT) >> 8);
    }
    else
    {
        enc28j60Write(ERXRDPTL, (NextPacketPtr - 1) & 0xFF);
        enc28j60Write(ERXRDPTH, (NextPacketPtr - 1) >> 8);
    }
    // decrement the packet counter indicate we are done with this packet
    enc28j60WriteOp(ENC28J60_BIT_FIELD_SET, ECON2, ECON2_PKTDEC);
    return (len);
}

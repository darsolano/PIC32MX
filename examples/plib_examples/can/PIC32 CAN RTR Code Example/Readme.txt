PIC32 CAN RTR Code Example

What you need to run this code example:

1. Explorer 16 Development Board and 9V power supply
2. ECAN Pictail Plus board
3. CAN node connection cable.
4. PIC32 Plug In Module for Explorer 16 Board.
5. REAL ICE or ICD 3 or ICD 2 (programmer)

Programming the PIC32:

1. Open MPLAB IDE and select File->Import
2. Select  PIC32 CAN RTR Code Example.hex file contained in the obj
directory.
3. 	Mount the PIC32 PIM on the Explorer 16 board and insert the
ECAN PICtail Plus board in the PICtail slot.
4. Connect CAN node connection cable between CAN2 and CAN1 sockets
on the ECAN Pictail Plus Board.
5. Power on the board. Connect the programmer to the board. Choose
this programmer in MPLAB IDE and then select Programmer->Program.
6. When the code is running, LED5  on the explorer 16 board
will toggle every one second. 

About the Code Example:

This code example demonstrates the use of the PIC32MX CAN Peripheral 
Library and PIC32MX CAN Module to send and receive Standard ID Remote 
Transmit Request (RTR) messages. The RTR mechanism allows a node on 
a CAN bus to request a transmission from another node on the bus. The 
PIC32MX CAN module features auto reply to RTR messages wherein a TX 
channel can be configured to automatically transmit messages when a 
RTR message is received. 

CAN1 sends a RTR message to CAN2. CAN2 is configured to reply to the 
RTR message. CAN2 replies with with the status of LED5 . CAN1 then 
uses this reply to switch LED5, ON or OFF. CAN2 will toggle LED5 
status every second. CAN1 queries (sends an RTR message) to CAN2 
every 10 milli- second. 

CAN1 uses Channel 0 (with 8 message buffers) for transmit and 
Channel 1 (with 8 message buffers) for receive. Filter 0 on CAN1 
is set up to accept messages with ID 0x201 and the accepted 
messages are stored in Channel 1. The payload of the received 
message contains the status of LED5. 

CAN2 uses Channel 0 (with 8 message buffers) for transmit and 
this transmit channel is RTR enabled. Filter 0 on CAN2 module 
is set up to accept Standard ID messages with ID 0x202. The 
filter points to Channel 0 which is a transmit Channel 0. A 
RX message filter can point to a TX Channel only if the channel 
is RTR enabled. CAN2 module auto transmits the message in Channel 
0 when the Filter 0 accepts a RTR message. 

When the code example is running successfully, it will be observed
that LED5 on the Explorer 16 Development Board will toggle every 
one second.



  

PIC32 CAN SID TX RX Code Example

Standard ID CAN Message Communication

What you need to run this code example:

1. Explorer 16 Development Board and 9V power supply
2. ECAN Pictail Plus board
3. CAN node connection cable.
4. PIC32 Plug In Module for Explorer 16 Board.
5. REAL ICE or ICD 3 or ICD 2 (programmer)

Programming the PIC32:

1. Open MPLAB IDE and select File->Import
2. Select  CAN TX RX Code Example.hex file contained in the obj
directory.
3. Mount the PIC32 PIM on the Explorer 16 board and insert the
ECAN PICtail Plus board in the PICtail slot.
4. Connect CAN node connection cable between CAN2 and CAN1 sockets
on the ECAN Pictail Plus Board.
5. Power on the board. Connect the programmer to the board. Choose
this programmer in MPLAB IDE and then select Programmer->Program.
6. When the code is running, LED5 and LED6 on the explorer 16 board
will toggle together 

About the Code Example:

This code example demonstrates the use of the PIC32MX CAN Peripheral
Library and PIC32MX CAN Module to send and receive Standard ID CAN messages
In this code example, CAN1 sends a message to CAN2 module every 1 second.
The data payload of this message switches LED5 on the Explorer 16 Development
board ON or OFF. CAN2 receives this message and toggles LED5. In return CAN2
sends a message to CAN1. The data payload of this message switches LED6 on
the Explorer 16 Development board ON or OFF. CAN1 receives this message 
and toggles LED6. 

CAN1 uses Channel 0 to transmit and Channel 1 to receive messages. The
size of both the channels is set to 8 messages. Filter 0 on CAN1 is 
configured to accept Standard ID messages with ID 0x201. The accepted 
message is stored in Channel 1. CAN2 uses Channel 0 to transmit and 
Channel 1 to receive messages. The size of both the channels is set to 8 
messages. Filter 0 on CAN2 is configured to accept Standard ID messages 
with ID 0x202. The accepted message is stored in Channel 1. 

If the code example runs successfully, it will be observed that LED5 and
LED6 on the Explorer 16 Development board will toggle together every one second.



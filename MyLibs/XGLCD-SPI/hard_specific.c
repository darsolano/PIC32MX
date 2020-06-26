
// Port Expander module connections
sbit  SPExpanderRST at RC0_bit;
sbit  SPExpanderCS  at RC1_bit;
sbit  SPExpanderRST_Direction at TRISC0_bit;
sbit  SPExpanderCS_Direction  at TRISC1_bit;
// End Port Expander module connections

void Init_mcu()
{
  // If Port Expander Library uses SPI1 module
  SPI1_Init();                                   // Initialize SPI module used with PortExpander

//  // If Port Expander Library uses SPI2 module
//  SPI2_Init();                                   // Initialize SPI module used with PortExpander

}

void UARTSW_SendByte(u08 data)
{
	// wait until uart is ready
	while(UARTSW_TxBusy);
	// set busy flag
	UARTSW_TxBusy = TRUE;
	// save data
	UARTSW_TxData = data;
	// set number of bits (+1 for stop bit)
	UARTSW_TxBitNum = 9;
	
	// set the start bit
	sbi(PORTD, PD5);
	// schedule the next bit
	outw(OCR1A, inw(TCNT1) + UARTSW_BaudRateDiv);
}

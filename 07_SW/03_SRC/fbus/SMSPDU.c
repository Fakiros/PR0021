unsigned char txt2pdu(char SPACE_BUFF *sms_obuf,unsigned char lenght)
{
 unsigned char loop0,loop1;

 for(loop0=0;loop0<lenght;loop0++){
  if(sms_obuf[loop0+1]&0x01) sms_obuf[loop0]|=0x80;
  else sms_obuf[loop0]&=(~0x80);

  for(loop1=loop0+1;loop1<lenght;loop1++){
   if(sms_obuf[loop1+1]&0x01) sms_obuf[loop1]|=0x80;
   else sms_obuf[loop1]&=~0x80;
   sms_obuf[loop1]=(sms_obuf[loop1]>>1);
  }
 }
 return(lenght-(loop0>>3));
}
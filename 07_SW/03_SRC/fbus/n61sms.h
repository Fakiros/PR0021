
extern void n61_init(int (*func)(int cmd, unsigned char c)); 
extern int n61_smssend(char *dest, char *msg);
extern  int n61_smsrecv(char *source, char *msg); 
extern  int n61_smsqueue(); 
extern int n61_update(); 



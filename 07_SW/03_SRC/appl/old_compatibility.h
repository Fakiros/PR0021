#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#define bit_set(v,m) ((v) |= (m))
#define bit_clear(v,m) ((v) &= ~(m))
#define bit_toggle(v,m) ((v) ^= (m))
#define bit_test(v, m) ((v) & (m))

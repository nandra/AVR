#ifndef _1wire_h_
#define _1wire_h_
#define MATCH_ROM	0x55
#define SKIP_ROM	0xCC
#define	SEARCH_ROM	0xF0

#define CONVERT_T	0x44		// DS1820 commands
#define READ		0xBE
#define WRITE		0x4E
#define EE_WRITE	0x48
#define EE_RECALL	0xB8

#define	SEARCH_FIRST	0xFF		// start new search
#define	PRESENCE_ERR	0xFF
#define	DATA_ERR	0xFE
#define LAST_DEVICE	0x00		// last device found
//			0x01 ... 0x40: continue searching

bit w1_reset(void);

uint w1_byte_wr( uchar b );
uint w1_byte_rd( void );

uchar w1_rom_search( uchar diff, uchar idata *id );

void w1_command( uchar command, uchar idata *id );
#endif

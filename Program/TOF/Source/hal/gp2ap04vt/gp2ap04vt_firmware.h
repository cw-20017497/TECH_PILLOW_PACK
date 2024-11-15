#ifndef _GP2AP04VT_FIRMWARE_H_
#define _GP2AP04VT_FIRMWARE_H_

#include "gp2ap04vt_api.h"

void gp2ap04vt_write_firmware(struct gp2ap04vt_sensor *);
void gp2ap04vt_write_firmware_linux(struct gp2ap04vt_sensor *, unsigned char *, unsigned int);

#ifndef __KERNEL__ 
#define FIRMWARE_CHECKSUM 0x85C1
#endif

#ifdef __KERNEL__ 
#define FIRMWARE_CHECKSUM 0xA877
#endif

#endif



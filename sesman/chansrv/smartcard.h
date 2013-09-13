/**
 * xrdp: A Remote Desktop Protocol server.
 *
 * Copyright (C) Laxmikant Rashinkar 2013 LK.Rashinkar@gmail.com
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

/*
 * smartcard redirection support
 */

#ifndef _SMARTCARD_C
#define _SMARTCARD_C

#include "parse.h"
#include "irp.h"
#include "trans.h"

#define SCARD_SHARE_EXCLUSIVE       0x00000001
#define SCARD_SHARE_SHARED          0x00000002
#define SCARD_SHARE_DIRECT          0x00000003

/* see [MS-RDPESC] 2.2.5 protocol identifier - Table A */
#define SCARD_PROTOCOL_UNDEFINED    0x00000000
#define SCARD_PROTOCOL_T0           0x00000001
#define SCARD_PROTOCOL_T1           0x00000002
#define SCARD_PROTOCOL_Tx           0x00000003
#define SCARD_PROTOCOL_RAW          0x00010000

/* see [MS-RDPESC] 2.2.5 protocol identifier - Table B */
#define SCARD_PROTOCOL_DEFAULT      0x80000000
#define SCARD_PROTOCOL_OPTIMAL      0x00000000

typedef struct reader_state
{
    char   reader_name[128];
    tui32  current_state;
    tui32  event_state;
    tui32  atr_len; /* number of bytes in atr[] */
    tui8   atr[36];

    /*
     * share mode flag, can be one of:
     *  SCARD_SHARE_EXCLUSIVE  app not willing to share smartcard with other apps
     *  SCARD_SHARE_SHARED     app willing to share smartcard with other apps
     *  SCARD_SHARE_DIRECT     app demands direct control of smart card, hence
     *                         it is not available to other readers
     */
    tui32  shared_mode_flag;

    /*
     * This field MUST have a value from Table A which is logically
     * OR'ed with a value from Table B.
     */
    tui32  preferred_protocol;

} READER_STATE;

void scard_device_announce(tui32 device_id);
int  APP_CC scard_get_wait_objs(tbus *objs, int *count, int *timeout);
int  APP_CC scard_check_wait_objs(void);
int  APP_CC scard_init(void);
int  APP_CC scard_deinit(void);
int  APP_CC scard_send_irp_establish_context(struct trans *con, int scope);
int  APP_CC scard_send_irp_list_readers(struct trans *con);

int  APP_CC scard_send_irp_get_status_change(struct trans *con, int wide, tui32 timeout,
                                             tui32 num_readers, READER_STATE* rsa);

int  APP_CC scard_send_irp_connect(struct trans *con, int wide, READER_STATE* rs);

#endif /* end #ifndef _SMARTCARD_C */

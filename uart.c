#include "uart.h"

#define STATE_SYNC     0
#define STATE_CMD      1
#define STATE_LENGTH   2
#define STATE_DATA1    3
#define STATE_DATA2    4

#define SYNC    0xAA

#define LED     0x01
#define BLINK   0x02
#define QUERY   0x03

#define ON      0x01
#define OFF     0x00

#define RESPONSE_OK       0x10
#define RESPONSE_ERROR    0x20

#define PACKET_LENGTH_MIN   1
#define PACKET_LENGTH_MAX   2

static Packet package;

int8 idx = STATE_SYNC;
int8 packet_gecerli = 0;

void uart_khbit(void)
{
    if (!kbhit(RS_422))
        return;

    switch (idx)
    {
        case STATE_SYNC:

            package.sync_byte = fgetc(RS_422);

            if (package.sync_byte != SYNC)
            {
                fprintf(RS_422, "SYNC ERROR\r\n");

                packet_gecerli = 0;
                Packet_response();

                while (kbhit(RS_422))
                    fgetc(RS_422);

                idx = STATE_SYNC;
                return;
            }

            idx = STATE_CMD;
            break;


        case STATE_CMD:

            package.cmd = fgetc(RS_422);

            if (package.cmd != LED &&
                package.cmd != BLINK &&
                package.cmd != QUERY)
            {
                fprintf(RS_422, "CMD ERROR\r\n");

                packet_gecerli = 0;
                Packet_response();

                while (kbhit(RS_422))
                    fgetc(RS_422);

                idx = STATE_SYNC;
                return;
            }

            idx = STATE_LENGTH;
            break;


        case STATE_LENGTH:

            package.length = fgetc(RS_422);

            if (package.cmd == LED)
            {
                if (package.length != PACKET_LENGTH_MAX)
                {
                    fprintf(RS_422, "LENGTH ERROR\r\n");

                    packet_gecerli = 0;
                    Packet_response();

                    while (kbhit(RS_422))
                        fgetc(RS_422);

                    idx = STATE_SYNC;
                    return;
                }
            }
            else
            {
                if (package.length != PACKET_LENGTH_MIN)
                {
                    fprintf(RS_422, "LENGTH ERROR\r\n");

                    packet_gecerli = 0;
                    Packet_response();

                    while (kbhit(RS_422))
                        fgetc(RS_422);

                    idx = STATE_SYNC;
                    return;
                }
            }

            idx = STATE_DATA1;
            break;


        case STATE_DATA1:

            package.data1 = fgetc(RS_422);

            if (package.cmd == LED)
            {
                if (package.data1 != OFF)
                {
                    fprintf(RS_422, "DATA1 ERROR\r\n");

                    packet_gecerli = 0;
                    Packet_response();

                    while (kbhit(RS_422))
                        fgetc(RS_422);

                    idx = STATE_SYNC;
                    return;
                }
            }
            else
            {
                if (package.data1 != ON)
                {
                    fprintf(RS_422, "DATA1 ERROR\r\n");

                    packet_gecerli = 0;
                    Packet_response();

                    while (kbhit(RS_422))
                        fgetc(RS_422);

                    idx = STATE_SYNC;
                    return;
                }
            }

            if (package.length == PACKET_LENGTH_MIN)
            {
                packet_gecerli = 1;

                Packet_solve();
                Packet_response();

                while (kbhit(RS_422))
                    fgetc(RS_422);

                idx = STATE_SYNC;
                return;
            }

            idx = STATE_DATA2;
            break;


        case STATE_DATA2:

            package.data2 = fgetc(RS_422);

            if (package.data2 != ON &&
                package.data2 != OFF)
            {
                fprintf(RS_422, "DATA2 ERROR\r\n");

                packet_gecerli = 0;
                Packet_response();

                while (kbhit(RS_422))
                    fgetc(RS_422);

                idx = STATE_SYNC;
                return;
            }

            if (package.length == PACKET_LENGTH_MAX)
            {
                packet_gecerli = 1;

                Packet_solve();
                Packet_response();

                while (kbhit(RS_422))
                    fgetc(RS_422);

                idx = STATE_SYNC;
                return;
            }

            fprintf(RS_422, "LENGTH ERROR\r\n");

            packet_gecerli = 0;
            Packet_response();

            while (kbhit(RS_422))
                fgetc(RS_422);

            idx = STATE_SYNC;
            return;


        default:

            fprintf(RS_422, "STATE ERROR\r\n");

            packet_gecerli = 0;
            Packet_response();

            while (kbhit(RS_422))
                fgetc(RS_422);

            idx = STATE_SYNC;
            return;
    }
}


void Packet_solve(void)
{
    switch (package.cmd)
    {
        case LED:

            if (package.data1 == OFF &&
                package.data2 == ON)
            {
                Led_ac();
            }
            else
            {
                Led_kapa();
            }

            return;

        case BLINK:

            Led_yan_son();

            return;

        case QUERY:

            if (LED_GetCaseState() == LED_BLINK)
            {
                fprintf(RS_422, "LED YANIP SONUYOR\r\n");
            }
            else if (LED_GetCaseState() == LED_ON)
            {
                fprintf(RS_422, "LED SU AN YANIYOR\r\n");
            }
            else
            {
                fprintf(RS_422, "LED SU AN KAPALI\r\n");
            }

            return;
    }
}


void Packet_response(void)
{
    int8 temp_response;
    int8 response_array[4];
    int8 idx_arr = 0;

    if (packet_gecerli)
        temp_response = RESPONSE_OK;
    else
        temp_response = RESPONSE_ERROR;

    response_array[idx_arr++] = 0xBB;
    response_array[idx_arr++] = package.cmd;
    response_array[idx_arr++] = PACKET_LENGTH_MIN;
    response_array[idx_arr++] = temp_response;

    for (int8 i = 0; i < idx_arr; i++)
        fputc(response_array[i], RS_422);

    package.sync_byte = 0;
    package.cmd = 0;
    package.length = 0;
    package.data1 = 0;
    package.data2 = 0;
}


extern "C" {
#include <stdio.h> 
#include <string.h>
}

#include "minsegbus.h"

MinSegBus::MinSegBus()
{
}


MinSegBus::~MinSegBus()
{
}

// Construct the byte array with the frame data
// for an unsigned 16-bit integer
void MinSegBus::ToByteArray(unsigned char iAddress,
    unsigned short iUnsignedShort,
    unsigned int iBufferLength,
    unsigned char *cBuff,
    unsigned int *iBytesReturned)
{
    int iBuffIdx;
    iBuffIdx = 0;
    int i;
    char *chArray = reinterpret_cast<char *>(&iUnsignedShort);
    unsigned short crc;

    // Two characters of zero, mark start condition
    cBuff[0] = 0x00;
    cBuff[1] = 0x00;

    // One byte for the address
    cBuff[2] = iAddress;

    // This is the function (type descriptor)
    cBuff[3] = 0x01;

    // Re-cast the 16-bit unsigned integer data as bytewise array
    cBuff[4] = chArray[0];
    cBuff[5] = chArray[1];

    // Calculate the CRC
    crc = 0xFFFF;
    for (i = 0; i < 6; i++)
    {
        crc=_bUpdateCRC(crc, cBuff[i]);
    }
    
    // Write the CRC value
    chArray = reinterpret_cast<char *>(&crc);
    cBuff[6] = chArray[0];
    cBuff[7] = chArray[1];

    // Two characters of zero, mark end condition
    cBuff[8] = 0x00;
    cBuff[9] = 0x00;

    // Update the byte counts
    *iBytesReturned = 10;
}

// Construct the byte array with the frame data
// for a 32-bit decimal (float)
void MinSegBus::ToByteArray(unsigned char iAddress,
    float fValue,
    unsigned int iBufferLength,
    unsigned char *cBuff,
    unsigned int *iBytesReturned)
{
    int iBuffIdx;
    iBuffIdx = 0;
    int i;
    char *chArray = reinterpret_cast<char *>(&fValue);
    unsigned short crc;

    // Two characters of zero, mark start condition
    cBuff[0] = 0x00;
    cBuff[1] = 0x00;

    // One byte for the address
    cBuff[2] = iAddress;

    // This is the function (type descriptor)
    cBuff[3] = 0x00;

    // Re-cast the 32-bit float integer data as bytewise array
    cBuff[4] = chArray[0];
    cBuff[5] = chArray[1];
    cBuff[6] = chArray[2];
    cBuff[7] = chArray[3];

    // Calculate the CRC
    crc = 0xFFFF;
    for (i = 0; i < 8; i++)
    {
        crc = _bUpdateCRC(crc, cBuff[i]);
    }

    // Write the CRC value
    chArray = reinterpret_cast<char *>(&crc);
    cBuff[8] = chArray[0];
    cBuff[9] = chArray[1];

    // Two characters of zero, mark end condition
    cBuff[10] = 0x00;
    cBuff[11] = 0x00;

    // Update the byte c2ounts
    *iBytesReturned = 12;
}

// Deconstruct the byte array with the frame data
// for an unsigned 16-bit integer
void MinSegBus::FromByteArray(unsigned char *iAddress,
    unsigned short *iUnsignedShort,
    unsigned char *cBuff,
    unsigned int *iErrorCount)
{
    int iBuffIdx;
    iBuffIdx = 0;
    int i;
    unsigned short crc;
    unsigned short iStoredCRC;


    // Reset the error counter
    iErrorCount = 0;

    // Are the first two characters zero?  (mark start condition)
    if (cBuff[0] != 0x00 && cBuff[1] != 0x00)
    {
        *iErrorCount++;
        return;
    }

    // Retrieve the address
    *iAddress = cBuff[2];

    // Retrieve the function (type descriptor), it must be one
    // to contain a 16-bit integer.
    if( cBuff[3] == 0x01)
    {
        // Calculate the CRC
        crc = 0xFFFF;
        for (i = 0; i < 6; i++)
        {
            crc = _bUpdateCRC(crc, cBuff[i]);
        }
            
        // Compare with recorded crc
        if (crc != *((unsigned short*)&cBuff[6]))
        {
            *iErrorCount++;
            return;
        }

        // Are the two ending zeros in place?
        if (cBuff[8] != 0x00 && cBuff[9] != 0x00)
        {
            *iErrorCount++;
            return;
        }

        // We've gotten this far, the value must be ok.
        *iUnsignedShort = *((unsigned short*)&cBuff[4]);
    }
    else
    {
        *iErrorCount++;
        return;
    }
}

// Deconstruct the byte array with the frame data
// for a 32-bit float
void MinSegBus::FromByteArray(unsigned char *iAddress,
    float *fValue,
    unsigned char *cBuff,
    unsigned int *iErrorCount)
{
    int iBuffIdx;
    iBuffIdx = 0;
    int i;
    unsigned short crc;
    unsigned short iStoredCRC;


    // Reset the error counter
    iErrorCount = 0;

    // Are the first two characters zero?  (mark start condition)
    if (cBuff[0] != 0x00 && cBuff[1] != 0x00)
    {
        *iErrorCount++;
        return;
    }

    // Retrieve the address
    *iAddress = cBuff[2];

    // Retrieve the function (type descriptor), it must be zero
    // to contain a 32-bit integer.
    if (cBuff[3] == 0x00)
    {
        // Calculate the CRC
        crc = 0xFFFF;
        for (i = 0; i < 8; i++)
        {
            crc = _bUpdateCRC(crc, cBuff[i]);
        }

        // Compare with recorded crc
        if (crc != *((unsigned short*)&cBuff[8]))
        {
            *iErrorCount++;
            return;
        }

        // Are the two ending zeros in place?
        if (cBuff[10] != 0x00 && cBuff[11] != 0x00)
        {
            *iErrorCount++;
            return;
        }

        // We've gotten this far, the value must be ok.
        *fValue = *((float*)&cBuff[4]);
    }
    else
    {
        *iErrorCount++;
        return;
    }
}

unsigned short MinSegBus::_bUpdateCRC(unsigned short crc, unsigned char data)
{
    int i;

    crc = crc ^ ((unsigned short)data << 8);
    for (i = 0; i<8; i++)
    {
        if (crc & 0x8000)
            crc = (crc << 1) ^ 0x1021;
        else
            crc <<= 1;
    }

    return crc;
}

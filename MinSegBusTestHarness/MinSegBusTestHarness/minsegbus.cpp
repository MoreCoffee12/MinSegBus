extern "C" {
#include <stdio.h> 
#include <string.h>
}

#include "minsegbus.h"


MinSegBus::MinSegBus()
{
    _iRingBufferCount = BUFF_SIZE;
    cRingBuffer.iWriteIndex = 0x00;
    for (int idx = 0; idx < BUFF_SIZE; ++idx)
    {
        cRingBuffer.cRingBuff[idx] = 0xFF;
    }

    _iErrorCount = 100;
    _iAddress = 0x00;

}


MinSegBus::~MinSegBus()
{
}

// Construct the byte array with the frame data
// for an unsigned 16-bit integer
void MinSegBus::ToByteArray(unsigned char iAddress,
    unsigned short *iUnsignedShort,
    unsigned int iShortCount,
    unsigned int iBufferLength,
    unsigned char *cBuff,
    unsigned int *iBytesReturned)
{
    int iBuffIdx;
    iBuffIdx = 0;
    unsigned int i;
    unsigned int idx;

    // Initialize the buffer pointer
    idx = 0;

    // Create the front half of the frame
    _bCreateFrontFrame((2 * iShortCount), iAddress,
        0x01, cBuff, &idx);

    // Re-cast the 16-bit unsigned integer data as bytewise array
    for (i = 0; i < iShortCount; i++)
    {
        cBuff[idx] = iUnsignedShort[i] & 0x00FF;
        idx++;
        cBuff[idx] = ( iUnsignedShort[i] & 0xFF00 ) >> 8;
        idx++;
    }

    // Create the back half of the frame
    _bCreateBackFrame(cBuff, &idx);

    // Update the byte counts
    *iBytesReturned = idx;
}

// Construct the byte array with the frame data
// for a 32-bit decimal (float)
void MinSegBus::FloatToByteArray(unsigned char iAddress,
    float *fValue,
    unsigned int iFloatCount,
    unsigned int iBufferLength,
    unsigned char *cBuff,
    unsigned int *iBytesReturned)
{
    int iBuffIdx;
    iBuffIdx = 0;
    unsigned int i;
    unsigned int idx;

    union 
    {
        float float_variable;
        char temp_array[4];
    } u;

    // Initialize the buffer pointer
    idx = 0;

    // Create the front half of the frame
    _bCreateFrontFrame((4 * iFloatCount), iAddress,
        0x00, cBuff, &idx);

    // Re-cast the 32-bit float data as bytewise array
    for (i = 0; i < iFloatCount; i++)
    {
        u.float_variable = fValue[i];
        cBuff[idx] = u.temp_array[0];
        idx++;
        cBuff[idx] = u.temp_array[1];
        idx++;
        cBuff[idx] = u.temp_array[2];
        idx++;
        cBuff[idx] = u.temp_array[3];
        idx++;
    }

    // Create the back half of the frame
    _bCreateBackFrame(cBuff, &idx);

    // Update the byte counts
    *iBytesReturned = idx;
}

// Deconstruct the byte array with the frame data
// for an unsigned 16-bit integer
void MinSegBus::FromByteArray(unsigned char *iAddress,
    unsigned short *iUnsignedShortArray,
    unsigned int iMaxShortCount,
    unsigned char *cBuff,
    unsigned int *iErrorCount)

{
    unsigned int iBuffIdx;
    unsigned int iFrameSize;

    // Initialize the buffer pointer
    iBuffIdx = 0;

    // Reset the error counter
    _iErrorCount = 0;

    // Is this a valid frame?
    if( !_bIsFrameValid(cBuff, &iFrameSize) )
    {
        ++(*iErrorCount);
        return;
    }

    // Retrieve the address
    *iAddress = cBuff[3];

    // Retrieve the function (type descriptor), it must be one
    // to contain a 16-bit integer.
    if( cBuff[4] == 0x01)
    {

        for (iBuffIdx = 0; iBuffIdx < (iFrameSize - 9)/2; iBuffIdx++)
        {
            iUnsignedShortArray[iBuffIdx] = *((unsigned short*)&cBuff[5 + (iBuffIdx * 2)]);

        }
  
    }
    else
    {
        ++_iErrorCount;
        *iErrorCount = _iErrorCount;
        return;
    }
}

unsigned int MinSegBus::iGetFrameCount_Short(unsigned int iShortCount)
{
    return 9 + (iShortCount * 2);
}


// Deconstruct the byte array with the frame data
// for a 32-bit float
void MinSegBus::FloatFromByteArray(unsigned char *iAddress,
    float *fValueArray,
    unsigned int iMaxFloatCount,
    unsigned char *cBuff,
    unsigned int *iErrorCount)
{

    unsigned int iBuffIdx;
    unsigned int iFrameSize;
    unsigned int iBytesSize;

    // Initialize the buffer pointer
    iBuffIdx = 0;

    // Reset the error counter
    _iErrorCount = 0;

    // Define the number of bytes for the float
    iBytesSize = sizeof(float);

    // Is this a valid frame?
    if( !_bIsFrameValid(cBuff, &iFrameSize) )
    {
        ++(*iErrorCount);
        return;
    }

    // Retrieve the address
    *iAddress = cBuff[3];

    // Make sure we don't overrun the buffer
    if ((iFrameSize - 9) / iBytesSize > iMaxFloatCount)
    {
        ++_iErrorCount;
        *iErrorCount = _iErrorCount;
        return;
    }

    // Retrieve the function (type descriptor), it must be zero (0x00)
    // to contain a 32-bit float.
    if (cBuff[4] == 0x00)
    {

        for (iBuffIdx = 0; iBuffIdx < ((iFrameSize - 9) / iBytesSize); iBuffIdx++)
        {

            fValueArray[iBuffIdx] = *((float*)&cBuff[5 + (iBuffIdx * iBytesSize)]);

        }

    }
    else
    {
        ++_iErrorCount;
        *iErrorCount = _iErrorCount;
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

bool MinSegBus::_bCreateFrontFrame(unsigned int iByteCount, unsigned char iAddress,
    unsigned char cType, unsigned char *cBuff, unsigned int * idx)
{
    // Two characters of zero, mark start condition
    cBuff[*idx] = 0x00;
    *idx = *idx + 1;
    cBuff[*idx] = 0x00;
    *idx = *idx + 1;

// One byte for the total number of bytes in the frame
cBuff[*idx] = iByteCount + 9;
*idx = *idx + 1;

// One byte for the address
cBuff[*idx] = iAddress;
*idx = *idx + 1;

// This is the function (type descriptor)
cBuff[*idx] = cType;
*idx = *idx + 1;

return true;
}


bool MinSegBus::_bCreateBackFrame(unsigned char *cBuff, unsigned int * idx)
{

    unsigned short crc;
    unsigned int i;

    // Calculate the CRC
    crc = 0xFFFF;
    for (i = 0; i < *idx; i++)
    {
        crc = _bUpdateCRC(crc, cBuff[i]);
    }

    // Write the CRC value
    cBuff[*idx] = crc & 0x00FF;
    *idx = *idx + 1;
    cBuff[*idx] = (crc & 0xFF00) >> 8;
    *idx = *idx + 1;

    // Two characters of zero, mark end condition
    cBuff[*idx] = 0x00;
    *idx = *idx + 1;
    cBuff[*idx] = 0x00;
    *idx = *idx + 1;

    return true;
}

bool  MinSegBus::_bIsFrameValid(unsigned char *cBuff,
    unsigned int *iFrameSize)
{

    unsigned short crc;
    unsigned int i;

    // Are the first two characters zero?  (mark start condition)
    // and is the frame size long enough?
    if (((cBuff[0] + cBuff[1]) > 0) || ( cBuff[2] < 11 ) )
    {
        return false;
    }

    // Retrieve the frame size
    *iFrameSize = cBuff[2];

    // Calculate the CRC
    crc = 0xFFFF;
    for (i = 0; i < *iFrameSize - 4; ++i)
    {
        crc = _bUpdateCRC(crc, cBuff[i]);
    }

    // Compare with recorded crc
    if (crc != *((unsigned short*)&cBuff[*iFrameSize - 4]))
    {
        return false;
    }

    // So far so good, verify that the last two bytes 
    // of the frame are also zero.
    if ((cBuff[*iFrameSize - 2] + cBuff[*iFrameSize - 1]) > 0)
    {
        return false;
    }

    return true;
}

unsigned int MinSegBus::iGetRingBuffCount()
{
    return _iRingBufferCount;
}

unsigned int MinSegBus::iGetAddress()
{
    return _iAddress;
}

unsigned int MinSegBus::iGetErrorCount()
{
    return _iErrorCount;
}

void MinSegBus::clearRingBuff()
{
    cRingBuffer.iWriteIndex = 0x00;
    for (int idx = 0; idx < BUFF_SIZE; ++idx)
    {
        cRingBuffer.cRingBuff[idx] = 0xFF;
    }
}

void MinSegBus::writeRingBuff(unsigned char cValue)
{
    cRingBuffer.cRingBuff[(cRingBuffer.iWriteIndex++) & BUFF_SIZE_MASK] = cValue;
}

unsigned char MinSegBus::readRingBuff(int iXn)
{
    return cRingBuffer.cRingBuff[(cRingBuffer.iWriteIndex + (~iXn)) & BUFF_SIZE_MASK];
}

void MinSegBus::writeRingBuff(unsigned char cValue,
    unsigned short *iUnsignedShortArray,
    unsigned int iShortCount)
{
    unsigned char iAddressTemp = 0;
    unsigned int iErrorCountTemp = 0;

    writeRingBuff(cValue, &iAddressTemp,
        iUnsignedShortArray,
        iShortCount,
        &iErrorCountTemp);

    _iErrorCount = iErrorCountTemp;
    _iAddress = iAddressTemp;

    return;
}

void MinSegBus::writeRingBuff(unsigned char cValue, unsigned char *iAddress,
    unsigned short *iUnsignedShortArray,
    unsigned int iShortCount,
    unsigned int *iErrorCount)
{
    unsigned int iFrameSize = iGetFrameCount_Short(iShortCount);

    // Calculate the starting index
    unsigned int idxStart = (iFrameSize - 1);

    // Assume there is an error, this will clear only if the
    // frame is successfully parsed.
    *iErrorCount = 0x01;

    // Save the value to ring buffer
    cRingBuffer.cRingBuff[(cRingBuffer.iWriteIndex++) & BUFF_SIZE_MASK] = cValue;

    // Check that leading and lagging zeros are in place and that
    // the frame size is correct
    if ((cValue + readRingBuff(1) + readRingBuff(idxStart) + readRingBuff(idxStart - 1) + readRingBuff(idxStart - 2)) != iFrameSize)
    {
        return;
    }

    // Calculate the CRC
    unsigned short crc = 0xFFFF;
    for (int i = idxStart; i > 3; --i)
    {
        crc = _bUpdateCRC(crc, readRingBuff(i));
    }

    // Compare with recorded crc and if it matches then the frame must
    // be at least a valid frame
    if (crc != (unsigned short)((readRingBuff(2) << 8) + readRingBuff(3)))
    {
        return;
    }

    // Retrieve the function (type descriptor), it must be one
    // to contain a 16-bit integer.
    if (readRingBuff(idxStart - 4) != 0x01)
    {
        return;
    }

    // Copy the data over
    for (unsigned int iBuffIdx = 0; iBuffIdx < iShortCount; ++iBuffIdx)
    {
        iUnsignedShortArray[iBuffIdx] = (unsigned short)((readRingBuff(idxStart - 6 - (iBuffIdx << 1)) << 8) + readRingBuff(idxStart - 5 - (iBuffIdx << 1)));
    }

    // Address
    *iAddress = readRingBuff(idxStart - 3);

    // Success
    *iErrorCount = 0x00;

    // Done
    return;
}

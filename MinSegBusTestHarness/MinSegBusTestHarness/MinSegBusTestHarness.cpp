// MinSegBusTestHarness.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "minsegbus.h"
#include <iostream>
#include "conio.h"

#define maxbuffer     0x0400
#define maxshortcount 0x000A
#define maxfloatcount 0x000A

int _tmain(int argc, _TCHAR* argv[])
{
	

    unsigned char iAddress;
    unsigned char cTemp;
    unsigned short iUnsignedShort;
    unsigned short iUnsignedShortArray[maxshortcount];
    unsigned char cBuff[maxbuffer];
    MinSegBus *mbus = new MinSegBus();
    unsigned int iBytesReturned;
    unsigned int iErrorCount;
    unsigned int iIdx;
    unsigned int iShortCount;
    int iTemp;
    float fValue;
    float fValueArray[maxfloatcount];

    /////////////////////////////////////////////////////////////////////
    // Test harness section for the deconstruction of a frame for 
    // a 16-bit integer.
    /////////////////////////////////////////////////////////////////////

    // Check that the methods catch a frame without the two leading zeros
    for (iIdx = 0; iIdx < maxbuffer; iIdx++)
    {
        cBuff[iIdx] = 0;
    }
    cBuff[0] = 1;
    iErrorCount = 0;
    mbus->FromByteArray(&iAddress, &iUnsignedShort, 1, cBuff, &iErrorCount);
    if (iErrorCount > 0x00)
    {
        std::cout << "Successfully caught first non-zero." << std::endl;
    }
    else
    {
        std::cout << "Failed to catch first non-zero." << std::endl;
        return 0;
    }

    cBuff[0] = 0;
    cBuff[1] = 1;
    iErrorCount = 0;
    mbus->FromByteArray(&iAddress, &iUnsignedShort, 1, &cBuff[0], &iErrorCount);
    if (iErrorCount > 0x00)
    {
        std::cout << "Successfully caught second non-zero." << std::endl;
    }
    else
    {
        std::cout << "Failed to catch second non-zero." << std::endl;
        return 0;
    }

    // Test the ability of the code to retrieve the address
    cBuff[0] = 0x00;
    cBuff[1] = 0x00;
    cBuff[2] = 0x0B;
    cBuff[3] = 0x01;
    cBuff[4] = 0x01;
    cBuff[5] = 0x00;
    cBuff[6] = 0x04;
    cBuff[7] = 0x31;
    cBuff[8] = 0x1C;
    cBuff[9] = 0x00;
    cBuff[10] = 0x00;
    iAddress = 0x01;
    iErrorCount = 0;
    mbus->FromByteArray(&iAddress, &iUnsignedShort, 1, &cBuff[0], &iErrorCount);
    if (iErrorCount == 0  && iAddress == 1)
    {
        std::cout << "Successfully retrieved the address." << std::endl;
    }
    else
    {
        std::cout << "Failed to retrieved the address." << std::endl;
        return 0;
    }

    // Test the ability of the code to detect invalid CRC
    cBuff[0] = 0x00;
    cBuff[1] = 0x00;
    cBuff[2] = 0x0B;
    cBuff[3] = 0x01;
    cBuff[4] = 0x01;
    cBuff[5] = 0x00;
    cBuff[6] = 0x04;
    cBuff[7] = 0xFF;
    cBuff[8] = 0x1C;
    cBuff[9] = 0x00;
    cBuff[10] = 0x00;
    iAddress = 0x01;
    iErrorCount = 0;
    iUnsignedShort = 0;
    mbus->FromByteArray(&iAddress, &iUnsignedShort, 1, cBuff, &iErrorCount);
    if (iErrorCount > 0x00 )
    {
        std::cout << "Successfully detected invalid CRC." << std::endl;
    }
    else
    {
        std::cout << "Failed to detected invalid CRC." << std::endl;
        return 0;
    }

    // Test the ability of the code to detect invalid end mark condition
    cBuff[0] = 0x00;
    cBuff[1] = 0x00;
    cBuff[2] = 0x0B;
    cBuff[3] = 0x01;
    cBuff[4] = 0x01;
    cBuff[5] = 0x00;
    cBuff[6] = 0x04;
    cBuff[7] = 0x31;
    cBuff[8] = 0x1C;
    cBuff[9] = 0x01;
    cBuff[10] = 0x00;
    iAddress = 0x01;
    iErrorCount = 0;
    iUnsignedShort = 0;
    mbus->FromByteArray(&iAddress, &iUnsignedShort, 1, &cBuff[0], &iErrorCount);
    if (iErrorCount > 0 )
    {
        std::cout << "Successfully trapped invalid end mark condition." << std::endl;
    }
    else
    {
        std::cout << "Failed to trap invalid end mark condition." << std::endl;
        return 0;
    }

    // Test the ability of the code to detect invalid end mark condition
    cBuff[0] = 0x00;
    cBuff[1] = 0x00;
    cBuff[2] = 0x0B;
    cBuff[3] = 0x01;
    cBuff[4] = 0x01;
    cBuff[5] = 0x00;
    cBuff[6] = 0x04;
    cBuff[7] = 0x31;
    cBuff[8] = 0x1C;
    cBuff[9] = 0x00;
    cBuff[10] = 0x01;
    iErrorCount = 0;
    iUnsignedShort = 0;
    mbus->FromByteArray(&iAddress, &iUnsignedShort, 1, &cBuff[0], &iErrorCount);
    if (iErrorCount == 1)
    {
        std::cout << "Successfully trapped invalid end mark condition." << std::endl;
    }
    else
    {
        std::cout << "Failed to trap invalid end mark condition." << std::endl;
        return 0;
    }

    // Test the ability of the code to retrieve the 16-bit value of 1024 (0x400)
    cBuff[0] = 0x00;
    cBuff[1] = 0x00;
    cBuff[2] = 0x0B;
    cBuff[3] = 0x01;
    cBuff[4] = 0x01;
    cBuff[5] = 0x00;
    cBuff[6] = 0x04;
    cBuff[7] = 0x31;
    cBuff[8] = 0x1C;
    cBuff[9] = 0x00;
    cBuff[10] = 0x00;
    iErrorCount = 0;
    iUnsignedShort = 0;
    mbus->FromByteArray(&iAddress, &iUnsignedShort, 1, &cBuff[0], &iErrorCount);
    if (iErrorCount == 0x00 && iUnsignedShort == 0x400)
    {
        std::cout << "Successfully retrieved the 16-bit integer." << std::endl;
    }
    else
    {
        std::cout << "Failed to retrieved the 16-bit integer." << std::endl;
        return 0;
    }

    /////////////////////////////////////////////////////////////////////
    // Test harness section for the deconstruction of a frame for 
    // a 16-bit integer
    /////////////////////////////////////////////////////////////////////

    // Construct the frame for a 16-bit integer
    iAddress = 0x001;
    iUnsignedShort = 1024;
    iBytesReturned = 0;
    mbus->ToByteArray(iAddress, &iUnsignedShort, 1, maxbuffer, &cBuff[0], &iBytesReturned);
    if ( iBytesReturned == 10)
    {
        std::cout << "ToByteArray returned the expected number of bytes." << std::endl;
    }
    else
    {
        std::cout << "ToByteArray failed to return the expected number of bytes." << std::endl;
        //return 0;
    }

    // Is the returned byte array correct?
    iAddress = 0x00;
    iUnsignedShort = 0x00;
    mbus->FromByteArray(&iAddress, &iUnsignedShort,1, &cBuff[0], &iErrorCount);
    if (iAddress == 0x01 && iUnsignedShort == 0x400 && iErrorCount == 0x00)
    {
        std::cout << "ToByteArray returned a valid frame for the 16-bit integer" << std::endl;
    }
    else
    {
        std::cout << "ToByteArray failed to return a valid frame for the 16-bit integer." << std::endl;
        return 0;
    }

    /////////////////////////////////////////////////////////////////////
    // Test harness section for the deconstruction of a frame for 
    // a 16-bit integer using the array, but with a single element
    /////////////////////////////////////////////////////////////////////

    // Construct the frame for a 16-bit integer
    iAddress = 0x001;
    iUnsignedShort = 1024;
    iBytesReturned = 0;
    mbus->ToByteArray(iAddress, &iUnsignedShort, 1, maxbuffer, &cBuff[0], &iBytesReturned);
    if (iBytesReturned == 11)
    {
        std::cout << "ToByteArray using the array call returned the expected number of bytes." << std::endl;
    }
    else
    {
        std::cout << "ToByteArray using the array call failed to return the expected number of bytes." << std::endl;
        return 0;
    }

    // Is the returned byte array correct?
    iAddress = 0x00;
    iUnsignedShort = 0x00;
    mbus->FromByteArray(&iAddress, &iUnsignedShort, 1, &cBuff[0], &iErrorCount);
    if (iAddress == 0x01 && iUnsignedShort == 0x400 && iErrorCount == 0x00)
    {
        std::cout << "ToByteArray using the array call returned a valid frame for the 16-bit integer" << std::endl;
    } 
    else
    {
        std::cout << "ToByteArray using the array call failed to return a valid frame for the 16-bit integer." << std::endl;
        return 0;
    }

    /////////////////////////////////////////////////////////////////////
    // Test harness section for the deconstruction of a frame for 
    // a 16-bit integer using the array, but with a two elements
    /////////////////////////////////////////////////////////////////////

    // Construct the frame for a 16-bit integer
    iAddress = 0x001;
    iUnsignedShortArray[0] = 1024;
    iUnsignedShortArray[1] = 24;
    iBytesReturned = 0;
    mbus->ToByteArray(iAddress, iUnsignedShortArray, 2, maxbuffer, &cBuff[0], &iBytesReturned);
    if (iBytesReturned == 13)
    {
        std::cout << "ToByteArray using the array call with 2 elements returned the expected number of bytes." << std::endl;
    }
    else
    {
        std::cout << "ToByteArray using the array call with 2 elements failed to return the expected number of bytes." << std::endl;
        return 0;
    }

    // Is the returned byte array correct?
    iAddress = 0x00;
    iUnsignedShortArray[0] = 0x00;
    iUnsignedShortArray[1] = 0x00;
    iErrorCount = 0;
    mbus->FromByteArray(&iAddress, iUnsignedShortArray, maxshortcount, &cBuff[0], &iErrorCount);
    if (iAddress == 0x01 && iUnsignedShortArray[0] == 0x400 && iUnsignedShortArray[1] == 0x018 && iErrorCount == 0x00)
    {
        std::cout << "ToByteArray using the array call with 2 elements returned a valid frame for the 16-bit integer" << std::endl;
    }
    else
    {
        std::cout << "ToByteArray using the array call with 2 elements failed to return a valid frame for the 16-bit integer." << std::endl;
        return 0;
    }

    /////////////////////////////////////////////////////////////////////
    // Test harness section for the deconstruction of a frame for 
    // a 16-bit integer using the array, but with a two elements, but
    // both of the zero.  This broke the code at one point.
    /////////////////////////////////////////////////////////////////////

    // Construct the frame for a 16-bit integer
    iAddress = 0x001;
    iUnsignedShortArray[0] = 0;
    iUnsignedShortArray[1] = 0;
    iBytesReturned = 0;
    mbus->ToByteArray(iAddress, iUnsignedShortArray, 2, maxbuffer, &cBuff[0], &iBytesReturned);
    if (iBytesReturned == 13)
    {
        std::cout << "ToByteArray using the array call with 2 elements (both zero) returned the expected number of bytes." << std::endl;
    }
    else
    {
        std::cout << "ToByteArray using the array call with 2 elements (both zero) failed to return the expected number of bytes." << std::endl;
        return 0;
    }

    // Is the returned byte array correct?
    iAddress = 0x00;
    iUnsignedShortArray[0] = 0x00;
    iUnsignedShortArray[1] = 0x00;
    iErrorCount = 0;
    mbus->FromByteArray(&iAddress, iUnsignedShortArray, maxshortcount, &cBuff[0], &iErrorCount);
    if (iAddress == 0x01 && iUnsignedShortArray[0] == 0 && iUnsignedShortArray[1] == 0 && iErrorCount == 0x00)
    {
        std::cout << "ToByteArray using the array call with 2 elements (both zero) returned a valid frame for the 16-bit integer" << std::endl;
    }
    else
    {
        std::cout << "ToByteArray using the array call with 2 elements (both zero) failed to return a valid frame for the 16-bit integer." << std::endl;
        return 0;
    }

    /////////////////////////////////////////////////////////////////////
    // Test harness section for the deconstruction of a frame for 
    // a 32-bit decimal number (float)
    /////////////////////////////////////////////////////////////////////

    // Construct the frame for a 32-bit float
    iAddress = 0x001;
    fValue = 3.14159f;
    iBytesReturned = 0;
    mbus->FloatToByteArray(iAddress, &fValue, 1, maxbuffer, cBuff, &iBytesReturned);
    if (iBytesReturned == 13)
    {
        std::cout << "ToByteArray returned the expected number of bytes for the float." << std::endl;
    }
    else
    {
        std::cout << "ToByteArray failed to return the expected number of bytes for the float." << std::endl;
        return 0;
    }

    // Is the returned byte array correct?
    iAddress = 0x00;
    fValue = 0.00;
    mbus->FloatFromByteArray(&iAddress, &fValue, 0x01, &cBuff[0], &iErrorCount);
    if (iAddress == 0x01 && fValue == 3.14159f && iErrorCount == 0x00)
    {
        std::cout << "ToByteArray returned a valid frame for the float" << std::endl;
    }
    else
    {
        std::cout << "ToByteArray failed to return a valid frame for the float." << std::endl;
        return 0;
    }

    std::cout << "All tests completed successfully!!!" << std::endl;


    /////////////////////////////////////////////////////////////////////
    // Test harness section for the deconstruction of a frame for 
    // a 32-bit decimal number (float)
    /////////////////////////////////////////////////////////////////////

    // Construct the frame for a 32-bit float
    iAddress = 0x001;
    fValueArray[0] = 3.14159f;
    fValueArray[1] = 6.14159f;
    iBytesReturned = 0;
    mbus->FloatToByteArray(iAddress, fValueArray, 2, maxbuffer, cBuff, &iBytesReturned);
    if (iBytesReturned == 17)
    {
        std::cout << "ToByteArray returned the expected number of bytes for the 2-element array of floats." << std::endl;
    }
    else
    {
        std::cout << "ToByteArray failed to return the expected number of bytes for the 2-element array of floats." << std::endl;
        return 0;
    }

    // Is the returned byte array correct?
    iAddress = 0x00;
    fValueArray[0] = 0.00f;
    fValueArray[1] = 0.00f;
    mbus->FloatFromByteArray(&iAddress, fValueArray, 0x02, cBuff, &iErrorCount);
    if (iAddress == 0x01 && fValueArray[0] == 3.14159f && fValueArray[1] == 6.14159f && iErrorCount == 0x00)
    {
        std::cout << "ToByteArray returned a valid frame for the 2-element array of floats." << std::endl;
    }
    else
    {
        std::cout << "ToByteArray failed to return a valid frame for the 2-element array of floats." << std::endl;
        return 0;
    }



    /////////////////////////////////////////////////////////////////////
    // Test harness section for the ring buffer management.  Begin by 
    // making sure the structure of the ring buffer is what we expect.
    /////////////////////////////////////////////////////////////////////
    if (mbus->iGetRingBuffCount() == 0x40)
    {
        std::cout << "Ring buffer returned the expected buffer count." << std::endl;
    }
    else
    {
        std::cout << "Ring buffer did not return the expected buffer count." << std::endl;
        return 0;
    }


    /////////////////////////////////////////////////////////////////////
    // Next, test that a single value can be read and written successfully
    /////////////////////////////////////////////////////////////////////
    mbus->writeRingBuff(0x03);
    cBuff[0] = mbus->readRingBuff(0x00);

    if (cBuff[0] == 0x03)
    {
        std::cout << "Ring buffer correctly saved and retrieved a single value." << std::endl;
    }
    else
    {
        std::cout << "Ring buffer failed to save and/or retrieve a single value." << std::endl;
        return 0;
    }

    /////////////////////////////////////////////////////////////////////
    // Test that a single value can be read and written successfully to
    // the next position
    /////////////////////////////////////////////////////////////////////
    mbus->writeRingBuff(0x04);
    cBuff[0] = mbus->readRingBuff(0x00);

    if (cBuff[0] == 0x04)
    {
        std::cout << "Ring buffer correctly saved and retrieved a single value, Test 2." << std::endl;
    }
    else
    {
        std::cout << "Ring buffer failed to save and/or retrieve a single value, Test 2." << std::endl;
        return 0;
    }

    cBuff[0] = mbus->readRingBuff(1);
    if (cBuff[0] == 0x03)
    {
        std::cout << "Ring buffer correctly saved and retrieved a single value, Test 3." << std::endl;
    }
    else
    {
        std::cout << "Ring buffer failed to save and/or retrieve a single value, Test 3." << std::endl;
        return 0;
    }

    /////////////////////////////////////////////////////////////////////
    // Write and read a complete sequence
    /////////////////////////////////////////////////////////////////////
    mbus->clearRingBuff();
    for (cTemp = mbus->iGetRingBuffCount(); cTemp > 0; cTemp--)
    {
        mbus->writeRingBuff(cTemp);
    }

    // Check the first value
    if (mbus->readRingBuff(0x00) != 0x01 )
    {
        std::cout << "Ring buffer failed to save and/or retrieve a series of values." << std::endl;
        return 0;
    }
    // Check the last value
    if (mbus->readRingBuff(mbus->iGetRingBuffCount() - 1) != mbus->iGetRingBuffCount())
    {
        std::cout << "Ring buffer failed to save and/or retrieve a series of values." << std::endl;
        return 0;
    }
    std::cout << "Ring buffer correctly saved and retrieved a series of values." << std::endl;

    /////////////////////////////////////////////////////////////////////
    // Write and read a complete frame of data
    /////////////////////////////////////////////////////////////////////
    mbus->clearRingBuff();

    // Construct the frame for a 16-bit integer
    iAddress = 0x001;
    iUnsignedShortArray[0] = 1024;
    iUnsignedShortArray[1] = 24;
    iBytesReturned = 0;
    mbus->ToByteArray(iAddress, iUnsignedShortArray, 2, maxbuffer, &cBuff[0], &iBytesReturned);
    if (iBytesReturned == 13)
    {
        std::cout << "ToByteArray using the array call with 2 elements returned the expected number of bytes in ring buffer test." << std::endl;
    }
    else
    {
        std::cout << "ToByteArray using the array call with 2 elements failed to return the expected number of bytes in ring buffer test." << std::endl;
        return 0;
    }

    // Write the frame to the ring buffer and clear the buffer
    for (iTemp = 0; iTemp < (int)iBytesReturned; iTemp++)
    {
        mbus->writeRingBuff(cBuff[iTemp]);
        cBuff[iTemp] = 0x00;
    }

    // Read the data back
    for (iTemp = 0; iTemp < (int)iBytesReturned; iTemp++)
    {
        cBuff[iTemp] = mbus->readRingBuff((unsigned char)iBytesReturned - iTemp - 1);
    }

    // Is the returned byte array correct?
    iAddress = 0x00;
    iUnsignedShortArray[0] = 0x00;
    iUnsignedShortArray[1] = 0x00;
    iErrorCount = 0;
    mbus->FromByteArray(&iAddress, iUnsignedShortArray, maxshortcount, &cBuff[0], &iErrorCount);
    if (iAddress == 0x01 && iUnsignedShortArray[0] == 0x400 && iUnsignedShortArray[1] == 0x018 && iErrorCount == 0x00)
    {
        std::cout << "ToByteArray using the array call with 2 elements returned a valid frame for the 16-bit integer in ring buffer test." << std::endl;
    }
    else
    {
        std::cout << "ToByteArray using the array call with 2 elements failed to return a valid frame for the 16-bit integer in ring buffer test." << std::endl;
        return 0;
    }

    /////////////////////////////////////////////////////////////////////
    // Write data and sequentially check to see if there is a complete
    // frame stored in the buffer
    /////////////////////////////////////////////////////////////////////
    // Construct the frame for a 16-bit integer
    iAddress = 0x003;
    iUnsignedShortArray[0] = 1024;
    iUnsignedShortArray[1] = 24;
    iUnsignedShortArray[2] = 1023;
    iUnsignedShortArray[3] = 23;
    iBytesReturned = 0;
    mbus->ToByteArray(iAddress, iUnsignedShortArray, 4, maxbuffer, &cBuff[0], &iBytesReturned);
    if (iBytesReturned == 17)
    {
        std::cout << "ToByteArray using the array call with 4 elements returned the expected number of bytes in ring buffer sequential test." << std::endl;
    }
    else
    {
        std::cout << "ToByteArray using the array call with 4 elements failed to return the expected number of bytes in ring buffer sequential test." << std::endl;
        return 0;
    }

    // Write the data and try to convert after each byte is written (C# friendly implementation)
    iTemp = 0;
    iErrorCount = 10;
    iAddress = 0x00;
    iUnsignedShortArray[0] = 0;
    iUnsignedShortArray[1] = 0;
    iUnsignedShortArray[2] = 0;
    iUnsignedShortArray[3] = 0;
    iShortCount = 4;
    while (iErrorCount > 0 && iTemp < 17)
    {

        mbus->writeRingBuff(cBuff[iTemp],
            iUnsignedShortArray,
            iShortCount);
        iErrorCount = mbus->iGetErrorCount();
        ++iTemp;

    }
    iAddress = mbus->iGetAddress();
    if (iUnsignedShortArray[0] == 1024 && iUnsignedShortArray[1] == 24 && iUnsignedShortArray[2] == 1023 && iUnsignedShortArray[3] == 23 && iAddress == 0x03)
    {
        std::cout << "writeRingBuff 4 elements (c# friendly) with arguments returned the expected values." << std::endl;
    }
    else
    {
        std::cout << "writeRingBuff 4 elements (c# friendly) with arguments failed to return the expected values." << std::endl;
        return 0;
    }

    /////////////////////////////////////////////////////////////////////
    // Write data and sequentially check to see if there is a complete
    // frame stored in the buffer
    /////////////////////////////////////////////////////////////////////
    // Construct the frame for a 16-bit integer
    iAddress = 0x001;
    iUnsignedShortArray[0] = 1024;
    iUnsignedShortArray[1] = 24;
    iBytesReturned = 0;
    mbus->ToByteArray(iAddress, iUnsignedShortArray, 2, maxbuffer, &cBuff[0], &iBytesReturned);
    if (iBytesReturned == 13)
    {
        std::cout << "ToByteArray using the array call with 2 elements returned the expected number of bytes in ring buffer sequential test." << std::endl;
    }
    else
    {
        std::cout << "ToByteArray using the array call with 2 elements failed to return the expected number of bytes in ring buffer sequential test." << std::endl;
        return 0;
    }

    // Write the data and try to convert after each byte is written (C# friendly implementation)
    iTemp = 0;
    iErrorCount = 10;
    iAddress = 0x00;
    iUnsignedShortArray[0] = 0;
    iUnsignedShortArray[1] = 0;
    iShortCount = 2;
    while (iErrorCount > 0 && iTemp < 13)
    {

        mbus->writeRingBuff(cBuff[iTemp], 
            iUnsignedShortArray,
            iShortCount);
        iErrorCount = mbus->iGetErrorCount();
        ++iTemp;

    }
    if (iUnsignedShortArray[0] == 1024 && iUnsignedShortArray[1] == 24)
    {
        std::cout << "writeRingBuff (c# friendly) with arguments returned the expected values." << std::endl;
    }
    else
    {
        std::cout << "writeRingBuff (c# friendly) with arguments failed to return the expected values." << std::endl;
        return 0;
    }

    // Write the data and try to convert after each byte is written (C# friendly implementation).
    // This loops through the data many times to ensure the ring buffer indexing is working
    // correctly
    iAddress = 0x00;
    iShortCount = 2;
    mbus->clearRingBuff();
    for (int iTemp2 = 0; iTemp2 < 100000; ++iTemp2)
    {

        iTemp = 0;
        iErrorCount = 10;
        iUnsignedShortArray[0] = 0;
        iUnsignedShortArray[1] = 0;
        while (iErrorCount > 0 && iTemp < 13)
        {

            mbus->writeRingBuff(cBuff[iTemp],
                iUnsignedShortArray,
                iShortCount);
            iErrorCount = mbus->iGetErrorCount();
            ++iTemp;

        }
        if (iUnsignedShortArray[0] != 1024 || iUnsignedShortArray[1] != 24)
        {
            std::cout << "writeRingBuff (c# friendly) with arguments, multiple loop, failed to return the expected values." << std::endl;
            return 0;
        }

    }
    std::cout << "writeRingBuff (c# friendly) with arguments, multiple loop, succeeded." << std::endl;

    // Write the data and try to convert after each byte is written (Cpp friendly implementation)
    iTemp = 0;
    iErrorCount = 10;
    iAddress = 0x00;
    iUnsignedShortArray[0] = 0;
    iUnsignedShortArray[1] = 0;
    iShortCount = 2;
    while (iErrorCount > 0  && iTemp < 13)
    {

        iErrorCount = 0;
        mbus->writeRingBuff(cBuff[iTemp], &iAddress,
            iUnsignedShortArray,
            iShortCount,
            &iErrorCount);
        ++iTemp;

    }
    if (iUnsignedShortArray[0] == 1024 && iUnsignedShortArray[1] == 24)
    {
        std::cout << "writeRingBuff (cpp friendly) with arguments returned the expected values." << std::endl;
    }
    else
    {
        std::cout << "writeRingBuff (cpp friendly) with arguments failed to return the expected values." << std::endl;
        return 0;
    }

    /////////////////////////////////////////////////////////////////////
    // If the code makes it this far, it must have worked
    /////////////////////////////////////////////////////////////////////

    std::cout << "All tests completed successfully!!!" << std::endl;

    return 0;
}


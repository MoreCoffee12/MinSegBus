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
    unsigned short iUnsignedShort;
    unsigned short iUnsignedShortArray[maxshortcount];
    unsigned char cBuff[maxbuffer];
    MinSegBus *mbus = new MinSegBus();
    unsigned int iBytesReturned;
    unsigned int iErrorCount;
    unsigned int iIdx;
    float fValue;
    float fValueArray[maxfloatcount];

    /////////////////////////////////////////////////////////////////////
    // Test harness section for the deconstruction of a frame for 
    // a 16-bit integer
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
        //return 0;
    }

    // Test the ability of the code to retrieve the 16-bit value of 4-element integer array
    cBuff[0] = 0x00;
    cBuff[1] = 0x00;
    cBuff[2] = 0x11;
    cBuff[3] = 0x01;
    cBuff[4] = 0x01;
    cBuff[5] = 0xD3;
    cBuff[6] = 0x00;
    cBuff[7] = 0x06;
    cBuff[8] = 0x00;
    cBuff[9] = 0x00;
    cBuff[10] = 0x00;
    cBuff[11] = 0xE8;
    cBuff[12] = 0x00;
    cBuff[13] = 0x74;
    cBuff[14] = 0xCB;
    cBuff[15] = 0x00;
    cBuff[16] = 0x00;
    iErrorCount = 0;
    iUnsignedShort = 0;
    mbus->FromByteArray(&iAddress, iUnsignedShortArray, 4, &cBuff[0], &iErrorCount);
    if (iErrorCount == 0x00 && iUnsignedShortArray[0] == 0x15C)
    {
        std::cout << "Successfully retrieved the 4-element 16-bit integer array." << std::endl;
    }
    else
    {
        std::cout << "Failed to retrieved the 4-element 16-bit integer array." << std::endl;
        //return 0;
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

    std::cout << "All tests completed successfully!!!" << std::endl;

    return 0;
}


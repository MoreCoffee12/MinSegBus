// MinSegBusTestHarness : Validate the functions for the MinSegBus Libraray.
//

#include <minsegbus.h>

MinSegBus mbus;

#define maxbuffer 0x0400
#define maxshortcount 0x000A
#define maxfloatcount 0x000A

void setup()
{
  Serial.begin(57600);
  Serial.println("Starting test.");
  delay(500);
}

void loop()
{
	

    unsigned char iAddress;
    unsigned char cTemp;
    unsigned short iUnsignedShort;
    unsigned short iUnsignedShortArray[maxshortcount];
    unsigned char cBuff[maxbuffer];
    unsigned int iBytesReturned;
    unsigned int iErrorCount;
    unsigned int iIdx;
    unsigned int iShortCount;
    int iTemp;
    float fValue;
    float fValueArray[maxfloatcount];

    /////////////////////////////////////////////////////////////////////
    // Test harness section for the deconstruction of a frame for 
    // a 16-bit integer
    /////////////////////////////////////////////////////////////////////

    // Construct the frame for a 16-bit integer
    iAddress = 0x001;
    iUnsignedShort = 1024;
    iBytesReturned = 0;
    mbus.ToByteArray(iAddress, &iUnsignedShort, 1, maxbuffer, &cBuff[0], &iBytesReturned);
    if ( iBytesReturned == 11)
    {
        Serial.println("Step 7 OK.");
    }
    else
    {
        Serial.println("Step 7 NOT OK.");
        Serial.println("");
        delay(2000);
        return;
    }
    delay(100);

    // Is the returned byte array correct?
    iAddress = 0x00;
    iUnsignedShort = 0x00;
    mbus.FromByteArray(&iAddress, &iUnsignedShort, 1, &cBuff[0], &iErrorCount);
    if (iAddress == 0x01 && iUnsignedShort == 0x400 && iErrorCount == 0x00)
    {
        Serial.println("Step 8 OK.");
    }
    else
    {
        Serial.println("Step 8 NOT OK.");
        Serial.println("");
        delay(2000);
        return;
    }
    delay(100);
    
    /////////////////////////////////////////////////////////////////////
    // Test harness section for the deconstruction of a frame for 
    // a 16-bit integer using the array, but with a two elements
    /////////////////////////////////////////////////////////////////////

    // Construct the frame for a 16-bit integer
    iAddress = 0x001;
    iUnsignedShortArray[0] = 1024;
    iUnsignedShortArray[1] = 24;
    iBytesReturned = 0;
    mbus.ToByteArray(iAddress, iUnsignedShortArray, 2, maxbuffer, &cBuff[0], &iBytesReturned);
    if (iBytesReturned == 13)
    {
        Serial.println("Step 9 OK.");
    }
    else
    {
        Serial.println("Step 9 NOT OK.");
        Serial.println("");
        delay(2000);
        return;
    }
    delay(100);

    // Is the returned byte array correct?
    iAddress = 0x00;
    iUnsignedShortArray[0] = 0x00;
    iUnsignedShortArray[1] = 0x00;
    mbus.FromByteArray(&iAddress, iUnsignedShortArray, maxshortcount, &cBuff[0], &iErrorCount);
    if (iAddress == 0x01 && iUnsignedShortArray[0] == 0x400 && iUnsignedShortArray[1] == 0x018 && iErrorCount == 0x00)
    {
        Serial.println("Step 10 OK.");
    }
    else
    {
        Serial.println("Step 10 NOT OK.");
        Serial.println("");
        delay(2000);
        return;
    }
    delay(100);
  
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
    mbus.ToByteArray(iAddress, iUnsignedShortArray, 2, maxbuffer, &cBuff[0], &iBytesReturned);
    if (iBytesReturned == 13)
    {
        Serial.println("Step 11 OK.");
    }
    else
    {
        Serial.println("Step 11 NOT OK.");
        Serial.println("");
        delay(2000);
        return;
    }
    delay(100);

    // Is the returned byte array correct?
    iAddress = 0x00;
    iUnsignedShortArray[0] = 0x00;
    iUnsignedShortArray[1] = 0x00;
    iErrorCount = 0;
    mbus.FromByteArray(&iAddress, iUnsignedShortArray, maxshortcount, &cBuff[0], &iErrorCount);
    if (iAddress == 0x01 && iUnsignedShortArray[0] == 0 && iUnsignedShortArray[1] == 0 && iErrorCount == 0x00)
    {
        Serial.println("Step 12 OK.");
    }
    else
    {
        Serial.println("Step 12 NOT OK.");
        Serial.println("");
        delay(2000);
        return;
    }
    delay(100);
  
    /////////////////////////////////////////////////////////////////////
    // Test harness section for the deconstruction of a frame for 
    // a 32-bit decimal number (float)
    /////////////////////////////////////////////////////////////////////

    // Construct the frame for a 32-bit float
    iAddress = 0x001;
    fValue = 3.14159f;
    iBytesReturned = 0;
    mbus.FloatToByteArray(iAddress, &fValue, 1, maxbuffer, cBuff, &iBytesReturned);
    if (iBytesReturned == 13)
    {
        Serial.println("Step 13 OK.");
    }
    else
    {
        Serial.println("Step 13 NOT OK.");
        Serial.println("");
        delay(2000);
        return;
    }
    delay(100);

    // Is the returned byte array correct?
    iAddress = 0x00;
    fValue = 0.00;
    mbus.FloatFromByteArray(&iAddress, &fValue, 0x01, &cBuff[0], &iErrorCount);
    if (iAddress == 0x01 && fValue == 3.14159f && iErrorCount == 0x00)
    {
        Serial.println("Step 14 OK.");
    }
    else
    {
        Serial.println("Step 14 NOT OK.");
        Serial.println("");
        delay(2000);
        return;
    }
    delay(100);

    /////////////////////////////////////////////////////////////////////
    // Test harness section for the deconstruction of a frame for 
    // a 32-bit decimal number (float)
    /////////////////////////////////////////////////////////////////////

    // Construct the frame for a 32-bit float
    iAddress = 0x001;
    fValueArray[0] = 3.14159f;
    fValueArray[1] = 6.14159f;
    iBytesReturned = 0;
    mbus.FloatToByteArray(iAddress, fValueArray, 2, maxbuffer, cBuff, &iBytesReturned);
    if (iBytesReturned == 17)
    {
        Serial.println("Step 15 OK.");
    }
    else
    {
        Serial.println("Step 15 NOT OK.");
        Serial.println("");
        delay(2000);
        return;
    }

    // Is the returned byte array correct?
    iAddress = 0x00;
    fValueArray[0] = 0.00f;
    fValueArray[1] = 0.00f;
    mbus.FloatFromByteArray(&iAddress, fValueArray, 0x02, cBuff, &iErrorCount);
    if (iAddress == 0x01 && fValueArray[0] == 3.14159f && fValueArray[1] == 6.14159f && iErrorCount == 0x00)
    {
        Serial.println("Step 16 OK.");
    }
    else
    {
        Serial.println("Step 16 NOT OK.");
        Serial.println("");
        delay(2000);
        return;
    }
    
    /////////////////////////////////////////////////////////////////////
    // Next, test that a single value can be read and written successfully
    /////////////////////////////////////////////////////////////////////
    mbus.writeRingBuff(0x03);
    cBuff[0] = mbus.readRingBuff(0x00);

    if (cBuff[0] == 0x03)
    {
        Serial.println("Step 17 OK.");
        delay(100);
    }
    else
    {
        Serial.println("Step 17 NOT OK.");
        Serial.println("");
        delay(2000);
        return;
    }
    
    /////////////////////////////////////////////////////////////////////
    // Write and read a complete sequence
    /////////////////////////////////////////////////////////////////////
    mbus.clearRingBuff();
    for (cTemp = mbus.iGetRingBuffCount(); cTemp > 0; cTemp--)
    {
        mbus.writeRingBuff(cTemp);
    }

    // Check the first value
    if (mbus.readRingBuff(0x00) != 0x01 )
    {
        Serial.println("Step 18 NOT OK.");
        Serial.println("");
        delay(2000);
        return;
    }
    // Check the last value
    if (mbus.readRingBuff(mbus.iGetRingBuffCount() - 1) != mbus.iGetRingBuffCount())
    {
        Serial.println("Step 18 NOT OK.");
        Serial.println("");
        delay(2000);
        return;
    }
    Serial.println("Step 18 OK.");
    delay(100);
    
     /////////////////////////////////////////////////////////////////////
    // Write and read a complete frame of data
    /////////////////////////////////////////////////////////////////////
    mbus.clearRingBuff();

    // Construct the frame for a 16-bit integer
    iAddress = 0x001;
    iUnsignedShortArray[0] = 1024;
    iUnsignedShortArray[1] = 24;
    iBytesReturned = 0;
    mbus.ToByteArray(iAddress, iUnsignedShortArray, 2, maxbuffer, &cBuff[0], &iBytesReturned);
    if (iBytesReturned == 13)
    {
        Serial.println("Step 19 OK.");
        delay(100);
    }
    else
    {
        Serial.println("Step 19 NOT OK.");
        Serial.println("");
        delay(2000);
        return;
    }

    // Write the frame to the ring buffer and clear the buffer
    for (iTemp = 0; iTemp < (int)iBytesReturned; iTemp++)
    {
        mbus.writeRingBuff(cBuff[iTemp]);
        cBuff[iTemp] = 0x00;
    }

    // Read the data back
    for (iTemp = 0; iTemp < (int)iBytesReturned; iTemp++)
    {
        cBuff[iTemp] = mbus.readRingBuff((unsigned char)iBytesReturned - iTemp - 1);
    }

    // Is the returned byte array correct?
    iAddress = 0x00;
    iUnsignedShortArray[0] = 0x00;
    iUnsignedShortArray[1] = 0x00;
    iErrorCount = 0;
    mbus.FromByteArray(&iAddress, iUnsignedShortArray, maxshortcount, &cBuff[0], &iErrorCount);
    if (iAddress == 0x01 && iUnsignedShortArray[0] == 0x400 && iUnsignedShortArray[1] == 0x018 && iErrorCount == 0x00)
    {
        Serial.println("Step 20 OK.");
        delay(100);
    }
    else
    {
        Serial.println("Step 20 NOT OK.");
        Serial.println("");
        delay(2000);
        return;
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
    mbus.ToByteArray(iAddress, iUnsignedShortArray, 2, maxbuffer, &cBuff[0], &iBytesReturned);
    if (iBytesReturned == 13)
    {
        Serial.println("Step 21 OK.");
        delay(100);
    }
    else
    {
        Serial.println("Step 21 NOT OK.");
        Serial.println("");
        delay(2000);
        return;
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

        mbus.writeRingBuff(cBuff[iTemp], 
            iUnsignedShortArray,
            iShortCount);
        iErrorCount = mbus.iGetErrorCount();
        ++iTemp;

    }
    if (iUnsignedShortArray[0] == 1024 && iUnsignedShortArray[1] == 24)
    {
        Serial.println("Step 22 OK.");
        delay(100);
    }
    else
    {
        Serial.println("Step 22 NOT OK.");
        Serial.println("");
        delay(2000);
        return;
    }

    // Write the data and try to convert after each byte is written (C# friendly implementation).
    // This loops through the data many times to ensure the ring buffer indexing is working
    // correctly
    iAddress = 0x00;
    iShortCount = 2;
    for (int iTemp2 = 0; iTemp2 < 100; ++iTemp2)
    {

        iTemp = 0;
        iErrorCount = 10;
        iUnsignedShortArray[0] = 0;
        iUnsignedShortArray[1] = 0;
        while (iErrorCount > 0 && iTemp < 13)
        {

            mbus.writeRingBuff(cBuff[iTemp],
                iUnsignedShortArray,
                iShortCount);
            iErrorCount = mbus.iGetErrorCount();
            ++iTemp;

        }
        if (iUnsignedShortArray[0] != 1024 || iUnsignedShortArray[1] != 24)
        {
          Serial.println("Step 23 NOT OK.");
          Serial.println("");
          delay(2000);
          return;
        }

    }
    Serial.println("Step 23 OK.");
    delay(100);


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
        mbus.writeRingBuff(cBuff[iTemp], &iAddress,
            iUnsignedShortArray,
            iShortCount,
            &iErrorCount);
        ++iTemp;

    }
    if (iUnsignedShortArray[0] == 1024 && iUnsignedShortArray[1] == 24)
    {
        Serial.println("Step 24 OK.");
        delay(100);
    }
    else
    {
        Serial.println("Step 24 NOT OK.");
        Serial.println("");
        delay(2000);
        return;
    }    
    
    // End of tests
    Serial.println("All tests completed successfully!!!");
    Serial.println("");
    delay(2000);
 

    return;
}

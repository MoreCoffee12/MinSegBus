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
    unsigned short iUnsignedShort;
    unsigned short iUnsignedShortArray[maxshortcount];
    unsigned char cBuff[maxbuffer];
    unsigned int iBytesReturned;
    unsigned int iErrorCount;
    unsigned int iIdx;
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
    
    Serial.println("All tests completed successfully!!!");
    Serial.println("");
    
    delay(2000);
    
    return;
}

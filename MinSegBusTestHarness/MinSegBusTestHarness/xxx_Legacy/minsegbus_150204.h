#ifndef MinSegBus_h
#define MinSegBus_h

class MinSegBus
{
public:
    MinSegBus();
    ~MinSegBus();

    void ToByteArray(unsigned char iAddress, 
                        unsigned short iUnsignedShort,
                        unsigned int iBufferLength,
                        unsigned char *cBuff,
                        unsigned int *iBytesReturned);

    void ToByteArray(unsigned char iAddress,
        float fValue,
        unsigned int iBufferLength,
        unsigned char *cBuff,
        unsigned int *iBytesReturned);

    void FromByteArray(unsigned char *iAddress,
        unsigned short *iUnsignedShort,
        unsigned char *cBuff,
        unsigned int *iErrorCount);

    void FromByteArray(unsigned char *iAddress,
        float *fValue,
        unsigned char *cBuff,
        unsigned int *iErrorCount);


private:
    unsigned short _bUpdateCRC(unsigned short crc, unsigned char data);
};



#endif
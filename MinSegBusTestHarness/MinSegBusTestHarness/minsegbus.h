#ifndef MinSegBus_h
#define MinSegBus_h

class MinSegBus
{
public:
    MinSegBus();
    ~MinSegBus();

    void ToByteArray(unsigned char iAddress,
        unsigned short *iUnsignedShort,
        unsigned int iShortCount,
        unsigned int iBufferLength,
        unsigned char *cBuff,
        unsigned int *iBytesReturned);

    void FloatToByteArray(unsigned char iAddress,
        float *fValue,
        unsigned int iFloatCount,
        unsigned int iBufferLength,
        unsigned char *cBuff,
        unsigned int *iBytesReturned);

    void FromByteArray(unsigned char *iAddress,
        unsigned short *iUnsignedShortArray,
        unsigned int iMaxShortCount,
        unsigned char *cBuff,
        unsigned int *iErrorCount);

    void FloatFromByteArray(unsigned char *iAddress,
        float *fValueArray,
        unsigned int iMaxFloatCount,
        unsigned char *cBuff,
        unsigned int *iErrorCount);


private:
    unsigned short _bUpdateCRC(unsigned short crc, unsigned char data);

    bool _bCreateFrontFrame(unsigned int iByteCount, unsigned char iAddress,
        unsigned char cType, unsigned char *cBuff, unsigned int *idx);

    bool _bCreateBackFrame(unsigned char *cBuff, unsigned int *idx);

    void _bIsFrameValid(unsigned char *cBuff,
        unsigned int *iErrorCount, unsigned int *iFrameSize);
};



#endif
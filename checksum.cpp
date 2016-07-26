#include "checksum.h"
#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"


/*
 data checksums
 The checksums are a mess. There are four "types" of checksums:
 Type 0: this is a CRC16 checksum of the first 0x1E unsigned chars of sector 0. The checksum itself is stored in block 0x01, offset 0x0E.
 
 Type 1: this is a CRC16 checksum of the data area header. As there are two data areas, there are two of these checksums.
 One is at block 0x08, offset 0x0E, and the other is at block 0x24, offset 0x0E.
 
 Type 2: this is a CRC16 checksum of the data area. As there are two data areas, there are two of these checksums.
 One is at block 0x08, offset 0x0C, and the other is at block 0x24, offset 0x0C.
 
 Type 3: this is another CRC16 checksum of the data area, except padded with zeroes. As there are two data areas,
 there are two of these checksums. One is at block 0x08, offset 0x0A, and the other is at block 0x24, offset 0x0A.
 
 As type 0 is a checksum of a *supposedly* read-only sector, it's not all that important. It's also very straightforward to understand.
 
 The type 1 checksum is a checksum of just one block, the data area header (0x08 and 0x24). As it's also stored WITHIN the
 data area header, a default value must be supplied for the checksum before actually calculating it. That value is 0x0005.
 
 The type 2 checksum is actually only a checksum of the first 4 blocks (EXCLUDING the data area header, and the access control blocks).
 
 The type 3 checksum is a checksum of the next 4 blocks (EXCLUDING the data area header, and the access control blocks),
 and then 0x0E blocks of zeroes.
 
 Just to re-iterate, the encryption is applied AFTER all this checksum mess is done.
 */

// CCITT CRC Code
// Update the CRC for transmitted and received data using
// the CCITT 16bit algorithm (X^16 + X^12 + X^5 + 1).
unsigned short Checksum::UpdateCcittCrc16(unsigned short crc16, unsigned char data)
{
    unsigned short num2 = (unsigned short) (data << 8);
    for (unsigned int i = 0; i < 8; i++)
    {
        int num3;
        if ((crc16 ^ num2) > 0x7fff)
        {
            num3 = 1;
        }
        else
        {
            num3 = 0;
        }
        crc16 = (unsigned short) ((crc16 << 1) ^ (num3 * 0x1021));
        num2 = (unsigned short) (num2 << 1);
    }
    return crc16;
}

unsigned short Checksum::ComputeCcittCrc16(void const* data, unsigned int bytes)
{
    unsigned short crc = 0xffff;
    unsigned char const* numPtr = (unsigned char const*)data;
    for (unsigned int i = 0; i < bytes; i++)
    {
        printf("%02X ",*((unsigned char*) (numPtr + i)));
        crc = UpdateCcittCrc16(crc, *((unsigned char const*) (numPtr + i)));
    }
    printf("\n");
    return crc;
}






bool Checksum::getChecksumParameters(int checksumType, unsigned int* checksumOffset, unsigned int* dataOffset, unsigned int* dataLength)
{
    switch (checksumType)
    {
        case 0:
            // Type 0 checksum.
            *checksumOffset = 0x1E; // The checksum itself is stored in block 0x01, offset 0x0E.
            *dataOffset = 0x00;
            *dataLength = 0x1E; // checksum of the first 0x1E unsigned chars of sector 0.
            break;
            
        case 1:
            // Type 1 checksum.
            // Type 1: this is a CRC16 checksum of the data area header. As there are two data areas,
            // there are two of these checksums. One is at block 0x08, offset 0x0E, and the other is at block 0x24, offset 0x0E.
            *checksumOffset = 0x0E; // Checksum is stored within the data header block.
            *dataOffset = 0x00;
            *dataLength = 0x10; // The type 1 checksum is a checksum of just one block, the data area header (blocks 0x08 and 0x24).
            break;
            
        case 2:
            // Type 2 checksum.
            // Type 2: this is a CRC16 checksum of the data area. As there are two data areas, there are two of these checksums.
            // One is at block 0x08, offset 0x0C, and the other is at block 0x24, offset 0x0C.
            *checksumOffset = 0x0C;
            *dataOffset = 0x10;
            *dataLength = 0x40; // Checksum of the first 4 blocks (EXCLUDING the data area header, and the access control blocks).
            break;
            
        case 3:
            // Type 3 checksum.
            // Type 3: this is another CRC16 checksum of the data area, except padded with zeroes. As there are two data areas,
            // there are two of these checksums. One is at block 0x08, offset 0x0A, and the other is at block 0x24, offset 0x0A.
            // The type 3 checksum is a checksum of the next 4 blocks  after the type 2 checksum
            // (EXCLUDING the data area header, and the access control blocks), and then 0x0E blocks of zeroes.
            *checksumOffset = 0x0A;
            *dataOffset = 0x50;
            *dataLength = 0x160;// 0x40;
            break;
            
        default:
            return false;
    }
    return true;
}








bool Checksum::computeChecksum(int type, void const* memoryIn, unsigned short* checksum, bool isTrap)
{
    unsigned int startBlock;
    unsigned int cntBlock;
    unsigned int block;
    unsigned char const* numPtr = (unsigned char const*)memoryIn;
    unsigned char const* toyID1 = (unsigned char const*)memoryIn;
    
    //memcpy(toyID1, memoryIn, 0x02);
    
    Crypt crypt;
    
    if ((type == 0) || (type == 1))
    {
        unsigned int dataLength;
        unsigned int dataOffset;
        unsigned int checksumOffset;
        
        if (!getChecksumParameters(type, &checksumOffset, &dataOffset, &dataLength))
        {
            return false;
        }
        if (type == 1)
        {
            unsigned char header[0x10];
            memcpy(header, (void const*) (numPtr + dataOffset), 0x10);
            *(header + 14) = 5;
            *(header + 15) = 0;
            printf("****************************HEADER :%X\n\n",*(header + 1));
            *checksum = ComputeCcittCrc16((void const*) &header, dataLength);
        }
        else
        {
            *checksum = ComputeCcittCrc16((void const*) (numPtr + dataOffset), dataLength);
        }
        return true;
    }
    if (type == 2)
    {
        startBlock = 1; // comprends + 0x08 ou + 0x24
        cntBlock = 4;
    }
    else if (type == 3)
    {
        startBlock = 5;
        
        if (isTrap) {
            cntBlock = 22;
        } else {
            cntBlock = 4;
        }
    }
    else
    {
        return false;
    }
    numPtr += (startBlock * 0x10);
    *checksum = 0xffff;
    block = startBlock;
    while(true)
    {
        if (block >= (startBlock + cntBlock))
        {
            if (type != 3)
            {
                return true;
            } // else if type == 3
            block = startBlock + cntBlock;
            break;
        }
        if (!crypt.IsAccessControlBlock(block))
        {
            printf("block:%02X: ",numPtr);
            for (unsigned int i = 0; i < 0x10; i++)
            {
                printf("%02X ",*((unsigned char*) (numPtr + i)));
                *checksum = UpdateCcittCrc16(*checksum, *((unsigned char*) (numPtr + i)));
            }
            printf("\n");
        }
        numPtr += 0x10;
        block++;
    }
    
    
    // Pad Type 3 checksum with 0x0E blocks of zeroes (BAD IDEA for BOBOX :)
    while (block < 0x1c) {
        if (!crypt.IsAccessControlBlock(block))
        {
            printf("padding with 0: ");
            for (unsigned int j = 0; j < 0x10; j++)
            {
                printf("%02X ",0);
                *checksum = UpdateCcittCrc16(*checksum, 0);
            }
            printf("\n");
        }
        block++;
    }
    
    return true;
}

// validateChecksum
// buffer: pointer to entire decrypted character as single chunk of memory
// type:   checksum type
// dataArea:  dataArea to validate.
//            A value of 0 indicates the first data area starting at 0x08
//            A value of 1 indicates the second data area starting at 0x24
// overwrite: if true, replace checksum in buffer with newly computed checksum
//
// returns true if old checksum in buffer matches computed checksum.
bool Checksum::validateChecksum(unsigned char *buffer, int type, int dataArea, bool overwrite)
{
    unsigned int checksumOffset;
    unsigned int areaSequenceOffset = 0x00;
    unsigned short computedChecksum;
    unsigned int dataLength;
    unsigned int dataOffset;
    unsigned int offset = 0;
    unsigned char *ptr;
    bool match;
    unsigned int compteur;
    
    
    printf("\n------ validateChecksum type=%i dataAera=%i overwrite=%i ------------------\n",type,dataArea,overwrite);
    
    /*
     printf("\nbuffer(%lu)=",sizeof(buffer));
     for(compteur = 0; compteur <= 0x20; compteur++) {
     printf("%02X ",buffer[compteur]);
     }
     printf("\n");
     */
    
    if (!getChecksumParameters(type, &checksumOffset, &dataOffset, &dataLength))
    {
        return false;
    }
    
    if (type != 0)
    {
        int dataAreaBlock;
        if (dataArea == 0)
        {
            dataAreaBlock = 0x08;
        }
        else
        {
            dataAreaBlock = 0x24;
        }
        offset += (unsigned int) (dataAreaBlock * 0x10);
    }
    
    ptr = buffer + offset;
    
    if(overwrite && type == 1) {
        // Before computing checksum 1 (and after computing checksum 2 and 3)
        // update sequence number.
        areaSequenceOffset = 0x09;
        ptr[areaSequenceOffset]++;  // increment sequence
    }
    
    //printf("%soffset=%X areaSequenceOffset=%i Offset=%06X DataOffset=%06X DataLenght=%06X  %s\n",KCYN,offset,areaSequenceOffset, checksumOffset,dataOffset,dataLength,KWHT);
    
    
    //printf("%stype=%i \n%s",KCYN,type, KWHT);
    printf("\nTOY = ");printf("%02X ",buffer[0x10]);printf("%02X \n",buffer[0x11]);
    
    
    if (( type > 2 && type < 5 && buffer[0x11] == 0x00) && ((buffer[0x10] >= 0xD2 && buffer[0x10] <= 0xDC) && buffer[0x11] == 0x00)) { // type 4 est une evolution du type 3 sur les Traps
        printf("%sTRAP DETECED, Using NEW type 3 CRC...%s\n",KCYN,KWHT);
        if (!computeChecksum(type, ptr, &computedChecksum, true)) return false; // NEW TYPE 3 for TRAPS
    } else {
        if (!computeChecksum(type, ptr, &computedChecksum, false)) return false; // STANDARD TYPE 3 with PADDING
    }
    
    
    unsigned short oldChecksum = (unsigned short) ((ptr[checksumOffset] & 0xff) | ((ptr[(int) (checksumOffset + 1)] & 0xff) << 8));
    printf("oldChecksum=%s%4X%s computedChecksum=%s%4X%s\n",KMAG,oldChecksum,KWHT,KYEL,computedChecksum,KWHT);
    match = (oldChecksum == computedChecksum);
    
    if(overwrite) {
        // overwrite old value with newly computed checksum
        ptr[checksumOffset] = computedChecksum & 0xff;
        ptr[checksumOffset+1] = (computedChecksum >> 8) & 0xff;
    }
    
    return match;
}

bool Checksum::ValidateAllChecksums(unsigned char *buffer, bool overwrite)
{
    bool OK = true;
    bool res;
    int dataArea;
    int type;
    // When computing checksums for overwrite, they have to be done in the following order.
    // Compute checksum 3 and 2, then increment the area sequence number by 1,
    // then compute checksum 1.
    //
    // In the logic below, the area sequence number is set just prior to computing checksum 1.
    for(dataArea = 0; dataArea <= 1; dataArea++) {
        for(type = 3; type >=0; type--) {
            res = validateChecksum(buffer, type, dataArea, overwrite);
            if(!res && !overwrite) {
                fprintf(stderr, "Checksum %sfailure%s for checksum type %d, data area %d\n\n",KRED,KWHT, type, dataArea );
            } else {
                fprintf(stderr, "Checksum %sOK%s for checksum type %d, data area %d\n\n",KGRN,KWHT, type, dataArea);
            }
            OK = OK && res;
        }
    }
    return OK;
}











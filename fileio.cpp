
#include "fileio.h"
#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"

void SkylanderIO::fprinthex(FILE *f, unsigned char *c, unsigned int n) {
    unsigned int h,i;
    unsigned char j;
    
    for (h=0; h<n; h+=16) {
        
        fprintf (f,"%04x: ",h);
        
        for (i=0; i<16; i++) {
            if (i+h < n)
                fprintf (f,"%02x ",*(c+i+h) & 0xff);
            else
                fprintf (f,"   ");
        }
        for (i=0; i<16; i++) {
            if (i+h < n) {
                j = *(c+i+h);
                if (j<32) j='.';
                if (j>=127) j='.';
                fprintf (f,"%c",j);
            } else
                fprintf(f," ");
        }
        fprintf(f,"\n");
    }
}


SkylanderIO::SkylanderIO ()
{
    sky = NULL;
    buffer=new unsigned char [1025];
}

SkylanderIO::~SkylanderIO ()
{
    delete buffer;
}


void SkylanderIO::initWithUnencryptedFile(char * name) throw (int)
{
    if (!sky) {
        ReadSkylanderFile(name);
        sky = new Skylander(buffer);
    }
}

void SkylanderIO::initWithEncryptedFile(char * name) throw (int)
{
    if (!sky) {
        ReadSkylanderFile(name);
        DecryptBuffer(buffer);
        sky = new Skylander(buffer);
    }
}

void SkylanderIO::initWithPortal(int number) throw (int) {
    
    
    if (!sky) {
        //printf("Reading Skylander from portal.\n");
        ReadPortal(buffer,number);
        DecryptBuffer(buffer);
        sky = new Skylander(buffer);
        // printf("\nSkylander read from portal.\n");
        
    }
}

void SkylanderIO::ReadPortal(unsigned char *s, int number) throw (int)
{
    
    unsigned char data[0x10];
    unsigned char *ptr;
    
    PortalIO *port;
    
    port = new PortalIO();
    
    // must start with a read of block zero
    port->ReadBlock(0, data, number);
    
    // I don't know that we need this, but the web driver sets the color when reading the data
    port->SetPortalColor(0xC8, 0xC8, 0xC8);
    
    ptr = s;
    memcpy(ptr, data, sizeof(data));
    
    for(int block=1; block < 0x40; ++block) {
        ptr += 0x10;
        port->ReadBlock(block, data, number);
        memcpy(ptr, data, sizeof(data));
    }
    
    delete port;
}

bool SkylanderIO::writeSkylanderToPortal(int number) throw (int)
{
    bool bResult;
    bool bNewSkylander = false;
    unsigned char data[0x10];
    
    unsigned char old[1024];
    unsigned char skydata[1024];
    
    Crypt crypt;
    
    if (sky) {
        
        PortalIO *port;
        
        ReadPortal(old,number);
        
        memcpy (skydata,sky->getData(),1024);
        
        EncryptBuffer(skydata);
        
        printf("\nWriting Skylander to portal.\n");
        
        port = new PortalIO();
        
        
        for(int i=0; i<2; i++) {
            // two pass write
            // write the access control blocks first
            bool selectAccessControlBlock;
            if(i == 0) {
                selectAccessControlBlock = 1;
            } else {
                selectAccessControlBlock = 0;
            }
            
            for(int block=0; block < 0x40; ++block) {
                bool changed, OK;
                int offset = block * BLOCK_SIZE;
                if(crypt.IsAccessControlBlock(block) == selectAccessControlBlock) {
                    changed = (memcmp(old + offset, skydata+offset,BLOCK_SIZE) != 0);
                    if(changed) {
                        port->WriteBlock( block, skydata+offset, bNewSkylander);
                    }
                }
            }
        }
        
        return true;
    }
    return false;
}


bool SkylanderIO::writeSkylanderToUnencryptedFile(char *name) throw (int)
{
    if (sky) {
        WriteSkylanderFile(name,sky->getData());
    }
}

bool SkylanderIO::writeSkylanderToEncryptedFile(char *name) throw (int)
{
    if (sky) {
        unsigned char skydata[1024];
        
        memcpy (skydata,sky->getData(),1024);
        EncryptBuffer(skydata);
        WriteSkylanderFile(name,skydata);
    }
    
}

// Encrypt entire buffer of character data
// Buffer is entire 1024 byte block of character data.

void SkylanderIO::EncryptBuffer(unsigned char* buffer) {
    unsigned int blockIndex;
    unsigned char* blockData;
    unsigned char const* tagBlocks0and1;
    
    unsigned int toyID = (buffer[0x10]) + ( buffer[0x11] * 0x100);
    printf("ENCRYPT BUFFER CALLED for %02X %02X (Toy N°%i) using Crypt Scheme %i\n",buffer[0x10], buffer[0x11], toyID, CryptScheme(toyID));
    
    tagBlocks0and1 = buffer;
    for(blockIndex = 0x08; blockIndex < 0x40; blockIndex++) {
        blockData = buffer + blockIndex * 0x10;
        bool DoCrypt = false;
        
        if (CryptScheme(toyID) == 1 && blockIndex >= 0x08 && blockIndex <= 0x15) DoCrypt = true;
        if (CryptScheme(toyID) == 1 && blockIndex >= 0x24 && blockIndex <= 0x31) DoCrypt = true;
        
        if (CryptScheme(toyID) == 0) DoCrypt = false;
        if (CryptScheme(toyID) == 8) DoCrypt = true;
        
        if (DoCrypt) {
            crypt.EncryptTagBlock(blockData, blockIndex, tagBlocks0and1);
        }
    }
}

// Decrypt entire buffer of character data
// Buffer is entire 1024 byte block of character data.
void SkylanderIO::DecryptBuffer(unsigned char* buffer) {
    unsigned int blockIndex;
    unsigned char* blockData;
    unsigned char const* tagBlocks0and1;
    
    unsigned int toyID = (buffer[0x10]) + ( buffer[0x11] * 0x100);
    printf("DECRYPT BUFFER CALLED for %02X %02X (Toy N°%i) using Crypt Scheme %i\n",buffer[0x10], buffer[0x11], toyID, CryptScheme(toyID));
    
    tagBlocks0and1 = buffer;
    for(blockIndex = 0x08; blockIndex < 0x40; blockIndex++) {
        blockData = buffer + blockIndex * 0x10;
        
        if (CryptScheme(toyID) == 9) {
            printf("%s UNKONWN TOY N°%i, WARNNING ON CRYPTED / NON-CRYPTED BLOCKS%s\n",KRED,toyID,KWHT);
            sleep(1);
            //exit(1);
        }

        bool DoDecrypt = false;
        
        if (CryptScheme(toyID) == 1 && blockIndex >= 0x08 && blockIndex <= 0x15) DoDecrypt = true;
        if (CryptScheme(toyID) == 1 && blockIndex >= 0x24 && blockIndex <= 0x31) DoDecrypt = true;
        
        if (CryptScheme(toyID) == 0) DoDecrypt = false;
        if (CryptScheme(toyID) == 8) DoDecrypt = true;
        
        if (DoDecrypt) {
            crypt.DecryptTagBlock(blockData, blockIndex, tagBlocks0and1);
        } else {
            printf("Nocrypt Tag Block %02X : ", blockIndex);
            for(int compteur = 0; compteur <= 15; compteur++) {
                printf("%02X ",blockData[compteur]);
            }
            printf("   =   ");
            for(int compteur = 0; compteur <= 15; compteur++) {
                printf("%02X ",blockData[compteur]);
            }
            printf("\n");
        }
    }
}


unsigned int SkylanderIO::CryptScheme(unsigned int toyID) {

switch (toyID) {


        
        //Code for Fileio.cpp
    case 0 : return 1;
    case 1 : return 1;
    case 2 : return 1;
    case 3 : return 1;
    case 4 : return 1;
    case 5 : return 1;
    case 6 : return 1;
    case 7 : return 1;
    case 8 : return 1;
    case 9 : return 1;
    case 10 : return 1;
    case 11 : return 1;
    case 12 : return 1;
    case 13 : return 1;
    case 14 : return 1;
    case 15 : return 1;
    case 16 : return 1;
    case 17 : return 1;
    case 18 : return 1;
    case 19 : return 1;
    case 20 : return 1;
    case 21 : return 1;
    case 22 : return 1;
    case 23 : return 1;
    case 24 : return 1;
    case 25 : return 1;
    case 26 : return 1;
    case 27 : return 1;
    case 28 : return 1;
    case 29 : return 1;
    case 30 : return 1;
    case 31 : return 1;
    case 32 : return 1;
    case 100 : return 1;
    case 101 : return 1;
    case 102 : return 1;
    case 103 : return 1;
    case 104 : return 1;
    case 105 : return 1;
    case 106 : return 1;
    case 107 : return 1;
    case 108 : return 1;
    case 109 : return 1;
    case 110 : return 1;
    case 111 : return 1;
    case 112 : return 1;
    case 113 : return 1;
    case 114 : return 1;
    case 115 : return 1;
    case 200 : return 0;
    case 201 : return 0;
    case 202 : return 0;
    case 203 : return 0;
    case 204 : return 0;
    case 205 : return 0;
    case 206 : return 0;
    case 207 : return 0;
    case 208 : return 0;
    case 209 : return 0;
    case 210 : return 8;
    case 211 : return 8;
    case 212 : return 8;
    case 213 : return 8;
    case 214 : return 8;
    case 215 : return 8;
    case 216 : return 8;
    case 217 : return 8;
    case 218 : return 8;
    case 219 : return 8;
    case 220 : return 8;
    case 230 : return 8;
    case 231 : return 8;
    case 232 : return 8;
    case 233 : return 8;
    case 300 : return 0;
    case 301 : return 0;
    case 302 : return 0;
    case 303 : return 0;
    case 304 : return 0;
    case 305 : return 0;
    case 306 : return 0;
    case 307 : return 0;
    case 308 : return 0;
    case 404 : return 1;
    case 416 : return 1;
    case 419 : return 1;
    case 430 : return 1;
    case 450 : return 1;
    case 451 : return 1;
    case 452 : return 1;
    case 453 : return 1;
    case 454 : return 1;
    case 455 : return 1;
    case 456 : return 1;
    case 457 : return 1;
    case 458 : return 1;
    case 459 : return 1;
    case 460 : return 1;
    case 461 : return 1;
    case 462 : return 1;
    case 463 : return 1;
    case 464 : return 1;
    case 465 : return 1;
    case 466 : return 1;
    case 467 : return 1;
    case 468 : return 1;
    case 469 : return 1;
    case 470 : return 1;
    case 471 : return 1;
    case 472 : return 1;
    case 473 : return 1;
    case 474 : return 1;
    case 475 : return 1;
    case 476 : return 1;
    case 477 : return 1;
    case 478 : return 1;
    case 479 : return 1;
    case 480 : return 1;
    case 481 : return 1;
    case 482 : return 1;
    case 483 : return 1;
    case 484 : return 1;
    case 485 : return 1;
    case 502 : return 1;
    case 503 : return 1;
    case 504 : return 1;
    case 505 : return 1;
    case 506 : return 1;
    case 507 : return 1;
    case 508 : return 1;
    case 509 : return 1;
    case 510 : return 1;
    case 514 : return 1;
    case 519 : return 1;
    case 526 : return 1;
    case 540 : return 1;
    case 541 : return 1;
    case 542 : return 1;
    case 543 : return 1;
    case 1004 : return 1;
    case 1015 : return 1;
    case 2004 : return 1;
    case 2015 : return 1;
    case 3000 : return 1;
    case 3001 : return 1;
    case 3002 : return 1;
    case 3003 : return 1;
    case 3004 : return 1;
    case 3005 : return 1;
    case 3006 : return 1;
    case 3007 : return 1;
    case 3008 : return 1;
    case 3009 : return 1;
    case 3010 : return 1;
    case 3011 : return 1;
    case 3012 : return 1;
    case 3013 : return 1;
    case 3014 : return 1;
    case 3015 : return 1;
    case 3228 : return 1;
    case 3423 : return 1;
    case 3424 : return 1;

    
    // DEFAULT FALLBACK IF UNKNOWN TOY
    default : return 9;
}

}



void SkylanderIO::ReadSkylanderFile(char *name) throw (int)
{
    FILE *file;
    unsigned long fileLen;
    
    //Open file
    file = fopen(name, "rb");
    if (!file)
    {
        throw 1;
        //		fprintf(stderr, "Unable to open file %s\n", name);
        //		return NULL;
    }
    
    //Get file length
    fseek(file, 0, SEEK_END);
    fileLen=ftell(file);
    fseek(file, 0, SEEK_SET);
    
    if(fileLen != 1024){
        throw 2;
    }
    //Read file contents into buffer
    fread(buffer, fileLen, 1, file);
    fclose(file);
    
    
}

bool SkylanderIO::WriteSkylanderFile(char *name, unsigned char * filedata) throw (int)
{
    FILE *file;
    bool OK = true;
    int count;
    
    file = fopen(name, "wb");
    if (!file)
    {
        throw 1;
        //		fprintf(stderr, "Unable to open file %s for writing.\n", name);
        //		return false;
    }
    
    count = fwrite(filedata, 1024, 1, file);
    if(count < 1) {
        fclose(file);
        throw 3;
        OK = false;
    }
    
    fclose(file);
    return OK;
}

void SkylanderIO::listSkylanders() {
    
    PortalIO *port;
    Skylander *sky;
    unsigned char data[0x10]; 
    
    
    port = new PortalIO();
    sky = new Skylander(buffer);
    
    try {
        for (int s=0; s<3; s++) 
        {
            memset(data,0,0x10);
            // must start with a read of block zero
            port->ReadBlock(1, data, s); 
            
            printf("%0d: %s\n",s,sky->toyName(data[0] + data[1] * 0x100));
        }
        delete sky;
        delete port;
        
    } catch (int e) {
        delete sky;
        delete port;
        if (e != 8)
            throw e;
    }
    
}


#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <unistd.h>
#include "checksum.h"
#include "crypt.h"
#include "fileio.h"
#include "portalio.h"
#include "skylander.h"


#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"


using namespace std;

void usage()
{
    printf("\n"
           "Usage:\n"
           "editor [-i <file>|-p] [-s <skylander>] [-d] [-e] [-o <file>|-P] [-M <money>] [-X experience] ... \n"
           "\n"
           "Reading/Writing:\n"
           "-i <file>\tread skylander data from file, with option to decrypt the data.\n"
           "-p\t\tread skylander data from portal and decrypt the data.\n"
           "-s <skylander> select which skylander.\n"
           "-d\t\tdecrypt the data read from the file.\n"
           "-o <file>\twrite skylander data to <filename>.\n"
           "-a\t\twrite skylander data to automatic filename.\n"
           "-P\t\tencrypt and write skylander data to the portal.\n"
           "-e\t\tencrypt data when writing file.\n"
           "-D\t\tdump the data of a skylander to the display.\n"
           "-l\t\tList skylanders on portal.\n"
           
           "\nUpgrade:\n"
           "-M <money>\tupgrade skylander money (max 65,000).\n"
           "-X <xp>\t\tupgrade skylander Experience (level 10 = 33,000).\n"
           "-H <hp>\t\tupgrade skylander Hero Points (max 100).\n"
           "-C <challenges>\tupgrade skylander challenges.\n"
           "-L <points>\tupgrade the skillpoints left path. (0 = set path)\n"
           "-R <points>\tupgrade the skillpoints right path. (0 = set path)\n"
           "-T <toyId>\t! Change the ToyID (UID Blck0 RW) !\n"
           "-c\t\tupdate checksums.\n"
           "\n"
           "Examples: \n"
           "editor -p -o spyro.bin\n"
           "This would save a copy of the figurine to the file dspyro.bak\n"
           "editor -i spyro.bin -o spyro_upgrade.bin -L 65535 -M 65000 -X 33000 -H 100\n"
           "upgrade spyro.bin using skills on the LEFT path seen in the character menu\n"
           "and write it to file spyro_upgrade.bin\n"
           "\n"
           "editor -i spyro.bin -P -M 65000 -X 33000\n"
           "Upgrade skylander, leave skills as is, and write to the portal.\n"
           "\n"
           "editor -i spyro.bin -P\n"
           "Read file from spyro.bin and write it to the portal.\n");
}

#include <iostream>
#include "md5.h"

using std::cout; using std::endl;


int main(int argc, char* argv[])
{
    printf(KWHT);
    fprintf(stdout,"\n\n\nEDITOR 2.24\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
    
    /*
     unsigned char test[16];
     
     cout << "md5 test 123... : " << md5("12345678901234567890123456789012345678901234567890123456789012345678901234567890123456") << endl;
     //test << md5("12345678901234567890123456789012345678901234567890123456789012345678901234567890123456");
     
     printf("MD5 digest : ");
     for(int compteur = 0; compteur <= 15; compteur++) {
     printf("%02X ",keyOut123[compteur]);
     }
     */
    
    /*
     unsigned char aze;
     aze = 0x00;
     const unsigned char eza = aze;
     MD5 context;
     context.update   ("01234567890123456789012345678901234567890123456789012345678901234567890123456789123456", 86);
     context.update (&eza, 1);
     context.finalize ();
     
     cout << "MD5 (12345...) = " << context <<endl;
     
     std::string str=md5("12345678901234567890123456789012345678901234567890123456789012345678901234567890123456");
     
     unsigned char MD5HashOut[17];
     unsigned int MD5charQty = (str.length()/2);
     int MD5Char1;
     int MD5Char2;
     
     for (int ByteCount = 0; ByteCount <= MD5charQty; ByteCount++) {
     
     std::string strbyte0 = str.substr (ByteCount,1);
     MD5Char1 = (int) str[ByteCount*2];
     if (MD5Char1 > 47 && MD5Char1 < 58) {
     MD5Char1 = MD5Char1 - 48;
     } else {
     MD5Char1 = MD5Char1 - 87;
     }
     MD5Char1 = 16*MD5Char1;
     
     std::string strbyte1 = str.substr ((ByteCount+1),1);
     MD5Char2 = (int) str[(ByteCount*2)+1];
     if (MD5Char2 > 47 && MD5Char2 < 58) {
     MD5Char2 = MD5Char2 - 48;
     } else {
     MD5Char2 = MD5Char2 - 87;
     }
     MD5HashOut[ByteCount] = MD5Char1 + MD5Char2;
     
     }
     
     printf("MD5 digest Dec : ");
     for(int compteur = 0; compteur <= 15; compteur++) {
     printf("%3i ",MD5HashOut[compteur]);
     }
     printf("\n");
     
     printf("MD5 digest Hex : ");
     for(int compteur = 0; compteur <= 15; compteur++) {
     printf("%2X ",MD5HashOut[compteur]);
     }
     printf("\n");
     */
    
    
    
    
    
    //printf("%i \n %s \n",argc, argv[0]);
    
    unsigned char *buffer, *original_data;
    bool OK, OK2;
    bool encrypt,decrypt,portalIn,portalOut,dump,upgrade,flash,list,autoFile;
    
    char * inFile, *outFile;
    
    const static char *legal_flags = "alFePpcDo:i:dM:X:H:C:L:R:s:T:";
    
    encrypt = false;
    decrypt = false;
    portalIn = false;
    portalOut = false;
    upgrade = false;
    dump = false;
    inFile = NULL;
    outFile = NULL;
    flash = false;
    list = false;
    autoFile = false;
    
    
    unsigned int money, toyid, xp, hp, challenges, skillleft, skillright,skylander_number;
    bool pathleft, pathright;
    
    money = 0;
    toyid = 0;
    xp = 0;
    hp = 0;
    challenges = 0;
    skillleft = 0;
    pathleft = false;
    skillright = 0;
    pathright = false;
    skylander_number = 0;
    
    SkylanderIO *skio;
    Checksum crc;
    
    int k;
    
    while ((k = getopt (argc, argv, legal_flags)) != -1) {
        switch (k) {
            case 'e': encrypt = true; break;
            case 'd': decrypt = true; break;
            case 'P': portalOut = true; break;
            case 'p': portalIn = true; break;
            case 'D': dump = true; break;
            case 'F': flash = true; break;
            case 'i':
                inFile = new char[strlen(optarg)+1];
                strcpy(inFile,optarg);
                break;
            case 'o':
                outFile = new char[strlen(optarg)+1];
                strcpy(outFile,optarg);
                break;
            case 'a':
                autoFile = true;
                outFile = new char[65];
                strcpy(outFile,"TEMP");
                break;
            case 'M':
                money = atoi(optarg);
                upgrade = true;
                break;
            case 'T':
                toyid = atoi(optarg);
                upgrade = true;
                break;
            case 'X':
                xp = atoi(optarg);
                upgrade = true;
                break;
            case 'H':
                hp = atoi(optarg);
                upgrade = true;
                break;
            case 'C':
                challenges = atoi(optarg);
                upgrade = true;
                break;
            case 'L':
                skillleft = atoi(optarg);
                pathleft = true;
                upgrade = true;
                break;
            case 'R':
                skillright = atoi(optarg);
                pathright = true;
                upgrade = true;
                break;
            case 's':
                skylander_number = atoi(optarg);
                break;
            case 'c':
                upgrade = true;
                break;
            case 'l':
                list = true;
                break;
            default:
                usage () ;
                exit (0);
                
        }
    }
    
    for (int WorkLoop = 1; WorkLoop < 2; WorkLoop++) {
        
        /*
         if WorkLoop == 1
         portalIn = true;
         dump = true;
         */
        
        
        try {
            
            // some entertainment.
            if (flash) {
                PortalIO  *pio ;
                pio = new PortalIO();
                
                pio->flash();
                exit (0);
            }
            
            if (list)  {
                
                skio = new SkylanderIO();
                printf ("Listing Skylanders.\n\n");
                skio->listSkylanders();
                printf ("End of Listing Skylanders.\n\n");
                
                exit (0);
            }
            
            // validate command line options
            if ( (!inFile && !portalIn) || (inFile && portalIn)) {
                printf ("%sMust Choose One of: read from file -i <file> or read from portal -p\n",KRED);
                usage();
                exit(0);
            }
            
            if (!outFile && !portalOut && !dump) {
                printf ("%sNothing to write. Choose file -o, portal -P or dump -D\n",KRED);
                usage();
                exit(0);
            }
            
            if (portalIn && upgrade) {
                printf ("%sIt is not recommended to upgrade directly from the portal (write to a file first)\n",KRED);
                usage();
                exit(0);
            }
            
            if (portalIn && portalOut) {
                printf ("%sIt is not recommended to read and write directly from the portal (write to a file first)\n",KRED);
                usage();
                exit(0);
            }
            
            skio = new SkylanderIO();
            
            printf ("====================================================\n\n");
            printf ("%sReading Skylander\n\n%s",KGRN,KWHT);
            
            if (portalIn) {
                skio->initWithPortal(skylander_number);
            }
            if (inFile) {
                if (decrypt) {
                    skio->initWithEncryptedFile(inFile);
                } else {
                    skio->initWithUnencryptedFile(inFile);
                }
            }
            
            unsigned int cptsteps;
            unsigned int cptacblock;
            unsigned int compteur;
            
            if(! skio->getSkylander()->validateChecksum()) {
                fprintf(stderr, "---================================----------------------------\n%sWarning. Skylander data read from portal, but checksums are incorrect.  File may be corrupt.\n%s",KRED,KWHT);
            }
            
            else {
                printf ("%sSkylander Checksum OK.\n%s",KGRN,KWHT);
            }
            
            if (dump) {
                Skylander * sky ;
                sky = skio->getSkylander() ;
                
                sky->dump();
                
                printf("Serial Number: %08lX\n",sky->getSerial());
                printf("Toy Type: %s (%d)\n\n",sky->getToyTypeName(),sky->getToyType());
                printf("Toy ID: %u\n\n",sky->getToyID());
                //            skio->fprinthex(stdout,sky->getToyID(),2);
                printf ("Trading ID: ");
                skio->fprinthex(stdout,sky->getTradingID(),8);
                
                //Debugging Use
                //printf("Area 0 sequence: %d\n",sky->getArea0Sequence());
                //printf("Area 1 sequence: %d\n",sky->getArea1Sequence());
                //printf("Area %d selected (higher sequence)\n\n",sky->getArea());
                
                printf("Experience: %d\n",sky->getXP());
                printf("Money: %d\n",sky->getMoney());
                printf("Skills: %04X - %s\n",sky->getSkill(), sky->getPath());
                printf("Platforms: %s\n",sky->getPlatformName());
                //printf("Nickname: %s\n",sky->getName());
                printf("Hat: %d\n",sky->getHat());
                printf("Hero Points: %d\n",sky->getHeroPoints());
                printf("Heroic Challenges: %08x\n",sky->getHeroicChallenges());
                printf("\n");
            }
            
            if (upgrade) {
                if (toyid) { skio->getSkylander()->setToyID(toyid); }
                if (money) { skio->getSkylander()->setMoney(money); }
                if (xp) { skio->getSkylander()->setXP(xp); }
                if (hp) { skio->getSkylander()->setHeroPoints(hp); }
                if (challenges) { skio->getSkylander()->setHeroicChallenges(challenges); }
                if (pathleft) { skio->getSkylander()->setSkillLeft(skillleft); }
                if (pathright) { skio->getSkylander()->setSkillRight(skillright); }
                
                
                skio->getSkylander()->computeChecksum();
            }
            
            
            if (outFile || portalOut)  {
                printf ("Writing Skylander.\n");
            }
            if (outFile) {
                if (autoFile) {
                    Skylander * sky;
                    sky = skio->getSkylander();
                    unsigned long num = sky->getSerial();
                    unsigned long serial = ((num>>24)&0xff) | // move byte 3 to byte 0
                    ((num<<8)&0xff0000) | // move byte 1 to byte 2
                    ((num>>8)&0xff00) | // move byte 2 to byte 1
                    ((num<<24)&0xff000000); // byte 0 to byte 3
                    
                    sprintf(outFile, "%s - %s - %08lX.dmp", sky->getToyTypeName(), sky->getPath(), serial);
                    printf("Saving to automatic filename: %s\n", outFile);
                }
                
                if (encrypt) {
                    skio->writeSkylanderToEncryptedFile(outFile);
                } else {
                    skio->writeSkylanderToUnencryptedFile(outFile);
                }
            }
            if (portalOut) {
                skio->writeSkylanderToPortal(skylander_number);
            }
            
            delete skio;
            
            
            printf("\nSuccess!\n\n");
            return 0;
            
        } catch (int e) {
            
            printf(KRED);
            switch (e) {
                case 1: printf ("Cannot open File.\n"); break;
                case 2: printf ("Invalid Skylander File.\n"); break;
                case 3: printf ("Cannot write to File.\n"); break;
                case 4: printf ("Unable to get USB Device List.\n"); break;
                case 5: printf ("Cannot Find Portal USB.\n"); break;
                case 6: printf ("%sUnable to write to Portal.\n",KRED); break;
                case 7: printf ("Invalid Skylander Block.\n"); break;
                case 8: printf ("Unable to read Skylander from Portal.\n"); break;
                case 9: printf ("Wireless portal not connected.\n"); break;
                case 10: printf ("Skylander Write Verify Error.\n"); break;
                case 11: printf ("No Skylander detected on portal.\n"); break;
                default: printf ("Unknown exception: %d.\n",e); break;
            }
            
        }
    }
}

#include "skylander.h"

//Easier to maintain list of all the ID numbers with the names
//See toydata.xls for all the data that makes this easy to update
//Need to eventually figure out a good way to incorporate the Series / Variant info also.
//Credit for most of the info:
//	https://github.com/reedstrm/SkyReader
//	https://github.com/bettse/SkyReader/tree/more_info
//	https://github.com/Proxmark/proxmark3/blob/master/client/lualibs/default_toys.lua

const char * Skylander::toyName(unsigned short toyID) {
    
    switch (toyID) {
            
        case 0 : return "Whirlwind  /  Element:air  /  Group:regular";                      //0000|0030|regular|air
        case 1 : return "Sonic Boom  /  Element:air  /  Group:regular";                     //0100|0030|regular|air
        case 2 : return "Warnado  /  Element:air  /  Group:regular";                        //0200|0030|regular|air
        case 3 : return "Lightning Rod  /  Element:air  /  Group:regular";                  //0300|0030|regular|air
        case 4 : return "Bash  /  Element:earth  /  Group:regular";                           //0400|0030|regular|earth
        case 5 : return "Terrafin  /  Element:earth  /  Group:regular";                       //0500|0030|regular|earth
        case 6 : return "Dino-Rang  /  Element:earth  /  Group:regular";                      //0600|0030|regular|earth
        case 7 : return "Prism Break  /  Element:earth  /  Group:regular";                    //0700|0030|regular|earth
        case 8 : return "Sunburn  /  Element:fire  /  Group:regular";                        //0800|0030|regular|fire
        case 9 : return "Eruptor  /  Element:fire  /  Group:regular";                        //0900|0030|regular|fire
        case 10 : return "Ignitor  /  Element:fire  /  Group:regular";                       //0a00|0030|regular|fire
        case 11 : return "Flameslinger  /  Element:fire  /  Group:regular";                  //0b00|0030|regular|fire
        case 12 : return "Zap  /  Element:water  /  Group:regular";                           //0c00|0030|regular|water
        case 13 : return "Wham-Shell  /  Element:water  /  Group:regular";                    //0d00|0030|regular|water
        case 14 : return "Gill Grunt  /  Element:water  /  Group:regular";                    //0e00|0030|regular|water
        case 15 : return "Slam Bam  /  Element:water  /  Group:regular";                      //0f00|0030|regular|water
        case 16 : return "Spyro  /  Element:magic  /  Group:regular";                         //1000|0030|regular|magic
        case 17 : return "Voodood  /  Element:magic  /  Group:regular";                       //1100|0030|regular|magic
        case 18 : return "Double Trouble  /  Element:magic  /  Group:regular";                //1200|0030|regular|magic
        case 19 : return "Trigger Happy  /  Element:tech  /  Group:regular";                 //1300|0030|regular|tech
        case 20 : return "Drobot  /  Element:tech  /  Group:regular";                        //1400|0030|regular|tech
        case 21 : return "Drill Sergeant  /  Element:tech  /  Group:regular";                //1500|0030|regular|tech
        case 22 : return "Boomer  /  Element:tech  /  Group:regular";                        //1600|0030|regular|tech
        case 23 : return "Wrecking Ball  /  Element:magic  /  Group:regular";                 //1700|0030|regular|magic
        case 24 : return "Camo  /  Element:life  /  Group:regular";                          //1800|0030|regular|life
        case 25 : return "Zook  /  Element:life  /  Group:regular";                          //1900|0030|regular|life
        case 26 : return "Stealth Elf  /  Element:life  /  Group:regular";                   //1a00|0030|regular|life
        case 27 : return "Stump Smash  /  Element:life  /  Group:regular";                   //1b00|0030|regular|life
        case 28 : return "Dark Spyro  /  Element:magic  /  Group:regular";                    //1c00|0030|regular|magic
        case 29 : return "Hex  /  Element:undead  /  Group:regular";                           //1d00|0030|regular|undead
        case 30 : return "Chop Chop  /  Element:undead  /  Group:regular";                     //1e00|0030|regular|undead
        case 31 : return "Ghost Roaster  /  Element:undead  /  Group:regular";                 //1f00|0030|regular|undead
        case 32 : return "Cynder  /  Element:undead  /  Group:regular";                        //2000|0030|regular|undead
        case 100 : return "Jet Vac  /  Element:air  /  Group:regular";                      //6400|0030|regular|air
        case 101 : return "Swarm  /  Element:air  /  Group:giant";                        //6500|0030|giant|air
        case 102 : return "Crusher  /  Element:earth  /  Group:giant";                      //6600|0030|giant|earth
        case 103 : return "Flashwing  /  Element:earth  /  Group:regular";                    //6700|0030|regular|earth
        case 104 : return "Hot Head  /  Element:fire  /  Group:giant";                     //6800|0030|giant|fire
        case 105 : return "Hot Dog  /  Element:fire  /  Group:regular";                      //6900|0030|regular|fire
        case 106 : return "Chill  /  Element:water  /  Group:regular";                        //6a00|0030|regular|water
        case 107 : return "Thumpback  /  Element:water  /  Group:giant";                    //6b00|0030|giant|water
        case 108 : return "Pop Fizz  /  Element:magic  /  Group:regular";                     //6c00|0030|regular|magic
        case 109 : return "Ninjini  /  Element:magic  /  Group:giant";                      //6d00|0030|giant|magic
        case 110 : return "Bouncer  /  Element:tech  /  Group:giant";                      //6e00|0030|giant|tech
        case 111 : return "Sprocket  /  Element:tech  /  Group:regular";                     //6f00|0030|regular|tech
        case 112 : return "Tree Rex  /  Element:life  /  Group:giant";                     //7000|0030|giant|life
        case 113 : return "Shroomboom  /  Element:life  /  Group:regular";                   //7100|0030|regular|life
        case 114 : return "Eye-Brawl  /  Element:undead  /  Group:giant";                    //7200|0030|giant|undead
        case 115 : return "Fright Rider  /  Element:undead  /  Group:regular";                 //7300|0030|regular|undead
        case 200 : return "Anvil Rain  /  Element:none  /  Group:item";                   //c800|0030|item|none
        case 201 : return "Treasure Chest  /  Element:none  /  Group:item";               //c900|0030|item|none
        case 202 : return "Healing Elixer  /  Element:none  /  Group:item";               //ca00|0030|item|none
        case 203 : return "Ghost Swords  /  Element:none  /  Group:item";                 //cb00|0030|item|none
        case 204 : return "Time Twister  /  Element:none  /  Group:item";                 //cc00|0030|item|none
        case 205 : return "Sky-Iron Shield  /  Element:none  /  Group:item";              //cd00|0030|item|none
        case 206 : return "Winged Boots  /  Element:none  /  Group:item";                 //ce00|0030|item|none
        case 207 : return "Sparx Dragonfly  /  Element:none  /  Group:item";              //cf00|0030|item|none
        case 208 : return "Dragonfire Cannon  /  Element:none  /  Group:item";            //d000|0030|item|none
        case 209 : return "Scorpion Striker Catapult  /  Element:none  /  Group:item";    //d100|0030|item|none
        case 210 : return "Trap MAGIC - Biters Bane";                  //d200|0230|trap|magic
        case 211 : return "Trap WATER - Tidal Tiki";                   //d300|0130|trap|water
        case 212 : return "Trap AIR - Breezy Bird";                  //d400|0330|trap|air
        case 213 : return "Trap UNDEAD - Spirit Sphere";                //d500|0430|trap|undead
        case 214 : return "Trap TECH - Tech Totem";                   //d600|0030|trap|tech
        case 215 : return "Trap FIRE - Eternal Flame";                //d700|0530|trap|fire
        case 216 : return "Trap EARTH - Banded Boulder";               //d800|0030|trap|earth
        case 217 : return "Trap LIFE - Oak Eagle";                    //d900|0330|trap|life
        case 218 : return "Trap DARK - Dark Dagger";                  //da00|0030|trap|dark
        case 220 : return "Trap KAOS - Kaos Trap!";                   //dc00|3030|trap|kaos
            
        case 230 : return "Hand Of Fate  /  Element:none  /  Group:item";                 //e600|0030|item|none
        case 231 : return "Piggy Bank  /  Element:none  /  Group:item";                   //e700|0030|item|none
        case 232 : return "Rocket Ram  /  Element:none  /  Group:item";                   //e800|0030|item|none
        case 233 : return "Tiki Speaky  /  Element:none  /  Group:item";                  //e900|0030|item|none
            
        case 300 : return "Dragons Peak  /  Element:none  /  Group:location";                 //2c01|0030|location|none
        case 301 : return "Empire of Ice  /  Element:none  /  Group:location";                //2d01|0030|location|none
        case 302 : return "Pirate Seas  /  Element:none  /  Group:location";                  //2e01|0030|location|none
        case 303 : return "Darklight Crypt  /  Element:none  /  Group:location";              //2f01|0030|location|none
        case 304 : return "Volcanic Vault  /  Element:none  /  Group:location";               //3001|0030|location|none
        case 305 : return "Mirror Of Mystery  /  Element:none  /  Group:location";            //3101|0030|location|none
        case 306 : return "Nightmare Express  /  Element:none  /  Group:location";            //3201|0030|location|none
        case 307 : return "Sunscraper Spire  /  Element:light  /  Group:location";             //3301|0030|location|light
        case 308 : return "Midnight Museum  /  Element:dark  /  Group:location";              //3401|0030|location|dark
            
        case 404 : return "Bash  /  Element:earth  /  Group:legendary";                         //9401|0030|legendary|earth
        case 416 : return "Spyro  /  Element:magic  /  Group:legendary";                        //a001|0030|legendary|magic
        case 419 : return "Trigger Happy  /  Element:tech  /  Group:legendary";                //a301|0030|legendary|tech
        case 430 : return "Chop Chop  /  Element:undead  /  Group:legendary";                    //ae01|0030|legendary|undead
        case 450 : return "Gusto  /  Element:air  /  Group:trapmaster";                        //c201|0030|trapmaster|air
        case 451 : return "Thunderbolt  /  Element:air  /  Group:trapmaster";                  //c301|0030|trapmaster|air
        case 452 : return "Fling Kong  /  Element:air  /  Group:regular";                   //c401|0030|regular|air
        case 453 : return "Blades  /  Element:air  /  Group:regular";                       //c501|0030|regular|air
        case 454 : return "Wallop  /  Element:earth  /  Group:trapmaster";                       //c601|0030|trapmaster|earth
        case 455 : return "Head Rush  /  Element:earth  /  Group:trapmaster";                    //c701|0030|trapmaster|earth
        case 456 : return "Fist Bump  /  Element:earth  /  Group:regular";                    //c801|0030|regular|earth
        case 457 : return "Rocky Roll  /  Element:earth  /  Group:regular";                   //c901|0030|regular|earth
        case 458 : return "Wildfire  /  Element:fire  /  Group:trapmaster";                     //ca01|0030|trapmaster|fire
        case 459 : return "Ka Boom  /  Element:fire  /  Group:trapmaster";                      //cb01|0030|trapmaster|fire
        case 460 : return "Trail Blazer  /  Element:fire  /  Group:regular";                 //cc01|0030|regular|fire
        case 461 : return "Torch  /  Element:fire  /  Group:regular";                        //cd01|0030|regular|fire
        case 462 : return "Snap Shot  /  Element:water  /  Group:trapmaster";                    //ce01|0030|trapmaster|water
        case 463 : return "Lob Star  /  Element:water  /  Group:trapmaster";                     //cf01|0030|trapmaster|water
        case 464 : return "Flip Wreck  /  Element:water  /  Group:regular";                   //d001|0030|regular|water
        case 465 : return "Echo  /  Element:water  /  Group:regular";                         //d101|0030|regular|water
        case 466 : return "Blastermind  /  Element:magic  /  Group:trapmaster";                  //d201|0030|trapmaster|magic
        case 467 : return "Enigma  /  Element:magic  /  Group:trapmaster";                       //d301|0030|trapmaster|magic
        case 468 : return "Deja Vu  /  Element:magic  /  Group:regular";                      //d401|0030|regular|magic
        case 469 : return "Cobra Cadabra  /  Element:magic  /  Group:regular";                //d501|0030|regular|magic
        case 470 : return "Jawbreaker  /  Element:tech  /  Group:trapmaster";                   //d601|0030|trapmaster|tech
        case 471 : return "Gearshift  /  Element:tech  /  Group:trapmaster";                    //d701|0030|trapmaster|tech
        case 472 : return "Chopper  /  Element:tech  /  Group:regular";                      //d801|0030|regular|tech
        case 473 : return "Tread Head  /  Element:tech  /  Group:regular";                   //d901|0030|regular|tech
        case 474 : return "Bushwhack  /  Element:life  /  Group:trapmaster";                    //da01|0030|trapmaster|life
        case 475 : return "Tuff Luck  /  Element:life  /  Group:trapmaster";                    //db01|0030|trapmaster|life
        case 476 : return "Food Fight  /  Element:life  /  Group:regular";                   //dc01|0030|regular|life
        case 477 : return "High Five  /  Element:life  /  Group:regular";                    //dd01|0030|regular|life
        case 478 : return "Krypt King  /  Element:undead  /  Group:trapmaster";                   //de01|0030|trapmaster|undead
        case 479 : return "Short Cut  /  Element:undead  /  Group:trapmaster";                    //df01|0030|trapmaster|undead
        case 480 : return "Bat Spin  /  Element:undead  /  Group:regular";                     //e001|0030|regular|undead
        case 481 : return "Funny Bone  /  Element:undead  /  Group:regular";                   //e101|0030|regular|undead
        case 482 : return "Knight light  /  Element:light  /  Group:trapmaster";                 //e201|0030|trapmaster|light
        case 483 : return "Spotlight  /  Element:light  /  Group:regular";                    //e301|0030|regular|light
        case 484 : return "Knight Mare  /  Element:dark  /  Group:trapmaster";                  //e401|0030|trapmaster|dark
        case 485 : return "Blackout  /  Element:dark  /  Group:regular";                     //e501|0030|regular|dark
        case 502 : return "Bop  /  Element:earth  /  Group:mini";                          //f601|0030|mini|earth
        case 503 : return "Spry  /  Element:magic  /  Group:mini";                         //f701|0030|mini|magic
        case 504 : return "Hijinx  /  Element:undead  /  Group:mini";                       //f801|0030|mini|undead
        case 505 : return "Terrabite  /  Element:earth  /  Group:mini";                    //f901|0030|mini|earth
        case 506 : return "Breeze  /  Element:air  /  Group:mini";                       //fa01|0030|mini|air
        case 507 : return "Weeruptor  /  Element:fire  /  Group:mini";                    //fb01|0030|mini|fire
        case 508 : return "Pet Vac  /  Element:air  /  Group:mini";                      //fc01|0030|mini|air
        case 509 : return "Small Fry  /  Element:fire  /  Group:mini";                    //fd01|0030|mini|fire
        case 510 : return "Drobit  /  Element:tech  /  Group:mini";                       //fe01|0030|mini|tech
        case 514 : return "Gill Runt  /  Element:water  /  Group:mini";                    //0202|0030|mini|water
        case 519 : return "Trigger Snappy  /  Element:tech  /  Group:mini";               //0702|0030|mini|tech
        case 526 : return "Whisper Elf  /  Element:life  /  Group:mini";                  //0e02|0030|mini|life
        case 540 : return "Barkley  /  Element:life  /  Group:mini";                      //1c02|0030|mini|life
        case 541 : return "Thumpling  /  Element:water  /  Group:mini";                    //1d02|0030|mini|water
        case 542 : return "Mini Jini  /  Element:magic  /  Group:mini";                    //1e02|0030|mini|magic
        case 543 : return "Eye Small  /  Element:undead  /  Group:mini";                    //1f02|0030|mini|undead
        case 1004 : return "Blast Zone  /  Element:fire  /  Group:swapforce";                  //EC03||swapforce|fire
        case 1015 : return "Wash Buckler  /  Element:water  /  Group:swapforce";                //F703||swapforce|water
        case 2004 : return "Blast Zone (Head)  /  Element:fire  /  Group:swapforce";           //D407||swapforce|fire
        case 2015 : return "Wash Buckler (Head)  /  Element:water  /  Group:swapforce";         //DF07||swapforce|water
        case 3000 : return "Scratch  /  Element:air  /  Group:regular";                     //b80b|0030|regular|air
        case 3001 : return "Pop Thorn  /  Element:air  /  Group:regular";                   //b90b|0030|regular|air
        case 3002 : return "Slobber Tooth  /  Element:earth  /  Group:regular";               //ba0b|0030|regular|earth
        case 3003 : return "Scorp  /  Element:earth  /  Group:regular";                       //bb0b|0030|regular|earth
        case 3004 : return "Fryno  /  Element:fire  /  Group:regular";                       //bc0b|0030|regular|fire
        case 3005 : return "Smolderdash  /  Element:fire  /  Group:regular";                 //bd0b|0030|regular|fire
        case 3006 : return "Bumble Blast  /  Element:life  /  Group:regular";                //be0b|0030|regular|life
        case 3007 : return "Zoo Lou  /  Element:life  /  Group:regular";                     //bf0b|0030|regular|life
        case 3008 : return "Dune Bug  /  Element:magic  /  Group:regular";                    //c00b|0030|regular|magic
        case 3009 : return "Star Strike  /  Element:magic  /  Group:regular";                 //c10b|0030|regular|magic
        case 3010 : return "Countdown  /  Element:tech  /  Group:regular";                   //c20b|0030|regular|tech
        case 3011 : return "Wind Up  /  Element:tech  /  Group:regular";                     //c30b|0030|regular|tech
        case 3012 : return "Roller Brawl  /  Element:undead  /  Group:regular";                //c40b|0030|regular|undead
        case 3013 : return "Grim Creeper  /  Element:undead  /  Group:regular";                //c50b|0030|regular|undead
        case 3014 : return "Rip Tide  /  Element:water  /  Group:regular";                    //c60b|0030|regular|water
        case 3015 : return "Punk Shock  /  Element:water  /  Group:regular";                  //c70b|0030|regular|water
            
            //Default fallback option with toyID
        default		:
            char* toyName = new char[25];
            sprintf(toyName, "zzUNKNOWN_%hu", toyID);
            return toyName;
    }
}

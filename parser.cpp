#include <stdio.h>
#include <stdlib.h>

#include "note_set.h"   // stog i struct Note (int pitch, int startTime, int stopTime)

FILE *midifile = fopen("prelude8", "rb");

short 
swap16(short num)
{
    return (num<<8)|(num>>8);
}

int
swap32(int num)
{
    return ((num>>24)&0xff) |
                    ((num<<8)&0xff0000) |
                    ((num>>8)&0xff00) |
                    ((num<<24)&0xff000000);
}

unsigned int
readVariable()
{
    unsigned int value = 0;
    unsigned int byte = 0;

    byte = (unsigned int)fgetc(midifile);


    if(byte & 0x80)    // MSB postavljen
    {
        value = byte & 0x7F;
        do
        {
            byte = (unsigned int)fgetc(midifile);
            value = (value << 7) | (byte & 0x7F);
        } while(byte & 0x80);
    }
    else value = (unsigned int)byte;
    return value;
}


int
main()
{   
    int trash;
    short hType;
    unsigned short numTracks;
    short timeDiv;
    
    add(0, 0);
    
    // ===============HEADER======================
    //

    fread(&trash, sizeof(trash), 1, midifile);    
    fread(&trash, sizeof(trash), 1, midifile);
    
    fread(&hType, sizeof(hType), 1, midifile);
    hType = swap16(hType);
    
    fread(&numTracks, sizeof(numTracks), 1, midifile);
    numTracks = swap16(numTracks);

    fread(&timeDiv, sizeof(timeDiv), 1, midifile);

    // ==============TRACKS========================
    //

    int noteIdx = 0;
    Note NOTES[2048];
    for(int track = 0; track < numTracks; track++)
    {
        printf("NOVA TRAKA\n");
        // ===========TRACK HEADER=================
        //
        //procitat dtime, procitat koji je tip eventa, 
        //ako je noteon/noteoff napravit nesto (i postavit da je dtime note.dtime)
        //
        unsigned int chunkSize;

        fread(&trash, sizeof(trash), 1, midifile);
        fread(&chunkSize, sizeof(chunkSize), 1, midifile);
        chunkSize = swap32(chunkSize);
        printf("chunkSize: %d\n", chunkSize);

        char endOfTrack = 0;
        unsigned int previousStatus = 0;
        while(!endOfTrack)
        {
            printf("citam event \n");
            //=========READ EVENTS================
            //
            unsigned int dtime = readVariable();
            printf("event dtime %d\n", dtime);
            
            unsigned int status = (unsigned int)fgetc(midifile);
            printf("event status %02X\n", status);
            if(status < 0x80)
            {
                printf("running status!\n");
                status = previousStatus;
                ungetc((char)status, midifile);
            }

            char channel = status & 0x0F;
            
            if(status >= 0x80 && status < 0x90) // note off
            {
                previousStatus = status;
                char pitch;
                fread(&pitch, sizeof(pitch), 1, midifile);
                char velocity;
                fread(&velocity, sizeof(velocity), 1, midifile);
                printf("noteOFF pitch: %d, velocity: %d\n", pitch, velocity);
            }

            else if(status >= 0x90 && status < 0xA0) // note on
            {
                previousStatus = status;
                char pitch;
                fread(&pitch, sizeof(pitch), 1, midifile);
                char velocity;
                fread(&velocity, sizeof(velocity), 1, midifile);
                printf("noteON pitch: %d, velocity: %d\n", pitch, velocity);
            }

            else if(status == 0xFF)
            {
                previousStatus = status;
                // jebeni usrani meta event 
                //
                unsigned int metaType = (unsigned int)fgetc(midifile);
                printf("META TYPE: %02X\n", metaType);

                unsigned int metaLen = readVariable();
                printf("META LEN: %d\n", metaLen);

                switch(metaType)
                {
                    case 0:
                        {
                        printf("sMETA seqnum: %d, %d\n", fgetc(midifile), fgetc(midifile));
                        }
                        break;

                    case 1:
                    case 2:
                    case 3:
                    case 4:
                    case 5:
                    case 6:
                    case 7:
                    case 127:
                        {
                        printf("PORUKA:   ");
                        char c;
                        for(int i = 0;
                                i < metaLen;
                                i++)
                        {
                            c = (char)fgetc(midifile);
                            printf("%c", c);
                        }
                        printf("\n");
                        }
                        break;
                    case 32:
                        {
                            printf("channel prefix %d", fgetc(midifile));
                        }
                        break;
                    case 47:
                        {
                            printf("=======EOT===========\n");
                            endOfTrack = 1;
                        }
                        break;
                    case 81:
                        {
                            // TEMPO - ne treba mi pa cu ga sam ignorirat
                            printf("        TEMPO\n");
                            fgetc(midifile);
                            fgetc(midifile);
                            fgetc(midifile);
                        }
                        break;
                    case 84:
                        {
                            // SMTPE offset - isto ignor
                            printf("        SMTPE OFFSET\n");
                            fgetc(midifile);
                            fgetc(midifile);
                            fgetc(midifile);
                            fgetc(midifile);
                            fgetc(midifile);
                        }
                        break;
                    case 88:
                        {
                            // timesig
                            printf("        TIMESIG\n");
                            fgetc(midifile);
                            fgetc(midifile);
                            fgetc(midifile);
                            fgetc(midifile);
                        }
                        break;
                    case 89:
                        {
                            // keysig - HMMMMMMMST mozda ce bit korisno
                            printf("           KEYSIG\n");
                            fgetc(midifile);
                            fgetc(midifile);
                        }
                        break;
                }
            }
            else if(status == 0xF0)
            {
                previousStatus = status;
                int metaLen = readVariable();
                for(int i = 0;
                        i < metaLen;
                        i++)
                {
                    char c = (char)fgetc(midifile);
                    printf("%c", c);
                }
                printf("\n");
            }
            
            else if(status >= 0xC0 &&
                    status < 0xE0)
            {
                previousStatus = status;
                char par1;
                par1 = fgetc(midifile);
                printf("OTHER(C, D) par1: %d\n", par1);
            }

            else if(status >= 0xE0 && status < 0xF0)
            {
                previousStatus = status;
                printf("OTHER(E) par1 %d, par2 %d\n", fgetc(midifile), fgetc(midifile));
            }
            else
                printf("unrecognized statoos: %02X\n", status);
        }
    }
    

    return 0;
}

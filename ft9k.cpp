#include <stdio.h>
#include <string.h>
#include "ft9k.hpp"

struct Note;

const char* Chord(int, Note *);

const char* Chord(int, int, Note *);

const char* Chord(Note *);

static int absTime = 0;
static int noteIdx = 0;
Note CURRENT[256];
static int currIdx = 0;

void
swap(int *x, int *y)
{
    *x = *x ^ *y;
    *y = *x ^ *y;
    *x = *x ^ *y;
}

void 
b_sort(int arr[], int n)  
{  
    int i, j;  
    for (i = 0; i < n-1; i++)      
      
    for (j = 0; j < n-i-1; j++)  
        if (arr[j] > arr[j+1])  
            swap(&arr[j], &arr[j+1]);  
} 

void
remove_dupes(int a[], int *n)
{
    for(int i = 0; i < *n; i++)
    {
        for(int j = i+1; j < *n; ++j)
        {
            if(a[j] == a[i])
            {
                for(int k = j; k < *n; k++)
                {
                    a[k] = a[k+1];
                }
                (*n)--;
                j--;
            }
        }
    }
}

Note*
notes_at_dtime(int dtime, Note *NOTES)
{
    int idx = 0;
    static Note notes[88];

    for(int i = 0; i < noteIdx; i++)
    {
        if(dtime >= NOTES[i].starttime &&
                dtime < NOTES[i].starttime + NOTES[i].duration)
            notes[idx++] = NOTES[i];
    }
    return notes;
}

Note*
notes_b_dtimes(int from, int to, Note *NOTES)
{
    int idx = 0; 
    static Note notes[88];

    for(int i = 0; i < noteIdx; i++)
    {
        if(
                !(
                    (NOTES[i].starttime <= from && NOTES[i].starttime + NOTES[i].duration <= from) ||
                    (NOTES[i].starttime >= to && NOTES[i].starttime + NOTES[i].duration >= to)
                 )
          )
        {
            notes[idx++] = NOTES[i];
        }
    }
    return notes;
}

const char* 
chord_from_notes(Note *n)
{
    int notes[88];

    //  pitch % 12 == 0 -> C
    //  pitch % 12 == 1 -> C# 
    //  etc.

    int c_size;
    for(c_size = 0; n[c_size].pitch; ++c_size)
        notes[c_size] = n[c_size].pitch % 12;

    //  maknut duplikate
    
    remove_dupes(notes, &c_size);

    //  sortirat notes
    
    b_sort(notes, c_size);
    
    //  terce 3 note
    //    D3:   43, 54, 35
    //    M3:   34, 45, 53
    //    S3:   33, 63, 36
    //    P3:   44
    //  iVD7:   47, 14, 71
    //  imm7:   37, 23, 72
    //  imD7:   46, 24, 62
    //  iVm7:   38, 13, 81
    //  
    //  terce 4 note
    //   VD7:   434, 143, 414, 341
    //   mD7:   433, 243, 324, 332
    //   mm7:   343, 234, 323, 432
    //   Vm7:   344, 134, 413, 441
    //   ms7:   334, 233, 423, 342
    //   ss7:   333
    //    p7:   443, 144, 314, 431

    int intervals[c_size-1];
    for(int i = 0; i < c_size; ++i)
        intervals[i] = notes[i + 1] - notes[i];

    static char c_identifier[5];
    static int c_root;
    
    if(c_size < 3)
        return "Chord size < 3";
    else if(c_size == 3)
    {
        // terce 3 note
        switch(intervals[0])
        {
            case 4:
            {
                switch(intervals[1])
                {
                    case 3:
                        strcpy(c_identifier, "D3");
                        c_root = notes[0];
                        break;

                    case 5:
                        strcpy(c_identifier, "M3");
                        c_root = notes[2];
                        break;

                    case 4:
                        strcpy(c_identifier, "P3");
                        c_root = notes[0];
                        break;

                    case 7:
                        strcpy(c_identifier, "iVD7");
                        c_root = notes[0];
                        break;

                    case 6:
                        strcpy(c_identifier, "imD7");
                        c_root = notes[0];
                        break;

                    default:
                        return "nesto cudno";
                }
                break;
            }

            case 5:
            {
                switch(intervals[1])
                {
                    case 4:
                        strcpy(c_identifier, "D3");
                        c_root = notes[1];
                        break;

                    case 3:
                        strcpy(c_identifier, "M3");
                        c_root = notes[1];
                        break;

                    default:
                        return "nesto cudno";
                }
                break;
            }

            case 3:
            {
                switch(intervals[1])
                {
                    case 5:
                        strcpy(c_identifier, "D3");
                        c_root = notes[2];
                        break;

                    case 4:
                        strcpy(c_identifier, "M3");
                        c_root = notes[0];
                        break;

                    case 3:
                        strcpy(c_identifier, "S3");
                        c_root = notes[0];
                        break;

                    case 6:
                        strcpy(c_identifier, "S3");
                        c_root = notes[2];
                    case 7:
                        strcpy(c_identifier, "imm7");
                        c_root = notes[0];
                        break;

                    case 8:
                        strcpy(c_identifier, "iVm7");
                        c_root = notes[0];
                        break;

                    default:
                        return "nesto cudno";
                }
                break;
            }

            case 6:
            {
                switch(intervals[1])
                {
                    case 3:
                        strcpy(c_identifier, "S3");
                        c_root = notes[1];
                        break;

                    case 2:
                        strcpy(c_identifier, "imD7");
                        c_root = notes[2];
                        break;

                    default:
                        return "nesto cudno";
                }
                break;
            }

            case 1:
            {
                switch(intervals[1])
                {
                    case 4:
                        strcpy(c_identifier, "iVD7");
                        c_root = notes[1];
                        break;

                    case 3:
                        strcpy(c_identifier, "iVm7");
                        c_root = notes[1];
                        break;

                    default:
                        return "nesto cudno";
                }
                break;
            }

            case 2:
            {
                switch(intervals[1])
                {
                    case 3:
                        strcpy(c_identifier, "imm7");
                        c_root = notes[1];
                        break;

                    case 4:
                        strcpy(c_identifier, "imD7");
                        c_root = notes[1];
                        break;

                    default:     
                        return "nesto cudno";
                }
                break;
            }

            case 7:
            {
                switch(intervals[1])
                {
                    case 1:
                        strcpy(c_identifier, "iVD7");
                        c_root = notes[2];
                        break;
                    
                    case 2:
                        strcpy(c_identifier, "imm7");
                        c_root = notes[2];
                        break;

                    default:
                        return "nesto cudno";
                }
                break;
            }
            
            case 8:
            {
                strcpy(c_identifier, "iVm7");
                c_root = notes[2];
                break;
            }

            default:
                return "nesto cudno";
        }
    }

    else if(c_size == 4)
    {
        // terce 4 note
        switch(intervals[0])
        {
            case 4:
            {
                switch(intervals[1])
                {
                    case 3:
                    {
                        switch(intervals[2])
                        {
                            case 4:
                                strcpy(c_identifier, "VD7");
                                c_root = notes[0];
                                break;

                            case 3:
                                strcpy(c_identifier, "mD7");
                                c_root = notes[0];
                                break;

                            case 2:
                                strcpy(c_identifier, "mm7");
                                c_root = notes[3];
                                break;

                            default:
                                return "nesto cudno";
                        }
                        break;
                    }

                    case 1:
                    {
                        switch(intervals[2])
                        {
                            case 4:
                                strcpy(c_identifier, "VD7");
                                c_root = notes[2];
                                break;

                            case 3:
                                strcpy(c_identifier, "Vm7");
                                c_root = notes[2];
                                break;

                            default:
                                return "nesto cudno";
                        }
                        break;
                    }

                    case 2:
                    {
                        switch(intervals[2])
                        {
                            case 3:
                                strcpy(c_identifier, "ms7");
                                c_root = notes[2];
                                break;

                            default:
                                return "nesto cudno";
                        }
                        break;
                    }

                }
                break;
            }

            case 3:
            {
                switch(intervals[1])
                {
                    case 4:
                    {
                        switch(intervals[2])
                        {
                            case 1:
                                strcpy(c_identifier, "VD7");
                                c_root = notes[3];
                                break;

                            case 3:
                                strcpy(c_identifier, "mm7");
                                c_root = notes[0];
                                break;

                            case 4:
                                strcpy(c_identifier, "Vm7");
                                c_root = notes[0];
                                break;

                            case 2:
                                strcpy(c_identifier, "ms7");
                                c_root = notes[3];
                                break;

                            default:
                                return "nesto cudno";

                        }
                        break;
                    }

                    case 3:
                    {
                        switch(intervals[2])
                        {
                            case 2:
                                strcpy(c_identifier, "mD7");
                                c_root = notes[3];
                                break;

                            case 4:
                                strcpy(c_identifier, "ms7");
                                c_root = notes[0];
                                break;

                            case 3:
                                strcpy(c_identifier, "ss7");
                                c_root = notes[0];
                                break;

                            default:
                                return "nesto cudno";
                        }
                        break;
                    }
                    
                    case 2:
                    {
                        switch(intervals[2])
                        {
                            case 4:
                                strcpy(c_identifier, "mD7");
                                c_root = notes[2];
                                break;

                            case 3:
                                strcpy(c_identifier, "mm7");
                                c_root = notes[2];
                                break;

                            default:
                                return "nesto cudno";
                        }
                        break;
                    }

                    default:
                        return "nesto cudno";
                }
                break;
            }

            case 1:
            {
                switch(intervals[1])
                {
                    case 4:
                    {
                        switch(intervals[2])
                        {
                            case 3:
                                strcpy(c_identifier, "VD7");
                                c_root = notes[1];
                                break;

                            case 4:
                                strcpy(c_identifier, "P7");
                                c_root = notes[1];
                                break;

                            default:
                                return "nesto cudno";
                        }
                    } 
                    case 3:
                        strcpy(c_identifier, "Vm7");
                        c_root = notes[1];
                        break;

                    default:
                        return "nesto cudno";

                }
                break;
            }

            case 2:
            {
                switch(intervals[1])
                {
                    case 4:
                        strcpy(c_identifier, "mD7");
                        c_root = notes[1];
                        break;

                    case 3:
                        strcpy(c_identifier, "mm7");
                        c_root = notes[1];
                        break;

                    default:
                        return "nesto cudno";
                }
                break;
            }
            
            default:
                return "nesto cudno";
        }
    }
    else
        return "vise od 4 note";

    static char chord[9];

    switch(c_root)
    {
        case 0:
            strcpy(chord, "C ");
            strcat(chord, c_identifier);
            break;

        case 1: 
            strcpy(chord, "C# ");
            strcat(chord, c_identifier);
            break;

        case 2:
            strcpy(chord, "D ");
            strcat(chord, c_identifier);
            break;

        case 3:
            strcpy(chord, "D# ");
            strcat(chord, c_identifier);
            break;

        case 4:
            strcpy(chord, "E ");
            strcat(chord, c_identifier);
            break;

        case 5:
            strcpy(chord, "F ");
            strcat(chord, c_identifier);
            break;

        case 6:
            strcpy(chord, "F# ");
            strcat(chord, c_identifier);
            break;

        case 7:
            strcpy(chord, "G ");
            strcat(chord, c_identifier);
            break;

        case 8:
            strcpy(chord, "G# ");
            strcat(chord, c_identifier);
            break;

        case 9:
            strcpy(chord, "A ");
            strcat(chord, c_identifier);
            break;

        case 10:
            strcpy(chord, "A# ");
            strcat(chord, c_identifier);
            break;

        case 11:
            strcpy(chord, "H ");
            strcat(chord, c_identifier);
            break;
    }

    return chord;
}

const char*
Chord(int at, Note *NOTES)
{
    return chord_from_notes(notes_at_dtime(at, NOTES));
}

const char*
Chord(int from, int to, Note *NOTES)
{
    return chord_from_notes(notes_b_dtimes(from, to, NOTES));
}

const char*
Chord(Note* n)
{
    return chord_from_notes(n);
}


void
addToCurrent(Note n)
{
    CURRENT[currIdx++] = n;
}

Note
removeFromCurrent(Note n)
{
    Note ret;
    for(int i = 0; i < currIdx; ++i)
    {
        if(n.pitch == CURRENT[i].pitch)
        {
            int startTime = CURRENT[i].starttime;
            CURRENT[i] = CURRENT[--currIdx];
            ret.duration = absTime - startTime;
            ret.pitch = CURRENT[i].pitch;
            ret.starttime = startTime;
            return ret;
        }
    }
    return {.pitch = -1, .starttime = -1, .duration = -1};
}

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
readVariable(FILE *midifile)
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


Note*
M_parse(const char* f)
{   
    static Note NOTES[2048];
    FILE *midifile = fopen(f, "rb");
    int trash;
    short hType;
    unsigned short numTracks;
    short timeDiv;
    
    
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

    for(int track = 0; track < numTracks; track++)
    {
        // ===========TRACK HEADER=================
        //
        //procitat dtime, procitat koji je tip eventa, 
        //ako je noteon/noteoff napravit nesto (i postavit da je dtime note.dtime)

        unsigned int chunkSize;

        fread(&trash, sizeof(trash), 1, midifile);
        fread(&chunkSize, sizeof(chunkSize), 1, midifile);
        chunkSize = swap32(chunkSize);

        char endOfTrack = 0;
        unsigned int previousStatus = 0;
        while(!endOfTrack)
        {
            //=========READ EVENTS================
            //
            unsigned int dtime = readVariable(midifile);
            absTime+=dtime;
            
            unsigned int status = (unsigned int)fgetc(midifile);
            if(status < 0x80)
            {
                status = previousStatus;
                ungetc((char)status, midifile);
            }

            char channel = status & 0x0F;
            
            if(status >= 0x80 && status < 0x90) // note off
            {
                previousStatus = status;
                char pit;
                fread(&pit, sizeof(pit), 1, midifile);
                char velocity;
                fread(&velocity, sizeof(velocity), 1, midifile);

                
                NOTES[noteIdx++] = removeFromCurrent({.pitch = (int)pit,
                        .starttime = 0, .duration = 0});
            }

            else if(status >= 0x90 && status < 0xA0) // note on
            {
                previousStatus = status;
                char pit;
                fread(&pit, sizeof(pit), 1, midifile);
                char velocity;
                fread(&velocity, sizeof(velocity), 1, midifile);
            

                addToCurrent({.pitch = (int)pit, .starttime = absTime, .duration = 0});
            }

            else if(status == 0xFF)
            {
                previousStatus = status;
                // meta event 
                //
                unsigned int metaType = (unsigned int)fgetc(midifile);

                unsigned int metaLen = readVariable(midifile);

                switch(metaType)
                {
                    case 0:
                        {
                        fgetc(midifile);
                        fgetc(midifile);
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
                        char c;
                        for(int i = 0;
                                i < metaLen;
                                i++)
                        {
                            c = (char)fgetc(midifile);
                        }
                        }
                        break;
                    case 32:
                        {
                            fgetc(midifile);
                        }
                        break;
                    case 47:
                        {
                            endOfTrack = 1;
                        }
                        break;
                    case 81:
                        {
                            // TEMPO - ne treba mi pa cu ga sam ignorirat
                            fgetc(midifile);
                            fgetc(midifile);
                            fgetc(midifile);
                        }
                        break;
                    case 84:
                        {
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
                            fgetc(midifile);
                            fgetc(midifile);
                            fgetc(midifile);
                            fgetc(midifile);
                        }
                        break;
                    case 89:
                        {
                            // keysig - mozda ce bit korisno
                            fgetc(midifile);
                            fgetc(midifile);
                        }
                        break;
                }
            }
            else if(status == 0xF0)
            {
                previousStatus = status;
                int metaLen = readVariable(midifile);
                for(int i = 0;
                        i < metaLen;
                        i++)
                {
                    char c = (char)fgetc(midifile);
                }
            }
            
            else if(status >= 0xC0 &&
                    status < 0xE0)
            {
                previousStatus = status;
                char par1;
                par1 = fgetc(midifile);
            }

            else if(status >= 0xE0 && status < 0xF0)
            {
                previousStatus = status;
                fgetc(midifile);
                fgetc(midifile);
            }
        }

        absTime = 0;
    }

    return NOTES;
}



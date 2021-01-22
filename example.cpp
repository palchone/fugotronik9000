#include <stdio.h>
#include <string.h>
#include "ft9k.hpp"

int
main()
{
    static Note *NOTES;
    NOTES = M_parse("./midi/Prelude1.mid");    // J.S.Bach - WTC1 - Preludij 1 u C-duru

    for(int i = 0; NOTES[i].pitch; i++)
    {
        printf("NOTE: pitch %d, start time %d, duration %d\n", 
                NOTES[i].pitch, NOTES[i].starttime, NOTES[i].duration);
    }
    
    // check for harmony change every 240 MIDI ticks, print only if changed and recognized
    const char previous[20] = "";
    
    for(int i = 0; i < 35000; i += 240) 
    {
        const char *current = Chord(i, i + 240, NOTES);
        if(!strcmp(current, previous)           ||
           !strcmp(current, "Chord size < 3")   ||
           !strcmp(current, "nesto cudno")      ||
           !strcmp(current, "vise od 4 note")) 
            continue;
        else
        {
            printf("harmony at %d. - %s\n", i, current);
            strcpy((char*)previous, (char*)current);
        }
    }
}

#include "note_set.h"

#define MAX 256

static Note array[256];
static s32 at;

void add(pitch p, s32 time)
{
    array[at++] = {.p=p, .startTime=time};
}

s32 remove(pitch p, s32 time)
{
    for(int i = 0; i < at; ++i)
    {
        if(p == array[i].p)
        {
            u32 startTime = array[i].startTime;
            array[i] = array[--at];
            return time-startTime;
        }
    }
    return -1;
}

Note *arrayOfNotes()
{
    return array;
}

s32 noteCount()
{
    return at;
}

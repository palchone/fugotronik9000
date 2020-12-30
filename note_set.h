#pragma once

typedef int pitch;

struct Note
{
    pitch p;
    s32 startTime;
};

void add(pitch, s32 startTime);
s32 remove(pitch, s32 endTime);
Note *arrayOfNotes();
s32 noteCount();


struct Note
{
    int pitch;
    int starttime;
    int duration;
};

typedef struct Note Note;

Note* M_parse(const char *);

const char* Chord(int, Note *);

const char* Chord(int, int, Note *);

const char* Chord(Note *);

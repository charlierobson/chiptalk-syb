

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

#define FALSE (0)
#define TRUE (!0)
typedef char *Rule[4]; //  A rule is four character pointers

#include "t2a.h"     //  prototypes mainly
#include "english.c" //  less messy than inline source

char bias = 0; //  added to allophone value before output to file

//  !!!! start of PHONEME.C

/* Tom Jennings
mru 17 Feb 1999
new 31 Dec 98

English text to allophone translation for the General Instrument SPO256-AL2.
An minor adaptation of John Wasser's PHONEME software. The allophone set has
been expanded from the original 41 to a larger subset of the 64 allophones;
I very carefully exploited the different opcodes for similar sounds, eg. the
voiced stops (three 'G's, etc) -- though I have to say this chip really sucks
at voiced stops, practically unintelligible. Never mind the cultural imperialism
embodied in silicon -- it is utterly incapable of making gutteral stops,
inflections,
dipthongs, etc, or 'speaking' anythnig but variants of english.

The character-to-phoneme lookup tables have been massively updated. The things
that were Big Deals in 1985 are incomprehensibly insignifigant today; namely
table
size (memory and CPU search time). The table had many errors (still have
many more) and I'm sure I introduced many new errors, but it speaks
signifigantly
better now.

A stream of ASCII characters is parsed via lookup table rules (" 'E' preceded
by 'H' sounds like 'EH' " etc); strings of phonemes are converted to SPO256-AL2
opcodes. Phonemes are simply looked up in a table to produce the allophone
opcode:

text        hello
phonemes    h EH l OW
opcodes        ;'MU


Direct improvements include: pauses/silence handled more explicitly (originally
only one pause/silence period was used, inter-word and inter-space); added many
rules to use the allophones for the position-dependent phonemes of the GI chip
(eg. /TT1/ vs. /TT2/).

Finally, SPO256-AL2 allophone opcodes are output directly.



This is a down'n'dirty program now; I rolled all of the source files into this
main, except for english.h which is a #include. And I created t2a.h for the
prototypes which didn't exist in 1985.




The original README file.


                    Final Version of
                ENGLISH TO PHONEME TRANSLATION
                           4/15/85

        Here it is one last time.  I have fixed all of the bugs I
        heard about and added a new feature or two (it now talks
        money as well as numbers).  I think that this version is
        good enough for most purposes.  I have proof-read the
        phoneme rules (found one bug) and made the program more
        "robust".  I added protection against the "toupper()"
        problem some people had with earlier versions.

        If you make a major addition (like better abbreviation
        handling or an exception dictionary) please send me a
        copy.  As before, this is all public domain and I make
        no copyright claims on it.  The part derived from the
        Naval Research Lab should be public anyway.  Sell it
        if you can!

                -John A. Wasser

Work address:
ARPAnet:        WASSER%VIKING.DEC@decwrl.ARPA
Usenet:         {allegra,Shasta,decvax}!decwrl!dec-rhea!dec-viking!wasser
Easynet:        VIKING::WASSER
Telephone:      (617)486-2505
USPS:           Digital Equipment Corp.
                Mail stop: LJO2/E4
                30 Porter Rd
                Littleton, MA  01460


   The files that make up this package are:

          english.c       Translation rules.
          phoneme.c       Translate a single word.
          parse.c         Split a file into words.
          spellwor.c      Spell an ASCII character or word.
          saynum.c        Say a cardinal or ordinal number (long int).

(tomj: All of the files mentioned above have been included in this single source
file; ENGLISH.C
is included with a #include statement).

*/

#define MAX_LENGTH 128

static FILE *In_file;  //  text input
static FILE *Out_file; //  phonemes out
static char *In_data;

static int Char, Char1, Char2, Char3;

/*
** main(argc, argv)
**    int argc;
**    char *argv[];
**
**    This is the main program.  It takes up to two file names (input
**    and output)  and translates the input file to phoneme codes
**    (see english.c) on the output file.
*/
int main(argc, argv) int argc;
char *argv[];
{
    int i; //  [tomj]

    if (argc < 2) {
        fprintf(stderr, "\nTry:\n");
        fprintf(stderr, "    t2a [opt] filein fileout\n");
        fprintf(stderr, "    stdin and stdout are used if files not specified\n");
        exit(0);
    }
    if (strstr(argv[1], "?") != 0) {
        fprintf(stderr,
                "T2A Text to Allophones, for General Instrument "
                "SPO256-AL2. Converts ASCII text\n");
        fprintf(stderr,
                "to SPO256-AL2 opcodes +32 decimal, eg. ASCII bias (opcode "
                "0 becomes ' ', etc)\n");
        fprintf(stderr,
                "By Tom Jennings, 18 Feb 1999, a revision of John Wasser's "
                "code from 1985. This\n");
        fprintf(stderr,
                "code is CopyLeft; source code is available for free from "
                "www.wps.com and elsewhere.\n");
        fprintf(stderr, "\nTry:\n");
        fprintf(stderr, "    t2a (-i infile) (-o outfile) (-t \"literal text used as infile\"\n");
        fprintf(stderr, "    stdin and/or stdout are used if files not specified\n");
        exit(0);
    }

    In_file = stdin;
    Out_file = stdout;

    i = 1;
    while (i < argc) {
        if (argv[i][0] == '-') {
            switch (toupper(argv[i][1])) { //  process options
                case 'I':
                    In_file = fopen(argv[i + 1], "r");
                    if (In_file == 0) {
                        fputs("Error: Cannot open input file.\n", stderr);
                        exit(1);
                    }
                    break;
                case 'O':
                    Out_file = fopen(argv[i + 1], "w"); //  [tomj]
                    if (Out_file == 0) {
                        fputs("Error: Cannot create output file.\n", stderr);
                        exit(2);
                    }
                    break;
                case 'T':
                    In_data = &argv[i + 1][0];
                    In_file = 0;
                    break;
            }
        }
        ++i;
    }

    xlate_file(); //  translate file

    return 0;
}

//  General Instrument SP0256-AL2 phonemes

/* This table contains the GI phonemes and their numeric values. The text is
the phoneme name generated by John Wasser's PHONEME program, the numeric
values are the GI opcodes.

The PHONEME software uses only a subset of the SPO256-AL2's
opcodes. These were hand-mapped, and will likely get filled
in and opcode use expanded later.
*/

struct _p2a {
    char *phoneme;
    char allophone;
} p2a[] = {
    {"IY", 0x13}, //  see
    {"IH", 0x0c}, //  sit
    {"EY", 0x14}, //  beige
    {"EH", 0x07}, //  end, get
    {"AE", 0x1a}, //  fat
    {"AA", 0x18}, //  hot (father?)
    {"AO", 0x17}, //  aught, lawn
    {"OW", 0x35}, //  lone, beau
    {"UH", 0x1e}, //  full, book
    {"UW", 0x1f}, //  food, fool
    {"ER", 0x34}, //  fir, murder
    {"AX", 0x0f}, //  suck, about
    {"AH", 0x0f}, //  but
    {"AY", 0x06}, //  sky, hide
    {"AW", 0x20}, //  out, how
    {"OY", 0x05}, //  boy

    {"h", 0x1b},  //  he
    {"p", 0x09},  //  pow, pack
    {"b", 0x3f},  //  business, back
    {"t", 0x0d},  //  to, time
    {"d", 0x21},  //  do, dime
    {"k", 0x2a},  //  sky
    {"g", 0x24},  //  got, goat
    {"f", 0x28},  //  fault, food
    {"v", 0x23},  //  vest, vault
    {"TH", 0x1d}, //  ether, thin
    {"DH", 0x36}, //  they, either
    {"s", 0x37},  //  vest, Sue
    {"z", 0x2b},  //  zoo
    {"SH", 0x25}, //  ship, leash
    {"ZH", 0x26}, //  azure, leisure
    {"HH", 0x39}, //  hoe, how
    {"m", 0x10},  //  milk, sum
    {"n", 0x0b},  //  thin, sun
    {"NG", 0x2c}, //  sung, anchor
    {"l", 0x2d},  //  lake, laugh
    {"w", 0x2e},  //  wear, wool
    {"y", 0x19},  //  yes, young
    {"r", 0x33},  //  rate
    {"CH", 0x32}, //  church, char
    {"j", 0x0a},  //  jar, dodge
    {"WH", 0x30}, //  whig, where
    {"P1", 0x00}, //  pause, 10ms
    {"P2", 0x01}, //  pause, 30ms
    {"P3", 0x02}, //  pause, 50ms
    {"P4", 0x03}, //  pause, 100ms
    {"P5", 0x04}, //  pause, 200ms

    {"R1", 0x27}, //  reed
    {"T1", 0x11}, //  part
    {"EY", 0x36}, //  they
    {"D1", 0x15}, //  could
    {"U1", 0x16}, //  to
    {"G3", 0x22}, //  wig
    {"RR", 0x0e}, //  brain
    {"K2", 0x08}, //  comb
    {"K1", 0x29}, //  cant
    {"XR", 0x2f}, //  pair
    {"Y1", 0x31}, //  yes
    {"R2", 0x34}, //  fir
    {"N2", 0x38}, //  no
    {"OR", 0x3a}, //  store
    {"AR", 0x3b}, //  alarm
    {"YR", 0x3c}, //  clear
    {"G2", 0x3d}, //  guest
    {"EL", 0x3e}, //  saddle
    {"B2", 0x1c}, //  caleb
    {"DT", 0x12}, //  they
    {"", 0}       //  end of table
};

/* Given a string of phonemes, output General Instrument SPO256-AL2 allophones,
 * with an ASCII bias. */

void outallo(s) char *s;
{
    struct _p2a *t;
    char *q, phoneme[3];

    q = s;       //  save for error report
    while (*s) { //  until string is exhausted,
        if (*s == ' ') {
            ++s;
            continue;
        }
        if (isupper(*s)) { //  upper case phonemes are two chars long,
            phoneme[0] = *s++;
            phoneme[1] = *s++;
            phoneme[2] = '\0';
        } else { //  lower case phonemes are one char long
            phoneme[0] = *s++;
            phoneme[1] = '\0';
        }

        for (t = p2a; *t->phoneme; ++t) { //  search the table for it
            if (strcmp(t->phoneme, phoneme) == 0) {
                outchar(t->allophone + bias);
                break;
            }
        }
        if (!*(t->phoneme)) {
            fprintf(stderr,
                    "Phoneme \"%s\" in string \"%s\" not in allophone table!\n",
                    phoneme, q);
        }
    }
}

void outstring(string) char *string;
{
    if (!*string)
        return;
    outallo(string);
}

char inchar()
{
    if (In_file) return getc(In_file);
    if (*In_data) return *In_data++;
    return EOF;
}

void outchar(int chr)
{
    fputc(chr, Out_file);
}

int makeupper(character) int character;
{
    if (islower(character))
        return toupper(character);
    else
        return character;
}

char new_char()
{
    /*
  If the cache is full of newline, time to prime the look-ahead
  again.  If an EOF is found, fill the remainder of the queue with
  EOF's.
  */
    if (Char == '\n' && Char1 == '\n' && Char2 == '\n' &&
        Char3 == '\n') { //  prime the pump again
        Char = inchar();
        if (Char == EOF) {
            Char1 = EOF;
            Char2 = EOF;
            Char3 = EOF;
            return Char;
        }
        if (Char == '\n')
            return Char;

        Char1 = inchar();
        if (Char1 == EOF) {
            Char2 = EOF;
            Char3 = EOF;
            return Char;
        }
        if (Char1 == '\n')
            return Char;

        Char2 = inchar();
        if (Char2 == EOF) {
            Char3 = EOF;
            return Char;
        }
        if (Char2 == '\n')
            return Char;

        Char3 = inchar();
    } else {
        /*
    Buffer not full of newline, shuffle the characters and
    either get a new one or propagate a newline or EOF.
    */
        Char = Char1;
        Char1 = Char2;
        Char2 = Char3;
        if (Char3 != '\n' && Char3 != EOF)
            Char3 = inchar();
    }
    return Char;
}

/*
** xlate_file()
**
**    This is the input file translator.  It sets up the first character
**    and uses it to determine what kind of text follows.
*/
void xlate_file()
{
    //  Prime the queue
    Char = '\n';
    Char1 = '\n';
    Char2 = '\n';
    Char3 = '\n';
    new_char(); //  Fill Char, Char1, Char2 and Char3

    while (Char != EOF) //  All of the words in the file
    {
        if (isdigit(Char))
            have_number();
        else if (isalpha(Char) || Char == '\'')
            have_letter();
        else if (is_punct(Char))
            have_punct();
        else if (Char == '$' && isdigit(Char1))
            have_dollars();
        else
            have_special();
    }
}

/* [tomj] Speak punctuation. Because punctuation is expressed only as pauses in
speaking, we
suppress multiple punctuation characters, speaking only the first one found. */

void have_punct()
{
    char buff[3];
    sprintf(buff, "%c ", Char);   //  format as required by find_rule(),
    find_rule(buff, 0, Rules[0]); //  speak it (one charact er);
    for (new_char(); is_punct(Char); new_char())
        ; //  gobble and throw away further punctuation
}

//  [tomj] Return true if the current character is punctuation.

int is_punct(char c)
{
    switch (c) {
        case '.':
        case ',':
        case ':':
        case ';':
        case '!':
        case '?':
        case '-':
            return (TRUE);
        default:
            return (FALSE);
    }
}

void have_dollars()
{
    long int value;

    value = 0L;
    for (new_char(); isdigit(Char) || Char == ','; new_char()) {
        if (Char != ',')
            value = 10 * value + (Char - '0');
    }

    say_cardinal(value); //  Say number of whole dollars

    //  Found a character that is a non-digit and non-comma

    //  Check for no decimal or no cents digits
    if (Char != '.' || !isdigit(Char1)) {
        if (value == 1L)
            outstring("dAAlER ");
        else
            outstring("dAAlAArz ");
        return;
    }

    //  We have '.' followed by a digit

    new_char(); //  Skip the period

    //  If it is ".dd " say as " DOLLARS AND n CENTS "
    if (isdigit(Char1) && !isdigit(Char2)) {
        if (value == 1L)
            outstring("dAAlER ");
        else
            outstring("dAAlAArz ");
        if (Char == '0' && Char1 == '0') {
            new_char(); //  Skip tens digit
            new_char(); //  Skip units digit
            return;
        }

        outstring("AAnd ");
        value = (Char - '0') * 10 + Char1 - '0';
        say_cardinal(value);

        if (value == 1L)
            outstring("sEHnt ");
        else
            outstring("sEHnts ");
        new_char(); //  Used Char (tens digit)
        new_char(); //  Used Char1 (units digit)
        return;
    }

    //  Otherwise say as "n POINT ddd DOLLARS "

    outstring("pOYnt ");
    for (; isdigit(Char); new_char()) {
        say_ascii(Char);
    }

    outstring("dAAlAArz ");

    return;
}

void have_special()
{
    if (Char == '\n')
        outchar('\n');
    else if (!isspace(Char))
        say_ascii(Char);

    new_char();
    return;
}

void have_number()
{
    long int value;
    int lastdigit;

    value = Char - '0';
    lastdigit = Char;

    for (new_char(); isdigit(Char); new_char()) {
        value = 10 * value + (Char - '0');
        lastdigit = Char;
    }

    //  Recognize ordinals based on last digit of number
    switch (lastdigit) {
        case '1': //  ST
            if (makeupper(Char) == 'S' && makeupper(Char1) == 'T' &&
                !isalpha(Char2) && !isdigit(Char2)) {
                say_ordinal(value);
                new_char(); //  Used Char
                new_char(); //  Used Char1
                return;
            }
            break;

        case '2': //  ND
            if (makeupper(Char) == 'N' && makeupper(Char1) == 'D' &&
                !isalpha(Char2) && !isdigit(Char2)) {
                say_ordinal(value);
                new_char(); //  Used Char
                new_char(); //  Used Char1
                return;
            }
            break;

        case '3': //  RD
            if (makeupper(Char) == 'R' && makeupper(Char1) == 'D' &&
                !isalpha(Char2) && !isdigit(Char2)) {
                say_ordinal(value);
                new_char(); //  Used Char
                new_char(); //  Used Char1
                return;
            }
            break;

        case '0': //  TH
        case '4': //  TH
        case '5': //  TH
        case '6': //  TH
        case '7': //  TH
        case '8': //  TH
        case '9': //  TH
            if (makeupper(Char) == 'T' && makeupper(Char1) == 'H' &&
                !isalpha(Char2) && !isdigit(Char2)) {
                say_ordinal(value);
                new_char(); //  Used Char
                new_char(); //  Used Char1
                return;
            }
            break;
    }

    say_cardinal(value);

    //  Recognize decimal points
    if (Char == '.' && isdigit(Char1)) {
        outstring("pOYnt ");
        for (new_char(); isdigit(Char); new_char()) {
            say_ascii(Char);
        }
    }

    //  Spell out trailing abbreviations
    if (isalpha(Char)) {
        while (isalpha(Char)) {
            say_ascii(Char);
            new_char();
        }
    }

    return;
}

void have_letter()
{
    char buff[MAX_LENGTH];
    int count;

    count = 0;
    buff[count++] = ' '; //  Required initial blank

    buff[count++] = makeupper(Char);

    for (new_char(); isalpha(Char) || Char == '\''; new_char()) {
        buff[count++] = makeupper(Char);
        if (count > MAX_LENGTH - 2) {
            buff[count++] = ' ';
            buff[count++] = '\0';
            xlate_word(buff);
            count = 1;
        }
    }

    buff[count++] = ' '; //  Required terminating blank
    buff[count++] = '\0';

    //  Check for AAANNN type abbreviations
    if (isdigit(Char)) {
        spell_word(buff);
        return;
    } else //  [tomj] "A" and "I" are words!
        if ((strlen(buff) == 3) && (strcmp(buff, " I ") != 0) &&
            (strcmp(buff, " A ") != 0)) //  one character, two spaces
        say_ascii(buff[1]);
    else if (Char == '.') //  Possible abbreviation
        abbrev(buff);
    else
        xlate_word(buff);

    if (Char == '-' && isalpha(Char1))
        new_char(); //  Skip hyphens
}

//  Handle abbreviations.  Text in buff was followed by '.'
void abbrev(buff) char buff[];
{
    if (strcmp(buff, " DR ") == 0) {
        xlate_word(" DOCTOR ");
        new_char();
    } else if (strcmp(buff, " MR ") == 0) {
        xlate_word(" MISTER ");
        new_char();
    } else if (strcmp(buff, " MRS ") == 0) {
        xlate_word(" MISSUS ");
        new_char();
    } else if (strcmp(buff, " PHD ") == 0) {
        spell_word(" PHD ");
        new_char();
    } else
        xlate_word(buff);
}

//  !!! end of PARSE.C

/*
**    English to Phoneme translation.
**
**    Rules are made up of four parts:
**
**        The left context.
**        The text to match.
**        The right context.
**        The phonemes to substitute for the matched text.
**
**    Procedure:
**
**        Seperate each block of letters (apostrophes included)
**        and add a space on each side.  For each unmatched
**        letter in the word, look through the rules where the
**        text to match starts with the letter in the word.  If
**        the text to match is found and the right and left
**        context patterns also match, output the phonemes for
**        that rule and skip to the next unmatched letter.
**
**
**    Special Context Symbols:
**
**        #    One or more vowels
**        :    Zero or more consonants
**        ^    One consonant.
**        .    One of B, D, V, G, J, L, M, N, R, W or Z (voiced
**            consonants)
**        %    One of ER, E, ES, ED, ING, ELY (a suffix)
**            (Right context only)
**        +    One of E, I or Y (a "front" vowel)
*/

int isvowel(char chr)
{
    return (chr == 'A' || chr == 'E' || chr == 'I' || chr == 'O' || chr == 'U');
}

int isconsonant(char chr)
{
    return (isupper(chr) && !isvowel(chr));
}

void xlate_word(word) char word[];
{
    int index; //  Current position in word
    int type;  //  First letter of match part

    index = 1; //  Skip the initial blank
    do {
        if (isupper(word[index]))
            type = word[index] - 'A' + 1;
        else
            type = 0;

        index = find_rule(word, index, Rules[type]);
    } while (word[index] != '\0');
}

int find_rule(word, index, rules) char word[];
int index;
Rule *rules;
{
    Rule *rule;
    char *left, *match, *right, *output;
    int remainder;

    for (;;) //  Search for the rule
    {
        rule = rules++;
        match = (*rule)[1];

        if (match == 0) //  bad symbol!
        {
            fprintf(stderr, "Error: Can't find rule for: '%c' in \"%s\"\n",
                    word[index], word);
            return index + 1; //  Skip it!
        }

        for (remainder = index; *match != '\0'; match++, remainder++) {
            if (*match != word[remainder])
                break;
        }

        if (*match != '\0') //  found missmatch
            continue;
        /*
    printf("\nWord: \"%s\", Index:%4d, Trying: \"%s/%s/%s\" = \"%s\"\n",
        word, index, (*rule)[0], (*rule)[1], (*rule)[2], (*rule)[3]);
    */
        left = (*rule)[0];
        right = (*rule)[2];

        if (!leftmatch(left, &word[index - 1]))
            continue;
        /*
    printf("leftmatch(\"%s\",\"...%c\") succeded!\n", left, word[index-1]);
    */
        if (!rightmatch(right, &word[remainder]))
            continue;
        /*
    printf("rightmatch(\"%s\",\"%s\") succeded!\n", right, &word[remainder]);
    */
        output = (*rule)[3];
        /*
    printf("Success: ");
    */
        outstring(output);
        return remainder;
    }
}

int leftmatch(
    pattern,
    context) char *pattern; //  first char of pattern to match in text
char *context;              //  last char of text to be matched
{
    char *pat;
    char *text;
    int count;

    if (*pattern == '\0') //  null string matches any context
    {
        return TRUE;
    }

    //  point to last character in pattern string
    count = (int)strlen(pattern);
    pat = pattern + (count - 1);

    text = context;

    for (; count > 0; pat--, count--) {
        //  First check for simple text or space
        if (isalpha(*pat) || *pat == '\'' || *pat == ' ') {
            if (*pat != *text) {
                return FALSE;
            } else {
                text--;
                continue;
            }
        }

        switch (*pat) {
            case '#': //  One or more vowels
                if (!isvowel(*text))
                    return FALSE;

                text--;

                while (isvowel(*text))
                    text--;
                break;

            case ':': //  Zero or more consonants
                while (isconsonant(*text))
                    text--;
                break;

            case '^': //  One consonant
                if (!isconsonant(*text))
                    return FALSE;
                text--;
                break;

            case '.': //  B, D, V, G, J, L, M, N, R, W, Z
                if (*text != 'B' && *text != 'D' && *text != 'V' && *text != 'G' &&
                    *text != 'J' && *text != 'L' && *text != 'M' && *text != 'N' &&
                    *text != 'R' && *text != 'W' && *text != 'Z')
                    return FALSE;
                text--;
                break;

            case '+': // E, I or Y (front vowel)
                if (*text != 'E' && *text != 'I' && *text != 'Y')
                    return FALSE;
                text--;
                break;

            case '%':
            default:
                fprintf(stderr, "Bad char in left rule: '%c'\n", *pat);
                return FALSE;
        }
    }

    return TRUE;
}

int rightmatch(
    pattern,
    context) char *pattern; //  first char of pattern to match in text
char *context;              //  last char of text to be matched
{
    char *pat;
    char *text;

    if (*pattern == '\0') //  null string matches any context
        return TRUE;

    pat = pattern;
    text = context;

    for (pat = pattern; *pat != '\0'; pat++) {
        //  First check for simple text or space
        if (isalpha(*pat) || *pat == '\'' || *pat == ' ') {
            if (*pat != *text) {
                return FALSE;
            } else {
                text++;
                continue;
            }
        }

        switch (*pat) {
            case '#': //  One or more vowels
                if (!isvowel(*text))
                    return FALSE;

                text++;

                while (isvowel(*text))
                    text++;
                break;

            case ':': //  Zero or more consonants
                while (isconsonant(*text))
                    text++;
                break;

            case '^': //  One consonant
                if (!isconsonant(*text))
                    return FALSE;
                text++;
                break;

            case '.': //  B, D, V, G, J, L, M, N, R, W, Z
                if (*text != 'B' && *text != 'D' && *text != 'V' && *text != 'G' &&
                    *text != 'J' && *text != 'L' && *text != 'M' && *text != 'N' &&
                    *text != 'R' && *text != 'W' && *text != 'Z')
                    return FALSE;
                text++;
                break;

            case '+': //  E, I or Y (front vowel)
                if (*text != 'E' && *text != 'I' && *text != 'Y')
                    return FALSE;
                text++;
                break;

            case '%': //  ER, E, ES, ED, ING, ELY (a suffix)
                if (*text == 'E') {
                    text++;
                    if (*text == 'L') {
                        text++;
                        if (*text == 'Y') {
                            text++;
                            break;
                        } else {
                            text--; //  Don't gobble L
                            break;
                        }
                    } else if (*text == 'R' || *text == 'S' || *text == 'D')
                        text++;
                    break;
                } else if (*text == 'I') {
                    text++;
                    if (*text == 'N') {
                        text++;
                        if (*text == 'G') {
                            text++;
                            break;
                        }
                    }
                    return FALSE;
                } else
                    return FALSE;

            default:
                fprintf(stderr, "Bad char in right rule:'%c'\n", *pat);
                return FALSE;
        }
    }

    return TRUE;
}

//  !!!! End of PHONEME.C

/*
**              Integer to Readable ASCII Conversion Routine.
**
** Synopsis:
**
**      say_cardinal(value)
**          long int     value;          -- The number to output
**
**    The number is translated into a string of phonemes
**
*/

static char *Cardinals[] = {
    "zIHrOW ", "wAHn ", "tUW ", "THrIY ", "fOWr ",
    "fAYv ", "sIHks ", "sEHvAXn ", "EYt ", "nAYn ",
    "tEHn ", "IYlEHvAXn ", "twEHlv ", "THERtIYn ", "fOWrtIYn ",
    "fIHftIYn ", "sIHkstIYn ", "sEHvEHntIYn ", "EYtIYn ", "nAYntIYn "};

static char *Twenties[] = {"twEHntIY ", "THERtIY ", "fAOrtIY ", "fIHftIY ",
                           "sIHkstIY ", "sEHvEHntIY ", "EYtIY ", "nAYntIY "};

static char *Ordinals[] = {
    "zIHrOWEHTH ", "fERst ", "sEHkAHnd ", "THERd ",
    "fOWrTH ", "fIHfTH ", "sIHksTH ", "sEHvEHnTH ",
    "EYtTH ", "nAYnTH ", "tEHnTH ", "IYlEHvEHnTH ",
    "twEHlvTH ", "THERtIYnTH ", "fAOrtIYnTH ", "fIHftIYnTH ",
    "sIHkstIYnTH ", "sEHvEHntIYnTH ", "EYtIYnTH ", "nAYntIYnTH "};

static char *Ord_twenties[] = {
    "twEHntIYEHTH ", "THERtIYEHTH ", "fOWrtIYEHTH ", "fIHftIYEHTH ",
    "sIHkstIYEHTH ", "sEHvEHntIYEHTH ", "EYtIYEHTH ", "nAYntIYEHTH "};

/*
** Translate a number to phonemes.  This version is for CARDINAL numbers.
**     Note: this is recursive.
*/
void say_cardinal(value) long int value;
{
    if (value < 0) {
        outstring("mAYnAHs ");
        value = (-value);
        if (value < 0) //  Overflow!  -32768
        {
            outstring("IHnfIHnIHtIY ");
            return;
        }
    }

    if (value >= 1000000000L) //  Billions
    {
        say_cardinal(value / 1000000000L);
        outstring("bIHlIYAXn ");
        value = value % 1000000000;
        if (value == 0)
            return;      //  Even billion
        if (value < 100) //  as in THREE BILLION AND FIVE
            outstring("AEnd ");
    }

    if (value >= 1000000L) //  Millions
    {
        say_cardinal(value / 1000000L);
        outstring("mIHlIYAXn ");
        value = value % 1000000L;
        if (value == 0)
            return;      //  Even million
        if (value < 100) //  as in THREE MILLION AND FIVE
            outstring("AEnd ");
    }

    //  Thousands 1000..1099 2000..99999
    //  1100 to 1999 is eleven-hunderd to ninteen-hunderd
    if ((value >= 1000L && value <= 1099L) || value >= 2000L) {
        say_cardinal(value / 1000L);
        outstring("THAWzAEnd ");
        value = value % 1000L;
        if (value == 0)
            return;      //  Even thousand
        if (value < 100) //  as in THREE THOUSAND AND FIVE
            outstring("AEnd ");
    }

    if (value >= 100L) {
        outstring(Cardinals[value / 100]);
        outstring("hAHndrEHd ");
        value = value % 100;
        if (value == 0)
            return; //  Even hundred
    }

    if (value >= 20) {
        outstring(Twenties[(value - 20) / 10]);
        value = value % 10;
        if (value == 0)
            return; //  Even ten
    }

    outstring(Cardinals[value]);
    return;
}

/*
** Translate a number to phonemes.  This version is for ORDINAL numbers.
**     Note: this is recursive.
*/
void say_ordinal(value) long int value;
{
    if (value < 0) {
        outstring("mAHnAXs ");
        value = (-value);
        if (value < 0) //  Overflow!  -32768
        {
            outstring("IHnfIHnIHtIY ");
            return;
        }
    }

    if (value >= 1000000000L) //  Billions
    {
        say_cardinal(value / 1000000000L);
        value = value % 1000000000;
        if (value == 0) {
            outstring("bIHlIYAXnTH ");
            return; //  Even billion
        }
        outstring("bIHlIYAXn ");
        if (value < 100) //  as in THREE BILLION AND FIVE
            outstring("AEnd ");
    }

    if (value >= 1000000L) //  Millions
    {
        say_cardinal(value / 1000000L);
        value = value % 1000000L;
        if (value == 0) {
            outstring("mIHlIYAXnTH ");
            return; //  Even million
        }
        outstring("mIHlIYAXn ");
        if (value < 100) //  as in THREE MILLION AND FIVE
            outstring("AEnd ");
    }

    //  Thousands 1000..1099 2000..99999
    //  1100 to 1999 is eleven-hunderd to ninteen-hunderd
    if ((value >= 1000L && value <= 1099L) || value >= 2000L) {
        say_cardinal(value / 1000L);
        value = value % 1000L;
        if (value == 0) {
            outstring("THAWzAEndTH ");
            return; //  Even thousand
        }
        outstring("THAWzAEnd ");
        if (value < 100) //  as in THREE THOUSAND AND FIVE
            outstring("AEnd ");
    }

    if (value >= 100L) {
        outstring(Cardinals[value / 100]);
        value = value % 100;
        if (value == 0) {
            outstring("hAHndrEHdTH ");
            return; //  Even hundred
        }
        outstring("hAHndrEHd ");
    }

    if (value >= 20) {
        if ((value % 10) == 0) {
            outstring(Ord_twenties[(value - 20) / 10]);
            return; //  Even ten
        }
        outstring(Twenties[(value - 20) / 10]);
        value = value % 10;
    }

    outstring(Ordinals[value]);
    return;
}

//  !!!! End of SAYNUM.C

/* [tomj] These tables were modified to make the speech less explicit and more
colloquial
eg. "PAREN" instead of "OPEN PAREN". */

static char *Ascii[] = {

    //  Control codes, 0 - 31 decimal
    "nUWl ",
    "stAArt AXv hEHdER ",
    "stAArt AXv tEHkst ",
    "EHnd AXv tEHkst ",
    "EHnd AXv trAEnsmIHSHAXn",
    "EHnkwAYr ",
    "AEk ",
    "bEHl ",
    "bAEkspEYs ",
    "tAEb ",
    "lIHnIYfIYd ",
    "vERtIHkAXl tAEb ",
    "fAOrmfIYd ",
    "kAErAYj rIYtERn ",
    "SHIHft AWt ",
    "SHIHft IHn ",
    "dIHlIYt ",
    "dIHvIHs kAAntrAAl wAHn ",
    "dIHvIHs kAAntrAAl tUW ",
    "dIHvIHs kAAntrAAl THrIY ",
    "dIHvIHs kAAntrAAl fOWr ",
    "nAEk ",
    "sIHnk ",
    "EHnd tEHkst blAAk ",
    "kAEnsEHl ",
    "EHnd AXv mEHsIHj ",
    "sUWbstIHtUWt ",
    "EHskEYp ",
    "fAYEHld sIYpERAEtER ",
    "grUWp sIYpERAEtER ",
    "rIYkAOrd sIYpERAEtER ",
    "yUWnIHt sIYpERAEtER ",

    "spEYs ",
    "bAENG ",
    "kwOWt ",
    "nUWmbER ",
    "dAAlER sAYn ",
    "pERsEHnt ",
    "AXnd ",
    "kwOWt ",
    "pEHrEHn ",
    "pEHrEHn ",
    "stAAr ",
    "plAHs ",
    "kAAmmAX ",
    "mIHnAHs ",
    "pIYrIYAAd ",
    "slAESH ",

    "zIHrOW ",
    "wAHn ",
    "tUW ",
    "THrIY ",
    "fOWr ",
    "fAYv ",
    "sIHks ",
    "sEHvAXn ",
    "EYt ",
    "nAYn ",

    "kAAlAXn ",
    "sEHmIP1HkAAlAXn ",
    "lEHs DHAEn ",
    "EHkwAXls ",
    "grEYtER DHAEn ",
    "kwEHsCHAXn mAArk ",
    "P2AEt ",

    "EY ",
    "bIY ",
    "sIY ",
    "dIY ",
    "IY ",
    "EHf ",
    "jIY  ",
    "EYtCH ",
    "AY ",
    "jEY ",
    "kEY ",
    "EHl ",
    "EHm ",
    "EHn ",
    "AA ",
    "pIY ",
    "kY1UW ",
    "AAr ",
    "EHz ",
    "tIY ",
    "yU1 ",
    "vIY ",
    "dAHblyUWw ",
    "EHks ",
    "wAYIY ",
    "zIY ",

    "lEHft brAEkEHt ",
    "bAEkslAESH ",
    "rAYt brAEkEHt ",
    "kAErEHt ",
    "AHndERskAOr ",
    "AEpAAstrAAfIH ",

    "EY ",
    "bIY ",
    "sIY ",
    "dIY ",
    "IY ",
    "EHf ",
    "jIY  ",
    "EYtCH ",
    "AY ",
    "jEY ",
    "kEY ",
    "EHl ",
    "EHm ",
    "EHn ",
    "AA ",
    "pIY ",
    "kY1 ",
    "AAr ",
    "EHz ",
    "tIY ",
    "yU1 ",
    "vIY ",
    "dAHblyUWw ",
    "EHks ",
    "wAYIY ",
    "zIY ",

    "lEHft brEYs ",
    "vERtIHkAXl bAAr ",
    "rAYt brEYs ",
    "tAYld ",
    "dEHl ",
};

void say_ascii(character) int character;
{
    outstring(Ascii[character & 0x7F]);
    outstring("P4 ");
}

void spell_word(word) char *word;
{
    for (word++; word[1] != '\0'; word++) {
        outstring(Ascii[(*word) & 0x7F]);
        outstring("P4 ");
    }
}

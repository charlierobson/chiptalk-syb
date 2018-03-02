/* Prototypes created for t2a.c */

void outallo(char *);		/* [tomj] new function */
int is_punct(char);		/* [tomj] new function */
void have_punct(void);		/* [tomj] new function */

void outstring(char *);
void outchar(int);
int makeupper(int);
char new_char(void);
void xlate_file(void);
void have_dollars(void);
void have_special(void);
void have_number(void);
void have_letter(void);
void abbrev(char *);
int isvowel(char);
int isconsonant(char);
void xlate_word(char *);
int find_rule(char *, int, Rule *);
int leftmatch(char *, char *);
int rightmatch(char *, char *);
void say_cardinal(long int);
void say_ordinal(long int);
void say_ascii(int);
void spell_word(char *);


/*

Tom Jennings
mru 16 Jan 1999
initial edit 31 Dec 98

Rule table from John Wasser's PHONEME program, substantially revised. When this
program was written, 4MHz processor speeds and 64K of memory was the cutting edge;
small table size was important. With 64MB of memory and a Pentium, who cares. 

I doubled vowels where possible, greatly improving intelligibility, giving your
poor aching brain the extra milliseconds it takes to decode the poorly-formed phonemes.
Some rules were "cleaned up", and many exceptions were added. In some places, very brief
pauses were added to phonically clean up prefixes and artifically add emphasis.

The rules were specifically tuned to match the capabilities (sic) of the General Instrument
SPO256-AL2, especially the terrible voiced stops. B, D and G generally never sound right.

This chip is amazingly culturally-centric, apparently designed for American English. There
are no trilled R's, gutteral stops, etc, so all you Spanish, German, Dutch speakers are
outa' luck, and Japanese, Chinese, etc, don't even bother to ask... The vowel set is barely
functional for english, even. 

Needless to say, text-to-phoneme is utterly a dead end, and I'm surprised this wasn't noticed
by the theorists before this chip was made. Or maybe they did know, and the chip manufacturers
simply didn't know, or care to ask. No matter what you do, this thing sounds terrible!



Tables modified to include pause phonemes.

*/

/*	English to Phoneme rules.
**
**	Derived from: 
**
**	     AUTOMATIC TRANSLATION OF ENGLISH TEXT TO PHONETICS
**	            BY MEANS OF LETTER-TO-SOUND RULES
**
**			NRL Report 7948
**
**		      January 21st, 1976
**	    Naval Research Laboratory, Washington, D.C.
**
**
**	Published by the National Technical Information Service as
**	document "AD/A021 929".
**
**
**
**	The Phoneme codes:
**
**		IY	bEEt		IH	bIt
**		EY	gAte		EH	gEt
**		AE	fAt		AA	fAther
**		AO	lAWn		OW	lOne
**		UH	fUll		UW	fOOl
**		ER	mURdER		AX	About
**		AH	bUt		AY	hIde
**		AW	hOW		OY	tOY
**	
**		p	Pack		b	Back
**		t	Time		d	Dime
**		k	Coat		g	Goat
**		f	Fault		v	Vault
**		TH	eTHer		DH	eiTHer
**		s	Sue		z	Zoo
**		SH	leaSH		ZH	leiSure
**		HH	How		m	suM
**		n	suN		NG	suNG
**		l	Laugh		w	Wear
**		y	Young		r	Rate
**		CH	CHar		j	Jar
**		WH	WHere
**
**
**	Rules are made up of four parts:
**	
**		The left context.
**		The text to match.
**		The right context.
**		The phonemes to substitute for the matched text.
**
**	Procedure:
**
**		Seperate each block of letters (apostrophes included) 
**		and add a space on each side.  For each unmatched 
**		letter in the word, look through the rules where the 
**		text to match starts with the letter in the word.  If 
**		the text to match is found and the right and left 
**		context patterns also match, output the phonemes for 
**		that rule and skip to the next unmatched letter.
**
**
**	Special Context Symbols:
**
**		#	One or more vowels
**		:	Zero or more consonants
**		^	One consonant.
**		.	One of B, D, V, G, J, L, M, N, R, W or Z (voiced 
**			consonants)
**		%	One of ER, E, ES, ED, ING, ELY (a suffix)
**			(Found in right context only)
**		+	One of E, I or Y (a "front" vowel)
**
*/


/* Context definitions */
static char Anything[] = "";	/* No context requirement */
static char Nothing[] = " ";	/* Context is beginning or end of word */

/* Phoneme definitions */
static char Pause[] = " ";	/* Short silence */
static char Silent[] = "";	/* No phonemes */

#define LEFT_PART	0
#define MATCH_PART	1
#define RIGHT_PART	2
#define OUT_PART	3

typedef char *Rule[4];	/* Rule is an array of 4 character pointers */

/*0 = Punctuation */
/*
**	LEFT_PART	MATCH_PART	RIGHT_PART	OUT_PART
*/

static Rule punct_rules[] =
	{
	{Anything,	" ",		Anything,	" P4P3 "	},
	{Anything,	"-",		Anything,	" P4 " 	},
	{".",		"'S",		Anything,	"z"	},
	{"#:.E",	"'S",		Anything,	"z"	},
	{"#",		"'S",		Anything,	"z"	},
	{Anything,	"'",		Anything,	" P1 "	},
	{Anything,	";",		Anything,	" P5 " 	},
	{Anything,	":",		Anything,	" P5 "	},
	{Anything,	",",		Anything,	" P5 "	},

	{Anything,	".",		"#",		Silent	},	/* U.S. */
	{Anything,	".",		"^",		Silent	},	/* U.S. */
	{Anything,	".",		Anything,	" P5P5P4 "	},

	{Anything,	"?",		Anything,	" P5P5P4 "	},
	{Anything,	"!",		Anything,	" P5P5P4 "	},
	{Anything,	0,		Anything,	Silent	},
	};

/*
**	LEFT_PART	MATCH_PART	RIGHT_PART	OUT_PART
*/
static Rule A_rules[] =
	{
	{Nothing,	"A",		Nothing,	"EHEY"	},
	{Anything,	"AHEAD",	Anything,	"AXhEHEHD1"	},
	{Anything,	"APROPOS",	Anything,	"AEprOWpOW"	},
	{Anything,	"ASS",		"H",		"AEAEss"	},
	{Anything,	"ALLEGE",	Anything,	"AXlEHdj"	},
	{Anything,	"AGAIN",	Anything,	"AXG3EHEHn"},
	{Nothing,	"ABLE",		Anything,	"EYhbAXl"},
	{Nothing,	"ABOVE",	Nothing,	"AXbAXAXvh"},
	{Nothing,	"ACRO",		".",		"AEhkrOW"},
	{Nothing,	"ARE",		Nothing,	"AAER"	},
	{Nothing,	"ALLY",		Nothing,	"AEAElAY"	},
	{Anything,	"ATOMIC",	Anything,	"AXtAAmP1IHk"	},
	{Anything,	"ARCH",		"#V",		"AXAXrP1kIH"	},
	{Anything,	"ARCH",		"#.",		"AXAXrCHIH"	},
	{Anything,	"ARCH",		"#^",		"AXAXrkP1IH"	},
	{Anything,	"ARGUE",	Anything,	"AAERgyUW"	},

	{Nothing,	"ABB",		Anything,	"AXAXb"	},
	{Nothing,	"AB",		Anything,	"AEAEB2P2"	},
	{Nothing,	"AN",		"#",		"AEn"	},
	{Nothing,	"ALLO",		"T",		"AElAA"	},
	{Nothing,	"ALLO",		"W",		"AElAW"	},
	{Nothing,	"ALLO",		Anything,	"AElOW"	},
	{Nothing,	"AR",		"O",		"AXER"	},

	{"#:",		"ALLY",		Anything,	"P1AXlIY"	},
	{"^",		"ABLE",		Anything,	"P1EYhbAXl"},
	{Anything,	"ABLE",		Anything,	"P1AXhbAXl"},
	{"^",		"ANCE",		Anything,	"P1AEns"},
	{Anything,	"AIR",		Anything,	"EYXR"	},
	{Anything,	"AIC",		Nothing,	"EYIHk"	},
	{"#:",		"ALS",		Nothing,	"AXlz"	},
	{Anything,	"ALK",		Anything,	"AOAOk"	},
	{Anything,	"ARR",		Anything,	"AAr"	},
	{Anything,	"ANG",		"+",		"EYnj"	},
	{" :",		"ANY",		Anything,	"EHnIY"	},
	{Anything,	"ARY",		Nothing,	"P1AXERIY"	},
	{"^",		"AS",		"#",		"EYs"	},
	{"#:",		"AL",		Nothing,	"AXl"	},
	{Anything,	"AL",		"^",		"AOl"	},
	{Nothing,	"AL",		"#",		"EHEYl"	},
	{"#:",		"AG",		"E",		"IHj"	},

	{Anything,	"AI",		Anything,	"EHEY"	},
	{Anything,	"AY",		Anything,	"EHEY"	}, 
	{Anything,	"AU",		Anything,	"AOAO"	},
	{Anything,	"AW",		Nothing,	"AOAO"	},
	{Anything,	"AW",		"^",		"AOAO"	},
	{":",		"AE",		Anything,	"EH"	},
	{Anything,	"A",		"TION",		"EY"	},	/* -mation */
	{"C",		"A",		"BL",		"EHEY"	},	/* cable */
	{"C",		"A",		"B#",		"AEAE"	},	/* cabin */
	{"C",		"A",		"PAB",		"EHEY"	},	/* capable */
	{"C",		"A",		"P#",		"AEAE"	},	/* capital capacity */
	{"C",		"A",		"T#^",		"AEAE"	},	/* category */

	{"^^^",		"A",		Anything,	"EY"	},	/* prefixed, eg. translate */
	{"^.",		"A",		"^E",		"EY"	},	/* grate */
	{"^.",		"A",		"^I",		"EY"	},	/* slaking */
	{"^^",		"A",		Anything,	"AE"	},	/* practice, apparent */
	{"^",		"A",		"^##",		"EY"	},	/* radio */
	{"^",		"A",		"^#",		"EY"	},	/* taker, paper, later */
	{"^",		"A",		"^#",		"EHEY"	},	/* ware */
	{Anything,	"A",		"^%",		"EY"	},
	{"#",		"A",		Nothing,	"AO"	},
	{Anything,	"A",		"WA",		"AX"	},
	{Anything,	"A",		Nothing,	"AX"	},
	{Anything,	"A",		"^+#",		"EY"	},
	{Anything,	"A",		"^+:#",		"AE"	},
	{" :",		"A",		"^+ ",		"EY"	},
	
	{Anything,	"A",		Anything,	"AE"	},
 	{Anything,	0,		Anything,	Silent	},
	};

/*
**	LEFT_PART	MATCH_PART	RIGHT_PART	OUT_PART
*/
static Rule B_rules[] =
	{
	{"B",		"B",		Anything,	Silent	},
	{Anything,	"BI",		"CYCLE",	"bAY"},
	{Anything,	"BI",		"CYCLE",	"bAY"},
	{Anything,	"BBQ",		Anything,	"bAXAXrbAXK1yUW"},
	{Anything,	"BARBEQUE",	Anything,	"bAXAXrbAXK1yUW"},
	{Anything,	"BARBAQUE",	Anything,	"bAXAXrbAXK1yUW"},
	{Anything,	"BARGAIN",	Anything,	"bAOrgEHn"},
	{Anything,	"BAGEL",	Anything,	"bEYgEHl"},
	{Anything,	"BEING",	Anything,	"bIYIHNG"},
	{Anything,	"BOMB",		Anything,	"bAAAAm"	},
	{Nothing,	"BOTH",		Nothing,	"bOWTH"	},
	{Anything,	"BUIL",		Anything,	"bIHl"	},
	{Nothing,	"BUS",		"Y",		"bIHz"	},
	{Nothing,	"BUS",		"#",		"bIHz"	},
	{Anything,	"BYE",		Anything,	"bAOAY"	},
	{Anything,	"BEAR",		Nothing,	"bEYER"	},
	{Anything,	"BEAR",		"%",		"bEYER"	},
	{Anything,	"BEAR",		"S",		"bEYER"	},
	{Anything,	"BEAR",		"#",		"bEYER"	},

	{Anything,	"BAN",		"ISH",		"bAEAEn"	},

	{Nothing,	"BE",		"^#",		"bIH"	},
	{Nothing,	"BY",		Anything,	"bAOAY"	},
	{"Y",		"BE",		Nothing,	"bIY"	},

	{Nothing,	"B",		"#",		"b"	},
	{Anything,	"B",		Nothing,	"B2"	},
	{Anything,	"B",		"#",		"B2"	},
	{Anything,	"B",		"L",		"B2"	},
	{Anything,	"B",		"R",		"B2"	},

	{Anything,	"B",		Anything,	"b"	},
	{Anything,	0,		Anything,	Silent	},
	};

/*
**	LEFT_PART	MATCH_PART	RIGHT_PART	OUT_PART
*/
static Rule C_rules[] =
	{
	{Anything,	"CHINESE",	Anything,	"CHAYnIYs"	}, 
	{Anything,	"COUNTRY",	Anything,	"kAXAXntrIY"	}, 
	{Anything,	"CHRIST",	Nothing,	"K2rAYst"	}, 
	{Anything,	"CHASSIS",	Anything,	"CHAXAXsIY"	}, 
	{Anything,	"CLOSET",	Anything,	"K2lAOAOzEHt"	}, 
	{Anything,	"CHINA",	Anything,	"CHAYnAX"	}, 
	{Nothing,	"CAFE",		Nothing,	"kAEfAEEY"	}, 
	{Anything,	"CELE",		Anything,	"sEHlP1EH"	},
	{Anything,	"CYCLE",	Anything,	"sAYK2UHl"	},
	{Anything,	"CHRON",	Anything,	"krAOn"	},
	{Anything,	"CREA",		"T",		"K2rIYEY"	},
	{Nothing,	"CRY",		Nothing,	"K2rIY"	},
	{Nothing,	"CHRY",		Anything,	"K2rAOAY"	},
	{Nothing,	"CRY",		"#",		"K2rAOAY"	},
	{Nothing,	"CAVEAT",	":",		"kAEvIYAEt"	}, 
	{"^",		"CUIT",		Anything,	"kIHt"	}, 
	{Anything,	"CHAIC",	Anything,	"kEYIHk"	},
	{Anything,	"CATION",	Anything,	"kEYSHAXn"	},
	{Nothing,	"CH",		"ARACT",		"k"	},
	{Nothing,	"CH",		"^",		"k"	},
	{"^E",		"CH",		Anything,	"k"	},
	{Anything,	"CH",		Anything,	"CH"	},
	{" S",		"CI",		"#",		"sAY"	},
	{Anything,	"CI",		"A",		"SH"	},
	{Anything,	"CI",		"O",		"SH"	},
	{Anything,	"CI",		"EN",		"SH"	},
	{Anything,	"C",		"+",		"s"	},
	{Anything,	"CK",		Anything,	"K1"	},
	{Anything,	"COM",		"%",		"kAHm"	},
/*	{Anything,	"C",		"^",		"K2"	},	*/

	{Anything,	"C",		"U",		"K2"	},	/* these same as K rules */
	{Anything,	"C",		"O",		"K2"	},
	{Anything,	"C",		"A^^",		"K2"	},
	{Anything,	"C",		"O^^",		"K2"	},
	{Anything,	"C",		"L",		"K2"	},
	{Anything,	"C",		"R",		"K2"	},

	{Anything,	"C",		"A",		"k"	},
	{Anything,	"C",		"E",		"k"	},
	{Anything,	"C",		"I",		"k"	},

	{Anything,	"C",		Nothing,	"K1"	},
	{Anything,	"C",		Anything,	"k"	},
	{Anything,	0,		Anything,	Silent	},
	};

/*
**	LEFT_PART	MATCH_PART	RIGHT_PART	OUT_PART
*/
static Rule D_rules[] =
	{
	{Anything,	"DEAD",		Anything,	"dEHEHD1"},
	{Nothing,	"DOGGED",	Anything,	"dAOgP1EHD1"	},
	{"#:",		"DED",		Nothing,	"dIHD1"	},
	{Nothing,	"DIG",		Anything,	"dIHIHg"	},
	{Nothing,	"DRY",		Nothing,	"drAOAY"	},
	{Nothing,	"DRY",		"#",		"drAOAY"	},
	{Nothing,	"DE",		"^#",		"dIH"	},
	{Nothing,	"DO",		Nothing,	"dUW"	},
	{Nothing,	"DOES",		Anything,	"dAHz"	},
	{Nothing,	"DOING",	Anything,	"dUWIHNG"},
	{Nothing,	"DOW",		Anything,	"dAW"	},
	{Anything,	"DU",		"A",		"jUW"	},
	{Anything,	"DYNA",		Anything,	"dAYnAXP1"	},
	{Anything,	"DYN",		"#",		"dAYnP1"	},
	{"D",		"D",		Anything,	Silent	},
	{Anything,	"D",		Nothing,	"D1"	},
	{Nothing,	"D",		Anything,	"d"	},
	{Anything,	"D",		Anything,	"d"	},
	{Anything,	0,		Anything,	Silent	},
	};

/*
**	LEFT_PART	MATCH_PART	RIGHT_PART	OUT_PART
*/
static Rule E_rules[] =
	{
	{Nothing,	"EYE",		Anything,	"AAAY"	},
	{Anything,	"ERED",		Nothing,	"ERD1"	},
	{Nothing,	"EGO",		Anything,	"IYgOW"	},
	{Nothing,	"ERR",		Anything,	"EHEHr"	},
	{"^",		"ERR",		Anything,	"EHEHr"	},
	{Anything,	"EV",		"ER",		"EHEHvh"	}, /* ZZ */
	{Anything,	"E",		"NESS",		Silent	},
/*	{Anything,	"E",		"^%",		"IY"	},	*/
	{Anything,	"ERI",		"#",		"IYXRIY"	},
	{Anything,	"ERI",		Anything,	"EHrIH"	},
	{"#:",		"ER",		"#",		"ER"	},
	{Anything,	"ER",		"#",		"EHEHr"	},
	{Anything,	"ER",		Anything,	"ER"	},
	{Nothing,	"EVIL",		Anything,	"IYvEHl"},
	{Nothing,	"EVEN",		Anything,	"IYvEHn"},
	{"M",		"EDIA",		Anything,	"IYdIYAX"	},
	{Anything,	"ECIA",		Anything,	"IYSHIYEY"	},
	{":",		"ELEG",		Anything,	"EHlEHg"	},
	
	{"#:",		"E",		"W",		Silent	},
	{"T",		"EW",		Anything,	"UW"	},
	{"S",		"EW",		Anything,	"UW"	},
	{"R",		"EW",		Anything,	"UW"	},
	{"D",		"EW",		Anything,	"UW"	},
	{"L",		"EW",		Anything,	"UW"	},
	{"Z",		"EW",		Anything,	"UW"	},
	{"N",		"EW",		Anything,	"UW"	},
	{"J",		"EW",		Anything,	"UW"	},
	{"TH",		"EW",		Anything,	"UW"	},
	{"CH",		"EW",		Anything,	"UW"	},
	{"SH",		"EW",		Anything,	"UW"	},
	{Anything,	"EW",		Anything,	"yUW"	},
	{Anything,	"E",		"O",		"IY"	},
	{"#:S",		"ES",		Nothing,	"IHz"	},
	{"#:C",		"ES",		Nothing,	"IHz"	},
	{"#:G",		"ES",		Nothing,	"IHz"	},
	{"#:Z",		"ES",		Nothing,	"IHz"	},
	{"#:X",		"ES",		Nothing,	"IHz"	},
	{"#:J",		"ES",		Nothing,	"IHz"	},
	{"#:CH",	"ES",		Nothing,	"IHz"	},
	{"#:SH",	"ES",		Nothing,	"IHz"	},
	{"#:",		"E",		"S ",		Silent	},
	{"#:",		"ELY",		Nothing,	"lIY"	},
	{"#:",		"EMENT",	Anything,	"P1mEHnt"	},
	{Anything,	"EFUL",		Anything,	"P1fUHl"	},
	{Anything,	"EE",		Anything,	"IY"	},
	{Anything,	"EARN",		Anything,	"ERn"	},
	{Nothing,	"EAR",		"^",		"ER"	},
	{"K.",		"EAD",		Anything,	"IYd"	},	/* knead, K silent */
	{"^.",		"EAD",		Anything,	"EHd"	},	/* dread, spread */
	{"D",		"EAD",		Anything,	"EHd"	},	/* dead */
	{Anything,	"EAD",		Anything,	"IYd"	},	/* bead */
	{"#:",		"EA",		Nothing,	"IYAX"	},
	{"#:",		"EA",		"S",		"IYAX"	},
	{Anything,	"EA",		"SU",		"EH"	},
	{Anything,	"EA",		Anything,	"IY"	},
	{Anything,	"EIGH",		Anything,	"EY"	},
	{"L",		"EI",		Anything,	"IY"	},
	{".",		"EI",		Anything,	"EY"	},
	{Anything,	"EI",		"N",		"AY"	},
	{Anything,	"EI",		Anything,	"IY"	},
	{Anything,	"EY",		Anything,	"IY"	},
	{Anything,	"EU",		Anything,	"yUW"	},

	{"#:",		"E",		"D ",		Silent	},
	{"#S",		"E",		"^",		Silent	},
	{":",		"E",		"X",		"EHEH"	},
	{"#:",		"E",		Nothing,	Silent	},
	{"+:",		"E",		Nothing,	Silent	},
	{"':^",		"E",		Nothing,	Silent	},
	{":",		"EQU",		Anything,	"IYkw"	},
	{"DG",		"E",		Anything,	Silent	},
	{"DH",		"E",		Anything,	"IY"	},
	{" :",		"E",		Nothing,	"IY"	},
	{"#",		"ED",		Nothing,	"D1"	},
	{Anything,	"E",		Anything,	"EH"	},
	{Anything,	0,		Anything,	Silent	},
	};

/*
**	LEFT_PART	MATCH_PART	RIGHT_PART	OUT_PART
*/
static Rule F_rules[] =
	{
	{Anything,	"FNORD",	Anything,	"fnAOORD1"	},
	{Anything,	"FOUR",		Anything,	"fOWr"	},
	{Anything,	"FUL",		Anything,	"P1fUHl"	},
	{Nothing,	"FLY",		Anything,	"flAOAY"	},
	{".",		"FLY",		Anything,	"flAOAY"	},
	{Anything,	"FIXED",	Anything,	"fIHkst"	},
	{Anything,	"FIVE",		Anything,	"fAOAYv"	},
	{Anything,	"FOOT",		Anything,	"fUHUHt"	},
	{Anything,	"F",		Anything,	"f"	},
	{Anything,	0,		Anything,	Silent	},
	};

/*
**	LEFT_PART	MATCH_PART	RIGHT_PART	OUT_PART
*/
static Rule G_rules[] =
	{
	{Anything,	"GADGET",	Anything,	"G2AEAED1P2jEHEHt"	},
	{Anything,	"GOD",		Anything,	"G3AAAAD1"	},
	{Anything,	"GET",		Anything,	"G3EHEHt"	},
	{Anything,	"GEN",		"^",		"jEHEHn"	},
	{Anything,	"GEN",		"#^",		"jEHEHn"	},
	{Anything,	"GEN",		Nothing,	"jEHEHn"	},
	{Anything,	"GIV",		Anything,	"G2IHIHvh"	},
	{"SU",		"GGES",		Anything,	"gjEHs"	},
	{Anything,	"GREAT",	Anything,	"G2rEYt"	},
	{Anything,	"GOOD",		Anything,	"G2UHUHD1"	},
	{Anything,	"GUE",		Anything,	"G3"	},

	{"D",		"G",		Anything,	"j"	},
	{"##",		"G",		Anything,	"g"	},
	{Anything,	"G",		"+",		"j"	},
	{Anything,	"GG",		Anything,	"G3P1"	},

	{"CAMPAI",	"G",		"N",		Silent,	},
	{"ARRAI",	"G",		"N",		Silent,	},
	{"ALI",		"G",		"N",		Silent,	},
	{"BENI",	"G",		"N",		Silent,	},
	{"ARRAI",	"G",		"N",		Silent,	},

	{Anything,	"G",		"A",		"g"	},
	{Anything,	"G",		"E",		"g"	},
	{Anything,	"G",		"I",		"g"	},
	{Anything,	"G",		"Y",		"g"	},

	{Anything,	"G",		"O",		"G2"	},
	{Anything,	"G",		"U",		"G2"	},
	{Anything,	"G",		"L",		"G2"	},
	{Anything,	"G",		"R",		"G2"	},


	{Anything,	"G",		Nothing,	"G3"	},
	{"N",		"G",		Anything,	"G3"	},
	{Anything,	"G",		Anything,	"G3"	},
	{Anything,	0,		Anything,	Silent	},
	};

/*
**	LEFT_PART	MATCH_PART	RIGHT_PART	OUT_PART
*/
static Rule H_rules[] =
	{{Anything,	"HONOR",	Anything,	"AOnER"	},
	{Anything,	"HEARD",	Anything,	"hERD1"	},
	{Anything,	"HEIGHT",	Anything,	"hAYt"	},
	{Anything,	"HONEST",	Anything,	"AOnEHst"	},
	{Anything,	"HOOD",		Anything,	"hUHUHD1"	},
	{"AB",		"HOR",		Anything,	"OWER"	},
	{Anything,	"HEAVY",	Anything,	"hAEvIY"	},
	{Anything,	"HEART",	Anything,	"hAArt"	},
	{Anything,	"HALF",		Anything,	"hAEAEf"	},
	{Anything,	"HIVE",		Anything,	"hAAAYv"	},
	{Anything,	"HEAVI",	":#",		"hAEvIY"	},
	{Nothing,	"HAV",		Anything,	"hAEvh"	},
	{Anything,	"HA",		Nothing,	"hAAAA"	},
	{Nothing,	"HI",		Nothing,	"hAAAY"	},
	{Anything,	"HE",		"T",		"hAE"	},
	{Anything,	"HE",		"X",		"hAE"	},
	{Anything,	"HY",		Anything,	"hAAAY"	},
	{Nothing,	"HANG",		Anything,	"hAENG"	},
	{Nothing,	"HERE",		Anything,	"hIYXR"	},
	{Nothing,	"HOUR",		Anything,	"AWER"	},
	{Anything,	"HOW",		Anything,	"hAW"	},
	{Anything,	"H",		"ONOR",		Silent	},
	{Anything,	"H",		"ONEST",	Silent	},
	{Anything,	"H",		"#",		"h"	},
	{Anything,	"H",		Anything,	Silent	},
	{Anything,	0,		Anything,	Silent	},
	};

/*
**	LEFT_PART	MATCH_PART	RIGHT_PART	OUT_PART
*/
static Rule I_rules[] =
	{
	{Nothing,	"I",		Nothing,	"AOAY"	},
	{Nothing,	"II",		Nothing,	"tUW"	},
	{Nothing,	"III",		Nothing,	"THrIY"	},

	{Nothing,	"INTRIGU",	"#",		"IHntrIYg"	},	/* intriguING */
	{Nothing,	"ISO",		Anything,	"AYsOW"	},
	{Anything,	"ITY",		Nothing,	"P1IHtIY"	},
	{Nothing,	"IN",		Anything,	"IHIHn"	},
	{Nothing,	"I",		"O",		"AY"	},
	{Anything,	"IFY",		Anything,	"P1IHfAY"	},
	{Anything,	"IGH",		Anything,	"AY"	},
	{Anything,	"ILD",		Anything,	"AYlD1"	},
	{Anything,	"IGN",		Nothing,	"AYn"	},
	{Anything,	"IN",		"D",		"AYn"	},
	{Anything,	"IER",		Anything,	"IYER"	},
	{Anything,	"IDEA",		Anything,	"AYdIYAX"	},
	{Nothing,	"IDL",		Anything,	"AYdAXlYYY"	},
	{Anything,	"IRON",		Anything,	"AAAYERn"	},
	{Anything,	"IBLE",		Anything,	"IHB2l"	},
	{"R",		"IEND",		Anything,	"AEnD1"	},
	{Anything,	"IEND",		Anything,	"IYnD1"	},
	{"#:R",		"IED",		Anything,	"IYD1"	},
	{Anything,	"IED",		Nothing,	"AYD1"	},
	{Anything,	"IEN",		Anything,	"IYEHn"	},
	{Anything,	"ION",		Anything,	"yAXn"	},
	{"CH",		"INE",		Anything,	"IYn"	},
	{"ENT",		"ICE",		Anything,	"AYs"	},	/* entice */
	{Anything,	"ICE",		Anything,	"IHs"	},	/* practice, lattice */
	{Anything,	"IEC",		"%",		"IYss"	},
	{"#.",		"IES",		Nothing,	"IYz"	},	/* families */
	{Anything,	"IES",		Nothing,	"AYz"	},	/* implies */
	{Anything,	"IE",		"T",		"AYEH"	},
	{Anything,	"IE",		"^",		"IY"	},
	{Anything,	"I",		"CATION",	"IH"	},	/* amplification */

	{Anything,	"ING",		Anything,	"IHNG"	},
	{Anything,	"IGN",		"^",		"AAAYn"	},
	{Anything,	"IGN",		"%",		"AAAYn"	},
	{Anything,	"IQUE",		Anything,	"IYk"	},
	{Anything,	"ISH",		Anything,	"IHSH"	},


	{Nothing,	"IR",		Anything,	"YR"	},
	{Anything,	"IR",		"#",		"AAAYr"	},
	{Anything,	"IR",		Anything,	"ER"	},
	{Anything,	"IZ",		"%",		"AAAYz"	},
	{Anything,	"IS",		"%",		"AAAYz"	},

	{"^CH",		"I",		".",		"AAAY"	},	/* archive */
	{"^CH",		"I",		"^",		"IH"	},	/* architect */
	{" #^",		"I",		"^",		"IH"	},	/* opinion */
	{"^#^",		"I",		"^",		"IH"	},	/* position, similar */
	{"^#^",		"I",		"#",		"IY"	},	/* radial, radius */
	{".",		"I",		Nothing,	"AOAY"	},
	{"#^",		"I",		"^#",		"AY"	},	/* -icide */
	{Anything,	"I",		"GUE",		"IY"	},
	{".",		"I",		"VE",		"AAAY"	},
	{Anything,	"I",		"VE",		"IH"	},
	{Anything,	"I",		"^+:#",		"IH"	},
	{".",		"I",		"O",		"AOAY"	},
	{"#^",		"I",		"^ ",		"IH"	},	/* cabin */
	{"#^",		"I",		"^#^",		"IH"	},	/* cabinet */
	{"#^",		"I",		"^",		"IY"	},	/* business */
	{"^",		"I",		"^#",		"AY"	},	/* bifocal */
	{"^",		"I",		"O",		"IY"	},
	{".",		"I",		"A",		"AY"	},	/* dial, trial */
	{Anything,	"I",		"A",		"IY"	},	/* axial */
	{" :",		"I",		"%",		"AY"	},
	{Anything,	"I",		"%",		"IY"	},
	{".",		"I",		".#",		"AAAYXX"	},	/* sliding */
	{Anything,	"I",		"D%",		"AHAY"	},
	{"+^",		"I",		"^+",		"AHAY"	},
	{Anything,	"I",		"T%",		"AHAY"	},
	{"#:^",		"I",		"^+",		"AHAY"	},
	{Anything,	"I",		"^+",		"AHAY"	},
	{".",		"I",		".",		"IHIH"	},
	{Anything,	"I",		"NUS",		"AAAY"	},
	{Anything,	"I",		Anything,	"IH"	},
	{Anything,	0,		Anything,	Silent	},
	};

/*
**	LEFT_PART	MATCH_PART	RIGHT_PART	OUT_PART
*/
static Rule J_rules[] =
	{
	{Anything,	"JAPANESE",	Anything,	"jAXpAEAEnIYss"	},
	{Anything,	"JAPAN",	Anything,	"jAXpAEAEn"	},
	{Anything,	"JULY",		Anything,	"jUWlAEAY"	},
	{Anything,	"JESUS",	Anything,	"jIYzAXs"	},
	{Anything,	"J",		Anything,	"j"	},
	{Anything,	0,		Anything,	Silent	},
	};

/*
**	LEFT_PART	MATCH_PART	RIGHT_PART	OUT_PART
*/
static Rule K_rules[] =
	{
	{Nothing,	"K",		"N",		Silent	},

	{Anything,	"K",		"U",		"K2"	},
	{Anything,	"K",		"O",		"K2"	},
	{Anything,	"K",		"A^^",		"K2"	},
	{Anything,	"K",		"O^^",		"K2"	},
	{Anything,	"K",		"L",		"K2"	},
	{Anything,	"K",		"R",		"K2"	},

	{Anything,	"K",		"A",		"k"	},
	{Anything,	"K",		"E",		"k"	},
	{Anything,	"K",		"I",		"k"	},

	{Anything,	"K",		Nothing,	"K1"	},
	{Anything,	"K",		Anything,	"k"	},
	{Anything,	0,		Anything,	Silent	},
	};

/*
**	LEFT_PART	MATCH_PART	RIGHT_PART	OUT_PART
*/
static Rule L_rules[] =
	{
	{"L",		"L",		Anything,	Silent	},
	{Nothing,	"LION",		Anything,	"lAYAXn"	},
	{Anything,	"LEAD",		Anything,	"lIYD1"	},
	{Anything,	"LEVEL",	Anything,	"lEHvAXl"	},
	{Anything,	"LIBER",	Anything,	"lIHbER"	},
	{Nothing,	"LOSE",		Anything,	"lUWz"	},
	{Nothing,	"LIV",		Anything,	"lIHv"	},
	{"^",		"LIV",		Anything,	"lAYv"	},
	{"#",		"LIV",		Anything,	"lIHv"	},
	{Anything,	"LIV",		Anything,	"lAYv"	},
	{Anything,	"LO",		"C#",		"lOW"	},
	{"#:^",		"L",		"%",		"l"	},

	{Anything,	"LY",		Nothing,	"P1lIY"	},
	{Anything,	"L",		Anything,	"l"	},
	{Anything,	0,		Anything,	Silent	},
	};

/*
**	LEFT_PART	MATCH_PART	RIGHT_PART	OUT_PART
*/
static Rule M_rules[] =
	{
	{"M",		"M",		Anything,	Silent	},
	{Nothing,	"MY",		Nothing,	"mAY"	},
	{Nothing,	"MARY",		Nothing,	"mEYXRIY"	},
	{"#",		"MARY",		Nothing,	"P1mEYXRIY"	},
	{Anything,	"MICRO",	Anything,	"mAYkrOW"	},
	{Anything,	"MONO",		".",		"mAAnOW"	},
	{Anything,	"MONO",		"^",		"mAAnAA"	},
	{Anything,	"MON",		"#",		"mAAAAn"	},
	{Anything,	"MOS",		Anything,	"mOWs"	},
	{Anything,	"MOV",		Anything,	"mUWvh"	},
	{"TH",		"M",		"#",		"m"	},
	{"TH",		"M",		Nothing,	"IHm"	},
	{Anything,	"M",		Anything,	"m"	},
	{Anything,	0,		Anything,	Silent	},
	};

/*
**	LEFT_PART	MATCH_PART	RIGHT_PART	OUT_PART
*/
static Rule N_rules[] =
	{
	{"N",		"N",		Anything,	Silent	},
	{Nothing,	"NOW",		Nothing,	"nAW"	},
	{"#",		"NG",		"+",		"nj"	},
	{Anything,	"NG",		"R",		"NGg"	},
	{Anything,	"NG",		"#",		"NGg"	},
	{Anything,	"NGL",		"%",		"NGgAXl"},
	{Anything,	"NG",		Anything,	"NG"	},
	{Anything,	"NK",		Anything,	"NGk"	},
	{Nothing,	"NONE",		Anything,	"N2AHn"	},
	{Nothing,	"NON",		":",		"N2AAAAn"	},
	{Anything,	"NUC",		"L",		"N2U1k"	},

	{"R",		"N",		Anything,	"n"	},
	{Anything,	"N",		"#R",		"n"	},
	{Anything,	"N",		"O",		"N2"	},

	{Anything,	"N",		Anything,	"n"	},
	{Anything,	0,		Anything,	Silent	},
	};

/*
**	LEFT_PART	MATCH_PART	RIGHT_PART	OUT_PART
*/
static Rule O_rules[] =
	{
	{Nothing,	"ONLY",		Anything,	"OWnlIY"},
	{Nothing,	"ONCE",		Anything,	"wAHns"	},
	{Nothing,	"OH",		Nothing,	"OW"	},
	{Nothing,	"OK",		Nothing,	"OWP3kEHEY"	},
	{Nothing,	"OKAY",		Nothing,	"OWP3kEHEY"	},
	{Nothing,	"OHIO",		Nothing,	"OWhAYOW"	},
	{Nothing,	"OVER",		Anything,	"OWvER"	},
	{Anything,	"OTHER",	Anything,	"AHDHER"},
	{Anything,	"OHM",		Nothing,	"OWm"	},

	{Anything,	"ORIGIN",	Anything,	"ORIHdjIHn"	},
	{Anything,	"OROUGH",	Anything,	"EROW"	},
	{Anything,	"OUGHT",	Anything,	"AOt"	},
	{Anything,	"OCCU",		"P",		"AAkP1U1"	},
	{Anything,	"OUGH",		Anything,	"AHf"	},
	{Anything,	"ORE",		Anything,	"OWr"	},
	{"#:",		"ORS",		Nothing,	"ERz"	},
	{Anything,	"ORR",		Anything,	"AOr"	},
	{"D",		"ONE",		Anything,	"AHn"	},
	{"^Y",		"ONE",		Anything,	"wAHn"	},	/* anyone, everyone, not bayonet */
	{Nothing,	"ONE",		Anything,	"wAHn"	},
	{Anything,	"OUR",		Nothing,	"AWr"	},	/* sour */
	{Anything,	"OUR",		"^",		"OR"	},	/* course */
	{Anything,	"OUR",		Anything,	"AOAWr"	},
	{"T",		"OWN",		Anything,	"AWn"	},
	{"BR",		"OWN",		Anything,	"AWn"	},
	{"FR",		"OWN",		Anything,	"AWn"	},
	{Anything,	"OLO",		Anything,	"AOAAlAO"	},
	{Anything,	"OULD",		Anything,	"UHD1"	},
	{Anything,	"OUP",		Anything,	"UWp"	},
	{Anything,	"OING",		Anything,	"OWIHNG"},
	{Anything,	"OMB",		"%",		"OWm"	},
	{Anything,	"OOR",		Anything,	"AOr"	},
	{Anything,	"OOK",		Anything,	"UHk"	},
	{Anything,	"ON'T",		Anything,	"OWnt"	},
	{Anything,	"OSS",		Nothing,	"AOs"	},

	{Anything,	"OF",		Nothing,	"AXAXvh"	},
	{"^",		"OR",		Nothing,	"AOAOr"	},
	{"#:",		"OR",		Nothing,	"ER"	},
	{Anything,	"OR",		Anything,	"AOAOr"	},
	{Anything,	"OW",		Nothing,	"OW"	},
	{Anything,	"OW",		"#",		"OW"	},	/* blowout, lower */
	{Anything,	"OW",		".",		"OW"	},
	{Anything,	"OW",		Anything,	"AW"	},
	{" L",		"OV",		Anything,	"AHvh"	},
	{" D",		"OV",		Anything,	"AHvh"	},
	{"GL",		"OV",		Anything,	"AHvh"	},
	{"^",		"OV",		Anything,	"OWvh"	},
	{Anything,	"OV",		Anything,	"AHvh"	},
	{Anything,	"OL",		"D",		"OWl"	},
	{Nothing,	"OU",		Anything,	"AW"	},
	{"H",		"OU",		"S#",		"AW"	},
	{"AC",		"OU",		"S",		"UW"	},
	{"^",		"OU",		"^L",		"AH"	},
	{Anything,	"OU",		Anything,	"AW"	},
	{Anything,	"OA",		Anything,	"OW"	},
	{Anything,	"OY",		Anything,	"OY"	},
	{Anything,	"OI",		Anything,	"OY"	},
	{"I",		"ON",		Anything,	"AXAXn"	},
	{"#:",		"ON",		Nothing,	"AXAXn"	},
	{"#^",		"ON",		Anything,	"AXAXn"	},
	{Anything,	"OF",		"^",		"AOf"	},
	{"#:^",		"OM",		Anything,	"AHm"	},
	{Anything,	"OO",		Anything,	"UW"	},

	{Anything,	"OUS",		Anything,	"AXs"	},

	{"^#^",		"O",		"^",		"AX"	},	/*  */
	{"^#^",		"O",		"#",		"OW"	},	/* */
	{"#",		"O",		".",		"OW"	},
	{"^",		"O",		"^#^",		"AXAX"	},	/* comical */
	{"^",		"O",		"^#",		"OW"	},
	{Anything,	"O",		"^%",		"OW"	},
	{Anything,	"O",		"^EN",		"OW"	},
	{Anything,	"O",		"^I#",		"OW"	},
	{Anything,	"O",		"E",		"OW"	},
	{Anything,	"O",		Nothing,	"OW"	},
	{"C",		"O",		"N",		"AA"	},
	{Anything,	"O",		"NG",		"AO"	},
	{" :^",		"O",		"N",		"AX"	},
	{Anything,	"O",		"ST ",		"OW"	},
	{Anything,	"O",		Anything,	"AO"	},
	{Anything,	0,		Anything,	Silent	},
	};

/*
**	LEFT_PART	MATCH_PART	RIGHT_PART	OUT_PART
*/
static Rule P_rules[] =
	{
	{Nothing,	"PI",		Nothing,	"pAY"	},
	{Anything,	"PUT",		Nothing,	"pUHt"	},
	{Anything,	"PROVE",	Anything,	"prUWv"	},
	{Anything,	"PLY",		Anything,	"plAY"	},
	{"P",		"P",		Anything,	Silent	},
	{Anything,	"PHE",		Nothing,	"fIY"	},
	{Anything,	"PHE",		"S ",		"fIY"	},
	{Anything,	"PEOP",		Anything,	"pIYp"	},
	{Anything,	"POW",		Anything,	"pAW"	},
	{Anything,	"PH",		Anything,	"f"	},
	{Anything,	"P",		Anything,	"p"	},
	{Anything,	0,		Anything,	Silent	},
	};

/*
**	LEFT_PART	MATCH_PART	RIGHT_PART	OUT_PART
*/
static Rule Q_rules[] =
	{
	{Anything,	"QUAR",		Anything,	"K2wAOr"	},
	{Anything,	"QUE",		Nothing,	"K1"	},
	{Anything,	"QUE",		"S",		"K1"	},
	{Anything,	"QU",		Anything,	"K2w"	},
	{Anything,	"Q",		Anything,	"k"	},
	{Anything,	0,		Anything,	Silent	},
	};

/*
**	LEFT_PART	MATCH_PART	RIGHT_PART	OUT_PART
*/
static Rule R_rules[] =
	{
	{Nothing,	"RUGGED",	Anything,	"rAXgP1EHD1"	},
	{Nothing,	"RUSSIA",	Anything,	"rAXSHP1AX"	},
	{Nothing,	"REALITY",	Anything,	"rIYAElIHtIY"	},
	{Anything,	"RADIO",	Anything,	"rEYdIYOW"	},
	{Anything,	"RADIC",	Anything,	"rAEdIHk"	},
	{Nothing,	"RE",		"^#",		"rIY"	},
	{Nothing,	"RE",		"^^#",		"rIY"	},
	{Nothing,	"RE",		"^^+",		"rIY"	},

	{"^",		"R",		Anything,	"R1"	},
	{Anything,	"R",		Anything,	"r"	},
	{Anything,	0,		Anything,	Silent	},
	};

/*
**	LEFT_PART	MATCH_PART	RIGHT_PART	OUT_PART
*/
static Rule S_rules[] =
	{
	{Anything,	"SAID",		Anything,	"sEHD1"	},
	{Anything,	"SECRET",	Anything,	"sIYkrEHt"	},
	{Nothing,	"SLY",		Anything,	"slAY"	},
	{Nothing,	"SATUR",	Anything,	"sAEAEtR2"	},
	{Anything,	"SOME",		Anything,	"sAHm"	},
	{Anything,	"S",		"HON#^",	"s"	},
	{Anything,	"SH",		Anything,	"SH"	},
	{"#",		"SUR",		"#",		"ZHER"	},
	{Anything,	"SUR",		"#",		"SHER"	},
	{"#",		"SU",		"#",		"ZHUW"	},
	{"#",		"SSU",		"#",		"SHUW"	},
	{"#",		"SED",		Nothing,	"zD1"	},
	{"#",		"SION",		Anything,	"P1ZHAXn"	},
	{"^",		"SION",		Anything,	"P1SHAXn"	},
	{"S",		"SIAN",		Anything,	"ssIYAXn"	},
	{"#",		"SIAN",		Anything,	"P1ZHIYAXn"	},
	{Anything,	"SIAN",		Anything,	"P1ZHAXn"	},
	{Nothing,	"SCH",		Anything,	"sk"	},
	{"#",		"SM",		Anything,	"zm"	},
	{"#",		"SN",		"'",		"zAXn"	},
	{"#",		"S",		"#",		"z"	},
	{".",		"S",		Nothing,	"z"	},
	{"#:.E",	"S",		Nothing,	"z"	},
	{"#:^##",	"S",		Nothing,	"z"	},
	{"#:^#",	"S",		Nothing,	"s"	},
	{"U",		"S",		Nothing,	"s"	},
	{" :#",		"S",		Nothing,	"z"	},
	{Anything,	"S",		"S",		Silent	},
	{Anything,	"S",		"C+",		Silent	},
	{Anything,	"S",		Anything,	"s"	},
	{Anything,	0,		Anything,	Silent	},
	};

/*
**	LEFT_PART	MATCH_PART	RIGHT_PART	OUT_PART
*/
static Rule T_rules[] =
	{
	{Nothing,	"THE",		Nothing,	"DTIY"	},
	{Nothing,	"THIS",		Nothing,	"DHIHIHss"	},
	{Nothing,	"THAN",		Nothing,	"DHAEAEn"	},
	{Nothing,	"THEM",		Nothing,	"DHEHEHm"	},
	{Nothing,	"TILDE",	Nothing,	"tIHldAX"	},
	{Nothing,	"TUESDAY",	Nothing,	"tUWzP2dEY"	},

	{Nothing,	"TRY",		Anything,	"trAY"	},
	{Nothing,	"THY",		Anything,	"DHAY"	},
	{Nothing,	"THEY",		Anything,	"DHEHEY"	},
	{Nothing,	"THERE",	Anything,	"DHEHXR"	},
	{Nothing,	"THEN",		Anything,	"DHEHEHn"	},
	{Nothing,	"THUS",		Anything,	"DHAHAHs"	},
	{Anything,	"THAT",		Nothing,	"DHAEt"	},

	{Anything,	"TRULY",	Anything,	"trUWlIY"	},
	{Anything,	"TRUTH",	Anything,	"trUWTH"	},
	{Anything,	"THEIR",	Anything,	"DHEHIYXR"	},
	{Anything,	"THESE",	Nothing,	"DHIYz"	},
	{Anything,	"THROUGH",	Anything,	"THrUW"	},
	{Anything,	"THOSE",	Anything,	"DHOWz"	},
	{Anything,	"THOUGH",	Nothing,	"DHOW"	},

	{Anything,	"TION",		Anything,	"P1SHAXn"	},
	{Anything,	"TIAN",		Anything,	"P1SHAXn"	},
	{Anything,	"TIEN",		Anything,	"SHAXn"	},

	{Anything,	"TEAR",		Nothing,	"tEYER"	},
	{Anything,	"TEAR",		"%",		"tEYER"	},
	{Anything,	"TEAR",		"#",		"tEYER"	},

	{"#",		"T",		"IA",		"SH"	},	/* initiation */
	{".",		"T",		"IA",		"SH"	},	/* differential */

	{Anything,	"THER",		Anything,	"DHP2ER"	},
	{Anything,	"TO",		Nothing,	"tUW"	},
	{"#",		"TH",		Anything,	"TH"	},
	{Anything,	"TH",		Anything,	"TH"	},
	{"#:",		"TED",		Nothing,	"P1tIHD1"	},
	{Anything,	"TUR",		"#",		"P1CHER"	},
	{Anything,	"TUR",		"^",		"tER"	},
	{Anything,	"TU",		"A",		"CHUW"	},
	{Nothing,	"TWO",		Anything,	"tUW"	},

	{"T",		"T",		Anything,	Silent	},

	{Anything,	"T",		"S",		"T1"	},
	{Anything,	"T",		Anything,	"t"	},
	{Anything,	0,		Anything,	Silent	},
	};

/*
**	LEFT_PART	MATCH_PART	RIGHT_PART	OUT_PART
*/
static Rule U_rules[] =
	{
	{Nothing,	"UN",		Nothing,	"yUWP3AEn"},
	{Nothing,	"USA",		Nothing,	"yUWP3AEssP3EHEY"},
	{Nothing,	"USSR",		Nothing,	"yUWP3AEssP3AEssP3AAAR"},

	{Nothing,	"U",		Nothing,	"yU2"	},
	{Nothing,	"UN",		"I",		"yUWn"	},
	{Nothing,	"UN",		":",		"AHnP1"	},
	{Nothing,	"UN",		Anything,	"AHn"	},
	{Nothing,	"UPON",		Anything,	"AXpAOn"},
	{"D",		"UP",		Anything,	"UWp"},
/*	{Anything,	"USE",		".",		"U1z"	},	*/
	{"T",		"UR",		"#",		"UHr"	},
	{"S",		"UR",		"#",		"UHr"	},
	{"R",		"UR",		"#",		"UHr"	},
	{"D",		"UR",		"#",		"UHr"	},
	{"L",		"UR",		"#",		"UHr"	},
	{"Z",		"UR",		"#",		"UHr"	},
	{"N",		"UR",		"#",		"UHr"	},
	{"J",		"UR",		"#",		"UHr"	},
	{"TH",		"UR",		"#",		"UHr"	},
	{"CH",		"UR",		"#",		"UHr"	},
	{"SH",		"UR",		"#",		"UHr"	},
	{"ARG",		"U",		"#",		"yUW"	},
	{Anything,	"UR",		"#",		"yUHr"	},
	{Anything,	"UR",		Anything,	"ER"	},
	{Anything,	"UY",		Anything,	"AAAY"	},

	{Anything,	"U",		"^#^",		"yUW"	},
	{Anything,	"U",		"^ ",		"AH"	},
	{Anything,	"U",		"%",		"UW"	},
	{" G",		"U",		"#",		Silent	},
	{"G",		"U",		"%",		Silent	},
	{"G",		"U",		"#",		"w"	},
	{"#N",		"U",		Anything,	"yUW"	},
	{"#M",		"U",		Anything,	"yUW"	},
	{"F",		"U",		"^^",		"UH"	},
	{"B",		"U",		"^^",		"UH"	},
	{"^",		"U",		"^E",		"yUW"	},
	{"^",		"U",		"^",		"AX"	},
	{Anything,	"U",		"^^",		"AH"	},
	{"T",		"U",		Anything,	"UW"	},
	{"S",		"U",		Anything,	"UW"	},
	{"R",		"U",		Anything,	"UW"	},
	{"D",		"U",		Anything,	"UW"	},
	{"L",		"U",		Anything,	"UW"	},
	{"Z",		"U",		Anything,	"UW"	},
	{"N",		"U",		Anything,	"UW"	},
	{"J",		"U",		Anything,	"UW"	},
	{"TH",		"U",		Anything,	"UW"	},
	{"CH",		"U",		Anything,	"UW"	},
	{"SH",		"U",		Anything,	"UW"	},
	{Anything,	"U",		Anything,	"yUW"	},
	{Anything,	0,		Anything,	Silent	},
	};

/*
**	LEFT_PART	MATCH_PART	RIGHT_PART	OUT_PART
*/
static Rule V_rules[] =
	{
	{Anything,	"VIEW",		Anything,	"vyUW"	},
	{Nothing,	"VERY",		Nothing,	"vEHERP1IY"	},
	{Anything,	"VARY",		Anything,	"vEYP1rIY"	},
	{Anything,	"V",		Anything,	"v"	},
	{Anything,	0,		Anything,	Silent	},
	};

/*
**	LEFT_PART	MATCH_PART	RIGHT_PART	OUT_PART
*/
static Rule W_rules[] =
	{
	{Nothing,	"WERE",		Anything,	"wER"	},
	{Anything,	"WEIGH",	Anything,	"wEHEY"	},
	{Anything,	"WOOD",		Anything,	"wUHUHD1"	},
	{Anything,	"WARY",		Anything,	"wEHERP1IY"	},
	{Anything,	"WHERE",	Anything,	"wEHr"	},
	{Anything,	"WHAT",		Anything,	"wAAAAt"	},
	{Anything,	"WANT",		Anything,	"wAAAAnt"	},
	{Anything,	"WHOL",		Anything,	"hOWl"	},
	{Anything,	"WHO",		Anything,	"hUW"	},
	{Anything,	"WHY",		Anything,	"wAOAY"	},
	{Anything,	"WEAR",		Anything,	"wEYER"	},
	{Anything,	"WEA",		"TH",		"wEH"	},
	{Anything,	"WEA",		"L",		"wEH"	},
	{Anything,	"WEA",		"P",		"wEH"	},
	{Anything,	"WA",		"S",		"wAA"	},
	{Anything,	"WA",		"T",		"wAA"	},
	{Anything,	"WH",		Anything,	"WH"	},
	{Anything,	"WAR",		Nothing,	"wAOr"	},
	{"BE",		"WILDER",	Anything,	"wIHldER"	},
	{Nothing,	"WILDER",	"NESS",		"wIHldER"	},
	{Nothing,	"WILD",		"ERNESS",	"wIHld"	},
	{Nothing,	"WILY",		Nothing,	"wAYlIY"	},
	{Anything,	"WOR",		"^",		"wER"	},
	{Anything,	"WR",		Anything,	"r"	},

	{Anything,	"W",		Anything,	"w"	},
	{Anything,	0,		Anything,	Silent	},
	};

/*
**	LEFT_PART	MATCH_PART	RIGHT_PART	OUT_PART
*/
static Rule X_rules[] =
	{
	{Anything,	"X",		Anything,	"ks"	},
	{Anything,	0,		Anything,	Silent	},
	};

/*
**	LEFT_PART	MATCH_PART	RIGHT_PART	OUT_PART
*/
static Rule Y_rules[] =
	{
	{Anything,	"YOUNG",	Anything,	"yAHNG"	},
	{Nothing,	"YOUR",		Anything,	"yUHER"	},
	{Nothing,	"YOU",		Anything,	"yUW"	},
	{Nothing,	"YES",		Anything,	"yEHs"	},
	{Anything,	"YTE",		Anything,	"AYtP1"	},

	{Anything,	"Y",		Nothing,	"IY"	},
	{Anything,	"Y",		Anything,	"IH"	},
/*
	{Nothing,	"Y",		Anything,	"y"	},
	{"PH",		"Y",		Anything,	"IH"	}, 
	{":S",		"Y",		".",		"IH"	}, 
	{"#^",		"Y",		".",		"AY"	}, 
	{"H",		"Y",		"^",		"AY"	}, 
	{"#",		"Y",		"#",		"OY"	}, 
	{"^",		"Y",		"Z",		"AY"	},
	{"#:^",		"Y",		Nothing,	"IY"	},
	{"#:^",		"Y",		"I",		"IY"	},
	{" :",		"Y",		Nothing,	"AY"	},
	{" :",		"Y",		"#",		"AY"	},
	{" :",		"Y",		".",		"AY"	},
	{" :",		"Y",		"^+:#",		"IH"	},
	{" :",		"Y",		"^#",		"AY"	},
*/
	{Anything,	"Y",		Anything,	"IH"	},
	{Anything,	0,		Anything,	Silent	},
	};

/*
**	LEFT_PART	MATCH_PART	RIGHT_PART	OUT_PART
*/
static Rule Z_rules[] =
	{
	{"Z",		"Z",		Anything,	Silent	},
	{Anything,	"Z",		Anything,	"z"	},
	{Anything,	0,		Anything,	Silent	},
	};

Rule *Rules[] =
	{
	punct_rules,
	A_rules, B_rules, C_rules, D_rules, E_rules, F_rules, G_rules, 
	H_rules, I_rules, J_rules, K_rules, L_rules, M_rules, N_rules, 
	O_rules, P_rules, Q_rules, R_rules, S_rules, T_rules, U_rules, 
	V_rules, W_rules, X_rules, Y_rules, Z_rules
	};

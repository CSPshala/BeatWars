//enumerations and macros go here

#ifndef	GLOBALS_H
#define GLOBALS_H

//pause screen enumerations
enum Pause_Enum {PAUSE_RESUME, PAUSE_EXIT, NUM_PAUSE_OPTIONS};

//debug cheats
enum Cheats_Enum {CHEAT_INVINCIBILITY, CHEAT_ONEHITKILL, CHEAT_EXTRALIFE};

// the spells
//enum Spells_Enum {PROXRUNE, REMOTERUNE, DECOY, BARRIER,
//				  IMMOLATION, FIREBALL, FROSTSPRAY, ICEBLAST,
//				  HEALSELF, HEALALLY, MANAGIFT, HEALCIRCLE,
//				  FORTIFYSELF, FORTIFYALLY, SPRINT, EMPOWER,
//				  RAISEDEAD, LIFEDRAIN, PLAGUE, FEARSYMBOL,
//				  NUM_SPELLS};

// flags for which group to use an object with
//enum OM_Flags {OMF_PLAYER = 1, OMF_ENEMY = 2, OMF_SPELL = 4, OMF_ENVOBJECT = 8, OMF_ALL = 16};

// the enumerated group number for an object's type
//enum OM_Group {OMG_PLAYERS, OMG_ENEMIES, OMG_SPELLS, OMG_ENVOBJECTS, NUM_OM_GROUPS };

// the flags used for aligning text in a rect with the CBitmapFont class
enum Alignment_Flags {ALIGN_LEFT = 1, ALIGN_CENTER = 2, ALIGN_RIGHT = 4};

// the flags used for aligning text in a rect with the CBitmapFont classenum Alignment_Flags {ALIGN_LEFT = 1, ALIGN_CENTER = 2, ALIGN_RIGHT = 4};
// Determine owner of Spells
enum Spell_Owners {OWNER_PLAYER, OWNER_ENEMY};

// the enumerated group for messages
enum MSGS {TEST_MSG};

// the enumerated group for events
enum EVENTS {REDUCE_ENEMY_HEALTH, TEST_EVENT};

// the enumerated value for enemy targets
enum Targets{TARGET_PLAYER, TARGET_WAYPOINT,TARGET_SKELETON};

//	Macro to safely Release COM Objects.
#ifndef SAFE_RELEASE
	#define SAFE_RELEASE(p)			if (p) { p->Release(); p = NULL; }
#endif

//	Macro to safely delete pointers.
#ifndef SAFE_DELETE
	#define SAFE_DELETE(p)			if (p) { delete p; p = NULL; }
#endif

//	Macro to safely delete array pointers.
#ifndef SAFE_DELETE_ARRAY
	#define SAFE_DELETE_ARRAY(p)	if (p) { delete [] p; p = NULL; }
#endif

//	Macro for reporting Wrapper errors
#ifndef DXERROR
	#define DXERROR(a)	{ OutputDebugString(a); OutputDebugString("\n"); return false; }
#endif

#endif // end GLOBALS_H

/*
 * See Licensing and Copyright notice in naev.h
 */


#ifndef PILOT_H
#  define PILOT_H


#include "physics.h"
#include "ship.h"
#include "ai.h"
#include "outfit.h"
#include "faction.h"
#include "sound.h"
#include "economy.h"


#define PLAYER_ID       1 /**< Player pilot ID. */


/* Hyperspace parameters. */
#define HYPERSPACE_ENGINE_DELAY  3. /**< Time to warm up engine (seconds). */
#define HYPERSPACE_FLY_DELAY     5. /**< Time it takes to hyperspace (seconds). */
#define HYPERSPACE_STARS_BLUR    3. /**< How long the stars blur at max (pixels). */
#define HYPERSPACE_STARS_LENGTH  1000 /**< Length the stars blur to at max (pixels). */
#define HYPERSPACE_FADEOUT       1. /**< How long the fade is (seconds). */
#define HYPERSPACE_FUEL          100.  /**< how much fuel it takes */
#define HYPERSPACE_THRUST        2000./**< How much thrust you use in hyperspace. */
#define HYPERSPACE_VEL           HYPERSPACE_THRUST*HYPERSPACE_FLY_DELAY /**< Velocity at hyperspace. */
#define HYPERSPACE_ENTER_MIN     HYPERSPACE_VEL*0.5 /**< Minimum entering distance. */
#define HYPERSPACE_ENTER_MAX     HYPERSPACE_VEL*0.6 /**< Maxmimu entering distance. */
#define HYPERSPACE_EXIT_MIN      1500. /**< Minimum distance to begin jumping. */

#define PILOT_SIZE_APROX      0.8   /**< aproximation for pilot size */
#define PILOT_DISABLED_ARMOR  0.3   /**< armour % that gets it disabled */
#define PILOT_REFUEL_TIME     3. /**< Time to complete refueling. */
#define PILOT_REFUEL_RATE     HYPERSPACE_FUEL/PILOT_REFUEL_TIME /**< Fuel per second. */

/* hooks */
#define PILOT_HOOK_NONE    0 /**< No hook. */
#define PILOT_HOOK_DEATH   1 /**< Pilot died. */
#define PILOT_HOOK_BOARD   2 /**< Pilot got boarded. */
#define PILOT_HOOK_DISABLE 3 /**< Pilot got disabled. */
#define PILOT_HOOK_JUMP    4 /**< Pilot jumped. */
#define PILOT_HOOK_HAIL    5 /**< Pilot is hailed. */


/* damage */
#define PILOT_HOSTILE_THRESHOLD  0.09 /**< Point at which pilot becomes hostile. */
#define PILOT_HOSTILE_DECAY      0.005 /**< Rate at which hostility decays. */

/* creation */
#define PILOT_PLAYER       (1<<0) /**< Pilot is a player. */
#define PILOT_ESCORT       (1<<1) /**< Pilot is an escort. */
#define PILOT_CARRIED      (1<<2) /**< Pilot usually resides in a fighter bay. */
#define PILOT_CREATED_AI   (1<<3) /** Pilot has already created AI. */
#define PILOT_EMPTY        (1<<5) /**< Do not add pilot to stack. */
#define PILOT_NO_OUTFITS   (1<<6) /**< Do not create the pilot with outfits. */
#define PILOT_HASTURRET    (1<<8) /**< Pilot has turrets. */
#define PILOT_HASBEAMS     (1<<9) /**< Pilot has beam weapons. */
/* dynamic */
#define PILOT_INVINCIBLE   (1<<10) /**< Pilot can't be hit ever. */
#define PILOT_HOSTILE      (1<<11) /**< Pilot is hostile to the player. */
#define PILOT_FRIENDLY     (1<<12) /**< Pilot is friendly to the player. */
#define PILOT_COMBAT       (1<<13) /**< Pilot is engaged in combat. */
#define PILOT_AFTERBURNER  (1<<14) /**< Pilot has his afterburner activated. */
#define PILOT_HYP_PREP     (1<<15) /**< Pilot is getting ready for hyperspace. */
#define PILOT_HYP_BEGIN    (1<<16) /**< Pilot is starting engines. */
#define PILOT_HYPERSPACE   (1<<17) /**< Pilot is in hyperspace. */
#define PILOT_HYP_END      (1<<18) /**< Pilot is exiting hyperspace. */
#define PILOT_BOARDED      (1<<19) /**< Pilot has been boarded already. */
#define PILOT_NOBOARD      (1<<20) /**< Pilot can't be boarded. */
#define PILOT_BOARDING     (1<<21) /**< Pilot is currently boarding it's target. */
#define PILOT_BRIBED       (1<<22) /**< Pilot has been bribed already. */
#define PILOT_DISTRESSED   (1<<23) /**< Pilot has distressed once already. */
#define PILOT_REFUELING    (1<<24) /**< Pilot is trying to refueling. */
#define PILOT_REFUELBOARDING (1<<25) /**< Pilot is actively refueling. */
#define PILOT_DISABLED     (1<<26) /**< Pilot is disabled. */
#define PILOT_DEAD         (1<<27) /**< Pilot is in it's dying throes */
#define PILOT_DEATH_SOUND  (1<<28) /**< Pilot just did death explosion. */
#define PILOT_EXPLODED     (1<<29) /**< Pilot did final death explosion. */
#define PILOT_DELETE       (1<<30) /**< Pilot will get deleted asap. */

/* makes life easier */
#define pilot_isPlayer(p)   pilot_isFlag(p,PILOT_PLAYER) /**< Checks if pilot is a player. */
#define pilot_isDisabled(p) pilot_isFlag(p,PILOT_DISABLED) /**< Checks if pilot is disabled. */


/**
 * @brief Contains the state of the outfit.
 *
 * Currently only applicable to beam weapons.
 */
typedef enum PilotOutfitState_ {
   PILOT_OUTFIT_OFF, /**< Normal state. */
   PILOT_OUTFIT_WARMUP, /**< Outfit is starting to warm up. */
   PILOT_OUTFIT_ON /**< Outfit is activated and running. */
} PilotOutfitState;


/**
 * @brief Stores outfit ammo.
 */
typedef struct PilotOutfitAmmo_ {
   Outfit *outfit; /**< Type of ammo. */
   int quantity; /**< Amount of ammo. */
   int deployed; /**< For fighter bays. */
} PilotOutfitAmmo;


/**
 * @brief Stores an outfit the pilot has.
 */
typedef struct PilotOutfitSlot_ {
   /* Outfit slot properties. */
   Outfit* outfit; /**< Associated outfit. */
   ShipMount mount; /**< Outfit mountpoint. */
   OutfitSlotType slot; /**< Slot type. */

   /* Current state. */
   PilotOutfitState state; /**< State of the outfit. */
   double timer; /**< Used to store when it was last used. */
   int quantity; /**< Quantty. */

   /* Type-specific data. */
   union {
      int beamid; /**< ID of the beam used in this outfit, only used for beams. */
      PilotOutfitAmmo ammo; /**< Ammo for launchers. */
   } u;
} PilotOutfitSlot;


/**
 * @brief Stores a pilot commodity.
 */
typedef struct PilotCommodity_ {
   Commodity* commodity; /**< Assosciated commodity. */
   int quantity; /**< Amount player has. */
   unsigned int id; /**< Special mission id for cargo, 0 means none. */
} PilotCommodity;


/**
 * @brief A wrapper for pilot hooks.
 */
typedef struct PilotHook_ {
   int type; /**< Type of hook. */
   int id; /**< Hook ID assosciated with pilot hook. */
} PilotHook;


/**
 * @brief Different types of escorts.
 */
typedef enum EscortType_e {
   ESCORT_TYPE_NULL, /**< Invalid escort type. */
   ESCORT_TYPE_BAY, /**< Escort is from a fighter bay. */
   ESCORT_TYPE_MERCENARY, /**< Escort is a mercenary. */
   ESCORT_TYPE_ALLY /**< Escort is an ally. */
} EscortType_t;


/**
 * @brief Stores an escort.
 */
typedef struct Escort_s {
   char *ship; /**< Type of the ship escort is flying. */
   EscortType_t type; /**< Type of escort. */
   unsigned int id; /**< ID of in-game pilot. */
} Escort_t;


/**
 * @brief The representation of an in-game pilot.
 */
typedef struct Pilot_ {

} Pilot;


/*
 * getting pilot stuff
 */
extern Pilot* player; /* the player */
Pilot* pilot_get( const unsigned int id );
unsigned int pilot_getNextID( const unsigned int id, int mode );
unsigned int pilot_getPrevID( const unsigned int id, int mode );
unsigned int pilot_getNearestEnemy( const Pilot* p );
unsigned int pilot_getNearestHostile (void); /* only for the player */
unsigned int pilot_getNearestPilot( const Pilot* p );
int pilot_getJumps( const Pilot* p );


/*
 * Combat.
 */
void pilot_shoot( Pilot* p, int type );
void pilot_shootSecondary( Pilot* p );
void pilot_shootStop( Pilot* p, const int secondary );
double pilot_hit( Pilot* p, const Solid* w, const unsigned int shooter,
      const DamageType dtype, const double damage );
void pilot_explode( double x, double y, double radius,
      DamageType dtype, double damage, unsigned int parent );
double pilot_face( Pilot* p, const double dir );


/*
 * Outfits.
 */
/* Raw changes. */
int pilot_addOutfitRaw( Pilot* pilot, Outfit* outfit, PilotOutfitSlot *s );
int pilot_addOutfitTest( Pilot* pilot, Outfit* outfit, PilotOutfitSlot *s, int warn );
int pilot_rmOutfitRaw( Pilot* pilot, PilotOutfitSlot *s );
/* Changes with checks. */
int pilot_addOutfit( Pilot* pilot, Outfit* outfit, PilotOutfitSlot *s );
int pilot_rmOutfit( Pilot* pilot, PilotOutfitSlot *s );
/* Ammo. */
int pilot_addAmmo( Pilot* pilot, PilotOutfitSlot *s, Outfit* ammo, int quantity );
int pilot_rmAmmo( Pilot* pilot, PilotOutfitSlot *s, int quantity );
/* Checks. */
const char* pilot_checkSanity( Pilot *p );
const char* pilot_canEquip( Pilot *p, PilotOutfitSlot *s, Outfit *o, int add );
int pilot_oquantity( Pilot* p, PilotOutfitSlot* w );
/* Other. */
char* pilot_getOutfits( Pilot* pilot );
void pilot_calcStats( Pilot* pilot );
/* Special outfit stuff. */
int pilot_getMount( const Pilot *p, const PilotOutfitSlot *w, Vector2d *v );
void pilot_switchSecondary( Pilot* p, PilotOutfitSlot *w );


/*
 * Cargo.
 */
/* Normal. */
int pilot_cargoUsed( Pilot* pilot ); /* gets how much cargo it has onboard */
int pilot_cargoFree( Pilot* p ); /* cargo space */
int pilot_addCargo( Pilot* pilot, Commodity* cargo, int quantity );
int pilot_rmCargo( Pilot* pilot, Commodity* cargo, int quantity );
int pilot_moveCargo( Pilot* dest, Pilot* src );
/* mission cargo - not to be confused with normal cargo */
unsigned int pilot_addMissionCargo( Pilot* pilot, Commodity* cargo, int quantity );
int pilot_rmMissionCargo( Pilot* pilot, unsigned int cargo_id, int jettison );


/* Misc. */
int pilot_refuelStart( Pilot *p );
void pilot_hyperspaceAbort( Pilot* p );
void pilot_clearTimers( Pilot *pilot );
int pilot_hasDeployed( Pilot *p );
int pilot_dock( Pilot *p, Pilot *target, int deployed );
double pilot_hyperspaceDelay( Pilot *p );


/*
 * creation
 */
void pilot_init( Pilot* dest, Ship* ship, const char* name, int faction, const char *ai,
      const double dir, const Vector2d* pos, const Vector2d* vel,
      const unsigned int flags );
unsigned int pilot_create( Ship* ship, const char* name, int faction, const char *ai,
      const double dir, const Vector2d* pos, const Vector2d* vel,
      const unsigned int flags );
Pilot* pilot_createEmpty( Ship* ship, const char* name,
      int faction, const char *ai, const unsigned int flags );
Pilot* pilot_copy( Pilot* src );


/*
 * init/cleanup
 */
void pilot_destroy(Pilot* p);
void pilots_free (void);
void pilots_clean (void);
void pilots_cleanAll (void);
void pilot_free( Pilot* p );

/*
 * Getters
 */
AI_Profile  *pilot_ai(const Pilot *);
const char  *pilot_name(const Pilot *);
int          pilot_id(const Pilot *);
int          pilot_fuelmax(const Pilot *);
int          pilot_lockon_count(const Pilot *);
Task        *pilot_task(const Pilot *);
double       pilot_tcontrol(const Pilot *);
unsigned int pilot_target(const Pilot *);
unsigned int pilot_parent(const Pilot *);
unsigned int pilot_credits(const Pilot *);
int          pilot_faction(const Pilot *);
int          pilot_outfit_count(const Pilot *);
int          pilot_ncommodities(const Pilot *);
double       pilot_shield(const Pilot *);
double       pilot_shieldmax(const Pilot *);
double       pilot_armour(const Pilot *);
double       pilot_armourmax(const Pilot *);
double       pilot_thrust(const Pilot *);
double       pilot_speed(const Pilot *);
double       pilot_turnrate(const Pilot *);
double       pilot_weap_speed(const Pilot *);
double       pilot_weap_range(const Pilot *);
double       pilot_timer(const Pilot *, int);
Solid       *pilot_solid(const Pilot *);
Ship        *pilot_ship(const Pilot *);
PilotOutfitSlot *pilot_secondary(const Pilot *);
PilotOutfitSlot **pilot_outfits(const Pilot *);

/*
 * Setters
 */
void         pilot_set_ai(Pilot *, AI_Profile *);
void         pilot_set_fuel(Pilot *, int);
void         pilot_set_task(Pilot *, Task *);
void         pilot_set_target(Pilot *, int);
void         pilot_set_tcontrol(Pilot *, double);
void         pilot_set_secondary(Pilot *, PilotOutfitSlot *);
void         pilot_set_timer(Pilot *, int, double);
void         pilot_set_credits(Pilot *, int);
void         pilot_add_credits(Pilot *, int);
/*
 * Flags
 */
int pilot_isFlag(Pilot *, int);
void pilot_setFlag(Pilot *, int);
void pilot_rmFlag(Pilot *, int);

/*
 * Movement.
 */
void pilot_setThrust( Pilot *p, double thrust );
void pilot_setTurn( Pilot *p, double turn );

/*
 * update
 */
void pilot_update( Pilot* pilot, const double dt );
void pilots_update( double dt );
void pilots_render( double dt );
void pilot_render( Pilot* pilot, const double dt );


/*
 * communication
 */
void pilot_message( Pilot *p, unsigned int target, const char *msg, int ignore_int );
void pilot_broadcast( Pilot *p, const char *msg, int ignore_int );
void pilot_distress( Pilot *p, const char *msg, int ignore_int );


/*
 * Sensors.
 */
void pilot_updateSensorRange (void);
int pilot_inRange( const Pilot *p, double x, double y );
int pilot_inRangePilot( const Pilot *p, const Pilot *target );
int pilot_inRangePlanet( const Pilot *p, int target );

/*
 * faction
 */
void pilot_setHostile( Pilot *p );
void pilot_rmHostile( Pilot *p );
void pilot_setFriendly( Pilot *p );
void pilot_rmFriendly( Pilot *p );
int pilot_isHostile( const Pilot *p );
int pilot_isNeutral( const Pilot *p );
int pilot_isFriendly( const Pilot *p );


/*
 * hooks
 */
void pilot_addHook( Pilot *pilot, int type, unsigned int hook );
void pilot_runHook( Pilot* p, int hook_type );


#endif /* PILOT_H */

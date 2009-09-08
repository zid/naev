/**
* @brief The representation of an in-game pilot.
*/
typedef struct Pilot_ {
  
	unsigned int id; /**< pilot's id, used for many functions */
	char* name; /**< pilot's name (if unique) */
	char* title; /**< title - usually indicating special properties - @todo use */

	int faction; /**< Pilot's faction. */

	/* Object caracteristics */
	Ship* ship; /**< ship pilot is flying */
	Solid* solid; /**< associated solid (physics) */
	double mass_cargo; /**< Amount of cargo mass added. */
	double mass_outfit; /**< Amount of outfit mass added. */
	int tsx; /**< current sprite x position., calculated on update. */
	int tsy; /**< current sprite y position, calculated on update. */

	/* Properties. */
	double cpu; /**< Amount of CPU the pilot has left. */
	double cpu_max; /**< Maximum amount of CPU the pilot has. */

	/* Movement */
	double thrust; /**< Pilot's thrust. */
	double speed; /**< Pilot's speed. */
	double turn; /**< Pilot's turn. */
	double turn_base; /**< Pilot's base turn. */

	/* Current health */
	double armour; /**< Current armour. */
	double shield; /**< Current shield. */
	double fuel; /**< Current fuel. */
	double armour_max; /**< Maximum armour. */
	double shield_max; /**< Maximum shield. */
	double fuel_max; /**< Maximum fuel. */
	double armour_regen; /**< Armour regeneration rate (per second). */
	double shield_regen; /**< Shield regeneration rate (per second). */
     
	/* Energy is handled a bit differently. */
	double energy; /**< Current energy. */
	double energy_max; /**< Maximum energy. */
	double energy_regen; /**< Energy regeneration rate (per second). */
	double energy_tau; /**< Tau regeneration rate for energy. */
	
	/* Associated functions */
	void (*think)(struct Pilot_*, const double); /**< AI thinking for the pilot */
	void (*update)(struct Pilot_*, const double); /**< updates the pilot */
	void (*render)(struct Pilot_*, const double); /**< for rendering the pilot */
                                 
	/* Outfit management */
	/* Global outfits. */
	int noutfits; /**< Total amount of slots. */
	PilotOutfitSlot **outfits; /**< Total outfits. */
	
	/* Per slot types. */
	int outfit_nlow; /**< Number of low energy slots. */
	PilotOutfitSlot *outfit_low; /**< The low energy slots. */
	int outfit_nmedium; /**< Number of medium energy slots. */
	PilotOutfitSlot *outfit_medium; /**< The medium energy slots. */
	int outfit_nhigh; /**< Number of high energy slots. */
	PilotOutfitSlot *outfit_high; /**< The high energy slots. */

	/* For easier usage. */
	PilotOutfitSlot *secondary; /**< secondary weapon */
	PilotOutfitSlot *afterburner; /**< the afterburner */

	/* Jamming */
	double jam_range; /**< Range at which pilot starts jamming. */
	double jam_chance; /**< Jam chance. */

	/* Cargo */
	unsigned int credits; /**< monies the pilot has */
	PilotCommodity* commodities; /**< commodity and quantity */
	int ncommodities; /**< number of commodities. */
	int cargo_free; /**< Free commodity space. */

	/* Weapon properties */
	double weap_range; /**< Average range of primary weapons */
	double weap_speed; /**< Average speed of primary weapons */
                                 
	/* Misc */
	uint32_t flags; /**< used for AI and others */
	double ptimer; /**< generic timer for internal pilot use */
	int lockons; /**< Stores how many seeking weapons are targetting pilot */
	int *mounted; /**< Number of mounted outfits on the mount. */
	double player_damage; /**< Accumulates damage done by player for hostileness.
	                           In per one of max shield + armour. */
	double engine_glow; /**< Amount of engine glow to display. */

	/* Hook attached to the pilot */
	PilotHook *hooks; /**< Pilot hooks. */
	int nhooks; /**< Number of pilot hooks. */

	/* Escort stuff. */
	unsigned int parent; /**< Pilot's parent. */
	Escort_t *escorts; /**< Pilot's escorts. */
	int nescorts; /**< Number of pilot escorts. */

	/* AI */
	unsigned int target; /**< AI target. */
	AI_Profile* ai; /**< ai personality profile */
	double tcontrol; /**< timer for control tick */
	double timer[MAX_AI_TIMERS]; /**< timers for AI */
	Task* task; /**< current action */
} Pilot;
                  

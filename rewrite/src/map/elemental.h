// Copyright (c) Athena Dev Teams - Licensed under GNU GPL
// For more information, see LICENCE in the main folder

#ifndef _ELEMENTAL_H_
#define _ELEMENTAL_H_

#include "map.h" // struct status_data, struct view_data, struct elemental_skill
#include "status.h" // struct status_data, struct status_change
#include "unit.h" // struct unit_data

//Min time between AI executions
#define MIN_ELEMTHINKTIME 100
//Min time before mobs do a check to call nearby friends for help (or for slaves to support their master)
#define MIN_ELEMLINKTIME 1000

//Distance that slaves should keep from their master.
#define ELEM_SLAVEDISTANCE 2

//Used to determine default enemy type of mobs (for use in eachinrange calls)
#define DEFAULT_ELEM_ENEMY_TYPE(ed) (BL_PC|BL_MOB|BL_HOM|BL_MER|BL_ELEM)

enum {
	ELEMTYPE_AGNI = 1,
	ELEMTYPE_AQUA,
	ELEMTYPE_VENTUS,
	ELEMTYPE_TERA,
};

enum {
	ELEMMODE_WAIT,
	ELEMMODE_PASSIVE,
	ELEMMODE_DEFENSIVE,
	ELEMMODE_OFFENSIVE,
};

struct s_elemental_db {
	int class_;
	char sprite[NAME_LENGTH], name[NAME_LENGTH];
	unsigned short lv;
	short range2, range3;
	struct status_data status;
	struct view_data vd;
	struct {
		unsigned short id, lv, mode;
	} skill[MAX_ELEMSKILL];
};

extern struct s_elemental_db elemental_db[MAX_ELEMENTAL_CLASS];

struct elemental_data {
	struct block_list bl;
	struct unit_data ud;
	struct view_data *vd;
	struct status_data base_status, battle_status;
	struct status_change sc;
	struct regen_data regen;

	struct s_elemental_db *db;
	struct s_elemental elemental;
	char blockskill[MAX_SKILL];

	struct map_session_data *master;
	int summon_timer;
	
	unsigned water_screen_flag : 1;

	// AI Stuff
	struct {
		enum MobSkillState skillstate;
		unsigned aggressive : 1; //Signals whether the mob AI is in aggressive mode or reactive mode. [Skotlex]
		//unsigned char steal_flag; //number of steal tries (to prevent steal exploit on mobs with few items) [Lupus]
		//unsigned steal_coin_flag : 1;
		//unsigned soul_change_flag : 1; // Celest
		//unsigned alchemist: 1;
		unsigned spotted: 1;
		unsigned char attacked_count; //For rude attacked.
		int provoke_flag; // Celest
		//unsigned npc_killmonster: 1; //for new killmonster behavior
		//unsigned rebirth: 1; // NPC_Rebirth used
		//unsigned int bg_id; // BattleGround System
	} state;
	struct {
		int id;
		unsigned int dmg;
		unsigned flag : 2; //0: Normal. 1: Homunc exp. 2: Pet exp
	} dmglog[DAMAGELOG_SIZE];

	unsigned int tdmg;
	int level;
	int target_id,attacked_id;
	unsigned int next_walktime,last_thinktime,last_linktime,last_pcneartime;
	short min_chase;
	int master_dist;
};

bool elem_class(int class_);
struct view_data * elem_get_viewdata(int class_);

int elem_create(struct map_session_data *sd, int class_, unsigned int lifetime);
int elem_data_received(struct s_elemental *elem, bool flag);
int elemental_save(struct elemental_data *ed);

void elemental_damage(struct elemental_data *ed, struct block_list *src, int hp, int sp);
void elemental_heal(struct elemental_data *ed, int hp, int sp);
int elemental_dead(struct elemental_data *ed, struct block_list *src);

int elem_delete(struct elemental_data *ed, int reply);
void elem_summon_stop(struct elemental_data *ed);

int elemental_get_lifetime(struct elemental_data *ed);
int elemental_get_type(struct elemental_data *ed);

int elemental_checkskill(struct elemental_data *ed, int skill_id);

// AI Stuff
int elem_target(struct elemental_data *ed,struct block_list *bl,int dist);
int elem_unlocktarget(struct elemental_data *ed, unsigned int tick);
int elem_can_reach(struct elemental_data *ed,struct block_list *bl,int range, int state);
void elem_log_damage(struct elemental_data *ed, struct block_list *src, int damage);

#define elem_stop_walking(ed, type) unit_stop_walking(&(ed)->bl, type)
#define elem_stop_attack(ed) unit_stop_attack(&(ed)->bl)

int do_init_elemental(void);

#endif /* _ELEMENTAL_H_ */

#ifndef GUARD_CONSTANTS_POKEMON_H
#define GUARD_CONSTANTS_POKEMON_H

// Pokemon types
#define TYPE_NONE             255
#define TYPE_NORMAL           0
#define TYPE_FIGHTING         1
#define TYPE_FLYING           2
#define TYPE_POISON           3
#define TYPE_GROUND           4
#define TYPE_ROCK             5
#define TYPE_BUG              6
#define TYPE_GHOST            7
#define TYPE_STEEL            8
#define TYPE_MYSTERY          9
#define TYPE_FIRE             10
#define TYPE_WATER            11
#define TYPE_GRASS            12
#define TYPE_ELECTRIC         13
#define TYPE_PSYCHIC          14
#define TYPE_ICE              15
#define TYPE_DRAGON           16
#define TYPE_DARK             17
#define TYPE_FAIRY            18
#define NUMBER_OF_MON_TYPES   19

// Pokemon egg groups
#define EGG_GROUP_NONE          0
#define EGG_GROUP_MONSTER       1
#define EGG_GROUP_WATER_1       2
#define EGG_GROUP_BUG           3
#define EGG_GROUP_FLYING        4
#define EGG_GROUP_FIELD         5
#define EGG_GROUP_FAIRY         6
#define EGG_GROUP_GRASS         7
#define EGG_GROUP_HUMAN_LIKE    8
#define EGG_GROUP_WATER_3       9
#define EGG_GROUP_MINERAL       10
#define EGG_GROUP_AMORPHOUS     11
#define EGG_GROUP_WATER_2       12
#define EGG_GROUP_DITTO         13
#define EGG_GROUP_DRAGON        14
#define EGG_GROUP_UNDISCOVERED  15

#define EGG_GROUPS_PER_MON      2

// Pokemon natures
#define NATURE_HARDY    0
#define NATURE_LONELY   1
#define NATURE_BRAVE    2
#define NATURE_ADAMANT  3
#define NATURE_NAUGHTY  4
#define NATURE_BOLD     5
#define NATURE_DOCILE   6
#define NATURE_RELAXED  7
#define NATURE_IMPISH   8
#define NATURE_LAX      9
#define NATURE_TIMID    10
#define NATURE_HASTY    11
#define NATURE_SERIOUS  12
#define NATURE_JOLLY    13
#define NATURE_NAIVE    14
#define NATURE_MODEST   15
#define NATURE_MILD     16
#define NATURE_QUIET    17
#define NATURE_BASHFUL  18
#define NATURE_RASH     19
#define NATURE_CALM     20
#define NATURE_GENTLE   21
#define NATURE_SASSY    22
#define NATURE_CAREFUL  23
#define NATURE_QUIRKY   24
#define NUM_NATURES     25

// Pokemon Stats
#define STAT_HP      0
#define STAT_ATK     1
#define STAT_DEF     2
#define STAT_SPEED   3
#define STAT_SPATK   4
#define STAT_SPDEF   5
#define NUM_STATS    6

#define STAT_ACC     6 // Only in battles.
#define STAT_EVASION 7 // Only in battles.

#define NUM_NATURE_STATS (NUM_STATS - 1) // excludes HP
#define NUM_BATTLE_STATS (NUM_STATS + 2) // includes Accuracy and Evasion

#define MIN_STAT_STAGE     0
#define DEFAULT_STAT_STAGE 6
#define MAX_STAT_STAGE    12

#define BEST_IV_SPREAD_HIDDEN_POWER_BUG      {31, 31, 31, 30, 31, 30}
#define BEST_IV_SPREAD_HIDDEN_POWER_DARK     {31, 31, 31, 31, 31, 31}
#define BEST_IV_SPREAD_HIDDEN_POWER_DRAGON   {30, 31, 31, 31, 31, 31}
#define BEST_IV_SPREAD_HIDDEN_POWER_ELECTRIC {31, 31, 31, 31, 30, 31}
#define BEST_IV_SPREAD_HIDDEN_POWER_FIGHTING {31, 31, 30, 30, 30, 30}
#define BEST_IV_SPREAD_HIDDEN_POWER_FIRE     {31, 30, 31, 30, 30, 31}
#define BEST_IV_SPREAD_HIDDEN_POWER_FLYING   {31, 31, 31, 30, 30, 30}
#define BEST_IV_SPREAD_HIDDEN_POWER_GHOST    {31, 30, 31, 31, 31, 30}
#define BEST_IV_SPREAD_HIDDEN_POWER_GRASS    {30, 31, 31, 31, 30, 31}
#define BEST_IV_SPREAD_HIDDEN_POWER_GROUND   {31, 31, 31, 31, 30, 30}
#define BEST_IV_SPREAD_HIDDEN_POWER_ICE      {31, 31, 31, 30, 31, 31}
#define BEST_IV_SPREAD_HIDDEN_POWER_POISON   {31, 31, 30, 31, 30, 30}
#define BEST_IV_SPREAD_HIDDEN_POWER_PSYCHIC  {30, 31, 31, 30, 31, 31}
#define BEST_IV_SPREAD_HIDDEN_POWER_ROCK     {31, 31, 30, 30, 31, 30}
#define BEST_IV_SPREAD_HIDDEN_POWER_STEEL    {31, 31, 31, 31, 31, 30}
#define BEST_IV_SPREAD_HIDDEN_POWER_WATER    {31, 31, 31, 30, 30, 31}

#define BEST_IV_SPREAD_SPECIAL               {31,  0, 31, 31, 31, 31}
#define BEST_IV_SPREAD_TRICK_ROOM            {31, 31, 31,  0, 31, 31}
#define BEST_IV_SPREAD_SPECIAL_TRICK_ROOM    {31,  0, 31,  0, 31, 31}

#define BEST_IV_SPREAD BEST_IV_SPREAD_HIDDEN_POWER_DARK

#define MAX_EV_SPREAD                     {252, 252, 252, 252, 252, 252}
#define MAX_EV_SPREAD_SPECIAL             {252,   0, 252, 252, 252, 252}
#define MAX_EV_SPREAD_TRICK_ROOM          {252, 252, 252,   0, 252, 252}
#define MAX_EV_SPREAD_SPECIAL_TRICK_ROOM  {252,   0, 252,   0, 252, 252}

#define WORST_IVS                1

#define ABILITY_SLOT_1 3
#define ABILITY_SLOT_2 1
#define ABILITY_HIDDEN 2

// Shiny odds
#define SHINY_ODDS 8 // Actual probability is SHINY_ODDS/65536

// Ribbon IDs used by TV and Pokénav
#define CHAMPION_RIBBON       0
#define COOL_RIBBON_NORMAL    1
#define COOL_RIBBON_SUPER     2
#define COOL_RIBBON_HYPER     3
#define COOL_RIBBON_MASTER    4
#define BEAUTY_RIBBON_NORMAL  5
#define BEAUTY_RIBBON_SUPER   6
#define BEAUTY_RIBBON_HYPER   7
#define BEAUTY_RIBBON_MASTER  8
#define CUTE_RIBBON_NORMAL    9
#define CUTE_RIBBON_SUPER    10
#define CUTE_RIBBON_HYPER    11
#define CUTE_RIBBON_MASTER   12
#define SMART_RIBBON_NORMAL  13
#define SMART_RIBBON_SUPER   14
#define SMART_RIBBON_HYPER   15
#define SMART_RIBBON_MASTER  16
#define TOUGH_RIBBON_NORMAL  17
#define TOUGH_RIBBON_SUPER   18
#define TOUGH_RIBBON_HYPER   19
#define TOUGH_RIBBON_MASTER  20
#define WINNING_RIBBON       21
#define VICTORY_RIBBON       22
#define ARTIST_RIBBON        23
#define EFFORT_RIBBON        24
#define MARINE_RIBBON        25
#define LAND_RIBBON          26
#define SKY_RIBBON           27
#define COUNTRY_RIBBON       28
#define NATIONAL_RIBBON      29
#define EARTH_RIBBON         30
#define WORLD_RIBBON         31

#define FIRST_GIFT_RIBBON MARINE_RIBBON
#define LAST_GIFT_RIBBON  WORLD_RIBBON
#define NUM_GIFT_RIBBONS  (1 + LAST_GIFT_RIBBON - FIRST_GIFT_RIBBON)

// The above gift ribbons (Marine - World) are
// special distribution ribbons that correspond to
// 1 bit each in the Pokémon struct. Gen 4 hard-codes
// each of these to the given name. In Gen 3 they're
// used to get an index into giftRibbons in the save block,
// which can have a value 0-64 (0 is 'no ribbon') that
// corresponds to one of the special ribbons listed
// in gGiftRibbonDescriptionPointers. Most of these were
// never distributed
#define MAX_GIFT_RIBBON 64

#define MIN_LEVEL 1
#define MAX_LEVEL 100

#define OT_ID_PLAYER_ID       0
#define OT_ID_PRESET          1
#define OT_ID_RANDOM_NO_SHINY 2
#define OT_ID_SHINY           3

#define MON_GIVEN_TO_PARTY      0
#define MON_GIVEN_TO_PC         1
#define MON_CANT_GIVE           2

#define PLAYER_HAS_TWO_USABLE_MONS     0
#define PLAYER_HAS_ONE_MON             1
#define PLAYER_HAS_ONE_USABLE_MON      2

#define MON_ALREADY_KNOWS_MOVE  0xFFFE
#define MON_HAS_MAX_MOVES       0xFFFF

#define LEVEL_UP_MOVE_ID   0x01FF
#define LEVEL_UP_MOVE_LV   0xFE00
#define LEVEL_UP_END       0xFFFF

#define MAX_LEVEL_UP_MOVES       20

#define MON_MALE       0x00
#define MON_FEMALE     0xFE
#define MON_GENDERLESS 0xFF

// Constants for AdjustFriendship
#define FRIENDSHIP_EVENT_GROW_LEVEL       0
#define FRIENDSHIP_EVENT_VITAMIN          1 // unused, handled by PokemonUseItemEffects
#define FRIENDSHIP_EVENT_BATTLE_ITEM      2 // unused, handled by PokemonUseItemEffects
#define FRIENDSHIP_EVENT_LEAGUE_BATTLE    3
#define FRIENDSHIP_EVENT_LEARN_TMHM       4
#define FRIENDSHIP_EVENT_WALKING          5
#define FRIENDSHIP_EVENT_FAINT_SMALL      6
#define FRIENDSHIP_EVENT_FAINT_FIELD_PSN  7
#define FRIENDSHIP_EVENT_FAINT_LARGE      8 // If opponent was >= 30 levels higher. See AdjustFriendshipOnBattleFaint

// Constants for GetLeadMonFriendshipScore
#define FRIENDSHIP_NONE        0
#define FRIENDSHIP_1_TO_49     1
#define FRIENDSHIP_50_TO_99    2
#define FRIENDSHIP_100_TO_149  3
#define FRIENDSHIP_150_TO_199  4
#define FRIENDSHIP_200_TO_254  5
#define FRIENDSHIP_MAX         6

// Friendship value that the majority of species use. This was changed in Generation 8 to 50.
#define STANDARD_FRIENDSHIP 70

#define MAX_FRIENDSHIP  255
#define MAX_SHEEN       255
#define MAX_CONDITION   255

#define MAX_PER_STAT_IVS 31
#define MAX_IV_MASK 31
#define USE_RANDOM_IVS (MAX_PER_STAT_IVS + 1)
#if P_EV_CAP >= GEN_6
#define MAX_PER_STAT_EVS 252
#else
#define MAX_PER_STAT_EVS 255
#endif
#define MAX_TOTAL_EVS 510
#if I_VITAMIN_EV_CAP >= GEN_8
#define EV_ITEM_RAISE_LIMIT MAX_PER_STAT_EVS
#else
#define EV_ITEM_RAISE_LIMIT 100
#endif

// Split defines.
#define SPLIT_PHYSICAL  0x0
#define SPLIT_SPECIAL   0x1
#define SPLIT_STATUS    0x2

// Growth rates
#define GROWTH_MEDIUM_FAST  0
#define GROWTH_ERRATIC      1
#define GROWTH_FLUCTUATING  2
#define GROWTH_MEDIUM_SLOW  3
#define GROWTH_FAST         4
#define GROWTH_SLOW         5

// Body colors for pokedex search
#define BODY_COLOR_RED      0
#define BODY_COLOR_BLUE     1
#define BODY_COLOR_YELLOW   2
#define BODY_COLOR_GREEN    3
#define BODY_COLOR_BLACK    4
#define BODY_COLOR_BROWN    5
#define BODY_COLOR_PURPLE   6
#define BODY_COLOR_GRAY     7
#define BODY_COLOR_WHITE    8
#define BODY_COLOR_PINK     9

#define F_SUMMARY_SCREEN_FLIP_SPRITE 0x80

// Evolution types
#define EVO_NONE                          0xffff // Not an actual evolution, used to generate offspring that can't evolve into the specified species, like regional forms.
#define EVO_FRIENDSHIP                    1      // Pokémon levels up with friendship ≥ 220
#define EVO_FRIENDSHIP_DAY                2      // Pokémon levels up during the day with friendship ≥ 220
#define EVO_FRIENDSHIP_NIGHT              3      // Pokémon levels up at night with friendship ≥ 220
#define EVO_LEVEL                         4      // Pokémon reaches the specified level
#define EVO_TRADE                         5      // Pokémon is traded
#define EVO_TRADE_ITEM                    6      // Pokémon is traded while it's holding the specified item
#define EVO_ITEM                          7      // specified item is used on Pokémon
#define EVO_LEVEL_ATK_GT_DEF              8      // Pokémon reaches the specified level with attack > defense
#define EVO_LEVEL_ATK_EQ_DEF              9      // Pokémon reaches the specified level with attack = defense
#define EVO_LEVEL_ATK_LT_DEF              10     // Pokémon reaches the specified level with attack < defense
#define EVO_LEVEL_SILCOON                 11     // Pokémon reaches the specified level with a Silcoon personality value
#define EVO_LEVEL_CASCOON                 12     // Pokémon reaches the specified level with a Cascoon personality value
#define EVO_LEVEL_NINJASK                 13     // Pokémon reaches the specified level (special value for Ninjask)
#define EVO_LEVEL_SHEDINJA                14     // Pokémon reaches the specified level (special value for Shedinja)
#define EVO_BEAUTY                        15     // Pokémon levels up with beauty ≥ specified value
#define EVO_LEVEL_FEMALE                  16     // Pokémon reaches the specified level, is female
#define EVO_LEVEL_MALE                    17     // Pokémon reaches the specified level, is male
#define EVO_LEVEL_NIGHT                   18     // Pokémon reaches the specified level, is night
#define EVO_LEVEL_DAY                     19     // Pokémon reaches the specified level, is day
#define EVO_LEVEL_DUSK                    20     // Pokémon reaches the specified level, is dusk (5-6 P.M)
#define EVO_ITEM_HOLD_DAY                 21     // Pokémon levels up, holds specified item at day
#define EVO_ITEM_HOLD_NIGHT               22     // Pokémon levels up, holds specified item at night
#define EVO_MOVE                          23     // Pokémon levels up, knows specified move
#define EVO_FRIENDSHIP_MOVE_TYPE          24     // Pokémon levels up with friendship ≥ 220, knows move with specified type
#define EVO_MAPSEC                        25     // Pokémon levels up on specified mapsec
#define EVO_ITEM_MALE                     26     // specified item is used on a male Pokémon
#define EVO_ITEM_FEMALE                   27     // specified item is used on a female Pokémon
#define EVO_LEVEL_RAIN                    28     // Pokémon reaches the specified level during rain in the overworld
#define EVO_SPECIFIC_MON_IN_PARTY         29     // Pokémon levels up with a specified Pokémon in party
#define EVO_LEVEL_DARK_TYPE_MON_IN_PARTY  30     // Pokémon reaches the specified level with a Dark Type Pokémon in party
#define EVO_TRADE_SPECIFIC_MON            31     // Pokémon is traded for a specified Pokémon
#define EVO_SPECIFIC_MAP                  32     // Pokémon levels up on specified map
#define EVO_LEVEL_NATURE_AMPED            33     // Pokémon reaches the specified level, it has a Hardy, Brave, Adamant, Naughty, Docile, Impish, Lax, Hasty, Jolly, Naive, Rash, Sassy, or Quirky nature.
#define EVO_LEVEL_NATURE_LOW_KEY          34     // Pokémon reaches the specified level, it has a Lonely, Bold, Relaxed, Timid, Serious, Modest, Mild, Quiet, Bashful, Calm, Gentle, or Careful nature.
#define EVO_CRITICAL_HITS                 35     // Pokémon performs specified number of critical hits in one battle
#define EVO_SCRIPT_TRIGGER_DMG            36     // Pokémon has specified HP below max, then player interacts trigger
#define EVO_DARK_SCROLL                   37     // interacts with Scroll of Darkness
#define EVO_WATER_SCROLL                  38     // interacts with Scroll of Waters
#define EVO_ITEM_NIGHT                    39     // specified item is used on Pokémon, is night
#define EVO_ITEM_DAY                      40     // specified item is used on Pokémon, is day
#define EVO_ITEM_HOLD                     41     // Pokémon levels up, holds specified item
#define EVO_LEVEL_FOG                     42     // Pokémon reaches the specified level during fog in the overworld

#define EVOS_PER_MON 10

// Evolution 'modes,' for GetEvolutionTargetSpecies
#define EVO_MODE_NORMAL            0
#define EVO_MODE_TRADE             1
#define EVO_MODE_ITEM_USE          2
#define EVO_MODE_ITEM_CHECK        3 // If an Everstone is being held, still want to show that the stone *could* be used on that Pokémon to evolve
#define EVO_MODE_BATTLE_SPECIAL    4
#define EVO_MODE_OVERWORLD_SPECIAL 5

#define MON_PIC_WIDTH 64
#define MON_PIC_HEIGHT 64
#define MON_PIC_SIZE (MON_PIC_WIDTH * MON_PIC_HEIGHT / 2)

// Most pokemon have 2 frames (a default and an alternate for their animation).
// There are 4 exceptions:
// - Deoxys has 2 frames, 1 for each form
// - Spinda has 1 frame, presumably to avoid the work of animating its spots
// - Unown has 1 frame, presumably to avoid the work of animating all 28 of its forms
#define MAX_MON_PIC_FRAMES 2

#define BATTLE_ALIVE_EXCEPT_BATTLER  0
#define BATTLE_ALIVE_SIDE            1

#define SKIP_FRONT_ANIM (1 << 7)

#define NUM_ABILITY_SLOTS (NUM_NORMAL_ABILITY_SLOTS + NUM_HIDDEN_ABILITY_SLOTS)
#define NUM_NORMAL_ABILITY_SLOTS 2
#define NUM_HIDDEN_ABILITY_SLOTS 1

// Species Flags
#define SPECIES_FLAG_LEGENDARY          (1 << 0)
#define SPECIES_FLAG_MYTHICAL           (1 << 1)
#define SPECIES_FLAG_MEGA_EVOLUTION     (1 << 2)
#define SPECIES_FLAG_PRIMAL_REVERSION   (1 << 3)
#define SPECIES_FLAG_ULTRA_BEAST        (1 << 4)
#define SPECIES_FLAG_ALOLAN_FORM        (1 << 5)
#define SPECIES_FLAG_GALARIAN_FORM      (1 << 6)
#define SPECIES_FLAG_HISUIAN_FORM       (1 << 7)
#define SPECIES_FLAG_ALL_PERFECT_IVS    (1 << 8)
#define SPECIES_FLAG_CANNOT_BE_TRADED   (1 << 9)
#define SPECIES_FLAG_ULTRA_BURST        (1 << 10)

#define LEGENDARY_PERFECT_IV_COUNT 3

#endif // GUARD_CONSTANTS_POKEMON_H

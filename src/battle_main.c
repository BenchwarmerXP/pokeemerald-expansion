#include "global.h"
#include "battle.h"
#include "battle_anim.h"
#include "battle_ai_script_commands.h"
#include "battle_arena.h"
#include "battle_controllers.h"
#include "battle_interface.h"
#include "battle_main.h"
#include "battle_message.h"
#include "battle_pyramid.h"
#include "battle_scripts.h"
#include "battle_setup.h"
#include "battle_tower.h"
#include "battle_util.h"
#include "berry.h"
#include "bg.h"
#include "data.h"
#include "debug.h"
#include "decompress.h"
#include "dma3.h"
#include "event_data.h"
#include "evolution_scene.h"
#include "graphics.h"
#include "gpu_regs.h"
#include "international_string_util.h"
#include "item.h"
#include "link.h"
#include "link_rfu.h"
#include "load_save.h"
#include "main.h"
#include "malloc.h"
#include "m4a.h"
#include "palette.h"
#include "party_menu.h"
#include "pokeball.h"
#include "pokedex.h"
#include "pokemon.h"
#include "random.h"
#include "recorded_battle.h"
#include "roamer.h"
#include "safari_zone.h"
#include "scanline_effect.h"
#include "sound.h"
#include "sprite.h"
#include "string_util.h"
#include "strings.h"
#include "task.h"
#include "text.h"
#include "trig.h"
#include "tv.h"
#include "util.h"
#include "window.h"
#include "constants/abilities.h"
#include "constants/battle_move_effects.h"
#include "constants/battle_string_ids.h"
#include "constants/hold_effects.h"
#include "constants/items.h"
#include "constants/moves.h"
#include "constants/party_menu.h"
#include "constants/rgb.h"
#include "constants/songs.h"
#include "constants/trainers.h"
#include "cable_club.h"

extern struct MusicPlayerInfo gMPlayInfo_SE1;
extern struct MusicPlayerInfo gMPlayInfo_SE2;

extern const struct BgTemplate gBattleBgTemplates[];
extern const struct WindowTemplate *const gBattleWindowTemplates[];

// this file's functions
static void CB2_InitBattleInternal(void);
static void CB2_PreInitMultiBattle(void);
static void CB2_PreInitIngamePlayerPartnerBattle(void);
static void CB2_HandleStartMultiPartnerBattle(void);
static void CB2_HandleStartMultiBattle(void);
static void CB2_HandleStartBattle(void);
static void TryCorrectShedinjaLanguage(struct Pokemon *mon);
static u8 CreateNPCTrainerParty(struct Pokemon *party, u16 trainerNum, bool8 firstTrainer);
static void BattleMainCB1(void);
static void sub_8038538(struct Sprite *sprite);
static void CB2_EndLinkBattle(void);
static void EndLinkBattleInSteps(void);
static void CB2_InitAskRecordBattle(void);
static void CB2_AskRecordBattle(void);
static void AskRecordBattle(void);
static void SpriteCB_MoveWildMonToRight(struct Sprite *sprite);
static void SpriteCB_WildMonShowHealthbox(struct Sprite *sprite);
static void SpriteCB_WildMonAnimate(struct Sprite *sprite);
static void sub_80392A8(void);
static void sub_803937C(void);
static void sub_803939C(void);
static void SpriteCb_MoveWildMonToRight(struct Sprite *sprite);
static void SpriteCb_WildMonShowHealthbox(struct Sprite *sprite);
static void SpriteCb_WildMonAnimate(struct Sprite *sprite);
static void sub_80398D0(struct Sprite *sprite);
static void SpriteCB_AnimFaintOpponent(struct Sprite *sprite);
static void SpriteCb_BlinkVisible(struct Sprite *sprite);
static void SpriteCallbackDummy_3(struct Sprite *sprite);
static void SpriteCB_BattleSpriteSlideLeft(struct Sprite *sprite);
static void TurnValuesCleanUp(bool8 var0);
static void SpriteCB_BounceEffect(struct Sprite *sprite);
static void BattleStartClearSetData(void);
static void BattleIntroGetMonsData(void);
static void BattleIntroPrepareBackgroundSlide(void);
static void BattleIntroDrawTrainersOrMonsSprites(void);
static void BattleIntroDrawPartySummaryScreens(void);
static void BattleIntroPrintTrainerWantsToBattle(void);
static void BattleIntroPrintWildMonAttacked(void);
static void BattleIntroPrintOpponentSendsOut(void);
static void BattleIntroPrintPlayerSendsOut(void);
static void BattleIntroOpponent1SendsOutMonAnimation(void);
static void BattleIntroOpponent2SendsOutMonAnimation(void);
static void BattleIntroRecordMonsToDex(void);
static void BattleIntroPlayer1SendsOutMonAnimation(void);
static void TryDoEventsBeforeFirstTurn(void);
static void HandleTurnActionSelectionState(void);
static void RunTurnActionsFunctions(void);
static void SetActionsAndBattlersTurnOrder(void);
static void UpdateBattlerPartyOrdersOnSwitch(u32 battler);
static void sub_803CDF8(void);
static bool8 AllAtActionConfirmed(void);
static void TryChangeTurnOrder(void);
static void CheckQuickClaw_CustapBerryActivation(void);
static void CheckFocusPunch_ClearVarsBeforeTurnStarts(void);
static void FreeResetData_ReturnToOvOrDoEvolutions(void);
static void ReturnFromBattleToOverworld(void);
static void TryEvolvePokemon(void);
static void WaitForEvoSceneToFinish(void);
static void HandleEndTurn_ContinueBattle(void);
static void HandleEndTurn_BattleWon(void);
static void HandleEndTurn_BattleLost(void);
static void HandleEndTurn_RanFromBattle(void);
static void HandleEndTurn_MonFled(void);
static void HandleEndTurn_FinishBattle(void);
static void SpriteCB_UnusedBattleInit(struct Sprite *sprite);
static void SpriteCB_UnusedBattleInit_Main(struct Sprite *sprite);
static void TrySpecialEvolution(void);
static u32 Crc32B (const u8 *data, u32 size);
static u32 GeneratePartyHash(const struct Trainer *trainer, u32 i);


// EWRAM vars
EWRAM_DATA u16 gBattle_BG0_X = 0;
EWRAM_DATA u16 gBattle_BG0_Y = 0;
EWRAM_DATA u16 gBattle_BG1_X = 0;
EWRAM_DATA u16 gBattle_BG1_Y = 0;
EWRAM_DATA u16 gBattle_BG2_X = 0;
EWRAM_DATA u16 gBattle_BG2_Y = 0;
EWRAM_DATA u16 gBattle_BG3_X = 0;
EWRAM_DATA u16 gBattle_BG3_Y = 0;
EWRAM_DATA u16 gBattle_WIN0H = 0;
EWRAM_DATA u16 gBattle_WIN0V = 0;
EWRAM_DATA u16 gBattle_WIN1H = 0;
EWRAM_DATA u16 gBattle_WIN1V = 0;
EWRAM_DATA u8 gDisplayedStringBattle[425] = {0}; // Increased in size to fit Juan's defeat text (SootopolisCity_Gym_1F_Text_JuanDefeat)
EWRAM_DATA u8 gDisplayedStringBattle[300] = {0};
EWRAM_DATA u8 gBattleTextBuff1[TEXT_BUFF_ARRAY_COUNT] = {0};
EWRAM_DATA u8 gBattleTextBuff2[TEXT_BUFF_ARRAY_COUNT] = {0};
EWRAM_DATA u8 gBattleTextBuff3[TEXT_BUFF_ARRAY_COUNT + 13] = {0};   // expanded for stupidly long z move names
EWRAM_DATA u8 gBattleTextBuff3[TEXT_BUFF_ARRAY_COUNT] = {0};
EWRAM_DATA static u32 sUnusedUnknownArray[25] = {0};
EWRAM_DATA u32 gBattleTypeFlags = 0;
EWRAM_DATA u8 gBattleTerrain = 0;
EWRAM_DATA u32 gUnusedFirstBattleVar1 = 0; // Never read
EWRAM_DATA struct MultiPartnerMenuPokemon gMultiPartnerParty[MULTI_PARTY_SIZE] = {0};
EWRAM_DATA static struct MultiPartnerMenuPokemon* sMultiPartnerPartyBuffer = NULL;
EWRAM_DATA u8 *gBattleAnimBgTileBuffer = NULL;
EWRAM_DATA u8 *gBattleAnimBgTilemapBuffer = NULL;
EWRAM_DATA struct UnknownPokemonStruct4 gMultiPartnerParty[MULTI_PARTY_SIZE] = {0};
EWRAM_DATA static struct UnknownPokemonStruct4* sMultiPartnerPartyBuffer = NULL;
EWRAM_DATA u8 *gUnknown_0202305C = NULL;
EWRAM_DATA u8 *gUnknown_02023060 = NULL;
EWRAM_DATA u8 gBattleBufferA[MAX_BATTLERS_COUNT][0x200] = {0};
EWRAM_DATA u8 gBattleBufferB[MAX_BATTLERS_COUNT][0x200] = {0};
EWRAM_DATA u8 gActiveBattler = 0;
EWRAM_DATA u32 gBattleControllerExecFlags = 0;
EWRAM_DATA u8 gBattlersCount = 0;
EWRAM_DATA u16 gBattlerPartyIndexes[MAX_BATTLERS_COUNT] = {0};
EWRAM_DATA u8 gBattlerPositions[MAX_BATTLERS_COUNT] = {0};
EWRAM_DATA u8 gActionsByTurnOrder[MAX_BATTLERS_COUNT] = {0};
EWRAM_DATA u8 gBattlerByTurnOrder[MAX_BATTLERS_COUNT] = {0};
EWRAM_DATA u8 gCurrentTurnActionNumber = 0;
EWRAM_DATA u8 gCurrentActionFuncId = 0;
EWRAM_DATA struct BattlePokemon gBattleMons[MAX_BATTLERS_COUNT] = {0};
EWRAM_DATA u8 gBattlerSpriteIds[MAX_BATTLERS_COUNT] = {0};
EWRAM_DATA u8 gCurrMovePos = 0;
EWRAM_DATA u8 gChosenMovePos = 0;
EWRAM_DATA u16 gCurrentMove = 0;
EWRAM_DATA u16 gChosenMove = 0;
EWRAM_DATA u16 gCalledMove = 0;
EWRAM_DATA s32 gBattleMoveDamage = 0;
EWRAM_DATA s32 gHpDealt = 0;
EWRAM_DATA s32 gTakenDmg[MAX_BATTLERS_COUNT] = {0};
EWRAM_DATA u16 gLastUsedItem = 0;
EWRAM_DATA u8 gLastUsedAbility = 0;
EWRAM_DATA u8 gBattlerAttacker = 0;
EWRAM_DATA u8 gBattlerTarget = 0;
EWRAM_DATA u8 gBattlerFainted = 0;
EWRAM_DATA u8 gEffectBattler = 0;
EWRAM_DATA u8 gPotentialItemEffectBattler = 0;
EWRAM_DATA u8 gAbsentBattlerFlags = 0;
EWRAM_DATA u8 gCritMultiplier = 0;
EWRAM_DATA u8 gMultiHitCounter = 0;
EWRAM_DATA const u8 *gBattlescriptCurrInstr = NULL;
EWRAM_DATA u32 gUnusedBattleMainVar = 0;
EWRAM_DATA u8 gChosenActionByBattler[MAX_BATTLERS_COUNT] = {0};
EWRAM_DATA const u8 *gSelectionBattleScripts[MAX_BATTLERS_COUNT] = {NULL};
EWRAM_DATA const u8 *gPalaceSelectionBattleScripts[MAX_BATTLERS_COUNT] = {NULL};
EWRAM_DATA u16 gLastPrintedMoves[MAX_BATTLERS_COUNT] = {0};
EWRAM_DATA u16 gLastMoves[MAX_BATTLERS_COUNT] = {0};
EWRAM_DATA u16 gLastLandedMoves[MAX_BATTLERS_COUNT] = {0};
EWRAM_DATA u16 gLastHitByType[MAX_BATTLERS_COUNT] = {0};
EWRAM_DATA u16 gLastResultingMoves[MAX_BATTLERS_COUNT] = {0};
EWRAM_DATA u16 gLockedMoves[MAX_BATTLERS_COUNT] = {0};
EWRAM_DATA u8 gLastHitBy[MAX_BATTLERS_COUNT] = {0};
EWRAM_DATA u16 gChosenMoveByBattler[MAX_BATTLERS_COUNT] = {0};
EWRAM_DATA u8 gMoveResultFlags = 0;
EWRAM_DATA u32 gHitMarker = 0;
EWRAM_DATA static u8 sUnusedBattlersArray[MAX_BATTLERS_COUNT] = {0};
EWRAM_DATA u8 gTakenDmgByBattler[MAX_BATTLERS_COUNT] = {0};
EWRAM_DATA u8 gUnusedFirstBattleVar2 = 0; // Never read
EWRAM_DATA u16 gSideStatuses[2] = {0};
EWRAM_DATA struct SideTimer gSideTimers[2] = {0};
EWRAM_DATA u32 gStatuses3[MAX_BATTLERS_COUNT] = {0};
EWRAM_DATA struct DisableStruct gDisableStructs[MAX_BATTLERS_COUNT] = {0};
EWRAM_DATA u16 gPauseCounterBattle = 0;
EWRAM_DATA u16 gPaydayMoney = 0;
EWRAM_DATA u16 gRandomTurnNumber = 0;
EWRAM_DATA u8 gBattleCommunication[BATTLE_COMMUNICATION_ENTRIES_COUNT] = {0};
EWRAM_DATA u8 gBattleOutcome = 0;
EWRAM_DATA struct ProtectStruct gProtectStructs[MAX_BATTLERS_COUNT] = {0};
EWRAM_DATA struct SpecialStatus gSpecialStatuses[MAX_BATTLERS_COUNT] = {0};
EWRAM_DATA u16 gBattleWeather = 0;
EWRAM_DATA struct WishFutureKnock gWishFutureKnock = {0};
EWRAM_DATA u16 gIntroSlideFlags = 0;
EWRAM_DATA u8 gSentPokesToOpponent[2] = {0};
EWRAM_DATA u16 gDynamicBasePower = 0;
EWRAM_DATA u16 gExpShareExp = 0;
EWRAM_DATA struct BattleEnigmaBerry gEnigmaBerries[MAX_BATTLERS_COUNT] = {0};
EWRAM_DATA struct BattleScripting gBattleScripting = {0};
EWRAM_DATA struct BattleStruct *gBattleStruct = NULL;
EWRAM_DATA u8 *gLinkBattleSendBuffer = NULL;
EWRAM_DATA u8 *gLinkBattleRecvBuffer = NULL;
EWRAM_DATA struct BattleResources *gBattleResources = NULL;
EWRAM_DATA u8 gActionSelectionCursor[MAX_BATTLERS_COUNT] = {0};
EWRAM_DATA u8 gMoveSelectionCursor[MAX_BATTLERS_COUNT] = {0};
EWRAM_DATA u8 gBattlerStatusSummaryTaskId[MAX_BATTLERS_COUNT] = {0};
EWRAM_DATA u8 gBattlerInMenuId = 0;
EWRAM_DATA bool8 gDoingBattleAnim = FALSE;
EWRAM_DATA u32 gTransformedPersonalities[MAX_BATTLERS_COUNT] = {0};
EWRAM_DATA u8 gPlayerDpadHoldFrames = 0;
EWRAM_DATA struct BattleSpriteData *gBattleSpritesDataPtr = NULL;
EWRAM_DATA struct MonSpritesGfx *gMonSpritesGfxPtr = NULL;
EWRAM_DATA struct BattleHealthboxInfo *gBattleControllerOpponentHealthboxData = NULL; // Never read
EWRAM_DATA struct BattleHealthboxInfo *gBattleControllerOpponentFlankHealthboxData = NULL; // Never read
EWRAM_DATA u16 gBattleMovePower = 0;
EWRAM_DATA u16 gMoveToLearn = 0;
EWRAM_DATA u32 gFieldStatuses = 0;
EWRAM_DATA struct FieldTimer gFieldTimers = {0};
EWRAM_DATA u8 gBattlerAbility = 0;
EWRAM_DATA u16 gPartnerSpriteId = 0;
EWRAM_DATA struct QueuedStatBoost gQueuedStatBoosts[MAX_BATTLERS_COUNT] = {0};
EWRAM_DATA bool8 gHasFetchedBall = FALSE;
EWRAM_DATA u8 gLastUsedBall = 0;
EWRAM_DATA u16 gLastThrownBall = 0;
EWRAM_DATA u16 gBallToDisplay = 0;
EWRAM_DATA bool8 gLastUsedBallMenuPresent = FALSE;
EWRAM_DATA u8 gPartyCriticalHits[PARTY_SIZE] = {0};
EWRAM_DATA static u8 sTriedEvolving = 0;

EWRAM_DATA u8 gBattleMonForms[MAX_BATTLERS_COUNT] = {0};

// IWRAM common vars
void (*gPreBattleCallback1)(void);
void (*gBattleMainFunc)(void);
struct BattleResults gBattleResults;
u8 gLeveledUpInBattle;
u8 gHealthboxSpriteIds[MAX_BATTLERS_COUNT];
u8 gMultiUsePlayerCursor;
u8 gNumberOfMovesToChoose;

// rom const data
static const struct ScanlineEffectParams sIntroScanlineParams16Bit =
{
    (void *)REG_ADDR_BG3HOFS, SCANLINE_EFFECT_DMACNT_16BIT, 1
};

// unused
static const struct ScanlineEffectParams sIntroScanlineParams32Bit =
{
    (void *)REG_ADDR_BG3HOFS, SCANLINE_EFFECT_DMACNT_32BIT, 1
};

const struct SpriteTemplate gUnknown_0831AC88 =
{
    .tileTag = 0,
    .paletteTag = 0,
    .oam = &gDummyOamData,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = sub_8038528,
};

static const u8 sText_ShedinjaJpnName[] = _("ヌケニン"); // Nukenin

const struct OamData gOamData_BattleSpriteOpponentSide =
{
    .y = 0,
    .affineMode = ST_OAM_AFFINE_NORMAL,
    .objMode = ST_OAM_OBJ_NORMAL,
    .bpp = ST_OAM_4BPP,
    .shape = SPRITE_SHAPE(64x64),
    .x = 0,
    .size = SPRITE_SIZE(64x64),
    .tileNum = 0,
    .priority = 2,
    .paletteNum = 0,
    .affineParam = 0,
};

const struct OamData gOamData_BattleSpritePlayerSide =
{
    .y = 0,
    .affineMode = ST_OAM_AFFINE_NORMAL,
    .objMode = ST_OAM_OBJ_NORMAL,
    .bpp = ST_OAM_4BPP,
    .shape = SPRITE_SHAPE(64x64),
    .x = 0,
    .size = SPRITE_SIZE(64x64),
    .tileNum = 0,
    .priority = 2,
    .paletteNum = 2,
    .affineParam = 0,
};

// Unknown and unused data. Feel free to remove.
static const u16 sUnused1[] = {0, 5, 0xfffe, 0};
static const u16 *const sUnused1Ptr = sUnused1;
static const u16 sUnused2[] = {0xfff0, 0, 0x0400, 0, 0, 0, 0x3c00, 0, 0x7ffe, 1, 0, 0};
static const u16 *const sUnused2Ptr = sUnused2;

static const s8 gUnknown_0831ACE0[] ={-32, -16, -16, -32, -32, 0, 0, 0};

// format: attacking type, defending type, damage multiplier
// the multiplier is a (decimal) fixed-point number:
// 20 is ×2.0 TYPE_MUL_SUPER_EFFECTIVE
// 10 is ×1.0 TYPE_MUL_NORMAL
// 05 is ×0.5 TYPE_MUL_NOT_EFFECTIVE
// 00 is ×0.0 TYPE_MUL_NO_EFFECT
const u8 gTypeEffectiveness[336] =
{
    TYPE_NORMAL, TYPE_ROCK, TYPE_MUL_NOT_EFFECTIVE,
    TYPE_NORMAL, TYPE_STEEL, TYPE_MUL_NOT_EFFECTIVE,
    TYPE_FIRE, TYPE_FIRE, TYPE_MUL_NOT_EFFECTIVE,
    TYPE_FIRE, TYPE_WATER, TYPE_MUL_NOT_EFFECTIVE,
    TYPE_FIRE, TYPE_GRASS, TYPE_MUL_SUPER_EFFECTIVE,
    TYPE_FIRE, TYPE_ICE, TYPE_MUL_SUPER_EFFECTIVE,
    TYPE_FIRE, TYPE_BUG, TYPE_MUL_SUPER_EFFECTIVE,
    TYPE_FIRE, TYPE_ROCK, TYPE_MUL_NOT_EFFECTIVE,
    TYPE_FIRE, TYPE_DRAGON, TYPE_MUL_NOT_EFFECTIVE,
    TYPE_FIRE, TYPE_STEEL, TYPE_MUL_SUPER_EFFECTIVE,
    TYPE_WATER, TYPE_FIRE, TYPE_MUL_SUPER_EFFECTIVE,
    TYPE_WATER, TYPE_WATER, TYPE_MUL_NOT_EFFECTIVE,
    TYPE_WATER, TYPE_GRASS, TYPE_MUL_NOT_EFFECTIVE,
    TYPE_WATER, TYPE_GROUND, TYPE_MUL_SUPER_EFFECTIVE,
    TYPE_WATER, TYPE_ROCK, TYPE_MUL_SUPER_EFFECTIVE,
    TYPE_WATER, TYPE_DRAGON, TYPE_MUL_NOT_EFFECTIVE,
    TYPE_ELECTRIC, TYPE_WATER, TYPE_MUL_SUPER_EFFECTIVE,
    TYPE_ELECTRIC, TYPE_ELECTRIC, TYPE_MUL_NOT_EFFECTIVE,
    TYPE_ELECTRIC, TYPE_GRASS, TYPE_MUL_NOT_EFFECTIVE,
    TYPE_ELECTRIC, TYPE_GROUND, TYPE_MUL_NO_EFFECT,
    TYPE_ELECTRIC, TYPE_FLYING, TYPE_MUL_SUPER_EFFECTIVE,
    TYPE_ELECTRIC, TYPE_DRAGON, TYPE_MUL_NOT_EFFECTIVE,
    TYPE_GRASS, TYPE_FIRE, TYPE_MUL_NOT_EFFECTIVE,
    TYPE_GRASS, TYPE_WATER, TYPE_MUL_SUPER_EFFECTIVE,
    TYPE_GRASS, TYPE_GRASS, TYPE_MUL_NOT_EFFECTIVE,
    TYPE_GRASS, TYPE_POISON, TYPE_MUL_NOT_EFFECTIVE,
    TYPE_GRASS, TYPE_GROUND, TYPE_MUL_SUPER_EFFECTIVE,
    TYPE_GRASS, TYPE_FLYING, TYPE_MUL_NOT_EFFECTIVE,
    TYPE_GRASS, TYPE_BUG, TYPE_MUL_NOT_EFFECTIVE,
    TYPE_GRASS, TYPE_ROCK, TYPE_MUL_SUPER_EFFECTIVE,
    TYPE_GRASS, TYPE_DRAGON, TYPE_MUL_NOT_EFFECTIVE,
    TYPE_GRASS, TYPE_STEEL, TYPE_MUL_NOT_EFFECTIVE,
    TYPE_ICE, TYPE_WATER, TYPE_MUL_NOT_EFFECTIVE,
    TYPE_ICE, TYPE_GRASS, TYPE_MUL_SUPER_EFFECTIVE,
    TYPE_ICE, TYPE_ICE, TYPE_MUL_NOT_EFFECTIVE,
    TYPE_ICE, TYPE_GROUND, TYPE_MUL_SUPER_EFFECTIVE,
    TYPE_ICE, TYPE_FLYING, TYPE_MUL_SUPER_EFFECTIVE,
    TYPE_ICE, TYPE_DRAGON, TYPE_MUL_SUPER_EFFECTIVE,
    TYPE_ICE, TYPE_STEEL, TYPE_MUL_NOT_EFFECTIVE,
    TYPE_ICE, TYPE_FIRE, TYPE_MUL_NOT_EFFECTIVE,
    TYPE_FIGHTING, TYPE_NORMAL, TYPE_MUL_SUPER_EFFECTIVE,
    TYPE_FIGHTING, TYPE_ICE, TYPE_MUL_SUPER_EFFECTIVE,
    TYPE_FIGHTING, TYPE_POISON, TYPE_MUL_NOT_EFFECTIVE,
    TYPE_FIGHTING, TYPE_FLYING, TYPE_MUL_NOT_EFFECTIVE,
    TYPE_FIGHTING, TYPE_PSYCHIC, TYPE_MUL_NOT_EFFECTIVE,
    TYPE_FIGHTING, TYPE_BUG, TYPE_MUL_NOT_EFFECTIVE,
    TYPE_FIGHTING, TYPE_ROCK, TYPE_MUL_SUPER_EFFECTIVE,
    TYPE_FIGHTING, TYPE_DARK, TYPE_MUL_SUPER_EFFECTIVE,
    TYPE_FIGHTING, TYPE_STEEL, TYPE_MUL_SUPER_EFFECTIVE,
    TYPE_POISON, TYPE_GRASS, TYPE_MUL_SUPER_EFFECTIVE,
    TYPE_POISON, TYPE_POISON, TYPE_MUL_NOT_EFFECTIVE,
    TYPE_POISON, TYPE_GROUND, TYPE_MUL_NOT_EFFECTIVE,
    TYPE_POISON, TYPE_ROCK, TYPE_MUL_NOT_EFFECTIVE,
    TYPE_POISON, TYPE_GHOST, TYPE_MUL_NOT_EFFECTIVE,
    TYPE_POISON, TYPE_STEEL, TYPE_MUL_NO_EFFECT,
    TYPE_GROUND, TYPE_FIRE, TYPE_MUL_SUPER_EFFECTIVE,
    TYPE_GROUND, TYPE_ELECTRIC, TYPE_MUL_SUPER_EFFECTIVE,
    TYPE_GROUND, TYPE_GRASS, TYPE_MUL_NOT_EFFECTIVE,
    TYPE_GROUND, TYPE_POISON, TYPE_MUL_SUPER_EFFECTIVE,
    TYPE_GROUND, TYPE_FLYING, TYPE_MUL_NO_EFFECT,
    TYPE_GROUND, TYPE_BUG, TYPE_MUL_NOT_EFFECTIVE,
    TYPE_GROUND, TYPE_ROCK, TYPE_MUL_SUPER_EFFECTIVE,
    TYPE_GROUND, TYPE_STEEL, TYPE_MUL_SUPER_EFFECTIVE,
    TYPE_FLYING, TYPE_ELECTRIC, TYPE_MUL_NOT_EFFECTIVE,
    TYPE_FLYING, TYPE_GRASS, TYPE_MUL_SUPER_EFFECTIVE,
    TYPE_FLYING, TYPE_FIGHTING, TYPE_MUL_SUPER_EFFECTIVE,
    TYPE_FLYING, TYPE_BUG, TYPE_MUL_SUPER_EFFECTIVE,
    TYPE_FLYING, TYPE_ROCK, TYPE_MUL_NOT_EFFECTIVE,
    TYPE_FLYING, TYPE_STEEL, TYPE_MUL_NOT_EFFECTIVE,
    TYPE_PSYCHIC, TYPE_FIGHTING, TYPE_MUL_SUPER_EFFECTIVE,
    TYPE_PSYCHIC, TYPE_POISON, TYPE_MUL_SUPER_EFFECTIVE,
    TYPE_PSYCHIC, TYPE_PSYCHIC, TYPE_MUL_NOT_EFFECTIVE,
    TYPE_PSYCHIC, TYPE_DARK, TYPE_MUL_NO_EFFECT,
    TYPE_PSYCHIC, TYPE_STEEL, TYPE_MUL_NOT_EFFECTIVE,
    TYPE_BUG, TYPE_FIRE, TYPE_MUL_NOT_EFFECTIVE,
    TYPE_BUG, TYPE_GRASS, TYPE_MUL_SUPER_EFFECTIVE,
    TYPE_BUG, TYPE_FIGHTING, TYPE_MUL_NOT_EFFECTIVE,
    TYPE_BUG, TYPE_POISON, TYPE_MUL_NOT_EFFECTIVE,
    TYPE_BUG, TYPE_FLYING, TYPE_MUL_NOT_EFFECTIVE,
    TYPE_BUG, TYPE_PSYCHIC, TYPE_MUL_SUPER_EFFECTIVE,
    TYPE_BUG, TYPE_GHOST, TYPE_MUL_NOT_EFFECTIVE,
    TYPE_BUG, TYPE_DARK, TYPE_MUL_SUPER_EFFECTIVE,
    TYPE_BUG, TYPE_STEEL, TYPE_MUL_NOT_EFFECTIVE,
    TYPE_ROCK, TYPE_FIRE, TYPE_MUL_SUPER_EFFECTIVE,
    TYPE_ROCK, TYPE_ICE, TYPE_MUL_SUPER_EFFECTIVE,
    TYPE_ROCK, TYPE_FIGHTING, TYPE_MUL_NOT_EFFECTIVE,
    TYPE_ROCK, TYPE_GROUND, TYPE_MUL_NOT_EFFECTIVE,
    TYPE_ROCK, TYPE_FLYING, TYPE_MUL_SUPER_EFFECTIVE,
    TYPE_ROCK, TYPE_BUG, TYPE_MUL_SUPER_EFFECTIVE,
    TYPE_ROCK, TYPE_STEEL, TYPE_MUL_NOT_EFFECTIVE,
    TYPE_GHOST, TYPE_NORMAL, TYPE_MUL_NO_EFFECT,
    TYPE_GHOST, TYPE_PSYCHIC, TYPE_MUL_SUPER_EFFECTIVE,
    TYPE_GHOST, TYPE_DARK, TYPE_MUL_NOT_EFFECTIVE,
    TYPE_GHOST, TYPE_STEEL, TYPE_MUL_NOT_EFFECTIVE,
    TYPE_GHOST, TYPE_GHOST, TYPE_MUL_SUPER_EFFECTIVE,
    TYPE_DRAGON, TYPE_DRAGON, TYPE_MUL_SUPER_EFFECTIVE,
    TYPE_DRAGON, TYPE_STEEL, TYPE_MUL_NOT_EFFECTIVE,
    TYPE_DARK, TYPE_FIGHTING, TYPE_MUL_NOT_EFFECTIVE,
    TYPE_DARK, TYPE_PSYCHIC, TYPE_MUL_SUPER_EFFECTIVE,
    TYPE_DARK, TYPE_GHOST, TYPE_MUL_SUPER_EFFECTIVE,
    TYPE_DARK, TYPE_DARK, TYPE_MUL_NOT_EFFECTIVE,
    TYPE_DARK, TYPE_STEEL, TYPE_MUL_NOT_EFFECTIVE,
    TYPE_STEEL, TYPE_FIRE, TYPE_MUL_NOT_EFFECTIVE,
    TYPE_STEEL, TYPE_WATER, TYPE_MUL_NOT_EFFECTIVE,
    TYPE_STEEL, TYPE_ELECTRIC, TYPE_MUL_NOT_EFFECTIVE,
    TYPE_STEEL, TYPE_ICE, TYPE_MUL_SUPER_EFFECTIVE,
    TYPE_STEEL, TYPE_ROCK, TYPE_MUL_SUPER_EFFECTIVE,
    TYPE_STEEL, TYPE_STEEL, TYPE_MUL_NOT_EFFECTIVE,
    TYPE_FORESIGHT, TYPE_FORESIGHT, TYPE_MUL_NO_EFFECT,
    TYPE_NORMAL, TYPE_GHOST, TYPE_MUL_NO_EFFECT,
    TYPE_FIGHTING, TYPE_GHOST, TYPE_MUL_NO_EFFECT,
    TYPE_ENDTABLE, TYPE_ENDTABLE, TYPE_MUL_NO_EFFECT
};

const u8 gTypeNames[NUMBER_OF_MON_TYPES][TYPE_NAME_LENGTH + 1] =
{
    [TYPE_NORMAL] = _("NORMAL"),
    [TYPE_FIGHTING] = _("FIGHT"),
    [TYPE_FLYING] = _("FLYING"),
    [TYPE_POISON] = _("POISON"),
    [TYPE_GROUND] = _("GROUND"),
    [TYPE_ROCK] = _("ROCK"),
    [TYPE_BUG] = _("BUG"),
    [TYPE_GHOST] = _("GHOST"),
    [TYPE_STEEL] = _("STEEL"),
    [TYPE_MYSTERY] = _("???"),
    [TYPE_FIRE] = _("FIRE"),
    [TYPE_WATER] = _("WATER"),
    [TYPE_GRASS] = _("GRASS"),
    [TYPE_ELECTRIC] = _("ELECTR"),
    [TYPE_PSYCHIC] = _("PSYCHC"),
    [TYPE_ICE] = _("ICE"),
    [TYPE_DRAGON] = _("DRAGON"),
    [TYPE_DARK] = _("DARK"),
};

// This is a factor in how much money you get for beating a trainer.
const struct TrainerMoney gTrainerMoneyTable[] =
{
    {TRAINER_CLASS_TEAM_AQUA, 5},
    {TRAINER_CLASS_AQUA_ADMIN, 10},
    {TRAINER_CLASS_AQUA_LEADER, 20},
    {TRAINER_CLASS_AROMA_LADY, 10},
    {TRAINER_CLASS_RUIN_MANIAC, 15},
    {TRAINER_CLASS_INTERVIEWER, 12},
    {TRAINER_CLASS_TUBER_F, 1},
    {TRAINER_CLASS_TUBER_M, 1},
    {TRAINER_CLASS_SIS_AND_BRO, 3},
    {TRAINER_CLASS_COOLTRAINER, 12},
    {TRAINER_CLASS_HEX_MANIAC, 6},
    {TRAINER_CLASS_LADY, 50},
    {TRAINER_CLASS_BEAUTY, 20},
    {TRAINER_CLASS_RICH_BOY, 50},
    {TRAINER_CLASS_POKEMANIAC, 15},
    {TRAINER_CLASS_SWIMMER_M, 2},
    {TRAINER_CLASS_BLACK_BELT, 8},
    {TRAINER_CLASS_GUITARIST, 8},
    {TRAINER_CLASS_KINDLER, 8},
    {TRAINER_CLASS_CAMPER, 4},
    {TRAINER_CLASS_OLD_COUPLE, 10},
    {TRAINER_CLASS_BUG_MANIAC, 15},
    {TRAINER_CLASS_PSYCHIC, 6},
    {TRAINER_CLASS_GENTLEMAN, 20},
    {TRAINER_CLASS_ELITE_FOUR, 25},
    {TRAINER_CLASS_LEADER, 25},
    {TRAINER_CLASS_SCHOOL_KID, 5},
    {TRAINER_CLASS_SR_AND_JR, 4},
    {TRAINER_CLASS_POKEFAN, 20},
    {TRAINER_CLASS_EXPERT, 10},
    {TRAINER_CLASS_YOUNGSTER, 4},
    {TRAINER_CLASS_CHAMPION, 50},
    {TRAINER_CLASS_FISHERMAN, 10},
    {TRAINER_CLASS_TRIATHLETE, 10},
    {TRAINER_CLASS_DRAGON_TAMER, 12},
    {TRAINER_CLASS_BIRD_KEEPER, 8},
    {TRAINER_CLASS_NINJA_BOY, 3},
    {TRAINER_CLASS_BATTLE_GIRL, 6},
    {TRAINER_CLASS_PARASOL_LADY, 10},
    {TRAINER_CLASS_SWIMMER_F, 2},
    {TRAINER_CLASS_PICNICKER, 4},
    {TRAINER_CLASS_TWINS, 3},
    {TRAINER_CLASS_SAILOR, 8},
    {TRAINER_CLASS_COLLECTOR, 15},
    {TRAINER_CLASS_PKMN_TRAINER_3, 15},
    {TRAINER_CLASS_PKMN_BREEDER, 10},
    {TRAINER_CLASS_PKMN_RANGER, 12},
    {TRAINER_CLASS_TEAM_MAGMA, 5},
    {TRAINER_CLASS_MAGMA_ADMIN, 10},
    {TRAINER_CLASS_MAGMA_LEADER, 20},
    {TRAINER_CLASS_LASS, 4},
    {TRAINER_CLASS_BUG_CATCHER, 4},
    {TRAINER_CLASS_HIKER, 10},
    {TRAINER_CLASS_YOUNG_COUPLE, 8},
    {TRAINER_CLASS_WINSTRATE, 10},
    {0xFF, 5},
};

#include "data/text/abilities.h"

static void (* const sTurnActionsFuncsTable[])(void) =
{
    [B_ACTION_USE_MOVE] = HandleAction_UseMove,
    [B_ACTION_USE_ITEM] = HandleAction_UseItem,
    [B_ACTION_SWITCH] = HandleAction_Switch,
    [B_ACTION_RUN] = HandleAction_Run,
    [B_ACTION_SAFARI_WATCH_CAREFULLY] = HandleAction_WatchesCarefully,
    [B_ACTION_SAFARI_BALL] = HandleAction_SafariZoneBallThrow,
    [B_ACTION_SAFARI_POKEBLOCK] = HandleAction_ThrowPokeblock,
    [B_ACTION_SAFARI_GO_NEAR] = HandleAction_GoNear,
    [B_ACTION_SAFARI_RUN] = HandleAction_SafariZoneRun,
    [B_ACTION_WALLY_THROW] = HandleAction_WallyBallThrow,
    [B_ACTION_EXEC_SCRIPT] = HandleAction_RunBattleScript,
    [B_ACTION_TRY_FINISH] = HandleAction_TryFinish,
    [B_ACTION_FINISHED] = HandleAction_ActionFinished,
    [B_ACTION_NOTHING_FAINTED] = HandleAction_NothingIsFainted,
};

static void (* const sEndTurnFuncsTable[])(void) =
{
    [0] = HandleEndTurn_ContinueBattle, //B_OUTCOME_NONE?
    [B_OUTCOME_WON] = HandleEndTurn_BattleWon,
    [B_OUTCOME_LOST] = HandleEndTurn_BattleLost,
    [B_OUTCOME_DREW] = HandleEndTurn_BattleLost,
    [B_OUTCOME_RAN] = HandleEndTurn_RanFromBattle,
    [B_OUTCOME_PLAYER_TELEPORTED] = HandleEndTurn_FinishBattle,
    [B_OUTCOME_MON_FLED] = HandleEndTurn_MonFled,
    [B_OUTCOME_CAUGHT] = HandleEndTurn_FinishBattle,
    [B_OUTCOME_NO_SAFARI_BALLS] = HandleEndTurn_FinishBattle,
    [B_OUTCOME_FORFEITED] = HandleEndTurn_FinishBattle,
    [B_OUTCOME_MON_TELEPORTED] = HandleEndTurn_FinishBattle,
};

const u8 gStatusConditionString_PoisonJpn[8] = _("どく$$$$$");
const u8 gStatusConditionString_SleepJpn[8] = _("ねむり$$$$");
const u8 gStatusConditionString_ParalysisJpn[8] = _("まひ$$$$$");
const u8 gStatusConditionString_BurnJpn[8] = _("やけど$$$$");
const u8 gStatusConditionString_IceJpn[8] = _("こおり$$$$");
const u8 gStatusConditionString_ConfusionJpn[8] = _("こんらん$$$");
const u8 gStatusConditionString_LoveJpn[8] = _("メロメロ$$$");

const u8 * const gStatusConditionStringsTable[7][2] =
{
    {gStatusConditionString_PoisonJpn, gText_Poison},
    {gStatusConditionString_SleepJpn, gText_Sleep},
    {gStatusConditionString_ParalysisJpn, gText_Paralysis},
    {gStatusConditionString_BurnJpn, gText_Burn},
    {gStatusConditionString_IceJpn, gText_Ice},
    {gStatusConditionString_ConfusionJpn, gText_Confusion},
    {gStatusConditionString_LoveJpn, gText_Love}
};

// code
void CB2_InitBattle(void)
{
    MoveSaveBlocks_ResetHeap();
    AllocateBattleResources();
    AllocateBattleSpritesData();
    AllocateMonSpritesGfx();
    RecordedBattle_ClearFrontierPassFlag();

    if (gBattleTypeFlags & BATTLE_TYPE_MULTI)
    {
        if (gBattleTypeFlags & BATTLE_TYPE_RECORDED)
        {
            CB2_InitBattleInternal();
        }
        else if (!(gBattleTypeFlags & BATTLE_TYPE_INGAME_PARTNER))
        {
            HandleLinkBattleSetup();
            SetMainCallback2(CB2_PreInitMultiBattle);
        }
        else
        {
            SetMainCallback2(CB2_PreInitIngamePlayerPartnerBattle);
        }
        gBattleCommunication[MULTIUSE_STATE] = 0;
    }
    else
    {
        CB2_InitBattleInternal();
    }
}

static void CB2_InitBattleInternal(void)
{
    s32 i;

    SetHBlankCallback(NULL);
    SetVBlankCallback(NULL);

    CpuFill32(0, (void*)(VRAM), VRAM_SIZE);

    SetGpuReg(REG_OFFSET_MOSAIC, 0);
    SetGpuReg(REG_OFFSET_WIN0H, DISPLAY_WIDTH);
    SetGpuReg(REG_OFFSET_WIN0V, WIN_RANGE(DISPLAY_HEIGHT / 2, DISPLAY_HEIGHT / 2 + 1));
    SetGpuReg(REG_OFFSET_WININ, 0);
    SetGpuReg(REG_OFFSET_WINOUT, 0);

    gBattle_WIN0H = DISPLAY_WIDTH;

    if (gBattleTypeFlags & BATTLE_TYPE_INGAME_PARTNER && gPartnerTrainerId != TRAINER_STEVEN_PARTNER)
    {
        gBattle_WIN0V = DISPLAY_HEIGHT - 1;
        gBattle_WIN1H = DISPLAY_WIDTH;
        gBattle_WIN1V = 32;
    }
    else
    {
        gBattle_WIN0V = WIN_RANGE(DISPLAY_HEIGHT / 2, DISPLAY_HEIGHT / 2 + 1);
        ScanlineEffect_Clear();

        for (i = 0; i < DISPLAY_HEIGHT / 2; i++)
        {
            gScanlineEffectRegBuffers[0][i] = 0xF0;
            gScanlineEffectRegBuffers[1][i] = 0xF0;
        }

        for (; i < DISPLAY_HEIGHT; i++)
        {
            gScanlineEffectRegBuffers[0][i] = 0xFF10;
            gScanlineEffectRegBuffers[1][i] = 0xFF10;
        }

        ScanlineEffect_SetParams(sIntroScanlineParams16Bit);
    }

    ResetPaletteFade();
    gBattle_BG0_X = 0;
    gBattle_BG0_Y = 0;
    gBattle_BG1_X = 0;
    gBattle_BG1_Y = 0;
    gBattle_BG2_X = 0;
    gBattle_BG2_Y = 0;
    gBattle_BG3_X = 0;
    gBattle_BG3_Y = 0;

#if DEBUG_OVERWORLD_MENU == TRUE
    if (!gIsDebugBattle)
#endif
    {
        gBattleTerrain = BattleSetup_GetTerrainId();
    }
    if (gBattleTypeFlags & BATTLE_TYPE_RECORDED)
        gBattleTerrain = BATTLE_TERRAIN_BUILDING;

    InitBattleBgsVideo();
    LoadBattleTextboxAndBackground();
    ResetSpriteData();
    ResetTasks();
    DrawBattleEntryBackground();
    FreeAllSpritePalettes();
    gReservedSpritePaletteCount = 4;
    SetVBlankCallback(VBlankCB_Battle);
    SetUpBattleVarsAndBirchZigzagoon();

    if (gBattleTypeFlags & BATTLE_TYPE_MULTI && gBattleTypeFlags & BATTLE_TYPE_BATTLE_TOWER)
        SetMainCallback2(CB2_HandleStartMultiPartnerBattle);
    else if (gBattleTypeFlags & BATTLE_TYPE_MULTI && gBattleTypeFlags & BATTLE_TYPE_INGAME_PARTNER)
        SetMainCallback2(CB2_HandleStartMultiPartnerBattle);
    else if (gBattleTypeFlags & BATTLE_TYPE_MULTI)
        SetMainCallback2(CB2_HandleStartMultiBattle);
    else
        SetMainCallback2(CB2_HandleStartBattle);

#if DEBUG_OVERWORLD_MENU == TRUE
    if (!gIsDebugBattle)
#endif
    {
        if (!(gBattleTypeFlags & (BATTLE_TYPE_LINK | BATTLE_TYPE_RECORDED)))
        {
            CreateNPCTrainerParty(&gEnemyParty[0], gTrainerBattleOpponent_A, TRUE);
            if (gBattleTypeFlags & BATTLE_TYPE_TWO_OPPONENTS && !BATTLE_TWO_VS_ONE_OPPONENT)
                CreateNPCTrainerParty(&gEnemyParty[PARTY_SIZE / 2], gTrainerBattleOpponent_B, FALSE);
            SetWildMonHeldItem();
            CalculateEnemyPartyCount();
        }
    if (!(gBattleTypeFlags & (BATTLE_TYPE_LINK | BATTLE_TYPE_RECORDED)))
    {
        CreateNPCTrainerParty(&gEnemyParty[0], gTrainerBattleOpponent_A, TRUE);
        if (gBattleTypeFlags & BATTLE_TYPE_TWO_OPPONENTS)
            CreateNPCTrainerParty(&gEnemyParty[3], gTrainerBattleOpponent_B, FALSE);
        SetWildMonHeldItem();
    }

    gMain.inBattle = TRUE;
    gSaveBlock2Ptr->frontier.disableRecordBattle = FALSE;

    for (i = 0; i < PARTY_SIZE; i++)
    {
        AdjustFriendship(&gPlayerParty[i], FRIENDSHIP_EVENT_LEAGUE_BATTLE);

        // Apply party-wide start-of-battle form changes for both sides.
        TryFormChange(i, B_SIDE_PLAYER, FORM_CHANGE_BEGIN_BATTLE);
        TryFormChange(i, B_SIDE_OPPONENT, FORM_CHANGE_BEGIN_BATTLE);
    }

    gBattleCommunication[MULTIUSE_STATE] = 0;
}

#define BUFFER_PARTY_VS_SCREEN_STATUS(party, flags, i)              \
    for ((i) = 0; (i) < PARTY_SIZE; (i)++)                          \
    {                                                               \
        u16 species = GetMonData(&(party)[(i)], MON_DATA_SPECIES2); \
        u16 hp = GetMonData(&(party)[(i)], MON_DATA_HP);            \
        u32 status = GetMonData(&(party)[(i)], MON_DATA_STATUS);    \
                                                                    \
        if (species == SPECIES_NONE)                                \
            continue;                                               \
                                                                    \
        /* Is healthy mon? */                                       \
        if (species != SPECIES_EGG && hp != 0 && status == 0)       \
            (flags) |= 1 << (i) * 2;                                \
                                                                    \
        if (species == SPECIES_NONE) /* Redundant */                \
            continue;                                               \
                                                                    \
        /* Is Egg or statused? */                                   \
        if (hp != 0 && (species == SPECIES_EGG || status != 0))     \
            (flags) |= 2 << (i) * 2;                                \
                                                                    \
        if (species == SPECIES_NONE) /* Redundant */                \
            continue;                                               \
                                                                    \
        /* Is fainted? */                                           \
        if (species != SPECIES_EGG && hp == 0)                      \
            (flags) |= 3 << (i) * 2;                                \
    }

// For Vs Screen at link battle start
static void BufferPartyVsScreenHealth_AtStart(void)
{
    u16 flags = 0;
    s32 i;

    BUFFER_PARTY_VS_SCREEN_STATUS(gPlayerParty, flags, i);
    gBattleStruct->multiBuffer.linkBattlerHeader.vsScreenHealthFlagsLo = flags;
    *(&gBattleStruct->multiBuffer.linkBattlerHeader.vsScreenHealthFlagsHi) = flags >> 8;
    gBattleStruct->multiBuffer.linkBattlerHeader.vsScreenHealthFlagsHi |= FlagGet(FLAG_SYS_FRONTIER_PASS) << 7;
}

static void SetPlayerBerryDataInBattleStruct(void)
{
    s32 i;
    struct BattleStruct *battleStruct = gBattleStruct;
    struct BattleEnigmaBerry *battleBerry = &battleStruct->multiBuffer.linkBattlerHeader.battleEnigmaBerry;

    if (IsEnigmaBerryValid() == TRUE)
    {
        for (i = 0; i < BERRY_NAME_LENGTH; i++)
            battleBerry->name[i] = gSaveBlock1Ptr->enigmaBerry.berry.name[i];
        battleBerry->name[i] = EOS;

        for (i = 0; i < BERRY_ITEM_EFFECT_COUNT; i++)
            battleBerry->itemEffect[i] = gSaveBlock1Ptr->enigmaBerry.itemEffect[i];

        battleBerry->holdEffect = gSaveBlock1Ptr->enigmaBerry.holdEffect;
        battleBerry->holdEffectParam = gSaveBlock1Ptr->enigmaBerry.holdEffectParam;
    }
    else
    {
        const struct Berry *berryData = GetBerryInfo(ItemIdToBerryType(ITEM_ENIGMA_BERRY));

        for (i = 0; i < BERRY_NAME_LENGTH; i++)
            battleBerry->name[i] = berryData->name[i];
        battleBerry->name[i] = EOS;

        for (i = 0; i < BERRY_ITEM_EFFECT_COUNT; i++)
            battleBerry->itemEffect[i] = 0;

        battleBerry->holdEffect = HOLD_EFFECT_NONE;
        battleBerry->holdEffectParam = 0;
    }
}

static void SetAllPlayersBerryData(void)
{
    s32 i;
    s32 j;

    if (!(gBattleTypeFlags & BATTLE_TYPE_LINK))
    {
        if (IsEnigmaBerryValid() == TRUE)
        {
            for (i = 0; i < BERRY_NAME_LENGTH; i++)
            {
                gEnigmaBerries[0].name[i] = gSaveBlock1Ptr->enigmaBerry.berry.name[i];
                gEnigmaBerries[2].name[i] = gSaveBlock1Ptr->enigmaBerry.berry.name[i];
            }
            gEnigmaBerries[0].name[i] = EOS;
            gEnigmaBerries[2].name[i] = EOS;

            for (i = 0; i < BERRY_ITEM_EFFECT_COUNT; i++)
            {
                gEnigmaBerries[0].itemEffect[i] = gSaveBlock1Ptr->enigmaBerry.itemEffect[i];
                gEnigmaBerries[2].itemEffect[i] = gSaveBlock1Ptr->enigmaBerry.itemEffect[i];
            }

            gEnigmaBerries[0].holdEffect = gSaveBlock1Ptr->enigmaBerry.holdEffect;
            gEnigmaBerries[2].holdEffect = gSaveBlock1Ptr->enigmaBerry.holdEffect;
            gEnigmaBerries[0].holdEffectParam = gSaveBlock1Ptr->enigmaBerry.holdEffectParam;
            gEnigmaBerries[2].holdEffectParam = gSaveBlock1Ptr->enigmaBerry.holdEffectParam;
        }
        else
        {
            const struct Berry *berryData = GetBerryInfo(ItemIdToBerryType(ITEM_ENIGMA_BERRY));

            for (i = 0; i < BERRY_NAME_LENGTH; i++)
            {
                gEnigmaBerries[0].name[i] = berryData->name[i];
                gEnigmaBerries[2].name[i] = berryData->name[i];
            }
            gEnigmaBerries[0].name[i] = EOS;
            gEnigmaBerries[2].name[i] = EOS;

            for (i = 0; i < BERRY_ITEM_EFFECT_COUNT; i++)
            {
                gEnigmaBerries[0].itemEffect[i] = 0;
                gEnigmaBerries[2].itemEffect[i] = 0;
            }

            gEnigmaBerries[0].holdEffect = 0;
            gEnigmaBerries[2].holdEffect = 0;
            gEnigmaBerries[0].holdEffectParam = 0;
            gEnigmaBerries[2].holdEffectParam = 0;
        }
    }
    else
    {
        s32 numPlayers;
        struct BattleEnigmaBerry *src;
        u32 battler;

        if (gBattleTypeFlags & BATTLE_TYPE_MULTI)
        {
            if (gBattleTypeFlags & BATTLE_TYPE_BATTLE_TOWER)
                numPlayers = 2;
            else
                numPlayers = 4;

            for (i = 0; i < numPlayers; i++)
            {
                src = (struct BattleEnigmaBerry *)(gBlockRecvBuffer[i] + 2);
                battler = gLinkPlayers[i].id;

                for (j = 0; j < BERRY_NAME_LENGTH; j++)
                    gEnigmaBerries[battler].name[j] = src->name[j];
                gEnigmaBerries[battler].name[j] = EOS;

                for (j = 0; j < BERRY_ITEM_EFFECT_COUNT; j++)
                    gEnigmaBerries[battler].itemEffect[j] = src->itemEffect[j];

                gEnigmaBerries[battler].holdEffect = src->holdEffect;
                gEnigmaBerries[battler].holdEffectParam = src->holdEffectParam;
            }
        }
        else
        {
            for (i = 0; i < 2; i++)
            {
                src = (struct BattleEnigmaBerry *)(gBlockRecvBuffer[i] + 2);

                for (j = 0; j < BERRY_NAME_LENGTH; j++)
                {
                    gEnigmaBerries[i].name[j] = src->name[j];
                    gEnigmaBerries[i + 2].name[j] = src->name[j];
                }
                gEnigmaBerries[i].name[j] = EOS;
                gEnigmaBerries[i + 2].name[j] = EOS;

                for (j = 0; j < BERRY_ITEM_EFFECT_COUNT; j++)
                {
                    gEnigmaBerries[i].itemEffect[j] = src->itemEffect[j];
                    gEnigmaBerries[i + 2].itemEffect[j] = src->itemEffect[j];
                }

                gEnigmaBerries[i].holdEffect = src->holdEffect;
                gEnigmaBerries[i + 2].holdEffect = src->holdEffect;
                gEnigmaBerries[i].holdEffectParam = src->holdEffectParam;
                gEnigmaBerries[i + 2].holdEffectParam = src->holdEffectParam;
            }
        }
    }
}

// This was inlined in Ruby/Sapphire
static void FindLinkBattleMaster(u8 numPlayers, u8 multiPlayerId)
{
    u8 found = 0;

    // If player 1 is playing the minimum version, player 1 is master.
    if (gBlockRecvBuffer[0][0] == 0x100)
    {
        if (multiPlayerId == 0)
            gBattleTypeFlags |= BATTLE_TYPE_IS_MASTER | BATTLE_TYPE_TRAINER;
        else
            gBattleTypeFlags |= BATTLE_TYPE_TRAINER;
        found++;
    }

    if (found == 0)
    {
        // If multiple different versions are being used, player 1 is master.
        s32 i;

        for (i = 0; i < numPlayers; i++)
        {
            if (gBlockRecvBuffer[0][0] != gBlockRecvBuffer[i][0])
                break;
        }

        if (i == numPlayers)
        {
            if (multiPlayerId == 0)
                gBattleTypeFlags |= BATTLE_TYPE_IS_MASTER | BATTLE_TYPE_TRAINER;
            else
                gBattleTypeFlags |= BATTLE_TYPE_TRAINER;
            found++;
        }

        if (found == 0)
        {
            // Lowest index player with the highest game version is master.
            for (i = 0; i < numPlayers; i++)
            {
                if (gBlockRecvBuffer[i][0] == 0x300 && i != multiPlayerId)
                {
                    if (i < multiPlayerId)
                        break;
                }
                if (gBlockRecvBuffer[i][0] > 0x300 && i != multiPlayerId)
                    break;
            }

            if (i == numPlayers)
                gBattleTypeFlags |= BATTLE_TYPE_IS_MASTER | BATTLE_TYPE_TRAINER;
            else
                gBattleTypeFlags |= BATTLE_TYPE_TRAINER;
        }
    }
}

static void CB2_HandleStartBattle(void)
{
    u8 playerMultiplayerId;
    u8 enemyMultiplayerId;

    RunTasks();
    AnimateSprites();
    BuildOamBuffer();

    playerMultiplayerId = GetMultiplayerId();
    gBattleScripting.multiplayerId = playerMultiplayerId;
    enemyMultiplayerId = playerMultiplayerId ^ BIT_SIDE;

    switch (gBattleCommunication[MULTIUSE_STATE])
    {
    case 0:
        if (!IsDma3ManagerBusyWithBgCopy())
        {
            ShowBg(0);
            ShowBg(1);
            ShowBg(2);
            ShowBg(3);
            sub_805EF14();
            gBattleCommunication[MULTIUSE_STATE] = 1;
        }
        if (gWirelessCommType)
            LoadWirelessStatusIndicatorSpriteGfx();
        break;
    case 1:
        if (gBattleTypeFlags & BATTLE_TYPE_LINK)
        {
            if (gReceivedRemoteLinkPlayers != 0)
            {
                if (IsLinkTaskFinished())
                {
                    // 0x300
                    *(&gBattleStruct->multiBuffer.linkBattlerHeader.versionSignatureLo) = 0;
                    *(&gBattleStruct->multiBuffer.linkBattlerHeader.versionSignatureHi) = 3;
                    BufferPartyVsScreenHealth_AtStart();
                    SetPlayerBerryDataInBattleStruct();

                    if (gTrainerBattleOpponent_A == TRAINER_UNION_ROOM)
                    {
                        gLinkPlayers[0].id = 0;
                        gLinkPlayers[1].id = 1;
                    }

                    SendBlock(bitmask_all_link_players_but_self(), &gBattleStruct->multiBuffer.linkBattlerHeader, sizeof(gBattleStruct->multiBuffer.linkBattlerHeader));
                    gBattleCommunication[MULTIUSE_STATE] = 2;
                }
                if (gWirelessCommType)
                    CreateWirelessStatusIndicatorSprite(0, 0);
            }
        }
        else
        {
            if (!(gBattleTypeFlags & BATTLE_TYPE_RECORDED))
                gBattleTypeFlags |= BATTLE_TYPE_IS_MASTER;
            gBattleCommunication[MULTIUSE_STATE] = 15;
            SetAllPlayersBerryData();
        }
        break;
    case 2:
        if ((GetBlockReceivedStatus() & 3) == 3)
        {
            u8 taskId;

            ResetBlockReceivedFlags();
            FindLinkBattleMaster(2, playerMultiplayerId);
            SetAllPlayersBerryData();
            taskId = CreateTask(InitLinkBattleVsScreen, 0);
            gTasks[taskId].data[1] = 0x10E;
            gTasks[taskId].data[2] = 0x5A;
            gTasks[taskId].data[5] = 0;
            gTasks[taskId].data[3] = gBattleStruct->multiBuffer.linkBattlerHeader.vsScreenHealthFlagsLo | (gBattleStruct->multiBuffer.linkBattlerHeader.vsScreenHealthFlagsHi << 8);
            gTasks[taskId].data[4] = gBlockRecvBuffer[enemyMultiplayerId][1];
            RecordedBattle_SetFrontierPassFlagFromHword(gBlockRecvBuffer[playerMultiplayerId][1]);
            RecordedBattle_SetFrontierPassFlagFromHword(gBlockRecvBuffer[enemyMultiplayerId][1]);
            SetDeoxysStats();
            gBattleCommunication[MULTIUSE_STATE]++;
        }
        break;
    case 3:
        if (IsLinkTaskFinished())
        {
            SendBlock(bitmask_all_link_players_but_self(), gPlayerParty, sizeof(struct Pokemon) * 2);
            gBattleCommunication[MULTIUSE_STATE]++;
        }
        break;
    case 4:
        if ((GetBlockReceivedStatus() & 3) == 3)
        {
            ResetBlockReceivedFlags();
            memcpy(gEnemyParty, gBlockRecvBuffer[enemyMultiplayerId], sizeof(struct Pokemon) * 2);
            gBattleCommunication[MULTIUSE_STATE]++;
        }
        break;
    case 7:
        if (IsLinkTaskFinished())
        {
            SendBlock(bitmask_all_link_players_but_self(), gPlayerParty + 2, sizeof(struct Pokemon) * 2);
            gBattleCommunication[MULTIUSE_STATE]++;
        }
        break;
    case 8:
        if ((GetBlockReceivedStatus() & 3) == 3)
        {
            ResetBlockReceivedFlags();
            memcpy(gEnemyParty + 2, gBlockRecvBuffer[enemyMultiplayerId], sizeof(struct Pokemon) * 2);
            gBattleCommunication[MULTIUSE_STATE]++;
        }
        break;
    case 11:
        if (IsLinkTaskFinished())
        {
            SendBlock(bitmask_all_link_players_but_self(), gPlayerParty + 4, sizeof(struct Pokemon) * 2);
            gBattleCommunication[MULTIUSE_STATE]++;
        }
        break;
    case 12:
        if ((GetBlockReceivedStatus() & 3) == 3)
        {
            ResetBlockReceivedFlags();
            memcpy(gEnemyParty + 4, gBlockRecvBuffer[enemyMultiplayerId], sizeof(struct Pokemon) * 2);
            TryCorrectShedinjaLanguage(&gEnemyParty[0]);
            TryCorrectShedinjaLanguage(&gEnemyParty[1]);
            TryCorrectShedinjaLanguage(&gEnemyParty[2]);
            TryCorrectShedinjaLanguage(&gEnemyParty[3]);
            TryCorrectShedinjaLanguage(&gEnemyParty[4]);
            TryCorrectShedinjaLanguage(&gEnemyParty[5]);
            gBattleCommunication[MULTIUSE_STATE]++;
        }
        break;
    case 15:
        InitBattleControllers();
        sub_8184E58();
        gBattleCommunication[SPRITES_INIT_STATE1] = 0;
        gBattleCommunication[SPRITES_INIT_STATE2] = 0;
        if (gBattleTypeFlags & BATTLE_TYPE_LINK)
        {
            s32 i;

            for (i = 0; i < 2 && (gLinkPlayers[i].version & 0xFF) == VERSION_EMERALD; i++);

            if (i == 2)
                gBattleCommunication[MULTIUSE_STATE] = 16;
            else
                gBattleCommunication[MULTIUSE_STATE] = 18;
        }
        else
        {
            gBattleCommunication[MULTIUSE_STATE] = 18;
        }
        break;
    case 16:
        if (IsLinkTaskFinished())
        {
            SendBlock(bitmask_all_link_players_but_self(), &gRecordedBattleRngSeed, sizeof(gRecordedBattleRngSeed));
            gBattleCommunication[MULTIUSE_STATE]++;
        }
        break;
    case 17:
        if ((GetBlockReceivedStatus() & 3) == 3)
        {
            ResetBlockReceivedFlags();
            if (!(gBattleTypeFlags & BATTLE_TYPE_IS_MASTER))
                memcpy(&gRecordedBattleRngSeed, gBlockRecvBuffer[enemyMultiplayerId], sizeof(gRecordedBattleRngSeed));
            gBattleCommunication[MULTIUSE_STATE]++;
        }
        break;
    case 18:
        if (BattleInitAllSprites(&gBattleCommunication[SPRITES_INIT_STATE1], &gBattleCommunication[SPRITES_INIT_STATE2]))
        {
            gPreBattleCallback1 = gMain.callback1;
            gMain.callback1 = BattleMainCB1;
            SetMainCallback2(BattleMainCB2);
            if (gBattleTypeFlags & BATTLE_TYPE_LINK)
            {
                gBattleTypeFlags |= BATTLE_TYPE_LINK_IN_BATTLE;
            }
        }
        break;
    case 5:
    case 9:
    case 13:
        gBattleCommunication[MULTIUSE_STATE]++;
        gBattleCommunication[1] = 1;
    case 6:
    case 10:
    case 14:
        if (--gBattleCommunication[1] == 0)
            gBattleCommunication[MULTIUSE_STATE]++;
        break;
    }
}

static void CB2_HandleStartMultiPartnerBattle(void)
{
    u8 playerMultiplayerId;
    u8 enemyMultiplayerId;

    RunTasks();
    AnimateSprites();
    BuildOamBuffer();

    playerMultiplayerId = GetMultiplayerId();
    gBattleScripting.multiplayerId = playerMultiplayerId;
    enemyMultiplayerId = playerMultiplayerId ^ BIT_SIDE;

    switch (gBattleCommunication[MULTIUSE_STATE])
    {
    case 0:
        if (!IsDma3ManagerBusyWithBgCopy())
        {
            ShowBg(0);
            ShowBg(1);
            ShowBg(2);
            ShowBg(3);
            sub_805EF14();
            gBattleCommunication[MULTIUSE_STATE] = 1;
        }
        if (gWirelessCommType)
            LoadWirelessStatusIndicatorSpriteGfx();
        // fall through
    case 1:
        if (gBattleTypeFlags & BATTLE_TYPE_LINK)
        {
            if (gReceivedRemoteLinkPlayers != 0)
            {
                u8 language;

                gLinkPlayers[0].id = 0;
                gLinkPlayers[1].id = 2;
                gLinkPlayers[2].id = 1;
                gLinkPlayers[3].id = 3;
                GetFrontierTrainerName(gLinkPlayers[2].name, gTrainerBattleOpponent_A);
                GetFrontierTrainerName(gLinkPlayers[3].name, gTrainerBattleOpponent_B);
                GetBattleTowerTrainerLanguage(&language, gTrainerBattleOpponent_A);
                gLinkPlayers[2].language = language;
                GetBattleTowerTrainerLanguage(&language, gTrainerBattleOpponent_B);
                gLinkPlayers[3].language = language;

                if (IsLinkTaskFinished())
                {
                    // 0x300
                    *(&gBattleStruct->multiBuffer.linkBattlerHeader.versionSignatureLo) = 0;
                    *(&gBattleStruct->multiBuffer.linkBattlerHeader.versionSignatureHi) = 3;
                    BufferPartyVsScreenHealth_AtStart();
                    SetPlayerBerryDataInBattleStruct();
                    SendBlock(bitmask_all_link_players_but_self(), &gBattleStruct->multiBuffer.linkBattlerHeader, sizeof(gBattleStruct->multiBuffer.linkBattlerHeader));
                    gBattleCommunication[MULTIUSE_STATE] = 2;
                }

                if (gWirelessCommType)
                    CreateWirelessStatusIndicatorSprite(0, 0);
            }
        }
        else
        {
            if (!(gBattleTypeFlags & BATTLE_TYPE_RECORDED))
                gBattleTypeFlags |= BATTLE_TYPE_IS_MASTER;
            gBattleCommunication[MULTIUSE_STATE] = 13;
            SetAllPlayersBerryData();
        }
        break;
    case 2:
        if ((GetBlockReceivedStatus() & 3) == 3)
        {
            u8 taskId;

            ResetBlockReceivedFlags();
            FindLinkBattleMaster(2, playerMultiplayerId);
            SetAllPlayersBerryData();
            taskId = CreateTask(InitLinkBattleVsScreen, 0);
            gTasks[taskId].data[1] = 0x10E;
            gTasks[taskId].data[2] = 0x5A;
            gTasks[taskId].data[5] = 0;
            gTasks[taskId].data[3] = 0x145;
            gTasks[taskId].data[4] = 0x145;
            gBattleCommunication[MULTIUSE_STATE]++;
        }
        break;
    case 3:
        if (IsLinkTaskFinished())
        {
            SendBlock(bitmask_all_link_players_but_self(), gPlayerParty, sizeof(struct Pokemon) * 2);
            gBattleCommunication[MULTIUSE_STATE]++;
        }
        break;
    case 4:
        if ((GetBlockReceivedStatus() & 3) == 3)
        {
            ResetBlockReceivedFlags();
            if (gLinkPlayers[playerMultiplayerId].id != 0)
            {
                memcpy(gPlayerParty, gBlockRecvBuffer[enemyMultiplayerId], sizeof(struct Pokemon) * 2);
                memcpy(gPlayerParty + MULTI_PARTY_SIZE, gBlockRecvBuffer[playerMultiplayerId], sizeof(struct Pokemon) * 2);
            }
            else
            {
                memcpy(gPlayerParty, gBlockRecvBuffer[playerMultiplayerId], sizeof(struct Pokemon) * 2);
                memcpy(gPlayerParty + MULTI_PARTY_SIZE, gBlockRecvBuffer[enemyMultiplayerId], sizeof(struct Pokemon) * 2);
            }
            gBattleCommunication[MULTIUSE_STATE]++;
        }
        break;
    case 5:
        if (IsLinkTaskFinished())
        {
            SendBlock(bitmask_all_link_players_but_self(), gPlayerParty + 2, sizeof(struct Pokemon));
            gBattleCommunication[MULTIUSE_STATE]++;
        }
        break;
    case 6:
        if ((GetBlockReceivedStatus() & 3) == 3)
        {
            ResetBlockReceivedFlags();
            if (gLinkPlayers[playerMultiplayerId].id != 0)
            {
                memcpy(gPlayerParty + 2, gBlockRecvBuffer[enemyMultiplayerId], sizeof(struct Pokemon));
                memcpy(gPlayerParty + 5, gBlockRecvBuffer[playerMultiplayerId], sizeof(struct Pokemon));
            }
            else
            {
                memcpy(gPlayerParty + 2, gBlockRecvBuffer[playerMultiplayerId], sizeof(struct Pokemon));
                memcpy(gPlayerParty + 5, gBlockRecvBuffer[enemyMultiplayerId], sizeof(struct Pokemon));
            }
            gBattleCommunication[MULTIUSE_STATE]++;
        }
        break;
    case 7:
        if (IsLinkTaskFinished())
        {
            SendBlock(bitmask_all_link_players_but_self(), gEnemyParty, sizeof(struct Pokemon) * 2);
            gBattleCommunication[MULTIUSE_STATE]++;
        }
        break;
    case 8:
        if ((GetBlockReceivedStatus() & 3) == 3)
        {
            ResetBlockReceivedFlags();
            if (GetMultiplayerId() != 0)
            {
                memcpy(gEnemyParty, gBlockRecvBuffer[0], sizeof(struct Pokemon) * 2);
            }
            gBattleCommunication[MULTIUSE_STATE]++;
        }
        break;
    case 9:
        if (IsLinkTaskFinished())
        {
            SendBlock(bitmask_all_link_players_but_self(), gEnemyParty + 2, sizeof(struct Pokemon) * 2);
            gBattleCommunication[MULTIUSE_STATE]++;
        }
        break;
    case 10:
        if ((GetBlockReceivedStatus() & 3) == 3)
        {
            ResetBlockReceivedFlags();
            if (GetMultiplayerId() != 0)
            {
                memcpy(gEnemyParty + 2, gBlockRecvBuffer[0], sizeof(struct Pokemon) * 2);
            }
            gBattleCommunication[MULTIUSE_STATE]++;
        }
        break;
    case 11:
        if (IsLinkTaskFinished())
        {
            SendBlock(bitmask_all_link_players_but_self(), gEnemyParty + 4, sizeof(struct Pokemon) * 2);
            gBattleCommunication[MULTIUSE_STATE]++;
        }
        break;
    case 12:
        if ((GetBlockReceivedStatus() & 3) == 3)
        {
            ResetBlockReceivedFlags();
            if (GetMultiplayerId() != 0)
                memcpy(gEnemyParty + 4, gBlockRecvBuffer[0], sizeof(struct Pokemon) * 2);
            TryCorrectShedinjaLanguage(&gPlayerParty[0]);
            TryCorrectShedinjaLanguage(&gPlayerParty[1]);
            TryCorrectShedinjaLanguage(&gPlayerParty[2]);
            TryCorrectShedinjaLanguage(&gPlayerParty[3]);
            TryCorrectShedinjaLanguage(&gPlayerParty[4]);
            TryCorrectShedinjaLanguage(&gPlayerParty[5]);
            TryCorrectShedinjaLanguage(&gEnemyParty[0]);
            TryCorrectShedinjaLanguage(&gEnemyParty[1]);
            TryCorrectShedinjaLanguage(&gEnemyParty[2]);
            TryCorrectShedinjaLanguage(&gEnemyParty[3]);
            TryCorrectShedinjaLanguage(&gEnemyParty[4]);
            TryCorrectShedinjaLanguage(&gEnemyParty[5]);
            gBattleCommunication[MULTIUSE_STATE]++;
        }
        break;
    case 13:
        InitBattleControllers();
        sub_8184E58();
        gBattleCommunication[SPRITES_INIT_STATE1] = 0;
        gBattleCommunication[SPRITES_INIT_STATE2] = 0;
        if (gBattleTypeFlags & BATTLE_TYPE_LINK)
        {
            gBattleCommunication[MULTIUSE_STATE] = 14;
        }
        else
        {
            gBattleCommunication[MULTIUSE_STATE] = 16;
        }
        break;
    case 14:
        if (IsLinkTaskFinished())
        {
            SendBlock(bitmask_all_link_players_but_self(), &gRecordedBattleRngSeed, sizeof(gRecordedBattleRngSeed));
            gBattleCommunication[MULTIUSE_STATE]++;
        }
        break;
    case 15:
        if ((GetBlockReceivedStatus() & 3) == 3)
        {
            ResetBlockReceivedFlags();
            if (!(gBattleTypeFlags & BATTLE_TYPE_IS_MASTER))
                memcpy(&gRecordedBattleRngSeed, gBlockRecvBuffer[enemyMultiplayerId], sizeof(gRecordedBattleRngSeed));
            gBattleCommunication[MULTIUSE_STATE]++;
        }
        break;
    case 16:
        if (BattleInitAllSprites(&gBattleCommunication[SPRITES_INIT_STATE1], &gBattleCommunication[SPRITES_INIT_STATE2]))
        {
            TrySetLinkBattleTowerEnemyPartyLevel();
            gPreBattleCallback1 = gMain.callback1;
            gMain.callback1 = BattleMainCB1;
            SetMainCallback2(BattleMainCB2);
            if (gBattleTypeFlags & BATTLE_TYPE_LINK)
            {
                gBattleTypeFlags |= BATTLE_TYPE_LINK_IN_BATTLE;
            }
        }
        break;
    }
}

static void sub_80379F8(u8 arrayIdPlus)
{
    s32 i;

    for (i = 0; i < (int)ARRAY_COUNT(gMultiPartnerParty); i++)
    {
        gMultiPartnerParty[i].species     = GetMonData(&gPlayerParty[arrayIdPlus + i], MON_DATA_SPECIES);
        gMultiPartnerParty[i].heldItem    = GetMonData(&gPlayerParty[arrayIdPlus + i], MON_DATA_HELD_ITEM);
        GetMonData(&gPlayerParty[arrayIdPlus + i], MON_DATA_NICKNAME, gMultiPartnerParty[i].nickname);
        gMultiPartnerParty[i].level       = GetMonData(&gPlayerParty[arrayIdPlus + i], MON_DATA_LEVEL);
        gMultiPartnerParty[i].hp          = GetMonData(&gPlayerParty[arrayIdPlus + i], MON_DATA_HP);
        gMultiPartnerParty[i].maxhp       = GetMonData(&gPlayerParty[arrayIdPlus + i], MON_DATA_MAX_HP);
        gMultiPartnerParty[i].status      = GetMonData(&gPlayerParty[arrayIdPlus + i], MON_DATA_STATUS);
        gMultiPartnerParty[i].personality = GetMonData(&gPlayerParty[arrayIdPlus + i], MON_DATA_PERSONALITY);
        gMultiPartnerParty[i].gender      = GetMonGender(&gPlayerParty[arrayIdPlus + i]);
        StripExtCtrlCodes(gMultiPartnerParty[i].nickname);
        if (GetMonData(&gPlayerParty[arrayIdPlus + i], MON_DATA_LANGUAGE) != LANGUAGE_JAPANESE)
            PadNameString(gMultiPartnerParty[i].nickname, CHAR_SPACE);
    }
    memcpy(sMultiPartnerPartyBuffer, gMultiPartnerParty, sizeof(gMultiPartnerParty));
}

static void CB2_PreInitMultiBattle(void)
{
    s32 i;
    u8 playerMultiplierId;
    s32 numPlayers = 4;
    u8 r4 = 0xF;
    u32 *savedBattleTypeFlags;
    void (**savedCallback)(void);

    if (gBattleTypeFlags & BATTLE_TYPE_BATTLE_TOWER)
    {
        numPlayers = 2;
        r4 = 3;
    }

    playerMultiplierId = GetMultiplayerId();
    gBattleScripting.multiplayerId = playerMultiplierId;
    savedCallback = &gBattleStruct->savedCallback;
    savedBattleTypeFlags = &gBattleStruct->savedBattleTypeFlags;

    RunTasks();
    AnimateSprites();
    BuildOamBuffer();

    switch (gBattleCommunication[MULTIUSE_STATE])
    {
    case 0:
        if (gReceivedRemoteLinkPlayers != 0 && IsLinkTaskFinished())
        {
            sMultiPartnerPartyBuffer = Alloc(sizeof(struct UnknownPokemonStruct4) * ARRAY_COUNT(gMultiPartnerParty));
            sub_80379F8(0);
            SendBlock(bitmask_all_link_players_but_self(), sMultiPartnerPartyBuffer, sizeof(struct UnknownPokemonStruct4) * ARRAY_COUNT(gMultiPartnerParty));
            gBattleCommunication[MULTIUSE_STATE]++;
        }
        break;
    case 1:
        if ((GetBlockReceivedStatus() & r4) == r4)
        {
            ResetBlockReceivedFlags();
            for (i = 0; i < numPlayers; i++)
            {
                if (i == playerMultiplierId)
                    continue;

                if (numPlayers == MAX_LINK_PLAYERS)
                {
                    if ((!(gLinkPlayers[i].id & 1) && !(gLinkPlayers[playerMultiplierId].id & 1))
                        || (gLinkPlayers[i].id & 1 && gLinkPlayers[playerMultiplierId].id & 1))
                    {
                        memcpy(gMultiPartnerParty, gBlockRecvBuffer[i], sizeof(struct UnknownPokemonStruct4) * ARRAY_COUNT(gMultiPartnerParty));
                    }
                }
                else
                {
                    memcpy(gMultiPartnerParty, gBlockRecvBuffer[i], sizeof(struct UnknownPokemonStruct4) * ARRAY_COUNT(gMultiPartnerParty));
                }
            }
            gBattleCommunication[MULTIUSE_STATE]++;
            *savedCallback = gMain.savedCallback;
            *savedBattleTypeFlags = gBattleTypeFlags;
            gMain.savedCallback = CB2_PreInitMultiBattle;
            ShowPartyMenuToShowcaseMultiBattleParty();
        }
        break;
    case 2:
        if (IsLinkTaskFinished() && !gPaletteFade.active)
        {
            gBattleCommunication[MULTIUSE_STATE]++;
            if (gWirelessCommType)
                SetLinkStandbyCallback();
            else
                SetCloseLinkCallback();
        }
        break;
    case 3:
        if (gWirelessCommType)
        {
            if (IsLinkRfuTaskFinished())
            {
                gBattleTypeFlags = *savedBattleTypeFlags;
                gMain.savedCallback = *savedCallback;
                SetMainCallback2(CB2_InitBattleInternal);
                Free(sMultiPartnerPartyBuffer);
                sMultiPartnerPartyBuffer = NULL;
            }
        }
        else if (gReceivedRemoteLinkPlayers == 0)
        {
            gBattleTypeFlags = *savedBattleTypeFlags;
            gMain.savedCallback = *savedCallback;
            SetMainCallback2(CB2_InitBattleInternal);
            Free(sMultiPartnerPartyBuffer);
            sMultiPartnerPartyBuffer = NULL;
        }
        break;
    }
}

static void CB2_PreInitIngamePlayerPartnerBattle(void)
{
    u32 *savedBattleTypeFlags;
    void (**savedCallback)(void);

    savedCallback = &gBattleStruct->savedCallback;
    savedBattleTypeFlags = &gBattleStruct->savedBattleTypeFlags;

    RunTasks();
    AnimateSprites();
    BuildOamBuffer();

    switch (gBattleCommunication[MULTIUSE_STATE])
    {
    case 0:
        sMultiPartnerPartyBuffer = Alloc(sizeof(struct UnknownPokemonStruct4) * ARRAY_COUNT(gMultiPartnerParty));
        sub_80379F8(3);
        gBattleCommunication[MULTIUSE_STATE]++;
        *savedCallback = gMain.savedCallback;
        *savedBattleTypeFlags = gBattleTypeFlags;
        gMain.savedCallback = CB2_PreInitIngamePlayerPartnerBattle;
        ShowPartyMenuToShowcaseMultiBattleParty();
        break;
    case 1:
        if (!gPaletteFade.active)
        {
            gBattleCommunication[MULTIUSE_STATE] = 2;
            gBattleTypeFlags = *savedBattleTypeFlags;
            gMain.savedCallback = *savedCallback;
            SetMainCallback2(CB2_InitBattleInternal);
            Free(sMultiPartnerPartyBuffer);
            sMultiPartnerPartyBuffer = NULL;
        }
        break;
    }
}

static void CB2_HandleStartMultiBattle(void)
{
    u8 playerMultiplayerId;
    s32 id;
    u8 var;

    playerMultiplayerId = GetMultiplayerId();
    gBattleScripting.multiplayerId = playerMultiplayerId;

    RunTasks();
    AnimateSprites();
    BuildOamBuffer();

    switch (gBattleCommunication[MULTIUSE_STATE])
    {
    case 0:
        if (!IsDma3ManagerBusyWithBgCopy())
        {
            ShowBg(0);
            ShowBg(1);
            ShowBg(2);
            ShowBg(3);
            sub_805EF14();
            gBattleCommunication[MULTIUSE_STATE] = 1;
        }
        if (gWirelessCommType)
            LoadWirelessStatusIndicatorSpriteGfx();
        break;
    case 1:
        if (gBattleTypeFlags & BATTLE_TYPE_LINK)
        {
            if (gReceivedRemoteLinkPlayers != 0)
            {
                if (IsLinkTaskFinished())
                {
                    // 0x300
                    *(&gBattleStruct->multiBuffer.linkBattlerHeader.versionSignatureLo) = 0;
                    *(&gBattleStruct->multiBuffer.linkBattlerHeader.versionSignatureHi) = 3;
                    BufferPartyVsScreenHealth_AtStart();
                    SetPlayerBerryDataInBattleStruct();

                    SendBlock(bitmask_all_link_players_but_self(), &gBattleStruct->multiBuffer.linkBattlerHeader, sizeof(gBattleStruct->multiBuffer.linkBattlerHeader));
                    gBattleCommunication[MULTIUSE_STATE]++;
                }
                if (gWirelessCommType)
                    CreateWirelessStatusIndicatorSprite(0, 0);
            }
        }
        else
        {
            if (!(gBattleTypeFlags & BATTLE_TYPE_RECORDED))
                gBattleTypeFlags |= BATTLE_TYPE_IS_MASTER;
            gBattleCommunication[MULTIUSE_STATE] = 7;
            SetAllPlayersBerryData();
        }
        break;
    case 2:
        if ((GetBlockReceivedStatus() & 0xF) == 0xF)
        {
            ResetBlockReceivedFlags();
            FindLinkBattleMaster(4, playerMultiplayerId);
            SetAllPlayersBerryData();
            SetDeoxysStats();
            var = CreateTask(InitLinkBattleVsScreen, 0);
            gTasks[var].data[1] = 0x10E;
            gTasks[var].data[2] = 0x5A;
            gTasks[var].data[5] = 0;
            gTasks[var].data[3] = 0;
            gTasks[var].data[4] = 0;

            for (id = 0; id < MAX_LINK_PLAYERS; id++)
            {
                RecordedBattle_SetFrontierPassFlagFromHword(gBlockRecvBuffer[id][1]);
                switch (gLinkPlayers[id].id)
                {
                case 0:
                    gTasks[var].data[3] |= gBlockRecvBuffer[id][1] & 0x3F;
                    break;
                case 1:
                    gTasks[var].data[4] |= gBlockRecvBuffer[id][1] & 0x3F;
                    break;
                case 2:
                    gTasks[var].data[3] |= (gBlockRecvBuffer[id][1] & 0x3F) << 6;
                    break;
                case 3:
                    gTasks[var].data[4] |= (gBlockRecvBuffer[id][1] & 0x3F) << 6;
                    break;
                }
            }
            ZeroEnemyPartyMons();
            gBattleCommunication[MULTIUSE_STATE]++;
        }
        else
            break;
        // fall through
    case 3:
        if (IsLinkTaskFinished())
        {
            SendBlock(bitmask_all_link_players_but_self(), gPlayerParty, sizeof(struct Pokemon) * 2);
            gBattleCommunication[MULTIUSE_STATE]++;
        }
        break;
    case 4:
        if ((GetBlockReceivedStatus() & 0xF) == 0xF)
        {
            ResetBlockReceivedFlags();
            for (id = 0; id < MAX_LINK_PLAYERS; id++)
            {
                if (id == playerMultiplayerId)
                {
                    switch (gLinkPlayers[id].id)
                    {
                    case 0:
                    case 3:
                        memcpy(gPlayerParty, gBlockRecvBuffer[id], sizeof(struct Pokemon) * 2);
                        break;
                    case 1:
                    case 2:
                        memcpy(gPlayerParty + MULTI_PARTY_SIZE, gBlockRecvBuffer[id], sizeof(struct Pokemon) * 2);
                        break;
                    }
                }
                else
                {
                    if ((!(gLinkPlayers[id].id & 1) && !(gLinkPlayers[playerMultiplayerId].id & 1))
                     || ((gLinkPlayers[id].id & 1) && (gLinkPlayers[playerMultiplayerId].id & 1)))
                    {
                        switch (gLinkPlayers[id].id)
                        {
                        case 0:
                        case 3:
                            memcpy(gPlayerParty, gBlockRecvBuffer[id], sizeof(struct Pokemon) * 2);
                            break;
                        case 1:
                        case 2:
                            memcpy(gPlayerParty + MULTI_PARTY_SIZE, gBlockRecvBuffer[id], sizeof(struct Pokemon) * 2);
                            break;
                        }
                    }
                    else
                    {
                        switch (gLinkPlayers[id].id)
                        {
                        case 0:
                        case 3:
                            memcpy(gEnemyParty, gBlockRecvBuffer[id], sizeof(struct Pokemon) * 2);
                            break;
                        case 1:
                        case 2:
                            memcpy(gEnemyParty + MULTI_PARTY_SIZE, gBlockRecvBuffer[id], sizeof(struct Pokemon) * 2);
                            break;
                        }
                    }
                }
            }
            gBattleCommunication[MULTIUSE_STATE]++;
        }
        break;
    case 5:
        if (IsLinkTaskFinished())
        {
            SendBlock(bitmask_all_link_players_but_self(), gPlayerParty + 2, sizeof(struct Pokemon));
            gBattleCommunication[MULTIUSE_STATE]++;
        }
        break;
    case 6:
        if ((GetBlockReceivedStatus() & 0xF) == 0xF)
        {
            ResetBlockReceivedFlags();
            for (id = 0; id < MAX_LINK_PLAYERS; id++)
            {
                if (id == playerMultiplayerId)
                {
                    switch (gLinkPlayers[id].id)
                    {
                    case 0:
                    case 3:
                        memcpy(gPlayerParty + 2, gBlockRecvBuffer[id], sizeof(struct Pokemon));
                        break;
                    case 1:
                    case 2:
                        memcpy(gPlayerParty + 5, gBlockRecvBuffer[id], sizeof(struct Pokemon));
                        break;
                    }
                }
                else
                {
                    if ((!(gLinkPlayers[id].id & 1) && !(gLinkPlayers[playerMultiplayerId].id & 1))
                     || ((gLinkPlayers[id].id & 1) && (gLinkPlayers[playerMultiplayerId].id & 1)))
                    {
                        switch (gLinkPlayers[id].id)
                        {
                        case 0:
                        case 3:
                            memcpy(gPlayerParty + 2, gBlockRecvBuffer[id], sizeof(struct Pokemon));
                            break;
                        case 1:
                        case 2:
                            memcpy(gPlayerParty + 5, gBlockRecvBuffer[id], sizeof(struct Pokemon));
                            break;
                        }
                    }
                    else
                    {
                        switch (gLinkPlayers[id].id)
                        {
                        case 0:
                        case 3:
                            memcpy(gEnemyParty + 2, gBlockRecvBuffer[id], sizeof(struct Pokemon));
                            break;
                        case 1:
                        case 2:
                            memcpy(gEnemyParty + 5, gBlockRecvBuffer[id], sizeof(struct Pokemon));
                            break;
                        }
                    }
                }
            }
            TryCorrectShedinjaLanguage(&gPlayerParty[0]);
            TryCorrectShedinjaLanguage(&gPlayerParty[1]);
            TryCorrectShedinjaLanguage(&gPlayerParty[2]);
            TryCorrectShedinjaLanguage(&gPlayerParty[3]);
            TryCorrectShedinjaLanguage(&gPlayerParty[4]);
            TryCorrectShedinjaLanguage(&gPlayerParty[5]);

            TryCorrectShedinjaLanguage(&gEnemyParty[0]);
            TryCorrectShedinjaLanguage(&gEnemyParty[1]);
            TryCorrectShedinjaLanguage(&gEnemyParty[2]);
            TryCorrectShedinjaLanguage(&gEnemyParty[3]);
            TryCorrectShedinjaLanguage(&gEnemyParty[4]);
            TryCorrectShedinjaLanguage(&gEnemyParty[5]);

            gBattleCommunication[MULTIUSE_STATE]++;
        }
        break;
    case 7:
        InitBattleControllers();
        sub_8184E58();
        gBattleCommunication[SPRITES_INIT_STATE1] = 0;
        gBattleCommunication[SPRITES_INIT_STATE2] = 0;
        if (gBattleTypeFlags & BATTLE_TYPE_LINK)
        {
            for (id = 0; id < MAX_LINK_PLAYERS && (gLinkPlayers[id].version & 0xFF) == VERSION_EMERALD; id++);

            if (id == MAX_LINK_PLAYERS)
                gBattleCommunication[MULTIUSE_STATE] = 8;
            else
                gBattleCommunication[MULTIUSE_STATE] = 10;
        }
        else
        {
            gBattleCommunication[MULTIUSE_STATE] = 10;
        }
        break;
    case 8:
        if (IsLinkTaskFinished())
        {
            u32* ptr = gBattleStruct->multiBuffer.battleVideo;
            ptr[0] = gBattleTypeFlags;
            ptr[1] = gRecordedBattleRngSeed; // UB: overwrites berry data
            SendBlock(bitmask_all_link_players_but_self(), ptr, sizeof(gBattleStruct->multiBuffer.battleVideo));
            gBattleCommunication[MULTIUSE_STATE]++;
        }
        break;
    case 9:
        if ((GetBlockReceivedStatus() & 0xF) == 0xF)
        {
            ResetBlockReceivedFlags();
            for (var = 0; var < 4; var++)
            {
                u32 blockValue = gBlockRecvBuffer[var][0];
                if (blockValue & 4)
                {
                    memcpy(&gRecordedBattleRngSeed, &gBlockRecvBuffer[var][2], sizeof(gRecordedBattleRngSeed));
                    break;
                }
            }

            gBattleCommunication[MULTIUSE_STATE]++;
        }
        break;
    case 10:
        if (BattleInitAllSprites(&gBattleCommunication[SPRITES_INIT_STATE1], &gBattleCommunication[SPRITES_INIT_STATE2]))
        {
            gPreBattleCallback1 = gMain.callback1;
            gMain.callback1 = BattleMainCB1;
            SetMainCallback2(BattleMainCB2);
            if (gBattleTypeFlags & BATTLE_TYPE_LINK)
            {
                gTrainerBattleOpponent_A = TRAINER_LINK_OPPONENT;
                gBattleTypeFlags |= BATTLE_TYPE_LINK_IN_BATTLE;
            }
        }
        break;
    }
}

void BattleMainCB2(void)
{
    AnimateSprites();
    BuildOamBuffer();
    RunTextPrinters();
    UpdatePaletteFade();
    RunTasks();

    if (JOY_HELD(B_BUTTON) && gBattleTypeFlags & BATTLE_TYPE_RECORDED && sub_8186450())
    {
        gSpecialVar_Result = gBattleOutcome = B_OUTCOME_PLAYER_TELEPORTED;
        ResetPaletteFadeControl();
        BeginNormalPaletteFade(PALETTES_ALL, 0, 0, 0x10, RGB_BLACK);
        SetMainCallback2(CB2_QuitRecordedBattle);
    }
}

static void FreeRestoreBattleData(void)
{
    gMain.callback1 = gPreBattleCallback1;
    gScanlineEffect.state = 3;
    gMain.inBattle = 0;
    ZeroEnemyPartyMons();
    m4aSongNumStop(SE_LOW_HEALTH);
    FreeMonSpritesGfx();
    FreeBattleSpritesData();
    FreeBattleResources();
}

void CB2_QuitRecordedBattle(void)
{
    UpdatePaletteFade();
    if (!gPaletteFade.active)
    {
        m4aMPlayStop(&gMPlayInfo_SE1);
        m4aMPlayStop(&gMPlayInfo_SE2);
        FreeRestoreBattleData();
        FreeAllWindowBuffers();
        SetMainCallback2(gMain.savedCallback);
    }
}

void sub_8038528(struct Sprite* sprite)
{
    sprite->data[0] = 0;
    sprite->callback = sub_8038538;
}

static void sub_8038538(struct Sprite *sprite)
{
    u16 *arr = (u16*)(gDecompressionBuffer);

    switch (sprite->data[0])
    {
    case 0:
        sprite->data[0]++;
        sprite->data[1] = 0;
        sprite->data[2] = 0x281;
        sprite->data[3] = 0;
        sprite->data[4] = 1;
        // fall through
    case 1:
        sprite->data[4]--;
        if (sprite->data[4] == 0)
        {
            s32 i;
            s32 r2;
            s32 r0;

            sprite->data[4] = 2;
            r2 = sprite->data[1] + sprite->data[3] * 32;
            r0 = sprite->data[2] - sprite->data[3] * 32;
            for (i = 0; i < 29; i += 2)
            {
                arr[r2 + i] = 0x3D;
                arr[r0 + i] = 0x3D;
            }
            sprite->data[3]++;
            if (sprite->data[3] == 21)
            {
                sprite->data[0]++;
                sprite->data[1] = 32;
            }
        }
        break;
    case 2:
        sprite->data[1]--;
        if (sprite->data[1] == 20)
            SetMainCallback2(CB2_InitBattle);
        break;
    }
}

static u32 Crc32B (const u8 *data, u32 size)
{
   s32 i, j;
   u32 byte, crc, mask;

   i = 0;
   crc = 0xFFFFFFFF;
   for (i = 0; i < size; ++i)
   {
        byte = data[i];
        crc = crc ^ byte;
        for (j = 7; j >= 0; --j)
        {
            mask = -(crc & 1);
            crc = (crc >> 1) ^ (0xEDB88320 & mask);
        }
   }
   return ~crc;
}

static u32 GeneratePartyHash(const struct Trainer *trainer, u32 i)
{
    const u8 *buffer = (const u8 *) &trainer->party[i];
    u32 n = sizeof(*trainer->party);
    return Crc32B(buffer, n);
}

void ModifyPersonalityForNature(u32 *personality, u32 newNature)
{
    u32 nature = GetNatureFromPersonality(*personality);
    s32 diff = abs((s32)nature - (s32)newNature);
    s32 sign = (nature > newNature) ? 1 : -1;
    if (diff > NUM_NATURES / 2)
    {
        diff = NUM_NATURES - diff;
        sign *= -1;
    }
    *personality -= (diff * sign);
}

u32 GeneratePersonalityForGender(u32 gender, u32 species)
{
    const struct SpeciesInfo *speciesInfo = &gSpeciesInfo[species];
    if (gender == MON_MALE)
        return ((255 - speciesInfo->genderRatio) / 2) + speciesInfo->genderRatio;
    else
        return speciesInfo->genderRatio / 2;
}

void CustomTrainerPartyAssignMoves(struct Pokemon *mon, const struct TrainerMon *partyEntry)
static u8 CreateNPCTrainerParty(struct Pokemon *party, u16 trainerNum, bool8 firstTrainer)
{
    u32 nameHash = 0;
    u32 personalityValue;
    u8 fixedIV;
    u8 level;
    s32 i, j;
    u16 ev;
    u8 monsCount;
    if (battleTypeFlags & BATTLE_TYPE_TRAINER && !(battleTypeFlags & (BATTLE_TYPE_FRONTIER
    u8 nickname[POKEMON_NAME_LENGTH + 1];
    u8 trainerName[(PLAYER_NAME_LENGTH * 3) + 1];
    u8 ability, gender, friendship;

    if (trainerNum == TRAINER_SECRET_BASE)
        return 0;

    if (gBattleTypeFlags & BATTLE_TYPE_TRAINER && !(gBattleTypeFlags & (BATTLE_TYPE_FRONTIER
                                                                        | BATTLE_TYPE_EREADER_TRAINER
                                                                        | BATTLE_TYPE_TRAINER_HILL)))
    {
        if (firstTrainer == TRUE)
            ZeroEnemyPartyMons();

        if (gBattleTypeFlags & BATTLE_TYPE_TWO_OPPONENTS)
        {
            if (gTrainers[trainerNum].partySize > 3)
                monsCount = 3;
            else
                monsCount = gTrainers[trainerNum].partySize;
        }
        else
        {
            monsCount = gTrainers[trainerNum].partySize;
        }

        for (i = 0; i < monsCount; i++)
        {
            s32 ball = -1;
            u32 personalityHash = GeneratePartyHash(trainer, i);
            const struct TrainerMon *partyData = trainer->party;
            u32 otIdType = OT_ID_RANDOM_NO_SHINY;
            u32 fixedOtId = 0;

            if (trainer->doubleBattle == TRUE)
            const struct TrainerMon *partyData = gTrainers[trainerNum].party.TrainerMon;
            
            fixedIV = partyData[i].iv;

// Comment out the following line if you have changed .iv to go 0-31, instead of 0-255 as in vanilla.
            fixedIV = fixedIV * MAX_PER_STAT_IVS / 255;

            fixedIV = fixedIV + TRAINER_IV_MODIFIER;

            gender = MON_MALE; // defaults to 0

            if (gTrainers[trainerNum].doubleBattle == TRUE)
                personalityValue = 0x80;
            else if (gTrainers[trainerNum].encounterMusic_gender & 0x80)
                personalityValue = 0x78;
            else
                personalityValue = 0x88; // Use personality more likely to result in a male Pokémon

            personalityValue += personalityHash << 8;
            if (partyData[i].gender == TRAINER_MON_MALE)
                personalityValue = (personalityValue & 0xFFFFFF00) | GeneratePersonalityForGender(MON_MALE, partyData[i].species);
            else if (partyData[i].gender == TRAINER_MON_FEMALE)
                personalityValue = (personalityValue & 0xFFFFFF00) | GeneratePersonalityForGender(MON_FEMALE, partyData[i].species);
            if (partyData[i].nature != 0)
                ModifyPersonalityForNature(&personalityValue, partyData[i].nature - 1);
            if (partyData[i].isShiny)
            {
                otIdType = OT_ID_PRESET;
                fixedOtId = HIHALF(personalityValue) ^ LOHALF(personalityValue);
            }
            CreateMon(&party[i], partyData[i].species, partyData[i].lvl, 0, TRUE, personalityValue, otIdType, fixedOtId);
            SetMonData(&party[i], MON_DATA_HELD_ITEM, &partyData[i].heldItem);

            CustomTrainerPartyAssignMoves(&party[i], &partyData[i]);
            SetMonData(&party[i], MON_DATA_IVS, &(partyData[i].iv));
            if (partyData[i].ev != NULL)
            {
                SetMonData(&party[i], MON_DATA_HP_EV, &(partyData[i].ev[0]));
                SetMonData(&party[i], MON_DATA_ATK_EV, &(partyData[i].ev[1]));
                SetMonData(&party[i], MON_DATA_DEF_EV, &(partyData[i].ev[2]));
                SetMonData(&party[i], MON_DATA_SPATK_EV, &(partyData[i].ev[3]));
                SetMonData(&party[i], MON_DATA_SPDEF_EV, &(partyData[i].ev[4]));
                SetMonData(&party[i], MON_DATA_SPEED_EV, &(partyData[i].ev[5]));
            }
            if (partyData[i].ability != ABILITY_NONE)
            {
                const struct SpeciesInfo *speciesInfo = &gSpeciesInfo[partyData[i].species];
                u32 maxAbilities = ARRAY_COUNT(speciesInfo->abilities);
                for (j = 0; j < maxAbilities; ++j)
                {
                    if (speciesInfo->abilities[j] == partyData[i].ability)
                        break;
                }
                if (j < maxAbilities)
                    SetMonData(&party[i], MON_DATA_ABILITY_NUM, &j);
            }
            SetMonData(&party[i], MON_DATA_FRIENDSHIP, &(partyData[i].friendship));
            if (partyData[i].ball != ITEM_NONE)
            {
                ball = partyData[i].ball;
                SetMonData(&party[i], MON_DATA_POKEBALL, &ball);
            }
            if (partyData[i].nickname != NULL)
            {
                SetMonData(&party[i], MON_DATA_NICKNAME, partyData[i].nickname);
            }
            CalculateMonStats(&party[i]);

        #if B_TRAINER_CLASS_POKE_BALLS >= GEN_7
            if (ball == -1)
            {
                ball = (sTrainerBallTable[trainer->trainerClass]) ? sTrainerBallTable[trainer->trainerClass] : ITEM_POKE_BALL;
                SetMonData(&party[i], MON_DATA_POKEBALL, &ball);
            }
        #endif
        }
                personalityValue = 0x88;

            for (j = 0; gTrainers[trainerNum].trainerName[j] != EOS; j++)
                nameHash += gTrainers[trainerNum].trainerName[j];

            if (gTrainers[trainerNum].doubleBattle == TRUE)
                personalityValue = 0x80;
            else if (gTrainers[trainerNum].encounterMusic_gender & 0x80)
            {
                personalityValue = 0x78;
                gender = MON_MALE;
            }
            else
            {
                 personalityValue = 0x88;
                 gender = MON_FEMALE;
            }

            if (partyData[i].gender == TRAINER_MON_MALE)
                gender = MON_MALE;
            else if (partyData[i].gender == TRAINER_MON_FEMALE)
                gender = MON_FEMALE;

            if (partyData[i].nature > 0)
                CreateMonWithGenderNatureLetter(&party[i], partyData[i].species, partyData[i].lvl, fixedIV, gender, partyData[i].nature, 0, partyData[i].shiny ? OT_ID_SHINY : OT_ID_RANDOM_NO_SHINY);
            else
            {
                CreateMon(&party[i], partyData[i].species, partyData[i].lvl, fixedIV, TRUE, personalityValue, partyData[i].shiny ? OT_ID_SHINY : OT_ID_RANDOM_NO_SHINY, 0);
            }

            if (partyData[i].friendship > 0)
            {
                if (partyData[i].friendship == TRAINER_MON_UNFRIENDLY)
                    friendship = 0;
                else if (partyData[i].friendship == TRAINER_MON_FRIENDLY)
                    friendship = MAX_FRIENDSHIP;
                SetMonData(&party[i], MON_DATA_FRIENDSHIP, &friendship);
            }

            if (partyData[i].nickname[0] != '\0')
                SetMonData(&party[i], MON_DATA_NICKNAME, &partyData[i].nickname);

            if (partyData[i].ability > 0)
            {
                ability = partyData[i].ability;

                if (partyData[i].ability == ABILITY_SLOT_1)
                    ability = 0;

                SetMonData(&party[i], MON_DATA_ABILITY_NUM, &ability);
            }

            if (partyData[i].ball > 0)
                SetMonData(&party[i], MON_DATA_POKEBALL, &partyData[i].ball);

            if (partyData[i].heldItem > 0)
                SetMonData(&party[i], MON_DATA_HELD_ITEM, &partyData[i].heldItem);

            if (partyData[i].moves[0] != '\0')
            {
                for (j = 0; j < MAX_MON_MOVES; j++)
                {
                    SetMonData(&party[i], MON_DATA_MOVE1 + j, &partyData[i].moves[j]);
                    SetMonData(&party[i], MON_DATA_MOVE1 + j, &partyData[i].moves[j]);
                    SetMonData(&party[i], MON_DATA_PP1 + j, &gBattleMoves[partyData[i].moves[j]].pp);
                    SetMonData(&party[i], MON_DATA_PP1 + j, &gBattleMoves[partyData[i].moves[j]].pp);
                }
            }

            if (partyData[i].iv > 0)
            {
                for (j = 0; j < NUM_STATS; j++)
                {
                    SetMonData(&party[i], MON_DATA_HP_IV + j, &fixedIV);
                }
            }
            else if (partyData[i].iv == WORST_IVS)
            {
                fixedIV = 0;

                for (j = 0; j < NUM_STATS; j++)
                {
                    SetMonData(&party[i], MON_DATA_HP_IV + j, &fixedIV);
                }
            }
            else
            {
                for (j = 0; j < NUM_STATS; j++)
                {
                    SetMonData(&party[i], MON_DATA_HP_IV + j, &partyData[i].ivs[j]);
                }
            }

            for (j = 0; j < NUM_STATS; j++)
            {
                SetMonData(&party[i], MON_DATA_HP_EV + j, &partyData[i].evs[j]);
            }

            StringCopy(trainerName, gTrainers[trainerNum].trainerName);
            SetMonData(&party[i], MON_DATA_OT_NAME, trainerName);
            CalculateMonStats(&party[i]);
        }

        gBattleTypeFlags |= gTrainers[trainerNum].doubleBattle;
    }

    return gTrainers[trainerNum].partySize;
}

void sub_8038A04(void) // unused
{
    if (REG_VCOUNT < 0xA0 && REG_VCOUNT >= 0x6F)
        SetGpuReg(REG_OFFSET_BG0CNT, BGCNT_SCREENBASE(24) | BGCNT_TXT256x512);
}

void CreateTrainerPartyForPlayer(void)
{
    ZeroPlayerPartyMons();
    gPartnerTrainerId = gSpecialVar_0x8004;
    CreateNPCTrainerPartyFromTrainer(gPlayerParty, &gTrainers[gSpecialVar_0x8004], TRUE, BATTLE_TYPE_TRAINER);
}

void VBlankCB_Battle(void)
{
    // Change gRngSeed every vblank unless the battle could be recorded.
    if (!(gBattleTypeFlags & (BATTLE_TYPE_LINK | BATTLE_TYPE_FRONTIER | BATTLE_TYPE_RECORDED)))
        Random();

    SetGpuReg(REG_OFFSET_BG0HOFS, gBattle_BG0_X);
    SetGpuReg(REG_OFFSET_BG0VOFS, gBattle_BG0_Y);
    SetGpuReg(REG_OFFSET_BG1HOFS, gBattle_BG1_X);
    SetGpuReg(REG_OFFSET_BG1VOFS, gBattle_BG1_Y);
    SetGpuReg(REG_OFFSET_BG2HOFS, gBattle_BG2_X);
    SetGpuReg(REG_OFFSET_BG2VOFS, gBattle_BG2_Y);
    SetGpuReg(REG_OFFSET_BG3HOFS, gBattle_BG3_X);
    SetGpuReg(REG_OFFSET_BG3VOFS, gBattle_BG3_Y);
    SetGpuReg(REG_OFFSET_WIN0H, gBattle_WIN0H);
    SetGpuReg(REG_OFFSET_WIN0V, gBattle_WIN0V);
    SetGpuReg(REG_OFFSET_WIN1H, gBattle_WIN1H);
    SetGpuReg(REG_OFFSET_WIN1V, gBattle_WIN1V);
    LoadOam();
    ProcessSpriteCopyRequests();
    TransferPlttBuffer();
    ScanlineEffect_InitHBlankDmaTransfer();
}

void SpriteCB_VsLetterDummy(struct Sprite *sprite)
{

}

static void SpriteCB_VsLetter(struct Sprite *sprite)
{
    if (sprite->data[0] != 0)
        sprite->pos1.x = sprite->data[1] + ((sprite->data[2] & 0xFF00) >> 8);
    else
        sprite->pos1.x = sprite->data[1] - ((sprite->data[2] & 0xFF00) >> 8);

    sprite->data[2] += 0x180;

    if (sprite->affineAnimEnded)
    {
        FreeSpriteTilesByTag(ANIM_SPRITES_START);
        FreeSpritePaletteByTag(ANIM_SPRITES_START);
        FreeSpriteOamMatrix(sprite);
        DestroySprite(sprite);
    }
}

void SpriteCB_VsLetterInit(struct Sprite *sprite)
{
    StartSpriteAffineAnim(sprite, 1);
    sprite->callback = SpriteCB_VsLetter;
    PlaySE(SE_MUGSHOT);
}

static void BufferPartyVsScreenHealth_AtEnd(u8 taskId)
{
    struct Pokemon *party1 = NULL;
    struct Pokemon *party2 = NULL;
    u8 multiplayerId = gBattleScripting.multiplayerId;
    u32 flags;
    s32 i;

    if (gBattleTypeFlags & BATTLE_TYPE_MULTI)
    {
        switch (gLinkPlayers[multiplayerId].id)
        {
        case 0:
        case 2:
            party1 = gPlayerParty;
            party2 = gEnemyParty;
            break;
        case 1:
        case 3:
            party1 = gEnemyParty;
            party2 = gPlayerParty;
            break;
        }
    }
    else
    {
        party1 = gPlayerParty;
        party2 = gEnemyParty;
    }

    flags = 0;
    BUFFER_PARTY_VS_SCREEN_STATUS(party1, flags, i);
    gTasks[taskId].data[3] = flags;

    flags = 0;
    BUFFER_PARTY_VS_SCREEN_STATUS(party2, flags, i);
    gTasks[taskId].data[4] = flags;
}

void CB2_InitEndLinkBattle(void)
{
    s32 i;
    u8 taskId;

    SetHBlankCallback(NULL);
    SetVBlankCallback(NULL);
    gBattleTypeFlags &= ~(BATTLE_TYPE_LINK_IN_BATTLE);

    if (gBattleTypeFlags & BATTLE_TYPE_FRONTIER)
    {
        SetMainCallback2(gMain.savedCallback);
        FreeBattleResources();
        FreeBattleSpritesData();
        FreeMonSpritesGfx();
    }
    else
    {
        CpuFill32(0, (void*)(VRAM), VRAM_SIZE);
        SetGpuReg(REG_OFFSET_MOSAIC, 0);
        SetGpuReg(REG_OFFSET_WIN0H, DISPLAY_WIDTH);
        SetGpuReg(REG_OFFSET_WIN0V, WIN_RANGE(DISPLAY_HEIGHT / 2, DISPLAY_HEIGHT / 2 + 1));
        SetGpuReg(REG_OFFSET_WININ, 0);
        SetGpuReg(REG_OFFSET_WINOUT, 0);
        gBattle_WIN0H = DISPLAY_WIDTH;
        gBattle_WIN0V = WIN_RANGE(DISPLAY_HEIGHT / 2, DISPLAY_HEIGHT / 2 + 1);
        ScanlineEffect_Clear();

        i = 0;
        while (i < 80)
        {
            gScanlineEffectRegBuffers[0][i] = 0xF0;
            gScanlineEffectRegBuffers[1][i] = 0xF0;
            i++;
        }

        while (i < 160)
        {
            gScanlineEffectRegBuffers[0][i] = 0xFF10;
            gScanlineEffectRegBuffers[1][i] = 0xFF10;
            i++;
        }

        ResetPaletteFade();

        gBattle_BG0_X = 0;
        gBattle_BG0_Y = 0;
        gBattle_BG1_X = 0;
        gBattle_BG1_Y = 0;
        gBattle_BG2_X = 0;
        gBattle_BG2_Y = 0;
        gBattle_BG3_X = 0;
        gBattle_BG3_Y = 0;

        InitBattleBgsVideo();
        LoadCompressedPalette(gBattleTextboxPalette, 0, 64);
        LoadBattleMenuWindowGfx();
        ResetSpriteData();
        ResetTasks();
        DrawBattleEntryBackground();
        SetGpuReg(REG_OFFSET_WINOUT, WINOUT_WIN01_BG0 | WINOUT_WIN01_BG1 | WINOUT_WIN01_BG2 | WINOUT_WIN01_OBJ | WINOUT_WIN01_CLR);
        FreeAllSpritePalettes();
        gReservedSpritePaletteCount = 4;
        SetVBlankCallback(VBlankCB_Battle);

        // Show end Vs screen with battle results
        taskId = CreateTask(InitLinkBattleVsScreen, 0);
        gTasks[taskId].data[1] = 0x10E;
        gTasks[taskId].data[2] = 0x5A;
        gTasks[taskId].data[5] = 1;
        BufferPartyVsScreenHealth_AtEnd(taskId);

        SetMainCallback2(CB2_EndLinkBattle);
        gBattleCommunication[MULTIUSE_STATE] = 0;
    }
}

static void CB2_EndLinkBattle(void)
{
    EndLinkBattleInSteps();
    AnimateSprites();
    BuildOamBuffer();
    RunTextPrinters();
    UpdatePaletteFade();
    RunTasks();
}

static void EndLinkBattleInSteps(void)
{
    s32 i;

    switch (gBattleCommunication[MULTIUSE_STATE])
    {
    case 0:
        ShowBg(0);
        ShowBg(1);
        ShowBg(2);
        gBattleCommunication[1] = 0xFF;
        gBattleCommunication[MULTIUSE_STATE]++;
        break;
    case 1:
        if (--gBattleCommunication[1] == 0)
        {
            BeginNormalPaletteFade(PALETTES_ALL, 0, 0, 0x10, RGB_BLACK);
            gBattleCommunication[MULTIUSE_STATE]++;
        }
        break;
    case 2:
        if (!gPaletteFade.active)
        {
            u32 battlerCount;
            u8 monsCount;

            gMain.anyLinkBattlerHasFrontierPass = RecordedBattle_GetFrontierPassFlag();

            if (gBattleTypeFlags & BATTLE_TYPE_MULTI)
                monsCount = 4;
            else
                monsCount = 2;

            for (i = 0; i < monsCount && (gLinkPlayers[i].version & 0xFF) == VERSION_EMERALD; i++);

            if (!gSaveBlock2Ptr->frontier.disableRecordBattle && i == monsCount)
            {
                if (FlagGet(FLAG_SYS_FRONTIER_PASS))
                {
                    FreeAllWindowBuffers();
                    SetMainCallback2(sub_80392A8);
                }
                else if (!gMain.anyLinkBattlerHasFrontierPass)
                {
                    SetMainCallback2(gMain.savedCallback);
                    FreeBattleResources();
                    FreeBattleSpritesData();
                    FreeMonSpritesGfx();
                }
                else if (gReceivedRemoteLinkPlayers == 0)
                {
                    CreateTask(Task_ReconnectWithLinkPlayers, 5);
                    gBattleCommunication[MULTIUSE_STATE]++;
                }
                else
                {
                    gBattleCommunication[MULTIUSE_STATE]++;
                }
            }
            else
            {
                SetMainCallback2(gMain.savedCallback);
                FreeBattleResources();
                FreeBattleSpritesData();
                FreeMonSpritesGfx();
            }
        }
        break;
    case 3:
        CpuFill32(0, (void*)(VRAM), VRAM_SIZE);

        for (i = 0; i < 2; i++)
            LoadChosenBattleElement(i);

        BeginNormalPaletteFade(PALETTES_ALL, 0, 0x10, 0, RGB_BLACK);
        gBattleCommunication[MULTIUSE_STATE]++;
        break;
    case 4:
        if (!gPaletteFade.active)
            gBattleCommunication[MULTIUSE_STATE]++;
        break;
    case 5:
        if (!FuncIsActiveTask(Task_ReconnectWithLinkPlayers))
            gBattleCommunication[MULTIUSE_STATE]++;
        break;
    case 6:
        if (IsLinkTaskFinished() == TRUE)
        {
            SetLinkStandbyCallback();
            BattlePutTextOnWindow(gText_LinkStandby3, 0);
            gBattleCommunication[MULTIUSE_STATE]++;
        }
        break;
    case 7:
        if (!IsTextPrinterActive(0))
        {
            if (IsLinkTaskFinished() == TRUE)
                gBattleCommunication[MULTIUSE_STATE]++;
        }
        break;
    case 8:
        if (!gWirelessCommType)
            SetCloseLinkCallback();
        gBattleCommunication[MULTIUSE_STATE]++;
        break;
    case 9:
        if (!gMain.anyLinkBattlerHasFrontierPass || gWirelessCommType || gReceivedRemoteLinkPlayers != 1)
        {
            gMain.anyLinkBattlerHasFrontierPass = 0;
            SetMainCallback2(gMain.savedCallback);
            FreeBattleResources();
            FreeBattleSpritesData();
            FreeMonSpritesGfx();
        }
        break;
    }
}

u32 GetBattleBgTemplateData(u8 arrayId, u8 caseId)
{
    u32 ret = 0;

    switch (caseId)
    {
    case 0:
        ret = gBattleBgTemplates[arrayId].bg;
        break;
    case 1:
        ret = gBattleBgTemplates[arrayId].charBaseIndex;
        break;
    case 2:
        ret = gBattleBgTemplates[arrayId].mapBaseIndex;
        break;
    case 3:
        ret = gBattleBgTemplates[arrayId].screenSize;
        break;
    case 4:
        ret = gBattleBgTemplates[arrayId].paletteMode;
        break;
    case 5: // Only this case is used
        ret = gBattleBgTemplates[arrayId].priority;
        break;
    case 6:
        ret = gBattleBgTemplates[arrayId].baseTile;
        break;
    }

    return ret;
}

static void sub_80392A8(void)
{
    s32 i;

    SetHBlankCallback(NULL);
    SetVBlankCallback(NULL);
    CpuFill32(0, (void*)(VRAM), VRAM_SIZE);
    ResetPaletteFade();
    gBattle_BG0_X = 0;
    gBattle_BG0_Y = 0;
    gBattle_BG1_X = 0;
    gBattle_BG1_Y = 0;
    gBattle_BG2_X = 0;
    gBattle_BG2_Y = 0;
    gBattle_BG3_X = 0;
    gBattle_BG3_Y = 0;
    InitBattleBgsVideo();
    SetGpuReg(REG_OFFSET_DISPCNT, DISPCNT_OBJ_ON | DISPCNT_OBJ_1D_MAP);
    LoadBattleMenuWindowGfx();

    for (i = 0; i < 2; i++)
        LoadChosenBattleElement(i);

    ResetSpriteData();
    ResetTasks();
    FreeAllSpritePalettes();
    gReservedSpritePaletteCount = 4;
    SetVBlankCallback(VBlankCB_Battle);
    SetMainCallback2(sub_803937C);
    BeginNormalPaletteFade(PALETTES_ALL, 0, 0x10, 0, RGB_BLACK);
    gBattleCommunication[MULTIUSE_STATE] = 0;
}

static void sub_803937C(void)
{
    sub_803939C();
    AnimateSprites();
    BuildOamBuffer();
    RunTextPrinters();
    UpdatePaletteFade();
    RunTasks();
}

static void sub_803939C(void)
{
    switch (gBattleCommunication[MULTIUSE_STATE])
    {
    case 0:
        ShowBg(0);
        ShowBg(1);
        ShowBg(2);
        gBattleCommunication[MULTIUSE_STATE]++;
        break;
    case 1:
        if (gMain.anyLinkBattlerHasFrontierPass && gReceivedRemoteLinkPlayers == 0)
            CreateTask(Task_ReconnectWithLinkPlayers, 5);
        gBattleCommunication[MULTIUSE_STATE]++;
        break;
    case 2:
        if (!FuncIsActiveTask(Task_ReconnectWithLinkPlayers))
            gBattleCommunication[MULTIUSE_STATE]++;
        break;
    case 3:
        if (!gPaletteFade.active)
        {
            BattlePutTextOnWindow(gText_RecordBattleToPass, 0);
            gBattleCommunication[MULTIUSE_STATE]++;
        }
        break;
    case 4:
        if (!IsTextPrinterActive(0))
        {
            HandleBattleWindow(0x18, 8, 0x1D, 0xD, 0);
            BattlePutTextOnWindow(gText_BattleYesNoChoice, 0xC);
            gBattleCommunication[CURSOR_POSITION] = 1;
            BattleCreateYesNoCursorAt(1);
            gBattleCommunication[MULTIUSE_STATE]++;
        }
        break;
    case 5:
        if (JOY_NEW(DPAD_UP))
        {
            if (gBattleCommunication[CURSOR_POSITION] != 0)
            {
                PlaySE(SE_SELECT);
                BattleDestroyYesNoCursorAt(gBattleCommunication[CURSOR_POSITION]);
                gBattleCommunication[CURSOR_POSITION] = 0;
                BattleCreateYesNoCursorAt(0);
            }
        }
        else if (JOY_NEW(DPAD_DOWN))
        {
            if (gBattleCommunication[CURSOR_POSITION] == 0)
            {
                PlaySE(SE_SELECT);
                BattleDestroyYesNoCursorAt(gBattleCommunication[CURSOR_POSITION]);
                gBattleCommunication[CURSOR_POSITION] = 1;
                BattleCreateYesNoCursorAt(1);
            }
        }
        else if (JOY_NEW(A_BUTTON))
        {
            PlaySE(SE_SELECT);
            if (gBattleCommunication[CURSOR_POSITION] == 0)
            {
                HandleBattleWindow(0x18, 8, 0x1D, 0xD, WINDOW_CLEAR);
                gBattleCommunication[1] = MoveRecordedBattleToSaveData();
                gBattleCommunication[MULTIUSE_STATE] = 10;
            }
            else
            {
                gBattleCommunication[MULTIUSE_STATE]++;
            }
        }
        else if (JOY_NEW(B_BUTTON))
        {
            PlaySE(SE_SELECT);
            gBattleCommunication[MULTIUSE_STATE]++;
        }
        break;
    case 6:
        if (IsLinkTaskFinished() == TRUE)
        {
            HandleBattleWindow(0x18, 8, 0x1D, 0xD, WINDOW_CLEAR);
            if (gMain.anyLinkBattlerHasFrontierPass)
            {
                SetLinkStandbyCallback();
                BattlePutTextOnWindow(gText_LinkStandby3, 0);
            }
            gBattleCommunication[MULTIUSE_STATE]++;
        }
        break;
    case 8:
        if (--gBattleCommunication[1] == 0)
        {
            if (gMain.anyLinkBattlerHasFrontierPass && !gWirelessCommType)
                SetCloseLinkCallback();
            gBattleCommunication[MULTIUSE_STATE]++;
        }
        break;
    case 9:
        if (!gMain.anyLinkBattlerHasFrontierPass || gWirelessCommType || gReceivedRemoteLinkPlayers != 1)
        {
            gMain.anyLinkBattlerHasFrontierPass = 0;
            if (!gPaletteFade.active)
            {
                SetMainCallback2(gMain.savedCallback);
                FreeBattleResources();
                FreeBattleSpritesData();
                FreeMonSpritesGfx();
            }
        }
        break;
    case 10:
        if (gBattleCommunication[1] == 1)
        {
            PlaySE(SE_SAVE);
            BattleStringExpandPlaceholdersToDisplayedString(gText_BattleRecordedOnPass);
            BattlePutTextOnWindow(gDisplayedStringBattle, 0);
            gBattleCommunication[1] = 0x80;
            gBattleCommunication[MULTIUSE_STATE]++;
        }
        else
        {
            BattleStringExpandPlaceholdersToDisplayedString(BattleFrontier_BattleTowerBattleRoom_Text_RecordCouldntBeSaved);
            BattlePutTextOnWindow(gDisplayedStringBattle, 0);
            gBattleCommunication[1] = 0x80;
            gBattleCommunication[MULTIUSE_STATE]++;
        }
        break;
    case 11:
        if (IsLinkTaskFinished() == TRUE && !IsTextPrinterActive(0) && --gBattleCommunication[1] == 0)
        {
            if (gMain.anyLinkBattlerHasFrontierPass)
            {
                SetLinkStandbyCallback();
                BattlePutTextOnWindow(gText_LinkStandby3, 0);
            }
            gBattleCommunication[MULTIUSE_STATE]++;
        }
        break;
    case 12:
    case 7:
        if (!IsTextPrinterActive(0))
        {
            if (gMain.anyLinkBattlerHasFrontierPass)
            {
                if (IsLinkTaskFinished() == TRUE)
                {
                    BeginNormalPaletteFade(PALETTES_ALL, 0, 0, 0x10, RGB_BLACK);
                    gBattleCommunication[1] = 0x20;
                    gBattleCommunication[MULTIUSE_STATE] = 8;
                }

            }
            else
            {
                BeginNormalPaletteFade(PALETTES_ALL, 0, 0, 0x10, RGB_BLACK);
                gBattleCommunication[1] = 0x20;
                gBattleCommunication[MULTIUSE_STATE] = 8;
            }
        }
        break;
    }
}

static void TryCorrectShedinjaLanguage(struct Pokemon *mon)
{
    u8 nickname[POKEMON_NAME_LENGTH + 1];
    u8 language = LANGUAGE_JAPANESE;

    if (GetMonData(mon, MON_DATA_SPECIES) == SPECIES_SHEDINJA
     && GetMonData(mon, MON_DATA_LANGUAGE) != language)
    {
        GetMonData(mon, MON_DATA_NICKNAME, nickname);
        if (StringCompareWithoutExtCtrlCodes(nickname, sText_ShedinjaJpnName) == 0)
            SetMonData(mon, MON_DATA_LANGUAGE, &language);
    }
}

u32 GetBattleWindowTemplatePixelWidth(u32 setId, u32 tableId)
{
    return gBattleWindowTemplates[setId][tableId].width * 8;
}

#define sBattler            data[0]
#define sSpeciesId          data[2]

void SpriteCb_WildMon(struct Sprite *sprite)
{
    sprite->callback = SpriteCb_MoveWildMonToRight;
    StartSpriteAnimIfDifferent(sprite, 0);
    BeginNormalPaletteFade(0x20000, 0, 10, 10, RGB(8, 8, 8));
}

static void SpriteCb_MoveWildMonToRight(struct Sprite *sprite)
{
    if ((gIntroSlideFlags & 1) == 0)
    {
        sprite->pos2.x += 2;
        if (sprite->pos2.x == 0)
        {
            sprite->callback = SpriteCb_WildMonShowHealthbox;
        }
    }
}

static void SpriteCb_WildMonShowHealthbox(struct Sprite *sprite)
{
    if (sprite->animEnded)
    {
        StartHealthboxSlideIn(sprite->sBattler);
        SetHealthboxSpriteVisible(gHealthboxSpriteIds[sprite->sBattler]);
        sprite->callback = SpriteCb_WildMonAnimate;
        StartSpriteAnimIfDifferent(sprite, 0);
        BeginNormalPaletteFade(0x20000, 0, 10, 0, RGB(8, 8, 8));
    }
}

static void SpriteCb_WildMonAnimate(struct Sprite *sprite)
{
    if (!gPaletteFade.active)
    {
        BattleAnimateFrontSprite(sprite, sprite->sSpeciesId, FALSE, 1);
    }
}

void SpriteCallbackDummy_2(struct Sprite *sprite)
{

}

// Unused
static void sub_80398BC(struct Sprite *sprite)
{
    sprite->data[3] = 6;
    sprite->data[4] = 1;
    sprite->callback = sub_80398D0;
}

static void sub_80398D0(struct Sprite *sprite)
{
    sprite->data[4]--;
    if (sprite->data[4] == 0)
    {
        sprite->data[4] = 8;
        sprite->invisible ^= 1;
        sprite->data[3]--;
        if (sprite->data[3] == 0)
        {
            sprite->invisible = FALSE;
            sprite->callback = SpriteCallbackDummy_2;
            sUnusedUnknownArray[0] = 0;
        }
    }
}

extern const struct MonCoords gMonFrontPicCoords[];

void SpriteCB_FaintOpponentMon(struct Sprite *sprite)
{
    u8 battler = sprite->sBattler;
    u16 species;
    u8 yOffset;

    if (gBattleSpritesDataPtr->battlerData[battler].transformSpecies != 0)
        species = gBattleSpritesDataPtr->battlerData[battler].transformSpecies;
    else
        species = sprite->sSpeciesId;

    if (species == SPECIES_UNOWN)
    {
        u32 personalityValue = GetMonData(&gEnemyParty[gBattlerPartyIndexes[battler]], MON_DATA_PERSONALITY);
        u16 unownForm = GET_UNOWN_LETTER(personalityValue);
        u16 unownSpecies;

        if (unownForm == 0)
            unownSpecies = SPECIES_UNOWN;  // Use the A Unown form.
        else
            unownSpecies = NUM_SPECIES + unownForm;  // Use one of the other Unown letters.

        yOffset = gMonFrontPicCoords[unownSpecies].y_offset;
    }
    else if (species == SPECIES_CASTFORM)
    {
        yOffset = gCastformFrontSpriteCoords[gBattleMonForms[battler]].y_offset;
    }
    else if (species > NUM_SPECIES)
    {
        yOffset = gMonFrontPicCoords[SPECIES_NONE].y_offset;
    }
    else
    {
        yOffset = gMonFrontPicCoords[species].y_offset;
    }

    sprite->data[3] = 8 - yOffset / 8;
    sprite->data[4] = 1;
    sprite->callback = SpriteCB_AnimFaintOpponent;
}

static void SpriteCB_AnimFaintOpponent(struct Sprite *sprite)
{
    s32 i;

    if (--sprite->data[4] == 0)
    {
        sprite->data[4] = 2;
        sprite->pos2.y += 8; // Move the sprite down.
        if (--sprite->data[3] < 0)
        {
            FreeSpriteOamMatrix(sprite);
            DestroySprite(sprite);
        }
        else // Erase bottom part of the sprite to create a smooth illusion of mon falling down.
        {
            u8 *dst = gMonSpritesGfxPtr->sprites.byte[GetBattlerPosition(sprite->sBattler)] + (sprite->data[3] << 8);
            u8* dst = gMonSpritesGfxPtr->sprites.byte[GetBattlerPosition(sprite->sBattler)] + (gBattleMonForms[sprite->sBattler] << 11) + (sprite->data[3] << 8);

            for (i = 0; i < 0x100; i++)
                *(dst++) = 0;

            StartSpriteAnim(sprite, 0);
        }
    }
}

// Used when selecting a move, which can hit multiple targets, in double battles.
void SpriteCb_ShowAsMoveTarget(struct Sprite *sprite)
{
    sprite->data[3] = 8;
    sprite->data[4] = sprite->invisible;
    sprite->callback = SpriteCb_BlinkVisible;
}

static void SpriteCb_BlinkVisible(struct Sprite *sprite)
{
    if (--sprite->data[3] == 0)
    {
        sprite->invisible ^= 1;
        sprite->data[3] = 8;
    }
}

void SpriteCb_HideAsMoveTarget(struct Sprite *sprite)
{
    sprite->invisible = sprite->data[4];
    sprite->data[4] = FALSE;
    sprite->callback = SpriteCallbackDummy_2;
}

void SpriteCb_OpponentMonFromBall(struct Sprite *sprite)
{
    if (sprite->affineAnimEnded)
    {
        if (!(gHitMarker & HITMARKER_NO_ANIMATIONS) || gBattleTypeFlags & (BATTLE_TYPE_LINK | BATTLE_TYPE_RECORDED_LINK))
        {
            if (HasTwoFramesAnimation(sprite->sSpeciesId))
                StartSpriteAnim(sprite, 1);
        }
        BattleAnimateFrontSprite(sprite, sprite->sSpeciesId, TRUE, 1);
    }
}

// This callback is frequently overwritten by SpriteCB_TrainerSlideIn
void SpriteCB_BattleSpriteStartSlideLeft(struct Sprite *sprite)
{
    sprite->callback = SpriteCB_BattleSpriteSlideLeft;
}

static void SpriteCB_BattleSpriteSlideLeft(struct Sprite *sprite)
{
    if (!(gIntroSlideFlags & 1))
    {
        sprite->pos2.x -= 2;
        if (sprite->pos2.x == 0)
        {
            sprite->callback = SpriteCallbackDummy_3;
            sprite->data[1] = 0;
        }
    }
}

static void UNUSED SetIdleSpriteCallback(struct Sprite *sprite)
// Unused
static void sub_80105DC(struct Sprite *sprite)
{
    sprite->callback = SpriteCallbackDummy_3;
}

static void SpriteCallbackDummy_3(struct Sprite *sprite)
{
}

#define sSpeedX data[1]
#define sSpeedY data[2]

void SpriteCB_FaintSlideAnim(struct Sprite *sprite)
{
    if (!(gIntroSlideFlags & 1))
    {
        sprite->pos2.x += sprite->sSpeedX;
        sprite->pos2.y += sprite->sSpeedY;
    }
}

#undef sSpeedX
#undef sSpeedY

#define sSinIndex           data[0]
#define sDelta              data[1]
#define sAmplitude          data[2]
#define sBouncerSpriteId    data[3]
#define sWhich              data[4]

void DoBounceEffect(u8 battler, u8 which, s8 delta, s8 amplitude)
{
    u8 invisibleSpriteId;
    u8 bouncerSpriteId;

    switch (which)
    {
    case BOUNCE_HEALTHBOX:
    default:
        if (gBattleSpritesDataPtr->healthBoxesData[battler].healthboxIsBouncing)
            return;
        break;
    case BOUNCE_MON:
        if (gBattleSpritesDataPtr->healthBoxesData[battler].battlerIsBouncing)
            return;
        break;
    }

    invisibleSpriteId = CreateInvisibleSpriteWithCallback(SpriteCB_BounceEffect);
    if (which == BOUNCE_HEALTHBOX)
    {
        bouncerSpriteId = gHealthboxSpriteIds[battler];
        gBattleSpritesDataPtr->healthBoxesData[battler].healthboxBounceSpriteId = invisibleSpriteId;
        gBattleSpritesDataPtr->healthBoxesData[battler].healthboxIsBouncing = 1;
        gSprites[invisibleSpriteId].sSinIndex = 128; // 0
    }
    else
    {
        bouncerSpriteId = gBattlerSpriteIds[battler];
        gBattleSpritesDataPtr->healthBoxesData[battler].battlerBounceSpriteId = invisibleSpriteId;
        gBattleSpritesDataPtr->healthBoxesData[battler].battlerIsBouncing = 1;
        gSprites[invisibleSpriteId].sSinIndex = 192; // -1
    }
    gSprites[invisibleSpriteId].sDelta = delta;
    gSprites[invisibleSpriteId].sAmplitude = amplitude;
    gSprites[invisibleSpriteId].sBouncerSpriteId = bouncerSpriteId;
    gSprites[invisibleSpriteId].sWhich = which;
    gSprites[bouncerSpriteId].pos2.x = 0;
    gSprites[bouncerSpriteId].pos2.y = 0;
}

void EndBounceEffect(u8 battler, u8 which)
{
    u8 bouncerSpriteId;

    if (which == BOUNCE_HEALTHBOX)
    {
        if (!gBattleSpritesDataPtr->healthBoxesData[battler].healthboxIsBouncing)
            return;

        bouncerSpriteId = gSprites[gBattleSpritesDataPtr->healthBoxesData[battler].healthboxBounceSpriteId].sBouncerSpriteId;
        DestroySprite(&gSprites[gBattleSpritesDataPtr->healthBoxesData[battler].healthboxBounceSpriteId]);
        gBattleSpritesDataPtr->healthBoxesData[battler].healthboxIsBouncing = 0;
    }
    else
    {
        if (!gBattleSpritesDataPtr->healthBoxesData[battler].battlerIsBouncing)
            return;

        bouncerSpriteId = gSprites[gBattleSpritesDataPtr->healthBoxesData[battler].battlerBounceSpriteId].sBouncerSpriteId;
        DestroySprite(&gSprites[gBattleSpritesDataPtr->healthBoxesData[battler].battlerBounceSpriteId]);
        gBattleSpritesDataPtr->healthBoxesData[battler].battlerIsBouncing = 0;
    }

    gSprites[bouncerSpriteId].pos2.x = 0;
    gSprites[bouncerSpriteId].pos2.y = 0;
}

static void SpriteCB_BounceEffect(struct Sprite *sprite)
{
    u8 bouncerSpriteId = sprite->sBouncerSpriteId;
    s32 index;

    if (sprite->sWhich == BOUNCE_HEALTHBOX)
        index = sprite->sSinIndex;
    else
        index = sprite->sSinIndex;

    gSprites[bouncerSpriteId].pos2.y = Sin(index, sprite->sAmplitude) + sprite->sAmplitude;
    sprite->sSinIndex = (sprite->sSinIndex + sprite->sDelta) & 0xFF;
}

#undef sSinIndex
#undef sDelta
#undef sAmplitude
#undef sBouncerSpriteId
#undef sWhich

void SpriteCb_PlayerMonFromBall(struct Sprite *sprite)
{
    if (sprite->affineAnimEnded)
        BattleAnimateBackSprite(sprite, sprite->sSpeciesId);
}

void sub_8039E60(struct Sprite *sprite)
{
    sub_8039E9C(sprite);
    if (sprite->animEnded)
        sprite->callback = SpriteCallbackDummy_3;
}

void SpriteCB_TrainerThrowObject(struct Sprite *sprite)
{
    StartSpriteAnim(sprite, 1);
    sprite->callback = sub_8039E60;
}

void sub_8039E9C(struct Sprite *sprite)
{
    if (sprite->animDelayCounter == 0)
        sprite->centerToCornerVecX = gUnknown_0831ACE0[sprite->animCmdIndex];
}

void BeginBattleIntroDummy(void)
{

}

void BeginBattleIntro(void)
{
    BattleStartClearSetData();
    gBattleCommunication[1] = 0;
    gBattleMainFunc = BattleIntroGetMonsData;
}

static void BattleMainCB1(void)
{
    u32 battler;

    gBattleMainFunc();
    for (battler = 0; battler < gBattlersCount; battler++)
        gBattlerControllerFuncs[battler](battler);
}

static void BattleStartClearSetData(void)
{
    s32 i;
    u32 j;
    u8 *dataPtr;

    TurnValuesCleanUp(FALSE);
    SpecialStatusesClear();

    for (i = 0; i < MAX_BATTLERS_COUNT; i++)
    {
        gStatuses3[i] = 0;

        dataPtr = (u8 *)&gDisableStructs[i];
        for (j = 0; j < sizeof(struct DisableStruct); j++)
            dataPtr[j] = 0;

        gDisableStructs[i].isFirstTurn = 2;
        sUnusedBattlersArray[i] = 0;
        gLastMoves[i] = 0;
        gLastLandedMoves[i] = 0;
        gLastHitByType[i] = 0;
        gLastResultingMoves[i] = 0;
        gLastHitBy[i] = 0xFF;
        gLockedMoves[i] = 0;
        gLastPrintedMoves[i] = 0;
        gBattleResources->flags->flags[i] = 0;
        gPalaceSelectionBattleScripts[i] = 0;
    }

    for (i = 0; i < 2; i++)
    {
        gSideStatuses[i] = 0;

        dataPtr = (u8 *)&gSideTimers[i];
        for (j = 0; j < sizeof(struct SideTimer); j++)
            dataPtr[j] = 0;
    }

    gBattlerAttacker = 0;
    gBattlerTarget = 0;
    gEffectBattler = 0;
    gBattleScripting.battler = 0;
    gBattlerAbility = 0;
    gBattleWeather = 0;

    dataPtr = (u8 *)&gWishFutureKnock;
    for (i = 0; i < sizeof(struct WishFutureKnock); i++)
        dataPtr[i] = 0;

    gHitMarker = 0;

    if (!(gBattleTypeFlags & BATTLE_TYPE_RECORDED))
    {
        if (!(gBattleTypeFlags & BATTLE_TYPE_LINK) && gSaveBlock2Ptr->optionsBattleSceneOff == TRUE)
            gHitMarker |= HITMARKER_NO_ANIMATIONS;
    }
    else if (!(gBattleTypeFlags & (BATTLE_TYPE_LINK | BATTLE_TYPE_RECORDED_LINK)) && GetBattleSceneInRecordedBattle())
        gHitMarker |= HITMARKER_NO_ANIMATIONS;

    gBattleScripting.battleStyle = gSaveBlock2Ptr->optionsBattleStyle;

    gMultiHitCounter = 0;
    gBattleOutcome = 0;
    gBattleControllerExecFlags = 0;
    gPaydayMoney = 0;
    gBattleResources->battleScriptsStack->size = 0;
    gBattleResources->battleCallbackStack->size = 0;

    for (i = 0; i < BATTLE_COMMUNICATION_ENTRIES_COUNT; i++)
        gBattleCommunication[i] = 0;

    gPauseCounterBattle = 0;
    gBattleMoveDamage = 0;
    gIntroSlideFlags = 0;
    gBattleScripting.animTurn = 0;
    gBattleScripting.animTargetsHit = 0;
    gLeveledUpInBattle = 0;
    gAbsentBattlerFlags = 0;
    gBattleStruct->runTries = 0;
    gBattleStruct->safariGoNearCounter = 0;
    gBattleStruct->safariPkblThrowCounter = 0;
    *(&gBattleStruct->safariCatchFactor) = gBaseStats[GetMonData(&gEnemyParty[0], MON_DATA_SPECIES)].catchRate * 100 / 1275;
    gBattleStruct->safariEscapeFactor = 3;
    gBattleStruct->wildVictorySong = 0;
    gBattleStruct->moneyMultiplier = 1;

    for (i = 0; i < 8; i++)
    {
        *((u8 *)gBattleStruct->lastTakenMove + i) = 0;
        *((u8 *)gBattleStruct->usedHeldItems + i) = 0;
        *((u8 *)gBattleStruct->choicedMove + i) = 0;
        *((u8 *)gBattleStruct->changedItems + i) = 0;
        *(i + 0 * 8 + (u8*)(gBattleStruct->lastTakenMoveFrom) + 0) = 0;
        *(i + 1 * 8 + (u8*)(gBattleStruct->lastTakenMoveFrom) + 0) = 0;
        *(i + 2 * 8 + (u8*)(gBattleStruct->lastTakenMoveFrom) + 0) = 0;
        *(i + 3 * 8 + (u8*)(gBattleStruct->lastTakenMoveFrom) + 0) = 0;
    }

    for (i = 0; i < MAX_BATTLERS_COUNT; i++)
    {
        *(gBattleStruct->AI_monToSwitchIntoId + i) = PARTY_SIZE;
    }

    gBattleStruct->givenExpMons = 0;
    gBattleStruct->palaceFlags = 0;

    gRandomTurnNumber = Random();

    dataPtr = (u8 *)(&gBattleResults);
    for (i = 0; i < sizeof(struct BattleResults); i++)
        dataPtr[i] = 0;

    gBattleResults.shinyWildMon = IsMonShiny(&gEnemyParty[0]);

    gBattleStruct->arenaLostPlayerMons = 0;
    gBattleStruct->arenaLostOpponentMons = 0;

    gBattleStruct->mega.triggerSpriteId = 0xFF;
    gBattleStruct->burst.triggerSpriteId = 0xFF;

    for (i = 0; i < ARRAY_COUNT(gSideTimers); i++)
    {
        gSideTimers[i].stickyWebBattlerId = 0xFF;
    }
    gBattleStruct->appearedInBattle = 0;
    gBattleStruct->beatUpSlot = 0;

    for (i = 0; i < PARTY_SIZE; i++)
    {
        gBattleStruct->usedHeldItems[i][B_SIDE_PLAYER] = 0;
        gBattleStruct->usedHeldItems[i][B_SIDE_OPPONENT] = 0;
        gBattleStruct->itemLost[i].originalItem = GetMonData(&gPlayerParty[i], MON_DATA_HELD_ITEM);
        gPartyCriticalHits[i] = 0;
        gBattleStruct->allowedToChangeFormInWeather[i][B_SIDE_PLAYER] = FALSE;
        gBattleStruct->allowedToChangeFormInWeather[i][B_SIDE_OPPONENT] = FALSE;
    }

    gBattleStruct->swapDamageCategory = FALSE; // Photon Geyser, Shell Side Arm, Light That Burns the Sky
    gSelectedMonPartyId = PARTY_SIZE; // Revival Blessing
}

void SwitchInClearSetData(u32 battler)
{
    s32 i;
    struct DisableStruct disableStructCopy = gDisableStructs[battler];
    struct DisableStruct disableStructCopy = gDisableStructs[gActiveBattler];
    s32 i;
    u8 *ptr;

    ClearIllusionMon(battler);
    if (gBattleMoves[gCurrentMove].effect != EFFECT_BATON_PASS)
    {
        for (i = 0; i < NUM_BATTLE_STATS; i++)
            gBattleMons[battler].statStages[i] = DEFAULT_STAT_STAGE;
        for (i = 0; i < gBattlersCount; i++)
        {
            if ((gBattleMons[i].status2 & STATUS2_ESCAPE_PREVENTION) && gDisableStructs[i].battlerPreventingEscape == battler)
                gBattleMons[i].status2 &= ~STATUS2_ESCAPE_PREVENTION;
            if ((gStatuses3[i] & STATUS3_ALWAYS_HITS) && gDisableStructs[i].battlerWithSureHit == battler)
            {
                gStatuses3[i] &= ~STATUS3_ALWAYS_HITS;
                gDisableStructs[i].battlerWithSureHit = 0;
            }
        }
    }
    if (gBattleMoves[gCurrentMove].effect == EFFECT_BATON_PASS)
    {
        gBattleMons[battler].status2 &= (STATUS2_CONFUSION | STATUS2_FOCUS_ENERGY | STATUS2_SUBSTITUTE | STATUS2_ESCAPE_PREVENTION | STATUS2_CURSED);
        gStatuses3[battler] &= (STATUS3_LEECHSEED_BATTLER | STATUS3_LEECHSEED | STATUS3_ALWAYS_HITS | STATUS3_PERISH_SONG | STATUS3_ROOTED
                                       | STATUS3_GASTRO_ACID | STATUS3_EMBARGO | STATUS3_TELEKINESIS | STATUS3_MAGNET_RISE | STATUS3_HEAL_BLOCK
                                       | STATUS3_AQUA_RING | STATUS3_POWER_TRICK);
        gStatuses4[battler] &= (STATUS4_MUD_SPORT | STATUS4_WATER_SPORT | STATUS4_INFINITE_CONFUSION);
        gBattleMons[gActiveBattler].status2 &= (STATUS2_CONFUSION | STATUS2_FOCUS_ENERGY | STATUS2_SUBSTITUTE | STATUS2_ESCAPE_PREVENTION | STATUS2_CURSED);
        gStatuses3[gActiveBattler] &= (STATUS3_LEECHSEED_BATTLER | STATUS3_LEECHSEED | STATUS3_ALWAYS_HITS | STATUS3_PERISH_SONG | STATUS3_ROOTED | STATUS3_MUDSPORT | STATUS3_WATERSPORT);

        for (i = 0; i < gBattlersCount; i++)
        {
            if (GetBattlerSide(battler) != GetBattlerSide(i)
             && (gStatuses3[i] & STATUS3_ALWAYS_HITS) != 0
             && (gDisableStructs[i].battlerWithSureHit == battler))
            {
                gStatuses3[i] &= ~(STATUS3_ALWAYS_HITS);
                gStatuses3[i] |= STATUS3_ALWAYS_HITS_TURN(2);
            }
        }
        if (gStatuses3[battler] & STATUS3_POWER_TRICK)
            SWAP(gBattleMons[battler].attack, gBattleMons[battler].defense, i);
    }
    else
    {
        gBattleMons[battler].status2 = 0;
        gStatuses3[battler] = 0;
        gStatuses4[battler] = 0;
        gBattleMons[gActiveBattler].status2 = 0;
        gStatuses3[gActiveBattler] = 0;
    }

    for (i = 0; i < gBattlersCount; i++)
    {
        if (gBattleMons[i].status2 & STATUS2_INFATUATED_WITH(battler))
            gBattleMons[i].status2 &= ~STATUS2_INFATUATED_WITH(battler);
        if ((gBattleMons[i].status2 & STATUS2_WRAPPED) && *(gBattleStruct->wrappedBy + i) == battler)
            gBattleMons[i].status2 &= ~STATUS2_WRAPPED;
        if (gBattleMons[i].status2 & STATUS2_INFATUATED_WITH(gActiveBattler))
            gBattleMons[i].status2 &= ~(STATUS2_INFATUATED_WITH(gActiveBattler));
        if ((gBattleMons[i].status2 & STATUS2_WRAPPED) && *(gBattleStruct->wrappedBy + i) == gActiveBattler)
            gBattleMons[i].status2 &= ~(STATUS2_WRAPPED);
    }

    gActionSelectionCursor[battler] = 0;
    gMoveSelectionCursor[battler] = 0;

    memset(&gDisableStructs[battler], 0, sizeof(struct DisableStruct));
    ptr = (u8 *)&gDisableStructs[gActiveBattler];
    for (i = 0; i < sizeof(struct DisableStruct); i++)
        ptr[i] = 0;

    if (gBattleMoves[gCurrentMove].effect == EFFECT_BATON_PASS)
    {
        gDisableStructs[battler].substituteHP = disableStructCopy.substituteHP;
        gDisableStructs[battler].battlerWithSureHit = disableStructCopy.battlerWithSureHit;
        gDisableStructs[battler].perishSongTimer = disableStructCopy.perishSongTimer;
        gDisableStructs[battler].battlerPreventingEscape = disableStructCopy.battlerPreventingEscape;
        gDisableStructs[gActiveBattler].substituteHP = disableStructCopy.substituteHP;
        gDisableStructs[gActiveBattler].battlerWithSureHit = disableStructCopy.battlerWithSureHit;
        gDisableStructs[gActiveBattler].perishSongTimer = disableStructCopy.perishSongTimer;
        gDisableStructs[gActiveBattler].perishSongTimerStartValue = disableStructCopy.perishSongTimerStartValue;
        gDisableStructs[gActiveBattler].battlerPreventingEscape = disableStructCopy.battlerPreventingEscape;
    }

    gMoveResultFlags = 0;
    gDisableStructs[battler].isFirstTurn = 2;
    gDisableStructs[battler].truantSwitchInHack = disableStructCopy.truantSwitchInHack;
    gLastMoves[battler] = MOVE_NONE;
    gLastLandedMoves[battler] = MOVE_NONE;
    gLastHitByType[battler] = 0;
    gLastResultingMoves[battler] = MOVE_NONE;
    gLastPrintedMoves[battler] = MOVE_NONE;
    gLastHitBy[battler] = 0xFF;
    gDisableStructs[gActiveBattler].isFirstTurn = 2;
    gDisableStructs[gActiveBattler].truantSwitchInHack = disableStructCopy.truantSwitchInHack;
    gLastMoves[gActiveBattler] = 0;
    gLastLandedMoves[gActiveBattler] = 0;
    gLastHitByType[gActiveBattler] = 0;
    gLastResultingMoves[gActiveBattler] = 0;
    gLastPrintedMoves[gActiveBattler] = 0;
    gLastHitBy[gActiveBattler] = 0xFF;

    gBattleStruct->lastTakenMove[battler] = 0;
    gBattleStruct->sameMoveTurns[battler] = 0;
    gBattleStruct->lastTakenMoveFrom[battler][0] = 0;
    gBattleStruct->lastTakenMoveFrom[battler][1] = 0;
    gBattleStruct->lastTakenMoveFrom[battler][2] = 0;
    gBattleStruct->lastTakenMoveFrom[battler][3] = 0;
    gBattleStruct->lastMoveFailed &= ~(gBitTable[battler]);
    gBattleStruct->palaceFlags &= ~(gBitTable[battler]);
    *(gBattleStruct->lastTakenMove + gActiveBattler * 2 + 0) = 0;
    *(gBattleStruct->lastTakenMove + gActiveBattler * 2 + 1) = 0;
    *(0 * 2 + gActiveBattler * 8 + (u8*)(gBattleStruct->lastTakenMoveFrom) + 0) = 0;
    *(0 * 2 + gActiveBattler * 8 + (u8*)(gBattleStruct->lastTakenMoveFrom) + 1) = 0;
    *(1 * 2 + gActiveBattler * 8 + (u8*)(gBattleStruct->lastTakenMoveFrom) + 0) = 0;
    *(1 * 2 + gActiveBattler * 8 + (u8*)(gBattleStruct->lastTakenMoveFrom) + 1) = 0;
    *(2 * 2 + gActiveBattler * 8 + (u8*)(gBattleStruct->lastTakenMoveFrom) + 0) = 0;
    *(2 * 2 + gActiveBattler * 8 + (u8*)(gBattleStruct->lastTakenMoveFrom) + 1) = 0;
    *(3 * 2 + gActiveBattler * 8 + (u8*)(gBattleStruct->lastTakenMoveFrom) + 0) = 0;
    *(3 * 2 + gActiveBattler * 8 + (u8*)(gBattleStruct->lastTakenMoveFrom) + 1) = 0;

    gBattleStruct->palaceFlags &= ~(gBitTable[gActiveBattler]);

    for (i = 0; i < ARRAY_COUNT(gSideTimers); i++)
    {
        // Switched into sticky web user slot, so reset stored battler ID
        if (gSideTimers[i].stickyWebBattlerId == battler)
            gSideTimers[i].stickyWebBattlerId = 0xFF;
    }

    for (i = 0; i < gBattlersCount; i++)
    {
        if (i != battler && GetBattlerSide(i) != GetBattlerSide(battler))
            gBattleStruct->lastTakenMove[i] = MOVE_NONE;

        gBattleStruct->lastTakenMoveFrom[i][battler] = 0;
        if (i != gActiveBattler && GetBattlerSide(i) != GetBattlerSide(gActiveBattler))
        {
            *(gBattleStruct->lastTakenMove + i * 2 + 0) = 0;
            *(gBattleStruct->lastTakenMove + i * 2 + 1) = 0;
        }
        *(i * 8 + gActiveBattler * 2 + (u8*)(gBattleStruct->lastTakenMoveFrom) + 0) = 0;
        *(i * 8 + gActiveBattler * 2 + (u8*)(gBattleStruct->lastTakenMoveFrom) + 1) = 0;
    }

    gBattleStruct->choicedMove[battler] = MOVE_NONE;
    gBattleResources->flags->flags[battler] = 0;
    gCurrentMove = MOVE_NONE;
    *(u8*)((u8*)(&gBattleStruct->choicedMove[gActiveBattler]) + 0) = 0;
    *(u8*)((u8*)(&gBattleStruct->choicedMove[gActiveBattler]) + 1) = 0;

    gBattleResources->flags->flags[gActiveBattler] = 0;
    gCurrentMove = 0;
    gBattleStruct->arenaTurnCounter = 0xFF;

    // Reset damage to prevent things like red card activating if the switched-in mon is holding it
    gSpecialStatuses[battler].physicalDmg = 0;
    gSpecialStatuses[battler].specialDmg = 0;

    // Reset G-Max Chi Strike boosts.
    gBattleStruct->bonusCritStages[battler] = 0;

    // Reset Dynamax flags.
    UndoDynamax(battler);

    gBattleStruct->overwrittenAbilities[battler] = ABILITY_NONE;

    // Clear selected party ID so Revival Blessing doesn't get confused.
    gSelectedMonPartyId = PARTY_SIZE;

    // Allow for illegal abilities within tests.
    #if TESTING
    if (gTestRunnerEnabled)
    {
        u32 side = GetBattlerSide(battler);
        u32 partyIndex = gBattlerPartyIndexes[battler];
        if (TestRunner_Battle_GetForcedAbility(side, partyIndex))
            gBattleMons[i].ability = gBattleStruct->overwrittenAbilities[i] = TestRunner_Battle_GetForcedAbility(side, partyIndex);
    }
    #endif // TESTING

    Ai_UpdateSwitchInData(battler);
}

const u8* FaintClearSetData(u32 battler)
    ClearBattlerMoveHistory(gActiveBattler);
    ClearBattlerAbilityHistory(gActiveBattler);
}

void FaintClearSetData(void)
{
    s32 i;
    const u8 *result = NULL;
    u8 *ptr;

    for (i = 0; i < NUM_BATTLE_STATS; i++)
        gBattleMons[battler].statStages[i] = DEFAULT_STAT_STAGE;

    gBattleMons[battler].status2 = 0;
    gStatuses3[battler] = 0;
    gStatuses4[battler] = 0;
    gBattleMons[gActiveBattler].status2 = 0;
    gStatuses3[gActiveBattler] = 0;

    for (i = 0; i < gBattlersCount; i++)
    {
        if ((gBattleMons[i].status2 & STATUS2_ESCAPE_PREVENTION) && gDisableStructs[i].battlerPreventingEscape == battler)
            gBattleMons[i].status2 &= ~STATUS2_ESCAPE_PREVENTION;
        if (gBattleMons[i].status2 & STATUS2_INFATUATED_WITH(battler))
            gBattleMons[i].status2 &= ~STATUS2_INFATUATED_WITH(battler);
        if ((gBattleMons[i].status2 & STATUS2_WRAPPED) && *(gBattleStruct->wrappedBy + i) == battler)
            gBattleMons[i].status2 &= ~STATUS2_WRAPPED;
        if (gBattleMons[i].status2 & STATUS2_INFATUATED_WITH(gActiveBattler))
            gBattleMons[i].status2 &= ~(STATUS2_INFATUATED_WITH(gActiveBattler));
        if ((gBattleMons[i].status2 & STATUS2_WRAPPED) && *(gBattleStruct->wrappedBy + i) == gActiveBattler)
            gBattleMons[i].status2 &= ~(STATUS2_WRAPPED);
    }

    gActionSelectionCursor[battler] = 0;
    gMoveSelectionCursor[battler] = 0;

    memset(&gDisableStructs[battler], 0, sizeof(struct DisableStruct));

    gProtectStructs[battler].protected = FALSE;
    gProtectStructs[battler].spikyShielded = FALSE;
    gProtectStructs[battler].kingsShielded = FALSE;
    gProtectStructs[battler].banefulBunkered = FALSE;
    gProtectStructs[battler].quash = FALSE;
    gProtectStructs[battler].obstructed = FALSE;
    gProtectStructs[battler].silkTrapped = FALSE;
    gProtectStructs[battler].endured = FALSE;
    gProtectStructs[battler].noValidMoves = FALSE;
    gProtectStructs[battler].helpingHand = FALSE;
    gProtectStructs[battler].bounceMove = FALSE;
    gProtectStructs[battler].stealMove = FALSE;
    gProtectStructs[battler].prlzImmobility = FALSE;
    gProtectStructs[battler].confusionSelfDmg = FALSE;
    gProtectStructs[battler].targetAffected = FALSE;
    gProtectStructs[battler].chargingTurn = FALSE;
    gProtectStructs[battler].fleeType = 0;
    gProtectStructs[battler].usedImprisonedMove = FALSE;
    gProtectStructs[battler].loveImmobility = FALSE;
    gProtectStructs[battler].usedDisabledMove = FALSE;
    gProtectStructs[battler].usedTauntedMove = FALSE;
    gProtectStructs[battler].flag2Unknown = FALSE;
    gProtectStructs[battler].flinchImmobility = FALSE;
    gProtectStructs[battler].notFirstStrike = FALSE;
    gProtectStructs[battler].usedHealBlockedMove = FALSE;
    gProtectStructs[battler].usesBouncedMove = FALSE;
    gProtectStructs[battler].usedGravityPreventedMove = FALSE;
    gProtectStructs[battler].usedThroatChopPreventedMove = FALSE;
    gProtectStructs[battler].statRaised = FALSE;
    gProtectStructs[battler].statFell = FALSE;
    gProtectStructs[battler].pranksterElevated = FALSE;
    ptr = (u8 *)&gDisableStructs[gActiveBattler];
    for (i = 0; i < sizeof(struct DisableStruct); i++)
        ptr[i] = 0;

    gProtectStructs[gActiveBattler].protected = 0;
    gProtectStructs[gActiveBattler].endured = 0;
    gProtectStructs[gActiveBattler].noValidMoves = 0;
    gProtectStructs[gActiveBattler].helpingHand = 0;
    gProtectStructs[gActiveBattler].bounceMove = 0;
    gProtectStructs[gActiveBattler].stealMove = 0;
    gProtectStructs[gActiveBattler].flag0Unknown = 0;
    gProtectStructs[gActiveBattler].prlzImmobility = 0;
    gProtectStructs[gActiveBattler].confusionSelfDmg = 0;
    gProtectStructs[gActiveBattler].targetNotAffected = 0;
    gProtectStructs[gActiveBattler].chargingTurn = 0;
    gProtectStructs[gActiveBattler].fleeFlag = 0;
    gProtectStructs[gActiveBattler].usedImprisonedMove = 0;
    gProtectStructs[gActiveBattler].loveImmobility = 0;
    gProtectStructs[gActiveBattler].usedDisabledMove = 0;
    gProtectStructs[gActiveBattler].usedTauntedMove = 0;
    gProtectStructs[gActiveBattler].flag2Unknown = 0;
    gProtectStructs[gActiveBattler].flinchImmobility = 0;
    gProtectStructs[gActiveBattler].notFirstStrike = 0;

    gDisableStructs[battler].isFirstTurn = 2;

    gLastMoves[battler] = MOVE_NONE;
    gLastLandedMoves[battler] = MOVE_NONE;
    gLastHitByType[battler] = 0;
    gLastResultingMoves[battler] = MOVE_NONE;
    gLastPrintedMoves[battler] = MOVE_NONE;
    gLastHitBy[battler] = 0xFF;
    gLastMoves[gActiveBattler] = 0;
    gLastLandedMoves[gActiveBattler] = 0;
    gLastHitByType[gActiveBattler] = 0;
    gLastResultingMoves[gActiveBattler] = 0;
    gLastPrintedMoves[gActiveBattler] = 0;
    gLastHitBy[gActiveBattler] = 0xFF;

    gBattleStruct->choicedMove[battler] = MOVE_NONE;
    gBattleStruct->sameMoveTurns[battler] = 0;
    gBattleStruct->lastTakenMove[battler] = MOVE_NONE;
    gBattleStruct->lastTakenMoveFrom[battler][0] = 0;
    gBattleStruct->lastTakenMoveFrom[battler][1] = 0;
    gBattleStruct->lastTakenMoveFrom[battler][2] = 0;
    gBattleStruct->lastTakenMoveFrom[battler][3] = 0;
    *(u8*)((u8*)(&gBattleStruct->choicedMove[gActiveBattler]) + 0) = 0;
    *(u8*)((u8*)(&gBattleStruct->choicedMove[gActiveBattler]) + 1) = 0;

    *(gBattleStruct->lastTakenMove + gActiveBattler * 2 + 0) = 0;
    *(gBattleStruct->lastTakenMove + gActiveBattler * 2 + 1) = 0;
    *(0 * 2 + gActiveBattler * 8 + (u8*)(gBattleStruct->lastTakenMoveFrom) + 0) = 0;
    *(0 * 2 + gActiveBattler * 8 + (u8*)(gBattleStruct->lastTakenMoveFrom) + 1) = 0;
    *(1 * 2 + gActiveBattler * 8 + (u8*)(gBattleStruct->lastTakenMoveFrom) + 0) = 0;
    *(1 * 2 + gActiveBattler * 8 + (u8*)(gBattleStruct->lastTakenMoveFrom) + 1) = 0;
    *(2 * 2 + gActiveBattler * 8 + (u8*)(gBattleStruct->lastTakenMoveFrom) + 0) = 0;
    *(2 * 2 + gActiveBattler * 8 + (u8*)(gBattleStruct->lastTakenMoveFrom) + 1) = 0;
    *(3 * 2 + gActiveBattler * 8 + (u8*)(gBattleStruct->lastTakenMoveFrom) + 0) = 0;
    *(3 * 2 + gActiveBattler * 8 + (u8*)(gBattleStruct->lastTakenMoveFrom) + 1) = 0;

    gBattleStruct->palaceFlags &= ~(gBitTable[battler]);

    for (i = 0; i < ARRAY_COUNT(gSideTimers); i++)
    {
        // User of sticky web fainted, so reset the stored battler ID
        if (gSideTimers[i].stickyWebBattlerId == battler)
            gSideTimers[i].stickyWebBattlerId = 0xFF;
    }

    for (i = 0; i < gBattlersCount; i++)
    {
        if (i != battler && GetBattlerSide(i) != GetBattlerSide(battler))
            gBattleStruct->lastTakenMove[i] = MOVE_NONE;

        gBattleStruct->lastTakenMoveFrom[i][battler] = 0;
        if (i != gActiveBattler && GetBattlerSide(i) != GetBattlerSide(gActiveBattler))
        {
            *(gBattleStruct->lastTakenMove + i * 2 + 0) = 0;
            *(gBattleStruct->lastTakenMove + i * 2 + 1) = 0;
        }
        *(i * 8 + gActiveBattler * 2 + (u8*)(gBattleStruct->lastTakenMoveFrom) + 0) = 0;
        *(i * 8 + gActiveBattler * 2 + (u8*)(gBattleStruct->lastTakenMoveFrom) + 1) = 0;
    }

    gBattleResources->flags->flags[battler] = 0;

    gBattleMons[battler].type1 = gSpeciesInfo[gBattleMons[battler].species].types[0];
    gBattleMons[battler].type2 = gSpeciesInfo[gBattleMons[battler].species].types[1];
    gBattleMons[battler].type3 = TYPE_MYSTERY;

    Ai_UpdateFaintData(battler);
    TryBattleFormChange(battler, FORM_CHANGE_FAINT);

    gBattleStruct->overwrittenAbilities[battler] = ABILITY_NONE;

    // If the fainted mon was involved in a Sky Drop
    if (gBattleStruct->skyDropTargets[battler] != 0xFF)
    {
        // Get battler id of the other Pokemon involved in this Sky Drop
        u8 otherSkyDropper = gBattleStruct->skyDropTargets[battler];

        // Clear Sky Drop data
        gBattleStruct->skyDropTargets[battler] = 0xFF;
        gBattleStruct->skyDropTargets[otherSkyDropper] = 0xFF;

        // If the other Pokemon involved in this Sky Drop was the target, not the attacker
        if (gStatuses3[otherSkyDropper] & STATUS3_SKY_DROPPED)
        {
            // Release the target and take them out of the semi-invulnerable state
            gStatuses3[otherSkyDropper] &= ~(STATUS3_SKY_DROPPED | STATUS3_ON_AIR);

            // Make the target's sprite visible
            gSprites[gBattlerSpriteIds[otherSkyDropper]].invisible = FALSE;

            // If the target was sky dropped in the middle of using Outrage/Petal Dance/Thrash,
            // confuse them upon release and print "confused via fatigue" message and animation.
            if (gBattleMons[otherSkyDropper].status2 & STATUS2_LOCK_CONFUSE)
            {
                gBattleMons[otherSkyDropper].status2 &= ~(STATUS2_LOCK_CONFUSE);

                // If the released mon can be confused, do so.
                // Don't use CanBeConfused here, since it can cause issues in edge cases.
                if (!(GetBattlerAbility(otherSkyDropper) == ABILITY_OWN_TEMPO
                    || gBattleMons[otherSkyDropper].status2 & STATUS2_CONFUSION
                    || IsBattlerTerrainAffected(otherSkyDropper, STATUS_FIELD_MISTY_TERRAIN)))
                {
                    gBattleMons[otherSkyDropper].status2 |= STATUS2_CONFUSION_TURN(((Random()) % 4) + 2);
                    gBattlerAttacker = otherSkyDropper;
                    result = BattleScript_ThrashConfuses;
                }
            }
        }
    }

    // Clear Z-Move data
    gBattleStruct->zmove.active = FALSE;
    gBattleStruct->zmove.toBeUsed[battler] = MOVE_NONE;
    gBattleStruct->zmove.effect = EFFECT_HIT;
    return result;
}

static void DoBattleIntro(void)
{
    s32 i;
    u32 battler;
    u8 *state = &gBattleStruct->introState;

    switch (*state)
    gBattleMons[gActiveBattler].type1 = gBaseStats[gBattleMons[gActiveBattler].species].type1;
    gBattleMons[gActiveBattler].type2 = gBaseStats[gBattleMons[gActiveBattler].species].type2;

    ClearBattlerMoveHistory(gActiveBattler);
    ClearBattlerAbilityHistory(gActiveBattler);
}

static void BattleIntroGetMonsData(void)
{
    switch (gBattleCommunication[MULTIUSE_STATE])
    {
    case 0: // Get Data of all battlers.
        battler = gBattleCommunication[1];
        BtlController_EmitGetMonData(battler, BUFFER_A, REQUEST_ALL_BATTLE, 0);
        MarkBattlerForControllerExec(battler);
        (*state)++;
    case 0:
        gActiveBattler = gBattleCommunication[1];
        BtlController_EmitGetMonData(0, REQUEST_ALL_BATTLE, 0);
        MarkBattlerForControllerExec(gActiveBattler);
        gBattleCommunication[MULTIUSE_STATE]++;
        break;
    case 1:
        if (gBattleControllerExecFlags == 0)
        {
            gBattleCommunication[1]++;
            if (gBattleCommunication[1] == gBattlersCount)
                gBattleMainFunc = BattleIntroPrepareBackgroundSlide;
            else
                gBattleCommunication[MULTIUSE_STATE] = 0;
        }
        break;
    case 2: // Start graphical intro slide.
        if (!gBattleControllerExecFlags)
        {
            battler = GetBattlerAtPosition(0);
            BtlController_EmitIntroSlide(battler, BUFFER_A, gBattleTerrain);
            MarkBattlerForControllerExec(battler);
            gBattleCommunication[0] = 0;
            gBattleCommunication[1] = 0;
            (*state)++;
        }
        break;
    case 3: // Wait for intro slide.
        if (!gBattleControllerExecFlags)
            (*state)++;
        break;
    case 4: // Copy battler data gotten in cases 0 and 1. Draw trainer/mon sprite.
        for (battler = 0; battler < gBattlersCount; battler++)
        {
            if ((gBattleTypeFlags & BATTLE_TYPE_SAFARI) && GetBattlerSide(battler) == B_SIDE_PLAYER)
            {
                memset(&gBattleMons[battler], 0, sizeof(struct BattlePokemon));
            }
            else
            {
                memcpy(&gBattleMons[battler], &gBattleResources->bufferB[battler][4], sizeof(struct BattlePokemon));
                gBattleMons[battler].type1 = gSpeciesInfo[gBattleMons[battler].species].types[0];
                gBattleMons[battler].type2 = gSpeciesInfo[gBattleMons[battler].species].types[1];
                gBattleMons[battler].type3 = TYPE_MYSTERY;
                gBattleMons[battler].ability = GetAbilityBySpecies(gBattleMons[battler].species, gBattleMons[battler].abilityNum);
                gBattleStruct->hpOnSwitchout[GetBattlerSide(battler)] = gBattleMons[battler].hp;
                gBattleMons[battler].status2 = 0;
                for (i = 0; i < NUM_BATTLE_STATS; i++)
                    gBattleMons[battler].statStages[i] = DEFAULT_STAT_STAGE;
            }
    }
}

static void BattleIntroPrepareBackgroundSlide(void)
{
    if (gBattleControllerExecFlags == 0)
    {
        gActiveBattler = GetBattlerAtPosition(0);
        BtlController_EmitIntroSlide(0, gBattleTerrain);
        MarkBattlerForControllerExec(gActiveBattler);
        gBattleMainFunc = BattleIntroDrawTrainersOrMonsSprites;
        gBattleCommunication[0] = 0;
        gBattleCommunication[1] = 0;
    }
}

static void BattleIntroDrawTrainersOrMonsSprites(void)
{
    u8 *ptr;
    s32 i;

    if (gBattleControllerExecFlags)
        return;

    for (gActiveBattler = 0; gActiveBattler < gBattlersCount; gActiveBattler++)
    {
        if ((gBattleTypeFlags & BATTLE_TYPE_SAFARI)
         && GetBattlerSide(gActiveBattler) == B_SIDE_PLAYER)
        {
            ptr = (u8 *)&gBattleMons[gActiveBattler];
            for (i = 0; i < sizeof(struct BattlePokemon); i++)
                ptr[i] = 0;
        }
        else
        {
            u16* hpOnSwitchout;

            ptr = (u8 *)&gBattleMons[gActiveBattler];
            for (i = 0; i < sizeof(struct BattlePokemon); i++)
                ptr[i] = gBattleBufferB[gActiveBattler][4 + i];

            gBattleMons[gActiveBattler].type1 = gBaseStats[gBattleMons[gActiveBattler].species].type1;
            gBattleMons[gActiveBattler].type2 = gBaseStats[gBattleMons[gActiveBattler].species].type2;
            gBattleMons[gActiveBattler].ability = GetAbilityBySpecies(gBattleMons[gActiveBattler].species, gBattleMons[gActiveBattler].abilityNum);
            hpOnSwitchout = &gBattleStruct->hpOnSwitchout[GetBattlerSide(gActiveBattler)];
            *hpOnSwitchout = gBattleMons[gActiveBattler].hp;
            for (i = 0; i < NUM_BATTLE_STATS; i++)
                gBattleMons[gActiveBattler].statStages[i] = DEFAULT_STAT_STAGE;
            gBattleMons[gActiveBattler].status2 = 0;
        }

            // Draw sprite.
            switch (GetBattlerPosition(battler))
            {
            case B_POSITION_PLAYER_LEFT: // player sprite
                BtlController_EmitDrawTrainerPic(battler, BUFFER_A);
                MarkBattlerForControllerExec(battler);
                break;
            case B_POSITION_OPPONENT_LEFT:
                if (gBattleTypeFlags & BATTLE_TYPE_TRAINER) // opponent 1 sprite
                {
                    BtlController_EmitDrawTrainerPic(battler, BUFFER_A);
                    MarkBattlerForControllerExec(battler);
                }
                else // wild mon 1
                {
                    BtlController_EmitLoadMonSprite(battler, BUFFER_A);
                    MarkBattlerForControllerExec(battler);
                    gBattleResults.lastOpponentSpecies = GetMonData(&gEnemyParty[gBattlerPartyIndexes[battler]], MON_DATA_SPECIES, NULL);
                }
                break;
            case B_POSITION_PLAYER_RIGHT:
                if (gBattleTypeFlags & (BATTLE_TYPE_MULTI | BATTLE_TYPE_INGAME_PARTNER)) // partner sprite
                {
                    BtlController_EmitDrawTrainerPic(battler, BUFFER_A);
                    MarkBattlerForControllerExec(battler);
                }
                break;
            case B_POSITION_OPPONENT_RIGHT:
                if (gBattleTypeFlags & BATTLE_TYPE_TRAINER)
                {
                    if (gBattleTypeFlags & (BATTLE_TYPE_MULTI | BATTLE_TYPE_TWO_OPPONENTS) && !BATTLE_TWO_VS_ONE_OPPONENT) // opponent 2 if exists
                    {
                        BtlController_EmitDrawTrainerPic(battler, BUFFER_A);
                        MarkBattlerForControllerExec(battler);
                    }
                }
                else if (IsBattlerAlive(battler)) // wild mon 2 if alive
                {
                    BtlController_EmitLoadMonSprite(battler, BUFFER_A);
                    MarkBattlerForControllerExec(battler);
                    gBattleResults.lastOpponentSpecies = GetMonData(&gEnemyParty[gBattlerPartyIndexes[battler]], MON_DATA_SPECIES, NULL);
                }
                break;
            }
        if (GetBattlerPosition(gActiveBattler) == B_POSITION_PLAYER_LEFT)
        {
            BtlController_EmitDrawTrainerPic(0);
            MarkBattlerForControllerExec(gActiveBattler);
        }

        if (gBattleTypeFlags & BATTLE_TYPE_TRAINER)
        {
            if (GetBattlerPosition(gActiveBattler) == B_POSITION_OPPONENT_LEFT)
            {
                BtlController_EmitDrawTrainerPic(0);
                MarkBattlerForControllerExec(gActiveBattler);
            }
            if (GetBattlerSide(gActiveBattler) == B_SIDE_OPPONENT
             && !(gBattleTypeFlags & (BATTLE_TYPE_EREADER_TRAINER
                                      | BATTLE_TYPE_FRONTIER
                                      | BATTLE_TYPE_LINK
                                      | BATTLE_TYPE_RECORDED_LINK
                                      | BATTLE_TYPE_TRAINER_HILL)))
            {
                HandleSetPokedexFlag(SpeciesToNationalPokedexNum(gBattleMons[gActiveBattler].species), FLAG_SET_SEEN, gBattleMons[gActiveBattler].personality);
            }
        }
        else
        {
            if (GetBattlerSide(gActiveBattler) == B_SIDE_OPPONENT)
            {
                if (!(gBattleTypeFlags & (BATTLE_TYPE_EREADER_TRAINER
                                      | BATTLE_TYPE_FRONTIER
                                      | BATTLE_TYPE_LINK
                                      | BATTLE_TYPE_RECORDED_LINK
                                      | BATTLE_TYPE_TRAINER_HILL)))
                {
                    HandleSetPokedexFlag(SpeciesToNationalPokedexNum(gBattleMons[gActiveBattler].species), FLAG_SET_SEEN, gBattleMons[gActiveBattler].personality);
                }
                BtlController_EmitLoadMonSprite(0);
                MarkBattlerForControllerExec(gActiveBattler);
                gBattleResults.lastOpponentSpecies = GetMonData(&gEnemyParty[gBattlerPartyIndexes[gActiveBattler]], MON_DATA_SPECIES, NULL);
            }
        }

        if (gBattleTypeFlags & BATTLE_TYPE_MULTI)
        {
            if (GetBattlerPosition(gActiveBattler) == B_POSITION_PLAYER_RIGHT
             || GetBattlerPosition(gActiveBattler) == B_POSITION_OPPONENT_RIGHT)
            {
                BtlController_EmitDrawTrainerPic(0);
                MarkBattlerForControllerExec(gActiveBattler);
            }
        }

        if (gBattleTypeFlags & BATTLE_TYPE_TWO_OPPONENTS && GetBattlerPosition(gActiveBattler) == B_POSITION_OPPONENT_RIGHT)
        {
            BtlController_EmitDrawTrainerPic(0);
            MarkBattlerForControllerExec(gActiveBattler);
        }

            if (gBattleTypeFlags & BATTLE_TYPE_ARENA)
                BattleArena_InitPoints();
        }

        if (gBattleTypeFlags & BATTLE_TYPE_TRAINER)
        {
            (*state)++;
        }
        else // Skip party summary since it is a wild battle.
        {
            if (B_FAST_INTRO == TRUE)
                *state = 7; // Don't wait for sprite, print message at the same time.
            else
                *state = 6; // Wait for sprite to load.
        }
        break;
    case 5: // draw party summary in trainer battles
        if (!gBattleControllerExecFlags)
        {
            struct HpAndStatus hpStatus[PARTY_SIZE];
        if (gBattleTypeFlags & BATTLE_TYPE_ARENA)
            BattleArena_InitPoints();
    }
    gBattleMainFunc = BattleIntroDrawPartySummaryScreens;
}

static void BattleIntroDrawPartySummaryScreens(void)
{
    s32 i;
    struct HpAndStatus hpStatus[PARTY_SIZE];

            for (i = 0; i < PARTY_SIZE; i++)
            {
                if (GetMonData(&gEnemyParty[i], MON_DATA_SPECIES_OR_EGG) == SPECIES_NONE
                 || GetMonData(&gEnemyParty[i], MON_DATA_SPECIES_OR_EGG) == SPECIES_EGG)
                {
                    hpStatus[i].hp = HP_EMPTY_SLOT;
                    hpStatus[i].status = 0;
                }
                else
                {
                    hpStatus[i].hp = GetMonData(&gEnemyParty[i], MON_DATA_HP);
                    hpStatus[i].status = GetMonData(&gEnemyParty[i], MON_DATA_STATUS);
                }
            }

            battler = GetBattlerAtPosition(B_POSITION_OPPONENT_LEFT);
            BtlController_EmitDrawPartyStatusSummary(battler, BUFFER_A, hpStatus, PARTY_SUMM_SKIP_DRAW_DELAY);
            MarkBattlerForControllerExec(battler);
    if (gBattleControllerExecFlags)
        return;

    if (gBattleTypeFlags & BATTLE_TYPE_TRAINER)
    {
        for (i = 0; i < PARTY_SIZE; i++)
        {
            if (GetMonData(&gEnemyParty[i], MON_DATA_SPECIES2) == SPECIES_NONE
             || GetMonData(&gEnemyParty[i], MON_DATA_SPECIES2) == SPECIES_EGG)
            {
                hpStatus[i].hp = 0xFFFF;
                hpStatus[i].status = 0;
            }
            else
            {
                hpStatus[i].hp = GetMonData(&gEnemyParty[i], MON_DATA_HP);
                hpStatus[i].status = GetMonData(&gEnemyParty[i], MON_DATA_STATUS);
            }
        }
        gActiveBattler = GetBattlerAtPosition(B_POSITION_OPPONENT_LEFT);
        BtlController_EmitDrawPartyStatusSummary(0, hpStatus, 0x80);
        MarkBattlerForControllerExec(gActiveBattler);

        for (i = 0; i < PARTY_SIZE; i++)
        {
            if (GetMonData(&gPlayerParty[i], MON_DATA_SPECIES2) == SPECIES_NONE
             || GetMonData(&gPlayerParty[i], MON_DATA_SPECIES2) == SPECIES_EGG)
            {
                hpStatus[i].hp = 0xFFFF;
                hpStatus[i].status = 0;
            }
            else
            {
                hpStatus[i].hp = GetMonData(&gPlayerParty[i], MON_DATA_HP);
                hpStatus[i].status = GetMonData(&gPlayerParty[i], MON_DATA_STATUS);
            }
        }
        gActiveBattler = GetBattlerAtPosition(B_POSITION_PLAYER_LEFT);
        BtlController_EmitDrawPartyStatusSummary(0, hpStatus, 0x80);
        MarkBattlerForControllerExec(gActiveBattler);

        gBattleMainFunc = BattleIntroPrintTrainerWantsToBattle;
    }
    else
    {
        // The struct gets set here, but nothing is ever done with it since
        // wild battles don't show the party summary.
        // Still, there's no point in having dead code.

        for (i = 0; i < PARTY_SIZE; i++)
        {
            if (GetMonData(&gPlayerParty[i], MON_DATA_SPECIES2) == SPECIES_NONE
             || GetMonData(&gPlayerParty[i], MON_DATA_SPECIES2) == SPECIES_EGG)
            {
                hpStatus[i].hp = 0xFFFF;
                hpStatus[i].status = 0;
            }
            else
            {
                if (B_FAST_INTRO == TRUE)
                    *state = 15; // Wait for text to be printed.
                else
                    *state = 14; // Wait for text and sprite.
            }
        }
        break;
    case 9: // print opponent sends out
        if (gBattleTypeFlags & BATTLE_TYPE_RECORDED_LINK && !(gBattleTypeFlags & BATTLE_TYPE_RECORDED_IS_MASTER))
            PrepareStringBattle(STRINGID_INTROSENDOUT, GetBattlerAtPosition(B_POSITION_PLAYER_LEFT));
        else
            PrepareStringBattle(STRINGID_INTROSENDOUT, GetBattlerAtPosition(B_POSITION_OPPONENT_LEFT));
        (*state)++;
        break;
    case 10: // wait for opponent sends out text
        if (!gBattleControllerExecFlags)
            (*state)++;
        break;
    case 11: // first opponent's mon send out animation
        if (gBattleTypeFlags & BATTLE_TYPE_RECORDED_LINK && !(gBattleTypeFlags & BATTLE_TYPE_RECORDED_IS_MASTER))
            battler = GetBattlerAtPosition(B_POSITION_PLAYER_LEFT);
        else
            battler = GetBattlerAtPosition(B_POSITION_OPPONENT_LEFT);

        BtlController_EmitIntroTrainerBallThrow(battler, BUFFER_A);
        MarkBattlerForControllerExec(battler);
        (*state)++;
        break;
    case 12: // nothing
        (*state)++;
    case 13: // second opponent's mon send out
        if (gBattleTypeFlags & (BATTLE_TYPE_MULTI | BATTLE_TYPE_TWO_OPPONENTS) && !BATTLE_TWO_VS_ONE_OPPONENT)
        {
            if (gBattleTypeFlags & BATTLE_TYPE_RECORDED_LINK && !(gBattleTypeFlags & BATTLE_TYPE_RECORDED_IS_MASTER))
                battler = GetBattlerAtPosition(B_POSITION_PLAYER_RIGHT);
            else
                battler = GetBattlerAtPosition(B_POSITION_OPPONENT_RIGHT);

            BtlController_EmitIntroTrainerBallThrow(battler, BUFFER_A);
            MarkBattlerForControllerExec(battler);
        }
        if (B_FAST_INTRO == TRUE
          && !(gBattleTypeFlags & (BATTLE_TYPE_RECORDED | BATTLE_TYPE_RECORDED_LINK | BATTLE_TYPE_RECORDED_IS_MASTER | BATTLE_TYPE_LINK)))
            *state = 15; // Print at the same time as trainer sends out second mon.
        else
            (*state)++;
        break;
    case 14: // wait for opponent 2 send out
        if (!gBattleControllerExecFlags)
            (*state)++;
        break;
    case 15: // wait for wild battle message
        if (!IsBattlerMarkedForControllerExec(GetBattlerAtPosition(B_POSITION_PLAYER_LEFT)))
            (*state)++;
        break;
    case 16: // print player sends out
        if (!(gBattleTypeFlags & BATTLE_TYPE_SAFARI))
        {
            if (gBattleTypeFlags & BATTLE_TYPE_RECORDED_LINK && !(gBattleTypeFlags & BATTLE_TYPE_RECORDED_IS_MASTER))
                battler = GetBattlerAtPosition(B_POSITION_OPPONENT_LEFT);
            else
                battler = GetBattlerAtPosition(B_POSITION_PLAYER_LEFT);

            // A hack that makes fast intro work in trainer battles too.
            if (B_FAST_INTRO == TRUE
                && gBattleTypeFlags & BATTLE_TYPE_TRAINER
                && !(gBattleTypeFlags & (BATTLE_TYPE_RECORDED | BATTLE_TYPE_RECORDED_LINK | BATTLE_TYPE_RECORDED_IS_MASTER | BATTLE_TYPE_LINK))
                && gSprites[gHealthboxSpriteIds[battler ^ BIT_SIDE]].callback == SpriteCallbackDummy)
            {
                return;
            }

            PrepareStringBattle(STRINGID_INTROSENDOUT, battler);
        }
        (*state)++;
        break;
    case 17: // wait for player send out message
        if (!(gBattleTypeFlags & BATTLE_TYPE_LINK && gBattleControllerExecFlags))
        {
            if (gBattleTypeFlags & BATTLE_TYPE_RECORDED_LINK && !(gBattleTypeFlags & BATTLE_TYPE_RECORDED_IS_MASTER))
                battler = GetBattlerAtPosition(B_POSITION_OPPONENT_LEFT);
            else
                battler = GetBattlerAtPosition(B_POSITION_PLAYER_LEFT);

            if (!IsBattlerMarkedForControllerExec(battler))
                (*state)++;
        }
        break;
    case 18: // player 1 send out
        if (gBattleTypeFlags & BATTLE_TYPE_RECORDED_LINK && !(gBattleTypeFlags & BATTLE_TYPE_RECORDED_IS_MASTER))
            battler = GetBattlerAtPosition(B_POSITION_OPPONENT_LEFT);
        else
            battler = GetBattlerAtPosition(B_POSITION_PLAYER_LEFT);

        BtlController_EmitIntroTrainerBallThrow(battler, BUFFER_A);
        MarkBattlerForControllerExec(battler);
        (*state)++;
        break;
    case 19: // player 2 send out
        if (gBattleTypeFlags & (BATTLE_TYPE_MULTI | BATTLE_TYPE_INGAME_PARTNER))
        {
            if (gBattleTypeFlags & BATTLE_TYPE_RECORDED_LINK && !(gBattleTypeFlags & BATTLE_TYPE_RECORDED_IS_MASTER))
                battler = GetBattlerAtPosition(B_POSITION_OPPONENT_RIGHT);
            else
                battler = GetBattlerAtPosition(B_POSITION_PLAYER_RIGHT);

            BtlController_EmitIntroTrainerBallThrow(battler, BUFFER_A);
            MarkBattlerForControllerExec(battler);
        }
        (*state)++;
        break;
    case 20: // set dex and battle vars
        if (!gBattleControllerExecFlags)
        {
            for (battler = 0; battler < gBattlersCount; battler++)
            {
                if (GetBattlerSide(battler) == B_SIDE_OPPONENT
                 && !(gBattleTypeFlags & (BATTLE_TYPE_EREADER_TRAINER
                                          | BATTLE_TYPE_FRONTIER
                                          | BATTLE_TYPE_LINK
                                          | BATTLE_TYPE_RECORDED_LINK
                                          | BATTLE_TYPE_TRAINER_HILL)))
                {
                    HandleSetPokedexFlag(SpeciesToNationalPokedexNum(gBattleMons[battler].species), FLAG_SET_SEEN, gBattleMons[battler].personality);
                }
            }
                hpStatus[i].hp = GetMonData(&gPlayerParty[i], MON_DATA_HP);
                hpStatus[i].status = GetMonData(&gPlayerParty[i], MON_DATA_STATUS);
            }
        }

        gBattleMainFunc = BattleIntroPrintWildMonAttacked;
    }

}

static void BattleIntroPrintTrainerWantsToBattle(void)
{
    if (gBattleControllerExecFlags == 0)
    {
        gActiveBattler = GetBattlerAtPosition(B_POSITION_OPPONENT_LEFT);
        PrepareStringBattle(STRINGID_INTROMSG, gActiveBattler);
        gBattleMainFunc = BattleIntroPrintOpponentSendsOut;
    }
}

static void BattleIntroPrintWildMonAttacked(void)
{
    if (gBattleControllerExecFlags == 0)
    {
        gBattleMainFunc = BattleIntroPrintPlayerSendsOut;
        PrepareStringBattle(STRINGID_INTROMSG, 0);
    }
}

static void BattleIntroPrintOpponentSendsOut(void)
{
    u32 position;

    if (gBattleControllerExecFlags)
        return;

    if (!(gBattleTypeFlags & BATTLE_TYPE_RECORDED))
        position = B_POSITION_OPPONENT_LEFT;
    else if (gBattleTypeFlags & BATTLE_TYPE_RECORDED_LINK)
    {
        if (gBattleTypeFlags & BATTLE_TYPE_RECORDED_IS_MASTER)
            position = B_POSITION_OPPONENT_LEFT;
        else
            position = B_POSITION_PLAYER_LEFT;
    }
    else
        position = B_POSITION_OPPONENT_LEFT;

    PrepareStringBattle(STRINGID_INTROSENDOUT, GetBattlerAtPosition(position));
    gBattleMainFunc = BattleIntroOpponent1SendsOutMonAnimation;
}

static void BattleIntroOpponent2SendsOutMonAnimation(void)
{
    u32 position;

    if (!(gBattleTypeFlags & BATTLE_TYPE_RECORDED))
        position = B_POSITION_OPPONENT_RIGHT;
    else if (gBattleTypeFlags & BATTLE_TYPE_RECORDED_LINK)
    {
        if (gBattleTypeFlags & BATTLE_TYPE_RECORDED_IS_MASTER)
            position = B_POSITION_OPPONENT_RIGHT;
        else
            position = B_POSITION_PLAYER_RIGHT;
    }
    else
        position = B_POSITION_OPPONENT_RIGHT;

    for (gActiveBattler = 0; gActiveBattler < gBattlersCount; gActiveBattler++)
    {
        if (GetBattlerPosition(gActiveBattler) == position)
        {
            BtlController_EmitIntroTrainerBallThrow(0);
            MarkBattlerForControllerExec(gActiveBattler);
        }
    }

    gBattleMainFunc = BattleIntroRecordMonsToDex;
}

static void BattleIntroOpponent1SendsOutMonAnimation(void)
{
    u32 position;

    if (gBattleTypeFlags & BATTLE_TYPE_RECORDED)
    {
        if (gBattleTypeFlags & BATTLE_TYPE_RECORDED_LINK)
        {
            if (gBattleTypeFlags & BATTLE_TYPE_RECORDED_IS_MASTER)
                position = B_POSITION_OPPONENT_LEFT;
            else
                position = B_POSITION_PLAYER_LEFT;
        }
        else
            position = B_POSITION_OPPONENT_LEFT;
    }
    else
        position = B_POSITION_OPPONENT_LEFT;

    if (gBattleControllerExecFlags)
        return;

    for (gActiveBattler = 0; gActiveBattler < gBattlersCount; gActiveBattler++)
    {
        if (GetBattlerPosition(gActiveBattler) == position)
        {
            BtlController_EmitIntroTrainerBallThrow(0);
            MarkBattlerForControllerExec(gActiveBattler);
            if (gBattleTypeFlags & (BATTLE_TYPE_MULTI | BATTLE_TYPE_TWO_OPPONENTS))
            {
                gBattleMainFunc = BattleIntroOpponent2SendsOutMonAnimation;
                return;
            }
        }
    }

    gBattleMainFunc = BattleIntroRecordMonsToDex;
}

static void BattleIntroRecordMonsToDex(void)
{
    if (gBattleControllerExecFlags == 0)
    {
        for (gActiveBattler = 0; gActiveBattler < gBattlersCount; gActiveBattler++)
        {
            if (GetBattlerSide(gActiveBattler) == B_SIDE_OPPONENT
             && !(gBattleTypeFlags & (BATTLE_TYPE_EREADER_TRAINER
                                      | BATTLE_TYPE_FRONTIER
                                      | BATTLE_TYPE_LINK
                                      | BATTLE_TYPE_RECORDED_LINK
                                      | BATTLE_TYPE_TRAINER_HILL)))
            {
                HandleSetPokedexFlag(SpeciesToNationalPokedexNum(gBattleMons[gActiveBattler].species), FLAG_SET_SEEN, gBattleMons[gActiveBattler].personality);
            }
        }
        gBattleMainFunc = BattleIntroPrintPlayerSendsOut;
    }
}

void sub_803B3AC(void) // unused
{
    if (gBattleControllerExecFlags == 0)
        gBattleMainFunc = BattleIntroPrintPlayerSendsOut;
}

static void BattleIntroPrintPlayerSendsOut(void)
{
    if (gBattleControllerExecFlags == 0)
    {
        u8 position;

        if (!(gBattleTypeFlags & BATTLE_TYPE_RECORDED))
            position = B_POSITION_PLAYER_LEFT;
        else if (gBattleTypeFlags & BATTLE_TYPE_RECORDED_LINK)
        {
            if (gBattleTypeFlags & BATTLE_TYPE_RECORDED_IS_MASTER)
                position = B_POSITION_PLAYER_LEFT;
            else
                position = B_POSITION_OPPONENT_LEFT;
        }
        else
            position = B_POSITION_PLAYER_LEFT;

        if (!(gBattleTypeFlags & BATTLE_TYPE_SAFARI))
            PrepareStringBattle(STRINGID_INTROSENDOUT, GetBattlerAtPosition(position));

        gBattleMainFunc = BattleIntroPlayer1SendsOutMonAnimation;
    }
}

static void BattleIntroPlayer2SendsOutMonAnimation(void)
{
    u32 position;

    if (!(gBattleTypeFlags & BATTLE_TYPE_RECORDED))
        position = B_POSITION_PLAYER_RIGHT;
    else if (gBattleTypeFlags & BATTLE_TYPE_RECORDED_LINK)
    {
        if (gBattleTypeFlags & BATTLE_TYPE_RECORDED_IS_MASTER)
            position = B_POSITION_PLAYER_RIGHT;
        else
            position = B_POSITION_OPPONENT_RIGHT;
    }
    else
        position = B_POSITION_PLAYER_RIGHT;

    for (gActiveBattler = 0; gActiveBattler < gBattlersCount; gActiveBattler++)
    {
        if (GetBattlerPosition(gActiveBattler) == position)
        {
            BtlController_EmitIntroTrainerBallThrow(0);
            MarkBattlerForControllerExec(gActiveBattler);
        }
    }

            gBattleStruct->switchInAbilitiesCounter = 0;
            gBattleStruct->switchInItemsCounter = 0;
            gBattleStruct->overworldWeatherDone = FALSE;
            SetAiLogicDataForTurn(AI_DATA); // get assumed abilities, hold effects, etc of all battlers
            Ai_InitPartyStruct(); // Save mons party counts, and first 2/4 mons on the battlefield.
            gBattleMainFunc = TryDoEventsBeforeFirstTurn;
        }
        break;
    }
}

static void TryDoEventsBeforeFirstTurn(void)
{
    s32 i, j;
    gBattleStruct->switchInAbilitiesCounter = 0;
    gBattleStruct->switchInItemsCounter = 0;
    gBattleStruct->overworldWeatherDone = FALSE;

    gBattleMainFunc = TryDoEventsBeforeFirstTurn;
}

static void BattleIntroPlayer1SendsOutMonAnimation(void)
{
    u32 position;

    if (!(gBattleTypeFlags & BATTLE_TYPE_RECORDED))
        position = B_POSITION_PLAYER_LEFT;
    else if (gBattleTypeFlags & BATTLE_TYPE_RECORDED_LINK)
    {
        if (gBattleTypeFlags & BATTLE_TYPE_RECORDED_IS_MASTER)
            position = B_POSITION_PLAYER_LEFT;
        else
            position = B_POSITION_OPPONENT_LEFT;
    }
    else
        position = B_POSITION_PLAYER_LEFT;

    if (gBattleControllerExecFlags)
        return;

    // Set invalid mons as absent(for example when starting a double battle with only one pokemon).
    if (!(gBattleTypeFlags & BATTLE_TYPE_SAFARI))
    {
        for (i = 0; i < gBattlersCount; i++)
        {
            struct Pokemon *party = GetBattlerParty(i);
            struct Pokemon *mon = &party[gBattlerPartyIndexes[i]];
            if (gBattleMons[i].hp == 0 || gBattleMons[i].species == SPECIES_NONE || GetMonData(mon, MON_DATA_IS_EGG))
                gAbsentBattlerFlags |= gBitTable[i];
        }
    }

    // Allow for illegal abilities within tests.
    #if TESTING
    if (gTestRunnerEnabled && gBattleStruct->switchInAbilitiesCounter == 0)
    {
        for (i = 0; i < gBattlersCount; ++i)
        {
            u32 side = GetBattlerSide(i);
            u32 partyIndex = gBattlerPartyIndexes[i];
            if (TestRunner_Battle_GetForcedAbility(side, partyIndex))
                gBattleMons[i].ability = gBattleStruct->overwrittenAbilities[i] = TestRunner_Battle_GetForcedAbility(side, partyIndex);
        }
    }
    #endif // TESTING
    for (gActiveBattler = 0; gActiveBattler < gBattlersCount; gActiveBattler++)
    {
        if (GetBattlerPosition(gActiveBattler) == position)
        {
            BtlController_EmitIntroTrainerBallThrow(0);
            MarkBattlerForControllerExec(gActiveBattler);
            if (gBattleTypeFlags & (BATTLE_TYPE_MULTI))
            {
                gBattleMainFunc = BattleIntroPlayer2SendsOutMonAnimation;
                return;
            }
        }
    }

    gBattleStruct->switchInAbilitiesCounter = 0;
    gBattleStruct->switchInItemsCounter = 0;
    gBattleStruct->overworldWeatherDone = FALSE;

    gBattleMainFunc = TryDoEventsBeforeFirstTurn;
}

void sub_803B598(void) // unused
{
    if (gBattleControllerExecFlags == 0)
    {
        for (gActiveBattler = 0; gActiveBattler < gBattlersCount; gActiveBattler++)
        {
            if (GetBattlerSide(gActiveBattler) == B_SIDE_PLAYER)
            {
                BtlController_EmitSwitchInAnim(0, gBattlerPartyIndexes[gActiveBattler], FALSE);
                MarkBattlerForControllerExec(gActiveBattler);
            }
        }

        gBattleStruct->switchInAbilitiesCounter = 0;
        gBattleStruct->switchInItemsCounter = 0;
        gBattleStruct->overworldWeatherDone = FALSE;

        gBattleMainFunc = TryDoEventsBeforeFirstTurn;
    }
}

static void TryDoEventsBeforeFirstTurn(void)
{
    s32 i;
    s32 j;
    u8 effect = 0;

    if (gBattleControllerExecFlags)
        return;

    if (gBattleStruct->switchInAbilitiesCounter == 0)
    {
        for (i = 0; i < gBattlersCount; i++)
            gBattlerByTurnOrder[i] = i;
        for (i = 0; i < gBattlersCount - 1; i++)
        {
            for (j = i + 1; j < gBattlersCount; j++)
            {
                if (GetWhichBattlerFaster(gBattlerByTurnOrder[i], gBattlerByTurnOrder[j], TRUE) != 0)
                    SwapTurnOrder(i, j);
            }
        }
    }
    if (!gBattleStruct->overworldWeatherDone
        && AbilityBattleEffects(ABILITYEFFECT_SWITCH_IN_WEATHER, 0, 0, ABILITYEFFECT_SWITCH_IN_WEATHER, 0) != 0)
    {
        gBattleStruct->overworldWeatherDone = TRUE;
        return;
    }

    if (!gBattleStruct->terrainDone && AbilityBattleEffects(ABILITYEFFECT_SWITCH_IN_TERRAIN, 0, 0, ABILITYEFFECT_SWITCH_IN_TERRAIN, 0) != 0)
    {
        gBattleStruct->terrainDone = TRUE;
        return;
    }

    // Totem boosts
    for (i = 0; i < gBattlersCount; i++)
    {
        if (gQueuedStatBoosts[i].stats != 0 && !gProtectStructs[i].eatMirrorHerb && gProtectStructs[i].activateOpportunist == 0)
        {
            gBattlerAttacker = i;
            BattleScriptExecute(BattleScript_TotemVar);
            return;
        }
    }

    // Check neutralizing gas
    if (AbilityBattleEffects(ABILITYEFFECT_NEUTRALIZINGGAS, 0, 0, 0, 0) != 0)
        return;

    // Check all switch in abilities happening from the fastest mon to slowest.
    while (gBattleStruct->switchInAbilitiesCounter < gBattlersCount)
    {
        gBattlerAttacker = gBattlerByTurnOrder[gBattleStruct->switchInAbilitiesCounter++];

        if (TryPrimalReversion(gBattlerAttacker))
        if (AbilityBattleEffects(ABILITYEFFECT_ON_SWITCHIN, gBattlerByTurnOrder[gBattleStruct->switchInAbilitiesCounter], 0, 0, 0) != 0)
            effect++;

        gBattleStruct->switchInAbilitiesCounter++;

        if (effect)
            return;
        if (AbilityBattleEffects(ABILITYEFFECT_ON_SWITCHIN, gBattlerAttacker, 0, 0, 0) != 0)
            return;
    }
    if (AbilityBattleEffects(ABILITYEFFECT_TRACE1, 0, 0, 0, 0) != 0)
    }
    if (AbilityBattleEffects(ABILITYEFFECT_INTIMIDATE1, 0, 0, 0, 0) != 0)
        return;
    if (AbilityBattleEffects(ABILITYEFFECT_TRACE, 0, 0, 0, 0) != 0)
        return;
    // Check all switch in items having effect from the fastest mon to slowest.
    while (gBattleStruct->switchInItemsCounter < gBattlersCount)
    {
        if (ItemBattleEffects(ITEMEFFECT_ON_SWITCH_IN, gBattlerByTurnOrder[gBattleStruct->switchInItemsCounter], FALSE))
            effect++;

        gBattleStruct->switchInItemsCounter++;

        if (effect)
            return;
    }

    if (AbilityBattleEffects(ABILITYEFFECT_OPPORTUNIST, 0, 0, 0, 0))
        return;

    for (i = 0; i < MAX_BATTLERS_COUNT; i++)
    {
        *(gBattleStruct->monToSwitchIntoId + i) = PARTY_SIZE;
        gChosenActionByBattler[i] = B_ACTION_NONE;
        gChosenMoveByBattler[i] = MOVE_NONE;
    }
    TurnValuesCleanUp(FALSE);
    SpecialStatusesClear();
    *(&gBattleStruct->field_91) = gAbsentBattlerFlags;
    BattlePutTextOnWindow(gText_EmptyString3, 0);
    gBattleMainFunc = HandleTurnActionSelectionState;
    ResetSentPokesToOpponentValue();

    for (i = 0; i < BATTLE_COMMUNICATION_ENTRIES_COUNT; i++)
        gBattleCommunication[i] = 0;

    for (i = 0; i < gBattlersCount; i++)
        gBattleMons[i].status2 &= ~(STATUS2_FLINCHED);

    *(&gBattleStruct->turnEffectsTracker) = 0;
    *(&gBattleStruct->turnEffectsBattlerId) = 0;
    *(&gBattleStruct->wishPerishSongState) = 0;
    *(&gBattleStruct->wishPerishSongBattlerId) = 0;
    gBattleScripting.moveendState = 0;
    gBattleStruct->faintedActionsState = 0;
    gBattleStruct->turnCountersTracker = 0;
    gMoveResultFlags = 0;

    gRandomTurnNumber = Random();

    memset(gQueuedStatBoosts, 0, sizeof(gQueuedStatBoosts));  // erase all totem boosts just to be safe

    SetAiLogicDataForTurn(AI_DATA); // get assumed abilities, hold effects, etc of all battlers

    if (gBattleTypeFlags & BATTLE_TYPE_ARENA)
    {
        StopCryAndClearCrySongs();
        BattleScriptExecute(BattleScript_ArenaTurnBeginning);
    }
}

static void HandleEndTurn_ContinueBattle(void)
{
    s32 i;

    if (gBattleControllerExecFlags == 0)
    {
        gBattleMainFunc = BattleTurnPassed;
        for (i = 0; i < BATTLE_COMMUNICATION_ENTRIES_COUNT; i++)
            gBattleCommunication[i] = 0;
        for (i = 0; i < gBattlersCount; i++)
        {
            gBattleMons[i].status2 &= ~(STATUS2_FLINCHED);
            if ((gBattleMons[i].status1 & STATUS1_SLEEP) && (gBattleMons[i].status2 & STATUS2_MULTIPLETURNS))
                CancelMultiTurnMoves(i);
        }
        gBattleStruct->turnEffectsTracker = 0;
        gBattleStruct->turnEffectsBattlerId = 0;
        gBattleStruct->wishPerishSongState = 0;
        gBattleStruct->wishPerishSongBattlerId = 0;
        gBattleStruct->turnCountersTracker = 0;
        gMoveResultFlags = 0;
    }
}

void BattleTurnPassed(void)
{
    s32 i;

    TurnValuesCleanUp(TRUE);
    if (gBattleOutcome == 0)
    {
        if (DoFieldEndTurnEffects())
            return;
        if (DoBattlerEndTurnEffects())
            return;
    }
    if (HandleFaintedMonActions())
        return;
    gBattleStruct->faintedActionsState = 0;
    if (HandleWishPerishSongOnTurnEnd())
        return;

    TurnValuesCleanUp(FALSE);
    gHitMarker &= ~(HITMARKER_NO_ATTACKSTRING);
    gHitMarker &= ~(HITMARKER_UNABLE_TO_USE_MOVE);
    gHitMarker &= ~(HITMARKER_x400000);
    gHitMarker &= ~(HITMARKER_x100000);
    gBattleScripting.animTurn = 0;
    gBattleScripting.animTargetsHit = 0;
    gBattleScripting.moveendState = 0;
    gBattleMoveDamage = 0;
    gMoveResultFlags = 0;

    for (i = 0; i < 5; i++)
        gBattleCommunication[i] = 0;

    if (gBattleOutcome != 0)
    {
        gCurrentActionFuncId = B_ACTION_FINISHED;
        gBattleMainFunc = RunTurnActionsFunctions;
        return;
    }

    if (gBattleResults.battleTurnCounter < 0xFF)
    {
        gBattleResults.battleTurnCounter++;
        gBattleStruct->arenaTurnCounter++;
    }

    for (i = 0; i < gBattlersCount; i++)
    {
        gChosenActionByBattler[i] = B_ACTION_NONE;
        gChosenMoveByBattler[i] = MOVE_NONE;
    }

    for (i = 0; i < MAX_BATTLERS_COUNT; i++)
        *(gBattleStruct->monToSwitchIntoId + i) = PARTY_SIZE;

    *(&gBattleStruct->absentBattlerFlags) = gAbsentBattlerFlags;
    BattlePutTextOnWindow(gText_EmptyString3, B_WIN_MSG);
    SetAiLogicDataForTurn(AI_DATA); // get assumed abilities, hold effects, etc of all battlers
    *(&gBattleStruct->field_91) = gAbsentBattlerFlags;
    BattlePutTextOnWindow(gText_EmptyString3, 0);
    gBattleMainFunc = HandleTurnActionSelectionState;
    gRandomTurnNumber = Random();

    if (gBattleTypeFlags & BATTLE_TYPE_PALACE)
        BattleScriptExecute(BattleScript_PalacePrintFlavorText);
    else if (gBattleTypeFlags & BATTLE_TYPE_ARENA && gBattleStruct->arenaTurnCounter == 0)
        BattleScriptExecute(BattleScript_ArenaTurnBeginning);
}

u8 IsRunningFromBattleImpossible(u32 battler)
{
    u8 holdEffect;
    u8 side;
    s32 i;

    if (gBattleMons[battler].item == ITEM_ENIGMA_BERRY_E_READER)
        holdEffect = gEnigmaBerries[battler].holdEffect;
    if (gBattleMons[gActiveBattler].item == ITEM_ENIGMA_BERRY)
        holdEffect = gEnigmaBerries[gActiveBattler].holdEffect;
    else
        holdEffect = ItemId_GetHoldEffect(gBattleMons[battler].item);

    gPotentialItemEffectBattler = battler;

    if (gBattleTypeFlags & BATTLE_TYPE_FIRST_BATTLE) // Cannot ever run from saving Birch's battle.
    {
        gBattleCommunication[MULTISTRING_CHOOSER] = B_MSG_DONT_LEAVE_BIRCH;
        return BATTLE_RUN_FORBIDDEN;
    }
    if (GetBattlerPosition(battler) == B_POSITION_PLAYER_RIGHT && WILD_DOUBLE_BATTLE
        && IsBattlerAlive(GetBattlerAtPosition(B_POSITION_PLAYER_LEFT))) // The second pokemon cannot run from a double wild battle, unless it's the only alive mon.
    {
        gBattleCommunication[MULTISTRING_CHOOSER] = B_MSG_CANT_ESCAPE;
        return BATTLE_RUN_FORBIDDEN;
    }

    if (holdEffect == HOLD_EFFECT_CAN_ALWAYS_RUN)
        return BATTLE_RUN_SUCCESS;
    if (B_GHOSTS_ESCAPE >= GEN_6 && IS_BATTLER_OF_TYPE(battler, TYPE_GHOST))
        return BATTLE_RUN_SUCCESS;
    if (gBattleTypeFlags & BATTLE_TYPE_LINK)
        return BATTLE_RUN_SUCCESS;
    if (GetBattlerAbility(battler) == ABILITY_RUN_AWAY)
        return BATTLE_RUN_SUCCESS;

    if ((i = IsAbilityPreventingEscape(battler)))
    if (holdEffect == HOLD_EFFECT_CAN_ALWAYS_RUN)
        return 0;
    if (gBattleTypeFlags & BATTLE_TYPE_LINK)
        return 0;
    if (gBattleMons[gActiveBattler].ability == ABILITY_RUN_AWAY)
        return 0;

    side = GetBattlerSide(gActiveBattler);

    for (i = 0; i < gBattlersCount; i++)
    {
        if (side != GetBattlerSide(i)
         && gBattleMons[i].ability == ABILITY_SHADOW_TAG)
        {
            gBattleScripting.battler = i;
            gLastUsedAbility = gBattleMons[i].ability;
            gBattleCommunication[MULTISTRING_CHOOSER] = B_MSG_PREVENTS_ESCAPE;
            return 2;
        }
        if (side != GetBattlerSide(i)
         && gBattleMons[gActiveBattler].ability != ABILITY_LEVITATE
         && !IS_BATTLER_OF_TYPE(gActiveBattler, TYPE_FLYING)
         && gBattleMons[i].ability == ABILITY_ARENA_TRAP)
        {
            gBattleScripting.battler = i;
            gLastUsedAbility = gBattleMons[i].ability;
            gBattleCommunication[MULTISTRING_CHOOSER] = B_MSG_PREVENTS_ESCAPE;
            return 2;
        }
    }
    i = AbilityBattleEffects(ABILITYEFFECT_CHECK_FIELD_EXCEPT_BATTLER, gActiveBattler, ABILITY_MAGNET_PULL, 0, 0);
    if (i != 0 && IS_BATTLER_OF_TYPE(gActiveBattler, TYPE_STEEL))
    {
        gBattleScripting.battler = i - 1;
        gLastUsedAbility = gBattleMons[i - 1].ability;
        gBattleCommunication[MULTISTRING_CHOOSER] = B_MSG_PREVENTS_ESCAPE;
        return 2;
    }

    if (!CanBattlerEscape(battler))
    if ((gBattleMons[gActiveBattler].status2 & (STATUS2_ESCAPE_PREVENTION | STATUS2_WRAPPED))
        || (gStatuses3[gActiveBattler] & STATUS3_ROOTED))
    {
        gBattleCommunication[MULTISTRING_CHOOSER] = B_MSG_CANT_ESCAPE;
        return 1;
    }
    if (gBattleTypeFlags & BATTLE_TYPE_FIRST_BATTLE)
    {
        gBattleCommunication[MULTISTRING_CHOOSER] = B_MSG_DONT_LEAVE_BIRCH;
        return 1;
    }
    return 0;
}

void SwitchPartyOrder(u32 battler)
{
    s32 i;
    u32 partyId1, partyId2;
    u8 partyId1;
    u8 partyId2;

    // gBattleStruct->field_60[battler][i]

    for (i = 0; i < (int)ARRAY_COUNT(gBattlePartyCurrentOrder); i++)
        gBattlePartyCurrentOrder[i] = *(battler * 3 + i + (u8*)(gBattleStruct->field_60));

    partyId1 = GetPartyIdFromBattlePartyId(gBattlerPartyIndexes[battler]);
    partyId2 = GetPartyIdFromBattlePartyId(*(gBattleStruct->monToSwitchIntoId + battler));
    SwitchPartyMonSlots(partyId1, partyId2);

    if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE)
    {
        for (i = 0; i < (int)ARRAY_COUNT(gBattlePartyCurrentOrder); i++)
        {
            *(battler * 3 + i + (u8*)(gBattleStruct->field_60)) = gBattlePartyCurrentOrder[i];
            *(BATTLE_PARTNER(battler) * 3 + i + (u8*)(gBattleStruct->field_60)) = gBattlePartyCurrentOrder[i];
        }
    }
    else
    {
        for (i = 0; i < (int)ARRAY_COUNT(gBattlePartyCurrentOrder); i++)
        {
            *(battler * 3 + i + (u8*)(gBattleStruct->field_60)) = gBattlePartyCurrentOrder[i];
        }
    }
}

enum
{
    STATE_TURN_START_RECORD,
    STATE_BEFORE_ACTION_CHOSEN,
    STATE_WAIT_ACTION_CHOSEN,
    STATE_WAIT_ACTION_CASE_CHOSEN,
    STATE_WAIT_ACTION_CONFIRMED_STANDBY,
    STATE_WAIT_ACTION_CONFIRMED,
    STATE_SELECTION_SCRIPT,
    STATE_WAIT_SET_BEFORE_ACTION,
    STATE_SELECTION_SCRIPT_MAY_RUN
};

static void HandleTurnActionSelectionState(void)
{
    s32 i, battler;

    gBattleCommunication[ACTIONS_CONFIRMED_COUNT] = 0;
    for (battler = 0; battler < gBattlersCount; battler++)
    {
        u32 position = GetBattlerPosition(battler);
        switch (gBattleCommunication[battler])
        {
        case STATE_TURN_START_RECORD: // Recorded battle related action on start of every turn.
            RecordedBattle_CopyBattlerMoves(battler);
            gBattleCommunication[battler] = STATE_BEFORE_ACTION_CHOSEN;

            // Do AI score computations here so we can use them in AI_TrySwitchOrUseItem
            if ((gBattleTypeFlags & BATTLE_TYPE_HAS_AI || IsWildMonSmart())
                    && (BattlerHasAi(battler) && !(gBattleTypeFlags & BATTLE_TYPE_PALACE)))
            {
                gBattleStruct->aiMoveOrAction[battler] = ComputeBattleAiScores(battler);
            }
            // fallthrough
            RecordedBattle_CopyBattlerMoves();
            gBattleCommunication[gActiveBattler] = STATE_BEFORE_ACTION_CHOSEN;
            break;
        case STATE_BEFORE_ACTION_CHOSEN: // Choose an action.
            *(gBattleStruct->monToSwitchIntoId + battler) = PARTY_SIZE;
            if (gBattleTypeFlags & BATTLE_TYPE_MULTI
                || (position & BIT_FLANK) == B_FLANK_LEFT
                || gBattleStruct->field_91 & gBitTable[GetBattlerAtPosition(BATTLE_PARTNER(position))]
                || gBattleCommunication[GetBattlerAtPosition(BATTLE_PARTNER(position))] == STATE_WAIT_ACTION_CONFIRMED)
            {
                if (gBattleStruct->absentBattlerFlags & gBitTable[battler])
                if (gBattleStruct->field_91 & gBitTable[gActiveBattler])
                {
                    gChosenActionByBattler[battler] = B_ACTION_NOTHING_FAINTED;
                    if (!(gBattleTypeFlags & BATTLE_TYPE_MULTI))
                        gBattleCommunication[battler] = STATE_WAIT_ACTION_CONFIRMED;
                    else
                        gBattleCommunication[battler] = STATE_WAIT_ACTION_CONFIRMED_STANDBY;
                }
                else
                {
                    if (gBattleMons[battler].status2 & STATUS2_MULTIPLETURNS
                        || gBattleMons[battler].status2 & STATUS2_RECHARGE)
                    {
                        gChosenActionByBattler[battler] = B_ACTION_USE_MOVE;
                        gBattleCommunication[battler] = STATE_WAIT_ACTION_CONFIRMED_STANDBY;
                    }
                    else if (WILD_DOUBLE_BATTLE
                             && position == B_POSITION_PLAYER_RIGHT
                             && (gBattleStruct->throwingPokeBall || gChosenActionByBattler[GetBattlerAtPosition(B_POSITION_PLAYER_LEFT)] == B_ACTION_RUN)
                             && gChosenActionByBattler[GetBattlerAtPosition(B_POSITION_PLAYER_LEFT)] != B_ACTION_NOTHING_FAINTED)
                    {
                        gBattleStruct->throwingPokeBall = FALSE;
                        gChosenActionByBattler[battler] = B_ACTION_NOTHING_FAINTED; // Not fainted, but it cannot move, because of the throwing ball.
                        gBattleCommunication[battler] = STATE_WAIT_ACTION_CONFIRMED_STANDBY;
                    }
                    else
                    {
                        gBattleStruct->itemPartyIndex[battler] = PARTY_SIZE;
                        BtlController_EmitChooseAction(battler, BUFFER_A, gChosenActionByBattler[0], gBattleResources->bufferB[0][1] | (gBattleResources->bufferB[0][2] << 8));
                        MarkBattlerForControllerExec(battler);
                        gBattleCommunication[battler]++;
                        BtlController_EmitChooseAction(0, gChosenActionByBattler[0], gBattleBufferB[0][1] | (gBattleBufferB[0][2] << 8));
                        MarkBattlerForControllerExec(gActiveBattler);
                        gBattleCommunication[gActiveBattler]++;
                    }
                }
            }
            break;
        case STATE_WAIT_ACTION_CHOSEN: // Try to perform an action.
            if (!(gBattleControllerExecFlags & ((gBitTable[battler]) | (0xF << 28) | (gBitTable[battler] << 4) | (gBitTable[battler] << 8) | (gBitTable[battler] << 12))))
            {
                RecordedBattle_SetBattlerAction(battler, gBattleResources->bufferB[battler][1]);
                gChosenActionByBattler[battler] = gBattleResources->bufferB[battler][1];
                RecordedBattle_SetBattlerAction(gActiveBattler, gBattleBufferB[gActiveBattler][1]);
                gChosenActionByBattler[gActiveBattler] = gBattleBufferB[gActiveBattler][1];

                switch (gBattleResources->bufferB[battler][1])
                switch (gBattleBufferB[gActiveBattler][1])
                {
                case B_ACTION_USE_MOVE:
                    if (AreAllMovesUnusable(battler))
                    {
                        gBattleCommunication[battler] = STATE_SELECTION_SCRIPT;
                        *(gBattleStruct->selectionScriptFinished + battler) = FALSE;
                        *(gBattleStruct->stateIdAfterSelScript + battler) = STATE_WAIT_ACTION_CONFIRMED_STANDBY;
                        *(gBattleStruct->moveTarget + battler) = gBattleResources->bufferB[battler][3];
                        gBattleCommunication[gActiveBattler] = STATE_SELECTION_SCRIPT;
                        *(gBattleStruct->selectionScriptFinished + gActiveBattler) = FALSE;
                        *(gBattleStruct->stateIdAfterSelScript + gActiveBattler) = STATE_WAIT_ACTION_CONFIRMED_STANDBY;
                        *(gBattleStruct->moveTarget + gActiveBattler) = gBattleBufferB[gActiveBattler][3];
                        return;
                    }
                    else if (gDisableStructs[battler].encoredMove != 0)
                    {
                        gChosenMoveByBattler[battler] = gDisableStructs[battler].encoredMove;
                        *(gBattleStruct->chosenMovePositions + battler) = gDisableStructs[battler].encoredMovePos;
                        gBattleCommunication[battler] = STATE_WAIT_ACTION_CONFIRMED_STANDBY;
                        return;
                    }
                    else
                    {
                        struct ChooseMoveStruct moveInfo;

                        moveInfo.zmove = gBattleStruct->zmove;
                        moveInfo.mega = gBattleStruct->mega;
                        moveInfo.species = gBattleMons[battler].species;
                        moveInfo.monType1 = gBattleMons[battler].type1;
                        moveInfo.monType2 = gBattleMons[battler].type2;
                        moveInfo.monType3 = gBattleMons[battler].type3;
                        moveInfo.species = gBattleMons[gActiveBattler].species;
                        moveInfo.monType1 = gBattleMons[gActiveBattler].type1;
                        moveInfo.monType2 = gBattleMons[gActiveBattler].type2;

                        for (i = 0; i < MAX_MON_MOVES; i++)
                        {
                            moveInfo.moves[i] = gBattleMons[battler].moves[i];
                            moveInfo.currentPp[i] = gBattleMons[battler].pp[i];
                            moveInfo.maxPp[i] = CalculatePPWithBonus(
                                                            gBattleMons[battler].moves[i],
                                                            gBattleMons[battler].ppBonuses,
                                                            i);
                        }

                        BtlController_EmitChooseMove(battler, BUFFER_A, (gBattleTypeFlags & BATTLE_TYPE_DOUBLE) != 0, FALSE, &moveInfo);
                        MarkBattlerForControllerExec(battler);
                        BtlController_EmitChooseMove(0, (gBattleTypeFlags & BATTLE_TYPE_DOUBLE) != 0, FALSE, &moveInfo);
                        MarkBattlerForControllerExec(gActiveBattler);
                    }
                    break;
                case B_ACTION_USE_ITEM:
                    if (FlagGet(B_FLAG_NO_BAG_USE))
                    {
                        RecordedBattle_ClearBattlerAction(battler, 1);
                        gSelectionBattleScripts[battler] = BattleScript_ActionSelectionItemsCantBeUsed;
                        gBattleCommunication[battler] = STATE_SELECTION_SCRIPT;
                        *(gBattleStruct->selectionScriptFinished + battler) = FALSE;
                        *(gBattleStruct->stateIdAfterSelScript + battler) = STATE_BEFORE_ACTION_CHOSEN;
                        return;
                    }

                    if (((gBattleTypeFlags & (BATTLE_TYPE_LINK
                    if (gBattleTypeFlags & (BATTLE_TYPE_LINK
                                            | BATTLE_TYPE_FRONTIER_NO_PYRAMID
                                            | BATTLE_TYPE_EREADER_TRAINER
                                            | BATTLE_TYPE_RECORDED_LINK))
                                            && !gTestRunnerEnabled)
                                            // Or if currently held by Sky Drop
                                            || gStatuses3[battler] & STATUS3_SKY_DROPPED)
                    {
                        RecordedBattle_ClearBattlerAction(battler, 1);
                        gSelectionBattleScripts[battler] = BattleScript_ActionSelectionItemsCantBeUsed;
                        gBattleCommunication[battler] = STATE_SELECTION_SCRIPT;
                        *(gBattleStruct->selectionScriptFinished + battler) = FALSE;
                        *(gBattleStruct->stateIdAfterSelScript + battler) = STATE_BEFORE_ACTION_CHOSEN;
                        return;
                    }
                    else
                    {
                        BtlController_EmitChooseItem(battler, BUFFER_A, gBattleStruct->battlerPartyOrders[battler]);
                        MarkBattlerForControllerExec(battler);
                        BtlController_EmitChooseItem(0, gBattleStruct->field_60[gActiveBattler]);
                        MarkBattlerForControllerExec(gActiveBattler);
                    }
                    break;
                case B_ACTION_SWITCH:
                    *(gBattleStruct->battlerPartyIndexes + battler) = gBattlerPartyIndexes[battler];
                    if (gBattleTypeFlags & BATTLE_TYPE_ARENA
                        || !CanBattlerEscape(battler))
                    *(gBattleStruct->field_58 + gActiveBattler) = gBattlerPartyIndexes[gActiveBattler];
                    if (gBattleMons[gActiveBattler].status2 & (STATUS2_WRAPPED | STATUS2_ESCAPE_PREVENTION)
                        || gBattleTypeFlags & BATTLE_TYPE_ARENA
                        || gStatuses3[gActiveBattler] & STATUS3_ROOTED)
                    {
                        BtlController_EmitChoosePokemon(battler, BUFFER_A, PARTY_ACTION_CANT_SWITCH, PARTY_SIZE, ABILITY_NONE, gBattleStruct->battlerPartyOrders[battler]);
                        BtlController_EmitChoosePokemon(0, PARTY_ACTION_CANT_SWITCH, PARTY_SIZE, ABILITY_NONE, gBattleStruct->field_60[gActiveBattler]);
                    }
                    else if (ItemId_GetHoldEffect(gBattleMons[battler].item) != HOLD_EFFECT_SHED_SHELL
                      && (i = IsAbilityPreventingEscape(battler)))   // must be last to keep i value integrity
                    else if ((i = ABILITY_ON_OPPOSING_FIELD(gActiveBattler, ABILITY_SHADOW_TAG))
                             || ((i = ABILITY_ON_OPPOSING_FIELD(gActiveBattler, ABILITY_ARENA_TRAP))
                                 && !IS_BATTLER_OF_TYPE(gActiveBattler, TYPE_FLYING)
                                 && gBattleMons[gActiveBattler].ability != ABILITY_LEVITATE)
                             || ((i = AbilityBattleEffects(ABILITYEFFECT_CHECK_FIELD_EXCEPT_BATTLER, gActiveBattler, ABILITY_MAGNET_PULL, 0, 0))
                                 && IS_BATTLER_OF_TYPE(gActiveBattler, TYPE_STEEL)))
                    {
                        BtlController_EmitChoosePokemon(battler, BUFFER_A, ((i - 1) << 4) | PARTY_ACTION_ABILITY_PREVENTS, PARTY_SIZE, gBattleMons[i - 1].ability, gBattleStruct->battlerPartyOrders[battler]);
                        BtlController_EmitChoosePokemon(0, ((i - 1) << 4) | PARTY_ACTION_ABILITY_PREVENTS, PARTY_SIZE, gLastUsedAbility, gBattleStruct->field_60[gActiveBattler]);
                    }
                    else
                    {
                        if (battler == 2 && gChosenActionByBattler[0] == B_ACTION_SWITCH)
                            BtlController_EmitChoosePokemon(battler, BUFFER_A, PARTY_ACTION_CHOOSE_MON, *(gBattleStruct->monToSwitchIntoId + 0), ABILITY_NONE, gBattleStruct->battlerPartyOrders[battler]);
                        else if (battler == 3 && gChosenActionByBattler[1] == B_ACTION_SWITCH)
                            BtlController_EmitChoosePokemon(battler, BUFFER_A, PARTY_ACTION_CHOOSE_MON, *(gBattleStruct->monToSwitchIntoId + 1), ABILITY_NONE, gBattleStruct->battlerPartyOrders[battler]);
                        if (gActiveBattler == 2 && gChosenActionByBattler[0] == B_ACTION_SWITCH)
                            BtlController_EmitChoosePokemon(0, PARTY_ACTION_CHOOSE_MON, *(gBattleStruct->monToSwitchIntoId + 0), ABILITY_NONE, gBattleStruct->field_60[gActiveBattler]);
                        else if (gActiveBattler == 3 && gChosenActionByBattler[1] == B_ACTION_SWITCH)
                            BtlController_EmitChoosePokemon(0, PARTY_ACTION_CHOOSE_MON, *(gBattleStruct->monToSwitchIntoId + 1), ABILITY_NONE, gBattleStruct->field_60[gActiveBattler]);
                        else
                            BtlController_EmitChoosePokemon(battler, BUFFER_A, PARTY_ACTION_CHOOSE_MON, PARTY_SIZE, ABILITY_NONE, gBattleStruct->battlerPartyOrders[battler]);
                            BtlController_EmitChoosePokemon(0, PARTY_ACTION_CHOOSE_MON, PARTY_SIZE, ABILITY_NONE, gBattleStruct->field_60[gActiveBattler]);
                    }
                    MarkBattlerForControllerExec(battler);
                    break;
                case B_ACTION_SAFARI_BALL:
                    if (IsPlayerPartyAndPokemonStorageFull())
                    {
                        gSelectionBattleScripts[battler] = BattleScript_PrintFullBox;
                        gBattleCommunication[battler] = STATE_SELECTION_SCRIPT;
                        *(gBattleStruct->selectionScriptFinished + battler) = FALSE;
                        *(gBattleStruct->stateIdAfterSelScript + battler) = STATE_BEFORE_ACTION_CHOSEN;
                        return;
                    }
                    break;
                case B_ACTION_SAFARI_POKEBLOCK:
                    BtlController_EmitChooseItem(battler, BUFFER_A, gBattleStruct->battlerPartyOrders[battler]);
                    MarkBattlerForControllerExec(battler);
                    BtlController_EmitChooseItem(0, gBattleStruct->field_60[gActiveBattler]);
                    MarkBattlerForControllerExec(gActiveBattler);
                    break;
                case B_ACTION_CANCEL_PARTNER:
                    gBattleCommunication[battler] = STATE_WAIT_SET_BEFORE_ACTION;
                    gBattleCommunication[GetBattlerAtPosition(BATTLE_PARTNER(GetBattlerPosition(battler)))] = STATE_BEFORE_ACTION_CHOSEN;
                    RecordedBattle_ClearBattlerAction(battler, 1);
                    if (gBattleMons[GetBattlerAtPosition(BATTLE_PARTNER(GetBattlerPosition(battler)))].status2 & STATUS2_MULTIPLETURNS
                        || gBattleMons[GetBattlerAtPosition(BATTLE_PARTNER(GetBattlerPosition(battler)))].status2 & STATUS2_RECHARGE)
                    {
                        BtlController_EmitEndBounceEffect(battler, BUFFER_A);
                        MarkBattlerForControllerExec(battler);
                        BtlController_EmitEndBounceEffect(0);
                        MarkBattlerForControllerExec(gActiveBattler);
                        return;
                    }
                    else if (gChosenActionByBattler[GetBattlerAtPosition(BATTLE_PARTNER(GetBattlerPosition(battler)))] == B_ACTION_SWITCH)
                    {
                        RecordedBattle_ClearBattlerAction(GetBattlerAtPosition(BATTLE_PARTNER(GetBattlerPosition(battler))), 2);
                    }
                    else if (gChosenActionByBattler[GetBattlerAtPosition(BATTLE_PARTNER(GetBattlerPosition(battler)))] == B_ACTION_RUN)
                    {
                        RecordedBattle_ClearBattlerAction(GetBattlerAtPosition(BATTLE_PARTNER(GetBattlerPosition(battler))), 1);
                    }
                    else if (gChosenActionByBattler[GetBattlerAtPosition(BATTLE_PARTNER(GetBattlerPosition(battler)))] == B_ACTION_USE_MOVE
                             && (gProtectStructs[GetBattlerAtPosition(BATTLE_PARTNER(GetBattlerPosition(battler)))].noValidMoves
                                || gDisableStructs[GetBattlerAtPosition(BATTLE_PARTNER(GetBattlerPosition(battler)))].encoredMove))
                    {
                        RecordedBattle_ClearBattlerAction(GetBattlerAtPosition(BATTLE_PARTNER(GetBattlerPosition(battler))), 1);
                    }
                    else if (gBattleTypeFlags & BATTLE_TYPE_PALACE
                             && gChosenActionByBattler[GetBattlerAtPosition(BATTLE_PARTNER(GetBattlerPosition(battler)))] == B_ACTION_USE_MOVE)
                    {
                        gRngValue = gBattlePalaceMoveSelectionRngValue;
                        RecordedBattle_ClearBattlerAction(GetBattlerAtPosition(BATTLE_PARTNER(GetBattlerPosition(battler))), 1);
                    }
                    else
                    {
                        RecordedBattle_ClearBattlerAction(GetBattlerAtPosition(BATTLE_PARTNER(GetBattlerPosition(battler))), 3);
                    }

                    gBattleStruct->mega.toEvolve &= ~(gBitTable[BATTLE_PARTNER(GetBattlerPosition(battler))]);
                    gBattleStruct->burst.toBurst &= ~(gBitTable[BATTLE_PARTNER(GetBattlerPosition(battler))]);
                    gBattleStruct->dynamax.toDynamax &= ~(gBitTable[BATTLE_PARTNER(GetBattlerPosition(battler))]);
                    gBattleStruct->dynamax.usingMaxMove[BATTLE_PARTNER(GetBattlerPosition(battler))] = FALSE;
                    gBattleStruct->zmove.toBeUsed[BATTLE_PARTNER(GetBattlerPosition(battler))] = MOVE_NONE;
                    BtlController_EmitEndBounceEffect(battler, BUFFER_A);
                    MarkBattlerForControllerExec(battler);
                    BtlController_EmitEndBounceEffect(0);
                    MarkBattlerForControllerExec(gActiveBattler);
                    return;
                case B_ACTION_DEBUG:
                    BtlController_EmitDebugMenu(battler, BUFFER_A);
                    MarkBattlerForControllerExec(battler);
                    break;
                }

                if (gBattleTypeFlags & BATTLE_TYPE_TRAINER
                    && gBattleTypeFlags & (BATTLE_TYPE_FRONTIER | BATTLE_TYPE_TRAINER_HILL)
                    && gBattleResources->bufferB[battler][1] == B_ACTION_RUN)
                    && gBattleBufferB[gActiveBattler][1] == B_ACTION_RUN)
                {
                    gSelectionBattleScripts[battler] = BattleScript_AskIfWantsToForfeitMatch;
                    gBattleCommunication[battler] = STATE_SELECTION_SCRIPT_MAY_RUN;
                    *(gBattleStruct->selectionScriptFinished + battler) = FALSE;
                    *(gBattleStruct->stateIdAfterSelScript + battler) = STATE_BEFORE_ACTION_CHOSEN;
                    return;
                }
                else if (gBattleTypeFlags & BATTLE_TYPE_TRAINER
                         && !(gBattleTypeFlags & (BATTLE_TYPE_LINK | BATTLE_TYPE_RECORDED_LINK))
                         && gBattleResources->bufferB[battler][1] == B_ACTION_RUN)
                         && gBattleBufferB[gActiveBattler][1] == B_ACTION_RUN)
                {
                    BattleScriptExecute(BattleScript_PrintCantRunFromTrainer);
                    gBattleCommunication[battler] = STATE_BEFORE_ACTION_CHOSEN;
                }
                else if (IsRunningFromBattleImpossible(battler) != BATTLE_RUN_SUCCESS
                         && gBattleResources->bufferB[battler][1] == B_ACTION_RUN)
                else if (IsRunningFromBattleImpossible()
                         && gBattleBufferB[gActiveBattler][1] == B_ACTION_RUN)
                {
                    gSelectionBattleScripts[battler] = BattleScript_PrintCantEscapeFromBattle;
                    gBattleCommunication[battler] = STATE_SELECTION_SCRIPT;
                    *(gBattleStruct->selectionScriptFinished + battler) = FALSE;
                    *(gBattleStruct->stateIdAfterSelScript + battler) = STATE_BEFORE_ACTION_CHOSEN;
                    return;
                }
                else
                {
                    gBattleCommunication[battler]++;
                }
            }
            break;
        case STATE_WAIT_ACTION_CASE_CHOSEN:
            if (!(gBattleControllerExecFlags & ((gBitTable[battler]) | (0xF << 28) | (gBitTable[battler] << 4) | (gBitTable[battler] << 8) | (gBitTable[battler] << 12))))
            {
                switch (gChosenActionByBattler[battler])
                {
                case B_ACTION_USE_MOVE:
                    switch (gBattleResources->bufferB[battler][1])
                    switch (gBattleBufferB[gActiveBattler][1])
                    {
                    case 3:
                    case 4:
                    case 5:
                    case 6:
                    case 7:
                    case 8:
                    case 9:
                        gChosenActionByBattler[battler] = gBattleResources->bufferB[battler][1];
                        gChosenActionByBattler[gActiveBattler] = gBattleBufferB[gActiveBattler][1];
                        return;
                    case 15:
                        gChosenActionByBattler[battler] = B_ACTION_SWITCH;
                        UpdateBattlerPartyOrdersOnSwitch(battler);
                        gChosenActionByBattler[gActiveBattler] = B_ACTION_SWITCH;
                        sub_803CDF8();
                        return;
                    default:
                        RecordedBattle_CheckMovesetChanges(B_RECORD_MODE_PLAYBACK);
                        if ((gBattleResources->bufferB[battler][2] | (gBattleResources->bufferB[battler][3] << 8)) == 0xFFFF)
                        sub_818603C(2);
                        if ((gBattleBufferB[gActiveBattler][2] | (gBattleBufferB[gActiveBattler][3] << 8)) == 0xFFFF)
                        {
                            gBattleCommunication[battler] = STATE_BEFORE_ACTION_CHOSEN;
                            RecordedBattle_ClearBattlerAction(battler, 1);
                        }
                        else if (TrySetCantSelectMoveBattleScript(battler))
                        {
                            RecordedBattle_ClearBattlerAction(battler, 1);
                            gBattleCommunication[battler] = STATE_SELECTION_SCRIPT;
                            *(gBattleStruct->selectionScriptFinished + battler) = FALSE;
                            gBattleResources->bufferB[battler][1] = B_ACTION_USE_MOVE;
                            *(gBattleStruct->stateIdAfterSelScript + battler) = STATE_WAIT_ACTION_CHOSEN;
                            RecordedBattle_ClearBattlerAction(gActiveBattler, 1);
                            gBattleCommunication[gActiveBattler] = STATE_SELECTION_SCRIPT;
                            *(gBattleStruct->selectionScriptFinished + gActiveBattler) = FALSE;
                            gBattleBufferB[gActiveBattler][1] = B_ACTION_USE_MOVE;
                            *(gBattleStruct->stateIdAfterSelScript + gActiveBattler) = STATE_WAIT_ACTION_CHOSEN;
                            return;
                        }
                        else
                        {
                            if (!(gBattleTypeFlags & BATTLE_TYPE_PALACE))
                            {
                                RecordedBattle_SetBattlerAction(battler, gBattleResources->bufferB[battler][2]);
                                RecordedBattle_SetBattlerAction(battler, gBattleResources->bufferB[battler][3]);
                                RecordedBattle_SetBattlerAction(gActiveBattler, gBattleBufferB[gActiveBattler][2]);
                                RecordedBattle_SetBattlerAction(gActiveBattler, gBattleBufferB[gActiveBattler][3]);
                            }

                            // Get the chosen move position (and thus the chosen move) and target from the returned buffer.
                            gBattleStruct->chosenMovePositions[battler] = gBattleResources->bufferB[battler][2] & ~(RET_MEGA_EVOLUTION | RET_ULTRA_BURST | RET_DYNAMAX);
                            gChosenMoveByBattler[battler] = gBattleMons[battler].moves[gBattleStruct->chosenMovePositions[battler]];
                            gBattleStruct->moveTarget[battler] = gBattleResources->bufferB[battler][3];

                            // Check to see if any gimmicks need to be prepared.
                            if (gBattleResources->bufferB[battler][2] & RET_MEGA_EVOLUTION)
                                gBattleStruct->mega.toEvolve |= gBitTable[battler];
                            else if (gBattleResources->bufferB[battler][2] & RET_ULTRA_BURST)
                                gBattleStruct->burst.toBurst |= gBitTable[battler];
                            else if (gBattleResources->bufferB[battler][2] & RET_DYNAMAX)
                                gBattleStruct->dynamax.toDynamax |= gBitTable[battler];

                            // Max Move check
                            if (ShouldUseMaxMove(battler, gChosenMoveByBattler[battler]))
                            {
                                gBattleStruct->dynamax.baseMove[battler] = gBattleMons[battler].moves[gBattleStruct->chosenMovePositions[battler]];
                                gBattleStruct->dynamax.usingMaxMove[battler] = TRUE;
                            }
                            gBattleCommunication[battler]++;

                            if (gTestRunnerEnabled)
                            {
                                TestRunner_Battle_CheckChosenMove(battler, gChosenMoveByBattler[battler], gBattleStruct->moveTarget[battler]);
                            }
                            *(gBattleStruct->chosenMovePositions + gActiveBattler) = gBattleBufferB[gActiveBattler][2];
                            gChosenMoveByBattler[gActiveBattler] = gBattleMons[gActiveBattler].moves[*(gBattleStruct->chosenMovePositions + gActiveBattler)];
                            *(gBattleStruct->moveTarget + gActiveBattler) = gBattleBufferB[gActiveBattler][3];
                            gBattleCommunication[gActiveBattler]++;
                        }
                        break;
                    }
                    break;
                case B_ACTION_USE_ITEM:
                    if ((gBattleResources->bufferB[battler][1] | (gBattleResources->bufferB[battler][2] << 8)) == 0)
                    if ((gBattleBufferB[gActiveBattler][1] | (gBattleBufferB[gActiveBattler][2] << 8)) == 0)
                    {
                        gBattleCommunication[battler] = STATE_BEFORE_ACTION_CHOSEN;
                    }
                    else
                    {
                        gLastUsedItem = (gBattleResources->bufferB[battler][1] | (gBattleResources->bufferB[battler][2] << 8));
                        if (ItemId_GetPocket(gLastUsedItem) == POCKET_POKE_BALLS)
                            gBattleStruct->throwingPokeBall = TRUE;
                        gBattleCommunication[battler]++;
                        gLastUsedItem = (gBattleBufferB[gActiveBattler][1] | (gBattleBufferB[gActiveBattler][2] << 8));
                        gBattleCommunication[gActiveBattler]++;
                    }
                    break;
                case B_ACTION_SWITCH:
                    if (gBattleResources->bufferB[battler][1] == PARTY_SIZE)
                    if (gBattleBufferB[gActiveBattler][1] == PARTY_SIZE)
                    {
                        gBattleCommunication[battler] = STATE_BEFORE_ACTION_CHOSEN;
                        RecordedBattle_ClearBattlerAction(battler, 1);
                    }
                    else
                    {
                        UpdateBattlerPartyOrdersOnSwitch(battler);
                        gBattleCommunication[battler]++;
                        sub_803CDF8();
                        gBattleCommunication[gActiveBattler]++;
                    }
                    break;
                case B_ACTION_RUN:
                    gHitMarker |= HITMARKER_RUN;
                    gBattleCommunication[battler]++;
                    break;
                case B_ACTION_SAFARI_WATCH_CAREFULLY:
                    gBattleCommunication[battler]++;
                    break;
                case B_ACTION_SAFARI_BALL:
                    gBattleCommunication[battler]++;
                    break;
                case B_ACTION_THROW_BALL:
                    gBattleStruct->throwingPokeBall = TRUE;
                    gBattleCommunication[battler]++;
                    break;
                case B_ACTION_SAFARI_POKEBLOCK:
                    if ((gBattleResources->bufferB[battler][1] | (gBattleResources->bufferB[battler][2] << 8)) != 0)
                        gBattleCommunication[battler]++;
                    if ((gBattleBufferB[gActiveBattler][1] | (gBattleBufferB[gActiveBattler][2] << 8)) != 0)
                    {
                        gBattleCommunication[gActiveBattler]++;
                    }
                    else
                        gBattleCommunication[battler] = STATE_BEFORE_ACTION_CHOSEN;
                    {
                        gBattleCommunication[gActiveBattler] = STATE_BEFORE_ACTION_CHOSEN;
                    }
                    break;
                case B_ACTION_SAFARI_GO_NEAR:
                    gBattleCommunication[battler]++;
                    break;
                case B_ACTION_SAFARI_RUN:
                    gHitMarker |= HITMARKER_RUN;
                    gBattleCommunication[battler]++;
                    break;
                case B_ACTION_WALLY_THROW:
                    gBattleCommunication[battler]++;
                    break;
                case B_ACTION_DEBUG:
                    gBattleCommunication[battler] = STATE_BEFORE_ACTION_CHOSEN;
                    break;
                }
            }
            break;
        case STATE_WAIT_ACTION_CONFIRMED_STANDBY:
            if (!(gBattleControllerExecFlags & ((gBitTable[battler])
            if (!(gBattleControllerExecFlags & ((gBitTable[gActiveBattler]) 
                                                | (0xF << 28)
                                                | (gBitTable[battler] << 4)
                                                | (gBitTable[battler] << 8)
                                                | (gBitTable[battler] << 12))))
                                                | (gBitTable[gActiveBattler] << 4) 
                                                | (gBitTable[gActiveBattler] << 8) 
                                                | (gBitTable[gActiveBattler] << 12))))
            {
                if (AllAtActionConfirmed())
                    i = TRUE;
                else
                    i = FALSE;

                if (((gBattleTypeFlags & (BATTLE_TYPE_MULTI | BATTLE_TYPE_DOUBLE)) != BATTLE_TYPE_DOUBLE)
                    || (position & BIT_FLANK) != B_FLANK_LEFT
                    || (*(&gBattleStruct->field_91) & gBitTable[GetBattlerAtPosition(position ^ BIT_FLANK)]))
                {
                    BtlController_EmitLinkStandbyMsg(battler, BUFFER_A, LINK_STANDBY_MSG_STOP_BOUNCE, i);
                    BtlController_EmitLinkStandbyMsg(0, 0, i);
                }
                else
                {
                    BtlController_EmitLinkStandbyMsg(battler, BUFFER_A, LINK_STANDBY_STOP_BOUNCE_ONLY, i);
                    BtlController_EmitLinkStandbyMsg(0, 1, i);
                }
                MarkBattlerForControllerExec(battler);
                gBattleCommunication[battler]++;
            }
            break;
        case STATE_WAIT_ACTION_CONFIRMED:
            if (!(gBattleControllerExecFlags & ((gBitTable[battler]) | (0xF << 28) | (gBitTable[battler] << 4) | (gBitTable[battler] << 8) | (gBitTable[battler] << 12))))
            {
                gBattleCommunication[ACTIONS_CONFIRMED_COUNT]++;
            }
            break;
        case STATE_SELECTION_SCRIPT:
            if (*(gBattleStruct->selectionScriptFinished + battler))
            {
                gBattleCommunication[battler] = *(gBattleStruct->stateIdAfterSelScript + battler);
            }
            else
            {
                gBattlerAttacker = battler;
                gBattlescriptCurrInstr = gSelectionBattleScripts[battler];
                if (!(gBattleControllerExecFlags & ((gBitTable[battler]) | (0xF << 28) | (gBitTable[battler] << 4) | (gBitTable[battler] << 8) | (gBitTable[battler] << 12))))
                {
                    gBattleScriptingCommandsTable[gBattlescriptCurrInstr[0]]();
                }
                gSelectionBattleScripts[battler] = gBattlescriptCurrInstr;
            }
            break;
        case STATE_WAIT_SET_BEFORE_ACTION:
            if (!(gBattleControllerExecFlags & ((gBitTable[battler]) | (0xF << 28) | (gBitTable[battler] << 4) | (gBitTable[battler] << 8) | (gBitTable[battler] << 12))))
            {
                gBattleCommunication[battler] = STATE_BEFORE_ACTION_CHOSEN;
            }
            break;
        case STATE_SELECTION_SCRIPT_MAY_RUN:
            if (*(gBattleStruct->selectionScriptFinished + battler))
            {
                if (gBattleResources->bufferB[battler][1] == B_ACTION_NOTHING_FAINTED)
                if (gBattleBufferB[gActiveBattler][1] == B_ACTION_NOTHING_FAINTED)
                {
                    gHitMarker |= HITMARKER_RUN;
                    gChosenActionByBattler[battler] = B_ACTION_RUN;
                    gBattleCommunication[battler] = STATE_WAIT_ACTION_CONFIRMED_STANDBY;
                }
                else
                {
                    RecordedBattle_ClearBattlerAction(battler, 1);
                    gBattleCommunication[battler] = *(gBattleStruct->stateIdAfterSelScript + battler);
                }
            }
            else
            {
                gBattlerAttacker = battler;
                gBattlescriptCurrInstr = gSelectionBattleScripts[battler];
                if (!(gBattleControllerExecFlags & ((gBitTable[battler]) | (0xF << 28) | (gBitTable[battler] << 4) | (gBitTable[battler] << 8) | (gBitTable[battler] << 12))))
                {
                    gBattleScriptingCommandsTable[gBattlescriptCurrInstr[0]]();
                }
                gSelectionBattleScripts[battler] = gBattlescriptCurrInstr;
            }
            break;
        }
    }

    // Check if everyone chose actions.
    if (gBattleCommunication[ACTIONS_CONFIRMED_COUNT] == gBattlersCount)
    {
        RecordedBattle_CheckMovesetChanges(B_RECORD_MODE_RECORDING);

        if (WILD_DOUBLE_BATTLE
            && gBattleStruct->throwingPokeBall
            && gChosenActionByBattler[GetBattlerAtPosition(B_POSITION_PLAYER_RIGHT)] != B_ACTION_NOTHING_FAINTED)
        {
            // if we choose to throw a ball with our second mon, skip the action of the first
            // (if we have chosen throw ball with first, second's is already skipped)
            // if throwing a ball in a wild battle with an in-game partner, skip partner's turn when throwing a ball
            if (gBattleTypeFlags & BATTLE_TYPE_INGAME_PARTNER)
                gChosenActionByBattler[GetBattlerAtPosition(B_POSITION_PLAYER_RIGHT)] = B_ACTION_NOTHING_FAINTED;
            else
                gChosenActionByBattler[GetBattlerAtPosition(B_POSITION_PLAYER_LEFT)] = B_ACTION_NOTHING_FAINTED;
        }

        sub_818603C(1);
        gBattleMainFunc = SetActionsAndBattlersTurnOrder;

        if (gBattleTypeFlags & BATTLE_TYPE_INGAME_PARTNER)
        {
            for (i = 0; i < gBattlersCount; i++)
            {
                if (gChosenActionByBattler[i] == B_ACTION_SWITCH)
                    SwitchPartyOrderInGameMulti(i, *(gBattleStruct->monToSwitchIntoId + i));
            }
        }
    }
}

static bool8 AllAtActionConfirmed(void)
{
    s32 i, count;

    for (count = 0, i = 0; i < gBattlersCount; i++)
    {
        if (gBattleCommunication[i] == STATE_WAIT_ACTION_CONFIRMED)
            count++;
    }

    if (count + 1 == gBattlersCount)
        return TRUE;
    else
        return FALSE;
}

static void UpdateBattlerPartyOrdersOnSwitch(u32 battler)
static void sub_803CDF8(void)
{
    gBattleStruct->monToSwitchIntoId[battler] = gBattleResources->bufferB[battler][1];
    RecordedBattle_SetBattlerAction(battler, gBattleResources->bufferB[battler][1]);
    *(gBattleStruct->monToSwitchIntoId + gActiveBattler) = gBattleBufferB[gActiveBattler][1];
    RecordedBattle_SetBattlerAction(gActiveBattler, gBattleBufferB[gActiveBattler][1]);

    if (gBattleTypeFlags & BATTLE_TYPE_LINK && gBattleTypeFlags & BATTLE_TYPE_MULTI)
    {
        *(battler * 3 + (u8 *)(gBattleStruct->battlerPartyOrders) + 0) &= 0xF;
        *(battler * 3 + (u8 *)(gBattleStruct->battlerPartyOrders) + 0) |= (gBattleResources->bufferB[battler][2] & 0xF0);
        *(battler * 3 + (u8 *)(gBattleStruct->battlerPartyOrders) + 1) = gBattleResources->bufferB[battler][3];
        *(gActiveBattler * 3 + (u8*)(gBattleStruct->field_60) + 0) &= 0xF;
        *(gActiveBattler * 3 + (u8*)(gBattleStruct->field_60) + 0) |= (gBattleBufferB[gActiveBattler][2] & 0xF0);
        *(gActiveBattler * 3 + (u8*)(gBattleStruct->field_60) + 1) = gBattleBufferB[gActiveBattler][3];

        *((BATTLE_PARTNER(battler)) * 3 + (u8 *)(gBattleStruct->battlerPartyOrders) + 0) &= (0xF0);
        *((BATTLE_PARTNER(battler)) * 3 + (u8 *)(gBattleStruct->battlerPartyOrders) + 0) |= (gBattleResources->bufferB[battler][2] & 0xF0) >> 4;
        *((BATTLE_PARTNER(battler)) * 3 + (u8 *)(gBattleStruct->battlerPartyOrders) + 2) = gBattleResources->bufferB[battler][3];
        *((gActiveBattler ^ BIT_FLANK) * 3 + (u8*)(gBattleStruct->field_60) + 0) &= (0xF0);
        *((gActiveBattler ^ BIT_FLANK) * 3 + (u8*)(gBattleStruct->field_60) + 0) |= (gBattleBufferB[gActiveBattler][2] & 0xF0) >> 4;
        *((gActiveBattler ^ BIT_FLANK) * 3 + (u8*)(gBattleStruct->field_60) + 2) = gBattleBufferB[gActiveBattler][3];
    }
}

void SwapTurnOrder(u8 id1, u8 id2)
{
    u32 temp;

    SWAP(gActionsByTurnOrder[id1], gActionsByTurnOrder[id2], temp);
    SWAP(gBattlerByTurnOrder[id1], gBattlerByTurnOrder[id2], temp);
}

// For AI, so it doesn't 'cheat' by knowing player's ability
u32 GetBattlerTotalSpeedStatArgs(u32 battler, u32 ability, u32 holdEffect)
u8 GetWhoStrikesFirst(u8 battler1, u8 battler2, bool8 ignoreChosenMoves)
{
    u32 speed = gBattleMons[battler].speed;
    u32 highestStat = GetHighestStatId(battler);

    // weather abilities
    u8 strikesFirst = 0;
    u8 speedMultiplierBattler1 = 0, speedMultiplierBattler2 = 0;
    u32 speedBattler1 = 0, speedBattler2 = 0;
    u8 holdEffect = 0;
    u8 holdEffectParam = 0;
    u16 moveBattler1 = 0, moveBattler2 = 0;

    if (WEATHER_HAS_EFFECT)
    {
        if (ability == ABILITY_SWIFT_SWIM       && holdEffect != HOLD_EFFECT_UTILITY_UMBRELLA && gBattleWeather & B_WEATHER_RAIN)
            speed *= 2;
        else if (ability == ABILITY_CHLOROPHYLL && holdEffect != HOLD_EFFECT_UTILITY_UMBRELLA && gBattleWeather & B_WEATHER_SUN)
            speed *= 2;
        else if (ability == ABILITY_SAND_RUSH   && gBattleWeather & B_WEATHER_SANDSTORM)
            speed *= 2;
        else if (ability == ABILITY_SLUSH_RUSH  && (gBattleWeather & (B_WEATHER_HAIL | B_WEATHER_SNOW)))
            speed *= 2;
    }

    // other abilities
    if (ability == ABILITY_QUICK_FEET && gBattleMons[battler].status1 & STATUS1_ANY)
        speed = (speed * 150) / 100;
    else if (ability == ABILITY_SURGE_SURFER && gFieldStatuses & STATUS_FIELD_ELECTRIC_TERRAIN)
        speed *= 2;
    else if (ability == ABILITY_SLOW_START && gDisableStructs[battler].slowStartTimer != 0)
        speed /= 2;
    else if (ability == ABILITY_PROTOSYNTHESIS && gBattleWeather & B_WEATHER_SUN && highestStat == STAT_SPEED)
        speed = (speed * 150) / 100;
    else if (ability == ABILITY_QUARK_DRIVE && gFieldStatuses & STATUS_FIELD_ELECTRIC_TERRAIN && highestStat == STAT_SPEED)
        speed = (speed * 150) / 100;

    // stat stages
    speed *= gStatStageRatios[gBattleMons[battler].statStages[STAT_SPEED]][0];
    speed /= gStatStageRatios[gBattleMons[battler].statStages[STAT_SPEED]][1];

    // player's badge boost
        if ((gBattleMons[battler1].ability == ABILITY_SWIFT_SWIM && gBattleWeather & WEATHER_RAIN_ANY)
            || (gBattleMons[battler1].ability == ABILITY_CHLOROPHYLL && gBattleWeather & WEATHER_SUN_ANY))
            speedMultiplierBattler1 = 2;
        else
            speedMultiplierBattler1 = 1;

        if ((gBattleMons[battler2].ability == ABILITY_SWIFT_SWIM && gBattleWeather & WEATHER_RAIN_ANY)
            || (gBattleMons[battler2].ability == ABILITY_CHLOROPHYLL && gBattleWeather & WEATHER_SUN_ANY))
            speedMultiplierBattler2 = 2;
        else
            speedMultiplierBattler2 = 1;
    }
    else
    {
        speedMultiplierBattler1 = 1;
        speedMultiplierBattler2 = 1;
    }

    speedBattler1 = (gBattleMons[battler1].speed * speedMultiplierBattler1)
                * (gStatStageRatios[gBattleMons[battler1].statStages[STAT_SPEED]][0])
                / (gStatStageRatios[gBattleMons[battler1].statStages[STAT_SPEED]][1]);

    if (gBattleMons[battler1].item == ITEM_ENIGMA_BERRY)
    {
        holdEffect = gEnigmaBerries[battler1].holdEffect;
        holdEffectParam = gEnigmaBerries[battler1].holdEffectParam;
    }
    else
    {
        holdEffect = ItemId_GetHoldEffect(gBattleMons[battler1].item);
        holdEffectParam = ItemId_GetHoldEffectParam(gBattleMons[battler1].item);
    }

    // badge boost
    if (!(gBattleTypeFlags & (BATTLE_TYPE_LINK | BATTLE_TYPE_RECORDED_LINK | BATTLE_TYPE_FRONTIER))
        && ShouldGetStatBadgeBoost(FLAG_BADGE03_GET, battler)
        && GetBattlerSide(battler) == B_SIDE_PLAYER)
        && FlagGet(FLAG_BADGE03_GET)
        && GetBattlerSide(battler1) == B_SIDE_PLAYER)
    {
        speedBattler1 = (speedBattler1 * 110) / 100;
    }

    // item effects
    if (holdEffect == HOLD_EFFECT_MACHO_BRACE || holdEffect == HOLD_EFFECT_POWER_ITEM)
        speed /= 2;
    else if (holdEffect == HOLD_EFFECT_IRON_BALL)
        speed /= 2;
    else if (holdEffect == HOLD_EFFECT_CHOICE_SCARF && !IsDynamaxed(battler))
        speed = (speed * 150) / 100;
    else if (holdEffect == HOLD_EFFECT_QUICK_POWDER && gBattleMons[battler].species == SPECIES_DITTO && !(gBattleMons[battler].status2 & STATUS2_TRANSFORMED))
        speed *= 2;

    // various effects
    if (gSideStatuses[GetBattlerSide(battler)] & SIDE_STATUS_TAILWIND)
        speed *= 2;
    if (gBattleResources->flags->flags[battler] & RESOURCE_FLAG_UNBURDEN)
        speed *= 2;

    // paralysis drop
    if (gBattleMons[battler].status1 & STATUS1_PARALYSIS && ability != ABILITY_QUICK_FEET)
        speed /= B_PARALYSIS_SPEED >= GEN_7 ? 2 : 4;

    return speed;
}

u32 GetBattlerTotalSpeedStat(u32 battler)
{
    u32 ability = GetBattlerAbility(battler);
    u32 holdEffect = GetBattlerHoldEffect(battler, TRUE);
    return GetBattlerTotalSpeedStatArgs(battler, ability, holdEffect);
}

s8 GetChosenMovePriority(u32 battler)
{
    u16 move;

    gProtectStructs[battler].pranksterElevated = 0;
    if (gProtectStructs[battler].noValidMoves)
        move = MOVE_STRUGGLE;
    else
        move = gBattleMons[battler].moves[*(gBattleStruct->chosenMovePositions + battler)];

    return GetMovePriority(battler, move);
}

s8 GetMovePriority(u32 battler, u16 move)
{
    s8 priority;
    u16 ability = GetBattlerAbility(battler);

    priority = gBattleMoves[move].priority;

    // Max Guard check
    if (gBattleStruct->dynamax.usingMaxMove[battler] && gBattleMoves[move].split == SPLIT_STATUS)
        return gBattleMoves[MOVE_MAX_GUARD].priority;

    if (ability == ABILITY_GALE_WINGS
        && (B_GALE_WINGS < GEN_7 || BATTLER_MAX_HP(battler))
        && gBattleMoves[move].type == TYPE_FLYING)
    {
        priority++;
    }
    else if (ability == ABILITY_PRANKSTER && IS_MOVE_STATUS(move))
    {
        gProtectStructs[battler].pranksterElevated = 1;
        priority++;
    }
    else if (gBattleMoves[move].effect == EFFECT_GRASSY_GLIDE && gFieldStatuses & STATUS_FIELD_GRASSY_TERRAIN && IsBattlerGrounded(battler))
    {
        priority++;
    }
    else if (ability == ABILITY_TRIAGE)
    {
        switch (gBattleMoves[move].effect)
        {
        case EFFECT_RESTORE_HP:
        case EFFECT_REST:
        case EFFECT_MORNING_SUN:
        case EFFECT_MOONLIGHT:
        case EFFECT_SYNTHESIS:
        case EFFECT_HEAL_PULSE:
        case EFFECT_HEALING_WISH:
        case EFFECT_SWALLOW:
        case EFFECT_WISH:
        case EFFECT_SOFTBOILED:
        case EFFECT_ABSORB:
        case EFFECT_ROOST:
        case EFFECT_JUNGLE_HEALING:
            priority += 3;
            break;
        }
    }

    if (gProtectStructs[battler].quash)
        priority = -8;

    return priority;
}

// Function for AI with variables provided as arguments to speed the computation time
u32 GetWhichBattlerFasterArgs(u32 battler1, u32 battler2, bool32 ignoreChosenMoves, u32 ability1, u32 ability2,
                              u32 holdEffectBattler1, u32 holdEffectBattler2, u32 speedBattler1, u32 speedBattler2, s32 priority1, s32 priority2)
{
    u32 strikesFirst = 0;

    // Battler 1
    // Quick Draw
    if (!ignoreChosenMoves && ability1 == ABILITY_QUICK_DRAW && !IS_MOVE_STATUS(gChosenMoveByBattler[battler1]) && RandomPercentage(RNG_QUICK_DRAW, 30))
        gProtectStructs[battler1].quickDraw = TRUE;
    // Quick Claw and Custap Berry
    if (!gProtectStructs[battler1].quickDraw
     && ((holdEffectBattler1 == HOLD_EFFECT_QUICK_CLAW && gRandomTurnNumber < (0xFFFF * GetBattlerHoldEffectParam(battler1)) / 100)
     || (holdEffectBattler1 == HOLD_EFFECT_CUSTAP_BERRY && HasEnoughHpToEatBerry(battler1, 4, gBattleMons[battler1].item))))
        gProtectStructs[battler1].usedCustapBerry = TRUE;

    // Battler 2
    // Quick Draw
    if (!ignoreChosenMoves && ability2 == ABILITY_QUICK_DRAW && !IS_MOVE_STATUS(gChosenMoveByBattler[battler2]) && RandomPercentage(RNG_QUICK_DRAW, 30))
        gProtectStructs[battler2].quickDraw = TRUE;
    // Quick Claw and Custap Berry
    if (!gProtectStructs[battler2].quickDraw
     && ((holdEffectBattler2 == HOLD_EFFECT_QUICK_CLAW && gRandomTurnNumber < (0xFFFF * GetBattlerHoldEffectParam(battler2)) / 100)
     || (holdEffectBattler2 == HOLD_EFFECT_CUSTAP_BERRY && HasEnoughHpToEatBerry(battler2, 4, gBattleMons[battler2].item))))
        gProtectStructs[battler2].usedCustapBerry = TRUE;

    if (priority1 == priority2)
    {
        // QUICK CLAW / CUSTAP - always first
        // LAGGING TAIL - always last
        // STALL - always last

        if (gProtectStructs[battler1].quickDraw && !gProtectStructs[battler2].quickDraw)
            strikesFirst = 0;
        else if (!gProtectStructs[battler1].quickDraw && gProtectStructs[battler2].quickDraw)
            strikesFirst = 1;
        else if (gProtectStructs[battler1].usedCustapBerry && !gProtectStructs[battler2].usedCustapBerry)
            strikesFirst = 0;
        else if (gProtectStructs[battler2].usedCustapBerry && !gProtectStructs[battler1].usedCustapBerry)
            strikesFirst = 1;
        else if (holdEffectBattler1 == HOLD_EFFECT_LAGGING_TAIL && holdEffectBattler2 != HOLD_EFFECT_LAGGING_TAIL)
            strikesFirst = 1;
        else if (holdEffectBattler2 == HOLD_EFFECT_LAGGING_TAIL && holdEffectBattler1 != HOLD_EFFECT_LAGGING_TAIL)
            strikesFirst = 0;
        else if (ability1 == ABILITY_STALL && ability2 != ABILITY_STALL)
            strikesFirst = 1;
        else if (ability2 == ABILITY_STALL && ability1 != ABILITY_STALL)
            strikesFirst = 0;
        else if (ability1 == ABILITY_MYCELIUM_MIGHT && ability2 != ABILITY_MYCELIUM_MIGHT && IS_MOVE_STATUS(gCurrentMove))
            strikesFirst = 1;
        else if (ability2 == ABILITY_MYCELIUM_MIGHT && ability1 != ABILITY_MYCELIUM_MIGHT && IS_MOVE_STATUS(gCurrentMove))
            strikesFirst = 0;
        else
        {
            if (speedBattler1 == speedBattler2 && Random() & 1)
            {
                strikesFirst = 2; // same speeds, same priorities
            }
            else if (speedBattler1 < speedBattler2)
            {
                // battler2 has more speed
                if (gFieldStatuses & STATUS_FIELD_TRICK_ROOM)
                    strikesFirst = 0;
                else
                    strikesFirst = 1;
            }
            else
            {
                // battler1 has more speed
                if (gFieldStatuses & STATUS_FIELD_TRICK_ROOM)
                    strikesFirst = 1;
                else
                    strikesFirst = 0;
            }
        }
    }
    else if (priority1 < priority2)
    {
        strikesFirst = 1; // battler2's move has greater priority
    if (holdEffect == HOLD_EFFECT_MACHO_BRACE)
        speedBattler1 /= 2;

    if (gBattleMons[battler1].status1 & STATUS1_PARALYSIS)
        speedBattler1 /= 4;

    if (holdEffect == HOLD_EFFECT_QUICK_CLAW && gRandomTurnNumber < (0xFFFF * holdEffectParam) / 100)
        speedBattler1 = UINT_MAX;

    // check second battlerId's speed

    speedBattler2 = (gBattleMons[battler2].speed * speedMultiplierBattler2)
                * (gStatStageRatios[gBattleMons[battler2].statStages[STAT_SPEED]][0])
                / (gStatStageRatios[gBattleMons[battler2].statStages[STAT_SPEED]][1]);

    if (gBattleMons[battler2].item == ITEM_ENIGMA_BERRY)
    {
        holdEffect = gEnigmaBerries[battler2].holdEffect;
        holdEffectParam = gEnigmaBerries[battler2].holdEffectParam;
    }
    else
    {
        holdEffect = ItemId_GetHoldEffect(gBattleMons[battler2].item);
        holdEffectParam = ItemId_GetHoldEffectParam(gBattleMons[battler2].item);
    }

    // badge boost
    if (!(gBattleTypeFlags & (BATTLE_TYPE_LINK | BATTLE_TYPE_RECORDED_LINK | BATTLE_TYPE_FRONTIER))
        && FlagGet(FLAG_BADGE03_GET)
        && GetBattlerSide(battler2) == B_SIDE_PLAYER)
    {
        speedBattler2 = (speedBattler2 * 110) / 100;
    }

    if (holdEffect == HOLD_EFFECT_MACHO_BRACE)
        speedBattler2 /= 2;

    if (gBattleMons[battler2].status1 & STATUS1_PARALYSIS)
        speedBattler2 /= 4;

    if (holdEffect == HOLD_EFFECT_QUICK_CLAW && gRandomTurnNumber < (0xFFFF * holdEffectParam) / 100)
        speedBattler2 = UINT_MAX;

    if (ignoreChosenMoves)
    {
        moveBattler1 = MOVE_NONE;
        moveBattler2 = MOVE_NONE;
    }
    else
    {
        if (gChosenActionByBattler[battler1] == B_ACTION_USE_MOVE)
        {
            if (gProtectStructs[battler1].noValidMoves)
                moveBattler1 = MOVE_STRUGGLE;
            else
                moveBattler1 = gBattleMons[battler1].moves[*(gBattleStruct->chosenMovePositions + battler1)];
        }
        else
            moveBattler1 = MOVE_NONE;

        if (gChosenActionByBattler[battler2] == B_ACTION_USE_MOVE)
        {
            if (gProtectStructs[battler2].noValidMoves)
                moveBattler2 = MOVE_STRUGGLE;
            else
                moveBattler2 = gBattleMons[battler2].moves[*(gBattleStruct->chosenMovePositions + battler2)];
        }
        else
            moveBattler2 = MOVE_NONE;
    }

    // both move priorities are different than 0
    if (gBattleMoves[moveBattler1].priority != 0 || gBattleMoves[moveBattler2].priority != 0)
    {
        // both priorities are the same
        if (gBattleMoves[moveBattler1].priority == gBattleMoves[moveBattler2].priority)
        {
            if (speedBattler1 == speedBattler2 && Random() & 1)
                strikesFirst = 2; // same speeds, same priorities
            else if (speedBattler1 < speedBattler2)
                strikesFirst = 1; // battler2 has more speed

            // else battler1 has more speed
        }
        else if (gBattleMoves[moveBattler1].priority < gBattleMoves[moveBattler2].priority)
            strikesFirst = 1; // battler2's move has greater priority

        // else battler1's move has greater priority
    }
    // both priorities are equal to 0
    else
    {
        if (speedBattler1 == speedBattler2 && Random() & 1)
            strikesFirst = 2; // same speeds, same priorities
        else if (speedBattler1 < speedBattler2)
            strikesFirst = 1; // battler2 has more speed

        // else battler1 has more speed
    }

    return strikesFirst;
}

u32 GetWhichBattlerFaster(u32 battler1, u32 battler2, bool32 ignoreChosenMoves)
{
    s32 priority1 = 0, priority2 = 0;
    u32 ability1 = GetBattlerAbility(battler1);
    u32 speedBattler1 = GetBattlerTotalSpeedStat(battler1);
    u32 holdEffectBattler1 = GetBattlerHoldEffect(battler1, TRUE);
    u32 speedBattler2 = GetBattlerTotalSpeedStat(battler2);
    u32 holdEffectBattler2 = GetBattlerHoldEffect(battler2, TRUE);
    u32 ability2 = GetBattlerAbility(battler2);

    if (!ignoreChosenMoves)
    {
        if (gChosenActionByBattler[battler1] == B_ACTION_USE_MOVE)
            priority1 = GetChosenMovePriority(battler1);
        if (gChosenActionByBattler[battler2] == B_ACTION_USE_MOVE)
            priority2 = GetChosenMovePriority(battler2);
    }

    return GetWhichBattlerFasterArgs(battler1, battler2, ignoreChosenMoves, ability1, ability2,
                                     holdEffectBattler1, holdEffectBattler2, speedBattler1, speedBattler2, priority1, priority2);
}

static void SetActionsAndBattlersTurnOrder(void)
{
    s32 turnOrderId = 0;
    s32 i, j, battler;

    if (gBattleTypeFlags & BATTLE_TYPE_SAFARI)
    {
        for (battler = 0; battler < gBattlersCount; battler++)
        {
            gActionsByTurnOrder[turnOrderId] = gChosenActionByBattler[battler];
            gBattlerByTurnOrder[turnOrderId] = battler;
            turnOrderId++;
        }
    }
    else
    {
        if (gBattleTypeFlags & BATTLE_TYPE_LINK)
        {
            for (battler = 0; battler < gBattlersCount; battler++)
            {
                if (gChosenActionByBattler[battler] == B_ACTION_RUN)
                {
                    turnOrderId = 5;
                    break;
                }
            }
        }
        else
        {
            if (gChosenActionByBattler[0] == B_ACTION_RUN)
            {
                battler = 0;
                turnOrderId = 5;
            }
            if (gChosenActionByBattler[2] == B_ACTION_RUN)
            {
                battler = 2;
                turnOrderId = 5;
            }
        }

        if (turnOrderId == 5) // One of battlers wants to run.
        {
            gActionsByTurnOrder[0] = gChosenActionByBattler[battler];
            gBattlerByTurnOrder[0] = battler;
            turnOrderId = 1;
            for (i = 0; i < gBattlersCount; i++)
            {
                if (i != battler)
                {
                    gActionsByTurnOrder[turnOrderId] = gChosenActionByBattler[i];
                    gBattlerByTurnOrder[turnOrderId] = i;
                    turnOrderId++;
                }
            }
            gBattleMainFunc = CheckFocusPunch_ClearVarsBeforeTurnStarts;
            gBattleStruct->focusPunchBattlerId = 0;
            return;
        }
        else
        {
            for (battler = 0; battler < gBattlersCount; battler++)
            {
                if (gChosenActionByBattler[battler] == B_ACTION_USE_ITEM
                  || gChosenActionByBattler[battler] == B_ACTION_SWITCH
                  || gChosenActionByBattler[battler] == B_ACTION_THROW_BALL)
                if (gChosenActionByBattler[gActiveBattler] == B_ACTION_USE_ITEM || gChosenActionByBattler[gActiveBattler] == B_ACTION_SWITCH)
                {
                    gActionsByTurnOrder[turnOrderId] = gChosenActionByBattler[battler];
                    gBattlerByTurnOrder[turnOrderId] = battler;
                    turnOrderId++;
                }
            }
            for (battler = 0; battler < gBattlersCount; battler++)
            {
                if (gChosenActionByBattler[battler] != B_ACTION_USE_ITEM
                  && gChosenActionByBattler[battler] != B_ACTION_SWITCH
                  && gChosenActionByBattler[battler] != B_ACTION_THROW_BALL)
                if (gChosenActionByBattler[gActiveBattler] != B_ACTION_USE_ITEM && gChosenActionByBattler[gActiveBattler] != B_ACTION_SWITCH)
                {
                    gActionsByTurnOrder[turnOrderId] = gChosenActionByBattler[battler];
                    gBattlerByTurnOrder[turnOrderId] = battler;
                    turnOrderId++;
                }
            }
            for (i = 0; i < gBattlersCount - 1; i++)
            {
                for (j = i + 1; j < gBattlersCount; j++)
                {
                    u8 battler1 = gBattlerByTurnOrder[i];
                    u8 battler2 = gBattlerByTurnOrder[j];
                    if (gActionsByTurnOrder[i] != B_ACTION_USE_ITEM
                        && gActionsByTurnOrder[j] != B_ACTION_USE_ITEM
                        && gActionsByTurnOrder[i] != B_ACTION_SWITCH
                        && gActionsByTurnOrder[j] != B_ACTION_SWITCH)
                    {
                        if (GetWhichBattlerFaster(battler1, battler2, FALSE))
                            SwapTurnOrder(i, j);
                    }
                }
            }
        }
    }
    gBattleMainFunc = CheckFocusPunch_ClearVarsBeforeTurnStarts;
    gBattleStruct->focusPunchBattlerId = 0;
}

static void TurnValuesCleanUp(bool8 var0)
{
    s32 i;
    u8 *dataPtr;

    for (i = 0; i < gBattlersCount; i++)
    {
        if (var0)
        {
            gProtectStructs[i].protected = FALSE;
            gProtectStructs[i].spikyShielded = FALSE;
            gProtectStructs[i].kingsShielded = FALSE;
            gProtectStructs[i].banefulBunkered = FALSE;
            gProtectStructs[i].quash = FALSE;
            gProtectStructs[i].usedCustapBerry = FALSE;
            gProtectStructs[gActiveBattler].protected = 0;
            gProtectStructs[gActiveBattler].endured = 0;
        }
        else
        {
            memset(&gProtectStructs[i], 0, sizeof(struct ProtectStruct));
            dataPtr = (u8*)(&gProtectStructs[gActiveBattler]);
            for (i = 0; i < sizeof(struct ProtectStruct); i++)
                dataPtr[i] = 0;

            if (gDisableStructs[i].isFirstTurn)
                gDisableStructs[i].isFirstTurn--;

            if (gDisableStructs[i].rechargeTimer)
            {
                gDisableStructs[i].rechargeTimer--;
                if (gDisableStructs[i].rechargeTimer == 0)
                    gBattleMons[i].status2 &= ~STATUS2_RECHARGE;
                gDisableStructs[gActiveBattler].rechargeTimer--;
                if (gDisableStructs[gActiveBattler].rechargeTimer == 0)
                    gBattleMons[gActiveBattler].status2 &= ~(STATUS2_RECHARGE);
            }
        }

        if (gDisableStructs[i].substituteHP == 0)
            gBattleMons[i].status2 &= ~STATUS2_SUBSTITUTE;

        gSpecialStatuses[i].parentalBondState = PARENTAL_BOND_OFF;
        if (gDisableStructs[gActiveBattler].substituteHP == 0)
            gBattleMons[gActiveBattler].status2 &= ~(STATUS2_SUBSTITUTE);
    }

    gSideTimers[0].followmeTimer = 0;
    gSideTimers[1].followmeTimer = 0;
}

void SpecialStatusesClear(void)
{
    memset(&gSpecialStatuses, 0, sizeof(gSpecialStatuses));
}

static void PopulateArrayWithBattlers(u8 *battlers)
{
    u32 i;
    for (i = 0; i < gBattlersCount; i++)
        battlers[i] = i;
}

static bool32 TryDoGimmicksBeforeMoves(void)
{
    if (!(gHitMarker & HITMARKER_RUN)
        && (gBattleStruct->mega.toEvolve || gBattleStruct->burst.toBurst || gBattleStruct->dynamax.toDynamax))
    for (gActiveBattler = 0; gActiveBattler < gBattlersCount; gActiveBattler++)
    {
        u32 i, battler;
        u8 order[MAX_BATTLERS_COUNT];

        PopulateArrayWithBattlers(order);
        SortBattlersBySpeed(order, FALSE);
        for (i = 0; i < gBattlersCount; i++)
        {
            // Dynamax Check
            if (gBattleStruct->dynamax.toDynamax & gBitTable[order[i]])
            {
                gBattlerAttacker = order[i];
                gBattleScripting.battler = gBattlerAttacker;
                gBattleStruct->dynamax.toDynamax &= ~(gBitTable[gBattlerAttacker]);
                PrepareBattlerForDynamax(gBattlerAttacker);
                BattleScriptExecute(BattleScript_DynamaxBegins);
                return TRUE;
            }
            // Mega Evo Check
            if (gBattleStruct->mega.toEvolve & gBitTable[order[i]]
                && !(gProtectStructs[order[i]].noValidMoves))
            {
                gBattlerAttacker = order[i];
                gBattleStruct->mega.toEvolve &= ~(gBitTable[gBattlerAttacker]);
                gLastUsedItem = gBattleMons[gBattlerAttacker].item;
                if (GetBattleFormChangeTargetSpecies(gBattlerAttacker, FORM_CHANGE_BATTLE_MEGA_EVOLUTION_MOVE) != SPECIES_NONE)
                    BattleScriptExecute(BattleScript_WishMegaEvolution);
                else
                    BattleScriptExecute(BattleScript_MegaEvolution);
                return TRUE;
            }
            // Ultra Burst Check
            if (gBattleStruct->burst.toBurst & gBitTable[order[i]]
                && !(gProtectStructs[order[i]].noValidMoves))
            {
                battler = gBattlerAttacker = order[i];
                gBattleStruct->burst.toBurst &= ~(gBitTable[battler]);
                gLastUsedItem = gBattleMons[battler].item;
                BattleScriptExecute(BattleScript_UltraBurst);
                return TRUE;
            }
        }
    }

    if (B_MEGA_EVO_TURN_ORDER >= GEN_7)
        TryChangeTurnOrder(); // This will just do nothing if no mon has mega evolved.
    return FALSE;
}

static bool32 TryDoMoveEffectsBeforeMoves(void)
        s32 i;
        u8 *dataPtr = (u8*)(&gSpecialStatuses[gActiveBattler]);

        for (i = 0; i < sizeof(struct SpecialStatus); i++)
            dataPtr[i] = 0;
    }
}

static void CheckFocusPunch_ClearVarsBeforeTurnStarts(void)
{
    if (!(gHitMarker & HITMARKER_RUN))
    {
        u32 i;
        u8 battlers[MAX_BATTLERS_COUNT];

        PopulateArrayWithBattlers(battlers);
        SortBattlersBySpeed(battlers, FALSE);
        for (i = 0; i < gBattlersCount; i++)
        {
            if (!(gBattleStruct->focusPunchBattlers & gBitTable[battlers[i]])
                && !(gBattleMons[battlers[i]].status1 & STATUS1_SLEEP)
                && !(gDisableStructs[battlers[i]].truantCounter)
                && !(gProtectStructs[battlers[i]].noValidMoves))
            {
                gBattleStruct->focusPunchBattlers |= gBitTable[battlers[i]];
                gBattlerAttacker = battlers[i];
                switch (gChosenMoveByBattler[gBattlerAttacker])
                {
                case MOVE_FOCUS_PUNCH:
                    BattleScriptExecute(BattleScript_FocusPunchSetUp);
                    return TRUE;
                case MOVE_BEAK_BLAST:
                    BattleScriptExecute(BattleScript_BeakBlastSetUp);
                    return TRUE;
                case MOVE_SHELL_TRAP:
                    BattleScriptExecute(BattleScript_ShellTrapSetUp);
                    return TRUE;
                }
            }
        }
    }

    return FALSE;
}

// In gen7, priority and speed are recalculated during the turn in which a pokemon mega evolves
static void TryChangeTurnOrder(void)
{
    u32 i, j;
    for (i = 0; i < gBattlersCount - 1; i++)
    {
        for (j = i + 1; j < gBattlersCount; j++)
        {
            u32 battler1 = gBattlerByTurnOrder[i];
            u32 battler2 = gBattlerByTurnOrder[j];

            if (gActionsByTurnOrder[i] == B_ACTION_USE_MOVE
                && gActionsByTurnOrder[j] == B_ACTION_USE_MOVE)
            {
                if (GetWhichBattlerFaster(battler1, battler2, FALSE))
                    SwapTurnOrder(i, j);
            }
        }
    }
}

static void CheckQuickClaw_CustapBerryActivation(void)
{
    u32 i, battler;

    if (!(gHitMarker & HITMARKER_RUN))
    {
        while (gBattleStruct->quickClawBattlerId < gBattlersCount)
        {
            battler = gBattlerAttacker = gBattleStruct->quickClawBattlerId;
            gBattleStruct->quickClawBattlerId++;
            if (gChosenActionByBattler[battler] == B_ACTION_USE_MOVE
             && gChosenMoveByBattler[battler] != MOVE_FOCUS_PUNCH   // quick claw message doesn't need to activate here
             && (gProtectStructs[battler].usedCustapBerry || gProtectStructs[battler].quickDraw)
             && !(gBattleMons[battler].status1 & STATUS1_SLEEP)
             && !(gDisableStructs[gBattlerAttacker].truantCounter)
             && !(gProtectStructs[battler].noValidMoves))
        while (gBattleStruct->focusPunchBattlerId < gBattlersCount)
        {
            gActiveBattler = gBattlerAttacker = gBattleStruct->focusPunchBattlerId;
            gBattleStruct->focusPunchBattlerId++;
            if (gChosenMoveByBattler[gActiveBattler] == MOVE_FOCUS_PUNCH
                && !(gBattleMons[gActiveBattler].status1 & STATUS1_SLEEP)
                && !(gDisableStructs[gBattlerAttacker].truantCounter)
                && !(gProtectStructs[gActiveBattler].noValidMoves))
            {
                if (gProtectStructs[battler].usedCustapBerry)
                {
                    gLastUsedItem = gBattleMons[battler].item;
                    PREPARE_ITEM_BUFFER(gBattleTextBuff1, gLastUsedItem);
                    if (GetBattlerHoldEffect(battler, FALSE) == HOLD_EFFECT_CUSTAP_BERRY)
                    {
                        // don't record berry since its gone now
                        BattleScriptExecute(BattleScript_CustapBerryActivation);
                    }
                    else
                    {
                        RecordItemEffectBattle(battler, GetBattlerHoldEffect(battler, FALSE));
                        BattleScriptExecute(BattleScript_QuickClawActivation);
                    }
                }
                else if (gProtectStructs[battler].quickDraw)
                {
                    gBattlerAbility = battler;
                    gProtectStructs[battler].quickDraw = FALSE;
                    gLastUsedAbility = gBattleMons[battler].ability;
                    PREPARE_ABILITY_BUFFER(gBattleTextBuff1, gLastUsedAbility);
                    RecordAbilityBattle(battler, gLastUsedAbility);
                    BattleScriptExecute(BattleScript_QuickDrawActivation);
                }
                BattleScriptExecute(BattleScript_FocusPunchSetUp);
                return;
            }
        }
    }

    TryClearRageStatuses();
    gCurrentTurnActionNumber = 0;
    gCurrentActionFuncId = gActionsByTurnOrder[gCurrentTurnActionNumber];
    gDynamicBasePower = 0;
    gBattleStruct->dynamicMoveType = 0;
    gBattleMainFunc = RunTurnActionsFunctions;
    gBattleCommunication[3] = 0;
    gBattleCommunication[4] = 0;
    gBattleScripting.multihitMoveEffect = 0;
    gBattleResources->battleScriptsStack->size = 0;
}

static void RunTurnActionsFunctions(void)
{
    if (gBattleOutcome != 0)
        gCurrentActionFuncId = B_ACTION_FINISHED;

    // Mega Evolve / Focus Punch-like moves after switching, items, running, but before using a move.
    if (gCurrentActionFuncId == B_ACTION_USE_MOVE && !gBattleStruct->effectsBeforeUsingMoveDone)
    {
        if (TryDoGimmicksBeforeMoves())
            return;
        else if (TryDoMoveEffectsBeforeMoves())
            return;
        gBattleStruct->effectsBeforeUsingMoveDone = TRUE;
    }

    *(&gBattleStruct->savedTurnActionNumber) = gCurrentTurnActionNumber;
    sTurnActionsFuncsTable[gCurrentActionFuncId]();

    if (gCurrentTurnActionNumber >= gBattlersCount) // everyone did their actions, turn finished
    {
        gHitMarker &= ~(HITMARKER_x100000);
        gBattleMainFunc = sEndTurnFuncsTable[gBattleOutcome & 0x7F];
    }
    else
    {
        if (gBattleStruct->savedTurnActionNumber != gCurrentTurnActionNumber) // action turn has been done, clear hitmarker bits for another battler
        {
            gHitMarker &= ~(HITMARKER_NO_ATTACKSTRING);
            gHitMarker &= ~(HITMARKER_UNABLE_TO_USE_MOVE);
        }
    }
}

static void HandleEndTurn_BattleWon(void)
{
    gCurrentActionFuncId = 0;

    if (gBattleTypeFlags & (BATTLE_TYPE_LINK | BATTLE_TYPE_RECORDED_LINK))
    {
        gSpecialVar_Result = gBattleOutcome;
        gBattleTextBuff1[0] = gBattleOutcome;
        gBattlerAttacker = GetBattlerAtPosition(B_POSITION_PLAYER_LEFT);
        gBattlescriptCurrInstr = BattleScript_LinkBattleWonOrLost;
        gBattleOutcome &= ~(B_OUTCOME_LINK_BATTLE_RAN);
    }
    else if (gBattleTypeFlags & BATTLE_TYPE_TRAINER
            && gBattleTypeFlags & (BATTLE_TYPE_FRONTIER | BATTLE_TYPE_TRAINER_HILL | BATTLE_TYPE_EREADER_TRAINER))
    {
        BattleStopLowHpSound();
        gBattlescriptCurrInstr = BattleScript_FrontierTrainerBattleWon;

        if (gTrainerBattleOpponent_A == TRAINER_FRONTIER_BRAIN)
            PlayBGM(MUS_VICTORY_GYM_LEADER);
        else
            PlayBGM(MUS_VICTORY_TRAINER);
    }
    else if (gBattleTypeFlags & BATTLE_TYPE_TRAINER && !(gBattleTypeFlags & BATTLE_TYPE_LINK))
    {
        BattleStopLowHpSound();
        gBattlescriptCurrInstr = BattleScript_LocalTrainerBattleWon;

        switch (gTrainers[gTrainerBattleOpponent_A].trainerClass)
        {
        case TRAINER_CLASS_ELITE_FOUR:
        case TRAINER_CLASS_CHAMPION:
            PlayBGM(MUS_VICTORY_LEAGUE);
            break;
        case TRAINER_CLASS_TEAM_AQUA:
        case TRAINER_CLASS_TEAM_MAGMA:
        case TRAINER_CLASS_AQUA_ADMIN:
        case TRAINER_CLASS_AQUA_LEADER:
        case TRAINER_CLASS_MAGMA_ADMIN:
        case TRAINER_CLASS_MAGMA_LEADER:
            PlayBGM(MUS_VICTORY_AQUA_MAGMA);
            break;
        case TRAINER_CLASS_LEADER:
            PlayBGM(MUS_VICTORY_GYM_LEADER);
            break;
        default:
            PlayBGM(MUS_VICTORY_TRAINER);
            break;
        }
    }
    else
    {
        gBattlescriptCurrInstr = BattleScript_PayDayMoneyAndPickUpItems;
    }

    gBattleMainFunc = HandleEndTurn_FinishBattle;
}

static void HandleEndTurn_BattleLost(void)
{
    gCurrentActionFuncId = 0;

    if (gBattleTypeFlags & (BATTLE_TYPE_LINK | BATTLE_TYPE_RECORDED_LINK))
    {
        if (gBattleTypeFlags & BATTLE_TYPE_FRONTIER)
        {
            if (gBattleOutcome & B_OUTCOME_LINK_BATTLE_RAN)
            {
                gBattlescriptCurrInstr = BattleScript_PrintPlayerForfeitedLinkBattle;
                gBattleOutcome &= ~(B_OUTCOME_LINK_BATTLE_RAN);
                gSaveBlock2Ptr->frontier.disableRecordBattle = TRUE;
            }
            else
            {
                gBattlescriptCurrInstr = BattleScript_FrontierLinkBattleLost;
                gBattleOutcome &= ~(B_OUTCOME_LINK_BATTLE_RAN);
            }
        }
        else
        {
            gBattleTextBuff1[0] = gBattleOutcome;
            gBattlerAttacker = GetBattlerAtPosition(B_POSITION_PLAYER_LEFT);
            gBattlescriptCurrInstr = BattleScript_LinkBattleWonOrLost;
            gBattleOutcome &= ~(B_OUTCOME_LINK_BATTLE_RAN);
        }
    }
    else
    {
        gBattlescriptCurrInstr = BattleScript_LocalBattleLost;
    }

    gBattleMainFunc = HandleEndTurn_FinishBattle;
}

static void HandleEndTurn_RanFromBattle(void)
{
    gCurrentActionFuncId = 0;

    if (gBattleTypeFlags & BATTLE_TYPE_FRONTIER && gBattleTypeFlags & BATTLE_TYPE_TRAINER)
    {
        gBattlescriptCurrInstr = BattleScript_PrintPlayerForfeited;
        gBattleOutcome = B_OUTCOME_FORFEITED;
        gSaveBlock2Ptr->frontier.disableRecordBattle = TRUE;
    }
    else if (gBattleTypeFlags & BATTLE_TYPE_TRAINER_HILL)
    {
        gBattlescriptCurrInstr = BattleScript_PrintPlayerForfeited;
        gBattleOutcome = B_OUTCOME_FORFEITED;
    }
    else
    {
        switch (gProtectStructs[gBattlerAttacker].fleeFlag)
        {
        default:
            gBattlescriptCurrInstr = BattleScript_GotAwaySafely;
            break;
        case 1:
            gBattlescriptCurrInstr = BattleScript_SmokeBallEscape;
            break;
        case 2:
            gBattlescriptCurrInstr = BattleScript_RanAwayUsingMonAbility;
            break;
        }
    }

    gBattleMainFunc = HandleEndTurn_FinishBattle;
}

static void HandleEndTurn_MonFled(void)
{
    gCurrentActionFuncId = 0;

    PREPARE_MON_NICK_BUFFER(gBattleTextBuff1, gBattlerAttacker, gBattlerPartyIndexes[gBattlerAttacker]);
    gBattlescriptCurrInstr = BattleScript_WildMonFled;

    gBattleMainFunc = HandleEndTurn_FinishBattle;
}

static void HandleEndTurn_FinishBattle(void)
{
    u32 i, battler;

    if (gCurrentActionFuncId == B_ACTION_TRY_FINISH || gCurrentActionFuncId == B_ACTION_FINISHED)
    {
        if (!(gBattleTypeFlags & (BATTLE_TYPE_LINK
                                  | BATTLE_TYPE_RECORDED_LINK
                                  | BATTLE_TYPE_FIRST_BATTLE
                                  | BATTLE_TYPE_SAFARI
                                  | BATTLE_TYPE_EREADER_TRAINER
                                  | BATTLE_TYPE_WALLY_TUTORIAL
                                  | BATTLE_TYPE_FRONTIER)))
        {
            for (battler = 0; battler < gBattlersCount; battler++)
            {
                if (GetBattlerSide(battler) == B_SIDE_PLAYER)
                {
                    if (gBattleResults.playerMon1Species == SPECIES_NONE)
                    {
                        gBattleResults.playerMon1Species = GetMonData(&gPlayerParty[gBattlerPartyIndexes[battler]], MON_DATA_SPECIES, NULL);
                        GetMonData(&gPlayerParty[gBattlerPartyIndexes[battler]], MON_DATA_NICKNAME, gBattleResults.playerMon1Name);
                    }
                    else
                    {
                        gBattleResults.playerMon2Species = GetMonData(&gPlayerParty[gBattlerPartyIndexes[battler]], MON_DATA_SPECIES, NULL);
                        GetMonData(&gPlayerParty[gBattlerPartyIndexes[battler]], MON_DATA_NICKNAME, gBattleResults.playerMon2Name);
                    }
                }
            }
            TryPutPokemonTodayOnAir();
        }

        if (!(gBattleTypeFlags & (BATTLE_TYPE_LINK
                                  | BATTLE_TYPE_RECORDED_LINK
                                  | BATTLE_TYPE_TRAINER
                                  | BATTLE_TYPE_FIRST_BATTLE
                                  | BATTLE_TYPE_SAFARI
                                  | BATTLE_TYPE_FRONTIER
                                  | BATTLE_TYPE_EREADER_TRAINER
                                  | BATTLE_TYPE_WALLY_TUTORIAL))
            && gBattleResults.shinyWildMon)
        {
            TryPutBreakingNewsOnAir();
        }

        sub_8186444();
        BeginFastPaletteFade(3);
        FadeOutMapMusic(5);
        if (B_TRAINERS_KNOCK_OFF_ITEMS == TRUE || B_RESTORE_HELD_BATTLE_ITEMS == TRUE)
            TryRestoreHeldItems();

        // Undo Dynamax HP multiplier before recalculating stats.
        for (i = 0; i < gBattlersCount; ++i)
        {
            if (IsDynamaxed(i))
                UndoDynamax(i);
        }

        for (i = 0; i < PARTY_SIZE; i++)
        {
            bool8 changedForm = FALSE;

            // Appeared in battle and didn't faint
            if ((gBattleStruct->appearedInBattle & gBitTable[i]) && GetMonData(&gPlayerParty[i], MON_DATA_HP, NULL) != 0)
                changedForm = TryFormChange(i, B_SIDE_PLAYER, FORM_CHANGE_END_BATTLE_TERRAIN);

            if (!changedForm)
                changedForm = TryFormChange(i, B_SIDE_PLAYER, FORM_CHANGE_END_BATTLE);

            // Clear original species field
            gBattleStruct->changedSpecies[B_SIDE_PLAYER][i] = SPECIES_NONE;
            gBattleStruct->changedSpecies[B_SIDE_OPPONENT][i] = SPECIES_NONE;

            // Recalculate the stats of every party member before the end
            if (!changedForm && B_RECALCULATE_STATS >= GEN_5)
                CalculateMonStats(&gPlayerParty[i]);
        }
        // Clear battle mon species to avoid a bug on the next battle that causes
        // healthboxes loading incorrectly due to it trying to create a Mega Indicator
        // if the previous battler would've had it.
        for (i = 0; i < MAX_BATTLERS_COUNT; i++)
        {
            gBattleMons[i].species = SPECIES_NONE;
        }
        gBattleMainFunc = FreeResetData_ReturnToOvOrDoEvolutions;
        gCB2_AfterEvolution = BattleMainCB2;
    }
    else
    {
        if (gBattleControllerExecFlags == 0)
            gBattleScriptingCommandsTable[gBattlescriptCurrInstr[0]]();
    }
}

static void FreeResetData_ReturnToOvOrDoEvolutions(void)
{
    if (!gPaletteFade.active)
    {
        ResetSpriteData();
        if (!(gBattleTypeFlags & (BATTLE_TYPE_LINK
                                  | BATTLE_TYPE_RECORDED_LINK
                                  | BATTLE_TYPE_FIRST_BATTLE
                                  | BATTLE_TYPE_SAFARI
                                  | BATTLE_TYPE_FRONTIER
                                  | BATTLE_TYPE_EREADER_TRAINER
                                  | BATTLE_TYPE_WALLY_TUTORIAL))
            && (B_EVOLUTION_AFTER_WHITEOUT >= GEN_6
                || gBattleOutcome == B_OUTCOME_WON
                || gBattleOutcome == B_OUTCOME_CAUGHT))
        if (gLeveledUpInBattle == 0 || gBattleOutcome != B_OUTCOME_WON)
        {
            gBattleMainFunc = ReturnFromBattleToOverworld;
            return;
        }
        else
        {
            gBattleMainFunc = TryEvolvePokemon;
        }
    }

    FreeAllWindowBuffers();
    if (!(gBattleTypeFlags & BATTLE_TYPE_LINK))
    {
        FreeMonSpritesGfx();
        FreeBattleResources();
        FreeBattleSpritesData();
    }
}

static void TryEvolvePokemon(void)
{
    s32 i;

    while (gLeveledUpInBattle != 0)
    {
        for (i = 0; i < PARTY_SIZE; i++)
        {
            if (gLeveledUpInBattle & gBitTable[i])
            {
                u16 species;
                u8 levelUpBits = gLeveledUpInBattle;

                levelUpBits &= ~(gBitTable[i]);
                gLeveledUpInBattle = levelUpBits;

                species = GetEvolutionTargetSpecies(&gPlayerParty[i], EVO_MODE_NORMAL, levelUpBits);
                if (species != SPECIES_NONE)
                {
                    FreeAllWindowBuffers();
                    gBattleMainFunc = WaitForEvoSceneToFinish;
                    EvolutionScene(&gPlayerParty[i], species, TRUE, i);
                    return;
                }
            }
        }
    }

    gBattleMainFunc = ReturnFromBattleToOverworld;
}

static void WaitForEvoSceneToFinish(void)
{
    if (gMain.callback2 == BattleMainCB2)
        gBattleMainFunc = TryEvolvePokemon;
}

static void ReturnFromBattleToOverworld(void)
{
    if (!(gBattleTypeFlags & BATTLE_TYPE_LINK))
    {
        RandomlyGivePartyPokerus(gPlayerParty);
        PartySpreadPokerus(gPlayerParty);
    }

    if (gBattleTypeFlags & BATTLE_TYPE_LINK && gReceivedRemoteLinkPlayers != 0)
        return;

    gSpecialVar_Result = gBattleOutcome;
    gMain.inBattle = 0;
    gMain.callback1 = gPreBattleCallback1;

    if (gBattleTypeFlags & BATTLE_TYPE_ROAMER)
    {
        UpdateRoamerHPStatus(&gEnemyParty[0]);

#ifndef BUGFIX
        if ((gBattleOutcome & B_OUTCOME_WON) || gBattleOutcome == B_OUTCOME_CAUGHT)
#else
        if ((gBattleOutcome == B_OUTCOME_WON) || gBattleOutcome == B_OUTCOME_CAUGHT) // Bug: When Roar is used by roamer, gBattleOutcome is B_OUTCOME_PLAYER_TELEPORTED (5).
#endif                                                                               // & with B_OUTCOME_WON (1) will return TRUE and deactivates the roamer.
            SetRoamerInactive();
    }

    m4aSongNumStop(SE_LOW_HEALTH);
    SetMainCallback2(gMain.savedCallback);
}

void RunBattleScriptCommands_PopCallbacksStack(void)
{
    if (gCurrentActionFuncId == B_ACTION_TRY_FINISH || gCurrentActionFuncId == B_ACTION_FINISHED)
    {
        if (gBattleResources->battleCallbackStack->size != 0)
            gBattleResources->battleCallbackStack->size--;
        gBattleMainFunc = gBattleResources->battleCallbackStack->function[gBattleResources->battleCallbackStack->size];
    }
    else
    {
        if (gBattleControllerExecFlags == 0)
            gBattleScriptingCommandsTable[gBattlescriptCurrInstr[0]]();
    }
}

void RunBattleScriptCommands(void)
{
    if (gBattleControllerExecFlags == 0)
        gBattleScriptingCommandsTable[gBattlescriptCurrInstr[0]]();
}

void SetTypeBeforeUsingMove(u32 move, u32 battlerAtk)
{
    u32 moveType, ateType, attackerAbility;
    u16 holdEffect = GetBattlerHoldEffect(battlerAtk, TRUE);

    if (move == MOVE_STRUGGLE)
        return;

    gBattleStruct->dynamicMoveType = 0;
    gBattleStruct->ateBoost[battlerAtk] = 0;
    gSpecialStatuses[battlerAtk].gemBoost = FALSE;

    if (gBattleMoves[move].effect == EFFECT_WEATHER_BALL)
    {
        if (WEATHER_HAS_EFFECT)
        {
            if (gBattleWeather & B_WEATHER_RAIN && holdEffect != HOLD_EFFECT_UTILITY_UMBRELLA)
                gBattleStruct->dynamicMoveType = TYPE_WATER | F_DYNAMIC_TYPE_2;
            else if (gBattleWeather & B_WEATHER_SANDSTORM)
                gBattleStruct->dynamicMoveType = TYPE_ROCK | F_DYNAMIC_TYPE_2;
            else if (gBattleWeather & B_WEATHER_SUN && holdEffect != HOLD_EFFECT_UTILITY_UMBRELLA)
                gBattleStruct->dynamicMoveType = TYPE_FIRE | F_DYNAMIC_TYPE_2;
            else if (gBattleWeather & (B_WEATHER_HAIL |B_WEATHER_SNOW))
                gBattleStruct->dynamicMoveType = TYPE_ICE | F_DYNAMIC_TYPE_2;
            else
                gBattleStruct->dynamicMoveType = TYPE_NORMAL | F_DYNAMIC_TYPE_2;
        }
    }
    else if (gBattleMoves[move].effect == EFFECT_HIDDEN_POWER)
    {
        u8 typeBits  = ((gBattleMons[battlerAtk].hpIV & 1) << 0)
                     | ((gBattleMons[battlerAtk].attackIV & 1) << 1)
                     | ((gBattleMons[battlerAtk].defenseIV & 1) << 2)
                     | ((gBattleMons[battlerAtk].speedIV & 1) << 3)
                     | ((gBattleMons[battlerAtk].spAttackIV & 1) << 4)
                     | ((gBattleMons[battlerAtk].spDefenseIV & 1) << 5);

        // Subtract 4 instead of 1 below because 3 types are excluded (TYPE_NORMAL and TYPE_MYSTERY and TYPE_FAIRY)
        // The final + 1 skips past Normal, and the following conditional skips TYPE_MYSTERY
        gBattleStruct->dynamicMoveType = ((NUMBER_OF_MON_TYPES - 4) * typeBits) / 63 + 1;
        if (gBattleStruct->dynamicMoveType >= TYPE_MYSTERY)
            gBattleStruct->dynamicMoveType++;
        gBattleStruct->dynamicMoveType |= F_DYNAMIC_TYPE_1 | F_DYNAMIC_TYPE_2;
    }
    else if (gBattleMoves[move].effect == EFFECT_CHANGE_TYPE_ON_ITEM)
    {
        if (holdEffect == gBattleMoves[move].argument)
            gBattleStruct->dynamicMoveType = ItemId_GetSecondaryId(gBattleMons[battlerAtk].item) | F_DYNAMIC_TYPE_2;
    }
    else if (gBattleMoves[move].effect == EFFECT_IVY_CUDGEL && holdEffect == HOLD_EFFECT_MASK)
    {
        gBattleStruct->dynamicMoveType = ItemId_GetSecondaryId(gBattleMons[battlerAtk].item) | F_DYNAMIC_TYPE_2;
    }
    else if (gBattleMoves[move].effect == EFFECT_REVELATION_DANCE)
    {
        if (gBattleMons[battlerAtk].type1 != TYPE_MYSTERY)
            gBattleStruct->dynamicMoveType = gBattleMons[battlerAtk].type1 | F_DYNAMIC_TYPE_2;
        else if (gBattleMons[battlerAtk].type2 != TYPE_MYSTERY)
            gBattleStruct->dynamicMoveType = gBattleMons[battlerAtk].type2 | F_DYNAMIC_TYPE_2;
        else if (gBattleMons[battlerAtk].type3 != TYPE_MYSTERY)
            gBattleStruct->dynamicMoveType = gBattleMons[battlerAtk].type3 | F_DYNAMIC_TYPE_2;
    }
    else if (gBattleMoves[move].effect == EFFECT_NATURAL_GIFT)
    {
        if (ItemId_GetPocket(gBattleMons[battlerAtk].item) == POCKET_BERRIES)
            gBattleStruct->dynamicMoveType = gNaturalGiftTable[ITEM_TO_BERRY(gBattleMons[battlerAtk].item)].type;
    }
    else if (gBattleMoves[move].effect == EFFECT_TERRAIN_PULSE)
    {
        if (IsBattlerTerrainAffected(battlerAtk, STATUS_FIELD_TERRAIN_ANY))
        {
            if (gFieldStatuses & STATUS_FIELD_ELECTRIC_TERRAIN)
                gBattleStruct->dynamicMoveType = TYPE_ELECTRIC | F_DYNAMIC_TYPE_2;
            else if (gFieldStatuses & STATUS_FIELD_GRASSY_TERRAIN)
                gBattleStruct->dynamicMoveType = TYPE_GRASS | F_DYNAMIC_TYPE_2;
            else if (gFieldStatuses & STATUS_FIELD_MISTY_TERRAIN)
                gBattleStruct->dynamicMoveType = TYPE_FAIRY | F_DYNAMIC_TYPE_2;
            else if (gFieldStatuses & STATUS_FIELD_PSYCHIC_TERRAIN)
                gBattleStruct->dynamicMoveType = TYPE_PSYCHIC | F_DYNAMIC_TYPE_2;
            else //failsafe
                gBattleStruct->dynamicMoveType = TYPE_NORMAL | F_DYNAMIC_TYPE_2;
        }
    }

    attackerAbility = GetBattlerAbility(battlerAtk);
    GET_MOVE_TYPE(move, moveType);
    if ((gFieldStatuses & STATUS_FIELD_ION_DELUGE && moveType == TYPE_NORMAL)
        || gStatuses4[battlerAtk] & STATUS4_ELECTRIFIED)
    {
        gBattleStruct->dynamicMoveType = TYPE_ELECTRIC | F_DYNAMIC_TYPE_2;
    }
    else if (gBattleMoves[move].type == TYPE_NORMAL
             && gBattleMoves[move].effect != EFFECT_HIDDEN_POWER
             && gBattleMoves[move].effect != EFFECT_WEATHER_BALL
             && gBattleMoves[move].effect != EFFECT_CHANGE_TYPE_ON_ITEM
             && gBattleMoves[move].effect != EFFECT_NATURAL_GIFT
             && ((attackerAbility == ABILITY_PIXILATE && (ateType = TYPE_FAIRY))
                 || (attackerAbility == ABILITY_REFRIGERATE && (ateType = TYPE_ICE))
                 || (attackerAbility == ABILITY_AERILATE && (ateType = TYPE_FLYING))
                 || ((attackerAbility == ABILITY_GALVANIZE) && (ateType = TYPE_ELECTRIC))
                )
             )
    {
        gBattleStruct->dynamicMoveType = ateType | F_DYNAMIC_TYPE_2;
        if (!IsDynamaxed(battlerAtk))
            gBattleStruct->ateBoost[battlerAtk] = 1;
    }
    else if (gBattleMoves[move].type != TYPE_NORMAL
             && gBattleMoves[move].effect != EFFECT_HIDDEN_POWER
             && gBattleMoves[move].effect != EFFECT_WEATHER_BALL
             && attackerAbility == ABILITY_NORMALIZE)
    {
        gBattleStruct->dynamicMoveType = TYPE_NORMAL | F_DYNAMIC_TYPE_2;
        if (!IsDynamaxed(battlerAtk))
            gBattleStruct->ateBoost[battlerAtk] = 1;
    }
    else if (gBattleMoves[move].soundMove && attackerAbility == ABILITY_LIQUID_VOICE)
    {
        gBattleStruct->dynamicMoveType = TYPE_WATER | F_DYNAMIC_TYPE_2;
    }
    else if (gStatuses4[battlerAtk] & STATUS4_PLASMA_FISTS && moveType == TYPE_NORMAL)
    {
        gBattleStruct->dynamicMoveType = TYPE_ELECTRIC | F_DYNAMIC_TYPE_2;
    }
    else if (move == MOVE_AURA_WHEEL && gBattleMons[battlerAtk].species == SPECIES_MORPEKO_HANGRY)
    {
        gBattleStruct->dynamicMoveType = TYPE_DARK | F_DYNAMIC_TYPE_2;
    }

    // Check if a gem should activate.
    GET_MOVE_TYPE(move, moveType);
    if (holdEffect == HOLD_EFFECT_GEMS
        && moveType == ItemId_GetSecondaryId(gBattleMons[battlerAtk].item))
    {
        gSpecialStatuses[battlerAtk].gemParam = GetBattlerHoldEffectParam(battlerAtk);
        gSpecialStatuses[battlerAtk].gemBoost = TRUE;
    }
}

// special to set a field's totem boost(s)
// inputs:
//  var8000: battler
//  var8001 - var8007: stat changes
void SetTotemBoost(void)
{
    u32 battler = gSpecialVar_0x8000;
    u32 i;

    for (i = 0; i < (NUM_BATTLE_STATS - 1); i++)
    {
        if (*(&gSpecialVar_0x8001 + i))
        {
            gQueuedStatBoosts[battler].stats |= (1 << i);
            gQueuedStatBoosts[battler].statChanges[i] = *(&gSpecialVar_0x8001 + i);
            gQueuedStatBoosts[battler].stats |= 0x80;  // used as a flag for the "totem flared to life" script
        }
    }
}

bool32 IsWildMonSmart(void)
{
#if B_SMART_WILD_AI_FLAG != 0
    return (FlagGet(B_SMART_WILD_AI_FLAG));
#else
    return FALSE;
#endif
}

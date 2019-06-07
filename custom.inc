|***
 *	custom.inc
 *	this is a blank structure file
 *	feel free to add/create your own commands whatever
 *	use the other files as a templates if you need examples
 *  the set_custom_declares() will be loded on mac start if you need to add your own variables
 *
 ***|





|***
 *	DES: custom declares
 *	USE: /call set_declares_custom
 *  NOTE: This function is initialized for you in the macs base prep routine.
 ***|
sub set_declares_custom()

/return




|***custom builds for raid vs group play
 *	USE: /build [raid|group]
 *  NOTE: this is for me, not you ;)
 ***|
#bind build_swap /build
sub Bind_build_swap(string _build)


	/if (!${Bool[${_build}]}) {
		VOUT ${owarning} ${sep} \aw/build\ax [${cnum}name\ax] needs a build name
	}


	/if (${_build.Equal[raid]}) {
		
		OUT Set Raid Build
		
		/call Bind_setCLR retort off TRUE
		/call Bind_setCLR grouphot off TRUE
		/call Bind_setCLR arbitrate off TRUE
		
		/call Bind_buff self off TRUE
		/call Bind_buff bc off TRUE
		/call Bind_buff tell off TRUE
		
		/call Bind_rez take off TRUE
		/call Bind_rez give off TRUE
		
		/call Bind_burn raid off TRUE
		/call Bind_burn auto off TRUE
		/call Bind_burn count 99 TRUE
		
		/showspelleffects off
		
	} else /if (${_build.Equal[group]}) {
		
		OUT Set Group Build
	
		/call Bind_setCLR retort on TRUE
		/call Bind_setCLR grouphot on TRUE
		/call Bind_setCLR arbitrate on TRUE
		
		/call Bind_buff self on TRUE
		/call Bind_buff bc on TRUE
		/call Bind_buff tell on TRUE
		
		/call Bind_rez take on TRUE
		/call Bind_rez give on TRUE
		
		/call Bind_burn raid on TRUE
		/call Bind_burn auto on TRUE
		/call Bind_burn count 3 TRUE

		/showspelleffects on

		
	}

/return





|==========================================================================================================================================
create a DB backup..
why the fuck doesnt this work???



#bind build_backup /backup
sub Bind_build_backup(string _build)

	/declare _stampDate 		string local ${Time.Year}${If[${Time.Month}<10,0${Time.Month},${Time.Month}]}${If[${Time.Day}<10,0${Time.Day},${Time.Day}]}
	/declare _stampTime 		string local ${If[${Time.Hour}<10,0${Time.Hour},${Time.Hour}]}${If[${Time.Minute}<10,0${Time.Minute},${Time.Minute}]}${If[${Time.Second}<10,0${Time.Second},${Time.Second}]}
	/declare _dbCoreBackup 	string local macros\tc\data\dbCore-${_stampDate}${_stampTime}.db

	| /echo backup::${_dbCoreBackup}


| /echo ${_stampDate}${_stampTime}
	| /sqlite ${_dbCoreBackup} dummy CREATE TABLE environment (currentBuild TEXT UNIQUE)
	| /sqlite ${dbCore} dummy INSERT INTO ${_dbCoreBackup}.environment SELECT * FROM environment;



	| /if (${sqlite.Status[dummy].NotEqual[SUCCESS]}) {
	| 	VSQL \arWARNING\ax something is wrong \at..\ax \awdbCore.db\ax -> \aw${_dbCoreBackup}.db\ax
	| } 

| /exec copy .\..\tc\data\dbCore.db dbCore${_stampDate}${_stampTime}.db


| /exec copy "e:\dbCore.db e:\dbCore12.db" bg



/sqlite ${dbCore} query_journal_mode PRAGMA journal_mode
/echo ${sqlite.result[query_journal_mode 1 journal_mode]}


/return

	/sqlite ${_dbCore} dummy INSERT INTO ${_dbCoreBackup}.environment SELECT * FROM environment


${sqlite.Rows[dummy]}

/sqlite DB query PRAGMA journal_mode=WAL

Knightly: /sqlite DB query PRAGMA journal_mode
[10:40 PM] Knightly: ${sqlite.result[query 1 journal_mode]}
[10:40 PM] Knightly: Will tell you the journal mode




|==========================================================================================================================================

templating

a simple. plain english flat file that can be filled out and share. something that can import into core and make a 100% ready character.
will need maps from all entries to existing DB values.







; Core Macro
; exspes.core.com
; template.ini
;
; this provides a template to import settings to creat a character inside a core database
; any field not filled out hree will be imported as FALSE
;
;
; My advice. 
; This is to be use as intended for Cores design. if you try and do stupid shit with it, don't tell me.
; seriously. just fill in the blanks. if your class doesnt use spells. dont put them in there. its not difficult.

; Notes
; thre will NOT be any conditions imported anywhere. dont ask, not doing it.





; chracter server
Server=

; character first name
Name=

; character class
Class=














; == auras (2) =======================================================
; there are 2 usable auras. if your class only uses one aura, then....

; name of the spell, song or disc you wish to use asn an aura (NOT ITEM)
Aura1Spell=

; name of the aura as it appears in your aura window
Aura1Buff=

Aura2Spell=
Aura2Buff=


; == AA Now (2) ======================================================
; name of the AA you always want running on your character

AANow1=
AANow2=


; == Disc Now (2) ====================================================
; name of the Disc you always want running on your character

DiscNow1=
DiscNow2=


; == Item Now (2) ====================================================
; name of the item you always want to have an effect on you, like a geomantra item

ItemNow1=
ItemNow2=

















; == buffs 1-10 ======================================================
; i'm only doing 1 here as an example. you can copy and paste the other 9 if you like. just change the #
; dont overthink it

; name of the buff you want casted
Buff1=

; is this a self buff TRUE/FALSE
Buff1self=

; aliase people /t you to get the buff cased on them. like |HP or |symbol
Buff1Alias=

; list of classes you box you want to have this buff eg:|CLR|RNG|WAR|SHD|PAL
BCBuff1Class=

; list of classes in a raid to use this buff one. eg: |ROG|RNG|BRD|MNK
RaidBuff1Class=

; list of classes you dont box to use this on (Mercs and other RL players)
ManualBuff1Class=






CREATE TABLE CCCCCCCC (currentBuild TEXT UNIQUE, swMaintenance, coreAuto, lstNoCastWith, lstNoMeleeWith, setShrinkItem, setIllusionItem, setIllusionBuff, setGTFO, lstRangeItem, lstRangeItemSummon, strRangeItemCount, strRangeSpell, setRangeClickItem, setRangeDisc, setRangeAA, autoInventoryItem, stoponDS, useGlyph, swSoS, setRestPct, swCorrectEXPAA, setLvlPct, setLvlMax, swEventsInc, swAAFireworks, swTakeGroupInvite, swTakeRaidInvite, swGiveRaidInvite, lstBlockedSpells, setBufftoWatch, swBurnControl, swBurnForce, swBurnAuto, setBurnPct, setBurnCount, swBurnRaid, setMA1, setMA2, setMobAgro, setEngageMAPct, setEngagePct, swAssistG, swAssist1, swAssist2, swAssist3, swAssistF, swAgro, swPull, swAdvPull, swOffTank, setCombatDistance, setCombatRadius, setzradius, swBandolier, setStopNukePct, swAlliance, setAllianceTrigger, swForceFadeRest, swCombatMelee, swCombatRange, swCombatNuke, swCombatDoT, swCombatHeal, swRezTake, swRezToken, lstRezToken, setMinRezPct, setMaxRezRange, swRezGive, swRezCombat, swRezEveryone, spireRaid, spireGroup, watchSpawn, swHomeSet, setHomeRadius, setHomeLeash, setHomeDelayMin, setHomeDelayMax, swStickBehind, swFaceFast, setFaceArc, setMount, setTieToon, setTieDistance, swTieLock, setTieMode, swdeBuffCripple, swdeBuffMezz, swdeBuffMalo, swdeBuffTash, swdeBuffSlow, swdeBuffEradicate, swdeBuffSnare, swOverride, swOverrideLOS, swOverrideSafeZone, swOverrideExclude, swOverrideName, swOverrideEngagePct, swOverrideRange, swOverrideMelee, swOverrideLoot, swOverrideSafeNames, swOverrideTrainSpell, lstBuffRemove, swBuffItem, swBuffCombat, swBuffPet, swBuffSelf, swBuffBC, swBuffRaid, swBuffTell, swBuffMerc, swBuffOther, swBuffMGB, swAutoCursor, setHealOverride, swHealSelf, swHealGroup, swHealXTarget, swHealPet, swHealCure, cureCurseItem, curePoisonItem, cureDiseaseItem, cureCorruptItem, noCureList, clickHealItem, echoChannel, relayTells, ClassChannel, setModeAgro, setModeHarvestCount, modeDrinkofChoice, modeOpenItem, setPetEngagePct, setPetType, swPet, swSwarm, setSwarmEngagePct, swNoSwarmonDS, setFam1, setFam1Buff, swFam1Leave, setFam2, setFam2Buff, swFam2Leave, setFam3, setFam3Buff, swFam3Leave, clickItemBuff1, clickItemBuff2, clickItemBuff3, clickItemBuff4, clickItemBuff5, clickItemBuff6, BuffAAUnity1, BuffAAUnity2, BuffAAUnity3, BuffAAUnity4, BuffAAUnity5, BuffAAUnity6, BuffAAUnity7, clickItem1, clickItem2, clickItem3, clickItem4, clickItem5, clickItem6, clickItemBurn1, clickItemBurn2, clickItemBurn3, clickItemBurn4, clickItemBurn5, clickItemBurn6, Breastplate1, Breastplate1Condition, Breastplate1effect, Breastplate2, Breastplate2Condition, Breastplate2effect, useCircle, CircleofPower, CircleofLife, CircleofMana, clickItemSwarm1, clickItemSwarm2, clickItemSwarm3, clickItemSwarm4, clickItemSwarm5, clickItemSwarm6, clickOddItem1, clickOddItem2, clickOddItem3, clickOddItem4, AANow1, AANow2, DiscNow1, DiscNow2, ItemNow1, ItemNow2, kick, bash, taunt, disarm, bandolierNormal, bandolierBash, bandolierBurn, bandolierRange, Food, FoodDispenser, FoodCount, Drink, DrinkDispenser, DrinkCount, tFoodDrink, TwincastSpell, GatherSpell, SilentCast, switchNukeDelay, buffGem, GoMNuke, GoMNukeCondition, GoMNuke2, GoMNuke2Condition, Buff1, Buff1self, Buff1selfCondition, Buff1Alias, BCBuff1Class, RaidBuff1Class, ManualBuff1Class, ManualBuff1Check, Buff1noBCinRaid, Buff2, Buff2self, Buff2selfCondition, Buff2Alias, BCBuff2Class, RaidBuff2Class, ManualBuff2Class, ManualBuff2Check, Buff2noBCinRaid, Buff3, Buff3self, Buff3selfCondition, Buff3Alias, BCBuff3Class, RaidBuff3Class, ManualBuff3Class, ManualBuff3Check, Buff3noBCinRaid, Buff4, Buff4self, Buff4selfCondition, Buff4Alias, BCBuff4Class, RaidBuff4Class, ManualBuff4Class, ManualBuff4Check, Buff4noBCinRaid, Buff5, Buff5self, Buff5selfCondition, Buff5Alias, BCBuff5Class, RaidBuff5Class, ManualBuff5Class, ManualBuff5Check, Buff5noBCinRaid, Buff6, Buff6self, Buff6selfCondition, Buff6Alias, BCBuff6Class, RaidBuff6Class, ManualBuff6Class, ManualBuff6Check, Buff6noBCinRaid, Buff7, Buff7self, Buff7selfCondition, Buff7Alias, BCBuff7Class, RaidBuff7Class, ManualBuff7Class, ManualBuff7Check, Buff7noBCinRaid, Buff8, Buff8self, Buff8selfCondition, Buff8Alias, BCBuff8Class, RaidBuff8Class, ManualBuff8Class, ManualBuff8Check, Buff8noBCinRaid, Buff9, Buff9self, Buff9selfCondition, Buff9Alias, BCBuff9Class, RaidBuff9Class, ManualBuff9Class, ManualBuff9Check, Buff9noBCinRaid, Buff10, Buff10self, Buff10selfCondition, Buff10Alias, BCBuff10Class, RaidBuff10Class, ManualBuff10Class, ManualBuff10Check, Buff10noBCinRaid, BuffMGBname, BuffMGBchk, BuffMGBalias, BuffMGBcount, BuffMGBmana, Aura1Spell, Aura1Buff, Aura2Spell, Aura2Buff, PetPoint, CLRPoint, DRUPoint, SHMPoint, WIZPoint, MAGPoint, ENCPoint, NECPoint, WARPoint, MNKPoint, ROGPoint, RNGPoint, BSTPoint, SHDPoint, BRDPoint, BERPoint, PALPoint, GoMpoint, PanicPoint, XHealClass, tXHeal, cureCurse, curePoison, cureDisease, cureCorrupt, switchShrink, switchHorn, abjuration, alteration, conjuration, divination, evocation, CLRAlliance, RampageTank, switchAAFade, useAAFadePct, tHealWard, switchLockMA, swModRod, switchVP, useVPPct, spellCripple, BreastplateHeal, BreastplateHealCondition, DivineRezCleric, ProtectOutsideGroup, ProtectInsideRaid, ProtectNPCList, useGroupHeal, useGroupHoT, setDivinetoGroupHeal, useCelestialRegen, CRTXT, switchForceGroupHeal, switchHealAggressive, switchCompleteHeal, CompleteHealpct, swArbitrate, healWholeGroup, healWholeGroup2, healWholeGroup3, healWholeGroupHoT, healRemedy1, healRemedy2, healRemedy3, healHoT, swSplash, splashMobCount, healSplash, healIntervention1, healIntervention2, healIntervention3, healIntervention4, healContravention1, healContravention2, healContravention3, healContravention4, healRenewal1, healRenewal2, healRenewal3, healPanic, healWard, healWardToon, switchIssuance, issuanceMobCount, healIssuance, doYaulp, spellYaulp, sumWeaponSpell, sumWeaponItem, tempPet, swRetort, spellRetort, spellRetortToon, Nuke1, Nuke1Condition, Nuke2, Nuke2Condition, Nuke3, Nuke3Condition, Nuke4, Nuke4Condition, Nuke5, Nuke5Condition, Nuke6, Nuke6Condition, Nuke7, Nuke7Condition, Nuke8, Nuke8Condition, DoT1, DoT1chk, DoT1Condition, DoT2, DoT2chk, DoT2Condition, DoT3, DoT3chk, DoT3Condition, DoT4, DoT4chk, DoT4Condition, DoT5, DoT5chk, DoT5Condition, DoT6, DoT6chk, DoT6Condition, DoT7, DoT7chk, DoT7Condition, DoT8, DoT8chk, DoT8Condition, tRez_Retry, tCheck_snap_retry, tCheck_Runes, tScorchedEarth_ignore, tScorchedEarth_fail, tTry_Door, tScorchedEarth_delay, tCheck_Familiar, tHarvest_delay, tCheck_EQBC, tOdds_and_Ends, tCheck_EXP, tCheck_Tie, tWatch_Spawn, tCast_Cure, tPetfarm_delay, tClear_Cursor, tPet_Buff, tSelf_Buff, tItem_Buff, tBC_Buff, tRaid_Buff, tMerc_Buff, tOther_buff, tTell_Buff, tMGB_Buff, tBlocked_Buff, tCheck_Aura, tCheck_Ammo, tHunt_delay, tAuto_Control_Toon, tZone_Pause, tHUD_Death_Check, tNuke_Delay, tWaste_Fireworks, tValidate_Zone, tMode_Open, tZoned_Delay, tCheck_status, tCheck_Observers, tAsk_for_rod, tAsk_for_weapons)







|***
 *	USE: /copydb <server> <class> <character>
 *	NOTE: This will copy an existing character database file over your current character.db file.
 ***|
#bind copydb /copydb
sub Bind_copydb(string _server, string _class, string _character, bool _echoOnly)

    | Validate Inputs
    /if (${_server.Length} == 0 || ${_class.Length} == 0 || ${_character.Length} == 0) {
        OUT Usage: /copydb <server> <class> <character> [echo only 0 | 1]
        /return
    }
    /if (!${Defined[_echoOnly]}) /varset _echoOnly false

    /declare _copyToDb string local macros\\tc\\data\\${MacroQuest.Server}${Me.Class.ShortName}${Me.DisplayName}.db
    /declare _copyToTable string local ${MacroQuest.Server}_${Me.Class.ShortName}_${Me.DisplayName}
   
    /declare _copyFromDb string local macros\\tc\\data\\${_server}${_class.Upper}${_character}.db
    /declare _copyFromTable string local ${_server}_${_class}_${_character}

 
 
    OUT copydb :: copying... \n${_copyFromDb} -> ${_copyToDb}... \n${_copyFromTable} -> ${_copyToTable}
    
 
 
 
    | Row Check to see if copyFromDb exists, else exit
    /sqlite ${_copyFromDb} rowCount SELECT 1 FROM ${_copyFromTable};
    /if (!${sqlite.Rows[rowCount]}) {
        OUT ${_copyFromDb} does not exist.
        /return
    }

    /declare _cmd string local sqlite ${_copyToDb} dummy ATTACH '.\\${_copyFromDb}' as 'source'; DROP TABLE ${_copyToTable}; CREATE TABLE ${_copyToTable} as SELECT * FROM source.${_copyFromTable}; DETACH 'source';


    /if (${_echoOnly}) {
        OUT /${_cmd}
    } else {
        /if (TRUE) /${_cmd}
        /sqlite ${_copyToDb} dummyCreate SELECT 1 FROM ${_copyToTable};
    }

    | Row check to see if copyToDb/Table exists, notify success/failure.
    /if (${sqlite.Rows[dummyCreate]}) {
        OUT ${_copyFromDb} was successfully copied to ${_copyToDb}.
        /delay 1s
       |  /call Bind_admin forceupdate
    } else {
        OUT Failed to copy ${_copyToDb}.
        OUT Create Table Query Status: ${sqlite.Status[dummy]}
        OUT ${sqlite.Status[dummyCreate]}
    }

/return








sub set_declares_custom()
    /declare memCurrentGem int outer 1
/return

sub mem_reset()
    /varset memCurrentGem 1
/return

sub try_mem(string spellName)
    /if (${Me.Gem[${spellName}]} && ${Me.Gem[${spellName}]} < ${memCurrentGem}) /return
    /if (${memCurrentGem} > ${Me.NumGems}) {
        VSPELL Not enough gems to memorize ${spellName}
        /return
    }
    /call mem_spell TRUE "${spellName}" ${memCurrentGem} FALSE TRUE
    /varcalc memCurrentGem ${memCurrentGem}+1
/return

#bind memall /memall
sub Bind_memall()
    /call memall_${Me.Class.ShortName}
/return

sub memall_WIZ()
    /declare _index                 int         local 0
    /declare _list                     string     local ${line${lineCurrent}}
    /call mem_reset
    /if (${Bool[${TwincastSpell}]}) /call try_mem "${TwincastSpell}"
    /if (${Bool[${GatherSpell}]}) /call try_mem "${GatherSpell}"
    /if (${Bool[${GoMNuke}]}) /call try_mem "${GoMNuke}"
    /if (${Bool[${GoMNuke2}]}) /call try_mem "${GoMNuke2}"
    /if (${swAlliance} && ${Bool[${WIZAlliance}]}) /call try_mem "${WIZAlliance}"
    /call try_mem "${NukeTwincast${lineCurrent}}"
    /for _index 1 to ${_list.Count[|]}
        /call try_mem "${_list.Arg[${_index},|]}"
    /next _index






|***
 *	pregen.inc
 *	Core Data Generation
 *
 *
 ***|
 
 
 
 
 |***
 *	DES: basic common use arrays
 *	USE: 
 *	NOTE: 
 ***|
sub pregen_data_arrays()
	/declare _count int local 0

	| sin/cos generation for ranged movement combat
	/declare cos[20] float outer
	/declare sin[20] float outer
	/for _count 1 to 20 
		/varcalc cos[${_count}] ${Math.Cos[9 * ${_count}]}
		/varcalc sin[${_count}] ${Math.Sin[9 * ${_count}]}
	/next _count	

	| schema
	/if (!${Defined[sqlTables]}) /declare sqlTables 										string outer |environment|bitch|expansions|lists|${chrID}
	/if (!${Defined[sqlTablesCount]}) /declare sqlTablesCount 					int outer ${sqlTables.Count[|]}

	| switches for table build bypass if build is current
	/for _count 1 to ${sqlTables.Count[|]}
		/if (!${Defined[bld${sqlTables.Arg[${_count},|]}current]}) {
			/declare bld${sqlTables.Arg[${_count},|]}current								bool outer FALSE
		}
	/next _count
	
	| Invisibilities
	/if (!${Defined[lstIV]}) /declare lstIV[2] 													string 	outer
	/if (!${Defined[lstIVU]}) /declare lstIVU[1]												string 	outer
	/varset lstIV[1] 										|Shared Perfected Camouflage|Shared Camouflage|Camouflage|Group Perfected Invisibility I|Invisibility|Perfected Invisibility|Phase March|Nethervoid March|Perfected Silent Presence|Cloak of Shadows|Cloak of Shadows I|Cloak of Shadows II|Gather Umbra|Invisibility Cloak
	/varset lstIV[2] 										|Perfected Natural Invisibility|Perfected Spirit Walk Invisibility|Perfected Group Spirit Walk Invisibility|Shauri's Sonorous Clouding|Shauri's Sonorous Clouding I|Shauri's Sonorous Clouding II|Shauri's Sonorous Clouding III|Selo's Song of Travel
	/varset lstIVU[1] 									|Group Perfected Invisibility to Undead|Perfected Invisibility to Undead|Invisibility to Undead|Invisibility versus Undead|Shauri's Sonorous Clouding|Shauri's Sonorous Clouding I|Shauri's Sonorous Clouding II|Shauri's Sonorous Clouding III

	| environment .. /environment []
	/if (!${Defined[lstTypesEnvironment]}) /declare lstTypesEnvironment[3]			string outer
	/varset lstTypesEnvironment[1]			|advloot|bitch|buffgemhold|combattie|delayzoning|dispbuild|dispinfo|dispexp|eqcash|glyphoutsideraid|manualaadiscnow|maxbuffcount|mealinmanual|moblvmax|moblvlmin|modepersistant|mqcaptions|mqclear|mqfontsize|nukeloop|restfull|safelistguild|TLP
	/varset lstTypesEnvironment[2]			|echoalt|echospell|echodisc|echoitem|echomelee|echopulse|echobc|echoverbose|echosql
	/varset lstTypesEnvironment[3]			|bczonereset|bcstop|bctake|bcenter|bcinvis|bckillmob|bcclean

/return



|***
 *	DES: generates the common use environment for Core
 *	USE: 
 *	NOTE: 
 ***|
sub pregen_environment()
	/call setDeclare swEchosSQL																					bool			outer TRUE
	                                                          
	| this is a makeshift /doevents to force binds to be cycled
	/call setDeclare bindTrigger 																				bool 			outer FALSE

	| holder for the current sub for debugging purposes       	
	/call setDeclare currentdebugsub 																		string 			outer

	| current assigned target
	/call setDeclare Target2Kill																				int				outer 0

	| Base Environment settings
	/call setDeclare forceINIcheck																			string 		outer TRUE
	
	| casting checks
	/call setDeclare castReturn																					string 		outer
	/call setDeclare CheckResisted																			int		 		outer 0
	/call setDeclare castBreak																					bool	 		outer FALSE

		
	| macro communications channel (group) for DanNet
	/call setDeclare DanNetMacroChannel																	string 		outer core_communications
	| join the group
	/djoin ${DanNetMacroChannel}

	| remote kill key (not documented)
	/call sql_control FALSE environment remoteKillKey										string		outer FALSE
		
	/call setDeclare combat																							string		outer FALSE
	/call setDeclare isValidTarget																			bool			outer FALSE
	/call setDeclare noInvis 																						bool 			outer TRUE
	/call setDeclare noFeigning 																				bool 			outer TRUE
	/call setDeclare currentLootCount																		int				outer 0
	/call setDeclare swNeedRez																					bool			outer FALSE
	/call setDeclare swAmIDead																					bool			outer FALSE
	
	/call sql_control FALSE environment swTLP														bool	 		outer FALSE
	/call sql_control FALSE environment swLog														bool	 		outer FALSE
	/call sql_control FALSE environment setMobLvlMin										int				outer 1
	/call sql_control FALSE environment setMobLvlMax										int				outer 150
	/call sql_control FALSE environment swBitch													bool			outer TRUE
	/call sql_control FALSE environment swNukeLoop											bool			outer FALSE
	/call sql_control FALSE environment swAddNameList										bool			outer TRUE
	/call sql_control FALSE environment swDispInfo											bool			outer TRUE
	/call sql_control FALSE environment swDispBuild											bool			outer TRUE
	/call sql_control FALSE environment swDispEXP												bool			outer TRUE
	/call sql_control FALSE environment swManualAADiscNow								bool			outer FALSE
	/call sql_control FALSE environment swDelayZoning										bool			outer TRUE
	/call sql_control FALSE environment swRestFull											bool			outer FALSE
	/call sql_control FALSE environment swRestModRod										bool			outer TRUE
	/call sql_control FALSE environment swRestBuff											bool			outer TRUE
	/call sql_control FALSE environment swADVLoot												bool			outer TRUE
	/call sql_control FALSE environment swGlyphOutsideRaid							bool			outer FALSE
	/call sql_control FALSE environment swMQClear												bool			outer FALSE
	/call sql_control FALSE environment swSafelistGuild									bool			outer FALSE
	/call sql_control FALSE environment swEQCash												bool			outer TRUE
	/call sql_control FALSE environment swModePersistent								bool			outer FALSE
	/call sql_control FALSE environment swCombatTie											bool			outer FALSE
	/call sql_control FALSE environment swDisableMQ2Melee								bool			outer TRUE
	/call sql_control FALSE environment swXpacControl										bool			outer TRUE
	/call sql_control FALSE environment swBuffGemHold										bool			outer TRUE
	/call sql_control FALSE environment setMQFontSize										int				outer 3
	/call sql_control FALSE environment swMQCaptions										bool			outer off
	/call sql_control FALSE environment swAutoAssist										bool			outer off
	/call sql_control FALSE environment setMaxBuffCount									int				outer 10
	/call sql_control FALSE environment swMealinManual									bool			outer FALSE
	/call sql_control FALSE environment lstNoInvite											string 		outer "|FALSE"
	/call sql_control FALSE environment lstNoRez												string 		outer "|FALSE"
	/call sql_control FALSE environment lstNoHeal												string 		outer "|FALSE"
	/call sql_control FALSE environment lstSafeNames										string 		outer "|FALSE"

	/call sql_control FALSE environment lstResidentMage									string		outer "|FALSE"
	/call sql_control FALSE environment lstResidentDruid								string		outer "|FALSE"
	/call sql_control FALSE environment lstResidentWizard								string		outer "|FALSE"
	/call sql_control FALSE environment lstResidentCleric								string		outer "|FALSE"

	| broadcast switches
	/call sql_control FALSE environment swBCZonereset										bool		 	outer TRUE
	/call sql_control FALSE environment swBCStop												bool		 	outer TRUE
	| /call sql_control FALSE environment swBCHere												bool		 	outer TRUE
	/call sql_control FALSE environment swBCTake												bool		 	outer TRUE
	/call sql_control FALSE environment swBCEnter												bool		 	outer TRUE
	/call sql_control FALSE environment swBCInvis												bool		 	outer TRUE
	/call sql_control FALSE environment swBCKillMob											bool		 	outer TRUE
	/call sql_control FALSE environment swBCClean												bool		 	outer TRUE
	/call sql_control FALSE environment swBCAnchor											bool		 	outer TRUE

	| environment echo switches
	/call sql_control FALSE environment swEchosAlt											bool			outer TRUE
	/call sql_control FALSE environment swEchosSpell										bool			outer TRUE
	/call sql_control FALSE environment swEchosDisc											bool			outer TRUE
	/call sql_control FALSE environment swEchosItem											bool			outer TRUE
	/call sql_control FALSE environment swEchosMelee										bool			outer TRUE
	/call sql_control FALSE environment swEchosPulse										bool			outer TRUE
	/call sql_control FALSE environment swEchosBC												bool			outer FALSE
	/call sql_control FALSE environment swEchosVerbose									bool			outer TRUE
	/call sql_control FALSE environment swEchosSQL											bool			outer TRUE
   		
	| visual outputs
	/call setDeclare cnum																								string		outer "\a-t"
	/call setDeclare ctar																								string		outer "\a-w"
	/call setDeclare cpc																								string		outer "\a-w"
	/call setDeclare csp																								string		outer "\a-r"
	/call setDeclare cbad																								string		outer "\ar"
	/call setDeclare cgood																							string		outer "\ag"
	/call setDeclare cbug																								string		outer "\at"
	/call setDeclare cinfo																							string		outer "\aw"
	/call setDeclare citem																							string		outer "\au"
	/call setDeclare sep																								string		outer "\at..\ax"
	/call setDeclare sepO																								string		outer "\at[\ax"
	/call setDeclare sepC																								string		outer "\at]\ax"
	/call setDeclare oenabled																						string		outer "\agEnabled\ax"
	/call setDeclare oon																								string		outer "\agon\ax"
	/call setDeclare odisabled																					string		outer "\arDISABLED\ax"
	/call setDeclare ooff																								string		outer "\aroff\ax"
	/call setDeclare owarning																						string		outer "\arWARNING\ax"
	/call setDeclare ofatal																							string		outer "\arFATAL ERROR\ax \at..\ax \arENDING MACRO\ax"
	/call setDeclare ocaution																						string		outer "\arCAUTION\ax"
	/call setDeclare otrue																							string		outer "\arTRUE\ax"
	/call setDeclare ofalse																							string		outer "\arFALSE\ax"
	/call setDeclare odestroy																						string		outer "\arDESTROY\ax"
	/call setDeclare oleave																							string		outer "\awLeave\ax"
	/call setDeclare oannounce																					string		outer "\amAnnounce\ax"
	/call setDeclare oadd																								string		outer "\agAdd\ax"
	/call setDeclare okeep																							string		outer "\agKeep\ax"
	/call setDeclare onotice																						string		outer "\ayNotice\ax"
 	/call setDeclare ocombat																						string		outer "\amCombat\ax"
	/call setDeclare oremove																						string		outer "\arRemoving\ax"
	/call setDeclare obroadcasted																				string 		outer "\amBroadcasted\ax"
	/call setDeclare wikiswitch																					string 		outer "[\a-wswitch\ax]"
	/call setDeclare wikitarget																					string 		outer "[\a-wTARGET\ax]"
	/call setDeclare wikiname																						string 		outer "[\a-wName\ax]"
	/call setDeclare wikispire																					string 		outer "[\ar0\ax\a-w|1|2|3\ax]"
	/call setDeclare wikipct																						string 		outer "[\a-w1,99\ax]"

	| HUD controls
	/call setDeclare setHUDBroadcast																		string		outer
	/call setDeclare hudWarningLength																		int				outer	0
	/call setDeclare hudNoticeLength																		int				outer	0
	/call setDeclare hudTargetLength																		int				outer	0
	/call setDeclare hudNotice																					string		outer FALSE
	/call setDeclare hudWarning																					string		outer	FALSE
	/call setDeclare hudTarNotice																				string		outer
	/call setDeclare tmpMode																						string		outer FALSE
	/call sql_control FALSE environment swHUDTime												bool 			outer TRUE

	| buff queue variables
	/call setDeclare queueCountMax																			int				outer 10
	/call setDeclare queueCount            															int				outer 0
	/if (!${Defined[queueBuffID]}) /declare queueBuffID[8] 							int  			outer
	/if (!${Defined[queueList]}) /declare queueList[${queueCountMax},2] int  			outer

	| set /crew stuffs
	/call sql_control FALSE environment lstCrew1												string		outer "|FALSE"
	/call sql_control FALSE environment setCrew1Leader									string		outer FALSE
	/call sql_control FALSE environment setCrew1Assist									string		outer FALSE
	/call sql_control FALSE environment setCrew1Looter									string		outer FALSE
	/call sql_control FALSE environment lstCrew2												string		outer "|FALSE"
	/call sql_control FALSE environment setCrew2Leader									string		outer FALSE
	/call sql_control FALSE environment setCrew2Assist									string		outer FALSE
	/call sql_control FALSE environment setCrew2Looter									string		outer FALSE
                                                            					
	| /here settings                                          					
	/call sql_control FALSE environment makeVisible											bool			outer TRUE
	/call sql_control FALSE environment clearTarget											bool			outer TRUE
	/call sql_control FALSE environment defaultTimerDelay								string		outer 1m
	/call sql_control FALSE environment baseAgroRange										int				outer 60
	/call sql_control FALSE environment emptyHands											bool			outer TRUE
	/call sql_control FALSE environment hereZRadius											int				outer 30

	| zone - core.zone.ini settings
	/call setDeclare currentZoneID																			int				outer	0
	/call setDeclare SafeZone		       																	bool			outer FALSE
	/call setDeclare RangeList		   																		string		outer FALSE
	/call setDeclare ExcludeList    																		string		outer FALSE
	/call setDeclare NameList       																		string		outer FALSE
	/call setDeclare NoMeleeList     		  															string		outer FALSE
	/call setDeclare NoCastList       																	string		outer FALSE
	/call setDeclare NoMezzList       																	string		outer FALSE
	/call setDeclare NoDiseaseList       																string		outer FALSE
	/call setDeclare NoMagicList       																	string		outer FALSE
	/call setDeclare NoPoisonList       																string		outer FALSE
	/call setDeclare NoFireList       																	string		outer FALSE
	/call setDeclare NoCorruptionList      															string		outer FALSE
	/call setDeclare NoColdList       																	string		outer FALSE
	/call setDeclare CharmMobList      																	string		outer FALSE
	/call setDeclare FullZoneName      																	string		outer FALSE
	/call setDeclare HuntMobList       																	string		outer FALSE
	/call setDeclare HarvestList       																	string		outer FALSE
/return



|***
 *	DES: static lists
 *	USE: 
 *	NOTE: 
 ***|
sub pregen_lists()
	| lists for .. you know..lists and stuff. cause we like lists.                 
	/call setDeclare lstClassAll																				string 		outer "|WAR|PAL|SHD|BRD|RNG|BST|BER|MNK|ROG|WIZ|MAG|ENC|NEC|CLR|DRU|SHM"
	/call setDeclare lstClassMelee    		  														string 		outer "|WAR|ROG|BST|MNK|SHD|PAL|BER|BRD|RNG"
	/call setDeclare lstClassCast																				string 		outer "|BST|SHD|PAL|RNG|CLR|DRU|SHM|ENC|WIZ|NEC|MAG"
	/call setDeclare lstClassPet																				string 		outer "|ENC|DRU|SHM|SHD|WIZ|BRD|BST|MAG|NEC"
	/call setDeclare lstClassHeal																				string 		outer "|CLR|DRU|SHM|PAL"
	/call setDeclare lstClassRez																				string 		outer "|CLR|DRU|SHM|PAL|NEC"
	/call setDeclare lstClassAASnare																		string 		outer "|RNG|DRU|SHD|NEC|ROG|WAR|WIZ|MNK|PAL|BER"
	/call setDeclare lstClassAAEradicate																string 		outer "|RNG|DRU|NEC|WIZ|ENC|MAG"

	| plugins
	/call sql_staticbuild FALSE lists lstPlugin													"|MQ2Cast|MQ2Exchange|MQ2MoveUtils|MQ2Debuffs|MQ2DanNet|MQ2SQLite"

	| buffs .. /buff []
  /call sql_staticbuild FALSE lists lstTypesBuff											"|Self|Item|Combat|BC|Raid|Tell|Pet|Merc|Other|MGB"

	| overrides .. /override []
	/call sql_staticbuild FALSE lists lstTypesOverride									"|EngagePct|Exclude|Loot|LOS|Melee|Name|Range|SafeNames|SafeZone|TrainSpell"

	| debuffs .. /debuff []
	/call sql_staticbuild FALSE lists lstTypesdeBuff										"|Mezz|Malo|Tash|Cripple|Slow|Eradicate|Snare"

	| heals .. /heal []
	/call sql_staticbuild FALSE lists lstTypesHeal											"|Self|Group|XTarget|Pet|Cure|Override"
	
	| combat .. /cc []
	/call sql_staticbuild FALSE lists lstTypesCC												"|agro|alliance|bandolier|circle|combatdistance|engage|faderest|force|melee|meleedisc|range|nuke|dot|heal|forceassist|nukedelay|offtank|pull|advpull|rest|saferadius|smartengage|stopnuke|zradius"

	| core .. /core []
	/call sql_staticbuild FALSE lists lstTypesCore											"|auto|checkcash|events|expadjust|expmaintain|explevel|fireworks|takeraidinvite|takegroupinvite|giveraidinvite"

	| HUD .. /hc []
	/call sql_staticbuild FALSE lists lstTypesHUD												"|time|update|delete"

	| burns .. /burn []
	/call sql_staticbuild FALSE lists lstTypesBurn											"|auto|count|engage|force|raid"

	| rezurection .. /rez []
  /call sql_staticbuild FALSE lists lstTypesRez												"|Combat|Everyone|Give|Pct|Range|Take|Token"
	| rezurection % .. /rex pct []  
	/call sql_staticbuild FALSE lists lstTypesRezPct										"|10|20|35|50|60|75|90|93|96"

	| pet crap .. /pc []
	/call sql_staticbuild FALSE lists lstTypesPC												"|dsswarm|pet|swarm|type|build|bag"

	| tie .. /tc
	/call sql_staticbuild FALSE lists lstTypesTC												"|distance|lock|mode|name|time"

	| off modes .. /mode []
	/call sql_staticbuild FALSE lists lstTypesMode											"|Agro|Collect|Drag|Drink|Fish|Forage|Harvest|HarvestCount|Hunt|Lush|Open|Petfarm|Scorch|Skill|Summon|Tradeskill|Trainspell"

	| spires .. /cls spire []
	/call sql_staticbuild FALSE lists lstTypesSpire											"|0|1|2|3"

	| loot .. /cursor []
	/call sql_staticbuild FALSE lists lstTypesCursor										"|Announce|Auto|Destroy|Ignore|Keep|Leave|Pass|Pause|Sell|What"

	/call sql_staticbuild FALSE lists ZoneFileList											"|SafeZone|NameList|ExcludeList|NoMezzList|RangeList|NoCastList|NoMeleeList|HuntMobList|CharmMobList|NoFireList|NoPoisonList|NoMagicList|NoCorruptionList|NoColdList|NoDiseaseList|HarvestList"
	/call sql_staticbuild FALSE lists lstTypesLC												"|SafeZone|Remove|Exclude|Name|NoMezz|NoCast|Range|NoMelee|NoMagic|NoDisease|NoFire|NoPoison|NoCold|NoCorruption|HuntMob"

	| movement .. /movement []
  /call sql_staticbuild FALSE lists lstTypesMovement									"|Arc|Behind|Fast|Gather|GOTO|Home|Leash|Mount|Radius"
  
	| port clickies .. /port item []
	/call sql_staticbuild FALSE lists lstTypesPortItem									"|primary|secondary|door|binden|stein|mirror|iksarstone|warportal|pokbrick|fellowship|brells|scepter|lamp|rofsignet|jaggedpine|tolans|touchstone|gorowyn|slide|arx|stratos|faithstone|gyrospire"


	| guild Hall port clickies .. /port guild []
	/call sql_staticbuild FALSE lists lstTypesPortGuildHall							"|lobby|TBL|qeynos|highhold|TBM|EoK|RoS|neriak1|neriak2|neriak3|neriakf|PoM|surefall|halas"

	| guild Hall port clickies .. /port aa []
  /call sql_staticbuild FALSE lists lstTypesPortAA										"|origin"

	| admin stuff .. /admin []
  /call sql_staticbuild FALSE lists lstTypesAdmin											"|blockedbuffs|forceupdate|get|journalmode|list|maintenance|migrate|notice|observer|reset|safelist|table|whitewash|xt|zonereset"

 	| clickie item on off -- /onoff []
  /call sql_staticbuild FALSE lists lstTypesOnOff											"|horn|shrink"
  /call sql_staticbuild FALSE lists tsType														"|movebank"
  
/return



|***
 *	DES: the smart ass response system
 *	USE: 
 *	NOTE: 
 ***|
sub pregen_bitch()

	| please take a number and have a fucking seat.
	/call setDeclare bitchcount																					string 		outer 37

	/call sql_staticbuild FALSE bitch bitch1														"Seriously... Again..."
	/call sql_staticbuild FALSE bitch bitch2														"Did your parents ever ask you to run away from home?"
	/call sql_staticbuild FALSE bitch bitch3														"How do you have the energy to talk so much?"
	/call sql_staticbuild FALSE bitch bitch4														"I wish I didn't have any responsibilities like you."
	/call sql_staticbuild FALSE bitch bitch5														"Aha, I see the Fuck-Up Fairy has visited us again!"
	/call sql_staticbuild FALSE bitch bitch6														"Will trade a Halfling for a rez, or three Gn0mes."
	/call sql_staticbuild FALSE bitch bitch7														"I'm sorry I bit you... pulled your hair... and punched you in the face..."
	/call sql_staticbuild FALSE bitch bitch8														"Go ahead, tell them everything you know. It'll only take 10 seconds."
	/call sql_staticbuild FALSE bitch bitch9														"Sometimes the first step to forgiveness, is realising the other person was born an idiot."
	/call sql_staticbuild FALSE bitch bitch10														"I'm getting Disc lag . . . again."
	/call sql_staticbuild FALSE bitch bitch11														"oh man....i figured out why i was lagging the other day...i look in my case and my kids knocked off my cpu fan. -- "
	/call sql_staticbuild FALSE bitch bitch12														"I'm jealous of all the people that haven't met you!"
	/call sql_staticbuild FALSE bitch bitch13														"You're slower than a herd of turtles stampeding through peanut butter."
	/call sql_staticbuild FALSE bitch bitch14														"Oh good, my dog found a chainsaw."
	/call sql_staticbuild FALSE bitch bitch15														"You are not as bad as people say, you are much, much worse."
	/call sql_staticbuild FALSE bitch bitch16														"Stupidity is not a crime so you are free to go."
	/call sql_staticbuild FALSE bitch bitch17														"AFK, playing Pokemon. More productive then this shit."
	/call sql_staticbuild FALSE bitch bitch18														"All I did was move the mouse over it."
	/call sql_staticbuild FALSE bitch bitch19														"You sound reasonable. It must be time to up my medication!"
	/call sql_staticbuild FALSE bitch bitch20														"I'll never forget the first time we met. Although, I'll keep trying."
	/call sql_staticbuild FALSE bitch bitch21														"Roses are red. Violets are blue. You're an idiot."
	/call sql_staticbuild FALSE bitch bitch22														"Sometimes, when I close my eyes, I... can't see you."
	/call sql_staticbuild FALSE bitch bitch23														"Shock me. Say something intelligent."
	/call sql_staticbuild FALSE bitch bitch24														"I'm typing this with my middle finger."
	/call sql_staticbuild FALSE bitch bitch25														"It takes courage just to be you. To get out of bed every single day, knowing full well, you gotta be you."
	/call sql_staticbuild FALSE bitch bitch26														"For someone with such a smart mouth you sure do have a lot of teeth."
	/call sql_staticbuild FALSE bitch bitch27														"Do you want a piece of gum?"
	/call sql_staticbuild FALSE bitch bitch28														"Good thing you're pretty."
	/call sql_staticbuild FALSE bitch bitch29														"You breathe through your mouth a lot don't you?"
	/call sql_staticbuild FALSE bitch bitch30														"Your mother was a hamster and your father smelt of elderberries!"
	/call sql_staticbuild FALSE bitch bitch31														"Don't worry about it too much. I did the same thing when I was eleven."
	/call sql_staticbuild FALSE bitch bitch32														"As the sun rose, Dawn crept across the lawn.. Searching for her car keys."
	/call sql_staticbuild FALSE bitch bitch33														"Thank you for nothing... You useless reptile."
	/call sql_staticbuild FALSE bitch bitch34														"Cows are like women, the crazy ones taste the best."
	/call sql_staticbuild FALSE bitch bitch35														"I think the real problem is someone made a stupid decision."
	/call sql_staticbuild FALSE bitch bitch36														"no one plays like you cause you are dumb"
	/call sql_staticbuild FALSE bitch bitch37														"When you're dead, you dont know you're dead. The pain is only felt by others. The same thing happens when you're stipid."
/return



|***
 *	DES: expansion listing
 *	USE: 
 *	NOTE: 
 ***|
sub pregen_expansions()
	/call sql_staticbuild FALSE expansions xpac1												"The Ruins of Kunark"
	/call sql_staticbuild FALSE expansions xpac2												"The Scars of Velious"
	/call sql_staticbuild FALSE expansions xpac3												"The Shadows of Luclin"
	/call sql_staticbuild FALSE expansions xpac4												"The Planes of Power"
	/call sql_staticbuild FALSE expansions xpac5												"The Legacy of Ykesha"
	/call sql_staticbuild FALSE expansions xpac6												"Lost Dungeons of Norrath"
	/call sql_staticbuild FALSE expansions xpac7												"Gates of Discord"
	/call sql_staticbuild FALSE expansions xpac8												"Omens of War"
	/call sql_staticbuild FALSE expansions xpac9												"Dragons of Norrath"
	/call sql_staticbuild FALSE expansions xpac10												"Depths of Darkhollow"
	/call sql_staticbuild FALSE expansions xpac11												"Prophecy of Ro"
	/call sql_staticbuild FALSE expansions xpac12												"The Serpent's Spine"
	/call sql_staticbuild FALSE expansions xpac13												"The Buried Sea"
	/call sql_staticbuild FALSE expansions xpac14												"Secrets of Faydwer"
	/call sql_staticbuild FALSE expansions xpac15												"Seeds of Destruction"
	/call sql_staticbuild FALSE expansions xpac16												"Underfoot"
	/call sql_staticbuild FALSE expansions xpac17												"House of Thule"
	/call sql_staticbuild FALSE expansions xpac18												"Veil of Alaris"
	/call sql_staticbuild FALSE expansions xpac19												"Rain of Fear"
	/call sql_staticbuild FALSE expansions xpac20												"Call of the Forsaken"
	/call sql_staticbuild FALSE expansions xpac21												"The Darkened Sea"
	/call sql_staticbuild FALSE expansions xpac22												"The Broken Mirror"
	/call sql_staticbuild FALSE expansions xpac23												"Empires of Kunark"
	/call sql_staticbuild FALSE expansions xpac24												"Ring of Scale"
	/call sql_staticbuild FALSE expansions xpac25												"The Burning Lands"
/return	



|***
 *	DES: generates the characters environment
 *	USE: 
 *	NOTE: 
 ***|
sub pregen_character()
	/declare _count int local 0

	| maintenance flag... 
	/call sql_control FALSE ${chrID} swMaintenance		 									bool			outer FALSE

	| base lists for mob mez control
	/call setDeclare mobList																						string		outer
	/call setDeclare addList 																						string 		outer	

	/call sql_control FALSE ${chrID} coreAuto				 										bool			outer FALSE
	/call sql_control FALSE ${chrID} lstNoCastWith											string		outer "|FALSE"
	/call sql_control FALSE ${chrID} lstNoMeleeWith											string		outer "|FALSE"
	/call sql_control FALSE ${chrID} setShrinkItem 											string 		outer FALSE
	/call sql_control FALSE ${chrID} setIllusionItem										string		outer FALSE
	/call sql_control FALSE ${chrID} setIllusionBuff										string		outer FALSE
	/call sql_control FALSE ${chrID} setGTFO														string		outer FALSE
	/call sql_control FALSE ${chrID} lstRangeItem												string		outer "|FALSE"
	/call sql_control FALSE ${chrID} lstRangeItemSummon									string		outer "|FALSE"
	/call sql_control FALSE ${chrID} strRangeItemCount									int				outer 5
	/call sql_control FALSE ${chrID} strRangeSpell											string		outer FALSE
	/call sql_control FALSE ${chrID} setRangeClickItem									string		outer FALSE
	/call sql_control FALSE ${chrID} setRangeDisc												string		outer FALSE
	/call sql_control FALSE ${chrID} setRangeAA													string		outer FALSE
	/call sql_control FALSE ${chrID} autoInventoryItem									string		outer "|FALSE"
	/call sql_control FALSE ${chrID} stoponDS														bool			outer FALSE		
	/call sql_control FALSE ${chrID} useGlyph														string		outer FALSE
	/call sql_control FALSE ${chrID} swSoS															bool			outer FALSE
	/call sql_control FALSE ${chrID} setRestPct 												int 			outer 90
	/call sql_control FALSE ${chrID} swCorrectEXPAA 										bool 			outer TRUE
	/call sql_control FALSE ${chrID} setLvlPct   												int 			outer 99
	/call sql_control FALSE ${chrID} setLvlMax      										int 			outer 110
	/call sql_control FALSE ${chrID} swEventsInc  											bool 			outer TRUE
	/call sql_control FALSE ${chrID} swAAFireworks											bool 			outer FALSE
	/call sql_control FALSE ${chrID} swTakeGroupInvite									bool 			outer FALSE
	/call sql_control FALSE ${chrID} swTakeRaidInvite										bool 			outer FALSE
	/call sql_control FALSE ${chrID} swGiveRaidInvite										bool 			outer FALSE
	/call sql_control FALSE ${chrID} lstBlockedSpells										string		outer FALSE
	/call sql_control FALSE ${chrID} setBufftoWatch											string		outer FALSE
	
	| burns
	/call sql_control FALSE ${chrID} swBurnControl											bool			outer FALSE
	/call sql_control FALSE ${chrID} swBurnForce												bool			outer FALSE
	/call sql_control FALSE ${chrID} swBurnAuto													bool			outer FALSE
	/call sql_control FALSE ${chrID} setBurnPct													int				outer 99
	/call sql_control FALSE ${chrID} setBurnCount												int				outer 5
	/call sql_control FALSE ${chrID} swBurnRaid													bool			outer TRUE

	| combat controls (assist)
	/call sql_control FALSE ${chrID} setMA1			 												string 		outer FALSE
	/call sql_control FALSE ${chrID} setMA2			 												string 		outer FALSE
	/call sql_control FALSE ${chrID} setMobAgro													int 			outer 60
	/call sql_control FALSE ${chrID} setEngageMAPct 										int 			outer 98
	/call sql_control FALSE ${chrID} setEngagePct 											int 			outer 98
	/call sql_control FALSE ${chrID} swAssistG													bool			outer FALSE
	/call sql_control FALSE ${chrID} swAssist1													bool			outer FALSE
	/call sql_control FALSE ${chrID} swAssist2													bool			outer FALSE
	/call sql_control FALSE ${chrID} swAssist3													bool			outer FALSE
	/call sql_control FALSE ${chrID} swAssistF													bool			outer FALSE
	/call sql_control FALSE ${chrID} swAgro															bool			outer FALSE 
	/call sql_control FALSE ${chrID} swPull															bool			outer FALSE
	/call sql_control FALSE ${chrID} swAdvPull													bool			outer FALSE
	/call sql_control FALSE ${chrID} swOffTank													bool			outer FALSE
	/call sql_control FALSE ${chrID} setCombatDistance									int				outer 15
	/call sql_control FALSE ${chrID} setCombatRadius										string		outer 30
	/call sql_control FALSE ${chrID} setzradius													int				outer 60	
	/call sql_control FALSE ${chrID} swBandolier												bool			outer FALSE
	/call sql_control FALSE ${chrID} setStopNukePct 										int 			outer 0
	/call sql_control FALSE ${chrID} swAlliance													bool			outer FALSE
	/call sql_control FALSE ${chrID} setAllianceTrigger									string		outer FALSE
	/call sql_control FALSE ${chrID} swForceFadeRest										bool			outer FALSE
	/call sql_control FALSE ${chrID} swCombatMelee											bool			outer FALSE
	/call sql_control FALSE ${chrID} swCombatRange											bool			outer FALSE
	/call sql_control FALSE ${chrID} swCombatNuke												bool			outer FALSE
	/call sql_control FALSE ${chrID} swCombatDoT												bool			outer FALSE
	/call sql_control FALSE ${chrID} swCombatHeal												bool			outer FALSE

	| rez	control
	/call setDeclare swValidRezFound 																		bool	 		outer FALSE
	/call setDeclare setRezWindowPct 																		int 			outer 0
	/call setDeclare swRezPct																						bool			outer FALSE
	/call setDeclare swRezRange																					bool			outer FALSE
	/call sql_control FALSE ${chrID} swRezTake 													bool 			outer TRUE 
	/call sql_control FALSE ${chrID} swRezToken													bool			outer FALSE
	/call sql_control FALSE ${chrID} lstRezToken												string		outer FALSE
	/call sql_control FALSE ${chrID} setMinRezPct												int				outer 90
	/call sql_control FALSE ${chrID} setMaxRezRange				 							int 			outer 60 
	/call sql_control FALSE ${chrID} swRezGive	 												bool 			outer FALSE 
	/call sql_control FALSE ${chrID} swRezCombat 												bool 			outer FALSE 
	/call sql_control FALSE ${chrID} swRezEveryone 											bool 			outer FALSE 

	| spire control
	/call setDeclare ${Me.Class.ShortName}spire0												int				outer 0
	/call sql_control FALSE ${chrID} spireRaid													int 			outer 0
	/call sql_control FALSE ${chrID} spireGroup													int 			outer 0

	| watchspawn stuff
	/call setDeclare hudwatchspawn																			string		outer FALSE
	/call setDeclare watchSpawnCount																		int				outer	FALSE
	/call sql_control FALSE ${chrID} watchSpawn													string		outer FALSE

 	| Movement control
	| /call setDeclare swHomeSet																					bool			outer FALSE
	/call setDeclare swsnapstuck																				bool			outer	FALSE
	/call sql_control FALSE ${chrID} swHomeSet													bool			outer FALSE
	/call sql_control FALSE ${chrID} setHomeRadius											int 			outer 5
	/call sql_control FALSE ${chrID} setHomeLeash												int 			outer 40
	/call sql_control FALSE ${chrID} setHomeDelayMin										int 			outer 100
	/call sql_control FALSE ${chrID} setHomeDelayMax										int 			outer 500	
	/call sql_control FALSE ${chrID} swStickBehind 											bool 			outer FALSE
	/call sql_control FALSE ${chrID} swFaceFast													bool			outer FALSE
	/call sql_control FALSE ${chrID} setFaceArc													int				outer 30
	/call sql_control FALSE ${chrID} setMount														string		outer FALSE

	| tie control
	/call setDeclare swSetTie       																		bool			outer FALSE
	/call sql_control FALSE ${chrID} setTieToon 												string 		outer FALSE
	/call sql_control FALSE ${chrID} setTieDistance 										int 			outer 5
	/call sql_control FALSE ${chrID} swTieLock	 												bool 			outer FALSE
	/call sql_control FALSE ${chrID} setTieMode	 												string		outer Stick
	
	| debuff switches	
	/call sql_control FALSE ${chrID} swdeBuffCripple										bool			outer FALSE
	/call sql_control FALSE ${chrID} swdeBuffMezz												bool			outer FALSE
	/call sql_control FALSE ${chrID} swdeBuffMalo												bool			outer FALSE
	/call sql_control FALSE ${chrID} swdeBuffTash												bool			outer FALSE
	/call sql_control FALSE ${chrID} swdeBuffSlow												bool			outer FALSE
	/call sql_control FALSE ${chrID} swdeBuffEradicate									bool			outer FALSE
	/call sql_control FALSE ${chrID} swdeBuffSnare											bool			outer FALSE

	| overrides
	/call sql_control FALSE ${chrID} swOverride													bool			outer FALSE
  /call sql_control FALSE ${chrID} swOverrideLOS											bool			outer FALSE
  /call sql_control FALSE ${chrID} swOverrideSafeZone									bool			outer FALSE
  /call sql_control FALSE ${chrID} swOverrideExclude									bool			outer FALSE
  /call sql_control FALSE ${chrID} swOverrideName											bool			outer FALSE
  /call sql_control FALSE ${chrID} swOverrideEngagePct								bool			outer FALSE
  /call sql_control FALSE ${chrID} swOverrideRange										bool			outer FALSE
  /call sql_control FALSE ${chrID} swOverrideMelee										bool			outer FALSE
  /call sql_control FALSE ${chrID} swOverrideLoot											bool			outer FALSE
  /call sql_control FALSE ${chrID} swOverrideSafeNames								bool			outer FALSE
  /call sql_control FALSE ${chrID} swOverrideTrainSpell								bool			outer FALSE

	| buffs
	/call sql_control FALSE ${chrID} lstBuffRemove											string		outer "|FALSE"
	/call setDeclare swBuff																							bool			outer FALSE
	/call sql_control FALSE ${chrID} swBuffItem	 												bool			outer FALSE
	/call sql_control FALSE ${chrID} swBuffCombat 											bool			outer FALSE
	/call sql_control FALSE ${chrID} swBuffPet 													bool 			outer FALSE
	/call sql_control FALSE ${chrID} swBuffSelf	 												bool			outer FALSE
	/call sql_control FALSE ${chrID} swBuffBC														bool			outer FALSE
	/call sql_control FALSE ${chrID} swBuffRaid													bool			outer FALSE
	/call sql_control FALSE ${chrID} swBuffTell													bool			outer FALSE
	/call sql_control FALSE ${chrID} swBuffMerc													bool			outer FALSE
	/call sql_control FALSE ${chrID} swBuffOther												bool			outer FALSE
	/call sql_control FALSE ${chrID} swBuffMGB													bool			outer FALSE

	| cursor                                                      			
	/call sql_control FALSE ${chrID} swAutoCursor												bool			outer FALSE	
                                                                			
	| heals                                                       			
	/call sql_control FALSE ${chrID} setHealOverride										int				outer 0
	/call setDeclare tmpHealOverride																		float			outer 99.99	
	/call sql_control FALSE ${chrID} swHealSelf													bool 			outer FALSE
	/call sql_control FALSE ${chrID} swHealGroup												bool 			outer FALSE
	/call sql_control FALSE ${chrID} swHealXTarget											bool 			outer FALSE
	/call sql_control FALSE ${chrID} swHealPet													bool 			outer FALSE
	/call sql_control FALSE ${chrID} swHealCure													bool 			outer FALSE
	/call sql_control FALSE ${chrID} cureCurseItem											string 		outer FALSE
	/call sql_control FALSE ${chrID} curePoisonItem 										string 		outer FALSE
	/call sql_control FALSE ${chrID} cureDiseaseItem										string 		outer FALSE
	/call sql_control FALSE ${chrID} cureCorruptItem										string 		outer FALSE
	/call sql_control FALSE ${chrID} noCureList	 												string 		outer "|FALSE"
	/call sql_control FALSE ${chrID} clickHealItem											string 		outer "|FALSE"

	
	/call sql_control FALSE ${chrID} echoChannel												string		outer FALSE
	/call sql_control FALSE ${chrID} relayTells													string		outer FALSE
	/call sql_control FALSE ${chrID} ClassChannel												int				outer FALSE

	| mode control
	/call setDeclare setMode																						bool			outer FALSE
	/call setDeclare swModeFALSE																				bool			outer FALSE
	/call setDeclare swModeStop																					bool			outer FALSE
	/call setDeclare swModeAgro																					bool			outer FALSE
	/call setDeclare setModeType																				string		outer FALSE
	/call setDeclare setModeCount																				int				outer 0
	/call setDeclare swModeDrag																					bool			outer FALSE 
	/call setDeclare swModeDrink																				bool			outer FALSE 
	/call setDeclare swModeForage																				bool			outer FALSE
	/call setDeclare swModePetfarm																			bool			outer FALSE
	/call setDeclare swModeScorch																				bool			outer FALSE
	/call setDeclare swModeHunt																					bool			outer FALSE
	/call setDeclare swModeFish																					bool			outer FALSE
	/call setDeclare swModeSkill																				bool			outer FALSE
	/call setDeclare swModeHarvest																			bool			outer FALSE
	/call setDeclare swModeHarvestCount																	bool			outer FALSE
	/call setDeclare swModeTradeskill																		bool			outer FALSE
	/call setDeclare swModeLush																					bool			outer FALSE
	/call setDeclare swModeTrainspell																		bool			outer FALSE
	/call setDeclare swModeTrainspellSPC																bool			outer FALSE
	/call setDeclare swModeOpen																					bool			outer FALSE	
	/call setDeclare swModeCollect																			bool			outer FALSE	
	/call setDeclare swModeSummon																				bool			outer FALSE	
	
	/call sql_control FALSE ${chrID} setModeAgro												int				outer FALSE 
	/call sql_control FALSE ${chrID} setModeHarvestCount								int				outer 999

	| general settings for mode item usage
	/call sql_control FALSE ${chrID} modeDrinkofChoice									string		outer FALSE
	/call sql_control FALSE ${chrID} modeOpenItem												string		outer FALSE

	| pet crap
	/call sql_control FALSE ${chrID} setPetEngagePct										int				outer 99
	/call sql_control FALSE ${chrID} setPetType													string		outer FALSE
	/call sql_control FALSE ${chrID} swPet															bool 			outer FALSE
	/call sql_control FALSE ${chrID} swSwarm														bool 			outer FALSE	
	/call sql_control FALSE ${chrID} setSwarmEngagePct									int				outer 99
	/call sql_control FALSE ${chrID} swNoSwarmonDS											bool	 		outer FALSE
		
	| familiars
	/for _count 1 to 3
		/call sql_control FALSE ${chrID} setFam${_count}									string		outer FALSE
		/call sql_control FALSE ${chrID} setFam${_count}Buff							string		outer FALSE
		/call sql_control FALSE ${chrID} swFam${_count}Leave							bool			outer FALSE	
	/next _count

	
	/if (${Me.Class.PetClass} || ${Select[${Me.Class.ShortName},ENC,DRU,SHM,SHD,NEC]}) {
		/call sql_control FALSE ${chrID} swPetTank 												bool 			outer FALSE
		/call sql_control FALSE ${chrID} swMAGWep 												bool 			outer TRUE
		/call sql_control FALSE ${chrID} setPetEpicClick 									string 		outer FALSE
		/call sql_control FALSE ${chrID} setPetEpicBuff 									string 		outer FALSE
		/call sql_control FALSE ${chrID} setMAGWepSpell 									string 		outer FALSE
		/call sql_control FALSE ${chrID} setMAGClosedBag 									string 		outer FALSE
		/call sql_control FALSE ${chrID} setMAGPetWepName									string 		outer FALSE
		/call sql_control FALSE ${chrID} setPetShrink 										string 		outer	FALSE
		/call sql_control FALSE ${chrID} setPetHealSpell 									string 		outer FALSE
		/call sql_control FALSE ${chrID} setPetRenewalSpell 							string 		outer FALSE
		/call sql_control FALSE ${chrID} setPetIllusion		 								string 		outer FALSE
		/call sql_control FALSE ${chrID} setPetIllusionBuff								string 		outer FALSE
		/call sql_control FALSE ${chrID} setPetAAHeal											string 		outer FALSE
		/call sql_control FALSE ${chrID} setPetBagSlot										int 			outer 30
		/call sql_control FALSE ${chrID} lstPetBlockedSpells							string		outer FALSE
			                                                            			
		| 10 pet buffs
		/for _count 1 to 10
			/call sql_control FALSE ${chrID} PetBuff${_count} 							string 		outer FALSE
			/call sql_control FALSE ${chrID} PetBuff${_count}chk						string 		outer FALSE
		/next _count
	}	

	| click Buffs                                               				
	/for _count 1 to 6
		/call sql_control FALSE ${chrID} clickItemBuff${_count}						string		outer FALSE
	/next _count	

 	| unity buffs
	/for _count 1 to 7
		/call sql_control FALSE ${chrID} BuffAAUnity${_count}							string		outer FALSE
	/next _count

	| clickie DPS things
	/for _count 1 to 6
		/call sql_control FALSE ${chrID} clickItem${_count}								string		outer FALSE
	/next _count
		           
	| click DPS Burn                                               				
	/for _count 1 to 6
		/call sql_control FALSE ${chrID} clickItemBurn${_count}						string		outer FALSE
	/next _count
	
	| bp clicky stacking
	/for _count 1 to 2
		/call sql_control FALSE ${chrID} Breastplate${_count}			 				string		outer FALSE
		/call sql_control FALSE ${chrID} Breastplate${_count}Condition		string		outer FALSE
		/call sql_control FALSE ${chrID} Breastplate${_count}effect				string		outer FALSE
	/next _count
	                                                              			
	/call sql_control FALSE ${chrID} useCircle													string		outer FALSE
	/call sql_control FALSE ${chrID} CircleofPower											string		outer FALSE
	/call sql_control FALSE ${chrID} CircleofLife												string		outer FALSE
	/call sql_control FALSE ${chrID} CircleofMana												string		outer FALSE
                                                                			
	/for _count 1 to 6
		/call sql_control FALSE ${chrID} clickItemSwarm${_count}					string		outer FALSE
	/next _count

	| clickie odd items
	/for _count 1 to 4
		/call sql_control FALSE ${chrID} clickOddItem${_count}						string		outer FALSE
	/next _count
		
	| AAs that you want to always keep going
	/call sql_control FALSE ${chrID} AANow1															string		outer FALSE
	/call sql_control FALSE ${chrID} AANow2															string		outer FALSE

	| discs that you want to always keep going
	/call sql_control FALSE ${chrID} DiscNow1														string		outer FALSE
	/call sql_control FALSE ${chrID} DiscNow2														string		outer FALSE
 
	| items that you want to always keep going
	/call sql_control FALSE ${chrID} ItemNow1														string		outer FALSE
	/call sql_control FALSE ${chrID} ItemNow2														string		outer FALSE

	| General class combat skills
	/call sql_control FALSE ${chrID} kick																bool			outer FALSE
	/call sql_control FALSE ${chrID} bash																bool			outer FALSE
	/call sql_control FALSE ${chrID} taunt															bool			outer FALSE
	/call sql_control FALSE ${chrID} disarm															bool			outer FALSE
	/call sql_control FALSE ${chrID} bandolierNormal										string  	outer FALSE
	/call setDeclare bandolierSwap																			string 		outer ${bandolierNormal}
	/call sql_control FALSE ${chrID} bandolierBash											string		outer FALSE
	/call sql_control FALSE ${chrID} bandolierBurn											string		outer FALSE
	/call sql_control FALSE ${chrID} bandolierRange											string		outer FALSE
	
	/if (${Select[${Me.Class.ShortName},BER]}) {
		/call sql_control FALSE ${chrID} frenzy														bool			outer FALSE
	}
	/if (${Select[${Me.Class.ShortName},ROG]}) {
		/declare flagSoS																									bool			outer FALSE
		/call sql_control FALSE ${chrID} backstab													bool			outer FALSE
	}
	/if (${Select[${Me.Class.ShortName},BRD,BER,MNK,ROG]}) {
		/call sql_control FALSE ${chrID} intimidation											bool			outer FALSE
	}
	/if (${Select[${Me.Class.ShortName},MNK,BST]}) {
		/call sql_control FALSE ${chrID} dragonpunch											bool			outer FALSE
		/call sql_control FALSE ${chrID} eaglestrike											bool			outer FALSE
		/call sql_control FALSE ${chrID} tigerclaw												bool			outer FALSE
		/call sql_control FALSE ${chrID} flyingkick												bool			outer FALSE
		/call sql_control FALSE ${chrID} roundkick												bool			outer FALSE
	}

	| food/drink
	/call sql_control FALSE ${chrID} Food																string 		outer FALSE
	/call sql_control FALSE ${chrID} FoodDispenser											string 		outer FALSE
	/call sql_control FALSE ${chrID} FoodCount													int			 	outer 0
	/call sql_control FALSE ${chrID} Drink															string 		outer FALSE
	/call sql_control FALSE ${chrID} DrinkDispenser											string 		outer FALSE
	/call sql_control FALSE ${chrID} DrinkCount													int			 	outer 0
	/call sql_control FALSE ${chrID} tFoodDrink													string		outer 10m
	/call timer_update timer_meal 1

	/if (${Select[${Me.Class.ShortName},MAG,ENC,CLR,NEC,WIZ,SHM,DRU]}) {
		/call sql_control FALSE ${chrID} TwincastSpell 										string 		outer FALSE
		/call sql_control FALSE ${chrID} GatherSpell 											string 		outer FALSE
		/call sql_control FALSE ${chrID} SilentCast	 											string		outer FALSE
	}
	
	/if (!${Select[${Me.Class.ShortName},MNK,BER,WAR]}) {
		/call sql_control FALSE ${chrID} switchNukeDelay									bool			outer FALSE
	}	
	
	/if (${Me.Class.CanCast} || ${Select[${Me.Class.ShortName},ROG]}) {
		/call sql_control FALSE ${chrID} buffGem 													string 		outer "gem8"
		/call fix_gem_id "${buffGem}"
		/call setDeclare buffNum																					int 			local ${Macro.Return}
		
		/call sql_control FALSE ${chrID} GoMNuke													string		outer FALSE
		/call sql_control FALSE ${chrID} GoMNukeCondition									string		outer FALSE
		/call sql_control FALSE ${chrID} GoMNuke2													string		outer FALSE
		/call sql_control FALSE ${chrID} GoMNuke2Condition								string		outer FALSE
	}
	
	| bc/tell buffs
	/call setDeclare Buff0 																							string 		outer FALSE
	/for _count 1 to ${setMaxBuffCount}                        			
		/call sql_control FALSE ${chrID} Buff${_count}										string 		outer FALSE
		/call sql_control FALSE ${chrID} Buff${_count}self								string 		outer FALSE
		/call sql_control FALSE ${chrID} Buff${_count}selfCondition				string 		outer FALSE
		/call sql_control FALSE ${chrID} Buff${_count}Alias								string 		outer "|FALSE" 
		/call sql_control FALSE ${chrID} BCBuff${_count}Class							string 		outer "${lstClassAll}"
		/call sql_control FALSE ${chrID} RaidBuff${_count}Class						string 		outer "${lstClassAll}"
		/call sql_control FALSE ${chrID} ManualBuff${_count}Class					string 		outer "${lstClassAll}"
		/call sql_control FALSE ${chrID} ManualBuff${_count}Check					string 		outer FALSE
		/call sql_control FALSE ${chrID} Buff${_count}noBCinRaid					bool	 		outer FALSE
	/next _count
	
	| MGB buff
	/if (${validate_class[FALSE, ${lstClassCast}]}) {	
		/call sql_control FALSE ${chrID} BuffMGBname											string 		outer FALSE	
		/call sql_control FALSE ${chrID} BuffMGBchk												string 		outer FALSE	
		/call sql_control FALSE ${chrID} BuffMGBalias											string 		outer "|FALSE"
		/call sql_control FALSE ${chrID} BuffMGBcount											int		 		outer 18
		/call sql_control FALSE ${chrID} BuffMGBmana											int		 		outer 40
	}                                                             			

	| auras
	/call sql_control FALSE ${chrID} Aura1Spell 												string 		outer FALSE
	/call sql_control FALSE ${chrID} Aura1Buff 													string 		outer FALSE
	/call sql_control FALSE ${chrID} Aura2Spell 												string 		outer FALSE
	/call sql_control FALSE ${chrID} Aura2Buff 													string 		outer FALSE
	
	/if (${Select[${Me.Class.ShortName},CLR,DRU,SHM,PAL]}) {
		/call setDeclare callID																						int 			outer 0
		/call setDeclare NULLPoint																				int 			outer 0
		/call sql_control FALSE ${chrID} PetPoint 												int				outer 80
		/call sql_control FALSE ${chrID} CLRPoint		 											int 			outer 80
		/call sql_control FALSE ${chrID} DRUPoint 												int 			outer 70
		/call sql_control FALSE ${chrID} SHMPoint 												int 			outer 60
		/call sql_control FALSE ${chrID} WIZPoint 												int 			outer 75
		/call sql_control FALSE ${chrID} MAGPoint 												int 			outer 75
		/call sql_control FALSE ${chrID} ENCPoint 												int 			outer 80
		/call sql_control FALSE ${chrID} NECPoint 												int 			outer 75
		/call sql_control FALSE ${chrID} WARPoint 												int 			outer 70
		/call sql_control FALSE ${chrID} MNKPoint 												int 			outer 60
		/call sql_control FALSE ${chrID} ROGPoint 												int 			outer 60
		/call sql_control FALSE ${chrID} RNGPoint 												int 			outer 90
		/call sql_control FALSE ${chrID} BSTPoint 												int 			outer 70
		/call sql_control FALSE ${chrID} SHDPoint 												int 			outer 90
		/call sql_control FALSE ${chrID} BRDPoint 												int 			outer 65
		/call sql_control FALSE ${chrID} BERPoint 												int 			outer 70
		/call sql_control FALSE ${chrID} PALPoint 												int 			outer 90
		/call sql_control FALSE ${chrID} GoMpoint 												int 			outer 80	
		/call sql_control FALSE ${chrID} PanicPoint 											int 			outer 40	
		/call sql_control FALSE ${chrID} XHealClass												string		outer "${lstClassAll}"
		/call timer_set FALSE tXHeal																			string		outer 3m 		timer_set_xtarget
	}

	/if (${Select[${Me.Class.ShortName},CLR,DRU,SHM,BST,PAL,RNG]}) {
 		/call sql_control FALSE ${chrID} cureCurse 												string 		outer FALSE
		/call sql_control FALSE ${chrID} curePoison 											string 		outer FALSE
		/call sql_control FALSE ${chrID} cureDisease 											string 		outer FALSE
		/call sql_control FALSE ${chrID} cureCorrupt 											string 		outer FALSE
	}                                                             			

	| onoff types
	/call sql_control FALSE ${chrID} switchShrink												bool			outer FALSE
	/call sql_control FALSE ${chrID} switchHorn													bool			outer FALSE
                                                                			
	                                                              			
	| Mode stuffs                                                 			
	| Train Spell spells                                          			
	/if (${Bool[${Me.Class.CanCast}]}) {                          			
		/call sql_control FALSE ${chrID} abjuration												string 		outer FALSE
		/call sql_control FALSE ${chrID} alteration												string 		outer FALSE
		/call sql_control FALSE ${chrID} conjuration											string 		outer FALSE
		/call sql_control FALSE ${chrID} divination												string 		outer FALSE
		/call sql_control FALSE ${chrID} evocation												string 		outer FALSE
	}

	| forced delay for mez target
	/if (${Select[${Me.Class.ShortName},ENC,BRD,NEC]}) {
 		/call sql_control FALSE ${chrID} setMezDelay											int 			outer 0
	}                                                             			


/return



|***
 *	DES: sets all timers used that are not specific to a class
 *	USE: 
 *	NOTE: 
 ***|
sub pregen_timers()
	/declare _count int local 0
	
	/call timer_set FALSE tRez_Retry 																		string 		outer 3m		timer_rez_retry
	/call timer_set FALSE tCheck_snap_retry															string		outer	15s		timer_check_snap_retry
	/call timer_set FALSE tCheck_Runes																	string		outer	10s		timer_check_runes
	/call timer_set FALSE tScorchedEarth_ignore													string		outer	30s		timer_scorched_earth_ignore
	
	/call timer_set FALSE tScorchedEarth_fail														string		outer	1m 		timer_auto_cursor_pause
	/call timer_set FALSE tTry_Door																			string		outer	4s 		timer_try_door


	/call timer_set FALSE tCheck_Runes																	string		outer	10s		timer_check_runes
	/call timer_set FALSE tScorchedEarth_fail														string		outer	1m 		timer_scorched_earth_fail
	/call timer_set FALSE tScorchedEarth_delay													string		outer	5s 		timer_ScorchedEarth_delay
	/call timer_set FALSE tCheck_Familiar																string		outer	30s		timer_Check_Familiar
	/call timer_set FALSE tHarvest_delay																string		outer	1s 		timer_Harvest_delay
	/call timer_set FALSE tCheck_EQBC																		string		outer	10m		timer_check_EQBC
	/call timer_set FALSE tOdds_and_Ends																string		outer	30m 	timer_check_odds_and_ends
	/call timer_set FALSE tCheck_EXP																		string		outer	5m 		timer_check_EXP
	/call timer_set FALSE tCheck_Tie																		string		outer	5 		timer_check_Tie
	/call timer_set FALSE tWatch_Spawn																	string		outer	1s 		timer_check_watch_spawn
	/call timer_set FALSE tCast_Cure																		string		outer	30s 	timer_check_cast_cure
	/call timer_set FALSE tPetfarm_delay																string		outer	1s 		timer_Petfarm_delay	
	/call timer_set FALSE tClear_Cursor																	string		outer 5s 		timer_clear_cursor   
	/call timer_set FALSE tPet_Buff																			string		outer 1m 		timer_check_buffs_pet
	/call timer_set FALSE tSelf_Buff																		string		outer 3m 		timer_check_buffs_self
	/call timer_set FALSE tItem_Buff																		string		outer 1m 		timer_check_buffs_item
	/call timer_set FALSE tBC_Buff																			string		outer 20s 	timer_check_buffs_BC
	/call timer_set FALSE tRaid_Buff																		string		outer 20s 	timer_check_buffs_raid
	/call timer_set FALSE tMerc_Buff																		string		outer 40s 	timer_check_buffs_merc
	/call timer_set FALSE tOther_buff																		string		outer 5m	 	timer_check_buffs_other
	/call timer_set FALSE tTell_Buff																		string		outer	10s 	timer_check_buffs_tell
	/call timer_set FALSE tMGB_Buff																			string		outer	5m 		timer_check_buffs_mgb                                   										
	/call timer_set FALSE tBlocked_Buff																	string		outer	30m		timer_check_buffs_blocked                                   										
	/call timer_set FALSE tCheck_Aura																		string		outer 3m 		timer_check_aura
	/call timer_set FALSE tCheck_Ammo																		string		outer	5s 		timer_Check_Ammo
	/call timer_set FALSE tHunt_delay																		string		outer	30s 	timer_wait_for_mob_to_kill
	/call timer_set FALSE tAuto_Control_Toon														string		outer	10s 	timer_auto_toon_control
	/call timer_set FALSE tZone_Pause																		string		outer	5s		timer_zone_pause
	/call timer_set FALSE tHUD_Death_Check															string		outer	10s		timer_HUD_death_check
	/call timer_set FALSE tNuke_Delay																		string		outer	1			timer_Nuke_Delay
	/call timer_set FALSE tWaste_Fireworks															string		outer 15m		timer_Waste_Fireworks
	/call timer_set FALSE tValidate_Zone																string		outer 10s		timer_validate_zone
	/call timer_set FALSE tMode_Open																		string		outer 1s		timer_mode_open
	/call timer_set FALSE tMode_Open																		string		outer 1s		timer_mode_open
	/call timer_set FALSE tOdds_and_Ends																string		outer 10m		timer_check_odds_and_ends
	/call timer_set FALSE tZoned_Delay																	string		outer 2s		timer_zoned_delay
	/call timer_set FALSE tCheck_status																	string		outer	2s 		timer_check_status


	| mod rod/pet weapon recovereing addicts
	/if (!${Select[${Me.Class.ShortName},MAG,WAR,ROG,MNK,BER]}) {
		/for _count 1 to ${lstResidentMage.Count[|]}
			/if (!${Bool[${lstResidentMage.Arg[${_count},|]}]}) /continue
			/call timer_set FALSE tAsk_for_rod															string		outer	1m 		timer_ask_for_rod_${lstResidentMage.Arg[${_count},|]}
			/call timer_set FALSE tAsk_for_weapons													string		outer	1m		timer_ask_for_weapons_${lstResidentMage.Arg[${_count},|]}
		/next _count		
	}
/return



|***
 *	DES: generate the zone file data
 *	USE: 
 *	NOTE: 
 ***|
sub pregen_zone()

	VSQL \awDB\ax::\awzone\ax
	
	/declare _countZone int local 0
	/declare _countArray int local 0
	/declare _countField int local 0  
	/declare _zoneFields string local |SafeZone|NameList|ExcludeList|NoMezzList|RangeList|NoCastList|NoMeleeList|HuntMobList|CharmMobList|NoFireList|NoPoisonList|NoMagicList|NoCorruptionList|NoColdList|NoDiseaseList|HarvestList
	/declare _tmpZone string local

	| get the fat assed list of zones
	/call generate_zone_short_names

	| check for table
	/sqlite ${dbCore} dummy SELECT name FROM sqlite_master WHERE type='table' AND name="zone"
	/if (!${sqlite.Rows[dummy]}) {
		/sqlite ${dbCore} dummy CREATE TABLE zone (ShortName UNIQUE, SafeZone, NameList, ExcludeList, NoMezzList, RangeList, NoCastList, NoMeleeList, HuntMobList, CharmMobList, NoFireList, NoPoisonList, NoMagicList, NoCorruptionList, NoColdList, NoDiseaseList, HarvestList)
		/sqlite ${dbCore} dummy INSERT INTO zone (ShortName) VALUES ("gfaydark")
	}

	| loop array
	/for _countArray 1 to ${lstZoneShortName.Size}
		| loop array instance
		/for _countZone 1 to ${lstZoneShortName[${_countArray}].Count[|]}
			/varset _tmpZone ${lstZoneShortName[${_countArray}].Arg[${_countZone},|]}
			/sqlite ${dbCore} dummy SELECT ShortName FROM zone WHERE ShortName="${_tmpZone}"
			/if (!${sqlite.Rows[dummy]}) {
				/sqlite ${dbCore} dummy INSERT INTO zone (ShortName) VALUES ("${_tmpZone}")	
			}
		/next _countZone
	/next _countArray

	| delete the fat assed zone list
	/deletevar lstZoneShortName
	
/return



|***
 *	DES: list of all EQ zone Short Names
 *	USE: 
 *	NOTE: lets try and not leave ths shit in memory.. mkay?
 ***|
sub generate_zone_short_names()
	/declare lstZoneShortName[53] string outer
	/varset lstZoneShortName[1] |qeynos|qeynos2|qrg|qeytoqrg|highkeep|freportn|freportw|freporte|runnyeye|qey2hh1|northkarana|southkarana|eastkarana|beholder|blackburrow
	/varset lstZoneShortName[2] |paw|rivervale|kithicor|commons|ecommons|erudnint|erudnext|nektulos|cshome|lavastorm|nektropos|halas|everfrost|soldunga|soldungb|misty|nro
	/varset lstZoneShortName[3] |sro|befallen|oasis|tox|hole|neriaka|neriakb|neriakc|neriakd|najena|qcat|innothule|feerrott|cazicthule|oggok|rathemtn|lakerathe|grobb|aviak
	/varset lstZoneShortName[4] |akanon|steamfont|lfaydark|crushbone|mistmoore|kaladima|felwithea|felwitheb|unrest|kedge|guktop|gukbottom|kaladimb|butcher|oot|cauldron
	/varset lstZoneShortName[5] |airplane|fearplane|permafrost|kerraridge|paineel|hateplane|arena|soltemple|erudsxing|stonebrunt|warrens|erudsxing2|bazaar|bazaar2|arena2
	/varset lstZoneShortName[6] |jaggedpine|nedaria|tutorial|load|load2|hateplaneb|shadowrest|tutoriala|tutorialb|clz|poknowledge|soldungc|guildlobby|barter|takishruins
	/varset lstZoneShortName[7] |freeporteast|freeportwest|freeportsewers|northro|southro|highpasshold|commonlands|oceanoftears|kithforest|befallenb|highpasskeep|innothuleb
	/varset lstZoneShortName[8] |toxxulia|mistythicket|steamfontmts|dragonscalea|crafthalls|weddingchapel|weddingchapeldark|dragoncrypt|arttest|fhalls|apprentice
	/varset lstZoneShortName[9] |fieldofbone|warslikswood|droga|cabwest|swampofnohope|firiona|lakeofillomen|dreadlands|burningwood|kaesora|sebilis|citymist|skyfire
	/varset lstZoneShortName[10] |frontiermtns|overthere|emeraldjungle|trakanon|timorous|kurn|karnor|chardok|dalnir|charasis|cabeast|nurga|veeshan|veksar|chardokb
	/varset lstZoneShortName[11] |iceclad|frozenshadow|velketor|kael|skyshrine|thurgadina|eastwastes|cobaltscar|greatdivide|wakening|westwastes|crystal|necropolis
	/varset lstZoneShortName[12] |templeveeshan|sirens|mischiefplane|growthplane|sleeper|thurgadinb
	/varset lstZoneShortName[13] |shadowhaven|nexus|echo|acrylia|sharvahl|paludal|fungusgrove|vexthal|sseru|katta|netherbian|ssratemple|griegsend|thedeep|shadeweaver
	/varset lstZoneShortName[14] |hollowshade|grimling|mseru|letalis|twilight|thegrey|tenebrous|maiden|dawnshroud|scarlet|umbral|akheva
	/varset lstZoneShortName[15] |codecay|pojustice|potranquility|ponightmare|podisease|poinnovation|potorment|povalor|bothunder|postorms|hohonora|solrotower|powar
	/varset lstZoneShortName[16] |potactics|poair|powater|pofire|poeartha|potimea|hohonorb|nightmareb|poearthb|potimeb
	/varset lstZoneShortName[17] |gunthak|dulak|torgiran|nadox|hatesfury
	/varset lstZoneShortName[18] |guka|ruja|taka|mira|mmca|gukb|rujb|takb|mirb|mmcb|gukc|rujc|takc|mirc|mmcc|gukd|rujd|takd|mird|mmcd|guke|ruje|take|mire|mmce|gukf
	/varset lstZoneShortName[19] |rujf|takf|mirf|mmcf|gukg|rujg|takg|mirg|mmcg|gukh|rujh|takh|mirh|mmch|ruji|taki|miri|mmci|rujj|takj|mirj|mmcj
	/varset lstZoneShortName[20] |abysmal|natimbi|qinimi|riwwi|barindu|ferubi|snpool|snlair|snplant|sncrematory|tipt|vxed|yxtta|uqua|kodtaz|ikkinz|qvic|inktuta|txevu|tacvi|qvicb
	/varset lstZoneShortName[21] |wallofslaughter|bloodfields|draniksscar|causeway|chambersa|chambersb|chambersc|chambersd|chamberse|chambersf|provinggrounds|anguish
	/varset lstZoneShortName[22] |dranikhollowsa|dranikhollowsb|dranikhollowsc|dranikhollowsd|dranikhollowse|dranikhollowsf|dranikhollowsg|dranikhollowsh|dranikhollowsi
	/varset lstZoneShortName[23] |dranikhollowsj|dranikcatacombsa|dranikcatacombsb|dranikcatacombsc|draniksewersa|draniksewersb|draniksewersc|riftseekers|harbingers|dranik
	/varset lstZoneShortName[24] |broodlands|stillmoona|stillmoonb|thundercrest|delvea|delveb|thenest|guildhall
	/varset lstZoneShortName[25] |illsalin|illsalina|illsalinb|illsalinc|dreadspire|drachnidhive|drachnidhivea|drachnidhiveb|drachnidhivec|westkorlach|westkorlacha
	/varset lstZoneShortName[26] |westkorlachb|westkorlachc|eastkorlach|eastkorlacha|shadowspine|corathus|corathusa|corathusb|nektulosa
	/varset lstZoneShortName[27] |arcstone|relic|skylance|devastation|devastationa|rage|ragea|takishruinsa|elddar|elddara|theater|theatera|freeportacademy|freeporttemple
	/varset lstZoneShortName[28] |freeportmilitia|freeportarena|freeportcityhall|freeporttheater|freeporthall
	/varset lstZoneShortName[29] |crescent|moors|stonehive|mesa|roost|steppes|icefall|valdeholm|frostcrypt|sunderock|vergalid|direwind|ashengate
	/varset lstZoneShortName[30] |kattacastrum|thalassius|atiiki|zhisza|silyssar|solteris|barren|buriedsea|jardelshook|monkeyrock|suncrest|deadbone|blacksail
	/varset lstZoneShortName[31] |maidensgrave|redfeather|shipmvp|shipmvu|shippvu|shipuvu|shipmvm
	/varset lstZoneShortName[32] |mechanotus|mansion|steamfactory|shipworkshop|gyrospireb|gyrospirez|dragonscale|lopingplains|hillsofshade|bloodmoon|crystallos
	/varset lstZoneShortName[33] |guardian|cryptofshade|dragonscaleb
	/varset lstZoneShortName[34] |oldfieldofbone|oldkaesoraa|oldkaesorab|oldkurn|oldkithicor|oldcommons|oldhighpass|thevoida|thevoidb|thevoidc|thevoidd|thevoide
	/varset lstZoneShortName[35] |thevoidf|thevoidg|oceangreenhills|oceangreenvillage|oldblackburrow|bertoxtemple|discord|discordtower|oldbloodfield|precipiceofwar|olddranik
	/varset lstZoneShortName[36] |toskirakk|korascian|rathechamber|oldfieldofboneb
	/varset lstZoneShortName[37] |brellsrest|fungalforest|underquarry|coolingchamber|shiningcity|arthicrex|foundation|lichencreep|pellucid|stonesnake|brellstemple
	/varset lstZoneShortName[38] |convorteum|brellsarena|crafthalls|weddingchapel|dragoncrypt
	/varset lstZoneShortName[39] |feerrott2|thulehouse1|thulehouse2|housegarden|thulelibrary|well|fallen|morellcastle|morelltower|alkabormare|miragulmare|thuledream
	/varset lstZoneShortName[40] |somnium|neighborhood|phylactery
	/varset lstZoneShortName[41] |argath|arelis|beastdomain|cityofbronze|eastsepulcher|sarithcity|rubak|resplendent|pillarsalra|windsong|guildhalllrg|sepulcher
	/varset lstZoneShortName[42] |westsepulcher|resplendent|shadowedmount|guildhalllrg|guildhallsml|plhogrinteriors1a1|plhogrinteriors1a2|plhogrinteriors3a1|plhogrinteriors3a2
	/varset lstZoneShortName[43] |plhogrinteriors3b1|plhogrinteriors3b2|plhdkeinteriors1a1|plhdkeinteriors1a2|plhdkeinteriors1a3|plhdkeinteriors3a1|plhdkeinteriors3a2
	/varset lstZoneShortName[44] |plhdkeinteriors3a3|guildhall3
	/varset lstZoneShortName[45] |kaelshard|eastwastesshard|crystalshard|shardslanding|xorbb|breedinggrounds|eviltree|grelleth|chapterhouse|phinteriortree|chelsithreborn|poshadow
	/varset lstZoneShortName[46] |pomischief|burnedwoods|heartoffear|heartoffearb|heartoffearc
	/varset lstZoneShortName[47] |thevoidh|ethernere|neriakd|deadhills|bixiewarfront|towerofrot|arginhiz
	/varset lstZoneShortName[48] |arxmentis|brotherisland|endlesscaverns|dredge|degmar|kattacastrumb|tempesttemple|thuliasaur
	/varset lstZoneShortName[49] |exalted|exaltedb|cosul|codecayb|pohealth
	/varset lstZoneShortName[50] |chardoktwo|frontiermtnsb|korshaext|korshaint|lceanium|scorchedwoods|drogab
	/varset lstZoneShortName[51] |charasisb|gorowyn|charasistwo|skyfiretwo|overtheretwo|veeshantwo
	/varset lstZoneShortName[52] |aalishai|stratos|trialsofsmoke|mearatas|esianti|empyr|chamberoftears
	/varset lstZoneShortName[53] |gnomemtn
/return



|***
 *	DES: Berzerker base variables
 *	USE: 
 *	NOTE: 
 ***|
sub pregen_BER()
	/declare _count int local 0
	/call setDeclare BERspire 																					string		outer "|Fundament: First Spire of Savagery|Fundament: Second Spire of Savagery|Fundament: Third Spire of Savagery"
	/call setDeclare BERsynergyAA																				string		outer "Fury's Synergy"
	/call setDeclare BEREpic1																						string		outer "Kerasian Axe of Ire"
	/call setDeclare BEREpic15																					string		outer "Raging Taelosian Alloy Axe"
	/call setDeclare BEREpic2																						string		outer "Vengeful Taelosian Blood Axe"
	/call sql_control FALSE ${chrID} BERAlliance												string		outer "Demolisher's Alliance"
	/call setDeclare BERAAUnity																					string		outer FALSE
	/call setDeclare BERAAFade																					string		outer "Self Preservation"
	                                                              			
	/call sql_control FALSE ${chrID} switchAAFade												bool			outer FALSE
	/call sql_control FALSE ${chrID} useAAFadePct												int				outer 50
                                                                			
	/call sql_control FALSE ${chrID} AEMelee					 									bool	 		outer FALSE
	/call sql_control FALSE ${chrID} DiscSnare 													string 		outer FALSE
	/call sql_control FALSE ${chrID} SnareAxe 													string 		outer FALSE

	| Volley Line
	/call sql_control FALSE ${chrID} DiscVolley 												string 		outer PREGENDATA
	
	/call sql_control FALSE ${chrID} CryOfChaos 												string 		outer "Ancient: Cry of Chaos"
	
	| Berserking Line
	/call sql_control FALSE ${chrID} DiscBerserking											string 		outer PREGENDATA
	| rest disc
	/call sql_control FALSE ${chrID} DiscRest		 												string		outer PREGENDATA

	| Combat Frenzy
	/call sql_control FALSE ${chrID} DiscCombatFrenzy 									string 		outer PREGENDATA
                                                                			
	| Disc Scream                                                 			
	/call sql_control FALSE ${chrID} DiscScream 												string 		outer PREGENDATA

	| Axe Throw
	/call sql_control FALSE ${chrID} ThrowingAxe 												string 		outer PREGENDATA

	| Jolt Line
	/call sql_control FALSE ${chrID} DiscJolt 													string 		outer PREGENDATA
	
	| Bloodlust
	/call sql_control FALSE ${chrID} SharedBloodLust 										string 		outer PREGENDATA
	/call sql_control FALSE ${chrID} SharedBloodLustRecourse						string 		outer PREGENDATA
	
	| Overwhelming Frenzy Line
	/call sql_control FALSE ${chrID} DiscOverwhelmingFrenzy							string 		outer PREGENDATA

	| Cleaving Anger Line
	/call sql_control FALSE ${chrID} DiscCleavingAnger									string 		outer PREGENDATA
                                                                			
	| Opening Attack Line                                         			
	/call sql_control FALSE ${chrID} DiscSlapintheFace									string 		outer PREGENDATA
                                                                			
	| Axe of Line                                                 			
	/call sql_control FALSE ${chrID} DiscAxeofRallos										string 		outer PREGENDATA

	| Flurry Line
	/call sql_control FALSE ${chrID} DiscVengeful												string 		outer PREGENDATA
                                                                			
	| Flurry Line
	/call sql_control FALSE ${chrID} DiscVengeful												string 		outer PREGENDATA

	| AE Spiral Line                                                   			
	/call sql_control FALSE ${chrID} DiscAESpiral												string 		outer PREGENDATA
                                                                			
	| Cry Havoc Line                                              			
	/call sql_control FALSE ${chrID} DiscCryHavoc												string 		outer PREGENDATA
                                                                			
	| Augmented Frenzy Line                                       			
	/call sql_control FALSE ${chrID} DiscAugmentedFrenzy								string 		outer PREGENDATA
	/call sql_control FALSE ${chrID} AugmentedFrenzyEffect 							string 		outer PREGENDATA

	| Festering line
	/call sql_control FALSE ${chrID} DiscFesteringRage 									string 		outer PREGENDATA

	| Frenzied Line
	/call sql_control FALSE ${chrID} DiscFrenzyResolve									string 		outer PREGENDATA
	                                                              			
	/call sql_control FALSE ${chrID} UntamedgRage												string 		outer "Untamed Rage"
	/call sql_control FALSE ${chrID} RecklessAbandon										string 		outer "Reckless Abandon"
 	/call sql_control FALSE ${chrID} frenzy															bool			outer FALSE
                                                                			
 	| Dichotomic Line                                             			
	/call sql_control FALSE ${chrID} DiscDicho													string 		outer PREGENDATA
                                                                			
	| Disconcerting Line                                          			
	/call sql_control FALSE ${chrID} DiscDisconcert											string 		outer PREGENDATA
                                                                			
	| Endurance Tap Line                                          			
	/call sql_control FALSE ${chrID} DiscEnduranceTap										string 		outer PREGENDATA
                                                                			
	| Pet Line                                                    			
	/call sql_control FALSE ${chrID} DiscPet														string 		outer PREGENDATA
                                                                			
	/call sql_control FALSE ${chrID} BERPoint 													int 			outer 60

	| Open Wound Line
	/call sql_control FALSE ${chrID} DiscOpenWound											string 		outer PREGENDATA
	/call sql_control FALSE ${chrID} OpenWoundEffect										string 		outer PREGENDATA
                                                                			
	/call sql_control FALSE ${chrID} buffGem 														string 		outer FALSE
	/call sql_control FALSE ${chrID} spellCripple 											string 		outer FALSE

/return		                                        		



|***
 *	DES: Bard base variables
 *	USE: 
 *	NOTE: 
 ***|
sub pregen_BRD()
	/declare _count int local 0
	/call timer_set FALSE tValidate_Lists																string		outer 15s timer_validate_lists
	/call setDeclare BRDspire																						string		outer "| Fundament: First Spire of the Minstrel|Fundament: Second Spire of the Minstrel|Fundament: Third Spire of the Minstrel"
	/call setDeclare BRDsynergyAA																				string		outer "Troubadour's Synergy"
	/call setDeclare BRDEpic1																						string		outer "Singing Short Sword"
	/call setDeclare BRDEpic15																					string		outer "Prismatic Dragon Blade"
	/call setDeclare BRDEpic2																						string		outer "Blade of Vesagran"
	/call sql_control FALSE ${chrID} BRDAlliance												string		outer PREGENDATA
	/call setDeclare BRDAAUnity																					string		outer FALSE
	/call setDeclare BRDAAFade																					string		outer "Fading Memories"

	/call sql_control FALSE ${chrID} switchAAFade												bool			outer FALSE
	/call sql_control FALSE ${chrID} useAAFadePct												int				outer 50

	| Servant Nuke AA1
	/call sql_control FALSE ${chrID} ServantNukeAA1 										string 		outer PREGENDATA
	
	| Servant Nuke AA2				
	/call sql_control FALSE ${chrID} ServantNukeAA2 										string 		outer PREGENDATA
	
	| Servant Nuke song
	/call sql_control FALSE ${chrID} songServantNuke										string 		outer PREGENDATA
	                                          		
	/call setDeclare lineType																						string		outer |caster|melee|mix|other|tank
	/call sql_control FALSE ${chrID} lineCurrent												string 		outer FALSE
	/call sql_control FALSE ${chrID} lineTank														string		outer FALSE
	/call sql_control FALSE ${chrID} lineCaster													string		outer FALSE
	/call sql_control FALSE ${chrID} lineMelee													string		outer FALSE
	/call sql_control FALSE ${chrID} lineMix														string		outer FALSE
	/call sql_control FALSE ${chrID} lineOther													string		outer FALSE

	| Single Mez song
	/call sql_control FALSE ${chrID} spellMez														string 		outer PREGENDATA

	| AE Mez song
	/call sql_control FALSE ${chrID} spellpbaeMez												string 		outer PREGENDATA

	| single target slow
	/call sql_control FALSE ${chrID} spellSlow													string		outer PREGENDATA
	
	/call sql_control FALSE ${chrID} switchRestSong											bool			outer FALSE
	/call sql_control FALSE ${chrID} spellRestSong											string		outer FALSE
	/call sql_control FALSE ${chrID} spellCripple 											string 		outer FALSE
	/call sql_control FALSE ${chrID} BRDPoint		 												int 			outer 80
	                                          		  										
	/call sql_control FALSE ${chrID} switchIntimidKick 									bool	 		outer FALSE
	/call sql_control FALSE ${chrID} swModRod														bool	 		outer TRUE

	| Nukes
	/for _count 1 to 8
		/call sql_control FALSE ${chrID} Nuke${_count}										string		outer FALSE
		/call sql_control FALSE ${chrID} Nuke${_count}Condition						string		outer FALSE
	/next _count	
	
	| DoTs
	/for _count 1 to 8
		/call sql_control FALSE ${chrID} DoT${_count}											string		outer FALSE
		/call sql_control FALSE ${chrID} DoT${_count}chk									string		outer FALSE
		/call sql_control FALSE ${chrID} DoT${_count}Condition						string		outer FALSE
	/next _count
	

/return



|***
 *	DES: Beastlord base variables
 *	USE: 
 *	NOTE: 
 ***|
sub pregen_BST()
	/declare _count int local 0

	/call setDeclare BSTspire					 																	string	outer "|Fundament: First Spire of the Savage Lord|Fundament: Second Spire of the Savage Lord|Fundament: Third Spire of the Savage Lord"
	/call setDeclare BSTsynergyAA																				string	outer "Primalist's Synergy"
	/call setDeclare BSTEpic1																						string	outer "Claw of the Savage Spirit"
	/call setDeclare BSTEpic15																					string	outer "Savage Lord's Totem"
	/call setDeclare BSTEpic2																						string	outer "Spiritcaller Totem of the Feral"
	/call sql_control FALSE ${chrID} BSTAlliance															string	outer "Venomous Alliance"
	/call setDeclare BSTAAUnity																					string	outer "Feralist's Unity"
	/call setDeclare BSTAAFade																					string	outer FALSE
	
	/call sql_control FALSE ${chrID} switchAAFade														bool		outer FALSE
	/call sql_control FALSE ${chrID} useAAFadePct														int			outer 50

	/call sql_control FALSE ${chrID} swModRod																bool	 	outer TRUE
	
	/call sql_control FALSE ${chrID} DiscForay 															string 	outer PREGENDATA
	/call sql_control FALSE ${chrID} DiscFocusClaws 													string 	outer PREGENDATA
	/call sql_control FALSE ${chrID} DiscSavage 															string 	outer PREGENDATA
	/call sql_control FALSE ${chrID} DiscRuaabri 														string 	outer PREGENDATA
	/call sql_control FALSE ${chrID} DiscVindication 												string 	outer PREGENDATA
	/call sql_control FALSE ${chrID} DiscBestial			 												string 	outer PREGENDATA

 	| pet swarm
 	/call sql_control FALSE ${chrID} ServantNuke		 													string 	outer PREGENDATA
  /call sql_control FALSE ${chrID} FeroSpell 															string 	outer PREGENDATA
  /call sql_control FALSE ${chrID} DiscEnduring														string 	outer PREGENDATA
  /call sql_control FALSE ${chrID} DiscRending															string 	outer PREGENDATA
	         
	| you/pet growl spell                                   		
	/call sql_control FALSE ${chrID} GrowlSpell 															string 	Pet PREGENDATA
	
	/call sql_control FALSE ${chrID} TasteofBlood														string 	Pet "Taste of Blood"

	/call sql_control FALSE ${chrID} ParagonTXT 															string 	outer "PARAGON DONE (+5790 HP/tick, +2450 Mana/tick, +264 End/tick)"
	                                           		
	| single heal                                      		
	/call sql_control FALSE ${chrID} FastHeal			 													string	outer PREGENDATA
	/call sql_control FALSE ${chrID} BSTPoint 																int 		outer 80
	
	/call sql_control FALSE ${chrID} switchSelfParagon		 										bool		outer TRUE
	/call sql_control FALSE ${chrID} selfParagonPct 													int			outer 80
	/call sql_control FALSE ${chrID} switchConsumeSpirit	 										bool		outer TRUE
	/call sql_control FALSE ${chrID} ConsumeSpiritPct												int			outer 80
	/call sql_control FALSE ${chrID} switchGroupParagon		 									bool		outer TRUE
	/call sql_control FALSE ${chrID} groupParagonPct 												int			outer 50
                                          		
	| single target slow
	/call sql_control FALSE ${chrID} spellSlow		 														string	outer PREGENDATA
	
	| single target cripple
	/call sql_control FALSE ${chrID} spellCripple 														string 	outer PREGENDATA
                                             		
	| rest disc
	/call sql_control FALSE ${chrID} DiscRest		 														string	outer PREGENDATA

	| Nukes
	/for _count 1 to 8
		/call sql_control FALSE ${chrID} Nuke${_count}													string		outer FALSE
		/call sql_control FALSE ${chrID} Nuke${_count}Condition								string		outer FALSE
	/next _count	
	
	| DoTs
	/for _count 1 to 8
		/call sql_control FALSE ${chrID} DoT${_count}													string		outer FALSE
		/call sql_control FALSE ${chrID} DoT${_count}chk												string		outer FALSE
		/call sql_control FALSE ${chrID} DoT${_count}Condition									string		outer FALSE
	/next _count

/return



|***
 *	DES: Cleric base variables
 *	USE: 
 *	NOTE: 
 ***|
sub pregen_CLR()
	/declare _count int local 0
	/call setDeclare CLRspire																						string		outer "|Fundament: First Spire of Divinity|Fundament: Second Spire of Divinity|Fundament: Third Spire of Divinity"
	/call setDeclare CLRsynergyAA																				string		outer "Templar's Synergy"
	/call setDeclare CLREpic1																						string		outer "Water Sprinkler of Nem Ankh"
	/call setDeclare CLREpic15																					string		outer "Harmony of the Soul"
	/call setDeclare CLREpic2																						string		outer "Aegis of Superior Divinity"
	/call sql_control FALSE ${chrID} CLRAlliance															string		outer PREGENDATA
	/call setDeclare CLRAAUnity																					string		outer "Saint's Unity"
	/call setDeclare CLRAAFade																					string		outer "Divine Peace"
	/call sql_control FALSE ${chrID} RampageTank 														string		outer FALSE
	
	/call sql_control FALSE ${chrID} switchAAFade														bool			outer FALSE
	/call sql_control FALSE ${chrID} useAAFadePct														int				outer 50

	/call timer_set FALSE tHealWard																			string		outer 185s timer_heal_ward
	/call sql_control FALSE ${chrID} switchLockMA														bool 			outer FALSE
	/call sql_control FALSE ${chrID} swModRod																bool	 		outer TRUE
	/call sql_control FALSE ${chrID} switchVP																bool			outer TRUE
	/call sql_control FALSE ${chrID} useVPPct																int				outer 40
	/call sql_control FALSE ${chrID} spellCripple 														string 		outer FALSE

	/call sql_control FALSE ${chrID} BreastplateHeal		 											string		outer FALSE
	/call sql_control FALSE ${chrID} BreastplateHealCondition								string		outer FALSE		

	/call setDeclare hurtIndex																					int 			outer 0
	/call setDeclare hurtPctHP																					int 			outer 100
	/call setDeclare hurtCLS		 																				string 		outer FALSE
	/call setDeclare hurtCount 																					int 			outer 0
	
	/call sql_control FALSE ${chrID} DivineRezCleric													bool 			outer FALSE
	/call sql_control FALSE ${chrID} ProtectOutsideGroup											string		outer "|FALSE"
	/call sql_control FALSE ${chrID} ProtectInsideRaid												string		outer "|FALSE"
	/call sql_control FALSE ${chrID} ProtectNPCList													string		outer "|FALSE"
	/call sql_control FALSE ${chrID} useGroupHeal 														int				outer 2
	/call sql_control FALSE ${chrID} useGroupHoT															bool			outer FALSE
	/call sql_control FALSE ${chrID} setDivinetoGroupHeal										bool			outer FALSE

  /call sql_control FALSE ${chrID} useCelestialRegen												bool		  outer TRUE
	/call sql_control FALSE ${chrID} CRTXT																		string		outer "Celestial Regeneration"
	
	/call sql_control FALSE ${chrID} switchForceGroupHeal										bool			outer FALSE
	/call sql_control FALSE ${chrID} RampageTank															string		outer FALSE
	/call sql_control FALSE ${chrID} switchHealAggressive										bool			outer FALSE
	/call sql_control FALSE ${chrID} switchCompleteHeal											bool			outer FALSE
	/call sql_control FALSE ${chrID} CompleteHealpct													int 			outer 70	
	/call sql_control FALSE ${chrID} swArbitrate															bool			outer FALSE

	| group heals (not cures) *mostly                                            		
	/call sql_control FALSE ${chrID} healWholeGroup 													string 		outer PREGENDATA
  /call sql_control FALSE ${chrID} healWholeGroup2													string 		outer FALSE
  /call sql_control FALSE ${chrID} healWholeGroup3													string	 	outer FALSE

	| group HoT
  /call sql_control FALSE ${chrID} healWholeGroupHoT 											string 	 	outer PREGENDATA

	| remedies
  /call sql_control FALSE ${chrID} healRemedy1															string 		outer PREGENDATA
  /call sql_control FALSE ${chrID} healRemedy2															string 		outer PREGENDATA
	/call sql_control FALSE ${chrID} healRemedy3															string 		outer PREGENDATA
                                                                			
  /call sql_control FALSE ${chrID} healHoT 																string 		outer FALSE
                                                                			
	| target AE splash heals                                      			
  /call sql_control FALSE ${chrID} swSplash 																bool	 	 	outer FALSE
  /call sql_control FALSE ${chrID} healSplash 															string 	 	outer PREGENDATA
	
	| interventions
  /call sql_control FALSE ${chrID} healIntervention1 											string		outer PREGENDATA
  /call sql_control FALSE ${chrID} healIntervention2 											string		outer PREGENDATA
  /call sql_control FALSE ${chrID} healIntervention3 											string 		outer PREGENDATA
  /call sql_control FALSE ${chrID} healIntervention4 											string		outer PREGENDATA

	| contraventions
  /call sql_control FALSE ${chrID} healContravention1 											string		outer PREGENDATA
  /call sql_control FALSE ${chrID} healContravention2 											string		outer PREGENDATA
  /call sql_control FALSE ${chrID} healContravention3 											string 		outer PREGENDATA
  /call sql_control FALSE ${chrID} healContravention4 											string		outer PREGENDATA

	| renewals
  /call sql_control FALSE ${chrID} healRenewal1 														string		outer PREGENDATA
  /call sql_control FALSE ${chrID} healRenewal2 														string		outer PREGENDATA
  /call sql_control FALSE ${chrID} healRenewal3 														string 		outer PREGENDATA

	| panic heals
  /call sql_control FALSE ${chrID} healPanic 															string 		outer PREGENDATA
                                                              				
  | wards                                                     				
  /call sql_control FALSE ${chrID} healWard			 													string		outer PREGENDATA
  /call sql_control FALSE ${chrID} healWardToon	 													string		outer "|FALSE"

	| healing pet shield
	/call sql_control FALSE ${chrID} switchIssuance													bool			outer FALSE
	/call sql_control FALSE ${chrID} issuanceMobCount												int				outer 4
  /call sql_control FALSE ${chrID} healIssuance 														string		outer PREGENDATA
	 
	| yodeling anyone?                                       		
	/call sql_control FALSE ${chrID} doYaulp 																bool			outer TRUE
  /call sql_control FALSE ${chrID} spellYaulp 															string		outer PREGENDATA
	
	| hammer of ass kickery	
  /call sql_control FALSE ${chrID} sumWeaponSpell 													string		outer PREGENDATA
  /call sql_control FALSE ${chrID} sumWeaponItem 													string		outer PREGENDATA
	
	| cute hammer pet
  /call sql_control FALSE ${chrID} tempPet 																string		outer PREGENDATA
  
  | retort spell
  /call sql_control FALSE ${chrID} swRetort																bool			outer FALSE
  /call sql_control FALSE ${chrID} spellRetort															string		outer PREGENDATA
  /call sql_control FALSE ${chrID} spellRetortToon													string		outer "|FALSE"
  
	| AE Target splash cure
  | /call sql_control FALSE ${chrID} splashCure															string 		outer PREGENDATA
	
	| Nukes
	/for _count 1 to 8
		/call sql_control FALSE ${chrID} Nuke${_count}													string		outer FALSE
		/call sql_control FALSE ${chrID} Nuke${_count}Condition								string		outer FALSE
	/next _count	
	
	| DoTs
	/for _count 1 to 8
		/call sql_control FALSE ${chrID} DoT${_count}													string		outer FALSE
		/call sql_control FALSE ${chrID} DoT${_count}chk												string		outer FALSE
		/call sql_control FALSE ${chrID} DoT${_count}Condition									string		outer FALSE
	/next _count
	
	/if (${swTLP}) {
  	/call sql_control FALSE ${chrID} spellRez	 														string		outer FALSE
	}
	
	
	
/return		                                        		



|***
 *	DES: Druid base variables
 *	USE: 
 *	NOTE: 
 ***|
sub pregen_DRU()
	/declare _count int local 0

	/call setDeclare DRUspire																						string		outer "|Fundament: First Spire of Nature|Fundament: Second Spire of Nature|Fundament: Third Spire of Nature"
	/call setDeclare DRUsynergyAA																				string		outer "Preserver's Synergy"
	/call setDeclare DRUEpic1																						string		outer "Nature Walker's Scimitar"
	/call setDeclare DRUEpic15																					string		outer "Staff of Living Brambles"
	/call setDeclare DRUEpic2																						string		outer "Staff of Everliving Brambles"
	/call sql_control FALSE ${chrID} DRUAlliance															string		outer "Bosquetender's Alliance"
	/call setDeclare DRUAAUnity																					string		outer "Wildtender's Unity"
	/call setDeclare DRUAAFade																					string		outer "Veil of the Underbrush"
	/call sql_control FALSE ${chrID} RampageTank 														string		outer FALSE
                                                                			
	/call sql_control FALSE ${chrID} switchAAFade														bool			outer FALSE
	/call sql_control FALSE ${chrID} useAAFadePct														int				outer 50
                                                                			
	/call sql_control FALSE ${chrID} swModRod																bool	 		outer FALSE
	                                                              			

	/call sql_control FALSE ${chrID} debuffRo			 													string		outer "Blessing of Ro"	
	/call sql_control FALSE ${chrID} WoodTXT																	string		outer "SoWood cast."
                                        		                    			
	/call sql_control FALSE ${chrID} spellCripple 														string 		outer FALSE
	/call sql_control FALSE ${chrID} AAWolfSpirit														string 		outer FALSE
                                                                				
	/call setDeclare hurtID		 																					string 		outer FALSE
	/call setDeclare hurtIndex																					int 			outer 0
	/call setDeclare hurtPctHP																					int 			outer 100
	/call setDeclare hurtCLS		 																				string 		outer FALSE
	/call setDeclare hurtCount 																					int 			outer 0
	/call sql_control FALSE ${chrID} switchHealAggressive										bool			outer FALSE

	/call sql_control FALSE ${chrID} RandRezDelay														int 			outer 10
	/call sql_control FALSE ${chrID} ProtectOutsideGroup											string		outer "|FALSE"
	/call sql_control FALSE ${chrID} ProtectInsideRaid												string		outer "|FALSE"
	/call sql_control FALSE ${chrID} ProtectNPCList													string		outer "|FALSE"
	/call sql_control FALSE ${chrID} useGroupHeal 														int			outer 2
	/call sql_control FALSE ${chrID} useGroupHoT															bool			outer TRUE
                                           	
	/call sql_control FALSE ${chrID} healPanic			 													string  	outer "Survival of the Serendipitous"
	
	
	/call sql_control FALSE ${chrID} healRejuvilation												string  	outer "Resurgence"
	
	
	/call sql_control FALSE ${chrID} healAdrenaline				 									string 		outer "Daggerthorn Rush"
	
	
	/call sql_control FALSE ${chrID} healSanavida	 													string  	outer "Panavida"
	
	
	/call sql_control FALSE ${chrID} healWholeGroup													string 		outer "Lunasoothe"


	| temp HP grow line
	/call sql_control FALSE ${chrID} switchGrowth														bool  		outer FALSE
  /call sql_control FALSE ${chrID} spellGrowth 														string  	outer PREGENDATA
	/call sql_control FALSE ${chrID} spellGrowthToon													string  	outer "|FALSE"


	| Nukes
	/for _count 1 to 8
		/call sql_control FALSE ${chrID} Nuke${_count}													string		outer FALSE
		/call sql_control FALSE ${chrID} Nuke${_count}Condition								string		outer FALSE
	/next _count	
	
	| DoTs
	/for _count 1 to 8
		/call sql_control FALSE ${chrID} DoT${_count}													string		outer FALSE
		/call sql_control FALSE ${chrID} DoT${_count}chk												string		outer FALSE
		/call sql_control FALSE ${chrID} DoT${_count}Condition									string		outer FALSE
	/next _count

	/if (${swTLP}) {
  	/call sql_control FALSE ${chrID} spellRez	 														string		outer FALSE
	}

	| ports	
	/call setDeclare looping																						string		outer "Zephyr: Looping Planes"
	/call setDeclare steamfront																					string		outer "Zephyr: Steamfront"
	/call setDeclare butcherblock																				string		outer "Zephyr: Butcherblock"
	/call setDeclare shardslanding																			string		outer "Zephyr: Shard's Landing"
	/call setDeclare pillars																						string		outer "Zephyr: Pillars of Alra"
	/call setDeclare beast																							string		outer "Zephyr: Beasts' Domain"
	/call setDeclare westkarna																					string		outer "Zephyr: West Karana"
	/call setDeclare lavastorm																					string		outer "Zephyr: Lavastorm"
	/call setDeclare undershore																					string		outer "Zephyr: Undershore"
	/call setDeclare misty																							string		outer "Zephyr: Misty"
	/call setDeclare ro																									string		outer "Zephyr: Ro"
	/call setDeclare feerrott																						string		outer "Zephyr: Feerrott"
	/call setDeclare commonlands																				string		outer "Zephyr: Commonlands"
	/call setDeclare surefall																						string		outer "Zephyr: Surefall Glade"
	/call setDeclare karana																							string		outer "Zephyr: Karana"
	/call setDeclare wallofslaughter 																		string		outer "Zephyr: Slaughter"
	/call setDeclare bloodfields 																				string		outer "Zephyr: Bloodfields"
	/call setDeclare lceanium																						string		outer "Zephyr: Lceanium"
	/call setDeclare dreadlands																					string		outer "Zephyr: Combines"
	/call setDeclare dawnshroud																					string		outer "Zephyr: Dawnshroud"
	/call setDeclare twilight																						string		outer "Zephyr: Twilight"
	/call setDeclare grimling																						string		outer "Zephyr: Grimling"
	/call setDeclare nexus																							string		outer "Zephyr: Nexus"
	/call setDeclare primarypush																				string		outer "Primary Anchor Push"
	/call setDeclare secondarypush																			string		outer "Secondary Anchor Push"
	/call setDeclare primary																						string		outer "Zephyr: Primary Anchor"
	/call setDeclare secondary																					string		outer "Zephyr: Secondary Anchor"
	/call setDeclare tempesttemple																			string		outer "Zephyr: Tempest Temple"
	/call setDeclare buriedsea																					string		outer "Zephyr: Buried Sea"
	/call setDeclare stonebrunt																					string		outer "Zephyr: Stonebrunt"
	/call setDeclare tox																								string		outer "Zephyr: Toxxulia"
	/call setDeclare direwind																						string		outer "Zephyr: Direwind"
	/call setDeclare steppes																						string		outer "Zephyr: Steppes"
	/call setDeclare blightfire																					string		outer "Zephyr: Blightfire Moors"
	/call setDeclare barindu																						string		outer "Zephyr: Barindu"
	/call setDeclare natimbi																						string		outer "Zephyr: Natimbi"
	/call setDeclare grounds																						string		outer "Zephyr: the Grounds"
	/call setDeclare potime																							string		outer "Zephyr: Plane of Time"
	/call setDeclare brells																							string		outer "Zephyr: Brell's Rest"
	/call setDeclare arcstone																						string		outer "Zephyr: Arcstone"
	/call setDeclare knowledge																					string		outer "Zephyr: Knowledge"
	/call setDeclare cobaltscar																					string		outer "Zephyr: Cobalt Scar"
	/call setDeclare wakening																						string		outer "Zephyr: Wakening Land"
	/call setDeclare greatdivide																				string		outer "Zephyr: Great Divide"
	/call setDeclare iceclad																						string		outer "Zephyr: Iceclad"	
	/call setDeclare esianti 																						string		outer "Zephyr: Esianti"
		
	
/return		                                        		



|***
 *	DES: Enchanter base variables
 *	USE: 
 *	NOTE: 
 ***|
sub pregen_ENC()
	/declare _count int local 0

	/call timer_set FALSE tValidate_Lists																string		outer 15s timer_validate_lists

	/call setDeclare ENCspire																						string		outer "|Fundament: First Spire of Enchantment|Fundament: Second Spire of Enchantment|Fundament: Third Spire of Enchantment"
	/call setDeclare ENCIoG																							string		outer "Illusions of Grandeur"
	/call setDeclare ENCMC																							string		outer "Mental Contortion"
	/call setDeclare ENCsynergyAA																				string		outer "Begulier's Synergy"
	/call setDeclare ENCEpic1																						string		outer "Staff of the Serpent"
	/call setDeclare ENCEpic15																					string		outer "Oculus of Persuasion"
	/call setDeclare ENCEpic2																						string		outer "Staff of Eternal Eloquence"
	/call sql_control FALSE ${chrID} ENCAlliance															string		outer "Chromatic Alliance"
	/call setDeclare ENCAAUnity																					string		outer "Orator's Unity"
	/call setDeclare ENCAAFade																					string		outer "Self Stasis"

	/call sql_control FALSE ${chrID} switchAAFade														bool			outer FALSE
	/call sql_control FALSE ${chrID} useAAFadePct														int				outer 50
	/call sql_control FALSE ${chrID} switchAABeamofSlumber										bool			outer FALSE
	
	| /if (!${Defined[aggroAnim]}) /declare aggroAnim string outer "|5|8|12|17|18|42|44|80|106|129|144"
	
	/call sql_control FALSE ${chrID} switchCharmPet													bool			outer FALSE
	
	
  | nights endless terror
  /call sql_control FALSE ${chrID} swTerror																bool			outer FALSE
  /call sql_control FALSE ${chrID} spellTerror															string		outer PREGENDATA
  /call sql_control FALSE ${chrID} lstTerrorToon														string		outer "|FALSE"

	/call sql_control FALSE ${chrID} switchGather			 											bool			outer FALSE
	/call sql_control FALSE ${chrID} GatherPercent			 											int				outer 60

	/call sql_control FALSE ${chrID} runeVeil			 													string		outer "Veil of Mindshadow"
	
	/call sql_control FALSE ${chrID} runeEldritch	 													string		outer "Eldritch Rune"

	/call sql_control FALSE ${chrID} runeSpray			 													string		outer "Glyph Spray"
	/call sql_control FALSE ${chrID} runeReactive	 													string		outer "Reactive Rune"
	/call sql_control FALSE ${chrID} runePoly			 													string		outer "Polyrefractive Rune"
	/call sql_control FALSE ${chrID} runeUnity			 													string		outer FALSE
	
	/call sql_control FALSE ${chrID} runeMind																string		outer FALSE
	
	/call sql_control FALSE ${chrID} runeEntAuspice													string		outer FALSE
		

	| spell damage rune
	/call sql_control FALSE ${chrID} runeLegion															string		outer PREGENDATA

	| self shield rune
	/call sql_control FALSE ${chrID} runeShieldofFate												string		outer PREGENDATA

	| ward rune combat innates
	/call sql_control FALSE ${chrID} runeWard			 													string		outer PREGENDATA

	| targetable brimstone rune
	/call sql_control FALSE ${chrID} runeBrimstone 													string		outer PREGENDATA

	| targetable auspice rune
	/call sql_control FALSE ${chrID} runeAuspice		 													string		outer PREGENDATA
	/call sql_control FALSE ${chrID} runeAuspiceToon													string  	outer "|FALSE"

	| aa crystal items
	/call sql_control FALSE ${chrID} mannaCrystal														string 		outer "Azure Mind Crystal"
	/call sql_control FALSE ${chrID} healCrystal															string 		outer "Sanguine Mind Crystal"
	/call sql_control FALSE ${chrID} AASlow			 														string		outer "Slowing Helix"

	| single slow
	/call sql_control FALSE ${chrID} spellSlow		 														string		outer PREGENDATA
		
	| cripple
	/call sql_control FALSE ${chrID} spellCripple 														string 		outer PREGENDATA

	| single target tash
	/call sql_control FALSE ${chrID} spellTash 															string 		outer PREGENDATA

	| single target mez
	/call sql_control FALSE ${chrID} spellMez																string 		outer PREGENDATA

	| PBAE Mez
	/call sql_control FALSE ${chrID} spellpbaeMez														string		outer PREGENDATA

	| PBAE stun
	/call sql_control FALSE ${chrID} spellStun																string		outer PREGENDATA
	
	| single target charm
	/call sql_control FALSE ${chrID} spellCharm															string		outer PREGENDATA

	/call sql_control FALSE ${chrID} ENCPoint 																int 			outer 80
	/call sql_control FALSE ${chrID} swModRod																bool	 		outer FALSE

	| Nukes
	/for _count 1 to 8
		/call sql_control FALSE ${chrID} Nuke${_count}													string		outer FALSE
		/call sql_control FALSE ${chrID} Nuke${_count}Condition								string		outer FALSE
	/next _count	
	
	| DoTs
	/for _count 1 to 8
		/call sql_control FALSE ${chrID} DoT${_count}													string		outer FALSE
		/call sql_control FALSE ${chrID} DoT${_count}chk												string		outer FALSE
		/call sql_control FALSE ${chrID} DoT${_count}Condition									string		outer FALSE
	/next _count

/return		                                        		


|***
 *	DES: Magician base variables
 *	USE: 
 *	NOTE: 
 ***|
sub pregen_MAG()
	/declare _count int local 0
	/declare _list int local 0
	/declare _line string local NULL
	
	/call setDeclare MAGspire																						string		outer "|Fundament: First Spire of the Elements|Fundament: First Spire of the Elements|Fundament: Third Spire of the Elements"
	/call setDeclare MAGsynergyAA																				string		outer "Conjurer's Synergy"
	/call setDeclare MAGEpic1																						string		outer "Orb of Mastery"
	/call setDeclare MAGEpic15																					string		outer "Staff of Elemental Essence"
	/call setDeclare MAGEpic2																						string		outer "Focus of Primal Elements"
	/call sql_control FALSE ${chrID} MAGAlliance															string		outer "Firebound Alliance"
	/call setDeclare MAGAAUnity																					string		outer "Thaumaturge's Unity"
	/call setDeclare MAGAAFade																					string		outer "Drape of Shadows"

	/call sql_control FALSE ${chrID} MAGAAGroupCOH														string		outer "Call of the Heroes"

	/call sql_control FALSE ${chrID} switchAAFade														bool			outer FALSE
	/call sql_control FALSE ${chrID} useAAFadePct														int				outer 50
	
	/call sql_control FALSE ${chrID} ServantNuke 														string 		outer PREGENDATA
	/call timer_set FALSE tCauldron_use																	string 		outer 360m timer_caldron_use
	/call timer_set FALSE tForce_Twincast																string		outer 5s timer_force_twincast
	/varset timer_force_twincast 1

	/call sql_control FALSE ${chrID} ParadoxSpell 														string 		outer "Grant Icebound Paradox"
	/call sql_control FALSE ${chrID} ParadoxRodName 													string 		outer "Summoned: Icebound Fragment"
	/call sql_control FALSE ${chrID} OrbSpell 																string 		outer "Summon Blazing Orb"
	/call sql_control FALSE ${chrID} OrbName 																string 		outer "Blazing Orb"
	/call sql_control FALSE ${chrID} SelfRodSpell 														string 		outer "Wand of Dark Modulation"
	/call sql_control FALSE ${chrID} SelfRodName 														string 		outer "Wand of Pelagic Modulation"
	/call sql_control FALSE ${chrID} ModRodSpell 														string 		outer "Large Modulation Shard"
	/call sql_control FALSE ${chrID} ModRodName 															string 		outer "Summoned: Large Modulation Shard"
	/call sql_control FALSE ${chrID} ElementRodSpell 												string 		outer "Summon Mutinous Minion"
	/call sql_control FALSE ${chrID} ElementRodName 													string 		outer "Summoned: Exigent Minion XXI"

	/call sql_control FALSE ${chrID} spellCripple 														string 		outer FALSE
	/call sql_control FALSE ${chrID} GatherSpell															string 		outer "Gather Magnitude"
	/call sql_control FALSE ${chrID} switchGather			 											bool			outer TRUE
	/call sql_control FALSE ${chrID} GatherPercent			 											int				outer 60
	/call sql_control FALSE ${chrID} NukeSalvo																string 		outer "Storm of Many"
	/call sql_control FALSE ${chrID} NukeSalvoCondition 											string		outer FALSE
	/call sql_control FALSE ${chrID} DoDoT								 										int				outer 0
	/call sql_control FALSE ${chrID} Cauldron				 												string		outer "Cauldron of Countless Goods"
	/call sql_control FALSE ${chrID} useRain																	bool 			outer FALSE
	/call sql_control FALSE ${chrID} doRainNuke															int 			outer 2
	/call sql_control FALSE ${chrID} RainNuke1 															string 		outer "Rain of Cutlasses"
	/call sql_control FALSE ${chrID} RainNuke2 															string 		outer "Coronal Rain"
	/call sql_control FALSE ${chrID} spellMalo 															string 		outer "Malosinete"
	/call sql_control FALSE ${chrID} swModRod																bool	 		outer FALSE

	/call sql_control FALSE ${chrID} MAGPoint 																int 			outer 80
		
	/call setDeclare lineType																						string		outer "|off|fire|magic|mix"
	/call sql_control FALSE ${chrID} lineCurrent															string 		outer FALSE
	/call sql_control FALSE ${chrID} lineFire																string		outer FALSE
	/call sql_control FALSE ${chrID} lineMagic																string		outer FALSE
	/call sql_control FALSE ${chrID} lineMix																	string		outer FALSE

	| add conditions
	/for _list 1 to ${lineType.Count[|]}
		/varset _line ${lineType.Arg[${_list},|].Left[1].Upper}${lineType.Arg[${_list},|].Right[-1].Lower}
		/for _count 1 to 4
			/call sql_control FALSE ${chrID} line${_line}Condition${_count}			string		outer FALSE
		/next _count
	/next _list		
	
	/call sql_control FALSE ${chrID} switchForceTwincast											bool			outer FALSE
	/call sql_control FALSE ${chrID} NukeTwincastFire												string		outer FALSE
	/call sql_control FALSE ${chrID} NukeTwincastMagic												string		outer FALSE
	/call sql_control FALSE ${chrID} NukeTwincastMix													string		outer FALSE
	/call sql_control FALSE ${chrID} NukeTwincastCondition 									string		outer FALSE	
	
	
	


/return		                                        		


|***
 *	DES: Monk base variables
 *	USE: 
 *	NOTE: 
 ***|
sub pregen_MNK()
	/declare _count int local 0

	/call setDeclare MNKspire																						string		outer "|Fundament: First Spire of the Sensei|Fundament: Second Spire of the Sensei|Fundament: Third Spire of the Sensei"
	/call setDeclare MNKsynergyAA																				string		outer "Calanin's Synergy"
	/call setDeclare MNKEpic1																						string		outer "Celestial Fists"
	/call setDeclare MNKEpic15																					string		outer "Fistwraps of Celestial Discipline"
	/call setDeclare MNKEpic2																						string		outer "Transcended Fistwraps of Immortality"
	/call sql_control FALSE ${chrID} MNKAlliance												string		outer "Doomwalker's Alliance"
	/call setDeclare MNKAAUnity																					string		outer FALSE
	/call setDeclare MNKAAFade																					string		outer "Imitate Death"
	
	/call sql_control FALSE ${chrID} spellCripple 											string 		outer FALSE
	/call sql_control FALSE ${chrID} switchMNKTrainMelee								bool			outer FALSE
	/call sql_control FALSE ${chrID} buffGem 														string 		outer "gem8"

	/call sql_control FALSE ${chrID} setFDAgroPct												int				outer FALSE
	/call sql_control FALSE ${chrID} setAgroPct													int				outer 95
	/call sql_control FALSE ${chrID} setFDPct														int				outer 25
 	/call sql_control FALSE ${chrID} DiscRest														string		outer PREGENDATA
 	
	/call sql_control FALSE ${chrID} AEMelee					 									bool	 		outer FALSE
	
	/call sql_control FALSE ${chrID} DiscDicho													string		outer 
	/call sql_control FALSE ${chrID} DiscBalance												string		outer PREGENDATA
	/call sql_control FALSE ${chrID} DiscMonkey													string		outer PREGENDATA
	/call sql_control FALSE ${chrID} DiscHeel														string		outer PREGENDATA
	/call sql_control FALSE ${chrID} DiscAshenhand											string		outer PREGENDATA
	/call sql_control FALSE ${chrID} DiscCrane													string		outer PREGENDATA
	| /call sql_control FALSE ${chrID} DiscDoubleAttack										string		outer PREGENDATA
	/call sql_control FALSE ${chrID} DiscFists													string		outer PREGENDATA
	/call sql_control FALSE ${chrID} DiscSynergy												string		outer PREGENDATA
	/call sql_control FALSE ${chrID} DiscFang														string		outer PREGENDATA
	/call sql_control FALSE ${chrID} DiscCurse													string		outer PREGENDATA
	/call sql_control FALSE ${chrID} DiscThrowing												string		outer PREGENDATA
	/call sql_control FALSE ${chrID} DiscShuriken												string		outer PREGENDATA
	/call sql_control FALSE ${chrID} DiscStorm													string		outer PREGENDATA
	/call sql_control FALSE ${chrID} DiscRest														string		outer PREGENDATA
	/call sql_control FALSE ${chrID} DiscDelayDeath											string		outer PREGENDATA
	/call sql_control FALSE ${chrID} DiscWu															string		outer PREGENDATA
	
	/call sql_control FALSE ${chrID} AASilent														string		outer "Silent Strikes"
	/call sql_control FALSE ${chrID} AAThunder													string		outer "Infusion of Thunder"
	/call sql_control FALSE ${chrID} AAFocus														string		outer "Focused Destructive Force"
	/call sql_control FALSE ${chrID} AATwinStrike												string		outer "Five Point Palm"
	/call sql_control FALSE ${chrID} AAChant														string		outer "Swift Tail's Chant"
	/call sql_control FALSE ${chrID} AAZanFi														string		outer "Zan Fi's Whistle"
	/call sql_control FALSE ${chrID} AAWasp															string		outer "Two-Finger Wasp Touch"
	
	/call sql_control FALSE ${chrID} MNKPoint 													int 			outer 80

/return		                                        		






|***
 *	DES: Necro base variables
 *	USE: 
 *	NOTE: 
 ***|
sub pregen_NEC()
	/declare _count int local 0
	/call setDeclare NECspire																						string		outer "|Fundament: First Spire of Necromancy|Fundament: Second Spire of Necromancy|Fundament: Third Spire of Necromancy"
	/call setDeclare NECsynergyAA																				string		outer "Defiler's Synergy"
	/call setDeclare NECEpic1																						string		outer "Scythe of the Shadowed Soul"
	/call setDeclare NECEpic15																					string		outer "Soulwhisper"
	/call setDeclare NECEpic2																						string		outer "Deathwhisper"
	/call sql_control FALSE ${chrID} NECAlliance															string		outer PREGENDATA
	/call setDeclare NECAAUnity																					string		outer "Mortifier's Unity"
	/call setDeclare NECAAFade																					string		outer FALSE

	/call sql_control FALSE ${chrID} switchAAFade														bool			outer FALSE
	/call sql_control FALSE ${chrID} useAAFadePct														int				outer 50

	/call timer_set FALSE tCheck_Blood																	string		outer 8s	timer_check_blood

	/call sql_control FALSE ${chrID} spellMalo																string 		outer PREGENDATA
	/call sql_control FALSE ${chrID} spellBlood															string 		outer "Impose for Blood"
	/call sql_control FALSE ${chrID} spellSlow		 														string		outer PREGENDATA
	/call sql_control FALSE ${chrID} spellCripple 														string 		outer PREGENDATA
	/call sql_control FALSE ${chrID} switchGather					 									bool			outer TRUE
	/call sql_control FALSE ${chrID} GatherPercent			 											int				outer 60
	| /call sql_control FALSE ${chrID} RandRezDelay														int 			outer 5
	/call sql_control FALSE ${chrID} swModRod																bool	 		outer FALSE

	| Nukes
	/for _count 1 to 8
		/call sql_control FALSE ${chrID} Nuke${_count}													string		outer FALSE
		/call sql_control FALSE ${chrID} Nuke${_count}Condition								string		outer FALSE
	/next _count	
	
	| DoTs
	/for _count 1 to 8
		/call sql_control FALSE ${chrID} DoT${_count}													string		outer FALSE
		/call sql_control FALSE ${chrID} DoT${_count}chk												string		outer FALSE
		/call sql_control FALSE ${chrID} DoT${_count}Condition									string		outer FALSE
	/next _count

	/if (${swTLP}) {
  	/call sql_control FALSE ${chrID} spellRez	 														string		outer FALSE
	}

/return		                                        		



|***
 *	DES: Paladin base variables
 *	USE: 
 *	NOTE: 
 ***|
sub pregen_PAL()
	/declare _count int local 0

	/call setDeclare PALspire																						string		outer "|Fundament: First Spire of Holiness|Fundament: Second Spire of Holiness|Fundament: Third Spire of Holiness"
	/call setDeclare PALsynergyAA																				string		outer "Knight's Synergy"
	/call setDeclare PALEpic1																						string		outer "Fiery Defender"
	/call setDeclare PALEpic15																					string		outer "Heartwood Blade"
	/call setDeclare PALEpic2																						string		outer "Nightbane, Sword of the Valiant"
	/call sql_control FALSE ${chrID} PALAlliance															string		outer PREGENDATA
	/call setDeclare PALAAUnity																					string		outer "Divine Protector's Unity"
	/call setDeclare PALAAFade																					string		outer "Balefire Burst"
	
	/call sql_control FALSE ${chrID} switchAAFade														bool			outer FALSE
	/call sql_control FALSE ${chrID} useAAFadePct														int				outer 50

	/call timer_set FALSE tcheck_MG																			string		outer	10s timer_check_MG
	/call timer_set FALSE tcheck_Steel																	string 		outer 2m timer_check_Steel
	/call timer_set FALSE tcheck_Stance																	string 		outer 7.5m timer_check_Stance

	/call setDeclare mobAECount																					int				outer 0 
	
	/call setDeclare hurtIndex																					int 			outer 0
	| /call setDeclare hurtID		 																					string 		outer FALSE
	/call setDeclare hurtPctHP																					int 			outer 100
	/call setDeclare hurtCLS		 																				string 		outer FALSE
	/call setDeclare hurtCount 																					int 			outer 0
	/call sql_control FALSE ${chrID} switchHealAggressive										bool			outer FALSE
	
	
  | Group Heal
  /call sql_control FALSE ${chrID} splashGroup															string 		outer PREGENDATA

	| self manna group heal 
  /call sql_control FALSE ${chrID} splashPanic															string 		outer PREGENDATA

	| AE Target splash cure
  /call sql_control FALSE ${chrID} splashCure															string 		outer PREGENDATA

	| self only fast heal
  /call sql_control FALSE ${chrID} healPanic1															string 		outer PREGENDATA

	| self only fast heal
  /call sql_control FALSE ${chrID} healPanic2															string 		outer PREGENDATA

	| single target heal
  /call sql_control FALSE ${chrID} healTouch																string 		outer PREGENDATA

	| single burst heal
  /call sql_control FALSE ${chrID} healBurst																string 		outer PREGENDATA

  | single light heal
  /call sql_control FALSE ${chrID} healLight																string 		outer PREGENDATA


	/call sql_control FALSE ${chrID} useGroupHeal 														int				outer FALSE
	/call sql_control FALSE ${chrID} RampageTank															string		outer FALSE
	/call sql_control FALSE ${chrID} RandRezDelay														int 			outer 5
	/call sql_control FALSE ${chrID} defCount																int				outer 2

	| yaulp attack buff
	/call sql_control FALSE ${chrID} doYaulp 																bool			outer FALSE
	| /call sql_control FALSE ${chrID} useYaulpPct															int				outer 98
	/call sql_control FALSE ${chrID} spellYaulp			 												string		outer FALSE

	| rest disc
	/call sql_control FALSE ${chrID} DiscRest		 														string		outer PREGENDATA
	
	| fast rune
	/call sql_control FALSE ${chrID} DiscFastRune			 											string		outer PREGENDATA

	| armor disc
	/call sql_control FALSE ${chrID} DefensiveDisc1			 										string		outer PREGENDATA

	| mantle disc
	/call sql_control FALSE ${chrID} DefensiveDisc2			 										string		outer PREGENDATA

	| guardian disc
	/call sql_control FALSE ${chrID} DefensiveDisc3			 										string		outer PREGENDATA


	| agro disc                                          	
	/call sql_control FALSE ${chrID} DiscUnflinching													string		outer PREGENDATA

	/call sql_control FALSE ${chrID} DiscReflexive														string		outer PREGENDATA
	/call sql_control FALSE ${chrID} AAHatePet																string		outer "Projection of Piety"
	/call sql_control FALSE ${chrID} DiscTwincast														string		outer "Hand of Tunare"
	/call sql_control FALSE ${chrID} AALayonHands														string		outer "Lay on Hands"
	/call sql_control FALSE ${chrID} AAHandofPiety														string		outer "Hand of Piety"
	/call sql_control FALSE ${chrID} AAForceofDisruption											string		outer "Force of Disruption"
	/call sql_control FALSE ${chrID} AADisruptivePersecution									string		outer "Disruptive Persecution"
	/call sql_control FALSE ${chrID} AADivineStun														string		outer "Divine Stun"
                                           	
	/call sql_control FALSE ${chrID} AAAEBeacon															string		outer "Beacon of the Righteous"
	/call sql_control FALSE ${chrID} AAAELodestar														string		outer "Hallowed Lodestar"
                                           	
	/call sql_control FALSE ${chrID} AAInquisitor														string		outer "Armor of the Inquisitor"
	/call sql_control FALSE ${chrID} AAGroupInquisitor												string		outer "Group Armor of the Inquisitor"
                                           	
	/call sql_control FALSE ${chrID} AAJudgement															string		outer "Inquisitor's Judgement"
	/call sql_control FALSE ${chrID} AAValorousRage													string		outer "Valorous Rage"
	/call sql_control FALSE ${chrID} AAShield																string		outer "Shield of Brilliance"
	/call sql_control FALSE ${chrID} chainStun																string		outer FALSE
	/call sql_control FALSE ${chrID} AEMelee					 												bool	 		outer FALSE
	                                         	
	| Nukes
	/for _count 1 to 8
		/call sql_control FALSE ${chrID} Nuke${_count}													string		outer FALSE
		/call sql_control FALSE ${chrID} Nuke${_count}Condition								string		outer FALSE
	/next _count	
	
	| DoTs
	/for _count 1 to 8
		/call sql_control FALSE ${chrID} DoT${_count}													string		outer FALSE
		/call sql_control FALSE ${chrID} DoT${_count}chk												string		outer FALSE
		/call sql_control FALSE ${chrID} DoT${_count}Condition									string		outer FALSE
	/next _count
	
	/call sql_control FALSE ${chrID} spellCripple 														string 		outer FALSE
		
	| single target challenge spell
	/call sql_control FALSE ${chrID} ChallengeSpell			 										string		outer PREGENDATA

	| def proc spell
	/call sql_control FALSE ${chrID} ValiantDefense			 										string		outer PREGENDATA

	| steel weapon heal proc
	/call sql_control FALSE ${chrID} Steel			 															string 		outer PREGENDATA
	/call sql_control FALSE ${chrID} switchSteel			 												bool	 		outer FALSE

	| stance temp HP buff
	/call sql_control FALSE ${chrID} Stance			 														string		outer PREGENDATA
	/call sql_control FALSE ${chrID} switchStance 														bool	 		outer FALSE

	/call sql_control FALSE ${chrID} Twincast																string 		outer "Glorious Exoneration"
	/call sql_control FALSE ${chrID} swModRod																bool	 		outer FALSE
	/call sql_control FALSE ${chrID} Harmonious 															string 		outer PREGENDATA
	/call sql_control FALSE ${chrID} switchHarmonious 												bool	 		outer FALSE

	/call sql_control FALSE ${chrID} useMG																		bool			outer TRUE
	/call sql_control FALSE ${chrID} useMGPct																int				outer 60
	/call sql_control FALSE ${chrID} useShieldFlash													bool			outer FALSE
	/call sql_control FALSE ${chrID} useShieldFlashPct												int				outer 20
	/call sql_control FALSE ${chrID} useLoH																	bool			outer FALSE
	/call sql_control FALSE ${chrID} useLoHPct																int				outer 10
	/call sql_control FALSE ${chrID} switchAttraction												bool			outer FALSE

	/if (${swTLP}) {
  	/call sql_control FALSE ${chrID} spellRez	 														string		outer FALSE
	}


/return		                                        		




|***
 *	DES: Ranger base variables
 *	USE: 
 *	NOTE: 
 ***|
sub pregen_RNG()
	/declare _count int local 0

	| unity type - rangers get azia and beza
	/call sql_control FALSE ${chrID} unityType																string		outer "Azia"
	
	/call setDeclare RNGspire																						string		outer "|Fundament: First Spire of the Pathfinders|Fundament: Second Spire of the Pathfinders|Fundament: Third Spire of the Pathfinders"
	/call setDeclare RNGauspice																					string		outer "Auspice of the Hunter"
	/call setDeclare RNGcheetah																					string		outer "Scarlet Cheetah's Fang"
	/call setDeclare RNGgroupguardian																		string		outer "Group Guardian of the Forest"
	/call setDeclare RNGsynergyAA																				string		outer "Outrider's Synergy"
	/call setDeclare RNGEpic1																						string		outer "Earthcaller"
	/call setDeclare RNGEpic15																					string		outer "Heartwood Blade"
	/call setDeclare RNGEpic2																						string		outer "Aurora, the Heartwood Blade"
	/call sql_control FALSE ${chrID} RNGAlliance															string		outer PREGENDATA
	/call setDeclare RNGAAUnity																					string		outer "Wildstalker's Unity (${unityType})"
	/call setDeclare RNGAAFade																					string		outer "Cover Tracks"
	
	/call sql_control FALSE ${chrID} switchAAFade														bool			outer FALSE
	/call sql_control FALSE ${chrID} useAAFadePct														int				outer 50
		

	/call sql_control FALSE ${chrID} spellCripple 														string 		outer FALSE

	/call sql_control FALSE ${chrID} AADefensive1 														string		outer "Outrider's Evasion"
	/call sql_control FALSE ${chrID} AADefensive2 														string		outer "Protection of the Spirit Wolf"
	/call sql_control FALSE ${chrID} AADefensive3 														string		outer "Bulwark of the Brownies"
	/call sql_control FALSE ${chrID} swModRod																bool	 		outer FALSE
	/call sql_control FALSE ${chrID} RNGPoint		 														int 			outer 80

	| Summer's line
	/call timer_set FALSE tNuke_Summer																	string		outer 3s 	timer_cast_nuke_summer

	/call sql_control FALSE ${chrID} NukeSummer		 													string		outer PREGENDATA
	/call sql_control FALSE ${chrID} NukeSummerCondition											string		outer FALSE
	/call sql_control FALSE ${chrID} switchNukeSummer		 										bool			outer TRUE

	| Nukes
	/for _count 1 to 8
		/call sql_control FALSE ${chrID} Nuke${_count}													string		outer FALSE
		/call sql_control FALSE ${chrID} Nuke${_count}Condition								string		outer FALSE
	/next _count	
	
	| DoTs
	/for _count 1 to 8
		/call sql_control FALSE ${chrID} DoT${_count}													string		outer FALSE
		/call sql_control FALSE ${chrID} DoT${_count}chk												string		outer FALSE
		/call sql_control FALSE ${chrID} DoT${_count}Condition									string		outer FALSE
	/next _count

	| ToT DS DoT / debuff
	/call sql_control FALSE ${chrID} DoTSwarmDS		 													string		outer PREGENDATA
	/call sql_control FALSE ${chrID} DoTSwarmDSdeBuff 												string		outer PREGENDATA
	
	| slow heal
  /call sql_control FALSE ${chrID} heal1	 																	string 		outer PREGENDATA

	| fast heal
  /call sql_control FALSE ${chrID} heal2				 														string 		outer PREGENDATA

	| bow disc
	/call sql_control FALSE ${chrID} DiscRange			 													string		outer PREGENDATA

	| rest disc
	/call sql_control FALSE ${chrID} DiscRest		 														string		outer PREGENDATA
	
	| melee disc
	/call sql_control FALSE ${chrID} DiscMele			 													string		outer PREGENDATA

	| agro/anti agro kicks
	/call sql_control FALSE ${chrID} DiscJolt			 													string		outer PREGENDATA
	/call sql_control FALSE ${chrID} DiscEnrage		 													string		outer PREGENDATA

	| AE group heal disc
	/call sql_control FALSE ${chrID} DiscHeal			 													string		outer PREGENDATA

	| ae melee disc
	/call sql_control FALSE ${chrID} BladeStorm			 												string		outer PREGENDATA

	| focused melee disc
	/call sql_control FALSE ${chrID} DiscSqual			 													string		outer PREGENDATA

/return
                                     		


|***
 *	DES: Rogue base variables
 *	USE: 
 *	NOTE: 
 ***|
sub pregen_ROG()

	/call setDeclare ROGspire																						string		outer "|Fundament: First Spire of the Rake|Fundament: Second Spire of the Rake|Fundament: Third Spire of the Rake"
	/call setDeclare ROGsynergyAA																				string		outer "Blackguard's Synergy"
	/call setDeclare ROGEpic1																						string		outer "Ragebringer"
	/call setDeclare ROGEpic15																					string		outer "Fatestealer"
	/call setDeclare ROGEpic2																						string		outer "Nightshade, Blade of Entropy"
	/call sql_control FALSE ${chrID} ROGAlliance															string		outer PREGENDATA
	/call setDeclare ROGAAUnity																					string		outer FALSE
	/call setDeclare ROGAAFade																					string		outer "Escape"
	
	/call sql_control FALSE ${chrID} switchAAFade														bool			outer FALSE
	/call sql_control FALSE ${chrID} useAAFadePct														int				outer 50

	| keep on cooldown
	/call timer_set FALSE tClick_Poison																	string		outer 30s 	timer_click_poison
	/call sql_control FALSE ${chrID} AEMelee					 												bool	 		outer FALSE
                                             		
	/call sql_control FALSE ${chrID} DiscDicho																string		outer PREGENDATA
	/call sql_control FALSE ${chrID} DiscRest				 												string 		outer PREGENDATA
	/call sql_control FALSE ${chrID} DiscAggro 															string 		outer PREGENDATA
	/call sql_control FALSE ${chrID} DiscAssault 														string 		outer PREGENDATA
	/call sql_control FALSE ${chrID} DiscBleed 															string 		outer PREGENDATA
	/call sql_control FALSE ${chrID} DiscPlay	 															string 		outer PREGENDATA
	/call sql_control FALSE ${chrID} DiscPhantom															string 		outer PREGENDATA
	/call sql_control FALSE ${chrID} DiscHack																string 		outer PREGENDATA
	/call sql_control FALSE ${chrID} DiscHiddenBlade													string 		outer PREGENDATA
	/call sql_control FALSE ${chrID} DiscBladePoison													string 		outer PREGENDATA
	/call sql_control FALSE ${chrID} DiscSneakAttack													string 		outer PREGENDATA
	/call sql_control FALSE ${chrID} DiscShadowHunter												string 		outer PREGENDATA
	/call sql_control FALSE ${chrID} DiscDisassociative											string 		outer PREGENDATA
	/call sql_control FALSE ${chrID} DiscAspBleeder													string 		outer PREGENDATA
	/call sql_control FALSE ${chrID} DiscBlinding														string 		outer PREGENDATA
                                             		
	| disable for burn                         		
	/call sql_control FALSE ${chrID} DiscVision 															string 		outer PREGENDATA
                                             		
	| burn                                     		
	/call sql_control FALSE ${chrID} DiscPinPoint 														string 		outer "Pinpoint Deficiencies"
	/call sql_control FALSE ${chrID} DiscMark 																string 		outer PREGENDATA
	/call sql_control FALSE ${chrID} DiscEradicator 													string 		outer PREGENDATA
	/call sql_control FALSE ${chrID} DiscRazor 															string 		outer PREGENDATA
	/call sql_control FALSE ${chrID} DiscFrenzy 															string 		outer PREGENDATA
	/call sql_control FALSE ${chrID} DiscTwistedChance												string 		outer PREGENDATA
                                             		              				        	
	/call sql_control FALSE ${chrID} TwistedShank														string 		outer "Twisted Shank"
	/call sql_control FALSE ${chrID} AbsorbingAgent													string 		outer "Absorbing Agent"
	                                           		              				        	
	/call sql_control FALSE ${chrID} clickPoisonBuff													string 		outer "|Bite of the Shissar Poison X|Tallon's Tactic Poison IV|Tallon's Tactic Poison V|Vallon's Tactic Poison IV|Vallon's Tactic Poison V|Mana Poison Coat|Hero's Toxin Poison I|Hero's Toxin Poison II"
	/call sql_control FALSE ${chrID} clickPoisonItem													string 		outer "Consigned Bite of the Shissar XVIII"
	/call sql_control FALSE ${chrID} clickPoisonDispenser										string 		outer "Selrach's Shadowscale Leggings"
	/call sql_control FALSE ${chrID} clickPoisonCount												int		 		outer 60
                                             		              				        	
	/call sql_control FALSE ${chrID} spellCripple 														string 		outer FALSE
                                             		              				        	
	/call sql_control FALSE ${chrID} ROGPoint 																int 			outer 80


/return



|***
 *	DES: Shadowknight base variables
 *	USE: 
 *	NOTE: 
 ***|
sub pregen_SHD()
	/declare _count int local 0
	
	/call timer_set FALSE tcheck_vot																		string		outer 1215s timer_check_vot
	/call timer_set FALSE tcheck_Skin																		string 		outer 45s timer_check_Skin
	/call timer_set FALSE tcheck_Stance																	string 		outer 3m timer_check_Stance

	| unity type - sks get azia and beza
	/call sql_control FALSE ${chrID} unityType																string		outer "Azia"

	/call setDeclare SHDspire																						string		outer "|Fundament: First Spire of the Reavers|Fundament: Second Spire of the Reavers|Fundament: Third Spire of the Reavers"
	/call setDeclare SHDsynergyAA																				string		outer "Knight's Synergy"
	/call setDeclare SHDEpic1																						string		outer "Innoruuk's Curse"
	/call setDeclare SHDEpic15																					string		outer "Innoruuk's Voice"
	/call setDeclare SHDEpic2																						string		outer "Innoruuk's Dark Blessing"
	/call sql_control FALSE ${chrID} SHDAlliance															string		outer "Bloodletting Alliance"
	/call setDeclare SHDAAUnity																					string		outer "Dark Lord's Unity (${unityType})"
	/call setDeclare SHDAAFade																					string		outer "Death Peace"
	
	/call sql_control FALSE ${chrID} switchAAFade														bool			outer FALSE
	/call sql_control FALSE ${chrID} useAAFadePct														int				outer 50

	/call setDeclare mobAECount																					int				outer 0

	/call sql_control FALSE ${chrID} BladeStrike 														string 		outer "Lacerating Blade"
	/call sql_control FALSE ${chrID} DiscLeech																string		outer	"Leechcurse Discipline"
	/call sql_control FALSE ${chrID} AELifeTap 															string 		outer "Deceitful Deflection"
	/call sql_control FALSE ${chrID} AAHatePet																string		outer "Projection of Doom"

	/call sql_control FALSE ${chrID} switchVoT				 												bool		 	outer TRUE
	/call sql_control FALSE ${chrID} SHDPoint 																int 			outer 80
	/call sql_control FALSE ${chrID} useShieldFlash													bool			outer FALSE
	/call sql_control FALSE ${chrID} useShieldFlashPct												int				outer 20
	/call sql_control FALSE ${chrID} switchAttraction												bool			outer TRUE

	| fast rune
	/call sql_control FALSE ${chrID} Withstand			 													string		outer PREGENDATA

	| blade DPS
	/call sql_control FALSE ${chrID} CrimsonBlade			 											string		outer PREGENDATA
	
	| rest disc
	/call sql_control FALSE ${chrID} DiscRest		 														string		outer PREGENDATA

	| acromony
	/call sql_control FALSE ${chrID} DiscFakeHate			 											string		outer PREGENDATA
	
	| rancor	
	/call sql_control FALSE ${chrID} DiscRancor				 											string		outer PREGENDATA

	| defensive mantle 
	/call sql_control FALSE ${chrID} DefensiveDisc1			 										string		outer PREGENDATA

	| defensive carpace
	/call sql_control FALSE ${chrID} DefensiveDisc2			 										string		outer PREGENDATA

	| defensive guardian
	/call sql_control FALSE ${chrID} DefensiveDisc3			 										string		outer PREGENDATA


	/call sql_control FALSE ${chrID} defCount																int				outer 2
	/call sql_control FALSE ${chrID} AEMelee					 												bool	 		outer FALSE

	| challenge	
	/call sql_control FALSE ${chrID} ChallengeSpell			 										string		outer PREGENDATA
	
	/call sql_control FALSE ${chrID} spellCripple														string 		outer FALSE
	/call sql_control FALSE ${chrID} swModRod																bool			outer FALSE
	/call sql_control FALSE ${chrID} Harmonious 															string 		outer "Harmonious Disruption"
	/call sql_control FALSE ${chrID} switchHarmonious 												bool	 		outer FALSE

	| single hate
	/call sql_control FALSE ${chrID} Hate1 																	string 		outer PREGENDATA
	/call sql_control FALSE ${chrID} Hate2 																	string 		outer PREGENDATA

	| AE Hate	
	/call sql_control FALSE ${chrID} AEHate1 																string	 	outer PREGENDATA
	/call sql_control FALSE ${chrID} AEHate2 																string	 	outer PREGENDATA

	| stance	
	/call sql_control FALSE ${chrID} Stance			 														string		outer PREGENDATA
	/call sql_control FALSE ${chrID} switchStance 														bool	 		outer FALSE

	| skin rune buff	
	/call sql_control FALSE ${chrID} Skin			 															string		outer PREGENDATA
	/call sql_control FALSE ${chrID} switchSkin			 												bool	 		outer FALSE
	
	| Nukes
	/for _count 1 to 8
		/call sql_control FALSE ${chrID} Nuke${_count}													string		outer FALSE
		/call sql_control FALSE ${chrID} Nuke${_count}Condition								string		outer FALSE
	/next _count	
	
	| DoTs
	/for _count 1 to 8
		/call sql_control FALSE ${chrID} DoT${_count}													string		outer FALSE
		/call sql_control FALSE ${chrID} DoT${_count}chk												string		outer FALSE 
		/call sql_control FALSE ${chrID} DoT${_count}Condition									string		outer FALSE
	/next _count

/return



|***
 *	DES: Shaman base variables
 *	USE: 
 *	NOTE: 
 ***|
sub pregen_SHM()
	/declare _count int local 0

	/call setDeclare SHMspire																						string		outer "|Fundament: First Spire of Ancestors|Fundament: Second Spire of Ancestors|Fundament: Third Spire of Ancestors"
	/call setDeclare SHMsynergyAA																				string		outer "Luminary's Synergy"
	/call setDeclare SHMAid																							string		outer "Ancestral Aid"
	/call setDeclare SHMEpic1																						string		outer "Spear of Fate"
	/call setDeclare SHMEpic15																					string		outer "Crafted Talisman of Fates"
	/call setDeclare SHMEpic2																						string		outer "Blessed Spiritstaff of the Heyokah"
	/call sql_control FALSE ${chrID} SHMAlliance															string		outer "Ancient Alliance"
	/call setDeclare SHMAAUnity																					string		outer "Visionary's Unity"
	/call setDeclare SHMAAFade																					string		outer FALSE
	
	/call sql_control FALSE ${chrID} switchAAFade														bool			outer FALSE
	/call sql_control FALSE ${chrID} useAAFadePct														int				outer 50
	
	/call setDeclare hurtID		 																					string 		outer FALSE
	/call setDeclare hurtIndex																					int 			outer 0
	/call setDeclare hurtPctHP																					int 			outer 100
	/call setDeclare hurtCLS		 																				string 		outer FALSE
	/call setDeclare hurtCount 																					int 			outer 0

	/call timer_set FALSE tTalisman																			string		outer 30s timer_cast_talisman
	/call sql_control FALSE ${chrID} spellTalisman	 													string		outer PREGENDATA
	/call sql_control FALSE ${chrID} switchTalisman			 										bool			outer TRUE
                                          		
	| /call sql_control FALSE ${chrID} AidTXT 																	string 		outer "ANCESTRAL AID Done (Str/Agi/Dex +300, HoT Heal +1840)"
	/call sql_control FALSE ${chrID} spellMalo 															string 		outer PREGENDATA
	/call sql_control FALSE ${chrID} spellCripple 														string 		outer PREGENDATA
	/call sql_control FALSE ${chrID} spellSlow																string 		outer PREGENDATA
	/call sql_control FALSE ${chrID} spellAESlow 														string 		outer PREGENDATA
	                                        		
	/call sql_control FALSE ${chrID} TC1									 										string		outer "Glacial Gift"
	/call sql_control FALSE ${chrID} TC2									 										string		outer "Frigid Gift"
	
	
	| Nukes
	/for _count 1 to 8
		/call sql_control FALSE ${chrID} Nuke${_count}													string		outer FALSE
		/call sql_control FALSE ${chrID} Nuke${_count}Condition								string		outer FALSE
	/next _count	
	
	| DoTs
	/for _count 1 to 8
		/call sql_control FALSE ${chrID} DoT${_count}													string		outer FALSE
		/call sql_control FALSE ${chrID} DoT${_count}chk												string		outer FALSE
		/call sql_control FALSE ${chrID} DoT${_count}Condition									string		outer FALSE
	/next _count
	
 	/if (${swTLP}) {
  	/call sql_control FALSE ${chrID} spellRez	 														string		outer FALSE
	}
	                                         		
	/call sql_control FALSE ${chrID} CanniPctHPs				 											int				outer 60
	/call sql_control FALSE ${chrID} CanniPctManna				 										int				outer 60
	/call sql_control FALSE ${chrID} spellCannibalization										string		outer PREGENDATA

	/call sql_control FALSE ${chrID} switchSurge															bool			outer TRUE
	/call sql_control FALSE ${chrID} switchHealAggressive										bool			outer FALSE
                                          		
	/call sql_control FALSE ${chrID} useGroupHeal 														bool			outer TRUE
	/call sql_control FALSE ${chrID} useGroupHoT															bool			outer TRUE
	/call sql_control FALSE ${chrID} RandRezDelay														int 			outer 10
                                          		
  /call sql_control FALSE ${chrID} healSurge																string 		outer PREGENDATA
  /call sql_control FALSE ${chrID} healMain																string 		outer PREGENDATA
  /call sql_control FALSE ${chrID} healIntervention1												string 		outer PREGENDATA
  /call sql_control FALSE ${chrID} healReckless1														string 		outer PREGENDATA
  /call sql_control FALSE ${chrID} healReckless2														string 		outer FALSE
  /call sql_control FALSE ${chrID} healReckless3														string 		outer FALSE
  /call sql_control FALSE ${chrID} healWholeGroup													string 		outer PREGENDATA
  /call sql_control FALSE ${chrID} healWholeGroupHoT												string 		outer PREGENDATA
	/call sql_control FALSE ${chrID} healHoT																	string		outer PREGENDATA                                          		
	/call sql_control FALSE ${chrID} ProtectOutsideGroup											string		outer "|FALSE"
	/call sql_control FALSE ${chrID} ProtectInsideRaid												string		outer "|FALSE"
	/call sql_control FALSE ${chrID} ProtectNPCList													string		outer "|FALSE"
	/call sql_control FALSE ${chrID} RampageTank															string		outer FALSE
	/call sql_control FALSE ${chrID} swModRod																bool	 		outer FALSE


	| temp HP grow line
	/call sql_control FALSE ${chrID} switchGrowth									bool  outer FALSE
 	/if (${Me.Level} >= 106) {
	  /call sql_control FALSE ${chrID} spellGrowth 								string  outer "Savage Growth"
	} else /if (${Me.Level} >= 101) {	
	  /call sql_control FALSE ${chrID} spellGrowth 								string  outer "Ferocious Growth"
	} else /if (${Me.Level} >= 96) {	
	  /call sql_control FALSE ${chrID} spellGrowth 								string  outer "Rampant Growth"
	} else /if (${Me.Level} >= 91) {	
	  /call sql_control FALSE ${chrID} spellGrowth 								string  outer "Unfettered Growth"
	} else /if (${Me.Level} >= 86) {	
	  /call sql_control FALSE ${chrID} spellGrowth 								string  outer "Untamed Growth"
	} else /if (${Me.Level} >= 81) {	
	  /call sql_control FALSE ${chrID} spellGrowth 								string  outer "Wild Growth"
	} else {	
	  /call sql_control FALSE ${chrID} spellGrowth 								string  outer FALSE
	}
	/call sql_control FALSE ${chrID} spellGrowthToon							string  outer |FALSE

/return



|***
 *	DES: Warrior base variables
 *	USE: 
 *	NOTE: 
 ***|
sub pregen_WAR()

	/call setDeclare WARspire																						string		outer "|Fundament: First Spire of the Warlord|Fundament: Second Spire of the Warlord|Fundament: Third Spire of the Warlord"
	/call setDeclare WARsynergyAA																				string		outer "Myrmidon's Synergy"
	/call setDeclare WAREpic1																						string		outer "Jagged Blade of War"
	/call setDeclare WAREpic15																					string		outer "Champion's Sword of Eternal Power"
	/call setDeclare WAREpic2																						string		outer "Kreljnok's Sword of Eternal Power"
	/call sql_control FALSE ${chrID} WARAlliance															string		outer PREGENDATA
	/call setDeclare WARAAUnity																					string		outer FALSE
	/call setDeclare WARAAFade																					string		outer FALSE
	
	/call sql_control FALSE ${chrID} switchAAFade														bool			outer FALSE
	/call sql_control FALSE ${chrID} useAAFadePct														int				outer 50
	/call sql_control FALSE ${chrID} AEMelee					 												bool		 	outer FALSE

	/call sql_control FALSE ${chrID} DiscDefensive 													string 		outer PREGENDATA
	/call sql_control FALSE ${chrID} DiscInsult 															string 		outer PREGENDATA
	/call sql_control FALSE ${chrID} DiscCyclone			 												string		outer PREGENDATA
	/call sql_control FALSE ${chrID} DiscWade				 												string		outer PREGENDATA
	/call sql_control FALSE ${chrID} DiscChallenge														string		outer PREGENDATA
	/call sql_control FALSE ${chrID} DiscShout				 												string		outer PREGENDATA
	/call sql_control FALSE ${chrID} DiscRest			 													string		outer PREGENDATA
	/call sql_control FALSE ${chrID} DiscStoutDefense 												string		outer PREGENDATA
	/call sql_control FALSE ${chrID} DiscField				 												string		outer PREGENDATA
	/call sql_control FALSE ${chrID} DiscFlash				 												string		outer PREGENDATA
	/call sql_control FALSE ${chrID} DiscNoTime				 											string		outer PREGENDATA
	/call sql_control FALSE ${chrID} DiscAttention								 						string		outer PREGENDATA
	/call sql_control FALSE ${chrID} DiscPhantom									 						string		outer PREGENDATA
	/call sql_control FALSE ${chrID} DiscShieldBash 													string		outer PREGENDATA
	/call sql_control FALSE ${chrID} DiscExpanse 														string		outer PREGENDATA
	/call sql_control FALSE ${chrID} DiscPrecision 													string		outer PREGENDATA
	/call sql_control FALSE ${chrID} DiscWarriors 														string		outer PREGENDATA
	/call sql_control FALSE ${chrID} DiscRoar 																string		outer PREGENDATA
	/call sql_control FALSE ${chrID} DiscShocking 														string		outer PREGENDATA
	/call sql_control FALSE ${chrID} AAGutPunch 															string		outer "Gut Punch"
	/call sql_control FALSE ${chrID} AAWarStomp 															string		outer "War Stomp"
	/call sql_control FALSE ${chrID} AABraceforImpact 												string		outer "Brace for Impact"
	/call sql_control FALSE ${chrID} AAMageHunter														string		outer "Mark of the Mage Hunter"
	/call sql_control FALSE ${chrID} AAVehementRage 													string		outer "Vehement Rage"
	/call sql_control FALSE ${chrID} AABladeGuardian 												string		outer "Blade Guardian"
	/call sql_control FALSE ${chrID} AAKneeStrike 														string		outer "Knee Strike"
	/call sql_control FALSE ${chrID} AAImpCommand														string		outer "Imperator's Command"
	/call sql_control FALSE ${chrID} AAHeroicBlade 													string		outer "Ward Sheol's Heroic Blade"
	/call sql_control FALSE ${chrID} AAResplendentGlory 											string		outer "Resplendent Glory"
	/call sql_control FALSE ${chrID} AAHoldtheLine 													string		outer "Hold the Line"
	/call sql_control FALSE ${chrID} AAAETaunt 															string		outer "Area Taunt"
	/call sql_control FALSE ${chrID} AABlastofAnger													string		outer "Blast of Anger"
	/call sql_control FALSE ${chrID} AARageoftheForsaken											string		outer "Rage of the Forsaken"
	/call sql_control FALSE ${chrID} AAWarlordsTenacity											string		outer "Warlords Tenacity"
	/call sql_control FALSE ${chrID} AAWarlordsBravery												string		outer "Warlords Bravery"
	/call sql_control FALSE ${chrID} defCount																int				outer 2
	                                        		
	/call sql_control FALSE ${chrID} PanicHPPCT		 													int				outer 25
	/call sql_control FALSE ${chrID} spellCripple 														string 		outer FALSE
	/call sql_control FALSE ${chrID} switchAttraction												bool			outer FALSE
	
	/call sql_control FALSE ${chrID} WARPoint 																int 			outer 80

/return



|***
 *	DES: Wizard base variables
 *	USE: 
 *	NOTE: 
 ***|
sub pregen_WIZ()
	/declare _count int local 0
	/declare _list int local 0
	/declare _line string local NULL

	/call setDeclare WIZspire																						string		outer "|Fundament: First Spire of Arcanum|Fundament: Second Spire of Arcanum|Fundament: Third Spire of Arcanum"
	/call setDeclare WIZmannaburn																				string		outer "Manna Burn"
	/call setDeclare WIZsynergyAA																				string		outer "Evoker's Synergy"
	/call setDeclare WIZEpic1																						string		outer "Staff of the Four"
	/call setDeclare WIZEpic15																					string		outer "Staff of Prismatic Power"
	/call setDeclare WIZEpic2																						string		outer "Staff of Phenomenal Power"
	/call sql_control FALSE ${chrID} WIZAlliance															string		outer PREGENDATA
	/call setDeclare WIZAAUnity																					string		outer "Etherealist's Unity"
	
	/call setDeclare WIZAAFade																					string		outer "A Hole in Space"
	/call sql_control FALSE ${chrID} switchAAFade														bool			outer FALSE
	/call sql_control FALSE ${chrID} useAAFadePct														int				outer 50

	/call timer_set FALSE tForce_Twincast																string		outer 3s timer_force_twincast
	/call sql_control FALSE ${chrID} GatherSpell															string 		outer PREGENDATA
	/call sql_control FALSE ${chrID} switchGather					 									bool			outer TRUE
	/call sql_control FALSE ${chrID} GatherPercent			 											int				outer 60
	
	/call sql_control FALSE ${chrID} tempPet						 											string		outer PREGENDATA
	/call sql_control FALSE ${chrID} switchManaBurn		 											bool			outer TRUE
	/call sql_control FALSE ${chrID} spellCripple 														string 		outer FALSE
	/call sql_control FALSE ${chrID} spellConcussion													string 		outer PREGENDATA
	/call sql_control FALSE ${chrID} spellConcussionPct											int 			outer 0

	/call setDeclare lineType																						string		outer "|off|fire|ice|magic|mix"
	/call sql_control FALSE ${chrID} lineCurrent															string 		outer FALSE
	/call sql_control FALSE ${chrID} lineFire																string		outer FALSE
	/call sql_control FALSE ${chrID} lineIce																	string		outer FALSE
	/call sql_control FALSE ${chrID} lineMagic																string		outer FALSE
	/call sql_control FALSE ${chrID} lineMix																	string		outer FALSE

	| add conditions
	/for _list 1 to ${lineType.Count[|]}
		/varset _line ${lineType.Arg[${_list},|].Left[1].Upper}${lineType.Arg[${_list},|].Right[-1].Lower}
		/for _count 1 to 8	
			/call sql_control FALSE ${chrID} line${_line}Condition${_count}			string		outer FALSE
		/next _count
	/next _list	
	
	/call sql_control FALSE ${chrID} lineAAFire															string		outer "Pyromancy"
	/call sql_control FALSE ${chrID} lineAAIce																string		outer "Cryomancy"
	/call sql_control FALSE ${chrID} lineAAMagic															string		outer "Arcomancy"
	/call sql_control FALSE ${chrID} lineAAMix																string		outer FALSE
	
	/call sql_control FALSE ${chrID} switchForceTwincast											bool			outer FALSE
	/call sql_control FALSE ${chrID} NukeTwincastFire												string		outer PREGENDATA
	/call sql_control FALSE ${chrID} NukeTwincastIce													string		outer PREGENDATA
	/call sql_control FALSE ${chrID} NukeTwincastMagic												string		outer PREGENDATA
	/call sql_control FALSE ${chrID} NukeTwincastMix													string		outer FALSE
	/call sql_control FALSE ${chrID} NukeTwincastCondition 									string		outer FALSE
	
	
	| faydwer ports
	/call setDeclare fay 																								string		outer "Translocate: Fay"
	/call setDeclare fay_group 																					string		outer "Fay Portal"
	/call setDeclare dragonscale 																				string		outer "Translocate: Dragonscale Hills"
	/call setDeclare dragonscale_group 																	string		outer "Dragonscale Hills Portal"

	| antonica Ports
	/call setDeclare northkarana 																				string		outer "Translocate: North"
	/call setDeclare northkarana_group 																	string		outer "North Portal"
	/call setDeclare commonlands 																				string		outer "Translocate: Common"
	/call setDeclare commonlands_group 																	string		outer "Common Portal"
	/call setDeclare nektulos 																					string		outer "Translocate: Nek"
	/call setDeclare nektulos_group 																		string		outer "Nek Portal"
	/call setDeclare qeyenos 																						string		outer "Translocate: West"
	/call setDeclare qeyenos_group 																			string		outer "West Portal"
	/call setDeclare northro 																						string		outer "Translocate: Ro"
	/call setDeclare northro_group 																			string		outer "Ro Portal"
	/call setDeclare cazic 																							string		outer "Translocate: Cazic"
	/call setDeclare cazic_group 																				string		outer "Cazic Portal"
	/call setDeclare undershore 																				string		outer "Translocate: Undershore"
	/call setDeclare undershore_group 																	string		outer "Undershore Portal"
	/call setDeclare westkarana 																				string		outer "Translocate: West Karana"
	/call setDeclare westkarana_group 																	string		outer "West Karana Portal"

	| discord ports 
	/call setDeclare bloodfields 																				string		outer "Translocate: Bloodfields"
	/call setDeclare bloodfields_group 																	string		outer "Bloodfields Portal"
	/call setDeclare wallofslaughter 																		string		outer "Translocate: Slaughter"
	/call setDeclare wallofslaughter_group 															string		outer "Slaughter Portal"

	| alaris ports
	/call setDeclare shardslanding 																			string		outer "Translocate: Shard's Landing"
	/call setDeclare shardslanding_group 																string		outer "Shard's Landing Portal"
	/call setDeclare pillars 																						string		outer "Translocate Pillars of Alra"
	/call setDeclare pillars_group 																			string		outer "Alra Portal"
	/call setDeclare sarith 																						string		outer "Translocate Sarith"
	/call setDeclare sarith_group 																			string		outer "Sarith Portal"

	| Kunark ports
	/call setDeclare dreadlands 																				string		outer "Translocate: Combine"
	/call setDeclare dreadlands_group 																	string		outer "Combine Portal"
	/call setDeclare lceanium																						string		outer "Translocate: Lceanium"
	/call setDeclare lceanium_group																			string		outer "Lceanium Portal"
	/call setDeclare skyfire 																						string		outer "Translocate: Skyfire"
	/call setDeclare skyfire_group 																			string		outer "Skyfire Portal"	

	| liclin ports
	/call setDeclare dawnshroud 																				string		outer "Translocate: Dawnshroud"
	/call setDeclare dawnshroud_group 																	string		outer "Dawnshroud Portal"
	/call setDeclare twilight 																					string		outer "Translocate: Twilight"
	/call setDeclare twilight_group 																		string		outer "Twilight Portal"
	/call setDeclare grimling 																					string		outer "Translocate: Grimling"
	/call setDeclare grimling_group 																		string		outer "Grimling Portal"
	/call setDeclare nexus 																							string		outer "Translocate: Nexus"
	/call setDeclare nexus_group 																				string		outer "Nexus Portal"

	| odus ports
	/call setDeclare tempesttemple 																			string		outer "Translocate: Tempest Temple"
	/call setDeclare tempesttemple_group 																string		outer "Tempest Temple Portal"
	/call setDeclare katta																							string		outer "Translocate: Katta Castrum"
	/call setDeclare katta_group																				string		outer "Katta Castrum Portal"
	/call setDeclare tox 																								string		outer "Translocate: Tox"
	/call setDeclare tox_group 																					string		outer "Tox Portal"
	/call setDeclare stonebrunt 																				string		outer "Translocate: Stonebrunt"
	/call setDeclare stonebrunt_group 																	string		outer "Stonebrunt Portal"

	| serpent's spine ports
	/call setDeclare icefall 																						string		outer "Translocate: Icefall Glacier"
	/call setDeclare icefall_group 																			string		outer "Icefall Glacier Portal"
	/call setDeclare sunderock 																					string		outer "Translocate: Sunderock Springs"
	/call setDeclare sunderock_group 																		string		outer "Sunderock Springs Portal"
	/call setDeclare blightfire 																				string		outer "Translocate: Blightfire Moors"
	/call setDeclare blightfire_group 																	string		outer "Blightfire Moors Portal"

	| taelosia ports
	/call setDeclare barnidu 																						string		outer "Translocate: Barindu"
	/call setDeclare barnidu_group 																			string		outer "Barindu Portal"
	/call setDeclare natimbi 																						string		outer "Translocate: Natimbi"
	/call setDeclare natimbi_group 																			string		outer "Natimbi Portal"

	| planes ports
	/call setDeclare grounds 																						string		outer "Translocate: the Grounds"
	/call setDeclare grounds_group 																			string		outer "Grounds Portal"
	/call setDeclare potime 																						string		outer "Translocate: Plane of Time"
	/call setDeclare potime_group 																			string		outer "Plane of Time Portal"
	/call setDeclare brells 																						string		outer "Translocate: Brell's Rest"
	/call setDeclare brells_group 																			string		outer "Brell's Rest Portal"
	/call setDeclare arcstone 																					string		outer "Translocate: Arcstone"
	/call setDeclare arcstone_group 																		string		outer "Arcstone Portal"
	/call setDeclare pok 																								string		outer "Translocate: Knowledge"
	/call setDeclare pok_group 																					string		outer "Knowledge Portal"
	/call setDeclare esianti 																						string		outer "Translocate: Esianti"
	/call setDeclare esianti_group 																			string		outer "Esianti Portal"

	| velious ports
	/call setDeclare cobaltscar 																				string		outer "Translocate: Cobalt Scar"
	/call setDeclare cobaltscar_group 																	string		outer "Cobalt Scar Portal"
	/call setDeclare wakening 																					string		outer "Translocate: Wakening Lands"
	/call setDeclare wakening_group 																		string		outer "Wakening Lands Portal"
	/call setDeclare greatdivide 																				string		outer "Translocate: Great Divide"
	/call setDeclare greatdivide_group 																	string		outer "Great Divide Portal"
	/call setDeclare iceclad 																						string		outer "Translocate: Iceclad"
	/call setDeclare iceclad_group 																			string		outer "Iceclad Portal"
	
	| misc ports	
	/call setDeclare bind 																							string		outer "Translocate"
	/call setDeclare bind_group																					string		outer "Teleport"
	/call setDeclare primary 																						string		outer "Translocate: Primary Anchor"
	/call setDeclare primary_group 																			string		outer "Primary Anchor Portal"
	/call setDeclare secondary 																					string		outer "Translocate: Secondary Anchor"
	/call setDeclare secondary_group																		string		outer "Secondary Anchor Portal"
	/call setDeclare pushprimary 																				string		outer "Primary Anchor Push"
	/call setDeclare pushprimary_group 																	string		outer "Teleport Primary Anchor"
	/call setDeclare pushsecondary 																			string		outer "Secondary Anchor Push"
	/call setDeclare pushsecondary_group																string		outer "Teleport Secondary Anchor"
	
	/call sql_control FALSE ${chrID} runeMagiWard	 													string		outer "Doomscale Husk"
	/call sql_control FALSE ${chrID} runeShieldofFate												string		outer "Shield of Consequence"
	/call sql_control FALSE ${chrID} runeCrystalwing													string		outer "Armor of the Codex"
	/call sql_control FALSE ${chrID} runeGuard			 													string		outer "Darkmist Guard"

	/call sql_control FALSE ${chrID} WIZPoint 																int 			outer 80
	/call sql_control FALSE ${chrID} swModRod																bool	 		outer TRUE
/return



|***
 *	DES: old INI migration
 *	USE: how about you dont
 *	INI: fuck INIs
 *	NOTE: this is not for you. dont mess with this shit..
 ***|
sub ini_migration(string _type, string _verbage)
	SDEBUG ini_migration(_type:${_type} _verbage:${_verbage})
	/declare _count int local 0
	/declare _countZone int local 0
	/declare _countArray int local 0
	/declare _countField int local 0  
	/declare _tmpShortName string local
	/declare _tmpZone string local
	/declare _tmpField string local
	/declare _tmpValue string local	


	| migrate core.zone.ini	
	/if (${_verbage.Equal[zone]}) {
		VOUT Migrating Zone INI File ${sep} ${cgood}Start\ax
		/declare _fieldList	string local |SafeZone|NameList|ExcludeList|NoMezzList|RangeList|NoCastList|NoMeleeList|HuntMobList|CharmMobList|NoFireList|NoPoisonList|NoMagicList|NoCorruptionList|NoColdList|NoDiseaseList|HarvestList

		| get the fat assed list
		/call generate_zone_short_names

		| loop array
		/for _countArray 1 to ${lstZoneShortName.Size}
			| /echo _countArray:${_countArray}

			| lopop the array
			/for _countZone 1 to ${lstZoneShortName[${_countArray}].Count[|]}

				| get the zone ShortName 
				/varset _tmpZone ${lstZoneShortName[${_countArray}].Arg[${_countZone},|]}
				| /echo _tmpZone:${_tmpZone}

				| get the field names for each shortname
				/for _countField 1 to ${_fieldList.Count[|]}
					| /if (${ZoneFileList.Equal[ShortName]}) /continue 
						/varset _tmpField ${_fieldList.Arg[${_countField},|]}
						/varset _tmpValue ${Ini[tc\core.zone.ini,"${_tmpZone}","${_tmpField}"]}
						/if (${_tmpValue.Equal[NULL]}) /continue 
						VOUT \aw${_tmpZone}\ax::\aw${_tmpField}\ax::\ag${_tmpField}\at
						/sqlite ${dbCore} dummy UPDATE zone SET ${_tmpField}="${_tmpValue}" WHERE ShortName="${_tmpZone}"
				
				/next _countField
			/next _countZone
		/next _countArray
		VOUT Migrating Zone INI File ${sep} ${cgood}End\ax
		
		| delete the fat assed zone list
		/deletevar lstZoneShortName			

	| migrate core.server.CLS.Name.INI
	} else /if (${_verbage.Equal[character]}) {
		/declare _tmpfile tc\core.${MacroQuest.Server}.${Me.Class.ShortName}.${Me.DisplayName}.ini
		
		| import unity buffs
		VOUT Updating \awbuff\ax::\awBuffAAUnity[${cnum}#\ax]\ax::\ag${${Me.Class.ShortName}AAUnity}\ax
		/for _count 1 to 7
			/call sql_update FALSE ${chrID} BuffAAUnity${_count} "${Ini[${_tmpfile},Buffs,BuffAAUnity${_count}]}"
		/next _count
		
		| buff gem
		VOUT Updating \awbuff\ax::\awbuffGem\ax::\ag${Ini[${_tmpfile},Spells,buffGem]}\ax
		/call sql_update FALSE ${chrID} buffGem "${Ini[${_tmpfile},Spells,buffGem]}"
		
		| import 10 buffs	
		/for _count 1 to 10
			VOUT Updating \awbuff\ax::\awBuff[${cnum}${_count}\ax]\ax::\ag${Ini[${_tmpfile},Buffs,Buff${_count}]}\ax
			/call sql_update FALSE ${chrID} Buff${_count} "${Ini[${_tmpfile},Buffs,Buff${_count}]}"
			/call sql_update FALSE ${chrID} Buff${_count}self "${Ini[${_tmpfile},Buffs,Buff${_count}self]}"
			/call sql_update FALSE ${chrID} Buff${_count}selfCondition "${Ini[${_tmpfile},Buffs,Buff${_count}selfCondition]}"
			/call sql_update FALSE ${chrID} Buff${_count}Alias "${Ini[${_tmpfile},Buffs,Buff${_count}Alias]}"
			/call sql_update FALSE ${chrID} BCBuff${_count}Class "${Ini[${_tmpfile},Buffs,BCBuff${_count}Class]}"
			/call sql_update FALSE ${chrID} RaidBuff${_count}Class "${Ini[${_tmpfile},Buffs,RaidBuff${_count}Class]}"
			/call sql_update FALSE ${chrID} ManualBuff${_count}Class "${Ini[${_tmpfile},Buffs,ManualBuff${_count}Class]}"
			/call sql_update FALSE ${chrID} Buff${_count}noBCinRaid "${Ini[${_tmpfile},Buffs,Buff${_count}noBCinRaid]}"
		/next _count


		/if (!${Select[${Me.Class.ShortName},ROG,WAR,MON]}) {

			| GOM nukes
			VOUT Updating \aw${chrID}\ax::\awGoM\ax::\ag${Ini[${_tmpfile},Spells,GoMNuke]}\ax
			/call sql_update FALSE ${chrID} GoMNuke "${Ini[${_tmpfile},Spells,GoMNuke]}"
			/call sql_update FALSE ${chrID} GoMNukeCondition "${Ini[${_tmpfile},Spells,GoMNukeCondition]}"
			VOUT Updating \aw${Me.Class.ShortName}\ax::\awGoM2\ax::\ag${Ini[${_tmpfile},Spells,GoMNuke2]}\ax
			/call sql_update FALSE ${chrID} GoMNuke2 "${Ini[${_tmpfile},Spells,GoMNuke2]}"
			/call sql_update FALSE ${chrID} GoMNuke2Condition "${Ini[${_tmpfile},Spells,GoMNuke2Condition]}"


			| Nukes
			/if (${Select[${Me.Class.ShortName},WIZ,MAG]}) {

				| Wizard lines
				/if (${Select[${Me.Class.ShortName},WIZ]}) {
					/call sql_update FALSE ${chrID} lineCurrent "${Ini[${_tmpfile},Spells,lineCurrent]}"
					/call sql_update FALSE ${chrID} lineFire "${Ini[${_tmpfile},Spells,lineFire]}"
					/call sql_update FALSE ${chrID} lineIce "${Ini[${_tmpfile},Spells,lineIce]}"
					/call sql_update FALSE ${chrID} lineMagic	"${Ini[${_tmpfile},Spells,lineMagic]}"
					/call sql_update FALSE ${chrID} lineMix "${Ini[${_tmpfile},Spells,lineMix]}"
					
					/call sql_update FALSE ${chrID} lineAAFire "${Ini[${_tmpfile},Spells,lineAAFire]}"
					/call sql_update FALSE ${chrID} lineAAIce "${Ini[${_tmpfile},Spells,lineAAIce]}"
					/call sql_update FALSE ${chrID} lineAAMagic "${Ini[${_tmpfile},Spells,lineAAMagic]}"
					/call sql_update FALSE ${chrID} lineAAMix "${Ini[${_tmpfile},Spells,lineAAMix]}"
					
					/call sql_update FALSE ${chrID} switchForceTwincast "${Ini[${_tmpfile},Spells,switchForceTwincast]}"
					/call sql_update FALSE ${chrID} NukeTwincastFire "${Ini[${_tmpfile},Spells,NukeTwincastFire]}"
					/call sql_update FALSE ${chrID} NukeTwincastIce "${Ini[${_tmpfile},Spells,NukeTwincastIce]}"
					/call sql_update FALSE ${chrID} NukeTwincastMagic "${Ini[${_tmpfile},Spells,NukeTwincastMagic]}"
					/call sql_update FALSE ${chrID} NukeTwincastMix "${Ini[${_tmpfile},Spells,NukeTwincastMix]}"
					/call sql_update FALSE ${chrID} NukeTwincastCondition "${Ini[${_tmpfile},Spells,NukeTwincastCondition]}"
				}

				} else {
					/for _count 1 to 8		
						VOUT Updating \aw${chrID}\ax::\awNuke[${cnum}${_count}\ax]\ax::\ag${Ini[${_tmpfile},Spells,Nuke${_count}]}\ax
						/call sql_update FALSE ${chrID} Nuke${_count} "${Ini[${_tmpfile},Spells,Nuke${_count}]}"
						/call sql_update FALSE ${chrID} Nuke${_count}Condition "${Ini[${_tmpfile},Spells,Nuke${_count}Condition]}"
					/next _count
				}
				
				| DoTs
				/if (!${Select[${Me.Class.ShortName},WIZ,WAR,BER,MON,ROG,MAG]}) {
					/for _count 1 to 8		
						VOUT Updating \aw${chrID}\ax::\awDoT[${cnum}${_count}\ax]\ax::\ag${Ini[${_tmpfile},Spells,DoT${_count}]}\ax
						/call sql_update FALSE ${chrID} DoT${_count} "${Ini[${_tmpfile},Spells,DoT${_count}]}"
						/call sql_update FALSE ${chrID} DoT${_count}chk "${Ini[${_tmpfile},Spells,DoT${_count}chk]}"
						/call sql_update FALSE ${chrID} DoT${_count}Condition "${Ini[${_tmpfile},Spells,DoT${_count}Condition]}"
					/next _count
				}
			}

			| AANow
			/for _count 1 to 2		
				VOUT Updating \aw${chrID}\ax::\awAANow[${cnum}${_count}\ax]\ax::\ag${Ini[${_tmpfile},Spells,AANow${_count}]}\ax
				/call sql_update FALSE ${chrID} AANow${_count} "${Ini[${_tmpfile},Spells,AANow${_count}]}"
			/next _count

			| DiscNow
			/for _count 1 to 2		
				VOUT Updating \aw${Me.Class.ShortName}\ax::\awDiscNow[${cnum}${_count}\ax]\ax::\ag${Ini[${_tmpfile},Spells,DiscNow${_count}]}\ax
				/call sql_update FALSE ${chrID} DiscNow${_count} "${Ini[${_tmpfile},Spells,DiscNow${_count}]}"
			/next _count

			| Auras
			/for _count 1 to 2
				VOUT Updating \awbuff\ax::\awAura[${cnum}${_count}\ax]\ax::\ag${Ini[${_tmpfile},Aura,Aura${_count}Spell]}\ax
				/call sql_update FALSE ${chrID} Aura${_count}Spell "${Ini[${_tmpfile},Aura,Aura${_count}Spell]}"
				/call sql_update FALSE ${chrID} Aura${_count}Buff "${Ini[${_tmpfile},Aura,Aura${_count}Buff]}"
			/next _count
						
			| clickItem
			/for _count 1 to 6
				VOUT Updating \awitem\ax::\awclickItem[${cnum}${_count}\ax]\ax::\ag${Ini[${_tmpfile},Items,clickItem${_count}]}\ax
				/call sql_update FALSE ${chrID} clickItem${_count} "${Ini[${_tmpfile},Items,clickItem${_count}]}"
			/next _count			

			| clickItemBurn
			/for _count 1 to 6
				VOUT Updating \awitem\ax::\awclickItemBurn[${cnum}${_count}\ax]\ax::\ag${Ini[${_tmpfile},Items,clickItemBurn${_count}]}\ax
				/call sql_update FALSE ${chrID} clickItemBurn${_count} "${Ini[${_tmpfile},Items,clickItemBurn${_count}]}"
			/next _count			

			| Breastplate
			/for _count 1 to 2
				VOUT Updating \awitem\ax::\awBreastplate[${cnum}${_count}\ax]\ax::\ag${Ini[${_tmpfile},Items,Breastplate${_count}]}\ax
				/call sql_update FALSE ${chrID} Breastplate${_count} "${Ini[${_tmpfile},Items,Breastplate${_count}]}"
				/call sql_update FALSE ${chrID} Breastplate${_count}Condition "${Ini[${_tmpfile},Items,Breastplate${_count}Condition]}"
				/call sql_update FALSE ${chrID} Breastplate${_count}effect "${Ini[${_tmpfile},Items,Breastplate${_count}effect]}"
			/next _count			

		
			| clickItemSwarm
			/for _count 1 to 4
				VOUT Updating \awitem\ax::\awclickItemSwarm[${cnum}${_count}\ax]\ax::\ag${Ini[${_tmpfile},Items,clickItemSwarm${_count}]}\ax
				/call sql_update FALSE ${chrID} clickItemSwarm${_count} "${Ini[${_tmpfile},Items,clickItemSwarm${_count}]}"
			/next _count			

			| clickOddItem
			/for _count 1 to 4
				VOUT Updating \awitem\ax::\awclickOddItem[${cnum}${_count}\ax]\ax::\ag${Ini[${_tmpfile},Items,clickOddItem${_count}]}\ax
				/call sql_update FALSE ${chrID} clickOddItem${_count} "${Ini[${_tmpfile},Items,clickOddItem${_count}]}"
			/next _count			

			| circle of ???
			VOUT Updating \awitem\ax::\awCircle\ax::\ag${Ini[${_tmpfile},Items,useCircle]}\ax
			/call sql_update FALSE ${chrID} useCircle "${Ini[${_tmpfile},Items,useCircle]}"
			/call sql_update FALSE ${chrID} CircleofPower "${Ini[${_tmpfile},Items,CircleofPower]}"
			/call sql_update FALSE ${chrID} CircleofLife "${Ini[${_tmpfile},Items,CircleofLife]}"
			/call sql_update FALSE ${chrID} CircleofMana "${Ini[${_tmpfile},Items,CircleofMana]}"


			| familiars / pets
			/for _count 1 to 3
				VOUT Updating \awpet\ax::\awsetFam[${cnum}${_count}\ax]\ax::\ag${Ini[${_tmpfile},Pet,sumFamiliar${_count}]}\ax
				/call sql_update FALSE ${chrID} setFam${_count} "${Ini[${_tmpfile},Pet,sumFamiliar${_count}]}"
				/call sql_update FALSE ${chrID} setFam${_count}Buff "${Ini[${_tmpfile},Pet,sumFamiliar${_count}Buff]}"
				/call sql_update FALSE ${chrID} swFam${_count}Leave "${Ini[${_tmpfile},Pet,sumFamiliar${_count}Leave]}"
			/next _count


			| range studff
			VOUT Updating \awcharacter\ax::\awlstRange\ax::\agRange Pulling things\ax
			/call sql_update FALSE ${chrID} lstRangeItem "${Ini[${_tmpfile},Common,RangeItem]}"
			/call sql_update FALSE ${chrID} lstRangeItemSummon "${Ini[${_tmpfile},Common,RangeItemSummon]}"
			/call sql_update FALSE ${chrID} strRangeItemCount "${Ini[${_tmpfile},Common,RangeItemCount]}"
			/call sql_update FALSE ${chrID} strRangeSpell "${Ini[${_tmpfile},Common,RangeSpell]}"
			/call sql_update FALSE ${chrID} setRangeClickItem "${Ini[${_tmpfile},Common,RangeClickItem]}"
			/call sql_update FALSE ${chrID} setRangeDisc "${Ini[${_tmpfile},Common,RangeDisc]}"


			| shrink
			VOUT Updating \awcharacter\ax::\awsetShrinkItem\ax::\ag${Ini[${_tmpfile},Common,ShrinkItem]}\ax
			/call sql_update FALSE ${chrID} setShrinkItem "${Ini[${_tmpfile},Common,ShrinkItem]}"


			| pet shit
			VOUT Updating \awpet\ax::\awstuff\ax::\agpet shit\ax
			/if (${Me.Class.PetClass} || ${Select[${Me.Class.ShortName},ENC,DRU,SHM,SHD,NEC]}) {
				| /call sql_update FALSE ${chrID} swPetTank "${Ini[${_tmpfile},Pet,]}"
				/call sql_update FALSE ${chrID} swMAGWep "${Ini[${_tmpfile},Pet,]}"
				/call sql_update FALSE ${chrID} setPetEpicClick "${Ini[${_tmpfile},Pet,EpicClick]}"
				/call sql_update FALSE ${chrID} setPetEpicBuff "${Ini[${_tmpfile},Pet,EpicPetBuff]}"
				/call sql_update FALSE ${chrID} setMAGWepSpell "${Ini[${_tmpfile},Pet,SummonedWepSpell]}"
				/call sql_update FALSE ${chrID} setMAGClosedBag "${Ini[${_tmpfile},Pet,SummonedWepClosedBag]}"
				/call sql_update FALSE ${chrID} setMAGPetWepName "${Ini[${_tmpfile},Pet,SummonedWepName]}"
				/call sql_update FALSE ${chrID} setPetShrink "${Ini[${_tmpfile},Pet,ShrinkPet]}"
				/call sql_update FALSE ${chrID} setPetHealSpell "${Ini[${_tmpfile},Pet,PetHealSpell]}"
				/call sql_update FALSE ${chrID} setPetRenewalSpell "${Ini[${_tmpfile},Pet,PetRenewalSpell]}"
				/call sql_update FALSE ${chrID} setPetIllusion "${Ini[${_tmpfile},Pet,PetIllusion]}"
				/call sql_update FALSE ${chrID} setPetIllusionBuff "${Ini[${_tmpfile},Pet,PetIllusionBuff]}"
				/call sql_update FALSE ${chrID} setPetAAHeal "${Ini[${_tmpfile},Pet,PetAAHeal]}"
				/call sql_update FALSE ${chrID} setPetBagSlot "${Ini[${_tmpfile},Pet,PetBagSlotNumber]}"
				                                                            			
				| 10 pet buffs
				/for _count 1 to 10
					/call sql_update FALSE ${chrID} PetBuff${_count} "${Ini[${_tmpfile},Pet,PetBuff${_count}]}"
					/call sql_update FALSE ${chrID} PetBuff${_count}chk "${Ini[${_tmpfile},Pet,PetBuff${_count}chk]}"
				/next _count
			}

			| healers heal points.. fuck all the other classes
			/if (${Select[${Me.Class.ShortName},CLR,DRU,SHM,PAL]}) {
				VOUT Updating \aw${Me.Class.ShortName}\ax::\aw${Me.Class.ShortName}Point\ax::\ag##'s\ax
				/call sql_update FALSE ${chrID} PetPoint "${Ini[${_tmpfile},Heal,PetPoint]}"
				/call sql_update FALSE ${chrID} CLRPoint "${Ini[${_tmpfile},Heal,CLRPoint]}"
				/call sql_update FALSE ${chrID} DRUPoint "${Ini[${_tmpfile},Heal,DRUPoint]}"
				/call sql_update FALSE ${chrID} SHMPoint "${Ini[${_tmpfile},Heal,SHMPoint]}"
				/call sql_update FALSE ${chrID} WIZPoint "${Ini[${_tmpfile},Heal,WIZPoint]}"
				/call sql_update FALSE ${chrID} MAGPoint "${Ini[${_tmpfile},Heal,MAGPoint]}"
				/call sql_update FALSE ${chrID} ENCPoint "${Ini[${_tmpfile},Heal,ENCPoint]}"
				/call sql_update FALSE ${chrID} NECPoint "${Ini[${_tmpfile},Heal,NECPoint]}"
				/call sql_update FALSE ${chrID} WARPoint "${Ini[${_tmpfile},Heal,WARPoint]}"
				/call sql_update FALSE ${chrID} MNKPoint "${Ini[${_tmpfile},Heal,MNKPoint]}"
				/call sql_update FALSE ${chrID} ROGPoint "${Ini[${_tmpfile},Heal,ROGPoint]}"
				/call sql_update FALSE ${chrID} RNGPoint "${Ini[${_tmpfile},Heal,RNGPoint]}"
				/call sql_update FALSE ${chrID} BSTPoint "${Ini[${_tmpfile},Heal,BSTPoint]}"
				/call sql_update FALSE ${chrID} SHDPoint "${Ini[${_tmpfile},Heal,SHDPoint]}"
				/call sql_update FALSE ${chrID} BRDPoint "${Ini[${_tmpfile},Heal,BRDPoint]}"
				/call sql_update FALSE ${chrID} BERPoint "${Ini[${_tmpfile},Heal,BERPoint]}"
				/call sql_update FALSE ${chrID} PALPoint "${Ini[${_tmpfile},Heal,PALPoint]}"
				/call sql_update FALSE ${chrID} GoMpoint "${Ini[${_tmpfile},Heal,GoMpoint]}"
				/call sql_update FALSE ${chrID} XHealClass "${Ini[${_tmpfile},Heal,XHealClass]}"
				| /call sql_update FALSE ${chrID} setToonControlXTslot "${Ini[${_tmpfile},Heal,ToonControlXTslot]}"
			}

			| class specific
			VOUT Updating \aw${Me.Class.ShortName}\ax::\agVARIOUS\ax
			
			| Wizard
			/if (${Select[${Me.Class.ShortName},WIZ]}) {
				/call sql_update FALSE ${chrID} runeMagiWard "${Ini[${_tmpfile},Rune,runeMagiWard]}"
				/call sql_update FALSE ${chrID} runeShieldofFate "${Ini[${_tmpfile},Rune,runeShieldofFate]}"
				/call sql_update FALSE ${chrID} runeCrystalwing "${Ini[${_tmpfile},Rune,runeCrystalwing]}"
				/call sql_update FALSE ${chrID} runeGuard "${Ini[${_tmpfile},Rune,runeGuard]}"
			}
			
			| Bard
			/if (${Select[${Me.Class.ShortName},BRD]}) {
				/call sql_update FALSE ${chrID} lineTank "${Ini[${_tmpfile},Song,lineTank]}"
				/call sql_update FALSE ${chrID} lineCaster "${Ini[${_tmpfile},Song,lineCaster]}"
				/call sql_update FALSE ${chrID} lineMelee "${Ini[${_tmpfile},Song,lineMelee]}"
				/call sql_update FALSE ${chrID} lineMix "${Ini[${_tmpfile},Song,lineMix]}"
				/call sql_update FALSE ${chrID} lineOther "${Ini[${_tmpfile},Song,lineOther]}"
			}


			| ROGUE
			/if (${Select[${Me.Class.ShortName},ROG]}) {
				/call sql_update FALSE ${chrID} clickPoisonBuff "${Ini[${_tmpfile},Poison,clickPoisonBuff]}"
				/call sql_update FALSE ${chrID} clickPoisonItem "${Ini[${_tmpfile},Poison,clickPoisonItem]}"
				/call sql_update FALSE ${chrID} clickPoisonDispenser "${Ini[${_tmpfile},Poison,clickPoisonDispenser]}"
				/call sql_update FALSE ${chrID} clickPoisonCount "${Ini[${_tmpfile},Poison,clickPoisonCount]}"
			}
			
			
			| BEASTLORD
			/if (${Select[${Me.Class.ShortName},BST]}) {
				/call sql_update FALSE ${chrID} GrowlSpell "${Ini[${_tmpfile},Buffs,GrowlSpell]}"
			}
		}

/return



|***
 *	DES: HUD pregeneration
 *	USE: 
 *	NOTE:
 ***|
sub hud_control(string _verbage)
	VOUT Updating MQ2HUD.INI ${sep} ${cinfo}${_verbage.Left[1].Upper}${_verbage.Right[-1].Lower}\ax
	/delay 5

	| main HUD for Core
	/if (${_verbage.Equal[core]}) {
		| Build
		HUDPATH "Core" "core1" "3,5,26,100,149,237,Build"
		HUDPATH "Core" "core2" "19,44,26,255,255,255,${currentBuild}"

		| control toon
		| HUDPATH "Core" "control1" "19,5,50,255,255,255,${If[${Bool[${autoControl}]},,Control]}"
		| HUDPATH "Core" "control1" "19,5,50,255,255,255,Control"
		| HUDPATH "Core" "control2" "19,5,50,0,255,0,${If[${Bool[${autoControl}]},Control,]}"
		| HUDPATH "Core" "control3" "19,44,50,100,149,237,${If[${Bool[${setToonControl}]},${setToonControl},]}"

		| loc Y X Z
		HUDPATH "Core" "loc1" "3,5,62,100,149,237,LOC"
		HUDPATH "Core" "loc2" "3,44,62,255,255,255,${Int[${Me.Y}]}, ${Int[${Me.X}]}, ${Int[${Me.Z}]}"

		| mode
		HUDPATH "Core" "mode0" "19,5,38,0,255,0,${If[${swAutoCursor},Mode,]}"
		HUDPATH "Core" "mode1" "19,5,38,255,0,0,${If[!${swAutoCursor},Mode,]}"	
		HUDPATH "Core" "mode2" "19,44,38,255,255,0,${If[${Bool[${tmpMode}]},${tmpMode},]}"
		
		| HUDPATH "Core" "mode3" "19,44,74,25,25,25,${If[${Bool[${tmpMode}]},,]}"

		| Target / LOS
		| HUDPATH "Core" "bodytype1" "3,5,85,100,149,237,[                       ]"
		HUDPATH "Core" "bodytype1" "3,5,86,255,255,255,${Target.Body}"
		HUDPATH "Core" "LoS1" "3,85,86,0,255,0,${If[${Bool[${Target}]},${If[${Target.LineOfSight},${Target.Distance} .. ID:${Target.ID},]},]}"
		HUDPATH "Core" "LoS2" "3,85,86,255,0,0,${If[${Bool[${Target}]},${If[${Target.LineOfSight},,${Target.Distance} .. ID:${Target.ID}]},]}"

		| current target info
		HUDPATH "Core" "target1" "3,70,97,100,149,237,${If[${Target.ID},[${Target.Class.ShortName} ${Target.Level}] ${Target.Name} ${If[${Bool[${Target.Guild}]},<${Target.Guild}>, ]}, ]}"
		
		| engage ##
		HUDPATH "Core" "engage1" "3,5,97,100,149,237,[     ][     ][     ]"

		| assist ##
		HUDPATH "Core" "engage2" "19,10,98,255,255,255,${If[(!${swOverrideEngagePct} && !${swAssist1} && !${swAssist2} && !${swAssist3} && !${swAssistG} && (${Bool[${setMA1}]} || ${Bool[${setMA2}]}),${setEngageMAPct},]}"
		HUDPATH "Core" "engage3" "19,10,98,255,255,255,${If[!${swOverrideEngagePct} && (${swAssist1} || ${swAssist2} || ${swAssist3} || ${swAssistG}),${setEngagePct},]}"
		HUDPATH "Core" "engage4" "19,10,98,25,25,25,${If[!${swOverrideEngagePct} && !${swAssist1} && !${swAssist2} && !${swAssist3} && !${swAssistG} && !${Bool[${setMA1}]} && !${Bool[${setMA2}]},##,]}"
		HUDPATH "Core" "engage7" "19,12,98,255,0,0,${If[(${swOverrideEngagePct},X,]}"

		| pet engage ##
		HUDPATH "Core" "pettarget1" "19,31,98,255,255,255,${If[!${swOverrideEngagePct} && ${swPet},${setPetEngagePct},]}"
		HUDPATH "Core" "pettarget2" "19,31,98,25,25,25,${If[!${swOverrideEngagePct} && !${swPet},##,]}"
		HUDPATH "Core" "pettarget3" "19,33,98,255,0,0,${If[${swOverrideEngagePct},X,]}"

		| swarmpet engage ##
		HUDPATH "Core" "pettarget4" "19,52,98,255,255,255,${If[!${swOverrideEngagePct} && ${swSwarm},${setSwarmEngagePct},]}"
		HUDPATH "Core" "pettarget5" "19,52,98,25,25,25,${If[(!${swOverrideEngagePct}) && !${swSwarm},##,]}"
		HUDPATH "Core" "pettarget6" "19,55,98,255,0,0,${If[${swOverrideEngagePct},X,]}"

		| target effect notice
		HUDPATH "Core" "tarnotice1" "19,5,110,255,255,255,${hudTarNotice}"
		
		| watchspawn counter
		HUDPATH "Core" "watchspawn1" "3,359,109,100,149,237,(     )(      )"
		HUDPATH "Core" "watchspawn2" "19,365,110,255,255,255,${SpawnCount[npc radius ${setMobAgro} targetable playerstate 4]}"
		HUDPATH "Core" "watchspawn3" "19,387,110,0,255,0,${If[${Bool[${watchSpawn}]},${hudwatchspawn},]}"
		HUDPATH "Core" "watchspawn4" "19,386,105,25,25,25,${If[${Bool[${watchSpawn}]},,___]}"

		| combat modes
		HUDPATH "Core" "botA1" "19,140,14,0,255,0,${If[${coreAuto},A,]}"
		HUDPATH "Core" "botA2" "19,140,9,25,25,25,${If[${coreAuto},,_]}"
		HUDPATH "Core" "botM1" "19,149,14,0,255,0,${If[${swCombatMelee},M,]}"
		HUDPATH "Core" "botM2" "19,149,9,25,25,25,${If[${swCombatMelee},,_]}"
		HUDPATH "Core" "botR1" "19,158,14,0,255,0,${If[${swCombatRange},R,]}"
		HUDPATH "Core" "botR2" "19,158,9,25,25,25,${If[${swCombatRange},,_]}"
		HUDPATH "Core" "botN1" "19,167,14,0,255,0,${If[${swCombatNuke},N,]}"
		HUDPATH "Core" "botN2" "19,167,9,25,25,25,${If[${swCombatNuke},,_]}"
		HUDPATH "Core" "botD1" "19,176,14,0,255,0,${If[${swCombatDoT},D,]}"
		HUDPATH "Core" "botD2" "19,176,9,25,25,25,${If[${swCombatDoT},,_]}"
		HUDPATH "Core" "botH1" "19,185,14,0,255,0,${If[${swCombatHeal},H,]}"
		HUDPATH "Core" "botH2" "19,185,9,25,25,25,${If[${swCombatHeal},,_]}"

		
		HUDPATH "Core" "agro1" "19,209,14,0,255,0,${If[${swAgro},A,]}"
		HUDPATH "Core" "agro2" "19,209,9,25,25,25,${If[${swAgro},,_]}"
	
		HUDPATH "Core" "pull1" "19,218,14,0,255,0,${If[${swPull},P,]}"
		HUDPATH "Core" "pull2" "19,218,9,25,25,25,${If[${swPull},,_]}"	
	
		HUDPATH "Core" "ot1" "19,227,14,0,255,0,${If[${swOffTank},O,]}"	
		HUDPATH "Core" "ot2" "19,227,9,25,25,25,${If[${swOffTank},,_]}"		
		
		HUDPATH "Core" "agro4" "19,237,14,255,255,255,${setMobAgro}"
	
		
		

		HUDPATH "Core" "bracket1" "3,197,25,100,149,237,[    ]"

		| combat distance
		HUDPATH "Core" "CD1" "19,200,26,255,255,255,${setCombatDistance}"
		
		| stick behind
		HUDPATH "Core" "stickback1" "19,218,26,0,255,0,${If[${swStickBehind},B,]}"
		HUDPATH "Core" "stickback2" "19,218,21,25,25,25,${If[${swStickBehind},,_]}"
		
		| face fast
		HUDPATH "Core" "facefast1" "19,227,26,0,255,0,${If[${swFaceFast},F,]}"
		HUDPATH "Core" "facefast2" "19,227,21,25,25,25,${If[${swFaceFast},,_]}"

		| stuff for camp status
		HUDPATH "Core" "camp1" "3,140,26,100,149,237,R        L"
		HUDPATH "Core" "camp3" "19,151,26,255,255,255,${If[${swHomeSet},${setHomeRadius},]}"
		HUDPATH "Core" "camp4" "19,180,26,255,255,255,${If[${swHomeSet},${setHomeLeash},]}"
		HUDPATH "Core" "camp5" "19,150,26,25,25,25,${If[${swHomeSet},,##      ##]}"

		| tie info
		HUDPATH "Core" "tie1" "19,140,38,100,149,237,${setTieMode}"
		HUDPATH "Core" "tie3" "19,167,38,255,255,255,${If[${swSetTie},${setTieDistance},]}"
		HUDPATH "Core" "tie6" "19,167,38,25,25,25,${If[${swSetTie},,##]}"
		HUDPATH "Core" "tie7" "19,190,38,0,255,0,${If[${Bool[${setTieToon}]},${If[${Bool[${Spawn[${setTieToon}]}]},${setTieToon},]},]}"
		HUDPATH "Core" "tie8" "19,190,38,255,0,0,${If[${Bool[${setTieToon}]},${If[!${Bool[${Spawn[${setTieToon}]}]},${If[${Bool[${setTieToon}]},${setTieToon},]},]},]}"
		HUDPATH "Core" "tie9" "19,190,33,25,25,25,${If[${Bool[${setTieToon}]},,____]}"

		| burn a thing
		HUDPATH "Core" "burn1" "19,140,50,0,255,0,${If[${swBurnAuto},Burn,]}"
		HUDPATH "Core" "burn2" "19,140,50,255,0,0,${If[${swBurnAuto},,Burn]}"
		HUDPATH "Core" "burncount1" "19,166,50,255,255,255,${If[!${swBurnForce} && ${setBurnCount},${setBurnCount},]}"
		HUDPATH "Core" "burncount2" "19,166,45,255,0,0,${If[${swBurnForce},__,]}"
		HUDPATH "Core" "burnforce1" "3,183,49,100,149,237,[      ]"
		HUDPATH "Core" "burnforce2" "19,189,50,255,255,255,${If[!${swBurnForce} && ${setBurnPct},${setBurnPct},]}"
		HUDPATH "Core" "burnforce3" "19,189,45,255,0,0,${If[${swBurnForce},__,]}"
		
		HUDPATH "Core" "burnraid1" "19,210,50,0,255,0,${If[${swBurnRaid},R,]}"
		HUDPATH "Core" "burnraid2" "19,210,45,25,25,25,${If[${swBurnRaid},,_]}"		
			
		| rez
		HUDPATH "Core" "rez1" "19,140,62,0,255,0,${If[${swRezTake},Rez,]}"
		HUDPATH "Core" "rez2" "19,140,62,255,0,0,${If[${swRezTake},,Rez]}"
		HUDPATH "Core" "rezG1" "19,163,62,0,255,0,${If[${swRezGive},G,]}"
		HUDPATH "Core" "rezG2" "19,163,57,25,25,25,${If[${swRezGive},,_]}"
		HUDPATH "Core" "rezC1" "19,172,62,0,255,0,${If[${swRezCombat} && ${swRezGive},C,]}"
		HUDPATH "Core" "rezC2" "19,172,57,25,25,25,${If[(${swRezCombat} && !${swRezGive}) || !${swRezCombat},_,]}"
		HUDPATH "Core" "rezE1" "19,181,62,0,255,0,${If[${swRezEveryone} && ${swRezGive},E,]}"
		HUDPATH "Core" "rezE2" "19,181,57,25,25,25,${If[(${swRezEveryone} && !${swRezGive}) || !${swRezEveryone},_,]}"
		HUDPATH "Core" "rezT1" "19,190,62,0,255,0,${If[${swRezToken} && ${swRezGive},T,]}"
		HUDPATH "Core" "rezT2" "19,190,57,25,25,25,${If[(${swRezToken} && !${swRezGive}) || !${swRezToken},_,]}"
		HUDPATH "Core" "rezRange" "19,201,62,255,255,255,${If[${swRezGive},${setMaxRezRange},]}"

		| HUD WARNING/OVERIDE notices
		HUDPATH "Core" "hudwarning1" "19,135,2,240,0,0,${If[${Bool[${hudWarning}]},${hudWarning},]}"

		| group broadcast stauts
		HUDPATH "Core" "hudbroadcast0" "19,280,2,255,51,153,${setHUDBroadcast}"

		| agro/puler/offtank status
	

		| ass..isting and stuff
		HUDPATH "Core" "setMA11" "3,280,60,100,149,237,MA1"
		HUDPATH "Core" "setMA12" "19,305,60,0,255,0,${If[${Bool[${setMA1}]},${If[${Bool[${Spawn[${setMA1}]}]},${setMA1},]},]}"
		HUDPATH "Core" "setMA13" "19,305,60,255,0,0,${If[${Bool[${setMA1}]},${If[!${Bool[${Spawn[${setMA1}]}]},${If[${Bool[${setMA1}]},${setMA1},]},]},]}"
		HUDPATH "Core" "setMA14" "19,305,55,25,25,25,${If[${Bool[${setMA1}]},,____]}"
	
		HUDPATH "Core" "setMA21" "3,280,72,100,149,237,MA2"
		HUDPATH "Core" "setMA22" "19,305,72,0,255,0,${If[${Bool[${setMA2}]},${If[${Bool[${Spawn[${setMA2}]}]},${setMA2},]},]}"
		HUDPATH "Core" "setMA23" "19,305,72,255,0,0,${If[${Bool[${setMA2}]},${If[!${Bool[${Spawn[${setMA2}]}]},${If[${Bool[${setMA2}]},${setMA2},]},]},]}"
		HUDPATH "Core" "setMA24" "19,305,67,25,25,25,${If[${Bool[${setMA2}]},,____]}"


		HUDPATH "Core" "ass0" "3,280,84,100,149,237,Assist"
		HUDPATH "Core" "ass1" "19,315,84,0,255,0,${If[${useSmartAssistR},Raid,]}"
		HUDPATH "Core" "ass2" "19,315,84,0,255,0,${If[${swAssist1},Raid1,]}"
		HUDPATH "Core" "ass3" "19,315,84,0,255,0,${If[${swAssist2},Raid2,]}"
		HUDPATH "Core" "ass4" "19,315,84,0,255,0,${If[${swAssist3},Raid3,]}"
		HUDPATH "Core" "ass5" "19,315,84,0,255,0,${If[${swAssistG},Group,]}"
		HUDPATH "Core" "ass6" "19,315,84,0,255,0,${If[${swAssistF},Force,]}"
		HUDPATH "Core" "ass7" "19,315,79,25,25,25,${If[!${swAssist1} && !${swAssist2} && !${swAssist3} && !${swAssistG} && !${swAssistF},____,]}"


		| lazy ass controls
		HUDPATH "Core" "rest0" "19,140,74,100,149,237,Rest"
		HUDPATH "Core" "rest3" "19,166,74,255,255,255,${If[${setRestPct},${setRestPct},]}"
		HUDPATH "Core" "rest4" "19,166,74,25,25,25,${If[${setRestPct},,##]}"

		| rest options
		HUDPATH "Core" "rest6" "19,182,74,0,255,0,${If[${swRestFull},F,]}"
		HUDPATH "Core" "rest7" "19,182,69,25,25,25,${If[${swRestFull},,_]}"
		HUDPATH "Core" "rest8" "19,191,74,0,255,0,${If[${swRestModRod},M,]}"
		HUDPATH "Core" "rest9" "19,191,69,25,25,25,${If[${swRestModRod},,_]}"
		HUDPATH "Core" "rest10" "19,200,74,0,255,0,${If[${swRestBuff},B,]}"
		HUDPATH "Core" "rest11" "19,200,69,25,25,25,${If[${swRestBuff},,_]}"



		| buff switches
		HUDPATH "Core" "buff1" "3,280,14,100,149,237,Buff"
		HUDPATH "Core" "buff3" "19,308,14,0,255,0,${If[${swBuffSelf},S,]}"
		HUDPATH "Core" "buff4" "19,308,9,25,25,25,${If[${swBuffSelf},,_]} "
		HUDPATH "Core" "buff5" "19,319,14,0,255,0,${If[${swBuffItem},I,]}"
		HUDPATH "Core" "buff6" "19,317,9,25,25,25,${If[${swBuffItem},,_]} "
		HUDPATH "Core" "buff7" "19,326,14,0,255,0,${If[${swBuffCombat},C,]}"
		HUDPATH "Core" "buff8" "19,326,9,25,25,25,${If[${swBuffCombat},,_]} "
		HUDPATH "Core" "buff9" "19,335,14,0,255,0,${If[${swBuffBC},B,]}"
		HUDPATH "Core" "buff10" "19,335,9,25,25,25,${If[${swBuffBC},,_]}"
		HUDPATH "Core" "buff11" "19,344,14,0,255,0,${If[${swBuffRaid},R,]}"
		HUDPATH "Core" "buff12" "19,344,9,25,25,25,${If[${swBuffRaid},,_]}"
		HUDPATH "Core" "buff13" "19,353,14,0,255,0,${If[${swBuffTell},T,]}"
		HUDPATH "Core" "buff14" "19,353,9,25,25,25,${If[${swBuffTell},,_]}"
		HUDPATH "Core" "buff15" "19,362,14,0,255,0,${If[${swBuffPet},P,]}"
		HUDPATH "Core" "buff16" "19,362,9,25,25,25,${If[${swBuffPet},,_]}"
		HUDPATH "Core" "buff17" "19,371,14,0,255,0,${If[${swBuffMerc},M,]}"
		HUDPATH "Core" "buff18" "19,371,9,25,25,25,${If[${swBuffMerc},,_]}"
		HUDPATH "Core" "buff19" "19,380,14,0,255,0,${If[${swBuffOther},O,]}"
		HUDPATH "Core" "buff20" "19,380,9,25,25,25,${If[${swBuffOther},,_]}"
		HUDPATH "Core" "buff21" "19,389,14,0,255,0,${If[${swBuffMGB},X,]}"
		HUDPATH "Core" "buff22" "19,389,9,25,25,25,${If[${swBuffMGB},,_]}"

		| deBuff switches
		HUDPATH "Core" "debuff1" "3,280,26,100,149,237,deBuff"
		HUDPATH "Core" "debuff3" "19,321,26,0,255,0,${If[${swdeBuffMezz},Z,]}"
		HUDPATH "Core" "debuff4" "19,321,21,25,25,25,${If[${swdeBuffMezz},,_]}"	
		HUDPATH "Core" "debuff5" "19,330,26,0,255,0,${If[${swdeBuffMalo},M,]}"
		HUDPATH "Core" "debuff6" "19,330,21,25,25,25,${If[${swdeBuffMalo},,_]}"	
		HUDPATH "Core" "debuff7" "19,339,26,0,255,0,${If[${swdeBuffTash},T,]}"
		HUDPATH "Core" "debuff8" "19,339,21,25,25,25,${If[${swdeBuffTash},,_]}"	
		HUDPATH "Core" "debuff9" "19,348,26,0,255,0,${If[${swdeBuffCripple},C,]}"
		HUDPATH "Core" "debuff10" "19,348,21,25,25,25,${If[${swdeBuffCripple},,_]}"	
		HUDPATH "Core" "debuff11" "19,357,26,0,255,0,${If[${swdeBuffSlow},S,]}"
		HUDPATH "Core" "debuff12" "19,357,21,25,25,25,${If[${swdeBuffSlow},,_]}"		
		HUDPATH "Core" "debuff13" "19,366,26,0,255,0,${If[${swdeBuffEradicate},E,]}"
		HUDPATH "Core" "debuff14" "19,366,21,25,25,25,${If[${swdeBuffEradicate},,_]}"		
		HUDPATH "Core" "debuff15" "19,375,26,0,255,0,${If[${swdeBuffSnare},R,]}"		
		HUDPATH "Core" "debuff16" "19,375,21,25,25,25,${If[${swdeBuffSnare},,_]}"		
		
		| heal switches
		HUDPATH "Core" "heal3" "19,305,38,255,255,255,${If[${setHealOverride},${setHealOverride},]}"
		HUDPATH "Core" "heal4" "19,305,38,25,25,25,${If[${setHealOverride},,##]}"
		HUDPATH "Core" "heal5" "19,321,38,0,255,0,${If[${swHealSelf},S,]}"
		HUDPATH "Core" "heal6" "19,321,33,25,25,25,${If[${swHealSelf},,_]}"
		HUDPATH "Core" "heal7" "19,330,38,0,255,0,${If[${swHealGroup},G,]}"
		HUDPATH "Core" "heal8" "19,330,33,25,25,25,${If[${swHealGroup},,_]}"
		HUDPATH "Core" "heal9" "19,339,38,0,255,0,${If[${swHealXTarget},X,]}"
		HUDPATH "Core" "heal10" "19,339,33,25,25,25,${If[${swHealXTarget},,_]}"
		HUDPATH "Core" "heal11" "19,348,38,0,255,0,${If[${swHealPet},P,]}"
		HUDPATH "Core" "heal12" "19,348,33,25,25,25,${If[${swHealPet},,_]}"
		HUDPATH "Core" "heal13" "19,357,38,0,255,0,${If[${swHealCure},C,]}"
		HUDPATH "Core" "heal14" "19,357,33,25,25,25,${If[${swHealCure},,_]}"
		HUDPATH "Core" "heal15" "19,280,38,100,149,237,${If[!${Select[${Me.Class.ShortName},CLR,SHM,PAL,DRU]},Heal,]}"
		HUDPATH "Core" "heal16" "19,280,38,255,0,0,${If[${Select[${Me.Class.ShortName},CLR,SHM,PAL,DRU]} && ${switchHealAggressive},AGG,]}"
		HUDPATH "Core" "heal17" "19,280,38,100,149,237,${If[${Select[${Me.Class.ShortName},CLR,SHM,PAL,DRU]} && !${switchHealAggressive},Heal,]}"

		| notice at bottom	
		HUDPATH "Core" "notice1" "19,5,132,255,255,255,${hudNotice}"

		HUDPATH "Core" "time1" "19,360,132,146,149,180,${If[${swHUDTime} || !${currentBuild},${Time.Time24},${GameTime}]}"
		
		| back ground generation. @korha
		HUDPATH "Core" "BG2a" "3,131,0,0,0,0,███████████████████████████████████"
		HUDPATH "Core" "BG2a2" "3,131,13,0,0,0,███████████████████████████████████"
		HUDPATH "Core" "BG2b" "3,3,25,0,0,0,███████████████████████████████████████████████████"
		HUDPATH "Core" "BG2c" "3,3,35,0,0,0,███████████████████████████████████████████████████"
		HUDPATH "Core" "BG2d" "3,3,45,0,0,0,███████████████████████████████████████████████████"
		HUDPATH "Core" "BG2e" "3,3,55,0,0,0,███████████████████████████████████████████████████"
		HUDPATH "Core" "BG2f" "3,3,65,0,0,0,███████████████████████████████████████████████████"
		HUDPATH "Core" "BG2g" "3,3,75,0,0,0,███████████████████████████████████████████████████"
		HUDPATH "Core" "BG2h" "3,3,85,0,0,0,███████████████████████████████████████████████████"
		HUDPATH "Core" "BG2i" "3,3,95,0,0,0,███████████████████████████████████████████████████"
		HUDPATH "Core" "BG2j" "3,3,105,0,0,0,███████████████████████████████████████████████████"
		HUDPATH "Core" "BG2k" "3,3,115,0,0,0,███████████████████████████████████████████████████"
		HUDPATH "Core" "BG2l" "3,3,125,0,0,0,███████████████████████████████████████████████████"
		HUDPATH "Core" "BG2m" "3,3,135,0,0,0,███████████████████████████████████████████████████"
		
		
	| elements HUD	
	} else /if (${_verbage.Equal[elements]}) {

		| target
		HUDPATH "Elements" "e.tar" "21,5,26,255,255,255,${Target.DisplayName}"


	| elements HUD	
	} else /if (${_verbage.Equal[dev]}) {

		HUDPATH "DEV" "LastMouseOverText" "21,4,50,0,255,0,${If[${EverQuest.LastMouseOver},${EverQuest.LastMouseOver.Name},]}"
		HUDPATH "DEV" "LastMouse7bg"      "21,2,50,0,0,0,${If[${EverQuest.LastMouseOver},█████████████████████████,]}"


	| debug HUD
	}	else /if (${_verbage.Equal[debug]}) {

		| --- row 1

		| current line
		HUDPATH "Debug" "debug.cl1" "3,5,150,255,255,255,CL"
		HUDPATH "Debug" "debug.cl2" "19,30,150,100,149,237,${Macro.CurLine}"
		| runtime
		HUDPATH "Debug" "debug.rt1" "3,5,160,255,255,255,RT"
		HUDPATH "Debug" "debug.rt2" "19,30,160,100,149,237,${Macro.RunTime}"
		| target2kill
		HUDPATH "Debug" "debug.T2K1" "3,5,170,255,255,255,T2K"
		HUDPATH "Debug" "debug.T2K2" "19,30,170,100,149,237,${Target2Kill}"
		| _tmpID
		HUDPATH "Debug" "debug.tid1" "3,5,180,255,255,255,tID"
		HUDPATH "Debug" "debug.tid2" "19,30,180,100,149,237,${_tmpID}"

		
		| --- row 2

		| bindtrigger
		HUDPATH "Debug" "debug.bt1" "3,70,150,255,255,255,BT"
		HUDPATH "Debug" "debug.bt2" "19,95,150,100,149,237,${If[${bindTrigger},T,F]}"
		| isValidTarget 
		HUDPATH "Debug" "debug.ivt1" "3,70,160,255,255,255,iVT"
		HUDPATH "Debug" "debug.ivt2" "19,95,160,100,149,237,${If[${isValidTarget},T,F]}"
		| combat
		HUDPATH "Debug" "debug.combat1" "19,70,170,255,0,0,${If[${Bool[${combat}]},${combat},]}"
		HUDPATH "Debug" "debug.combat2" "19,70,170,255,255,255,${If[${Bool[${combat}]},,${combat}]}"
		| burn
		| HUDPATH "Debug" "debug.burn1" "19,70,180,255,0,0,${If[${swBurnControl},BURN,]}"
		| HUDPATH "Debug" "debug.burn2" "19,70,180,255,255,255,${If[${swBurnControl},,BURN]}"

		| --- row 3

		| mob lists
		| HUDPATH "Debug" "debug.mL1" "3,120,150,255,255,255,mL"
		| HUDPATH "Debug" "debug.mL2" "19,140,150,100,149,237,${If[${Bool[${mobList}]},${mobList},--]}"
		| add lists
		| HUDPATH "Debug" "debug.aL1" "3,120,160,255,255,255,aL"
		| HUDPATH "Debug" "debug.aL2" "19,140,160,100,149,237,${If[${Bool[${addList}]},${addList},--]}"
		| queue count
		| HUDPATH "Debug" "debug.qC1" "3,120,170,255,255,255,qC"
		| HUDPATH "Debug" "debug.qC2" "19,140,170,100,149,237,${queueCount}"
		
		| timer - validate zone
		HUDPATH "Debug" "debug.tVZ1" "3,120,150,255,255,255,tVZ"
		HUDPATH "Debug" "debug.tVZ2" "19,145,150,100,149,237,${timer_validate_zone}"
		| timer -  check tie
		HUDPATH "Debug" "debug.tCT1" "3,120,160,255,255,255,tCT"
		HUDPATH "Debug" "debug.tCT2" "19,145,160,100,149,237,${timer_check_Tie}"
		| timer - Clear Cursor
		HUDPATH "Debug" "debug.tCC1" "3,120,170,255,255,255,tCC"
		HUDPATH "Debug" "debug.tCC2" "19,145,170,100,149,237,${timer_clear_cursor}"
		| timer - Blocked Buffs
		HUDPATH "Debug" "debug.tBB1" "3,120,180,255,255,255,tBB"
		HUDPATH "Debug" "debug.tBB2" "19,145,180,100,149,237,${timer_check_buffs_blocked}"
		
			
		| --- row 4
		| timer - self buff
		HUDPATH "Debug" "debug.tSB1" "3,170,150,255,255,255,tBS"
		HUDPATH "Debug" "debug.tSB2" "19,195,150,100,149,237,${timer_check_buffs_self}"
		| timer - buff item
		HUDPATH "Debug" "debug.tBI1" "3,170,160,255,255,255,tBI"
		HUDPATH "Debug" "debug.tBI2" "19,195,160,100,149,237,${timer_check_buffs_item}"
		| timer - buff broadcast
		HUDPATH "Debug" "debug.tBB1" "3,170,170,255,255,255,tBB"
		HUDPATH "Debug" "debug.tBB2" "19,195,170,100,149,237,${timer_check_buffs_BC}"
		| timer - buff raid
		HUDPATH "Debug" "debug.tBR1" "3,170,180,255,255,255,tBR"
		HUDPATH "Debug" "debug.tBR2" "19,195,180,100,149,237,${timer_check_buffs_raid}"
		
		
		| --- row 5
		| timer - buff tell
		HUDPATH "Debug" "debug.tBT1" "3,220,150,255,255,255,tBT"
		HUDPATH "Debug" "debug.tBT2" "19,245,150,100,149,237,${timer_check_buffs_tell}"
		| timer - buff pet
		HUDPATH "Debug" "debug.tBP1" "3,220,160,255,255,255,tBP"
		HUDPATH "Debug" "debug.tBP2" "19,245,160,100,149,237,${timer_check_buffs_pet}"
		| timer - buff merc
		HUDPATH "Debug" "debug.tBM1" "3,220,170,255,255,255,tBM"
		HUDPATH "Debug" "debug.tBM2" "19,245,170,100,149,237,${timer_check_buffs_merc}"
		| timer - buff group
		HUDPATH "Debug" "debug.tBG1" "3,220,180,255,255,255,tBG"
		HUDPATH "Debug" "debug.tBG2" "19,245,180,100,149,237,${timer_check_buffs_other}"


		| --- row 6
		| timer - buff tell
		HUDPATH "Debug" "debug.cR1" "3,270,150,255,255,255,cR"
		HUDPATH "Debug" "debug.cr2" "19,295,150,100,149,237,${castReturn}"
		| zone
		HUDPATH "Debug" "debug.tBG2" "19,270,180,100,149,237,${Zone.ShortName}"
		

		| --- bottom line
		| curent sub
		HUDPATH "Debug" "debug.sub1" "19,5,197,100,149,237,${currentdebugsub}"

	}

/return



|***
 *	DES: MQ2MyButtons INI generation/correction
 *	USE: 
 *	NOTE: Yes I force this shit.. bite me..
 ***|
sub mybutton_control()
	/if (!${Macro.IsTLO[MyButtons]}) /return

	| window label
	/noparse /ini ".\..\MQ2MyButtons.ini" "UISettings" "WindowTitle" "Core"

	| here command
	/noparse /ini ".\..\MQ2MyButtons.ini" "Button1" "Label" "Combat Now"
	/noparse /ini ".\..\MQ2MyButtons.ini" "Button1" "Command" "/here"
	/noparse /ini ".\..\MQ2MyButtons.ini" "Button1" "Red" "255"
	/noparse /ini ".\..\MQ2MyButtons.ini" "Button1" "Green" "255"
	/noparse /ini ".\..\MQ2MyButtons.ini" "Button1" "Blue" "153"
	
	| auto/manual
	/noparse /ini ".\..\MQ2MyButtons.ini" "Button2" "Label" "Auto On/Off"
	/noparse /ini ".\..\MQ2MyButtons.ini" "Button2" "Command" "/core auto"
	/noparse /ini ".\..\MQ2MyButtons.ini" "Button2" "Red" "255"
	/noparse /ini ".\..\MQ2MyButtons.ini" "Button2" "Green" "255"
	/noparse /ini ".\..\MQ2MyButtons.ini" "Button2" "Blue" "153"	

	| pull switch
	/noparse /ini ".\..\MQ2MyButtons.ini" "Button3" "Label" "Puller On/Off"
	/noparse /ini ".\..\MQ2MyButtons.ini" "Button3" "Command" "/cc pull"
	/noparse /ini ".\..\MQ2MyButtons.ini" "Button3" "Red" "255"
	/noparse /ini ".\..\MQ2MyButtons.ini" "Button3" "Green" "255"
	/noparse /ini ".\..\MQ2MyButtons.ini" "Button3" "Blue" "153"

	| agro switch
	/noparse /ini ".\..\MQ2MyButtons.ini" "Button4" "Label" "Agro On/Off"
	/noparse /ini ".\..\MQ2MyButtons.ini" "Button4" "Command" "/cc agro"
	/noparse /ini ".\..\MQ2MyButtons.ini" "Button4" "Red" "255"
	/noparse /ini ".\..\MQ2MyButtons.ini" "Button4" "Green" "255"
	/noparse /ini ".\..\MQ2MyButtons.ini" "Button4" "Blue" "153"

	| tie all switch
	/noparse /ini ".\..\MQ2MyButtons.ini" "Button5" "Label" " TIE (All) to me"
	/noparse /ini ".\..\MQ2MyButtons.ini" "Button5" "Command" "/dge all /tie"
	/noparse /ini ".\..\MQ2MyButtons.ini" "Button5" "Red" "255"
	/noparse /ini ".\..\MQ2MyButtons.ini" "Button5" "Green" "255"
	/noparse /ini ".\..\MQ2MyButtons.ini" "Button5" "Blue" "153"

	| in charege command
	/noparse /ini ".\..\MQ2MyButtons.ini" "Button6" "Label" "In Charge"
	/noparse /ini ".\..\MQ2MyButtons.ini" "Button6" "Command" "/imincharge"
	/noparse /ini ".\..\MQ2MyButtons.ini" "Button6" "Red" "255"
	/noparse /ini ".\..\MQ2MyButtons.ini" "Button6" "Green" "255"
	/noparse /ini ".\..\MQ2MyButtons.ini" "Button6" "Blue" "255"

	| 
	/noparse /ini ".\..\MQ2MyButtons.ini" "Button7" "Label" ""
	/noparse /ini ".\..\MQ2MyButtons.ini" "Button7" "Command" ""
	/noparse /ini ".\..\MQ2MyButtons.ini" "Button7" "Red" "255"
	/noparse /ini ".\..\MQ2MyButtons.ini" "Button7" "Green" "255"
	/noparse /ini ".\..\MQ2MyButtons.ini" "Button7" "Blue" "153"

	| set toons home camp
	/noparse /ini ".\..\MQ2MyButtons.ini" "Button8" "Label" "Home Camp"
	/noparse /ini ".\..\MQ2MyButtons.ini" "Button8" "Command" "/movement home"
	/noparse /ini ".\..\MQ2MyButtons.ini" "Button8" "Red" "255"
	/noparse /ini ".\..\MQ2MyButtons.ini" "Button8" "Green" "255"
	/noparse /ini ".\..\MQ2MyButtons.ini" "Button8" "Blue" "153"

	| builds crew 1
	/noparse /ini ".\..\MQ2MyButtons.ini" "Button9" "Label" "Build Crew 1"
	/noparse /ini ".\..\MQ2MyButtons.ini" "Button9" "Command" "/crew 1"
	/noparse /ini ".\..\MQ2MyButtons.ini" "Button9" "Red" "255"
	/noparse /ini ".\..\MQ2MyButtons.ini" "Button9" "Green" "255"
	/noparse /ini ".\..\MQ2MyButtons.ini" "Button9" "Blue" "153"

	| gather your group
	/noparse /ini ".\..\MQ2MyButtons.ini" "Button10" "Label" "Gather Crew 1"
	/noparse /ini ".\..\MQ2MyButtons.ini" "Button10" "Command" "/movement gather 1"
	/noparse /ini ".\..\MQ2MyButtons.ini" "Button10" "Red" "255"
	/noparse /ini ".\..\MQ2MyButtons.ini" "Button10" "Green" "255"
	/noparse /ini ".\..\MQ2MyButtons.ini" "Button10" "Blue" "153"
	
	| start self
	/noparse /ini ".\..\MQ2MyButtons.ini" "Button11" "Label" "Start (Me)"
	/noparse /ini ".\..\MQ2MyButtons.ini" "Button11" "Command" "/mac tc/core.mac"
	/noparse /ini ".\..\MQ2MyButtons.ini" "Button11" "Red" "255"
	/noparse /ini ".\..\MQ2MyButtons.ini" "Button11" "Green" "255"
	/noparse /ini ".\..\MQ2MyButtons.ini" "Button11" "Blue" "153"

	| start everyone
	/noparse /ini ".\..\MQ2MyButtons.ini" "Button12" "Label" "Start (All)"
	/noparse /ini ".\..\MQ2MyButtons.ini" "Button12" "Command" "/dga all /mac tc/core.mac"
	/noparse /ini ".\..\MQ2MyButtons.ini" "Button12" "Red" "255"
	/noparse /ini ".\..\MQ2MyButtons.ini" "Button12" "Green" "255"
	/noparse /ini ".\..\MQ2MyButtons.ini" "Button12" "Blue" "153"

/return





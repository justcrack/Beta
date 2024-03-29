|***
 *	modes.inc
 *	Core Mode Specific Functions
 *	for doing strange stuff....
 *
 *
 ***|




|***
 *	DES: set mode type
 *	USE: /mode []
 *  NOTE: starts/ends non-standard modes
 ***|
#bind mode /mode
sub Bind_mode(string _type, string _verbage)
	/declare _bind string local Mode
	/declare _list string local ${sql_return[lstTypes${_bind}]}
	/if (${Bool[${_type}]}) {
		/varset _type ${list_isoloate[lstTypes${_bind}, ${_type}]}
		}
	/declare _count int local 0
	/declare _listout string local
	/declare _sep bool local		
	
	| set mode agro range
	/if (${_type.Equal[agro]}) {
		/if (${Range.Between[1,999999:${Int[${_verbage}]}]}) {
			/call sql_update FALSE ${chrID} setModeAgro ${_verbage}  
		} else {
			VOUT /mode agro [${cinfo}${setModeAgro}\ax] allowable range is ${cinfo}1 - 99999\ax
			/return
		}	
		
	| set harvest counts	
	} else /if (${_type.Find[harvestcount]}) {
		/if (${Range.Between[1,999999:${Int[${_verbage}]}]}) {
			/call sql_update FALSE ${chrID} setModeHarvestCount ${_verbage}  
		} else {
			VOUT /${_bind.Lower} harvestcount [${cinfo}${setModeHarvestCount}\ax] allowable range is ${cinfo}1 - 999999\ax
			/return
		}	

	| all other switches
	} else /if (${_list.Find[${_type}]}) {
		/varset sw${_bind}${_type} ${If[${sw${_bind}${_type}},FALSE,TRUE]}
		/if ((${_type.Equal[hunt]} || ${_type.Equal[scorch]})) {
			/call set_stop NOECHO
			/call clear_combat
		}
	}		

	| output
	/if (${swMQClear}) /mqclear
	/for _count 1 to ${_list.Count[|]}
		/varset _listout ${_listout} ${If[${_sep},${sep},]} ${If[${sw${_bind}${_list.Arg[${_count},|]}},${cgood},${cinfo}]}${_list.Arg[${_count},|].Lower}\ax
		/if (${_list.Arg[${_count},|].Equal[agro]}) /varset _listout ${_listout}:${cnum}${setModeAgro}\ax
		/if (${_list.Arg[${_count},|].Equal[harvestcount]}) /varset _listout ${_listout}:${cnum}${setModeHarvestCount}\ax
		/varset _sep TRUE
	/next _count
	VOUT /${_bind.Lower} [${_listout}]				

	/varset swOverrideLOS ${sql_return[swOverrideLOS]}
	/varset swOverrideEngagePct ${sql_return[swOverrideEngagePct]}
	/varset swAutoCursor ${sql_return[swAutoCursor]}

/return



|***
 *	DES: stop all non standard modes
 *	USE: /call switch_clear_mode
 *	INI: NA
 *  NOTE: 
 ***|
sub switch_clear_mode()
	/declare _count int local 0
	/declare _list string local ${sql_return[lstTypesMode]}
	/if (${setMode}) /varset setMode FALSE

	/for _count 1 to ${_list.Count[|]}
		/if (${swMode${_list.Arg[${_count},|]}}) {
			/varset swMode${_list.Arg[${_count},|]} FALSE  
		}
	/next _count	
	
	/varset swOverrideLOS ${sql_return[swOverrideLOS]}
	/varset swOverrideEngagePct ${sql_return[swOverrideEngagePct]}
	/varset swAutoCursor ${sql_return[swAutoCursor]}
	/varset swModeTrainspellSPC FALSE
/return



|***
 *	DES: which modes do we want to do?
 *	USE: /call do_mode
 *  NOTE: 
 ***|
sub do_mode()
	AMIDEAD

	CLEARCURSOR TRUE
	/doevents
	/if (${swModeDrag}) /call mode_drag
	/if (${swModeForage} && ${Me.AbilityReady[forage]}) /call mode_forage
	/if (${swModeFish} && ${Me.AbilityReady[fishing]}) /call mode_fish
	/if (${swModePetfarm} && !${timer_Petfarm_delay}) /call mode_petfarm
	/if (${swModeHunt} && !${timer_wait_for_mob_to_kill}) /call mode_hunt
	/if (${swModeScorch} && !${timer_ScorchedEarth_delay}) /call mode_scorched_earth
	/if (${swModeHarvest} && !${timer_Harvest_delay}) /call mode_harvest
	/if (${swModeTradeskill}) /call mode_tradeskill
	/if (${swModeLush} && !${Me.Drunk}) /call mode_get_waisted
	/if (${swModeDrink} && !${Me.Drunk}) /call mode_get_drunk
	/if (${swModeTrainspell}) /call mode_trainspell
	/if (${swModeOpen} && !${timer_mode_open}) /call mode_open
	/if (${swModeSkill}) /call mode_skill_train
	/if (${swModeCollect}) /call mode_collect
	/if (${swModeSummon}) /call mode_summon

/return



|***
 *	DES: central get target and kill for hunting modes
 *	USE: /call mode_get_mob
 *  NOTE: 
 ***|
sub mode_get_mob()
	:movetomob
	CHECKINPUTS
	AMIDEAD

	CLEARCURSOR TRUE

	/if (!${Spawn[${Target2Kill}].ID} || ${Spawn[${Target2Kill}].Dead} || ${Target.Dead}) {
		/call set_stop NOECHO
		/call clear_combat
		/return
	}

	| /call check_watch_spawn

	| get us to the mob
	| /if (${Spawn[${Target2Kill}].Distance} > ${setCombatDistance} && ${Spawn[${Target2Kill}].LineOfSight} && ${Spawn[${Target2Kill}].Distance} > ${setMobAgro}) {
	/if (${Spawn[${Target2Kill}].Distance} > ${setCombatDistance} && ${Spawn[${Target2Kill}].Distance} > ${setMobAgro}) {
		/squelch /face ID ${Target2Kill} nolook ${If[${swFaceFast},fast,]}
		
		| use nav for movement
		/if (${setTieMode.Equal[nav]} && ${Bool[${Plugin[MQ2Nav]}]} && !${Navigation.Active}) {
			| /echo nav to ${Target2Kill}
			/if (${Navigation.Active}) /nav stop log=off
			/squelch /nav ID ${Target2Kill} log=off

		| use stick for movement
		} else /if (${setTieMode.Equal[stick]} && ${Bool[${Plugin[MQ2MoveUtils]}]} && !${Stick.Active}) {
			/squelch /stick ID ${Target2Kill} ${setCombatDistance} UW
		}
		
	| at the mob, do something about it		
	} else /if (${Spawn[${Target2Kill}].LineOfSight} && ${Spawn[${Target2Kill}].Distance} <= ${setMobAgro}) {
		/squelch /face ID ${Target2Kill} nolook ${If[${swFaceFast},fast,]}
		/squelch /target ID ${Target2Kill}
	}
					
	/if (${Spawn[${Target2Kill}].Distance} < ${setMobAgro} && ${Spawn[${Target2Kill}].Distance} > 31) {
		/squelch /target ID ${Target2Kill}
		/if (${Me.Pet.ID} && !${Me.Pet.Target.ID}) /pet attack
		/if (${Target.ID} && ${Spawn[${Target2Kill}].LineOfSight}) /range
	} else /if (${Spawn[${Target2Kill}].Distance} < ${setCombatDistance}) {
		/call set_combat COMBAT
		/if (${Me.Pet.ID} && !${Me.Pet.Target.ID}) /pet attack
		
		/if (${Bool[${ModeNuke}]}) {
			/if (${validate_cast[FALSE, FIND, "${ModeNuke}", ${Target2Kill}]}) /call core_cast2 "${ModeNuke}" FIND ${Target2Kill} FALSE 
		}
	} 

	/if (${Spawn[${Target2Kill}].ID} && ${Spawn[${Target2Kill}].Distance} > ${setCombatDistance}) /goto :movetomob	

/return



|***
 *	DES: Train non spell skills. Best to use during a duel with a pet.
 *	USE:
 *  NOTE: This is not working yet...
 ***|
sub mode_skill_train(string _skilltype)
	
	|**
	/declare _sep bool local
	/declare _listout string local
	/declare _count int local 0
	/declare _bind string local ${Me.Class.ShortName.Lower}
  /declare _type string	local |beg|bindwound
	**|
	
	
	AMIDEAD
	UPDATEHUD "Skill Train"
	/delay 1

	/if (${Me.Skill[Taunt]} < ${Skill[Taunt].SkillCap} && ${Me.AbilityReady[Taunt]}) /doability Taunt
	/if (${Me.Skill[Disarm]} < ${Skill[Disarm].SkillCap} && ${Me.AbilityReady[Disarm]}) /doability Disarm
	/if (${Me.Skill[Bash]} < ${Skill[Bash].SkillCap} && ${Me.AbilityReady[Bash]}) /doability Bash
	/if (${Me.Skill[Kick]} < ${Skill[Kick].SkillCap} && ${Me.AbilityReady[Kick]}) /doability Kick
	/if (${Me.Skill[Sneak]} < ${Skill[Sneak].SkillCap} && ${Me.AbilityReady[Sneak]}) /doability sneak
	
	/if (${Target.ID} && ${Target.ID} != ${Me.ID}) {
		/if (${Me.Skill[Begging]} < ${Skill[Begging].SkillCap} && ${Me.AbilityReady[Begging]}) /doability begging
	}
		
	| Bind Wounds
	/if (${Me.AbilityReady[Bind Wound]} && ${FindItem[=Bandages].ID}) {
		/if (${Me.Skill[Bind Wound]} < ${Skill[Bind Wound].SkillCap} && ${Me.PctHPs} < 99*.${tmpHealOverride}) {
			/squelch /target ID ${Me.ID}
		  /delay 3s ${Target.ID} == ${Me.ID}
			/delay 2s
			/doability "Bind Wound"
		}
	}
	
	UPDATEHUD FALSE
/return



|***
 *	DES: to help your rusty dagger collection.. why else would you fish?
 *	USE: /call do_fish_mode | /mode fish
 *  NOTE: 
 ***|
sub mode_fish()
	AMIDEAD
	UPDATEHUD "Fish"
	/if (!${Me.Standing}) /stand
	/if (${Me.ItemReady[Fisherman's Companion]} && !${Me.ItemReady[Brell's Fishin' Pole]}) {
		/if (${Me.Invis}) /docommand /makemevisable
		/if (${validate_cast[FALSE, item, "Fisherman's Companion", ${Me.ID}]}) /call core_cast2 "Fisherman's Companion" item ${Me.ID}
	}
	/if (${Me.AbilityReady[fishing]}) /doability fishing
	UPDATEHUD FALSE
/return



|***
 *	DES: to play in the dirt
 *	USE: /call mode_forage | /mode forage
 *  NOTE: 
 ***|
sub mode_forage()
	/if (${Select[${Me.CombatState},COMBAT]}) /return
	AMIDEAD
	UPDATEHUD "Forage"
	/if (!${swAutoCursor}) /varset swAutoCursor TRUE
	/if (!${Me.Standing}) /stand
	/if (${Me.AbilityReady[forage]}) /doability forage
	/delay 1s
	UPDATEHUD FALSE
/return



|***
 *	DES: Kill people then drag them around like trophys!!! Win Win...
 *	USE: /call mode_drag | /mode drag
 *  NOTE: 
 ***|
sub mode_drag()
	/declare _count 		int local
	/declare _corpseID 	int local
	UPDATEHUD "Drag"
	CHECKINPUTS
	AMIDEAD

	/delay 1
	/if (${SpawnCount[pccorpse radius 110]}) {
		/for _count 1 to ${SpawnCount[pccorpse radius 110]}
			AMIDEAD

			/varset _corpseID ${NearestSpawn[${_count},pccorpse radius 110].ID}
			/if (${Spawn[${_corpseID}].Distance} > 15) {
				/squelch /target id ${_corpseID}
				/delay 5 ${Target.ID} == ${_corpseID}
				/corpse
			}
		/next _count
	}
	UPDATEHUD FALSE
/return



|***
 *	DES: turns the toon into a hunter of things.
 *	USE: /call mode_hunt | /mode hunt
 *  NOTE: works better in open zones. don't say i didn't wan you.
 ***|
sub mode_hunt()
	/declare _count int local 0
	/doevents
	AMIDEAD

	UPDATEHUD "Hunt"
	CLEARCURSOR TRUE
	
	/for _count 1 to ${HuntMobList.Count[|]}
		/if (${Spawn[npc ${HuntMobList.Arg[${_count},|]} radius ${setModeAgro} targetable].ID}) {
			/varset Target2Kill ${NearestSpawn[npc ${HuntMobList.Arg[${_count},|]}].ID}
			/call echos hunt 0 0 ${Target2Kill} 0
			/goto :haveatarget
		}
	/next _count	

	:haveatarget
	/if (!${Target2Kill}) {
		/call timer_set timer_wait_for_mob_to_kill ${tHunt_delay}
		/call check_buff_routines TRUE
	}

	/if (!${Bool[${FindItemCount[${lstRangeItemSummon}]}]} && ${Bool[${lstRangeItemSummon}]}) /call check_ammo
	/if (${swADVL}) {
		/if ((${coreAuto} || ${swOverrideLoot}) && !${SafeZone} && !${swOverrideSafeZone}) /call do_loot
	}
	/if (${Spawn[${Target2Kill}].ID}) {
		/call mode_get_mob
	} else /if (${Spawn[${Target2Kill}].Dead} || !${Spawn[${Target2Kill}].ID} || !${timer_scorched_earth_fail}) {
		/if (!${timer_scorched_earth_fail}) /call timer_set timer_scorched_earth_ignore_${Target2Kill} ${tScorchedEarth_ignore} 
		/call set_stop NOECHO
		/call clear_combat
	}	
		
	UPDATEHUD FALSE
/return



|***
 *	DES: Pet Farm
 *	USE: /call mode_petfarm | /mode petfarm
 *  NOTE: Use with caution. For static pet classes only.
 ***|
sub mode_petfarm()
	AMIDEAD

	/declare _tmpID					int local 0
	/declare _spawnCount 		int local 0
	/declare _resetCount		int local 0
	/declare _nameloop			int local 1

	/varset swOverrideEngagePct TRUE
	/varset swOverrideLOS TRUE
	
	UPDATEHUD "Pet Farm"
	:loopfindmob
	/doevents
	/if (!${Select[${Me.Class.ShortName},MAG,ENC,NEC,BST,SHD,SHM,RNG,DRU]} || (${SafeZone} && !${swOverrideSafeZone})) {
		/call echos failboat 0 0 0 0
		/call switch_clear_mode
		/return
	}
	/call check_watch_spawn
	/if ((${coreAuto} || ${swOverrideLoot}) && !${SafeZone} && !${swOverrideSafeZone}) /call do_loot
	
	/if (${Select[${Me.Class.ShortName},MAG]} && ${switchHealPet}) /call pet_heal_MAG

	/if (${SpawnCount[npc ${If[${swOverrideLOS},,los]} radius ${setMobAgro} zradius ${setzradius} targetable]}) {
		/varset _spawnCount ${SpawnCount[npc ${If[${swOverrideLOS},,los]} radius ${setMobAgro} zradius ${setzradius}]}	
		/varset Target2Kill ${Spawn[${NearestSpawn[1, ${If[${swOverrideLOS},,los]} npc radius ${setMobAgro} zradius ${setzradius} targetable]}].ID}

		| skip excluded mobs
		:loopnamedmob
		/if (${ExcludeList.Find[${Spawn[id ${Target2Kill}].DisplayName}]})	{
			/varcalc _nameloop ${_nameloop}+1
			/if (${_spawnCount} > 1) {
				/varset Target2Kill ${Spawn[${NearestSpawn[${_nameloop}, ${If[${swOverrideLOS},,los]} npc radius ${setMobAgro} zradius ${setzradius} targetable]}].ID}

				/goto :loopnamedmob
			} else {
				/varset Target2Kill 0
				/varset _nameloop 1
			}
		}
				
		/if (!${Target2Kill}) /goto :nomob
		/varset _resetCount 0
	} else {
		:nomob
		/doevents
		/varset _resetCount 0
		/varset _spawnCount 0
		/varset Target2Kill 0
		/varset _nameloop 1
		/if (${timer_Petfarm_delay}) /goto :loopfindmob

		/if (!${timer_Petfarm_delay}) /call timer_update timer_Petfarm_delay ${tPetfarm_delay}
		/if (${Me.Pet.Target.ID}) /pet back off
		/if (${Me.AltAbilityReady[Summon Companion]} && ${Me.Pet.Distance} > 80) {
			/if (${validate_cast[FALSE, alt, "Summon Companion", ${Me.Pet.ID}]}) /call core_cast2 "Summon Companion" alt 0 FALSE
		}

		/return
	}
	
	:gettarget
	CHECKTIE
	/doevents
	/squelch /target ID ${Target2Kill}
	/if (!${ExcludeList.Find[${Spawn[id ${Target2Kill}].DisplayName}]}) {
		/if (!${Pet.Target.ID}) {
			| /delay 5
			/call echos petfarm 0 ${Target2Kill} 0 0
			/pet attack
			/delay 2s
		}
	}
	/varcalc _resetCount ${_resetCount}+1
	
	/if (${_resetCount} > 30) {
		/varset _resetCount 0
		/if (${validate_cast[FALSE, alt, "Summon Companion", ${Me.Pet.ID}]}) /call core_cast2 "Summon Companion" alt 0 FALSE
		/goto :loopfindmob
	}		
		
	:loopwaittilldead
	/delay 1s
	/doevents
	CHECKTIE
	/if (${Spawn[${Target2Kill}].Type.Equal[CORPSE]} || !${Spawn[${_tmpID}].ID}) /goto :loopfindmob
	/if (${Spawn[${Target2Kill}].Distance} > ${setMobAgro}) {
		/goto :loopfindmob
	}
	/if (${Me.Pet.Target.ID} != ${Target2Kill}) /goto :gettarget
	/if (${Spawn[${Target2Kill}].Type.NotEqual[CORPSE]}) /goto :loopwaittilldead

	UPDATEHUD FALSE
/return



|***
 *	DES: Scorched Earth Mode
 *	USE: /call mode_scorched_earth | /mode scorch
 *  NOTE: This will kill everything in a zone. or at least try /shrug
 ***|
sub mode_scorched_earth()
	AMIDEAD
	/declare _spawnCount 		int local 0
	/declare _resetCount		int local 0
	/declare _nameloop			int local 1
	/declare _tmpID 				int local
	/declare _tmpRange 			int local ${Math.Calc[${setMobAgro}/10]}
	/declare _count 				int local 0


	/varset swOverrideEngagePct TRUE
	/varset swOverrideLOS TRUE
	/call clear_combat
	
	:loopfindmob
	UPDATEHUD "Kill 'Em All"
	/doevents
	/if (!${swModeScorch}) /return
	/if (${SafeZone} && !${swOverrideSafeZone}) {
		/call echos failboat 0 0 0 0
		/call switch_clear_mode
		/return
	}
	/call check_watch_spawn
	/if ((${coreAuto} || ${swOverrideLoot}) && !${SafeZone} && !${swOverrideSafeZone}) /call do_loot
	/call check_buff_routines TRUE
	/if (!${Bool[${FindItemCount[${lstRangeItemSummon}]}]} && ${Bool[${lstRangeItemSummon}]}) /call check_ammo



	/if (${SpawnCount[npc ${If[${swOverrideLOS},,los]} radius ${setModeAgro} targetable]}) {
		
		| find something in our local range first
		/while (${_tmpRange} <= ${setMobAgro} && !${Target2Kill}) {
			/if (${SpawnCount[npc ${If[${swOverrideLOS},,los]} range ${setMobLvlMin} ${setMobLvlMax} radius ${_tmpRange} zradius ${setzradius} targetable]}) {
				/for _count 1 to ${SpawnCount[npc ${If[${swOverrideLOS},,los]} range ${setMobLvlMin} ${setMobLvlMax} radius ${_tmpRange} zradius ${setzradius} targetable]}
					AMIDEAD
					/if (!${ExcludeList.Find[${Spawn[id ${_tmpID}].DisplayName}]}) {
						/varset _tmpID ${NearestSpawn[${_count}, npc ${If[${swOverrideLOS},,los]} range ${setMobLvlMin} ${setMobLvlMax} radius ${_tmpRange} zradius ${setzradius} targetable].ID}
					}
			
				/next _count
			}
			/varcalc _tmpRange ${_tmpRange}+10
		}	
		
		| nothing in local range, check our max range
		/if (!${Target2Kill}) {
			/if (${SpawnCount[npc ${If[${swOverrideLOS},,los]} radius ${setModeAgro} targetable]}) {
				/varset _spawnCount ${SpawnCount[npc ${If[${swOverrideLOS},,los]} radius ${setModeAgro} targetable]}	
				/varset Target2Kill ${Spawn[${NearestSpawn[1, npc ${If[${swOverrideLOS},,los]} radius ${setModeAgro} targetable]}].ID}
				/call timer_set timer_scorched_earth_fail ${tScorchedEarth_fail}

				:loopignoreddmob
				
				/if (${ExcludeList.Find[${Spawn[id ${Target2Kill}].DisplayName}]})	{
					/varcalc _nameloop ${_nameloop}+1
					/if (${_spawnCount} > 1) {
						/varset Target2Kill ${Spawn[${NearestSpawn[${_nameloop}, npc ${If[${swOverrideLOS},,los]} radius ${setModeAgro} targetable]}].ID}
						/goto :loopignoreddmob
					} else {
						/varset Target2Kill 0
						/varset _nameloop 1
					}
				}
				/if (!${Target2Kill}) /goto :nomob
				/varset _resetCount 0
			}
		}
				
	} else {
		:nomob
		/doevents
		/varset _resetCount 0
		/varset _spawnCount 0
		/varset Target2Kill 0
		/varset _nameloop 1
		/if (!${timer_ScorchedEarth_delay}) /call timer_set timer_ScorchedEarth_delay ${tScorchedEarth_delay}
		/if (${Me.Pet.Target.ID}) /pet back off
		/if (${Me.Pet.ID} && ${Me.AltAbilityReady[Summon Companion]} && ${Me.Pet.Distance} > 80) {
			/if (${validate_cast[FALSE, alt, "Summon Companion", ${Me.Pet.ID}]}) /call core_cast2 "Summon Companion" alt 0 FALSE
		}
		/return
	}
	/call echos hunt 0 0 ${Target2Kill} 0

	:gettarget
	/doevents
	
	/if (!${ExcludeList.Find[${Spawn[id ${Target2Kill}].DisplayName}]} && !${timer_scorched_earth_ignore_${Target2Kill}}) {
		/if (${Spawn[${Target2Kill}].ID}) {
			/call mode_get_mob
		} else /if (${Spawn[${Target2Kill}].Dead} || !${Spawn[${Target2Kill}].ID} || !${timer_scorched_earth_fail}) {
			/if (!${timer_scorched_earth_fail}) /call timer_set timer_scorched_earth_ignore_${Target2Kill} ${tScorchedEarth_ignore} 
			/call set_stop NOECHO
			/call clear_combat
		}
	}
	
	/varcalc _resetCount ${_resetCount}+1

	/if (${_resetCount} > 30) {
		/varset _resetCount 0
		/if (${Me.Pet.ID} && ${Me.AltAbilityReady[Summon Companion]} && ${Me.Pet.Distance} > 80) {
			/if (${validate_cast[FALSE, alt, "Summon Companion", ${Me.Pet.ID}]}) /call core_cast2 "Summon Companion" alt 0 FALSE
		}
		/goto :loopfindmob
	}		
		
	:loopwaittilldead
	/delay 1s
	/doevents
	/if (${Spawn[${Target2Kill}].Type.Equal[CORPSE]} || !${Spawn[${Target2Kill}].ID}) /goto :loopfindmob
	/if (${Spawn[${Target2Kill}].Distance} > ${setMobAgro}) {
		/goto :loopfindmob
	}
	/if (${Me.Pet.Target.ID} != ${Target2Kill}) /goto :gettarget
	/if (${Spawn[${Target2Kill}].Type.NotEqual[CORPSE]}) /goto :loopwaittilldead

	UPDATEHUD FALSE
/return



|***
 *	DES: Harvest Mode
 *	USE: /call mode_Harvest | /mode harvest
 *  NOTE: Will run around and pick up shit off the ground
 ***|
sub mode_harvest()
	AMIDEAD
	/declare _count int local
	UPDATEHUD "Harvest"
	/if (!${swAutoCursor}) /varset swAutoCursor TRUE

	CLEARCURSOR TRUE
	/for _count 1 to ${HarvestList.Count[|]}
		/if (${FindItemCount[${HarvestList.Arg[${_count},|]}]} >= (${setModeHarvestCount})) {
			/varset swModeHarvest FALSE
			/autoinventory
			/return
		}
		/if (${GroundItemCount[${HarvestList.Arg[${_count},|]}]}) {
			/call get_ground_harvest "${HarvestList.Arg[${_count},|]}"
		} else {
			/if (!${timer_Harvest_delay}) /call timer_set timer_Harvest_delay ${tHarvest_delay}
		}
	/next _count	

	UPDATEHUD FALSE
/return



|***
 *	DES: Collection go getter thingie
 *	USE:
 *  NOTE: Will run around and pick collection glowing globes
 ***|
sub mode_collect()
	AMIDEAD
	/declare _count int local
	UPDATEHUD "Collect"
	/if (!${swAutoCursor}) /varset swAutoCursor TRUE

	/if (!${Navigation.MeshLoaded}) {
 		/call switch_clear_mode
 		OUT NO Nav Mesh for this zone. 
 		/return	
	}

	/if (!${GroundItemCount[Glowing Globe]}) {
		/call timer_update timer_Harvest_delay ${tHarvest_delay}
		/return		
	}

	/squelch /itemtarget Glowing Globe
	/delay 2s ${Bool[${Ground.ID}]}
			
	/if (!${Navigation.PathExists[item]}) {
		VOUT No Nav Path
		/call switch_clear_mode
		/return
	}	
  	
	/nav item log=off
	/delay 1s
	/delay 3m !${Navigation.Active}
					
	/if (${Ground.Distance3D} < 19) {
		| /squelch /nav stop
		/invoke ${Ground[glowing].Grab}
		| /click left item
		/delay 1s

		CLEARCURSOR TRUE
		/delay 1s
		/call timer_update timer_Harvest_delay ${tHarvest_delay}
		/return
	}
				


	UPDATEHUD FALSE
/return



|***
 *	DES: moves to the ground item and picks it up
 *	USE: /call get_ground_harvest "ITEM NAME"
 *  NOTE: 
 ***|
sub get_ground_harvest(string _tmpItem)
	/squelch /itemtarget ${_tmpItem}
	/delay 2s ${Bool[${Ground.ID}]}
	/if (${Bool[${Ground.ID}]}) {
		/if (${Ground.Distance3D} < ${setModeAgro}) {
			
		/if (${Bool[${Plugin[MQ2Nav]}]} && ${setTieMode.Equal[nav]}) {	
			/if (${Navigation.MeshLoaded}) {	
				/delay 1 
				
				/if (${Navigation.PathExists[item]}) {
	        /nav item log=off
	    	} else /if (!${Navigation.PathExists[item]}) {
	    		/echo no path
	    	}
				
				| /nav loc ${Ground.Y} ${Ground.X} ${Ground.Z}
				| /nav item
				
				| /while (${Navigation.Active} || ${Ground.Distance3D} > 19) {
				| 	/delay 1s
				| }
				
				/if (${Ground.Distance3D} < 19) /call Bind_grab ${_tmpItem}
				
				
				/nav stop log=off
				
			} else /if (!${Navigation.MeshLoaded}) {
				OUT NO Nav Mesh for this zone. Returning to Stick.
				/call Bind_setTieControl mode stick
			}
				

		} else {
			:loopMove 
			/face nolook loc ${Ground.Y}, ${Ground.X}
			/delay 1 
			/keypress forward hold 
			/if (${Ground.Distance3D} > 19) /goto :loopMove
			/keypress forward
			/click left item
			/delay 5s (${Cursor.ID})
			/delay ${Math.Rand[10]}
			| VOUT Keep ${sep} ${cinfo}${Cursor.Name}\ax ${sep} [${ctar}${FindItemCount[${Cursor.Name}]}\ax] 
			/autoinventory
			CLEARCURSOR TRUE
			/delay 5s (!${Cursor.ID})
			/delay ${Math.Rand[10]}
		}

	}
/return



|***
 *	DES: Tradeskill combind
 *	USE: /call mode_tradeskill 
 *  NOTE: 
 ***|
#event mode_CombineError "#*#There was no place to put that#*#"
#event mode_CombineError "#*#You cannot combine these items in this container type!#*#"
#event mode_CombineError "#*#did not accept these items#*#"
#event mode_CombineError "#*#Sorry, but you don't have everything you need for this recipe in your general inventory.#*#"
sub Event_mode_CombineError(string _line)
	/varset swModeTradeskill FALSE
	/call echos mode "Tradeskill" swModeTradeskil 0 0
	/call echos failboat 0 0 0 0
	| /varset swAutoCursor ${Ini[${INIClass},"Common","swAutoCursor"]}
/return

sub mode_tradeskill()
	/if (!${Window[TradeskillWnd].Open}) {
		VOUT Tradeskill window must be open.
		VOUT Make All option must be deselected.
		/varset swModeTradeskill FALSE
		/return
	}
	/if (!${swAutoCursor}) /varset swAutoCursor TRUE
	UPDATEHUD "Tradeskill"

	/nomodkey /notify TradeskillWnd CombineButton leftmouseup
	/delay 1.5s ${Cursor.ID}
	/doevents
	
	UPDATEHUD FALSE
/return



|***
 *	DES: Alcohol Tolerance
 *	USE: /call mode_get_waisted 
 *  NOTE: 
 ***|
sub mode_get_waisted()
	/declare _ale string 	local Summoned: Ale
	/declare _aleCount int local 20	
	/declare _pole string	local Brell's Fishin' Pole
	/declare _box string local Fisherman's Companion

	| no fishermans companion. end this mess
	/if (!${FindItem[=${_box}].ID}) {
		VOUT ${onotice} ${sep} No ${_box}
		VOUT This Mode uses Fisherman's Companion.
		VOUT Go get one.
		/varset swModeLush FALSE
		/return
	}

	UPDATEHUD "Drink"
	/if (!${swAutoCursor}) /varset swAutoCursor TRUE
	| check max stats too ?
	
	| need booze! keep a stock on hand
	/if (${FindItemCount[${_ale}]} < ${_aleCount} && ${Me.ItemReady[${_pole}]} && !${Me.Drunk}) {
		/if (${validate_cast[FALSE, item, "${_pole}", ${Me.ID}]}) /call core_cast2 "${_pole}" item ${Me.ID} FALSE
		CLEARCURSOR TRUE
	}
	
	| lets get lit
	/if (${FindItemCount[${_ale}]} && ${Me.Drunk} < ${Skill[Alcohol Tolerance].SkillCap}) {
		VOUT Drinking ${sep} ${cinfo}${_ale}\ax
		/useitem ${_ale}
	}
	UPDATEHUD FALSE
/return



|***
 *	DES: 
 *	USE: /call mode_get_drunk 
 *  NOTE: 
 ***|
sub mode_get_drunk()
	/if (!${FindItem[${modeDrinkofChoice}].ID} || !${Bool[${modeDrinkofChoice}]}) {
		/varset swModeDrink FALSE
		/return
	}
	UPDATEHUD "Drink"

	| start drinking.
	/while (${Me.Skill[Alcohol Tolerance]} < ${Skill[Alcohol Tolerance].SkillCap}) {
		/if (!${Me.ItemReady[=${modeDrinkofChoice}]}) /break
		| /if (${Me.Drunk} < 200) /useitem ${modeDrinkofChoice}
		/if (${Me.Drunk} < 200) /call core_cast2 "${modeDrinkofChoice}" item 0 FALSE
		/delay 2
		/delay 2s ${Me.Drunk} < 200
	}

	| maxed skill turn this off
	/if (${Me.Skill[Alcohol Tolerance]} == ${Skill[Alcohol Tolerance].SkillCap}) {	
		VOUT Alcohol Tolerance Maxed ${sep} (${cnum}${Me.Skill[Alcohol Tolerance]}\ax)
		/varset swModeDrink FALSE
		/return
	}
	
	UPDATEHUD FALSE
/return



|***
 *	DES: train spells
 *	USE: /call mode_trainspell | /mode trainspell
 *  NOTE: 
 ***|
sub mode_trainspell(string _type)
	AMIDEAD
	UPDATEHUD "Train Spell"
	
	/if (${_type.Equal[specialize]} && !${swModeTrainspellSPC}) {
		/varset swModeTrainspellSPC TRUE
	}

	/declare _skill string local |alteration|abjuration|conjuration|divination|evocation
	/declare _count int local 0
	/declare _gem[5] bool local FALSE

	| figure out what we want to cast
	/for _count 1 to ${_skill.Count[|]}
		
		| validate skill caps and spell. if cap is maxed, move on		
		/if (!${swOverrideTrainSpell}) {
			/if (${Me.Skill[${If[${swModeTrainspellSPC},specialize ,]}${_skill.Arg[${_count},|]}]} == ${Me.SkillCap[${If[${swModeTrainspellSPC},specialize ,]}${_skill.Arg[${_count},|]}]}) /continue
		}
		
		| if the skill spell is set to FALSE, move on
		/if (!${Bool[${_skill.Arg[${_count},|]}]}) /continue
		
		| if the spell is FALSE, move on
		/if (!${Bool[${Spell[${${_skill.Arg[${_count},|]}}].Skill}]}) /continue

		| if I already have the spell memed, move on
		/if (!${Me.Gem[${_count}].Name.Equal[${${_skill.Arg[${_count},|]}}]}) /call mem_spell TRUE "${${_skill.Arg[${_count},|]}}" ${_count} FALSE
		
		/varset _gem[${_count}] TRUE
	/next _count

	| start getting things done
	/for _count 1 to ${_skill.Count[|]}
		/if (!${_gem[${_count}]}) /continue
		/if (${validate_cast[FALSE, spell, "${${_skill.Arg[${_count},|]}}", ${Me.ID}]}) /call core_cast2 "${${_skill.Arg[${_count},|]}}" 0 0 FALSE
		/call sort_cursor TRUE
	/next _count
/return



|***
 *	DES: open something
 *	USE: /call mode_open | /mode open
 *  NOTE: 
 ***|
sub mode_open(string _type)
	AMIDEAD
	UPDATEHUD "Open"

	/call timer_set timer_mode_open ${tMode_Open}
	/declare _tmpID int local

	/if (${SpawnCount[${modeOpenItem} npc radius 20]}) {
		/varset _tmpID ${Spawn[${modeOpenItem} npc radius 20].ID}
		/squelch /target ID ${_tmpID}
		/if (${Target.Dead}) /return
		/delay 2s ${Target.ID} == ${_tmpID}
		/open
		/delay 1s
		/return
	} 

	UPDATEHUD FALSE
/return


| /edit modeOpenItem "a hollows mushroom"



|***
 *	DES: summons a corpse in lobby
 *	USE: /call mode_summon | /mode summon
 *  NOTE: 
 ***|
sub mode_summon(string _type)

	UPDATEHUD "Summon Corpse"

	| set stone to buy
	/declare _soulstone string local
	/if (${Me.Level} <= 110) /varset _soulstone Draconic Soulstone
	/if (${Me.Level} <= 105) /varset _soulstone	Torrential Soulstone
	/if (${Me.Level} <= 100) /varset _soulstone	Coalescent Soulstone
	/if (${Me.Level} <= 95) /varset _soulstone Luminous Soulstone
	/if (${Me.Level} <= 90)	/varset _soulstone Phantasmal Soulstone
	/if (${Me.Level} <= 85)	/varset _soulstone Iridescent Soulstone
	/if (${Me.Level} <= 80)	/varset _soulstone Prismatic Soulstone
	/if (${Me.Level} <= 75)	/varset _soulstone Glowing Soulstone
	/if (${Me.Level} <= 70)	/varset _soulstone Pristine Soulstone
	/if (${Me.Level} <= 55)	/varset _soulstone Faceted Soulstone
	/if (${Me.Level} <= 50)	/varset _soulstone Greater Soulstone
	/if (${Me.Level} <= 40)	/varset _soulstone Soulstone
	/if (${Me.Level} <= 30)	/varset _soulstone Lesser Soulstone
	/if (${Me.Level} <= 20)	/varset _soulstone Minor Soulstone

	:start

	| in the lobby?
	/if (${Zone.ShortName.NotEqual[GuildLobby]}) /goto :bailmode

	| do we need to buy a stone?
  /if (!${FindItem[${_soulstone}].ID}) {

		/nav spawn npc A Disciple of Luclin |log=off

		:priestinventory
			/delay 10s !${Navigation.Active}
			/delay 1s
			/squelch /target A Disciple of Luclin
			/delay 1s
			/nomodkey /click right target
			/delay 5s ${Window[MerchantWnd].Open}
			/if (!${Window[MerchantWnd].Open}) /goto :priestinventory
		
		/delay 1s
		/call vendor_buy "${_soulstone}" 1

		:closepriestinventory
			/nomodkey /notify MerchantWnd MW_Done_Button LeftMouseUp
			/delay 10
			/if (${Window[MerchantWnd].Open}) /goto :closepriestinventory
  }

	/if (!${FindItem[${_soulstone}].ID}) /goto :start

	| move to rezer
  /nav spawn npc A Priest of Luclin |log=off
  /delay 10s !${Navigation.Active}

	| get stone on cursor
  /itemnotify "${_soulstone}" leftmouseup
  /delay 5

	:givetopriest
	  /squelch /target A Priest of Luclin
	  /delay 5
	  /nomodkey /click left target
	  /delay 5
	  /delay 5s ${Window[GiveWnd].Open}
	  /if (!${Window[GiveWnd].Open}) /goto :givetopriest
		/if (${Window[GiveWnd].Open}) {
			/notify GiveWnd GVW_Give_Button leftmouseup
			/delay 3s
			/if (${Window[GiveWnd].Open}) /goto :GiveSoulStone
		}

	:CloseInvWnd
		/if (${Window[InventoryWindow].Open}) {
			/nomodkey /notify InventoryWindow IW_DoneButton LeftMouseUp
			/delay 5
			/if (${Window[InventoryWindow].Open}) /goto :CloseInvWnd
		}


  /if (${Window[InventoryWindow].Open}) /nomodkey /notify InventoryWindow IW_DoneButton LeftMouseUp
  /delay 5

	:bailmode
	/squelch /target clear
	/varset swModeSummon FALSE

	UPDATEHUD FALSE

/return


|***
 *	class.enc.inc
 *	Core Enchanter Include
 *
 * 
 ***|


  

|***
 *	DES: Main Enchanter Routine
 *	USE: 
 *  NOTE: 
 ***|
sub core_ENC()
	DEBUG core_ENC()
			
	/if (${coreAuto} && !${Me.Invis}) {
		/if (!${check_engaged[]} && ${swPet} && !${switchCharmPet}) /call check_pet
	}

	| validate/fix lists
	/if (!${timer_validate_lists}) {
		/if (!${SpawnCount[NPC radius ${setMobAgro} playerstate 4]}) {
			/if (${mobList.Count[|]}) /varset mobList
			/if (${addList.Count[|]}) /varset addList
		}
		/call timer_update timer_validate_lists ${tValidate_Lists}
	}
	
	/while (${prep_combat[]}) {
		CHECKINPUTS
		/if (${swHealSelf}) /call check_heal_self_${Me.Class.ShortName}
		/if (!${timer_check_cast_cure} && ${swHealCure}) /call cast_cure FALSE
		/if (${exit_class_loop[]}) /return
				
		/if (${coreAuto} && ${swdeBuffMezz}) {
			/call get_target_ENC
			/if (${swBurnControl}) /call burn_${Me.Class.ShortName}
			/if (${isValidTarget} && ${Target.ID} && ${Target.LineOfSight} && ${Target.Distance} <= ${setMobAgro} && (${Target.PctHPs} <= ${setEngagePct} || ${Spawn[PC ID ${Me.TargetOfTarget.ID} Radius 60].ID})) /goto :loop
			| /if (${isValidTarget} && ${Target.LineOfSight} && ${Target.Distance} <= ${setMobAgro} && (${Target.PctHPs} <= ${setEngagePct} || ${Spawn[PC ID ${Me.TargetOfTarget.ID} Radius 60].ID})) /goto :loop
			/if (${Bool[${mobList}]}) /call list_delete mobList ${Target2Kill}
			/return
			:loop
			/if (${Spawn[${Target2Kill}].Type.Equal[Corpse]} || !${Target2Kill} || !${Target.ID} || ${Target.Type.Equal[corpse]} || !${Spawn[${Target2Kill}].ID}) /call get_target_ENC
			/call check_for_adds
			/if (${addList.Count[|]}) /call mez_adds
			/call check_for_adds
			/if (${addList.Count[|]}) /return
			/if (${mobList.Find[|]}) /call list_update mobList ${Target2Kill}
			/call check_for_adds
			/if (${addList.Count[|]}) /return
			/if (!${addList.Count[|]}) /call check_deBuffs ${Target2Kill}
			/if (${mobList.Find[|]}) /call list_update mobList ${Target2Kill}
		}

		/if (${isValidTarget}) {
			/call set_combat ${combat.Upper}
			/if (${swBurnControl}) /call burn_${Me.Class.ShortName}
			/call check_deBuffs ${Target2Kill}
			/if (${swSwarm} || ${swPet}) /call send_pet
			/if (${swCombatMelee}) /call use_skill_melee ${Target2Kill}
			/if (${swdeBuffMezz}) {
				/call check_for_adds
				/if (${addList.Count[|]}) /return
			}
			/call disc_${Me.Class.ShortName}
			/if (${swCombatDoT}) /call cast_DOT
			/if (${swdeBuffMezz}) {
				/call check_for_adds
				/if (${addList.Count[|]}) /return
			}
			/if (${swCombatNuke}) /call cast_nuke
			/if (${swdeBuffMezz}) {
				/call check_for_adds
				/if (${addList.Count[|]}) /return
			}
			/if (${swTerror}) /call check_buffs_tmp_list "${spellTerror}" "${lstTerrorToon}"
			/if (${swAlliance}) /call trigger_alliance ${Target2Kill}
			/call click_BP
			/call click_item
			/if (${swSwarm}) /call click_item_swarm ${Target2Kill}
			/call click_DPS_${Me.Class.ShortName}
		}

		/if (${coreAuto}) {	
			/if (${validate_class[FALSE, ${lstClassCast}]}) /call use_mod_rod
			| /if (${switchCharmPet} && ${swPet} && !${Me.Pet.ID}) /call find_mob_to_charm
			/if (${swBuffItem}) /call check_crystals
		}
	}
/return TRUE



|***
 *	DES: Keeps chanter runed. cause the're fragile and shit.
 *	USE: /call check_rune or /rb
 *  NOTE: 
 ***|
sub check_rune_ENC()
	DEBUG check_rune_ENC()
	/call timer_update timer_check_runes ${tCheck_Runes}
	AMIDEAD
	CHECKEXIT
	CHECKTIE
	UPDATEHUD "BUFF Rune"

	/if (${validate_cast[FALSE, alt, "${runeVeil}", ${Me.ID}]}) /call core_cast2 "${runeVeil}" alt 0 FALSE
	/if (${validate_cast[FALSE, alt, "${runeEldritch}", ${Me.ID}]}) /call core_cast2 "${runeEldritch}" alt 0 FALSE
	/if (${validate_cast[FALSE, alt, "${runeSpray}", ${Me.ID}]}) /call core_cast2 "${runeSpray}" alt 0 FALSE
	/if (${validate_cast[FALSE, alt, "${runeReactive}", ${Me.ID}]}) /call core_cast2 "${runeReactive}" alt 0 FALSE
	/if (${validate_cast[FALSE, alt, "${runeMind}", ${Me.ID}]}) /call core_cast2 "${runeMind}" alt 0 FALSE
	/if (${validate_cast[FALSE, spell, "${runeAuspice}", ${Me.ID}]} && ${FindItemCount[Peridot]}) /call core_cast2 "${runeAuspice}" 0 ${Me.ID} FALSE
	
	/if (${validate_cast[FALSE, spell, "${runeWard}", ${Me.ID}]}) /call core_cast2 "${runeWard}" ${buffGem} 0 FALSE
	/if (${validate_cast[FALSE, spell, "${runePoly}", ${Me.ID}]}) /call core_cast2 "${runePoly}" ${buffGem} ${Me.ID} FALSE
	/if (${validate_cast[FALSE, spell, "${runeBrimstone}", ${Me.ID}]}) /call core_cast2 "${runeBrimstone}" ${buffGem} ${Me.ID} FALSE
	/if (${validate_cast[FALSE, spell, "${runeShieldofFate}", ${Me.ID}]}) /call core_cast2 "${runeShieldofFate}" ${buffGem} ${Me.ID} FALSE
	/if (${validate_cast[FALSE, spell, "${runeLegion}", ${Me.ID}]} && ${FindItemCount[Peridot]}) /call core_cast2 "${runeLegion}" ${buffGem} ${Me.ID} FALSE

	/if (${FindItemCount[Peridot]}) /call check_buffs_tmp_list "${runeAuspice}" "${runeAuspiceToon}"

	| epic 2.0
	/if (${validate_cast[FALSE, epicitem, "2", ${Me.ID}]}) /call core_cast2 "${${Me.Class.ShortName}Epic2}" item 0 FALSE


	UPDATEHUD FALSE
/return



|***
 *	DES: basic DPS discs
 *	USE: 
 *  NOTE: 
 ***|
sub disc_ENC()
	DEBUG disc_ENC()
	AMIDEAD
	CHECKEXIT
	CHECKINPUTS
	CHECKTIE
	
	| /if (${validate_cast[FALSE, alt, "Crippling Aurora", ${Me.ID}]}) /call core_cast2 "Crippling Aurora" alt 0 FALSE

	| swarm pet
	/if (${swSwarm}) {
		/if (${validate_cast[FALSE, alt, "Phantasmal Opponent", ${Me.ID}]} && (${Target.PctHPs} <= ${setSwarmEngagePct} || ${swOverrideEngagePct})) /call core_cast2 "Phantasmal Opponent" alt 0 FALSE
	}


	| /if (${validate_cast[FALSE, alt, "Chromatic Haze", ${Me.ID}]}) /call core_cast2 "Chromatic Haze" alt 0 FALSE
/return



|***
 *	DES: Keep the chanter stocked with self AA crystals
 *	USE: 
 *  NOTE: 
 ***|
sub check_crystals()
	DEBUG check_crystals()
	AMIDEAD
	CHECKEXIT
	CHECKINPUTS
	CHECKTIE

	/if (!${FindItem[${mannaCrystal}].ID}) {
		/if (${Bool[${mannaCrystal}]} && !${Cursor.Name.Equal[${mannaCrystal}]}) {
			/if (${validate_cast[FALSE, alt, "Azure Mind Crystal", ${Me.ID}]}) /call core_cast2 "Azure Mind Crystal" alt 0 check_for_adds
			/call sort_cursor TRUE
		}
	}

	/if (!${FindItem[${healCrystal}].ID}) {
		/if (${Bool[${healCrystal}]} && !${Cursor.Name.Equal[${healCrystal}]}) {
			/if (${validate_cast[FALSE, alt, "Sanguine Mind Crystal", ${Me.ID}]}) /call core_cast2 "Sanguine Mind Crystal" alt 0 check_for_adds
			/call sort_cursor TRUE
		}
	}

	/call sort_cursor TRUE

	/if (${switchGather}) { 
		
		| AA manna crystal
		/if (${Bool[${mannaCrystal}]}) {
			/if (${Me.PctMana} < ${GatherPercent}*.${tmpHealOverride} && ${Me.ItemReady[${mannaCrystal}]}) {
				| /if (${validate_cast[FALSE, alt , "${mannaCrystal}", ${Me.ID}]}) {
					/call core_cast2 "${mannaCrystal}" item 0 FALSE
				| }
			}
		}

		| aa Gather Mana 
		/if (${Me.PctMana} < ${GatherPercent}*.${tmpHealOverride}) {
			/if (${validate_cast[FALSE, alt, "Gather Mana", ${Me.ID}]}) /call core_cast2 "Gather Mana" alt 0 FALSE
		}
	}

/return



|***
 *	DES: 
 *	USE: 
 *  NOTE: 
 ***|
sub click_DPS_ENC()
	DEBUG click_DPS_ENC()
	/call click_horn_of_unity 	
/return



|***
 *	DES: 
 *	USE: 
 *  NOTE: 
 ***|
sub check_heal_self_ENC()
	DEBUG check_heal_self_ENC()
	/if (${Me.PctHPs} > ${${Me.Class.ShortName}Point}*.${tmpHealOverride}) /return

	AMIDEAD
	CHECKEXIT
	CHECKTIE
	
	| AA Sanguine mind crystal summoned item 5 min
	/if (${Me.PctHPs} <= 80*.${tmpHealOverride} && ${Me.ItemReady[${healCrystal}]}) {
		/if (${validate_cast[FALSE, alt , "${healCrystal}", ${Me.ID}]}) /call core_cast2 "${healCrystal}" item 0 FALSE
	}	
	
	| 3 decoys 10 min
	/if (${Me.PctHPs} <= 70*.${tmpHealOverride} && ${validate_cast[FALSE, alt, "Doppelganger", ${Me.ID}]}) /call core_cast2 "Doppelganger" alt ${Me.ID} FALSE
	
	| rune and anti hate 20 min
	/if (${Me.PctHPs} <= 40*.${tmpHealOverride} && ${validate_cast[FALSE, alt, "Dimensional Shield", ${Me.ID}]}) /call core_cast2 "Dimensional Shield" alt 0 FALSE
	
/return



|***
 *	DES: 
 *	USE: 
 *  NOTE: 
 ***|
sub mez_adds()
	AMIDEAD
	CHECKINPUTS
	/declare _tmpID string local

	:loopmezzadds
	/if (${addList.Count[|]}) {
		/varset _tmpID ${addList.Arg[1,|]}
		/if (${_tmpID} == ${Target2Kill}) {
			/call list_delete addList ${_tmpID}
			/call list_delete mobList ${_tmpID}
			/goto :loopmezzadds
		}
		/if (!${Spawn[${_tmpID}].Type.Equal[NPC]}) {
			/call clear_timer ${_tmpID}
		} else {
			/squelch /target id ${_tmpID}
			/delay 5 ${Target.ID} == ${_tmpID}
			/if (${SpawnCount[npc los radius 30 zradius 15 targetable]} > 1 ) {
				
				VOUT ${sepO} ${cinfo}${SpawnCount[npc radius 30]}\ax ${sepC} ${If[${SpawnCount[npc los radius 15 targetable]} == 1,Target,Targets]} in point blank range.

				/if (${swdeBuffTash}) /call cast_tash ${_tmpID}
				/delay 2.25s ${Me.SpellReady[${Spell[${spellpbaeMez}].RankName}]}
				
				/if (${Me.SpellReady[${Spell[${spellpbaeMez}].RankName}]}) {
					/if (${Me.Casting.Name.NotEqual[${Spell[${spellpbaeMez}].RankName}]}) /stopcast
					/if (${validate_cast[FALSE, spell, "${spellpbaeMez}", ${Me.ID}]}) /call core_cast2 "${spellpbaeMez}" 0 0 check_for_adds
				}
				/if (${Me.SpellReady[${Spell[${spellStun}].RankName}]}) {
					/if (${Me.Casting.Name.NotEqual[${Spell[${spellStun}].RankName}]}) /stopcast
					/if (${validate_cast[FALSE, spell, "${spellStun}", ${Me.ID}]}) /call core_cast2 "${spellStun}" 0 0 check_for_adds
				}
			}
			
			| use AA beam of slumber?
			/if (${switchAABeamofSlumber}) {
				/if (${Spawn[${_tmpID}].NearestSpawn[3,npc radius 30].ID} && ${Spawn[${_tmpID}].Distance3D} <= 200 && ${Spawn[${_tmpID}].Distance3D} >= ${setMobAgro}*.5) {
					/if (${Me.AltAbilityReady[Beam of Slumber]}) {
						/if (${Me.Casting.ID}) /stopcast
						/face ID ${_tmpID} ${If[${swFaceFast},fast,]}
						/if (${validate_cast[FALSE, alt, "Beam of Slumber", ${Me.ID}]}) /call core_cast2 "Beam of Slumber" alt 0 FALSE
					} 
				}
			}
			
			/if (!${timer_mez_${_tmpID}}) /call cast_mez ${_tmpID}
		}
		/call list_delete addList ${_tmpID}
		| VOUT ${sepO} ${cinfo}${addList.Count[|]}\ax ${sepC} ${If[${addList.Count[|]} == 1,Target,Targets]} left to mez.
		/if (!${addList.Count[|]} && ${swBuffSelf} && !${timer_check_runes}) /call check_rune_${Me.Class.ShortName}
		/goto :loopmezzadds
	}
/return





|***
 *	DES: mez the target
 *	USE: debufs
 *  NOTE: 
 ***|
sub cast_mez(int _tmpID)
	DEBUG cast_mez(${_tmpID})
	/if (${timer_mez_${_tmpID}}) /return
	/declare _resistCount int local 0

	:loopmez
	AMIDEAD
	CHECKINPUTS
	/if (${ExcludeList.Find[${Spawn[${_tmpID}].DisplayName}]}) /return
	/if (${Spawn[${_tmpID}].Type.Equal[corpse]}) /return
	/if (!${Spawn[${_tmpID}].ID}) /return
	/if (!${Target.LineOfSight}) /return

	| check for mez immune list mobs
	/if (${NoMezzList.Find[${Spawn[ID ${_tmpID}].DisplayName}]}) {
	  /call timer_update timer_mez_${_tmpID} ${Spell[${spellMez}].Duration.TotalSeconds}m
	  | VOUT ${owarning} IMMUNE TO\ax ${csp}${spellMez}\ax ${sep} [${ctar}${Target.DisplayName}\ax]
	  /call list_delete addList ${_tmpID}	
		/return
	}
	
	/if (${Target.Mezzed.Duration} && ${Spell[${Target.Mezzed}].Name.Find[${spellMez}]}) {
		/call timer_update timer_mez_${_tmpID} ${Target.Mezzed.Duration.TotalSeconds}s
		/return
	} else /if (${Target.PctHPs} < 90) {
		/call timer_update timer_mez_${_tmpID} 5m
		VOUT [${ctar}${Target.DisplayName}\ax] is being attacked!
		/return
	}
	
	| mez ready?
	/delay 2.25s ${Me.SpellReady[${Spell[${spellMez}].RankName}]}
	/if (${validate_cast[FALSE, spell, "${spellMez}", ${_tmpID}]}) /call core_cast2 "${spellMez}" 0 ${_tmpID} check_for_adds
	/if (${Select[${castReturn},CAST_SUCCESS,CAST_IMMUNE,CAST_NOHOLD,CAST_TAKEHOLD]}) {
		/call timer_update timer_mez_${_tmpID} ${Spell[${spellMez}].Duration.Seconds}s
		/if (${Select[${castReturn},CAST_IMMUNE]}) {
		  /call timer_update timer_mez_${_tmpID} ${Spell[${spellMez}].Duration.TotalSeconds}m
		  /if (${NoMezzList.Find[${Spawn[ID ${_tmpID}].DisplayName}]}) {
			  | VOUT ${owarning} IMMUNE TO\ax ${csp}${spellMez}\ax ${sep} [${ctar}${Target.DisplayName}\ax]
			  ECHOCHANNEL MEZ IMMUNE to ${spellMez} ... ${Target.DisplayName}
			}
		  /call list_delete addList ${_tmpID}
		}
		/return
	} else /if (${Select[${castReturn},CAST_RESISTED,CAST_RESIST]}) {
		/varcalc _resistCount ${_resistCount}+1
		/if (${_resistCount}==2) /call cast_tash ${_tmpID}
	}
	| VOUT ${sepO} ${cinfo}${addList.Count[|]}\ax ${sepC} ${If[${addList.Count[|]} == 1,Target,Targets]} left to mez.

	/goto :loopmez
/return



|***
 *	DES: tashes the target
 *	USE: /call cast_tash [ID] | /debuff tash
 *  NOTE: 
 ***|
sub cast_tash(int _tmpID)
	DEBUG cast_tash(${_tmpID})
	/if (${timer_tash_${_tmpID}}) /return
	AMIDEAD
	CHECKEXIT
	CHECKTIE

	| if it's tashed, mark it and move on. better things to do
	/if (${Target.Tashed.ID}) {
		/call timer_update timer_tash_${_tmpID} ${Spell[${spellTash}].Duration.TotalSeconds}s		
		/return TRUE
	}
	
	| more then one mob
	/if (${SpawnCount[npc radius 49 playerstate 4]} >= 2 && ${Me.AltAbilityReady[Bite of Tashani]}) {
		/if (${validate_cast[FALSE, alt, "Bite of Tashani", ${Me.ID}]}) /call core_cast2 "Bite of Tashani" alt 0 FALSE
		
	| one mob at a time	
	} else /if (${SpawnCount[npc radius 49 playerstate 4]} == 1 || !${Me.AltAbilityReady[Bite of Tashani]}) {
		/if (${validate_cast[FALSE, spell, "${spellTash}", ${_tmpID}]}) /call core_cast2 "${spellTash}" 0 ${_tmpID} check_for_adds
		| set timer for tash.
		/if (${Select[${castReturn},CAST_SUCCESS,CAST_IMMUNE,CAST_RESISTED,CAST_RESIST,CAST_NOHOLD,CAST_TAKEHOLD]}) {
			/call timer_update timer_tash_${_tmpID} ${Spell[${spellTash}].Duration.TotalSeconds}s
		}
	}	

/return TRUE



|***
 *	DES: this will check each mob in area of the chanter. take note of its current animation and decide if it is attacking the enchanter.
 *	USE: 
 *  NOTE: this is not used in the code. reamnents from an old design left here.. because

sub is_attacking(int _tmpID)
	DEBUG is_attacking(${_tmpID})
	/declare _ani int local
	/varset _ani ${Spawn[${_tmpID}].Animation}
	
	/if (${_ani} == 5) /return TRUE
	/if (${_ani} == 7) /return TRUE
	/if (${_ani} == 8) /return TRUE
	/if (${_ani} == 12) /return TRUE
	/if (${_ani} == 13) /return TRUE
	/if (${_ani} == 42) /return TRUE
	/if (${_ani} == 44) /return TRUE
	/if (${_ani} == 80) /return TRUE
	/if (${_ani} == 106) /return TRUE
	/if (${_ani} == 110) /return TRUE
	/if (${_ani} == 129) /return TRUE
	/if (${_ani} == 144) /return TRUE
/return FALSE
 ***|


 |***
 *	DES: we need a target. specific for the enchanter
 *	USE: uses /setMA1-2 or assists
 *  NOTE: 
 *	----Need to work this out of the system. use get_assist_target instead..
 ***|
sub get_target_ENC()
	DEBUG get_target_ENC()
	/if (${Me.Moving}) /return
	/call get_assist_target
	/if (!${ExcludeList.Find[${Target.DisplayName}]}) {
		/if (${Spawn[${Target2Kill}].Type.Equal[npc]} && ${Spawn[${Target2Kill}].LineOfSight}) {
			/varset isValidTarget TRUE
			/if (${Defined[timer_mez_${Target2Kill}]}) /deletevar timer_mez_${Target2Kill}
			/call timer_update timer_mez_${Target2Kill} timer outer 600s
			/call list_add mobList ${Target2Kill}
		}
		/return
	}
	/varset isValidTarget FALSE
/return



|***
 *	DES: Enchanter burn routines
 *	USE: establish all comp/trigger skills
 *	NOTE: 
 ***| 
sub burn_ENC()
	DEBUG burn_ENC()
	CHECKEXIT
	CHECKINPUTS
	
	/if (${validate_cast[FALSE, alt, "${ENCIoG}", ${Me.ID}]}) /call core_cast2 "${ENCIoG}" alt 0 FALSE

	/call cast_AA_spire

	/if (${validate_cast[FALSE, alt, "Improved Twincast", ${Me.ID}]}) /call core_cast2 "Improved Twincast" alt 0 FALSE

	| # DD spell damage bonus
	/if (${validate_cast[FALSE, alt, "Calculated Insanity", ${Me.ID}]}) /call core_cast2 "Calculated Insanity" alt 0 FALSE

	| anti hate
	
	/if (${Me.AltAbilityReady[Silent Casting]}) {
		/if (${validate_cast[FALSE, alt, "Silent Casting", ${Me.ID}]}) /call core_cast2 "Silent Casting" alt 0 FALSE
	} else /if (!${Me.AltAbilityReady[Silent Casting]} && ${Me.AltAbilityReady[Dimensional Shield]}) {
		/if (${validate_cast[FALSE, alt, "Dimensional Shield", ${Me.ID}]}) /call core_cast2 "Dimensional Shield" alt 0 FALSE
	}

	| resist mod
	/if (${validate_cast[FALSE, alt, "Focus of Arcanum", ${Me.ID}]}) /call core_cast2 "Focus of Arcanum" alt 0 FALSE

	| Group DD Bonus
	/if (${validate_cast[FALSE, alt, "Chromatic Haze", ${Me.ID}]}) /call core_cast2 "Chromatic Haze" alt 0 FALSE
	
	/call use_glyph
	/call click_burn_circle
	/call click_item_burn
/return



|***
 *	DES: Charm Mobs. cause fucking charmed mobs and shits 
 *	USE: /call find_mob_to_charm()
 *	NOTE: 
 ***|   
sub find_mob_to_charm()
	DEBUG find_mob_to_charm()

	/declare _count int local 0
	/declare _spawns int local 0
	/declare _tmpID int local 0
	/declare _maxlvl int local ${Int[${Math.Calc[${Me.Level}-1]}]}
	
	/delay 1
	
	/if (${SpawnCount[npc los Range 1 ${_maxlvl} radius ${setMobAgro} targetable]}) {
		/varset _spawns ${SpawnCount[npc los Range 1 ${_maxlvl} radius ${setMobAgro} targetable]} 
		/for _count 1 to ${_spawns}
			/varset _tmpID ${NearestSpawn[${_count}, npc los Range 1 ${_maxlvl} radius ${setMobAgro} targetable].ID}
			/if (${CharmMobList.Find[${Spawn[id ${_tmpID}].DisplayName}]} && !${Spawn[${_tmpID}].Master.Type.Equal[Pet]} && !${Spawn[${_tmpID}].Master.Type.Equal[Corpse]}) {	
				ECHOCHANNEL Oh yeah found a mob to charm up in this bitch! Grabbing ${Spawn[id ${_tmpID}].DisplayName}

				/squelch /target ID ${_tmpID}
				/if (!${Target.Tashed.Duration}) /call cast_tash ${_tmpID}
				/if (${validate_cast[FALSE, spell, "${spellpbaeMez}", ${Me.ID}]}) /call core_cast2 "${spellpbaeMez}" 0 0 check_for_adds

				/if (${Target.Level} <= 46) {
					/if (${validate_cast[FALSE, alt, "Dire Charm", ${_tmpID}]}) /call core_cast2 "Dire Charm" alt ${_tmpID} FALSE					
				} else {
					/if (${validate_cast[FALSE, spell, "${spellCharm}", ${_tmpID}]}) /call core_cast2 "${spellCharm}" 0 ${_tmpID} FALSE
				}
				
				/if (${Defined[timer_mez_${_tmpID}]}) /deletevar timer_mez_${_tmpID}
				/break
			}
		/next _count
	}
/return



|***
 *	DES: Class controls
 *	USE: /enc []
 *  NOTE:
 ***|
#bind setENC /enc
sub Bind_setENC(string _type, string _verbage)
	/if (!${validate_class[TRUE, |ENC]}) /return

	/declare _sep bool local
	/declare _listout string local
	/declare _count int local 0
	/declare _bind string local ${Me.Class.ShortName.Lower}
  /declare _lstClass string local |bos|charm|gather|healpoint|spiregroup|spireraid|terror

	| charm pet
	/if (${_type.Equal[charm]}) {
		/call sql_update FALSE ${chrID} switchCharmPet ${If[${switchCharmPet},FALSE,TRUE]}  
		VOUT /${_bind} charm ${sep} ${If[${switchCharmPet},${oenabled},${odisabled}]}
		/return

	| AA Beam of slumber
	} else /if (${_type.Equal[bos]}) {
		/call sql_update FALSE ${chrID} switchAABeamofSlumber ${If[${switchAABeamofSlumber},FALSE,TRUE]}  

	| gather enable/disable /pct
	} else /if (${_type.Equal[gather]}) {
		/if (${Bool[${_verbage}]} && ${Range.Between[1,99:${Int[${_verbage}]}]}) {
			/call sql_update FALSE ${chrID} GatherPercent ${_verbage} 
		} else /if (${Bool[${_verbage}]} && !${Range.Between[1,99:${Int[${_verbage}]}]}) {
			VOUT /${_bind} gather [${cinfo}${GatherPercent}\ax] allowable range ${sep} ${cinfo}1 - 99\ax
			/return
		}	else /if (!${Bool[${_verbage}]}) {
			/call sql_update FALSE ${chrID} switchGather ${If[${switchGather},FALSE,TRUE]}  
		}			

	| spire control
	} else /if (${_type.Equal[spiregroup]} || ${_type.Equal[spireraid]}) {
		/call set_character_spire FALSE ${_type} ${_verbage}

	| terror spell		
	} else /if (${_type.Equal[terror]}) {
	 	/call set_switch ${chrID} swTerror ${_verbage}  

	| healpoint
	} else /if (${_type.Equal[healpoint]}) {
		/call set_character_healpoint FALSE ${_verbage}
		/return

	}

	/if (${swMQClear}) /mqclear
	/for _count 1 to ${_lstClass.Count[|]}
		/varset _listout ${_listout} ${If[${_sep},${sep},]} ${cinfo}${_lstClass.Arg[${_count},|]}\ax
		/if (${_lstClass.Arg[${_count},|].Equal[spiregroup]}) /varset _listout ${_listout}:${cnum}${spireGroup}\ax
		/if (${_lstClass.Arg[${_count},|].Equal[spireraid]}) /varset _listout ${_listout}:${cnum}${spireRaid}\ax
		/if (${_lstClass.Arg[${_count},|].Equal[healpoint]}) /varset _listout ${_listout}:${cnum}${${Me.Class.ShortName}Point}\ax
		/if (${_lstClass.Arg[${_count},|].Equal[gather]}) /varset _listout ${_listout}:${If[${switchGather},${cnum}${GatherPercent}\ax,${cbad}${GatherPercent}\ax]}
		/if (${_lstClass.Arg[${_count},|].Equal[bos]}) /varset _listout ${_listout}:${If[${Bool[${switchAABeamofSlumber}]},${oon},${ooff}]}
		/if (${_lstClass.Arg[${_count},|].Equal[terror]}) /varset _listout ${_listout}:${If[${swTerror},${oon},${ooff}]}
		/varset _sep TRUE
	/next _count
	VOUT /${_bind} [${_listout}]

/return

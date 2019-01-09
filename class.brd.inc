|***
 *	class.brd.inc
 *	Core Bard Include
 *
 *  
 ***|




|***
 *	DES: Main Bard Routine
 *	USE: 
 *  NOTE: 
 ***|
sub core_BRD()
	DEBUG core_BRD

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
		/if (${swHealSelf}) /call patch_heal_${Me.Class.ShortName}
		/if (${swHealCure}) /call cast_cure
		/if (${exit_class_loop[]}) /return FALSE
	
		/if (${coreAuto} && ${swdeBuffMezz}) {
			/call get_assist_target
			/if (${isValidTarget} && ${Target.ID} && ${Target.LineOfSight} && ${Target.Distance} <= ${setMobAgro} && (${Target.PctHPs} <= ${setEngagePct} || ${Spawn[PC ID ${Me.TargetOfTarget.ID} Radius 60].ID})) /goto :loop
			/if (${Defined[timer_mez_${Target2Kill}]}) /deletevar timer_mez_${Target2Kill}
			/call list_delete mobList ${Target2Kill}
			/return
			:loop
			/if (${Spawn[${Target2Kill}].Type.Equal[Corpse]}  || !${Target2Kill} || !${Target.ID} || ${Target.Type.Equal[corpse]} || !${Spawn[${Target2Kill}].ID}) /call get_assist_target
			/call check_for_adds
			/if (${addList.Count[|]}) /call mez_adds_BRD
			/call check_for_adds
			/if (${addList.Count[|]}) /return
			/if (${mobList.Find[|]}) /call list_update mobList ${Target2Kill}
			/call check_for_adds
			/if (${addList.Count[|]}) /return
		}
		
		/if (${isValidTarget}) {
			/call set_combat ${combat.Upper}
			/if (${swBurnControl}) /call burn_${Me.Class.ShortName}
			/if (${swSwarm} || ${swPet}) /call send_pet
			/if (${swCombatMelee}) /call use_skill_melee ${Target2Kill}
			/if (${swdeBuffMezz}) {
				/call check_for_adds
				/if (${addList.Count[|]}) /return
			}			
			/call disc_${Me.Class.ShortName}
			/call check_deBuffs ${Target2Kill}
			/if (${swCombatDoT}) /call cast_DOT
			/if (${swCombatNuke}) /call cast_nuke
			/call pulse
			/if (${setAlliance}) /call trigger_alliance ${Target2Kill}
			/if (${swdeBuffMezz}) {
				/call check_for_adds
				/if (${addList.Count[|]}) /return
			}		
			/call click_BP
			/call click_item
			/if (${swSwarm}) /call click_item_swarm ${Target2Kill}
			/call click_DPS_${Me.Class.ShortName}
	
			/if (${coreAuto}) {	
				/if (${switchAAFade} && ${Me.PctHPs} <= ${useAAFadePct}) /call cast_AA_fade
				/if (${swModRod} && ${validate_class[FALSE, |BRD]}) /call use_mod_rod
				/if (${validate_class[FALSE, ${lstClassMelee}]}) /call cast_stamina_recovery
			}
		} 
	}
/return TRUE




|***
 *	DES: Combat skills, Disciplines and Alternate abilities.
 *	USE: Disc
 *  NOTE: 
 ***|
sub disc_BRD()
	AMIDEAD
	CHECKEXIT
	CHECKINPUTS

	/if (${is_target_dead[${Target2Kill}]}) {
		/call pulse_stop
		/return
	}
	
	/if (${swdeBuffSlow} && !${timer_slow_${Target2Kill}}) /call pulse_slow ${Target2Kill}
	/call pulse_stop
	
	| this is a weapon from Sebilis Hardcore Heratige
	/if (!${Me.Song[Doben's Spry Sonata].ID}) {
		/if (${validate_cast[FALSE, item, "Darkened Breath of Harmony", ${Me.ID}]}) /call core_cast2 "Darkened Breath of Harmony" item 0 FALSE
	}
	
	/if (${swSwarm}) {
		/call cast_servant_swarm ${Target2Kill} FALSE
	}

	/if (${validate_cast[FALSE, alt, "Cacophony", ${Target2Kill}]}) /call core_cast2 "Cacophony" alt ${Target2Kill} FALSE
	/if (${validate_cast[FALSE, alt, "Bladed Song", ${Target2Kill}]}) /call core_cast2 "Bladed Song" alt ${Target2Kill} FALSE
	/if (${Spawn[${Target2Kill}].PctHPs} > 50) {
		/if (${validate_cast[FALSE, alt, "Boastful Bellow", ${Target2Kill}]}) /call core_cast2 "Boastful Bellow" alt ${Target2Kill} FALSE	
		/if (${is_target_dead[${Target2Kill}]}) {
			/call pulse_stop
			/return
		}
	}
	
	| trigger Frenzied kicks AA for bards if target is debuffed from intimidate
	/if (${switchIntimidKick}) {
		/if (${Bool[${intimidation}]} && ${Target.Buff[Harbinger's Intimidation].ID} && ${Spawn[${Target2Kill}].Distance} <= 15) {
			/if (${validate_cast[FALSE, alt, "Frenzied Kicks", ${Me.ID}]}) /call core_cast2 "Frenzied Kicks" alt 0 FALSE
		}
	}
	
/return TRUE



|***
 *	DES: emergency healing
 *	USE: /call patch_heal_${Me.Class.ShortName}
 *  NOTE: 
 ***|
sub patch_heal_BRD()
	AMIDEAD
	CHECKEXIT
	/if (${Me.PctHPs} > ${${Me.Class.ShortName}Point}*.${tmpHealOverride}) /return
	/call click_item_heal
/return TRUE



|***
 *	DES: DPS Clickies
 *	USE: /call click_DPS_${Me.Class.ShortName}
 *  NOTE: 
 ***|
sub click_DPS_BRD()
	AMIDEAD
	CHECKEXIT
	CHECKINPUTS
	/if (${is_target_dead[${Target2Kill}]}) {
		/call pulse_stop
		/return
	}
	/if (${swCombatMelee}) /call stick_to_target
	/call click_horn_of_unity 	
/return TRUE



|***
 *	DES: rotates bard swarm pets as DPS fillers
 *	USE: /call cast_servant_swarm
 *  NOTE: send in ALL THE PETS
 ***|
sub cast_servant_swarm(int _tmpID)
	AMIDEAD
	CHECKEXIT
	CHECKINPUTS
	/if (${is_target_dead[${Target2Kill}]}) {
		/call pulse_stop
		/return
	}
	
	/if (${Spawn[${_tmpID}].LineOfSight} && ${Target.PctHPs} > 10 && ${Spawn[${_tmpID}].Distance} < ${setMobAgro}) {
		/call get_target_2kill
		/if (${Target.ID}) /pet swarm

		/if (${Me.AltAbilityReady[${ServantNukeAA1}]}) {
			/call pulse_stop
			/if (${validate_cast[FALSE, alt, "${ServantNukeAA1}", ${_tmpID}]}) /call core_cast2 "${ServantNukeAA1}" alt ${_tmpID} FALSE
			/return
		} else /if (${Me.AltAbilityReady[${ServantNukeAA2}]}) {
			/call pulse_stop
			/if (${validate_cast[FALSE, alt, "${ServantNukeAA2}", ${_tmpID}]}) /call core_cast2 "${ServantNukeAA2}" alt ${_tmpID} FALSE
			/return
		} else /if (${Me.SpellReady[${songServantNuke}]}) {
			/call pulse_stop
			/if (${validate_cast[FALSE, pulse, "${songServantNuke}", ${_tmpID}]}) /call core_cast2 "${songServantNuke}" pulse ${_tmpID} FALSE
						/return
		}
	}
/return TRUE



|***
 *	DES: memorize songs
 *	USE: /call mem_song_line [] | /brd line [TANK|CASTER|MELEE|MIX|OTHER]
 *  NOTE:
 ***|
sub mem_song_line(string _line)
	AMIDEAD
	CHECKEXIT
	CHECKINPUTS
	/declare _count 				int local
	/declare _index 				int local	
	/declare _list[1] 			string ${line${lineCurrent}}
	/declare _currentSong 	string NULL
	
	/for _count 1 to ${_list.Size}
		/for _index 1 to ${_list[${_count}].Count[|]}
			/varset _currentSong ${Spell[${_list[${_count}].Arg[${_index},|]}].RankName}
			/if (${Me.Gem[${_index}].Name.Equal[${_currentSong}]}) /continue
			/call mem_spell TRUE "${_currentSong}" "${_index}" FALSE
		/next _index	
	/next _count	
	VOUT Line ${sep} ${cinfo}${lineCurrent}\ax memorization completed.
/return TRUE



|***
 *	DES: Sing "Pulse" a single song
 *	USE: /call pulse [gem##] 
 *  NOTE:
 ***|
sub pulse()
	/if (${is_target_dead[${Target2Kill}]}) {
		/call pulse_stop
		/return
	}

	/declare _pulseSong			bool local FALSE
	/declare _count 				int local
	/declare _index 				int local	
	/declare _list[1] 			string ${line${lineCurrent.Left[1].Upper}${lineCurrent.Right[-1].Lower}}

	UPDATEHUD "PULSE Song"
	
	/for _count 1 to ${_list.Size}
		/for _index 1 to ${_list[${_count}].Count[|]}
			/if (${is_target_dead[${Target2Kill}]}) {
				/call pulse_stop
				/return
			}

			AMIDEAD
			CHECKINPUTS
						
			/if (${validate_cast[FALSE, pulse, "${Spell[${_list[${_count}].Arg[${_index},|]}].RankName}", ${Me.ID}]}) /call core_cast2 "${Spell[${_list[${_count}].Arg[${_index},|]}].RankName}" pulse 0 FALSE
		/next _index	
	/next _count	

	UPDATEHUD FALSE
/return TRUE



|***
 *	DES: quit singing. your voice sounds like ass.
 *	USE: 
 *  NOTE: 
 ***|
sub pulse_stop()
	/if (${Me.Casting.ID} || ${Me.BardSongPlaying}) {
		/melody stop
		/stopsong
		/stopcast
		/delay 2s !${Me.Casting.ID}
	}	
/return TRUE



|***
 *	DES: 
 *	USE: 
 *  NOTE: 
 ***|
sub mez_adds_BRD()
	AMIDEAD

	CHECKINPUTS
	/declare _tmpID string local

	:loopmezzadds
	/if (${addList.Count[|]} && ${Me.CurrentMana} >= ${Spell[${spellMez}].Mana}) {
		/varset _tmpID ${addList.Arg[1,|]}
		/if (${_tmpID} == ${Target2Kill}) {
			/call list_delete addList ${_tmpID}
			/goto :loopmezzadds
		}
		/if (!${Spawn[${_tmpID}].Type.Equal[NPC]}) {
			/call clear_timer ${_tmpID}
		} else {
			/squelch /target id ${_tmpID}
			/delay 5 ${Target.ID} == ${_tmpID}
			/delay 4s ${Me.SpellReady[${spellpbaeMez}]}
			/if (${SpawnCount[npc los radius 30 setzradius 15 targetable]} > 3 ) {
				VOUT ${sepO} ${cinfo}${SpawnCount[npc radius 30 setzradius 15]}\ax ${sepC} Targets in point blank range.
				/if (${Me.Gem[${spellpbaeMez}]} && ${Me.SpellReady[${spellpbaeMez}]}) {
					/if (${Me.Casting.Name.NotEqual[${spellpbaeMez}]}) /call pulse_stop
					/if (${validate_cast[FALSE, pulse, "${spellpbaeMez}", ${Me.ID}]}) /call core_cast2 "${spellMez}" pulse 0 FALSE

				}
			}
			/call cast_mez_BRD ${_tmpID}
		}
		/call list_delete addList ${_tmpID}
		VOUT ${sepO} ${cinfo}${addList.Count[|]}\ax ${sepC} ${If[${addList.Count[|]} == 1,Target,Targets]} left to mez.
		/goto :loopmezzadds
	}
/return TRUE



|***
 *	DES: mez the target
 *	USE: debufs
 *  NOTE: 
 ***|
sub cast_mez_BRD(int _tmpID)

	:loopmez
	AMIDEAD

	/if (!${Target.LineOfSight} || ${ExcludeList.Find[${Target.DisplayName}]} || ${Spawn[${_tmpID}].Type.Equal[corpse]} || !${Spawn[${_tmpID}].ID}) /return

	| check for mez immune list mobs
	/if (${NoMezzList.Find[${Spawn[ID ${_tmpID}].CleanName}]}) {
	  /call timer_update timer_mez_${_tmpID} ${Spell[${spellMez}].Duration.TotalSeconds}m
	  VOUT ${owarning} IMMUNE TO\ax ${csp}${spellMez}\ax ${sep} [${ctar}${Target.CleanName}\ax]
	  /call list_delete addList ${_tmpID}	
		/return
	}
	
	/if (${Target.Mezzed.Duration} && ${Spell[${Target.Mezzed}].Name.Find[${spellMez}]}) {
		/call timer_update timer_mez_${_tmpID} ${Target.Mezzed.Duration.TotalSeconds}s
		/return
	} else /if (${Target.PctHPs} < 90) {
		/call timer_update timer_mez_${_tmpID} 10m
		VOUT [${ctar}${Target.CleanName}\ax] is being attacked!
		/return
	}
	/if (${Me.CurrentMana}>${Spell[${spellMez}].Mana}) {
		/delay 6s ${Me.SpellReady[${spellMez}]}
		/target ID ${_tmpID}
		/delay 5 ${Target.ID} == ${_tmpID}
		/if (${validate_cast[FALSE, pulse, "${spellMez}", ${_tmpID}]}) /call core_cast2 "${spellMez}" pulse ${_tmpID} FALSE
		/if (${Target.Mezzed.Duration.TotalSeconds}) /call timer_update timer_mez_${_tmpID} ${Spell[${spellMez}].Duration.Seconds}s
		/goto :loopmez
	}
/return TRUE



|***
 *	DES: Bard AA version of Breather
 *	USE: /call cast_AA_Rallying
 *  NOTE: 
 *  AA - Rallying Solo
 *  Song - Rallying Solo Azia V
 ***|
sub cast_AA_Rallying()
 	/if (${SafeZone} && !${swOverrideSafeZone} || !${Me.AltAbilityReady[Rallying Solo]}) /return
	AMIDEAD
	CHECKINPUTS
 	/declare _endurancePoint int local 30
 	/declare _manaPoint int local 30

 	/if (((${Me.PctEndurance} < ${_endurancePoint}) || (${Me.PctMana} < ${_manaPoint})) && ${Select[${Me.CombatState},RESTING]}) {
		/if (${validate_cast[FALSE, alt, "Rallying Solo", ${Me.ID}]}) /call core_cast2 "Rallying Solo" alt ${Me.ID} FALSE
	}
/return TRUE



 |***
 *	DES: Bard burn routines
 *	USE: establish all comp/trigger skills
 *	NOTE: 
 ***| 
sub burn_BRD()
	AMIDEAD
	CHECKEXIT
	CHECKINPUTS
	/if (${is_target_dead[${Target2Kill}]}) {
		/call pulse_stop
		/return
	}

	/if (${Me.Casting.ID}) /call pulse_stop
	
	| epic 2.0
	/if (${validate_cast[FALSE, epicitem, "2", ${Me.ID}]} && ${Spawn[${Target2Kill}].Distance} <= ${setCombatDistance}) /call core_cast2 "${${Me.Class.ShortName}Epic2}" item 0 FALSE
	
 	/if (${validate_cast[FALSE, alt, "Bladed Song", ${Me.ID}]}) /call core_cast2 "Bladed Song" alt 0 FALSE
 	/if (${validate_cast[FALSE, alt, "Thousand Blades", ${Me.ID}]}) /call core_cast2 "Thousand Blades" disc 0 FALSE	
	/if (${validate_cast[FALSE, alt, "Fierce Eye", ${Me.ID}]}) /call core_cast2 "Fierce Eye" alt 0 FALSE
	/call cast_AA_spire
	/if (${validate_cast[FALSE, alt, "Funeral Dirge", ${Me.ID}]}) /call core_cast2 "Funeral Dirge" alt 0 FALSE
	/if (${validate_cast[FALSE, alt, "Quick Time", ${Me.ID}]} && !${Me.Song[Ruaabri's Fury].ID}) /call core_cast2 "Quick Time" alt 0 FALSE

	/call use_glyph
	/call click_burn_circle
	/call click_item_burn	
/return TRUE



|***
 *	DES: AA Slow
 *	USE: /call pulse_slow [TARGET ID]
 *  NOTE: 
 ***|
sub pulse_slow(int _tmpID)
	/if (${timer_slow_${_tmpID}}) /return
	AMIDEAD
	CHECKEXIT
	CHECKINPUTS
	/if (${is_target_dead[${Target2Kill}]}) {
		/call pulse_stop
		/return
	}
		
	| dont waste time if its slowed already
	/if (${Target.Slowed.Duration}) {
		/call timer_update timer_slow_${_tmpID} ${Target.Slowed.Duration.TotalSeconds}s
		/return
	}
	
	/if (${validate_cast[FALSE, pulse, "${spellSlow}", ${_tmpID}]}) /call core_cast2 "${spellSlow}" pulse ${_tmpID} FALSE

/return TRUE



|***
 *	DES: issues the bard commands for running around
 *	USE: /dex BARDNAME /travel
 *  NOTE: 
 ***|
#bind brd_travel /travel
sub Bind_brd_travel()
	/if (!${validate_class[TRUE, |BRD]}) /return
	
	/if (${validate_cast[FALSE, alt, "Selo's Sonata", ${Me.ID}]}) /call core_cast2 "Selo's Sonata" alt 0 FALSE
	/delay 1
	/if (${validate_cast[FALSE, alt, "Shauri's Sonorous Clouding", ${Me.ID}]}) /call core_cast2 "Shauri's Sonorous Clouding" alt 0 FALSE
	
/return



|***
 *	DES: Class controls
 *	USE: /brd []
 *  NOTE:
 ***|
#bind setBRD /brd
sub Bind_setBRD(string _type, string _verbage)
	/if (!${validate_class[TRUE, |BRD]}) /return

	/declare _sep bool local
	/declare _listout string local
	/declare _count int local 0
	/declare _bind string local ${Me.Class.ShortName.Lower}
  /declare _lstClass string	local |fade|healpoint|intimidkick|line|restsong|rod|spiregroup|spireraid

	| enable/disable rest song
	/if (${_type.Equal[restsong]}) {
		/call sql_update FALSE ${Me.Class.ShortName} switchRestSong ${If[${switchRestSong},FALSE,TRUE]}
		VOUT /${_bind} restsong ${sep} ${If[${switchRestSong},${oenabled},${odisabled}]}

	| use Modulation Rods
	} else /if (${_type.Equal[rod]}) {
		/call sql_update FALSE ${Me.Class.ShortName} swModRod ${If[${swModRod},FALSE,TRUE]} 	

	| set AA fade use %	
	} else /if (${_type.Equal[fade]}) {
		/call set_character_fade FALSE ${_verbage}

	| enable/disable sets bard song line
	} else /if (${_type.Equal[line]}) {
		/if (${Bool[${lineType.Find[${_verbage.Lower}]}]}) {
			/call sql_update FALSE ${Me.Class.ShortName} lineCurrent ${_verbage.Left[1].Upper}${_verbage.Right[-1].Lower}
			/delay 2
			/call mem_song_line ${lineCurrent}
		}
		/for _count 1 to ${lineType.Count[|]}
			/varset _listout ${_listout} ${If[${_sep},${sep},]} ${If[${lineCurrent.Equal[${lineType.Arg[${_count},|]}]},${cgood},${cinfo}]}${lineType.Arg[${_count},|]}\ax   
			/varset _sep TRUE
		/next _count
		VOUT /${_bind} line [${_listout}]	
		/return

	| intimidation/frenzied kick control
	} else /if (${_type.Equal[intimidkick]}) {
		/call sql_update FALSE ${Me.Class.ShortName} switchIntimidKick ${If[${switchIntimidKick},FALSE,TRUE]}
		VOUT /${_bind} intimidkick ${sep} ${If[${switchIntimidKick},${oenabled},${odisabled}]}

	| spire control
	} else /if (${_type.Equal[spiregroup]} || ${_classType.Equal[spireraid]}) {
		/call set_character_spire FALSE ${_type} ${_verbage}

	| healpoint
	} else /if (${_type.Find[healpoint]}) {
		/call set_character_healpoint FALSE ${_verbage}
		/return

	}

	/if (${swMQClear}) /mqclear
	/for _count 1 to ${_lstClass.Count[|]}
		/varset _listout ${_listout} ${If[${_sep},${sep},]} ${cinfo}${_lstClass.Arg[${_count},|]}\ax
		/if (${_lstClass.Arg[${_count},|].Equal[fade]}) /varset _listout ${_listout}:${If[${switchAAFade},${cnum}${useAAFadePct}\ax,${cbad}${useAAFadePct}\ax]}
		/if (${_lstClass.Arg[${_count},|].Equal[healpoint]}) /varset _listout ${_listout}:${cnum}${${Me.Class.ShortName}Point}\ax
		/if (${_lstClass.Arg[${_count},|].Equal[line]}) /varset _listout ${_listout}:${cnum}${lineCurrent}\ax
		/if (${_lstClass.Arg[${_count},|].Equal[rod]}) /varset _listout ${_listout}:${If[${Bool[${swModRod}]},${oon},${ooff}]}
		/if (${_lstClass.Arg[${_count},|].Equal[spiregroup]}) /varset _listout ${_listout}:${cnum}${spireGroup}\ax
		/if (${_lstClass.Arg[${_count},|].Equal[spireraid]}) /varset _listout ${_listout}:${cnum}${spireRaid}\ax

		/varset _sep TRUE
	/next _count
	VOUT /${_bind} [${_listout}]
/return TRUE



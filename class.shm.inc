|***
 *	class.shm.inc
 *	Core Shaman Include
 *
 *
 ***|



|***
 *	DES: Main Shaman Routine
 *	USE: 
 *  NOTE: 
 ***|
sub core_SHM()
	DEBUG core_SHM()
	CHECKINPUTS
	AMIDEAD
	CHECKTIE
	
	| correct any and all XTarget healing spots
	/if (${coreAuto}) {
	  /if (${swHealXTarget}) {
	  	/if (!${timer_set_xtarget}) {
		    /call set_xtarget
		    /call timer_update timer_set_xtarget ${tXHeal}
		  }	
		}
		
		/if (!${Me.Invis}) {
			/call check_cani
			/if (!${check_engaged[]} && ${swPet}) /call check_pet
		}
	}	

	/while (${prep_combat[]}) {
		CHECKINPUTS
		/if (${swHealSelf} && !${Group.Members}) /call check_heal_self_${Me.Class.ShortName}
		/if (${swHealGroup} && ${Group.Members}) /call check_heal_group_${Me.Class.ShortName}
		/if (${swHealXTarget}) /call check_heal_XTarget_${Me.Class.ShortName}
		/if (!${timer_check_cast_cure} && ${swHealCure}) /call cast_cure FALSE
		/if (${swHealPet}) /call check_heal_pet_${Me.Class.ShortName}
		/if (${exit_class_loop[]}) /return

		/if (${isValidTarget}) {
			/call set_combat ${combat.Upper}
			/if (${swBurnControl}) /call burn_${Me.Class.ShortName}
			/if (${swCombatMelee}) /call use_skill_melee ${Target2Kill}
			/call disc_${Me.Class.ShortName}
			/call check_deBuffs ${Target2Kill}
			/if (${swCombatDoT}) /call cast_DOT
			/if (${swCombatNuke}) /call cast_nuke
			/if (${swAlliance}) /call trigger_alliance ${Target2Kill}
			/if (${swSwarm} || ${swPet}) /call send_pet
			/call click_BP
			/call click_item
			/if (${swSwarm}) /call click_item_swarm ${Target2Kill}
			/call click_DPS_${Me.Class.ShortName}
		}

		/if (${coreAuto}) {
			/if (${switchAAFade} && ${Me.PctHPs} <= ${useAAFadePct}) /call cast_AA_fade
			/call check_cani	
			/if (${swModRod} && ${validate_class[FALSE, ${lstClassCast}]}) /call use_mod_rod
			/if (${validate_class[FALSE, ${lstClassMelee}]}) /call cast_stamina_recovery
		}
	}

/return


|***
 *	DES: Combat skills, Disciplines and Alternate abilities.
 *	USE: Disc
 *  NOTE: 
 ***|
sub disc_SHM()
	DEBUG disc_SHM()
	CHECKEXIT
	CHECKINPUTS
	/if (${switchTalisman}) /call cast_talisman
/return



|***
 *	DES: DPS Clickies
 *	USE: this shits auto, learn to love it.
 *  NOTE: 
 ***|
sub click_DPS_SHM()
	DEBUG click_DPS_SHM()
	CHECKEXIT
	CHECKINPUTS
	/call click_horn_of_unity 	
/return
	


|***
 *	DES: 
 *	USE: 
 *  NOTE: 
 ***|
sub check_heal_group_SHM()
	DEBUG check_heal_group_SHM()
	AMIDEAD
	CHECKEXIT
	CHECKTIE

	/declare _count int local 0
	/varset hurtIndex ${Group.Member[${Me}].Index}
	/varset hurtPctHP ${If[${setHealOverride},${Float[${setHealOverride}].Centi},99.99]}
	/varset hurtCLS ${Me.Class.ShortName}
	/varset hurtCount 0

	| Do group members need healing? 
  /call check_health_group
  	
	CHECKEXIT
	UPDATEHUD "HEAL Group"
	CHECKREZ
	/if (${switchAAFade} && ${Me.PctHPs} <= ${useAAFadePct}) /call cast_AA_fade
	
	
	| Group regeneration
 	/if (${useGroupHoT} && !${hurtCount}) { 
 		/if (${SpawnCount[npc radius ${setMobAgro} playerstate 4 targetable]}) {
	 	 	/if (${Me.SpellReady[${Spell[${healWholeGroupHoT}].RankName}]} && !${Bool[${Me.Song[Celestial Regeneration]}]} && !${Bool[${Me.Song[${healWholeGroupHoT}]}]} && !${Bool[${Me.Song[Elixir of the Seas]}]} && !${Bool[${Me.Song[Elixir of Wulthan]}]}) {
		    /if (${validate_cast[FALSE, spell, "${healWholeGroupHoT}", ${Me.ID}]}) /call core_cast2 "${healWholeGroupHoT}" 0 ${Me.ID} FALSE
		  }
		}
	}	
	
	| no one is hurt. we're out of here
	/if (!${hurtCount}) {
	  | check for surge
	  /if (${switchSurge}) {
			/if (${SpawnCount[npc radius ${setMobAgro} playerstate 4 targetable]}) /call cast_surge_heal
		}	
		UPDATEHUD FALSE
		/return
	}

	CHECKREZ
	
	/call check_no_cast_buff

  | stationary ward heal
  /if (${hurtCount} >= 3 && ${Me.AltAbilityReady[Call of the Ancients]}) {
    /if (${validate_cast[FALSE, alt, "Call of the Ancients", ${Me.ID}]}) /call core_cast2 "Call of the Ancients" alt ${Me.ID} FALSE
  } 
  
	| Panic Ancestral Guard
	/if (${Me.PctHPs} < ${PanicPoint}*.${tmpHealOverride}) {
		/if (${Me.Casting.ID}) /stopcast
		/if (${validate_cast[FALSE, alt, "Ancestral Guard", ${Me.ID}]}) /call core_cast2 "Ancestral Guard" alt 0 FALSE
	}
  
	| aggressive switch sort 	
  /if (${switchHealAggressive}) {
 		/if (${Me.SpellReady[${Spell[${healReckless1}].RankName}]} || ${Me.SpellReady[${Spell[${healReckless2}].RankName}]} || ${Me.SpellReady[${Spell[${healReckless3}].RankName}]}) /goto :aggressivesort
 	} 	
 	
	| intervention/main heals
	/if (${Group.Member[${hurtIndex}].PctHPs} <= ${${hurtCLS}Point}*.${tmpHealOverride}) {
		/if (${Me.SpellReady[${Spell[${healIntervention1}].RankName}]}) {
			/if (${validate_cast[FALSE, spell, "${healIntervention1}", ${Group.Member[${hurtIndex}].ID}]}) /call core_cast2 "${healIntervention1}" 0 ${Group.Member[${hurtIndex}].ID} FALSE
		} else /if (${Me.SpellReady[${Spell[${healMain}].RankName}]}) {
			/if (${validate_cast[TRUE, spell, "${healMain}", ${Group.Member[${hurtIndex}].ID}]}) /call core_cast2 "${healMain}" 0 ${Group.Member[${hurtIndex}].ID} FALSE
		}
	}
  
 	| Reckless Heals
	:aggressivesort
  /if (${Group.Member[${hurtIndex}].PctHPs} <= ${${hurtCLS}Point}*.${tmpHealOverride}) {
		/for _count 1 to 3
			/if (${validate_cast[FALSE, spell, "${healReckless${_count}}", ${Group.Member[${hurtIndex}].ID}]}) /call core_cast2 "${healReckless${_count}}" 0 ${Group.Member[${hurtIndex}].ID} FALSE
			/break
		/next _count 
	}
	
	| check for personal debuffs
	/if (${swHealCure}) {
		/if (${Debuff.Count}) {
			| AA Radient Cure SELF ONLY 100' range 1 min
			/if (${Me.AltAbilityReady[Radiant Cure]} && (${Debuff.Diseased} || ${Debuff.Poisoned} || ${Debuff.Cursed})) {
				/if (${validate_cast[FALSE, alt, "Radiant Cure", ${Me.ID}]}) /call core_cast2 "Radiant Cure" alt ${Me.ID} FALSE
			}
			| AA Purified Spirits SELF ONLY 0' range 2 min
			/if (${Me.AltAbilityReady[Purified Spirits]} && ${Debuff.Count}) {
				/if (${validate_cast[FALSE, alt, "Purified Spirits", ${Me.ID}]}) /call core_cast2 "Purified Spirits" alt ${Me.ID} FALSE
			}
		}  
	}
  
  | check for surge
  /if (${switchSurge}) {
		/if (${SpawnCount[npc radius ${setMobAgro} playerstate 4 targetable]}) /call cast_surge_heal
	}

	UPDATEHUD FALSE
/return



|***
 *	DES: Heals xtarget people
 *	USE: 
 *  NOTE: 
 ***|
sub check_heal_XTarget_SHM()
	DEBUG check_heal_XTarget_SHM()
	/if (!${validate_expansion[FALSE, |16]}) /return
	CHECKREZ
	AMIDEAD
	SIMPLEEXIT
	CHECKINPUTS
	UPDATEHUD "HEAL XTarget"
		
	/declare _count int local 0
	/varset hurtIndex				${Group.Member[${Me}].Index}
	/varset hurtPctHP 			${If[${setHealOverride},${Float[${setHealOverride}].Centi},99.99]}
	/varset hurtCLS 				${Me.Class.ShortName}
	/varset hurtCount				0

	| does anyone need healing?
	/call check_health_xtarget
  	
	/if (!${hurtCount}) {
		UPDATEHUD FALSE
		/return	
	}

	/call check_no_cast_buff

	| aggressive switch sort 	
  /if (${switchHealAggressive}) {
 		/if (${Me.SpellReady[${Spell[${healReckless1}].RankName}]} || ${Me.SpellReady[${Spell[${healReckless2}].RankName}]} || ${Me.SpellReady[${Spell[${healReckless3}].RankName}]}) /goto :aggressivesort
 	} 	 	
 	
	| intervention/main heals
	/if (${Me.XTarget[${hurtIndex}].PctHPs} <= ${${hurtCLS}Point}*.${tmpHealOverride}) {
		/if (${Me.SpellReady[${Spell[${healIntervention1}].RankName}]}) {
			/if (${validate_cast[FALSE, spell, "${healIntervention1}", ${Me.XTarget[${hurtIndex}].ID}]}) /call core_cast2 "${healIntervention1}" 0 ${Me.XTarget[${hurtIndex}].ID} FALSE
		} else /if (${Me.SpellReady[${Spell[${healMain}].RankName}]}) {
			/if (${validate_cast[FALSE, spell, "${healMain}", ${Me.XTarget[${hurtIndex}].ID}]}) /call core_cast2 "${healMain}" 0 ${Me.XTarget[${hurtIndex}].ID} FALSE
		} 
	} 

	| reckless heals
	:aggressivesort
	/if (${Me.XTarget[${hurtIndex}].PctHPs} <= ${${hurtCLS}Point}*.${tmpHealOverride}) {
  	/if (${Me.SpellReady[${Spell[${healReckless1}].RankName}]}) {
    	/if (${validate_cast[FALSE, spell, "${healReckless1}", ${Me.XTarget[${hurtIndex}].ID}]}) /call core_cast2 "${healReckless1}" 0 ${Me.XTarget[${hurtIndex}].ID} FALSE
    } else /if (${Me.SpellReady[${Spell[${healReckless2}].RankName}]}) {
      /if (${validate_cast[FALSE, spell, "${healReckless2}", ${Me.XTarget[${hurtIndex}].ID}]}) /call core_cast2 "${healReckless2}" 0 ${Me.XTarget[${hurtIndex}].ID} FALSE
    } else /if (${Me.SpellReady[${Spell[${healReckless3}].RankName}]}) {
      /if (${validate_cast[FALSE, spell, "${healReckless3}", ${Me.XTarget[${hurtIndex}].ID}]}) /call core_cast2 "${healReckless3}" 0 ${Me.XTarget[${hurtIndex}].ID} FALSE
    } 
  }
		
	UPDATEHUD FALSE  
/return



|***
 *	DES: Checks and Heals Pets. Group ONLY
 *	USE: /heal pet
 *  NOTE: 
 ***|
sub check_heal_pet_SHM()
	DEBUG check_heal_pet_SHM()
	AMIDEAD
	SIMPLEEXIT
	CHECKINPUTS
	CHECKREZ
	UPDATEHUD "HEAL Pet"

	/declare _count int local 0
	
	
	/if (!${Group}) {
		/if (${Me.Pet.PctHPs} >= ${PetPoint}) /return FALSE

		| aggressive switch sort 	
	  /if (${switchHealAggressive}) {
	 		/if (${Me.SpellReady[${Spell[${healReckless1}].RankName}]} || ${Me.SpellReady[${Spell[${healReckless2}].RankName}]} || ${Me.SpellReady[${Spell[${healReckless3}].RankName}]}) /goto :aggressivesolosort
	 	} 	 	
	 	
		| intervention/main heals
		/if (${Me.Pet.PctHPs} <= ${PetPoint}) {
			/if (${Me.SpellReady[${Spell[${healIntervention1}].RankName}]}) {
				/if (${validate_cast[FALSE, spell, "${healIntervention1}", ${Me.Pet.ID}]}) /call core_cast2 "${healIntervention1}" 0 ${Me.Pet.ID} FALSE
			} else /if (${Me.SpellReady[${Spell[${healMain}].RankName}]}) {
				/if (${validate_cast[FALSE, spell, "${healMain}", ${Me.Pet.ID}]}) /call core_cast2 "${healMain}" 0 ${Me.Pet.ID} FALSE
			} 
		} 
	  
	 	| Reckless Heals
		:aggressivesolosort
	  /if (${Group.Member[${hurtIndex}].Pet.PctHPs} <= ${PetPoint}) {
			/for _count 1 to 3
				/if (${validate_cast[FALSE, spell, "${healReckless${_count}}", ${Me.Pet.ID}]}) /call core_cast2 "${healReckless${_count}}" 0 ${Me.Pet.ID} FALSE
				/break
			/next _count 
		}

	} else {
	
		/varset hurtIndex ${Group.Member[${Me}].Index}
	  
		/call check_health_pet

		| any pets hurt?
	  /if (${Group.Member[${hurtIndex}].ID} == ${Me.ID}) {
	 		UPDATEHUD FALSE
	 		/return
		}
		
		| aggressive switch sort 	
	  /if (${switchHealAggressive}) {
	 		/if (${Me.SpellReady[${Spell[${healReckless1}].RankName}]} || ${Me.SpellReady[${Spell[${healReckless2}].RankName}]} || ${Me.SpellReady[${Spell[${healReckless3}].RankName}]}) /goto :aggressivesort
	 	} 	 	
	 	
		| intervention/main heals
		/if (${Group.Member[${hurtIndex}].Pet.PctHPs} <= ${PetPoint}) {
			/if (${Me.SpellReady[${Spell[${healIntervention1}].RankName}]}) {
				/if (${validate_cast[FALSE, spell, "${healIntervention1}", ${Group.Member[${hurtIndex}].Pet.ID}]}) /call core_cast2 "${healIntervention1}" 0 ${Group.Member[${hurtIndex}].Pet.ID} FALSE
			} else /if (${Me.SpellReady[${Spell[${healMain}].RankName}]}) {
				/if (${validate_cast[FALSE, spell, "${healMain}", ${Group.Member[${hurtIndex}].Pet.ID}]}) /call core_cast2 "${healMain}" 0 ${Group.Member[${hurtIndex}].Pet.ID} FALSE
			} 
		} 
	  
	 	| Reckless Heals
		:aggressivesort
	  /if (${Group.Member[${hurtIndex}].Pet.PctHPs} <= ${PetPoint}) {
			/for _count 1 to 3
				/if (${validate_cast[FALSE, spell, "${healReckless${_count}}", ${Group.Member[${hurtIndex}].Pet.ID}]}) /call core_cast2 "${healReckless${_count}}" 0 ${Group.Member[${hurtIndex}].Pet.ID} FALSE
				/break
			/next _count 
		}
	}
	
	UPDATEHUD FALSE
/return



|***
 *	DES: surge heal
 *	USE: part of heal routine
 *  NOTE: this will try and get a twinheal proc first.
 ***|
sub cast_surge_heal()
	DEBUG cast_surge_heal()
	/if (!${Bool[${TC1}]} && !${Bool[${TC2}]}) /return
	/if (!${Me.SpellReady[${Spell[${TC1}].RankName}]} && !${Me.SpellReady[${Spell[${TC2}].RankName}]}) /return
	/if (${is_target_dead[${Target2Kill}]}) /return	

	CHECKEXIT
	AMIDEAD
	CHECKINPUTS

	| get the main assisst ID
	/declare _tmpID int local

	/if (${swAssistG}) {
		/varset _tmpID ${Spawn[pc ${Group.MainAssist}].ID}
	} else /if (${swAssist1}) {
		/varset _tmpID ${Spawn[pc ${Raid.MainAssist}].ID}
	} else /if (${swAssist2}) {
	} else /if (${swAssist3}) {
	} else /if (${Bool[${setMA1}]} && ${Bool[${Spawn[pc ${setMA1}].ID}]} && !${Bool[${Spawn[pc ${setMA1}].Dead}]}) {
		/varset _tmpID ${Spawn[pc ${setMA1}].ID}
	} else /if (${Bool[${setMA2}]} && ${Bool[${Spawn[pc ${setMA2}].ID}]} && !${Bool[${Spawn[pc ${setMA2}].Dead}]}) {
		/varset _tmpID ${Spawn[pc ${setMA2}].ID}
	}
	
	/if (!${Spawn[${_tmpID}].LineOfSight} || ${Spawn[${_tmpID}].Distance} > ${Spell[${TC1}].MyRange}) /return
	
  /if (${Me.SpellReady[${Spell[${TC1}].RankName}]} && !${Me.Song[Healing Twincast].ID}) {
		/if (${validate_cast[FALSE, spell, "${TC1}", ${_tmpID}]}) /call core_cast2 "${TC1}" 0 ${_tmpID} FALSE
	} else /if (${Me.SpellReady[${Spell[${TC2}].RankName}]} && !${Me.Song[Healing Twincast].ID}) {
		/if (${validate_cast[FALSE, spell, "${TC2}", ${_tmpID}]}) /call core_cast2 "${TC2}" 0 ${_tmpID} FALSE
	}

	| spalsh surge heal	
	/if (${Me.SpellReady[${Spell[${healSurge}].RankName}]} && ${Me.Song[Healing Twincast].ID}) {
		/delay 2.25s ${Me.SpellReady[${Spell[${healSurge}].RankName}]}
		/if (${validate_cast[FALSE, spell, "${healSurge}", ${_tmpID}]}) /call core_cast2 "${healSurge}" 0 ${_tmpID} FALSE
	}
/return



 |***
 *	DES: Shaman burn routines
 *	USE: establish all comp/trigger skills
 *	NOTE: 
 ***| 
sub burn_SHM()
	DEBUG burn_SHM()
	CHECKEXIT
	CHECKINPUTS

	| epic 2.0
	/if (${validate_cast[FALSE, epicitem, "2", ${Me.ID}]}) /call core_cast2 "${${Me.Class.ShortName}Epic2}" item 0 FALSE

	| swarm aa pet
	/if (${swSwarm}) {
		/if (${validate_cast[FALSE, alt, "Spirit Call", ${Target2Kill}]}) /call core_cast2 "Spirit Call" alt ${Target2Kill} FALSE
	}

	| Lower spell resists
	/if (${validate_cast[FALSE, alt, "Dampen Resistance", ${Target2Kill}]}) /call core_cast2 "Dampen Resistance" alt ${Target2Kill} FALSE

	| Twincast next 6 DOTs/HOTs
	/if (${validate_cast[FALSE, alt, "Fleeting Spirit", ${Target2Kill}]}) /call core_cast2 "Fleeting Spirit" alt ${Target2Kill} FALSE

	| Increase spell cost by 14% for 100% exceptional heals on instant heals for 18 seconds
	/if (${validate_cast[FALSE, alt, "Spiritual Blessing", ${Target2Kill}]}) /call core_cast2 "Spiritual Blessing" alt ${Target2Kill} FALSE

	/call cast_AA_spire

	/call use_glyph
	/call click_burn_circle
	/call click_item_burn
/return



|***
 *	DES: Control for Talisman Buff
 *	USE: /call cast_talisman
 *  NOTE: keeps talisman buff line up
 ***|
sub cast_talisman()
	DEBUG cast_talisman()
	/if (${Me.Buff[${spellTalisman}].ID}) /return
	AMIDEAD
	CHECKEXIT
	CHECKINPUTS
	/call timer_update timer_cast_Talisman ${tTalisman}
	/if (${validate_cast[FALSE, FIND, "${spellTalisman}", ${Me.ID}]}) /call core_cast2 "${spellTalisman}" FIND ${Me.ID} FALSE
/return



|***
 *	DES: We want Mana
 *	USE: 
 *  NOTE: 
 ***|
sub check_cani()
	DEBUG check_cani()
	CHECKEXIT
	CHECKINPUTS
	AMIDEAD
	
	/declare _spell string local ${Spell[${spellCannibalization}].RankName}
	
	| canni spell first...
	/if (${Me.PctMana} <= ${CanniPctManna} && ${Me.PctHPs} >= ${CanniPctHPs}) {
		/declare _cancast bool local FALSE
	
		/if (${_spell.Equal[Cannibalize]} && ${Me.CurrentHPs} > 50) /varset _cancast TRUE
		/if (${_spell.Equal[Cannibalize II]} && ${Me.CurrentHPs} > 67) /varset _cancast TRUE
		/if (${_spell.Equal[Cannibalize III]} && ${Me.CurrentHPs} > 74) /varset _cancast TRUE
		/if (${_spell.Equal[Cannibalize IV]} && ${Me.CurrentHPs} > 148) /varset _cancast TRUE
		/if (${_spell.Equal[Ancient Chaotic Pain]} && ${Me.CurrentHPs} > 668) /varset _cancast TRUE
		/if (${_spell.Equal[Pained Memory]} && ${Me.CurrentHPs} > 688) /varset _cancast TRUE
		/if (${_spell.Equal[Ancient: Ancestral Calling]} && ${Me.CurrentHPs} > 868) /varset _cancast TRUE
		/if (${_spell.Equal[Ancestral Bargain]} && ${Me.CurrentHPs} > 1042) /varset _cancast TRUE
		/if (${_spell.Equal[Ancestral Bargain Rk. II]} && ${Me.CurrentHPs} > 1085) /varset _cancast TRUE
		/if (${_spell.Equal[Ancestral Bargain Rk. III]} && ${Me.CurrentHPs} > 1128) /varset _cancast TRUE
		/if (${_spell.Equal[Ancestral Hearkening]} && ${Me.CurrentHPs} > 1161) /varset _cancast TRUE
		/if (${_spell.Equal[Ancestral Hearkening Rk. II]} && ${Me.CurrentHPs} > 1324) /varset _cancast TRUE
		/if (${_spell.Equal[Ancestral Hearkening Rk. III]} && ${Me.CurrentHPs} > 1141) /varset _cancast TRUE
		/if (${_spell.Equal[Ancestral Obligation]} && ${Me.CurrentHPs} > 1417) /varset _cancast TRUE
		/if (${_spell.Equal[Ancestral Obligation Rk. II]} && ${Me.CurrentHPs} > 1615) /varset _cancast TRUE
		/if (${_spell.Equal[Ancestral Obligation Rk. III]} && ${Me.CurrentHPs} > 1721) /varset _cancast TRUE
		/if (${_spell.Equal[Ancestral Covenant]} && ${Me.CurrentHPs} > 1559) /varset _cancast TRUE
		/if (${_spell.Equal[Ancestral Covenant Rk. II]} && ${Me.CurrentHPs} > 1777) /varset _cancast TRUE
		/if (${_spell.Equal[Ancestral Covenant Rk. III]} && ${Me.CurrentHPs} > 1893) /varset _cancast TRUE
		/if (${_spell.Equal[Ancestral Arrangement]} && ${Me.CurrentHPs} > 2366) /varset _cancast TRUE
		/if (${_spell.Equal[Ancestral Arrangement Rk. II]} && ${Me.CurrentHPs} > 2484) /varset _cancast TRUE
		/if (${_spell.Equal[Ancestral Arrangement Rk. III]} && ${Me.CurrentHPs} > 2608) /varset _cancast TRUE
		/if (${_spell.Equal[Ancestral Pact]} && ${Me.CurrentHPs} > 4564) /varset _cancast TRUE
		/if (${_spell.Equal[Ancestral Pact Rk. II]} && ${Me.CurrentHPs} > 4792) /varset _cancast TRUE
		/if (${_spell.Equal[Ancestral Pact Rk. III]} && ${Me.CurrentHPs} > 5032) /varset _cancast TRUE
		/if (${_spell.Equal[Tribal Pact]} && ${Me.CurrentHPs} > 6693) /varset _cancast TRUE
		/if (${_spell.Equal[Tribal Pact Rk. II]} && ${Me.CurrentHPs} > 7028) /varset _cancast TRUE
		/if (${_spell.Equal[Tribal Pact Rk. III]} && ${Me.CurrentHPs} > 7379) /varset _cancast TRUE
		/if (${_spell.Equal[Tribal Bargain]} && ${Me.CurrentHPs} > 8117) /varset _cancast TRUE
		/if (${_spell.Equal[Tribal Bargain Rk. II]} && ${Me.CurrentHPs} > 8523) /varset _cancast TRUE
		/if (${_spell.Equal[Tribal Bargain Rk. III]} && ${Me.CurrentHPs} > 8949) /varset _cancast TRUE

		/if (${_cancast}) {
			/if (${Me.SpellReady[${_spell}]}) {
				/if (${validate_cast[FALSE, spell, "${spellCannibalization}", ${Me.ID}]}) /call core_cast2 "${spellCannibalization}" 0 0 FALSE
				/return
			}
		}

		| use the aa cani
		/if (${validate_cast[FALSE, alt, "Cannibalization", ${Me.ID}]}) /call core_cast2 "Cannibalization" alt 0 FALSE
	}
	
/return



|***
 *	DES: self heal only
 *	USE: /call check_heal_self_${Me.Class.ShortName}
 *  NOTE: 
 ***|
sub check_heal_self_SHM()
	DEBUG check_heal_self_SHM()
	AMIDEAD
	CHECKEXIT
	CHECKTIE
	
	UPDATEHUD "HEAL Self"
	/if (${Me.PctHPs} > ${SHMPoint}*.${tmpHealOverride}) /return
	/declare _count int local 0
	
	| Panic Ancestral Guard
	/if (${Me.PctHPs} < ${PanicPoint}*.${tmpHealOverride}) {
		/if (${Me.Casting.ID}) /stopcast
		/if (${validate_cast[FALSE, alt, "Ancestral Guard", ${Me.ID}]}) /call core_cast2 "Ancestral Guard" alt 0 FALSE
	}	

	| single heal over time
	/if (${Bool[${healHoT}]}) {
		/if (${Me.PctHPs} <= ${SHMPoint}*.${tmpHealOverride}) {
			/if (${Spell[${healHoT}].Stacks} && !${Me.Song[${healHoT}].Duration}) {
				/if (${validate_cast[FALSE, spell, "${healHoT}", ${Me.ID}]}) /call core_cast2 "${healHoT}" 0 ${Me.ID} check_health_group
			}
		}
	}	
	
	| aggressive switch sort 	
  /if (${switchHealAggressive}) {
 		/if (${Me.SpellReady[${Spell[${healReckless1}].RankName}]} || ${Me.SpellReady[${Spell[${healReckless2}].RankName}]} || ${Me.SpellReady[${Spell[${healReckless3}].RankName}]}) /goto :aggressivesort
 	} 	 	
 	
	| intervention/main heals
	/if (${Me.PctHPs} <= ${SHMPoint}) {
		/if (${Me.SpellReady[${Spell[${healIntervention1}].RankName}]}) {
			/if (${validate_cast[FALSE, spell, "${healIntervention1}", ${Me.ID}]}) /call core_cast2 "${healIntervention1}" 0 ${Me.ID} FALSE
		} else /if (${Me.SpellReady[${Spell[${healMain}].RankName}]}) {
			/if (${validate_cast[FALSE, spell, "${healMain}", ${Me.ID}]}) /call core_cast2 "${healMain}" 0 ${Me.ID} check_health_group
		} 
	} 
  
 	| Reckless Heals
	:aggressivesort
  /if (${Me.PctHPs} <= ${SHMPoint}) {
		/for _count 1 to 3
			/if (${validate_cast[FALSE, spell, "${healReckless${_count}}", ${Me.ID}]}) /call core_cast2 "${healReckless${_count}}" 0 ${Me.ID} check_health_group
			/break
		/next _count 
	}
	
	UPDATEHUD FALSE	
/return



|***
 *	DES: Shaman Alliance
 *	USE: 
 *  NOTE: 
 ***|
 sub alliance_SHM()
 	DEBUG alliance_SHM()
	CHECKEXIT
	CHECKINPUTS

	/if (!${Me.SpellReady[${Spell[${SHMAlliance}].RankName}]}) /return FALSE
	
	/if (${swAssistG}) {
		/target pc ${Group.MainAssist}
		/delay 5 ${Target.BuffsPopulated}
		/if (${Target.BuffsPopulated} && !${Target.Buff[${SHMAlliance}].ID}) {
			/if (${validate_cast[FALSE, spell, "${SHMAlliance}", ${Group.MainAssist}]}) /call core_cast2 "${SHMAlliance}" 0 ${Group.MainAssist} FALSE
		}
	} else /if (${swAssist1}) {
		/target pc ${Raid.MainAssist}
		/delay 5 ${Target.BuffsPopulated}
		/if (${Target.BuffsPopulated} && !${Target.Buff[${SHMAlliance}].ID}) {
			/if (${validate_cast[FALSE, spell, "${SHMAlliance}", ${Raid.MainAssist}]}) /call core_cast2 "${SHMAlliance}" 0 ${Raid.MainAssist} FALSE
		}
	}
	/if (${Bool[${setMA1}]} && ${Spawn[pc ${setMA1}].ID}) {
		/target pc {setMA1}
		/delay 5 ${Target.BuffsPopulated}
		/if (${Target.BuffsPopulated} && !${Target.Buff[${SHMAlliance}].ID}) {
			/if (${validate_cast[FALSE, spell, "${SHMAlliance}", ${setMA1}]}) /call core_cast2 "${SHMAlliance}" 0 ${setMA1} FALSE
		}
	} else /if (${Bool[${setMA2}]} && ${Spawn[pc ${setMA2}].ID}) {
		/target pc ${setMA2}
		/delay 5 ${Target.BuffsPopulated}
		/if (${Target.BuffsPopulated} && !${Target.Buff[${SHMAlliance}].ID}) {
			/if (${validate_cast[FALSE, spell, "${SHMAlliance}", ${setMA2}]}) /call core_cast2 "${SHMAlliance}" 0 ${setMA2} FALSE
		}
	} 
/return



|***
 *	DES: MGB shaman AA Ancestral Aid if both AAs are ready
 *	USE: 
 *  NOTE:
 ***|
sub cast_AA_aid()
	DEBUG cast_AA_aid()
	AMIDEAD
	CHECKEXIT
	CHECKINPUTS
	/if (${Me.AltAbilityReady[Mass Group Buff]} && ${Me.AltAbilityReady[${SHMAid}]} && !${Bool[${Me.Song[${SHMAid}]}]}) {
		/if (${validate_cast[FALSE, alt, "Mass Group Buff", ${Me.ID}]}) /call core_cast2 "Mass Group Buff" alt 0 FALSE
		/if (${validate_cast[FALSE, alt, "${SHMAid}", ${Me.ID}]}) /call core_cast2 "${SHMAid}" alt 0 FALSE
	} else /if (${Bool[${Me.Song[${SHMAid}]}]}) {
		VOUT ${cbad}QUIT\ax trying to cast ${csp}Ancestral Aid\ax while its running.	
	}		
/return TRUE



|***
 *	DES: Class controls
 *	USE: /shm []
 *  NOTE:
 ***|
#bind setSHM /shm
sub Bind_setSHM(string _type, string _verbage)
	/if (!${validate_class[TRUE, |SHM]}) /return

	/declare _sep bool local
	/declare _listout string local
	/declare _count int local 0
	/declare _bind string local ${Me.Class.ShortName.Lower}
  /declare _lstClass string	local |aid|aggressive|cannihps|cannimanna|epic|fade|groupheal|grouphot|grow|healpoint|rampage|rod|spiregroup|spireraid|surge|talisman|xtclass|xttime

	| enable/disable talisman line of spells
	/if (${_type.Find[talisman]}) {
		/call set_switch ${chrID} switchTalisman ${_verbage}

	| set rampage tank toon
	} else /if (${_type.Equal[rampage]}) {
		/if (${Bool[${_verbage}]}) {
			/call sql_update FALSE ${chrID} RampageTank ${_verbage.Left[1].Upper}${_verbage.Right[-1].Lower}
		} else /if (!${Bool[${_verbage}]}) {
			/call sql_update FALSE ${chrID} RampageTank FALSE
		}

	| set AA fade use %	
	} else /if (${_type.Equal[fade]}) {
		/call set_character_fade FALSE ${_verbage}

	| set the XTarget class to heal
	} else /if (${_type.Equal[xtclass]}) {
		/call set_character_xthealclass FALSE ${_verbage}
		/return	
		
	| use Modulation Rods
	} else /if (${_type.Equal[rod]}) {
		/call set_switch ${chrID} swModRod ${_verbage} 	
		
	| use grow temp buff
	} else /if (${_type.Equal[grow]}) {
		/call set_switch ${chrID} switchGrowth ${_verbage}	
	
	| force epic click
	} else /if (${_type.Equal[epic]}) {
		/if (${Me.Casting.ID}) /stopcast
		/if (${validate_cast[FALSE, epicitem, "2", ${Me.ID}]}) /call core_cast2 "${${Me.Class.ShortName}Epic2}" item 0 FALSE	
		/return

	| use group Heals
	} else /if (${_type.Equal[groupheal]}) {
		/call set_switch ${chrID} useGroupHeal ${_verbage}

	| use surge Heals
	} else /if (${_type.Equal[surge]}) {
		/call set_switch ${chrID} switchSurge ${_verbage}

	
	| use group Heal over Time	
	} else /if (${_type.Equal[grouphot]}) {
		/call set_switch ${chrID} useGroupHoT ${_verbage}
	 	
	| set Class Heal Point	
	} else /if (${_type.Equal[healpoint]}) {
		/call set_character_healpoint FALSE ${_verbage}
		/return

	| canni HP pct
	} else /if (${_type.Equal[cannihps]}) {
		/if (${Range.Between[1,99:${Int[${_verbage}]}]}) {
			/call sql_update FALSE ${chrID} CanniPctHPs ${_verbage}
		} else {
			VOUT /${_bind} cannihp [${cinfo}${CanniPctHPs}\ax] allowable range ${sep} ${cinfo}1 - 99\ax
			/return
		}	

	| mgb aid
	} else /if (${Bool[${_type.Find[aid]}]}) {
		/stopcast
		/delay 2s !${Me.Casting.ID}
		/call cast_AA_aid
		/return
		
	| enable/disable lead with aggerssive heals
	} else /if (${_type.Equal[aggressive]}) {
		/call set_switch ${chrID} switchHealAggressive ${_verbage}

	| canni Manna pct
	} else /if (${_type.Equal[cannimanna]}) {
		/if (${Range.Between[1,99:${Int[${_verbage}]}]}) {
			/call sql_update FALSE ${chrID} CanniPctManna ${_verbage}
		} else {
			VOUT /${_bind} cannimanna [${cinfo}${CanniPctHPs}\ax] allowable range ${sep} ${cinfo}1 - 99\ax
			/return
		}	
		
	| set XT refresh list Time
	} else /if (${_type.Equal[xttime]}) {
		/if (!${validate_expansion[TRUE, |16]}) /return
		/if (${Range.Between:${Int[${_verbage}]}]}) {
			/call sql_update FALSE timer tXHeal ${_verbage}
		} else {
			VOUT /${_bind} xttime [${cinfo}${tXHeal}m\ax] allowable range ${sep} ${cinfo}1 - 5 min\ax
			/return
		}	

	| spire control
	} else /if (${_type.Equal[spiregroup]} || ${_type.Equal[spireraid]}) {
		/call set_character_spire FALSE ${_type} ${_verbage}

	}
	
	/if (${swMQClear}) /mqclear
	/for _count 1 to ${_lstClass.Count[|]}
		/varset _listout ${_listout} ${If[${_sep},${sep},]} ${cinfo}${_lstClass.Arg[${_count},|]}\ax
		/if (${_lstClass.Arg[${_count},|].Equal[spiregroup]}) /varset _listout ${_listout}:${cnum}${spireGroup}\ax
		/if (${_lstClass.Arg[${_count},|].Equal[spireraid]}) /varset _listout ${_listout}:${cnum}${spireRaid}\ax
		/if (${_lstClass.Arg[${_count},|].Equal[xttime]}) /varset _listout ${_listout}:${cnum}${tXHeal}\ax
		/if (${_lstClass.Arg[${_count},|].Equal[cannihps]}) /varset _listout ${_listout}:${cnum}${CanniPctHPs}\ax
		/if (${_lstClass.Arg[${_count},|].Equal[cannimanna]}) /varset _listout ${_listout}:${cnum}${CanniPctManna}\ax
		/if (${_lstClass.Arg[${_count},|].Equal[groupheal]}) /varset _listout ${_listout}:${If[${useGroupHeal},${oon},${ooff}]}
		/if (${_lstClass.Arg[${_count},|].Equal[grouphot]}) /varset _listout ${_listout}:${If[${useGroupHoT},${oon},${ooff}]}
		/if (${_lstClass.Arg[${_count},|].Equal[talisman]}) /varset _listout ${_listout}:${If[${switchTalisman},${oon},${ooff}]}
		/if (${_lstClass.Arg[${_count},|].Equal[surge]}) /varset _listout ${_listout}:${If[${switchSurge},${oon},${ooff}]}
		/if (${_lstClass.Arg[${_count},|].Equal[grow]}) /varset _listout ${_listout}:${If[${switchGrowth},${oon},${ooff}]}
		/if (${_lstClass.Arg[${_count},|].Equal[aggressive]}) /varset _listout ${_listout}:${If[${switchHealAggressive},${oon},${ooff}]}
		/if (${_lstClass.Arg[${_count},|].Equal[rampage]}) /varset _listout ${_listout}:${If[${Bool[${RampageTank}]},${cnum}${RampageTank}\ax,${ooff}]}
		/if (${_lstClass.Arg[${_count},|].Equal[healpoint]}) /varset _listout ${_listout}:${cnum}->\ax
		/if (${_lstClass.Arg[${_count},|].Equal[rod]}) /varset _listout ${_listout}:${If[${Bool[${swModRod}]},${oon},${ooff}]}
		/if (${_lstClass.Arg[${_count},|].Equal[fade]}) /varset _listout ${_listout}:${If[${switchAAFade},${cnum}${useAAFadePct}\ax,${cbad}${useAAFadePct}\ax]}

		/varset _sep TRUE
	/next _count
	VOUT /${_bind} [${_listout}]

/return


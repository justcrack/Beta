|***
 *	class.mag.inc
 *	Core Magician Include
 *
 * 
 ***| 



|***
 *	DES: Main Magician Routine
 *	USE: 
 *	NOTE: 
 ***|
sub core_MAG()
	DEBUG core_MAG()
	CHECKINPUTS
	AMIDEAD
	CHECKTIE

	/if (${coreAuto}) {
		/if (${switchGather}) {
			/if (${Me.PctMana} <= ${GatherPercent}*.${tmpHealOverride}) /call check_gather
		}
		/if (!${check_engaged[]} && ${swPet}) /call check_pet
		/if (!${timer_caldron_use}) /call use_cauldron
	}

	/while (${prep_combat[]}) {
		CHECKINPUTS
		/if (${swHealSelf}) /call patch_heal_${Me.Class.ShortName}
		/if (${swHealPet}) /call pet_heal_${Me.Class.ShortName}
		/if (!${timer_check_cast_cure} && ${swHealCure}) /call cast_cure FALSE
		/if (${exit_class_loop[]}) /return FALSE

		/if (${isValidTarget}) {
			/call set_combat ${combat.Upper}
			/if (${swBurnControl}) /call burn_${Me.Class.ShortName}
			/if (${swSwarm} || ${swPet}) /call send_pet
			/if (${swSwarm}) /call cast_nuke_servant
			/if (${swCombatMelee}) /call use_skill_melee ${Target2Kill}
			/call disc_${Me.Class.ShortName}
			/if (${swAlliance}) /call trigger_alliance ${Target2Kill}
			/call check_deBuffs ${Target2Kill}
			/call click_DPS_${Me.Class.ShortName}
			/if (${switchForceTwincast}) /call force_twincast_${Me.Class.ShortName}
			/if (${swPet}) /call cast_nuke_salvo
			/call cast_nuke_line
			/if (${useRain}) /call cast_nuke_rain
			/call click_BP
			/call click_item
			/if (${swSwarm}) /call click_item_swarm ${Target2Kill}
		}

		/if (${coreAuto}) {	
			/call Bind_setMAG rods
			/if (${switchAAFade} && ${Me.PctHPs} <= ${useAAFadePct}) /call cast_AA_fade
			/if (${swModRod} && ${validate_class[FALSE, ${lstClassCast}]}) /call use_mod_rod
		}
	}
/return TRUE



|***
 *	DES: Discs
 *	USE: auto
 *	NOTE: 
 ***|
sub disc_MAG()
	DEBUG disc_MAG()
	CHECKEXIT
	CHECKTIE
	/if (${is_target_dead[${Target2Kill}]}) /return
	/if (${validate_cast[FALSE, alt,"Force of Elements", ${Target2Kill}]}) /call core_cast2 "Force of Elements" alt ${Target2Kill} FALSE
/return



|***
 *	DES: summon cauldron item
 *	USE: auto on timer
 *	NOTE: 
 ***|
sub use_cauldron()
	DEBUG use_cauldron()
	CHECKEXIT
	CHECKTIE
	/call timer_update timer_caldron_use ${tCauldron_use}

	/if (${Bool[${Cauldron}]}) {
		/if (${validate_cast[FALSE, item, "${Cauldron}", ${Me.ID}]}) /call core_cast2 "${Cauldron}" item 0 FALSE
		/call sort_cursor TRUE
	}
/return



|***
 *	DES: click dps thingies on bad guys
 *	USE: 
 *	NOTE: this is filler DPS; compensate for global cooldown
 ***|
sub click_DPS_MAG()
	DEBUG click_DPS_MAG()
	AMIDEAD
	CHECKEXIT
	CHECKINPUTS
	/call click_horn_of_unity 	
	
	| click summon rods. DPS fillers....
	/if (${validate_cast[FALSE, item, "${ParadoxRodName}", ${Target2Kill}]}) /call core_cast2 "${ParadoxRodName}" item ${Target2Kill} FALSE
	/if (${validate_cast[FALSE, item, "${OrbName}", ${Target2Kill}]}) /call core_cast2 "${OrbName}" item ${Target2Kill} FALSE

	/if (${swSwarm}) {
		/if (${validate_cast[FALSE, item, "${ElementRodName}", ${Target2Kill}]}) /call core_cast2 "${ElementRodName}" item ${Target2Kill} FALSE
		/return
	}
/return



|***
 *	DES: Pet nuke
 *	USE: auto when 3+ pets in range
 *	NOTE: 
 ***|
sub cast_nuke_salvo()
	DEBUG cast_nuke_salvo()
	CHECKEXIT
	CHECKTIE
	/if (${SpawnCount[pet radius ${setMobAgro}]} < 3) /return
	/if (${validate_cast[FALSE, spell, "${NukeSalvo}", ${Target2Kill}]}) /call core_cast2 "${NukeSalvo}" 0 ${Target2Kill} FALSE
/return



|***
 *	DES: Pet heal routines
 *	USE: /call petHeal_${Me.Class.ShortName}
 *	NOTE: 
 ***|
sub pet_heal_MAG()
	DEBUG pet_heal_MAG()
	CHECKEXIT
	CHECKTIE
	/if (${validate_cast[FALSE, alt, "${setPetAAHeal}", ${Me.Pet.ID}]} && ${Pet.PctHPs} < 40) /call core_cast2 "${setPetAAHeal}" alt ${Me.Pet.ID} FALSE
	/if (${validate_cast[FALSE, spell, "${setPetHealSpell}", ${Me.Pet.ID}]} && ${Pet.PctHPs} < 60) /call core_cast2 "${setPetHealSpell}" ${PetHealType} ${Me.Pet.ID} FALSE
	/if (${validate_cast[FALSE, spell, "${setPetRenewalSpell}", ${Me.Pet.ID}]} && ${Pet.PctHPs} < 80) /call core_cast2 "${setPetRenewalSpell}" ${PetRenewalType} ${Me.Pet.ID} FALSE
/return



|***
 *	DES: emergency healing
 *	USE: auto
 *	NOTE: 
 ***|
sub patch_heal_MAG()
	DEBUG patch_heal_MAG()
	CHECKEXIT
	CHECKTIE
	/if (${Me.PctHPs} > ${${Me.Class.ShortName}Point}*.${tmpHealOverride}) /return
	/call click_item_heal
/return



 |***
 *	DES: Magician burn routines
 *	USE: establish all comp/trigger skills
 *	NOTE: 
 ***| 
sub burn_MAG()
	DEBUG burn_MAG()
	CHECKEXIT
	CHECKTIE
	/if (${validate_cast[FALSE, alt, "Elemental Ward", ${Me.ID}]}) {
		/call core_cast2 "Elemental Ward" alt 0 FALSE
	}
	
	/if (${Target.PctHPs} > 20) {
		/if (${validate_cast[FALSE, alt, "Focus of Arcanum", ${Me.ID}]}) {
			/call core_cast2 "Focus of Arcanum" alt 0 FALSE
		}
	}
	
	| direct damage bonus
	/if (${Target.PctHPs} > 10) {
		/if (${Bool[${Me.Buff[Elemental Union]}]}) {
			/if (${validate_cast[FALSE, alt, "Heart of Flames", ${Me.ID}]}) {
				/call core_cast2 "Heart of Flames" alt 0 FALSE
			}
		} else /if (!${Bool[${Me.Buff[Heart of Flames]}]} && !${Me.AltAbilityReady[Heart of Flames]}) {
			/if (${validate_cast[FALSE, alt, "Elemental Union", ${Me.ID}]}) {
				/call core_cast2 "Elemental Union" alt 0 FALSE
			}
		} else /if (!${Me.AltAbilityReady[Heart of Flames]} && !${Bool[${Me.Buff[Heart of Flames]}]} && !${Me.AltAbilityReady[Elemental Union]} && !${Bool[${Me.Buff[Elemental Union]}]}) {
			/call use_glyph
		}
	}

	/call cast_AA_spire
	/if (${validate_cast[FALSE, alt, "${SilentCast}", ${Me.ID}]}) {
		/call core_cast2 "${SilentCast}" alt 0 FALSE
	}
	/if (${validate_cast[FALSE, alt, "Heart of Stone", ${Me.ID}]}) {
		/call core_cast2 "Heart of Stone" alt 0 FALSE
	}

	| pet AAs
	/if (${swPet}) {
		/if (${validate_cast[FALSE, alt, "Servant of Ro", ${Target2Kill}]}) {
			/call core_cast2 "Servant of Ro" alt ${Target2Kill} FALSE
		}
		/if (${validate_cast[FALSE, alt, "Companion's Fury", ${Me.Pet.ID}]}) {
			/call core_cast2 "Companion's Fury" alt ${Me.Pet.ID} FALSE
		}
	}
	
	| swarm pets
	/if (${swSwarm}) {
		/if (${validate_cast[FALSE, alt, "Host of the Elements", ${Target2Kill}]}) {
			/call core_cast2 "Host of the Elements" alt ${Target2Kill} FALSE
		}
	}

	| epic 2.0
	/if (${validate_cast[FALSE, epicitem, "2", ${Me.ID}]}) {
		/call core_cast2 "${${Me.Class.ShortName}Epic2}" item ${Me.Pet.ID} FALSE
	}

	/call click_burn_circle
	/call click_item_burn
/return



|***
 *  DES: Control for getting Twincast either spell or Proc
 *  USE: /call force_twincast_MAG
 *	NOTE: Twincast lets get it.
 ***|
sub force_twincast_MAG()
	DEBUG force_twincast_MAG()
	/if (!${switchForceTwincast} || ${timer_force_twincast}) /return
	/if (${is_target_dead[${Target2Kill}]}) /return
	AMIDEAD
	CHECKEXIT
	CHECKINPUTS
	/if (!${timer_force_twincast}) /call timer_update timer_force_twincast ${tForce_Twincast}
	/if (!${Me.Buff[Improved Twincast].ID} && !${Me.Buff[Twincast].ID}) {
		/if (${validate_cast[FALSE, spell, "${TwincastSpell}", ${Me.ID}]}) {
			/call core_cast2 "${TwincastSpell}" 0 0 FALSE
		}
		/return
	} else /if (!${Me.Buff[Improved Twincast].ID} && !${Me.Buff[${TwincastSpell}].ID} && !${Me.Buff[Twincast].ID}) {
		/if (!${cast_condition[NukeTwincastCondition]}) /return
		/if (${validate_cast[FALSE, spell, "${NukeTwincast${lineCurrent}}", ${Target2Kill}]}) {
			/call core_cast2 "${NukeTwincast${lineCurrent}}" 0 ${Target2Kill} FALSE
		}
	}	
/return



|***
 *	DES: lemme heare everyone say ... hey .. we want some ..... umm manna..
 *	USE: 
 *	NOTE: 
 ***|
sub check_gather()
	DEBUG check_gather()
	CHECKEXIT
	CHECKINPUTS
	CHECKTIE

	/if (${Me.Buff[Improved Twincast].ID} || ${Me.Buff[${TwincastSpell}].ID} || ${Me.Buff[Twincast].ID}) /return

	| pet consumption for manna		
	/if (${Me.Pet.ID}) {
		/if (${Me.PctMana} <= 19 && ${validate_cast[FALSE, alt, "Elemental Conversion", ${Me.ID}]}) {
			/call core_cast2 "Elemental Conversion" alt 0 FALSE
		}
	}
	
	| rods
	/if (${Bool[${SelfRodName}]}) {
		/if (${validate_cast[FALSE, item, "${SelfRodName}", ${Me.ID}]}) {
			/call core_cast2 "${SelfRodName}" item 0 FALSE
		}
	}
	/if (${Bool[${ModRodName}]}) {
		/if (${validate_cast[FALSE, item, "${ModRodName}", ${Me.ID}]}) {
			/call core_cast2 "${ModRodName}" item 0 FALSE
		}
	}
	
	| stop here to save time is nothing else is available
	/if (!${Me.SpellReady[${Spell[${GatherSpell}].RankName}]} && !${Me.AltAbilityReady[Forceful Rejuvenation]}) /return
		
	| force rejuv if we can proc gather spell again
	/if (!${Me.SpellReady[${Spell[${GatherSpell}].RankName}]}) {
		/if (${validate_cast[FALSE, alt, "Forceful Rejuvenation", ${Me.ID}]}) {
			/call core_cast2 "Forceful Rejuvenation" alt 0 FALSE
		}
	}

	/if (${Me.SpellReady[${Spell[${GatherSpell}].RankName}]}) {
		/if (${validate_cast[FALSE, spell, "${GatherSpell}", ${Me.ID}]}) {
			/call core_cast2 "${GatherSpell}" 0 0 FALSE
		}
	}

/return



|***
 *	DES: call of the hero command
 *	USE:/coh [group]
 *	NOTE:
 ***|
#bind call_of_the_hero /call_of_the_hero
sub Bind_call_of_the_hero(string _sender, string _verbage)
	DEBUG Bind_call_of_the_hero(${_sender}, ${_verbage})
  /if (${_verbage.Equal[group]}) {
  	/call mem_spell TRUE "${MAGAAGroupCOH}" ${buffGem} TRUE
	  /if (${Me.Invis}) /makemevisible
	  ECHOCHANNEL Calling ... Group
 		/if (${validate_cast[FALSE, spell, "${MAGAAGroupCOH}", ${Me.ID}]}) {
 			/call core_cast2 "${MAGAAGroupCOH}" ${buffGem} 0 FALSE
 		}
  } else {
		/delay 15s ${Me.AltAbilityReady[Call of the Hero]}
	  /if (${Me.Invis}) /makemevisible
	  ECHOCHANNEL Calling ... ${_sender}
 		/if (${validate_cast[FALSE, alt, "Call of the Hero", ${Spawn[pc ${_sender}].ID}]}) {
 			/call core_cast2 "Call of the Hero" alt ${Spawn[pc ${_sender}].ID} FALSE
 		}
  }
/return


		
|***
 *	DES: Class controls
 *	USE: /mag []
 *	NOTE:
 ***|
#bind setMAG /mag
sub Bind_setMAG(string _type, string _verbage)
	/if (!${validate_class[TRUE, |MAG]}) /return

	/declare _sep bool local
	/declare _listout string local
	/declare _count int local 0
	/declare _bind string local ${Me.Class.ShortName.Lower}
  /declare _lstClass string local |fade|forcetwincast|gather|healpoint|line|rod|rods|spiregroup|spireraid
	
	| use Modulation Rods
	/if (${_type.Equal[rod]}) {
		/call set_switch ${chrID} swModRod ${_verbage} 	

	| gather enable/disable /pct
	} else /if (${_type.Find[gather]}) {
		/if (${Bool[${_verbage}]} && ${Range.Between[1,99:${Int[${_verbage}]}]}) {
			/call sql_update FALSE ${chrID} GatherPercent ${_verbage} 
		} else /if (${Bool[${_verbage}]} && !${Range.Between[1,99:${Int[${_verbage}]}]}) {
			VOUT /${_bind} gather(${cnum}${GatherPercent}\ax) allowable range ${sep} ${cinfo}1 - 99\ax
		}	else {
			/call set_switch ${chrID} switchGather ${_verbage}  
		}			
		
	| set AA fade use %	
	} else /if (${_type.Equal[fade]}) {
		/call set_character_fade FALSE ${_verbage}

	| enable/disable force twincasting
	} else /if (${_type.Equal[forcetwincast]}) {
		/call set_switch ${chrID} switchForceTwincast ${_verbage}


	| get rods.. cause things and stuff
	} else /if (${_type.Equal[rods]}) {

		| self mod rod
		/if (${Bool[${SelfRodName}]} && !${FindItem[=${SelfRodName}].ID}) {
			/if (${validate_cast[FALSE, spell, "${SelfRodSpell}", ${Me.ID}]}) /call core_cast2 "${SelfRodSpell}" ${buffGem} ${Me.ID} FALSE
			/call sort_cursor TRUE
		}
		
		| AA group mod rod
		/if (${Bool[${ModRodName}]} && !${FindItem[=${ModRodName}].ID}) {
			/if (${validate_cast[FALSE, alt, "${ModRodSpell}", ${Me.ID}]}) /call core_cast2 "${ModRodSpell}" alt 0 FALSE
			/call sort_cursor TRUE
		}

		| paradox rod	
		 /if (${Bool[${ParadoxRodName}]} && !${FindItem[=${ParadoxRodName}].ID} ) {
		 	/if (${validate_cast[FALSE, spell, "${ParadoxSpell}", ${Me.ID}]}) /call core_cast2 "${ParadoxSpell}" ${buffGem} ${Me.ID} FALSE
		 	/call sort_cursor TRUE
		 }
		
		| elemental orb
		/if (${Bool[${OrbName}]} && !${FindItem[=${OrbName}].ID}) {
			/if (${validate_cast[FALSE, spell, "${OrbSpell}", ${Me.ID}]}) /call core_cast2 "${OrbSpell}" ${buffGem} 0 FALSE
			/call sort_cursor TRUE
		}
		
		| elemental rod
		/if (${Bool[${ElementRodName}]} && !${FindItem[=${ElementRodName}].ID}) {
			/if (${validate_cast[FALSE, spell, "${ElementRodSpell}", ${Me.ID}]}) /call core_cast2 "${ElementRodSpell}" ${buffGem} 0 FALSE
			/call sort_cursor TRUE
		}
		/return

	| spire control
	} else /if (${_type.Equal[spiregroup]} || ${_type.Equal[spireraid]}) {
		/call set_character_spire FALSE ${_type} ${_verbage}

	| healpoint
	} else /if (${_type.Find[healpoint]}) {
		/call set_character_healpoint FALSE ${_verbage}
		/return
		
	| spell line. not quite done with this. 
	} else /if (${_type.Equal[line]}) {
		| mem a line of spells.
		/if (${Bool[${lineType.Find[${_verbage.Lower}]}]}) {
			/call sql_update FALSE ${chrID} lineCurrent ${_verbage.Left[1].Upper}${_verbage.Right[-1].Lower}
			/if (${_verbage.Equal[off]}) {
				VOUT Casting line ${sep} ${odisabled}
				/return
			}
			
			/call mem_nuke_line ${lineCurrent}
			/return
		}
		/for _count 1 to ${lineType.Count[|]}
			/varset _listout ${_listout} ${If[${_sep},${sep},]} ${If[${lineCurrent.Equal[${lineType.Arg[${_count},|]}]},${cgood},${cinfo}]}${lineType.Arg[${_count},|]}\ax   
			/varset _sep TRUE
		/next _count
		VOUT /${_bind} line [${_listout}]	
		/return		
	
	}
	
	/if (${swMQClear}) /mqclear
	/for _count 1 to ${_lstClass.Count[|]}
		/varset _listout ${_listout} ${If[${_sep},${sep},]} ${cinfo}${_lstClass.Arg[${_count},|]}\ax
		/if (${_lstClass.Arg[${_count},|].Equal[spiregroup]}) /varset _listout ${_listout}:${cnum}${spireGroup}\ax
		/if (${_lstClass.Arg[${_count},|].Equal[spireraid]}) /varset _listout ${_listout}:${cnum}${spireRaid}\ax
		/if (${_lstClass.Arg[${_count},|].Equal[healpoint]}) /varset _listout ${_listout}:${cnum}${${Me.Class.ShortName}Point}\ax
		/if (${_lstClass.Arg[${_count},|].Equal[gather]}) /varset _listout ${_listout}:${cnum}${If[${switchGather},${cnum}${GatherPercent}\ax,${ooff}]}\ax
		/if (${_lstClass.Arg[${_count},|].Equal[fade]}) /varset _listout ${_listout}:${If[${switchAAFade},${cnum}${useAAFadePct}\ax,${cbad}${useAAFadePct}\ax]}
		/if (${_lstClass.Arg[${_count},|].Equal[forcetwincast]}) /varset _listout ${_listout}:${If[${switchForceTwincast},${oon},${ooff}]}
		/if (${_lstClass.Arg[${_count},|].Equal[rod]}) /varset _listout ${_listout}:${If[${Bool[${swModRod}]},${oon},${ooff}]}
		/if (${_lstClass.Arg[${_count},|].Equal[line]}) /varset _listout ${_listout}:${cnum}->\ax
		/varset _sep TRUE
	/next _count
	VOUT /${_bind} [${_listout}]
/return




|***
 *	class.dru.inc
 *	Core Druid Include
 *
 *	
 ***|



|***
 *	DES: Main Druid Routine
 *	USE: 
 *  NOTE: 
 ***|
sub core_DRU()
	DEBUG core_DRU()
	CHECKINPUTS
	AMIDEAD
	CHECKTIE
	
	/if (${coreAuto}) {
	  /if (${swHealXTarget}) {
	  	/if (!${timer_set_xtarget}) {
		    /call set_xtarget
		    /call timer_update timer_set_xtarget ${tXHeal}
		  }	
		}
		
		/if (!${check_engaged[]} && ${swPet}) /call check_pet
	}	

	/while (${prep_combat[]}) {
		CHECKINPUTS
		/if (${swHealSelf} && !${Group.Members}) /call check_heal_self_${Me.Class.ShortName}
		/if (${swHealGroup} && ${Group.Members}) /call check_heal_group_${Me.Class.ShortName}
		/if (${swHealXTarget}) /call check_heal_XTarget_${Me.Class.ShortName}
		/if (${swHealPet} && ${Group.Members}) /call check_heal_pet_${Me.Class.ShortName}
		/if (${swHealCure}) /call cast_cure

		/if (${exit_class_loop[]}) /return

		/if (${isValidTarget}) {
			CHECKREZ
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
sub disc_DRU()
	DEBUG disc_DRU()
	CHECKEXIT
	CHECKINPUTS
	/if (${is_target_dead[${Target2Kill}]}) /return

	/if (${validate_cast[FALSE, alt, "Season's Wrath", ${Target2Kill}]}) /call core_cast2 "Season's Wrath" alt ${Target2Kill} FALSE
	/if (${validate_cast[FALSE, alt, "Storm Strike", ${Target2Kill}]}) /call core_cast2 "Storm Strike" alt ${Target2Kill} FALSE
	
	| wolf spirit AAs
	/if (${AAWolfSpirit.Equal[black]}) {	
		/if (!${Me.Buff[Spirit of the Black Wolf].ID} && ${Me.AltAbilityReady[Group Spirit of the Black Wolf]}) {
			/if (${validate_cast[FALSE, alt, "Group Spirit of the Black Wolf", ${Me.ID}]}) /call core_cast2 "Group Spirit of the Black Wolf" alt 0 FALSE
		} else /if (!${Me.Buff[Group Spirit of the Black Wolf].ID} && !${Me.AltAbilityReady[Group Spirit of the Black Wolf]} && ${Me.AltAbilityReady[Spirit of the Black Wolf]}) {
			/if (${validate_cast[FALSE, alt, "Spirit of the Black Wolf", ${Me.ID}]}) /call core_cast2 "Spirit of the Black Wolf" alt 0 FALSE
		}
	} else /if (${AAWolfSpirit.Equal[white]}) {	
		/if (!${Me.Buff[Spirit of the White Wolf].ID} && ${Me.AltAbilityReady[Group Spirit of the White Wolf]}) {
			/if (${validate_cast[FALSE, alt, "Group Spirit of the White Wolf", ${Me.ID}]}) /call core_cast2 "Group Spirit of the White Wolf" alt 0 FALSE
		} else /if (!${Me.Buff[Group Spirit of the White Wolf].ID} && !${Me.AltAbilityReady[Group Spirit of the White Wolf]} && ${Me.AltAbilityReady[Spirit of the White Wolf]}) {
			/if (${validate_cast[FALSE, alt, "Spirit of the White Wolf", ${Me.ID}]}) /call core_cast2 "Spirit of the White Wolf" alt 0 FALSE
		}
	}
	
/return



|***
 *	DES: 
 *	USE: 
 *  NOTE: 
 ***|
sub click_DPS_DRU()
	DEBUG click_DPS_DRU()
	CHECKEXIT
	CHECKINPUTS
	/if (${is_target_dead[${Target2Kill}]}) /return
	/call click_horn_of_unity 	
	
	| epic 2.0
	/if (${validate_cast[FALSE, epicitem, "2", ${Me.ID}]} && ${Spawn[${Target2Kill}].Distance} <= ${setCombatDistance}) /call core_cast2 "${${Me.Class.ShortName}Epic2}" item 0 FALSE
	
	
/return



|***
 *	DES: casts druid Grasp of Ro AA deBuff
 *	USE: automatic
 *  NOTE: 
 ***|
sub cast_AA_ro(int _tmpID)
	DEBUG cast_AA_ro(${_tmpID})
	CHECKEXIT
	CHECKINPUTS
	/if (${timer_ro_${_tmpID}} || ${Target.Buff[${Spell[${debuffRo}].Trigger}].ID}) /return
	/if (${is_target_dead[${_tmpID}]}) /return
	
	/if (!${Bool[${debuffRo}]} || !${Me.AltAbilityReady[${debuffRo}]}) /return FALSE
	
	/if (${validate_cast[FALSE, alt, "${debuffRo}", ${_tmpID}]}) /call core_cast2 "${debuffRo}" alt ${_tmpID} FALSE
	
	/if (${Select[${castReturn},CAST_SUCCESS,CAST_IMMUNE,CAST_RESISTED,CAST_RESIST,CAST_NOHOLD,CAST_TAKEHOLD]}) {
		/call timer_update timer_ro_${_tmpID} ${Spell[${debuffRo}].Trigger.Duration.TotalSeconds}s
	}
	
/return



|***
 *	DES: Check Group and heal the fucks if the need it.
 *	USE: /healgroup
 *  NOTE: 
 ***|
sub check_heal_group_DRU()
	DEBUG check_heal_group_DRU()
	CHECKTIE
	AMIDEAD
	CHECKEXIT
	CHECKREZ
	CHECKFADE

	UPDATEHUD "HEAL Group"

	/declare _count int local 0
	/varset hurtIndex ${Group.Member[${Me}].Index}
	/varset hurtPctHP ${If[${setHealOverride},${Float[${setHealOverride}].Centi},99.99]}
	/varset hurtCLS ${Me.Class.ShortName}
	/varset hurtCount 0

	| Do group members need healing? 
	/for _count 0 to ${Group}
		/if (${Group.Member[${_count}].PctHPs} > ${${Group.Member[${_count}].Class.ShortName}Point}*.${tmpHealOverride}) /continue
		/if (${Group.Member[${_count}].PctHPs} > ${hurtPctHP}) /continue
		/if (${Group.Member[${_count}].Distance} > ${setMobAgro}) /continue
		/if (!${Group.Member[${_count}].Present} || ${Group.Member[${_count}].Dead}) /continue
		/if (${Group.Member[${_count}].DisplayName.Equal[${RampageTank}]}) /continue
		/if (${lstNoHeal.Find[${Group.Member[${_count}].DisplayName}]}) /continue

		| we found a slacker
		/varset hurtPctHP ${Group.Member[${_count}].PctHPs}
		/varset hurtIndex ${_count}
		/varset hurtCLS ${Group.Member[${_count}].Class.ShortName}
		/varcalc hurtCount ${hurtCount}+1
		/if (${useGroupHeal}) {
			/if (${hurtCount} >= ${useGroupHeal}) /break
		}
	
	/next _count
	
	
	| if no one is hurt. back to the start
	/if (!${hurtCount}) {
		UPDATEHUD FALSE
		/return	
	}
  
	CHECKEXIT

 	/call check_no_cast_buff

	| Panic heal
	/if (${Group.Member[${hurtIndex}].PctHPs} < ${PanicPoint}*.${tmpHealOverride}) {
		/if (${Me.Casting.ID}) /stopcast
		/if (${validate_cast[FALSE, spell, "${healPanic}", ${Group.Member[${hurtIndex}].ID}]}) {
			/call core_cast2 "${healPanic}" 0 ${Group.Member[${hurtIndex}].ID} FALSE
		}
	}

	
	| single AA heals
	
	| 2 min
	/if (${Group.Member[${hurtIndex}].PctHPs} < ${${hurtCLS}Point}*.${tmpHealOverride}) {
		/if (${validate_cast[FALSE, alt, "Wrath of the Wild", ${Group.Member[${hurtIndex}].ID}]}) {
			/call core_cast2 "Wrath of the Wild" alt ${Group.Member[${hurtIndex}].ID} FALSE
		}
	}	
	
	| 3 min
	/if (${Group.Member[${hurtIndex}].PctHPs} < ${${hurtCLS}Point}*.${tmpHealOverride}) {
		/if (${validate_cast[FALSE, alt, "Wildtender's Survival", ${Group.Member[${hurtIndex}].ID}]}) {
			/call core_cast2 "Wildtender's Survival" alt ${Group.Member[${hurtIndex}].ID} FALSE
		}
	}	
	| 5 min
	/if (${Group.Member[${hurtIndex}].PctHPs} < ${${hurtCLS}Point}*.7*.${tmpHealOverride}) {
		/if (${validate_cast[FALSE, alt, "Swarm of Fireflies", ${Me.ID}]}) {
			/call core_cast2 "Swarm of Fireflies" alt 0 FALSE
		}
	}
	
	| 8 min
	/if (${Group.Member[${hurtIndex}].PctHPs} < ${${hurtCLS}Point}*.6*.${tmpHealOverride}) {
		/if (${validate_cast[FALSE, alt, "Convergence of Spirits", ${Group.Member[${hurtIndex}].ID}]}) {
			/call core_cast2 "Convergence of Spirits" alt ${Group.Member[${hurtIndex}].ID} FALSE
		}
	}
	
	| 8 min (same timer)
	| /if (${Group.Member[${hurtIndex}].PctHPs} < ${${hurtCLS}Point}*.5*.${tmpHealOverride}) {
	| 	/if (${validate_cast[FALSE, alt, "Peaceful Convergence of Spirits", ${Group.Member[${hurtIndex}].ID}]}) {
	| 		/call core_cast2 "Peaceful Convergence of Spirits" alt ${Group.Member[${hurtIndex}].ID} FALSE
	| 	}
	| }
  
  
  
	| Group Heals
	CHECKINPUTS

  | Group Heal
  /if (${useGroupHeal}) {
	  /if (${hurtCount} >= ${useGroupHeal}) {
	  	
	  	| AA group heals
		  /if (!${SafeZone}) {
		  	| 9 min
		  	/if (${validate_cast[FALSE, alt, "Peaceful Spirit of the Wood", ${Me.ID}]}) {
		  		/call core_cast2 "Peaceful Spirit of the Wood" alt ${Me.ID} FALSE
		  		/return
		  	}
				| 5 min
			 	/if (${validate_cast[FALSE, alt, "Blessing of Tunare", ${Me.ID}]}) {
			 		/call core_cast2 "Blessing of Tunare" alt 0 FALSE
			 		/return
			 	}
			}			  	
	  	
	  	| group heal
	    /if (${validate_cast[FALSE, spell, "${healWholeGroup}", ${Me.ID}]}) {
	    	/call core_cast2 "${healWholeGroup}" 0 0 check_health_group
	    	/return
	    }
	  } 
	}

	| single target heals
  	/if (${Me.SpellReady[${Spell[${healRejuvilation}].RankName}]}) {
			/if (${validate_cast[FALSE, spell, "${healRejuvilation}", ${Group.Member[${hurtIndex}].ID}]}) {
				/call core_cast2 "${healRejuvilation}" 0 ${Group.Member[${hurtIndex}].ID} FALSE
				/return
			}
    }
    
    /if (${Me.SpellReady[${Spell[${healAdrenaline}].RankName}]}) {
      /if (${validate_cast[FALSE, spell, "${healAdrenaline}", ${Group.Member[${hurtIndex}].ID}]}) {
      	/call core_cast2 "${healAdrenaline}" 0 ${Group.Member[${hurtIndex}].ID} FALSE
      	/return
      }
    } 

    /if (${Me.SpellReady[${Spell[${healSanavida}].RankName}]}) {
      /if (${validate_cast[FALSE, spell, "${healSanavida}", ${Group.Member[${hurtIndex}].ID}]}) {
      	/call core_cast2 "${healSanavida}" 0 ${Group.Member[${hurtIndex}].ID} FALSE
      	/return
      }
    }
    
	UPDATEHUD FALSE
/return




|***
 *	DES: self heal only
 *	USE: /call check_heal_self_${Me.Class.ShortName}
 *  NOTE: 
 ***|
sub check_heal_self_DRU()
	DEBUG check_heal_self_DRU()
	AMIDEAD
	CHECKEXIT
	CHECKTIE
	
	UPDATEHUD "HEAL Self"
	/if (${Me.PctHPs} > ${DRUPoint}*.${tmpHealOverride}) /return
	/declare _count int local 0

	| panic heal
	/if (${Me.SpellReady[${Spell[${healPanic}].RankName}]}) {
		/if (${Me.PctHPs} < ${PanicPoint}*.${tmpHealOverride}) {
			/if (${Me.Casting.ID}) /stopcast
			/if (${validate_cast[FALSE, spell, "${healPanic}", ${Me.ID}]}) /call core_cast2 "${healPanic}" 0 ${Me.ID} check_health_group
		}
	}	

	
	| single AA heals
	
	| 2 min
	/if (${Me.PctHPs} < ${DRUPoint}*.${tmpHealOverride}) {
		/if (${validate_cast[FALSE, alt, "Wrath of the Wild", ${Me.ID}]}) 
			/call core_cast2 "Wrath of the Wild" alt ${Me.ID} FALSE
		}
	}	
	| 3 min
	/if (${Me.PctHPs} < ${DRUPoint}*.${tmpHealOverride}) {
		/if (${validate_cast[FALSE, alt, "Wildtender's Survival", ${Me.ID}]}) 
			/call core_cast2 "Wildtender's Survival" alt ${Me.ID} FALSE
		}
	}	
	| 5 min
	/if (${Me.PctHPs} < ${DRUPoint}*.${tmpHealOverride}) {
		/if (${validate_cast[FALSE, alt, "Swarm of Fireflies", ${Me.ID}]}) {
			/call core_cast2 "Swarm of Fireflies" alt 0 FALSE
		}
	}
	
	| 8 min
	/if (${Me.PctHPs} < ${DRUPoint}*.${tmpHealOverride}) {
		/if (${validate_cast[FALSE, alt, "Convergence of Spirits", ${Me.ID}]}) {
			/call core_cast2 "Convergence of Spirits" alt ${Me.ID} FALSE
		}
	}	
	
	| single target heals
	/if (${Me.SpellReady[${Spell[${healRejuvilation}].RankName}]}) {
   /if (${validate_cast[FALSE, spell, "${healRejuvilation}", ${Me.ID}]}) /call core_cast2 "${healRejuvilation}" 0 ${Me.ID} FALSE
  } else /if (${Me.SpellReady[${Spell[${healAdrenaline}].RankName}]}) {
    /if (${validate_cast[FALSE, spell, "${healAdrenaline}", ${Me.ID}]}) /call core_cast2 "${healAdrenaline}" 0 ${Me.ID} FALSE
  } else /if (${Me.SpellReady[${Spell[${healSanavida}].RankName}]}) {
    /if (${validate_cast[FALSE, spell, "${healSanavida}", ${Me.ID}]}) /call core_cast2 "${healSanavida}" 0 ${Me.ID} FALSE
  } 	

/return



|***
 *	DES: Check XTargets and heal the bastards if the need it.
 *	USE: /healxtarget
 *  NOTE: 
 ***|
sub check_heal_XTarget_DRU()
	DEBUG check_heal_XTarget_DRU()
	/if (!${validate_expansion[FALSE, |16]}) /return
	CHECKREZ
	AMIDEAD
	SIMPLEEXIT
	CHECKINPUTS

	UPDATEHUD "HEAL XTarget"
	
	/declare _count int local 0
	/varset hurtIndex ${Group.Member[${Me}].Index}
	/varset hurtPctHP ${If[${setHealOverride},${Float[${setHealOverride}].Centi},99.99]}
	/varset hurtCLS ${Me.Class.ShortName}
	/varset hurtCount 0

	| does anyone need healing?
	/call check_health_xtarget
  	
	/if (!${hurtCount}) {
		UPDATEHUD FALSE
		/return	
	}

	/call check_no_cast_buff

	| panic heal
	/if (${Me.SpellReady[${Spell[${healPanic}].RankName}]}) {
		/if (${Me.XTarget[${hurtIndex}].PctHPs} < ${PanicPoint}*.${tmpHealOverride}) {
			/if (${Me.Casting.ID}) /stopcast
			/if (${validate_cast[FALSE, spell, "${healPanic}", ${Me.XTarget[${hurtIndex}].ID}]}) /call core_cast2 "${healPanic}" 0 ${Me.XTarget[${hurtIndex}].ID} FALSE
		}
	}
	
  /if (${Me.XTarget[${hurtIndex}].PctHPs} <= ${${hurtCLS}Point}*.${tmpHealOverride}) {
		/if (${Me.Casting.ID}) /stopcast
  	/if (${Me.SpellReady[${Spell[${healRejuvilation}].RankName}]}) {
     /if (${validate_cast[FALSE, spell, "${healRejuvilation}", ${Me.XTarget[${hurtIndex}].ID}]}) /call core_cast2 "${healRejuvilation}" 0 ${Me.XTarget[${hurtIndex}].ID} FALSE
    } else /if (${Me.SpellReady[${Spell[${healSanavida}].RankName}]}) {
      /if (${validate_cast[FALSE, spell, "${healSanavida}", ${Me.XTarget[${hurtIndex}].ID}]}) /call core_cast2 "${healSanavida}" 0 ${Me.XTarget[${hurtIndex}].ID} FALSE
    } else /if (${Me.SpellReady[${Spell[${healAdrenaline}].RankName}]}) {
      /if (${validate_cast[FALSE, spell, "${healAdrenaline}", ${Me.XTarget[${hurtIndex}].ID}]}) /call core_cast2 "${healAdrenaline}" 0 ${Me.XTarget[${hurtIndex}].ID} FALSE
    } 
  } 	

	UPDATEHUD FALSE
/return



|***
 *	DES: Checks and Heals Pets. Group ONLY
 *	USE: /healpet [toggle]
 *  NOTE: 
 ***|
sub check_heal_pet_DRU()
	DEBUG check_heal_pet_DRU()
	AMIDEAD
	SIMPLEEXIT
	CHECKINPUTS
	CHECKREZ
	UPDATEHUD "HEAL Pet"

	/declare _count int local 0
	/varset hurtIndex ${Group.Member[${Me}].Index}
  
	/call check_health_pet

	| any pets hurt?
  /if (${Group.Member[${hurtIndex}].ID} == ${Me.ID}) {
 		UPDATEHUD FALSE
 		/return
	}
  
  /if (${Group.Member[${hurtIndex}].Pet.PctHPs} < ${PetPoint}) {
  	/if (${Me.SpellReady[${Spell[${healRejuvilation}].RankName}]}) {
    	/if (${validate_cast[FALSE, spell, "${healRejuvilation}", ${Group.Member[${hurtIndex}].Pet.ID}]}) /call core_cast2 "${healRejuvilation}" 0 ${Group.Member[${hurtIndex}].Pet.ID} FALSE
  	} else /if (${Me.SpellReady[${Spell[${healSanavida}].RankName}]}) {
    	/if (${validate_cast[FALSE, spell, "${healSanavida}", ${Group.Member[${hurtIndex}].Pet.ID}]}) /call core_cast2 "${healSanavida}" 0 ${Group.Member[${hurtIndex}].Pet.ID} FALSE
    }
  } 
  
/return



|***
 *	DES: MGB druid SoWood if both AAs are ready
 *	USE: /bc wood, /wood
 *  NOTE: Will not cast wood if the buff is present.
 ***|
sub cast_AA_spirit_wood()
	DEBUG cast_AA_spirit_wood()
	AMIDEAD
	CHECKINPUTS
	/if (${Me.AltAbilityReady[Mass Group Buff]} && ${Me.AltAbilityReady[Spirit of the Wood]} && !${Bool[${Me.Song[Spirit of the Wood]}]}) {
		/if (${validate_cast[FALSE, alt, "Mass Group Buff", ${Me.ID}]}) /call core_cast2 "Mass Group Buff" alt 0 FALSE
		/if (${validate_cast[FALSE, alt, "Spirit of the Wood", ${Me.ID}]}) /call core_cast2 "Spirit of the Wood" alt 0 FALSE
	} else /if (${Bool[${Me.Song[Spirit of the Wood]}]}) {
		VOUT ${owarning} ${sep} trying to cast Spirit of the Wood while its running.	
	}		
/return



 |***
 *	DES: Druid burn routines
 *	USE: establish all comp/trigger skills
 *	NOTE: 
 ***| 
sub burn_DRU()
	DEBUG burn_DRU()
	CHECKEXIT
	CHECKINPUTS
	
	| swarm pets
	/if (${swSwarm}) {
		/if (${validate_cast[FALSE, alt, "Nature's Guardian", ${Target2Kill}]}) /call core_cast2 "Nature's Guardian" alt ${Target2Kill} FALSE
		
	}
	
	| defense
	/if (${validate_cast[FALSE, alt, "Protection of Direwood", ${Me.ID}]}) /call core_cast2 "Protection of Direwood" alt 0 FALSE
	/if (${validate_cast[FALSE, alt, "Silent Casting", ${Me.ID}]}) /call core_cast2 "Silent Casting" alt 0 FALSE

	| damage boost
	/if (${validate_cast[FALSE, alt, "Distructive Vortex", ${Me.ID}]}) /call core_cast2 "Distructive Vortex" alt 0 FALSE
	/if (${validate_cast[FALSE, alt, "Natures Fury", ${Me.ID}]}) /call core_cast2 "Natures Fury" alt 0 FALSE
	/if (${validate_cast[FALSE, alt, "Group Spirit of the Black Wolf", ${Me.ID}]}) /call core_cast2 "Group Spirit of the Black Wolf" alt 0 FALSE
	/if (${validate_cast[FALSE, alt, "Seasons Wrath", ${Target2Kill}]}) /call core_cast2 "Seasons Wrath" alt ${Target2Kill} FALSE
	
	| heal boost
	/if (${validate_cast[FALSE, alt, "Nature's Blessing", ${Me.ID}]}) /call core_cast2 "Nature's Blessing" alt 0 FALSE

	/call cast_AA_spire
	
	| AA AE heal
	/if (${validate_cast[FALSE, alt, "Nature's Boon", ${Me.ID}]}) /call core_cast2 "Nature's Boon" alt 0 FALSE

	/call click_burn_circle
	/call click_item_burn
/return



|***
 *	DES: Class controls
 *	USE: /dru [groupheal]
 *  NOTE:
 ***|
#bind setDRU /dru
sub Bind_setDRU(string _type, string _verbage)
	/if (!${validate_class[TRUE, |DRU]}) /return

	/declare _sep bool local
	/declare _listout string local
	/declare _count int local 0
	/declare _bind string local ${Me.Class.ShortName.Lower}
  /declare _lstClass string local |fade|groupheal|grow|healpoint|rampage|rod|spiregroup|spireraid|wolf|xttime

	| use Modulation Rods
	/if (${_type.Equal[rod]}) {
		/call set_switch ${chrID} swModRod ${_verbage} 	

	| use group Heals
	} else /if (${_type.Equal[groupheal]}) {
		/call set_character_groupheal FALSE ${_verbage}

	| use grow temp buff
	} else /if (${_type.Equal[grow]}) {
		/call set_switch ${chrID} switchGrowth Spells ${_verbage} 	

	| set rampage tank toon
	} else /if (${_type.Equal[rampage]}) {
		/if (${Bool[${_verbage}]}) {
			/call sql_update FALSE ${chrID} RampageTank ${_verbage.Left[1].Upper}${_verbage.Right[-1].Lower}
		} else /if (!${Bool[${_verbage}]}) {
			/call sql_update FALSE ${chrID} RampageTank FALSE
		}
		
	| wolf spirit AAs
	} else /if (${_type.Equal[wolf]}) {
		/declare _wolflist string local |FALSE|black|white

		/if (${_verbage.Equal[false]}) {
			/call sql_update FALSE ${chrID} AAWolfSpirit FALSE
		} else /if (${Bool[${_verbage.Equal[black]}]}) {
			/call sql_update FALSE ${chrID} AAWolfSpirit black	
		} else /if (${Bool[${_verbage.Equal[white]}]}) {
			/call sql_update FALSE ${chrID} AAWolfSpirit white 	
		}

		/for _count 1 to ${_wolflist.Count[|]}
			/varset _listout ${_listout} ${If[${_sep},${sep},]} ${If[${AAWolfSpirit.Equal[${_wolflist.Arg[${_count},|]}]},${cgood},${cinfo}]}${_wolflist.Arg[${_count},|]}\ax
			/varset _sep TRUE
		/next _count
		VOUT /dru wolf [${_listout}]
		/return

	| set Class Heal Point	
	} else /if (${_type.Equal[healpoint]}) {
		/call set_character_healpoint FALSE ${_verbage}
		/return

	| set AA fade use %	
	} else /if (${_type.Equal[fade]}) {
		/call set_character_fade FALSE ${_verbage}

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
		/if (${_lstClass.Arg[${_count},|].Equal[fade]}) /varset _listout ${_listout}:${If[${switchAAFade},${cnum}${useAAFadePct}\ax,${cbad}${useAAFadePct}\ax]}
		/if (${_lstClass.Arg[${_count},|].Equal[groupheal]}) /varset _listout ${_listout}:${If[${useGroupHeal},${cnum}${useGroupHeal}\ax,${ooff}]}
		/if (${_lstClass.Arg[${_count},|].Equal[grow]}) /varset _listout ${_listout}:${If[${switchGrowth},${oon},${ooff}]}
		/if (${_lstClass.Arg[${_count},|].Equal[healpoint]}) /varset _listout ${_listout}:${cnum}->\ax
		/if (${_lstClass.Arg[${_count},|].Equal[rampage]}) /varset _listout ${_listout}:${If[${Bool[${RampageTank}]},${cnum}${RampageTank}\ax,${ooff}]}
		/if (${_lstClass.Arg[${_count},|].Equal[rod]}) /varset _listout ${_listout}:${If[${Bool[${swModRod}]},${oon},${ooff}]}
		/if (${_lstClass.Arg[${_count},|].Equal[spiregroup]}) /varset _listout ${_listout}:${cnum}${spireGroup}\ax
		/if (${_lstClass.Arg[${_count},|].Equal[spireraid]}) /varset _listout ${_listout}:${cnum}${spireRaid}\ax
		/if (${_lstClass.Arg[${_count},|].Equal[wolf]}) /varset _listout ${_listout}:${cnum}->\ax
		/if (${_lstClass.Arg[${_count},|].Equal[xttime]}) /varset _listout ${_listout}:${cnum}${tXHeal}\ax
		/varset _sep TRUE
	/next _count
	VOUT /${_bind} [${_listout}]
/return




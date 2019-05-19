|***
 *	class.clr.inc
 *	Core Cleric Include
 *
 * 
 ***| 
 


|***
 *	DES: Main Cleric Routine
 *	USE: returning the dead to life 'cause they were stupid
 *  NOTE: 
 ***|
sub core_CLR()
	DEBUG core_CLR()
	
	| correct any and all XTarget healing spots
	/if (${coreAuto}) {
	  /if (${swHealXTarget} && !${timer_set_xtarget}) {
	    /call set_xtarget
	    /call timer_update timer_set_xtarget ${tXHeal}
		}
	
		| sort the summoned cleric primary and any mana regen from VP if needed
		/if (!${Me.Invis}) {
			/if (${Bool[${sumWeaponItem}]} && (${InvSlot[mainhand].Item.Name.NotEqual[${sumWeaponItem}]} || !${Bool[${Me.Primary}]})) /call cast_sum_weapon
			/if (!${check_exit[]} && !${SafeZone} && !${Me.Buff[Revival Sickness].ID} && ${switchVP}) {
			 	/if (${Me.PctMana} < ${useVPPct}*.${tmpHealOverride} && !${swNeedRez} && !${swAmIDead}) {
			 		/if (${validate_cast[FALSE, alt, "Veturika's Perseverance", ${Me.ID}]}) /call core_cast2 "Veturika's Perseverance" alt 0 FALSE
			 	}
			}
		}
	}

	/while (${prep_combat[]}) {
		CHECKINPUTS
		CHECKREZ
		/if (${swHealSelf} && !${Group.Members}) /call check_heal_self_${Me.Class.ShortName}
		/if (${swHealGroup} && ${Group.Members}) /call check_heal_group_${Me.Class.ShortName}
		/if (${swHealXTarget}) /call check_heal_XTarget_${Me.Class.ShortName}
		/if (${swHealPet} && ${Group.Members}) /call check_heal_pet_${Me.Class.ShortName}
		/if (${swHealCure}) /call cast_cure
		/if (${doYaulp}) /call cast_yaulp
		CHECKFADE
		/if (${swAlliance}) /call trigger_alliance
		/if (!${timer_heal_ward}) /call check_heal_ward
		/if (${swRetort}) /call check_buffs_tmp_list "${spellRetort}" "${spellRetortToon}"

		/if (${swModRod} && ${validate_class[FALSE, ${lstClassCast}]}) /call use_mod_rod
		/if (${swBurnControl}) /call burn_${Me.Class.ShortName}
		/if (${exit_class_loop[]}) /return
		
		/if (${isValidTarget}) {
			CHECKREZ
			/call set_combat ${combat.Upper}
			/if (${swCombatMelee}) /call use_skill_melee ${Target2Kill}
			/call disc_${Me.Class.ShortName}
			/call check_deBuffs ${Target2Kill}
			/if (${swCombatDoT}) /call cast_DOT
			/if (${swCombatNuke}) /call cast_nuke
			/call click_BP
			/call click_item
			/call click_DPS_${Me.Class.ShortName}
		}
	}

/return



|***
 *	DES: DPS Clickies
 *	USE: this shits auto, learn to love it.
 *  NOTE: 
 ***|
sub click_DPS_CLR()
	DEBUG click_DPS_CLR()
	AMIDEAD
	CHECKEXIT
	/if (${is_target_dead[${Target2Kill}]}) /return
	/call click_horn_of_unity 	
	/if (${swSwarm}) /call click_item_swarm ${Target2Kill}
/return



|***
 *	DES: Combat skills, Disciplines and Alternate abilities.
 *	USE: Disc
 *  NOTE: 
 ***|
sub disc_CLR()
	DEBUG disc_CLR()
	CHECKTIE
	AMIDEAD
	CHECKEXIT
	CHECKINPUTS
	/if (${is_target_dead[${Target2Kill}]}) /return

	/if (${doYaulp}) /call cast_yaulp

	/if (${swSwarm} && !${Me.Pet.ID}) /call cast_temp_pet ${Target2Kill} "${tempPet}"

	| pet AA hammer	
	/if (${swBurnForce} || (${swBurnAuto} && ${NameList.Find[${Target.DisplayName}]})) {
		/if (${validate_cast[FALSE, alt, "Celestial Hammer", ${Target2Kill}]}) /call core_cast2 "Celestial Hammer" alt ${Target2Kill} FALSE
	}	
	
	| undead aa nuke
	/if (${Target.Body.Name.Equal[Undead]}) {
		/if (${validate_cast[FALSE, alt, "Turn Undead", ${Target2Kill}]}) /call core_cast2 "Turn Undead" alt ${Target2Kill} FALSE
	}	
/return



|***
 *	DES: This will summon the clerics awesom nut smasher hammer if you do not have a good primary weapon.
 *	USE: 
 *  NOTE: 
 ***|
sub cast_sum_weapon()
	DEBUG cast_sum_weapon()
	/if (${Me.Primary}) /return
	CHECKTIE
	AMIDEAD
	CHECKEXIT
	CHECKINPUTS
	/if (!${Bool[${FindItem[${sumWeaponItem}]}]}) {
		/if (${validate_cast[FALSE, spell, "${sumWeaponSpell}", ${Me.ID}]}) /call core_cast2 "${sumWeaponSpell}" ${buffGem} ${Me.ID} FALSE
	}
	/if (${Bool[${Cursor.ID}]}) {
		/call sort_cursor TRUE
	} else {
		/exchange "${sumWeaponItem}" mainhand 
	}
/return



|***
 *	DES: cleric main heal sub
 *	USE: /call check_heal_group_${Me.Class.ShortName}
 *  NOTE: bread and butter baby...
 ***|
sub check_heal_group_CLR()
	DEBUG check_heal_group_CLR()
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
  	
	| Group Regen Heal 
 	/if (${useGroupHoT} && !${hurtCount} && !${SafeZone}) {
	 	/if (${SpawnCount[npc radius ${setMobAgro} playerstate 4 targetable]} >= 1) {
			/if (!${Me.Song[Focused Celestial Regeneration].ID} && !${Me.Song[${healWholeGroupHoT}].ID} && !${Me.Song[Shear of Renewal].ID} && !${Me.Song[Ghost of Renewal].ID} && !${Me.Song[Cloud of Renewal].ID}) {


				/declare _found bool TRUE
				| check regen SPA
				/if (${Spell[${healWholeGroupHoT}].HasSPA[374]}) {
					/for _count 1 to ${Spell[${healWholeGroupHoT}].NumEffects}
						/if (!${Spell[${Spell[${healWholeGroupHoT}].Base2[${_count}]}].Duration}) /continue
						/if (!${Me.Song[${Spell[${healWholeGroupHoT}].Base2[${_count}]}].ID}) /varset _found FALSE
						/if (!${Spell[${Spell[${healWholeGroupHoT}].Base2[${_count}]}].NewStacks}) /varset _found TRUE
						/if (${_found}) /break
					/next _count
				} else {
					/if (!${Me.Song[${healWholeGroupHoT}].ID}) /varset _found FALSE
					/if (${Spell[${healWholeGroupHoT}].NewStacks}) /varset _found FALSE
				}
				/if (!${_found} && ${validate_cast[FALSE, spell, "${healWholeGroupHoT}", ${Me.ID}]}) {
					/call core_cast2 "${healWholeGroupHoT}" ${buffGem} 0 FALSE
				}
			}
		}
	}
	
	| if no one is hurt. back to the start
	/if (!${hurtCount}) {
		UPDATEHUD FALSE
		/return	
	}
	
	| Complete Heal
	| requires group roll Main Tank set to function
	/if (${switchCompleteHeal} && ${Group.Member[${hurtIndex}].ID} == ${Group.MainTank.ID}) { 
		/if (${Group.MainTank.PctHPs} <= ${${Group.MainTank.Class.ShortName}Point}*.${CompleteHealpct}) {
			/if (${validate_cast[FALSE, spell, "Complete Heal", ${Group.MainTank.ID}]}) {
				/call core_cast2 "Complete Heal" 0 ${Group.MainTank.ID}
			}
		}
	}

	| Focused Celestial Regeneration - out of raid only
	/if (!${Raid.Members} && !${SafeZone}) {
		/if (${Group.Member[${hurtIndex}].PctHPs} <= ${${hurtCLS}Point}*.${tmpHealOverride} && !${Me.Song[Celestial Regeneration].ID}) { 
			/if (${validate_cast[FALSE, alt, "Focused Celestial Regeneration", ${Group.Member[${hurtIndex}].ID}]}) {
				/call core_cast2 "Focused Celestial Regeneration" alt ${Group.Member[${hurtIndex}].ID} FALSE
			}
		}
	}
	
	| Dichotomic Heal
	/if (${Me.Song[Gift of Mana].ID}) {
		/if (${Group.Member[${hurtIndex}].PctHPs} < ${GoMpoint}*.${tmpHealOverride}) { 
			/if (${validate_cast[FALSE, spell, "${GoMNuke}", ${Group.Member[${hurtIndex}].ID}]}) {
				/call core_cast2 "${GoMNuke}" spell ${Group.Member[${hurtIndex}].ID} FALSE
			}
	  }
	}
  
	CHECKFADE

	| Panic AA single target heal
  /if (${Group.Member[${hurtIndex}].PctHPs} <= ${PanicPoint}*.${tmpHealOverride}) {
		/if (${validate_cast[FALSE, alt, "Burst of Life", ${Group.Member[${hurtIndex}].ID}]}) {
			/call core_cast2 "Burst of Life" alt ${Group.Member[${hurtIndex}].ID}
		}
  }
	
	| panic group heal. AA Beacon of life (3 min use)
  /if ((${hurtCount} >= ${Math.Calc[${Group.Members}/2]}) && ${Group.Members} > 2 && !${SafeZone}) {
	  /if (${validate_cast[FALSE, alt, "Beacon of Life", ${Me.ID}]}) {
	  	/call core_cast2 "Beacon of Life" alt 0
	  }
	}		  	
  
	| AA Rune
	/if (${Group.Member[${hurtIndex}].PctHPs} < ${PanicPoint}*.${tmpHealOverride} && ${Me.SpellReady[${Spell[${healPanic}].RankName}]}) {
		/if (${validate_cast[FALSE, spell, "${healPanic}", ${Group.Member[${hurtIndex}].ID}]} && ${Me.Casting.ID}) /stopcast
		/call core_cast2 "${healPanic}" 0 ${Group.Member[${hurtIndex}].ID}
		/if (!${Raid.Members} && ${validate_cast[FALSE, alt, "Divine Guardian", ${Group.Member[${hurtIndex}].ID}]}) {
			/call core_cast2 "Divine Guardian" alt ${Group.Member[${hurtIndex}].ID}
		}
	}

	| Arbitration
	/if (${swArbitrate} && ${useGroupHeal}) {
	  /if (${Group.Member[${hurtIndex}].PctHPs} < ${${hurtCLS}Point}*.${tmpHealOverride}*.5 || ${hurtCount} >= ${useGroupHeal}) {
	    /call cast_divine_arbitration
	  }
	}
    
  | Group Heal
  /if (${useGroupHeal}) {
	  /if (${hurtCount} >= ${useGroupHeal}) {
	    /if (${validate_cast[FALSE, spell, "${healWholeGroup}", ${Me.ID}]}) {
	    	/call core_cast2 "${healWholeGroup}" 0 0 check_health_group
	    }
	  } 
	}
	
	| issuance AE heal
	/if (${switchIssuance} && !${SafeZone}) {
		/if (${SpawnCount[npc radius ${setMobAgro} playerstate 4]} >= ${issuanceMobCount}) /call Bind_issuance
	}
	
	| splash heal
	/if (${swSplash} && (${Group} > 1 || ${Raid})) {
		/if (${validate_cast[FALSE, spell, "${splashHeal}", ${Me.ID}]}) {
			/cast ${Spell[${splashHeal}].RankName} loc ${Me.X} ${Me.Y} ${Me.Z}
		}
	}

	| check for personal debuffs
	/if (${swHealCure}) {
		| AA Purified Spirits SELF ONLY 0' range 2 min
		/if (${Debuff.Count} && ${validate_cast[FALSE, alt, "Purified Spirits", ${Me.ID}]}) /call core_cast2 "Purified Spirits" alt ${Me.ID} FALSE
		| AA Radient Cure SELF ONLY 100' range 1 min
		/if (${Debuff.Count} && ${validate_cast[FALSE, alt, "Radiant Cure", ${Me.ID}]}) /call core_cast2 "Radiant Cure" alt 0 FALSE 
	}
	
	| heal breastplate
	/if (${Bool[${BreastplateHeal}]}) {
		/if (${Me.ItemReady[${BreastplateHeal}]} && ${cast_condition[BreastplateHealCondition]}) {
			/if (${validate_cast[FALSE, item, "${BreastplateHeal}", ${Group.Member[${hurtIndex}].ID}]}) /call core_cast2 "${BreastplateHeal}" item ${Group.Member[${hurtIndex}].ID}
		}
	}
	  
	AMIDEAD
	CHECKFADE
 	CHECKREZ
 	
 	| agressive healing sort
 	/if (${switchHealAggressive}) {
 		/if (${Me.SpellReady[${Spell[${healIntervention1}].RankName}]} || ${Me.SpellReady[${Spell[${healIntervention2}].RankName}]} || ${Me.SpellReady[${Spell[${healIntervention3}].RankName}]} || ${Me.SpellReady[${Spell[${healIntervention4}].RankName}]}) /goto :aggressivesort
 	} 

 	/call check_health_group
 	
 	| Remedy Heals 
  /if (${Group.Member[${hurtIndex}].PctHPs} <= ${${hurtCLS}Point}*.${tmpHealOverride}) {
		/for _count 1 to 3
			| /if (!${Me.SpellReady[${Spell[${healRemedy${_count}}].RankName}]} || ${Me.SpellInCooldown}) /continue
			/if (${validate_cast[FALSE, spell, "${healRemedy${_count}}", ${Group.Member[${hurtIndex}].ID}]}) {
				/call core_cast2 "${healRemedy${_count}}" 0 ${Group.Member[${hurtIndex}].ID}
				/break
			}
		/next _count 
	}	
	
	| Intervention Heals
	:aggressivesort
  /if (${Group.Member[${hurtIndex}].PctHPs} <= ${${hurtCLS}Point}*.${tmpHealOverride}) {
		/for _count 1 to 4
			| /if (!${Me.SpellReady[${Spell[${healIntervention${_count}}].RankName}]} || ${Me.SpellInCooldown}) /continue
			/if (${validate_cast[FALSE, spell, "${healIntervention${_count}}", ${Group.Member[${hurtIndex}].ID}]}) {
				/call core_cast2 "${healIntervention${_count}}" 0 ${Group.Member[${hurtIndex}].ID}
				/break
			}
		/next _count
	}
	
	| Renewal Heals
  /if (${Group.Member[${hurtIndex}].PctHPs} <= ${${hurtCLS}Point}*.${tmpHealOverride}) {
		/for _count 1 to 3
			| /if (!${Me.SpellReady[${Spell[${healRenewal${_count}}].RankName}]} || ${Me.SpellInCooldown}) /continue
			/if (${validate_cast[FALSE, spell, "${healRenewal${_count}}", ${Group.Member[${hurtIndex}].ID}]}) {
				/call core_cast2 "${healRenewal${_count}}" 0 ${Group.Member[${hurtIndex}].ID}
				/break
			}
		/next _count 
	}

 	| single heal over time
	/if (${Bool[${healHoT}]}) {
		/if (${Group.Member[${hurtIndex}].PctHPs} <= ${${hurtCLS}Point}*.${tmpHealOverride}) {
			/declare _stacks bool local FALSE
			/declare _has bool local FALSE
			/dquery ${Group.Member[${hurtIndex}].DisplayName} -q "Spell[${healHoT}].Stacks" -o _stacks
			/dquery ${Group.Member[${hurtIndex}].DisplayName} -q "Me.Song[${healHoT}].Duration" -o _has
			/if (${_stacks} && !${_has}) {
				/if (${validate_cast[FALSE, spell, "${healHoT}", ${Group.Member[${hurtIndex}].ID}]}) {
					/call core_cast2 "${healHoT}" ${buffGem} ${Group.Member[${hurtIndex}].ID} 
				}
			}
		}
	}

  | check for group debuffs
	/if (${swHealCure} && ${Group.Members}) /call cast_cure
	
	UPDATEHUD FALSE
/return



|***
 *	DES: self heal only
 *	USE: /call check_heal_self_${Me.Class.ShortName}
 *  NOTE: 
 ***|
sub check_heal_self_CLR()
	DEBUG check_heal_self_CLR()
	CHECKTIE
	AMIDEAD
	CHECKEXIT
	
	UPDATEHUD "HEAL Self"
	/if (${Me.PctHPs} > ${CLRPoint}*.${tmpHealOverride}) /return
	/declare _count int local 0

	| self single target regen
	/if (${validate_cast[FALSE, spell, "${healHoT}", ${Me.ID}]}) {
		/call core_cast2 "${healHoT}" ${buffGem} ${Me.ID} 
	}

	| check for personal debuffs
	/if (${swHealCure}) {
		/if (${Debuff.Count}) {
			| AA Purified Spirits SELF ONLY 0' range 2 min
			/if (${validate_cast[FALSE, alt, "Purified Spirits", ${Me.ID}]}) {
				/call core_cast2 "Purified Spirits" alt ${Me.ID} 
				/return
			}
			| AA Radient Cure 100' range 1 min
			/if (${validate_cast[FALSE, alt, "Radiant Cure", ${Me.ID}]}) {
				/call core_cast2 "Radiant Cure" alt 0  
			}
		}
	}
	
	| heal breastplate
	/if (${Bool[${BreastplateHeal}]} && ${Me.ItemReady[${BreastplateHeal}]}) {
		/if (${Me.PctHPs} <= ${CLRPoint}*.${tmpHealOverride}*.5 && ${cast_condition[BreastplateHealCondition]}) {
			/if (${validate_cast[FALSE, item, "${BreastplateHeal}", ${Me.ID}]}) {
				/call core_cast2 "${BreastplateHeal}" item 0 
			}
		}
	}
	
	| Panic AA single target heal and rune
  /if (${Me.PctHPs} <= ${PanicPoint}*.${tmpHealOverride}) {
  	
  	| aa heal
		/if (${validate_cast[FALSE, alt, "Burst of Life", ${Me.ID}]}) {
			/call core_cast2 "Burst of Life" alt ${Me.ID} 
		}

		| Panic Rune
		/if (${validate_cast[FALSE, alt, "Divine Guardian", ${Me.ID}]}) {
			/call core_cast2 "Divine Guardian" alt ${Me.ID} 
		}
	}
	
 	| agressive healing sort
 	/if (${switchHealAggressive}) {
 		/if (${Me.SpellReady[${Spell[${healIntervention1}].RankName}]} || ${Me.SpellReady[${Spell[${healIntervention2}].RankName}]} || ${Me.SpellReady[${Spell[${healIntervention3}].RankName}]} || ${Me.SpellReady[${Spell[${healIntervention4}].RankName}]}) /goto :aggressivesort
 	} 
 	
 	| Remedy Heals 
  /if (${Me.PctHPs} <= ${CLRPoint}*.${tmpHealOverride}) {
		/for _count 1 to 3
			/if (!${Me.SpellReady[${Spell[${healRemedy${_count}}].RankName}]}) /continue
			/if (${validate_cast[FALSE, spell, "${healRemedy${_count}}", ${Me.ID}]}) {
				/call core_cast2 "${healRemedy${_count}}" 0 ${Me.ID} 
				/break
			}
		/next _count 
	}	
	
	| Contravention Heals
	:aggressivesort
  /if (${Me.PctHPs} <= ${CLRPoint}*.${tmpHealOverride}) {
		/for _count 1 to 4
			/if (!${Me.SpellReady[${Spell[${healContravention${_count}}].RankName}]}) /continue
			/if (${validate_cast[FALSE, spell, "${healContravention${_count}}", ${Target2Kill}]}) {
				/call core_cast2 "${healContravention${_count}}" 0 ${Target2Kill} FALSE
				/break
			}
		/next _count
	}	
	
	| Renewal Heals
  /if (${Me.PctHPs} <= ${CLRPoint}*.${tmpHealOverride}) {
		/for _count 1 to 3
			/if (${Me.SpellReady[${Spell[${healRenewal${_count}}].RankName}]}) /continue
			/if (${validate_cast[FALSE, spell, "${healRenewal${_count}}", ${Me.ID}]}) {
			/call core_cast2 "${healRenewal${_count}}" 0 ${Me.ID} FALSE
				/break
			}
		/next _count 
	}

	UPDATEHUD FALSE
/return



|***
 *	DES: Check XTargets and heal the bastards if the need it.
 *	USE: /call check_heal_XTarget_${Me.Class.ShortName}
 *  NOTE: 
 ***|
sub check_heal_XTarget_CLR()
	DEBUG check_heal_XTarget_CLR()
	/if (!${validate_expansion[FALSE, |16]}) /return
	CHECKTIE
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
	/for _count 1 to 13
  	/if (!${Me.XTarget[${_count}].ID}) /continue
  	/if (${Me.XTarget[${_count}].Dead}) /continue
  	/if (${Me.XTarget[${_count}].PctHPs} > ${hurtPctHP}) /continue
  	/if (${Me.XTarget[${_count}].Distance} > ${setMobAgro}) /continue
		/if (${Me.XTarget[${_count}].DisplayName.Equal[${RampageTank}]}) /continue
		/if (${lstNoHeal.Find[${Me.XTarget[${_count}].DisplayName}]}) /continue
  	/if (!${Me.XTarget[${_count}].Type.Equal[PC]} && !${ProtectNPCList.Find[${Me.XTarget[${_count}].DisplayName}]}) /continue
		/if (${Me.XTarget[${_count}].PctHPs} > ${${Me.XTarget[${_count}].Class.ShortName}Point}*.${tmpHealOverride}) /continue

		/varset hurtIndex ${_count}
		/varset hurtPctHP ${Me.XTarget[${_count}].PctHPs}
		/varset hurtCLS ${Me.XTarget[${_count}].Class.ShortName}
		/varset hurtCount 1

  /next _count
  	
	/if (!${hurtCount}) {
		UPDATEHUD FALSE
		/return	
	}

	/call check_no_cast_buff

	| AA heal
	/if (${Me.XTarget[${hurtIndex}].PctHPs} < 45*.${tmpHealOverride} && ${Me.AltAbilityReady[Burst of Life]}) {
		/if (${validate_cast[FALSE, alt, "Burst of Life", ${Me.XTarget[${hurtIndex}].ID}]}) {
			/call core_cast2 "Burst of Life" alt ${Me.XTarget[${hurtIndex}].ID} FALSE
		}
	}

	| panic heal
	/if (${Me.XTarget[${hurtIndex}].PctHPs} < ${PanicPoint}*.${tmpHealOverride} && ${Me.SpellReady[${Spell[${healPanic}].RankName}]}) {
		/if (${Me.Casting.ID}) /stopcast
		/if (${validate_cast[FALSE, spell, "${healPanic}", ${Me.XTarget[${hurtIndex}].ID}]}) {
			/call core_cast2 "${healPanic}" 0 ${Me.XTarget[${hurtIndex}].ID} FALSE
		}
	}
	
 	| agressive healing sort
 	/if (${switchHealAggressive}) {
 		/if (${Me.SpellReady[${Spell[${healIntervention1}].RankName}]} || ${Me.SpellReady[${Spell[${healIntervention2}].RankName}]} || ${Me.SpellReady[${Spell[${healIntervention3}].RankName}]} || ${Me.SpellReady[${Spell[${healIntervention4}].RankName}]}) /goto :aggressivesort
 	} 
	
	| Remedy Heals 
  /if (${Me.XTarget[${hurtIndex}].PctHPs} <= ${${hurtCLS}Point}*.${tmpHealOverride}) {
		/for _count 1 to 3
			/if (${Me.SpellReady[${Spell[${healRemedy${_count}}].RankName}]}) /continue
			/if (${validate_cast[FALSE, spell, "${healRemedy${_count}}", ${Me.XTarget[${hurtIndex}].ID}]}) {
				/call core_cast2 "${healRemedy${_count}}" 0 ${Me.XTarget[${hurtIndex}].ID} check_health_group
				/return
			}
		/next _count
	}	
	
	| Intervention Heals
	:aggressivesort
  /if (${Me.XTarget[${hurtIndex}].PctHPs} <= ${${hurtCLS}Point}*.${tmpHealOverride}) {
		/for _count 1 to 4
			/if (!${Me.SpellReady[${Spell[${healIntervention${_count}}].RankName}]}) /continue
			/if (${validate_cast[FALSE, spell, "${healIntervention${_count}}", ${Me.XTarget[${hurtIndex}].ID}]}) {
				/call core_cast2 "${healIntervention${_count}}" 0 ${Me.XTarget[${hurtIndex}].ID} check_health_group
				/return
			}
		/next _count
	}	
	
	| Renewal Heals
  /if (${Me.XTarget[${hurtIndex}].PctHPs} <= ${${hurtCLS}Point}*.${tmpHealOverride}) {
		/for _count 1 to 3
			/if (!${Me.SpellReady[${Spell[${healRenewal${_count}}].RankName}]}) /continue
			/if (${validate_cast[FALSE, spell, "${healRenewal${_count}}", ${Me.XTarget[${hurtIndex}].ID}]}) {
				/call core_cast2 "${healRenewal${_count}}" 0 ${Me.XTarget[${hurtIndex}].ID} check_health_group
				/return
			}
		/next _count
	}	
	
	UPDATEHUD FALSE
/return



|***
 *	DES: Checks and Heals Pets. Group ONLY
 *	USE: /heal pet
 *  NOTE: 
 ***|
sub check_heal_pet_CLR()
	DEBUG check_pet_heal_CLR()
	CHECKTIE
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

	| Panic heal
	/if (${Group.Member[${hurtIndex}].Pet.PctHPs} < .${PanicPoint}*${PetPoint} && ${Me.SpellReady[${Spell[${healPanic}].RankName}]}) {
		/if (${Me.Casting.ID}) /stopcast
		/if (${validate_cast[FALSE, spell, "${healPanic}", ${Group.Member[${hurtIndex}].Pet.ID}]}) /call core_cast2 "${healPanic}" 0 ${Group.Member[${hurtIndex}].Pet.ID} FALSE
	}
	
 	| agressive healing sort
 	/if (${switchHealAggressive}) {
 		/if (${Me.SpellReady[${Spell[${healIntervention1}].RankName}]} || ${Me.SpellReady[${Spell[${healIntervention2}].RankName}]} || ${Me.SpellReady[${Spell[${healIntervention3}].RankName}]} || ${Me.SpellReady[${Spell[${healIntervention4}].RankName}]}) /goto :aggressivesort
 	} 
 	
 	| Remedy Heals 
  /if (${Group.Member[${hurtIndex}].Pet.PctHPs} <= ${PetPoint}) {
		/for _count 1 to 3
			/if (${validate_cast[FALSE, spell, "${healRemedy${_count}}", ${Group.Member[${hurtIndex}].Pet.ID}]}) {
				/call core_cast2 "${healRemedy${_count}}" 0 ${Group.Member[${hurtIndex}].Pet.ID} FALSE
				/return
			}
		/next _count 
	}
	
	| Intervention Heals
	:aggressivesort
  /if (${Group.Member[${hurtIndex}].Pet.PctHPs} <= ${PetPoint}) {
		/for _count 1 to 4
			/if (${validate_cast[FALSE, spell, "${healIntervention${_count}}", ${Group.Member[${hurtIndex}].Pet.ID}]}) {
				/call core_cast2 "${healIntervention${_count}}" 0 ${Group.Member[${hurtIndex}].Pet.ID} FALSE
				/return
			}
		/next _count
	}
	
	| Renewal Heals
  /if (${Group.Member[${hurtIndex}].Pet.PctHPs} <= ${PetPoint}) {
		/for _count 1 to 3
			/if (${validate_cast[FALSE, spell, "${healRenewal${_count}}", ${Group.Member[${hurtIndex}].Pet.ID}]}) {
				/call core_cast2 "${healRenewal${_count}}" 0 ${Group.Member[${hurtIndex}].Pet.ID} FALSE
				/return
			}
		/next _count 
	}
		
	UPDATEHUD FALSE
/return



|***
 *	DES: Use cleric divine arbitration AA or Epic shield click.
 *	USE: auto.
 *  NOTE: DB::CLR::setDivinetoGroupHeal=TRUE will force a group heal after the health arbitration.
 ***|
sub cast_divine_arbitration()
	DEBUG cast_divine_arbitration()
	AMIDEAD
	SIMPLEEXIT
	
	| Second epic shield
  /if (${Me.ItemReady[=${CLREpic2}]} && !${FindItem[=${CLREpic2}].Timer}) {    
    /if (${validate_cast[FALSE, item, "${CLREpic2}", ${Me.ID}]}) /call core_cast2 "${CLREpic2}" item 0 FALSE
    
  | First epic shield  
  } else /if (${Me.ItemReady[=${CLREpic15}]} && !${FindItem[=${CLREpic15}].Timer}) {
    /if (${validate_cast[FALSE, item, "${CLREpic15}", ${Me.ID}]}) /call core_cast2 "${CLREpic15}" item 0 FALSE
    
  | AA arb
  } else /if (${Me.AltAbilityReady[Divine Arbitration]}) {
    /if (${validate_cast[FALSE, alt, "Divine Arbitration", ${Me.ID}]}) /call core_cast2 "Divine Arbitration" alt 0 FALSE
  }
  	
	/if (${setDivinetoGroupHeal} && ${useGroupHeal}) {
		/if (${validate_cast[FALSE, spell, "${healWholeGroup}", ${Me.ID}]}) /call core_cast2 "${healWholeGroup}" 0 0 FALSE
	}
/return



|***
 *	DES: MGB cleric celestial regeneration if both AAs are ready
 *	USE: /bc cr, /cr
 *  NOTE: 
 *	Will not cast cr if the buff is present or if your name is Norbert.
 ***|
Sub cast_AA_celest_regen()
	DEBUG cast_AA_celest_regen()
	AMIDEAD
	SIMPLEEXIT
	CHECKINPUTS
	/if (${Me.AltAbilityReady[Mass Group Buff]} && ${Me.AltAbilityReady[Celestial Regeneration]} && !${Bool[${Me.Song[Celestial Regeneration]}]}) {
		/if (${validate_cast[FALSE, alt, "Mass Group Buff", ${Me.ID}]}) /call core_cast2 "Mass Group Buff" alt ${Me.ID} FALSE
		/if (${validate_cast[FALSE, alt, "Celestial Regeneration", ${Me.ID}]}) /call core_cast2 "Celestial Regeneration" alt ${Me.ID} FALSE
	} else /if (${Bool[${Me.Song[Celestial Regeneration]}]}) {
		VOUT ${cbad}QUIT\ax trying to cast Celestial Regeneration while its running.	
	}		
	CHECKFADE
/return



|***
 *	DES: Will the cleric floatie shield AE regen. Twin cast it if the AA is ready.
 *	USE: spell must be memed.
 *  NOTE: 
 ***|
#bind issuance /issuance
sub Bind_issuance()
	/if (!${Select[${Me.Class.ShortName},CLR]}) /return FALSE
	AMIDEAD
	SIMPLEEXIT
	
	DEBUG Bind_issuance()
	/if (!${Me.SpellReady[${Spell[${healIssuance}].RankName}]}) {
		/delay 2.5s ${Me.SpellReady[${Spell[${healIssuance}].RankName}]}
	}

	:castIssuance
	/if (!${Spawn[${Target2Kill}].CanSplashLand}) /return 

	/if (${Spawn[${Target2Kill}].Distance} > 100) {
		/if (${validate_cast[FALSE, spell, "${healIssuance}", ${Target2Kill}]}) /cast "${healIssuance}" loc ${Spawn[${Target2Kill}].X} ${Spawn[${Target2Kill}].Y} ${Spawn[${Target2Kill}].Z}
		/delay 4s !${Me.Casting.ID}
	} 
	
	| can we recast?
	/if (${Me.AltAbilityReady[Forceful Rejuvenation]}) {
		
		/if (${validate_cast[FALSE, alt, "Forceful Rejuvenation", ${Me.ID}]}) /call core_cast2 "Forceful Rejuvenation" alt 0 FALSE
		/delay 4s !${Me.Casting.ID}
		/goto :castIssuance
	} 

/return	



|***
 *	DES: Buff Heal Ward of Surety spell
 *	USE: have the spell memed and the toon identified.
 *  NOTE: 
 ***|
sub check_heal_ward()
	DEBUG check_heal_ward()
	AMIDEAD
	SIMPLEEXIT
	CHECKINPUTS
	CHECKTIE

	/if (${coreAuto}) /call timer_update timer_heal_ward ${tHealWard}
	/if (!${Me.SpellReady[${Spell[${healWard}].RankName}]}) /return

	/declare _wardToon string local
	/declare _count int local
	/declare _DNqout string local

	/for _count 1 to ${healWardToon.Count[|]}
		/varset _wardToon ${healWardToon.Arg[${_count},|]}

		| me?
		/if (${swBuffSelf}) {
			/if (${_wardToon.Equal[${Me.DisplayName}]} && !${Me.Song[${healWard}].ID}) {
				/if (${validate_cast[FALSE, spell, "${healWard}", ${Me.ID}]}) /call core_cast2 "${healWard}" 0 ${Me.ID} FALSE
				/return
			}
		}

		| get out if not buffing others
		/if (!${swBuffBC}) /return
		
		| not here?
		/if (!${Spawn[pc ${_wardToon}].ID}) /continue 
		
 		| rampage tank? fuck those guys!
		/if (${Bool[${RampageTank}]} && ${RampageTank.Equal[${_wardToon}]}) /continue

		| dead?
		/if (${Spawn[${_wardToon}].Type.Equal[corpse]}) /continue

		| spell stacks ?
		/dquery ${_wardToon} -q "Spell[${healWard}].Stacks" -o _DNqout
		/if (!${_DNqout}) /continue
		
		| has the buff?
		/dquery ${_wardToon} -q "Me.Song[${healWard}].ID" -o _DNqout
		/if (${Bool[${_DNqout}]}) /continue
		
		/if (${validate_cast[FALSE, spell, "${healWard}", ${Spawn[pc ${_wardToon}].ID}]}) /call core_cast2 "${healWard}" 0 ${Spawn[pc ${_wardToon}].ID} FALSE

	/next _count	
	
/return


 
|***
 *	DES: burn routines
 *	USE: establish all comp/trigger skills
 *	NOTE: 
 ***| 
sub burn_CLR()
	DEBUG burn_CLR()
	CHECKTIE
	AMIDEAD
	SIMPLEEXIT
	CHECKINPUTS
	
	| Group AA Celestial Regeneration
 	/if (${useGroupHoT}) {
 	 	/if (${Me.AltAbilityReady[Celestial Regeneration]} && !${Bool[${Me.Song[Celestial Regeneration]}]} && !${Bool[${Me.Song[Focused Celestial Regeneration]}]}) {
	 		/if (${validate_cast[FALSE, alt, "Celestial Regeneration", ${Me.ID}]}) /call core_cast2 "Celestial Regeneration" alt 0 FALSE
	  } 
	}

	| reflect spells (3 min use)
	/if (${validate_cast[FALSE, alt, "Shield of Reverence", ${Me.ID}]}) /call core_cast2 "Shield of Reverence" alt 0 FALSE

	| alternate agro stuffs
		| anti agro (9 min reuse)
		/if (!${Me.Buff[Divine Retribution].ID}) {
			/if (${validate_cast[FALSE, alt, "Silent Casting", ${Me.ID}]}) /call core_cast2 "Silent Casting" alt 0 FALSE
		}
		| melee block anti agro (20 min reuse)
		/if (!${Me.Song[Silent Casting].ID}) {
			/if (${validate_cast[FALSE, alt, "Divine Retribution", ${Me.ID}]}) /call core_cast2 "Divine Retribution" alt 0 FALSE
		}

	| heal boosts
		| heal power bost: 35% (15 min reuse)
		/if (!${Me.Buff[Healing Frenzy].ID}) {
			/if (${validate_cast[FALSE, alt, "Flurry of Life", ${Me.ID}]}) /call core_cast2 "Flurry of Life" alt 0 FALSE
		}

		| - ~50% to cast times
		/if (!${Me.Buff[Celestial Rapidity].ID}) {
			/if (${validate_cast[FALSE, alt, "Celestial Rapidity", ${Me.ID}]}) /call core_cast2 "Celestial Rapidity" alt 0 FALSE
		}

		| 100% exceptional heal (15 min reuse)
		/if (!${Me.Song[Flurry of Life].ID}) {
			/if (${validate_cast[FALSE, alt, "Healing Frenzy", ${Me.ID}]}) /call core_cast2 "Healing Frenzy" alt 0 FALSE
		}


 	| 50% chance to twincast (10 min resuse)
 	/if (${validate_cast[FALSE, alt, "Channeling the Divine", ${Me.ID}]}) /call core_cast2 "Channeling the Divine" alt 0 FALSE

	| heal ward (20 min reuse)
	/if (${validate_cast[FALSE, alt, "Exquisite Benediction", ${Me.ID}]}) /call core_cast2 "Exquisite Benediction" alt 0 FALSE

  | AA 3rd Spire (7min 30 sec reuse)
	/if (!${Me.Buff[Third Spire of Divinity].ID}) { 
    /if (${validate_cast[FALSE, alt, "${${Me.Class.ShortName}spire.Arg[${spireGroup},|]}", ${Me.ID}]}) /call core_cast2 "${${Me.Class.ShortName}spire.Arg[${spireGroup},|]}" alt 0 FALSE
  }	
  
	| direct damage boost if using aggressive healing
	/if (${switchHealAggressive}) {
    /if (${validate_cast[FALSE, alt, "Battle Frenzy", ${Me.ID}]}) /call core_cast2 "Battle Frenzy" alt 0 FALSE
	}

	/call use_glyph
	/call click_burn_circle
	/call click_item_burn
/return



|***
 *	DES: forces an intervention spell on that target
 *	USE: /intervention [TargetID]
 *  NOTE: 
 ***|
#bind clr_intervention /intervention
sub Bind_clr_intervention(int _tmpID)
	/if (!${Bool[${_tmpID}]}) /return FALSE

	/declare _count int local 0

	/for _count 1 to 4
		/if (${Me.SpellReady[${Spell[${healIntervention${_count}}].RankName}]}) {
			/if (${validate_cast[FALSE, spell, "${healIntervention${_count}}", ${_tmpID}]}) /call core_cast2 "${healIntervention${_count}}" 0 ${_tmpID} FALSE
			/break
		}
	/next _count
	
/return TRUE



|***
 *	DES: Class controls
 *	USE: /clr []
 *  NOTE:
 ***|
#bind setCLR /clr
sub Bind_setCLR(string _type, string _verbage, bool _silent)
	/if (!${validate_class[TRUE, |CLR]}) /return

	/declare _sep bool local
	/declare _listout string local
	/declare _count int local 0
	/declare _bind string local ${Me.Class.ShortName.Lower}
  /declare _lstClass string local |aggressive|arbitrate|ch|divinerez|fade|groupheal|grouphot|healpoint|issuance|rampage|retort|rod|spiregroup|spireraid|splash|vp|xtclass|xttime|yaulp

	| enable/disable lead with aggerssive heals
	/if (${_type.Equal[aggressive]}) {
		/call set_switch ${chrID} switchHealAggressive ${_verbage}

	| use Modulation Rods
	} else /if (${_type.Equal[rod]}) {
		/call set_switch ${chrID} swModRod ${_verbage} 	

	| use Issuance Shield Heal
	} else /if (${_type.Equal[issuance]}) {
		/if (${Bool[${_verbage}]} && ${Range.Between[1,50:${Int[${_verbage}]}]}) {
			/call sql_update FALSE ${chrID} issuanceMobCount ${_verbage} 
		} else /if (${Bool[${_verbage}]} && !${Range.Between[1,50:${Int[${_verbage}]}]}) {
			VOUT /${_bind} issuance [${cinfo}${issuanceMobCount}\ax] allowable range ${sep} ${cinfo}1 - 50\ax
			/return
		}	else /if (!${Bool[${_verbage}]}) {
			/call set_switch ${chrID} switchIssuance ${_verbage} 	 
		}
		
	| mgb celestial regen
	} else /if (${_type.Equal[regen]}) {
		/delay 2s !${Me.Casting.ID}
		/call cast_AA_celest_regen
		/return
		
	| set rampage tank toon
	} else /if (${_type.Equal[rampage]}) {
		/if (${Bool[${_verbage}]}) {
			/call sql_update FALSE ${chrID} RampageTank ${_verbage.Left[1].Upper}${_verbage.Right[-1].Lower}
		} else /if (!${Bool[${_verbage}]}) {
			/call sql_update FALSE ${chrID} RampageTank FALSE
		}
		
	| spire control
	} else /if (${_type.Equal[spiregroup]} || ${_type.Equal[spireraid]}) {
		/call set_character_spire FALSE ${_type} ${_verbage}
		
	| use splashes
	} else /if (${_type.Equal[splash]}) {
		/call set_switch ${chrID} swSplash ${_verbage} 	 		
	
	| set XT refresh list Time
	} else /if (${_type.Equal[xttime]}) {
		/if (!${validate_expansion[TRUE, |16]}) /return
		/if (${Bool[${_verbage}]} && ${Range.Between[1,60:${Int[${_verbage}]}]}) {
			/call sql_update FALSE ${chrID} tXHeal ${_verbage}
		} else {
			VOUT /${_bind} xttime [${cinfo}${tXHeal}m\ax] allowable range ${sep} ${cinfo}1 - 60 min\ax
			/return
		}	
		
	| to Yaulp or not		
	} else /if (${_type.Equal[yaulp]}) {
	 	/call set_switch ${chrID} doYaulp ${_verbage}  

	| retort spell		
	} else /if (${_type.Equal[retort]}) {
	 	/call set_switch ${chrID} swRetort ${_verbage}  

	| Veturika's Perseverance	Control	
	} else /if (${_type.Equal[vp]}) {
		/if (${Bool[${_verbage}]} && ${Range.Between[1,99:${Int[${_verbage}]}]}) {
			/call sql_update FALSE ${chrID} useVPPct ${_verbage} 
		} else /if (${Bool[${_verbage}]} && !${Range.Between[1,99:${Int[${_verbage}]}]}) {
			VOUT /${_bind} vp [${cinfo}${useVPPct}\ax] allowable range ${sep} ${cinfo}1 - 99\ax
			/return
		}	else /if (!${Bool[${_verbage}]}) {
			/call set_switch ${chrID} switchVP ${_verbage}  
		}	
		
	| set group Heals
	} else /if (${_type.Equal[groupheal]}) {
		/call set_character_groupheal FALSE ${_verbage}

	| use group Heal over Time	
	} else /if (${_type.Equal[grouphot]}) {
		/call set_switch ${chrID} useGroupHoT ${_verbage} 	

	| arbitration (sheild AND AA)	
	} else /if (${_type.Equal[arbitrate]}) {
		/call set_switch ${chrID} swArbitrate ${_verbage} 	

	| set rampage tank toon
	} else /if (${_type.Equal[rampage]}) {
		/if (${Bool[${_verbage}]}) {
			/call sql_update FALSE ${chrID} RampageTank ${_verbage.Left[1].Upper}${_verbage.Right[-1].Lower}
		} else /if (!${Bool[${_verbage}]}) {
			/call sql_update ${chrID} RampageTank FALSE
		}
				
	| set Class Heal Point	
	} else /if (${_type.Equal[healpoint]}) {
		/call set_character_healpoint FALSE ${_verbage}
		/return

	| use divine rez
	} else /if (${_type.Equal[divinerez]}) {
		/call set_switch ${chrID} DivineRezCleric ${_verbage} 	

	| set the XTarget class to heal
	} else /if (${_type.Equal[xtclass]}) {
		/call set_character_xthealclass FALSE ${_verbage}
		/return	

	| set AA fade use %	
	} else /if (${_type.Equal[fade]}) {
		/call set_character_fade FALSE ${_verbage}
		
	| Complete Heal
	} else /if (${_type.Equal[ch]}) {
		/if (${Bool[${_verbage}]} && ${Range.Between[1,99:${Int[${_verbage}]}]}) {
			/call sql_update FALSE ${chrID} CompleteHealpct ${_verbage} 
		} else /if (${Bool[${_verbage}]} && !${Range.Between[1,99:${Int[${_verbage}]}]}) {
			VOUT /${_bind} ch(${cnum}${CompleteHealpct}\ax) allowable range ${sep} ${cinfo}1 - 99\ax
			/return
		}	else /if (!${Bool[${_verbage}]}) {
			/call set_switch ${chrID} switchCompleteHeal ${_verbage}  
		}

	}

	/if (${_silent}) /return
	/if (${swMQClear}) /mqclear
	/for _count 1 to ${_lstClass.Count[|]}
		/varset _listout ${_listout} ${If[${_sep},${sep},]} ${cinfo}${_lstClass.Arg[${_count},|]}\ax
		/if (${_lstClass.Arg[${_count},|].Equal[aggressive]}) /varset _listout ${_listout}:${If[${switchHealAggressive},${oon},${ooff}]}
		/if (${_lstClass.Arg[${_count},|].Equal[arbitrate]}) /varset _listout ${_listout}:${If[${swArbitrate},${oon},${ooff}]}
		/if (${_lstClass.Arg[${_count},|].Equal[ch]}) /varset _listout ${_listout}:${If[${switchCompleteHeal},${cnum}${CompleteHealpct}\ax,${ooff}]}
		/if (${_lstClass.Arg[${_count},|].Equal[divinerez]}) /varset _listout ${_listout}:${If[${DivineRezCleric},${oon},${ooff}]}
		/if (${_lstClass.Arg[${_count},|].Equal[fade]}) /varset _listout ${_listout}:${If[${switchAAFade},${cnum}${useAAFadePct}\ax,${ooff}]}
		/if (${_lstClass.Arg[${_count},|].Equal[groupheal]}) /varset _listout ${_listout}:${If[${useGroupHeal},${cnum}${useGroupHeal}\ax,${ooff}]}
		/if (${_lstClass.Arg[${_count},|].Equal[grouphot]}) /varset _listout ${_listout}:${If[${useGroupHoT},${oon},${ooff}]}
		/if (${_lstClass.Arg[${_count},|].Equal[healpoint]}) /varset _listout ${_listout}:${cnum}->\ax
		/if (${_lstClass.Arg[${_count},|].Equal[issuance]}) /varset _listout ${_listout}:${If[${switchIssuance},${cnum}${issuanceMobCount}\ax,${ooff}]}
		/if (${_lstClass.Arg[${_count},|].Equal[rampage]}) /varset _listout ${_listout}:${If[${Bool[${RampageTank}]},${cnum}${RampageTank}\ax,${ooff}]}
		/if (${_lstClass.Arg[${_count},|].Equal[retort]}) /varset _listout ${_listout}:${If[${swRetort},${oon},${ooff}]}
		/if (${_lstClass.Arg[${_count},|].Equal[rod]}) /varset _listout ${_listout}:${If[${swModRod},${oon},${ooff}]}
		/if (${_lstClass.Arg[${_count},|].Equal[spiregroup]}) /varset _listout ${_listout}:${If[${spireGroup},${cnum}${spireGroup}\ax,${ooff}]}
		/if (${_lstClass.Arg[${_count},|].Equal[spireraid]}) /varset _listout ${_listout}:${If[${spireRaid},${cnum}${spireRaid}\ax,${ooff}]}
		/if (${_lstClass.Arg[${_count},|].Equal[splash]}) /varset _listout ${_listout}:${If[${swSplash},${cnum}${oon}\ax,${ooff}]}
		/if (${_lstClass.Arg[${_count},|].Equal[vp]}) /varset _listout ${_listout}:${If[${switchVP},${cnum}${useVPPct}\ax,${ooff}]}
		/if (${_lstClass.Arg[${_count},|].Equal[xtclass]}) /varset _listout ${_listout}:${cnum}->\ax
		/if (${_lstClass.Arg[${_count},|].Equal[xttime]}) /varset _listout ${_listout}:${cnum}${tXHeal}\ax
		/if (${_lstClass.Arg[${_count},|].Equal[yaulp]}) /varset _listout ${_listout}:${If[${doYaulp},${oon},${ooff}]}
	/varset _sep TRUE
	/next _count
	VOUT /${_bind} [${_listout}]
/return




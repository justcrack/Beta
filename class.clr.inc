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
		/if (${switchAAFade} && ${Me.PctHPs} <= ${useAAFadePct}) /call cast_AA_fade
		/if (${Me.SpellReady[${healWard}]} && !${timer_heal_ward}) /call check_heal_ward
		/if (${swRetort}) /call check_buffs_tmp_list "${spellRetort}" "${spellRetortToon}"

		/if (${swModRod} && ${validate_class[FALSE, ${lstClassCast}]}) /call use_mod_rod
		/if (${exit_class_loop[]}) /return
		
		/if (${isValidTarget}) {
			CHECKREZ
			/call set_combat ${combat.Upper}
			/if (${swBurnControl}) /call synergy_${Me.Class.ShortName}
			/if (${swCombatMelee}) /call use_skill_melee ${Target2Kill}
			/call disc_${Me.Class.ShortName}
			/call check_deBuffs ${Target2Kill}
			/if (${swCombatDoT}) /call cast_DOT
			/if (${swCombatNuke}) /call cast_nuke
			/if (${setAlliance}) /call cast_alliance_${Me.Class.ShortName}
			/call click_BP
			/call click_item
			/if (${swSwarm} && !${Me.Pet.ID}) /call cast_temp_pet ${Target2Kill} "${tempPet}"
			/if (${swSwarm}) /call click_item_swarm ${Target2Kill}
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
/return



|***
 *	DES: Combat skills, Disciplines and Alternate abilities.
 *	USE: Disc
 *  NOTE: 
 ***|
sub disc_CLR()
	DEBUG disc_CLR()
	AMIDEAD
	CHECKEXIT
	CHECKINPUTS
	/if (${is_target_dead[${Target2Kill}]}) /return
	CHECKTIE

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
	AMIDEAD
	CHECKEXIT
	CHECKTIE

	UPDATEHUD "HEAL Group"

	/declare _count int local 0
	/varset hurtIndex ${Group.Member[${Me}].Index}
	/varset hurtPctHP ${If[${setHealOverride},${Float[${setHealOverride}].Centi},99.99]}
	/varset hurtCLS ${Me.Class.ShortName}
	/varset hurtCount 0

	| Do group members need healing? 
  /call check_health_group
	
	CHECKEXIT
	CHECKREZ
	/if (${switchAAFade} && ${Me.PctHPs} <= ${useAAFadePct}) /call cast_AA_fade
	
	| Group Regen Heal 
 	/if (${useGroupHoT} && !${hurtCount} && !${SafeZone}) {
	 	/if (${SpawnCount[npc radius ${setMobAgro} playerstate 4 targetable]} >= 1) {
	 	 	/if (${Me.AltAbilityReady[Celestial Regeneration]} && !${Raid.Members} && !${Bool[${Me.Song[Celestial Regeneration]}]} && !${Bool[${Me.Song[Focused Celestial Regeneration]}]} && !${Bool[${Me.Song[${healWholeGroupHoT}]}]} && !${Bool[${Me.Song[Shear of Renewal]}]} && !${Bool[${Me.Song[Ghost of Renewal]}]}) {
		 		/if (${validate_cast[FALSE, alt, "Celestial Regeneration", ${Me.ID}]}) /call core_cast2 "Celestial Regeneration" alt 0 FALSE
		  } else /if (${Me.SpellReady[${healWholeGroupHoT}]} && !${Bool[${Me.Song[Celestial Regeneration]}]} && !${Bool[${Me.Song[Focused Celestial Regeneration]}]} && !${Bool[${Me.Song[${healWholeGroupHoT}]}]} && !${Bool[${Me.Song[Shear of Renewal]}]} && !${Bool[${Me.Song[Ghost of Renewal]}]}) {
		    /if (${validate_cast[FALSE, spell, "${healWholeGroupHoT}", ${Me.ID}]}) /call core_cast2 "${healWholeGroupHoT}" 0 0 FALSE
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
			/if (${validate_cast[FALSE, spell, "Complete Heal", ${Group.MainTank.ID}]}) /call core_cast2 "Complete Heal" 0 ${Group.MainTank.ID} check_health_group
		}
	}

	| Focused Celestial Regeneration - out of raid only
	/if (!${Raid.Members}) {
		/if (${Group.Member[${hurtIndex}].PctHPs} <= ${${hurtCLS}Point}*.${tmpHealOverride} && ${Group.Member[${hurtIndex}].Distance} <= ${Spell[Focused Celestial Regeneration].MyRange} && !${Bool[${Me.Song[Celestial Regeneration]}]} && !${SafeZone}) { 
			/if (${Me.AltAbilityReady[Focused Celestial Regeneration]}) {
				/if (${validate_cast[FALSE, alt, "Focused Celestial Regeneration", ${Group.Member[${hurtIndex}].ID}]}) /call core_cast2 "Focused Celestial Regeneration" alt ${Group.Member[${hurtIndex}].ID} FALSE
			}
		}
	}
	
  | AA Spire
	/if (!${Raid.Members} && ${Group.Member[${hurtIndex}].PctHPs} <= ${${hurtCLS}Point}*.${tmpHealOverride} && ${Group.Member[${hurtIndex}].Distance} <= ${Spell[${CLRspire3}].MyRange} && !${Bool[${Me.Song[Celestial Regeneration]}]} && !${SafeZone}) { 
    /if (${validate_cast[FALSE, alt, "${CLRspire3}", ${Group.Member[${hurtIndex}].ID}]}) /call core_cast2 "${CLRspire3}" alt ${Group.Member[${hurtIndex}].ID} FALSE
  }	
  
	| Dichotomic Heal
	/if (${Me.Song[Gift of Mana].ID}) {
		/if (${Me.SpellReady[${GoMNuke}]} && ${Group.Member[${hurtIndex}].PctHPs} < ${GoMpoint}*.${tmpHealOverride}) { 
			/call cast_nuke_GoM ${Group.Member[${hurtIndex}].ID}
	  }
	}
  
	/if (${switchAAFade} && ${Me.PctHPs} <= ${useAAFadePct}) /call cast_AA_fade

	| Panic AA
	/if (${Me.AltAbilityReady[Burst of Life]}) {
	  /if (${Group.Member[${hurtIndex}].PctHPs} <= 45*.${tmpHealOverride}) {
			/if (${validate_cast[FALSE, alt, "Burst of Life", ${Group.Member[${hurtIndex}].ID}]}) /call core_cast2 "Burst of Life" alt ${Group.Member[${hurtIndex}].ID} FALSE
	  }
	}
  
	| Panic heal
	/if (${Me.SpellReady[${healPanic}]}) {
		/if (${Group.Member[${hurtIndex}].PctHPs} < 20*.${setHealOverride}) {
			/if (${Me.Casting.ID}) /stopcast
			/if (${validate_cast[FALSE, spell, "${healPanic}", ${Group.Member[${hurtIndex}].ID}]}) /call core_cast2 "${healPanic}" 0 ${Group.Member[${hurtIndex}].ID} FALSE
			/if (${validate_cast[FALSE, alt, "Divine Guardian", ${Group.Member[${hurtIndex}].ID}]}) /call core_cast2 "Divine Guardian" alt ${Group.Member[${hurtIndex}].ID} FALSE
		}
	}

	| Arbitration
  /if (${Group.Member[${hurtIndex}].PctHPs} < ${${hurtCLS}Point}*.${tmpHealOverride}*.5 || ${hurtCount} > ${useGroupHeal}) {
    /call cast_divine_arbitration
  }
  
  | Forced Group Heals ONLY
	/if (${switchForceGroupHeal}) {
		/if (${Group.Member[${hurtIndex}].PctHPs} <= ${${hurtCLS}Point}*.${tmpHealOverride}) {
			/if (${Me.SpellReady[${healWholeGroup}]}) {
				/if (${validate_cast[FALSE, spell, "${healWholeGroup}", ${Me.ID}]}) /call core_cast2 "${healWholeGroup}" 0 0 FALSE
			} else /if (${Me.SpellReady[${healWholeGroup2}]}) {
				/if (${validate_cast[FALSE, spell, "${healWholeGroup2}", ${Me.ID}]}) /call core_cast2 "${healWholeGroup2}" 0 0 check_health_group
			} else /if (${Me.SpellReady[${healWholeGroup3}]}) {
				/if (${validate_cast[FALSE, spell, "${healWholeGroup3}", ${Me.ID}]}) /call core_cast2 "${healWholeGroup3}" 0 0 check_health_group
			}
		}
		| if forced group healing. restart heal cycle at this point.
		/if (${switchAAFade} && ${Me.PctHPs} <= ${useAAFadePct}) /call cast_AA_fade
		/return
	}
    
  | Group Heal
  /if (${useGroupHeal}) {
	  /if (${hurtCount} >= ${useGroupHeal} && ${Me.SpellReady[${healWholeGroup}]}) {
	    /if (${validate_cast[FALSE, spell, "${healWholeGroup}", ${Me.ID}]}) /call core_cast2 "${healWholeGroup}" 0 0 FALSE
	  } 
	}
	
	| issuance AE heal
	/if (${switchIssuance} && !${SafeZone}) {
		/if (${SpawnCount[npc radius ${setMobAgro} playerstate 4]} >= ${issuanceMobCount}) /call cast_issuance_heal
	}
  
  | ward of surety short buff 
	/if (!${Bool[${timer_heal_ward}]} && ${Me.SpellReady[${healWard}]}) /call check_heal_ward

	| check for personal debuffs
	/if (${swHealCure}) {
		/if (${Debuff.Count}) {
			| AA Purified Spirits SELF ONLY 0' range 2 min
			/if (${validate_cast[FALSE, alt, "Purified Spirits", ${Me.ID}]}) /call core_cast2 "Purified Spirits" alt ${Me.ID} FALSE
			| AA Radient Cure SELF ONLY 100' range 1 min
			/if (${validate_cast[FALSE, alt, "Radiant Cure", ${Me.ID}]}) /call core_cast2 "Radiant Cure" alt 0 FALSE 
		}
	}
	
	| heal breastplate
	/if (${Bool[${BreastplateHeal}]}) {
		/if (${hurtCount} > 2 && ${Me.ItemReady[${BreastplateHeal}]} && ${cast_condition[BreastplateHealCondition]}) {
			/if (${validate_cast[FALSE, item, "${BreastplateHeal}", ${Group.Member[${hurtIndex}].ID}]}) /call core_cast2 "${BreastplateHeal}" item ${Group.Member[${hurtIndex}].ID} FALSE
		}
	}
	
	| AA heal stuffs
  /if (${hurtCount} && !${SafeZone} && !${timer_zoned_delay}) {
	  /if (${hurtCount} > 2) {
		  /if (${validate_cast[FALSE, alt, "Beacon of Life", ${Me.ID}]}) /call core_cast2 "Beacon of Life" alt 0 FALSE
 			/if (${validate_cast[FALSE, alt, "Fury of Life", ${Me.ID}]}) /call core_cast2 "Fury of Life" alt 0 FALSE
			/if (${validate_cast[FALSE, alt, "Silent Casting", ${Me.ID}]}) /call core_cast2 "Silent Casting" alt 0 FALSE
			/if (${validate_cast[FALSE, alt, "Shield of Reverence", ${Me.ID}]}) /call core_cast2 "Shield of Reverence" alt 0 FALSE
		}
		
		/if (${hurtCount} == 3) {
		 	/if (${validate_cast[FALSE, alt, "Channeling the Divine", ${Me.ID}]}) /call core_cast2 "Channeling the Divine" alt 0 FALSE
		} else /if (${hurtCount} == 4) {
 			/if (${validate_cast[FALSE, alt, "Healing Frenzy", ${Me.ID}]}) /call core_cast2 "Healing Frenzy" alt 0 FALSE
 		} else /if (${hurtCount} == 5) {
			/if (${validate_cast[FALSE, alt, "Exquisite Benediction", ${Me.ID}]}) /call core_cast2 "Exquisite Benediction" alt 0 FALSE
		} else /if (${hurtCount} == 6) {
			/if (${validate_cast[FALSE, alt, "Divine Retribution", ${Me.ID}]}) /call core_cast2 "Divine Retribution" alt 0 FALSE
			/call use_glyph
		}
  } 
  
  
	AMIDEAD
	/if (${switchAAFade} && ${Me.PctHPs} <= ${useAAFadePct}) /call cast_AA_fade
 	CHECKINPUTS
 	CHECKREZ
 	
 	| agressive healing sort
 	/if (${switchHealAggressive}) {
 		/if (${Me.SpellReady[${healIntervention1}]} || ${Me.SpellReady[${healIntervention2}]} || ${Me.SpellReady[${healIntervention3}]} || ${Me.SpellReady[${healIntervention4}]}) /goto :CHGinterventionheals
 	} 
 	
 	
 	| Remedy Heals 
  /if (${Group.Member[${hurtIndex}].PctHPs} <= ${${hurtCLS}Point}*.${tmpHealOverride}) {
		/for _count 1 to 3
			/if (${Me.SpellReady[${healRemedy${_count}}]}) {
				/if (${validate_cast[FALSE, spell, "${healRemedy${_count}}", ${Group.Member[${hurtIndex}].ID}]}) /call core_cast2 "${healRemedy${_count}}" 0 ${Group.Member[${hurtIndex}].ID} check_health_group
				/break
			}
		/next _count 
	}	
	
	| Intervention Heals
	:CHGinterventionheals
  /if (${Group.Member[${hurtIndex}].PctHPs} <= ${${hurtCLS}Point}*.${tmpHealOverride}) {
		/for _count 1 to 4
			/if (${Me.SpellReady[${healIntervention${_count}}]}) {
				/if (${validate_cast[FALSE, spell, "${healIntervention${_count}}", ${Group.Member[${hurtIndex}].ID}]}) /call core_cast2 "${healIntervention${_count}}" 0 ${Group.Member[${hurtIndex}].ID} check_health_group
				/break
			}
		/next _count
	}
	
	| Renewal Heals
  /if (${Group.Member[${hurtIndex}].PctHPs} <= ${${hurtCLS}Point}*.${tmpHealOverride}) {
		/for _count 1 to 3
			/if (${Me.SpellReady[${healRenewal${_count}}]}) {
				/if (${validate_cast[FALSE, spell, "${healRenewal${_count}}", ${Group.Member[${hurtIndex}].ID}]}) /call core_cast2 "${healRenewal${_count}}" 0 ${Group.Member[${hurtIndex}].ID} check_health_group
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
				/if (${validate_cast[FALSE, spell, "${healHoT}", ${Group.Member[${hurtIndex}].ID}]}) /call core_cast2 "${healHoT}" 0 ${Group.Member[${hurtIndex}].ID} FALSE
			}
		}
	}

  | check for group debuffs
	/if (${swHealCure}) {
		/if (${Bool[${Group.Members}]} && ${hurtCount}) /call cast_cure
	}
	
	UPDATEHUD FALSE
/return



|***
 *	DES: self heal only
 *	USE: /call check_heal_self_${Me.Class.ShortName}
 *  NOTE: 
 ***|
sub check_heal_self_CLR()
	DEBUG check_heal_self_CLR()
	AMIDEAD
	CHECKEXIT
	CHECKTIE
	
	UPDATEHUD "HEAL Self"
	/if (${Me.PctHPs} > ${CLRPoint}*.${tmpHealOverride}) /return
	/declare _count int local 0

	| check for personal debuffs
	/if (${swHealCure}) {
		/if (${Debuff.Count}) {
			| AA Purified Spirits SELF ONLY 0' range 2 min
			/if (${validate_cast[FALSE, alt, "Purified Spirits", ${Me.ID}]}) /call core_cast2 "Purified Spirits" alt ${Me.ID} FALSE
			| AA Radient Cure SELF ONLY 100' range 1 min
			/if (${validate_cast[FALSE, alt, "Radiant Cure", ${Me.ID}]}) /call core_cast2 "Radiant Cure" alt 0 FALSE 
		}
	}

	
	| heal breastplate
	/if (${Bool[${BreastplateHeal}]} && ${Me.ItemReady[${BreastplateHeal}]}) {
		/if (${Me.PctHPs} <= ${CLRPoint}*.${tmpHealOverride}*.5 && ${cast_condition[BreastplateHealCondition]}) {
			/if (${validate_cast[FALSE, item, "${BreastplateHeal}", ${Me.ID}]}) /call core_cast2 "${BreastplateHeal}" item 0 FALSE
		}
	}
	
	
 	| agressive healing sort
 	/if (${switchHealAggressive}) {
 		/if (${Me.SpellReady[${healContravention1}]} || ${Me.SpellReady[${healContravention2}]} || ${Me.SpellReady[${healContravention3}]} || ${Me.SpellReady[${healContravention4}]}) /goto :CHGinterventionheals
 	} 
 	
 	| Remedy Heals 
  /if (${Me.PctHPs} <= ${CLRPoint}*.${tmpHealOverride}) {
		/for _count 1 to 3
			/if (${Me.SpellReady[${healRemedy${_count}}]}) {
				/if (${validate_cast[FALSE, spell, "${healRemedy${_count}}", ${Me.ID}]}) /call core_cast2 "${healRemedy${_count}}" 0 ${Me.ID} FALSE
				/break
			}
		/next _count 
	}	
	
	| Contravention Heals
	:CHScontraventionheals
  /if (${Me.PctHPs} <= ${CLRPoint}*.${tmpHealOverride}) {
		/for _count 1 to 4
			/if (${Me.SpellReady[${healContravention${_count}}]}) {
				/if (${validate_cast[FALSE, spell, "${healContravention${_count}}", ${Target2Kill}]}) /call core_cast2 "${healContravention${_count}}" 0 ${Target2Kill} FALSE
				/break
			}
		/next _count
	}	
	
	| Renewal Heals
  /if (${Me.PctHPs} <= ${CLRPoint}*.${tmpHealOverride}) {
		/for _count 1 to 3
			/if (${Me.SpellReady[${healRenewal${_count}}]}) {
				/if (${validate_cast[FALSE, spell, "${healRenewal${_count}}", ${Me.ID}]}) /call core_cast2 "${healRenewal${_count}}" 0 ${Me.ID} FALSE
				/break
			}
		/next _count 
	}				

	| AA heal stuffs
  /if (${hurtCount} && !${SafeZone} && !${timer_zoned_delay}) {
	  /if (${Me.PctHPs} <= ${CLRPoint}*.${tmpHealOverride}*.7) {
		  /if (${validate_cast[FALSE, alt, "Beacon of Life", ${Me.ID}]}) /call core_cast2 "Beacon of Life" alt 0 FALSE
 			/if (${validate_cast[FALSE, alt, "Fury of Life", ${Me.ID}]}) /call core_cast2 "Fury of Life" alt 0 FALSE
			/if (${validate_cast[FALSE, alt, "Silent Casting", ${Me.ID}]}) /call core_cast2 "Silent Casting" alt 0 FALSE
			/if (${validate_cast[FALSE, alt, "Shield of Reverence", ${Me.ID}]}) /call core_cast2 "Shield of Reverence" alt 0 FALSE
		}
		
		/if (${Me.PctHPs} <= ${CLRPoint}*.${tmpHealOverride}*.6) {
		 	/if (${validate_cast[FALSE, alt, "Channeling the Divine", ${Me.ID}]}) /call core_cast2 "Channeling the Divine" alt 0 FALSE
		} else /if (${Me.PctHPs} <= ${CLRPoint}*.${tmpHealOverride}*.5) {
 			/if (${validate_cast[FALSE, alt, "Healing Frenzy", ${Me.ID}]}) /call core_cast2 "Healing Frenzy" alt 0 FALSE
 		} else /if (${Me.PctHPs} <= ${CLRPoint}*.${tmpHealOverride}*.4) {
			/if (${validate_cast[FALSE, alt, "Exquisite Benediction", ${Me.ID}]}) /call core_cast2 "Exquisite Benediction" alt 0 FALSE
		} else /if (${Me.PctHPs} <= ${CLRPoint}*.${tmpHealOverride}*.3) {
			/if (${validate_cast[FALSE, alt, "Divine Retribution", ${Me.ID}]}) /call core_cast2 "Divine Retribution" alt 0 FALSE
			/call use_glyph
		}
  } 
  
 	| issuance AE heal
	/if (${switchIssuance} && !${SafeZone}) {
		/if (${SpawnCount[npc radius ${setMobAgro} playerstate 4]} >= ${issuanceMobCount}) /call cast_issuance_heal
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

	| AA heal
	/if (${Me.XTarget[${hurtIndex}].PctHPs} < 45*.${tmpHealOverride} && ${Me.AltAbilityReady[Burst of Life]}) {
		/if (${validate_cast[FALSE, alt, "Burst of Life", ${Me.XTarget[${hurtIndex}].ID}]}) /call core_cast2 "Burst of Life" alt ${Me.XTarget[${hurtIndex}].ID} FALSE
	}

	| panic heal
	/if (${Me.SpellReady[${healPanic}]}) {
		/if (${Me.XTarget[${hurtIndex}].PctHPs} < 20*.${setHealOverride}) {
			/if (${Me.Casting.ID}) /stopcast
			/if (${validate_cast[FALSE, spell, "${healPanic}", ${Me.XTarget[${hurtIndex}].ID}]}) /call core_cast2 "${healPanic}" 0 ${Me.XTarget[${hurtIndex}].ID} FALSE
		}
	}
	
	/if (${switchHealAggressive}) {
		/if (${Me.SpellReady[${healIntervention1}]} || ${Me.SpellReady[${healIntervention2}]} || ${Me.SpellReady[${healIntervention3}]} || ${Me.SpellReady[${healIntervention4}]}) /goto :CHXTinterventionheals
	} 
	
	| Remedy Heals 
  /if (${Me.XTarget[${hurtIndex}].PctHPs} <= ${${hurtCLS}Point}*.${tmpHealOverride}) {
		/for _count 1 to 3
			/if (${Me.SpellReady[${healRemedy${_count}}]}) {
				/if (${validate_cast[FALSE, spell, "${healRemedy${_count}}", ${Me.XTarget[${hurtIndex}].ID}]}) /call core_cast2 "${healRemedy${_count}}" 0 ${Me.XTarget[${hurtIndex}].ID} check_health_group
				/break
			}
		/next _count 
	}	
	
	| Intervention Heals
	:CHXTinterventionheals
  /if (${Me.XTarget[${hurtIndex}].PctHPs} <= ${${hurtCLS}Point}*.${tmpHealOverride}) {
		/for _count 1 to 4
			/if (${Me.SpellReady[${healIntervention${_count}}]}) {
				/if (${validate_cast[FALSE, spell, "${healIntervention${_count}}", ${Me.XTarget[${hurtIndex}].ID}]}) /call core_cast2 "${healIntervention${_count}}" 0 ${Me.XTarget[${hurtIndex}].ID} check_health_group
				/break
			}
		/next _count
	}	
	
	| Renewal Heals
  /if (${Me.XTarget[${hurtIndex}].PctHPs} <= ${${hurtCLS}Point}*.${tmpHealOverride}) {
		/for _count 1 to 3
			/if (${Me.SpellReady[${healRenewal${_count}}]}) {
				/if (${validate_cast[FALSE, spell, "${healRenewal${_count}}", ${Me.XTarget[${hurtIndex}].ID}]}) /call core_cast2 "${healRenewal${_count}}" 0 ${Me.XTarget[${hurtIndex}].ID} check_health_group
				/break
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
	AMIDEAD
	SIMPLEEXIT
	CHECKINPUTS
	CHECKREZ
	UPDATEHUD "HEAL Pet"

	/declare _count int local 0
	/varset hurtIndex ${Group.Member[${Me}].Index}
	/varset hurtPctHP 100
  
	/call check_health_pet

	| any pets hurt?
  /if (${Group.Member[${hurtIndex}].ID} == ${Me.ID}) {
 		UPDATEHUD FALSE
 		/return
	}
	
 	/if (${switchHealAggressive}) {
 		/if (${Me.SpellReady[${healIntervention1}]} || ${Me.SpellReady[${healIntervention2}]} || ${Me.SpellReady[${healIntervention3}]} || ${Me.SpellReady[${healIntervention4}]}) /goto :PETinterventionheals
 	}
 	
 	| Remedy Heals 
  /if (${Group.Member[${hurtIndex}].Pet.PctHPs} <= ${PetPoint}) {
		/for _count 1 to 3
			/if (${Me.SpellReady[${healRemedy${_count}}]}) {
				/if (${validate_cast[FALSE, spell, "${healRemedy${_count}}", ${Group.Member[${hurtIndex}].Pet.ID}]}) /call core_cast2 "${healRemedy${_count}}" 0 ${Group.Member[${hurtIndex}].Pet.ID} FALSE
				/return
			}
		/next _count 
	}
	
	| Intervention Heals
	:PETinterventionheals
  /if (${Group.Member[${hurtIndex}].Pet.PctHPs} <= ${PetPoint}) {
		/for _count 1 to 4
			/if (${Me.SpellReady[${healIntervention${_count}}]}) {
				/if (${validate_cast[FALSE, spell, "${healIntervention${_count}}", ${Group.Member[${hurtIndex}].Pet.ID}]}) /call core_cast2 "${healIntervention${_count}}" 0 ${Group.Member[${hurtIndex}].Pet.ID} FALSE
				/return
			}
		/next _count
	}
	
	| Renewal Heals
  /if (${Group.Member[${hurtIndex}].Pet.PctHPs} <= ${PetPoint}) {
		/for _count 1 to 3
			/if (${Me.SpellReady[${healRenewal${_count}}]}) {
				/if (${validate_cast[FALSE, spell, "${healRenewal${_count}}", ${Group.Member[${hurtIndex}].Pet.ID}]}) /call core_cast2 "${healRenewal${_count}}" 0 ${Group.Member[${hurtIndex}].Pet.ID} FALSE
				/return
			}
		/next _count 
	}
		
	| Panic heal
	/if (${Me.SpellReady[${healPanic}]}) {
		/if (${Group.Member[${hurtIndex}].Pet.PctHPs} < ${PetPoint}) {
			/if (${Me.Casting.ID}) /stopcast
			/if (${validate_cast[FALSE, spell, "${healPanic}", ${Group.Member[${hurtIndex}].Pet.ID}]}) /call core_cast2 "${healPanic}" 0 ${Group.Member[${hurtIndex}].Pet.ID} FALSE
			/return
		}
	}

	UPDATEHUD FALSE
/return



|***
 *	DES: Use cleric divine arbitration AA or Epic shield click.
 *	USE: auto.
 *  NOTE: Setting [Heal] -> setDivinetoGroupHeal to TRUE in the clerics INI will force a group heal after the health arbitration.
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
  
  | heal speed boost
  /if (${validate_cast[FALSE, alt, "Celestial Rapidity", ${Me.ID}]}) /call core_cast2 "Celestial Rapidity" alt 0 FALSE
	
	/if (${setDivinetoGroupHeal} && ${useGroupHeal} && ${Me.SpellReady[${healWholeGroup}]}) {
		/if (${validate_cast[FALSE, spell, "${healWholeGroup}", ${Me.ID}]}) /call core_cast2 "${healWholeGroup}" 0  FALSE
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
		/if (${Bool[${Raid.Members}]}) /rs ${CRTXT}
	} else /if (${Bool[${Me.Song[Celestial Regeneration]}]}) {
		VOUT ${cbad}QUIT\ax trying to cast Celestial Regeneration while its running.	
	}		
	/if (${switchAAFade} && ${Me.PctHPs} <= ${useAAFadePct}) /call cast_AA_fade
/return



|***
 *	DES: Will the cleric floatie shield AE regen. Twin cast it if the AA is ready.
 *	USE: spell must be memed.
 *  NOTE: 
 ***|
sub cast_issuance_heal()
	DEBUG cast_issuance_heal()
	/if (!${Me.SpellReady[${healIssuance}]}) /return
	AMIDEAD
	SIMPLEEXIT
	CHECKINPUTS
	
	:castIssuance
	/if (!${Spawn[${Target2Kill}].CanSplashLand}) /return 

	/if (${Spawn[${Target2Kill}].Distance} > 100) {
		/if (${validate_cast[FALSE, spell, "${healIssuance}", ${Target2Kill}]}) /cast "${healIssuance}" loc ${Target.X} ${Target.Y} ${Target.Z}
		/delay 4s !${Me.Casting.ID}
	} else {
		/if (${validate_cast[FALSE, spell, "${healIssuance}", ${Target2Kill}]}) /cast "${healIssuance}" loc ${Me.X} ${Me.Y} ${Me.Z}
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
	/if (${coreAuto}) /call timer_update timer_heal_ward ${tHealWard}

	AMIDEAD
	SIMPLEEXIT
	CHECKINPUTS
	CHECKTIE
	
	/declare _wardToon string local
	/declare _count int local
	/declare _DNqout string local

	/for _count 1 to ${healWardToon.Count[|]}
		/varset _wardToon ${healWardToon.Arg[${_count},|]}

		| ward ready?
		/if (!${Me.SpellReady[${healWard}]}) /return

		| me?
		/if (${_wardToon.Equal[${Me.DisplayName}]} && !${Me.Song[${healWard}].ID}) {
			/if (${validate_cast[FALSE, spell, "${healWard}", ${Me.ID}]}) /call core_cast2 "${healWard}" 0 ${Me.ID} FALSE
			/return
		}

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
 *	DES: Cleric burn routines
 *	USE: establish all comp/trigger skills
 *	NOTE: all heal burns are located in the main heal routines
 ***| 
sub synergy_CLR()
	DEBUG synergy_CLR()
	SIMPLEEXIT
	/call use_glyph
	/call click_burn_circle
	/call click_item_burn
/return



|***
 *	DES: WORK IN PROGRESS
 *	USE:
 *	NOTE:
 ***| 
sub cast_alliance_CLR()
/return



|***
 *	DES: Class controls
 *	USE: /clr []
 *  NOTE:
 ***|
#bind setCLR /clr
sub Bind_setCLR(string _type, string _verbage)
	/if (!${validate_class[TRUE, |CLR]}) /return

	/declare _sep bool local
	/declare _listout string local
	/declare _count int local 0
	/declare _bind string local ${Me.Class.ShortName.Lower}
  /declare _lstClass string local |aggressive|ch|fade|forcegroupheal|groupheal|grouphot|healpoint|issuance|rampage|retort|rod|spiregroup|spireraid|vp|xtclass|xttime|yaulp

	| enable/disable lead with aggerssive heals
	/if (${_type.Equal[aggressive]}) {
		/call set_switch ${Me.Class.ShortName} switchHealAggressive ${_verbage}

	| force group heals
	} else /if (${_type.Equal[forcegroupheal]}) {
	 	/call set_switch ${Me.Class.ShortName} switchForceGroupHeal ${_verbage}  

	| use Modulation Rods
	} else /if (${_type.Equal[rod]}) {
		/call set_switch ${Me.Class.ShortName} swModRod ${_verbage} 	

	| use Issuance Shield Heal
	} else /if (${_type.Equal[issuance]}) {
		/if (${Bool[${_verbage}]} && ${Range.Between[1,50:${Int[${_verbage}]}]}) {
			/call sql_update FALSE ${Me.Class.ShortName} issuanceMobCount ${_verbage} 
		} else /if (${Bool[${_verbage}]} && !${Range.Between[1,50:${Int[${_verbage}]}]}) {
			VOUT /${_bind} issuance [${cinfo}${issuanceMobCount}\ax] allowable range ${sep} ${cinfo}1 - 50\ax
			/return
		}	else /if (!${Bool[${_verbage}]}) {
			/call set_switch ${Me.Class.ShortName} switchIssuance ${_verbage} 	 
		}		
		
	| mgb celestial regen
	} else /if (${_type.Equal[regen]}) {
		/delay 2s !${Me.Casting.ID}
		/call cast_AA_celest_regen
		/return
		
	| set rampage tank toon
	} else /if (${_type.Equal[rampage]}) {
		/if (${Bool[${_verbage}]}) {
			/call sql_update FALSE ${Me.Class.ShortName} RampageTank ${_verbage.Left[1].Upper}${_verbage.Right[-1].Lower}
		} else /if (!${Bool[${_verbage}]}) {
			/call sql_update FALSE ${Me.Class.ShortName} RampageTank FALSE
		}
		
	| spire control
	} else /if (${_type.Equal[spiregroup]} || ${_type.Equal[spireraid]}) {
		/call set_character_spire FALSE ${_type} ${_verbage}
	
	| set XT refresh list Time
	} else /if (${_type.Equal[xttime]}) {
		/if (!${validate_expansion[TRUE, |16]}) /return
		/if (${Bool[${_verbage}]} && ${Range.Between[1,10:${Int[${_verbage}]}]}) {
			/call sql_update FALSE timer tXHeal ${_verbage}
		} else {
			VOUT /${_bind} xttime [${cinfo}${tXHeal}m\ax] allowable range ${sep} ${cinfo}1 - 10 min\ax
			/return
		}	
		
	| to Yaulp or not		
	} else /if (${_type.Equal[yaulp]}) {
	 	/call set_switch ${Me.Class.ShortName} doYaulp ${_verbage}  

	| retort spell		
	} else /if (${_type.Equal[retort]}) {
	 	/call set_switch ${Me.Class.ShortName} swRetort ${_verbage}  

	| Veturika's Perseverance	Control	
	} else /if (${_type.Equal[vp]}) {
		/if (${Bool[${_verbage}]} && ${Range.Between[1,99:${Int[${_verbage}]}]}) {
			/call sql_update FALSE ${Me.Class.ShortName} useVPPct ${_verbage} 
		} else /if (${Bool[${_verbage}]} && !${Range.Between[1,99:${Int[${_verbage}]}]}) {
			VOUT /${_bind} vp [${cinfo}${useVPPct}\ax] allowable range ${sep} ${cinfo}1 - 99\ax
			/return
		}	else /if (!${Bool[${_verbage}]}) {
			/call set_switch ${Me.Class.ShortName} switchVP ${_verbage}  
		}	
		
	| set group Heals
	} else /if (${_type.Equal[groupheal]}) {
		/call set_character_groupheal FALSE ${_verbage}

	| use group Heal over Time	
	} else /if (${_type.Equal[grouphot]}) {
		/call set_switch ${Me.Class.ShortName} useGroupHoT ${_verbage} 	

	| set rampage tank toon
	} else /if (${_type.Equal[rampage]}) {
		/if (${Bool[${_verbage}]}) {
			/call sql_update FALSE ${Me.Class.ShortName} RampageTank ${_verbage.Left[1].Upper}${_verbage.Right[-1].Lower}
		} else /if (!${Bool[${_verbage}]}) {
			/call sql_update ${Me.Class.ShortName} RampageTank FALSE
		}
				
	| set Class Heal Point	
	} else /if (${_type.Equal[healpoint]}) {
		/call set_character_healpoint FALSE ${_verbage}
		/return

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
			/call sql_update FALSE ${Me.Class.ShortName} CompleteHealpct ${_verbage} 
		} else /if (${Bool[${_verbage}]} && !${Range.Between[1,99:${Int[${_verbage}]}]}) {
			VOUT /${_bind} ch(${cnum}${CompleteHealpct}\ax) allowable range ${sep} ${cinfo}1 - 99\ax
			/return
		}	else /if (!${Bool[${_verbage}]}) {
			/call set_switch ${Me.Class.ShortName} switchCompleteHeal ${_verbage}  
		}

	}
	/if (${swMQClear}) /mqclear
	/for _count 1 to ${_lstClass.Count[|]}
		/varset _listout ${_listout} ${If[${_sep},${sep},]} ${cinfo}${_lstClass.Arg[${_count},|]}\ax
		/if (${_lstClass.Arg[${_count},|].Equal[aggressive]}) /varset _listout ${_listout}:${If[${switchHealAggressive},${oon},${ooff}]}
		/if (${_lstClass.Arg[${_count},|].Equal[ch]}) /varset _listout ${_listout}:${If[${switchCompleteHeal},${cnum}${CompleteHealpct}\ax,${ooff}]}
		/if (${_lstClass.Arg[${_count},|].Equal[fade]}) /varset _listout ${_listout}:${If[${switchAAFade},${cnum}${useAAFadePct}\ax,${ooff}]}
		/if (${_lstClass.Arg[${_count},|].Equal[forcegroupheal]}) /varset _listout ${_listout}:${If[${switchForceGroupHeal},${oon},${ooff}]}
		/if (${_lstClass.Arg[${_count},|].Equal[groupheal]}) /varset _listout ${_listout}:${If[${useGroupHeal},${cnum}${useGroupHeal}\ax,${ooff}]}
		/if (${_lstClass.Arg[${_count},|].Equal[grouphot]}) /varset _listout ${_listout}:${If[${useGroupHoT},${oon},${ooff}]}
		/if (${_lstClass.Arg[${_count},|].Equal[healpoint]}) /varset _listout ${_listout}:${cnum}->\ax
		/if (${_lstClass.Arg[${_count},|].Equal[issuance]}) /varset _listout ${_listout}:${If[${switchIssuance},${cnum}${issuanceMobCount}\ax,${ooff}]}
		/if (${_lstClass.Arg[${_count},|].Equal[rampage]}) /varset _listout ${_listout}:${If[${Bool[${RampageTank}]},${cnum}${RampageTank}\ax,${ooff}]}
		/if (${_lstClass.Arg[${_count},|].Equal[retort]}) /varset _listout ${_listout}:${If[${swRetort},${oon},${ooff}]}
		/if (${_lstClass.Arg[${_count},|].Equal[rod]}) /varset _listout ${_listout}:${If[${swModRod},${oon},${ooff}]}
		/if (${_lstClass.Arg[${_count},|].Equal[spiregroup]}) /varset _listout ${_listout}:${If[${spireGroup},${cnum}${spireGroup}\ax,${ooff}]}
		/if (${_lstClass.Arg[${_count},|].Equal[spireraid]}) /varset _listout ${_listout}:${If[${spireRaid},${cnum}${spireRaid}\ax,${ooff}]}
		/if (${_lstClass.Arg[${_count},|].Equal[vp]}) /varset _listout ${_listout}:${If[${switchVP},${cnum}${useVPPct}\ax,${ooff}]}
		/if (${_lstClass.Arg[${_count},|].Equal[xtclass]}) /varset _listout ${_listout}:${cnum}->\ax
		/if (${_lstClass.Arg[${_count},|].Equal[xttime]}) /varset _listout ${_listout}:${cnum}${tXHeal}\ax
		/if (${_lstClass.Arg[${_count},|].Equal[yaulp]}) /varset _listout ${_listout}:${If[${doYaulp},${oon},${ooff}]}
	/varset _sep TRUE
	/next _count
	VOUT /${_bind} [${_listout}]
/return


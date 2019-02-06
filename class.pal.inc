|***
 *	class.pal.inc
 * 	Core Paladin Include
 *
 *	
 ***|
 



|***
 *	DES: Main Paladin Routine
 *	USE: primarly for making unicorn jokes at. also useful for throwing at raid mobs as speed bumps.
 *  NOTE: 
 ***|
sub core_PAL()
	DEBUG core_PAL()
	
	/if (${coreAuto}) {
	  /if (${swHealXTarget} && !${timer_set_xtarget}) {
	    /call set_xtarget
	    /call timer_update timer_set_xtarget ${tXHeal}
	  }

		/if (${useShieldFlash}) /call cast_AA_shield_flash

		| aa LoH
		/if (${swHealSelf} && ${useLoH}) {
		  /if (${Me.PctHPs} <= ${useLoHPct}*.${tmpHealOverride}) {
				/if (${validate_cast[FALSE, alt, "Lay on Hands", ${Me.ID}]}) /call core_cast2 "Lay on Hands" alt ${Me.ID} FALSE
			}
		}	

		/if (${useMG}) {
			/if (!${swNeedRez} && !${timer_check_MG}) /call cast_AA_marrs_gift
		}
	}
		
	/while (${prep_combat[]}) {
		CHECKINPUTS
		/if (${swHealSelf}) /call check_heal_self_${Me.Class.ShortName}
		/if (${swHealGroup} && ${Group.Members}) /call check_heal_group_${Me.Class.ShortName}
		/if (${swHealXTarget}) /call check_heal_XTarget_${Me.Class.ShortName}
		/if (${swHealPet} && ${Group.Members}) /call check_heal_pet_${Me.Class.ShortName}
		/if (${swHealCure}) /call cast_cure
		/if (${exit_class_loop[]}) /return
		
		/if (${isValidTarget}) {
			/call set_combat ${combat.Upper}
			/if (${swCombatMelee}) /call use_skill_melee ${Target2Kill}
			/if (${swAgro}) /call get_agro_${Me.Class.ShortName}
			/if (${swBurnControl}) /call burn_${Me.Class.ShortName}
			/if (${swSwarm} || ${swPet}) /call send_pet
			/call disc_${Me.Class.ShortName}
			/call check_deBuffs ${Target2Kill}
			/if (${swCombatDoT}) /call cast_DOT
			/if (${swCombatNuke}) /call cast_nuke
			/if (${Bool[${setAlliance}]}) /call cast_alliance_${Me.Class.ShortName} ${Target2Kill}
			/call click_BP
			/call click_item
			/if (${swSwarm}) /call click_item_swarm ${Target2Kill}
			/call click_DPS_${Me.Class.ShortName}
			/if (${switchHarmonious}) /call check_harmonious
			/if (${switchSteel} && !${timer_check_Steel}) /call check_steel	
			/if (${switchStance} && (!${timer_check_Stance} || !${Bool[${Me.Buff[${Stance}]}]})) /call check_stance
		}

		/if (${coreAuto}) {	
			/if (${switchAAFade} && ${Me.PctHPs} <= ${useAAFadePct}) /call cast_AA_fade
			/if (${swModRod} && ${validate_class[FALSE, ${lstClassCast}]}) /call use_mod_rod
			/if (${validate_class[FALSE, ${lstClassMelee}]}) /call cast_stamina_recovery
		}
	}

/return



|***
 *	DES: common use combat skills
 *	USE: 
 *  NOTE: 
 ***|
sub disc_PAL()
	DEBUG disc_PAL()
	AMIDEAD
	CHECKEXIT
	CHECKINPUTS
	/if (${is_target_dead[${Target2Kill}]}) /return
	CHECKTIE

	| spam rune
	/if (${validate_cast[FALSE, disc, "${DiscFastRune}", ${Me.ID}]}) /call core_cast2 "${DiscFastRune}" disc 0 FALSE

	| defensive	
	/if (${coreAuto} && ${swAgro} && ${SpawnCount[npc radius ${setCombatRadius} playerstate 4 targetable]} >= ${defCount} && !${Bool[${Me.ActiveDisc}]}) {
		/declare _count int local 0
		/for _count 1 to 3
			/if (${validate_cast[FALSE, activedisc, "${DefensiveDisc${_count}}", ${Me.ID}]}) /call core_cast2 "${DefensiveDisc${_count}}" disc 0 FALSE
		/next _count
	}
		
	/if (${useShieldFlash}) /call cast_AA_shield_flash
	
	| aa LoH
	/if (${swHealSelf}) {
		/if (${useLoH}) {
		  /if (${Me.PctHPs} <= ${useLoHPct}.${tmpHealOverride}) {
				/if (${validate_cast[FALSE, alt, "Lay on Hands", ${Me.ID}]}) /call core_cast2 "Lay on Hands" alt ${Me.ID} FALSE
			}
		}	
	}

	| AA inquisitors armor stacking control
	/if (${SpawnCount[npc ${If[${swOverrideLOS},,los]} radius 30 targetable]} >= ${defCount}) {
		/if (!${Me.Song[[${AAGroupInquisitor}].ID}) { 
			/if (${validate_cast[FALSE, alt, "${AAInquisitor}", ${Me.ID}]}) /call core_cast2 "${AAInquisitor}" alt 0 FALSE

		| dont use group armor in raids
		} else /if (!${Me.Song[${AAInquisitor}].ID} && !${Raid.Members}) {
			/if (${validate_cast[FALSE, alt, "${AAGroupInquisitor}", ${Me.ID}]})  /call core_cast2 "${AAGroupInquisitor}" alt 0 FALSE
		}
	}

	| undead cripple
	/if (${Target.Body.Name.Equal[Undead]}) {
		/if (${validate_cast[FALSE, alt, "Vanquish the Fallen", ${Target2Kill}]}) /call core_cast2 "Vanquish the Fallen" alt ${Target2Kill} FALSE
	}
	
	| Yaulp.. all the time!!
	/if (${doYaulp}) /call cast_yaulp
	
/return



|***
 *	DES: emergency healing
 *	USE: /call patch_heal_${Me.Class.ShortName}
 *  NOTE: 
 ***|
sub check_heal_self_PAL()
	DEBUG check_heal_self_PAL()
	AMIDEAD
	CHECKEXIT
  CHECKINPUTS

	/if (${Me.PctHPs} > ${PALPoint}*.${tmpHealOverride}) /return
	/if (${useShieldFlash}) /call cast_AA_shield_flash

	| Hand of Piety AA Group heal	
	/if (${Me.PctHPs} < ${PALPoint}*.50*.${tmpHealOverride} && ${SpawnCount[npc ${If[${swOverrideLOS},,los]} radius ${setCombatRadius} targetable]} >= ${defCount}) {
		/if (${validate_cast[FALSE, alt, "${AAHandofPiety}", ${Me.ID}]}) /call core_cast2 "${AAHandofPiety}" alt 0 FALSE
	}
	
	| AA Gift of Life -- not in raids
	/if (${Me.PctHPs} < ${PALPoint}*.30*.${tmpHealOverride} && !${Raid.Members}) {
		/if (${validate_cast[FALSE, alt, "Gift of Life", ${Me.ID}]}) /call core_cast2 "Gift of Life" alt 0 FALSE
	}	

	| Panic Heal 1
	/if (${Me.Gem[${healPanic1}]}) {
	  /if (${Me.PctHPs} <= ${PALPoint}*.45*.${tmpHealOverride}) {
			/if (${validate_cast[FALSE, spell, "${healPanic1}", ${Me.ID}]}) /call core_cast2 "${healPanic1}" 0 0 FALSE
		}
	}

	| dpsheal
	/if (${Me.PctHPs} < ${PALPoint}*.40*.${tmpHealOverride}) {
		/if (${validate_cast[FALSE, disc, "${DiscReflexive}", ${Target2Kill}]}) /call core_cast2 "${DiscReflexive}" disc ${Target2Kill} FALSE
	}

	| Panic Heal 2
	/if (${Me.Gem[${healPanic2}]}) {
	  /if (${Me.PctHPs} <= ${PALPoint}*.20*.${tmpHealOverride}) {
			/if (${validate_cast[FALSE, spell, "${healPanic2}", ${Me.ID}]}) /call core_cast2 "${healPanic2}" 0 0 FALSE
		}
	}
	
	| aa LoH
	/if (${useLoH}) {
	  /if (${Me.PctHPs} <= ${useLoHPct}.${tmpHealOverride}) {
			/if (${validate_cast[FALSE, alt, "Lay on Hands", ${Me.ID}]}) /call core_cast2 "Lay on Hands" alt ${Me.ID} FALSE
		}
	}

	| touch heal
	/if (${Me.Gem[${healTouch}]}) {
		/if (${Me.PctHPs} < ${PALPoint}*.20*.${setHealOverride}) {
			/if (${validate_cast[FALSE, spell, "${healTouch}", ${Me.ID}]}) /call core_cast2 "${healTouch}" 0 ${Me.ID}
		} 	
	}
	
	/call click_item_heal
/return



|***
 *	DES: I'm more important then you are!
 *	USE: /call get_agro_PAL
 *  NOTE: 
 ***|
sub get_agro_PAL()
	DEBUG get_agro_PAL()
	AMIDEAD
	CHECKEXIT	  
	CHECKINPUTS

	/if (${is_target_dead[${Target2Kill}]}) /return
	/if (${swPull}) /call set_combat ${combat.Upper}
	/if (${useShieldFlash}) /call cast_AA_shield_flash
	
	| aa LoH
	/if (${swHealSelf}) {
		/if (${useLoH}) {
		  /if (${Me.PctHPs} <= ${useLoHPct}.${tmpHealOverride}) {
				/if (${validate_cast[FALSE, alt, "Lay on Hands", ${Me.ID}]}) /call core_cast2 "Lay on Hands" alt ${Me.ID} FALSE
			}
		}	
	}	

	| challenge
	/call cast_challenge ${Target2Kill}

	| chain stun
	/if (${validate_cast[FALSE, alt, "${chainStun}", ${Target2Kill}]}) /call core_cast2 "${chainStun}" alt ${Target2Kill} FALSE

	| AE Stuns
	/if (${AEMelee}) {
		/varset mobAECount ${SpawnCount[npc ${If[${swOverrideLOS},,los]} radius 60 playerstate 4 targetable]}
		
		/if (${mobAECount} > 1) {
			/if (!${Bool[${Raid.Members}]} || ${Bool[${Raid.Members}]} && ${Raid.MainAssist.Name.Equal[${Me.DisplayName}]}) {
				/if (${validate_cast[FALSE, alt, "${AAAELodestar}", ${Target2Kill}]}) /call core_cast2 "${AAAELodestar}" alt ${Target2Kill} FALSE
			}
		} else /if (${mobAECount} > 4) {
			/if (!${Bool[${Raid.Members}]} || ${Bool[${Raid.Members}]} && ${Raid.MainAssist.Name.Equal[${Me.DisplayName}]}) {
				/if (${validate_cast[FALSE, alt, "${AAAEBeacon}", ${Me.ID}]}) /call core_cast2 "${AAAEBeacon}" alt 0 FALSE
			}
		}
	}

	| AA pet hate (the little sword thingie)
	/if (${Me.PctAggro} < 80 || ${Me.TargetOfTarget.Name.NotEqual[${Me.DisplayName}]}) {
		/if (${validate_cast[FALSE, alt, "${AAHatePet}", ${Target2Kill}]}) /call core_cast2 "${AAHatePet}" alt ${Target2Kill} FALSE
	}

	| snap hate
	/if (!${Raid.Members}) {
		/if (${Me.PctAggro} < 60 || ${Me.TargetOfTarget.Name.NotEqual[${Me.DisplayName}]}) {
			/if (${validate_cast[FALSE, alt, "Ageless Enmity", ${Target2Kill}]}) /call core_cast2 "Ageless Enmity" alt ${Target2Kill} FALSE
		}
	}

	| named fake hate
	/if ((${Me.PctAggro} < 60 || ${Me.TargetOfTarget.Name.NotEqual[${Me.DisplayName}]}) && ${NameList.Find[${Target.DisplayName}]}) {
		/if (${validate_cast[FALSE, disc, "${DiscUnflinching}", ${Target2Kill}]}) /call core_cast2 "${DiscUnflinching}" disc ${Target2Kill} FALSE
	}

	| Valiant Defense line. agro+heal
	/if (${Me.PctAggro} < 95 || ${Me.TargetOfTarget.Name.NotEqual[${Me.DisplayName}]}) {
		/if (${validate_cast[FALSE, spell, "${ValiantDefense}", ${Target2Kill}]}) /call core_cast2 "${ValiantDefense}" 0 ${Target2Kill} FALSE
	}
/return



|***
 *	DES: 
 *	USE: 
 *  NOTE: 
 ***|
sub click_DPS_PAL()
	DEBUG click_DPS_PAL()
	/if (${is_target_dead[${Target2Kill}]}) /return
	AMIDEAD
	CHECKEXIT
	CHECKINPUTS
	/call click_horn_of_unity 	
	/if (${useShieldFlash}) /call cast_AA_shield_flash
	
	| aa LoH
	/if (${swHealSelf}) {
		/if (${useLoH}) {
		  /if (${Me.PctHPs} <= ${useLoHPct}.${tmpHealOverride}) {
				/if (${validate_cast[FALSE, alt, "Lay on Hands", ${Me.ID}]}) /call core_cast2 "Lay on Hands" alt ${Me.ID} FALSE
			}
		}	
	}	
	
	/if (${validate_cast[FALSE, epicitem, "2", ${Me.ID}]} && ${Spawn[${Target2Kill}].Distance} <= ${setCombatDistance}) /call core_cast2 "${${Me.Class.ShortName}Epic2}" item 0 FALSE

/return



|***
 *	DES: Palading group heal. centered more to splashing
 *	USE: 
 *  NOTE: 
 ***|
sub check_heal_group_PAL()
	DEBUG check_heal_group_PAL()
	AMIDEAD
	CHECKEXIT
 	CHECKINPUTS	
	CHECKTIE
	
	/declare _count int local 0

	| Do group members need healing? 
  /call check_health_group
  
	UPDATEHUD "HEAL Group"
	/if (${useShieldFlash}) /call cast_AA_shield_flash

	| Panic Splash
	/if (${Group}) {
	  /if (${Group.Member[${hurtIndex}].PctHPs} <= 45*.${tmpHealOverride}) {
			/if (${validate_cast[FALSE, spell, "${splashPanic}", ${Me.ID}]}) /call core_cast2 "${splashPanic}" 0 0 FALSE
		}
	}
		
	| Burst Heal 
  /if (${Group.Member[${hurtIndex}].PctHPs} <= ${${hurtCLS}Point}*.${tmpHealOverride}) {
  	/if (${validate_cast[FALSE, spell, "${healBurst}", ${Group.Member[${hurtIndex}].ID}]}) /call core_cast2 "${healBurst}" 0 ${Group.Member[${hurtIndex}].ID} FALSE
  }		

	| Light Heal 
  /if (${Group.Member[${hurtIndex}].PctHPs} <= ${${hurtCLS}Point}*.${tmpHealOverride}) {
  	/if (${validate_cast[FALSE, spell, "${healLight}", ${Group.Member[${hurtIndex}].ID}]}) /call core_cast2 "${healLight}" 0 ${Group.Member[${hurtIndex}].ID} FALSE
  }		
				
		
	| group heals
	/if (${useGroupHeal}) {
		| Group Splash
	  /if (${hurtCount} >= 1) {
	    /if (${validate_cast[FALSE, spell, "${splashGroup}", ${Me.ID}]}) /call core_cast2 "${splashGroup}" 0 0 FALSE
	  } 
			
		| AA Splash
	  /if (${hurtCount} >= 3) {
	  	/if (${validate_cast[FALSE, alt, "${AAHandofPiety}", ${Me.ID}]}) /call core_cast2 "${AAHandofPiety}" alt 0 FALSE
	  }
	}

	AMIDEAD
 	CHECKINPUTS
	| Touch Heal 
  /if (${Group.Member[${hurtIndex}].PctHPs} <= ${${hurtCLS}Point}*.${tmpHealOverride}) {
  	/if (${validate_cast[FALSE, spell, "${healTouch}", ${Group.Member[${hurtIndex}].ID}]}) /call core_cast2 "${healTouch}" 0 ${Group.Member[${hurtIndex}].ID} FALSE
  }
	 
	UPDATEHUD
/return



|***
 *	DES: Check XTargets and heal the bastards if the need it.
 *	USE: /call check_heal_XTarget_${Me.Class.ShortName}
 *  NOTE: 
 ***|
sub check_heal_XTarget_PAL()
	DEBUG check_heal_XTarget_PAL()
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

	| Touch Heal
  /if (${Me.XTarget[${hurtIndex}].PctHPs} <= ${${hurtCLS}Point}*.${tmpHealOverride}) {
	  /if (${validate_cast[FALSE, spell, "${healTouch}", ${Me.XTarget[${hurtIndex}].}]}) /call core_cast2 "${healTouch}" 0 ${Me.XTarget[${hurtIndex}].} FALSE
	}
	
	| Burst Heal 
  /if (${Me.XTarget[${hurtIndex}].PctHPs} <= ${${hurtCLS}Point}*.${tmpHealOverride}) {
  	/if (${validate_cast[FALSE, spell, "${healBurst}", ${Me.XTarget[${hurtIndex}].ID}]}) /call core_cast2 "${healBurst}" 0 ${Me.XTarget[${hurtIndex}].ID} FALSE
  }		

	| Burst Heal 
  /if (${Me.XTarget[${hurtIndex}].PctHPs} <= ${${hurtCLS}Point}*.${tmpHealOverride}) {
  	/if (${validate_cast[FALSE, spell, "${healLight}", ${Me.XTarget[${hurtIndex}].ID}]}) /call core_cast2 "${healLight}" 0 ${Me.XTarget[${hurtIndex}].ID} FALSE
  }		

	UPDATEHUD FALSE
/return



|***
 *	DES: Checks and Heals Pets. Group ONLY
 *	USE: /healpet
 *  NOTE: 
 ***|
sub check_heal_pet_PAL()
	DEBUG check_heal_pet_PAL()
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
   
  /if (${useShieldFlash}) /call cast_AA_shield_flash
  
	| Burst Heal 
  /if (${Group.Member[${hurtIndex}].Pet.PctHPs} <= ${PetPoint}) {
  	/if (${validate_cast[FALSE, spell, "${healBurst}", ${Group.Member[${hurtIndex}].Pet.ID}]}) /call core_cast2 "${healBurst}" 0 ${Group.Member[${hurtIndex}].Pet.ID} FALSE
  }		

	| Light Heal 
  /if (${Group.Member[${hurtIndex}].Pet.PctHPs} <= ${PetPoint}) {
  	/if (${validate_cast[FALSE, spell, "${healLight}", ${Group.Member[${hurtIndex}].Pet.ID}]}) /call core_cast2 "${healLight}" 0 ${Group.Member[${hurtIndex}].Pet.ID} FALSE
  }		

	| Touch Heal 
  /if (${Group.Member[${hurtIndex}].Pet.PctHPs} <= ${PetPoint}) {
  	/if (${validate_cast[FALSE, spell, "${healTouch}", ${Group.Member[${hurtIndex}].Pet.ID}]}) /call core_cast2 "${healTouch}" 0 ${Group.Member[${hurtIndex}].Pet.ID} FALSE
  }

/return



|***
 *	DES: Mar's Gift 30ish k HP/15k manna 
 *	USE: auto
 *  NOTE: 
 ***|
sub cast_AA_marrs_gift()
	DEBUG cast_AA_marrs_gift()
	AMIDEAD
	SIMPLEEXIT
  CHECKINPUTS

	/call timer_update timer_check_MG ${tcheck_MG}
	/if (!${validate_cast[FALSE, alt, "Marr's Gift", ${Me.ID}]}) /return
	/if (${timer_zone_pause} || ${SafeZone} && !${swOverrideSafeZone}  || ${Me.Sitting}) /return
	
	/if (${Me.PctMana} < ${useMGPct}*.${tmpHealOverride}) {
		/call core_cast2 "Marr's Gift" alt 0 FALSE
	} else /if (${swHealSelf} && ${Me.PctHPs} < ${useMGPct}*.${tmpHealOverride}) {
		/call core_cast2 "Marr's Gift" alt 0 FALSE
	}
/return	



|***
 *	DES: keeps Paladin Reinvigorating Steel applied
 *	USE: /call check_steel
 *  NOTE: 
 ***|
sub check_steel()
	DEBUG check_steel()
	/if (${SafeZone} && !${swOverrideSafeZone}) /return
	AMIDEAD
	SIMPLEEXIT
  CHECKINPUTS

	/if (${validate_cast[FALSE, spell, "${Steel}", ${Me.ID}]}) {
		/call core_cast2 "${Steel}" 0 0 FALSE
		/call timer_update timer_check_Steel ${tcheck_Steel}
	}
/return



 |***
 *	DES: Paladin burn routines
 *	USE: establish all comp/trigger skills
 *	NOTE: 
 ***| 
sub burn_PAL()
	DEBUG burn_PAL()
	/if (${is_target_dead[${Target2Kill}]}) /return
	AMIDEAD
	CHECKEXIT
  CHECKINPUTS

	| defensive	
	/declare _count int local 0
	/for _count 1 to 3
		/if (${Bool[${Me.ActiveDisc}]}) /break
		/if (${validate_cast[FALSE, disc, "${DefensiveDisc${_count}}", ${Me.ID}]}) {
			/call core_cast2 "${DefensiveDisc${_count}}" disc 0 FALSE
			/break
		}
	/next _count

	| deflection
	/if (!${Bool[${Me.ActiveDisc}]} && ${Me.PctHPs} < 50*.${tmpHealOverride}) {
		/if (${validate_cast[FALSE, disc, "Deflection Discipline", ${Me.ID}]}) {
			/call core_cast2 "Deflection Discipline" disc 0 FALSE
			/return
		}
	}
	
	/if (${useShieldFlash}) /call cast_AA_shield_flash

	| aa LoH
	/if (${swHealSelf}) {
		/if (${useLoH}) {
		  /if (${Me.PctHPs} <= ${useLoHPct}.${tmpHealOverride}) {
				/if (${validate_cast[FALSE, alt, "Lay on Hands", ${Me.ID}]}) /call core_cast2 "Lay on Hands" alt ${Me.ID} FALSE
			}
		}	
	}

	/call cast_AA_spire	

	/if (${validate_cast[FALSE, alt, "${AAJudgement}", ${Me.ID}]}) /call core_cast2 "${AAJudgement}" alt 0 FALSE
	/if (${validate_cast[FALSE, alt, "${AAValorousRage}", ${Me.ID} ]]}) /call core_cast2 "${AAValorousRage}" alt 0 FALSE
	/if (${validate_cast[FALSE, alt, "${AAShield}", ${Me.ID}]}) /call core_cast2 "${AAShield}" alt 0 FALSE
	
	/call use_glyph
	/call click_burn_circle
	/call click_item_burn
/return



|***
 *	DES: Alliance (Holly Alliance|spell)
 *	USE: 
 *  NOTE: 6 casts to fulm.
 ***|
sub cast_alliance_PAL(int _tmpID)
	DEBUG cast_alliance_${Me.Class.ShortName()

  | check alliance group/raid
  /if (!${Bool[${setAlliance}]}) /return FALSE
	/if (${setAlliance.Equal[raid]} && !${Raid.Members}) /return FALSE  
	/if (${setAlliance.Equal[group]} && (!${Bool[${Group.Members]} || ${Raid.Members})) /return FALSE  
	| enough paladins to make it worth casting?
	/if (${SpawnCount[paladin radius ${setMobAgro} setzradius ${setzradius}]} < 3) /return FALSE
	| have the alliance on it already?
	/if (${Target.Buff[${${Me.Class.ShortName}Alliance}].ID}) /return FALSE
	| spell ready?
	| /if (!${check_ready[spell, "${${Me.Class.ShortName}Alliance}"]}) /return FALSE
	| no target
	/if (${is_target_dead[${_tmpID}]}) /return

	AMIDEAD
	CHECKEXIT
  CHECKINPUTS

	/if (${validate_cast[FALSE, spell, "${${Me.Class.ShortName}Alliance}", ${_tmpID}]}) /call core_cast2 "${${Me.Class.ShortName}Alliance}" alt ${_tmpID} FALSE

/return TRUE



|***
 *	DES: make the pally spto it shit and splash at its feet
 *	USE: /dex PALNAME /splash | /splash
 *  NOTE: 
 ***|
#bind pal_splash /splash
sub Bind_pal_splash()
	/if (!${validate_class[TRUE, |PAL]}) /return FALSE

	| splash
	VOUT Splashing these dumb asses ${sep}
	/if (${validate_cast[FALSE, spell, "${splashCure}", ${Me.ID}]}) /cast "${splashCure}" loc ${Me.X} ${Me.Y} ${Me.Z}
	
/return TRUE



|***
 *	DES: Class controls
 *	USE: /pal [] [##]
 *  NOTE:
 ***|
#bind setPAL /pal
sub Bind_setPAL(string _type, string _verbage)
	/if (!${validate_class[FALSE, |PAL]}) /return

	/declare _sep bool local
	/declare _listout string local
	/declare _count int local 0
	/declare _bind string local ${Me.Class.ShortName.Lower}
  /declare _lstClass string	local |ae|attraction|defcount|fade|groupheal|harmonious|healpoint|loh|mg|rampage|rod|shieldflash|spiregroup|spireraid|stance|steel|stun|taunt|xtclass|xttime|yaulp
	
	| to Yaulp or not		
	/if (${_type.Equal[yaulp]}) {
	 	/call set_switch ${Me.Class.ShortName} doYaulp ${_verbage}  
		
	| AA Marr's Gift	Control	
	} else /if (${_type.Equal[mg]}) {
		/if (${Bool[${_verbage}]} && ${Range.Between[1,99:${_verbage}]}) {
			/call sql_update FALSE ${Me.Class.ShortName} useMGPct ${_verbage} 
		} else /if (${Bool[${_verbage}]} && !${Range.Between[1,99:${_verbage}]}) {
			VOUT /${_bind} mg [${cinfo}##\ax] allowable range ${sep} ${cinfo}1 - 99\ax
			/return
		}	else /if (!${Bool[${_verbage}]}) {
			/call set_switch ${Me.Class.ShortName} useMG ${_verbage}  
		}	

	| AA Lay on Hands	
	} else /if (${_type.Equal[loh]}) {
		/if (${Bool[${_verbage}]} && ${Range.Between[1,99:${_verbage}]}) {
			/call sql_update FALSE ${Me.Class.ShortName} useLoHPct ${_verbage} 
		} else /if (${Bool[${_verbage}]} && !${Range.Between[1,99:${_verbage}]}) {
			VOUT /${_bind} loh [${cinfo}##\ax] allowable range ${sep} ${cinfo}1 - 99\ax
			/return
		}	else /if (!${Bool[${_verbage}]}) {
			/call set_switch  ${Me.Class.ShortName} useLoH ${_verbage}  
		}	
		
	| Shield Flash oh shit
	} else /if (${_type.Equal[shieldflash]}) {
		/if (${Bool[${_verbage}]} && ${Range.Between[1,99:${_verbage}]}) {
			/call sql_update FALSE ${Me.Class.ShortName} useShieldFlashPct ${_verbage} 
		} else /if (${Bool[${_verbage}]} && !${Range.Between[1,99:${_verbage}]}) {
			VOUT /${_bind} shieldflash [${cinfo}##\ax] allowable range ${sep} ${cinfo}1 - 99\ax
			/return
		}	else /if (!${Bool[${_verbage}]}) {
			/call set_switch ${Me.Class.ShortName} useShieldFlash ${_verbage}  
		}			

	| spire control
	} else /if (${_type.Equal[spiregroup]} || ${_type.Equal[spireraid]}) {
		/call set_character_spire FALSE ${_type} ${_verbage}

	| set AA fade use %	
	} else /if (${_type.Equal[fade]}) {
		/call set_character_fade FALSE ${_verbage}

	| use group Heals
	} else /if (${_type.Equal[groupheal]}) {
		/call set_character_groupheal FALSE ${_verbage}
		
	| use Modulation Rods
	} else /if (${_type.Equal[rod]}) {
		/call set_switch ${Me.Class.ShortName} swModRod ${_verbage} 	

	| set Class Heal Point	
	} else /if (${_type.Equal[healpoint]}) {
		/call set_character_healpoint FALSE ${_verbage}
		/return
			
	| on/off taunt
	} else /if (${_type.Equal[taunt]}) {
		/call set_switch ${Me.Class.ShortName} taunt ${_verbage}  

	| on/off switch for Steel
	} else /if (${_type.Equal[steel]}) {
		/call set_switch ${Me.Class.ShortName} switchSteel ${_verbage}  
		VOUT /${_bind} steel (${cnum}${Steel}\ax) ${sep} ${If[${switchSteel},${oenabled},${odisabled}]}
		/return
	
	| on/off switch for Harmonious
	} else /if (${_type.Equal[harmonious]}) {
		/call set_switch ${Me.Class.ShortName} switchHarmonious ${_verbage}  
		VOUT /${_bind} harmonious (${cnum}${Harmonious}\ax) ${sep} ${If[${switchHarmonious},${oenabled},${odisabled}]}
		/return

	| on/off switch for Stance
	} else /if (${_type.Equal[stance]}) {
		/call set_switch ${Me.Class.ShortName} switchStance ${_verbage}  
		VOUT /${_bind} stance (${cnum}${Stance}\ax) ${sep} ${If[${switchStance},${oenabled},${odisabled}]}
		/return

	| set the XTarget class to heal
	} else /if (${_type.Equal[xtclass]}) {
		/call set_character_xthealclass FALSE ${_verbage}
		/return


	| set XT refresh list Time
	} else /if (${_type.Equal[xttime]}) {
		/if (${Range.Between[1,10:${Int[${_verbage}]}]}) {
			/call sql_update FALSE timer tXHeal ${_verbage}
			/call echos number "XTarget refresh time" ${_verbage} 0 0
		} else /if (!${Bool[${_verbage}]} || !${Range.Between[1,10:${Int[${_verbage}]}]}) {
			VOUT /${_bind} xttime [${cinfo}${tXHeal}m\ax] allowable range ${sep} ${cinfo}1 - 5 min\ax
		}	

	| AE or Focused Melee
	} else /if (${_type.Equal[ae]}) {
		/call set_switch ${Me.Class.ShortName} AEMelee ${_verbage}

	| attraction Divine call while pulling
	} else /if (${Bool[${_type.Equal[attraction]}]}) {
		/call set_switch ${Me.Class.ShortName} switchAttraction ${_verbage}

	| set rampage tank toon
	} else /if (${_type.Equal[rampage]}) {
		/if (${Bool[${_verbage}]}) {
			/call sql_update FALSE ${Me.Class.ShortName} RampageTank ${_verbage.Left[1].Upper}${_verbage.Right[-1].Lower}
		} else /if (!${Bool[${_verbage}]}) {
			/call sql_update FALSE ${Me.Class.ShortName} RampageTank FALSE
		}

	| Stun Control
	} else /if (${_type.Equal[stun]}) {
		/if (${Bool[${_verbage}]}) {
			/if (${Bool[${_verbage.Equal[stop]}]}) {
					/call sql_update FALSE ${Me.Class.ShortName} chainStun FALSE
			} else /if (${Bool[${_verbage.Find[divine]}]}) {
					/call sql_update FALSE ${Me.Class.ShortName} chainStun "Divine Stun"
			} else /if (${Bool[${_verbage.Find[force]}]}) {
					/call sql_update FALSE ${Me.Class.ShortName} chainStun "Force of Disruption"
			} else /if (${Bool[${_verbage.Find[disruptive]}]}) {
					/call sql_update FALSE ${Me.Class.ShortName} chainStun "Disruptive Persecution"
			}	
		} else {
			/declare _stunType |stop|divine|force|disruptive
			/for _count 1 to ${_stunType.Count[|]}
				/varset _listout ${_listout} ${If[${_sep},${sep},]} ${cinfo}${_stunType.Arg[${_count},|]}\ax
				/varset _sep TRUE
			/next _count
			VOUT /${_bind} stun [${_listout}]
		}
		VOUT Chain Stun: ${cinfo}${chainStun}\ax
		/return
		
	| defensive mob agro count
	} else /if (${_type.Equal[defcount]}) {
		/if (${Range.Between[1,50:${Int[${_verbage}]}]}) {
			/call sql_update FALSE ${Me.Class.ShortName} defCount ${_verbage}
		} else /if (!${Bool[${_verbage}]} || !${Range.Between[1,50:${Int[${_verbage}]}]}) {
			VOUT /${_bind} defcount [${cinfo}${countsetMobAgro}\ax] allowable range ${sep} ${cinfo}1 - 50 min\ax
			/return
		}	

	}

	/if (${swMQClear}) /mqclear
	/for _count 1 to ${_lstClass.Count[|]}
		/varset _listout ${_listout} ${If[${_sep},${sep},]} ${cinfo}${_lstClass.Arg[${_count},|]}\ax
		/if (${_lstClass.Arg[${_count},|].Equal[ae]}) /varset _listout ${_listout}:${If[${AEMelee},${oon},${ooff}]}
		/if (${_lstClass.Arg[${_count},|].Equal[attraction]}) /varset _listout ${_listout}:${If[${Bool[${switchAttraction}]},${oon},${ooff}]}
		/if (${_lstClass.Arg[${_count},|].Equal[defcount]}) /varset _listout ${_listout}:${cnum}${defCount}\ax
		/if (${_lstClass.Arg[${_count},|].Equal[fade]}) /varset _listout ${_listout}:${If[${switchAAFade},${cnum}${useAAFadePct}\ax,${ooff}]}
		/if (${_lstClass.Arg[${_count},|].Equal[groupheal]}) /varset _listout ${_listout}:${If[${useGroupHeal},${cnum}${useGroupHeal}\ax,${ooff}]}
		/if (${_lstClass.Arg[${_count},|].Equal[harmonious]}) /varset _listout ${_listout}:${If[${Bool[${switchHarmonious}]},${oon},${ooff}]}
		/if (${_lstClass.Arg[${_count},|].Equal[healpoint]}) /varset _listout ${_listout}:${cnum}->\ax
		/if (${_lstClass.Arg[${_count},|].Equal[loh]}) /varset _listout ${_listout}:${If[${useLoH},${cnum}${useLoHPct}\ax,${ooff}]}
		/if (${_lstClass.Arg[${_count},|].Equal[mg]}) /varset _listout ${_listout}:${If[${useMG},${cnum}${useMGPct}\ax,${ooff}]}
		/if (${_lstClass.Arg[${_count},|].Equal[rampage]}) /varset _listout ${_listout}:${If[${Bool[${RampageTank}]},${cnum}${RampageTank}\ax,${ooff}]}
		/if (${_lstClass.Arg[${_count},|].Equal[rod]}) /varset _listout ${_listout}:${If[${Bool[${swModRod}]},${oon},${ooff}]}
		/if (${_lstClass.Arg[${_count},|].Equal[shieldflash]}) /varset _listout ${_listout}:${If[${useShieldFlash},${cnum}${useShieldFlashPct}\ax,${cbad}${useShieldFlashPct}\ax]}
		/if (${_lstClass.Arg[${_count},|].Equal[spiregroup]}) /varset _listout ${_listout}:${If[${spireGroup},${cnum}${spireGroup}\ax,${ooff}]}
		/if (${_lstClass.Arg[${_count},|].Equal[spireraid]}) /varset _listout ${_listout}:${If[${spireRaid},${cnum}${spireRaid}\ax,${ooff}]}
		/if (${_lstClass.Arg[${_count},|].Equal[stance]}) /varset _listout ${_listout}:${If[${Bool[${switchStance}]},${oon},${ooff}]}
		/if (${_lstClass.Arg[${_count},|].Equal[steel]}) /varset _listout ${_listout}:${If[${Bool[${switchSteel}]},${oon},${ooff}]}
		/if (${_lstClass.Arg[${_count},|].Equal[stun]}) /varset _listout ${_listout}:${If[${Bool[${chainStun}]},${oon},${ooff}]}
		/if (${_lstClass.Arg[${_count},|].Equal[taunt]}) /varset _listout ${_listout}:${If[${Bool[${taunt}]},${oon},${ooff}]}
		/if (${_lstClass.Arg[${_count},|].Equal[xtclass]}) /varset _listout ${_listout}:${cnum}->\ax
		/if (${_lstClass.Arg[${_count},|].Equal[xttime]}) /varset _listout ${_listout}:${cnum}${tXHeal}\ax
		/if (${_lstClass.Arg[${_count},|].Equal[yaulp]}) /varset _listout ${_listout}:${If[${doYaulp},${oon},${ooff}]}
		/varset _sep TRUE
	/next _count
	VOUT /${_bind} [${_listout}]
/return

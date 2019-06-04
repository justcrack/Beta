|***
 *	class.rog.inc
 *	Core Rogue Include
 *
 * 
 * 
 ***|
 


|***
 *	DES: Main Rogue Routine
 *	USE: 
 *  NOTE: 
 ***|
sub core_ROG()
	DEBUG core_ROG()
	CHECKINPUTS
	AMIDEAD
	CHECKTIE

	/if (${coreAuto}) {
		/if (!${timer_click_poison}) /call click_poison
		/if (${swSoS}) /call automaticSoS
	}
	
	/while (${prep_combat[]}) {
		CHECKINPUTS
		/if (${swHealSelf}) /call patch_heal_${Me.Class.ShortName}
		/if (!${timer_check_cast_cure} && ${swHealCure}) /call cast_cure FALSE
		/if (${exit_class_loop[]}) /return FALSE

		/if (${isValidTarget}) {
			/call sneak_attack
			/call set_combat ${combat.Upper}
			/if (${swAlliance}) /call trigger_alliance ${Target2Kill}
			/if (${swBurnControl}) /call burn_${Me.Class.ShortName}
			/if (${swSwarm} || ${swPet}) /call send_pet
			/if (${swCombatMelee}) /call use_skill_melee ${Target2Kill}
			/call disc_${Me.Class.ShortName}
			/call check_deBuffs ${Target2Kill}
			/call click_BP
			/call click_item
			/if (${swSwarm}) /call click_item_swarm ${Target2Kill}
			/call click_DPS_${Me.Class.ShortName}
		}

		/if (${coreAuto}) {	
			/if (${switchAAFade} && ${Me.PctHPs} <= ${useAAFadePct}) /call cast_AA_fade
			/if (${validate_class[FALSE, ${lstClassMelee}]}) /call cast_stamina_recovery
		}
	}
/return TRUE



|***
 *	DES: execute the target!
 *	USE: 
 *  NOTE: 
 ***|
sub sneak_attack()
	DEBUG sneak_attack()
	/if (${Bool[${DiscSneakAttack}]} && !${Me.CombatAbilityReady[${Spell[${DiscSneakAttack}].RankName}]}) /return
	CHECKEXIT
	/if (${is_target_dead[${Target2Kill}]}) /return
	
	/if (${Target.ID} && ${Me.Invis} && ${Me.Sneaking}) {
		/if (${validate_cast[FALSE, disc, "${DiscSneakAttack}", ${Target2Kill}]}) /call core_cast2 "${DiscSneakAttack}" disc ${Target2Kill} FALSE
		/doability backstab
	}
/return



|***
 *	DES: Combat skills, Disciplines and Alternate abilities.
 *	USE: Disc
 *  NOTE: 
 ***|
sub disc_ROG()
	DEBUG disc_ROG()
	CHECKEXIT
	CHECKINPUTS
	/if (${is_target_dead[${Target2Kill}]}) /return
	/call set_combat ${combat.Upper}
	/if (${validate_cast[FALSE, disc, "${DiscAssault}", ${Target2Kill}]}) /call core_cast2 "${DiscAssault}" disc ${Target2Kill} FALSE
	
	/if (${validate_cast[FALSE, alt, "Shadow's Flanking", ${Me.ID}]}) /call core_cast2 "Shadow's Flanking" alt 0 FALSE
	/if (${validate_cast[FALSE, disc, "${DiscVision}", ${Me.ID}]} && !${Bool[${Me.Song[${DiscVision}]}]}) /call core_cast2 "${DiscVision}" disc 0 FALSE
	/if (${validate_cast[FALSE, alt, "${TwistedShank}", ${Target2Kill}]}) /call core_cast2 "${TwistedShank}" alt ${Target2Kill} FALSE
	/if (${validate_cast[FALSE, alt, "${AbsorbingAgent}", ${Target2Kill}]}) /call core_cast2 "${AbsorbingAgent}" alt ${Target2Kill} FALSE
	/if (${validate_cast[FALSE, disc, "${DiscShadowHunter}", ${Target2Kill}]}) /call core_cast2 "${DiscShadowHunter}" disc ${Target2Kill} FALSE
	/if (${validate_cast[FALSE, disc, "${DiscDisassociative}", ${Target2Kill}]}) /call core_cast2 "${DiscDisassociative}" disc ${Target2Kill} FALSE
	/if (${validate_cast[FALSE, disc, "${DiscBleed}", ${Target2Kill}]}) /call core_cast2 "${DiscBleed}" disc ${Target2Kill} FALSE

	/if (${Me.CombatAbilityReady[${Spell[${DiscPlay}].RankName}]}) {
		/if (${NameList.Find[${Spawn[${Target2Kill}].DisplayName}]} && ${Me.CombatAbilityReady[${Spell[${DiscFrenzy}].RankName}]}) /goto :skipDiscPlay
		/if (${validate_cast[FALSE, disc, "${DiscPlay}", ${Me.ID}]} && !${Bool[${Me.ActiveDisc}]}) /call core_cast2 "${DiscPlay}" disc 0 FALSE
		:skipDiscPlay
	}
	
	/if (!${NameList.Find[${Target.DisplayName}]} && !${Me.CombatAbilityReady[${Spell[${DiscFrenzy}].RankName}]}) {
		/if (${validate_cast[FALSE, disc, "${DiscPlay}", ${Me.ID}]} && !${Bool[${Me.ActiveDisc}]}) /call core_cast2 "${DiscPlay}" disc 0 FALSE
	}		
	
	/if (${Target.ID} && ${Me.CombatAbilityReady[${Spell[${DiscPhantom}].RankName}]} && ${Bool[${DiscPhantom}]}) /call core_cast2 "${DiscPhantom}" disc ${Target2Kill} FALSE
	/if (${validate_cast[FALSE, disc, "${DiscHiddenBlade}", ${Me.ID}]}) /call core_cast2 "${DiscHiddenBlade}" disc 0 FALSE
	/if (${validate_cast[FALSE, disc, "${DiscHack}", ${Target2Kill}]}) /call core_cast2 "${DiscHack}" disc ${Target2Kill} FALSE
	/if (${validate_cast[FALSE, disc, "${DiscBladePoison}", ${Me.ID}]}) /call core_cast2 "${DiscBladePoison}" disc 0 FALSE
	/if (${validate_cast[FALSE, disc, "${DiscAggro}", ${Target2Kill}]}) /call core_cast2 "${DiscAggro}" disc ${Target2Kill} FALSE
	/if (${validate_cast[FALSE, disc, "${DiscBlinding}", ${Target2Kill}]} && ${Target.Level} <= 110) /call core_cast2 "${DiscBlinding}" disc ${Target2Kill} FALSE
	
/return



|***
 *	DES: DPS Clickies
 *	USE: this shits auto, learn to love it.
 *  NOTE: 
 ***|
sub click_DPS_ROG()
	DEBUG click_DPS_ROG()
	CHECKEXIT
	CHECKINPUTS
	/if (${is_target_dead[${Target2Kill}]}) /return
	/call click_horn_of_unity 	
	/if (${validate_cast[FALSE, epicitem, "2", ${Me.ID}]}) /call core_cast2 "${${Me.Class.ShortName}Epic2}" item 0 FALSE
/return



|***
 *	DES: emergency healing
 *	USE: auto
 *  NOTE: 
 ***|
sub patch_heal_ROG()
	DEBUG patch_heal_ROG()
	AMIDEAD
	CHECKEXIT
	/if (${Me.PctHPs} > ${${Me.Class.ShortName}Point}*.${tmpHealOverride}) /return
	/if (${Me.PctHPs} <= ${${Me.Class.ShortName}Point}*.${tmpHealOverride} && ${validate_cast[FALSE, alt, "Tumble", ${Me.ID}]}) /call core_cast2 "Tumble" alt 0 FALSE
	/call click_item_heal
/return


|***
 *	DES: SoS switch
 *	USE: auto
 *  NOTE: Work in progress
 ***|
sub automaticSoS()
	DEBUG automaticSoS()
	/if (!${swSoS}) /return
	/if (${Me.Combat} || ${Me.Stunned} || ${Me.State.Equal[FEIGN]}) /return
	/if (${Me.Casting.ID} || ${Window[TradeWnd].Open} || ${Window[MerchantWnd].Open} || ${Window[BigBankWnd].Open} || ${Me.Mount.ID}) /return

	AMIDEAD
	CHECKTIE
	
	/declare _sos bool local FALSE

	/if (${Me.Invis} && ${Me.Sneaking} && !${Me.AbilityReady[hide]} && !${Me.AbilityReady[sneak]}) {
		/return
	}

	/if (!${Me.Invis} && ${Me.AbilityReady[hide]} && ${Me.Sneaking} && !${Me.AbilityReady[sneak]} ) {
		/if (${Me.AbilityReady[hide]}) /doability hide
		/return	
	} else /if (!${Me.Invis} && ${Me.Sneaking} && !${Me.AbilityReady[sneak]}) {
		/if (${Me.AbilityReady[sneak]}) /doability sneak
		/return
	}
	
	/if (!${_sos} && ${Me.AbilityReady[hide]} && ${Me.AbilityReady[sneak]}) {
		/doability hide
		/delay 1
		/doability sneak
	}
/return



|***
 *	DES: keep poisons running
 *	USE: /call click_poison OR /poison
 *  NOTE: 
 ***|
sub click_poison()
	DEBUG click_poison()
	CHECKEXIT
	CHECKTIE
	/declare _count int local 0
	/declare _found bool local FALSE
	
	| keep a stock on hand
	/if (${FindItemCount[${clickPoisonItem}]} < ${clickPoisonCount} && ${Me.ItemReady[${clickPoisonDispenser}]}) {
		/if (${validate_cast[FALSE, item, "${clickPoisonDispenser}", ${Me.ID}]}) /call core_cast2 "${clickPoisonDispenser}" item 0 FALSE
		/call sort_cursor TRUE
	}
	
	| check the buffs
	/for _count 1 to ${clickPoisonBuff.Count[|]}
		AMIDEAD
		CHECKINPUTS
		/if (${Bool[${Me.Buff[${clickPoisonBuff.Arg[${_count},|]}]}]}) /varset _found TRUE
	/next _count		
	/if (!${_found}) {
		/if (${validate_cast[FALSE, item, "${clickPoisonItem}", ${Me.ID}]}) /call core_cast2 "${clickPoisonItem}" item 0 FALSE
	}
	/call timer_update timer_click_poison ${tClick_Poison}
/return



|***
 *	DES: Rogue burn routines
 *	USE: establish all comp/trigger skills
 *	NOTE: 
 ***| 
sub burn_ROG()
	DEBUG burn_ROG()
	CHECKEXIT
	CHECKINPUTS
	
	/if (${is_target_dead[${Target2Kill}]}) /return
	/call set_combat ${combat.Upper}

	/if (${Me.ActiveDisc.Name.Equal[${DiscPlay}]} && ${Me.CombatAbilityReady[${Spell[${DiscFrenzy}].RankName}]}) {
		:loopmakestop
		/delay 5
		/if (${Me.ActiveDisc.Name.Equal[${DiscPlay}]} && ${Me.CombatAbilityReady[${Spell[${DiscFrenzy}].RankName}]}) /stopdisc
		/delay 5
		/if (${Me.ActiveDisc.Name.Equal[${DiscPlay}]} && ${Me.CombatAbilityReady[${Spell[${DiscFrenzy}].RankName}]}) /goto :loopmakestop
	}

	| envenomed blades / dirty fighting stack issue
	/if (${Target.ID} && ${Me.AltAbilityReady[Dirty Fighting]} && !${Me.Buff[Envenomed Blades].ID}) {
		/if (${validate_cast[FALSE, alt, "Dirty Fighting", ${Target2Kill}]}) /call core_cast2 "Dirty Fighting" alt ${Target2Kill} FALSE
	} else /if (${Me.AltAbilityReady[Envenomed Blades]} && !${Me.Buff[Dirty Fighting].ID}) {
		/if (${validate_cast[FALSE, alt, "Envenomed Blades", ${Me.ID}]}) /call core_cast2 "Envenomed Blades" alt 0 FALSE
	}

	/if (${validate_cast[FALSE, disc, "${DiscDicho}", ${Me.ID}]}) /call core_cast2 "${DiscDicho}" disc 0 FALSE

	/if (!${AEMelee}) {
		/if (${validate_cast[FALSE, alt, "Focused Rake's Rampage", ${Me.ID}]}) /call core_cast2 "Focused Rake's Rampage" alt 0 FALSE
	} else /if (${AEMelee}) {
		/if (${validate_cast[FALSE, alt, "Rake's Rampage", ${Me.ID}]}) /call core_cast2 "Rake's Rampage" alt 0 FALSE
	}

	/if (${validate_cast[FALSE, disc, "${DiscPinPoint}", ${Target2Kill}]}) /call core_cast2 "${DiscPinPoint}" disc ${Target2Kill} FALSE
	/if (${validate_cast[FALSE, disc, "${DiscAggro}", ${Target2Kill}]}) /call core_cast2 "${DiscAggro}" disc ${Target2Kill} FALSE
	/if (${validate_cast[FALSE, alt, "Rogue's Fury", ${Me.ID}]}) /call core_cast2 "Rogue's Fury" alt 0 FALSE
	/if (${validate_cast[FALSE, disc, "${DiscMark}", ${Target2Kill}]} && ${Me.PctEndurance} > 15) /call core_cast2 "${DiscMark}" disc ${Target2Kill} FALSE
	/call cast_AA_spire
	/if (${validate_cast[FALSE, alt, "Intensity of the Resolute", ${Me.ID}]} && ${Raid.Members}) /call core_cast2 "Intensity of the Resolute" alt 0 FALSE
  /if (!${Me.ActiveDisc.ID}) {
  	/if (${validate_cast[FALSE, disc, "${DiscFrenzy}", ${Me.ID}]}) /call core_cast2 "${DiscFrenzy}" disc 0 FALSE
  	/delay 2
  }
  /if (${validate_cast[FALSE, disc, "${DiscTwistedChance}", ${Me.ID}]} && !${Me.ActiveDisc.ID}) /call core_cast2 "${DiscTwistedChance}" disc 0 FALSE
  /if (${validate_cast[FALSE, disc, "${DiscEradicator}", ${Me.ID}]} && !${Me.ActiveDisc.ID}) /call core_cast2 "${DiscEradicator}" disc 0 FALSE
  /if (${validate_cast[FALSE, disc, "${DiscRazor}", ${Me.ID}]} && !${Me.ActiveDisc.ID}) /call core_cast2 "${DiscRazor}" disc 0 FALSE

  /if (${validate_cast[FALSE, disc, "${DiscAspBleeder}", ${Me.ID}]} && !${Me.ActiveDisc.ID}) /call core_cast2 "${DiscAspBleeder}" disc 0 FALSE

	/call click_burn_circle
	/call click_item_burn
/return



|***
 *	DES: Class controls
 *	USE: /rog []
 *  NOTE:
 ***|
#bind setROG /rog
sub Bind_setROG(string _type, string _verbage)
	/if (!${validate_class[FALSE, |ROG]}) /return

	/declare _sep bool local
	/declare _listout string local
	/declare _count int local 0
	/declare _bind string local ${Me.Class.ShortName.Lower}
  /declare _lstClass string local |ae|fade|healpoint|poison|sos|spiregroup|spireraid
	
	| set poison
	/if (${_type.Equal[poison]}) {
		
		| set max num of poison to keep
		/if (${Range.Between[1,100:${Int[${_verbage}]}]}) {
			/call sql_update FALSE ${chrID} clickPoisonCount ${_verbage}
			VOUT Rogue Poison Keep ${sep} ${cinfo}${_num}\ax
			/return
		} else /if (!${Bool[${_verbage}]} || !${Range.Between[1,100:${Int[${_verbage}]}]}) {
			VOUT /${_bind} poison [${cinfo}${clickPoisonCount}\ax] of poison to keep ${cinfo}1 - 100\ax
			/return
		}

	| set AA fade use %	
	} else /if (${_type.Equal[fade]}) {
		/call set_character_fade FALSE ${_verbage}

	| spire control
	} else /if (${_type.Equal[spiregroup]} || ${_type.Equal[spireraid]}) {
		/call set_character_spire FALSE ${_type} ${_verbage}

	| auto SOS... or not...
	} else /if (${_type.Equal[sos]}) {
		/call set_switch ${chrID} swSoS ${_verbage}

	| AE or Focused Melee
	} else /if (${_type.Equal[ae]}) {
		/call set_switch ${chrID} AEMelee ${_verbage}

	| healpoint
	} else /if (${_type.Find[healpoint]}) {
		/call set_character_healpoint FALSE ${_verbage}
		/return

	}
	/if (${swMQClear}) /mqclear
	/for _count 1 to ${_lstClass.Count[|]}
		/varset _listout ${_listout} ${If[${_sep},${sep},]} ${cinfo}${_lstClass.Arg[${_count},|]}\ax
		/if (${_lstClass.Arg[${_count},|].Equal[spiregroup]}) /varset _listout ${_listout}:${cnum}${spireGroup}\ax
		/if (${_lstClass.Arg[${_count},|].Equal[spireraid]}) /varset _listout ${_listout}:${cnum}${spireRaid}\ax
		/if (${_lstClass.Arg[${_count},|].Equal[healpoint]}) /varset _listout ${_listout}:${cnum}${${Me.Class.ShortName}Point}\ax
		/if (${_lstClass.Arg[${_count},|].Equal[poison]}) /varset _listout ${_listout}:${cnum}${clickPoisonCount}\ax
		/if (${_lstClass.Arg[${_count},|].Equal[ae]}) /varset _listout ${_listout}:${If[${Bool[${AEMelee}]},${oon},${ooff}]}
		/if (${_lstClass.Arg[${_count},|].Equal[fade]}) /varset _listout ${_listout}:${If[${switchAAFade},${cnum}${useAAFadePct}\ax,${cbad}${useAAFadePct}\ax]}
		/if (${_lstClass.Arg[${_count},|].Equal[sos]}) /varset _listout ${_listout}:${If[${swSoS},${oon},${ooff}]}

		/varset _sep TRUE
	/next _count
	VOUT /${_bind} [${_listout}]
/return

|***
 *	class.ber.inc
 *	Core Berzerker Include  
 *
 *	
 ***|



|***
 *	DES: Main Berzerker Routine
 *	USE: 
 *  NOTE: 
 ***|
sub core_BER()
	DEBUG core_BER()
	
	/while (${prep_combat[]}) {
		CHECKINPUTS
		/if (${exit_class_loop[]}) /return

		/if (${swHealSelf}) /call patch_heal_${Me.Class.ShortName}
		/if (${swHealCure}) /call cast_cure

		/if (${isValidTarget}) {
			/call set_combat ${combat.Upper}
			/if (${validate_cast[FALSE, disc, "${DiscSlapintheFace}", ${Target2Kill}]} && ${Me.PctAggro} <= 0 && !${Me.Combat}) /call core_cast2 "${DiscSlapintheFace}" disc ${Target2Kill}
			/if (${swBurnControl}) /call burn_${Me.Class.ShortName}
			/if (${swSwarm} || ${swPet}) /call send_pet
			/if (${swCombatMelee}) /call use_skill_melee ${Target2Kill}
			/call disc_${Me.Class.ShortName}
			/if (${Bool[${setAlliance}]}) /call cast_alliance_${Me.Class.ShortName} ${Target2Kill}
			/call check_deBuffs ${Target2Kill}
			/call click_BP
			/call click_item
			/if (${swSwarm}) /call click_item_swarm ${Target2Kill}
			/call click_DPS_${Me.Class.ShortName}
		}

		/if (${coreAuto}) {	
			/if (${switchAAFade} && ${Me.PctHPs} <= ${useAAFadePct}) /call cast_AA_fade
			/if (${validate_class[FALSE, ${lstClassMelee}]}) /call cast_stamina_recovery
			/call check_ammo
		}
	} 
/return



|***
 *	DES: 
 *	USE: 
 *  NOTE: 
 ***|
sub disc_BER()
	DEBUG disc_BER()
	AMIDEAD

	CHECKINPUTS
	/if (${validate_cast[FALSE, disc, "${DiscOverwhelmingFrenzy}", ${Target2Kill}]}) /call core_cast2 "${DiscOverwhelmingFrenzy}" disc ${Target2Kill} FALSE
	/if (${validate_cast[FALSE, disc, "${DiscEnduranceTap}", ${Me.ID}]}) /call core_cast2 "${DiscEnduranceTap}" disc 0 FALSE
	/if (${Bool[${Me.Buff[Frenzied Resolve]}]} && ${Spawn[${Target2Kill}].Distance} > ${setCombatDistance}) {
		/if (${validate_cast[FALSE, alt, "Drawn to Blood", ${Me.ID}]}) /call core_cast2 "Drawn to Blood" alt 0 FALSE
	}
	/if (${validate_cast[FALSE, alt, "Battle Leap", ${Me.ID}]} && !${Me.Underwater} && !${Me.Song[Battle Leap Warcry].ID}) /call core_cast2 "Battle Leap" alt 0 FALSE
	
	/call is_burn_time
	/if (!${Macro.Return}) {
		/if (${validate_cast[FALSE, disc, "${DiscDisconcert}", ${Me.ID}]}) /call core_cast2 "${DiscDisconcert}" disc 0 FALSE			
		/if (${validate_cast[FALSE, alt, "${RecklessAbandon}", ${Me.ID}]}) /call core_cast2 "${RecklessAbandon}" alt 0 FALSE
	}
		
	/if (${validate_cast[FALSE, alt, "Binding Axe", ${Me.ID}]}) /call core_cast2 "Binding Axe" alt 0 FALSE
	
	/if (${AEMelee}) {
		/if (${validate_cast[FALSE, alt, "Rampage", ${Me.ID}]} && ${Bool[${Me.Song[Strike of Savagery]}]}) /call core_cast2 "Rampage" alt 0 FALSE
	}
		
	/if (!${Me.Song[Reckless Abandon].ID}) {
		/if (${validate_cast[FALSE, alt, "Vehement Rage", ${Me.ID}]}) /call core_cast2 "Vehement Rage" alt 0 FALSE
	}

	| to agro or not ?
	/if (!${swAgro}) {
		/if (${validate_cast[FALSE, alt, "Distraction Attack", ${Me.ID}]} && ${Me.PctAggro} > 96) /call core_cast2 "Distraction Attack" alt 0 FALSE
		/if (${validate_cast[FALSE, disc, "${DiscJolt}", ${Target2Kill}]} && ${Me.PctAggro} > 80) /call core_cast2 "${DiscJolt}" disc ${Target2Kill}
	}

	/if (${Me.PctHPs} < 90) {
		/if (${validate_cast[FALSE, disc, "${DiscCombatFrenzy}", ${Target2Kill}]}) /call core_cast2 "${DiscCombatFrenzy}" disc ${Target2Kill} FALSE
	}
		
	/if (${Me.PctHPs} > 75 && ${Me.PctEndurance} < 50) {
		/if (${validate_cast[FALSE, alt, "Communion of Blood", ${Me.ID}]}) /call core_cast2 "Communion of Blood" alt 0 FALSE
	}
	
	| this hits up to 8 targets and can cause fear.
	/if (${AEMelee}) {
		/if (${validate_cast[FALSE, alt, "War Cry of the Braxi", ${Me.ID}]}) /call core_cast2 "War Cry of the Braxi" alt 0 FALSE
	}

	| dumb ass zerker has a dicho and a dicho ##. cant use the name to cast with..
	/if (${Me.CombatAbilityReady[${DiscDicho}]} && ${Me.CurrentEndurance} > ${Spell[${DiscDicho}].EnduranceCost}*2) /disc 49307
	
	/if (${AEMelee}) {
		/if (${validate_cast[FALSE, disc, "${DiscAESpiral}", ${Me.ID}]}) /call core_cast2 "${DiscAESpiral}" disc 0 FALSE
	}

	/if (${Me.PctHPs} > 90 && !${Me.Song[${OpenWoundEffect}].ID}) {
		/if (${validate_cast[FALSE, alt, "Bloodfury", ${Me.ID}]}) /call core_cast2 "Bloodfury" alt 0 FALSE
	}
	
	/if (${validate_cast[FALSE, disc, "${DiscAugmentedFrenzy}", ${Me.ID}]} && !${Me.Buff[${AugmentedFrenzyEffect}].ID}) /call core_cast2 "${DiscAugmentedFrenzy}" disc 0 FALSE
	/if (${validate_cast[FALSE, disc, "${DiscCryHavoc}", ${Me.ID}]} && !${Me.Song[${DiscCryHavoc}].ID}) /call core_cast2 "${DiscCryHavoc}" disc 0 FALSE
	/if (${validate_cast[FALSE, disc, "${DiscFesteringRage}", ${Me.ID}]} && !${Me.Song[${DiscFesteringRage}].ID}) /call core_cast2 "${DiscFesteringRage}" disc 0 FALSE
	
	| swarm pet
	/if (${swSwarm}) {
		/if (${validate_cast[FALSE, disc, "${DiscPet}", ${Target2Kill}]}) /call core_cast2 "${DiscPet}" disc ${Target2Kill} FALSE
	}

	| axes
	/if (${validate_cast[FALSE, disc, "${DiscVolley}", ${Target2Kill}]}) /call core_cast2 "${DiscVolley}" disc ${Target2Kill} FALSE
	/if (${validate_cast[FALSE, disc, "${DiscAxeofRallos}", ${Target2Kill}]}) /call core_cast2 "${DiscAxeofRallos}" disc ${Target2Kill} FALSE
	/if (${validate_cast[FALSE, disc, "${ThrowingAxe}", ${Target2Kill}]}) /call core_cast2 "${ThrowingAxe}" disc ${Target2Kill} FALSE

	/if (!${Me.Song[${SharedBloodLustRecourse}].ID} && ${Bool[${Me.TargetOfTarget}]}) {
		/if (${validate_cast[FALSE, disc, "${SharedBloodLust}", ${Me.ID}]}) /call core_cast2 "${SharedBloodLust}" disc 0 FALSE
	}
/return



|***
 *	DES: Agro Routines
 * 	USE: manual/Named/raid. all sorts...
 *  NOTE: Zerker agro  control is built in to the routines. not here.
 ***|
sub get_agro_BER()
/return



|***
 *	DES: Patch Healing.  It's a zerker. use what we got...
 *	USE: auto
 *  NOTE: 
 ***|
sub patch_heal_BER()
	DEBUG patch_heal_BER()
	CHECKEXIT
	/if (${Me.PctHPs} > ${${Me.Class.ShortName}Point}*.${tmpHealOverride}) /return
	
	/if (${validate_cast[FALSE, alt, "Uncanny Resilience", ${Me.ID}]}) /call core_cast2 "Uncanny Resilience" alt 0 FALSE
	/if (${validate_cast[FALSE, alt, "Blood Sustenance", ${Me.ID}]}) /call core_cast2 "Blood Sustenance" alt 0 FALSE
	/call click_item_heal
/return



|***
 *	DES: DPS Clickies
 *	USE: this shits auto, learn to love it.
 *  NOTE: 
 ***|
sub click_DPS_BER()
	DEBUG click_DPS_BER()
	CHECKEXIT
	CHECKINPUTS
	/call click_horn_of_unity 	

	| epic 2.0
	/if (${validate_cast[FALSE, epicitem, "2", ${Me.ID}]} && ${Spawn[${Target2Kill}].Distance} <= ${setCombatDistance}) /call core_cast2 "${${Me.Class.ShortName}Epic2}" item 0 FALSE
/return



 |***
 *	DES: Berzerker burn routines
 *	USE: establish all comp/trigger skills
 *	NOTE: 
 ***| 
sub burn_BER()
	DEBUG burn_BER()
	CHECKEXIT
	CHECKINPUTS
	
	/if (${Me.ActiveDisc.Name.Equal[${DiscDisconcert}]}) {
		:loopmakestop
		/delay 5
		/if (${Me.ActiveDisc.Name.Equal[${DiscDisconcert}]}) /stopdisc
		/delay 5
		/if (${Me.ActiveDisc.Name.Equal[${DiscDisconcert}]}) /goto :loopmakestop
	}
	
	/if (${validate_cast[FALSE, alt, "Desperation", ${Me.ID}]} && ${Me.PctHPs} < 90) /call core_cast2 "Desperation" alt 0 FALSE
	/if (${Me.CombatAbilityReady[${DiscScream}]} && ${Me.CurrentEndurance} > ${Spell[${DiscScream}].EnduranceCost}) /disc ${DiscScream}
	/if (${Me.CombatAbilityReady[${CryOfChaos}]} && !${Me.Song[${CryOfChaos}].ID}) {
		/delay ${Math.Rand[8]}s ${Me.Song[${CryOfChaos}].ID}
		/if (!${Me.Song[${CryOfChaos}].ID}) {
			/if (${Raid.Members}) {
				/if (${validate_cast[FALSE, alt, "Cry of Battle", ${Me.ID}]}) /call core_cast2 "Cry of Battle" alt 0 FALSE
			}
			/if (${Me.CombatAbilityReady[${CryOfChaos}]} && ${Me.CurrentEndurance} > ${Spell[${CryOfChaos}].EnduranceCost}) /disc ${CryOfChaos}
		}
	} 
	
	| AA rampage choice
	/if (${AEMelee} && ${SpawnCount[npc group radius 50 playerstate 4]} > 1) {
		/if (${validate_cast[FALSE, alt, "Furious Rampage", ${Me.ID}]}) /call core_cast2 "Furious Rampage" alt 0 FALSE
	} else /if (!${AEMelee}) {
		/if (${validate_cast[FALSE, alt, "Focused Furious Rampage", ${Me.ID}]}) /call core_cast2 "Focused Furious Rampage" alt 0 FALSE
	}

	/if (!${swAgro}) {
		/if (${validate_cast[FALSE, alt, "Silent Strikes", ${Me.ID}]}) /call core_cast2 "Silent Strikes" alt 0 FALSE
	}
	
	/if (${validate_cast[FALSE, alt, "${UntamedgRage}", ${Me.ID}]}) /call core_cast2 "${UntamedgRage}" alt 0 FALSE

	/if (${validate_cast[FALSE, alt, "Blinding Fury", ${Me.ID}]}) /call core_cast2 "Blinding Fury" alt 0 FALSE
	/if (${validate_cast[FALSE, alt, "Juggernaut Surge", ${Me.ID}]}) /call core_cast2 "Juggernaut Surge" alt 0 FALSE
	/call use_glyph
	
	/if (!${Me.Song[Reckless Abandon].ID} && ${Raid.Members}) {
		/if (${validate_cast[FALSE, alt, "Intensity of the Resolute", ${Me.ID}]}) /call core_cast2 "Intensity of the Resolute" alt 0 FALSE
	}
	
	/call cast_AA_spire
	/if (${validate_cast[FALSE, alt, "Blood Pact", ${Me.ID}]}) /call core_cast2 "Blood Pact" alt 0 FALSE

	/if (${Bool[${Spawn[group PC Shaman].ID}]}) {
	 	/if (!${Me.Buff[Savage Spirit].ID} && !${Bool[${Me.ActiveDisc}]}) {
	 		/if (${validate_cast[FALSE, disc, "${DiscBerserking}", ${Me.ID}]}) /call core_cast2 "${DiscBerserking}" disc 0 FALSE		
	 	}
		/if (!${Bool[${Me.ActiveDisc}]} && ${Me.AltAbilityReady[Savage Spirit]}) {
		 /alt act 465
		 /delay 5
		 /return
		}
	} else {
		/if (!${Bool[${Me.ActiveDisc}]} && ${Me.AltAbilityReady[Savage Spirit]}) {
		 	/alt act 465
		 	/delay 5
		 	/return
		}
		/if (!${Me.Buff[Savage Spirit].ID} && !${Bool[${Me.ActiveDisc}]}) {
			/if (${validate_cast[FALSE, disc, "${DiscBerserking}", ${Me.ID}]}) /call core_cast2 "${DiscBerserking}" disc 0 FALSE		
		}
	}
	
	/if (!${Me.Buff[Savage Spirit].ID} && !${Bool[${Me.ActiveDisc}]}) {

		/if (${Me.PctHPs} < 89) {
			/if (${validate_cast[FALSE, disc, "${DiscFrenzyResolve}", ${Me.ID}]}) /call core_cast2 "${DiscFrenzyResolve}" disc 0 FALSE
		}

		/if (!${Me.CombatAbilityReady[${DiscFrenzyResolve}]}) /disc ${DiscVengeful}
		/if (!${Me.CombatAbilityReady[${DiscFrenzyResolve}]} && !${Me.CombatAbilityReady[${DiscVengeful}]}) /disc ${DiscCleavingAnger}
	}

	/call click_burn_circle
	/call click_item_burn	
/return



|***
 *  DES: Alliance (Demolisher's Alliance|spell)
 *  USE: 
 *  NOTE: 6 casts to fulm.
 ***|
sub cast_alliance_BER(int _tmpID)
	DEBUG cast_alliance_${Me.Class.ShortName()
		
	| force trigger while alliance is on target
	/while (${Target.Buff[${${Me.Class.ShortName}Alliance}].ID}) {
		/if (${validate_cast[FALSE, disc, "${setAllianceTrigger}", ${_tmpID}]}) /call core_cast2 "${setAllianceTrigger}" disc ${_tmpID} FALSE
	}

	| check alliance group/raid
	/if (!${Bool[${setAlliance}]}) /return FALSE
	/if (${setAlliance.Equal[raid]} && !${Raid.Members}) /return FALSE  
	/if (${setAlliance.Equal[group]} && !${Group}) /return FALSE  

	| enough berserker's to make it worth casting?
	/if (${SpawnCount[berserker radius ${setMobAgro} setzradius ${setzradius}]} < 3) /return FALSE

	| have the alliance on it already?
	/if (${Target.Buff[${${Me.Class.ShortName}Alliance}].ID}) /return FALSE

	| spell ready?
	/if (!${Me.CombatAbilityReady[${Spell[${${Me.Class.ShortName}Alliance}].RankName}]}) /return FALSE

	| no target
	/if (${is_target_dead[${_tmpID}]}) /return
	AMIDEAD
	CHECKEXIT
	CHECKINPUTS
	/if (${validate_cast[FALSE, disc, "${${Me.Class.ShortName}Alliance}", ${_tmpID}]} && ${NameList.Find[${Spawn[${Target2Kill}].DisplayName}]}) /call core_cast2 "${${Me.Class.ShortName}Alliance}" disc ${_tmpID} FALSE
/return TRUE



|***
 *	DES: Class controls
 *	USE: /ber []
 *  NOTE:
 ***|
#bind setBER /ber
sub Bind_setBER(string _type, string _verbage)
	/if (!${validate_class[TRUE, |BER]}) /return

	/declare _sep bool local
	/declare _listout string local
	/declare _count int local 0
	/declare _bind string local ${Me.Class.ShortName.Lower}
  /declare _lstClass string	local |ae|fade|healpoint|spiregroup|spireraid

	| AE or Focused Melee
	/if (${_type.Equal[ae]}) {
		/call set_switch ${Me.Class.ShortName} AEMelee ${_verbage}

	| spire control
	} else /if (${_type.Equal[spiregroup]} || ${_type.Equal[spireraid]}) {
		/call set_character_spire FALSE ${_type} ${_verbage}

	| set AA fade use %	
	} else /if (${_type.Equal[fade]}) {
		/call set_character_fade FALSE ${_verbage}

	| healpoint
	} else /if (${_type.Find[healpoint]}) {
		/call set_character_healpoint FALSE ${_verbage}
		/return

	}
	/for _count 1 to ${_lstClass.Count[|]}
		/varset _listout ${_listout} ${If[${_sep},${sep},]} ${cinfo}${_lstClass.Arg[${_count},|]}\ax
		/if (${_lstClass.Arg[${_count},|].Equal[spiregroup]}) /varset _listout ${_listout}:${cnum}${spireGroup}\ax
		/if (${_lstClass.Arg[${_count},|].Equal[spireraid]}) /varset _listout ${_listout}:${cnum}${spireRaid}\ax)
		/if (${_lstClass.Arg[${_count},|].Equal[healpoint]}) /varset _listout ${_listout}:${cnum}${${Me.Class.ShortName}Point}\ax
		/if (${_lstClass.Arg[${_count},|].Equal[ae]}) /varset _listout ${_listout}:${If[${AEMelee},${oon},${ooff}]}
		/if (${_lstClass.Arg[${_count},|].Equal[fade]}) /varset _listout ${_listout}:${If[${switchAAFade},${cnum}${useAAFadePct}\ax,${cbad}${useAAFadePct}\ax]}

		/varset _sep TRUE
	/next _count
	VOUT /${_bind} [${_listout}]
/return
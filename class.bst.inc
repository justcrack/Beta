|***
 *	class.bst.inc
 *	Core Beastlord Include
 *
 *
 ***|
 



|***
 *	DES: Main Beastlord Routine
 *	USE: 
 *  NOTE: 
 ***|
sub core_BST()
	DEBUG core_BST()

	/if (${coreAuto}) {
		/call check_mana_${Me.Class.ShortName}
		/if (${swPet}) /call check_pet
	}

	/while (${prep_combat[]}) {
		CHECKINPUTS
		/if (${swHealSelf}) /call patch_heal_${Me.Class.ShortName}
		/if (${swHealPet}) /call pet_heal_${Me.Class.ShortName}
		/if (!${timer_check_cast_cure} && ${swHealCure}) /call cast_cure FALSE
		/if (${exit_class_loop[]}) /return		

		/if (${isValidTarget}) {
			/call set_combat ${combat.Upper}
			/if (${swBurnControl}) /call burn_${Me.Class.ShortName}
			/if (${swSwarm} || ${swPet}) /call send_pet
			/if (${swSwarm}) /call cast_nuke_servant
			/if (${swCombatMelee}) /call use_skill_melee ${Target2Kill}
			/call disc_${Me.Class.ShortName}
			/if (${swAlliance}) /call trigger_alliance ${Target2Kill}
			/call check_deBuffs ${Target2Kill}
			/if (${swCombatDoT}) /call cast_DOT
			/if (${swCombatNuke}) /call cast_nuke
			/call click_BP
			/call click_item
			/if (${swSwarm}) /call click_item_swarm ${Target2Kill}
			/call click_DPS_${Me.Class.ShortName}
			/call check_mana_${Me.Class.ShortName}
		}

		/if (${coreAuto}) {	
			CHECKFADE
			/call check_mana_BST
			/if (${swModRod} && ${validate_class[FALSE, ${lstClassCast}]}) /call use_mod_rod
			/if (${validate_class[FALSE, ${lstClassMelee}]}) /call cast_stamina_recovery
		}	
	}
	
/return



|***
 *	DES: 
 *	USE: /call disc_BST
 *  NOTE: 
 ***|
sub disc_BST()
	DEBUG disc_BST()

	/if (${Me.PctEndurance} < 2) /return
	AMIDEAD
	CHECKEXIT
	CHECKINPUTS

	/if (${is_target_dead[${Target2Kill}]}) /return
	
	/if (${validate_cast[FALSE, disc, "${DiscBestial}", ${Me.ID}]} && !${Me.Song[${DiscBestial}].ID}) /call core_cast2 "${DiscBestial}" disc 0 FALSE
	/if (${validate_cast[FALSE, disc, "${DiscFocusClaws}", ${Target2Kill}]}) /call core_cast2 "${DiscFocusClaws}" disc ${Target2Kill} FALSE
	/if (${validate_cast[FALSE, alt, "${DiscEnduring}", ${Target2Kill}]} && !${Bool[${Target.Buff[${DiscEnduring}]}]}) /call core_cast2 "${DiscEnduring}" alt ${Target2Kill} FALSE
	/if (${validate_cast[FALSE, alt, "Roar of Thunder", ${Target2Kill}]} && ${Me.PctAggro}) /call core_cast2 "Roar of Thunder" alt ${Target2Kill} FALSE
	/if (${validate_cast[FALSE, alt, "Chameleon Strike", ${Target2Kill}]}) /call core_cast2 "Chameleon Strike" alt ${Target2Kill} FALSE
	/if (${validate_cast[FALSE, disc, "${DiscForay}", ${Target2Kill}]}) /call core_cast2 "${DiscForay}" disc ${Target2Kill} FALSE
	/if (${validate_cast[FALSE, disc, "${DiscRending}", ${Target2Kill}]}) /call core_cast2 "${DiscRending}" disc ${Target2Kill} FALSE
			
	| do we want the mobs attention?
	/if (${swAgro}) {
		/if (${validate_cast[FALSE, alt, "Roaring Strike", ${Target2Kill}]} && ${Me.PctAggro} < 80) /call core_cast2 "Roaring Strike" alt ${Target2Kill} FALSE
	} else /if (!${swAgro}) {
	}

	/if (${Me.Pet.ID}) {
		/if (!${Me.Song[Growl of the].ID} && !${Me.Song[${GrowlSpell}].ID} && !${Me.PetBuff[Savage Rage]} && !${Me.Song[${DiscSavage}].ID}) {
			/if (${validate_cast[FALSE, spell, "${GrowlSpell}", ${Me.ID}]}) /call core_cast2 "${GrowlSpell}" ${buffGem} 0 FALSE
		}
	}
	
/return



|***
 *	DES: Focused Paragon. will maintain the bst
 *	USE: /call check_mana_${Me.Class.ShortName}
 *  NOTE: 
 ***|
sub check_mana_BST()
	DEBUG check_mana_BST()

	AMIDEAD
	SIMPLEEXIT
	
	| consumption of Spirits AA
	/if (${switchConsumeSpirit}) {
		/if (${Me.PctMana} < ${ConsumeSpiritPct}*.${tmpHealOverride}) {
		 	/if (${validate_cast[FALSE, alt, "Consumption of Spirit", ${Me.ID}]}) {
		 		/call core_cast2 "Consumption of Spirit" alt 0 FALSE
		 		/return
		 	}
		}
	}
	
	| Paragon Group Members	NOT IN RAID.... if 2+ need stamina or mana
	/declare _count int local 0
	/declare _countHurt int local 0

	/if (${Group} && !${Raid} && ${Me.AltAbilityReady[Paragon of Spirit]}) {
		/for _count 0 to ${Group}
			AMIDEAD
			CHECKEXIT
			/if (${Group.Member[${_count}].Name.Equal[${Me.DisplayName}]}) /continue
			/if (${Group.Member[${_count}].PctMana} >= ${groupParagonPct}*.${tmpHealOverride} && ${Group.Member[${_count}].PctEndurance} >= ${groupParagonPct}*.${tmpHealOverride}) /continue
			/if (${Group.Member[${_count}].Distance} > 100) /continue
			/if (${Group.Member[${_count}].Type.Equal[CORPSE]}) /continue
			/if (${Group.Member[${_count}].Class.ShortName.Equal[BRD]}) /continue
			

			/varcalc _countHurt ${_countHurt}+1
		/next _count				
		
		/if (${_countHurt} > 1) {
			/if (${validate_cast[FALSE, alt, "Paragon of Spirit", ${Me.ID}]}) {
				/call core_cast2 "Paragon of Spirit" alt 0 FALSE
				ECHOCHANNEL AA:Paragon of Spirit
				/delay 2s
			}
	 	}

	}
	
	/if (${Me.Song[Paragon of Spirit].ID}) /return
	/if (${Me.Song[Focused Paragon of Spirit].ID}) /return
		
	| self paragon
	/if (${switchSelfParagon}) {
		/if (${Me.PctMana} <= ${selfParagonPct}*.${tmpHealOverride} || ${Me.PctEndurance} <= ${selfParagonPct}*.${tmpHealOverride}) {
			/if (!${Me.Song[Focused Paragon].ID} && !${Me.Song[Paragon of Spirits].ID}) {
				/if (${validate_cast[FALSE, alt, "Focused Paragon of Spirits", ${Me.ID}]}) {
					/call core_cast2 "Focused Paragon of Spirits" alt ${Me.ID} FALSE
					/delay 2s
				}
			}
		}
	}
		
/return


 
|***
 *	DES: emergency healing
 *	USE: /call patch_heal_${Me.Class.ShortName}
 *  NOTE: 
 ***|
sub patch_heal_BST()
	DEBUG patch_heal_BST()

	/if (${Me.PctHPs} > ${${Me.Class.ShortName}Point}*.${tmpHealOverride}) /return
	AMIDEAD
	CHECKEXIT
	
 	/if (${Me.PctHPs} < ${${Me.Class.ShortName}Point}*.${tmpHealOverride}) {
 		/if (${validate_cast[FALSE, alt, "Warder's Gift", ${Me.ID}]}) /call core_cast2 "Warder's Gift" alt 0 FALSE
	}

	/if (${Me.PctHPs} <= ${${Me.Class.ShortName}Point}*.${tmpHealOverride}) {
		/if (${validate_cast[FALSE, spell, "${FastHeal}", ${Me.ID}]}) /call core_cast2 "${FastHeal}" 0 ${Me.ID} FALSE
	}
	/call click_item_heal
	
/return



|***
 *	DES: If you have to ask
 *	USE: auto
 *  NOTE: 
 ***|
sub pet_heal_BST()
	DEBUG pet_heal_BST()

	/if (!${Me.Pet.ID}) /return
	AMIDEAD
	SIMPLEEXIT

	/if (${Pet.PctHPs} < 20) {
		/if (${validate_cast[FALSE, alt, "Mend Companion, "${Me.Pet.ID}]}) /call core_cast2 "Mend Companion" alt ${Me.Pet.ID} FALSE
	}
	/if (${Pet.PctHPs} < 60) {
		/if (${validate_cast[FALSE, spell, "${setPetHealSpell}", ${Me.Pet.ID}]}) /call core_cast2 "${setPetHealSpell}" 0 ${Me.Pet.ID} FALSE
	}
	/if (${Pet.PctHPs} < 80) {
		/if (${validate_cast[FALSE, spell, "${setPetRenewalSpell}", ${Me.Pet.ID}]}) /call core_cast2 "${setPetRenewalSpell}" 0 ${Me.Pet.ID} FALSE
	}

/return



|***
 *	DES: MGB beast Paragon if both AAs are ready
 *	USE: /paragon
 *  NOTE: 
 ***|
#bind cast_AA_paragon /paragon
sub Bind_cast_AA_paragon()
	/if (${Select[${Me.Class.ShortName},BST]}) /call cast_AA_paragon
/return

sub cast_AA_paragon()
	DEBUG cast_AA_paragon()
	AMIDEAD
	CHECKEXIT
	CHECKINPUTS

	/if (${Me.AltAbilityReady[Mass Group Buff]} && ${Me.AltAbilityReady[Paragon of Spirit]} && !${Bool[${Me.Song[Paragon of Spirit]}]}) {
		/if (${validate_cast[FALSE, alt, "Mass Group Buff", ${Me.ID}]}) /call core_cast2 "Mass Group Buff" alt 0 FALSE
		/if (${validate_cast[FALSE, alt, "Paragon of Spirit", ${Me.ID}]}) /call core_cast2 "Paragon of Spirit" alt 0 FALSE
		| /if (${Raid.Members}) /rs ${ParagonTXT}
	} else /if (${Bool[${Me.Song[Paragon of Spirit]}]}) {
		VOUT ${owarning} trying to cast ${csp}Paragon of Spirit\ax while its running.	
	}		
/return



|***
 *	DES: DPS Clickies
 *	USE: this shits auto, learn to love it.
 *  NOTE: 
 ***|
sub click_DPS_BST()
	DEBUG click_DPS_BST()

	AMIDEAD
	CHECKEXIT
	CHECKINPUTS
	/if (${is_target_dead[${Target2Kill}]}) /return
	/call click_horn_of_unity 	

/return



 |***
 *	DES: Beast burn routines
 *	USE: establish all comp/trigger skills
 *	NOTE: 
 ***| 
sub burn_BST()
	DEBUG burn_BST()

	AMIDEAD
	CHECKEXIT
	CHECKINPUTS
	/if (${is_target_dead[${Target2Kill}]}) /return
	
	/if (${validate_cast[FALSE, alt, "Bestial Alignment", ${Me.ID}]}) /call core_cast2 "Bestial Alignment" alt 0 FALSE
	/if (${validate_cast[FALSE, alt, "Frenzied Swipes", ${Me.ID}]}) /call core_cast2 "Frenzied Swipes" alt 0 FALSE
	/if (${validate_cast[FALSE, alt, "Ferociousness", ${Me.ID}]} && !${Me.Song[${DiscSavage}].ID}) /call core_cast2 "Ferociousness" alt 0 FALSE


	/if (${swSwarm}) {
		/if (${validate_cast[FALSE, alt, "Attack of the Warders", ${Target2Kill}]}) /call core_cast2 "Attack of the Warders" alt ${Target2Kill} FALSE
	}

	/if (${Me.Pet.ID}) {
		/if (${validate_cast[FALSE, alt, "Companion's Fury", ${Pet.ID}]}) /call core_cast2 "Companion's Fury" alt 0 FALSE
	}
	
	/if (${swCombatMelee} || ${swCombatDoT}) {
		/if (${validate_cast[FALSE, disc, "${DiscVindication}", ${Me.ID}]}) /call core_cast2 "${DiscVindication}" disc 0 FALSE
	}

	/call use_glyph


	/if (${Me.CombatAbilityReady[${Spell[${DiscRuaabri}].RankName}]} && !${Me.Song[Frenzy of Spirit].ID} && !${Me.Buff[Dichotomic Fury].ID}) {
		/if (${validate_cast[FALSE, disc, "${DiscRuaabri}", ${Me.ID}]}) /call core_cast2 "${DiscRuaabri}" disc 0 FALSE
	} else {
		/if (${Me.Song[${DiscRuaabri}].ID}) /return
		/if (${validate_cast[FALSE, alt, "Frenzy of Spirit", ${Me.ID}]}) /call core_cast2 "Frenzy of Spirit" alt 0 FALSE
		/if (${validate_cast[FALSE, alt, "BloodLust", ${Me.ID}]}) /call core_cast2 "BloodLust" alt 0 FALSE
		/call cast_AA_spire
		/if (${validate_cast[FALSE, disc, "${DiscSavage}", ${Me.ID}]} && !${Me.Song[Bestial Alignment].ID}) /call core_cast2 "${DiscSavage}" disc 0 FALSE
		/if (${validate_cast[FALSE, alt, "Group Bestial Alignment", ${Me.ID}]} && !${Me.Buff[Ferociousness].ID}) /call core_cast2 "Group Bestial Alignment" alt 0 FALSE
	}
	
	/call use_glyph
	/call click_burn_circle
	/call click_item_burn	

/return



|***
 *	DES: Class controls
 *	USE: /bst []
 *  NOTE:
 ***|
#bind setBST /bst
sub Bind_setBST(string _type, string _verbage)
	/if (!${validate_class[TRUE, |BST]}) /return

	/declare _sep bool local
	/declare _listout string local
	/declare _count int local 0
	/declare _bind string local ${Me.Class.ShortName.Lower}
  /declare _lstClass string	local |consume|fade|groupparagon|healpoint|rod|selfparagon|spiregroup|spireraid

	| spire control
	/if (${_type.Equal[spiregroup]} || ${_type.Equal[spireraid]}) {
		/call set_character_spire FALSE ${_type} ${_verbage}
		
	| set AA fade use %	
	} else /if (${_type.Equal[fade]}) {
		/call set_character_fade FALSE ${_verbage}

	| use Modulation Rods
	} else /if (${_type.Equal[rod]}) {
		/call set_switch ${chrID} swModRod ${_verbage} 	

	| healpoint
	} else /if (${_type.Equal[healpoint]}) {
		/call set_character_healpoint FALSE ${_verbage}
		/return

	| focused paragon	AA
	} else /if (${_type.Equal[selfparagon]}) {
		/if (${Bool[${_verbage}]} && ${Range.Between[1,99:${Int[${_verbage}]}]}) {
			/call sql_update FALSE ${chrID} selfParagonPct ${_verbage}
		} else /if (${Bool[${_verbage}]} && !${Range.Between[1,99:${Int[${_verbage}]}]}) {
			VOUT /${_bind} paragon [${cinfo}${selfParagonPct}\ax] allowable range ${sep} ${cinfo}1 - 99\ax
			/return
		} else {
		 	/call set_switch ${chrID} switchSelfParagon ${_verbage}  
		}
		
	| group paragon	AA
	} else /if (${_type.Equal[groupparagon]}) {
		/if (${Bool[${_verbage}]} && ${Range.Between[1,99:${Int[${_verbage}]}]}) {
			/call sql_update FALSE ${chrID} groupParagonPct ${_verbage}
		} else /if (${Bool[${_verbage}]} && !${Range.Between[1,99:${Int[${_verbage}]}]}) {
			VOUT /${_bind} paragon [${cinfo}${groupParagonPct}\ax] allowable range ${sep} ${cinfo}1 - 99\ax
			/return
		} else {
		 	/call set_switch ${chrID} switchGroupParagon ${_verbage}
		}

	| consume spirit AA
	} else /if (${_type.Equal[consume]}) {
		/if (${Bool[${_verbage}]} && ${Range.Between[1,99:${Int[${_verbage}]}]}) {
			/call sql_update FALSE ${chrID} ConsumeSpiritPct ${_verbage}
		} else /if (${Bool[${_verbage}]} && !${Range.Between[1,99:${Int[${_verbage}]}]}) {
			VOUT /${_bind} consume [${cinfo}${ConsumeSpiritPct}\ax] allowable range ${sep} ${cinfo}1 - 99\ax
			/return
		} else {
		 	/call set_switch  ${chrID} switchConsumeSpirit ${_verbag}  
		}
	}		
	
	/if (${swMQClear}) /mqclear
	/for _count 1 to ${_lstClass.Count[|]}
		/varset _listout ${_listout} ${If[${_sep},${sep},]} ${cinfo}${_lstClass.Arg[${_count},|]}\ax
		/if (${_lstClass.Arg[${_count},|].Equal[spiregroup]}) /varset _listout ${_listout}:${cnum}${spireGroup}\ax
		/if (${_lstClass.Arg[${_count},|].Equal[spireraid]}) /varset _listout ${_listout}:${cnum}${spireRaid}\ax
		/if (${_lstClass.Arg[${_count},|].Equal[healpoint]}) /varset _listout ${_listout}:${cnum}${${Me.Class.ShortName}Point}\ax
		/if (${_lstClass.Arg[${_count},|].Equal[selfparagon]}) /varset _listout ${_listout}:${If[${switchSelfParagon},${cnum}${selfParagonPct}\ax,${cbad}${selfParagonPct}\ax]}
		/if (${_lstClass.Arg[${_count},|].Equal[groupparagon]}) /varset _listout ${_listout}:${If[${switchGroupParagon},${cnum}${groupParagonPct}\ax,${cbad}${groupParagonPct}\ax]}
		/if (${_lstClass.Arg[${_count},|].Equal[consume]}) /varset _listout ${_listout}:${If[${switchConsumeSpirit},${cnum}${ConsumeSpiritPct}\ax,${cbad}${ConsumeSpiritPct}\ax]}
		/if (${_lstClass.Arg[${_count},|].Equal[rod]}) /varset _listout ${_listout}:${If[${Bool[${swModRod}]},${oon},${ooff}]}
		/if (${_lstClass.Arg[${_count},|].Equal[fade]}) /varset _listout ${_listout}:${If[${switchAAFade},${cnum}${useAAFadePct}\ax,${cbad}${useAAFadePct}\ax]}

		/varset _sep TRUE
	/next _count
	VOUT /${_bind} [${_listout}]

/return

		
		

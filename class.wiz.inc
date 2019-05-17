|***
 *	class.wiz.inc
 *	Core Wizard Include
 *
 *	
 ***|
 


|***
 *	DES: Main Wizard Routine
 *	USE: 
 *  NOTE: 
 ***|
sub core_WIZ()
	DEBUG core_WIZ()
	
	/if (${coreAuto}) {
		/if (${Me.PctMana} <= ${GatherPercent}*.${tmpHealOverride}) /call check_harvest
		/if (${swBuffSelf} && !${timer_check_runes}) /call check_rune_${Me.Class.ShortName}
	}
	
	/while (${prep_combat[]}) {
		CHECKINPUTS
		/if (${swHealSelf}) /call check_heal_self_${Me.Class.ShortName}
		/if (${swHealCure}) /call cast_cure
		/if (${exit_class_loop[]}) /return FALSE
	
		/if (${isValidTarget}) {
			/call set_combat ${combat.Upper}
			/if (${swBurnControl}) /call burn_${Me.Class.ShortName}
			/if (${swSwarm} || ${swPet}) /call send_pet
			/if (${swSwarm} && !${Me.Pet.ID}) /call cast_temp_pet ${Target2Kill} "${tempPet}"
			/if (${swAlliance}) /call trigger_alliance ${Target2Kill}
			/call check_deBuffs ${Target2Kill}
			/if (${swCombatNuke}) {
				/if (${switchForceTwincast}) /call force_twincast_${Me.Class.ShortName}
				/call cast_nuke_line
			}
			/if (${swCombatMelee}) /call use_skill_melee ${Target2Kill}
			/call disc_${Me.Class.ShortName}
			/call click_BP
			/call click_item
			/if (${swSwarm}) /call click_item_swarm ${Target2Kill}
			/call click_DPS_${Me.Class.ShortName}
		}
		
		/if (${coreAuto}) {	
			CHECKFADE
			/if (${swModRod} && ${validate_class[FALSE, ${lstClassCast}]}) /call use_mod_rod
		}	
	}	
/return TRUE



|***
 *	DES: Combat skills, Disciplines and Alternate abilities.
 *	USE: Disc
 *  NOTE: 
 ***|
sub disc_WIZ()
	DEBUG disc_WIZ()
	CHECKEXIT
	CHECKINPUTS
	CHECKTIE
	/if (${is_target_dead[${Target2Kill}]}) /return
	
	| debuff the mob
	/if (${swdeBuffCripple}) {
		/if (${validate_cast[FALSE, alt, "Lower Element", ${Target2Kill}]} && !${Target.Buff[Lower Element].ID} && !${Target.Crippled.Duration}) /call core_cast2 "Lower Element" alt ${Target2Kill} FALSE
	}

	| agro control
	/if (${swAgro}) {
	} else /if (!${swAgro}) {
		/if (${validate_cast[FALSE, alt, "Arcane Whisper", ${Target2Kill}]} && ${Me.PctAggro} > 30) /call core_cast2 "Arcane Whisper" alt ${Target2Kill} FALSE
		/if (${validate_cast[FALSE, alt, "Mind Crash", ${Target2Kill}]} && ${Me.PctAggro} > 80 && ${NameList.Find[${Spawn[${Target2Kill}].DisplayName}]}) /call core_cast2 "Mind Crash" alt ${Target2Kill} FALSE
		/if (${validate_cast[FALSE, alt, "Concussive Intuition", ${Target2Kill}]} && ${Me.PctAggro} > 80) /call core_cast2 "Concussive Intuition" alt ${Target2Kill} FALSE
		/if (${validate_cast[FALSE, spell, "${spellConcussion}", ${Target2Kill}]} && ${Me.PctAggro} > ${spellConcussionPct}) /call core_cast2 "${spellConcussion}" 0 ${Target2Kill} FALSE
	}
	
	| filler DPS. used when inbetween/down spellgem casts
	| aa Force of Flame
	/if (${lineCurrent.Equal[Fire]} && !${NoFireList.Find[${Spawn[${Target2Kill}].DisplayName}]}) {
		/if (${validate_cast[FALSE, alt, "Force of Flame", ${Target2Kill}]}) /call core_cast2 "Force of Flame" alt ${Target2Kill} FALSE
		/return
	| aa Force of Ice
	} else /if (${lineCurrent.Equal[Ice]} && !${NoColdList.Find[${Spawn[${Target2Kill}].DisplayName}]}) {
		/if (${validate_cast[FALSE, alt, "Force of Ice", ${Target2Kill}]}) /call core_cast2 "Force of Ice" alt ${Target2Kill} FALSE
		/return
	| aa Force of Will
	} else /if (${lineCurrent.Equal[Magic]} && !${NoMagicList.Find[${Spawn[${Target2Kill}].DisplayName}]}) {
		/if (${validate_cast[FALSE, alt, "Force of Will", ${Target2Kill}]}) /call core_cast2 "Force of Will" alt ${Target2Kill} FALSE
		/return
	}
/return



|***
 *	DES: summon/cast Magician DPS Clickies
 *	USE: this shits auto, learn to love it.
 *  NOTE: 
 ***|
sub click_DPS_WIZ()
	DEBUG click_DPS_WIZ()
	CHECKINPUTS
	AMIDEAD
	/call click_horn_of_unity 	
/return



|***
 *	DES: We want Mana
 *	USE: /wiz gather #
 *  NOTE: 
 ***|
sub check_harvest()
	DEBUG check_harvest()
	CHECKEXIT
	CHECKINPUTS
	CHECKTIE

	/if (${Me.Buff[Improved Twincast].ID} || ${Me.Buff[${TwincastSpell}].ID}) /return

	/if (${switchGather}) {
		/if (!${check_ready[spell, "${Spell[${GatherSpell}].RankName}"]}) {
			/if (${validate_cast[FALSE, alt, "Forceful Rejuvenation", ${Me.ID}]}) /call core_cast2 "Forceful Rejuvenation" alt 0 FALSE
			/delay 5			
		}
		/if (${Me.PctMana} < ${GatherPercent}*.${tmpHealOverride}) {
			/if (${check_ready[spell, "${Spell[${GatherSpell}].RankName}"]}) {
				/if (${validate_cast[FALSE, spell, "${GatherSpell}", ${Me.ID}]}) /call core_cast2 "${GatherSpell}" 0 0 FALSE
			}
			/if (${Me.AltAbilityReady[Harvest of Druzzil]}) {
				/if (${validate_cast[FALSE, alt, "Harvest of Druzzil", ${Me.ID}]}) /call core_cast2 "Harvest of Druzzil" alt 0 FALSE
			}
		}
	}
/return



|***
 *	DES: emergency healing
 *	USE: /call check_heal_self_${Me.Class.ShortName}
 *  NOTE: 
 ***|
sub check_heal_self_WIZ()
	DEBUG check_heal_self_WIZ()
	AMIDEAD
	CHECKEXIT
	/if (${Me.PctHPs} > ${${Me.Class.ShortName}Point}*.${tmpHealOverride}) /return
	/call click_item_heal
/return



|***
 *	DES: Keeps chanter runed. cause the're fragile and shit.
 *	USE: /call check_rune or /rb
 *  NOTE: 
 ***|
sub check_rune_WIZ()
	DEBUG check_rune_WIZ()
	AMIDEAD
	CHECKEXIT
	UPDATEHUD "BUFF Runes"
	/call timer_update timer_check_runes ${tCheck_Runes}
	/if (${validate_cast[FALSE, spell, "${runeMagiWard}", ${Me.ID}]} && ${FindItemCount[Peridot]} > 1) /call core_cast2 "${runeMagiWard}" ${buffGem} 0 FALSE TRUE
	/if (${validate_cast[FALSE, spell, "${runeShieldofFate}", ${Me.ID}]}) /call core_cast2 "${runeShieldofFate}" ${buffGem} 0 FALSE TRUE
	/if (${validate_cast[FALSE, spell, "${runeCrystalwing}", ${Me.ID}]}) /call core_cast2 "${runeCrystalwing}" ${buffGem} 0 FALSE TRUE
	/if (${validate_cast[FALSE, spell, "${runeGuard}", ${Me.ID}]}) /call core_cast2 "${runeGuard}" ${buffGem} 0 FALSE TRUE
	UPDATEHUD FALSE
/return



|***
 *	DES: Wizard burn routines
 *	USE: establish all comp/trigger skills
 *	NOTE: 
 ***| 
sub burn_WIZ()
	DEBUG burn_WIZ()
	CHECKEXIT
	CHECKINPUTS
	
	| shhhhh..
	/if (${validate_cast[FALSE, alt, "Silent Casting", ${Me.ID}]}) /call core_cast2 "Silent Casting" alt 0 FALSE

	| next 45 spells +51% crit rate
	/if (${validate_cast[FALSE, alt, "Frenzied Devastation", ${Me.ID}]}) /call core_cast2 "Frenzied Devastation" alt 0 FALSE
	| increase chance to land
	/if (${validate_cast[FALSE, alt, "Focus of Arcanum", ${Me.ID}]}) /call core_cast2 "Focus of Arcanum" alt 0 FALSE


	| alternate twincast
	/if (!${Me.Buff[${TwincastSpell}].ID}) {
		/if (${validate_cast[FALSE, alt, "Improved Twincast", ${Me.ID}]}) /call core_cast2 "Improved Twincast" alt 0 FALSE
	} else /if (!${Me.Buff[Improved Twincast].ID} && !${Me.AltAbilityReady[Improved Twincast]} && ${check_ready[spell, "${TwincastSpell}"]}) {
		/if (${validate_cast[FALSE, spell, "${TwincastSpell}", ${Me.ID}]}) /call core_cast2 "${TwincastSpell}" 0 0 FALSE
	}
	
	/call cast_AA_spire

	| boost spell type damage
	/if (${validate_cast[FALSE, alt, "Fury of the Gods", ${Me.ID}]}) /call core_cast2 "Fury of the Gods" alt 0 FALSE

	/if (${validate_cast[FALSE, alt, "Arcane Destruction", ${Me.ID}]}) /call core_cast2 "Arcane Destruction" alt 0 FALSE
	
	| increase damage
	/if (${validate_cast[FALSE, alt, "Arcane Fury", ${Me.ID}]}) /call core_cast2 "Arcane Fury" alt 0 FALSE
	
	| aww baby
	/if (${switchManaBurn}) {
		/if (${Target.ID} != ${Me.ID}) {
			/target ID ${Target2Kill}
		}
		/if (!${Target.Buff[Mana Burn].ID}) {
			/if (${validate_cast[FALSE, alt, "Mana Burn", ${Target2Kill}]}) /call core_cast2 "Mana Burn" alt ${Target2Kill} FALSE
		}
	}

	| swarm pet
	/if (${swSwarm}) {
		/if (${validate_cast[FALSE, alt, "Call of Xuzl", ${Target2Kill}]}) /call core_cast2 "Call of Xuzl" alt ${Target2Kill} FALSE
	}

	| epic 2.0
	/if (${validate_cast[FALSE, epicitem, "2", ${Me.ID}]}) /call core_cast2 "${${Me.Class.ShortName}Epic2}" item 0 FALSE

	/call use_glyph	
	/call click_burn_circle
	/call click_item_burn
	
/return



|***
 *	DES: Control for getting Twincast either spell or Proc
 *	USE: /call force_twincast_wiz
 *  NOTE:
 ***|
sub force_twincast_WIZ()
	DEBUG force_twincast_WIZ()
	/if (!${switchForceTwincast} || ${timer_force_twincast}) /return
	AMIDEAD
	CHECKEXIT
	CHECKINPUTS
	/if (${is_target_dead[${Target2Kill}]}) /return
	/if (!${timer_force_twincast}) /call timer_update timer_force_twincast ${tForce_Twincast}
	| /if (!${cast_condition[NukeTwincastCondition]}) /return
	/if (!${Me.Buff[Improved Twincast].ID} && !${Me.Buff[Twincast].ID} && ${Me.SpellReady[${Spell[${TwincastSpell}].RankName}]}) {
		/if (${validate_cast[FALSE, spell, "${TwincastSpell}", ${Me.ID}]}) /call core_cast2 "${TwincastSpell}" 0 0 FALSE
		/return
	} else /if (!${Me.Buff[Improved Twincast].ID} && !${Me.Buff[${TwincastSpell}].ID} && !${Me.Buff[Twincast].ID}) {
		/if (${validate_cast[FALSE, spell, "${NukeTwincast${lineCurrent}}", ${Target2Kill}]}) /call core_cast2 "${NukeTwincast${lineCurrent}}" 0 ${Target2Kill} FALSE
	}
/return



|***
 *	DES: Make sure you are using the AA casting boost you want..
 *	USE: /call sort_mancy_lineAA
 *  NOTE:
 ***|
sub sort_mancy_lineAA()
	DEBUG sort_mancy_lineAA()
	/if (${lineCurrent.Equal[Fire]} && ${Me.Buff[${lineAAFire}].ID}) /return TRUE
	/if (${lineCurrent.Equal[Ice]} && ${Me.Buff[${lineAAIce}].ID}) /return TRUE
	/if (${lineCurrent.Equal[Magic]} && ${Me.Buff[${lineAAMagic}].ID}) /return TRUE
	/if (${lineCurrent.Equal[Mix]}&& ${Me.Buff[${lineAAMix}].ID}) /return TRUE
	/if (${validate_cast[FALSE, alt, "${lineAA${lineCurrent}}", ${Me.ID}]}) /call core_cast2 "${lineAA${lineCurrent}}" alt 0 FALSE	
/return



|***
 *	DES: Class controls
 *	USE: /wiz []
 *  NOTE:
 ***|
#bind setWIZ /wiz
sub Bind_setWIZ(string _type, string _verbage)
	/if (!${validate_class[TRUE, |WIZ]}) /return

	/declare _sep bool local
	/declare _listout string local
	/declare _count int local 0
	/declare _bind string local ${Me.Class.ShortName.Lower}
  /declare _lstClass string local |fade|forcetwincast|gather|healpoint|line|manaburn|rod|spiregroup|spireraid

	| gather enable/disable /pct
	/if (${_type.Equal[gather]}) {
		/if (${Bool[${_verbage}]} && ${Range.Between[1,99:${Int[${_verbage}]}]}) {
			/call sql_update FALSE ${chrID} GatherPercent ${_verbage} 
		} else /if (${Bool[${_verbage}]} && !${Range.Between[1,99:${Int[${_verbage}]}]}) {
			VOUT /${_bind} gather(${cnum}${GatherPercent}\ax) allowable range ${sep} ${cinfo}1 - 99\ax
			/return
		}	else /if (!${Bool[${_verbage}]}) {
			/call set_switch ${chrID} switchGather ${_verbage]}  
			VOUT /${_bind} gather(${cnum}${GatherPercent}\ax) ${sep} ${If[${switchGather},${oenabled},${odisabled}]}
			/return
		}			

	| spell line. not quite done with this. 
	} else /if (${_type.Equal[line]}) {
		| mem a line of spells.
		/if (${lineType.Find[${_verbage.Lower}]}) {
			/call sql_update FALSE ${chrID} lineCurrent ${_verbage.Left[1].Upper}${_verbage.Right[-1].Lower}
			/if (${_verbage.Equal[off]}) {
				VOUT Casting line ${sep} ${odisabled}
				/return
			}
			/call sort_mancy_lineAA
			/delay 2
			
			/call mem_nuke_line ${lineCurrent}
			/return
		}
		/for _count 1 to ${lineType.Count[|]}
			/varset _listout ${_listout} ${If[${_sep},${sep},]} ${If[${lineCurrent.Equal[${lineType.Arg[${_count},|]}]},${cgood},${cinfo}]}${lineType.Arg[${_count},|]}\ax   
			/varset _sep TRUE
		/next _count
		VOUT /${_bind} line [${_listout}]	
		/return


	| enable/disable force twincasting
	} else /if (${_type.Equal[forcetwincast]}) {
		/call set_switch ${chrID} switchForceTwincast ${_verbage}

	| use Modulation Rods
	} else /if (${_type.Equal[rod]}) {
		/call set_switch ${chrID} swModRod ${_verbage} 	

	| set AA fade use %	
	} else /if (${_type.Equal[fade]}) {
		/call set_character_fade FALSE ${_verbage}

	| enable / disable manaburn AA
	} else /if (${_type.Equal[manaburn]}) {
		/call set_switch WIZ switchManaBurn ${_verbage}

	| spire control
	} else /if (${_type.Equal[spiregroup]} || ${_type.Equal[spireraid]}) {
		/call set_character_spire FALSE ${_type} ${_verbage}

	| healpoint
	} else /if (${_type.Equal[healpoint]}) {
		/call set_character_healpoint FALSE ${_verbage}
		/return

	}
	/if (${swMQClear}) /mqclear
	/for _count 1 to ${_lstClass.Count[|]}
		/varset _listout ${_listout} ${If[${_sep},${sep},]} ${cinfo}${_lstClass.Arg[${_count},|]}\ax
		/if (${_lstClass.Arg[${_count},|].Equal[spiregroup]}) /varset _listout ${_listout}:${cnum}${spireGroup}\ax
		/if (${_lstClass.Arg[${_count},|].Equal[spireraid]}) /varset _listout ${_listout}:${cnum}${spireRaid}\ax
		/if (${_lstClass.Arg[${_count},|].Equal[healpoint]}) /varset _listout ${_listout}:${cnum}${${Me.Class.ShortName}Point}\ax
		/if (${_lstClass.Arg[${_count},|].Equal[gather]}) /varset _listout ${_listout}:${cnum}${GatherPercent}\ax
		/if (${_lstClass.Arg[${_count},|].Equal[rod]}) /varset _listout ${_listout}:${If[${Bool[${swModRod}]},${oon},${ooff}]}
		/if (${_lstClass.Arg[${_count},|].Equal[manaburn]}) /varset _listout ${_listout}:${If[${Bool[${switchManaBurn}]},${oon},${ooff}]}
		/if (${_lstClass.Arg[${_count},|].Equal[forcetwincast]}) /varset _listout ${_listout}:${If[${Bool[${switchForceTwincast}]},${oon},${ooff}]}
		/if (${_lstClass.Arg[${_count},|].Equal[fade]}) /varset _listout ${_listout}:${If[${switchAAFade},${cnum}${useAAFadePct}\ax,${cbad}${useAAFadePct}\ax]}
		/if (${_lstClass.Arg[${_count},|].Equal[line]}) /varset _listout ${_listout}:${cnum}${lineCurrent}\ax
		/varset _sep TRUE
	/next _count
	VOUT /${_bind} [${_listout}]
/return



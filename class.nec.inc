|***
 *	class.nec.inc
 *	Core Necro Include
 *
 *
 ***|



|***
 *	DES: Main Necro Routine
 *	USE: 
 *  NOTE: 
 ***|
sub core_NEC()
	DEBUG core_NEC()


	/if (${coreAuto}) {
		/if (${Me.PctMana} <= ${GatherPercent}*.${tmpHealOverride}) /call check_bloom
		/if (!${check_engaged[]} && ${swPet}) /call check_pet
	}
	
	/while (${prep_combat[]}) {
		CHECKINPUTS
		/if (${swHealSelf}) /call patch_heal_${Me.Class.ShortName}
		/if (${swHealCure}) /call cast_cure
		/if (${exit_class_loop[]}) /return

		| /if (${swHealPet} && ${Me.Pet.ID}) /call pet_heal_${Me.Class.ShortName}
	
		/if (${isValidTarget}) {
			/call set_combat ${combat.Upper}
			/if (${swBurnControl}) /call burn_${Me.Class.ShortName}
			/if (${swSwarm} || ${swPet}) /call send_pet
			/if (${swSwarm}) /call cast_temp_pet ${Target2Kill} "${tempPet}"
			/if (!${timer_check_blood}) /call cast_blood ${Target2Kill}
			/if (${setAlliance}) /call trigger_alliance ${Target2Kill}
			/call check_deBuffs ${Target2Kill}
			/if (${swCombatNuke}) /call cast_nuke
			/if (${swCombatDoT}) /call cast_DOT
			/if (${swCombatMelee}) /call use_skill_melee ${Target2Kill}
			/call disc_${Me.Class.ShortName}
			/call click_BP
			/call click_item
			/if (${swSwarm}) /call click_item_swarm ${Target2Kill}
			/call click_DPS_${Me.Class.ShortName}
		}
		
		/if (${coreAuto}) {	
			/if (${switchAAFade} && ${Me.PctHPs} <= ${useAAFadePct}) /call cast_AA_fade
			/if (${swModRod} && ${validate_class[FALSE, ${lstClassCast}]}) /call use_mod_rod
		}	
	}	
/return



|***
 *	DES: 
 *	USE: 
 *  NOTE: 
 ***|
sub patch_heal_NEC()
	DEBUG patch_heal_NEC()
	AMIDEAD
	CHECKEXIT
	/if (${Me.PctHPs} > ${${Me.Class.ShortName}Point}*.${tmpHealOverride}) /return

	| aa Dying grasp
	/if (${Me.PctHPs} <= ${${Me.Class.ShortName}Point}*.${tmpHealOverride} && ${validate_cast[FALSE, alt, "Dying Grasp", ${Target2Kill}]}) /call core_cast2 "Dying Grasp" alt ${Target2Kill} FALSE

	/call click_item_heal
/return



|***
 *	DES: 
 *	USE: 
 *  NOTE: 
 ***|
sub disc_NEC()
	DEBUG disc_NEC()
	CHECKEXIT
	CHECKINPUTS
	CHECKTIE
	/if (${is_target_dead[${Target2Kill}]}) /return

	| aa wake the dead
	/if (${swSwarm}) {
		| wake the dead
		/if (${SpawnCount[corpse radius ${Spell[Wake the Dead].MyRange}]} > 3) { 
			/if (${validate_cast[FALSE, alt, "Wake the Dead", ${Target2Kill}]} && ${Target.PctHPs} > 15) /call core_cast2 "Wake the Dead" alt ${Target2Kill} FALSE
		}
		| aa swarm of decay
		/if (${validate_cast[FALSE, alt, "Swarm of Decay", ${Target2Kill}]}) /call core_cast2 "Swarm of Decay" alt ${Target2Kill} FALSE
		| aa Rise of bones
		/if (${validate_cast[FALSE, alt, "Rise of Bones", ${Target2Kill}]}) /call core_cast2 "Rise of Bones" alt ${Target2Kill} FALSE
	}
	
	/if (${validate_cast[FALSE, alt, "Hand of Death", ${Target2Kill}]}) /call core_cast2 "Hand of Death" alt ${Target2Kill} FALSE
	
	| epic 2.0
	/if (${validate_cast[FALSE, epicitem, "2", ${Target2Kill}]}) /call core_cast2 "${${Me.Class.ShortName}Epic2}" item ${Target2Kill} FALSE 
	
/return



|***
 *  DES: Necromancer burn routines
 *  USE: establish all comp/trigger skills
 *  NOTE: 
 ***| 
sub burn_NEC()
	DEBUG burn_NEC()
  CHECKEXIT
  CHECKINPUTS
  
	/if (${is_target_dead[${Target2Kill}]}) /return
	/if (${Me.Song[Gift of Deathly Resolve].ID}) /call cast_DOT
	/if (!${Me.Song[Gift of Deathly Resolve].ID} && ${Me.Buff[Deathly Resolve].ID}) /call cast_nuke
	/call cast_AA_spire


	| Pet Burnage
	/if (${swPet} && ${Me.Pet.ID}) {
		/if (${validate_cast[FALSE, alt, "Companion's Fury", ${Me.Pet.ID}]}) /call core_cast2 "Companion's Fury" alt ${Me.Pet.ID} FALSE
	}   

	/if (${Target.PctHPs} > 15 && ${Me.PctHPs} > 90) {
		/if (${validate_cast[FALSE, alt, "Life Burn", ${Target2Kill}]}) /call core_cast2 "Life Burn" alt ${Target2Kill} FALSE
	}

	/if (!${Me.Buff[Third Spire of Necromancy].ID}) {
		/if (${validate_cast[FALSE, alt, "Embalmer's Carapace", ${Target2Kill}]}) /call core_cast2 "Embalmer's Carapace" alt ${Target2Kill} FALSE
	}		
	
	/if (${validate_cast[FALSE, alt, "Funeral Pyre", ${Target2Kill}]}) /call core_cast2 "Funeral Pyre" alt ${Target2Kill} FALSE

	/if (${validate_cast[FALSE, alt, "Gift of Deathly Resolve", ${Me.ID}]}) /call core_cast2 "Gift of Deathly Resolve" alt 0 FALSE

	/if (${validate_cast[FALSE, alt, "Mercurial Torment", ${Me.ID}]}) /call core_cast2 "Mercurial Torment" alt 0 FALSE

	/if (${validate_cast[FALSE, alt, "Focus of Arcanum", ${Me.ID}]}) /call core_cast2 "Focus of Arcanum" alt 0 FALSE        

	/call use_glyph
	/call click_burn_circle
	/call click_item_burn
		
/return



|***
 *	DES: 
 *	USE: 
 *  NOTE: 
 ***|
sub click_DPS_NEC()
	DEBUG click_DPS_NEC()
	CHECKEXIT
	CHECKTIE
	CHECKINPUTS
	/call click_horn_of_unity 	
/return



|***
 *	DES: casts call of blood line of spells
 *	USE: /call cast_blood [ID]
 *  NOTE: 
 ***|
sub cast_blood(int _tmpID)
	DEBUG cast_blood(${_tmpID})
	CHECKEXIT
	CHECKINPUTS
	/call timer_update timer_check_blood ${tCheck_Blood}
	/if (${Bool[${Me.Song[Chaotic Power]}]}) /return
	/if (${is_target_dead[${Target2Kill}]}) /return
	/if (${validate_cast[FALSE, spell, "${spellBlood}", ${_tmpID}]}) /call core_cast2 "${spellBlood}" 0 ${_tmpID} FALSE
/return



|***
 *	DES: 
 *	USE: 
 *  NOTE: 
sub sac_toon()
	/declare sactarget string local NAMEHERE
	/if (${Spawn[pc ${sactarget}].ID}) {
		 /delay 1
		 /target id ${spawn[${sactarget}].ID}
		/delay 1
		/autoinventory
		/call MQ2Cast "Sacrifice" gem1 5s -targetID|${Spawn[${sactarget}].ID} 
		/delay 15s ${Cursor.ID}
		/autoinventory
		/delay 3s
	}
/return
 ***|
 
 
 
|***
 *	DES: We want Mana
 *	USE: /nec gather #
 *  NOTE: 
 ***|
sub check_bloom()
	DEBUG check_bloom()
	CHECKEXIT
	CHECKINPUTS
	CHECKTIE

	/if (${switchGather}) {
		/if (!${check_ready[spell, ${GatherSpell}]} && ${check_ready[alt, "Forceful Rejuvenation"]}) {
			/if (${validate_cast[FALSE, alt, "Forceful Rejuvenation", ${Me.ID}]}) /call core_cast2 "Forceful Rejuvenation" alt 0 FALSE
			/delay 5			
		}
		/if (${Me.PctMana} < ${GatherPercent}*.${tmpHealOverride}) {
			/if (${validate_cast[FALSE, spell, "${GatherSpell}", ${Me.ID}]}) /call core_cast2 "${GatherSpell}" 0 0 FALSE
			/if (${validate_cast[FALSE, alt, "Death Bloom", ${Me.ID}]}) /call core_cast2 "Death Bloom" alt 0 FALSE
		}
	}
/return
 
 

|***
 *	DES: Class controls
 *	USE: /nec []
 *  NOTE:
 ***|
#bind setNEC /nec
sub Bind_setNEC(string _type, string _verbage)
	/if (!${validate_class[TRUE, |NEC]}) /return

	/declare _sep bool local
	/declare _listout string local
	/declare _count int local 0
	/declare _bind string local ${Me.Class.ShortName.Lower}
  /declare _lstClass string local |fade|gather|healpoint|rod|spiregroup|spireraid
	
	| spire control
	/if (${_type.Equal[spiregroup]} || ${_type.Equal[spireraid]}) {
		/call set_character_spire FALSE ${_type} ${_verbage}
		
	| use Modulation Rods
	} else /if (${_type.Equal[rod]}) {
		/call sql_update FALSE ${Me.Class.ShortName} swModRod ${If[${swModRod},FALSE,TRUE]} 	

	| gather enable/disable /pct
	} /if (${_type.Find[gather]}) {
		/if (${Bool[${_classVerbage}]} && ${Range.Between[1,99:${Int[${_verbage}]}]}) {
			/call sql_update FALSE ${Me.Class.ShortName} GatherPercent ${_verbage} 
		} else /if (${Bool[${_classVerbage}]} && !${Range.Between[1,99:${Int[${_verbage}]}]}) {
			VOUT /${_bind} gather(${cnum}${GatherPercent}\ax) allowable range ${sep} ${cinfo}1 - 99\ax
			/return
		}	else /if (!${Bool[${_classVerbage}]}) {
			/call sql_update FALSE ${Me.Class.ShortName} switchGather ${If[${switchGather},FALSE,TRUE]}  
		}		

	| set AA fade use %	
	} else /if (${_type.Equal[fade]}) {
		/call set_character_fade FALSE ${_verbage}

	| healpoint
	} else /if (${_type.Find[healpoint]}) {
		/call set_character_healpoint FALSE ${_verbage}
		/return

	/if (${swMQClear}) /mqclear
	/for _count 1 to ${_lstClass.Count[|]}
		/varset _listout ${_listout} ${If[${_sep},${sep},]} ${cinfo}${_lstClass.Arg[${_count},|]}\ax
		/if (${_lstClass.Arg[${_count},|].Equal[spiregroup]}) /varset _listout ${_listout}:${cnum}${spireGroup}\ax
		/if (${_lstClass.Arg[${_count},|].Equal[spireraid]}) /varset _listout ${_listout}:${cnum}${spireRaid}\ax
		/if (${_lstClass.Arg[${_count},|].Equal[healpoint]}) /varset _listout ${_listout}:${cnum}${${Me.Class.ShortName}Point}\ax
		/if (${_lstClass.Arg[${_count},|].Equal[gather]}) /varset _listout ${_listout}:${If[${switchGather},${cnum}${GatherPercent}\ax,${ooff}]}
		/if (${_lstClass.Arg[${_count},|].Equal[rod]}) /varset _listout ${_listout}:${If[${Bool[${swModRod}]},${oon},${ooff}]}
		/if (${_lstClass.Arg[${_count},|].Equal[fade]}) /varset _listout ${_listout}:${If[${switchAAFade},${cnum}${useAAFadePct}\ax,${cbad}${useAAFadePct}\ax]}

		/varset _sep TRUE
	/next _count
	VOUT /${_bind} [${_listout}]
/return






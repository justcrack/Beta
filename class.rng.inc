|***
 *	class.rng.inc
 *	Core Ranger Include
 *
 *
 ***|
 



|***
 *	DES: Main Ranger Routine
 *	USE: 
 *  NOTE: 
 ***|
sub core_RNG()
	DEBUG core_RNG

	/while (${prep_combat[]}) {
		CHECKINPUTS
		/if (${swHealSelf}) /call check_heal_self_${Me.Class.ShortName}
		/if (${swHealCure}) /call cast_cure
		/if (${exit_class_loop[]}) /return FALSE

		/if (${isValidTarget}) {
			/call set_combat ${combat.Upper}
			/if (${swBurnControl}) /call burn_${Me.Class.ShortName}
			/if (${swSwarm} || ${swPet}) /call send_pet
			/if (${swCombatMelee}) /call use_skill_melee ${Target2Kill}
			/call disc_${Me.Class.ShortName}
			/call check_deBuffs ${Target2Kill}
			/if (${swCombatDoT}) /call cast_DOT
			/if (${swCombatNuke}) /call cast_nuke
			/if (${swAlliance}) /call trigger_alliance ${Target2Kill}
			/if (${switchNukeSummer} && ${swCombatNuke}) /call cast_nuke_summer ${Target2Kill}
			/call click_BP
			/call click_item
			/if (${swSwarm}) /call click_item_swarm ${Target2Kill}
			/call click_DPS_${Me.Class.ShortName}
		}
	
		/if (${coreAuto}) {	
			CHECKFADE
			/if (${swModRod} && ${validate_class[FALSE, ${lstClassCast}]}) /call use_mod_rod
			/if (${validate_class[FALSE, ${lstClassMelee}]}) /call cast_stamina_recovery
		}
	}
/return TRUE



|***
 *	DES: Combat skills, Disciplines and Alternate abilities.
 *	USE: /call disc_${Me.Class.ShortName}
 *  NOTE: 
 ***|
sub disc_RNG()
	DEBUG disc_RNG()
	AMIDEAD
	CHECKEXIT
	CHECKINPUTS
	/if (${is_target_dead[${Target2Kill}]}) /return
	/call set_combat ${combat.Upper}
	
	| agro control
	/if (${swAgro}) {
		/if (${validate_cast[FALSE, disc, "${DiscEnrage}", ${Target2Kill}]}) /call core_cast2 "${DiscEnrage}" disc ${Target2Kill}
	} else /if (!${swAgro}) {
		/if (${validate_cast[FALSE, disc, "${DiscJolt}", ${Target2Kill}]}) /call core_cast2 "${DiscJolt}" disc ${Target2Kill}
	}	
		
	| defensive	
	/if (${coreAuto} && ${SpawnCount[npc radius ${setCombatRadius} playerstate 4 targetable]} > 1 && !${Bool[${Me.ActiveDisc}]}) {
		/if (${validate_cast[FALSE, disc, "${AADefensive1}", ${Me.ID}]} && ${Me.PctHPs} < 60*.${tmpHealOverride}) /call core_cast2 "${AADefensive1}" disc 0
		/if (${validate_cast[FALSE, disc, "${AADefensive2}", ${Me.ID}]} && ${Me.PctHPs} < 40*.${tmpHealOverride}) /call core_cast2 "${AADefensive2}" disc 0
		/if (${validate_cast[FALSE, disc, "${AADefensive3}", ${Me.ID}]} && ${Me.PctHPs} < 20*.${tmpHealOverride}) /call core_cast2 "${AADefensive3}" disc 0

		| deflection
		/if (${validate_cast[FALSE, disc, "Weapon Shield Discipline", ${Me.ID}]} && ${Me.PctHPs} < 10*.${tmpHealOverride}) /call core_cast2 "Weapon Shield Discipline" disc 0
	}

	| elemental arrows rotation
	/if (${Me.AltAbilityReady[Elemental Arrow]}) {
		/if (!${Bool[${Target.Buff[Elemental Arrow]}]} && !${Bool[${Target.Buff[Glacial Arrow]}]} && !${Bool[${Target.Buff[Volatile Arrow]}]}) {
			/if (${validate_cast[FALSE, alt, "Elemental Arrow", ${Target2Kill}]}) /call core_cast2 "Elemental Arrow" alt ${Target2Kill}
		}
	} else /if (${Me.AltAbilityReady[Glacial Arrow]}) {
		/if (!${Bool[${Target.Buff[Elemental Arrow]}]} && !${Bool[${Target.Buff[Glacial Arrow]}]}) {
			/if (${validate_cast[FALSE, alt, "Glacial Arrow", ${Target2Kill}]}) /call core_cast2 "Glacial Arrow" alt ${Target2Kill}
		}
	}	else /if (${Me.AltAbilityReady[Volatile Arrow]}) {
		/if (!${Bool[${Target.Buff[Elemental Arrow]}]} && !${Bool[${Target.Buff[Volatile Arrow]}]}) {
			/if (${validate_cast[FALSE, alt, "Volatile Arrow", ${Target2Kill}]}) /call core_cast2 "Volatile Arrow" alt ${Target2Kill}
		}
	}		

	| reverse DS DOT
	/if (${swCombatDoT}) {
		/if (${validate_cast[FALSE, spell, "${DoTSwarmDS}", ${Target2Kill}]} && !${Bool[${Spawn[${Target.Buff[${DoTSwarmDSdeBuff}].Caster}].ID}]}) /call core_cast2 "${DoTSwarmDS}" 0 ${Target2Kill}
	}
	/if (${validate_cast[FALSE, disc, "${BladeStorm}", ${Target2Kill}]}) /call core_cast2 "${BladeStorm}" disc ${Target2Kill}
	/if (${validate_cast[FALSE, disc, "${DiscSqual}", ${Target2Kill}]}) /call core_cast2 "${DiscSqual}" disc ${Target2Kill}
	
/return TRUE



|***
 *	DES: MGB ranger Auspice if both AAs are ready
 *	USE: 
 *  NOTE: Will not cast spice if the buff is present or if your name is Norbert.
 ***|
sub cast_AA_auspice()
	DEBUG cast_AA_auspice()
	AMIDEAD
	CHECKEXIT
	CHECKINPUTS
	/if (${Me.AltAbilityReady[Mass Group Buff]} && ${Me.AltAbilityReady[${RNGauspice}]} && !${Bool[${Me.Song[${RNGauspice}]}]}) {
		/if (${validate_cast[FALSE, alt, "Mass Group Buff", ${Me.ID}]}) /call core_cast2 "Mass Group Buff" alt 0
		/if (${validate_cast[FALSE, alt, "${RNGauspice}", ${Me.ID}]}) /call core_cast2 "${RNGauspice}" alt 0
	} else /if (${Bool[${Me.Song[${RNGauspice}]}]}) {
		VOUT ${cbad}QUIT\ax trying to cast ${csp}Auspice of the Hunter\ax while its running.	
	}		
/return TRUE



|***
 *	DES: emergency healing
 *	USE: /call check_heal_self_${Me.Class.ShortName}
 *  NOTE: 
 ***|
sub check_heal_self_RNG()
	DEBUG check_heal_self_RNG()
	/if (${Me.PctHPs} > ${${Me.Class.ShortName}Point}*.${tmpHealOverride}) /return
	AMIDEAD
	CHECKEXIT
	
	| heal 1
	/if (${Me.PctHPs} <= ${RNGPoint}*.${tmpHealOverride}) {
		/if (${validate_cast[FALSE, spell, "${heal1}", ${Me.ID}]}) /call core_cast2 "${heal1}" 0 ${Me.ID}
	}
	
	| Reflexive Healing Disc
	/if (${Me.PctHPs} <= ${RNGPoint}*.${tmpHealOverride} || ${SpawnCount[npc ${If[${swOverrideLOS},,los]} radius ${Spell[${DiscHeal}].MyRange} playerstate 4 targetable]} >= 3) {
		/if (${validate_cast[FALSE, disc, "${DiscHeal}", ${Me.ID}]} && !${Bool[${Me.Song[Bladespur Reflection]}]}) /call core_cast2 "${DiscHeal}" disc ${Me.ID}
	}
	
	| heal 2
	/if (${Me.PctHPs} <= ${RNGPoint}*.${tmpHealOverride}) {
		/if (${validate_cast[FALSE, spell, "${heal2}", ${Me.ID}]}) /call core_cast2 "${heal2}" 0 ${Me.ID}
	}
	
	/call click_item_heal

/return TRUE



|***
 *	DES: DPS Clickies
 *	USE: /call click_DPS_${Me.Class.ShortName}
 *  NOTE: 
 ***|
sub click_DPS_RNG()
	DEBUG click_DPS_RNG()
	/if (${is_target_dead[${Target2Kill}]}) /return
	AMIDEAD
	CHECKEXIT
	CHECKINPUTS
	/call set_combat ${combat.Upper}
	/call use_skill_melee ${Target2Kill}
	/call click_horn_of_unity 	

	
	/if (!${Bool[${Me.ActiveDisc}]} && !${Bool[${Me.Song[Empowered Blades]}]} && !${Bool[${Me.Buff[${useGlyph}]}]}) {
		| epic 2.0
		/if (${validate_cast[FALSE, epicitem, "2", ${Me.ID}]} && ${Spawn[${Target2Kill}].Distance} <= ${setCombatDistance}) /call core_cast2 "${${Me.Class.ShortName}Epic2}" item 0
	}
	
/return TRUE



|***
 *	DES: Control for Spam Agro Nuke
 *	USE: /call cast_nuke_summer
 *  NOTE: When burn is engaged, the timer for this is ignored.
 ***|
sub cast_nuke_summer(int _tmpID)
	DEBUG cast_nuke_summer(${_tmpID})
	/if (${is_target_dead[${Target2Kill}]}) /return
	/if (${timer_cast_nuke_summer}) /return
	AMIDEAD
	CHECKEXIT
	CHECKINPUTS
	/call timer_update timer_cast_nuke_summer ${tNuke_Summer}
	/if (!${cast_condition[NukeSummerCondition]}) /return FALSE
	
	/if (${validate_cast[FALSE, spell, "${NukeSummer}", ${_tmpID}]}) {
		/call core_cast2 "${NukeSummer}" 0 ${_tmpID} cast_break
	}
/return TRUE



 |***
 *	DES: Ranger burn routines
 *	USE: establish all comp/trigger skills
 *	NOTE: 
 ***| 
sub burn_RNG()
	DEBUG burn_RNG()
	/if (${is_target_dead[${Target2Kill}]}) /return
	AMIDEAD
	CHECKEXIT
	CHECKINPUTS
	/call set_combat ${combat.Upper}

	/call cast_AA_spire

	| cheeta
	/if (${validate_cast[FALSE, alt, "${RNGcheetah}", ${Me.ID}]}) /call core_cast2 "${RNGcheetah}" alt 0 FALSE
	
	| auspice
	/if (${validate_cast[FALSE, alt, "${RNGauspice}", ${Me.ID}]}) /call core_cast2 "${RNGauspice}" alt 0 FALSE
	
	| MELEE
	/if (${swCombatMelee}) {
		/if (${Target.Distance} <= ${setCombatDistance}) {

			| weapon proc
			/if (${validate_cast[FALSE, alt, "Imbued Ferocity", ${Me.ID}]}) /call core_cast2 "Imbued Ferocity" alt 0 FALSE

			/if (!${Bool[${Me.ActiveDisc}]} && !${Bool[${Me.Buff[${useGlyph}]}]}) {
				/if (${validate_cast[FALSE, alt, "Empowered Blades", ${Me.ID}]}) /call core_cast2 "Empowered Blades" alt 0 FALSE
			} else /if (!${Bool[${Me.ActiveDisc}]} && !${Bool[${Me.Song[Empowered Blades]}]} && !${Bool[${Me.Buff[${useGlyph}]}]}) {
				/if (${validate_cast[FALSE, disc, "${DiscMele}", ${Me.ID}]}) /call core_cast2 "${DiscMele}" disc 0 FALSE
			} else /if (!${Bool[${Me.ActiveDisc}]} && !${Me.CombatAbilityReady[${DiscMele}]} && !${Me.AltAbilityReady[Empowered Blades]} && !${Bool[${Me.Song[Empowered Blades]}]}) {
				/call use_glyph
			} else /if (!${Bool[${Me.ActiveDisc}]} && !${Me.CombatAbilityReady[${DiscMele}]} && !${Me.AltAbilityReady[Empowered Blades]} && !${Bool[${Me.Song[Empowered Blades]}]} && !${Bool[${Me.Buff[${useGlyph}]}]}) {
				/if (${validate_cast[FALSE, epicitem, "2", ${Me.ID}]}) /call core_cast2 "${${Me.Class.ShortName}Epic2}" item 0 FALSE
			}
		}


	}
	
	| RANGE
	/if (${swCombatRange}) {
		/if (${Target.Distance} >= 31) {
			/if (!${Bool[${Me.ActiveDisc}]} && !${Bool[${Me.Buff[${useGlyph}]}]}) {
				/if (${validate_cast[FALSE, disc, "${DiscRange}", ${Me.ID}]}) /call core_cast2 "${DiscRange}" disc 0 FALSE
			} else /if (!${Bool[${Me.ActiveDisc}]} && !${Me.CombatAbilityReady[${DiscRange}]}) {
				/call use_glyph
			}
		}
	
	}
	
	| attack / dmg AA
	/if (${validate_cast[FALSE, alt, "Outrider's Accuracy", ${Me.ID}]}) /call core_cast2 "Outrider's Accuracy" alt 0 FALSE
	/if (${validate_cast[FALSE, alt, "Outrider's Attack", ${Me.ID}]}) /call core_cast2 "Outrider's Attack" alt 0 FALSE
	
	| guardian buff (group first)
	/if (!${Me.Song[Guardian of the Forest].ID} && ${Me.AltAbilityReady[${RNGgroupguardian}]}) {
		/if (${validate_cast[FALSE, alt, "${RNGgroupguardian}", ${Me.ID}]}) /call core_cast2 "${RNGgroupguardian}" alt 0 FALSE
	} else /if (!${Me.Song[${RNGgroupguardian}].ID} && ${Me.AltAbilityReady[Guardian of the Forest]}) {  
		/if (${validate_cast[FALSE, alt, "Guardian of the Forest", ${Me.ID}]}) /call core_cast2 "Guardian of the Forest" alt 0 FALSE
	}
		
	| swarm pets
	/if (${swSwarm}) {
		/if (${validate_cast[FALSE, alt, "Pack Hunt", ${Target2Kill}]}) /call core_cast2 "Pack Hunt" alt ${Target2Kill} FALSE   
	}
	
	| ae regen 
	/if (${validate_cast[FALSE, disc, "${DiscHeal}", ${Target2Kill}]} && !${Me.Song[Bladespur Reflection].ID}) /call core_cast2 "${DiscHeal}" disc ${Target2Kill} FALSE
			
	/call click_burn_circle
	/call click_item_burn
 /return TRUE
 	


|***
 *	DES: Class controls
 *	USE: /RNG []
 *  NOTE:
 ***|
#bind setRNG /rng
sub Bind_setRNG(string _type, string _verbage)
	/declare _sep bool local
	/declare _listout string local
	/declare _count int local 0
	/declare _bind string local ${Me.Class.ShortName.Lower}
  /declare _lstClass string local |fade|healpoint|rod|spice|spiregroup|spireraid|summer|taunt|unity

	
	| enable/disable summer line of spells
	/if (${_type.Find[summer]}) {
		/call set_switch ${chrID} switchNukeSummer ${_verbage}

	| spire control
	} else /if (${_type.Equal[spiregroup]} || ${_type.Equal[spireraid]}) {
		/call set_character_spire FALSE ${_type} ${_verbage}

	| set AA fade use %	
	} else /if (${_type.Equal[fade]}) {
		/call set_character_fade FALSE ${_verbage}

	| use Modulation Rods
	} else /if (${_type.Equal[rod]}) {
		/call set_switch ${chrID} swModRod ${_verbage} 	

	| on/off taunt
	} else /if (${_type.Equal[taunt]}) {
		/call set_switch ${chrID} taunt ${_verbage}  
		
	| mgb spice
	} else /if (${Bool[${_type.Find[spice]}]}) {
		/stopcast
		/delay 2s !${Me.Casting.ID}
		/call cast_AA_auspice
		/return

	| aa unity adjustment
	} else /if (${_type.Equal[unity]}) {
		/if (${unityType.Equal[Azia]}) {
			/call sql_update FALSE ${chrID} unityType "Beza"
		} else /if (${unityType.Equal[Beza]}) {
			/call sql_update FALSE ${chrID} unityType "Azia"
		}
		/call setDeclare RNGAAUnity "Wildstalker's Unity (${unityType})"
		
	| healpoint
	} else /if (${_type.Find[healpoint]}) {
		/call set_character_healpoint FALSE ${_verbage}
		/return

	}

	/if (${swMQClear}) /mqclear
	/for _count 1 to ${_lstClass.Count[|]}
		/varset _listout ${_listout} ${If[${_sep},${sep},]} ${cinfo}${_lstClass.Arg[${_count},|]}\ax
		/if (${_lstClass.Arg[${_count},|].Equal[fade]}) /varset _listout ${_listout}:${If[${switchAAFade},${cnum}${useAAFadePct}\ax,${cbad}${useAAFadePct}\ax]}
		/if (${_lstClass.Arg[${_count},|].Equal[healpoint]}) /varset _listout ${_listout}:${cnum}${${Me.Class.ShortName}Point}\ax
		/if (${_lstClass.Arg[${_count},|].Equal[rod]}) /varset _listout ${_listout}:${If[${Bool[${swModRod}]},${oon},${ooff}]}
		/if (${_lstClass.Arg[${_count},|].Equal[spiregroup]}) /varset _listout ${_listout}:${cnum}${spireGroup}\ax
		/if (${_lstClass.Arg[${_count},|].Equal[spireraid]}) /varset _listout ${_listout}:${cnum}${spireRaid}\ax
		/if (${_lstClass.Arg[${_count},|].Equal[summer]}) /varset _listout ${_listout}:${If[${switchNukeSummer},${oon},${ooff}]}
		/if (${_lstClass.Arg[${_count},|].Equal[taunt]}) /varset _listout ${_listout}:${If[${Bool[${taunt}]},${oon},${ooff}]}
		/if (${_lstClass.Arg[${_count},|].Equal[unity]}) /varset _listout ${_listout}:${cnum}${unityType}\ax

		/varset _sep TRUE
	/next _count
	VOUT /${_bind} [${_listout}]
/return TRUE



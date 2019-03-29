|***
 *	class.shd.inc
 *	Core Shadowknight Include
 *
 * 
 * 
 ***|



|***
 *	DES: Main Shadow Knight Routine
 *	USE: 
 *  NOTE: 
 ***|
sub core_SHD()
	DEBUG core_SHD()

	/if (${coreAuto}) {
		/if (!${check_engaged[]} && ${swPet}) /call check_pet
	}	
	
	/if (${useShieldFlash}) /call cast_AA_shield_flash

	/while (${prep_combat[]}) {
		CHECKINPUTS
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
			/call click_BP
			/call click_item
			/if (${swSwarm}) /call click_item_swarm ${Target2Kill}
			/call click_DPS_${Me.Class.ShortName}
			/if (${switchHarmonious}) /call check_harmonious
			/if (${switchSkin} && !${timer_check_Skin}) /call check_skin	
			/if (${switchStance} && (!${timer_check_Stance} || !${Bool[${Me.Buff[${Stance}]}]})) /call check_stance
		}
		/if (${coreAuto}) {	
			/if (${switchAAFade} && ${Me.PctHPs} <= ${useAAFadePct}) /call cast_AA_fade
			/if (${swModRod} && ${validate_class[FALSE, ${lstClassCast}]}) /call use_mod_rod
			/if (${validate_class[FALSE, ${lstClassMelee}]}) /call cast_stamina_recovery
		}
	}
/return TRUE



|***
 *	DES: DPS Clickies
 *	USE: this shits auto, learn to love it.
 *  NOTE: 
 ***|
sub disc_SHD()
	DEBUG disc_SHD()
	AMIDEAD
	CHECKEXIT
	CHECKINPUTS
	/if (${is_target_dead[${Target2Kill}]}) /return
	CHECKTIE

	| defensive	
	/if (${coreAuto} && ${swAgro} && ${SpawnCount[npc radius ${setCombatRadius} playerstate 4 targetable]} >= ${defCount}) {
		/declare _count int local 0
		/for _count 1 to 3
			/if (${Bool[${Me.ActiveDisc}]}) /break
			/if (${validate_cast[FALSE, activedisc, "${DefensiveDisc${_count}}", ${Me.ID}]}) /call core_cast2 "${DefensiveDisc${_count}}" disc 0 FALSE
		/next _count
	}
	/if (${useShieldFlash}) /call cast_AA_shield_flash

	| AA Voice of Thule AA
	/if (${swAgro} && !${timer_check_vot} && ${switchVoT}) {
		/if (!${Me.Buff[Voice of Thule].ID}) {	
			/if (${validate_cast[FALSE, alt, "Voice of Thule", ${Me.ID}]}) /call core_cast2 "Voice of Thule" alt ${Me.ID} FALSE
			/call timer_update timer_check_vot ${tcheck_vot}
		}
	} else /if ((!${swAgro} && ${Me.Buff[Voice of Thule].ID}) || !${switchVoT}) {
			/invoke ${Me.Buff[Voice of Thule].Remove}
	}

	| get me sum...
	/if (${swAgro}) /call get_agro_${Me.Class.ShortName}

	| tap 'dat ass..
	/if (${swHealSelf}) {
		/if (${validate_cast[FALSE, disc, "${DiscRancor}", ${Target2Kill}]} && ${Me.PctHPs} < ${${Me.Class.ShortName}Point}*.${tmpHealOverride}) /call core_cast2 "${DiscRancor}" disc ${Target2Kill} FALSE
		/if (${validate_cast[FALSE, alt, "Leech Touch", ${Target2Kill}]} && ${Me.PctHPs} < ${${Me.Class.ShortName}Point}*.${tmpHealOverride}) /call core_cast2 "Leech Touch" alt ${Target2Kill} FALSE
		/if (${SpawnCount[npc ${If[${swOverrideLOS},,los]} radius ${setCombatRadius}*2 targetable]} >= ${defCount}) {
			/if (${validate_cast[FALSE, disc, "${DiscLeech}", ${Me.ID}]} && ${Me.PctHPs} < ${${Me.Class.ShortName}Point}*.${tmpHealOverride}) /call core_cast2 "${DiscLeech}" disc 0 FALSE
		}
		/if (${validate_cast[FALSE, alt, "Vicious Bite of Chaos", ${Target2Kill}]} && ${Me.PctHPs} < ${${Me.Class.ShortName}Point}*.${tmpHealOverride}) /call core_cast2 "Vicious Bite of Chaos" alt ${Target2Kill} FALSE
	}

	| heal clickies
	/call click_item_heal

	| swarm pets
	/if (${swSwarm}) {
		/if (${validate_cast[FALSE, alt, "Chattering Bones", ${Target2Kill}]}) /call core_cast2 "Chattering Bones" alt ${Target2Kill} FALSE
	}
	
	/if (${validate_cast[FALSE, disc, "${Withstand}", ${Me.ID}]}) /call core_cast2 "${Withstand}" disc 0 FALSE
	/if (${validate_cast[FALSE, alt, "Thought Leech", ${Target2Kill}]} &&  ${Me.PctMana} <= 60*.${tmpHealOverride}) /call core_cast2 "Thought Leech" alt ${Target2Kill} FALSE
	/if (${validate_cast[FALSE, disc, "${BladeStrike}", ${Target2Kill}]}) /call core_cast2 "${BladeStrike}" disc ${Target2Kill} FALSE

/return



|***
 *	DES: take what you want. there is no try...
 *	USE: /call get_agro_${Me.Class.ShortName}
 *  NOTE: 
 ***|
sub get_agro_SHD()
	DEBUG get_agro_SHD()
	AMIDEAD
	CHECKEXIT
	CHECKINPUTS
	/if (${is_target_dead[${Target2Kill}]}) /return
	/if (${swPet}) /call send_pet
	
	/call cast_challenge ${Target2Kill}
	/if (${useShieldFlash}) /call cast_AA_shield_flash
	
	| AA pet hate
	/if (${Me.PctAggro} < 80 || ${Me.TargetOfTarget.Name.NotEqual[${Me.DisplayName}]}) {
		/if (${validate_cast[FALSE, alt, "${AAHatePet}", ${Target2Kill}]}) /call core_cast2 "${AAHatePet}" alt ${Target2Kill} FALSE
	}

	| single hate
	/if (${Me.PctAggro} < 90 || ${Me.TargetOfTarget.Name.NotEqual[${Me.DisplayName}]}) {
		/if (${validate_cast[FALSE, spell, "${Hate1}", ${Target2Kill}]}) /call core_cast2 "${Hate1}" 0 ${Target2Kill} FALSE
	}
	/if (${Me.PctAggro} < 80 || ${Me.TargetOfTarget.Name.NotEqual[${Me.DisplayName}]}) {
		/if (${validate_cast[FALSE, spell, "${Hate2}", ${Target2Kill}]}) /call core_cast2 "${Hate2}" 0 ${Target2Kill} FALSE
	}

	| AE Hate
	/if (${AEMelee}) {
		/varset mobAECount ${SpawnCount[npc ${If[${swOverrideLOS},,los]} radius 50]}
		
		/if (${mobAECount} > 1) {
			/if (${Me.AltAbilityReady[Explosion of Hatred]}) {
				/if (${validate_cast[FALSE, alt, "Explosion of Hatred", ${Me.ID}]}) /call core_cast2 "Explosion of Hatred" alt 0 FALSE
			} else /if (${Me.SpellReady[${Spell[${AEHate1}].RankName}]}) {
				/if (${validate_cast[FALSE, spell, "${AEHate1}", ${Me.ID}]}) /call core_cast2 "${AEHate1}" 0 0 FALSE
			}
		} 
		
		/if (${mobAECount} > 3) {
			/if (${Me.AltAbilityReady[Explosion of Spite]}) {
				/if (${validate_cast[FALSE, alt, "Explosion of Spite", ${Me.ID}]}) /call core_cast2 "Explosion of Spite" alt 0 FALSE
			} else /if (${Me.SpellReady[${Spell[${AEHate2}].RankName}]}) {
				/if (${validate_cast[FALSE, spell, "${AEHate2}", ${Me.ID}]}) /call core_cast2 "${AEHate2}" 0 0 FALSE
			}
		}	

		| AE lifetap	
		/if (${SpawnCount[npc ${If[${swOverrideLOS},,los]} radius ${setCombatRadius}*2 playerstate 4 targetable]} >= 2 && !${SpawnCount[pc enchanter radius ${setCombatRadius}*2]}) {
			/if (${validate_cast[FALSE, spell, "${AELifeTap}", ${Target2Kill}]}) /call core_cast2 "${AELifeTap}" 0 ${Target2Kill} FALSE
		}
	}
	
	| fake hate
	/if (${Me.PctAggro} < 90 || ${Me.TargetOfTarget.Name.NotEqual[${Me.DisplayName}]}) { 
		/if (${validate_cast[FALSE, alt, "Ageless Enmity", ${Target2Kill}]}) /call core_cast2 "Ageless Enmity" alt ${Target2Kill} FALSE
	}
	| named fake hate
	/if (${NameList.Find[${Target.DisplayName}]} && !${swOverrideName}) {
		/if (${Me.PctAggro} < 90 || ${Me.TargetOfTarget.Name.NotEqual[${Me.DisplayName}]}) {
			/if (${validate_cast[FALSE, alt, "Mindless Hatred", ${Target2Kill}]} && ${Target.Level} <= 110) /call core_cast2 "Mindless Hatred" alt ${Target2Kill} FALSE
		}
		/if (${Me.PctAggro} < 90 || ${Me.TargetOfTarget.Name.NotEqual[${Me.DisplayName}]}) { 
			/if (${validate_cast[FALSE, disc, "${DiscFakeHate}", ${Target2Kill}]}) /disc ${DiscFakeHate}
		}
	}
/return



|***
 *	DES: DPS Clickies
 *	USE: this shits auto, learn to love it.
 *  NOTE: 
 ***|
sub click_DPS_SHD()
	DEBUG click_DPS_SHD()
	AMIDEAD
	CHECKEXIT
	CHECKINPUTS
	/call click_horn_of_unity 	
/return



|***
 *	DES: keeps Shadowknigt Vizat's Skin
 *	USE: /call check_skin
 *  NOTE: 
 ***|
sub check_skin()
	DEBUG check_skin()
	AMIDEAD
	SIMPLEEXIT
	/if (${useShieldFlash}) /call cast_AA_shield_flash
	CHECKINPUTS
	/if (${is_target_dead[${Target2Kill}]}) /return	
	/if (!${Bool[${Me.Buff[${Skin}]}]} && ${Me.SpellReady[${Spell[${Skin}].RankName}]}) {
		/if (${validate_cast[FALSE, spell, "${Skin}", ${Me.ID}]}) /call core_cast2 "${Skin}" 0 0 FALSE
		/call timer_update timer_check_Skin ${tcheck_Skin}
	}
/return



 |***
 *	DES: Shadowknight burn routines
 *	USE: establish all comp/trigger skills
 *	NOTE: 
 ***| 
sub burn_SHD()
	DEBUG burn_SHD()
	AMIDEAD
	CHECKEXIT
	CHECKINPUTS
	/if (${is_target_dead[${Target2Kill}]}) /return		
	
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

	/if (${validate_cast[FALSE, alt, "T`Vyl's Resolve", ${Me.ID}]}) /call core_cast2 "T`Vyl's Resolve" alt ${Target2Kill} FALSE
	/if (${validate_cast[FALSE, disc, "${CrimsonBlade}", ${Me.ID}]}) /call core_cast2 "${CrimsonBlade}" disc 0 FALSE
	/call cast_AA_spire
	/if (${validate_cast[FALSE, alt, "Veil of Darkness", ${Me.ID}]}) /call core_cast2 "Veil of Darkness" alt ${Target2Kill} FALSE
	/if (${validate_cast[FALSE, alt, "Harm Touch", ${Me.ID}]}) /call core_cast2 "Harm Touch" alt ${Target2Kill} FALSE
	/if (${swAgro}) {
		/if (${validate_cast[FALSE, alt, "Scourge Skin", ${Me.ID}]}) /call core_cast2 "Scourge Skin" alt 0 FALSE
	}

	| AA Visage of Decay/Death stacking issues
	/if (!${Me.Buff[Visage of Decay].ID}) {
		/if (${validate_cast[FALSE, alt, "Visage of Death", ${Me.ID}]}) /call core_cast2 "Visage of Death" alt 0 FALSE
	} else /if (!${Me.AltAbilityReady[Visage of Death]} && !${Me.Buff[Visage of Death].ID}) {
		/if (${validate_cast[FALSE, alt, "Visage of Decay", ${Me.ID}]})	/call core_cast2 "Visage of Decay" alt 0 FALSE
	}
	
	| epic 2.0
	/if (${validate_cast[FALSE, epicitem, "2", ${Me.ID}]} && ${Spawn[${Target2Kill}].Distance} <= ${setCombatDistance}) /call core_cast2 "${${Me.Class.ShortName}Epic2}" item 0 FALSE

	/call use_glyph	
	/call click_burn_circle
	/call click_item_burn
/return



|***
 *	DES: Class controls
 *	USE: /shd [] [##]
 *  NOTE:
 ***|
#bind setSHD /shd
sub Bind_setSHD(string _type, string _verbage)
	/if (!${validate_class[TRUE, |SHD]}) /return

	/declare _sep bool local
	/declare _listout string local
	/declare _count int local 0
	/declare _bind string local ${Me.Class.ShortName.Lower}
  /declare _lstClass string local |ae|attraction|defcount|fade|harmonious|healpoint|rod|shieldflash|skin|spiregroup|spireraid|stance|taunt|unity|vot
	
	| Shield Flash oh shit
	/if (${_type.Equal[shieldflash]}) {
		/if (${Bool[${_verbage}]} && ${Range.Between[1,99:${_verbage}]}) {
			/call sql_update FALSE ${Me.Class.ShortName} useShieldFlashPct ${_verbage} 
		} else /if (${Bool[${_verbage}]} && !${Range.Between[1,99:${_verbage}]}) {
			VOUT /${_bind} shieldflash [${cinfo}##\ax] allowable range ${sep} ${cinfo}1 - 99\ax
			/return
		}	else /if (!${Bool[${_verbage}]}) {
			/call set_switch ${Me.Class.ShortName} useShieldFlash ${_verbage}  
		}			

	| use Modulation Rods
	} else /if (${_type.Equal[rod]}) {
		/call set_switch ${Me.Class.ShortName} swModRod ${_verbage}

	| on/off switch for Vizat's Skin
	} else /if (${_type.Find[skin]}) {
		/call set_switch ${Me.Class.ShortName} switchSkin ${_verbage} 
	
	| on/off switch for Harmonious
	} else /if (${_type.Find[harmonious]}) {
		/call set_switch ${Me.Class.ShortName} switchHarmonious ${_verbage}  

	| on/off switch for Stance
	} else /if (${_type.Find[stance]}) {
		/call set_switch ${Me.Class.ShortName} switchStance ${_verbage}  

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

	| on/off taunt
	} else /if (${_type.Equal[taunt]}) {
		/call set_switch ${Me.Class.ShortName} taunt ${_verbage}  

	| AE or Focused Melee
	} else /if (${_type.Equal[ae]}) {
		/call set_switch ${Me.Class.ShortName} AEMelee ${_verbage}

	| attraction Hate's Attraction while pulling
	} else /if (${_type.Equal[attraction]}) {
		/call set_switch ${Me.Class.ShortName} switchAttraction ${_verbage}

	| AA Voice of Thule
	} else /if (${_type.Equal[vot]}) {
		/call set_switch ${Me.Class.ShortName} switchVoT Buffs ${_verbage}

	| aa unity adjustment
	} else /if (${_type.Equal[unity]}) {
		/if (${unityType.Equal[Azia]}) {
			/call sql_update FALSE ${Me.Class.ShortName} unityType "Beza"
		} else /if (${unityType.Equal[Beza]}) {
			/call sql_update FALSE ${Me.Class.ShortName} unityType "Azia"
		}
		/call setDeclare SHDAAUnity "Dark Lord's Unity (${unityType})"


	| defensive mob agro count
	} else /if (${Bool[${_type.Equal[defcount]}]}) {
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
		/if (${_lstClass.Arg[${_count},|].Equal[ae]}) /varset _listout ${_listout}:${If[${Bool[${AEMelee}]},${oon},${ooff}]}
		/if (${_lstClass.Arg[${_count},|].Equal[attraction]}) /varset _listout ${_listout}:${If[${Bool[${switchAttraction}]},${oon},${ooff}]}
		/if (${_lstClass.Arg[${_count},|].Equal[defcount]}) /varset _listout ${_listout}:${cnum}${defCount}\ax
		/if (${_lstClass.Arg[${_count},|].Equal[fade]}) /varset _listout ${_listout}:${If[${switchAAFade},${cnum}${useAAFadePct}\ax,${cbad}${useAAFadePct}\ax]}
		/if (${_lstClass.Arg[${_count},|].Equal[harmonious]}) /varset _listout ${_listout}:${If[${Bool[${switchHarmonious}]},${oon},${ooff}]}
		/if (${_lstClass.Arg[${_count},|].Equal[healpoint]}) /varset _listout ${_listout}:${cnum}${${Me.Class.ShortName}Point}\ax
		/if (${_lstClass.Arg[${_count},|].Equal[rod]}) /varset _listout ${_listout}:${If[${Bool[${swModRod}]},${oon},${ooff}]}
		/if (${_lstClass.Arg[${_count},|].Equal[shieldflash]}) /varset _listout ${_listout}:${If[${useShieldFlash},${cnum}${useShieldFlashPct}\ax,${cbad}${useShieldFlashPct}\ax]}
		/if (${_lstClass.Arg[${_count},|].Equal[spiregroup]}) /varset _listout ${_listout}:${cnum}${spireGroup}\ax
		/if (${_lstClass.Arg[${_count},|].Equal[spireraid]}) /varset _listout ${_listout}:${cnum}${spireRaid}\ax
		/if (${_lstClass.Arg[${_count},|].Equal[skin]}) /varset _listout ${_listout}:${If[${Bool[${switchSkin}]},${oon},${ooff}]}
		/if (${_lstClass.Arg[${_count},|].Equal[stance]}) /varset _listout ${_listout}:${If[${Bool[${switchStance}]},${oon},${ooff}]}
		/if (${_lstClass.Arg[${_count},|].Equal[taunt]}) /varset _listout ${_listout}:${If[${Bool[${taunt}]},${oon},${ooff}]}
		/if (${_lstClass.Arg[${_count},|].Equal[unity]}) /varset _listout ${_listout}:${cnum}${unityType}\ax
		/if (${_lstClass.Arg[${_count},|].Equal[vot]}) /varset _listout ${_listout}:${If[${Bool[${switchVoT}]},${oon},${ooff}]}
		/varset _sep TRUE
	/next _count
	VOUT /${_bind} [${_listout}]
/return

|***
 *	class.war.inc
 *	Core Warrior Include
 *
 *	
 ***|
 


|***
 *	DES: Main Warrior Routine
 *	USE: 
 *  NOTE: 
 ***|
sub core_WAR()
	DEBUG core_WAR()
	CHECKINPUTS
	AMIDEAD
	CHECKTIE

	/while (${prep_combat[]}) {
		CHECKINPUTS
		/if (${swHealSelf}) /call patch_heal_${Me.Class.ShortName}
		/if (${swHealCure}) /call cast_cure
		/if (${exit_class_loop[]}) /return

		/if (${isValidTarget}) {
			/call set_combat ${combat.Upper}
			/if (${swBurnControl}) /call burn_${Me.Class.ShortName}
			/if (${swSwarm} || ${swPet}) /call send_pet
			/if (${swCombatMelee}) /call use_skill_melee ${Target2Kill}
			/call disc_${Me.Class.ShortName}
			/if (${swAlliance}) /call trigger_alliance ${Target2Kill}
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
/return



|***
 *	DES: 
 *	USE: 
 *  NOTE: 
 ***|
sub disc_WAR()
	DEBUG disc_WAR()
	AMIDEAD

	CHECKINPUTS
	/if (${check_exit[]}) /return
	/if (${is_target_dead[${Target2Kill}]}) /return
	
	/if (${swAgro}) /call get_agro_${Me.Class.ShortName}

	/if (${validate_cast[FALSE, alt, "${AABraceforImpact}", ${Me.ID}]}) /call core_cast2 "${AABraceforImpact}" alt 0 FALSE
	/if (${validate_cast[FALSE, alt, "${AAWarStomp}", ${Me.ID}]]}) /call core_cast2 "${AAWarStomp}" alt 0 FALSE
	/if (${validate_cast[FALSE, alt, "${AAGutPunch}", ${Target2Kill}]}) /call core_cast2 "${AAGutPunch}" alt ${Target2Kill} FALSE

	/if (${validate_cast[FALSE, alt, "${AAImpCommand}", ${Me.ID}]}) /call core_cast2 "${AAImpCommand}" alt 0 FALSE
	/if (${validate_cast[FALSE, alt, "${AAKneeStrike}", ${Target2Kill}]}) /call core_cast2 "${AAKneeStrike}" alt ${Target2Kill} FALSE
	/if (${validate_cast[FALSE, alt, "${AAMageHunter}", ${Me.ID}]]}) /call core_cast2 "${AAMageHunter}" alt 0 FALSE
	
	/if (${validate_cast[FALSE, alt, "${AAHeroicBlade}", ${Me.ID}]]} && !${swAgro}) /call core_cast2 "${AAHeroicBlade}" alt 0 FALSE
	/if (${validate_cast[FALSE, alt, "${AABladeGuardian}", ${Me.ID}]]}) /call core_cast2 "${AABladeGuardian}" alt 0 FALSE
	/if (${validate_cast[FALSE, alt, "${AAVehementRage}", ${Me.ID}]]}) /call core_cast2 "${AAVehementRage}" alt 0 FALSE

	/if (${validate_cast[FALSE, disc, "${DiscField}", ${Me.ID}]]} && !${Me.Song[${DiscField}].ID}) /call core_cast2 "${DiscField}" disc 0 FALSE
	/if (${validate_cast[FALSE, disc, "${DiscShieldBash}", ${Target2Kill}]}) /call core_cast2 "${DiscShieldBash}" disc ${Target2Kill} FALSE


	/if (${AEMelee}) {
		/if (${validate_cast[FALSE, disc, "${DiscExpanse}", ${Me.ID}]} && ${SpawnCount[npc ${If[${swOverrideLOS},,los]} radius 30 targetable]} >= 3) /call core_cast2 "${DiscExpanse}" disc 0 FALSE
		/if (${validate_cast[FALSE, disc, "${DiscWade}", ${Me.ID}]} && ${SpawnCount[npc ${If[${swOverrideLOS},,los]} radius 30 targetable]} >= 3) /call core_cast2 "${DiscWade}" disc 0 FALSE
		/if (${validate_cast[FALSE, disc, "${DiscChallenge}", ${Me.ID}]} && ${SpawnCount[npc ${If[${swOverrideLOS},,los]} radius 30 targetable]} >= 3) /call core_cast2 "${DiscChallenge}" disc 0 FALSE
	}


/return



|***
 *	DES: 
 *	USE: 
 *  NOTE: 
 ***|
sub get_agro_WAR()
	DEBUG get_agro_WAR()
	AMIDEAD

	CHECKINPUTS
	CHECKEXIT
	/if (${is_target_dead[${Target2Kill}]}) /return
	
	/if (${validate_cast[FALSE, alt, "${AARageoftheForsaken}", ${Target2Kill}]} && ${Me.TargetOfTarget.Name.NotEqual[${Me.CleanName}]}) /call core_cast2 "${AARageoftheForsaken}" alt ${Target2Kill} FALSE

	/if (${validate_cast[FALSE, disc, "${DiscInsult}", ${Target2Kill}]} && ${Me.TargetOfTarget.Name.NotEqual[${Me.CleanName}]}) /call core_cast2 "${DiscInsult}" disc ${Target2Kill} FALSE

	/if (${AEMelee}) {
		/if (${validate_cast[FALSE, alt, "${AAAETaunt}", ${Me.ID}]} && ${SpawnCount[npc ${If[${swOverrideLOS},,los]} radius 50 targetable]} > 1) /call core_cast2 "${AAAETaunt}" alt 0 FALSE
	}

	/if (${validate_cast[FALSE, disc, "${DiscCyclone}", ${Target2Kill}]} && ${Me.TargetOfTarget.Name.NotEqual[${Me.CleanName}]}) /call core_cast2 "${DiscCyclone}" disc ${Target2Kill} FALSE

	/if (${validate_cast[FALSE, disc, "${DiscShout}", ${Me.ID}]}) /call core_cast2 "${DiscShout}" disc 0 FALSE

	/if (${validate_cast[FALSE, alt, "${AABlastofAnger}", ${Target2Kill}]} && ${Me.TargetOfTarget.Name.NotEqual[${Me.CleanName}]}) /call core_cast2 "${AABlastofAnger}" alt ${Target2Kill} FALSE

/return



|***
 *	DES: Emergency Self heals
 *	USE: 
 *  NOTE: 
 ***|
sub patch_heal_WAR()
	DEBUG patch_heal_WAR()
	AMIDEAD
	CHECKEXIT
	/if (${Me.PctHPs} > ${${Me.Class.ShortName}Point}*.${tmpHealOverride}) /return

	/if (!${Raid.Members}) {
		/if (${validate_cast[FALSE, activedisc, "${DiscStoutDefense}", ${Me.ID}]]}) /call core_cast2 "${DiscStoutDefense}" disc 0 FALSE
	}
	/if (${Me.PctHPs} <= ${PanicHPPCT}*.${tmpHealOverride}) {
		/if (${validate_cast[FALSE, disc, "${DiscNoTime}", ${Me.ID}]]}) /call core_cast2 "${DiscNoTime}" disc 0 FALSE
	}
	/call click_item_heal
/return



|***
 *	DES: DPS Clickies
 *	USE: this shits auto, learn to love it.
 *  NOTE: 
 ***|
sub click_DPS_WAR()
	DEBUG click_DPS_WAR()
	AMIDEAD
	CHECKINPUTS
	/call click_horn_of_unity 	
/return



 |***
 *	DES: Warrior burn routines
 *	USE: establish all comp/trigger skills
 *	NOTE: 
 ***| 
sub burn_WAR()
	DEBUG burn_WAR()
	AMIDEAD

	CHECKINPUTS
	/if (${check_exit[]}) /return	
	
	/if (${validate_cast[FALSE, disc, "${DiscAttention}", ${Me.ID}]}) /call core_cast2 "${DiscAttention}" disc ${Target2Kill} FALSE
	/if (${validate_cast[FALSE, disc, "${DiscPhantom}", ${Me.ID}]} && ${Me.TargetOfTarget.Name.NotEqual[${Me.DisplayName}]}) /call core_cast2 "${DiscPhantom}" disc ${Target2Kill} FALSE

	/if (${Me.AltAbilityReady[${AAResplendentGlory}]} && !${Me.Song[${AAHoldtheLine}].ID}) {
		/if (${validate_cast[FALSE, alt, "${AAResplendentGlory}", ${Me.ID}]]}) /call core_cast2 "${AAResplendentGlory}" alt 0 FALSE
	} else /if (${Me.AltAbilityReady[${AAHoldtheLine}]} && !${Me.Song[${AAResplendentGlory}].ID}) {
		/if (${validate_cast[FALSE, alt, "${AAHoldtheLine}", ${Me.ID}]]}) /call core_cast2 "${AAHoldtheLine}" alt 0 FALSE
	}
	/if (${validate_cast[FALSE, alt, "${AAWarlordsTenacity}", ${Me.ID}]}) /call core_cast2 "${AAWarlordsTenacity}" alt 0 FALSE
	/if (${validate_cast[FALSE, alt, "${AAWarlordsBravery}"m ${Me.ID}]}) /call core_cast2 "${AAWarlordsBravery}" alt 0 FALSE

	/call cast_AA_spire
	/if (${validate_cast[FALSE, disc, "${DiscFlash}", ${Me.ID}]}) /call core_cast2 "${DiscFlash}" disc 0 FALSE
	/call use_glyph
	/call click_burn_circle
	/call click_item_burn
/return



|***
 *	DES: Class controls
 *	USE: /war []
 *  NOTE:
 ***|
#bind setWAR /war
sub Bind_setWAR(string _type, string _verbage)
	/if (!${validate_class[TRUE, |WAR]}) /return

	/declare _sep bool local
	/declare _listout string local
	/declare _count int local 0
	/declare _bind string local ${Me.Class.ShortName.Lower}
  /declare _lstClass string	local |ae|attraction|fade|healpoint|spiregroup|spireraid|taunt

	| spire control
	/if (${_type.Equal[spiregroup]} || ${_type.Equal[spireraid]}) {
		/call set_character_spire FALSE ${_type} ${_verbage}

	| healpoint
	} else /if (${_type.Find[healpoint]}) {
		/call set_character_healpoint FALSE ${_verbage}
		/return

	| set AA fade use %	
	} else /if (${_type.Equal[fade]}) {
		/call set_character_fade FALSE ${_verbage}

	| on/off taunt
	} else /if (${_type.Equal[taunt]}) {
		/call set_switch ${Me.Class.ShortName} taunt ${_verbage}

	| attraction Warlord's Grasp while pulling
	} else /if (${_type.Equal[attraction]}) {
		/call set_switch ${Me.Class.ShortName} switchAttraction ${_verbage}

	| panic heal pct
	} else /if (${_type.Find[panicheal]}) {
		/if (${Range.Between[1,99:${Int[${_verbage}]}]}) {
			/call sql_update FALSE ${Me.Class.ShortName} PanicHPPCT ${_verbage}
		} else /if (!${Range.Between[1,99:${Int[${_verbage}]}]}) {
			VOUT /${_bind} panicheal [${cinfo}${${PanicHPPCT}\ax] allowable range ${sep} ${cinfo}1 - 99\ax
			/return
		}
		
	| AE or Focused Melee
	} else /if (${_type.Equal[ae]}) {
		/call set_switch ${Me.Class.ShortName} AEMelee ${_verbage}

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
		/if (${_lstClass.Arg[${_count},|].Equal[attraction]}) /varset _listout ${_listout}:${If[${Bool[${switchAttraction}]},${oon},${ooff}]}
		/if (${_lstClass.Arg[${_count},|].Equal[spiregroup]}) /varset _listout ${_listout}:${cnum}${spireGroup}\ax
		/if (${_lstClass.Arg[${_count},|].Equal[spireraid]}) /varset _listout ${_listout}:${cnum}${spireRaid}\ax
		/if (${_lstClass.Arg[${_count},|].Equal[healpoint]}) /varset _listout ${_listout}:${cnum}${${Me.Class.ShortName}Point}\ax
		/if (${_lstClass.Arg[${_count},|].Equal[panicheal]}) /varset _listout ${_listout}:${cnum}${PanicHPPCT}\ax
		/if (${_lstClass.Arg[${_count},|].Equal[defcount]}) /varset _listout ${_listout}:${cnum}${defCount}\ax
		/if (${_lstClass.Arg[${_count},|].Equal[taunt]}) /varset _listout ${_listout}:${If[${Bool[${taunt}]},${oon},${ooff}]}
		/if (${_lstClass.Arg[${_count},|].Equal[fade]}) /varset _listout ${_listout}:${If[${switchAAFade},${cnum}${useAAFadePct}\ax,${cbad}${useAAFadePct}\ax]}
		/if (${_lstClass.Arg[${_count},|].Equal[ae]}) /varset _listout ${_listout}:${If[${AEMelee},${oon},${ooff}]}

		/varset _sep TRUE
	/next _count
	VOUT /${_bind} [${_listout}]
/return
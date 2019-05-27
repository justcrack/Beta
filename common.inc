|***
 *	common.inc
 *	Core shared Routines/Functions
 *
 *
 ***|




|***
 *	DES: echos misc stuff to the MQ Window
 *	USE: /call echos [type] "desc text" [VARIABLE] [Target ID] ["text2"]
 *  NOTE: 
 ***|
sub echos(string _type, string _verbage, string _tmpVariable, int _tmpID, string _verbage2)

	| on/off switch echos
	/if (${_type.Equal[switch]}) {
		VOUT ${_verbage} ${sep} ${If[${${_tmpVariable}},${oenabled},${odisabled}]}

	| control switches
	} else /if (${_type.Equal[setswitch]}) {
		VOUT ${_verbage} ${_verbage2} ${If[${Bool[${_tmpID}]},[\a-w${_tmpID}\ax] ,]}${sep} ${If[${${_tmpVariable}},${oenabled},${odisabled}]}

	| variable %s
	} else /if (${_type.Equal[number]}) {
		VOUT ${_verbage} ${sep} ${cinfo}${_tmpVariable}\ax

	| core Initialize
	} else /if (${_type.Equal[initialize]}) {
		OUT Initialize ${sep} ${cinfo}${currentBuild}\ax

	| zone initialization
	} else /if (${_type.Equal[zone]}) {
		VOUT Zone ${sep} ${cinfo}${Zone.Name}\ax

	| ending macro
	} else /if (${_type.Equal[ending]}) {
		VOUT ${owarning} ${sep} ${cbad}ENDING\ax Build ${sep} ${cinfo}${currentBuild}\ax

	| need target
	} else /if (${_type.Equal[needtarget]}) {
		VOUT ${onotice} ${sep} Bad target.

	| need cursor item
	} else /if (${_type.Equal[needcursor]}) {
		VOUT ${onotice} ${sep} need something on cursor.

	| cursor item
	} else /if (${_type.Equal[cursor]}) {
		VOUT Cursor ${sep} ${cinfo}${_verbage}\ax ${sep} ${_tmpVariable}

	| hunt
	} else /if (${_type.Equal[hunt]}) {
		VOUT Hunt ${sep} [${ctar}${Spawn[${_tmpID}].DisplayName}\ax] ${sep} ${If[${Spawn[${_tmpID}].LineOfSight},${cgood}${Int[${Spawn[${_tmpID}].Distance}]}\ax,${cbad}${Int[${Spawn[${_tmpID}].Distance}]}\ax]}

	| mode switches
	} else /if (${_type.Equal[mode]}) {
		VOUT Mode ${sep} ${cinfo}${_verbage}\ax ${sep} ${If[${${_tmpVariable}},${oenabled},${odisabled}]}

	| petfarm
	} else /if (${_type.Equal[petfarm]}) {
		VOUT Pet ${sep} [${ctar}${Spawn[${Target2Kill}].DisplayName}\ax] ${sep} ${If[${Spawn[${Target2Kill}].LineOfSight},${cgood}${Spawn[${Target2Kill}].Distance}\ax,${cbad}${Spawn[${Target2Kill}].Distance}\ax]}

	| pull
	| } else /if (${_type.Equal[pull]} && ${Target2Kill}) {
	| 	VOUT Pull ${sep} [${ctar}${Spawn[${Target2Kill}].DisplayName}\ax] ${sep} ${If[${Spawn[${Target}].LineOfSight},${cgood}${Int[${Target.Distance}]}\ax,${cbad}${Int[${Target.Distance}]}\ax]}

	| grab (offtanks)
	| } else /if (${_type.Equal[grab]} && ${Target2Kill}) {
	| 	VOUT Grab ${sep} [${ctar}${Spawn[${Target2Kill}].DisplayName}\ax] ${sep} ${If[${Spawn[${Target}].LineOfSight},${cgood}${Int[${Target.Distance}]}\ax,${cbad}${Int[${Target.Distance}]}\ax]}
 
 	| you suck
	} else /if (${_type.Equal[failboat]}) {
		/if (${swBitch}) {
			/call echo_bitch MQWindow
		} else {
			VOUT ${cbad}Error\ax in command.
		}

	| info
	} else /if (${_type.Equal[wiki]}) {
		VOUT WIKI ${sep} ${cinfo}https://github.com/exspes007/core/wiki\ax

	| override switches
	}	else /if (${_type.Equal[override]}) {
		VOUT Override ${sep} ${cinfo}${_verbage}\ax ${sep} ${If[${${_tmpVariable}},${oenabled},${odisabled}]}

	| debuff switches
	}	else /if (${_type.Equal[debuff]}) {
		VOUT deBuff ${sep} ${cinfo}${_verbage}\ax ${sep} ${If[${${_tmpVariable}},${oenabled},${odisabled}]}

	| heal switches
	}	else /if (${_type.Equal[heal]}) {
		VOUT Heal ${sep} ${cinfo}${_verbage}\ax ${sep} ${If[${${_tmpVariable}},${oenabled},${odisabled}]}

	| buff echos
	}	else /if (${_type.Equal[buff]}) {
		VOUT Buff ${sep} ${cinfo}${_verbage}\ax ${sep} ${If[${${_tmpVariable}},${oenabled},${odisabled}]}

	| rez echos
	}	else /if (${_type.Equal[rez]}) {
		VOUT Rez ${sep} ${cinfo}${_verbage}\ax ${sep} ${If[${${_tmpVariable}},${oenabled},${odisabled}]}

	| arg list echos
	} else /if (${_type.Equal[listtype]}) {
		
		/declare _listout string local
		/declare _sep bool local FALSE
		/declare _count int local 0
		/for _count 1 to ${${_tmpVariable}Type.Count[|]}
			/varset _listout ${_listout} ${If[${_sep},${sep},]} ${cinfo}${${_tmpVariable}Type.Arg[${_count},|]}\ax
			/varset _sep TRUE
		/next _count
		VOUT /${_verbage.Lower} [${_listout}]
	}
/return



|***
 *	DES: Check to see if toon is engaged in any way.
 *	USE: /call check_engaged
 *  NOTE: 
 ***|
sub check_engaged()
	CHECKINPUTS
	| rogue SOS .. 
	/if (${Select[${Me.Class.ShortName},ROG]}) {
		/if (${coreAuto} && ${swSoS}) /return FALSE
	}
	
	| invis
	/if (${Me.Invis}) /return TRUE
	
	| spawn count
	/if (!${swBuffCombat}) {
		/if (${SpawnCount[npc ${If[${swOverrideLOS},,los]} radius ${setMobAgro} playerstate 4]}) /return TRUE
	}

	| spell cast'in. songs as well
	/if (${Me.Casting.ID}) /return TRUE

	| movement
	/if (${Me.Moving} && !${Select[${Me.Class.ShortName},BRD]}) /return TRUE
	| /if (${Stick.Active}) /return TRUE
	
	| combat
	/if (${Me.Combat}) /return TRUE
	/if (${Me.AutoFire}) /return TRUE
	
	| am i the puller or main agro with mobs in range?
	/if (${swPull} && ${swAgro}) {
		/if (${SpawnCount[npc ${If[${swOverrideLOS},,los]} radius ${setCombatDistance}*2 playerstate 4]}) /return TRUE
	}
	/if (${Group.MainAssist.Name.Equal[${Me.DisplayName}]}) {
		/if (${SpawnCount[npc ${If[${swOverrideLOS},,los]} radius ${setCombatDistance}*2 playerstate 4]}) /return TRUE
	}

/return FALSE



|***
 *	DES: give weapons to pet
 *	USE: /call give_to_pet
 *  NOTE: 
 ***|
sub give_to_pet()
	/if (${Target.ID} != ${Me.Pet.ID}) {
		/squelch /target ID ${Me.Pet.ID}
		/delay 1s ${Target.ID} == ${Me.Pet.ID}
	}
	:loopgiveitem
	AMIDEAD
	CHECKINPUTS
	/delay 5 ${Cursor.ID}
	/if (${Cursor.ID} && ${Me.Pet.ID}) {
		/if (${Cursor.Name.Find[summoned:]}) {
			/click left target
			/delay 5s !${Bool[${Cursor.ID}]}
		} else {
			CLEARCURSOR TRUE
		}
		/goto :loopgiveitem
	}
/return



|***
 *	DES: Summon pet. I mean, it won't summon cheeze
 *	USE: /call cast_summon_pet
 *  NOTE: 
 ***|
sub cast_summon_pet()
	AMIDEAD
	CHECKEXIT
	CHECKINPUTS

	/if (${swPet} && ${coreAuto} && !${Me.Pet.ID}) {
		/if (${validate_cast[FALSE, alt, "Suspended Minion", ${Me.ID}]}) {
			/call core_cast2 "Suspended Minion" alt 0 FALSE
		}
	}
	
	| component check
	/if (${swPet} && !${Me.Pet.ID}) {
		/declare _error bool local FALSE
		/if (${validate_class[FALSE, |ENC]} && !${FindItem[=Tiny Dagger].ID}) /varset _error TRUE
		/if (${validate_class[FALSE, |MAG]} && !${FindItem[=Malachite].ID}) /varset _error TRUE
		/if (${validate_class[FALSE, |NEC|SHD]} && !${FindItem[=Bone Chips].ID}) /varset _error TRUE
		
		/if (${_error}) {
			OUT Missing pet component. 
			OUT Shutting pets off 'till you go buy some.
			OUT Don't forget to turn back on.
			/call set_switch pet swPet FALSE
			/return
		}
	}

	VOUT Building ${sep} ${csp}${setPetType}\ax
	/if (!${Me.Pet.ID}) {
		/if (${validate_cast[FALSE, spell, "${setPetType}", ${Me.ID}]}) {
			/call core_cast2 "${setPetType}" ${buffGem} 0 FALSE
			/call timer_update timer_check_buffs_pet 20
		}
		/delay 20s !${Me.Casting.ID}
		/pet hold on
		/pet ghold on
	}
	
	/if (${coreAuto} && ${swPet}) { 
		/if (${Bool[${swPetTank}]}) { 
			/pet taunt on
		} else /if (!${Bool[${swPetTank}]}) { 
			/pet taunt off
		}
	}
	/call send_pet
/return



|***
 *	DES: Shrinks pet.. wtf did you expect..
 *	USE: /call cast_shrink_pet
 *  NOTE: 
 ***|
sub cast_shrink_pet()
	AMIDEAD
	CHECKEXIT
	CHECKINPUTS

	/if (${check_engaged[]}) /return
	/if (${Me.Pet.ID} && ${Bool[${setPetShrink}]} && ${Me.Pet.Height} > 1.40) {
		/if (${Me.Book[${Spell[${setPetShrink}].RankName}]}) {
			/if (${validate_cast[FALSE, spell, "${setPetShrink}", ${Me.Pet.ID}]}) /call core_cast2 "${setPetShrink}" ${buffGem} ${Me.Pet.ID} FALSE
		} else /if (${Me.ItemReady[${setPetShrink}]}) {
			/if (${validate_cast[FALSE, item, "${setPetShrink}", ${Me.Pet.ID}]}) /call core_cast2 "${setPetShrink}" item ${Me.Pet.ID} FALSE
		} else /if (${Me.AltAbilityReady[${setPetShrink}]}) {
			/if (${validate_cast[FALSE, alt, "${setPetShrink}", ${Me.Pet.ID}]}) /call core_cast2 "${setPetShrink}" alt ${Me.Pet.ID} FALSE
		}
	}
/return



|***
 *	DES: fixes pet for.. umm.. pet stuff.. yah that..
 *	USE: /call check_pet
 *  NOTE: chanter pets sword ID=10855
 ***|
sub check_pet()
	AMIDEAD
	CHECKEXIT

	/if (!${Me.Pet.ID}) /call cast_summon_pet
	/if (${swBuffPet} && !${timer_check_buffs_pet}) /call check_buffs_pet
	/if (${Me.Pet.ID} && ${swMAGWep} && (!${Bool[${Me.Pet.Equipment[primary]}]} || (!${Bool[${Me.Pet.Equipment[offhand]}]} && !${Select[${Me.Class.ShortName},DRU]}) || ${Me.Pet.Equipment[primary]} == 10855)) {
		/if (!${Select[${Me.Class.ShortName},MAG]} && !${Bool[${FindItem[${setMAGClosedBag}]}]}) {
			/call tell_resident_mage weapons
		} else /if (${Select[${Me.Class.ShortName},MAG]} && !${Bool[${FindItem[${setMAGClosedBag}]}]}) {
			/call cast_summon_weapons
		}
	}	

	CHECKTIE
	CHECKINPUTS
	/if (${Me.Pet.ID} && ${swMAGWep} && (!${Bool[${Me.Pet.Equipment[primary]}]} || ${Me.Pet.Equipment[primary]} == 10855) && (${Bool[${FindItem[${setMAGClosedBag}]}]} || ${Bool[${FindItem[Pouch of Quellious]}]})) {
		CLEARCURSOR TRUE
		/if (${Bool[${FindItem[${setMAGClosedBag}]}]} || ${Bool[${FindItem[Pouch of Quellious]}]}) /call cast_summon_weapons
	}
		
/return



|***
 *	DES: Summon and equip pet weapons
 *	USE: 
 *  NOTE: MAG,ENG,BST,NEC
 ***|
sub cast_summon_weapons(int _tmpID)
	CHECKTIE
	AMIDEAD
	CHECKINPUTS

	/declare _count int local 0
	
	/if (!${Me.Pet.ID}) /return
 	/if (${Select[${Me.Class.ShortName},MAG]} && !${FindItem[${setMAGClosedBag}].ID} && !${FindItem[${setMAGPetWepName}].ID}) {
 		/if (${validate_cast[FALSE, spell, "${setMAGWepSpell}", ${Me.ID}]}) /call core_cast2 "${setMAGWepSpell}" ${buffGem} ${Me.ID} FALSE
 		/delay 20s !${Me.Casting.ID}
 		/delay 5
 		CLEARCURSOR TRUE
 	}
	
	/delay 5
	
	/if (${FindItem[${setMAGClosedBag}].ID}) {
		AMIDEAD
		/while (${Cursor.ID}) {
			/autoinventory
			/delay 1s !${Cursor.ID}
		}
		| /call Bind_swap "${FindItem[${setMAGClosedBag}].ID}" ${setPetBagSlot}
		/squelch /exchange ${FindItem[${setMAGClosedBag}].ID} pack${setPetBagSlot}
		CLEARCURSOR TRUE
		/delay 2s !${Cursor.ID}
		/delay 5
		/if (${validate_cast[FALSE, item, "${setMAGClosedBag}", ${Me.ID}]}) /call core_cast2 "${setMAGClosedBag}" item 0 FALSE
		/delay 5
		/delay 5s !${Me.Casting.ID}
		CLEARCURSOR TRUE		
		/delay 5

		/if (${Bool[${FindItem[Pouch of Quellious]}]}) {
			AMIDEAD
			/nomodkey /itemnotify ${FindItem[Pouch of Quellious].InvSlot} rightmouseup
			/delay 1s
			/for _count 1 to 2
				/if (${Bool[${FindItem[${setMAGPetWepName}]}]}) {
					/nomodkey /itemnotify ${FindItem[${setMAGPetWepName}].InvSlot} leftmouseup
					/delay 3s ${Bool[${Cursor.ID}]}
					/delay 1s
					/call give_to_pet
					/delay 3s !${Bool[${Cursor.ID}]}
					/delay 1s
					/if (${Select[${Me.Class.ShortName},DRU]}) /break
				}
			/next _count
			/if (${Window[GiveWnd].Open}) {
				/nomodkey /notify GiveWnd GVW_Give_Button leftmouseup
				/keypress ESC
				/keypress ESC
			}

			/while (${Bool[${FindItem[Pouch of Quellious]}]}) {
				/delay 1s
				/nomodkey /itemnotify ${FindItem[=Pouch of Quellious].InvSlot} leftmouseup
				/delay 1s ${Cursor.Name.Equal["Pouch of Quellious"]}
				/delay 1s
				/if (${Cursor.Name.Equal["Pouch of Quellious"]}) /destroy
			}
		}
		
		| cose the window
		/while (${Window[InventoryWindow].Open}) {
			AMIDEAD
			/nomodkey /keypress inventory
			/delay 1s !${Window[InventoryWindow].Open}
		}

	}
/return



|***
 *	DES: keep exp at a specified level then max aa
 *	USE: 
 *  NOTE: 
 ***|
sub auto_adjust_EXP()
	CHECKTIE
	AMIDEAD
	CHECKEXIT
	CHECKINPUTS

	/call timer_update timer_check_EXP ${tCheck_EXP}
	
	| set EXP cause we deaded too much
	/if (${Me.Level} <= ${setLvlMax} && ${Me.PctExp} <= ${setLvlPct}) {
		/if (${swDispEXP}) VOUT ${ocaution} ${sep} ${cinfo}${Me.PctExp}%\ax below Maintain EXP [${cinfo}${setLvlMax}\ax] ${sep} ${cinfo}${setLvlPct}%\ax
		/alternateadv off
	| set to AA cause the group healer is awesom	
	} else /if (${Me.Level} == ${setLvlMax} && ${Me.PctExp} >= ${setLvlPct} && ${Me.AAPoints} < ${Float[${Math.Calc[${Me.Level}*2]}].Int}) {
		| VOUT Maintain EXP [${cinfo}${setLvlMax}\ax] ${sep} ${cinfo}${setLvlPct}%\ax. AA:${cinfo}${Me.AAPoints} < ${Float[${Math.Calc[${Me.Level}*2]}].Int}\ax 
		/alternateadv on 100
	| set back to exp cause nothing else to buy
	} else /if (${Me.Level} == ${setLvlMax} && ${Me.PctExp} >= ${setLvlPct} && (${Me.AAPoints} == ${Float[${Math.Calc[${Me.Level}*2]}].Int})) {
		/alternateadv off
	}
/return



|***
 *	DES: Check for running auras, If they are duped, remove the second one
 *	USE: /call check_aura
 *  NOTE: 
 ***|
sub check_aura()
	CHECKCOMBATCAST
	CHECKTIE
	AMIDEAD
	CHECKEXIT
	CHECKINPUTS

	/call timer_update timer_check_aura ${tCheck_Aura}
	UPDATEHUD "BUFF Aura"
	
	| Aura: berzerker, monk, warrior
	/if (${validate_class[FALSE, |BER|MNK|WAR]}) {
		/if (${Bool[${Aura1Buff}]} && !${Bool[${Me.Aura[1].Name.Equal[${Aura1Buff}]}]}) {
			VOUT Disc ${sep} ${csp}${Aura1Spell}\ax
			/disc ${Aura1Spell}
			/delay 8s
		}
		/return
	}
		
	| Aura: Bard
	/if (${validate_class[FALSE, |BRD]}) {
		/if (${Bool[${Aura1Buff}]} && !${Bool[${Me.Aura[1].Name.Equal[${Aura1Buff}]}]}) {
				/if (${validate_cast[TRUE, pulse, "${Aura1Buff}", ${Me.ID}]}) /call core_cast2 "${Aura1Buff}" pulse 0 FALSE
		}
		/return
	}

	| everyone else...
	/declare _count int local 0
	/declare _aura int local 0
	/declare _found bool FALSE

	/for _aura 1 to 2
		CHECKCOMBATCAST
		/if (${Bool[${Aura${_aura}Spell}]}) {
			/for _count 1 to 2	
				CHECKCOMBATCAST
				/if (${Bool[${Aura${_aura}Buff}]}) {
					
					/if (${Me.Aura[${_count}].Name.Equal[${Aura${_aura}Buff}]}) {
						/varset _found TRUE
						/continue
					}
					/if (${Me.Aura[${_count}].Name.Equal[${Spell[${Aura${_aura}Spell}].RankName}]}) {
						/varset _found TRUE
						/continue
					}

				}
			/next _count
			
			/if (!${_found}) {
				/if (${validate_cast[FALSE, spell, "${Aura${_aura}Spell}", ${Me.ID}]}) /call core_cast2 "${Aura${_aura}Spell}" ${buffGem} ${Me.ID} FALSE
			} else /if (${_found}) {
				/varset _found FALSE
			}
		
		}
	/next _aura
	
	UPDATEHUD FALSE
/return



|***
 *	DES: Checks current item buffs. if one is missing. fix it.
 *	USE: /ib and auto
 *  NOTE: 
 ***|
sub check_buffs_item()
	CHECKTIE
	AMIDEAD
	CHECKEXIT
	UPDATEHUD "BUFF Item"
	/declare _count int	local 0
	
	| check clickItemBuff
	/for _count 1 to 6
		/if (${validate_cast[FALSE, item, "${clickItemBuff${_count}}", ${Me.ID}]}) /call core_cast2 "${clickItemBuff${_count}}" item 0 FALSE
	/next _count
	
  /if (${Me.ItemReady[${setIllusionItem}]} && !${Bool[${Me.Buff[${setIllusionBuff}]}]} && ${Spell[${setIllusionBuff}].NewStacks}) {
  	/call core_cast2 "${setIllusionItem}" item 0 FALSE
		| VOUT Item ${sep} ${citem}${setIllusionItem}\ax
 		| /call MQ2Cast "${setIllusionItem}" item 30s
 	}  	
 	
 	| check Items
	/if (${Bool[${ItemNow1}]} || ${Bool[${ItemNow2}]}) {
		/call check_item_now
	}

	/if (!${Bool[${timer_Check_Familiar}]}) /call sort_familiar
 	
	/call remove_unwanted_buffs	
	/if (${coreAuto}) {
		/if (${switchShrink}) /call check_shrink
		/call timer_update timer_check_buffs_item ${tItem_Buff}
	}
	UPDATEHUD FALSE
/return



|***
 *	DES: Checks current buffs. NOT SONGS. IF one is found to match the INI listing per character, it is removed.
 *	USE: /call remove_unwanted_buffs
 *  NOTE: 
***|
sub remove_unwanted_buffs()
	CHECKINPUTS
	AMIDEAD
	CHECKEXIT
	
	/declare _count int local 0
	/for _count 1 to ${lstBuffRemove.Count[|]}
		AMIDEAD
		/if (${Me.Buff[${lstBuffRemove.Arg[${_count},|]}].ID})  {
			VOUT Buff ${sep} ${cinfo}${lstBuffRemove.Arg[${_count},|]}\ax ${sep} ${oremove}
			/invoke ${Me.Buff[${lstBuffRemove.Arg[${_count},|]}].Remove}
		}
	/next _count		
/return



|***
 *	DES: Chekcs self buffs. If somethings missing, mem and cast it.
 *	USE: /sb and/or auto
 *  NOTE: 
 ***|
sub check_buffs_self()
	/if (${coreAuto}) {
		/if (!${timer_check_buffs_self}) /call timer_update timer_check_buffs_self ${tSelf_Buff}
		/if (${watch_for_death[]}) /return 
	}
	/if (!${Me.FreeBuffSlots}) /return
	/declare _count int local 0
	/declare _i int local 0
	/declare _found bool local TRUE

	| check Unity AA buffs
	/if (${Bool[${${Me.Class.ShortName}AAUnity}]}) {
		UPDATEHUD "BUFF AA Unity"
		/for _count 1 to 7
			/if (!${Bool[${BuffAAUnity${_count}}]}) /continue
			/if (!${Me.AltAbilityReady[${${Me.Class.ShortName}AAUnity}]}) /break
			/if (${Me.CurrentMana} < ${Spell[${${Me.Class.ShortName}AAUnity}].Mana}) /break
			/if (!${Bool[${BuffAAUnity${_count}}]}) /continue
			/if (!${Spell[${BuffAAUnity${_count}}].NewStacks}) /continue
			/if (!${Bool[${Me.Buff[${BuffAAUnity${_count}}].Duration}]}) {
				/if (${validate_cast[FALSE, alt, "${${Me.Class.ShortName}AAUnity}", ${Me.ID}]}) /call core_cast2 "${${Me.Class.ShortName}AAUnity}" alt 0 FALSE
				/break
			}
		/next _count
	}
	
	| check self buffs
	/for _count 1 to ${setMaxBuffCount}
		CHECKCOMBATCAST
		UPDATEHUD "BUFF Self"
		
		| is theere a buff there?
		/if (!${Bool[${Buff${_count}}]}) /continue		

		| self buff ?
		/if (!${Bool[${Buff${_count}self}]}) /continue		

		AMIDEAD
		CHECKEXIT
		CHECKTIE
		CHECKINPUTS

		| stacking / exist ?
		/if (${Spell[${Buff${_count}}].HasSPA[374]} || ${Spell[${Buff${_count}}].HasSPA[340]}) {
			
			/for _i 1 to ${Spell[${Buff${_count}}].NumEffects}
				/if (!${Bool[${Spell[${Spell[${Buff${_count}}].Base2[${_i}]}].Duration}]}) /continue
				
				| do we have the triggered buff?
				/if (!${Me.Buff[${Spell[${Spell[${Buff${_count}}].Base2[${_i}]}].Name}].Duration}) /varset _found FALSE
				| does it stack
				/if (!${Spell[${Spell[${Buff${_count}}].Base2[${_i}]}].NewStacks}) /varset _found TRUE

			/next _i

		} else {

			| is it there?
			/if (!${Bool[${Me.Buff[${Buff${_count}}].Duration}]}) /varset _found FALSE

			| stacks?
			/if (!${Spell[${Buff${_count}}].NewStacks}) /varset _found TRUE		
			
		}
		
		/if (${_found}) /continue

		| fail the condition?
		| /if (!${cast_condition[Buff${_count}selfCondition]}) /return		

		/if (${Cursor.ID}) CLEARCURSOR TRUE

		/if (${Select[${Me.Class.ShortName},BRD]} && ${Me.Casting.ID}) /call pulse_stop 14
		/if (${validate_cast[TRUE, FIND, "${Buff${_count}}", ${Me.ID}]}) /call core_cast2 "${Buff${_count}}" FIND ${Me.ID} FALSE
	/next _count

	| check AA
	/if (${Bool[${AANow1}]} || ${Bool[${AANow2}]}) {
		/call check_AA_now
	}

	| check Discs
	/if (${Bool[${DiscNow1}]} || ${Bool[${DiscNow2}]}) {
		/call check_Disc_now
	}

	| wizard mancy line spells
	/if (${validate_class[FALSE, |WIZ]}) {
		/call sort_mancy_lineAA
	}

	/if (!${Bool[${timer_Check_Familiar}]}) /call sort_familiar

	/if (${coreAuto}) {
		| check shrink
		/if (${switchShrink}) /call check_shrink
	}

	| ditch unwanted buffs
	/if (${Bool[${lstBuffRemove}]}) {
		/call remove_unwanted_buffs	
	}

	| check auras
	/if (${Bool[${Aura1Spell}]} || ${Bool[${Aura2Spell}]}) {
		/if (!${timer_check_aura}) /call check_aura
	}

	/if (${Target.ID}) /squelch /target clear

	UPDATEHUD FALSE	
/return



|***
 *	DES: Checks pets buffs. If somethings missing, mem and cast it.
 *	USE: /pb and/or auto
 *  NOTE: this will do AA or Spell
 ***|
sub check_buffs_pet()
	/if (!${Me.Pet.ID}) /return
	CHECKINPUTS
	CHECKCOMBATCAST

	/if (${coreAuto} || ${swModePetfarm} || ${swModeHunt}) {
		/call timer_update timer_check_buffs_pet ${tPet_Buff}
	}

	/declare _count int local 0
	/declare _spellType string local
	
	/for _count 1 to 10
		AMIDEAD
		CHECKEXIT
		CHECKTIE
		/if (${check_engaged[]}) /return
		CHECKINPUTS
		CHECKCOMBATCAST
		UPDATEHUD "BUFF Pet"
		/if (${Me.Pet.Buff[${PetBuff${_count}chk}]}) /continue
		
		/if (${Me.Book[${Spell[${PetBuff${_count}}].RankName}]}) {
			/if (${validate_cast[FALSE, spell, "${PetBuff${_count}}", ${Me.Pet.ID}]}) /call core_cast2 "${PetBuff${_count}}" ${buffGem} ${Me.Pet.ID} FALSE
		} else /if (${Me.AltAbilityReady[${PetBuff${_count}}]}) {	
			/if (${validate_cast[FALSE, alt, "${PetBuff${_count}}", ${Me.Pet.ID}]}) /call core_cast2 "${PetBuff${_count}}" alt 0 FALSE
		} else /if (${Me.ItemReady[${PetBuff${_count}}]}) {
			/if (${validate_cast[FALSE, item, "${PetBuff${_count}}", ${Me.Pet.ID}]}) /call core_cast2 "${PetBuff${_count}}" item 0 FALSE		
		}

	/next _count
	
	| chekck for pet illusion
	/if (${Bool[${setPetIllusion}]} && !${Bool[${Me.Pet.Buff[${setPetIllusionBuff}]}]} && ${Me.Pet.Distance} < 100 && ${Spell[${setPetIllusionBuff}].NewStacks}) {
		| /call MQ2Cast "${setPetIllusion}" item -targetid|${Me.Pet.ID}
		/call core_cast2 "${setPetIllusion}" item ${Me.Pet.ID} FALSE
	}

	| shrink the little bastards..
	/if (${Me.Pet.ID}) /call cast_shrink_pet

	| pet epic click
	/if (${Me.ItemReady[${setPetEpicClick}]} && !${Me.Pet.Buff[Elemental Conjunction]} && !${Me.Pet.Buff[${setPetEpicBuff}]} && !${Me.Pet.Buff[Blessing of Unity]} && ${Spell[${setPetEpicBuff}].NewStacks}) {
		/if (${validate_cast[FALSE, item, "${setPetEpicClick}", ${Me.ID}]}) /call core_cast2 "${setPetEpicClick}" item 0 FALSE
	}
	
	/if (${Target.ID}) /squelch /target clear
	UPDATEHUD FALSE	
/return



|***
 *	DES: checks your toon for buffs that should stop casting for some reason
 *	USE: /call check_no_cast_buff
 *  NOTE: this is by toon.
 ***|
sub check_no_cast_buff()
	| disabled?
	/if (${lstNoCastWith.Arg[1,|].Equal[FALSE]} || !${Bool[${lstNoCastWith}]}) /return FALSE

	/declare _count int local 0
	/for _count 1 to ${lstNoCastWith.Count[|]}
		AMIDEAD
		CHECKINPUTS
		/if (${Bool[${Me.Buff[${lstNoCastWith.Arg[${_count},|]}]}]}) /return TRUE
		/if (${Bool[${Target.Buff[${lstNoCastWith.Arg[${_count},|]}]}]}) /return TRUE
		/if (${Bool[${Me.Song[${lstNoCastWith.Arg[${_count},|]}]}]}) /return TRUE
	/next _count	
/return FALSE



|***
 *	DES: checks your toon for buffs that should stop melee for some reason
 *	USE: /call check_no_melee_buff
 *  NOTE: this is by toon.
 ***|
sub check_no_melee_buff()
	| disabled?
	/if (${lstNoMeleeWith.Arg[1,|].Equal[FALSE]} || !${Bool[${lstNoMeleeWith}]}) /return FALSE

	/declare _count int local 0
	/for _count 1 to ${lstNoMeleeWith.Count[|]}
		AMIDEAD
		CHECKINPUTS
		/if (${Bool[${Me.Buff[${lstNoMeleeWith.Arg[${_count},|]}]}]}) /return TRUE
		/if (${Bool[${Target.Buff[${lstNoMeleeWith.Arg[${_count},|]}]}]}) /return TRUE
		/if (${Bool[${Me.Song[${lstNoMeleeWith.Arg[${_count},|]}]}]}) /return TRUE
	/next _count	
/return FALSE



|***
 *	DES: checks your toon and target for buffs that should prevent a thing
 *	USE: /call check_no_do_a_thing_buff
 *  NOTE: we assume return FALSE, as in no buffs found. if we find a buff, return TRUE
 ***|
sub check_no_do_a_thing_buff(string _type)

	/if (!${Bool[${no${_type.Left[1].Upper}${_type.Right[-1].Lower}With}]}) /return FALSE
	/if (!${Bool[${_type}]}) /return FALSE
	/declare _count int local 0

	/for _count 1 to ${no${_type.Left[1].Upper}${_type.Right[-1].Lower}With.Count[|]}
		AMIDEAD
		CHECKINPUTS

		/if (${Bool[${Me.Buff[${no${_type.Left[1].Upper}${_type.Right[-1].Lower}With.Arg[${_count},|]}]}]}) /return TRUE
		/if (${Target.ID}) {
			/if (${Bool[${Target.Buff[${no${_type.Left[1].Upper}${_type.Right[-1].Lower}With.Arg[${_count},|]}]}]}) /return TRUE
		}
		/if (${Bool[${Me.Song[${no${_type.Left[1].Upper}${_type.Right[-1].Lower}With.Arg[${_count},|]}]}]}) /return TRUE
	
	/next _count	
/return FALSE




|***
 *	DES: checks and clicks minature horn of unity
 *	USE: /horn or /onoff horn
 *  NOTE: 
 ***|
sub click_horn_of_unity()
	CHECKINPUTS

	/if (!${Bool[${switchHorn}]} || ${SafeZone} && !${swOverrideSafeZone} || ${Bool[${timer_zone_pause}]}) /return
	/if (!${Bool[${Me.Song[Blessing of Unity]}]} && (${Me.PctHPs} < 95*.${tmpHealOverride} || ${Me.PctMana} < 95*.${tmpHealOverride})) {
		/if (${Select[${Me.Class.ShortName},BRD]} && ${Me.Casting.ID}) /call pulse_stop 15
		/if (${validate_cast[FALSE, item, "Miniature Horn of Unity", ${Me.ID}]}) /call core_cast2 "Miniature Horn of Unity" item 0 FALSE
	}
/return



|***
 *	DES: Broadcasted Buff Routines
 *	USE: automatic any peers
 *  NOTE: 
  ***|
sub check_buffs_BC()
	CHECKCOMBATCAST

	/if (${coreAuto}) /call timer_update timer_check_buffs_BC ${tBC_Buff}
	UPDATEHUD "BUFF Broadcast"	

	/declare _toon int local 1
	/declare _count int local 0
	/declare _toonName string local FALSE
	/declare _spellID int local 
	/declare _i int local 0
	/declare _DNqout string local
	/declare _base2 string local
	/declare _found bool
	
	| each toon
	/while (${_toon} <= ${DanNet[${DanNetMacroChannel}].PeerCount}) {
		
		| get the name to something usable
		/varset _toonName ${DanNet[${DanNetMacroChannel}].Peers.Arg[${_toon},|]}
		
		| healers ignore the rampage tank
		/if (${validate_class[FALSE, |CLR|PAL|DRU|SHM]}) {
			/if (${Bool[${RampageTank}]} && ${RampageTank.Equal[${_toonName}]}) /continue
		}


		| each buff, cycle all ${setMaxBuffCount}, used or not
		/for _count 1 to ${setMaxBuffCount}
			/varset _found TRUE
			| SDEBUG ${_count}::${setMaxBuffCount}::\at${_toonName}\ax::\aw${Buff${_count}}\ax
			CHECKCOMBATCAST
			
			| make sure the count is in the correct range
			/if (!${Range.Between[1,${setMaxBuffCount}:${Int[${_count}]}]}) /continue
			AMIDEAD
			CHECKTIE
			CHECKINPUTS

			| no BC Buff
			/if (!${Bool[${Buff${_count}}]}) /continue
			
			| bcbuff someone else bitches...
			/if (${_toonName.Equal[${Me.DisplayName}]}) /continue
	
			| no class to buff?
			/if (!${Bool[${BCBuff${_count}Class}]}) /continue

			| no use this BC buff in raids
			/if (${Raid.Members} && ${Buff${_count}noBCinRaid}) /continue

			| buffee is dead?			
			/if (!${Spawn[pc ${_toonName}].ID} || ${Spawn[pc ${_toonName}].Dead}) /continue
			
			| targets class not listed
			/if (!${BCBuff${_count}Class.Find[${Spawn[pc ${_toonName}].Class.ShortName}]}) /continue

			| bufeee out of range
			/if (${Spawn[pc ${_toonName}].Distance} > ${Spell[${Buff${_count}}].MyRange}) /continue
			
			| buffee out of AE range
			/if (${Spell[${Buff${_count}}].TargetType.Equal[Group V2]}) {
				/if (${Spawn[pc ${_toonName}].Distance} > ${Spell[${Buff${_count}}].AERange}) /continue
			}

			| is the spell ready?
			| /if (!${Me.SpellReady[${Buff${_count}}]} && ${Me.Gem[${Buff${_count}}]}) /continue

			| any buff slots available?
			/dquery ${_toonName} -q "Me.FreeBuffSlots" -o _DNqout -t
			/if (${_DNqout.Equal[0]}) /continue

			| SPA or not?
			/if (${Spell[${Buff${_count}}].HasSPA[374]} || ${Spell[${Buff${_count}}].HasSPA[340]}) {
				| SDEBUG ${sep} SPA

				| loop the effects
				/for _i 1 to ${Spell[${Buff${_count}}].NumEffects}
					| skip any secondary procs with no duration
					/if (!${Bool[${Spell[${Spell[${Buff${_count}}].Base2[${_i}]}].Duration}]}) /continue
					/if (!${_found}) /continue
					
					| parsing issues: set the name of the secondary spell before passing the request to the other toon. should not need to do this.
					/varset _base2 ${Spell[${Spell[${Buff${_count}}].Base2[${_i}]}].Name}

					| do we have the triggered buff?
					/dquery ${_toonName} -q "Me.Buff[${_base2}].ID" -o _DNqout -t 1s
					| SDEBUG .Base2.ID \at${_DNqout}\ax
					/if (!${Bool[${_DNqout}]}) 	/varset _found FALSE

					| does it stack
					/dquery ${_toonName} -q "Spell[${_base2}].NewStacks" -o _DNqout -t 1s 
					| SDEBUG .Base2.NewStacks \at${_DNqout}\ax
					/if (!${Bool[${_DNqout}]}) /varset _found TRUE

					| is it buff blocked 
					/dquery ${_toonName} -q "lstBlockedSpells.Find[${_base2}]" -o _DNqout -t 1s 
					| SDEBUG .Base2.Blocked Buff \at${_DNqout}\ax
					/if (${Bool[${_DNqout}]}) /varset _found TRUE

				/next _i

			| no secondary procing buffs. just check for the spells main name
			} else {
	
				| has the buff?
				/dquery ${_toonName} -q "Me.Buff[${Buff${_count}}].Name" -o _DNqout -t 1s
				| SDEBUG .Name \at${_DNqout}\ax
				/if (!${Bool[${_DNqout}]}) /varset _found FALSE

				| stacks				
				/dquery ${_toonName} -q "Spell[${Buff${_count}}].NewStacks" -o _DNqout -t 1s
				| SDEBUG .NewStacks \at${_DNqout}\ax
				/if (!${Bool[${_DNqout}]}) /varset _found TRUE

				| is it buff blocked 
				/dquery ${_toonName} -q "lstBlockedSpells.Find[${Buff${_count}}]" -o _DNqout -t 1s 
				/if (${Bool[${_DNqout}]}) /varset _found TRUE

			}

			/if (${Me.CurrentMana} > ${Spell[${Buff${_count}}].Mana} && !${_found}) { 
				| bards STFU
				/if (${Select[${Me.Class.ShortName},BRD]} && ${Me.Casting.ID}) /call pulse_stop 16

				| can we cast the spell already				
				/if (${validate_cast[FALSE, spell, "${Buff${_count}}", ${Spawn[pc ${_toonName}].ID}]}) /call core_cast2 "${Buff${_count}}" ${buffGem} ${Spawn[pc ${_toonName}].ID} FALSE
			}
		/next _count
		
		| keep the line moving please...
		/varcal _toon ${_toon}+1
	}	

	UPDATEHUD FALSE
/return



|***
 *	DES: make sure you have ammo
 *	USE: /call check_ammo
 *  NOTE:
 ***|
sub check_ammo()
	| SDEBUG \atcheck_ammo\ax()
	AMIDEAD
	CHECKEXIT
	CHECKINPUTS

	/call timer_update timer_Check_Ammo ${tCheck_Ammo}
	/declare _count int local 0
	CLEARCURSOR TRUE
	
	| enumerate the |'s
	/for _count 1 to ${lstRangeItem.Count[|]}
		| do we have enough of the summoned item
		| /if (${FindItemCount[${lstRangeItemSummon.Arg[${_count},|]}]} > ${strRangeItemCount}) /continue
		/if (${FindItemCount[${lstRangeItemSummon.Arg[${_count},|]}]}) /continue

		| is there an entry for this loop?
		/if (!${Bool[${lstRangeItem.Arg[${_count},|]}]} || !${Bool[${lstRangeItemSummon.Arg[${_count},|]}]}) /continue

		| sort what type of item we need to get
		/if (${Select[${Me.Class.ShortName},BRD]} && ${Me.Casting.ID}) /call pulse_stop
		/if (${Me.Book[${lstRangeItem.Arg[${_count},|]}]}) {
			/if (${validate_cast[FALSE, spell, "${lstRangeItem.Arg[${_count},|]}", ${Me.ID}]}) /call core_cast2 "${lstRangeItem.Arg[${_count},|]}" ${buffGem} ${Me.ID} FALSE
		} else /if (${Me.ItemReady[${lstRangeItem.Arg[${_count},|]}]}) {
			/if (${validate_cast[FALSE, item, "${lstRangeItem.Arg[${_count},|]}", ${Me.ID}]}) /call core_cast2 "${lstRangeItem.Arg[${_count},|]}" item 0 FALSE
		} else /if (${Me.AltAbilityReady[${lstRangeItem.Arg[${_count},|]}]}) {
			/if (${validate_cast[FALSE, alt, "${lstRangeItem.Arg[${_count},|]}", ${Me.ID}]}) /call core_cast2 "${lstRangeItem.Arg[${_count},|]}" alt ${Me.ID} FALSE
		} else /if (${Me.CombatAbilityReady[${lstRangeItem.Arg[${_count},|]}]} || ${Select[${Me.Class.ShortName},BER]}) {
			/if (${validate_cast[FALSE, disc, "${lstRangeItem.Arg[${_count},|]}", ${Me.ID}]}) /call core_cast2 "${lstRangeItem.Arg[${_count},|]}" disc ${Me.ID} FALSE
		}
	/next _count
	CLEARCURSOR TRUE

/return



|***
 *	DES: Baby got back.
 *	USE: /call check_Shrink
 *  NOTE: no, i don't know why the mage will not shrink.
 ***|
sub check_shrink()
	AMIDEAD
	CHECKEXIT
	CHECKINPUTS

	/if (${Me.Height} > 2.04) {
		/if (${Me.Book[${setShrinkItem}]}) {
			/if (${validate_cast[FALSE, spell, "${setShrinkItem}", ${Me.ID}]}) {
				/call core_cast2 "${setShrinkItem}" ${buffGem} ${Me.ID} FALSE
			}
		} else /if (${Me.ItemReady[${setShrinkItem}]}) {
			/if (${validate_cast[FALSE, item, "${setShrinkItem}", ${Me.ID}]}) {
				/call core_cast2 "${setShrinkItem}" item ${Me.ID} FALSE
			}
		}
	}
/return



|***
 *	DES: update a timer
 *	USE: /call timer_update [TIMER VARIABLE NAME] [DURATION]
 *  NOTE: This will update existing timers..
 ***|
sub timer_update(string _my_timer, string _my_duration)
	/if (!${Defined[${_my_timer}]}) /declare ${_my_timer} timer outer
	/varset ${_my_timer} ${_my_duration}
/return TRUE



|***
 *	DES: rest/not rest routines.
 *	USE: /call do_rest
 *  NOTE: 
 ***|
sub do_rest()
	| the dead dont need to rest
	/if (!${coreAuto} || ${swAmIDead} || ${Me.Zoning}) /return FALSE
	
	| dont rest while in tie
	/if (${swTieLock} && ${swSetTie}) /return FALSE
	
	| clerics and pallys wait 'till yaulp gone
	/if (${Bool[${Me.Buff[Lesser Yaulp]}]} || ${Bool[${Me.Buff[Yaulp]}]}) /return
	CHECKINPUTS

	/if (${swHomeSet} && !${Bool[${combat}]} && !${SpawnCount[npc radius ${setCombatDistance}*2 playerstate 4 targetable]} && ((${Me.Y} != ${MakeCamp.AnchorY}) && (${Me.X} != ${MakeCamp.AnchorX}))) {
		/if (${swPull}) /squelch /makecamp unpause
		/nav locxyz ${MakeCamp.AnchorX} ${MakeCamp.AnchorY} ${Me.Z} log=off
		/delay 3s ((${Me.Y} == ${MakeCamp.AnchorY}) && (${Me.X} == ${MakeCamp.AnchorX}))
	}

	/declare _needrest bool local FALSE
	/declare _resting bool local FALSE
	
	:loopneedmorerest
	/varset _needrest FALSE
	CHECKREZ
	/if (${SpawnCount[npc ${If[${swOverrideLOS},,los]} radius ${setMobAgro} playerstate 4]} || !${coreAuto}) {
		/varset _needrest FALSE
		/if (${Me.State.NotEqual[STAND]}) /stand
		/if (${Me.Casting.ID} || ${Me.BardSongPlaying} || ${Window[CastingWindow].Open}) {
			/stopsong
			/stopcast
			/delay 10 !${Me.Casting.ID}						
		}
		/return
	}

	AMIDEAD
	CLEARCURSOR
	CHECKTIE
	CHECKINPUTS
	| check self click buffs
	/if (!${Me.Invis} && ${swRestBuff}) {
		/if (${swBuffItem} && !${check_engaged[]} && !${timer_check_buffs_item}) /call check_buffs_item
		/if (${swBuffTell} && !${timer_check_buffs_tell}) /call check_buffs_tell
	}
	
	/if (${swADVLoot}) {
		/if (${coreAuto} && (${Bool[${Group.Members}]} && ${Group.MasterLooter.ID}==${Me.ID}) && ${Me.UseAdvancedLooting} && !${AdvLoot.LootInProgress} && !${Stick.Active}) /call do_loot
	}
	
	| bard rest song
	/if (${Me.Class.ShortName.Equal[BRD]}) {
		/if (${switchRestSong} && !${Me.Song[${spellRestSong}].ID}) {
			OUT Lullaby ${sep} ${csp}${Spell[${spellRestSong}].RankName}\ax
			/call mem_spell FALSE "${spellRestSong}" ${buffGem} TRUE
			/cast ${Me.Gem[${Spell[${spellRestSong}].RankName}]}
			/delay 5
			/while (${Window[CastingWindow].Open}) {
				/delay 5
			}	
		}
	}	
			
	/if (${setRestPct} && !${Me.Moving} && !${SpawnCount[npc ${If[${swOverrideLOS},,los]} radius ${setMobAgro} playerstate 4]}) {
			
		| Hybrid: RNG,SHD,BST,PAL,BRD
		/if ((${lstClassMelee.Find[${Me.Class.ShortName}]} && ${lstClassCast.Find[${Me.Class.ShortName}]}) || ${Me.Class.ShortName.Equal[BRD]}) {

			| rest to full requirement
			/if (${swRestFull} && ${_resting}) {
				/if (${Me.PctEndurance} < 99*.${tmpHealOverride}) /varset _needrest TRUE
				/if (${Me.PctMana} < 99*.${tmpHealOverride}) /varset _needrest TRUE
			} else {
				/if (${Me.PctEndurance} < ${setRestPct}*.${tmpHealOverride}) /varset _needrest TRUE
				/if (${Me.PctMana} < ${setRestPct}*.${tmpHealOverride}) /varset _needrest TRUE
			}

			| bards suck...
			/if (${Me.Class.ShortName.Equal[BRD]}) {
				| /call cast_AA_Rallying
				/if (${swModRod} && ${swRestModRod}) /call use_mod_rod
				/if (${Me.PctEndurance} < ${setRestPct}*.${tmpHealOverride}) /varset _needrest TRUE
				/if (${Me.PctMana} < ${setRestPct}*.${tmpHealOverride}) /varset _needrest TRUE
				
			} else {
				/if (${swModRod} && ${swRestModRod}) /call use_mod_rod
				/call cast_stamina_recovery
			}
			/if (${Select[${Me.Class.ShortName},PAL]}) {
				/if (${switchHarmonious}) /call check_harmonious
				/if (${switchSteel} && !${timer_check_Steel}) /call check_steel	
			} else /if (${Select[${Me.Class.ShortName},SHD]}) {
				/if (${swPet} && ${swRestBuff}) /call check_pet
				/if (${switchHarmonious}) /call check_harmonious
				/if (${switchSkin} && !${timer_check_Skin}) /call check_skin	
			} else /if (${Select[${Me.Class.ShortName},RNG]}) {
			} else /if (${Select[${Me.Class.ShortName},BST]}) {
			} 

		| Melee: WAR,MNK,ROG,BER
		} else /if (${lstClassMelee.Find[${Me.Class.ShortName}]} && !${lstClassCast.Find[${Me.Class.ShortName}]}) {
			/if (${Me.PctEndurance} < ${setRestPct}*.${tmpHealOverride}) /varset _needrest TRUE

			| rest to full requirement
			/if (${swRestFull}) {
				/if (${Me.PctEndurance} < 99*.${tmpHealOverride}) /varset _needrest TRUE
			}

			/call cast_stamina_recovery
			/if (${Select[${Me.Class.ShortName},WAR]}) {
			} else /if (${Select[${Me.Class.ShortName},MNK]}) {
			} else /if (${Select[${Me.Class.ShortName},ROG]}) {
				/if (${coreAuto} && ${swSoS}) /call automaticSoS
			} else /if (${Select[${Me.Class.ShortName},BER]}) {
			}
			
		| Caster: CLR,DRU,SHM,ENC,WIZ,NEC,MAG
		} else /if (!${lstClassMelee.Find[${Me.Class.ShortName}]} && ${lstClassCast.Find[${Me.Class.ShortName}]}) {
			/if (${Me.PctMana} < ${setRestPct}*.${tmpHealOverride}) {
				/varset _needrest TRUE
				/if (${swModRod} && ${swRestModRod}) /call use_mod_rod
			}
			
			| rest to full requirement
			/if (${swRestFull}) {
				/if (${Me.PctMana} < 99*.${tmpHealOverride}) /varset _needrest TRUE
				/if (${swModRod} && ${swRestModRod}) /call use_mod_rod
			}			

			/if (${Select[${Me.Class.ShortName},CLR]}) {
				/if (${swCombatHeal}) {
					/if ((${swHealGroup} || ${swHealXTarget}) && ${SpawnCount[npc radius ${setMobAgro} playerstate 4]}) {
						/call check_heal_group_${Me.Class.ShortName}
					}
				}				
				
			} else /if (${Select[${Me.Class.ShortName},DRU]}) {
				/if (${swHealGroup} && ${swCombatHeal}) /call check_heal_group_${Me.Class.ShortName}
			} else /if (${Select[${Me.Class.ShortName},SHM]}) {
				/if (${swHealGroup} && ${swCombatHeal}) /call check_heal_group_${Me.Class.ShortName}
				/if (${Me.PctMana} < 60*.${tmpHealOverride} && !${swNeedRez}) /call check_cani
			} else /if (${Select[${Me.Class.ShortName},ENC]}) {
			} else /if (${Select[${Me.Class.ShortName},WIZ]}) {
				/if (${swRestBuff}) /call sort_mancy_lineAA
			} else /if (${Select[${Me.Class.ShortName},NEC]}) {
				/if (${swPet} && ${swRestBuff}) /call check_pet
			} else /if (${Select[${Me.Class.ShortName},MAG]}) {
				/if (${swPet} && ${swRestBuff}) /call check_pet
			}
		} 			
	}
		
	/if (${_needrest}) {
		UPDATEHUD "Rest"
		/if (${Me.State.NotEqual[SIT]} && !${Me.Mount.ID}) /sit
		/varset _resting TRUE
		/delay 2
		CHECKTIE
		AMIDEAD
		CHECKINPUTS
		/goto :loopneedmorerest
	} 
	
	UPDATEHUD FALSE
/return



|***
 *	DES: set Home camp spot
 *	USE: /call set_home
 *  NOTE: 
 ***|
sub set_home()
	/if (${swHomeSet}) {
		/varset swSetTie FALSE
		VOUT ${cgood}Camp\ax R(${cnum}${setHomeRadius}\ax) L(${cnum}${setHomeLeash}\ax) ${sep} Y(${cnum}${Me.Y}\ax) X(${cnum}${Me.X}\ax)
		/squelch /makecamp on radius ${setHomeRadius} leash ${setHomeLeash} mindelay ${setHomeDelayMin} maxdelay ${setHomeDelayMax}		
	} else /if (!${swHomeSet}) {
		VOUT ${ocaution} ${sep} No Camp established.
		/squelch /makecamp off
	}
	/squelch /moveto unpause
	/squelch /stick off
/return



|***
 *	DES: stops follow and re-establishes camp if needed. This stops everything.. seriously.. everything.
 *	USE: /stop or /alto or /squelch stop or /call set_stop [NOECHO]
 *  NOTE: just because you stop something, does not mean you will not start it right back up again.
 ****|
sub set_stop(string _echo)
	UPDATEHUD "Stop"
	/varset combat FALSE
	/if (!${_echo.Equal[NOECHO]}) VOUT I was told to ${cbad}STOP\ax !! 
	
	| seriously. stop casting
	/if (${Select[${Me.Class.ShortName},BRD]}) {
		 /call pulse_stop 1
	} else {
		/if (${Me.Casting.ID}) /stopcast
	}
	
	/if (${Stick.Active}) /squelch /stick off
	/if (${AdvPath.Following}) /squelch /afollow off
	/if (${Navigation.Active}) /nav stop log=off
	/if (${swsnapstuck}) {
		/varset swsnapstuck FALSE
	}
	
	/if (${swSetTie}) {
		/varset swSetTie FALSE
	}


	| stop moving
	/if (${Me.Moving}) {
		/keypress forward
		/keypress back
	}
 	 	
	| /call clear_combat
	/if (${Me.Pet.ID} && ${Bool[${Me.Pet.Target}]}) /pet back off
	/doevents flush
	/if (${Bool[${swHomeSet}]}) {
		/call set_switch ${chrID} swHomeSet TRUE
		/call set_home
	}

	UPDATEHUD FALSE
/return



|***
 *	DES: Clicks usefull 1 off items
 *	USE: /call click_item 
 *  NOTE: 
 ***|
sub click_item()
	/declare _count int local 0
	/for _count 1 to 6
		/if (${Spawn[${Target.ID}].PctHPs} < 5*.${tmpHealOverride}) /break
		/if (${Spawn[${Target.ID}].Distance} > 100) /break
		/if (${is_target_dead[${Target2Kill}]}) /break
		/if (!${Bool[${clickItem${_count}}]}) /continue
		AMIDEAD
		CHECKTIE
		CHECKINPUTS
		/if (${validate_cast[FALSE, item, "${clickItem${_count}}", ${Me.ID}]}) {
			/call core_cast2 "${clickItem${_count}}" item 0 FALSE
		}
	/next _count
/return



|***
 *	DES: Clicks swarmpet items
 *	USE: /call click_item_swarm [TARGETID]
 *  NOTE: 
 ***|
sub click_item_swarm(int _tmpID)
	/declare _count int local 0
	
	| check for a DS on target
	/if (${swNoSwarmonDS}) {
		/if (${Target.ID} != ${_tmpID}) /squelch /target ID ${_tmpID}
		/if (${Target.DSed.ID}) /return FALSE		
	}
	
	/for _count 1 to 4
		/if (${Spawn[${Target.ID}].PctHPs} < 5*.${tmpHealOverride}) /break
		/if (${is_target_dead[${_tmpID}]}) /break
		/if (${Spawn[${Target.ID}].Distance} > 100) /break
		/if (!${Bool[${clickItemSwarm${_count}}]}) /continue
		AMIDEAD
		CHECKTIE
		CHECKINPUTS
		/if (${validate_cast[FALSE, item, "${clickItemSwarm${_count}}", ${_tmpID}]}) {
			/call core_cast2 "${clickItemSwarm${_count}}" item ${_tmpID} FALSE
		}
	/next _count
/return



|***
 *	DES: how to handle cursor items
 *	USE: /call sort_cursor || CLEARCURSOR
 *  NOTE: 
 ***|
sub sort_cursor(bool _ignoreTimer)
	/if (!${Cursor.ID}) /return
	/if (!${Bool[${_ignoreTimer}]} && ${timer_clear_cursor}) /return
	AMIDEAD

	| we have faith in the all mighty AutoInventoryItem=
	/if (${Bool[${autoInventoryItem.Find[${Cursor.Name}]}]}) /autoinventory 

	/if (!${Bool[${_ignoreTimer}]} && ${autoInventoryItem.Find[${Cursor.Name}]}) {
		/if (${swAutoCursor} && ${timer_clear_cursor}) /return
		/if (!${swAutoCursor} && !${autoInventoryItem.Find[${Cursor.Name}]}) /return
	}
	
	| allow for absence of override for not in same zone as master looter
	/if (!${swAutoCursor} && !${swOverrideLoot} && !${Bool[${_ignoreTimer}]}) /return
	
	/if (!${Cursor.ID}) /return
	/declare _cursorChoice string local 

	| corect the INI entry if not set for the new form
	/if (!${Ini[${INILoot},${Cursor.Name.Left[1]},${Cursor.Name}].Length}) {
		/ini "${INILoot}" "${Cursor.Name.Left[1]}" "${Cursor.Name}" "|Keep"

	| its in the ini, but its an old entry format so fix it
	} else /if (!${Ini[${INILoot},"${Cursor.Name.Left[1]}","${Cursor.Name}"].Count[|]}) {
		VOUT Fixing Loot: ${Cursor.Name}
		/ini "${INILoot}" "${Cursor.Name.Left[1]}" "${Cursor.Name}" |${Ini[${INILoot},"${Cursor.Name.Left[1]}","${Cursor.Name}"]}
	}		
	
	| wtf we gotta do with the item?
	/if (${Cursor.ID}) /varset _cursorChoice ${Ini[${INILoot},"${Cursor.Name.Left[1]}","${Cursor.Name}"].Arg[1,|]}

	| do the things that we do best
	/if (${autoInventoryItem.Find[${Cursor.Name}]} || ${Select[${_cursorChoice},Keep,Sell,Announce]}) {
		/if (${Select[${_cursorChoice},Announce]}) ECHOCHANNEL - Cursor .. ${Cursor.Name}
		/if (${Cursor.ID}) VOUT Cursor ${sep} ${okeep} ${sep} ${cinfo}${Cursor.Name}\ax ${sep} (${cnum}${FindItemCount[=${Cursor.Name}]}\ax)
		/if (${Cursor.ID}) /nomodkey /autoinventory
	} else /if (${Select[${_cursorChoice},Destroy]}) {
		/if (${Cursor.ID}) VOUT Cursor ${sep} ${odestroy} ${sep} ${cinfo}${Cursor.Name}\ax
		/if (${Cursor.ID} && ${_cursorChoice.Equal[Destroy]}) /nomodkey /destroy
	} else /if (${Select[${_cursorChoice},Leave]}) {
		/if (${Cursor.ID}) VOUT Cursor ${sep} ${oleave} ${sep} ${cinfo}${Cursor.Name}\ax
		/if (${Cursor.ID} && ${_cursorChoice.Equal[Leave]}) /nomodkey /drop
	} else /if (${Select[${_cursorChoice},Ignore]}) {
		/return
	}

	/if (${swAutoCursor}) {
		/call timer_update timer_clear_cursor ${tClear_Cursor}
	}
/return



|***
 *	DES: XTARGET controls
 *	USE: its in the toons INI
 *  NOTE: for healers its /heal xtarget
 ***|
sub set_xtarget()
	/if (!${validate_expansion[FALSE, |16]}) /return	
	/if (!${Select[${Me.Class.ShortName},SHM,CLR,PAL,DRU]}) {
		/call echo_bitch MQWindow
		/if (${Defined[swHealXTarget]}) /call set_switch class swHealXTarget FALSE 
		/return
	}
	
	VOUT Sorting XTargets ${sep} ${cinfo}${tXHeal}\ax
	/declare _x 				int 		local
	/declare _b 				int 		local
	/declare _count 		int			local 0
	/declare _name			string	local

	| reset extended target slots	
	/call clear_xtarget
	
	/call timer_update timer_set_xtarget ${tXHeal}
		
	| bail if we are not healing the slackers and xtarget is clear
	/if (!${swHealXTarget}) /return

	| set protection for NPCs
	/if (${Bool[${ProtectNPCList}]}) {
		/for _count 1 to ${ProtectNPCList.Count[|]}
			/varset _name ${Spawn[=${ProtectNPCList.Arg[${_x},|]} npc].DisplayName}

			/if (${_b} > 13 || !${Bool[${Spawn[${_name}]}]}) {
			} else /if (${Bool[${Me.XTarget[${_name}]}]}) {
				/varcalc _b ${_b}+1
			} else /if (${Bool[${Spawn[${_name}]}]}) {
				/varcalc _b ${_b}+1
				/xtarget set ${_b} ${Spawn[${_name}].Name}
			}
		/next _count
	}	

	| protect PCs when outside group
	/if (!${Raid.Members} && ${Bool[${ProtectOutsideGroup}]}) {
		/for _x 1 to ${ProtectOutsideGroup.Count[|]}
			/varset _name ${Spawn[=${ProtectOutsideGroup.Arg[${_x},|]} pc].DisplayName}
			/if (${Spawn[pc ${_name}].Distance} > ${setMobAgro}) /continue
			/if (${Group.Member[${_name}].ID}) /continue
			/if (${_b} > 13 || !${Bool[${Spawn[pc ${_name}]}]}) /continue
			/if (${Bool[${Me.XTarget[${_name}]}]}) {
				/varcalc _b ${_b}+1
			} else /if (${Bool[${Spawn[pc ${_name}]}]}) {
				/varcalc _b ${_b}+1
				/xtarget set ${_b} ${Spawn[pc ${_name}].Name}
			}
		/next _x
	}				

	| set raid member classes to protect
	/if (${Raid.Members}) {
		/if (${Bool[${ProtectInsideRaid}]}) {
			/for _x 1 to ${ProtectInsideRaid.Count[|]}
			/varset _name ${Spawn[=${ProtectInsideRaid.Arg[${_x},|]} pc].DisplayName}

				/if (${Spawn[pc ${_name}].Distance} > ${setMobAgro}) /continue
				/if (${Bool[${Group.Member[${_name}]}]}) /continue
				/if (${_b} > 13 || !${Bool[${Spawn[pc ${_name}]}]}) {
				} else /if (${Bool[${Me.XTarget[${_name}]}]}) {
					/varcalc _b ${_b}+1
				} else /if (${Bool[${Spawn[pc ${_name}]}]}) {
					/varcalc _b ${_b}+1
					/xtarget set ${_b} ${Spawn[pc ${_name}].Name}
				}
			/next _x
		}
		
		| set raid protected classes
		/for _count 1 to ${XHealClass.Count[|]}

			| no class?
			/if (!${SpawnCount[pc ${XHealClass.Arg[${_count},|]} raid radius ${setMobAgro}]}) /continue

			/for _x 1 to ${SpawnCount[pc ${XHealClass.Arg[${_count},|]} raid radius ${setMobAgro}]}

				/if (${_b} > 13) /break
				
				| are they a group member?
				/if (${Group.Member[${NearestSpawn[${_x}, pc ${XHealClass.Arg[${_count},|]} raid]}].ID}) /continue
				
				| on the xtarget
				/if (${Bool[${Me.XTarget[${NearestSpawn[${_x}, pc ${XHealClass.Arg[${_count},|]} raid].Name}]}]}) /continue

				| me?
				/if (${NearestSpawn[${_x}, pc ${XHealClass.Arg[${_count},|]} raid radius ${setMobAgro}].DisplayName.Equal[${Me.DisplayName}]}) /continue

				/varcalc _b ${_b}+1
				/xtarget set ${_b} ${NearestSpawn[${_x}, pc ${XHealClass.Arg[${_count},|]} raid radius ${setMobAgro}]}
			/next _x
			
		/next _count	
		
	}
	
/return



|***
 *	DES: Sticks you to the target
 *	USE: /call stick_to_target [target ID]
 *  NOTE: for the love of whatever you find important, don't touch this ever again...
 ***|
sub stick_to_target()
	AMIDEAD	
	CHECKINPUTS
	/if (!${swCombatMelee}) || !${coreAuto}) /return

	| if the stick behind switch is on. we do that over ranged decision combat
	/if (${swStickBehind}) {
		/call get_assist_target
		/if (${Target.ID} && !${Range.Between[0,${setFaceArc}:${Math.Abs[${Math.Calc[${Target.HeadingTo.Degrees}-${Me.Heading.Degrees}]}]}]}) /squelch /face ID ${Target2Kill} nolook ${If[${swFaceFast},fast,]}
			
		/if (!${Stick.Active} && ${Target.ID} && ${isValidTarget} && ${swStickBehind} && !${Stick.Behind} && !${timer_snap_recheckID_${Target2Kill}} && ${swCombatMelee}) {
			/squelch /stick id ${Target2Kill} ${setCombatDistance} snaproll
		} 

		:checkstuck
		AMIDEAD	
		CHECKINPUTS
		/if (${is_target_dead[${Target2Kill}]}) /return
		/if (${MoveUtils.Stuck}) {
			/delay 1
			/squelch /stick off
			/call timer_update timer_check_snap_retry ${tCheck_snap_retry}
			/return
		}	

		/if (${Spawn[${Target2Kill}].Distance} < ${setCombatDistance} && !${swStickBehind} && !${timer_snap_recheckID_${Target2Kill}}) {
			/squelch /stick off
			/return
		}
		/if (!${Stick.Behind} && ${swStickBehind} && !${timer_snap_recheckID_${Target2Kill}}) /goto :checkstuck

	} else /if (!${swStickBehind}) {
		/if (${swModeHunt}) /varset isValidTarget TRUE
		:movetomob
		AMIDEAD	
		CHECKINPUTS
		/if (${is_target_dead[${Target2Kill}]}) /return
		/if (!${Stick.Active} && ${Target.ID} && ${isValidTarget} && ${Spawn[${Target2Kill}].Distance} > ${setCombatDistance} && ${swCombatMelee}) {
			/if (${coreAuto}) {
				/if (!${Range.Between[0,${setFaceArc}:${Math.Abs[${Math.Calc[${Target.HeadingTo.Degrees}-${Me.Heading.Degrees}]}]}]}) /squelch /face ID ${Target2Kill} nolook ${If[${swFaceFast},fast,]}
			}

			/if (${coreAuto}) /squelch /stick id ${Target2Kill} ${setCombatDistance} 
		}
		/delay 1
		/if (${Spawn[${Target2Kill}].Distance} > ${setCombatDistance}) /goto :movetomob
		/if (${Spawn[${Target2Kill}].Distance} < ${setCombatDistance}) /squelch /stick off
		/return
	}
	
	/squelch /stick off
/return



|***
 *	DES: Acquire target, get bitches. This will establish ${Target2Kill} for everyone.
 *	USE: /call get_assist_target
 *  NOTE: 
 ***|
sub get_assist_target()
	AMIDEAD
	CHECKINPUTS

	/if (!${coreAuto}) /return
	/if (${Group.MainAssist.Name.Equal[${Me.DisplayName}]} && !${swAssistF}) /return
	/if (${swAssistF} && ${Target2Kill} == ${Me.ID}) /return
	
	| if no MA present. move along..	
	/if (!${Bool[${Spawn[${setMA1}].ID}]} && !${Bool[${Spawn[${setMA2}].ID}]} && !${swAssist1} && !${swAssist2} && !${swAssist3} && !${swAssistG} && !${swAssistF}) {
		/delay 1
		/return
	} 	
	
	| cannnot assist yourself
	/if (${Spawn[pc ${setMA1}].ID} == ${Me.ID}) /call Bind_setCombatControl setMA1 FALSE
	/if (${Spawn[pc ${setMA2}].ID} == ${Me.ID}) /call Bind_setCombatControl setMA2 FALSE
	
	| if we dont have underfoot expansion, skip ALL smartassist go directly to jail. 
	/if (!${validate_expansion[FALSE, |16]}) /goto :skippingtoMAtarget
	/if (${swAssistF}) /goto :skippingtoMAforcedtarget

	| no, you still cannot assist yourself. get over it
	/if (${Group.MainAssist.Spawn.ID} == ${Me.ID}) /call validate_assist NOECHO X
	/if (${Spawn[=${Raid.MainAssist} pc].ID} == ${Me.ID}) /call validate_assist NOECHO X

	| tar acquisition when using smart assist. set XTarget positions.
	/if (${swAssistG} || ${swAssist1} || ${swAssist2} || ${swAssist3}) {

		| if the raid/group roles are not set or dead, move on tho setMA1/2	
		/if (${swAssistG}) {
			/if (!${Bool[${Group.MainAssist}]}) /goto :skippingtoMAtarget
			/if (${Bool[${Spawn[${Group.MainAssist}].Dead}]}) /goto :skippingtoMAtarget
		} else /if (${swAssist1} || ${swAssist2} || ${swAssist3}) {		
		}	
	
		| set the 1st xtarget slot to the assists target
		/if (${swAssistG}) {
			/if (${Me.XTarget[1].TargetType.NotEqual[Group Assist Target]} && ${Group.Members}) /xtarget set 1 groupassisttarget
		} else /if (${swAssist1}) {
			/if (${Me.XTarget[1].TargetType.NotEqual[Raid Assist 1 Target]} && ${Raid.Members}) /xtarget set 1 raidassist1target
		} else /if (${swAssist2}) {
			/if (${Me.XTarget[1].TargetType.NotEqual[Raid Assist 2 Target]} && ${Raid.Members}) /xtarget set 1 raidassist2target
		} else /if (${swAssist3}) {
			/if (${Me.XTarget[1].TargetType.NotEqual[Raid Assist 3 Target]} && ${Raid.Members}) /xtarget set 1 raidassist3target
		}
		
		| smart assist group
		/if (${swAssistG}) {
			| /if (${Bool[${Spawn[${Me.XTarget[1].ID}].Type.Equal[NPC]}]}) {
				/if (${Me.XTarget[1].PctHPs} <= ${setEngagePct} || ${swOverrideEngagePct} || ${swdeBuffMezz}) {
					/varset Target2Kill ${Me.XTarget[1].ID}
					/delay 1
					/if (${Spawn[id ${Target2Kill}].Distance} <= ${setMobAgro} && ${Target.ID} != ${Target2Kill}) {
						/if (${swCombatMelee} || ${swCombatRange} || ${swCombatNuke} || ${swCombatDoT}) /squelch /target ID ${Target2Kill}
					}
					/return
				}
			| }

		| smart assist raid
		} else /if (${swAssist1} || ${swAssist2} || ${swAssist3}) {
			| /if (${Bool[${Spawn[${Me.XTarget[1].ID}].Type.Equal[NPC]}]}) {
				/if (${Me.XTarget[1].PctHPs} <= ${setEngagePct} || ${swOverrideEngagePct}) {
					/varset Target2Kill ${Me.XTarget[1].ID}
					/delay 1
					/if (${Spawn[id ${Target2Kill}].Distance} <= ${setMobAgro} && ${Target.ID} != ${Target2Kill}) {
						/if (${swCombatMelee} || ${swCombatRange} || ${swCombatNuke} || ${swCombatDoT}) /squelch /target ID ${Target2Kill}
					}
					/return
				}	
			| }
		}

		/return
	} 

	| target acquisition when not using manual assist.
	:skippingtoMAtarget
	/if (${SpawnCount[npc radius ${setMobAgro}]} && (${Bool[${setMA1}]} || ${Bool[${setMA2}]})) {
		| setMA1
		/if (${Bool[${setMA1}]} && ${Bool[${Spawn[${setMA1}].ID}]} && !${Bool[${Spawn[${setMA1}].Dead}]}) {
			/if (${swCombatMelee} || ${swCombatRange} || ${swCombatNuke} || ${swCombatDoT}) /squelch /assist ${setMA1}
			/delay 1s ${Me.AssistComplete}
			| /if (!${Target2Kill} && ${Target.ID} && ${Spawn[${Target.ID}].Type.Equal[npc]}) {
			/if (!${Target2Kill} && ${Target.ID}) {
				/varset Target2Kill ${Target.ID}
			} else /if (${Target2Kill} != ${Target.ID} && ${Target2Kill} != 0 && ${Target.ID} && ${Spawn[${Target}].Type.Equal[npc]}) {
				/delay 1
				/varset Target2Kill 0
				/return
			}
	
		| setMA2
		} else /if (${Bool[${setMA2}]} && ${Bool[${Spawn[${setMA2}].ID}]} && !${Bool[${Spawn[${setMA2}].Dead}]} && (!${Bool[${Spawn[${setMA1}].ID}]} || ${Bool[${Spawn[${setMA1}].Dead}]})) {
			/if (${swCombatMelee} || ${swCombatRange} || ${swCombatNuke} || ${swCombatDoT}) /squelch /assist ${setMA2}
			/delay 1s ${Me.AssistComplete}
			| /if (!${Target2Kill} && ${Target.ID} && ${Spawn[${Target.ID}].Type.Equal[npc]}) {
			/if (!${Target2Kill} && ${Target.ID}) {
				/varset Target2Kill ${Target.ID}
			} else /if (${Target2Kill} != ${Target.ID} && ${Target2Kill} != 0 && ${Target.ID} && ${Spawn[${Target}].Type.Equal[npc]}) {
				/delay 1
				/varset Target2Kill 0
				/return
			}	
			
		} 		
		AMIDEAD
		CHECKINPUTS
		| /if (${Target2Kill} == ${Target.ID} && ${Spawn[${Target}].Type.Equal[npc]}) {
		/if (${Target2Kill} == ${Target.ID}) {
			/if (${Target.PctHPs} > ${setEngageMAPct} && !${swOverrideEngagePct}) {
				/delay 3
				/goto :skippingtoMAtarget
			} 
		} else {
			/if (${Target.ID}) /squelch /target clear
			/return
		}
	}
	
	| target acquisition when not using manual Forced assist
	:skippingtoMAforcedtarget	
	/if (${Bool[${swAssistF}]} && ${Target2Kill} != 0) {
		
		/if (${Spawn[${Target2Kill}].ID}) {
			/if (${Target.ID} != ${Target2Kill} && ${Spawn[${Target2Kill}].ID}) {
				/squelch /target ID ${Target2Kill}
			}
 		} else /if (!${Spawn[${Target2Kill}].ID}) {
 		
 		}
		
		AMIDEAD
		CHECKINPUTS
		| /if (${Target2Kill} && ${Spawn[${Target}].Type.Equal[npc]}) {
		/if (${Target2Kill}) {
			/if (${Spawn[${Target2Kill}].PctHPs} > ${setEngageMAPct} && !${swOverrideEngagePct}) {
				/delay 3
				/goto :skippingtoMAforcedtarget
			} 
		} else {
			/if (${Target.ID}) /squelch /target clear
			/return
		}
	}
/return



|***
 *	DES: Seriously, do you need to ask?
 *	USE: mob dead = win!
 *  NOTE: ass.U.ME the mob is alive until, it is not.
 ***|
sub is_target_dead(int _tmpID)
	AMIDEAD

	/declare _dead bool local FALSE
	
	/if (!${coreAuto}) {
		/if (!${Bool[${_tmpID}]} && ${Target.ID}) /varset _tmpID ${Target.ID}
		/if (!${Bool[${_tmpID}]}) {
			/call clear_combat
			/return TRUE
		}
	}
	
	| no target ID? shits dead 'till we get one to check..
	/if (!${Target.ID} && !${swModeHunt}) {
		/varset _dead TRUE
	} else /if (${_tmpID} == 0 || !${Bool[${_tmpID}]}) {
		/varset _dead TRUE
	}

	| targets dead? were done here
	/if (${Target.Dead}) {
		/varset _dead TRUE
	| } else /if (${Spawn[_tmpID].ID}) {
	| 	/varset _dead TRUE
	} else /if (${Target.Type.Equal[Corpse]} || ${Spawn[${Target.ID}].Type.Equal[CORPSE]}) {
		/varset _dead TRUE
	} else /if (${Spawn[${_tmpID}]].Dead} || ${Spawn[${Target.ID}].Dead}) {
		/varset _dead TRUE
	| } else /if (!${Spawn[${_tmpID}].ID} || !${Bool[${Spawn[ID ${_tmpID}]}]}) {
	} else /if (!${Spawn[${_tmpID}].ID}) {
		/varset _dead TRUE
	} else /if (${Spawn[${Target.ID}].Type.Equal[PCCORPSE]}) {
		/varset _dead TRUE
	}

	/if (${_dead}) {
		/call clear_combat
	}
/return ${_dead}



|***
 *	DES: Clear all combat variables
 *	USE: /call clear_combat
 *  NOTE: 
 ***|
sub clear_combat()

	/if (${Select[${Me.Class.ShortName},BRD]}) /call pulse_stop 2
	
	/if (${combat.Equal[COMBAT]}) /varset combat FALSE
	/if (${Me.AutoFire}) /squelch /autofire
	/if (${Me.Combat}) /squelch /attack off
	/if (!${Select[${Me.Class.ShortName},BRD]}) {
		/if (${coreAuto} && ${Bool[${Me.Casting}]} && (!${Spell[${Me.Casting.ID}].Category.Equal[Transport]} || !${Spell[${Me.Casting.ID}].Category.Equal[Heals]})) /stopcast
	}
	/if (${Bool[${Target2Kill}]}) {
		/call clear_timer ${Target2Kill}
		/varset Target2Kill 0
	}
	
	| /if (${Me.MercID} && ${Me.MercenaryStance.NotEqual[Passive]}) /stance Passive
	/if (${swBurnControl}) /varset swBurnControl FALSE
	/if (${isValidTarget}) /varset isValidTarget FALSE
	/if (${Stick.Active}) /squelch /stick off
	/if (${coreAuto} && ${Target.ID}) /squelch /target clear
	/if (!${coreAuto} && !${Spawn[${Target.ID}].Type.Equal[CORPSE]}) /squelch /target clear
/return TRUE



|***
 *	DES: Clears all defined timers for a specific ID
 *	USE: /call clear_timer [TARGETID]
 *  NOTE: timers that need clearing when the target is deaded
 ***|
sub clear_timer(int _tmpID)
	CHECKINPUTS
	/if (!${_tmpID}) /return

	| mezed/resist timer for enchanters/necros/bards
	/if (${Defined[timer_mez_${_tmpID}]}) /deletevar timer_mez_${_tmpID}
	| slowed/resist timer
	/if (${Defined[timer_slow_${_tmpID}]}) /deletevar timer_slow_${_tmpID}
	| tash/resist timer
	/if (${Defined[timer_tash_${_tmpID}]}) /deletevar timer_tash_${_tmpID}
	| cripple/resist timer
	/if (${Defined[timer_cripple_${_tmpID}]}) /deletevar timer_cripple_${_tmpID}
	| eradicate magic for rangers, druids, and INT casters
	/if (${Defined[timer_eradicate_${_tmpID}]}) /deletevar timer_eradicate_${_tmpID}
	| dont want to keep snaring the same mob
	/if (${Defined[timer_snare_${_tmpID}]}) /deletevar timer_snare_${_tmpID}
	| druid Ro debuff
	/if (${Defined[timer_ro_${_tmpID}]}) /deletevar timer_ro_${_tmpID}
	| stick to mobs ass check
	/if (${Defined[timer_snap_recheckID_${_tmpID}]}) /deletevar timer_snap_recheckID_${_tmpID}
	| druid ro timer
	/if (${Defined[timer_ro_${_tmpID}]}) /deletevar timer_ro_${_tmpID}


/return



|***
 *	DES: conditions to check for each cast
 *	USE: /if (!${cast_condition[CONDITION]}) /return ??
 *  NOTE: DO NOT PASS VARIABLE CONTAINER. JUST PASS ITS' 
 ***|
sub cast_condition(str _condition)
	CHECKINPUTS	

	| is the base variable string set to FALSE in the INI? if so. we do not need to check and consider valid
	/if (${${_condition}.Replace[^,$].Equal[FALSE]}) {
		/return TRUE

	| if there is a condition and it does evaluate as TRUE.
	} else /if (${${_condition}.Replace[^,$]}) {
		/return TRUE
		
	| when in doubt, blame your little sister.
	} 

/return FALSE



|***
 *	DES: Cast nukes
 *	USE: /call cast_nuke
 *  NOTE: "I say we take off and nuke the entire site from orbit." --Ripley
 ***|
sub cast_nuke()
	/if (${timer_Nuke_Delay}) /return	
	/call get_assist_target
	/declare _count int local 0
	
	/for _count 1 to 8
		/if (!${Bool[${Nuke${_count}}]}) /continue
		/if (${is_target_dead[${Target2Kill}]}) {
			/if (${Select[${Me.Class.ShortName},BRD]}) /call pulse_stop 3
			/return
		}

		/if (!${Range.Between[1,8:${Int[${_count}]}]}) /continue
		/if (!${cast_condition[Nuke${_count}Condition]}) /continue

		AMIDEAD
		CHECKEXIT
		CHECKINPUTS
		/if (${is_target_dead[${Target2Kill}]}) /return
		/if (${Me.CurrentMana} < ${Spell[${Nuke${_count}}].Mana} || ${Me.PctMana} < 2) /continue 
		
		| bard nukes
		/if (${Select[${Me.Class.ShortName},BRD]}) {
			/if (${validate_cast[FALSE, pulse, "${Nuke${_count}}", ${Target2Kill}]}) /call core_cast2 "${Nuke${_count}}" pulse ${Target2Kill} cast_break

		| everyone else
		} else { 
			/if (${validate_cast[FALSE, spell, "${Nuke${_count}}", ${Target2Kill}]}) /call core_cast2 "${Nuke${_count}}" 0 ${Target2Kill} cast_break
		}
		/call timer_update timer_Nuke_Delay ${tNuke_Delay}

		| maintain the loop of nukes till all are cycled.
		/if (${swNukeLoop}) {
			/delay 10s !${Me.Casting.ID}
			/delay 2.5s
			/delay ${timer_Nuke_Delay}
		}
	/next _count
/return



|***
 *	DES: Cast nuke Lines
 *	USE: /call cast_nuke
 *  NOTE: "The only good bug is a dead bug" -- Would you like to know more?
 ***|
sub cast_nuke_line()
	/if (${lineCurrent.Equal[off]}) /return
	/if (${timer_Nuke_Delay}) /return	
	/call get_assist_target
	/declare _count int local 0
	/declare _list string local ${line${lineCurrent}}
	
	/for _count 1 to ${_list.Count[|]}
		/if (!${Range.Between[1,${_list.Count[|]}:${Int[${_count}]}]}) /continue
		AMIDEAD
		CHECKEXIT
		CHECKINPUTS
		
		/if (${is_target_dead[${Target2Kill}]}) /return
		/if (${Me.CurrentMana} < ${Spell[${_list.Arg[${_count},|]}].Mana} || ${Me.PctMana} < 2) /break 
		/if (!${cast_condition[line${lineCurrent}Condition${_count}]}) /continue 
		/if (!${Me.SpellReady[${Spell[${_list.Arg[${_count},|]}].RankName}]}) /continue
		
		/if (${validate_cast[FALSE, spell, "${Spell[${_list.Arg[${_count},|]}].RankName}", ${Target2Kill}]}) /call core_cast2 "${Spell[${_list.Arg[${_count},|]}].RankName}" 0 ${Target2Kill} cast_break
		| SDEBUG cast return for: ${Spell[${_list.Arg[${_count},|]}].RankName} .. ${castReturn}
		/call timer_update timer_Nuke_Delay ${tNuke_Delay}

		| toons ini switch. maintain the loop of nuked till all are cycled.
		/if (${swNukeLoop}) {
			/delay 10s !${Me.Casting.ID}
			/delay 2.5s
			/delay ${timer_Nuke_Delay}
		}
	/next _count
/return


|***
 *	DES: memorize spell line
 *	USE: /call mem_nuke_line [] 
 *  NOTE:
 ***|
sub mem_nuke_line(string _line)
	AMIDEAD
	CHECKEXIT
	CHECKINPUTS
	/declare _index 				int 		local 0
	/declare _list		 			string 	local ${line${lineCurrent}}
	
	| mem twincast spell
	/if (!${Me.Gem[1].Name.Equal[${NukeTwincast${lineCurrent}}]} && ${Bool[${NukeTwincast${lineCurrent}}]}) {
		/call mem_spell TRUE "${NukeTwincast${lineCurrent}}" 1 FALSE
	}	
	
	| mem remainder line of spells
	/for _index 1 to ${_list.Count[|]}
		/if (${Me.Gem[${_index}].Name.Equal[${Spell[${_list.Arg[${_index},|]}].RankName}]}) /continue
		/call mem_spell TRUE "${Spell[${_list.Arg[${_index},|]}].RankName}" ${Int[${Math.Calc[${_index}+1]}]} FALSE
	/next _index	
	VOUT Line ${sep} ${cinfo}${lineCurrent}\ax memorization completed.
/return



|***
 *	DES: Cast DoT's
 *	USE: /call cast_DOT
 *  NOTE: 
 ***|
sub cast_DOT()
	/call get_assist_target
	/declare _count int local 0
	/for _count 1 to 8
		/if (!${Range.Between[1,8:${Int[${_count}]}]}) /continue
		/if (!${Bool[${DoT${_count}}]}) /continue
		AMIDEAD
		CHECKEXIT
		CHECKINPUTS
		/if (${is_target_dead[${Target2Kill}]}) {
			/if (${Select[${Me.Class.ShortName},BRD]}) /call pulse_stop 4
			/return
		}
		/if (!${cast_condition[DoT${_count}Condition]}) /continue 
		/if (${Me.CurrentMana} < ${Spell[${DoT${_count}}].Mana} || ${Me.PctMana} < 2) /continue 

		/if (${Target.BuffDuration[${DoT${_count}chk}].TotalSeconds} >= 3 && ${Spawn[${Target.Buff[${DoT${_count}chk}].Caster}].ID} == ${Me.ID}) /continue
		/if (${Target.Buff[${DoT${_count}chk}].ID}) /continue

		| bard dots
		/if (${Select[${Me.Class.ShortName},BRD]}) {
			/if (${validate_cast[FALSE, pulse, "${DoT${_count}}", ${Target2Kill}]}) /call core_cast2 "${DoT${_count}}" pulse ${Target2Kill} cast_break

		| all other dots
		} else {
			/if (${validate_cast[FALSE, spell, "${DoT${_count}}", ${Target2Kill}]}) /call core_cast2 "${DoT${_count}}" 0 ${Target2Kill} cast_break
		}

	/next _count
/return



|***
 *	DES: casts AE Rain spells. Magician mostly...
 *	USE: /call cast_nuke_rain | /userain [TRUE|FALSE]
 *  NOTE: I'd suggest not using this. though it is kind of fun to watch
 ***|
sub cast_nuke_rain()
	/if (${timer_Nuke_Delay}) /return	
	/call get_assist_target
	/declare _count int local 0
	/for _count 1 to ${doRainNuke}
		AMIDEAD
		CHECKEXIT
		CHECKINPUTS
		/if (${is_target_dead[${Target2Kill}]}) /return

		/if (${Spawn[${Target2Kill}].LineOfSight}) {
			/if (${validate_cast[FALSE, spell, "${RainNuke${_count}}", ${Target2Kill}]}) /call core_cast2 "${RainNuke${_count}}" 0 ${Target2Kill} cast_break
			/if (${Select[${castReturn},CAST_SUCCESS]}) /call timer_update timer_Nuke_Delay ${tNuke_Delay}
		}
	/next _count
/return



|***
 *	DES: Returns the toons target to the intended kill ID.
 *	USE: /call get_target_2kill
 *  NOTE: sometimes people need focus.
***|
sub get_target_2kill()
	AMIDEAD
	CHECKEXIT
	CHECKINPUTS

	/if (${is_target_dead[${Target2Kill}]}) /return
	/if (${Target.ID} != ${Target2Kill} && ${Spawn[id ${Target2Kill}].Distance} <= ${setMobAgro}) {
		/squelch /target ID ${Target2Kill}
	}	

/return



|***
 *	DES: Uses Entropy of Nature/EradicateMagic/Whatever on target. only once every 60 seconds.
 *	USE: /call cast_AA_eradicate_magic [target ID] & /debuff eradicate
 *  NOTE: 
 ***|
sub cast_AA_eradicate_magic(int _tmpID)
	/if (${timer_eradicate_${_tmpID}}) /return
	AMIDEAD
	CHECKEXIT
	CHECKINPUTS

	/if (!${lstClassAAEradicate.Find[${Me.Class.ShortName}]}) {
		VOUT ${onotice} ${sep} Class does not have the AA Eradicate.
		/call set_switch ${chrID} swdeBuffEradicate FALSE
		/call echos switch "deBuff Eradicate" swdeBuffEradicate	0 0
		/return
	}
	
	/if (!${Defined[_debuffEradicate]}) /declare _debuffEradicate string local
	/if (${validate_class[FALSE, |ENC|MAG|NEC|WIZ]}) /varset _debuffEradicate Eradicate Magic
	/if (${validate_class[FALSE, |RNG|DRU]}) /varset _debuffEradicate Entropy of Nature
	
	/if (${validate_cast[FALSE, alt, "${_debuffEradicate}", ${_tmpID}]}) /call core_cast2 "${_debuffEradicate}" alt ${_tmpID} FALSE
	/if (${Select[${castReturn},CAST_SUCCESS,CAST_IMMUNE,CAST_RESISTED,CAST_RESIST,CAST_NOHOLD,CAST_TAKEHOLD]}) /call timer_update timer_eradicate_${_tmpID} 1m

/return



|***
 *	DES: Uses class specific AA snares
 *	USE: /call cast_AA_snare [target ID] & /debuff snare
 *  NOTE: did I mention this is for classes with AA snares..
 ***|
sub cast_AA_snare(int _tmpID)
	/if (${timer_snare_${_tmpID}}) /return
	AMIDEAD
	CHECKEXIT

	/if (!${Select[${Me.Class.ShortName},RNG,DRU,WAR,ROG,NEC,SHD,WIZ,MNK,PAL,BER]}) {
		VOUT ${onotice} ${sep} Not an AA snaring class. Delete toon and reroll a ranger.
		/call set_switch ${chrID} swdeBuffSnare FALSE
		/call echos debuff "Snare" swdeBuffSnare 0 0	
		/return
	}
	
	| if its snared, move on
	/if (${Target.Snared.ID}) {
		/call timer_update timer_snare_${Target2Kill} 1m
		/return
	}

	| Berzerkers
	/if (${Select[${Me.Class.ShortName},BER]}) {
		/if (${validate_cast[FALSE, disc, "${DiscSnare}", ${_tmpID}]}) /call core_cast2 "${DiscSnare}" disc ${_tmpID} FALSE

	| Rangers and Druids
	} else /if (${Select[${Me.Class.ShortName},DRU,RNG]}) {
		/if (${validate_cast[FALSE, alt, "Entrap", ${_tmpID}]}) /call core_cast2 "Entrap" alt ${_tmpID} FALSE

	| Monks
	} else /if (${Select[${Me.Class.ShortName},MNK]}) {
		/if (${validate_cast[FALSE, alt, "Crippling Strike", ${_tmpID}]}) /call core_cast2 "Crippling Strike" alt ${_tmpID} FALSE

	| Necromancers and Shadowknights
	} else /if (${Select[${Me.Class.ShortName},NEC,SHD]}) {
		/if (${validate_cast[FALSE, alt, "Encroaching Darkness", ${_tmpID}]}) /call core_cast2 "Encroaching Darkness" alt ${_tmpID} FALSE

	| Rogues
	} else /if (${Select[${Me.Class.ShortName},ROG]}) {
		/if (${validate_cast[FALSE, alt, "Ligament Slice", ${_tmpID}]}) /call core_cast2 "Ligament Slice" alt ${_tmpID} FALSE

	| Warriors
	} else /if (${Select[${Me.Class.ShortName},WAR]}) {
		/if (${validate_cast[FALSE, alt, "Call of Challenge", ${_tmpID}]}) /call core_cast2 "Call of Challenge" alt ${_tmpID} FALSE

	| Wizards
	} else /if (${Select[${Me.Class.ShortName},WIZ]}) {
		/if (${validate_cast[FALSE, alt, "Atol's Shackles", ${_tmpID}]}) /call core_cast2 "Atol's Shackles" alt ${_tmpID} FALSE

	| Paladins
	}	else /if (${Select[${Me.Class.ShortName},PAL]}) {
		/if (${Target.Body.Name.Equal[Undead]}) {
			/if (${validate_cast[FALSE, alt, "Halt the Dead", ${_tmpID}]}) /call core_cast2 "Halt the Dead" alt ${_tmpID} FALSE
		} else /if (${Target.Body.Name.NotEqual[Undead]}) {
			/if (${validate_cast[FALSE, alt, "Shackles of Tunare", ${_tmpID}]}) /call core_cast2 "Shackles of Tunare" alt ${_tmpID} FALSE
		}
	}
		
	| set timers on cast results
	/if (${Select[${castReturn},CAST_SUCCESS,CAST_RESISTED,CAST_RESIST,CAST_NOHOLD,CAST_TAKEHOLD]}) {
		/call timer_update timer_snare_${_tmpID} 1m
	} else /if (${Select[${castReturn},CAST_IMMUNE]}) {
		/call timer_update timer_snare_${_tmpID} 4m
	}

/return



|***
 *	DES: Users class specific Breastplates. Will alternate 2 if you have both. 
 *	USE: /call click_BP
 *  NOTE: 
 ***|
sub click_BP()
	AMIDEAD
	CHECKEXIT
	CHECKINPUTS
	
	/if (${Select[${Me.Class.ShortName},BRD]}) /call pulse_stop 5
	
	| is at least one ready?
	/if (!${Me.ItemReady[${Breastplate1}]} && !${Me.ItemReady[${Breastplate2}]}) /return
	
	| do I have one of the effects?
	/if (${Bool[${Me.Song[${Breastplate1effect}]}]} || ${Bool[${Me.Buff[${Breastplate2effect}]}]}) /return
	
	/if (${Me.ItemReady[${Breastplate1}]}) {
		/if (!${cast_condition[Breastplate1Condition]}) /return
		/if (!${Bool[${Me.Buff[${Breastplate2effect}]}]} && ${Spell[${Breastplate1effect}].NewStacks}) {
			/if (${validate_cast[FALSE, item, "${Breastplate1}", ${Me.ID}]}) /call core_cast2 "${Breastplate1}" item 0 FALSE
		}
	} else /if (${Me.ItemReady[${Breastplate2}]}) {
		/if (!${cast_condition[Breastplate2Condition]}) /return
		/if (!${Bool[${Me.Song[${Breastplate1effect}]}]} && ${Spell[${Breastplate2effect}].NewStacks}) {
			/if (${validate_cast[FALSE, item, "${Breastplate2}", ${Me.ID}]}) /call core_cast2 "${Breastplate2}" item 0 FALSE
		}
	}

/return



|***
 *	DES: Uses a mod rod if you have one in your inventory
 *	USE: /call use_mod_rod
 *  NOTE: listed all i could think of. dont want to keep up with this in an INI.
 ***|
sub use_mod_rod()
	/if (${Select[${Me.Class.ShortName},ROG,BER,WAR,MNK]}) /return
	AMIDEAD
	CHECKEXIT
	CHECKINPUTS
		
	| use all the rods!!
	/if (${Me.PctMana} < 70*.${tmpHealOverride}) {
		/if (${Me.ItemReady[Summoned: Glowing Modulation Shard]} && ${Me.CurrentHPs} > 45500) { 
			/if (${validate_cast[FALSE, item, "Summoned: Glowing Modulation Shard", ${Me.ID}]}) /call core_cast2 "Summoned: Glowing Modulation Shard" item 0 FALSE
		} else /if (${Me.ItemReady[Summoned: Giant Modulation Shard]} && ${Me.CurrentHPs} > 37500) {
			/if (${validate_cast[FALSE, item, "Summoned: Giant Modulation Shard", ${Me.ID}]}) /call core_cast2 "Summoned: Giant Modulation Shard" item 0 FALSE
		} else /if (${Me.ItemReady[Summoned: Large Modulation Shard]} && ${Me.CurrentHPs} > 23100) {
			/if (${validate_cast[FALSE, item, "Summoned: Large Modulation Shard", ${Me.ID}]}) /call core_cast2 "Summoned: Large Modulation Shard" item 0 FALSE
		} else /if (${Me.ItemReady[Summoned: Medium Modulation Shard]} && ${Me.CurrentHPs} > 15100) {
			/if (${validate_cast[FALSE, item, "Summoned: Medium Modulation Shard", ${Me.ID}]}) /call core_cast2 "Summoned: Medium Modulation Shard" item 0 FALSE
		} else /if (${Me.ItemReady[Summoned: Small Modulation Shard]} && ${Me.CurrentHPs} > 7600) {
			/if (${validate_cast[FALSE, item, "Summoned: Small Modulation Shard", ${Me.ID}]}) /call core_cast2 "Summoned: Small Modulation Shard" item 0 FALSE
		} else /if (${Me.ItemReady[Summoned: Modulating Rod]} && ${Me.CurrentHPs} > 325) {
			/if (${validate_cast[FALSE, item, "Summoned: Modulating Rod", ${Me.ID}]}) /call core_cast2 "Summoned: Modulating Rod" item 0 FALSE
		} else /if (${Me.ItemReady[Wand of Pelagic Transvergence]} && ${Me.CurrentHPs} > 12850) {
			/if (${validate_cast[FALSE, item, "Wand of Pelagic Transvergence", ${Me.ID}]}) /call core_cast2 "Wand of Pelagic Transvergence" item 0 FALSE
		} else /if (${Me.ItemReady[Wand of Phantasmal Modulation]} && ${Me.CurrentHPs} > 11100) {
			/if (${validate_cast[FALSE, item, "Wand of Phantasmal Modulation", ${Me.ID}]}) /call core_cast2 "Wand of Phantasmal Modulation" item 0 FALSE 
		} else /if (${Me.ItemReady[Wand of Pelagic Transvergence]} && ${Me.CurrentHPs} > 3594) {
			/if (${validate_cast[FALSE, item, "Wand of Pelagic Transvergence", ${Me.ID}]}) /call core_cast2 "Wand of Pelagic Transvergence" item 0 FALSE
		} else /if (${Me.ItemReady[Wand of Phantasmal Transvergence]} && ${Me.CurrentHPs} > 3138) {
			/if (${validate_cast[FALSE, item, "Wand of Phantasmal Transvergence", ${Me.ID}]}) /call core_cast2 "Wand of Phantasmal Transvergence" item 0 FALSE
		} else /if (${Me.ItemReady[Wand of Temporal Mastery]} && ${Me.CurrentHPs} > 1686) {
			/if (${validate_cast[FALSE, item, "Wand of Temporal Mastery", ${Me.ID}]}) /call core_cast2 "Wand of Temporal Mastery" item 0 FALSE
		} else /if (${Me.ItemReady[Wand of Elemental Transvergence]} && ${Me.CurrentHPs} > 920) {
			/if (${validate_cast[FALSE, item, "Wand of Elemental Transvergence", ${Me.ID}]}) /call core_cast2 "Wand of Elemental Transvergence" item 0 FALSE
		} else /if (${Me.ItemReady[Rod of Arcane Transvergence]} && ${Me.CurrentHPs} > 2530) {
			/if (${validate_cast[FALSE, item, "Rod of Arcane Transvergence", ${Me.ID}]}) /call core_cast2 "Rod of Arcane Transvergence" item 0 FALSE
		} else /if (${Me.ItemReady[Rod of Spectral Transvergence]} && ${Me.CurrentHPs} > 1720) {
			/if (${validate_cast[FALSE, item, "Rod of Spectral Transvergence", ${Me.ID}]}) /call core_cast2 "Rod of Spectral Transvergence" item 0 FALSE
		} else /if (${Me.ItemReady[Rod of Ethereal Transvergence]} && ${Me.CurrentHPs} > 1320) {
			/if (${validate_cast[FALSE, item, "Rod of Ethereal Transvergence", ${Me.ID}]}) /call core_cast2 "Rod of Ethereal Transvergence" item 0 FALSE
		} else /if (${Me.ItemReady[Rod of Mechamagical Mastery]} && ${Me.CurrentHPs} > 1400) {
			/if (${validate_cast[FALSE, item, "Rod of Mechamagical Mastery", ${Me.ID}]}) /call core_cast2 "Rod of Mechamagical Mastery" item 0 FALSE
		} else /if (${Me.ItemReady[Rod of Prime Transvergence]} && ${Me.CurrentHPs} > 1000) {
			/if (${validate_cast[FALSE, item, "Rod of Prime Transvergence", ${Me.ID}]}) /call core_cast2 "Rod of Prime Transvergence" item 0 FALSE
		} else /if (${Me.ItemReady[Rod of Mystical Transvergence]} && ${Me.CurrentHPs} > 550) {
			/if (${validate_cast[FALSE, item, "Rod of Mystical Transvergence", ${Me.ID}]}) /call core_cast2 "Rod of Mystical Transvergence" item 0 FALSE
		}
	}
	
	| ask for a rod cause i'm fu'kn lazy...
	/if (!${Select[${Me.Class.ShortName},MAG]} && !${Bool[${FindItem[Modulation Shard]}]}) {
		/call tell_resident_mage shard
	}	

/return



|***
 *	DES: Send Pet to attack
 *	USE: /call send_pet
 *  NOTE: 
 ***| 
sub send_pet(int _tmpID)
	/if (!${coreAuto}) /return
	/if (!${swPet}) /return
	AMIDEAD
	CHECKEXIT
	CHECKINPUTS
	/if (${Spawn[${Target2Kill}].Distance} > ${setMobAgro} || ${Spawn[${Target.ID}].Type.Equal[corpse]} || ${Target.Dead}) /return
	
	/if (!${Target2Kill}) {
		/call get_assist_target
		/if (!${validate_target[FALSE, ${Target2Kill}]}) /return
	}

	/squelch /target ID ${Target2Kill}
	
	/if (${swPet}) {
		/if (${Me.Pet.ID} && (${Target.PctHPs} <= ${setPetEngagePct} || ${swOverrideEngagePct}) && !${Bool[${Me.Pet.Target}]}) {
			/call echos petfarm "filler" ${Target2Kill} 0 0
			/pet attack
		}
	}
	
	| send in the swarm
	/if (${swSwarm} && (${Target.PctHPs} <= ${setSwarmEngagePct} || ${swOverrideEngagePct})) /pet swarm

/return



|***
 *	DES: Will use a glyph and buy it if able.
 *	USE: /call use_glyph
 *  NOTE: 
 ***|
sub use_glyph()
	AMIDEAD
	CHECKINPUTS
	/if (!${Bool[${useGlyph}]} || !${Raid.Members} && !${swGlyphOutsideRaid}) /return
	/if (!${Me.AltAbility[${useGlyph}].ID} && ${Me.AAPoints} >= ${AltAbility[${useGlyph}].Cost}) /alt buy ${AltAbility[${useGlyph}].ID}
	/if (${validate_cast[FALSE, alt, "${useGlyph}", ${Me.ID}]}) /call core_cast2 "${useGlyph}" alt 0 FALSE
	/if (!${Me.AltAbility[${useGlyph}].ID} && ${Me.AAPoints} >= ${AltAbility[${useGlyph}].Cost}) /alt buy ${AltAbility[${useGlyph}].ID}

/return



|***
 *	DES: will halt spell casting and force Gift of Manna nukes if setup
 *	USE: 
 *  NOTE: this is checked on most spell casts. prevents doing stupid shit.
 ***|
sub cast_break()
	AMIDEAD
	CHECKINPUTS
	/if (${Me.Zoning}) /varset castBreak CAST_BREAK

	/if (${Spell[${Me.Casting.ID}].Category.Equal[Transport]}) /return FALSE
	/if (${Spell[${Me.Casting.ID}].Category.Equal[Heal]}) /return FALSE
	
	/if (${Target.PctHPs} <= ${Int[${setStopNukePct}]}) /varset castBreak CAST_BREAK
	
	/if (${swAmIDead}) /varset castBreak CAST_BREAK
	/if (${swNeedRez}) /varset castBreak CAST_BREAK
		
	/if (${Target.Mezzed.ID} || !${Spawn[${Target2Kill}].ID} || !${Bool[${Target2Kill}]}) {
		/varset castBreak CAST_BREAK
	| } else /if (${Bool[${Me.Song[Gift of Mana (${GoMLevel})]}]} && ${Bool[${combat}]} && (${Bool[${GoMNuke}]} || ${Bool[${GoMNuke2}]})) {
	} else /if (${Bool[${Me.Song[Gift of Mana]}]} && ${Bool[${combat}]} && (${Bool[${GoMNuke}]} || ${Bool[${GoMNuke2}]})) {
		/stopcast
		/call cast_nuke_GoM ${Target2Kill}
	}

/return ${castBreak}



|***
 *	DES: Cures/removes debuffs, we hope...
 *	USE: 
 *  NOTE: 
 ***|
sub cast_cure()
	/call timer_update timer_check_cast_cure ${tCast_Cure}
	AMIDEAD
	CHECKINPUTS

	UPDATEHUD "HEAL Cure"

	| if i'm fine skip to everyone else
	/if (!${Debuff.Counters}) {
		/if (!${Bool[${Group.Members}]} || ${swPull} || ${swAgro}) /return
	} else /if (${Debuff.Counters}) {
		
		| Monk
		/if (${Select[${Me.Class.ShortName},MNK]}) {
			/if (${validate_cast[FALSE, alt, "Purify Body", ${Me.ID}]}) /call core_cast2 "Purify Body" alt 0 FALSE
		| paladin, Cleric, Shaman, Druid
		} else /if (${Select[${Me.Class.ShortName},PAL,CLR,SHM,DRU]} && ${Me.AltAbilityReady[Radiant Cure]}) {
			/if (${Debuff.Curses} || ${Debuff.Poisons} || ${Debuff.Diseases}) {
				/if (${validate_cast[FALSE, alt, "Radiant Cure", ${Me.ID}]}) /call core_cast2 "Radiant Cure" alt 0 FALSE
			}
			| paladin splash
			/if (${Select[${Me.Class.ShortName},PAL]}) {
				/call Bind_pal_splash
			}
			
		| Necromancer
		} else /if (${Select[${Me.Class.ShortName},NEC]} && ${Me.AltAbilityReady[Embrace the Decay]}) {
			/if (${Debuff.Curses} || ${Debuff.Poisons} || ${Debuff.Diseases} || ${Debuff.Corruptions}) {
				/if (${validate_cast[FALSE, alt, "Embrace the Decay", ${Me.ID}]}) /call core_cast2 "Embrace the Decay" alt 0 FALSE
			}
		}
		
		/if (${Debuff.Curses} && ${Me.ItemReady[${cureCurseItem}]}) {
			/if (${validate_cast[FALSE, item, "${cureCurseItem}", ${Me.ID}]}) /call core_cast2 "${cureCurseItem}" item ${Me.ID} FALSE
		}
		
		/if (${Debuff.Corruptions} && ${Me.ItemReady[${cureCorruptItem}]}) {
			/if (${validate_cast[FALSE, item, "${cureCorruptItem}", ${Me.ID}]}) /call core_cast2 "${cureCorruptItem}" item ${Me.ID} FALSE
		}
		
		/if (${Debuff.Poisons} && ${Me.ItemReady[${curePoisonItem}]}) {
			/if (${validate_cast[FALSE, item, "${curePoisonItem}", ${Me.ID}]}) /call core_cast2 "${curePoisonItem}" item ${Me.ID} FALSE
		}
		
		/if (${Debuff.Diseases} && ${Me.ItemReady[${cureDiseaseItem}]}) {
			/if (${validate_cast[FALSE, item, "${cureDiseaseItem}", ${Me.ID}]}) /call core_cast2 "${cureDiseaseItem}" item ${Me.ID} FALSE
		}
	}

	/declare _count int local 0
	/declare _toonName string local
	/declare _DNqout string local
	
	| each toon
	/for _count 1 to ${DanNet[${DanNetMacroChannel}].PeerCount}
		AMIDEAD
		CHECKINPUTS
		
		| get the name to something usable
		/varset _toonName ${${DanNet[${DanNetMacroChannel}].Peers}.Arg[${_count},|]}
				
		| toon here?
		/if (!${Spawn[pc ${_toonName}].ID}) /continue
			
		| toon dead?
		/if (${Spawn[pc ${_toonName}].Dead}) /continue
	
		| already checked me
		/if (${_toonName.Equal[${Me.DisplayName}]}) /continue		

		| are they debuffed?
		/dquery ${_toonName} -q "Debuff" -o _DNqout -t 1s 
		/if (!${_DNqout}) /continue		

		| AA Radient Cure 
		| 100' 1 min
		/if (${validate_class[FALSE, |PAL|SHM|CLR|DRU]} && ${Me.AltAbilityReady[Radiant Cure]}) {
			/if (${validate_cast[FALSE, alt, "Radiant Cure", ${Me.ID}]}) /call core_cast2 "Radiant Cure" alt 0 FALSE
			/continue
		}

		| AA Purify Soul
		| 200' 5 min
		/if (${validate_class[FALSE, |PAL|CLR|DRU]} && ${Me.AltAbilityReady[Purify Soul]}) {
			/if (${validate_cast[FALSE, alt, "Purify Soul", ${Spawn[pc ${_toonName}].ID}]}) /call core_cast2 "Purify Soul" alt ${Spawn[pc ${_toonName}].ID} FALSE
			/continue
		}
		
		| AA Group Purify Soul . were in deep shit if we've gotten this far.
		| 0/100' 15 min
		/if (${validate_class[FALSE, |CLR|DRU]} && ${Me.AltAbilityReady[Group Purify Soul]}) {
			/if (${validate_cast[FALSE, alt, "Group Purify Soul", ${Me.ID}]} && ${Spawn[pc ${_toonName}].Distance} <= 100) /call core_cast2 "Group Purify Soul" alt 0 FALSE
			/continue
		}
		
		| paladin splash 50' range
		/if (${Select[${Me.Class.ShortName},PAL]}) {
			/call Bind_pal_splash
		}
		
		| cure with spell
		
		/if (${Bool[${cureCurse}]}) {
			/dquery ${_toonName} -q "Debuff.Curses" -o _DNqout -t 1s 
			/if (${_DNqout} && ${validate_cast[FALSE, spell, "${cureCurse}", ${Spawn[pc ${_toonName}].ID}]}) /call core_cast2 "${cureCurse}" ${buffGem} ${Spawn[pc ${_toonName}].ID} FALSE
		}
		
		/if (${Bool[${cureCorrupt}]}) {
			/dquery ${_toonName} -q "Debuff.Corruptions" -o _DNqout -t 1s 
			/if (${_DNqout} && ${validate_cast[FALSE, spell, "${cureCorrupt}", ${Spawn[pc ${_toonName}]}]}) /call core_cast2 "${cureCorrupt}" ${buffGem} ${Spawn[pc ${_toonName}].ID} FALSE
		}
		
		/if (${Bool[${curePoison}]}) {
			/dquery ${_toonName} -q "Debuff.Poisons" -o _DNqout -t 1s 
			/if (${_DNqout} && ${validate_cast[FALSE, spell, "${curePoison}", ${Spawn[pc ${_toonName}].ID}]}) /call core_cast2 "${curePoison}" ${buffGem} ${Spawn[pc ${_toonName}].ID} FALSE
		}
		
		/if (${Bool[${cureDisease}]}) {
			/dquery ${_toonName} -q "Debuff.Diseases" -o _DNqout -t 1s 
			/if (${_DNqout} && ${validate_cast[FALSE, spell, "${cureDisease}", ${Spawn[pc ${_toonName}].ID}]}) /call core_cast2 "${cureDisease}" ${buffGem} ${Spawn[pc ${_toonName}].ID} FALSE
		}

	/next _count
	
	UPDATEHUD FALSE

/return



|***
 *	DES:targets and casts a free targeting area spell.
 *	USE: /call cast_free_target [SPELL] [TARGET ID]
 *  NOTE: i could never get this to work right...
 ***|
sub cast_free_target(string _tmpSpell, int _tmpID)
	/if (${validate_cast[FALSE, spell, "${_tmpSpell}", ${_tmpID}) /call core_cast2 "${_tmpSpell}" 0 ${_tmpID} FALSE
	/squelch /face ID ${_tmpID} ${If[${swFaceFast},fast,]}
	/delay 5
	/click left center
	/delay 4s !${Me.Casting.ID}

/return



|***
 *	DES: use base melee discs
 *	USE: /call use_skill_melee TARGET
 *  NOTE: 
 ***|
sub use_skill_melee(int _tmpID)
 	/if (!${swCombatMelee}) /return
	AMIDEAD
	CHECKEXIT
	CHECKINPUTS
	
	/if (!${Bool[${_tmpID}]}) /return	
	/if (${is_target_dead[${_tmpID}]}) /return
	
	/if (${Target.ID} != ${_tmpID}) {
		/squelch /target ID ${_tmpID}
		/delay 5 ${Target.ID} == ${_tmpID}
	}
	
	/if (${coreAuto}) {
		/if (${Target.ID} && !${Range.Between[0,${setFaceArc}:${Math.Abs[${Math.Calc[${Target.HeadingTo.Degrees}-${Me.Heading.Degrees}]}]}]}) /squelch /face ID ${_tmpID} nolook ${If[${swFaceFast},fast,]}
	}

	| taunt /onoff agro
	| /if (${validate_class[FALSE, |WAR|PAL|SHD|RNG]}) {
	/if (${taunt} && ${swAgro}) {
		/if (${Me.AbilityReady[Taunt]} && ${Spawn[${_tmpID}].Distance} <= 18 && ${Me.TargetOfTarget.Name.NotEqual[${Me.DisplayName}]}) {
			VMELEE Melee ${sep} ${csp}Taunting\ax ${sep} [${ctar}${Spawn[${_tmpID}].DisplayName}\ax]
			/if (${Target.ID}) /doability Taunt
		}
	}
	
	| disarm
	/if (${Bool[${disarm}]} && !${Skill[Disarm].Auto}) {
		/if (${Me.AbilityReady[Disarm]} && ${Spawn[${_tmpID}].Distance} <= 15 && ${swAgro}) {
			VMELEE Melee ${sep} ${csp}Disarm\ax ${sep} [${ctar}${Spawn[${_tmpID}].DisplayName}\ax]
			/if (${Target.ID}) /doability Disarm
		}
	}
		
	| kick
	/if (${Bool[${kick}]} && !${Skill[Kick].Auto}) {
	 	/if (!${Select[${Me.Class.ShortName},MNK]} && ${Me.AbilityReady[Kick]} && ${Spawn[${_tmpID}].Distance} <= 15) {
			VMELEE Melee ${sep} ${csp}Kick\ax ${sep} [${ctar}${Spawn[${_tmpID}].DisplayName}\ax]
			/if (${Target.ID}) /doability Kick
		}
	}
	
	| bash
	/if (${Bool[${bash}]} && !${Skill[Bash].Auto}) {
		/if (${Me.AbilityReady[Bash]} && ${Spawn[${_tmpID}].Distance} <= 15) {
			VMELEE Melee ${sep} ${csp}Bash\ax ${sep} [${ctar}${Spawn[${_tmpID}].DisplayName}\ax]
			/if (${Bool[${swBandolier}]} && ${Bool[${bandolierBash}]} && ${Bool[${bandolierNormal}]}) {
				/if (${Target.ID}) {
					/call bandolier_swap Bash
					/doability Bash
					/call bandolier_swap Normal
				}
			} else /if (!${Bool[${swBandolier}]}) {
				/if (${Target.ID}) /doability Bash
			}
		}
	}	
	
	| Intimidation
	/if (${validate_class[FALSE, |BRD|ROG|BER]}) {
		/if (${Bool[${intimidation}]}) {
			/if (${Me.AbilityReady[Intimidation]} && ${Spawn[${_tmpID}].Distance} <= 15) {
				VMELEE Melee ${sep} ${csp}Intimidation\ax ${sep} [${ctar}${Spawn[${Target2Kill}].DisplayName}\ax]
				/if (${Target.ID}) /doability Intimidation		
			}
		}
	}

	| Zerker only skill
	/if (${validate_class[FALSE, |BER]}) {
		/if (${Bool[${frenzy}]} && && !${Skill[Frenzy].Auto}) {
			/if (${Me.AbilityReady[Frenzy]} && ${Spawn[${_tmpID}].Distance} <= 13) {
				VMELEE Melee ${sep} ${csp}Frenzy\ax ${sep} [${ctar}${Spawn[${_tmpID}].DisplayName}\ax]
				/if (${Target.ID}) /doability Frenzy
			}
		}
	}

	| rogue only skill
	/if (${validate_class[FALSE, |ROG]}) { 
		/if (${Bool[${backstab}]} && && !${Skill[Backstab].Auto}) {
			/if (${Me.AbilityReady[Backstab]} && ${Spawn[${_tmpID}].Distance} <= 13) {
				VMELEE Melee ${sep} ${csp}Backstab\ax ${sep} [${ctar}${Spawn[${Target2Kill}].DisplayName}\ax]
				/if (${Target.ID}) /doability Backstab
			}
		}
	}
	
	| monk/bst only skill 
	/if (${validate_class[FALSE, |MNK|BST]} && ${Spawn[${_tmpID}].Distance} <= 15) {
		| fist
		/if (${Bool[${dragonpunch}]} && !${Skill[Dragon Punch].Auto} && ${Me.AbilityReady[Dragon Punch]}) {
			VMELEE Melee ${sep} ${csp}Dragon Punch\ax ${sep} [${ctar}${Spawn[${_tmpID}].DisplayName}\ax]
			/if (${Target.ID}) /doability "Dragon Punch"
		} else /if (${Bool[${eaglestrike}]} && !${Skill[Eagle Strike].Auto} && ${Me.AbilityReady[Eagle Strike]}) {
			VMELEE Melee ${sep} ${csp}Eagle Strike\ax ${sep} [${ctar}${Spawn[${_tmpID}].DisplayName}\ax]
			/if (${Target.ID}) /doability "Eagle Strike"
		} else /if (${Bool[${tigerclaw}]} && !${Skill[Tiger Claw].Auto} && ${Me.AbilityReady[Tiger Claw]}) {
			VMELEE Melee ${sep} ${csp}Tiger Claw\ax ${sep} [${ctar}${Spawn[${_tmpID}].DisplayName}\ax]
			/if (${Target.ID}) /doability "Tiger Claw"
		}
		| kick
	 	/if (${Bool[${kick}]} && !${Skill[Kick].Auto} && ${Me.AbilityReady[Kick]}) {
			VMELEE Melee ${sep} ${csp}Kick\ax ${sep} [${ctar}${Spawn[${_tmpID}].DisplayName}\ax]
			/if (${Target.ID}) /doability Kick
		} else /if (${Bool[${flyingkick}]} && !${Skill[Flying Kick].Auto} && ${Me.AbilityReady[Flying Kick]}) {
			VMELEE Melee ${sep} ${csp}Flying Kick\ax ${sep} [${ctar}${Spawn[${_tmpID}].DisplayName}\ax]
			/if (${Target.ID}) /doability "Flying Kick"
		} else /if (${Bool[${roundkick}]} && !${Skill[Round Kick].Auto} && ${Me.AbilityReady[Round Kick]}) {
			VMELEE Melee ${sep} ${csp}Round Kick\ax ${sep} [${ctar}${Spawn[${_tmpID}].DisplayName}\ax]
			/if (${Target.ID}) /doability "Round Kick"
		}		
	}

/return
 


 |***
 *	DES: single swarm pet
 *	USE: /call cast_nuke_servant
 *  NOTE: 
 ***|
sub cast_nuke_servant()
	/if (!${swSwarm}) /return
	AMIDEAD
	CHECKEXIT
	CHECKINPUTS
	/if (${validate_cast[FALSE, spell, "${ServantNuke}", ${Target2Kill}]}) /call core_cast2 "${ServantNuke}" 0 ${Target2Kill} cast_break
	/if (${validate_cast[FALSE, alt, "${ServantNuke}", ${Target2Kill}]}) /call core_cast2 "${ServantNuke}" alt ${Target2Kill} FALSE

/return



|***
 *	DES: preps toon for combat modes
 *	USE: /call set_combat []
 *  NOTE: 
 ***|
sub set_combat(str _mode)

	| SDEBUG set_combat(\at${_mode}\ax)
	| AMIDEAD
	/if (${is_target_dead[${Target2Kill}]}) /return	
	
	| out of range?
	/if (${Target.Distance} > ${setMobAgro}) /return
	
	UPDATEHUD "${_mode}"
	/if (${Me.State.NotEqual[STAND]}) /stand
	/if (${coreAuto} && ${Me.State.Equal[FEIGN]}) /squelch /stand
	/if (!${coreAuto} && ${Me.Invis}) /makemevisible
		
	| combat	
	/if (${Bool[${_mode}]}) {
		
		| face the mob if meele
		/if (${coreAuto}) {
			/if (${swCombatMelee} || ${swCombatRange}) {
				/if (!${Range.Between[0,${setFaceArc}:${Math.Abs[${Math.Calc[${Target.HeadingTo.Degrees}-${Me.Heading.Degrees}]}]}]}) /squelch /face ID ${Target2Kill} nolook ${If[${swFaceFast},fast,]}
			}
		}

		| stop melee on DS ??
		/if (${coreAuto} && ${Bool[${Target.DSed}]} && ${stoponDS} && !${swAgro}) {
			/call set_stop NOECHO
			/call clear_combat
			/return
		} 
						
		| not within ${combatradius} and far enought to range?
		/if (${Target.Distance} > 30) {
			| SDEBUG  Target.Distance:${Target.Distance}

			| range mobs if enabled
			/if (${swCombatRange}) {
				
				| SDEBUG range
			
				| no need to move were are at a good distance
				/if (${Stick.Active}) /squelch /stick off		
			
				/if (${RangeList.Find[${Target.DisplayName}]}) {
					/if (${Me.AutoFire}) /squelch /autofire
				} else {
					/if (${Spawn[${Target2Kill}].LineOfSight} && !${Me.AutoFire}) /squelch /autofire
				}

			| cannot range, so melee if in agro range?
			} else /if (!${swCombatRange} && ${swCombatMelee}) {
				/if (${NoMeleeList.Find[${Target.DisplayName}]}) {
					/squelch /attack off
					/return
				} else {
					/if (${Me.AutoFire}) /squelch /autofire
					/squelch /attack on
					/if (${coreAuto}) /call stick_to_target ${Target2Kill}
				}

			}

		| mob too close for range
		} else /if (${Target.Distance} <= 31) {
			| SDEBUG melee
	
			| adjust range if melee is diabled to keep close to target. give them some range to.. range again
			/if (${Target.Distance} < 31 && ${swCombatRange} && !${swCombatMelee}) {
				/if (${coreAuto} && !${Stick.Active} && ${Me.TargetOfTarget.Name.NotEqual[${Me.DisplayName}]}) /call moveto_range ${Target2Kill} 35
				
			| can we get to the melee already?
			} else /if (${swCombatMelee}) {
				/if (${NoMeleeList.Find[${Target.DisplayName}]}) {
					/squelch /attack off
					/return FALSE
				} else {
					/if (${Me.AutoFire}) /squelch /autofire
					/squelch /attack on
					/if (${coreAuto}) /call stick_to_target ${Target2Kill}
				}
			}
		}
		
		| face the mob if meele
		/if (${coreAuto}) {
			/if (${swCombatMelee} || ${swCombatRange}) {
				/if (!${Range.Between[0,${setFaceArc}:${Math.Abs[${Math.Calc[${Target.HeadingTo.Degrees}-${Me.Heading.Degrees}]}]}]}) /squelch /face ID ${Target2Kill} nolook ${If[${swFaceFast},fast,]}
			}
		}		
	}

/return



|***
 *	DES: Gift of Manna Nuke
 *	USE: /call cast_nuke_GoM
 *  NOTE: 
 ***|
sub cast_nuke_GoM(int _tmpID)

	CHECKEXIT
	CHECKINPUTS

	| nuke 1
	/if (${Me.SpellReady[${GoMNuke}]}) {
		/if (${Me.CurrentMana} < ${Spell[${GoMNuke}].Mana} || ${Me.CurrentMana} < 2) /return 

		/squelch /target ID ${_tmpID}
		/if (!${cast_condition[GoMNukeCondition]}) /return
		VSPELL GoM ${sep} ${csp}${GoMNuke}\ax ${sep} [${ctar}${Spawn[id ${_tmpID}].DisplayName}\ax]
		/call cast "${Spell[${GoMNuke}].RankName}"
		/call check_cast_events
		/return

	| nuke 2
	} else /if (${Me.SpellReady[${GoMNuke2}]}) {
		/if (${Me.CurrentMana} < ${Spell[${GoMNuke2}].Mana} || ${Me.CurrentMana} < 2) /return 
		/squelch /target ID ${_tmpID}
		/if (!${cast_condition[GoMNuke2Condition]}) /return
		VSPELL GoM Backup ${sep} ${csp}${GoMNuke2}\ax ${sep} [${ctar}${Spawn[id ${_tmpID}].DisplayName}\ax]
		/call cast "${Spell[${GoMNuke2}].RankName}"
		/call check_cast_events
		/return
	}

/return



|***
 *	DES: buffs mercs in your group
 *	USE: /call check_buffs_manual [mercenary|raid]
 *  NOTE: 
 ***|
sub check_buffs_manual(string _type)
	/if (!${coreAuto}) /return
	CHECKCOMBATCAST
	/if (${watch_for_death[]}) /return 

	| # people in group
	/declare _i int local
	| # of buffs to cast
	/declare _x int local
	| targets buff count
	/declare _y int local
	| declare type types
	/declare _GroupRaid string local NULL
	/declare _PCMercenary string local NULL
	/declare _INIline string local NULL
	
	| set type types..
	/if (${_type.Equal[raid]}) {
		/call timer_update timer_check_buffs_raid ${tRaid_Buff}
		/varset _GroupRaid Raid
		/varset _PCMercenary pc
		/varset _INIline Raid
	} else /if (${_type.Equal[group]}) {
		/call timer_update timer_check_buffs_other ${tOther_buff}
		/varset _GroupRaid Group
		/varset _PCMercenary pc
		/varset _INIline Manual
	} else /if (${_type.Equal[mercenary]}) {
		/call timer_update timer_check_buffs_merc ${tMerc_Buff}
		/varset _GroupRaid Group
		/varset _PCMercenary mercenary
		/varset _INIline Manual
	} else {
		/return
	}
		
	/if (${${_GroupRaid}.Members} && ${Me.PctMana} > 10 && !${Me.Casting.ID}) {
		UPDATEHUD "BUFF ${_type}"
		/for _i 0 to ${${_GroupRaid}.Members}
			
			| do not buff self with this mode!!
			/if (${${_GroupRaid}.Member[${_i}].DisplayName.Equal[${Me.DisplayName}]}) /continue
			
			| is the right type of member
			/if (${Spawn[${${_GroupRaid}.Member[${_i}].ID}].Type.NotEqual[${_PCMercenary}]}) /continue
			
			/for _x 1 to ${setMaxBuffCount}
				/if (!${Bool[${Buff${_x}}]}) /continue
				/if (!${Bool[${${_INIline}Buff${_x}Class}]}) /continue
				AMIDEAD
				CHECKEXIT
				CHECKTIE
				CLEARCURSOR TRUE
				
				/if (!${${_INIline}Buff${_x}Class.Find[${Spawn[${${_GroupRaid}.Member[${_i}].ID}].Class.ShortName}]}) /continue 
				/if (!${Spawn[${${_GroupRaid}.Member[${_i}].ID}].Type.Equal[${_PCMercenary}]}) /continue
				/if (${Spawn[${${_GroupRaid}.Member[${_i}].ID}].Distance} > ${setMobAgro} && ${Me.PctMana} <= 20) /continue
	
				| stop what you're doing and get the target
				/if (${Stick.Active}) /squelch /stick off
				/squelch /target ID ${${_GroupRaid}.Member[${_i}].ID}
				/delay 1s ${Target.ID} == ${${_GroupRaid}.Member[${_i}].ID}
				/delay 1s ${Target.BuffCount}
				/delay 1s

				| check the buffs, if it doesnt stack mark it and dont come back
				/for _y 1 to ${Target.BuffCount}	
					AMIDEAD
					/if (${timer_manual_buffs${Spell[${ManualBuff${_x}Check}].ID}${${_GroupRaid}.Member[${_i}].ID}}) /continue
					
					/if (!${Spell[${Target.Buff[${_y}]}].WillStack[${Spell[${Buff${_x}}]}]} || ${Target.Buff[${_y}].ID}) {
						/call timer_update timer_manual_buffs${Spell[${ManualBuff${_x}Check}].ID}${${_GroupRaid}.Member[${_i}].ID} ${Spell[${Spell[${ManualBuff${_x}Check}].ID}].Duration.TotalSeconds}s
					} else {
						/call timer_update timer_manual_buffs${Spell[${ManualBuff${_x}Check}].ID}${${_GroupRaid}.Member[${_i}].ID} 1
						/delay 2
					}
				/next _y

				/if (${Bool[${timer_manual_buffs${Spell[${Buff${_x}}].ID}${Group.Member[${_i}].ID}}]}) /continue
				
				| if target doesnt have the buff, hook em up
				/if (!${Target.Buff[${ManualBuff${_x}Check}].ID}) {
					CLEARCURSOR TRUE
					VSPELL ${_GroupRaid} Buffing ${sep} ${csp}${Buff${_x}}\ax on [${cpc}${${_GroupRaid}.Member[${_i}].DisplayName}\ax]
					/call core_cast2 "${Spell[${Buff${_x}}].RankName}" ${buffGem} ${${_GroupRaid}.Member[${_i}].ID} FALSE
					| /call MQ2Cast "${Spell[${Buff${_x}}].RankName}" ${buffGem} -targetid|${${_GroupRaid}.Member[${_i}].ID} 
				}
			/next _x	
		/next _i
		UPDATEHUD FALSE
	}

/return



|***
 *	DES: preps the queue with target toon
 *	USE: /call cast_buffs_queue [toon name] [buff word]
 *  NOTE:
 ***|
sub cast_buffs_queue(string _sender, string _verbage)
	CHECKINPUTS

	/declare _a int local 0
	/declare _i int local 0

	/for _i 1 to ${setMaxBuffCount}
		/for _a 1 to ${Buff${_i}Alias.Count[|]}
			AMIDEAD
			/if (${_verbage.Find["${Buff${_i}Alias.Arg[${_a},|]}"]}) {
				/call queue_buff_add ${Spawn[pc ${_sender}].ID} ${_i}
			}
		/next _a
	/next _i

/return



 |***
 *	DES: will loosly tie to a specific toon. will moove close to the toon when it moves. set to timer [Timers] -> CheckTieTime
 *	USE: /tie [on/off toggle] requires /tc name [NAME] and /tc distance ##
 *  NOTE: 
 **|
sub set_tie()

	| FORCE EMULATE FOLLOW.. HATE DOING THIS
	/if (${swTieLock}) {
		/if (${swSetTie} && ${Spawn[pc ${setTieToon}].ID}) {
			/if (!${Stick.Active}) {
				/squelch /face ID ${Spawn[pc ${setTieToon}].ID} ${If[${swFaceFast},fast,]}
				/squelch /stick ID ${Spawn[pc ${setTieToon}].ID} ${setTieDistance} UW
			}
			AMIDEAD
			CHECKINPUTS
			/delay 1
		}
		/return
	}
	
	/if (${timer_check_Tie}) /return
	/if (${coreAuto}) /call timer_update timer_check_Tie ${tCheck_Tie}

	AMIDEAD
	CHECKINPUTS

	| reasons to not tie to a toon
	/if (${swSetTie} && ${Spawn[pc ${setTieToon}].ID} && ${Spawn[pc ${setTieToon}].Dead}) /return
	/if (${Me.Combat} && ${swCombatTie}) /return
	/if (${Spawn[pc ${setTieToon}].Distance} <= ${setTieDistance} || !${Spawn[pc ${setTieToon}].ID}) {
		
		| no nav plugin
		/if (${setTieMode.Equal[nav]}) {
		
		| no moveutils plugin
		} else /if (${setTieMode.Equal[stick]}) {
			/if (${Stick.Active}) /squelch /stick off
			/if (${Me.Moving}) {
				/keypress forward
				/keypress back
			}
			/return
		}
	}
		
	| turn off camp
	/if (${Bool[${MakeCamp}]}) {
		/squelch /makecamp off
		/call set_switch ${chrID} swHomeSet FALSE
	}
		
	| MQ2Nav mode for stick
	/if (${setTieMode.Equal[nav]}) {

		/if (${Navigation.MeshLoaded}) {
			/if (!${Navigation.Active} && ${Spawn[pc ${setTieToon}].Distance} > ${setTieDistance}) {
				/if (${Target.ID}) /squelch /target clear
				/squelch /face ID ${Spawn[pc ${setTieToon}].ID} ${If[${swFaceFast},fast,]}
				/nav spawn pc ${setTieToon} |log=off
		
			:navtime
			} else /if (${Spawn[pc ${setTieToon}].Distance} <= ${setTieDistance}) {
				/if (${Navigation.Active}) {
					/nav stop log=off
				}
			}

		} else /if (!${Navigation.MeshLoaded}) {
			VOUT NO Nav Mesh for this zone. Returning to Stick.
			/call Bind_setTieControl mode stick
		}
		
	| MQ2MoveUtils mode for stick
	} else /if (${setTieMode.Equal[stick]}) {
		| all present and or accounted for SIR!
		/if (${Spawn[pc ${setTieToon}].Distance} > ${setTieDistance}) {
			/if (${Target.ID}) /squelch /target clear
			/squelch /face ID ${Spawn[pc ${setTieToon}].ID} ${If[${swFaceFast},fast,]}
			/squelch /stick ID ${Spawn[pc ${setTieToon}].ID} ${setTieDistance} UW
		}

		:okweresticking
		AMIDEAD
		CHECKINPUTS
		/if (${Spawn[pc ${setTieToon}].Distance} <= ${setTieDistance}) {
			/squelch /stick off
			/return
		}
		
		/if (!${Stick.Stopped} && ${Spawn[pc ${setTieToon}].ID}) {
			/goto :okweresticking
		} else {
			/if (${Stick.Active}) /squelch /stick off
			/if (${Me.Moving}) {
				/keypress forward
				/keypress back
			}
		}
	
	}

/return



 |***
 *	DES: Will sort out your familiar
 *	USE: auto
 *  NOTE: 
 ***|
sub sort_familiar()
	/call timer_update timer_Check_Familiar ${tCheck_Familiar}
	CHECKINPUTS
	/if (${check_exit[]}) /return
	/declare _count int local 0
	
	/for _count 1 to 3
		/if (${Bool[${Me.Buff[${setFam${_count}Buff}].Duration}]}) /continue
		/if (!${Bool[${setFam${_count}}]}) /continue
				
		/if (${Me.Book[${setFam${_count}}]}) {
			/if (${validate_cast[FALSE, spell, "${setFam${_count}}", ${Me.ID}]}) /call core_cast2 "${setFam${_count}}" ${buffGem} ${Me.Pet.ID} FALSE
		} else /if (${Me.AltAbilityReady[${setFam${_count}}]}) {	
			/if (${validate_cast[FALSE, alt, "${setFam${_count}}", ${Me.ID}]}) /call core_cast2 "${setFam${_count}}" alt 0 FALSE
		} else /if (${Me.ItemReady[${setFam${_count}}]}) {
			/if (${validate_cast[FALSE, item, "${setFam${_count}}", ${Me.ID}]}) /call core_cast2 "${setFam${_count}}" item 0 FALSE		
		}	
			
		/delay 5
		/if (${swFam${_count}Leave}) /familiar get lost
		/delay 5

	/next _count
	
/return



|***
*	DES: check and claim Free Station Cash
*	USE: manual
* NOTE: Credit: v1.0 - Initial release on Macroquest VIP forums - Dewey2461 on 7-7-2014 [butchered by me]
***|
sub check_free_ingame_cash()
	
	| if nothing to claim. no reason to stick around.. like a bad house guest... go away..
	/if (${Window[MKPW_ClaimWindow].Child[MKPW_ClaimClickHereBtn].Text.EqualCS[Claimed]}) /return
	/declare _current_funds string local 0

	/if (${Window[MKPW_ClaimWindow].Child[MKPW_ClaimClickHereBtn].Text.EqualCS[Claim]}) {
		| open market window
		/if (!${Window[MarketplaceWnd]}) /market

		| get current funds
		/varset _current_funds ${Window[MarketplaceWnd].Child[MKPW_AvailableFundsUpper].Text}
		
		| click claim button
		/notify MKPW_ClaimWindow MKPW_ClaimClickHereBtn leftmouseup
		/delay 1s
		/delay 10s ${Window[MarketplaceWnd].Child[MKPW_AvailableFundsUpper].Text.NotEqual[${_current_funds}]}
		/varset _current_funds ${Window[MarketplaceWnd].Child[MKPW_AvailableFundsUpper].Text}
		VOUT In Game Cash Collected [${cnum}${_current_funds}\ax]   

		| close market window
		/if (${Window[MarketplaceWnd]}) /market

	}

/return



|***
*	DES: will click items and autoinventory the result on a timer.
*	USE: /call odds_and_ends
* NOTE: good for poisons, whatevers.
***|
sub odds_and_ends()
	CHECKEXIT
	/declare _count int local
	/for _count 1 to 4
		/if (!${Bool[${clickOddItem${_count}}]}) /continue
		AMIDEAD
		CLEARCURSOR TRUE
		CHECKINPUTS
		/if (${validate_cast[FALSE, item, "${clickOddItem${_count}}", ${Me.ID}]}) /call core_cast2 "${clickOddItem${_count}}" item 0 FALSE
	/next _count
	CLEARCURSOR TRUE

/return



|***
 *	DES: If the followng criteria are met, the calling function will be skipped.
 *	USE: /call check_exit [TRUE|NULL] 
 *  NOTE: All reasons to not attack a target..
 ***|
sub check_exit()
	/if (${Me.Zoning}) /return TRUE
	
	| invis 
	/if (${Me.Invis}) /return TRUE
	
	
	| am i dead on the floor?
	/if (${Me.State.Equal[HOVER]} || ${Me.Hovering} || ${Me.Dead}) /return TRUE
	
	| did I just die and get sent to bind ?
	/if (${coreAuto} && ${Me.Buff[Revival Sickness].ID} && ${Zone.ID} == ${Me.ZoneBound.ID}) /return TRUE
		
	| movement
	/if ((${Me.Moving} || ${Stick.Active}) && !${Select[${Me.Class.ShortName},BRD]}) /return TRUE

	| clear ToTD if it procs.
	/if (${coreAuto} && ${Select[${Me.Class.ShortName},CLR,SHM,BER]}) {
		/if (${Me.Song[Touch of the Divine].ID}) {
			/delay 3s
			/docommand /removebuff Touch of the Divine
			ECHOCHANNEL REMOVING .. Touch of the Divine
			/return TRUE
		}
	}
	
	| invulnerable debuff?
	/if (${Debuff.Invulnerable}) /return TRUE	
	
	| SELF BUFFS/SONGS
	/if (${Me.Song[Divine Aura].ID}) /return TRUE
	/if (${Me.Song[Divine Barrier].ID}) /return TRUE
	/if (${Me.Song[Sacred Barrier].ID}) /return TRUE
	/if (${Me.Song[Touch of the Divine].ID}) /return TRUE
	/if (${Me.Song[Bestowed Divine Aura Beza].ID} || ${Me.Song[Bestowed Divine Aura Azia].ID}) /return TRUE

	| bad self buffs
	/if (${Me.Buff[Bewilder].ID}) /return TRUE
	/if (${Me.Buff[Chaotic Bewildering].ID}) /return TRUE
	
	| SELF STATES
	/if (${Me.State.Equal[FEIGN]}) /return TRUE
	/if (${Me.Ducking}) /return TRUE
	/if (${Me.Stunned}) /return TRUE
	/if (${Me.Charmed.ID}) /return TRUE
	
	| check no cast list and mezz?
	/if (${coreAuto} && ${Target.ID}) {
		/if (${Bool[${NoCastList.Find[${Target.DisplayName}]}]}) /return TRUE
		/if (${Bool[${Target.Mezzed}]} && !${swPull}) /return TRUE
	}

/return FALSE



|***
 *	DES: Updates Notice sction of HUD.
 *	USE: auto
 *  NOTE: 
 ***|
sub update_HUD_notices(string _mode)
	/if (${Me.Zoning}) /return FALSE
	
	/declare _count int local 0
	/declare _index int local 0
	/declare _sep bool local FALSE
	/declare _tmphudWarning string local  
	/declare _tmphudNotice string	local  
	/declare _tmptarNotice string	local  
	
	| current mode display
	/if (${Bool[${_mode}]}) {
		/varset tmpMode "${_mode}"
	} else /if (!${Bool[${_mode}]} || ${_mode.Equal[NULL]}) {
		/varset tmpMode FALSE
	}

	|**  HUD warning notices.. the red stuff up top **|
	
	| rez warning
	/if (${swAmIDead} || ${swNeedRez}) /varset _tmphudWarning ${_tmphudWarning} NEED REZ
			
	| check for Invis
	/for _count 1 to ${lstIV.Size}
		/for _index 1 to ${lstIV[${_count}].Count[|]}
			/if (${Me.Buff[${lstIV[${_count}].Arg[${_index},|]}].Name.Equal[${lstIV[${_count}].Arg[${_index},|]}]}) {
				/varset _tmphudWarning ${_tmphudWarning} IV
				/break
			}
		/next _index	
	/next _count

	| check for invis vs undead
	/for _count 1 to ${lstIVU.Size}
		/for _index 1 to ${lstIVU[${_count}].Count[|]}
			/if (${Me.Buff[${lstIVU[${_count}].Arg[${_index},|]}].Name.Equal[${lstIVU[${_count}].Arg[${_index},|]}]}) {
				/varset _tmphudWarning ${_tmphudWarning} IVU 
				/break
			}
			/next _index	
	/next _count

	| override warnings
	| /for _count 1 to ${sql_return[lstTypesOverride].Count[|]}
	| 	/if (${swOverride${lstTypesOverride.Arg[${_count},|]}}) {
	| 		/varset _tmphudWarning ${_tmphudWarning} ${lstTypesOverride.Arg[${_count},|]}
	| 	}
	| /next _count	
	
	| finaly something to update.. maybe..
	/if (${_tmphudWarning.Length} != ${hudWarningLength}) {
		/varset hudWarningLength ${_tmphudWarning.Length}
		/varset hudWarning ${_tmphudWarning}
	} 		
	
	|**  HUD bottom notices.. safe zone, modes, stuff like that  **|
	
	/if (${Select[${Me.Class.ShortName},BRD,WIZ]}) {
		/varset _tmphudNotice ${_tmphudNotice} ${lineCurrent}
	}
	/if (${AdvPath.Playing}) /varset _tmphudNotice ${_tmphudNotice} AdvPath
	/if (${SafeZone}) /varset _tmphudNotice ${_tmphudNotice} SafeZone
	/if (${Stick.Active}) /varset _tmphudNotice ${_tmphudNotice} Stick
	/if (${Select[${Me.Class.ShortName},MAG]}) {
		/if (${Bool[${useRain}]}) /varset _tmphudNotice ${_tmphudNotice} AE
	}
	| /if (${Select[${Me.Class.ShortName},CLR,PAL,SHM,DRU]}) {
	| 	/if (${Bool[${RampageTank}]}) /varset _tmphudNotice ${_tmphudNotice} RT[${RampageTank}]
	| }

	| Mode Notice
	/if (${swModeHunt}) /varset _tmphudNotice ${_tmphudNotice} Hunt[${setModeAgro}]
	/if (${swModePetfarm}) /varset _tmphudNotice ${_tmphudNotice} PetFarm
	/if (${swModeScorch}) /varset _tmphudNotice ${_tmphudNotice} Scorch[${setModeAgro}]
	/if (${swModeHarvest}) /varset _tmphudNotice ${_tmphudNotice} Harvest[${setModeAgro}]
	/if (${swModeDrag}) /varset _tmphudNotice ${_tmphudNotice} Drag
	/if (${swModeForage}) /varset _tmphudNotice ${_tmphudNotice} Forage
	/if (${swModeFish}) /varset _tmphudNotice ${_tmphudNotice} Fish
	/if (${swModeSkill}) /varset _tmphudNotice ${_tmphudNotice} Skill Trainer
	/if (${swModeLush} || ${Me.Drunk}) /varset _tmphudNotice ${_tmphudNotice} Drunk[${Me.Drunk}]

	| spit out the verbage
	/if (${_tmphudNotice.Length} != ${hudNoticeLength}) {
		/varset hudNoticeLength ${_tmphudNotice.Length}
		/varset hudNotice ${_tmphudNotice}
	} 	
		
	| target debuffs notices
	/varset _sep FALSE
	/if (${Bool[${Target.Maloed}]}) /varset _tmptarNotice ${_tmptarNotice} Malo
	/if (${Bool[${Target.Tashed}]}) /varset _tmptarNotice ${_tmptarNotice} Tash
	/if (${Bool[${Target.Snared}]}) /varset _tmptarNotice ${_tmptarNotice} Snare
	/if (${Bool[${Target.Slowed}]}) /varset _tmptarNotice ${_tmptarNotice} Slow
	/if (${Bool[${Target.DSed}]}) /varset _tmptarNotice ${_tmptarNotice} DS
	/if (${Target.Buff[${setBufftoWatch}].ID}) /varset _tmptarNotice ${_tmptarNotice} ${setBufftoWatch}
	
	| target things and stuff
	/if (${_tmptarNotice.Length} != ${hudTargetLength}) {
		/varset hudTargetLength ${_tmptarNotice.Length}
		/varset hudTarNotice ${_tmptarNotice}
	} 

/return TRUE



|***
 *	DES: will cast Temp pet
 *	USE: /call cast_temp_pet [MOBID] [Temp Spell Name]
 *  NOTE: 
 ***|
sub cast_temp_pet(int _tmpID, string _tmpSpell)
	AMIDEAD
	CHECKEXIT
	CHECKINPUTS
	/if (${is_target_dead[${_tmpID}]}) /return FALSE
	
	/if (${validate_cast[FALSE, spell, "${_tmpSpell}", ${_tmpID}]}) /call core_cast2 "${_tmpSpell}" 0 ${_tmpID} FALSE

/return TRUE



|***
 *	DES: Cleric/Paladin AA Yaulp
 *	USE: /call cast_yaulp
 *  NOTE: if doYaulp is TRUE, will use at 98% manna or less.
 *	NOTE: Pallys keep that shit up ALL the time
 ***|
sub cast_yaulp()
	/if (${Me.Sitting} || ${Me.Mount.ID}) /return FALSE
	/if (${Me.Buff[Lesser Yaulp].ID} || ${Me.Buff[Yaulp].ID}) /return FALSE
	/if (!${Select[${Me.CombatState},COMBAT]}) /return FALSE
	AMIDEAD
	SIMPLEEXIT
	
	| use spell Yaulp
	/if (${Bool[${spellYaulp}]} && ${Me.Gem[${Spell[${spellYaulp}].RankName}]}) {
		/if (${Spell[${Spell[${spellYaulp}].RankName}].NewStacks}) {
			/if (${validate_cast[FALSE, spell, "${spellYaulp}", ${Me.ID}]}) /call core_cast2 "${spellYaulp}" 0 0 FALSE
		}
	
	| use Donals Helm Yaulp (cleric only)
	} else /if (${Select[${Me.Class.ShortName},CLR]} && ${Me.ItemReady[Darkened Donal's Helm of Mourning]}) {
		/if (!${Spell[Yaulp XIV Rk. II].NewStacks}) /return
		/if (${validate_cast[FALSE, item, "Darkened Donal's Helm of Mourning", ${Me.ID}]}) /call core_cast2 "Darkened Donal's Helm of Mourning" item 0 FALSE
	
	| use AA yaulp
	} else /if (!${FindItem[Darkened Donal's Helm of Mourning].ID} && ${Me.AltAbilityReady[Yaulp (Lesser)]}) {
		/if (!${Spell[Yaulp (Lesser)].NewStacks} || ${Bool[${Me.Buff[Lesser Yaulp]}]} || ${Bool[${Me.Buff[Yaulp]}]}) /return
			/if (${validate_cast[FALSE, alt, "Yaulp (Lesser)", ${Me.ID}]}) /call core_cast2 "Yaulp (Lesser)" alt 0 FALSE

	}

/return



|***
 *	DES: uses melee rest disc
 *	USE: /call cast_stamina_recovery
 *  NOTE: 
 ***|
sub cast_stamina_recovery()
 	/if (${Defined[DiscRest]}) {
		/if (!${Bool[${DiscRest}]}) /return
 	} else /if (!${Defined[DiscRest]}) {
 		/return
 	}
 	/if (${SafeZone} && !${swOverrideSafeZone} || !${Me.CombatAbilityReady[${Spell[${DiscRest}].RankName}]} || ${swAmIDead}) /return
	AMIDEAD
	CHECKINPUTS
	
	/declare _endurancePoint int local 0

	/if (${DiscRest.Equal[Respite]} || ${DiscRest.Equal[Reprive]} || ${DiscRest.Equal[Rest]} || ${DiscRest.Equal[breather]}) {
		/if (${Me.PctEndurance} > 33) /return
		/if (${Me.CombatAbility[${DiscRest} Rk. III]}) /varset _endurancePoint 29
		/if (${Me.CombatAbility[${DiscRest} Rk. II]}) /varset _endurancePoint 25
		/if (${Me.CombatAbility[${DiscRest}]}) /varset _endurancePoint 21
	} else /if (${DiscRest.Equal[Hiatus]}) {
		/if (${Me.PctEndurance} > 65) /return
		/if (${Me.CombatAbility[${DiscRest} Rk. III]} && (${Me.CurrentEndurance} < 102500 && ${Me.PctEndurance} < 65)) /varset _endurancePoint 65
		/if (${Me.CombatAbility[${DiscRest} Rk. II]} && (${Me.CurrentEndurance} < 97500 && ${Me.PctEndurance} < 63)) /varset _endurancePoint 63
		/if (${Me.CombatAbility[${DiscRest}]} && (${Me.CurrentEndurance} < 92500 && ${Me.PctEndurance} < 60)) /varset _endurancePoint 60
	}
	
	| Force Fade to get rest dfor stamina while in combat
	/if (${swForceFadeRest} && ${Bool[${${Me.Class.ShortName}AAFade}]}) {
		/if (${Select[${Me.CombatState},COMBAT]} && ${Me.PctEndurance} < ${_endurancePoint} && ${Me.AltAbilityReady[${${Me.Class.ShortName}AAFade}]}) {
			/call clear_combat
			/call set_stop NOECHO
			/if (${validate_cast[FALSE, alt, "${${Me.Class.ShortName}AAFade}", ${Me.ID}]}) /call core_cast2 "${${Me.Class.ShortName}AAFade}" alt 0 FALSE
			/delay 10
			/if (${validate_cast[FALSE, disc, "${DiscRest}", ${Me.ID}]}) /call core_cast2 "${DiscRest}" disc 0 FALSE
			/return
		}
	}

	| need to be out of combat to use
 	/if (!${Select[${Me.CombatState},RESTING,COOLDOWN,ACTIVE]}) /return

 	/if (${Me.PctEndurance} < ${_endurancePoint}) {
 		/if (${validate_cast[FALSE, disc, "${DiscRest}", ${Me.ID}]}) /call core_cast2 "${DiscRest}" disc 0 FALSE
 	}

/return



|***
 *	DES: corrects the Gem## to simply a #
 *	USE: /call fix_gem_id [gem##] | ${fix_gem[gem##]}
 *  NOTE:
 ***|
sub fix_gem_id(string _spellGem)
	/declare _gem int local 0
	/if (${_spellGem.Length} == 1 || ${_spellGem.Length} == 4) {
		/varset _gem ${_spellGem.Right[1]}
	} else /if (${_spellGem.Length} == 2 || ${_spellGem.Length} == 5) {
		/varset _gem ${_spellGem.Right[2]} 
	}

/return ${_gem}



|***
 *	DES: Memorize a spell/song
 *	USE: /call mem_spell [T|F echo memming] [spell name] [gem to mem it in] [TRUE|FALSE wait for recycle to cast]
 *  NOTE: 
 ***|
sub mem_spell(bool _echo, string _spellname, string _spellgem, bool _wait)
 	/if (${Me.Gem[${Spell[${_spellname}].RankName}]}) /return TRUE
	AMIDEAD
	CHECKINPUTS

	| do we even have the spell?
	/if (!${Me.Book[${Spell[${_spellname}].RankName}]}) {
		/if (!${Me.Book[${Spell[${_spellname}].RankName}]} && !${timer_mem_spell_timer}) {
			/call timer_update timer_mem_spell_timer 60s
			VOUT ${onotice} ${sep} [${_spellname}] Bad Spell Name.
		}
		/return FALSE
	}

	| just the facts ma'am
	/call fix_gem_id "${_spellgem}"
	/declare _gem int local ${Macro.Return}
	/declare _counter int local 0
	
	/if (${_echo}) VOUT Mem ${sep} ${csp}${Spell[${_spellname}].RankName}\ax
	/memspell ${_gem} "${Spell[${_spellname}].RankName}"

	| wait for spell to be ready
	/while (${_wait}) {
		/if (${_counter} >= 600) /break
		/if (${Me.SpellReady[${Spell[${_spellname}].RankName}]}) /break
		/delay 5
		/varcalc _counter ${_counter}+5
		| one more check to make sure we mem the spell
		/if (${_counter} == 300) /if (!${Me.Gem[${Me.Gem[${Spell[${_spellname}].RankName}]}]}) /memspell ${_gem} "${Spell[${_spellname}].RankName}"
	}
	
	| yah we dont wat for no one...
	/if (!${_wait}) {
		/delay 3s ${Me.SpellReady[${Spell[${_spellname}].RankName}]}
	}

/return TRUE



|***
 *	DES: sends a tell to the mages in the lstResidentMage for shit you need.
 *	USE: /call tell_resident_mage [shard|weapons]
 *  NOTE: This will send the same tell to all mages in the list. welcome to ignore
 ***|
sub tell_resident_mage(string _request)
	/if (${Select[${Me.Class.ShortName},MAG]}) /return FALSE
	AMIDEAD
	CHECKEXIT
	CHECKINPUTS
	
	/declare _toon string local FALSE
	/if (${Bool[${get_resident[mage, 150]}]}) {
		/varset _toon ${get_resident[mage, 150]}
	} else {
		/return FALSE
	}

	| ask for weapons
	/if (${validate_class[FALSE, ${lstClassPet}]}) {
		/if (${_request.Equal[weapons]} && !${timer_ask_for_weapons_${_toon}} && !${FindItem[${setMAGClosedBag}].ID}) {
			/t ${_toon} weapons
			/call timer_update timer_ask_for_weapons_${_toon} ${tAsk_for_weapons}
		}
	}
	
	| ask for ModRod - shard
	/if (${_request.Equal[shard]} && !${timer_ask_for_rod_${_toon}} && !${FindItem[Modulation Shard].ID}) {
		/t ${_toon} shard
		/call timer_update timer_ask_for_rod_${_toon} ${tAsk_for_rod}
	}

/return TRUE



|***
 *	DES: Res routines for the healer slobs.
 *	USE: /call check_for_corpse
 *  NOTE: looks for corpses.. rez them
 ***|
sub check_for_corpse()
	/if (${Me.State.Equal[HOVER]} || ${Me.Hovering} || ${Me.Dead}) /return FALSE
	
	| fuck your corpse
	/if (!${swRezGive}) /return	FALSE
	| no one dead
	/if (!${SpawnCount[pccorpse radius ${setMaxRezRange}]}) /return FALSE
	| no combat rez
	/if (!${swRezCombat} && ${SpawnCount[npc radius ${setCombatRadius} playerstate 4]}) /return FALSE
	| no safe zone ressing
	/if (${SafeZone} && !${swOverrideSafeZone}) /return FALSE
	| not enough manna for AA rez
	/if (${Me.PctMana} < 2 && !${swRezToken}) /return FALSE

	| no emergency tokens AND dont use them when a cleric is around
	/if (${swRezToken}) {
		/if (!${Me.ItemReady[Token of Resurrection]}) /return FALSE
		/if (${Spawn[pc CLR radius ${setMaxRezRange}].ID}) /return FALSE

		| disable rezemergency if you dont have tokens or are on a TLP box
		/if (!${FindItem[Token of Resurrection].ID} || ${swTLP}) {
			/varset swRezToken FALSE
			ECHOCHANNEL ${Me.DisplayName} needs Rez Tokens....
		}
	}
		
	| set rez name/type	
	/if	 (${swTLP}) {
		/if (${Select[${Me.Class.ShortName},NEC]}) {
			/if (!${FindItem[Essence Emerald].ID}) /return FALSE
			/declare _rezNEC						string	local "${spellRez}"

		| cleric stuff
		} else /if (${Select[${Me.Class.ShortName},CLR,PAL,SHM,DRU]}) {
			/declare _rez${Me.Class.ShortName}			string	local "${spellRez}"
		}
		
	} else {

		| necro stuff
		/if (${Select[${Me.Class.ShortName},NEC]}) {
			/if (!${FindItem[Essence Emerald].ID}) /return FALSE
			/declare _rezNEC						string	local "Convergence"

		| cleric stuff
		} else /if (${Select[${Me.Class.ShortName},CLR]}) {
			/declare _divinerezCLR			string	local "Divine Resurrection"
			/declare _rezCLR						string	local "Blessing of Resurrection"
		
		| pally, shaman, druid stuff
		} else /if (${Select[${Me.Class.ShortName},PAL,SHM,DRU]}) {
			/if (${Select[${Me.CombatState},COMBAT]}) {
				/declare _rezDRU						string	local "Call of the Wild"
				/declare _rezSHM						string	local "Call of the Wild"
				/declare _rezPAL						string	local "Gift of Resurrection"
			} else {
				/declare _rezDRU						string	local "Rejuvenation of Spirit"
				/declare _rezSHM						string	local "Rejuvenation of Spirit"
				/declare _rezPAL						string	local "Restore Life"
			}
		}

		/if (${swRezToken}) {		
			/declare _rezEmergency			string	local "Token of Resurrection"
		}
		
	}

	/declare _count 						int 		local 0
	/declare _tmpID 						int 		local 0

	| make the damn variables
	/for _count 1 to ${SpawnCount[pccorpse radius ${setMaxRezRange}]}
		/varset _tmpID ${NearestSpawn[${_count},pccorpse radius ${setMaxRezRange}].ID}
		
		| for the people that suck ass (not in a good way)
		/if (${lstNoRez.Find[${Spawn[${_tmpID}].Name}]}) {
			/if (!${Defined[timer_corpse_${_tmpID}]}) /declare timer_corpse_${_tmpID} timer outer 7200
			/continue
		}	
		
		/if (!${Defined[timer_corpse_${_tmpID}]}) /declare timer_corpse_${_tmpID} timer outer 1
	/next _count

	UPDATEHUD "Resurrect"
	
	| clerics first group or raid
	/if (${SpawnCount[pccorpse cleric raid radius ${setMaxRezRange}]} && ${Raid.Members} || ${SpawnCount[pccorpse cleric group radius ${setMaxRezRange}]}) {
		/for _count 1 to ${SpawnCount[pccorpse cleric radius ${setMaxRezRange}]}
		AMIDEAD
		/if (!${timer_corpse_${NearestSpawn[${_count},pccorpse cleric radius ${setMaxRezRange}].ID}}) {
			/varset _tmpID ${NearestSpawn[${_count},pccorpse cleric radius ${setMaxRezRange}].ID}
			/if (${Select[${Me.Class.ShortName},CLR]} && ${Me.AltAbilityReady[${_divinerezCLR}]} && ${DivineRezCleric}) {
				/call cast_rez ${_tmpID} ${_divinerezCLR}		
			} else /if (${Select[${Me.Class.ShortName},CLR,DRU,PAL,SHM,NEC]} && ${Me.AltAbilityReady[${_rez${Me.Class.ShortName}}]}) {
				/call cast_rez ${_tmpID} ${_rez${Me.Class.ShortName}}
			} else /if (${swRezToken} && ${lstRezToken.Find[${Spawn[${_tmpID}].Class.ShortName}]}) {
				/call cast_rez ${_tmpID} ${_rezEmergency}
			}
			| /return
		}
		/next _count
	} 
	
	| group members
	/if (${SpawnCount[pccorpse group radius ${setMaxRezRange}]}) {
		/for _count 1 to ${SpawnCount[pccorpse radius ${setMaxRezRange} group]}
		/if (!${timer_corpse_${NearestSpawn[${_count}, pccorpse radius ${setMaxRezRange} group].ID}}) {
			/varset _tmpID ${NearestSpawn[${_count}, pccorpse radius ${setMaxRezRange} group].ID}
			/if (${swRezToken} && ${lstRezToken.Find[${Spawn[${_tmpID}].Class.ShortName}]}) {
				/call cast_rez ${_tmpID} ${_rezEmergency}
			} else {
				/call cast_rez ${_tmpID} ${_rez${Me.Class.ShortName}}
			}
			| /return
		}
		/next _count
	}		
	
	| tanks
	/if (${Raid.Members}) {
		/if (${SpawnCount[pccorpse raid tank radius ${setMaxRezRange}]}) {
			/for _count 1 to ${SpawnCount[pccorpse raid radius ${setMaxRezRange} tank]}
			/if (!${timer_corpse_${NearestSpawn[${_count},pccorpse raid radius ${setMaxRezRange} tank].ID}}) {
				/varset _tmpID ${NearestSpawn[${_count},pccorpse raid radius ${setMaxRezRange} tank].ID}
				/if (${swRezToken} && ${lstRezToken.Find[${Spawn[${_tmpID}].Class.ShortName}]}) {
					/call cast_rez ${_tmpID} ${_rezEmergency}
				} else {
					/call cast_rez ${_tmpID} ${_rez${Me.Class.ShortName}}
				}
				| /return
			}
			/next _count
		} 
	
		| remaining raid members
		/if (${SpawnCount[pccorpse raid radius ${setMaxRezRange}]}) {
			/for _count 1 to ${SpawnCount[pccorpse raid radius ${setMaxRezRange}]}
			/if (!${timer_corpse_${NearestSpawn[${_count},pccorpse raid radius ${setMaxRezRange}].ID}}) {
				/varset _tmpID ${NearestSpawn[${_count},pccorpse raid radius ${setMaxRezRange}].ID}
				/if (${swRezToken} && ${lstRezToken.Find[${Spawn[${_tmpID}].Class.ShortName}]}) {
					/call cast_rez ${_tmpID} ${_rezEmergency}
				} else {
					/call cast_rez ${_tmpID} ${_rez${Me.Class.ShortName}}
				}
				| /return
			}
			/next _count
		} 
	}
	
	| guildies - all the time
	/if (${SpawnCount[pccorpse guild radius ${setMaxRezRange}]}) {
		/for _count 1 to ${SpawnCount[pccorpse radius ${setMaxRezRange} GUILD]}
		/if (!${timer_corpse_${NearestSpawn[${_count}, pccorpse radius ${setMaxRezRange} guild].ID}}) {
			/varset _tmpID ${NearestSpawn[${_count}, pccorpse radius ${setMaxRezRange} guild].ID}
			/if (${swRezToken} && ${lstRezToken.Find[${Spawn[${_tmpID}].Class.ShortName}]}) {
				/call cast_rez ${_tmpID} ${_rezEmergency}
			} else {
				/call cast_rez ${_tmpID} ${_rez${Me.Class.ShortName}}
			}
			| /return
		}
		/next _count
	}
	
	| outside guild and raid
	| needs /rez everyone switch to be true
	/if (${swRezEveryone}) {
		/if (${SpawnCount[pccorpse radius ${setMaxRezRange}]}) {
			/for _count 1 to ${SpawnCount[pccorpse radius ${setMaxRezRange}]}
			/if (!${timer_corpse_${NearestSpawn[${_count}, pccorpse radius ${setMaxRezRange}].ID}}) {
				/varset _tmpID ${NearestSpawn[${_count}, pccorpse radius ${setMaxRezRange}].ID}
				/if (${swRezToken} && ${lstRezToken.Find[${Spawn[${_tmpID}].Class.ShortName}]}) {
					/call cast_rez ${_tmpID} ${_rezEmergency}
				} else {
					/call cast_rez ${_tmpID} ${_rez${Me.Class.ShortName}}
				}
				| /return
			}
			/next _count
		}	
	}

	UPDATEHUD FALSE

/return



|***
 *	DES: rez/call casting code.
 *	USE: /call cast_rez [ID] [rez aa|token]
 *  NOTE: 
 ***|
sub cast_rez(int _tmpID, string _rez)
	/if (${Me.Dead}) /return FALSE
	/if (${timer_corpse_${_tmpID}}) /return FALSE
	AMIDEAD
	CHECKEXIT

	| ignore the people we dont like
	/if (${lstNoRez.Find[${Spawn[pccorpse ${_tmpID}].DisplayName}]}) {
		/call timer_update timer_corpse_${_tmpID}} 30m
		/return FALSE
	}
		
	| get the corpse close to us
	/if (${Spawn[pccorpse ${_tmpID}].Distance} > ${setCombatRadius}*2 && ${Spawn[pccorpse ${_tmpID}].Distance} <= ${setMaxRezRange}) {
		/squelch /corpse
	}

	| can we AA res or token?
	/if (${Me.AltAbilityReady[${_rez}]} || (${Me.ItemReady[Token of Resurrection]} && ${swRezToken})) {	

		| res the damn person
		
		| AA rez
		/if (!${swRezToken}) {
			| /if (!${Spawn[pccorpse ${_tmpID}].ID}) /return
			/delay 5s !${Me.Casting.ID}
			/squelch /target ID ${_tmpID}
			/delay 1s ${Target.ID} == ${_tmpID}
			/if (${validate_cast[FALSE, FIND, "${_rez}", ${Me.ID}]}) /call core_cast2 "${_rez}" FIND ${_tmpID} FALSE

		| token rez
		} else /if (${swRezToken}) {
			/delay 5s !${Me.Casting.ID}
			/squelch /target ID ${_tmpID}
			/delay 1s ${Target.ID} == ${_tmpID}
			/if (${validate_cast[FALSE, item, "Token of Resurrection", ${Me.ID}]}) /call core_cast2 "Token of Resurrection" item ${_tmpID} FALSE
		}
	
		| if cast was good, set the timers.
		/if (${Select[${castReturn},CAST_SUCCESS]}) { 
			/call timer_update timer_corpse_${_tmpID} ${tRez_Retry}
			ECHOCHANNEL ${Me.Class.ShortName} Rez: ${Spawn[${_tmpID}].DisplayName}
		} else /if (${Select[${castReturn},CAST_CANCELLED,CAST_INTERRUPTED]}) {
			/call timer_update timer_corpse_${_tmpID} 7s
		} else {
			/if (${Select[${Me.Class.ShortName},CLR]}) {
				/call timer_update timer_corpse_${_tmpID} 30s
			} else /if (${Select[${Me.Class.ShortName},DRU,SHM,PAL]}) {
				/call timer_update timer_corpse_${_tmpID} 45s
			} else {
				/call timer_update timer_corpse_${_tmpID} 1m
			}
		}
		
	}

/return




|******************************************************************
 *	LIST CONTROL FOR MEZ ROUTINES AND /TELL BUFFING
 ******************************************************************
 *  USE CAUTION WHEN MAKING CHANGES HERE
 ******************************************************************|

|***
 *	DES: add to a list
 *	USE: /call list_add [mobList|addList] [ID]
 *  NOTE: 
 ***|
sub list_add(string _listName, string _tmpID)
	/if (${${_listName}.Find[${_tmpID}]}) {
		/return ${${_listName}}
	
	} else /if (!${${_listName}.Find[${_tmpID}]}) {
		/varset ${_listName} ${${_listName}}|${_tmpID}
		/return ${_listName}
	}

/return



|***
 *	DES: delete from a list
 *	USE: /call list_delete [LISTNAME] [ID|MOBNAME]
 *  NOTE: 
 ***|
sub list_delete(string _listName, string _tmpID)
	/if (${Bool[${${_listName}.Equal[NULL]}]}) /return
	/if (!${Bool[${_tmpID}]}) /return

	/varset _tmpID |${_tmpID}
	/declare _a int local
	/declare _b int local
	/declare _strLeft string local
	/declare _strRight string local
	
	/varcalc _a ${${_listName}.Find[${_tmpID}]}-1
	| SDEBUG _a::${_a}
	/varcalc _b ${${_listName}.Length}-${_tmpID.Length}-${_a}
	| SDEBUG _b::${_b}

	/if (${_a} > 0) /varset _strLeft ${${_listName}.Left[${_a}]}
	/if (${_b} > 0) /varset _strRight ${If[${Select[${_b},0,NULL,FALSE,""]},,${${_listName}.Right[${_b}]}]} 
	
	/varset ${_listName} ${_strLeft}${_strRight}

/return ${_strLeft}${_strRight}



|***
 *	DES: pulls proper case from an existing list control
 *	USE: /varset ${list_isoloate[listname, variable to isolate]}
 *  NOTE: 
 ***|
sub list_isoloate(string _list, string _tmpID)
	| get the variable data to parse
	/varset _list ${sql_return[${_list}]}
	| find the string and cut it out
	/declare _a int local
	/varcalc _a ${_list.Find[${_tmpID}]}-1
| and return the part we want
/return ${_list.Right[-${_a}].Left[${_tmpID.Length}]}



|***
 *	DES: update a list
 *	USE: /call list_update [mobList|addList] [ID]
 *  NOTE: 
 ***|
sub list_update(string _listName, string _tmpID)
	/declare _remove bool local FALSE 

	/if (${${_listName}.Find[${_tmpID}]}) {
		
		| not an NPC or its a pet
		/if (!${Spawn[${_tmpID}].Type.Equal[NPC]} && !${Spawn[${_tmpID}].Master.Type.Equal[NPC]}) /varset _remove TRUE
		
		| no spawn in zone
		/if (!${Spawn[${_tmpID}].ID}) /varset _remove TRUE

		| mob out of range
		/if (${Spawn[${_tmpID}].Distance} > ${setMobAgro}) /varset _remove TRUE
		
		| NPC corpse
		/if (${Spawn[npccorpse ${_tmpID}].ID}) /varset _remove TRUE
		/if (${Spawn[${_tmpID}].Type.Equal[Corpse]}) /varset _remove TRUE
		
		/if (${_remove}) {
			/call list_delete ${_listName} ${_tmpID}
		}
	}

/return



|***
 *	DES: checks for adds in the local area, calls updates to the needed lists.
 *	USE: /call check_for_adds
 *  NOTE: 
 ***|
sub check_for_adds()
	/declare _tmpID string local
	/declare _numMobs int local 1

	:loopnextmob
	/if (${NearestSpawn[${_numMobs},npc radius ${setMobAgro} targetable los].ID}) {
		/varset _tmpID ${NearestSpawn[${_numMobs},npc radius ${setMobAgro} targetable ${If[${swOverrideLOS},,los]} playerstate 4].ID}
		/if (${swdeBuffEradicate} && !${Defined[timer_eradicate_${_tmpID}]}) /call timer_update timer_eradicate_${_tmpID} 1
		/if (${swdeBuffTash} && !${Defined[timer_tash_${_tmpID}]}) /call timer_update timer_tash_${_tmpID} 1
		/if (${swdeBuffSlow} && !${Defined[timer_slow_${_tmpID}]}) /call timer_update timer_slow_${_tmpID} 1
		/if (${swdeBuffCripple} && !${Defined[timer_cripple_${_tmpID}]}) /call timer_update timer_cripple_${_tmpID} 1
		/if (${swdeBuffMezz} && !${Defined[timer_mez_${_tmpID}]}) /call timer_update timer_mez_${_tmpID} 1
		/if (${Spawn[id ${_tmpID}].Type.Equal[NPC]} && ${Spawn[id ${_tmpID}].Distance}<=${setMobAgro} && ${Spawn[id ${_tmpID}].LineOfSight} && !${ExcludeList.Find[${Spawn[id ${_tmpID}].DisplayName}]} && ${Spawn[id ${_tmpID}].Type.NotEqual[Untargetable]}) {
			/if (!${timer_mez_${_tmpID}}) {
				/if (${_tmpID} != ${Me.XTarget[1].ID}) /call list_add addList ${_tmpID}
				/call list_add mobList ${_tmpID}
				/if (${Macro.Return.Equal[1]}) {
					| VOUT ${owarning} ADD ${sep} [${ctar}${Spawn[${_tmpID}].DisplayName}\ax] ${sep} [${ctar}${_tmpID}\ax]
					/if (${Me.Casting.Name.NotEqual[${Spell[${spellMez}].RankName}]} && ${Me.Casting.ID}) {
						/stopcast
					}
					/if (${NoMezzList.Find[${Spawn[${_tmpID}].DisplayName}]}) {
						VOUT ${owarning} IMMUNE TO\ax ${csp}${Spell[${spellMez}].RankName}\ax ${sep} [${ctar}${_tmpID}\ax]
						/call timer_update timer_mez_${_tmpID} 1.5m
					}          	
				}
			} else /if (${timer_mez_${_tmpID}}) {
				/call list_delete mobList ${_tmpID}
			}
		}
		/varcalc _numMobs ${_numMobs}+1
		/goto :loopnextmob
	}

/return



|***
 *	DES: adds /t for buffs from queue
 *	USE: uses INI [Buffs] 
 *  NOTE: 
 ***|
sub queue_buff_add(int _buffee, int _buffnumber)
  /declare _count				int local
  /declare _compare			int local
	AMIDEAD
  /if (${queueCount} < ${queueCountMax} && !${Spawn[${_buffee}].Dead} && ${Spawn[${_buffee}].ID}) {
    /varset _compare 0
    /for _count 1 to ${queueCountMax}
      /if ((${_buffee}==${queueList[${_count},1]} || (${Select[${Spell[${queueBuffID[${queueList[${_count},2]}]}].TargetType},"Group v2","AE PC v2","Group v1","AE PC v1"]} && (${Group.Member[${Spawn[${_buffee}].DisplayName}].ID} || ${Group.Member[${Spawn[${_buffee}].Master.DisplayName}].ID}) && ${Group.Member[${Spawn[${queueList[${_count},1]}].DisplayName}].ID})) && ${_buffnumber}==${queueList[${_count},2]}) {
        /varset _compare 1
      }
    /next _count

    /if (!${_compare}) {
      /for _count 1 to ${queueCountMax}
        /if (!${queueList[${_count},1]}) {
          /varset queueList[${_count},1] ${_buffee}
          /varset queueList[${_count},2] ${_buffnumber}
          /varcalc queueCount ${queueCount}+1
          /break
        }
      /next _count
    }
  }

/return



|***
 *	DES: removes /t for buffs from queue
 *	USE: uses INI [Buffs] 
 *  NOTE: 
 ***|
sub queue_buff_delete(int _buffee, int _buffnumber)
	/declare _count 			int local
	/for _count 1 to ${queueCountMax}
		/if (((${Spawn[${queueList[${_count},1]}].State.Equal["DEAD"]} || !${Spawn[${queueList[${_count},1]}].ID}) || ((${queueList[${_count},1]}==${_buffee} || (!${_buffee} && (${Group.Member[${Spawn[${queueList[${_count},1]}].DisplayName}].ID} || ${Group.Member[${Spawn[${queueList[${_count},1]}].Master.DisplayName}].ID}))) && (${queueBuffID[${queueList[${_count},2]}]}==${_buffnumber} || ${queueList[${_count},2]}==${_buffnumber}))) && ${queueList[${_count},1]}) {
			/varset queueList[${_count},1] 0
			/varset queueList[${_count},2] 0
			/varcalc queueCount ${queueCount}-1
		}
	/next _count

/return



|***
 *	DES: Buff on /t requests
 *	USE: uses INI [Buffs] 
 *  NOTE: queue counts start at 1, but they are empty.
 ***|
sub check_buffs_tell()
	CHECKINPUTS
	UPDATEHUD "BUFF tell"
	/declare _count					int local
	/declare _tmpVAR				int local ${Math.Calc[${setMaxBuffCount}]}
	/declare _tmpID					int local 0

	/if (${coreAuto}) {
		/call timer_update timer_check_buffs_tell ${tTell_Buff}
	}
	
	/for _count 1 to ${queueCount}
		| make sure we are at least in the correct range
		/if (!${Range.Between[1,${queueCount}:${Int[${_count}]}]}) /continue
	
		| queues default to 1 but empty. if empty., go away
		/if (${queueCount} < 1 || ${queueCount} == 0 || ${queueList[${_count},1]} == 0) /return

		| target dead or not in zone? fuck their request 
		/if (${Spawn[${queueList[${_count},1]}].Dead} || !${Spawn[${queueList[${_count},1]}].ID}) {
			/call queue_buff_delete ${queueList[${_count},1]} ${queueList[${_count},2]}
			/return
		}

		/varset _tmpID ${queueList[${_count},1]}
		/varset _tmpVAR ${queueList[${_count},2]}
		| /if (${_tmpID}) /break
	/next _count

	| no target. were out of here
	/if (!${_tmpID}) /return

	| even more checks for items/spells
	/if (${FindItem[${Buff${_tmpVAR}}].ID}) {
		/if (${FindItem[${Buff${_tmpVAR}}].TimerReady} > 20) /return
	} else {
		/if (!${Me.SpellReady[${Spell[${Buff${_tmpVAR}}].RankName}]} && ${Me.Gem[${Buff${_tmpVAR}}]}) /return
	}	

	| bards are a pita..
	/if (${Select[${Me.Class.ShortName},BRD]} && ${Me.Casting.ID}) /call pulse_stop 6
	/squelch /target ID ${_tmpID}
	/delay 5s ${Target.ID} == ${_tmpID}
	/if (${Target.ID} != ${_tmpID} || !${Target.ID}) /return
	
	| do something.. seriously we made it this far..
	/if (${FindItem[${Buff${_tmpVAR}}].ID}) {
		/if (${validate_cast[FALSE, item, "${Buff${_tmpVAR}}", ${_tmpID}]}) /call core_cast2 "${Buff${_tmpVAR}}" item ${_tmpID} FALSE
	} else /if (${Me.AltAbility[${Buff${_tmpVAR}}]}) {
		/if (${validate_cast[FALSE, alt, "${Buff${_tmpVAR}}", ${_tmpID}]}) /call core_cast2 "${Buff${_tmpVAR}}" alt ${_tmpID} FALSE
	} else {
		/if (${validate_cast[FALSE, spell, "${Buff${_tmpVAR}}", ${_tmpID}]}) /call core_cast2 "${Buff${_tmpVAR}}" ${buffGem} ${_tmpID} FALSE
	}

	| clear that request form the queue. we no longer like that person
	/if (${Select[${castReturn},CAST_SUCCESS]}) /call queue_buff_delete ${_tmpID} ${_tmpVAR}
	
	UPDATEHUD FALSE

/return



|***
 *	DES: advanced loot routine(s)
 *	USE: auto
 *  NOTE: set a person in the group as master looter. Or, as the raid master looter.
 *	NOTE2: The event in this sectction is designed to handle lore items the toon already posseses. as of this, MQ has no property .Member value for LORE in ${AdvLoot
 ***|
#Event forcedloreleave "#*#already has #1# and it is lore#*#"
#Event forcedloreleave "#*#does not want #1#. It is either on their never list or they have selected No#*#"
sub Event_forcedloreleave(string line, string _itemName)
	/if (!${swADVLoot}) /return
	/varset _itemName ${AdvLoot.SList[1].Name}
	VOUT Cursor ${sep} ${cinfo}${_itemName}\ax -> ${oleave} 
	/ini "${INILoot}" "${_itemName.Left[1]}" "${_itemName}" Leave
	/doevents flush
/return

sub do_loot()
	/if (!${swADVLoot} || ${Me.Zoning}) /return
	AMIDEAD
	CHECKINPUTS
	/if (${Stick.Active} || !${Me.FreeInventory} || !${Me.UseAdvancedLooting}) /return

	| I am not the raid loot master /sigh
	/if (${Raid.Members} && ${Raid.MasterLooter.ID} != ${Me.ID} && !${swOverrideLoot}) /return
	/if (!${Raid.Members} && ${Bool[${Group.Members}]} && ${Group.MasterLooter.ID} != ${Me.ID} && !${swOverrideLoot}) /return

	UPDATEHUD "Loot"

	/delay 2s !${AdvLoot.LootInProgress}
	/if (${Me.UseAdvancedLooting} && ${Group.MasterLooter.Name.Equal[${Me.DisplayName}]} && !${swAutoCursor}) /varset swAutoCursor TRUE
	
	/declare _scount int local 0
	/declare _pcount int local 0
	
	| Item To Process Name (the thingie in the list of advloot)
	/declare _advlItem string local
	
	| _advlChoice is what is listed to do with the items: Leave,Destroy,Keep,Sell,Announce
	/declare _advlChoice string local
	
	| if we want to give it to someone else
	/declare _advlPerson string local

	/if (${Raid.Members} || ${Bool[${Group.Members}]}) {
		| Shared List Sorting
		/for _scount ${AdvLoot.SCount} downto 1
			/varset _advlItem ${AdvLoot.SList[${_scount}].Name}
			
			| corect the INI entry if not set for the new form
			/if (!${Ini[${INILoot},"${_advlItem.Left[1]}","${_advlItem}"].Count[|]}) {
				VOUT Fixing Loot: ${_advlItem}
				/ini "${INILoot}" "${_advlItem.Left[1]}" "${_advlItem}" |${Ini[${INILoot},"${_advlItem.Left[1]}","${_advlItem}"]}
			}
			/varset _advlChoice ${Ini[${INILoot},"${_advlItem.Left[1]}","${_advlItem}"].Arg[1,|]}
			/varset _advlPerson ${Ini[${INILoot},"${_advlItem.Left[1]}","${_advlItem}"].Arg[2,|]}

			| Does it exist in the loot.ini (check to see if there is a choice for it)
			/if (${AdvLoot.SList[${_scount}].ID} && !${Select[${_advlChoice},Leave,Keep,Destroy,Sell,Announce,Ignore]}) {
				| /echo ${_advlChoice}
				| Add item to ini if it not in there
				/if (${AdvLoot.SList[${_scount}].NoDrop} || ${FindItem[${_advlItem}].Lore} || ${FindItemBank[${_advlItem}].Lore}) {
					VOUT ${oadd} ${sep} ${_advlItem} = ${oleave} [no drop/lore].
					/ini "${INILoot}" "${_advlItem.Left[1]}" "${_advlItem}" |Leave
				} else {
					VOUT ${oadd} ${sep} ${_advlItem} = ${okeep} [default].
					/ini "${INILoot}" "${_advlItem.Left[1]}" "${_advlItem}" |Keep
				}
				| if we make a new entry, update.
				/varset _advlChoice ${Ini[${INILoot},"${_advlItem.Left[1]}","${_advlItem}"].Arg[1,|]}
			}  
			
			| now we do something with the item.
			
			| if we are going to leave the item or if it is lore AND we have one already. Conversly, we loot if we intend to keep or destroy the item
			| pass the item off if there is a person listed and in group
			/if (${Bool[${_advlPerson}]} && ${Group.Member[${_advlPerson}].ID}) {
					/delay 1s !${AdvLoot.LootInProgress}
					/if (!${AdvLoot.LootInProgress}) /advloot shared ${_scount} giveto ${_advlPerson}
					/delay 1
			
			| ditch the item if we need too
			} else /if (${Select[${_advlChoice},Leave,Announce]} || ${FindItem[${_advlItem}].Lore} || ${FindItemBank[${_advlItem}].Lore}) {
				/if (${Select[${_advlChoice},Announce]}) ECHOCHANNEL Loot: Leaving ${_advlItem}
				/delay 1s !${AdvLoot.LootInProgress}
				/if (!${AdvLoot.LootInProgress}) /advloot shared ${_scount} leave
				/delay 1
				/continue

			| or we keep it.
			} else /if (${AdvLoot.SList[${_scount}].ID}) {
				/if (${Select[${_advlChoice},Keep,Destroy,Sell]}) {
					/delay 1s !${AdvLoot.LootInProgress}
					/if (!${AdvLoot.LootInProgress}) /advloot shared ${_scount} giveto ${Me.Name}
					/delay 1
				}
			}
		/next _scount
	}
	
	| Personal List sorting. ALSO for solo killing
	/if (${AdvLoot.PCount}) {
		/for _pcount ${AdvLoot.PCount} downto 1
			/varset _advlItem ${AdvLoot.PList[${_pcount}].Name}
			/varset _advlChoice ${Ini[${INILoot},"${_advlItem.Left[1]}","${_advlItem}"].Arg[1,|]}
			
			/if (${AdvLoot.PList[${_pcount}].ID} && !${Select[${_advlChoice},Leave,Keep,Destroy,Sell,Announce,Ignore]}) {
				/if (${Select[${_advlChoice},Announce]}) {
					ECHOCHANNEL Loot: Leaving ${_advlItem}
					/delay 1s !${AdvLoot.LootInProgress}
					/if (!${AdvLoot.LootInProgress}) /advloot personal ${_pcount} leave
				} else /if (${AdvLoot.PList[${_pcount}].NoDrop} || ${FindItem[${_advlItem}].Lore} || ${FindItemBank[${_advlItem}].Lore}) {
					VOUT Cursor ${sep} ${cinfo}${_advlItem}\ax = ${oleave} loot file [no drop].
					/ini "${INILoot}" "${_advlItem.Left[1]}" "${_advlItem}" |Leave
					| /goto :looppersonalitem
				} else {
					VOUT Cursor ${sep} ${cinfo}${_advlItem}\ax = ${okeep} loot file [default].
					/ini "${INILoot}" "${_advlItem.Left[1]}" "${_advlItem}" |Keep
				}
				/varset _advlChoice ${Ini[${INILoot},"${_advlItem.Left[1]}","${_advlItem}"].Arg[1,|]}
			} 
			
			
			/if (${Select[${_advlChoice},Destroy]}) {
				/delay 1s !${AdvLoot.LootInProgress}
				/advloot personal ${_pcount} loot
				/delay 1s !${AdvLoot.LootInProgress}
				/delay 5
				/if (${FindItem[${_advlItem}].ID}) {
					
| evil delete code. had to change this. like > 9000 times 
					:loopfind_advlItem
					/delay 5
					/nomodkey /shift /itemnotify in Pack${Math.Calc[${FindItem[=${_advlItem}].ItemSlot}-22]} ${Math.Calc[${FindItem[=${_advlItem}].ItemSlot2}+1]} leftmouseup
					CLEARCURSOR TRUE
					/if (${Window[ConfirmationDialogBox].Open}) {
						/delay 1
						/nomodkey /notify ConfirmationDialogBox Yes_Button leftmouseup
					}
					/delay 3 !${Cursor.ID}
					/if (${FindItem[=${_advlItem}].ID}) /goto :loopfind_advlItem
| evil delete code end...

				}          
			} else /if (${Select[${_advlChoice},Leave,Announce]} || ${FindItem[${_advlItem}].Lore} || ${FindItemBank[${_advlItem}].Lore}) {
				/if (${Select[${_advlChoice},Announce]}) ECHOCHANNEL Loot: Leaving ${_advlItem}
				/delay 1s !${AdvLoot.LootInProgress}
				/advloot personal ${_pcount} leave
				/advloot shared ${_pcount} leave
				/continue
			} else /if (${Select[${_advlChoice},Keep,Sell]}) {
				/delay 1s !${AdvLoot.LootInProgress}
				/advloot personal ${_pcount} loot
				/delay 1
				/if (${Window[ConfirmationDialogBox].Open}) {
					/nomodkey /notify ConfirmationDialogBox Yes_Button leftmouseup
					/delay 1
				}
			} 
		/next _pcount
	}        
	UPDATEHUD FALSE

/return



|***
 *	DES: buy items from a vendor
 *	USE: 
 *  NOTE: you have to open the merchants window. this works on the loyalty vendor in PoK as well.
 ***|
sub vendor_buy(string _itemtoBuy, int _buyAmount)
	/if (!${Select[${Target.Type},npc,pet]} || !${Target.ID} || ${Target.Type.Equal[pet]} && !${Target.DisplayName.Find[familiar]}) /return
	/declare _count int local 0
	/declare _qty int local 0
	/declare _listItem int local 0

	/varcalc _qty ${_buyAmount}-${FindItemCount[${_itemtoBuy}]}
	/delay 3
	/if (${FindItemCount[${_itemtoBuy}]} >= ${_buyAmount}) {
		VOUT I already have ${_itemtoBuy} ${sep} (${cnum}${FindItemCount[${_itemtoBuy}]}\ax)
		/return
	}   

	| set item in the list window
	/if (${Target.ID} == ${Spawn[npc alerynril].ID}) {
		/varset _listItem ${Window[NewPointMerchantWnd].Child[NewPointMerchant_ItemList].List[=${_itemtoBuy}]}
	} else {
		/varset _listItem ${Window[MerchantWnd].Child[ItemList].List[=${_itemtoBuy},2]}}
	} 	

	/if (!${_listItem}) {
		VOUT ${ocaution} ${sep} Couldn't find ${_itemtoBuy}
		/return
	} else {
		/if (${Target.ID} == ${Spawn[npc alerynril].ID}) {
			/notify NewPointMerchantWnd NewPointMerchant_ItemList listselect ${_listItem}
		} else {
			/notify MerchantWnd ItemList listselect ${_listItem}
		}
		/delay 5       
	}	

	VOUT Buying ${sep} ${citem}${_itemtoBuy}\ax 'till I get ${_buyAmount} 

	:loopbuyitems
	CHECKINPUTS
	/if (${InventoryFull}) /return

	/if (${_qty} > 0) {
		/if (${_qty} > 19) {
			| /if (${InventoryFull}) /return
			/if (${Target.ID} == ${Spawn[npc alerynril].ID}) {
				/shiftkey /notify NewPointMerchantWnd NewPointMerchant_PurchaseButton leftmouseup
			} else {            	
				/shiftkey /notify merchantwnd MW_Buy_Button leftmouseup
			}	
			/delay 5 ${FindItemCount[B${_itemtoBuy}]} >= ${_qty}
			/varcalc _qty ${_buyAmount}-${FindItemCount[${_itemtoBuy}]} 
			/if (${_qty} <= 0) /return
			}
			/delay 5
			/if (${_qty} > 19) /goto :loopbuyitems
		}
		/if (${_qty} > 0 && ${_qty} < 20) {
			/for _count 1 to ${_qty}
				| /if (${Bool[${InventoryFull}]}) /return 
				/if (${Target.ID} == ${Spawn[npc alerynril].ID}) {
					/ctrlkey /notify NewPointMerchantWnd NewPointMerchant_PurchaseButton leftmouseup
				} else {            	
					/ctrlkey /notify merchantwnd MW_Buy_Button leftmouseup
				}
				/delay 1s ${FindItemCount[${_itemtoBuy}]} >= ${Math.Calc[${FindItemCount[${_itemtoBuy}]}+${_count}]}
				/echo Buying ${_itemtoBuy} ${_count} of ${_qty}
				/if (${_count} >= ${_qty}) /return
			/next _count
			/varcalc _qty ${_buyAmount}-${FindItemCount[${_itemtoBuy}]}

		}
	}

/return



|***
 *	DES: finds the items to sell to a vendor
 *	USE: 
 *  NOTE: 
 ***|
sub vendor_sell(string _itemtoSell)
	/declare _pack 					int local
	/declare _slot 					int local
	/declare _lootItem 			string local
  /if (!${Window[MerchantWnd].Open}) /return

	/if (${_itemtoSell.Equal[lootfile]}) {
		| sell from top slots
		/for _pack 1 to 10
		| Check if top level inv slot is not a container/bag
		/if (${InvSlot[pack${_pack}].Item.Container} == 0) {
			| If top level inv slot is empty check for an item marked sell
			/if (${InvSlot[pack${_pack}].Item.ID}) {
				/varset _lootItem ${InvSlot[pack${_pack}].Item}
				/if (${Ini[${INILoot},"${_lootItem.Left[1]}","${_lootItem}"].Equal[Sell]}) /call vendor_sell_item "${_lootItem}"
			}
		}
		/next _pack   	
		
		| sell from bags
		/for _pack 1 to 10
			| Check if top level inv slot has a bag
			/if (${InvSlot[pack${_pack}].Item.Container} == 0) /goto :nextpack
			| Set _pack 1 to number of bag slots and loop through slots looking for items marked sell
			/for _slot 1 to ${InvSlot[pack${_pack}].Item.Container}
				/if (${InvSlot[pack${_pack}].Item.Item[${_slot}].ID}) {
					/varset _lootItem ${InvSlot[pack${_pack}].Item.Item[${_slot}].Name}
					/if (${Ini[${INILoot},"${_lootItem.Left[1]}","${_lootItem}"].Equal[Sell]}) /call vendor_sell_item "${_lootItem}"
				}
			/next _slot
			:nextpack
		/next _pack  	
		/doevents flush sellstuff
	} else {
		/call vendor_sell_item "${_itemtoSell}"
	}
	VOUT ${onotice} ${sep} Done Selling.

/return



|***
 *	DES: sells the actual item to the vendor
 *	USE: 
 *  NOTE: 
 ***|
sub vendor_sell_item(string _itemtoSell)
  /if (!${Window[MerchantWnd].Open}) {
  	VOUT Go and open a merchant window.
  	/return
  }

	/while (${FindItemCount[=${_itemtoSell}]}) {
		VOUT Selling ${sep} ${citem}${_itemtoSell}\ax ${sep} (${cnum}${FindItemCount[${_itemtoSell}]}\ax)
		/if (${InvSlot[pack${Select[${FindItem[=${_itemtoSell}].ItemSlot},23,24,25,26,27,28,29,30,31,32]}].Item.Container}) {
			/squelch /nomodkey /itemnotify in Pack${Math.Calc[${FindItem[=${_itemtoSell}].ItemSlot}-22]} ${Math.Calc[${FindItem[=${_itemtoSell}].ItemSlot2}+1]} leftmouseup
		} else /if (${FindItem[=${_itemtoSell}].InvSlot}) {
			/squelch /nomodkey /itemnotify ${FindItem[=${_itemtoSell}].InvSlot} leftmouseup
		} else /if (!${FindItem[=${_itemtoSell}].InvSlot}) {
			VOUT ${onotice} ${sep} No ${citem}${_itemtoSell}\ax to sell.
			/break
		}
		/delay 5
		/nomodkey /shiftkey /notify merchantwnd MW_Sell_Button leftmouseup
		/delay 1s
	}

/return



|***
 *	DES: Make sure the AA is up all the time
 *	USE: /call check_AA_now
 *  NOTE: This is hardcoded for only 2 skills. suck it up.
 ***|
sub check_AA_now()
	SIMPLEEXIT
	AMIDEAD

	/if (${Bool[${AANow1}]} && !${Me.Buff[${AANow1}].ID} && !${Me.Song[${AANow1}].ID}) {
		/if (${validate_cast[FALSE, alt, "${AANow1}", ${Me.ID}]}) /call core_cast2 "${AANow1}" alt 0 FALSE
	}

	/if (${Bool[${AANow2}]} && !${Me.Buff[${AANow2}].ID} && !${Me.Song[${AANow2}].ID}) {
		/if (${validate_cast[FALSE, alt, "${AANow2}", ${Me.ID}]}) /call core_cast2 "${AANow2}" alt 0 FALSE
	}
/return


|***
 *	DES: Make sure the Item Effect is up all the time
 *	USE: /call check_item_now
 *  NOTE:
 ***|
sub check_item_now()
	SIMPLEEXIT
	AMIDEAD

	/if (${Bool[${ItemNow1}]}) {
		/if (${validate_cast[FALSE, item, "${ItemNow1}", ${Me.ID}]}) /call core_cast2 "${ItemNow1}" item 0 FALSE
	}

	/if (${Bool[${ItemNow2}]}) {
		/if (${validate_cast[FALSE, alt, "${ItemNow2}", ${Me.ID}]}) /call core_cast2 "${ItemNow2}" alt 0 FALSE
	}
/return



|***
 *	DES: Make sure the Disc is up all the time
 *	USE: /call check_disc_now
 *  NOTE: This is hardcoded for only 2 skills.
 ***|
sub check_disc_now()
	SIMPLEEXIT
	AMIDEAD

	| nowDisc1
	/if (${Bool[${DiscNow1}]} && ${Me.CombatAbilityReady[${DiscNow1}]} && ${Spell[${DiscNow1}].NewStacks}) {
		/if (${Select[${Me.Class.ShortName},BRD]} && ${Me.Casting.ID}) /call pulse_stop 7
		/if (!${Bool[${Me.Buff[${DiscNow1.Left[-7]}]}]} && !${Bool[${Me.Song[${DiscNow1.Left[-7]}]}]}) {
			/if (${validate_cast[FALSE, disc, "${DiscNow1}", ${Me.ID}]}) /call core_cast2 "${DiscNow1}" disc 0 FALSE
		}
	}
	| nowDisc2
	/if (${Bool[${DiscNow2}]} && ${Me.CombatAbilityReady[${DiscNow2}]} && ${Spell[${DiscNow2}].NewStacks}) {
		/if (${Select[${Me.Class.ShortName},BRD]} && ${Me.Casting.ID}) /call pulse_stop 8
		/if (!${Bool[${Me.Buff[${DiscNow2.Left[-7]}]}]} && !${Bool[${Me.Song[${DiscNow2.Left[-7]}]}]}) {
			/if (${validate_cast[FALSE, disc, "${DiscNow2}", ${Me.ID}]}) /call core_cast2 "${DiscNow2}" disc 0 FALSE
		}
	}	

/return



|***
 *	DES: 
 *	USE: /call get_target_advanced
 *  NOTE: 
 ***|
sub get_target_advanced(bool _debug)
	/if (!${swAdvPull} || !${coreAuto} || ${swPull}) /return
	AMIDEAD
	CHECKINPUTS
	CHECKTIE



	| Advanced pull code here



/return



|***
 *	DES: last mob is dedded we need a new thing(s) to kill
 *	USE: /call get_target
 *  NOTE: got lost hunting for bitches. have your people call my people.
 *	SERIOUS: this will find a valid target to kill within the pullers defined agro range. bring it to the group and start teaching the gospel.
 ***|
sub get_target(bool _debug)
	/if (!${swPull} || !${coreAuto} || ${swAdvPull}) /return
	AMIDEAD
	CHECKINPUTS
	CHECKTIE
	/declare _tmpID int local
	/declare _tmpAgroRange int local ${Math.Calc[${setMobAgro}/10]}
	/declare _tmpCombatRange int local 31
	/declare _count int local 0
	/declare _mobHP int local 102
	/declare _priorityMob bool local FALSE
	
	/if (!${swOffTank}) {
		UPDATEHUD "Pull"		
	} else /if (${swOffTank})
		UPDATEHUD "Grab"		
	}
	
	| /if (${_debug}) /echo count:${SpawnCount[npc ${If[${swOverrideLOS},,los]} range ${setMobLvlMin} ${setMobLvlMax} radius ${_tmpAgroRange} zradius ${setzradius} targetable]}

	| time to step the agro range out so we get mobs closer first	
	/while (${_tmpAgroRange} <= ${setMobAgro} && !${Target2Kill}) {

		| search local ${setMobAgro} range in steps of 10 for mob acquisition
		| /echo count:${SpawnCount[npc ${If[${swOverrideLOS},,los]} range ${setMobLvlMin} ${setMobLvlMax} radius ${_tmpAgroRange} zradius ${setzradius} targetable]}
		/if (${SpawnCount[npc ${If[${swOverrideLOS},,los]} range ${setMobLvlMin} ${setMobLvlMax} radius ${_tmpAgroRange} zradius ${setzradius} targetable]}) {
			/for _count 1 to ${SpawnCount[npc ${If[npc ${If[${swOverrideLOS},,los]} range ${setMobLvlMin} ${setMobLvlMax} radius ${_tmpAgroRange} zradius ${setzradius} targetable]}
				/if (!${swPull} || !${coreAuto}) /return
				AMIDEAD
				
				/varset _tmpID ${NearestSpawn[${_count}, npc ${If[${swOverrideLOS},,los]} range ${setMobLvlMin} ${setMobLvlMax} radius ${_tmpAgroRange} zradius ${setzradius}].ID}
				| /echo _count:${_count} _tmpID:{_tmpID}
				
				| these things are our friends. dont kill them
				/if (${Select[${Spawn[id ${_tmpID}].Type},AURA,BANNER,CAMPFIRE,CORPSE,CHEST,ITEM,TRIGGER,TRAP,TIMER,MOUNT,Mercenary]}) /continue
				/if (${ExcludeList.Find[${Spawn[id ${_tmpID}].DisplayName}]}) /continue
				| /echo /if (${ExcludeList.Find[${Spawn[id ${_tmpID}].DisplayName}]}) /continue
				/if (${_tmpID} == ${Me.Pet.ID}) /continue
				
				| CYA for shit MQ will pick up in the environment that does not have a name
				/if (${Spawn[id ${_tmpID}].DisplayName.Length} < 1) /continue

				| get the lowest HP mob if we can
				/if (${Spawn[id ${_tmpID}].PctHPs} <= ${_mobHP}) {
					/varset _mobHP ${Spawn[id ${_tmpID}].PctHPs}
					/varset Target2Kill ${_tmpID}
					/varset combat COMBAT
				}

				| is this a priority mob?
				/if (${NoMezzList.Find[${Spawn[id ${_tmpID}].DisplayName}]} || ${NameList.Find[${Spawn[id ${_tmpID}].DisplayName}]}) {

					| off tanks have to sort by a different position
					/if (${swOffTank} && ${Me.XTarget[1].ID} == ${_tmpID}) /continue 

					/varset _priorityMob TRUE
					/break
				}					
		
			/next _count
		}
		
		| offtank has nothing special to do? back to your damn job!
		/if (${swOffTank} && !${_priorityMob}) {
			/call get_assist_target
			/return
		}
		
		/if (${_priorityMob}) /break
		/varcalc _tmpAgroRange ${_tmpAgroRange}+${setCombatDistance}
	}
		
	| lastly, kill all the evil pets. if the owner is dead.
	/if (!${_priorityMob}) {
		/if (${SpawnCount[pet ${If[${swOverrideLOS},,los]} range ${setMobLvlMin} ${setMobLvlMax} radius ${setMobAgro} zradius ${setzradius} targetable]} && !${SpawnCount[npc ${If[${swOverrideLOS},,los]} range ${setMobLvlMin} ${setMobLvlMax} radius ${setCombatRadius} zradius ${setzradius} targetable]}) {
			/for _count 1 to ${SpawnCount[pet ${If[${swOverrideLOS},,los]} range ${setMobLvlMin} ${setMobLvlMax} radius ${setMobAgro} zradius ${setzradius} targetable]}
				/if (!${swPull} || !${coreAuto}) /return
				AMIDEAD
				/varset _tmpID ${NearestSpawn[${_count}, pet ${If[${swOverrideLOS},,los]} range ${setMobLvlMin} ${setMobLvlMax} radius ${setMobAgro} zradius ${setzradius} targetable].ID}
				/if (${Spawn[id ${_tmpID}].Master.Type.Equal[Corpse]} || !${Spawn[id ${_tmpID}].Master.ID}) /varset Target2Kill ${_tmpID}
			/next _count	
		} 
	}

	/if (${Target2Kill}) {
		| Disable /camp for pull
		/if (${swHomeSet}) {
			/squelch /makecamp pause
		}
	
		| target the pull
		/if (${Target.ID} != ${Target2Kill}) /squelch /target ID ${Target2Kill}
		
		/varset _tmpCombatRange ${Spawn[id ${Target2Kill}].MaxRangeTo}
	}
		
	| get the mob to us
	/while (!${is_target_dead[${Target2Kill}]} && ${Spawn[id ${Target2Kill}].Distance} >= ${_tmpCombatRange} && !${Select[${Me.Class.ShortName},MAG,NEC,BST,ENC]} && ${Spawn[ID ${Target2Kill}].Distance} <= ${setMobAgro}) { 
		CLEARCURSOR TRUE
		CHECKINPUTS
		AMIDEAD

		/if (${SpawnCount[npc ${If[${swOverrideLOS},,los]} radius ${setMobAgro} targetable]} && ${Me.State.NotEqual[STAND]}) /stand
		/if (${Me.Stunned}) /return
		/if (!${swPull} || !${coreAuto}) /return
		
		| keep us facing the target just enough...
		/if (${Target.ID} && (${swCombatMelee} || ${swCombatRange})) {
			/if (!${Range.Between[0,${setFaceArc}:${Math.Abs[${Math.Calc[${Target.HeadingTo.Degrees}-${Me.Heading.Degrees}]}]}]}) /face ID ${Target2Kill} nolook ${If[${swFaceFast},fast,]}
		}
			
		| check target is LoS. if stays out of sight too long. reset
		/if (!${Spawn[${Target}].LineOfSight} && !${swOverrideLOS}) {
			/delay 2s ${Spawn[${Target}].LineOfSight}
			/if (!${Spawn[${Target}].LineOfSight}) {
				/call clear_combat
				/return
			}
		
		| check targets range. if out of range too long. reset
		} else /if (${Spawn[${Target}].Distance} > ${setMobAgro}) {
			/delay 2s ${Spawn[${Target}].Distance} < ${setMobAgro}
			/if (${Spawn[${Target}].Distance} > ${setMobAgro}) {
				/call clear_combat
				/return
			}
		}
		
		| shadowknight attraction
		/if (${Select[${Me.Class.ShortName},SHD]}) {
			/if (${switchAttraction} && ${Target.Distance} < ${setMobAgro} && ${Target.Distance} > 40) {
				/if (${validate_cast[FALSE, alt, "Hate's Attraction", ${Target2Kill}]}) /call core_cast2 "Hate's Attraction" alt ${Target2Kill} FALSE
			}
		
		| paladin attraction
		} else /if (${Select[${Me.Class.ShortName},PAL]}) {
			/if (${switchAttraction} && ${Target.Distance} < ${setMobAgro} && ${Target.Distance} > 40) {
				/if (${validate_cast[FALSE, alt, "Divine Call", ${Target2Kill}]}) /call core_cast2 "Divine Call" alt ${Target2Kill} FALSE
			}
			
		| warrior attraction
		} else /if (${Select[${Me.Class.ShortName},WAR]}) {
			/if (${switchAttraction} && ${Target.Distance} < ${setMobAgro} && ${Target.Distance} > 40) {
				/if (${validate_cast[FALSE, alt, "Warlord's Grasp", ${Target2Kill}]}) /call core_cast2 "Warlord's Grasp" alt ${Target2Kill} FALSE
			}
		}

		| spell pull mob
		/if (${Bool[${Spell[${strRangeSpell}]}]} && ${Bool[${Me.Gem[${strRangeSpell}]}]}) {
			/if (${validate_cast[FALSE, spell, "${strRangeSpell}", ${Target2Kill}]}) /call core_cast2 "${strRangeSpell}" 0 ${Target2Kill} FALSE

		| item pull mob
		} else /if (${Me.ItemReady[${setRangeClickItem}]}) {
			/if (${validate_cast[FALSE, item, "${setRangeClickItem}", ${Target2Kill}]}) /call core_cast2 "${setRangeClickItem}" item ${Target2Kill} FALSE

		| disc pull mob
		} else /if (${Me.CombatAbilityReady[${setRangeDisc}]}) {
			/if (${validate_cast[FALSE, disc, "${setRangeDisc}", ${Target2Kill}]}) /call core_cast2 "${setRangeDisc}" disc ${Target2Kill} FALSE

		| AA pull mob
		} else /if (${Me.AltAbilityReady[${setRangeAA}]}) {
			/if (${validate_cast[FALSE, alt, "${setRangeAA}", ${Target2Kill}]}) /call core_cast2 "${setRangeAA}" alt ${Target2Kill} FALSE

		| pew pew			/echo pew pew
		} else {
			/if (${Spawn[ID ${Target2Kill}].Distance} > 31) {
				/for _count 1 to ${lstRangeItem.Count[|]}
					/if (!${FindItemCount[${lstRangeItemSummon.Arg[${_count},|]}]} && ${Bool[${lstRangeItemSummon}]}) /call check_ammo
				/next _count
				
				/if (${Bool[${swBandolier}]} && ${Bool[${bandolierRange}]} && ${Bool[${bandolierNormal}]}) {
					/call bandolier_swap Range
					/range
					/call bandolier_swap Normal
				} else /if (!${Bool[${swBandolier}]}) {
					/range
				}
			}
		}

	| stick to the mob at /cc combatdistance ##
	/if (${Spawn[ID ${Target2Kill}].Distance} < 32 && ${Spawn[ID ${Target2Kill}].Distance} > ${setCombatDistance}) {
			/if (${swCombatMelee}) /squelch /stick ID ${Target2Kill} ${setCombatDistance}
		}

		/delay 1
		/if (${is_target_dead[${Target2Kill}]}) /break
	}
		

	/if (${Spawn[ID ${Target2Kill}].Distance} <= ${_tmpCombatRange} && !${Select[${Me.Class.ShortName},MAG,NEC,BST,ENC]} && !${Me.Rooted.ID}) {
		/if (${swCombatMelee}) /squelch /stick ID ${Target2Kill} ${setCombatDistance}
		/return

	| pet auto send to kill
	} else /if (${Target2Kill} && ${Spawn[id ${Target2Kill}].Distance} <= ${setMobAgro} && ${Select[${Me.Class.ShortName},MAG,NEC,BST,ENC]}) {
		/if (${Target.ID} != ${Target2Kill}) /squelch /target ID ${Target2Kill}
		/if (${Target.ID} && !${Bool[${Me.Pet.Target}]}) {
			/squelch /pet attack		
		}
	}

	UPDATEHUD FALSE

/return



|***
 *	DES: tells your BC people to hand in up to 4 items to your target. includes command giver
 *	USE: /bc handin |item1|item2|item3|item4
 *  NOTE: THIS HAS NOT BEEN TESTED!! you could be handing in your soul and not even know it.
 ***|
sub hand_in(string _sender, string _verbage)
	/if (${Spawn[pc ${_sender}].ID}) /assist ${_sender}
	/delay 1s ${Me.AssistComplete}
	/if (Target.Distance} > 100) /return
	/if (Target.Distance} < 100) /squelch /stick 10
	/delay 5
	/if (${Target.ID}) {

		| item 1
		/if (${FindItem[${_verbage.Arg[1,|]}].ID}) {
			/nomodkey /shift /itemnotify "${_verbage.Arg[2,|]}" leftmouseup
			/click left target
			/delay 5 !${Cursor.ID}
		} else {
			VOUT Didn't Find ${_verbage.Arg[1,|]}
		}

		| item 2
		/if (${FindItem[${_verbage.Arg[2,|]}].ID}) {
			/nomodkey /shift /itemnotify "${_verbage.Arg[3,|]}" leftmouseup
			/click left target
			/delay 5 !${Cursor.ID}
		} else {
			VOUT Didn't Find ${_verbage.Arg[2,|]}
		}

		| item 3
		/if (${FindItem[${_verbage.Arg[3,|]}].ID}) {
			/nomodkey /shift /itemnotify "${_verbage.Arg[4,|]}" leftmouseup
			/click left target
			/delay 5 !${Cursor.ID}
		} else {
			VOUT Didn't Find ${_verbage.Arg[3,|]}
		}

		| item 4
		/if (${FindItem[${_verbage.Arg[4,|]}].ID}) {
			/nomodkey /shift /itemnotify "${_verbage.Arg[5,|]}" leftmouseup
			/click left target
			/delay 5 !${Cursor.ID}
		} else {
			VOUT Didn't Find ${_verbage.Arg[4,|]}
		}

		/nomodkey /notify GiveWnd GVW_give_Button leftmouseup
	}

/return



|***
 *	DES: will dump all worn or invnetory augments into an INI.
 *	USE: /echoaugments
 *  NOTE: recursive of augments in items in bags.
 ***|
sub echo_augments()
	/declare _slot 			int local
	/declare _augslot 	int local
	/declare _bagslot 	int local
	
	| worn inventory skip powersource (21)
	/for _slot 0 to 22
		/if (${_slot} == 21) /continue
		/for _augslot 0 to ${Me.Inventory[${_slot}].Augs}
			/if (${Me.Inventory[${_slot}].AugSlot[${_augslot}].Empty}) /continue
			/ini "${INIAugDump}" "${Me.DisplayName} - Worn" "${Me.Inventory[${_slot}].Name} ${Me.Inventory[${_slot}].AugSlot[${_augslot}].Type}" "${Me.Inventory[${_slot}].AugSlot[${_augslot}].Name}"
		/next _augslot
	/next _slot	

	
	| bag inventory
	/for _slot 23 to 32
			| top slot augs
			/if (${Me.Inventory[${_slot}].Type.Equal[Augmentation]}) {
				/ini "${INIAugDump}" "${Me.DisplayName} - Inventory" "Slot:${_slot}" "${Me.Inventory[${_slot}].Name}"
			}

			| top slot item	with augs
			/if (${Me.Inventory[${_slot}].Augs}) {
				/for _augslot 0 to ${Me.Inventory[${_slot}].Augs}
					/if (!${Me.Inventory[${_slot}].AugSlot[${_augslot}].Empty}) /ini "${INIAugDump}" "${Me.DisplayName} - Inventory" "${Me.Inventory[${_slot}].Name} ${Me.Inventory[${_slot}].AugSlot[${_augslot}].Type}" "${Me.Inventory[${_slot}].AugSlot[${_augslot}].Name}"
				/next _augslot
			}
				
			| inside bags
			/if (${InvSlot[${_slot}].Item.Container}) {
				/for _bagslot 1 to ${InvSlot[${_slot}].Item.Container}
					/if (${Me.Inventory[${_slot}].Item[${_bagslot}].Type.Equal[Augmentation]}) {
						/ini "${INIAugDump}" "${Me.DisplayName} - Inventory" "${Me.Inventory[${_slot}].Name}:${_slot}" "${Me.Inventory[${_slot}].Item[${_bagslot}].Name}"
					}
					/if (${Me.Inventory[${_slot}].Item[${_bagslot}].Augs}) {
	
						/for _augslot 0 to ${Me.Inventory[${_slot}].Item[${_bagslot}].Augs}
							/if (!${Me.Inventory[${_slot}].Item[${_bagslot}].AugSlot[${_augslot}].Empty}) /ini "${INIAugDump}" "${Me.DisplayName} - Inventory" "${Me.Inventory[${_slot}].Item[${_bagslot}].Name} ${Me.Inventory[${_slot}].Item[${_bagslot}].AugSlot[${_augslot}].Type}" "${Me.Inventory[${_slot}].Item[${_bagslot}].AugSlot[${_augslot}].Name}"
						/next _augslot	
					}
				/next _bagslot
			}
		/next _slot
		VOUT Augments Dumped.

/return



|***
 *	DES: will watch for a spawn in the zone
 *	USE: /watchspawn [NAME]
 *  NOTE: resets on start
 ***|
sub check_watch_spawn()
	/declare _correctWatchCount int local ${SpawnCount[npc ${watchSpawn}]}
	/varset hudwatchspawn ${_correctWatchCount}
	
	| count mismatch
	/if (${watchSpawnCount} != ${_correctWatchCount}) {
		/varset watchSpawnCount ${_correctWatchCount}
		/squelch /mapshow ${watchSpawn}
	}

	/call timer_update timer_check_watch_spawn ${tWatch_Spawn}

/return



|***
 *	DES: Simply gives cash to an NPC. 
 *	USE: /givecoin [ammount] [stacks] [plat|gold|copper|silver]
 *  NOTE: It's not dumb if it works.
 ***|
sub give_coin(int _ammount, int _stacks, string _coin)
	/declare _count 			int local 
	/declare _countloop 	int local 1
	/declare _coinID			int local 99

	| check target
	/if (!${Target.ID} || ${Taret.ID} == ${Me.ID} || ${Target.ID} == ${Pet.ID}) {
		VOUT ${owarning} ${sep} Need a target to give the coin too. Try again.
		/return
	}
	
	| check coin type
	/if (${_coin.Equal[plat]}) {
		/varset _coinID 0
	} else /if (${_coin.Equal[gold]}) {
		/varset _coinID 1
	} else /if (${_coin.Equal[silver]}) {
		/varset _coinID 2
	} else /if (${_coin.Equal[copper]}) {
		/varset _coinID 3
	} else /if (${_coin.Equal[NULL]}) {
		VOUT ${owarning} ${sep} Invalid coin type. Try again.
		/return
	}

	/while (${_countloop} < ${_ammount}) {
		/if (!${Window[InventoryWindow].Open}) /invoke ${Window[InventoryWindow].DoOpen}
		/if (!${Target.ID} || ${Taret.ID} == (${Me.ID} || ${Pet.ID})) /return
		/delay 2s
		/for _count 1 to ${_stacks}
			/ctrl /notify InventoryWindow IW_Money${_coinID} leftmouseup
			/delay 5
			/click left target 
			/delay 5 
		/next _count 
		/delay 3 
		/notify GiveWnd GVW_Give_Button leftmouseup 

		/delay 5 
		/varset _countloop ${Math.Calc[${_countloop}+1]}
	}

	VOUT ${onotice} ${sep} Done giving cash for bribes.

/return



|***
 *	DES: keeps SK/PAL harmonious up.. always..
 *	USE: /call check_harmonious
 *  NOTE: 
 ***|
sub check_harmonious()
	/if (!${switchHarmonious}) /return
	AMIDEAD
	CHECKEXIT
	CHECKINPUTS
	/if (!${Bool[${Me.Buff[${Harmonious}]}]} && ${Me.PctMana} > 10) {
		/if (${validate_cast[FALSE, spell, "${Harmonious}", ${Me.ID}]}) /call core_cast2 "${Harmonious}" ${buffGem} 0 FALSE
	}

/return



|***
 *	DES: prep sub for all calsses to decide if they are going to be in combat and if its time to burn
 *	USE: /call prep_combat
 *  NOTE:
 **|
sub prep_combat()
	/if (${Me.Zoning}) /return FALSE
  CHECKINPUTS
	CLEARCURSOR
	
 	| manual mode
	/if (!${coreAuto}) {
		/if (!${Bool[${combat}]}) /return FALSE
		/if (${Target.ID}) {
			/if (${Target.ID} == ${Me.ID}) /return FALSE
			/if (${Bool[${combat}]}) {
				/if (${is_burn_time[]}) /varset swBurnControl TRUE
			}
			| /if (${Me.MercID}) /call merc_control
			/return TRUE
		} else /if (!${Target.ID}) {
			/if (${Bool[${combat}]} || ${isValidTarget}) /call clear_combat
			/return FALSE
		}
	
	| combat healing
	} else /if (${coreAuto} && ${swCombatHeal}) {

		| heal check
		/if (${timer_zoned_delay}) /return FALSE
		/if (!${SafeZone} && !${swOverrideSafeZone}) /call set_base_target_timers ${Target2Kill}
		/if (${validate_target[FALSE, ${Target2Kill}]}) /varset isValidTarget TRUE
		/varset combat COMBAT
		/if (${is_burn_time[]}) /varset swBurnControl TRUE
		| /if (${Me.MercID}) /call merc_control
		/return TRUE
		
	| offtank check for mobs
	} else /if (${coreAuto} && ${swOffTank}) {
		/call get_target
		/if (${Me.XTarget[1].ID} != ${Target2Kill}) {
			/varset combat COMBAT
			/return TRUE
		}
	
	| automatic mode
	} else /if (${coreAuto}) {
		/if (!${Me.Invis} || (${coreAuto} && ${Select[${Me.Class.ShortName},ROG]} && ${swSoS})) {
			/if (${validate_target[FALSE, ${Target2Kill}]}) /varset isValidTarget TRUE
			/if (!${isValidTarget} || !${Spawn[${Target2Kill}].ID}) {
				/call clear_combat
				/return FALSE

			} else /if (${isValidTarget} && ${Spawn[${Target2Kill}].ID}) {
				/if (!${SafeZone} && !${swOverrideSafeZone}) /call set_base_target_timers ${Target2Kill}
				/if (${Me.Class.ShortName.Equal[BRD]}) {
					/if (${Me.Song[${spellRestSong}].ID}) /stopsong
				}
				/varset combat COMBAT
				/if (${is_burn_time[]}) /varset swBurnControl TRUE
				| /if (${Me.MercID}) /call merc_control
				/return TRUE
				
			}
		}
	}

/return FALSE



|***
 *	DES: do we want to exit the class while loop
 *	USE: ${exit_class_loop[]}
 *  NOTE: assume no exit (FALSE)
 ***|
sub exit_class_loop()
	/if (${coreAuto}) {	
		| no target.. were done
		/if (!${Spawn[${Target2Kill}].ID}) /return TRUE
		| targets dead. why bother
		/if (${is_target_dead[${Target2Kill}]}) /return TRUE
		
	| manual mode	
	} else /if (!${coreAuto}) {
		| no target. go away
		/if (!${Target.ID}) /return TRUE
		| forced combat in manual
		/if (${Bool[${combat}]}) {
			| target dead. end
			/if (${is_target_dead[${Target2Kill}]}) /return TRUE
		}

	}
	
/return FALSE



 |***
 *	DES: Check if we need to engage burn routines. /combat heal/dual are treated differently
 *	USE: auto
 *  NOTE: we work off established assist target for non healers only.
 ***|
sub is_burn_time()

	| dont do shit in safe zone..
	/if (${SafeZone} && !${swOverrideSafeZone}) {
		/varset swBurnControl FALSE
		/return FALSE
	}


	/if (${coreAuto}) {

	}
	
	| no raid burning if turned off
	/if (!${swBurnRaid} && ${Raid.Members}) {
		/varset swBurnControl FALSE
		/return FALSE
	}
	
	| if always set to burn, always burn while in combat
	/if (${swBurnForce} && ${Me.Combat}) /return TRUE
	
	| remainder requires automated modes
	/if (!${coreAuto}) /return FALSE

	| no mobs around
	/if (!${SpawnCount[npc radius ${setMobAgro} playerstate 4]}) {
		/varset swBurnControl FALSE
		/return FALSE
	}

	
	| if the current target is a namer
	/if (${swBurnAuto} && ${Target2Kill} && ${Spawn[${Target2Kill}].PctHPs} <= ${setBurnPct} && (${NameList.Find[${Target.DisplayName}]} && !${swOverrideName})) /return TRUE
	

	| if number of mobs in the local area reaches the /burn count ## threashold
	/if (${setBurnCount}) {
		/if (${SpawnCount[npc radius ${setMobAgro} targetable playerstate 4]} >= ${setBurnCount}) /return TRUE
	}
	
	| healers and off combat routines simply check for a named in the local area to start a burn. no direct target of a named is required. just be in the area.
	/if (!${swCombatHeal}) /return FALSE

	/if (${Bool[${combat}]}) {
		/declare _count int local 0
		/declare _mobID int local 0

		/for _count 1 to ${SpawnCount[npc radius ${setMobAgro} playerstate 4]}
			/varset _mobID ${NearestSpawn[${_count}, npc radius ${setMobAgro}].ID}
			/if (${swBurnAuto} && ${NameList.Find[${Spawn[id ${_mobID} radius ${setMobAgro}].DisplayName}]} && !${swOverrideName}) /return TRUE
		/next _count 	
	}
	
/return FALSE



|***
 *	DES: Designed to check for the one-off crap.
 *	USE: /call check1offBuffs
 *  NOTE: THIS NEEDS TO BE WORKED OUT OF THE CODE.. MOVE TO check_exit...
 ***|
sub check1offBuffs()
	| AMIDEAD
	| /if (${Me.Buff[Holistic Health].ID}) /call cure_Anashti_DT
	| /if (${Me.Buff[Pyroignition].ID}) /call CurePyroignitionDot
	| /if (${Me.Buff[Withering Physicality].ID} || ${Me.Buff[Withering Limbs].ID} || ${Me.Buff[Withering Faith].ID}) /call CureDamselofDecay

/return




|***
 *	DES: food coma time
 *	USE: /call last_supper 
 *  NOTE: 
 ***|
sub last_supper()
	| summon food and drink if we are using items	
	| summon food
	/if (${Bool[${FoodDispenser}]}) {
		/if (${FindItemCount[=${Food}]} < ${FoodCount}) {
			/if (${validate_cast[FALSE, item, "${FoodDispenser}", ${Me.ID}]}) /call core_cast2 "${FoodDispenser}" item 0 FALSE
			CLEARCURSOR TRUE
		}		
	}
	
	| summon drink
	/if (${Bool[${DrinkDispenser}]}) {
		/if (${FindItemCount[=${Drink}]} < ${DrinkCount}) {
			/if (${validate_cast[FALSE, item, "${DrinkDispenser}", ${Me.ID}]}) /call core_cast2 "${DrinkDispenser}" item 0 FALSE
			CLEARCURSOR TRUE
		}		
	}
	
	| dont spam this shit
	/call timer_update timer_meal ${tFoodDrink}		

	| for a frame of reference hunger and thirst seem to go to ~10k
	| one bite of black bread from a cleric is 500
	| one drink of a kadim constitution is almost 6k

	| eat, drink and be merry
	/if (${FindItemCount[${Food}]} && ${Me.Hunger} < 3000) /useitem "${Food}"
	/if (${FindItemCount[${Drink}]} && ${Me.Thirst} < 3000) /useitem "${Drink}"
			
/return



|***
 *	DES: check all buff routines
 *	USE: /call check_buff_routines [TRUE|FALSE] TRUE forces the check even when in manual
 *  NOTE: 
 ***|
sub check_buff_routines(bool _force)
	/if (!${coreAuto} && !${_force}) /return
	AMIDEAD
	CHECKINPUTS
	/if (${timer_zoned_delay} && !${_force}) /return FALSE
	/if (!${Me.Invis} || (${Select[${Me.Class.ShortName},ROG]} && ${swSoS})) {
		
		/if (${swBuffGemHold}) {
			/declare _tmpSpell string local FALSE
			/if (${Bool[${Me.Gem[${fix_gem_id[${buffGem}]}]}]})  {
				/varset _tmpSpell ${Me.Gem[${fix_gem_id[${buffGem}]}]}
			}		
		}
		
		| check tell request for buffs
	  /if (${swBuffTell} && !${timer_check_buffs_tell}) /call check_buffs_tell
		
		| class specific buff checks
		/if (${Select[${Me.Class.ShortName},BER]}) {
		} else /if (${Select[${Me.Class.ShortName},BRD]}) {
		} else /if (${Select[${Me.Class.ShortName},BST]}) {
		} else /if (${Select[${Me.Class.ShortName},CLR]}) {
		} else /if (${Select[${Me.Class.ShortName},DRU]}) {
			/if (${switchGrowth}) /call check_buffs_tmp_list "${spellGrowth}" "${spellGrowthToon}"
		} else /if (${Select[${Me.Class.ShortName},ENC]}) {
			/if (!${timer_check_runes}) /call check_rune_${Me.Class.ShortName}
 			/if (${swBuffItem}) /call check_crystals
		} else /if (${Select[${Me.Class.ShortName},MAG]}) {
			| /call Bind_setMAG rods
		} else /if (${Select[${Me.Class.ShortName},MNK]}) {
		} else /if (${Select[${Me.Class.ShortName},NEC]}) {
		} else /if (${Select[${Me.Class.ShortName},PAL]}) {
			/if (${switchHarmonious}) /call check_harmonious
			/if (${switchSteel} && !${timer_check_Steel}) /call check_steel	
			/if (${switchStance} && (!${timer_check_Stance} || !${Bool[${Me.Buff[${Stance}]}]})) /call check_stance
		} else /if (${Select[${Me.Class.ShortName},RNG]}) {
		} else /if (${Select[${Me.Class.ShortName},ROG]}) {
		} else /if (${Select[${Me.Class.ShortName},SHD]}) {
			/if (${switchHarmonious}) /call check_harmonious
			/if (${switchSkin} && !${timer_check_Skin}) /call check_skin
			/if (${switchStance} && (!${timer_check_Stance} || !${Bool[${Me.Buff[${Stance}]}]})) /call check_stance
		} else /if (${Select[${Me.Class.ShortName},SHM]}) {
			/if (${switchGrowth}) /call check_buffs_tmp_list "${spellGrowth}" "${spellGrowthToon}"
		} else /if (${Select[${Me.Class.ShortName},WAR]}) {
		} else /if (${Select[${Me.Class.ShortName},WIZ]}) {
			/if (!${timer_check_runes}) /call check_rune_${Me.Class.ShortName}
		}

		| check main routines
		/if (!${check_engaged[]})  {
			| /if (${swBuffItem} && !${timer_check_buffs_item}) /call check_buffs_item
			/if (${swBuffSelf} && !${timer_check_buffs_self}) /call check_buffs_self
		  /if (${swBuffRaid} && !${timer_check_buffs_raid}) /call check_buffs_manual raid
			/if (${swBuffBC}  && !${timer_check_buffs_BC}) /call check_buffs_BC
			/if (${swBuffPet} && ${Me.Pet.ID} && !${timer_check_buffs_pet}) /call check_buffs_pet
			/if (${swBuffSelf} && (${Bool[${Aura1Spell}]} || ${Bool[${Aura2Spell}]}) && !${timer_check_aura}) /call check_aura
			/if (${swBuffMerc} && !${timer_check_buffs_merc}) /call check_buffs_manual mercenary
			/if (${swBuffOther} && !${timer_check_buffs_other}) /call check_buffs_manual group
			/if (${swBuffMGB} && !${timer_check_buffs_mgb}) /call check_buffs_mgb
		}

		/if (${swBuffGemHold}) {
			/if (${Bool[${_tmpSpell}]}) {
				/call mem_spell TRUE "${_tmpSpell}" ${buffGem} FALSE	
				/varset _tmpSpell FALSE
			}
		}
		
	}

/return



|***
 *	DES: finds an item
 *	USE: /call check_for_item "ITEM NAME"  .. use quotes
 *  NOTE: 
 ***|
sub check_for_item(string _verbage)
	/declare _slot 			int local
	/declare _augslot 	int local
	/declare _bagslot 	int local
	/declare _found			bool FALSE

	VOUT Searching ${sep} ${cinfo}${_verbage}\ax

	| check bank for shits
	/for _slot 1 to 28
		| -- search top bank containers
		/if (${Me.Bank[${_slot}].Container}) {
			/for _bagslot 1 to ${Me.Bank[${_slot}].Container}
				| -- ITEMS: search inside bank containers top slots
				/if (${Me.Bank[${_slot}].Item[${_bagslot}].Name.Equal[${_verbage}]}) {
					/varset _found TRUE
					VOUT Bank Bag:${cinfo}${_slot}\ax ${sep} Bag Slot:${cinfo}${_bagslot}\ax ${If[${Me.Bank[${_slot}].Item[${_bagslot}].Stack} > 1,${sep} Stack:${cgood}${Me.Bank[${_slot}].Item[${_bagslot}].Stack}\ax,]}	
				}
				| -- AUG: search aug slots in items in bags Augments in items inside bags
				/if (${Me.Bank[${_slot}].Item[${_bagslot}].Augs}) {
					/for _augslot 0 to ${Me.Bank[${_slot}].Item[${_bagslot}].Augs}
						/if (${Me.Bank[${_slot}].Item[${_bagslot}].AugSlot[${_augslot}].Name.Equal[${_verbage}]}) {
							/varset _found TRUE
							VOUT Bank Bag:${cinfo}${_slot}\ax ${sep} Slot:${cinfo}${_bagslot}\ax ${sep} Aug:${cinfo}${Int[${Math.Calc[${_augslot}+1]}]}\ax
						}
					/next _augslot	
				}
			/next _bagslot
		| -- AUG/ITEM: search top bank slots
		} else /if (!${Me.Bank[${_slot}].Container}) {
			/if (${Me.Bank[${_slot}].Name.Equal[${_verbage}]}) {
				/varset _found TRUE
				VOUT Bank Slot:${cinfo}${_slot}\ax ${If[${Me.Bank[${_slot}].Stack} > 1,${sep} Stack:${cgood}${Me.Bank[${_slot}].Stack}\ax,]}	
			}		
			| -- AUG: search top bank slot items with aug slots
			/if (${Me.Bank[${_slot}].Augs}) {
				/for _augslot 0 to ${Me.Bank[${_slot}].Augs}
					/if (${Me.Bank[${_slot}].AugSlot[${_augslot}].Name.Equal[${_verbage}]}) {
						/varset _found TRUE
						VOUT Bank Slot:${cinfo}${_slot}\ax ${sep} Aug:${cinfo}${Int[${Math.Calc[${_augslot}+1]}]}\ax
					}
				/next _augslot
			}				
		}
	/next _slot	
	
	| Augment search worn items (skip powersource)
	/for _slot 0 to 22
		/if (${_slot} == 21) /next _slot
		/for _augslot 0 to ${Me.Inventory[${_slot}].Augs}
			/if (${Me.Inventory[${_slot}].AugSlot[${_augslot}].Name.Equal[${_verbage}]}) {
				/varset _found TRUE
				VOUT ${cinfo}${Me.Inventory[${_slot}].Name}\ax ${sep} Aug:${cinfo}${Int[${Math.Calc[${_augslot}+1]}]}\ax
			}
		/next _augslot
	/next _slot	
		
	| Check Inventory
	| -- search top inventory slots
	/for _slot 23 to 32
		| -- search inside bags top slots
		/if (${InvSlot[${_slot}].Item.Container}) {
			/for _bagslot 1 to ${InvSlot[${_slot}].Item.Container}
				/if (${Me.Inventory[${_slot}].Item[${_bagslot}].Name.Equal[${_verbage}]}) {
					/varset _found TRUE
					VOUT Inventory Bag:${cinfo}${Int[${Math.Calc[${_slot}-22]}]}\ax ${sep} Slot:${cinfo}${_bagslot}\ax ${If[${Me.Inventory[${_slot}].Item[${_bagslot}].Stack} > 1,${sep} Stack:${cgood}${Me.Inventory[${_slot}].Item[${_bagslot}].Stack}\ax,]}	
				}
				| -- search aug slots in items in bags
				/if (${Me.Inventory[${_slot}].Item[${_bagslot}].Augs}) {
					/for _augslot 0 to ${Me.Inventory[${_slot}].Item[${_bagslot}].Augs}
						/if (${Me.Inventory[${_slot}].Item[${_bagslot}].AugSlot[${_augslot}].Name.Equal[${_verbage}]}) {
							/varset _found TRUE
							VOUT Inventory Bag:${cinfo}${Int[${Math.Calc[${_slot}-22]}]}\ax ${sep} Slot:${cinfo}${_bagslot}\ax ${sep} Aug:${cinfo}${Int[${Math.Calc[${_augslot}+1]}]}\ax
						}
					/next _augslot	
				}
			/next _bagslot
		} else /if (!${InvSlot[${_slot}].Item.Container}) {
			| -- search top inventory slots
			/if (${Me.Inventory[${_slot}].Name.Equal[${_verbage}]}) {
				/varset _found TRUE
				VOUT Inventory Slot:${cinfo}${Int[${Math.Calc[${_slot}-22]}]}\ax ${If[${Me.Inventory[${_slot}].Stack} > 1,${sep} Stack:${cgood}${Me.Inventory[${_slot}].Stack}\ax,]}	
			}
			| -- search top slot items with aug slots
			/if (${Me.Inventory[${_slot}].Augs}) {
				/for _augslot 0 to ${Me.Inventory[${_slot}].Augs}
					/if (${Me.Inventory[${_slot}].AugSlot[${_augslot}].Name.Equal[${_verbage}]}) {
						/varset _found TRUE
						VOUT Inventory Slot:${cinfo}${Int[${Math.Calc[${_slot}-22]}]}\ax ${sep} Aug:${cinfo}${Int[${Math.Calc[${_augslot}+1]}]}\ax
					}
				/next _augslot
			}
		}
	/next _slot
	
	/if (!${_found}) VOUT ${cbad}Not Found\ax ${sep} ${cinfo}${_verbage}\ax

/return



|***
 *	DES: one line echo bitch
 *	USE: /call echo_bitch [MQWindow|ECHO|Say]
 *  NOTE:
 ***| 	
sub echo_bitch(string _out)
	/declare _bitch					string local 
	/declare _rnd						int local
	
	:loopgetrandom
	/varset _rnd ${Math.Rand[${bitchcount}]}
	/if (!${Range.Between[1,${bitchcount}:${_rnd}]}) /goto :loopgetrandom

	/varset _bitch ${sql_return[bitch${_rnd}]}

	/if (${_out.Equal[MQWindow]}) {
		VOUT \a-g${_bitch}\ax
	} else /if (${_out.Equal[ECHO]}) {
		ECHOCHANNEL ${_bitch}
	} else /if (${_out.Equal[say]}) {
		/say ${_bitch}
	}

/return




|***
 *	DES: cast classes AA fade
 *	USE: /call cast_AA_fade
 *  NOTE:
 ***|
sub cast_AA_fade()
	AMIDEAD
		
	| mobs in range.? dont use fade for no fucking reason
	/if (!${SpawnCount[npc radius ${setCombatRadius}]}) /return FALSE
	
	| does the class even have a fade?
	/if (!${Bool[${${Me.Class.ShortName}AAFade}]}) /return FALSE

	| made it this far. may as well see if the shit works
	/if (${Me.AltAbilityReady[${${Me.Class.ShortName}AAFade}]}) {
		ECHOCHANNEL Fading ${Me.PctHPs} <= ${useAAFadePct} casting .. ${${Me.Class.ShortName}AAFade}
		/if (${validate_cast[FALSE, alt, "${${Me.Class.ShortName}AAFade}", ${Me.ID}]}) /call core_cast2 "${${Me.Class.ShortName}AAFade}" alt 0 FALSE
	}

/return TRUE



|***
 *	DES: keeps Paladin/Shadowknight Stance running
 *	USE: /call check_Stance
 *  NOTE: 
 ***|
sub check_stance()
	AMIDEAD
	CHECKEXIT
	CHECKINPUTS
	/call timer_update timer_check_Stance ${tcheck_Stance}
	/if (${validate_cast[FALSE, spell, "${Stance}", ${Me.ID}]}) /call core_cast2 "${Stance}" 0 0 FALSE

/return



 |***
 *	DES: Circle of [Power|Life|Mana]
 *	USE: keep Circle of [Power|Life|Mana] going
 *	NOTE: these are all on the same timer. so u just get one
 ***|  
sub click_burn_circle()
	/if (!${Bool[${useCircle}]}) /return FALSE
	AMIDEAD
	CHECKEXIT	
	CHECKINPUTS
	
	| circle of power
	/if (${useCircle.Equal[Power]}) {
		/if (${Bool[${Me.Song[Circle of Power]}]}) /return
		/if (${Me.ItemReady[${CircleofPower}]} && ${Spell[${FindItem[${CircleofPower}].Spell}].NewStacks}) {
			/if (${validate_cast[FALSE, item, "${CircleofPower}", ${Me.ID}]}) /call core_cast2 "${CircleofPower}" item 0 FALSE
		}

	| circle of life
	} else /if (${useCircle.Equal[Life]}) {
		/if (${Bool[${Me.Song[Circle of Life]}]}) /return
		/if (${Me.ItemReady[${CircleofLife}]} && ${Spell[${FindItem[${CircleofLife}].Spell}].NewStacks}) {
			/if (${validate_cast[FALSE, item, "${CircleofLife}", ${Me.ID}]}) /call core_cast2 "${CircleofLife}" item 0 FALSE
		}		

	| circle of Mana
	} else /if (${useCircle.Equal[Mana]}) {
		/if (${Bool[${Me.Song[Circle of Mana]}]}) /return
		/if (${Me.ItemReady[${CircleofMana}]} && ${Spell[${FindItem[${CircleofMana}].Spell}].NewStacks}) {
			/if (${validate_cast[FALSE, item, "${CircleofMana}", ${Me.ID}]}) /call core_cast2 "${CircleofMana}" item 0 FALSE
		}
	}

/return TRUE



|***
 *	DES: Clicks usefull 1 off items during a Burn
 *	USE: 
 *  NOTE: 
 ***|
sub click_item_burn()
	/declare _count int local 0
	/for _count 1 to 6
		/if (!${Bool[${clickItemBurn${_count}}]}) /continue
		/if (${is_target_dead[${Target2Kill}]}) /break
		AMIDEAD
		CHECKINPUTS
		/if (${validate_cast[FALSE, item, "${clickItemBurn${_count}}", ${Me.ID}]}) /call core_cast2 "${clickItemBurn${_count}}" item 0 FALSE
	/next _count

/return



|***
 *	DES: cast whatever spire you set for burns
 *	USE: /call cast_AA_spire
 *  NOTE: 
 ***|
sub cast_AA_spire()
	AMIDEAD
	CHECKEXIT
	CHECKINPUTS
	/if (${Raid.Members}) {
		/if (!${Me.Buff[${${Me.Class.ShortName}spire.Arg[${spireRaid},|]}].ID} && ${Spell[${${Me.Class.ShortName}spire.Arg[${spireRaid},|]}].NewStacks}) {
			/if (${validate_cast[FALSE, alt, "${${Me.Class.ShortName}spire.Arg[${spireRaid},|]}", ${Me.ID}]}) /call core_cast2 "${${Me.Class.ShortName}spire.Arg[${spireRaid},|]}" alt 0 FALSE
		}
	} else /if (!${Raid.Members} && ${Group}) {
		/if (!${Me.Buff[${${Me.Class.ShortName}spire.Arg[${spireGroup},|]}].ID} && ${Spell[${${Me.Class.ShortName}spire.Arg[${spireGroup},|]}].NewStacks}) {
			/if (${validate_cast[FALSE, alt, "${${Me.Class.ShortName}spire.Arg[${spireGroup},|]}", ${Me.ID}]}) /call core_cast2 "${${Me.Class.ShortName}spire.Arg[${spireGroup},|]}" alt 0 FALSE
		}
	}

/return



|***
 *	DES: memorize spell lines
 *	USE: /call mem_spell_line []
 *  NOTE:
 ***|
sub mem_spell_line(string _line)
	AMIDEAD
	CHECKEXIT
	CHECKINPUTS
	/declare _count 				int local
	/declare _index 				int local	
	/declare _list[1] 			string ${line${lineCurrent}}
	/declare _currentSpell 	string NULL
	
	/for _count 1 to ${_list.Size}
		/for _index 1 to ${_list[${_count}].Count[|]}
			/varset _currentSpell ${Spell[${_list[${_count}].Arg[${_index},|]}].RankName}
			/if (${Me.Gem[${_index}].Name.Equal[${_currentSong}]}) /continue
			/call mem_spell TRUE "${_currentSpell}" "${_index}" FALSE
		/next _index	
	/next _count	
	VOUT Line ${sep} ${cinfo}${lineCurrent}\ax memorization complete.

/return



|***
 *	DES: passive toggle AA Agro switches
 *	USE: 
 *  NOTE:
 ***|
sub check_AA_agro()
	AMIDEAD
	CHECKINPUTS
	
	| turn agro ON
	/if (${swAgro}) {

		| adjust bold attacks AA
		/if (${Me.AltAbilityReady[Bold Attacks: Disabled]}) {
			| /call MQ2Cast "Bold Attacks: Disabled" alt
			/alt act ${Me.AltAbility[Bold Attacks: Disabled].ID}
			VALT AA ${sep} ${cinfo}Bold Attacks\ax ${sep} ${oenabled}
		}

		| SHD Soul Flay
		/if (${Select[${Me.Class.ShortName},SHD]}) {
			/if (${Me.AltAbilityReady[Soul Flay: Disabled]}) {
				| /call MQ2Cast "Soul Flay: Disabled" alt
				/alt act ${Me.AltAbility[Soul Flay: Disabled].ID}
				VALT AA ${sep} ${cinfo}Soul Flay\ax ${sep} ${oenabled}
			}			
		}

		| BST Combat Subtlety
		/if (${Select[${Me.Class.ShortName},BST]}) {
			/if (${Me.AltAbilityReady[Combat Subtlety: Enabled]}) {
				| /call MQ2Cast "Combat Subtlety: Enabled" alt
				/alt act ${Me.AltAbility[Combat Subtlety: Enabled].ID}
				VALT AA ${sep} ${cinfo}Combat Subtlety\ax ${sep} ${odisabled}
			}			
		}		
		
		| ranger ferocious kicks
		/if (${Select[${Me.Class.ShortName},RNG]}) {
			/if (${Me.AltAbilityReady[Ferocious Kick Subtlety: Enabled]}) {
				| /call MQ2Cast "Ferocious Kick Subtlety: Enabled" alt
				/alt act ${Me.AltAbility[Ferocious Kick Subtlety: Enabled].ID}
				VALT AA ${sep} ${cinfo}Ferocious Kick Subtlety\ax ${sep} ${odisabled}
			}
		}

	| turn agro OFF
	} else /if (!${swAgro}) {

		| adjust bold attacks AA
		/if (${Me.AltAbilityReady[Bold Attacks: Enabled]}) {
			| /call MQ2Cast "Bold Attacks: Enabled" alt
			/alt act ${Me.AltAbility[Bold Attacks: Enabled].ID}
			VALT AA ${sep} ${cinfo}Bold Attacks\ax ${sep} ${odisabled}
		}

		| SHD Soul Flay
		/if (${Select[${Me.Class.ShortName},SHD]}) {
			/if (${Me.AltAbilityReady[Soul Flay: Enabled]}) {
				| /call MQ2Cast "Soul Flay: Enabled" alt
				/alt act ${Me.AltAbility[Soul Flay: Enabled].ID}
				VALT AA ${sep} ${cinfo}Soul Flay\ax ${sep} ${odisabled}
			}			
		}

		| BST Combat Subtlety
		/if (${Select[${Me.Class.ShortName},BST]}) {
			/if (${Me.AltAbilityReady[Combat Subtlety: Disabled]}) {
				| /call MQ2Cast "Combat Subtlety: Disabled" alt
				/alt act ${Me.AltAbility[Combat Subtlety: Disabled].ID}
				VALT AA ${sep} ${cinfo}Combat Subtlety\ax ${sep} ${oenabled}
			}			
		}
		
		| ranger ferocious kicks
		/if (${Select[${Me.Class.ShortName},RNG]}) {
			/if (${Me.AltAbilityReady[Ferocious Kick Subtlety: Disabled]}) {
				| /call MQ2Cast "Ferocious Kick Subtlety: Disabled" alt
				/alt act ${Me.AltAbility[Ferocious Kick Subtlety: Disabled].ID}
				VALT AA ${sep} ${cinfo}Ferocious Kick Subtlety\ax ${sep} ${oenabled}
			}
		}

	}

/return



|***
 *	DES: Challenge Spells for paladin/shadowknight
 *	USE: /call cast_challenge TARGET
 *  NOTE:
 ***|
sub cast_challenge(int _tmpID)
	/if (${Target.Buff[${ChallengeSpell}].ID}) /return
	AMIDEAD
	CHECKEXIT
	CHECKINPUTS
	/if (${validate_cast[FALSE, spell, "${ChallengeSpell}", ${_tmpID}]}) /call core_cast2 "${ChallengeSpell}" 0 ${_tmpID} cast_break

/return



|***
 *	DES: validates casting of "things on a target
 *	USE: /call ${validate_cast[[TRUE|FALSE], [spell|alt|disc|item|pulse|FIND], name, targetID, ForceTarget]
 *  Note: This is designed tomake sure whatever we are attempting to cast, is castable. be it a disc, item, spell, booger flick.. whatever.. IS IT READY TO BE USED?
 ***|
sub validate_cast(bool _debug, string _type, string _spell, int _tmpID)
	| /if (${_debug}) SDEBUG \atvalidate_cast\ax(type: ${_type}, name:${_spell}, target:${_tmpID})
	
	| _spell set to false
	/if (!${Bool[${_spell}]}) /return FALSE
	
	| is the issued target dead?
	/if (${_tmpID}) {
		/if (!${Spawn[${_tmpID}].ID}) /return FALSE
	}

	| exit in manual when force stop issued to release the target..
	/if (!${coreAuto}) {
		/if (${_tmpID} != ${Me.ID} && (!${Target.ID} || ${Target.Dead})) {
			/if (!${Spell[${_spell}].SpellType.Equal[Beneficial]}) /return FALSE
		}
	}

	CHECKINPUTS
	
	/if (${Me.Zoning}) /return FALSE
	
	| what type is it?
	/if (${_type.Equal[FIND]}) {
		/if (${Me.Book[${_spell}]}) {
			/varset _type spell
		} else /if (${Me.AltAbilityReady[${_spell}]}) {	
			/varset _type alt
		} else /if (${Me.CombatAbilityReady[${_spell}]}) {
			/varset _type disc
		} else /if (${Me.ItemReady[${_spell}]}) {
			/varset _type item
		}
	}
		
	| check spell
	/if (${_type.Equal[spell]}) {
		| /if (${_debug}) SDEBUG validate spell start: ${Spell[${_spell}].RankName}
		
		| am i casting a spell?
		/if (${Me.Casting.ID}) /return FALSE
		
		| global cooldown? is this shit broke?
		/if (${Me.SpellInCooldown}) /return FALSE
		
		| are we moving?
		/if (${Me.Moving}) /return FALSE
		/if (${Stick.Active} && !${setModeType.Equal[hunt]} && !${setModeType.Equal[scorch]}) /return FALSE
		/if (${Navigation.Active}) /return FALSE
		/if (${AdvPath.Following}) /return FALSE
		
		| correct rank name
		/varset _spell ${Spell[${_spell}].RankName}
		
		| gem in cooldown?
		/if (${Me.GemTimer[${_spell}]}) /return FALSE
		
		| enough manna to cast?
		/if (${Me.CurrentMana} < ${Spell[${_spell}].Mana}) /return FALSE
		
		| /if (!${Me.SpellReady[${_spell}]} && !${Spell[${_spell}].SpellType.Equal[Utility Beneficial]}) /return FALSE
		| /if (!${Me.SpellReady[${_spell}]} && !${Spell[${_spell}].SpellType.Equal[Beneficial]} && !${Spell[${_spell}].Category.Equal[Heals]}) /return FALSE

		| dont fail buffs so they can be memed when needed
		/if (!${Me.SpellReady[${_spell}]} && !${Spell[${_spell}].Category.Equal[Heals]}) {
			/declare _pass bool local FALSE
			/if (${Spell[${_spell}].SpellType.Equal[Beneficial]}) /varset _pass TRUE
			/if (${Spell[${_spell}].SpellType.Equal[Pet]}) /varset _pass TRUE
			/if (${Spell[${_spell}].SpellType.Equal[aura]}) /varset _pass TRUE
			
			/if (!${_pass}) /return FALSE
		}
		
		| check cast range
		/if (${Spell[${_spell}].Range}) {
			/if (${Spawn[${_tmpID}].Distance} > ${Spell[${_spell}].MyRange}) /return FALSE
		}
		| check AE range
		/if (${Spell[${_spell}].TargetType.Equal[Group V2]}) {
			/if (${Spawn[${_tmpID}].Distance} > ${Spell[${_spell}].AERange}) /return FALSE
		}
	
		| restist types vs zone control
		/if (!${Spell[${_spell}].SpellType.Equal[Beneficial]}) {
			/if (${NoDiseaseList.Find[${Target.DisplayName}]} && ${Spell[${_spell}].ResistType.Equal[Disease]}) /return FALSE
			/if (${NoMagicList.Find[${Target.DisplayName}]} && ${Spell[${_spell}].ResistType.Equal[Magic]}) /return FALSE
			/if (${NoPoisonList.Find[${Target.DisplayName}]} && ${Spell[${_spell}].ResistType.Equal[Poison]}) /return FALSE
			/if (${NoFireList.Find[${Target.DisplayName}]} && ${Spell[${_spell}].ResistType.Equal[Fire]}) /return FALSE
			/if (${NoColdList.Find[${Target.DisplayName}]} && ${Spell[${_spell}].ResistType.Equal[Cold]}) /return FALSE
			/if (${NoCorruptionList.Find[${Target.DisplayName}]} && ${Spell[${_spell}].ResistType.Equal[Corruption]}) /return FALSE
		}
		
		| check for stacking issues
		/if (${_tmpID} == ${Me.ID}) {
			/if (${Spell[${_spell}].SpellType.Equal[Beneficial]} && !${Spell[${_spell}].Category.Equal[Heals]}) {
				/if (!${Spell[${_spell}].NewStacks}) /return FALSE
				/if (${Me.Buff[${_spell}].ID}) /return FALSE
				/if (${Me.Song[${_spell}].ID}) /return FALSE		
			}
		}
		
		/if (!${Bool[${Me.Book[${_spell}]}]}) /return FALSE
		| /if (${_debug}) SDEBUG validate spell END: ${Spell[${_spell}].RankName}


	| check alt ability
	} else /if (${_type.Equal[alt]}) {
		/if (!${Me.AltAbilityReady[${_spell}]}) /return FALSE
		/if (${Me.CurrentMana} < ${Spell[${_spell}].Mana}) /return FALSE
		/if (${Spell[${_spell}].Range}) {
			/if (${Spawn[${_tmpID}].Distance} > ${Spell[${_spell}].MyRange}) /return FALSE
		}		
		| stacking
		/if (${Spell[${_spell}].SpellType.Equal[Beneficial]} && (${_tmpID} == ${Me.ID})) {
			/if (!${Spell[${_spell}].NewStacks}) /return FALSE
			/if (${Me.Buff[${_spell}].ID}) /return FALSE
			/if (${Me.Song[${_spell}].ID}) /return FALSE		
		}
	
	| check disc
	} else /if (${_type.Equal[disc]}) {
		/varset _spell ${Spell[${_spell}].RankName}
		/if (!${Bool[${_spell}]}) /return FALSE
		/if (!${Me.CombatAbilityReady[${_spell}]}) /return FALSE
		/if (${Me.CurrentEndurance} < ${Spell[${_spell}].EnduranceCost}) /return FALSE
		/if (${Spell[${_spell}].Range}) {
			/if (${Spawn[${_tmpID}].Distance} > ${Spell[${_spell}].MyRange}) /return FALSE
		}
		/if (${Bool[${Me.Song[${_spell}].Duration}]}) /return FALSE
		/if (${Bool[${Me.Buff[${_spell}].Duration}]}) /return FALSE		
					
		| stacking
		/if (${Spell[${_spell}].SpellType.Equal[Beneficial]} && (${_tmpID} == ${Me.ID})) {
			/if (!${Spell[${_spell}].NewStacks}) /return FALSE
		}

	| check disc that requires running disc slot
	} else /if (${_type.Equal[activedisc]}) {
		/varset _spell ${Spell[${_spell}].RankName}
		/if (${_debug}) OUT ${cbug}validate_cast(error:${_debug}, type:${_type}, name:${_spell}, RunningDisc:${Bool[${Me.ActiveDisc}]})\ax
		/if (!${Bool[${_spell}]}) /return FALSE
		/if (${Bool[${Me.ActiveDisc}]}) /return FALSE
		/if (!${Me.CombatAbilityReady[${_spell}]}) /return FALSE
		/if (${Me.CurrentEndurance} < ${Spell[${_spell}].EnduranceCost}) /return FALSE

	| check item
	} else /if (${_type.Equal[item]}) {

		/if (${_debug}) OUT ${cbug}validate_cast(error:${_debug}, type:${_type}, name:"${_spell}", ID:${_tmpID})\ax
		/if (!${Me.ItemReady[=${_spell}]}) /return FALSE
		/if (${_tmpID} != ${Me.ID}) {
			/if (${Spawn[${_tmpID}].Distance} > ${Spell[${FindItem[=${_spell}].Spell}].MyRange} && ${Spell[${FindItem[=${_spell}].Spell}].Range} != 0) /return FALSE
		}

		/if (${_tmpID} == ${Me.ID}) {
			/if (${Me.Buff[${FindItem[=${_spell}].Spell}].ID}) /return FALSE
			/if (${Me.Song[${FindItem[=${_spell}].Spell}].ID}) /return FALSE
		}
					
		| stacking
		/if (${Spell[${FindItem[=${_spell}].Spell}].SpellType.Equal[Beneficial]} && (${_tmpID} == ${Me.ID})) {
			/if (!${Spell[${FindItem[=${_spell}].Spell}].NewStacks}) /return FALSE
		}

	| epic clicks
	} else /if (${_type.Equal[epicitem]}) {
		/if (${_debug}) OUT ${cbug}validate_cast(error:${_debug}, type:${_type}, name:${${Me.Class.ShortName}Epic${_spell}}, ID:${_tmpID})\ax
		/if (!${Me.ItemReady[=${${Me.Class.ShortName}Epic${_spell}}]}) /return FALSE
		/if (${_tmpID} != ${Me.ID}) {
			/if (${Spawn[${_tmpID}].Distance} > ${Spell[${FindItem[=${${Me.Class.ShortName}Epic${_spell}}].Spell}].MyRange} && ${Spell[${FindItem[=${${Me.Class.ShortName}Epic${_spell}}].Spell}].Range} != 0) /return FALSE
		}
		/if (${Me.Buff[${FindItem[=${${Me.Class.ShortName}Epic${_spell}}].Spell}].ID}) /return FALSE
		/if (${Me.Song[${FindItem[=${${Me.Class.ShortName}Epic${_spell}}].Spell}].ID}) /return FALSE
		
		/if (${Spell[${FindItem[=${${Me.Class.ShortName}Epic${_spell}}].Spell}].SpellType.Equal[Beneficial]} && (${Target.ID} == ${Me.ID})) {
			/if (!${Spell[${FindItem[=${${Me.Class.ShortName}Epic${_spell}}].Spell}].NewStacks}) /return FALSE
		}

	| check bard shit.. gotta be difficult..
	} else /if (${_type.Equal[pulse]}) {
		/varset _spell ${Spell[${_spell}].RankName}
		/if (${Me.CurrentMana} < ${Spell[${_spell}].Mana}) /return FALSE
		/if (${Me.Buff[${_spell}].Duration.Seconds} > 6) /return FALSE
		/if (${Me.Song[${_spell}].Duration.Seconds} > 6) /return FALSE
		/if (!${Spell[${_spell}].NewStacks}) /return FALSE
	}

/return TRUE



|***
 *	DES: spell/disc/aa/item cast wrapper
 *	USE: /call core_cast2 "SPELLNAME" [alt|activedisc|disc|gem#|item|epicitem|pulse] [TargetID|0 (for castless skills)] [subname|FALSE]
 *  NOTE: MQ2Cast ["spell name"|"item name"|"AA name"|"AA#"] [[item|slotname]|alt|gem#] [give up time][s|m] [custom subroutine name] [-targetid|###] [-maxtries|#] [-recast|#] [-setin|setname] [-bandolier|setname]
 *  NOTE: the note above is full of crap... i wouldn't trust a thing it says..!
 ***|
sub core_cast2(string _spell, string _type, int _tmpID, string _sub)
	| SDEBUG \atcore_cast2\ax(spell: ${_spell}, type: ${_type}, tar: ${_tmpID}, sub: ${_sub)
	/if (${Me.Casting.ID} && !${Select[${Me.Class.ShortName},BRD]}) /return
	AMIDEAD
	| CHECKINPUTS
	
	| make sure we have our target 
	/if (${Bool[${_tmpID}]}) {
		/if (${Target.ID} != ${_tmpID}) {
			/squelch /target ID ${_tmpID}
			/delay 1s ${Target.ID} == ${_tmpID}
		}
	}
	
	| what type is it?
	/if (${_type.Equal[FIND]}) {
		/if (${Me.Book[${Spell[${_spell}].RankName}]}) {
			/varset _type ${buffGem}
		} else /if (${Me.AltAbilityReady[${_spell}]}) {	
			/varset _type alt
		} else /if (${Me.CombatAbilityReady[${_spell}]}) {
			/varset _type disc
		} else /if (${Me.ItemReady[${Spell[${_spell}].RankName}]}) {
			/varset _type item
		}
	} 	

	| spell
	/if (!${Select[${_type},alt,item,disc,pulse,epicitem,activedisc]}) {
		/declare _castLoop int local 5
		/varset castReturn CAST_NOTREADY
		/varset castBreak FALSE
		
		| check any subs? yes.. no.. maybe??
		/if (${Bool[${_sub}]}) {
			| SDEBUG /call ${_sub}
			/call ${_sub}
			/if (${castBreak}) {
				/varset castReturn CAST_BREAK
				/return CAST_BREAK
			}
		}
		
		| correct the rank
		/varset _spell ${Spell[${_spell}].RankName}

		| mem spell if needed??
		/if (${_type.Equal[${buffGem}]} && !${Me.Gem[${_spell}]}) /call mem_spell TRUE "${_spell}" ${_type} TRUE


		| cast GOM if song proc?
		/if (${_tmpID} != 0 && ${Target.ID} && ${Me.Song[Gift of Mana].ID} && !${Select[${Me.Class.ShortName},CLR]}) {
			/call cast_nuke_GoM ${_tmpID}
			/return
		}

		| is the spell memed
		/if (${Me.Gem[${_spell}]}) {

			| echos
			/if (${_tmpID} == 0) {
				VSPELL Spell ${sep} ${csp}${_spell}\ax
			} else /if (${_tmpID} != 0 && ${Target.ID}) {
				VSPELL Spell ${sep} ${csp}${Spell[${_spell}].RankName}\ax ${sep} [${ctar}${Spawn[id ${_tmpID}].DisplayName}\ax]
			}
			VBC Spell "${_spell}" ${_tmpID} 0 		

			/while (${_castLoop}) {
			
				| can we san no MQ2Cast ???
				/cast ${Me.Gem[${_spell}]}
	
				| set delays for casting. GTFO out of this as fast as possible
				/delay ${Math.Calc[${Spell[${_spell}].MyCastTime.Float}+5]}s !${Me.Casting.ID}
 				
 				| how did i do coach?
				/call check_cast_events
						
				| good cast
				/if (${Select[${castReturn},CAST_SUCCESS]}) /break
				
				| spell is not ready
				/if (${Select[${castReturn},CAST_NOTREADY]}) {
					/varcalc _castLoop ${_castLoop}-1
					/delay 2
				}

			}

		}

		| delay for longer casts if there is any, cooldowns or open spell cast windows
		/while (${Me.Casting.ID} || ${Window[CastingWindow].Open} || ${Me.SpellInCooldown}) {
			/delay 3
		}
		
		/return

	| AA
	} else /if (${Select[${_type},alt]}) {
		
		/if (${Select[${Me.Class.ShortName},BRD]}) {
			/if (${Me.BardSongPlaying}) /call pulse_stop 9
			/if (${coreAuto} && (${swCombatMelee} || ${swCombatRange})) {
				/if (!${Range.Between[0,${setFaceArc}:${Math.Abs[${Math.Calc[${Target.HeadingTo.Degrees}-${Me.Heading.Degrees}]}]}]}) /squelch /face ID ${Target2Kill} nolook ${If[${swFaceFast},fast,]}
				/call stick_to_target ${Target2Kill}
			}
		}
		
		/if (${_tmpID} == 0) VALT AA ${sep} ${csp}${_spell}\ax
		/if (${_tmpID} != 0 && ${Target.ID}) VALT AA ${sep} ${csp}${_spell}\ax ${sep} [${ctar}${Spawn[id ${_tmpID}].DisplayName}\ax]
		/alt act ${Me.AltAbility[${_spell}].ID}
		VBC Alt "${_spell}" ${_tmpID} 0 

		| delay for aa casting
		/delay ${Math.Calc[${Spell[${_spell}].MyCastTime.Float}+5]}s !${Me.Casting.ID}
		/delay 5

		/while (${Me.Casting.ID} || ${Window[CastingWindow].Open}) {
			/delay 5
		}		
		
	| item
	} else /if (${Select[${_type},item,epicitem]}) {

		/if (${Select[${Me.Class.ShortName},BRD]}) {
			/if (${Me.BardSongPlaying}) /call pulse_stop 9
			/if (${coreAuto} && (${swCombatMelee} || ${swCombatRange})) {
				/if (!${Range.Between[0,${setFaceArc}:${Math.Abs[${Math.Calc[${Target.HeadingTo.Degrees}-${Me.Heading.Degrees}]}]}]}) /squelch /face ID ${Target2Kill} nolook ${If[${swFaceFast},fast,]}
				/call stick_to_target ${Target2Kill}
			}
		}

		/if (${_tmpID} == 0) VITEM Item ${sep} ${citem}${_spell}\ax
		/if (${_tmpID} != 0 && ${Target.ID}) VITEM Item ${sep} ${citem}${_spell}\ax ${sep} [${ctar}${Spawn[id ${_tmpID}].DisplayName}\ax]	
		VBC Item "${_spell}" ${_tmpID} 0 
		/useitem "${_spell}" 
		/delay 5
		/while (${Me.Casting.ID} && ${Window[CastingWindow].Open}) {
			/delay 5
		}	
		
	| disc
	} else /if (${Select[${_type},disc,activedisc]}) {
		/varset _spell ${Spell[${_spell}].RankName}

		/if (${Select[${Me.Class.ShortName},BRD]}) {
			/if (${Me.BardSongPlaying}) /call pulse_stop 9
			/if (${coreAuto} && (${swCombatMelee} || ${swCombatRange})) {
				/if (!${Range.Between[0,${setFaceArc}:${Math.Abs[${Math.Calc[${Target.HeadingTo.Degrees}-${Me.Heading.Degrees}]}]}]}) /squelch /face ID ${Target2Kill} nolook ${If[${swFaceFast},fast,]}
				/call stick_to_target ${Target2Kill}
			}
		}

		/if (${_tmpID} == 0) VDISC Disc ${sep} ${csp}${_spell}\ax
		/if (${_tmpID} != 0 && ${Target.ID}) VDISC Disc ${sep} ${csp}${_spell}\ax ${sep} [${ctar}${Spawn[ID ${_tmpID}].DisplayName}\ax]
		VBC Disc "${_spell}" ${_tmpID} 0 
		/disc ${_spell}
		/delay 5
		/while (${Me.Casting.ID} && ${Window[CastingWindow].Open}) {
			/delay 5
		}	
			
	| bard song	
	} else /if (${Select[${_type},pulse]}) {
		/varset _spell ${Spell[${_spell}].RankName}
		
		/if (${Select[${Me.Class.ShortName},BRD]}) {
			/if (${Me.BardSongPlaying}) /call pulse_stop 9
			/if (${coreAuto} && (${swCombatMelee} || ${swCombatRange})) {
				/if (!${Range.Between[0,${setFaceArc}:${Math.Abs[${Math.Calc[${Target.HeadingTo.Degrees}-${Me.Heading.Degrees}]}]}]}) /squelch /face ID ${Target2Kill} nolook ${If[${swFaceFast},fast,]}
				/call stick_to_target ${Target2Kill}
			}
		}
	
		| stop any playing songs for the new one
		/if (${Me.BardSongPlaying}) /call pulse_stop
		/if (${_tmpID} == 0) VPULSE Pulse ${sep} ${csp}${_spell}\ax
		/if (${_tmpID} != 0 && ${Target.ID}) VPULSE Pulse ${sep} ${csp}${_spell}\ax ${sep} [${ctar}${Spawn[ID ${_tmpID}].DisplayName}\ax]
		/cast ${Me.Gem[${_spell}]}

		| adjust delay for Metronome AA (fuck bards)
		/if (${Me.AltAbility[Metronome]}) {
			/delay ${Spell[${_spell}].CastTime.Seconds}s
		} else {
			/delay ${Math.Calc[${Spell[${_spell}].CastTime.Seconds}+${Math.Calc[${Spell[${_spell}].CastTime.Seconds}*.05]}]}s
		}

		/while (${Me.Casting.ID} && ${Window[CastingWindow].Open}) {
			/delay 5
		}		
		/if (${Me.BardSongPlaying}) /call pulse_stop
	}
	
	/call check_cast_events
	| SDEBUG outside:${castReturn}

	/if (!${Select[${_type},alt,item,disc,pulse,epicitem,activedisc]}) {
		/delay 2 !${Me.SpellReady[${_spell}]}
	} else /if (${Select[${_type},item,epicitem]}) {
		/delay 2 !${Me.ItemReady[${_spell}]}
	} else /if (${Select[${_type},alt]}) {
		/delay 2 !${Me.AltAbilityReady[${_spell}]}
	} else /if (${Select[${_type},disc,activedisc]}) {
		/delay 2 !${Me.CombatAbilityReady[${_spell}]}
	} else /if (${Select[${_type},pulse]}) {
		/delay 2 !${Me.SpellReady[${_spell}]}
	}

/return FALSE



sub cast(string _spell)
	/delay 2.5s ${Me.SpellReady[${_spell}]}
	/cast ${Me.Gem[${_spell}]}
	/delay ${Math.Calc[${Spell[${_spell}].MyCastTime.Float}+2]}s ${Me.Casting.ID}
	/while (${Me.Casting.ID} || ${Window[CastingWindow].Open} || ${Me.SpellInCooldown}) {
		/delay 5
	}
/return



|***
 *	DES: removes ranks from spell names. returns the name only
 *	USE: /call spell_remove_rank [spellname]
 *	NOTE:
 ***|
sub spell_remove_rank(string _spell)
	/if (${Spell[${_spell}].Rank} == 3) {
		/varset _spell ${_spell.Left[-8]}	
	} else /if (${Spell[${_spell}].Rank} == 2) 
		/varset _spell ${_spell.Left[-7]}	
	} else /if (${Spell[${_spell}].Rank} == 1) 
		/varset _spell ${_spell}
	} else /if (${Spell[${_spell}].Rank} == 0) 
		/varset _spell ${_spell}
	}

/return ${_spell}




|***
 *	DES: check specific classes ONLY
 *	USE: /if (${validate_class[BOOL, |CLS|CLS]) /dowhatever
 *	NOTE: 
 ***|
sub validate_class(bool _debug, string _class)
	/declare _pass bool local FALSE
	/declare _count int local 0

	/for _count 1 to ${_class.Count[|]}
		/if (${Me.Class.ShortName.Upper.Equal[${_class.Arg[${_count},|]}]}) {
			/varset _pass TRUE
			/break
		}
	/next _count

	/if (${_debug}) {
		/if (!${_pass}) /call echos failboat 0 0 0 0
	}

/return ${_pass}



|***
 *	DES: is the plugin available ?
 *	USE: /if (${validate_plugin[BOOL, |plugin|plugin]) /dowhatever
 *	NOTE: cycle all plugins. return FALSE if any passed are not loaded
 ***|
sub validate_plugin(bool _error, string _plugin)
	/if (!${Bool[${_plugin}]}) VOUT ${owarning} ${sep} validate_plugin failure.
	/declare _plugcount int local 0	
	/declare _count int local 0

	/for _count 1 to ${_plugin.Count[|]}
		AMIDEAD
		/if (${Bool[${Plugin[${_plugin.Arg[${_count},|]}]}]}) {
			/varcalc _plugcount ${_plugcount}+1
		} else /if (!${Bool[${Plugin[${_plugin.Arg[${_count},|]}]}]} && ${_error}) {
			 VOUT Plugin ${cinfo}$ ${_plugin.Arg[${_count},|]}\ax is not loaded.
		}
	/next _count
	
	/if (${_plugcount} == ${_plugin.Count[|]}) {
		/return TRUE
	} else /if (${_plugcount} != ${_plugin.Count[|]}) {
		/return FALSE
	}

/return FALSE



|***
 *	DES: slows the target
 *	USE: /call cast_slow [ID] | /debuff slow
 *  NOTE: 
 ***|
sub cast_slow(int _tmpID)
	AMIDEAD
	CHECKEXIT
	CHECKINPUTS
	/if (${is_target_dead[${_tmpID}]}) /return
	
	/squelch /target ID ${_tmpID}
	/delay 1s ${Target.ID} == ${_tmpID}

	| necros only slow undead 
	/if (${validate_class[FALSE, |NEC]} && ${Target.Body.Name.NotEqual[Undead]}) /return
	
	| dont waste time if its slowed already
	/if (${Target.Slowed.Duration} && ${Target.Slowed.Name.NotEqual[Tendon Slice]}) {
		/call timer_update timer_slow_${Target2Kill} ${Target.Slowed.Duration.TotalSeconds}s
		/return
	}
	
	| check the things	
	/if (${Spawn[${_tmpID}].Type.Equal[corpse]} || !${Spawn[${_tmpID}].ID}) /return
	
	UPDATEHUD "Slow"	
	
	| if the spell is identified in the INI and memed we will use that over the AA
	/if (${Bool[${Me.Gem[${spellSlow}]}]}) {
		/delay 2.25s ${Me.SpellReady[${Spell[${spellSlow}].RankName}]}	
		/if (${validate_cast[FALSE, spell, "${spellSlow}", ${_tmpID}]}) {
			/call core_cast2 "${spellSlow}" 0 ${_tmpID} FALSE
		}

	| spell not identified, using AA.
	} else /if ((${spellSlow.Equal[NULL]} || !${Bool[${spellSlow}]} || !${Bool[${Me.Gem[${spellSlow}]}]}) && ${spellSlow.NotEqual[Time's Antithesis]}) {
		/if (${Select[${Me.Class.ShortName},ENC]}) /varset spellSlow Slowing Helix
		/if (${Select[${Me.Class.ShortName},SHM]}) /varset spellSlow Turgur's Swarm
		/if (${Select[${Me.Class.ShortName},BST]}) /varset spellSlow Sha's Reprisal
		/if (${Select[${Me.Class.ShortName},NEC]}) /varset spellSlow Death's Malaise	

		/delay 2.25s ${Me.AltAbilityReady[${spellSlow}]}	
		/if (${validate_cast[FALSE, alt, "${spellSlow}", ${_tmpID}]}) {
			/call core_cast2 "${spellSlow}" alt ${_tmpID} FALSE
		}

	| Time clickie slow
	} else /if (${spellSlow.Equal[Time's Antithesis]}) {
		/delay 6s ${Me.ItemReady[${spellSlow}]}	
		/if (${validate_cast[FALSE, item, "${spellSlow}", ${_tmpID}]}) {
			/call core_cast2 "${spellSlow}" item ${_tmpID} FALSE
		}
		
	}
	
	| set time based upn cast return	
	/if (${Select[${castReturn},CAST_SUCCESS,CAST_IMMUNE,CAST_NOHOLD,CAST_TAKEHOLD]}) {
		/call timer_update timer_slow_${_tmpID} ${Spell[${spellSlow}].Duration.TotalSeconds}s
	} else /if (${Select[${castReturn},CAST_RESISTED,CAST_RESIST]}) {
		/call timer_update timer_slow_${_tmpID} 15s
	}
	
	UPDATEHUD FALSE

/return



|***
 *	DES: casts Malo or Tash depending on the class
 *	USE:  /call cast_malo [target ID]
 *  NOTE: 
 ***|
sub cast_malo(int _tmpID)
	AMIDEAD
	CHECKEXIT
	CHECKINPUTS
	/if (${is_target_dead[${_tmpID}]}) /return
	
	/squelch /target ID ${_tmpID}
	/delay 1s ${Target.ID} == ${_tmpID}
	
	| check the things .. normal things	
	/if (${Spawn[${_tmpID}].Type.Equal[corpse]} || !${Spawn[${_tmpID}].ID}) /return

	| dont waste time if its Maloed already
	/if (${Target.Maloed.Duration}) {
		/call timer_update timer_malo_${Target2Kill} ${Target.Maloed.Duration.TotalSeconds}s
		/return
	}
	UPDATEHUD "Malo"

	| now the spell 
	/if (${Bool[${Me.Gem[${spellMalo}]}]}) {
		/delay 2.25s ${Me.SpellReady[${Spell[${spellMalo}].RankName}]}	
		/if (${validate_cast[FALSE, spell, "${spellMalo}", ${_tmpID}]}) /call core_cast2 "${spellMalo}" 0 ${_tmpID} FALSE
		
	| fall back to AA	
	} else /if (${spellMalo.Equal[NULL]} || !${Bool[${spellMalo}]} || !${Bool[${Me.Gem[${spellMalo}]}]}) {
		
		| magician and shaman
		/if (${Select[${Me.Class.ShortName},MAG,SHM]} && ${SpawnCount[npc radius ${setMobAgro} playerstate 4]} >= 2 && ${Me.AltAbilityReady[Wind of Malaise]}) {
			/varset spellMalo Wind of Malaise
		} else /if (${Select[${Me.Class.ShortName},MAG,SHM]} && (${SpawnCount[npc radius ${setMobAgro} playerstate 4]} == 1 || !${Me.AltAbilityReady[Wind of Malaise]})) {
			/varset spellMalo Malaise
		}

		| necromancer		
		/if (${Select[${Me.Class.ShortName},NEC]}) /varset spellMalo Scent of Thule

		/delay 2.25s ${Me.AltAbilityReady[${spellMalo}]}	
		/if (${validate_cast[FALSE, alt, "${spellMalo}", ${_tmpID}]}) {
			/call core_cast2 "${spellMalo}" alt ${_tmpID} FALSE
		}
	
	}
	
	| set the timers	
	/if (${Select[${castReturn},CAST_SUCCESS,CAST_IMMUNE,CAST_NOHOLD,CAST_TAKEHOLD]}) {
		/call timer_update timer_malo_${_tmpID} ${Spell[${spellMalo}].Duration.TotalSeconds}s
	} else /if (${Select[${castReturn},CAST_RESISTED,CAST_RESIST]}) {
		/call timer_update timer_malo_${_tmpID} 15s
	}
	
	UPDATEHUD FALSE	

/return



|***
 *	DES: cripples the target
 *	USE: /call cast_cripple [ID] | /debuff cripple
 *  NOTE: not for wizards.
 ***|
sub cast_cripple(int _tmpID)
	/if (${timer_cripple_${_tmpID}}) /return
	/if (${Spawn[${_tmpID}].Type.Equal[corpse]} || !${Spawn[${_tmpID}].ID}) /return

	AMIDEAD
	CHECKEXIT
	CHECKTIE
	CHECKINPUTS

	/squelch /target ID ${_tmpID}
	/delay 1s ${Target.ID} == ${_tmpID}

	| if its crippled, move on
	/if (${Target.Crippled.Duration} || ${Target.BuffDuration[${spellCripple}].TotalSeconds}) {
		/call timer_update timer_cripple_${_tmpID} ${Target.Crippled.Duration.TotalSeconds}s
		/return
	}

	UPDATEHUD "Cripple"	

	| spell first
	/if (${Bool[${Me.Gem[${spellCripple}]}]}) {
		/delay 2.25s ${Me.SpellReady[${Spell[${spellCripple}].RankName}]}	
		/if (${Select[${Me.Class.ShortName},ENC]}) {
			/if (${validate_cast[FALSE, spell, "${spellCripple}", ${_tmpID}]})  /call core_cast2 "${spellCripple}" 0 ${_tmpID} check_for_adds
		} else {
			/if (${validate_cast[FALSE, spell, "${spellCripple}", ${_tmpID}]})  /call core_cast2 "${spellCripple}" 0 ${_tmpID}
		}
		
	| fall back to alt ability	
	} else /if (${Select[${Me.Class.ShortName},ENC,WIZ,SHM]} && (!${Bool[${Me.Gem[${spellCripple}]}]} || ${spellCripple.Equal[NULL]} || !${Bool[${spellCripple}]})) {
		/if (${Select[${Me.Class.ShortName},ENC]}) /varset spellCripple Slowing Helix
		/if (${Select[${Me.Class.ShortName},SHM]}) /varset spellCripple Crippling Spirit
		/if (${Select[${Me.Class.ShortName},WIZ]}) /varset spellCripple Lower Element
		
		/if (${Select[${Me.Class.ShortName},ENC]}) {
			/if (${validate_cast[FALSE, alt, "${spellCripple}", ${_tmpID}]})  /call core_cast2 "${spellCripple}" alt ${_tmpID} check_for_adds
		} else {
			/if (${validate_cast[FALSE, alt, "${spellCripple}", ${_tmpID}]})  /call core_cast2 "${spellCripple}" alt ${_tmpID}
		}		
		
	}

	| set timers
	/if (${Select[${castReturn},CAST_SUCCESS,CAST_IMMUNE,CAST_NOHOLD,CAST_TAKEHOLD]}) {
		/call timer_update timer_cripple_${_tmpID} ${Spell[${spellCripple}].Duration.TotalSeconds}s
	} else /if (${Select[${castReturn},CAST_RESISTED,CAST_RESIST]}) {
		/call timer_update timer_cripple_${_tmpID} 15s
	}
	UPDATEHUD FALSE

/return



|***
 *	DES: sets a base line for all target2kill timers
 *	USE: /call set_base_target_timers
 *  NOTE: we use this to prevent undeclared variables since targets change .. a lot.
 ***|
sub set_base_target_timers(int _tmpID)
	CHECKINPUTS
	
	| movement stick behind
	/if (${swStickBehind}) {
		/if (!${Defined[timer_snap_recheckID_${_tmpID}]}) /call timer_update timer_snap_recheckID_${_tmpID} 1
	}
	
	| class eradicate buffs AA
	/if (${swdeBuffEradicate}) {
		/if (!${Defined[timer_eradicate_${_tmpID}]}) /call timer_update timer_eradicate_${_tmpID} 1
	}
	
	/if (${swdeBuffTash}) {
		/if (!${Defined[timer_tash_${_tmpID}]}) /call timer_update timer_tash_${_tmpID} 1
	}
	
	/if (${swdeBuffSlow}) {
		/if (!${Defined[timer_slow_${_tmpID}]}) /call timer_update timer_slow_${_tmpID} 1
	}
	
	/if (${swdeBuffCripple}) {
		/if (!${Defined[timer_cripple_${_tmpID}]}) /call timer_update timer_cripple_${_tmpID} 1

		/if (${Me.Class.ShortName.Equal[DRU]}) {
			/if (!${Defined[timer_ro_${_tmpID}]}) /call timer_update timer_ro_${_tmpID} 1
		}		
	}
	
	/if (${swdeBuffMezz}) {
		/if (!${Defined[timer_mez_${_tmpID}]}) /call timer_update timer_mez_${_tmpID} 1
	}
	
	/if (${swdeBuffSnare}) {
		/if (!${Defined[timer_snare_${_tmpID}]}) /call timer_update timer_snare_${_tmpID} 1
	}
	/if (${swdeBuffMalo}) {
		/if (!${Defined[timer_malo_${_tmpID}]}) /call timer_update timer_malo_${_tmpID} 1
	}
	

	

/return 
 
 
 
|***
 *	DES: cycles all debuff routines.
 *	USE: check_deBuffs
 *  NOTE: 
 ***|
sub check_deBuffs(int _tmpID)
	AMIDEAD
	CHECKEXIT
	CHECKTIE
	CHECKINPUTS
	
	/call set_base_target_timers ${Target2Kill}

	| /if (${Target.ID} != ${_tmpID}) /squelch /target ID ${_tmpID}
	
	| tash
	/if (${swdeBuffTash}) {
	 	/if (!${Bool[${timer_tash_${_tmpID}}]}) {
			/if (${Select[${Me.Class.ShortName},ENC]}) { 
				/call cast_tash ${_tmpID}
			} else {
			}
		}
	}
	
	| malo
	/if (${swdeBuffMalo}) {
		/if (!${Bool[${timer_malo_${_tmpID}}]}) {
			/call cast_malo ${_tmpID}
		}
	}
	
	| slow. spell if spellSlow defined otherwise AA
	/if (${swdeBuffSlow}) {
		/if (!${Bool[${timer_slow_${_tmpID}}]} && !${validate_class[FALSE, |BRD]}) {
			/if (${Select[${Me.Class.ShortName},SHM]}) {
				/if (${SpawnCount[npc radius ${setMobAgro} playerstate 4]} > 2) {
					/if (${validate_cast[FALSE, spell, "${spellAESlow}", ${_tmpID}]}) /call core_cast2 "${spellAESlow}" 0 ${_tmpID} FALSE
				} else {
					/call cast_slow ${_tmpID}
				}
			} else {			
				/call cast_slow ${_tmpID}
			}
			/if (${Select[${Me.Class.ShortName},ENC]}) { 
				/call check_for_adds
				/if (${addList.Count[|]}) /return
			}
		}
	}
		
	| cripple
	/if (${swdeBuffCripple}) {
		
		/if (${Select[${Me.Class.ShortName},DRU]}) {
			/call cast_AA_ro ${_tmpID}	
		}	else /if (${Select[${Me.Class.ShortName},ENC]}) { 
			/if (!${Bool[${timer_cripple_${_tmpID}}]}) {		
				/if (${Spell[${spellCripple}].ID} != ${Spell[${spellSlow}].ID}) /call cast_cripple ${_tmpID}
				/call check_for_adds
				/if (${addList.Count[|]}) /return
			}
		} else {
			/call cast_cripple ${_tmpID}
		}
	}
	
	| eradicate magic AA line
	/if (${swdeBuffEradicate} && ${validate_class[FALSE, ${lstClassAAEradicate}]}) {
		/if (!${Bool[${timer_eradicate_${_tmpID}}]}) {
			/call cast_AA_eradicate_magic ${_tmpID}
			/if (${Select[${Me.Class.ShortName},ENC]}) { 
				/call check_for_adds
				/if (${addList.Count[|]}) /return
			}
		}
	}
	
	| aa snare
	/if (${swdeBuffSnare} && ${validate_class[FALSE, ${lstClassAASnare}]}) {
		/if (!${Bool[${timer_snare_${_tmpID}}]}) {
			/call cast_AA_snare ${_tmpID}
		}
	}

/return



|***
 *	DES: swaps the bandolier if needed
 *	USE: /call bandolier_swap BANDTYPE
 *  NOTE: 
 ***|
sub bandolier_swap(string _band)
	/if (${bandolierSwap.Equal[${bandolier${_band}}]}) /return TRUE
	/if (!${swBandolier}) /return FALSE
	/if (!${Bool[${bandolierNormal}]}) /return FALSE
	/if (!${Bool[${bandolier${_band}}]}) /return FALSE
	/delay 1
	/bandolier Activate ${bandolier${_band}}
	/varset bandolierSwap ${bandolier${_band}}

/return TRUE



|***
 *	DES: check to see that you have a specific expansion
 *	USE: /if (!${validate_expansion[TRUE/FALSE, |#|#|#]}) /return
 *  NOTE: check for any FALSE returns. if even one, bail as bad.
 *  
 *  1	The Ruins of Kunark												2	The Scars of Velious
 *  3	The Shadows of Luclin											4	The Planes of Power
 *  5	The Legacy of Ykesha											6	Lost Dungeons of Norrath
 *  7	Gates of Discord													8	Omens of War
 *  9	Dragons of Norrath												10	Depths of Darkhollow
 *  11 Prophecy of Ro														12 The Serpent's Spine
 *  13 The Buried Sea														14 Secrets of Faydwer
 *  15 Seeds of Destruction											16 Underfoot
 *  17 House of Thule														18 Veil of Alaris
 *  19 Rain of Fear															20 Call of the Forsaken
 *  21 The Darkened Sea													22 The Broken Mirror
 *  23 Empires of Kunark												24 Ring of Scale		 
 *  25 The Burning Lands
 *   
 ***|
sub validate_expansion(bool _error, string _exp)
	/if (!${swXpacControl} || !${swTLP}) /return TRUE
	/declare _pass bool local TRUE
	/declare _count int local 0

	/for _count 1 to ${_exp.Count[|]}
		AMIDEAD
		/if (!${Me.HaveExpansion[${_exp.Arg[${_count},|]}]}) {
			OUT ${onotice} ${sep} Expansion ${cinfo}${sql_return[xpac${_exp.Arg[${_count},|]}]}\ax required for this feature.
			/varset _pass FALSE
			/break
		}
	/next _count
/return ${_pass}



|***
 *	DES: check and adjust the debuff switches
 *	USE: /call validate_debufs [T|F echo error] [debuff switch|NULL]
 *  NOTE: 
 ***|
sub validate_debuff(bool _error, string _deBuffType)
	| check mezz switch
	/if (${Bool[${_deBuffType.Equal[mezz]}]} || ${swdeBuffMezz}) {
		/if (!${validate_class[${_error}, |ENC|BRD|MAG|NEC]}) {
			/call sql_update FALSE ${chrID} swdeBuffMezz FALSE
			/return FALSE
		}
	}
	
	| check malo switch
	/if (${Bool[${_deBuffType.Equal[malo]}]} || ${swdeBuffMalo}) {
		/if (!${validate_class[${_error}, |MAG|SHM|NEC]}) {
			/call sql_update FALSE ${chrID} swdeBuffMalo FALSE
			/return FALSE
		}
	}
	
	| check tash switch
	/if (${Bool[${_deBuffType.Equal[tash]}]} || ${swdeBuffTash}) {
		/if (!${validate_class[${_error}, |ENC]}) {
			/call sql_update FALSE ${chrID} swdeBuffTash FALSE
			/return FALSE
		}
	}
	
	| check cripple switch
	/if (${Bool[${_deBuffType.Equal[cripple]}]} || ${swdeBuffCripple}) {
		/if (!${validate_class[${_error}, |ENC|SHM|DRU|MAG|NEC|SHD|BST|WIZ]}) {
			/call sql_update FALSE ${chrID} swdeBuffCripple FALSE
			/return FALSE
		}
	}
	
	| check slow swich
	/if (${Bool[${_deBuffType.Equal[slow]}]} || ${swdeBuffSlow}) {
		/if (!${validate_class[${_error}, |ENC|SHM|BRD|NEC|BST]}) {
			/call sql_update FALSE ${chrID} swdeBuffSlow FALSE
			/return FALSE
		}
	}
	
	| check eradicate switch
	/if (${Bool[${_deBuffType.Equal[eradicate]}]} || ${swdeBuffEradicate}) {
		/if (!${validate_class[${_error}, ${lstClassAAEradicate}]}) {
			/call sql_update FALSE ${chrID} swdeBuffEradicate FALSE
			/return FALSE
		}
	}
	
	| check snare switch
	/if (${Bool[${_deBuffType.Equal[snare]}]} || ${swdeBuffSnare}) {
		/if (!${validate_class[${_error}, ${lstClassAASnare}]}) {
			/call sql_update FALSE ${chrID} swdeBuffSnare FALSE
			/return FALSE
		}
	}

/return TRUE



|***
 *	DES: watches to see if you are deaded. if so.. theres a couple things we need to do.
 *	USE: /if (${watch_for_death[]}) /do a thing
 *  NOTE: returns swAmIDead TRUE if you died
 ***|
sub watch_for_death()
	/if (${Me.Zoning}) /return FALSE
	CHECKINPUTS

	| check for current flags. watch for rez box if I have them
	/if (${swAmIDead}) /return TRUE
	/if (${swNeedRez}) /return TRUE

	| check death states
	/if (${Me.State.Equal[HOVER]} || ${Me.Hovering} || ${Me.Dead}) {
		/if (${Me.Casting.ID}) /stopcast
		/varset swAmIDead TRUE
	
	| did I just die and get sent to bind ?
	} else /if ((${Me.Buff[Resurrection Sickness].ID} || ${Me.Buff[Revival Sickness].ID}) && ${Zone.ID} == ${Me.ZoneBound.ID}) {
		/varset swAmIDead TRUE
	}

	| if we died, shut shit down
	/if (${swAmIDead}) {
		/doevents flush
		/delay 2s

		| stop all the things(s)
		/varset swHomeSet FALSE
		/call set_stop NOECHO
		/call clear_combat
		/if (${Me.Casting.ID}) /stopcast
		/varset Target2Kill 0
		/if (${coreAuto}) ECHOCHANNEL Need a rez!
		| /return TRUE
	}

/return ${swAmIDead}
	


|***
 *	DES: watches for the rez box if we have bben marked as dead or called.
 *	USE: /call  watch_for_rezbox()
 *  NOTE: 
 ***|
sub watch_for_rezbox()
	/if (${Me.Zoning}) /return FALSE
	CHECKINPUTS
	
	| do I have corpses around ?
	/if (!${SpawnCount[pccorpse name ${Me.DisplayName}]}) {

		| dont rezz me
		/if (!${swRezTake}) /return
		
		| not dead? GO AWAY
		/if (!${swAmIDead} && !${swNeedRez}) /return
			
		| bail if no rez box to check
		/if (!${Bool[${Window[ConfirmationDialogBox].Child[CD_TextOutput].Text.Find["Do you wish this?"]}]}) /return
	}

	| is there a rez window?
	/if (${Window[ConfirmationDialogBox].Open}) {

	| /if (${Bool[${Window[ConfirmationDialogBox].Child[CD_TextOutput].Text.Find["Do you wish this?"]}]}) {

		| get rez window % to something we can use			
		/if (${Bool[${Window[ConfirmationDialogBox].Child[CD_TextOutput].Text.Find["percent)"]}]}) {
			/if (${Bool[${Window[ConfirmationDialogBox].Child[CD_TextOutput].Text.Find["(10 percent)"]}]}) { 
				/varset setRezWindowPct 10
			} else /if (${Bool[${Window[ConfirmationDialogBox].Child[CD_TextOutput].Text.Find["(20 percent)"]}]}) {
				/varset setRezWindowPct 20
			} else /if (${Bool[${Window[ConfirmationDialogBox].Child[CD_TextOutput].Text.Find["(35 percent)"]}]}) { 
				/varset setRezWindowPct 35
			} else /if (${Bool[${Window[ConfirmationDialogBox].Child[CD_TextOutput].Text.Find["(50 percent)"]}]}) { 
				/varset setRezWindowPct 50
			} else /if (${Bool[${Window[ConfirmationDialogBox].Child[CD_TextOutput].Text.Find["(60 percent)"]}]}) { 
				/varset setRezWindowPct 60
			} else /if (${Bool[${Window[ConfirmationDialogBox].Child[CD_TextOutput].Text.Find["(75 percent)"]}]}) { 
				/varset setRezWindowPct 75
			} else /if (${Bool[${Window[ConfirmationDialogBox].Child[CD_TextOutput].Text.Find["(90 percent)"]}]}) { 
				/varset setRezWindowPct 90
			} else /if (${Bool[${Window[ConfirmationDialogBox].Child[CD_TextOutput].Text.Find["(96 percent)"]}]}) { 
				/varset setRezWindowPct 96
			}
		}
			
		| we do not accept any 0% rezes
		/if (${Bool[${Window[ConfirmationDialogBox].Child[CD_TextOutput].Text.Find["(0 percent)"]}]}) {
			/varset swValidRezFound FALSE
		| 10% rez and higher
		} else /if (${setMinRezPct} == 10 && ${Select[${setRezWindowPct},10,20,35,50,60,75,90,93,96]}) {
			/varset swValidRezFound TRUE
		| 20% rez and higher
		} else /if (${setMinRezPct} == 20 && ${Select[${setRezWindowPct},20,35,50,60,75,90,93,96]}) {
			/varset swValidRezFound TRUE
		| 45% rez and higher
		} else /if (${setMinRezPct} == 35 && ${Select[${setRezWindowPct},35,50,60,75,90,93,96]}) {
			/varset swValidRezFound TRUE
		| 50% rez and higher
		} else /if (${setMinRezPct} == 50 && ${Select[${setRezWindowPct},50,60,75,90,93,96]}) {
			/varset swValidRezFound TRUE
		| 60% rez and higher
		} else /if (${setMinRezPct} == 60 && ${Select[${setRezWindowPct},60,75,90,93,96]}) {
			/varset swValidRezFound TRUE
		| 75% rez and higher
		} else /if (${setMinRezPct} == 75 && ${Select[${setRezWindowPct},75,90,93,96]}) {
			/varset swValidRezFound TRUE
		| 90% rez and higher
		} else /if (${setMinRezPct} == 90 && ${Select[${setRezWindowPct},90,93,96]}) {
			/varset swValidRezFound TRUE
		| 93% rez
		} else /if (${setMinRezPct} == 93 && ${Select[${setRezWindowPct},93,96]}) {
			/varset swValidRezFound TRUE
		| 96% rez
		} else /if (${setMinRezPct} == 96 && ${Select[${setRezWindowPct},96]}) {
			/varset swValidRezFound TRUE
		| 100% rez.. we take these	
		} else /if (${Bool[${Window[ConfirmationDialogBox].Child[CD_TextOutput].Text.Find["(100 percent)"]}]}) {
			/varset swValidRezFound TRUE
		| we take call to corpse as well
		} else /if (${Bool[${Window[ConfirmationDialogBox].Child[CD_TextOutput].Text.Find["you will still be able to get a resurrection later"]}]}) {
			/varset swNeedRez TRUE
			/varset swValidRezFound TRUE
		} 
			
		| more waiting
		/if (${swValidRezFound}) {
			/nomodkey /notify ConfirmationDialogBox Yes_Button leftmouseup
			/delay 5
			/if (${Window[RespawnWnd].Open}) {
				/if (${Zone.ShortName.Equal[Powar]}) {
					/nomodkey /notify RespawnWnd RW_OptionsList listselect 1
				} else {
					/nomodkey /notify RespawnWnd RW_OptionsList listselect 2 
				}
				/delay 1s
				/nomodkey /notify RespawnWnd RW_SelectButton leftmouseup
				
			}

			| Clear ALL THE MARKERS!
			/delay 2s !${Me.State.Equal[HOVER]}
			/delay 2s !${Me.Hovering}
			/delay 2s !${Me.Dead}
			/varset Target2Kill 0
			/call set_stop NOECHO
			/varset swAmIDead FALSE
			/varset swValidRezFound FALSE
			/varset setRezWindowPct 0
			/call set_switch ${chrID} swHomeSet FALSE
			/return TRUE			
		} else /if (!${swValidRezFound}) {
			/nomodkey /notify ConfirmationDialogBox No_Button leftmouseup	
			/delay 5
			/return FALSE
		} 
	}

/return



|***
 *	DES: this is for waisting AAs. it does nothing more. if you expect it to do more, go away.
 *	USE: /call cast_AA_fireworks()
 *  NOTE: 
 ***|
sub cast_AA_fireworks()
	/if (!${swAAFireworks} || ${Raid.Members}) /return
	/call timer_update timer_Waste_Fireworks ${tWaste_Fireworks}
	/if (${Me.AAPoints} < 20) /return

	| show me the money
	/if (${Me.AAPoints} >= 120 && !${Me.AltAbilityReady[Glyph of Fireworks II]}) {
		| /call buy_AA 897
		/alt buy 897
		/call timer_update timer_Waste_Fireworks 1m
		/return
	} else /if (${Me.AltAbilityReady[Glyph of Fireworks II]}) {
		VOUT 'yo .. hold my beer ..
		/if (${validate_cast[FALSE, alt, "Glyph of Fireworks II", ${Me.ID}]}) /call core_cast2 "Glyph of Fireworks II" alt 0 FALSE
	}

/return



|***
 *	DES: click self healing items
 *	USE: /call click_item_heal
 *  NOTE: 
 ***|
sub click_item_heal()
	CHECKEXIT
	AMIDEAD
	/declare _count int local 0
	
	/for _count 1 to ${clickHealItem.Count[|]}
		/if (${Me.PctHPs} >= ${${Me.Class.ShortName}Point}*.${tmpHealOverride}) /break
		/if (!${FindItem[${clickHealItem.Arg[${_count},|]}].ID}) /continue
		/if (!${Me.ItemReady[${clickHealItem.Arg[${_count},|]}]}) /continue
		
		/if (${validate_cast[FALSE, item, "${clickHealItem.Arg[${_count},|]}", ${Me.ID}]}) /call core_cast2 "${clickHealItem.Arg[${_count},|]}" item 0 FALSE
	/next _count		

/return



|***
 *	DES: make sure you have the correct zone INI info loaded
 *	USE: /call validate_zone True|False True|False
 *  NOTE: 
 ***|
sub validate_zone(bool _error, bool _force)
	/if (${timer_validate_zone} && !${_force}) /return
	/call timer_update timer_validate_zone ${tValidate_Zone}
	/declare _zoned bool local FALSE

	| zone matches. nothing to see here. move on
	/if (${currentZoneID} == ${Zone.ID} && !${_force}) /return TRUE
		
	| zone does not match. wtf..
	/if (${currentZoneID} != ${Zone.ID} || ${_force}) {
		/call echos zone 0 0 0 0
		/call control_zone FALSE
		/varset swAmIDead FALSE
		/varset _zoned TRUE
		/call timer_update timer_zone_pause ${tZone_Pause}
		/doevents flush
	
		| check for mesh
		/if (!${Navigation.MeshLoaded}) VOUT ${owarning} ${sep} NO Mesh Loaded

		| make it right with the world
		/vardata currentZoneID Zone.ID
		
	}
	| yah. we zoned and stuff
	/if (${_zoned}) {
		/if (${swBurnForce}) /varset swBurnForce FALSE
		/squelch /makecamp off
		/if (${swHomeSet}) /call set_switch ${chrID} swHomeSet FALSE
		/if (!${swModePersistent}) /call switch_clear_mode
		/if (${coreAuto} && ${switchShrink}) /call check_shrink
	}

/return



|***
 *	DES: Buys an AA
 *	USE: /call buy_AA "AA NAME"
 *  NOTE: 
 ***|
sub buy_AA(string _name)
	/if (!${Bool[${_name}]}) /return

	/if (${AltAbility[${_name}].CanTrain}) {
		/if (${Me.AltAbility[${_name}].CanTrain}) {
			/alt buy ${AltAbility[${_name}].NextIndex}
		} else {
			/alt buy ${AltAbility[${_name}].Index}
		}
	}

/return



|***
 *	DES: asssit controlls
 *	USE: /call validate_assist [ECHO] [x|g|1|2|3|f|MA1|MA2] [NAME]
 *  NOTE: 
 ***|
sub validate_assist(string _echo, string _assist, string _name)

	/declare _assistMode string local |g|1|2|3|MA1|MA2|f
	/declare _count int local 0
	/declare _listout string local
	/declare _sep bool local FALSE


	| disable all assists
	/if (${_assist.Equal[x]}) {
		OUT Setting Assissts ${sep} ${ooff}
 		/call sql_update FALSE ${chrID} swAssist1 FALSE
 		/call sql_update FALSE ${chrID} swAssist2 FALSE
 		/call sql_update FALSE ${chrID} swAssist3 FALSE
		/call sql_update FALSE ${chrID} swAssistG FALSE
		/call sql_update FALSE ${chrID} swAssistF FALSE
		/xtarget set 1 Autohater

	| group assist
	} else /if (${_assist.Equal[g]}) {
 		/call sql_update FALSE ${chrID} swAssist1 FALSE
 		/call sql_update FALSE ${chrID} swAssist2 FALSE
 		/call sql_update FALSE ${chrID} swAssist3 FALSE
		/call set_switch ${chrID} swAssistG ${_name}
		/call sql_update FALSE ${chrID} swAssistF FALSE
		/if (${swAssistG}) {
			/xtarget set 1 groupassisttarget	
		} else {
			/xtarget set 1 Autohater
		}
		/if (${swAssistG} && ${swPull}) /call set_switch ${chrID} swPull FALSE

	| raid assist 1
	} else /if (${_assist.Equal[1]}) {
 		/call set_switch ${chrID} swAssist1 ${_name}
 		/call sql_update FALSE ${chrID} swAssist2 FALSE
 		/call sql_update FALSE ${chrID} swAssist3 FALSE
		/call sql_update FALSE ${chrID} swAssistG FALSE
		/call sql_update FALSE ${chrID} swAssistF FALSE
		/if (${swAssist1}) {
			/xtarget set 1 raidassist1target
		} else {
			/xtarget set 1 Autohater
		}
		/if (${swAssist1} && ${swPull}) /call set_switch ${chrID} swPull FALSE

	| raid assist 2
	} else /if (${_assist.Equal[2]}) {
 		/call sql_update FALSE ${chrID} swAssist1 FALSE
 		/call set_switch ${chrID} swAssist2 ${_name}
 		/call sql_update FALSE ${chrID} swAssist3 FALSE
		/call sql_update FALSE ${chrID} swAssistG FALSE
		/call sql_update FALSE ${chrID} swAssistF FALSE
		/if (${swAssist2}) {
			/xtarget set 1 raidassist2target
		} else {
			/xtarget set 1 Autohater
		}
		/if (${swAssist2} && ${swPull}) /call set_switch ${chrID} swPull FALSE

	| raid assist 3
	} else /if (${_assist.Equal[3]}) {
 		/call sql_update FALSE ${chrID} swAssist1 FALSE
 		/call sql_update FALSE ${chrID} swAssist2 FALSE
 		/call set_switch ${chrID} swAssist3 ${_name}
		/call sql_update FALSE ${chrID} swAssistG FALSE
		/call sql_update FALSE ${chrID} swAssistF FALSE
		/if (${swAssist3}) {
			/xtarget set 1 raidassist3target
		} else {
			/xtarget set 1 Autohater
		}
		/if (${swAssist3} && ${swPull}) /call set_switch ${chrID} swPull FALSE

	| set assist to forced from driver
	} else /if (${_assist.Equal[f]}) {
		/call sql_update FALSE ${chrID} setMA1 FALSE
		/call sql_update FALSE ${chrID} setMA2 FALSE
 		/call sql_update FALSE ${chrID} swAssist1 FALSE
 		/call sql_update FALSE ${chrID} swAssist2 FALSE
 		/call sql_update FALSE ${chrID} swAssist3 FALSE
		/call sql_update FALSE ${chrID} swAssistG FALSE
		/call set_switch ${chrID} swAssistF ${_name}
		/xtarget set 1 Autohater
		/if (${swAssistF} && ${swPull}) /call set_switch ${chrID} swPull FALSE

	| set assist to setMA1
	} else /if (${_assist.Equal[MA1]}) {
		/if (${Bool[${_name}]}) {
			/call sql_update FALSE ${chrID} setMA1 ${_name.Left[1].Upper}${_name.Right[-1].Lower}
		} else /if (!${Bool[${_name}]})  {
			/call sql_update FALSE ${chrID} setMA1 FALSE
		} 				
		/xtarget set 1 Autohater
		/if (${setMA1} && ${swPull}) /call set_switch ${chrID} swPull FALSE
		
	| set assist to setMA2
	} else /if (${_assist.Equal[MA2]}) {
		/if (${Bool[${_name}]}) {
			/call sql_update FALSE writeclass setMA2 ${_name.Left[1].Upper}${_name.Right[-1].Lower}
		} else /if (!${Bool[${_name}]})  {
			/call sql_update FALSE ${chrID} setMA2 FALSE
		} 				
		/xtarget set 1 Autohater
		/if (${setMA2} && ${swPull}) /call set_switch ${chrID} swPull FALSE
	} 

	/if (${_echo.Equal[NOECHO]}) /return
	/if (${swMQClear}) /mqclear
	/for _count 1 to ${_assistMode.Count[|]}
		/varset _listout ${_listout} ${If[${_sep},${sep},]} ${cinfo}${_assistMode.Arg[${_count},|]}\ax

		/if (${_assistMode.Arg[${_count},|].Equal[g]}) /varset _listout ${_listout}:${If[${swAssistG},${oon}\ax,${ooff}]}
		/if (${_assistMode.Arg[${_count},|].Equal[1]}) /varset _listout ${_listout}:${If[${swAssist1},${oon}\ax,${ooff}]}
		/if (${_assistMode.Arg[${_count},|].Equal[2]}) /varset _listout ${_listout}:${If[${swAssist2},${oon}\ax,${ooff}]}
		/if (${_assistMode.Arg[${_count},|].Equal[3]}) /varset _listout ${_listout}:${If[${swAssist3},${oon}\ax,${ooff}]}
		/if (${_assistMode.Arg[${_count},|].Equal[MA1]}) /varset _listout ${_listout}:${If[${Bool[${setMA1}]},${cnum}${setMA1}\ax,${ooff}]}
		/if (${_assistMode.Arg[${_count},|].Equal[MA2]}) /varset _listout ${_listout}:${If[${Bool[${setMA2}]},${cnum}${setMA2}\ax,${ooff}]}
		/varset _sep TRUE
	/next _count
	VOUT /setassist [${_listout}]

/return



|***
 *	DES: cast or trigger alliance on target
 *	USE: /call trigger_alliance TARGETID
 *  NOTE:
 ***|
sub trigger_alliance(int _tmpID)
	AMIDEAD
	CHECKINPUTS
	
	| early GTFO if not ready
	/if (!${Me.CombatAbilityReady[${Spell[${${Me.Class.ShortName}Alliance}].RankName}]} && ${Me.CombatAbility[${Spell[${${Me.Class.ShortName}Alliance}].RankName}]}) /return
	/if (!${Me.SpellReady[${Spell[${${Me.Class.ShortName}Alliance}].RankName}]} && ${Me.Book[${Spell[${${Me.Class.ShortName}Alliance}].RankName}]}) /return

	| correct healer targets to the assists ID, not the mobs..	
	/if (${Select[${Me.Class.ShortName},CLR,SHM,DRU]}) {
		/if (${Raid.Members}) {
			/varset _tmpID ${Spawn[pc ${Raid.MainAssist}].ID}
		} else /if (!${Raid.Members} && ${Group}) {
			/varset _tmpID ${Spawn[pc ${Group.MainAssist}].ID}			
		} else /if (${Bool[${setMA1}]} && ${Bool[${Spawn[pc ${setMA1}].ID}]} && !${Bool[${Spawn[pc ${setMA1}].Dead}]}) {
			/varset _tmpID ${Spawn[pc ${setMA1}].ID}
		} else /if (${Bool[${setMA2}]} && ${Bool[${Spawn[pc ${setMA2}].ID}]} && !${Bool[${Spawn[pc ${setMA2}].Dead}]}) {
			/varset _tmpID ${Spawn[pc ${setMA2}].ID}
		}
	}

	| is the spawn still alive?
	/if (${is_target_dead[${_tmpID}]}) /return FALSE
	
	| adjust our current target
	/target ID ${_tmpID}
	/delay 5 ${Target.ID} == ${_tmpID}
	
	| are we there yet?
	/delay 5 ${Target.BuffsPopulated}
	
	| have the alliance on it already?
	/if (${Target.BuffsPopulated}) {
		/if (${Target.Buff[${${Me.Class.ShortName}Alliance}].ID}) /return FALSE	
		/if (${Bool[${Target.Buff[${${Me.Class.ShortName}Alliance}]}]}) /return FALSE	
		/if ((${Spawn[${Target.Buff[${${Me.Class.ShortName}Alliance}].Caster}].ID} == ${Me.ID})) /return FALSE
	} else /if (!${Target.BuffsPopulated}) {
		/return FALSE	
	}
	
	| alliance time
	/if (!${Target.BuffDuration[${${Me.Class.ShortName}Alliance}].TotalSeconds}) {
		/if (${validate_cast[FALSE, FIND, "${Spell[${${Me.Class.ShortName}Alliance}].RankName}", ${_tmpID}]}) /call core_cast2 "${Spell[${${Me.Class.ShortName}Alliance}].RankName}" FIND ${_tmpID} FALSE
		/return
	}

	| if theres no forced casted trigger, gtfo
	/if (!${Bool[${${Me.Class.ShortName}AllianceTrigger}]}) /return

	/declare _count int local 0

	| is one of the trigger spells ready?
	/for _count 1 to ${setAllianceTrigger.Count[|]}
		/if (${Me.SpellReady[${Spell[${setAllianceTrigger.Arg[${_count},|]}].RankName}]}) {
			/if (${validate_cast[FALSE, FIND, "${Spell[${setAllianceTrigger.Arg[${_count},|]}].RankName}", ${_tmpID}]}) /call core_cast2 "${Spell[${setAllianceTrigger.Arg[${_count},|]}].RankName}" FIND ${_tmpID} FALSE
			/continue
		}
	/next _count

/return



|***
 *	DES: sets an outer variable
 *	USE: /call setDeclare [variable Name] [bool|int|string|float|soon] [local|outer|global] [value]
 *  NOTE: 
 ***|
sub setDeclare(string _name, string _type, string _scope, string _value)
	| define the variable if it does not exist
	/if (!${Defined[${_name}]}) /declare ${_name} ${_type} ${_scope}

	| populate the variable
	/varset ${_name} ${_value}

/return



|***
 *	DES: creates timers
 *	USE: /call timer_set [NAME] [INT|BOOL|STRING|FLOAT] [INISECTION] [value] [timer_check_blah_blah]
 *  NOTE: 
 ***|
sub timer_set(bool _debug, string _name, string _type, string _scope, string _value, string _my_timer)

	| is timer in the database?
	/call sql_control ${_debug} ${chrID} ${_name} ${_type} ${_scope} ${_value}

	/if (${Bool[${_my_timer}]}) {
		/call timer_update ${_my_timer} 1
	}

/return



|***
 *	DES: active AA Shield Flash when HP reach a specific %
 *	USE: /if (${useShieldFlash}) /call cast_AA_shield_flash
 *  NOTE: 
 ***|
sub cast_AA_shield_flash()
	SIMPLEEXIT
	/if (!${coreAuto}) /return
	/if (${Me.PctHPs}*.${tmpHealOverride} > ${useShieldFlashPct}) /return
	/if (${validate_cast[FALSE, alt, "Shield Flash", ${Me.ID}]}) /call core_cast2 "Shield Flash" alt 0 FALSE

/return



|***
 *	DES: checks senders anme or guild status to the lstSafeNames ins core.ini
 *	USE: ${validate_lstSafeNames[${_sender}]}
 *  NOTE: we assume no one is trustable
 ***|
sub validate_lstSafeNames(string _sender)
	| override and accept everyone
	/if (${swOverrideSafeNames}) /return TRUE

	| basic safe list check
	/if (${lstSafeNames.Find[${_sender}]}) /return TRUE

	| guild safe list check
	/if (${swSafelistGuild} && ${Bool[${Me.Guild}]}) {
		/if (${Spawn[PC ${_sender}].Guild.Equal[${Me.Guild}]}) /return TRUE
	}

/return FALSE



|***
 *	DES: Make sure that what ever we are targeting is something we want to kill.
 *	USE: /if (${validate_target[FALSE, target ID]}
 *  NOTE: in manual mode. anything is a valid target.
**|
sub validate_target(bool _error, int _tmpID)
	/if (!${coreAuto}) /return TRUE
	AMIDEAD
	CHECKINPUTS
		
	/if (!${Target.ID}) /return FALSE
	/if (!${Bool[${_tmpID}]}) /return FALSE
	/if (${SafeZone} && !${swOverrideSafeZone}) /return FALSE

	| check smart assists
	/if (!${swAssistF}) {
		
		/if (${Target.ID}) {
			/if ((${swAssist1} || ${swAssist2} || ${swAssist3}) && ${Spawn[${_tmpID}].ID} != ${Me.XTarget[1].ID}) {
				/call clear_combat
				/return FALSE
			} else /if (${swAssistG} && ${Group.MainAssist.ID} && !${Bool[${Spawn[${Group.MainAssist}].Dead}]} && ${Spawn[${_tmpID}].ID} != ${Me.XTarget[1].ID}) {
				/call clear_combat
				/return FALSE
			}
		}
		
		/if (!${swOverrideEngagePct} && !${swPull}) {
			/if (${Spawn[${_tmpID}].PctHPs} > ${setEngageMAPct} && !${swAssist1} && !${swAssist2} && !${swAssist3} && !${swAssistG}) {
				/delay 1
				/return FALSE
			} else /if (${Spawn[${_tmpID}].PctHPs} > ${setEngagePct} && (${swAssist1} || ${swAssist2} || ${swAssist3}|| ${swAssistG})) {
				/delay 1
				/return FALSE
			}
		}
	}
	
	| shit we really shouldnt't be attacking
	/if (${Select[${Spawn[${_tmpID}].Type},AURA,BANNER,CAMPFIRE,CORPSE,CHEST,ITEM,TRIGGER,TRAP,TIMER,MOUNT,Mercenary]}) /return FALSE
	/if (${ExcludeList.Find[${Spawn[${_tmpID}].DisplayName}]}) /return FALSE
	/if (${Target.ID} && ${Bool[${Target.Mezzed.Duration}]} && !${swPull}) /return FALSE
	/if ((!${Spawn[${_tmpID}].LineOfSight} && !${swOverrideLOS}) || ${Spawn[${_tmpID}].Distance} > ${setMobAgro}) /return FALSE
	/if (${Spawn[${_tmpID}].Type.Equal[PC]}) /return FALSE
	/if (${Spawn[${_tmpID}].Master.Type.Equal[PC]}) /return FALSE
	/if (${Target.Type.Equal[PC]}) /varset isValidTarget FALSE
	/if (${_tmpID} == ${Me.ID} || ${_tmpID} == ${Me.Pet.ID}) /return FALSE

	/if (${Select[${Me.Class.ShortName},ROG]} && ${swSoS}) /makemevisible

/return TRUE



|***
 *	DES: zephyr/TL requesting toon
 *	USE: this is built by the /port bind request
 *  NOTE: 
 ***|
#bind taxi_service /taxi_service
sub Bind_taxi_service(string _sender, string _location)

	| pick a reason to not do something.. whatever you like
	/if (!${Select[${Me.Class.ShortName},DRU,WIZ]}) /return
	/if (!${Defined[${_location}]}) {
		/t ${_sender} ${_location} is not a place. Please quit bothering me.
		/return
	}
	CHECKSAFELIST
	/if (${Spawn[pc ${_sender}].Distance} > 100) /return
	
	| send 'em on their way with a valid location request
	/call mem_spell TRUE "${${_location}}" ${buffGem} TRUE
	/t ${_sender} Please enjoy your flight. ${${_location}} on its way.
	/if (${validate_cast[FALSE, spell, "${${_location}}", ${Spawn[pc ${_sender}].ID}]}) /call core_cast2 "${${_location}}" ${buffGem} ${Spawn[pc ${_sender}].ID}]} FALSE

/return



|***
 *	DES: returns a valid resident toon name if there is one
 *	USE: ${get_resident[|mage|wizard|druid|cleric, 100]}
 *  NOTE: 
 ***|
sub get_resident(string _class, int _distance)
	
	/varset _class ${_class.Left[1].Upper}${_class.Right[-1].Lower}
	/declare _name string local FALSE
	/declare _count int local 0
	
	/for _count 1 to ${lstResident${_class}.Count[|]}
		/if (!${Bool[${lstResident${_class}.Arg[${_count},|]}]}) /continue
		/if (!${Bool[${Spawn[${lstResident${_class}.Arg[${_count},|]}].ID}]}) /continue
		/if (${Spawn[${lstResident${_class}.Arg[${_count},|]}].Dead}) /continue
		/if (${Spawn[${lstResident${_class}.Arg[${_count},|]}].Distance} > ${_distance}) /continue

		/varset _name ${lstResident${_class}.Arg[${_count},|].Left[1].Upper}${lstResident${_class}.Arg[${_count},|].Right[-1].Lower}
		/if (${Bool[${_name}]}) /break
	/next _count	

/return ${_name}



|***
 *	DES: Checks external inputs: #events and forces bind checks
 *	USE: CHECKINPUTS
 *  NOTE: 
 ***|
sub check_inputs()
	| set a non-use variable to force bind checks. better this then an echo
	/varset bindTrigger ${If[${bindTrigger},FALSE,TRUE]}
		
	| check the event queue
	/doevents
	
/return TRUE



|***
 *	DES: the the mass group buff
 *	USE: CHECKINPUTS
 *  NOTE: 
 ***|
sub check_buffs_mgb()
	
	| do not use outside raid
	/if (!${Raid.Members}) /return FALSE
	
	| caster?
	/if (!${validate_class[FALSE, ${lstClassCast}]}) /return FALSE
	
	| need more mana
	/if (${Me.PctMana} < ${BuffMGBMana}) /return FALSE
	
	| more people!
	/if (${Raid.Members} < ${BuffMGBCount}) /return FALSE

	| is my stuff ready?
	/if (!${Me.AltAbilityReady[Tranquil Blessings]} && !${Me.AltAbilityReady[Mass Group Buff]}) /return FALSE
	
	| do I have
	/if (${Bool[${Me.Buff[${BuffMGBchk}].Duration}]}) /return FALSE
	
	| TB or MGB ??
	/if (${Me.AltAbilityReady[Tranquil Blessings]}) {
		/if (${validate_cast[FALSE, alt, "Tranquil Blessings", ${Me.ID}]}) /call core_cast2 "Tranquil Blessings" alt 0 FALSE	
	} else /if (${Me.AltAbilityReady[Mass Group Buff]}) {
		/if (${validate_cast[FALSE, alt, "Mass Group Buff", ${Me.ID}]}) /call core_cast2 "Mass Group Buff" alt 0 FALSE	
	}
	/if (${validate_cast[FALSE, spell, "${BuffMGBname}", ${Me.ID}]}) /call core_cast2 "${BuffMGBname}" ${buffgem} ${Me.ID} FALSE

/return


|***
 *	DES: checks and casts short term temp buffs that have a toon list.
 *	USE: /call check_buffs_tmp_list "${spellvariable}" "${toonlistvariable}"
 *  NOTE:
 ***|
sub check_buffs_tmp_list(string _spell, string _toonList)
	CHECKCOMBATCAST
	AMIDEAD
	SIMPLEEXIT
	CHECKINPUTS
	CHECKTIE
	
	/declare _count int local
	/declare _buffToon string local
	/declare _DNqout string local
	/varset _spell ${Spell[${_spell}].RankName}

	/for _count 1 to ${_toonList.Count[|]}
		/if (!${Bool[${_toonList.Arg[${_count},|]}]}) /continue
		/varset _buffToon ${_toonList.Arg[${_count},|]}
		
		| ready?
		/if (!${Me.SpellReady[${_spell}]}) /return
		
		| me?
		/if (${swBuffSelf}) {
			/if (${_toonList.Arg[${_count},|].Equal[${Me.DisplayName}]} && !${Me.Buff[${_spell}].ID}) {
				/if (${validate_cast[FALSE, spell, "${_spell}", ${Me.ID}]}) /call core_cast2 "${_spell}" 0 ${Me.ID} FALSE
				/continue
			}		
		}
		
		| get out if not buffing others
		/if (!${swBuffBC}) /return
		
		| ini set to FALSE?
		/if (!${Bool[${_buffToon}]}) /return
		
		| seriously dont buff corpses
		/if (${Spawn[${_toonList.Arg[${_count},|]}].Type.Equal[corpse]}) /continue
		
		| no one to cast on?
		/if (!${Spawn[${_toonList.Arg[${_count},|]}].ID}) /continue

		| spell stacks ?
		/dquery ${_toonList.Arg[${_count},|]} -q "Spell[${_spell}].NewStacks" -o _DNqout
		/if (!${_DNqout}) /continue
			
		| has the rune?
		/dquery ${_toonList.Arg[${_count},|]} -q "Me.Buff[${_spell}].ID" -o _DNqout
		/if (${Bool[${_DNqout}]}) /continue

		/if (${validate_cast[FALSE, spell, "${_spell}", ${Spawn[=${_toonList.Arg[${_count},|]} pc].ID}]}) /call core_cast2 "${_spell}" 0 ${Spawn[=${_toonList.Arg[${_count},|]} pc].ID} FALSE
	
	/next _count	

/return



|***
 *	DES: blanket value return for a variable
 *	USE: /varset var ${sql_return[FIELDNAME]}
 *  NOTE: 
 ***|
sub sql_return(string _variable)
	/declare _count int local 0
	/declare _countArray int local
	/declare _found bool local FALSE
	/declare _db string local ${dbCore}
		
	/for _count 1 to ${sqlTablesCount}

		| adjust db for character
		/if (${sqlTables.Arg[${_count},|].Equal[${chrID}]}) /varset _db ${chrDBID}
	
		
		/sqlite ${_db} dummyreturn SELECT ${_variable} FROM ${sqlTables.Arg[${_count},|]} WHERE currentBuild="${currentBuild}"
		/if (${sqlite.Rows[dummyreturn]}) {
			/varset _found TRUE
			/return ${sqlite.Result[dummyreturn 1 ${_variable}]}
		}			
	/next _count		
	
	| show error if any
	/if (!${_found}) OUT ${cbad}SQLite ERROR\ax ${sep} could not find ${sep} ${cnum}${_variable}\ax
	
/return FALSE



|***
 *	DES: delete colums from tables
 *	USE: 
 *  NOTE: attention span of a fucking gnat...
 ***|
sub sql_delete(string _table, string _variable)

/return



|***
 *	DES: build the sql database and tables.
 *	USE: NA
 *  NOTE: this will build the database and initial build fields for each table.
 ***|
sub sql_schema(bool _debug)
		
	| ok, something needs building...
	/declare _count int local 0
	/declare _db string local ${dbCore}

	VSQL \awDB\ax::\awcurrentBuild\ax::\aw${currentBuild}\ax
	
	| /delay ${Math.Calc[${Math.Rand[50]}+${Group.Member[${Me}].Index}0]}

	| cycle each table in the list
	/for _count 1 to ${sqlTables.Count[|]}
		/if (${_debug}) SDEBUG _count:${_count} .. ${sqlTables.Arg[${_count},|]}

		| adjust db for character
		/varset _db ${If[${sqlTables.Arg[${_count},|].Equal[${chrID}]},${chrDBID},${dbCore}]}
		/if (${_debug}) SDEBUG _db ${_db}
		
		| get the build from current iteration table. if it matches, we will continue on. if not???
		/sqlite ${_db} prevBuild SELECT currentBuild FROM ${sqlTables.Arg[${_count},|]}
		/if (${_debug}) SDEBUG prevBuild SELECT currentBuild FROM ${sqlTables.Arg[${_count},|]} == ${sqlite.Result[prevBuild 1 currentBuild]} || ${swForceUpdate}
	
		| query the build from database and store it as prevBuild
		/if (${sqlite.Rows[prevBuild]} || ${swForceUpdate}) {

			| if the previos build exists does equal the current build from core.mac
			/if (${sqlite.Result[prevBuild 1 currentBuild].Equal[${currentBuild}]} && !${swForceUpdate}) {
				VSQL \awDB\ax::\aw${sqlTables.Arg[${_count},|]}\ax::\a-tCurrent\ax
				/varset bld${sqlTables.Arg[${_count},|]}current TRUE
				| /sqlite ${_db} dummy UPDATE ${sqlTables.Arg[${_count},|]} SET currentBuild="${currentBuild}" 
				
				/goto :loadTableData

			| if the previous build does not match the current build from core.mac
			} else /if (${sqlite.Result[prevBuild 1 currentBuild].NotEqual[${currentBuild}]} || ${swForceUpdate}) {
				VSQL \awDB\ax::\aw${sqlTables.Arg[${_count},|]}\ax::\agUpdating\ax
				/sqlite ${_db} dummy UPDATE ${sqlTables.Arg[${_count},|]} SET currentBuild="${currentBuild}"
			}
		
		| if no rows returned, then there is no database. we got work to do
		} else {
			VSQL \awDB\ax::\aw${sqlTables.Arg[${_count},|]}\ax::\agBuilding\ax
			/if (${_debug}) SDEBUG build::${sqlTables.Arg[${_count},|]} \arMissing\ax
			/sqlite ${_db} dummy CREATE TABLE ${sqlTables.Arg[${_count},|]} (currentBuild TEXT UNIQUE)
			/sqlite ${_db} dummy INSERT INTO ${sqlTables.Arg[${_count},|]} (currentBuild) VALUES ("${currentBuild}")
		}

		| load table data	
		:loadTableData
		/if (${sqlTables.Arg[${_count},|].Equal[${chrID}]} || ${swForceUpdate}) {
			/call pregen_character
			/call pregen_${Me.Class.ShortName}
		} else {
			/call pregen_${sqlTables.Arg[${_count},|]}
		}
		
		| /delay ${Math.Rand[20]}
	/next _count
	
	| get pragma write mode and hang on to it
	/sqlite ${dbCore} query_journal_core PRAGMA journal_mode=WAL
	/sqlite ${chrDBID} query_journal_character PRAGMA journal_mode=WAL
			
/return TRUE



|***
 *	DES: gets a list of a tables colum names
 *	USE: 
 *  NOTE:
 ***|
sub sql_getcolums(bool _debug, string _table)
	/declare _count int local 0
	/declare _db string local ${dbCore}

	| adjust db for character
	/if (${_table.Equal[${chrID}]}) /varset _db ${chrDBID}
	
	| get table pragman data
	/sqlite ${_db} colnamequery PRAGMA table_info(${_table})
	
	/if (${_debug}) echo /sqlite ${_db} colnamequery PRAGMA table_info(${_table}) : ${sqlite.Rows[colnamequery]}

	| /echo ${sqlite.Rows[colnamequery]}
	| /return

	| cycle table by toon
	/if (${sqlTables.Find[${_table}]}) {	
		/for _count 2 to ${sqlite.Rows[colnamequery]}
			/sqlite ${dbCore} ${_db} SELECT ${sqlite.Result[colnamequery ${_count} name]} FROM ${_table} WHERE currentBuild="${currentBuild}"
			/echo ${sqlite.Result[colnamequery ${_count} name]} ${sep} ${cnum}${sql_return[${sqlite.Result[colnamequery ${_count} name]}]}\ax
		/next _count
	}

/return



|***
 *	DES: sql data controler
 *	USE: see below
 *  NOTE: theres a lot
 *  _debug - echo shit....
 *  _table - table the field is stored in
 *  _field - the table field MUST MATCH MQ VARIABLE NAME
 *  _type - string, int, bool
 *  _scope - outer, local, global
 *  _value - value of the data you wish to change
 ***|
sub sql_control(bool _debug, string _table, string _field, string _type, string _scope, string _value, bool _spell)
	/if (${_debug}) /echo \awsql_update\ax(table:\at${_table}\ax field:\at${_field}\ax type:\at${_type}\ax scope:\at${_scope}\ax value:\at${_value}\ax)
	/declare _count int local 0 
	/declare _tmpVar string local
	/declare _db string local ${dbCore}

	| adjust db for character
	/if (${_table.Equal[${chrID}]}) /varset _db ${chrDBID}

	/if (${bld${_table}current}) {
		/if (${_debug}) SDEBUG bld${_table}current is current we are skipping the majority of sql_control
		/sqlite ${_db} dummy SELECT ${_field} FROM ${_table} WHERE currentBuild="${currentBuild}"
		/goto :currentbuildskip
	}

	| check field

	/sqlite ${_db} dummy SELECT ${_field} FROM ${_table}
	/if (${_debug}) SDEBUG	/sqlite dbCore dummy SELECT ${_field} FROM ${_table} ${sqlite.Rows[dummy]}

	/if (!${sqlite.Rows[dummy]}) {
		/sqlite ${_db} dummy ALTER TABLE ${_table} ADD ${_field} 
		/if (${_debug}) SDEBUG \aw${_table}\ax::\aw${_field}\ax
	}	

	| checking for unique /primary key. if not there, build it and reattach query.
	/if (${sqlTables.Find[${_table}]}) {
		/sqlite ${_db} dummy SELECT ${_field} FROM ${_table} WHERE currentBuild="${currentBuild}"
		/if (!${sqlite.Rows[dummy]}) {
			/sqlite ${_db} dummy INSERT INTO ${_table} (currentBuild) VALUES ("${currentBuild}")
			/sqlite ${_db} dummy SELECT ${_field} FROM ${_table} WHERE currentBuild="${currentBuild}"
		}
	}

	| k on to the data. finally.. this is taking forever..
	/if (${sqlTables.Find[${_table}]} && ${sqlite.Result[dummy 1 ${_field}].Equal[NULL]}) {
		/if (${_value.Equal[PREGENDATA]}) {
			/for _count 1 to ${Ini[${INIPreGen},${Me.Class.ShortName},${_field}].Count[|]}
				/if (${Me.Level} >= ${Ini[${INIPreGen},${Me.Class.ShortName},${_field}].Arg[${_count},|]}) {
					/varset _tmpVar ${Ini[${INIPreGen},${Me.Class.ShortName},${_field},noparse].Arg[${Math.Calc[${_count}+1]},|]}
					/if (${_debug}) /echo PREGENDATA: ${Ini[${INIPreGen},${Me.Class.ShortName},${_field},noparse].Arg[${Math.Calc[${_count}+1]},|]}
					/sqlite ${_db} dummy UPDATE ${_table} SET ${_field}="${_tmpVar}" WHERE currentBuild="${currentBuild}"
					/sqlite ${_db} dummy SELECT ${_field} FROM ${_table} WHERE currentBuild="${currentBuild}"
					/break
				}
				/varset _count ${Math.Calc[${_count}+1]}	
			/next _count	
			
		| if what we are passing does not equal whats in the database, take new value and reattach query
		} else /if (${_value.NotEqual[PREGENDATA]}) {
			/sqlite ${_db} dummy UPDATE ${_table} SET ${_field}="${_value}" WHERE currentBuild="${currentBuild}"
			/sqlite ${_db} dummy SELECT ${_field} FROM ${_table} WHERE currentBuild="${currentBuild}"
		}

	| not using a toon name, using a currentBuild
	} else /if (${sqlite.Result[dummy 1 ${_field}].Equal[NULL]}) {
		| new empty fields needs some sweet loving...
		/sqlite ${_db} dummy UPDATE ${_table} SET ${_field}="${_value}" WHERE currentBuild="${currentBuild}"
		/sqlite ${_db} dummy SELECT ${_field} FROM ${_table} WHERE currentBuild="${currentBuild}"
	}

	| skipping because data is current
	:currentbuildskip
		
	| what has been added to DB
	/if (${_debug}) SDEBUG \aw${_table}\ax::\aw${_field}\ax::\ag${_value}\ax

	| is this an update from edit or to establish
	| define the variable if it does not exist
	/if (!${Defined[${_field}]}) /declare ${_field} ${_type} outer
	| populate the variable
	/varset ${_field} ${sqlite.Result[dummy 1 ${_field}]}

  | Not a spell, move on
  /if (!${_spell}) /return

	| adjust rank names for spells
  /varset ${_field} ${Spell[${${_field}}].RankName}
  /if (${_debug}) SDEBUG \aw${_table}\ax::\aw${_field}\ax::\ag${${_field}}\ax
  /delay 3    
/return TRUE



|***
 *	DES: sql data adder
 *	USE: adds tables/fields at mac start time. nothing else.
 *  NOTE: theres a lot
 *  _debug - echo shit....
 *  _table - table the field is stored in
 *  _field - the table field MUST MATCH MQ VARIABLE NAME
 ***|
sub sql_staticbuild(bool _debug, string _table, string _field, string _value)
	/if (${_debug}) /echo \awsql_staticbuild\ax(\awDB\ax::\aw${_table}\ax::\aw${_field}\ax::${cnum}"${_value}"\ax)

	| skip if build is current
	/if (${bld${_table}current}) /return

	/declare _count int local 0 

	| check field
	/sqlite ${dbCore} dummy SELECT ${_field} FROM ${_table}

	| if it does not exist OR if the actual data has changed, we need to adjust
	/if (!${sqlite.Rows[dummy]} || ${sqlite.Result[dummy 1 ${_field}].NotEqual[${_value}]}) {
	
		/sqlite ${dbCore} dummy ALTER TABLE ${_table} ADD ${_field} 
		/sqlite ${dbCore} dummy UPDATE ${_table} SET ${_field}="${_value}" WHERE currentBuild="${currentBuild}"
		
		/if (${_debug}) VSQL \aw${_table}\ax::\aw${_field}\ax
	}	

	| what has been added to DB
	/if (${_debug}) VSQL \aw${_table}\ax::\aw${_field}\ax
	    
/return TRUE



|***
 *	DES: adds ranknames to spells 
 *	USE:/call rank_add [debug] [name] / ${rank_add[FALSE, "name"]}
 *  NOTE: 
 ***|
sub rank_add(bool _debug, string _spell)
	/if (${_debug}) /echo Start Name:\at${_spell}\ax

	| rk3 spell
	/if (${Me.Book[${Me.Book[${_spell} Rk. III]}].ID}) {
		/varset _spell ${Me.Book[${Me.Book[${_spell} Rk. III]}].Name} 
	
	| rk2 spell
	} else /if (${Me.Book[${Me.Book[${_spell} Rk. II]}].ID}) {
		/varset _spell ${Me.Book[${Me.Book[${_spell} Rk. II]}].Name}

	| rk1 spell
	} else /if (${Me.Book[${Me.Book[${_spell}]}].ID}) {
		/varset _spell ${Me.Book[${Me.Book[${_spell}]}].Name}

	| rk3 disc
	} else /if (${Me.CombatAbility[${_spell} Rk. III]}) {
	 	/varset _spell ${${_spell}} Rk. III

	| rk2 disc
	} else /if (${Me.CombatAbility[${_spell} Rk. II]}) {
	 	/varset _spell ${_spell} Rk. II
	
	| rk1 disc
	} else /if (${Me.CombatAbility[${_spell} Rk. II]}) {
	 	/varset _spell ${_spell}

	} 

	/if (${_debug}) /echo End Name:\at${_spell}\ax

/return ${_spell}



|***
 *	DES: sql data updater
 *	USE: 
 *  NOTE:
 *  _debug - echo shit....
 *  _table - table the field is stored in
 *  _field - the table field MUST MATCH MQ VARIABLE NAME
 *  _value - value of the data you wish to change
 ***|
sub sql_update(bool _debug, string _table, string _field, string _value)
	/if (${_debug}) VSQL \awsql_update\ax(table:\at${_table}\ax field:\at${_field}\ax value:\at${_value}\ax)

	/declare _db string local ${dbCore}

	| adjust db for character
	/if (${_table.Equal[${chrID}]}) /varset _db ${chrDBID}

	| and reattach query.
	/if (${sqlTables.Find[${_table}]}) {	
		/sqlite ${_db} dummy SELECT ${_field} FROM ${_table} WHERE currentBuild="${currentBuild}"
	}

	| k on to the data. finally.. this is taking forever..
	/if (${sqlTables.Find[${_table}]}) {
		/if (${_value.NotEqual[${sqlite.Result[dummy 1 ${_field}]}]} || ${_value.Equal[FALSE]}) {
			/sqlite ${_db} dummy UPDATE ${_table} SET ${_field}="${_value}"
			/sqlite ${_db} dummy SELECT ${_field} FROM ${_table}
		}	
	}
	
	| what has been added to DB
	/if (${_debug}) VSQL \awDB\ax::\aw${_table}\ax::\aw${_field}\ax::\ag${_value}\ax::${sqlite.Status[dummy]}
	
	| populate the variable
	/varset ${_field} ${sqlite.Result[dummy 1 ${_field}]}

/return



|***
 *	DES: basic 3-way switch control
 *	USE: /call set_switch [dbTable] [switchname] [set type] || ${set_switch[dbTable, switchname, value]}
 *  NOTE:
 ***|
sub set_switch(string _table, string _switch, string _verbage) 
	| SDEBUG \atset_switch\ax(_table:${_table}, _switch:${_switch}, _verbage:${_verbage}) 
	| swap the switch status
	/if (${_verbage.Equal[NULL]} || (!${Bool[${_verbage}]} && ${_verbage.NotEqual[FALSE]}) || ${_verbage.Equal[TOGGLE]}) {
		/call sql_update FALSE ${_table} ${_switch} ${If[${${_switch}},FALSE,TRUE]}
		/return ${${_switch}}

	| set switch TRUE
	} else /if (${_verbage.Equal[on]} || ${_verbage.Equal[TRUE]}) {
		/call sql_update FALSE ${_table} ${_switch} TRUE
		/return ${${_switch}}

	| set switch FALSE		
	} else /if (${_verbage.Equal[off]} || ${_verbage.Equal[FALSE]}) {
		/call sql_update FALSE ${_table} ${_switch} FALSE
		/return ${${_switch}}
	}

/return



|***
 *	DES: This will clear all store alises that core has created in your MacroQuest2.INI file. or those with matching names.
 *	USE: /core whitewash [NOECHO]
 *  NOTE: IF you instance/sandbox MQ/EQ it is best you run this on all toons at the same time.
 ***|
sub whitewash(string _echo)
	/if (!${_echo.Equal[NOECHO]}) VOUT ${owarning} ${sep} Deleting all Core ${cinfo}${Float[${currentBuild}].Precision[2]}\ax aliases. ${cbad}Please Wait\ax
	/declare _count 				int local 0
	/declare _todelete[1] 	string local
	/declare _index 				int local 0
	/declare _increment			int local 1
	
	| core aliases
	/varset _todelete[1] |take|door|stack|here|sendpets|killmob|mirror|stein|stop|blurb|bcsay|echoaugments|autologin|rc|purify|buy|sell|on|off|zonereset|givecoin|oneoff|gtfo|lesson|clean|checkvar|bcsay
	
	/for _count 1 to ${_todelete.Size}
		/for _index 1 to ${_todelete[${_count}].Count[|]}
			/if (!${_echo.Equal[NOECHO]}) VOUT Deleting Alias: ${cinfo}${_todelete[${_count}].Arg[${_index},|]}\ax
			/squelch /alias /${_todelete[${_count}].Arg[${_index},|]} delete
			/varcalc _increment ${_increment}+1
		/next _index	
	/next _count
	
	/if (!${_echo.Equal[NOECHO]}) {
		/call echos ending
		/endm
	}

/return 



|***
 *	DES: controls for the current zone
 *	USE: /call control_zone
 *  NOTE: 
 ***| 
sub control_zone(bool _debug)

	/declare _count int local	0
	/declare _tmpList string local
	/declare _zonelist string local ${sql_return[ZoneFileList]}

	| is the zone there?
	/sqlite ${dbCore} dummy SELECT ShortName FROM zone WHERE ShortName="${Zone.ShortName.Lower}"
	/if (!${sqlite.Rows[dummy]}) {

		| zone is not there
		VSQL Zone Build ${sep} \awzone\ax::\at${Zone.Name}\ax
		/sqlite ${dbCore} dummy INSERT INTO zone (ShortName) VALUES ("${Zone.ShortName.Lower}")
	} 
	
	| get the zone data
	/sqlite ${dbCore} curZoneData SELECT * FROM zone WHERE ShortName="${Zone.ShortName.Lower}"

	| populate the zone fields
	/for _count 1 to ${_zonelist.Count[|]}
		/varset ${_zonelist.Arg[${_count},|]} ${sqlite.Result[curZoneData 1 ${_zonelist.Arg[${_count},|]}]}
		/if (${_debug}) VSQL \awzone\ax::\aw${_zonelist.Arg[${_count},|]}\ax::\at${sqlite.Result[curZoneData 1 ${_zonelist.Arg[${_count},|]}]}\ax
	/next _count
	
/return



|***
 *	DES: removes rank names from spells
 *	USE: /call rank_remove [debug] [name] / ${rank_remove[FALSE, "name"]}
 *  NOTE: 
 ***|
sub rank_remove(bool _debug, string _spell)
	/if (${_debug}) /echo Start Name:\at${_spell}\ax

	| rank 3	
	/if (${Spell[${_spell}].Name.Find[Rk. III]}) {
		/varset _spell ${Spell[${_spell}].Name.Left[-8]}

	| rank2
	} else /if (${Spell[${_spell}].Name.Find[Rk. II]}) {
		/varset _spell ${Spell[${_spell}].Name.Left[-7]}
		
	| no rank.. slacker bastards
	} 
	
	/if (${_debug}) /echo End Name:\at${_spell}\ax

/return ${_spell}



|***
 *	DES: sets the spires a character will use
 *	USE: /call set_character_spire [debug] [spiregroup|spireraid] [0|1|2|3]
 *  NOTE: 0 is off / default
 ***|
sub set_character_spire(bool _debug, string _type, int _verbage)
	| SDEBUG \atset_character_spire\ax(_debug:${_debug}, _type:${_type}, _verbage:${_verbage})
	/declare _sep bool local
	/declare _listout string local
	/declare _count int local 0
	/declare _list string local ${sql_return[lstTypesSpire]}

	| correct the variable case for the database
	/varset _type ${_type.Left[5].Lower}${_type.Mid[6,1].Upper}${_type.Right[-6].Lower}
		
	| validate a 0 to 3 entry for the spire type
	/if (${Range.Between[0,3:${Int[${_verbage}]}]}) {
		/call sql_update ${_debug} ${chrID} ${_type} ${_verbage}
	} 
	
	/if (${swMQClear}) /mqclear
	/for _count 1 to ${_list.Count[|]}
		/varset _listout ${_listout} ${If[${_sep},${sep},]} ${If[${${_type}} == ${_list.Arg[${_count},|]},${cgood},${cinfo}]}${_list.Arg[${_count},|]}\ax   
		/varset _sep TRUE
	/next _count
	VOUT /${Me.Class.ShortName.Lower} ${_type.Lower} [${_listout}]	

/return



|***
 *	DES: sets the spires a character will use
 *	USE: /call set_character_fade [debug] [spiregroup|spireraid] [0|1|2|3]
 *  NOTE: 0 is off / default
 ***|
sub set_character_fade(bool _debug, int _verbage)
	/declare _sep bool local
	/declare _listout string local
	/declare _count int local 0

	| validate % entry
	/if (${Bool[${_verbage}]} && ${Range.Between[1,99:${Int[${_verbage}]}]}) {
		/call sql_update FALSE ${chrID} useAAFadePct ${_verbage}
	} else /if (${Bool[${_verbage}]} && !${Range.Between[1,99:${Int[${_verbage}]}]}) {
		VOUT /${Me.Class.ShortName.Lower} fade(${cnum}${useAAFadePct}\ax) allowable range ${sep} ${cinfo}1 - 99\ax
		/return FALSE
	} else {
	 	/call sql_update FALSE ${chrID} switchAAFade ${If[${switchAAFade},FALSE,TRUE]}  
	}

/return TRUE



|***
 *	DES: sets the XTarget Heal classes a character will use
 *	USE: /call set_character_XTHealClass [debug] [class short name]
 *  NOTE:
 ***|
sub set_character_xthealclass(bool _debug, string _verbage)
	/declare _sep bool local
	/declare _listout string local
	/declare _count int local 0

	/if (${Bool[${_verbage}]} && ${lstClassAll.Find[${_verbage}]}) {
		
		| if its there, delete
		/if (${XHealClass.Find[${_verbage}]}) {
			/call Bind_sql_edit XHealClass "${_verbage.Upper}" delete

		| if its not there, add	
		}	else /if (!${XHealClass.Find[${_verbage}]}) {
			/call Bind_sql_edit XHealClass "${_verbage.Upper}" add 
		}			
		
		| update xtargets window and timer
	  /if (${swHealXTarget}) {
	    /call set_xtarget
	    /call timer_update timer_set_xtarget ${tXHeal}
		}			
		
	}
		
	/if (${swMQClear}) /mqclear
	/for _count 1 to ${lstClassAll.Count[|]}
		/varset _listout ${_listout} ${If[${_sep},${sep},]} ${cinfo}${lstClassAll.Arg[${_count},|].Lower}\ax:${If[${XHealClass.Find[${lstClassAll.Arg[${_count},|]}]},${oon},${ooff}]}
		/varset _sep TRUE
	/next _count
	VOUT /${Me.Class.ShortName.Lower} xtclass [${_listout}]
		
/return



|***
 *	DES: sets the healpoints for healing a class
 *	USE: /call set_character_healpoint [debug] [%## heal at]
 *  NOTE: these damn people gethurt too much
 ***|
sub set_character_healpoint(bool _debug, int _verbage, bool _silent)
	/declare _sep bool local
	/declare _listout string local
	/declare _count int local 0
	
	| not a "Healer" class
	/if (!${Select[${Me.Class.ShortName},CLR,PAL,DRU,SHM]}) {
		/if (${Range.Between[1,99:${Int[${_verbage}]}]}) {
			/call sql_update ${_debug} ${chrID} ${Me.Class.ShortName}Point ${_verbage}
		} else /if (!${Range.Between[1,99:${Int[${_verbage}]}]}) {
			VOUT /${Me.Class.ShortName.Lower} healpoint(${cnum}${${Me.Class.ShortName}Point}\ax) allowable range ${sep} ${cinfo}1 - 99\ax
		}
		/return
		
	| Healer Classes
	} else {
		/if (${Target.ID} && ${Range.Between[1,99:${Int[${_verbage}]}]} && ${Spawn[${Target.ID}].Type.NotEqual[Pet]}) {
			/call sql_update ${_debug} ${chrID} ${Target.Class.ShortName}Point ${_verbage}
		} else /if (${Target.ID} && ${Range.Between[1,99:${Int[${_verbage}]}]} && ${Spawn[${Target.ID}].Type.Equal[Pet]}) {
			/call sql_update ${_debug} ${chrID} PetPoint ${_verbage}
		} else {
			VOUT /${Me.Class.ShortName.Lower} healpoint [${cinfo}##\ax] [${cinfo}TARGET\ax]
		}
		
		/if (${_silent}) /return
		/if (${swMQClear}) /mqclear
		/for _count 1 to ${lstClassAll.Count[|]}
			/varset _listout ${_listout} ${If[${_sep},${sep},]} ${cinfo}${lstClassAll.Arg[${_count},|].Lower}\ax:${cnum}${${lstClassAll.Arg[${_count},|].Upper}Point}\ax
			/varset _sep TRUE
		/next _count
		/varset _listout ${_listout} ${If[${_sep},${sep},]} pet:${cnum}${PetPoint}\ax
		/varset _listout ${_listout} ${If[${_sep},${sep},]} gom:${cnum}${GoMpoint}\ax
		/varset _listout ${_listout} ${If[${_sep},${sep},]} panic:${cnum}${PanicPoint}\ax
		VOUT /${Me.Class.ShortName.Lower} healpoint [${_listout}]
	}		
		
/return
		


|***
 *	DES: control for heal classes groupheal switch
 *	USE: /call set_character_healgroup [debug] [0-6]
 *  NOTE:
 ***|
sub set_character_groupheal(bool _debug, int _verbage)
	/if (!${Bool[${_verbage}]} || ${_verbage} == FALSE) {
		/call sql_update FALSE ${chrID} useGroupHeal 0
	} else /if (${Bool[${_verbage}]} && ${Range.Between[1,6:${Int[${_verbage}]}]}) {
		/call sql_update FALSE ${chrID} useGroupHeal ${_verbage} 
	} else /if (${Bool[${_verbage}]} && !${Range.Between[1,6:${Int[${_verbage}]}]}) {
		VOUT /${_bind} groupheal [${cinfo}${useGroupHeal}\ax] allowable range ${sep} ${cinfo}1 - 6\ax
		/return
	}
/return



|***
 *	DES: clear a sql query from memory
 *	USE: /call sql_clear DEBUG QUERYNAME
 *  NOTE: 
 ***|
sub sql_clear(bool _debug, string _query)
	/if (!${sqlite.Clear[${_query}]})  {
		VOUT ${owarning} ${seo} Couldn't clear memory for: ${cinfo}${_query}\ax
	}
/return



|***
 *	DES: check group health
 *	USE: 
 *  NOTE: shared by PAL,DRU,CLR,SHM
 ***|
sub check_health_group()
	/declare _count int local 0

	/for _count 0 to ${Group}
		/if (${Group.Member[${_count}].PctHPs} > ${${Group.Member[${_count}].Class.ShortName}Point}*.${tmpHealOverride}) /continue
		/if (${Group.Member[${_count}].PctHPs} > ${hurtPctHP}) /continue
		/if (${Group.Member[${_count}].Distance} > ${setMobAgro}) /continue
		/if (!${Group.Member[${_count}].Present} || ${Group.Member[${_count}].Dead}) /continue
		/if (${Group.Member[${_count}].DisplayName.Equal[${RampageTank}]}) /continue
		/if (${lstNoHeal.Find[${Group.Member[${_count}].DisplayName}]}) /continue

		| we found a slacker
		/varset hurtPctHP ${Group.Member[${_count}].PctHPs}
		/varset hurtIndex ${_count}
		/varset hurtCLS ${Group.Member[${_count}].Class.ShortName}
		/varcalc hurtCount ${hurtCount}+1
		/if (${useGroupHeal}) {
			/if (${hurtCount} >= ${useGroupHeal}) /break
		}
	
	/next _count

/return




|***
 *	DES: check XTarget health
 *	USE: 
 *  NOTE: shared by PAL,DRU,CLR,SHM
 ***|
sub check_health_xtarget()
	/declare _count int local 0

	/for _count 1 to 13
  	/if (!${Me.XTarget[${_count}].ID}) /continue
  	/if (${Me.XTarget[${_count}].Dead}) /continue
  	/if (${Me.XTarget[${_count}].PctHPs} > ${hurtPctHP}) /continue
  	/if (${Me.XTarget[${_count}].Distance} > ${setMobAgro}) /continue
		/if (${Me.XTarget[${_count}].DisplayName.Equal[${RampageTank}]}) /continue
		/if (${lstNoHeal.Find[${Me.XTarget[${_count}].DisplayName}]}) /continue
  	/if (!${Me.XTarget[${_count}].Type.Equal[PC]} && !${ProtectNPCList.Find[${Me.XTarget[${_count}].DisplayName}]}) /continue
		/if (${Me.XTarget[${_count}].PctHPs} > ${${Me.XTarget[${_count}].Class.ShortName}Point}*.${tmpHealOverride}) /continue

		/varset hurtIndex ${_count}
		/varset hurtPctHP ${Me.XTarget[${_count}].PctHPs}
		/varset hurtCLS ${Me.XTarget[${_count}].Class.ShortName}
		/varset hurtCount 1

  /next _count

/return



|***
 *	DES: check pet health
 *	USE: 
 *  NOTE: shared by PAL,DRU,CLR,SHM
 ***|
sub check_health_pet()
	/declare _count int local 0
	| /varset hurtPctHP 100
	
	/for _count 0 to ${Group}
		/if (!${Group.Member[${_count}].Pet.ID}) /continue
    /if (${Group.Member[${_count}].Pet.PctHPs} >= ${PetPoint}) /continue
    /if (${Group.Member[${_count}].Pet.Distance} > ${setMobAgro}) /continue
    /if (${Group.Member[${_count}].Pet.PctHPs} >= ${hurtPctHP}) /continue
		/varset hurtIndex ${_count}
		/varset hurtPctHP ${Group.Member[${_count}].Pet.PctHPs}
  /next _count

/return



|***
 *	DES: just clears the xtarget list
 *	USE: 
 *  NOTE: really thats all it does
 ***| 
sub clear_xtarget()
	/declare _count int local 0
	/for _count 1 to 13
		/xtarget set ${_count} Autohater
	/next _count
	
/return



|***
 *	DES: checks to see if something is ready to be used
 *	USE: 
 *  NOTE:
 ***| 
sub check_ready(string _type, string _tocheck)

	/if (${_type.Equal[spell]}) {
		| /if (!${Me.Gem[${Spell[${_tocheck}].RankName}]}) /return FALSE
		/if (${Me.SpellReady[${Spell[${_tocheck}].RankName}]}) /return TRUE

	} else /if (${_type.Equal[alt]}) {
		/if (${Me.AltAbilityReady[${_tocheck}]}) /return TRUE
		
	} else /if (${_type.Equal[disc]}) {
		/if (${Me.CombatAbility[${Spell[${_tocheck}].RankName}]}) /return TRUE
		
	} else /if (${_type.Equal[item]}) {
		/if (${Me.ItemReady[${_tocheck}]}) /return TRUE

	}

/return FALSE



|***
 *  DES: Engages a merc based off curent control scructure
 *  USE: /call merc_control
 *  NOTE: 
 
 ${Mercenary.State}'s
 SUSPENDED (ID:1)
 ACTIVE (ID:5)
 DEAD (ID: )
 COMBAT (ID: )

 ${Mercenary.Stance}'s
 Rogue   : Passive, Balanced, Burn
 Warrior : Passive, Assist, Aggressive
 Cleric  : Passive, Balanced, Reactive, Effecient
 Wizard  : Passive, Balanced, Burn, Burn AE

 ***|
sub merc_control()

	| manual mode
	/if (!${coreAuto}) {
		/if (!${Bool[${combat}]} && !${Target.ID} && ${Me.MercenaryStance.NotEqual[Passive]}) {
			/stance Passive
			/return FALSE
		}
		/if (${Target.ID}) {
			/if (${Target.ID} == ${Me.ID} && ${Me.MercenaryStance.NotEqual[Passive]}) {
				/stance Passive
				/return FALSE
			}
			/if (${Bool[${combat}]}) {
				/if (${swBurnControl}) {
					/if (${Select[${Mercenary.Class.Name},Rogue,Wizzard,]}) {
						/if (${Me.MercenaryStance.NotEqual[Burn]}) /stance Burn						
					} else /if (${Select[${Mercenary.Class.Name},Warrior]}) {
						/if (${Me.MercenaryStance.NotEqual[Aggressive]}) /stance Aggressive						
					} else /if (${Select[${Mercenary.Class.Name},Cleric]}) {
						/if (${Me.MercenaryStance.NotEqual[Reactive]}) /stance Reactive						
					}
				} else {
					/if (${Select[${Mercenary.Class.Name},Rogue,Wizzard,Cleric]}) {
						/if (${Me.MercenaryStance.NotEqual[Balanced]}) /stance Balanced						
					} else /if (${Select[${Mercenary.Class.Name},Warrior]}) {
						/if (${Me.MercenaryStance.NotEqual[Assist]}) /stance Assist						
					} 
				}
			}
			/return TRUE
		} else /if (!${Target.ID}) {
			/if ((${Bool[${combat}]} || ${isValidTarget}) && ${Me.MercenaryStance.NotEqual[Passive]}) /stance Passive
			/return FALSE
		}

	| automatic mode
	} else /if (${coreAuto}) {
		/if (!${isValidTarget} || !${Spawn[${Target2Kill}].ID}) {
			/if (${Me.MercenaryStance.NotEqual[Passive]}) /stance Passive
			/return FALSE
		} else /if (${isValidTarget} && ${Spawn[${Target2Kill}].ID}) {
			/if (${swBurnControl}) {
				/if (${Select[${Mercenary.Class.Name},Rogue,Wizzard,]}) {
					/if (${Me.MercenaryStance.NotEqual[Burn]}) /stance Burn						
				} else /if (${Select[${Mercenary.Class.Name},Warrior]}) {
					/if (${Me.MercenaryStance.NotEqual[Aggressive]}) /stance Aggressive						
				} else /if (${Select[${Mercenary.Class.Name},Cleric]}) {
					/if (${Me.MercenaryStance.NotEqual[Reactive]}) /stance Reactive						
				}
			} else {
				/if (${Select[${Mercenary.Class.Name},Rogue,Wizzard,Cleric]}) {
					/if (${Me.MercenaryStance.NotEqual[Balanced]}) /stance Balanced						
				} else /if (${Select[${Mercenary.Class.Name},Warrior]}) {
					/if (${Me.MercenaryStance.NotEqual[Assist]}) /stance Assist						
				} 
			}
		}
	}

/return TRUE



|***
 *	DES: generates a list of blocked buffs to be used as a reference for other bc buffers
 *	USE: whenever i suppose
 *  NOTE: 
 ***|
sub check_buffs_blocked(bool _debuf, bool _silent)
	/declare _count int local 0
	/declare _tmpList string local
	
	/if (!${_silent}) OUT ${onotice} ${sep} Updating Blocked Buffs

	| cycle the blocked buff list and add them to a string
	/squelch /windowstate BlockedBuffWnd open
	/for _count 0 to 39
		| skip empty ones
		/if (${Window[BlockedBuffWnd].Child[BW_BLOCKEDBUFF${_count}_Button].Tooltip.Length} < 1) /continue

		| + -> ++
		/varset _tmpList ${_tmpList}|${Window[BlockedBuffWnd].Child[BW_BLOCKEDBUFF${_count}_Button].Tooltip}
	/next _count
	/squelch /windowstate BlockedBuffWnd close

	| write it so the buffers dont go apeshit
	/call Bind_sql_edit lstBlockedSpells "${_tmpList}" NA TRUE
	
	| set the timer again
	/call timer_update timer_check_buffs_blocked ${tBlocked_Buff}

/return



|***
 *	DES: moves the toon to a valid/LoS location of a target id with a given distance
 *	USE: /call moveto_range [ID] [DISTANCE]
 *  NOTE: requires MQ2nav, ${cos[]}/${sin[]} are evaluated at start of mac in pregen.inc
 ***|
sub moveto_range(int _tmpID, int _range)

	/declare _i int local 0
	/declare _locX float local 0
	/declare _locY float local 0
	/declare _locZ float local 0
	/declare _tmpX float local 0
	/declare _tmpY float local 0
	/declare _dist float local ${Math.Calc[${If[${_range}==0,1,${_range}]} / ${If[${Spawn[${_tmpID}].Distance}==0,1,${Spawn[${_tmpID}].Distance}]}]}
	
	/varcalc _locX ${_dist}*${Me.X} + (1-${_dist})*${Spawn[${_tmpID}].X}	
	/varcalc _locY ${_dist}*${Me.Y} + (1-${_dist})*${Spawn[${_tmpID}].Y}	
	/if (${Navigation.PathExists[locyxz ${_locY} ${_locX} ${Me.Z}]} && ${EverQuest.ValidLoc[${_locX} ${_locY} ${Me.Z}]} && ${LineOfSight[${_locY},${_locX},${Me.Z}:${Spawn[${_tmpID}].Y},${Spawn[${_tmpID}].X},${Spawn[${_tmpID}].Z}]}) {
		/nav locyxz ${_locY} ${_locX} ${Me.Z} log=off
		/delay 5 ${Navigation.Active}
		/delay 5s !${Navigation.Active}
		/return
	}

	/for _i 1 to 10
		/varcalc _tmpX ${_locX}*${cos[${_i}]} - ${_locY}*${sin[${_i}]}
		/varcalc _tmpY ${_locX}*${cos[${_i}]} + ${_locY}*${sin[${_i}]}
		| /varcalc _tmpZ ${_locZ}*${cos[${_i}]} + ${_locY}*${sin[${_i}]}
		/if (${Navigation.PathExists[locyxz ${_tmpY} ${_tmpX} ${Me.Z}]} && ${EverQuest.ValidLoc[${_tmpX} ${_tmpY} ${Me.Z}]} && ${LineOfSight[${_tmpY},${_tmpX},${Me.Z}:${Spawn[${_tmpID}].Y},${Spawn[${_tmpID}].X},${Spawn[${_tmpID}].Z}]}) {
			/nav locyxz ${_tmpY} ${_tmpX} ${Me.Z} log=off
			/delay 5 ${Navigation.Active}
			/delay 5s !${Navigation.Active}
			/break
		}

		/varcalc _tmpX ${_locX}*${cos[${_i}]} + ${_locY}*${sin[${_i}]}
		/varcalc _tmpY ${_locX}*${cos[${_i}]} - ${_locY}*${sin[${_i}]}
		/if (${Navigation.PathExists[locyxz ${_tmpY} ${_tmpX} ${Me.Z}]} && ${EverQuest.ValidLoc[${_tmpX} ${_tmpY} ${Me.Z}]} && ${LineOfSight[${_tmpY},${_tmpX},${Me.Z}:${Spawn[${_tmpID}].Y},${Spawn[${_tmpID}].X},${Spawn[${_tmpID}].Z}]}) {
			/nav locyxz ${_tmpY} ${_tmpX} ${Me.Z} log=off
			/delay 5 ${Navigation.Active}
			/delay 5s !${Navigation.Active}
			/break
		}
	    
	/next _i

/return



|***
 *	DES: waits for something to get done
 *	USE: /call wait_while [thing.to.wait.for] [duration]
 *  NOTE: 
 ***|
sub wait_while(string _what, string _time)
	/while (${${_what}}) {
 		/delay ${_time}
	}
	
/return



|***
 *	DES: process casting results from core_ cast2()
 *	USE: 
 *  NOTE: not for public consumption
 ***|
sub check_cast_events(int _checkLoops)
	
	/if (!${_checkLoops}) /varset _checkLoops 1

	/while (${_checkLoops} > 0) {
		/doevents CAST_BEGIN
		/doevents CAST_CANNOTSEE    
		/doevents CAST_COLLAPSE
		/doevents CAST_COMPONENTS
		/doevents CAST_DISTRACTED    
		/doevents CAST_FDFAIL    
		/doevents CAST_FIZZLE  
		/doevents CAST_IMMUNE
		/doevents CAST_INTERRUPTED
		/doevents CAST_NOMOUNT   
		/doevents CAST_NOTARGET
		/doevents CAST_NOTREADY
		/doevents CAST_OUTDOORS
		/doevents CAST_OUTOFMANA  
		/doevents CAST_OUTOFRANGE
		/doevents CAST_RECOVER    
		| /varset CheckResisted 1
		| /doevents CAST_RESISTED    
		| /varset CheckResisted 0  
		| /doevents CAST_RESISTEDYOU    
		/doevents CAST_STANDING 
		/doevents CAST_STUNNED
		/doevents CAST_TAKEHOLD    
		| SDEBUG \atcheck_cast_events\ax(#:${_checkLoops}) castResult:${castResult}
		/varcalc _checkLoops ${_checkLoops}-1
	}
	
/return



|***
 *	DES: swap items. cause fuck MQ2Exchange
 *	USE: /swap "Item Name" slot or /call Bind_swap "Item Name" slot
 *  NOTE: 
 ***|
#Bind swap /swap
sub Bind_swap(string _item, int _toslot)
   
    /declare _swapslot string local NULL
    /declare _pack int local 0
    /declare _slot int local 0

		| in a bag
		/if (${InvSlot[pack${Select[${FindItem[=${_item}].ItemSlot},23,24,25,26,27,28,29,30,31,32]}].Item.Container}) {
			/varset _pack ${Int[${Math.Calc[${FindItem[=${_item}].ItemSlot}-22]}]}
			/varset _slot ${Int[${Math.Calc[${FindItem[=${_item}].ItemSlot2}+1]}]}
			/varset  _swapslot |inbag|pack${_pack}|${_slot}
			/squelch /nomodkey /itemnotify in pack${_pack} ${_slot} leftmouseup
		
		| in a top slot
		} else /if (${FindItem[=${_item}].InvSlot}) {
			/varset _slot ${FindItem[=${_item}].InvSlot}
			/varset  _swapslot |topslot|${_slot}
			/squelch /nomodkey /itemnotify ${_slot} leftmouseup

		| in your ass
		} else /if (!${FindItem[=${_item}].InvSlot}) {
			VOUT ${onotice} Cannot Find ${sep} ${citem}${_item}\ax.
			/return FALSE
		}

    /delay 1s ${Cursor.ID}
    
    | put item in slot
    /nomodkey /shiftkey /itemnotify ${_toslot} leftmouseup

    /delay 1s ${Cursor.ID}

		| place the swaped item back where you found the other one
		/if (${_swapslot.Arg[1,|].Equal[topslot]}) {
			/nomodkey /shiftkey /itemnotify ${_swapslot.Arg[2,|]} leftmouseup
		} else /if (${_swapslot.Arg[1,|].Equal[inbag]}) {
			/nomodkey /shiftkey /itemnotify in ${_swapslot.Arg[2,|]} ${_swapslot.Arg[3,|]} leftmouseup
		}
/return TRUE



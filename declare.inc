|***
 *	declare.inc
 *	All created alias', #binds and / commands
 *	
 *
 ***|

 


|* Created Events ******************************************************************************************************
  
|***
 *	DES: stuf we do cause we are lazy.
 *	USE: 
 *  NOTE: 
 ***|
sub set_alias()

	| /on and /off will remain binds. they need to overwrite the builtin EQ commands
	| /if (!${Bool[${Alias[/on]}]}) 									/noparse /squelch /alias /on       		    	/dgexecute /broadcast_on
	| /if (!${Bool[${Alias[/off]}]}) 									/noparse /squelch /alias /off						    /dgexecute /broadcast_off

	/if (!${Bool[${Alias[/on]}]}) 									/noparse /squelch /alias /on       		    	/broadcast_on
	/if (!${Bool[${Alias[/off]}]}) 									/noparse /squelch /alias /off						    /broadcast_off



  /if (!${Bool[${Alias[/sendpets]}]}) 						/squelch /alias /sendpets										VOUT SendPets: 
	/if (!${Bool[${Alias[/blurb]}]}) 								/squelch /alias /blurb 											VOUT Blurb: 
  /if (!${Bool[${Alias[/echoaugments]}]}) 				/squelch /alias /echoaugments								VOUT Echo Augment file!
                                            			
  /if (!${Bool[${Alias[/buy]}]}) 									/squelch /alias /buy												/echo Buy:
  /if (!${Bool[${Alias[/sell]}]}) 								/squelch /alias /sell												/echo Sell:
                                            			                                           			
	/if (!${Bool[${Alias[/givecoin]}]}) 						/noparse /squelch /alias /givecoin			    VOUT Bribe Time:
                                            			
 	| utility
  /if (!${Bool[${Alias[/autologin]}]}) 						/squelch /alias /autologin									/plugin mq2autologin unload
	
/return



|***
 *	DES: turns everyone off
 *	USE: /off  
 *  NOTE: this will show those whiney bastards
 ***|
#bind broadcast_off /broadcast_off
sub Bind_broadcast_off()
	/declare _count int local 0
	/declare _toonName string local FALSE
	
	/for _count 1 to ${DanNet.PeerCount}
		/varset _toonName ${DanNet.Peers.Arg[${_count},|].Left[1].Upper}${DanNet.Peers.Arg[${_count},|].Right[-1]}
		/if (${_toonName.Equal[${Me.DisplayName}]}) /continue
		/if (!${Spawn[pc ${_toonName}].ID}) /continue
		/sqlite ${dbCore} onoff UPDATE character SET coreAuto="FALSE" WHERE chrID="${MacroQuest.Server}_${Spawn[pc ${_toonName}].Class.ShortName}_${_toonName}"
		/dexecute ${_toonName} /varset coreAuto FALSE
		/dexecute ${_toonName} VOUT /core auto ${odisabled}
		/dexecute ${_toonName} /squelch /target clear
	/next _count
/return



|***
 *	DES: turns everyone on.. 
 *	USE: /on  
 *  NOTE: never judge anothers kink.. never know what you may like??!
 ***|
#bind broadcast_on /broadcast_on
sub Bind_broadcast_on()
	/declare _count int local 0
	/declare _toonName string local FALSE
	
	/for _count 1 to ${DanNet.PeerCount}
		/varset _toonName ${DanNet.Peers.Arg[${_count},|].Left[1].Upper}${DanNet.Peers.Arg[${_count},|].Right[-1]}
		/if (${_toonName.Equal[${Me.DisplayName}]}) /continue
		/if (!${Spawn[pc ${_toonName}].ID}) /continue
		/sqlite ${dbCore} onoff UPDATE character SET coreAuto="TRUE" WHERE chrID="${MacroQuest.Server}_${Spawn[pc ${_toonName}].Class.ShortName}_${_toonName}"
		/dexecute ${_toonName} /varset coreAuto TRUE
		/dexecute ${_toonName} VOUT /core auto ${oenabled}
	/next _count
/return



|***
 *	DES: raid and group invite accept controls
 *	USE: EVENTS
 *  NOTE:
 ***| 
#event Join "#1# invites you to join a #2#.#*#"
sub Event_Join(string line, string _sender, string _groupraid)
	CHECKSAFELIST

	| group invites
	/if (${_groupraid.Equal[group]}) {
		/if (!${swTakeGroupInvite}) {
			/return
		} else /if (${swTakeGroupInvite}) {
			VOUT Accepting invite from ${cpc}${_sender}\ax for ${_groupraid}.
			/invite
		}
	}
	
	| raid invites
	/if (${_groupraid.Equal[raid]}) {
		/delay 1s
		/if (!${swTakeRaidInvite}) {
			/if (${Window[ConfirmationDialogbox].Child[CD_TextOutPut].Text.Find["join a raid"]}) {
				/nomodkey /notify ConfirmationDialogBox No_Button leftmouseup
				/return
			}
			/raiddecline
		} else /if (${swTakeRaidInvite}) {
			VOUT Accepting invite from ${cpc}${_sender}\ax for ${_groupraid}.
			/if (${Window[ConfirmationDialogbox].Child[CD_TextOutPut].Text.Find["join a raid"]}) {
				/nomodkey /notify ConfirmationDialogBox Yes_Button leftmouseup
				/return
			}
			/raidaccept	
		} 		
	}

/return



|***
 *	DES: contains the various BC commands witn NO variables passed
 *	USE: /bc SOMETHINGORANOTHER
 *  NOTE: works off tells or broadcasts
 ***| 
#event broadcast_command "<#1#> #2#"
#event broadcast_command "[#1#(msg)] #2#"
sub Event_broadcast_command(string line, string _sender, string _cmd)

	/if (${_cmd.Equal[stop]}) /call set_stop FALSE
 
	|***
	* DES: casts AA radient cure
	* USE: /rc | /bc rc
	***|
	/if (${_cmd.Equal[rc]}) {
		/if (!${Select[${Me.Class.ShortName},CLR,DRU,SHM,PAL]}) /return FALSE
		/if (!${Me.AltAbilityReady[Radiant Cure]}) /return FALSE
		/stopcast
		/delay 1s !${Me.Casting.ID}
		/if (${validate_cast[FALSE, alt, "Radiant Cure", ${Me.ID}]}) /call core_cast2 "Radiant Cure" alt 0 FALSE
		ECHOCHANNEL Radiant Cure
		/return
	}
		
	|***
	* DES: casts AA purified spirits
	* USE: /purify | /bc purify
	***|
	/if (${_cmd.Equal[purify]}) {
		/if (!${Select[${Me.Class.ShortName},CLR,DRU,SHM]}) /return FALSE
		/if (!${Me.AltAbilityReady[Purified Spirits]}) /return FALSE
		/if (${Me.Casting.ID}) /stopcast
		/delay 1s !${Me.Casting.ID}
		/if (${validate_cast[FALSE, alt, "Purified Spirits", ${Spawn[pc ${_sender}].ID}]}) /call core_cast2 "Purified Spirits" alt ${Spawn[pc ${_sender}].ID} FALSE
		ECHOCHANNEL Purified Spirits: ${Spawn[pc ${_sender}].Name}
		/return
	}
	
/return

	
#event camping "It will take you about 30 seconds to prepare your camp."
sub Event_camping(string line)
	/call clear_combat
	VOUT Current Build ${cinfo}${currentBuild}\ax ${sep} ${cbad}STOPPED\ax for camp.
	/endm
	/squelch /makecamp off
	/squelch /moveto pause
	/squelch /stick off
/return


#event skillup "#*#You have become better at #1#! (#2#)"
sub Event_skillup(string line, string skillname, int skillup)
	VOUT [${cinfo}${skillname}\ax] has increased to (${cnum}${skillup}\ax)! [Max:${cnum}${Skill[${skillname}].SkillCap}\ax]
	/if (${Me.Class.ShortName.Equal[MNK]}) {
		/if (${switchMNKTrainMelee}) /call melee_train_MNK
  }
   
/return


|***
	DES: This is for lazy people like me that have to invite people to raids and dont wanna do the work.
	USE:
***|
#event raidinvite             "#1# tells you#*#'#*#RI#*#'"
#event raidinvite             "#1# tells you#*#'#*#R I#*#'"
#event raidinvite             "#1# tells you#*#'#*#LFG#*#'"
#event raidinvite             "#1# tells you#*#'#*#LFR#*#'"
#event raidinvite             "#1# tells you#*#'#*#invite#*#'"

sub Event_raidinvite(string line, string _sender)
	/if (!${swGiveRaidInvite}) /return
	/if (${Bool[${Raid.Member[${_sender}]}]}) {
		VOUT ${cpc}${_sender}\ax is already in the raid.
		/return
	}
	/if (${lstNoInvite.Find[${_sender}]}) {
		VOUT ${cbad}NO\ax invites for ${cpc}${_sender}\ax.
		/return
	}
	/declare _timeDelay ${Math.Calc[${Math.Rand[40]}+20]}
	/varset _timeDelay ${Int[${_timeDelay}]}
	/delay 1m !${Raid.Locked}
	VOUT Delay: ${cbug}${_timeDelay}\ax \ag- \ax ${cpc}${_sender}\ax requested a raid invite. [Raid: ${If[${Raid.Locked},${cbad}LOCKED\ax,${cgood}UNLOCKED\ax]}]
	/delay 1m !${Raid.Locked}
	/delay ${_timeDelay}
	/raidinvite ${_sender}
/return



|***
*	DES: Song Sets for Bards. 
*	USE: /song [melee|caster|tank|mix]

#event Song "[MQ2] - Song: #1#"
sub Event_Song(string line, string _group)
	/if (${Select[${Me.Class.ShortName},BRD]}) {
		/call mem_song_set ${_group}
	}
/return 
***|


|***
 *	DES: dumps all your augments to a INI file
 *	USE: 
 *  NOTE: 
	
#event echoaugment "[MQ2] - Echo Augment file!"
sub Event_echoaugment()
	/call echo_augments
/return

 ***| 

|***
 *	DES: 
 *	USE: 
 *  NOTE: this is being done away with.. slowly..

#event checkOneOff "[MQ2] - Force OneOff Check!"
sub Event_checkOneOff()
	/call check1offBuffs
/return
 ***| 	


|***
 *	DES: khajiit has wares
 *	USE: gives coins to a NPC..
 *  NOTE: 
 ***| 	
#event bribetime "[MQ2] - Bribe Time: #1# #2# #3#"
sub Event_bribetime(string line, int _amount, int _stacks, string _coin)
	/doevents flush
	/declare _coinID			int local 99

	| validate coin type
	/if (${_coin.Equal[plat]}) {
		/varset _coinID 0
	} else /if (${_coin.Equal[gold]}) {
		/varset _coinID 1
	} else /if (${_coin.Equal[silver]}) {
		/varset _coinID 2
	} else /if (${_coin.Equal[copper]}) {
		/varset _coinID 3
	} else /if (${_coinID} == 99) {
		VOUT ${ocaution} ${sep} Invalid coin type. Try again.
		/return
	}

	/if (!${Window[InventoryWindow].Open}) /invoke ${Window[InventoryWindow].DoOpen}

	/declare _count 			int local 
	/declare _countloop 	int local 1

	:loopgivemorecash
		VOUT Giving ${_coin} stack ${sep} ${_countloop} of ${_stacks}
		/if (!${Window[InventoryWindow].Open}) /return
		/if (!${Target.ID} || ${Target.ID} == (${Me.ID} || ${Pet.ID})) /return
		/delay 2s
		/for _count 1 to ${_stacks}
			AMIDEAD

			/ctrl /notify InventoryWindow IW_Money${_coinID} leftmouseup
			/delay 5
			/click left target 
			/delay 5 
		/next _count 
		/delay 3 
		/notify GiveWnd GVW_Give_Button leftmouseup 

		/delay 5 
		/varset _countloop ${Math.Calc[${_countloop}+1]}
		/if (${_countloop} < ${_amount}) /goto :loopgivemorecash

	VOUT ${onotice} ${sep} Done giving cash for bribes.	How the mighty have fallen.
/return 
 
 
  
|***
 *	DES: Tells for specific things.. i guess?
 *	USE: /t uselesstoon IwantThisBuff
 *  NOTE:
 ***| 	
#event Talk "#1# tells you,#*# '#2#'"

sub Event_Talk(string line, string _sender, string _chatText)
	
	/if (${Bool[${relayTells}]} && !${_sender.Find[Banker]} && !${_sender.Find[${Me}]} && !${_sender.Find[${relayTells}]} && !${_sender.Find[${Me.Pet.DisplayName}]} ) {
		/tell ${relayTells} ${_sender} told me: ${_chatText}
	}

	CHECKSAFELIST

	| Tells for buffs.
	/if (${swBuffTell} && ${Spawn[${_sender}].ID} && ${_sender.NotEqual[${Me.DisplayName}]}) {
    /call cast_buffs_queue "${_sender}" "${_chatText}"
    /return
  }
  
	| Remote kill execution. CHANGE THE CORE.INI
  /if (${Bool[${remoteKillKey}]} && ${_chatText.Equal[${remoteKillKey}]}) {
  	/call set_stop FALSE
  	/call clear_combat
  	VOUT ${owarning} ${sep} Recieved tell from ${cpc}${_sender}\ax to end current macro.
  	/endm
  }
  
  | take a tell to reform your group.
  /if (${_chatText.Equal[formgroup]}) /call set_crew crew1
  /if (${_chatText.Equal[makemeleader]}) /makeleader ${_sender}
  /if (${_chatText.Equal[makememasterlooter]}) /delegategmasterlooter ${_sender}
  
  | raid chat
  /if (${_chatText.Equal[raidinviteme]} && ${Me.DisplayName.Equal[${Raid.Leader}]} && !${Raid.Locked}) /raidinvite ${_sender}
  /if (${_chatText.Equal[makemeraidleader]} && ${Me.DisplayName.Equal[${Raid.Leader}]} && !${Raid.Locked}) /makeraidleader ${_sender}
  /if (${_chatText.Equal[makemedzleader]}) /dzmakeleader ${_sender}
  /if (${_chatText.Equal[makemetaskleader]}) /taskmakeleader ${_sender}
  
/return



|***
 *	DES: This is a cover all for the null slot issue when swapping gear.
 *	USE: NA
 *  NOTE: 
 ***| 	
#event NullSlot "#*#Invalid item slot 'null#*#"
sub Event_NullSlot
	VOUT ${owarning} ${sep} Opening all bags to correct null slot
	/keypress OPEN_INV_BAGS
	/timed 10 /keypress CLOSE_INV_BAGS
/return



|***
 *	DES: catch when a zone happens
 *	USE: 
 *  NOTE: 
 ***| 
| #event Zone "You have entered #*#" 
#event Zone "LOADING, PLEASE WAIT..."
sub Event_Zone(string _line)

	| force a delay while zoning
	/if (${swDelayZoning} && ${Me.Zoning}) {
		:stillfuckingzoningOMGwillthiseverend
		/delay 20s !${Me.Zoning}
		/if (${swDelayZoning} && ${Me.Zoning}) /goto :stillfuckingzoningOMGwillthiseverend
	} else /if (!${Me.Zoning}) {
		/call validate_zone FALSE TRUE
	}
	
	/call timer_update timer_zoned_delay ${tZoned_Delay}
/return FALSE



|***
 *	DES: Buy things from vendors
 *	USE: /echo Buy: 'itemname' ##
 *  NOTE: single quotes
 ***| 
#event buystuff	"[MQ2] Buy: '#1#' #2#"
sub Event_buystuff(string line, string _itemtoBuy, int _buyAmount)
	/if (${_itemtoBuy.Equal[NULL]}) {
		VOUT Need an item to buy, in single qoutes.
	} else /if (${_buyAmount} < 1) {
		VOUT Need a valid quantity. 
	} else {
		/call vendor_buy "${_itemtoBuy}" ${_buyAmount}
	}
/return



|***
 *	DES: Buy things from vendors
 *	USE: /echo Sell: 'itemname' or loot
 *  NOTE: single quotes.. yes, the item name has to be in single quotes
 ***| 
#event sellstuff	"[MQ2] Sell: '#1#'"
sub Event_sellstuff(string line, string _itemtoSell)
	/call vendor_sell "${_itemtoSell}"
/return



|***
 *	DES: rez emote
 *	USE: 
 *  NOTE: 
 ***| 	
#event resurrection_exp_gain "You regain some experience from resurrection."
sub Event_resurrection_exp_gain()
	/delay 2s !${Me.State.Equal[HOVER]}
	/delay 2s !${Me.Hovering}
	/delay 2s !${Me.Dead}
	/varset tmpMode FALSE
	/call set_stop NOECHO
	/call clear_combat
	/varset swAmIDead FALSE
/return



|***
 *	DES: force self item buffing
 *	USE: /ib
 *  NOTE:
 ***| 	
#bind itembuffs /ib
sub Bind_itembuffs()
	/call check_buffs_item
	/if (${Select[${Me.Class.ShortName},ENC]}) /call check_crystals
	/if (${Select[${Me.Class.ShortName},MAG]}) /call Bind_setMAG rods
/return



|***
 *	DES: force self spell buffing
 *	USE: /sb
 *  NOTE:
 ***| 	
#bind selfbuffs /sb
sub Bind_selfbuffs()

	/if (${swBuffGemHold}) {
		/declare _tmpSpell string local FALSE
		/if (${Bool[${Me.Gem[${fix_gem_id[${buffGem}]}]}]})  {
			/varset _tmpSpell ${Me.Gem[${fix_gem_id[${buffGem}]}]}
		}
	}
	
	/call check_buffs_self
	/if (${Select[${Me.Class.ShortName},SHD,PAL]}) /call check_harmonious
	/if (${Select[${Me.Class.ShortName},WIZ,ENC]}) /call check_rune_${Me.Class.ShortName}
	/if (${Bool[${AANow1}]} || ${Bool[${AANow2}]}) /call check_AA_now
	/if (${Bool[${DiscNow1}]} || ${Bool[${DiscNow2}]}) /call check_Disc_now
	/if (${Bool[${Aura1Spell}]} || ${Bool[${Aura2Spell}]}) /call check_aura
	
	/if (${swBuffGemHold}) {
		/if (${Bool[${_tmpSpell}]}) {
			/call mem_spell TRUE "${_tmpSpell}" ${buffGem} FALSE	
			/varset _tmpSpell FALSE
		}	
	}
/return



|***
 *	DES: alternate to /bc stop. self only
 *	USE: /alto
 *  NOTE:
 ***| 	
#bind setstop /alto
sub Bind_setstop()
	/call set_stop FALSE
	/call clear_combat
/return



|***
 *	DES: use primary anchor.
 *	USE: /primary
 *  NOTE:

#bind click_primary_anchor /primary
sub Bind_click_primary_anchor()
	/if (${Me.ItemReady[Primary Anchor Transport Device]}) {
		/if (${Me.Invis}) /makemevisible
		/if (!${swModePersistent}) /call switch_clear_mode
		/call set_stop NOECHO
		/call clear_combat
		/if (${validate_cast[FALSE, item, "Primary Anchor Transport Device", ${Me.ID}]}) /call core_cast2 "Primary Anchor Transport Device" item 0 FALSE
	}
/return
 ***| 


|***
 *	DES: use secondary anchor
 *	USE: /secondary
 *  NOTE:

#bind click_secondary_anchor /secondary
sub Bind_click_secondary_anchor()
	/if (${Me.ItemReady[Secondary Anchor Transport Device]}) {
		/if (${Me.Invis}) /makemevisible
		/if (!${swModePersistent}) /call switch_clear_mode
		/call set_stop NOECHO
		/call clear_combat
		/if (${validate_cast[FALSE, item, "Secondary Anchor Transport Device", ${Me.ID}]}) /call core_cast2 "Secondary Anchor Transport Device" item 0 FALSE
	}
/return
 ***| 


|***
 *	DES: will use which ever anchor is available
 *	USE: /anchor
 *  NOTE:
#bind anchor /anchor
sub Bind_anchor()
	/if (${swBCAnchor}) {
		/dgexecute /prisec_anchor
	} 
	/call Bind_prisec_anchor
/return

|***
 *	DES: sub for /anchor

#bind prisec_anchor /prisec_anchor
sub Bind_prisec_anchor()
	/if (${Me.ItemReady[Primary Anchor Transport Device]}) {
		/call Bind_click_primary_anchor
		/return
	} else /if (${Me.ItemReady[Secondary Anchor Transport Device]}) {
		/call Bind_click_secondary_anchor
		/return			
	}

/return
 ***| 


|***
 *	DES: use Drunkard's Stein
 *	USE: /stein
 *  NOTE:
#bind click_drunkards_stein /stein
sub Bind_click_drunkards_stein()
	/if (!${Me.ItemReady[Drunkard's Stein]}) /return
		/if (${Me.Invis}) /makemevisible
		/if (!${swModePersistent}) /call switch_clear_mode
		/call set_stop NOECHO
		/call clear_combat
	/if (${validate_cast[FALSE, item, "Drunkard's Stein", ${Me.ID}]}) /call core_cast2 "Drunkard's Stein" item 0 FALSE
/return
 ***| 


|***
 *	DES: use Mirror Fragment of Anashti Sul
 *	USE: /mirror
 *  NOTE:
#bind click_mirror_fragment /mirror
sub Bind_click_mirror_fragment()
	/if (!${Me.ItemReady[Mirror Fragment of Anashti Sul]}) /return
		/if (${Me.Invis}) /makemevisible
		/if (!${swModePersistent}) /call switch_clear_mode
		/call set_stop NOECHO
		/call clear_combat
	/if (${validate_cast[FALSE, item, "Mirror Fragment of Anashti Sul", ${Me.ID}]}) /call core_cast2 "Mirror Fragment of Anashti Sul" item 0 FALSE
/return
 ***| 


|***
 *	DES: keeps an eye out for a mob
 *	USE: /watchspawn
 *  NOTE:
 ***| 
#bind watchspawn /watchspawn
sub Bind_watchspawn(string _spawn)
	/if (${Bool[${_spawn}]}) {
		/call sql_update FALSE character watchSpawn "${_spawn}"
	} else /if (!${Bool[${_spawn}]} && ${Target.ID}) {
		/call sql_update FALSE character watchSpawn "${Target.DisplayName}"
	} else /if (!${Bool[${_spawn}]} && !${Target.ID}) {
		VOUT ${sep} ${cinfo}/watchspawn\ax requires a target or mob partial name. 
		VOUT ${sep} NPC and CORPSE is also acceptable.
		/if (!${Bool[${_spawn}]}) /call sql_update FALSE character watchSpawn FALSE
	}
	
	/if (${_spawn.Equal[FALSE]}) /mapshow reset
/return
  
  

|***
 *	DES: enable override
 *	USE: /override []
 *  NOTE: starts/ends overrides
 ***|
#bind override /override
sub Bind_override(string _type, string _verbage, bool _silent)
	/declare _count int local 0
	/declare _bind string local Override
	/declare _table string local character
	/declare _listout string local
	/declare _sep bool local
	/varset _type ${list_isoloate[${lstTypes${_bind}}, ${_type}]}

	| see if its something we can change?
	/for _count 1 to ${lstTypes${_bind}.Count[|]}
		/if (${_type.Equal[${lstTypes${_bind}.Arg[${_count},|].Lower}]}) {
			/call set_switch ${_table} sw${_bind}${_type} ${_verbage}
		}
	/next _count
	
	| spit out the results
	/if (${_silent}) /return
	/if (${swMQClear}) /mqclear
	/for _count 1 to ${lstTypes${_bind}.Count[|]}
		/varset _listout ${_listout} ${If[${_sep},${sep},]} ${If[${sw${_bind}${lstTypes${_bind}.Arg[${_count},|]}},${cgood},${cinfo}]}${lstTypes${_bind}.Arg[${_count},|].Lower}\ax
		/varset _sep TRUE
	/next _count
	VOUT /${_bind.Lower} [${_listout}]	
/return  



|***
 *	DES: set various debuff things
 *	USE: /debuff []
 *  NOTE:
 ***| 	
#bind deBuff /debuff
sub Bind_deBuff(string _type, string _verbage, bool _silent)
	/declare _count int local 0
	/declare _bind string local deBuff
	/declare _table string local character
	/declare _listout string local
	/declare _sep bool local
	/varset _type ${list_isoloate[${lstTypes${_bind}}, ${_type}]}

	| can i do this debuff?
	/if (${Bool[${_type.Equal[mezz]}]}) {
		/if (!${validate_debuff[TRUE, mezz]}) /return
	} else /if (${Bool[${_type.Equal[malo]}]}) {
		/if (!${validate_debuff[TRUE, malo]}) /return
	} else /if (${Bool[${_type.Equal[tash]}]}) {
		/if (!${validate_debuff[TRUE, tash]}) /return
	} else /if (${Bool[${_type.Equal[cripple]}]}) {
		/if (!${validate_debuff[TRUE, cripple]}) /return
	} else /if (${Bool[${_type.Equal[slow]}]}) {
		/if (!${validate_debuff[TRUE, slow]}) /return
	} else /if (${Bool[${_type.Equal[eradicate]}]}) {
		/if (!${validate_debuff[TRUE, eradicate]}) /return
	} else /if (${Bool[${_type.Equal[snare]}]}) {
		/if (!${validate_debuff[TRUE, snare]}) /return
	}	

	| see if its something we can change?
	/for _count 1 to ${lstTypes${_bind}.Count[|]}
		/if (${_type.Equal[${lstTypes${_bind}.Arg[${_count},|].Lower}]}) {
			/call set_switch ${_table} sw${_bind}${_type} ${_verbage}
		}
	/next _count
	
	| spit out the results
	/if (${_silent}) /return
	/for _count 1 to ${lstTypes${_bind}.Count[|]}
		/varset _listout ${_listout} ${If[${_sep},${sep},]} ${If[${sw${_bind}${lstTypes${_bind}.Arg[${_count},|]}},${cgood},${cinfo}]}${lstTypes${_bind}.Arg[${_count},|].Lower}\ax
		/varset _sep TRUE
	/next _count
	VOUT /${_bind.Lower} [${_listout}]	
	
	| clear stickbehind. it will force bad targeting for the mezzer.
	/if (${swStickBehind} && ${swdeBuffMezz}) {
		/call sql_update movement swStickBehind FALSE
		VOUT /${_bind.Lower} behind ${sep} ${swStickBehind}
	}	
/return  



|***
 *	DES: set various heal switches
 *	USE: /heal []
 *  NOTE:
 ***| 	
#bind heal /heal
sub Bind_heal(string _type, string _verbage, bool _silent)
	
	/declare _count int local
	/declare _bind string local Heal
	/declare _table string local character
	/declare _listout string local
	/declare _sep bool local
	/varset _type ${list_isoloate[${lstTypes${_bind}}, ${_type}]}

	| can I do this heal
	/if (${_type.Equal[self]}) {
	} else /if (${_type.Equal[group]}) {
		/if (!${validate_class[TRUE, ${lstClassHeal}]}) /return
	} else /if (${_type.Equal[xtarget]}) {
		/if (!${validate_class[TRUE, ${lstClassHeal}]}) /return
	} else /if (${_type.Equal[pet]}) {
		/if (!${validate_class[TRUE, ${lstClassPet}|CLR|PAL]}) /return
	}
				
	| see if its something we can change?
	/for _count 1 to ${lstTypes${_bind}.Count[|]}
		/if (${_type.Equal[override]}) /continue
		/if (${_type.Equal[${lstTypes${_bind}.Arg[${_count},|].Lower}]}) {
			/call set_switch ${_table} sw${_bind}${_type} ${_verbage}
		}
	/next _count

	| heal xtarget correction(s)
	/if (${_type.Equal[xtarget]}) {
		/if (!${swHealXTarget}) /call clear_xtarget
		/if (!${validate_expansion[TRUE, |16]}) {
			/call set_switch ${_table} swHealXTarget FALSE
			/return
		}
	}

	| heal override
	/if (${_type.Equal[override]}) {
		/if (${Range.Between[0,99:${Int[${_verbage}]}]}) {
			/call sql_update FALSE ${_table} setHealOverride ${_verbage} 
		} else {
			VOUT /${_bind.Lower} override(${cnum}${setHealOverride}\ax) allowable range ${cinfo}0 - 99\ax
			/return
		}
	}

	/if (${_silent}) /return
	/if (${swMQClear}) /mqclear
	/for _count 1 to ${lstTypes${_bind}.Count[|]}
		/if (${lstTypes${_bind}.Arg[${_count},|].Equal[override]}) {
			/varset _listout ${_listout} ${If[${_sep},${sep},]} ${cinfo}${lstTypes${_bind}.Arg[${_count},|].Lower}\ax:${If[${setHealOverride},${cnum}${setHealOverride}\ax,${ooff}]}
			/continue
		}
		/varset _listout ${_listout} ${If[${_sep},${sep},]} ${If[${sw${_bind}${lstTypes${_bind}.Arg[${_count},|]}},${cgood},${cinfo}]}${lstTypes${_bind}.Arg[${_count},|].Lower}\ax
		/varset _sep TRUE
	/next _count
	VOUT /${_bind.Lower} [${_listout}]			
/return
	


|***
 *	DES: set various buff switches
 *	USE: /buff []
 *  NOTE:
 ***| 	
#bind buff /buff
sub Bind_buff(string _type, string _verbage, bool _silent)
	/declare _count int local 0
	/declare _bind string local Buff
	/declare _table string local buff
	/declare _listout string local
	/declare _sep bool local
	/varset _type ${list_isoloate[${lstTypes${_bind}}, ${_type}]}

	| see if its something we can change?
	/for _count 1 to ${lstTypes${_bind}.Count[|]}
		/if (${_type.Equal[${lstTypes${_bind}.Arg[${_count},|].Lower}]}) {
			/if (${_type.Equal[pet]} && !${validate_class[FALSE, ${lstClassPet}]}) /break
			/call set_switch ${_table} sw${_bind}${_type} ${_verbage}
		}
	/next _count
	
	| spit out the results
	/if (${_silent}) /return
	/if (${swMQClear}) /mqclear
	/for _count 1 to ${lstTypes${_bind}.Count[|]}
		/varset _listout ${_listout} ${If[${_sep},${sep},]} ${If[${sw${_bind}${lstTypes${_bind}.Arg[${_count},|]}},${cgood},${cinfo}]}${lstTypes${_bind}.Arg[${_count},|].Lower}\ax
		/varset _sep TRUE
	/next _count
	VOUT /${_bind.Lower} [${_listout}]
/return



|***
 *	DES: cursor control
 *	USE: /cursor []
 *  NOTE: 
 ***| 
#bind cursor /cursor
sub Bind_cursor(string _type, string _verbage)
	/declare _properType string local
	/declare _count int local 0
	/declare _bind string local Cursor
	/declare _passto string local NULL
	/declare _listout string local
	/declare _sep bool local
	
	| set cursot ro auto enable/disable
	/if (${_type.Equal[auto]}) {
		/call set_switch character swAutoCursor ${_verbage}  
		/if (${timer_auto_cursor_pause}) /varset timer_auto_cursor_pause 1

	| pause cursor controlls
	} else /if (${_type.Equal[pause]}) {
		/call timer_update timer_auto_cursor_pause ${If[${Bool[${_verbage}]},${_verbage},30s]}
		VOUT /${_bind.Lower} ${_type.Lower} ${sep} ${cinfo}${If[${Bool[${_verbage}]},${_verbage},30s]}\ax

	| set item to be passed to another
	} else /if (${Cursor.ID} && ${_type.Equal[pass]}) {

		/if (${Bool[${Ini[${INILoot},${Cursor.Name.Left[1]},${Cursor.Name}].Arg[1,|]}]}) {
			/varset _properType ${Ini[${INILoot},${Cursor.Name.Left[1]},${Cursor.Name}].Arg[1,|]}
		} else {
			/varset _properType Keep
		}

		| /varset _properType ${_cursorType.Left[1].Upper}${_cursorType.Right[-1].Lower}
		/if (${Bool[${_verbage}]}) {
			/varset _passto ${_verbage.Left[1].Upper}${_verbage.Right[-1].Lower}
		}

		/ini "${INILoot}" "${Cursor.Name.Left[1]}" "${Cursor.Name}" |${_properType}|${_passto}
		VOUT /${_bind.Lower} ${_type.Lower} ${sep} \a-w${Cursor.Name}\ax=${cinfo}${Ini[${INILoot},${Cursor.Name.Left[1]},${Cursor.Name}]}\ax
		
	| echo what the item is set too
	} else /if (${Cursor.ID} && ${_type.Equal[what]}) {
		VOUT /${_bind.Lower} ${_type.Lower} ${sep} \a-w${Cursor.Name}\ax=${cinfo}${Ini[${INILoot},${Cursor.Name.Left[1]},${Cursor.Name}]}\ax
		/return TRUE

	| remainder of the commands
	} else /if (${Cursor.ID} && ${lstTypes${_bind}.Find[${_type}]}) {
		/varset _properType ${_type.Left[1].Upper}${_type.Right[-1].Lower}
		/if (${Bool[${Ini[${INILoot},${Cursor.Name.Left[1]},${Cursor.Name}].Arg[2,|]}]}) {
			/varset _passto ${Ini[${INILoot},${Cursor.Name.Left[1]},${Cursor.Name}].Arg[2,|]}
		}
		/ini "${INILoot}" "${Cursor.Name.Left[1]}" "${Cursor.Name}" |${_properType}${If[${Bool[${_passto}]},|${Ini[${INILoot},${Cursor.Name.Left[1]},${Cursor.Name}].Arg[2,|]},]}
		VOUT /${_bind.Lower} ${_type.Lower} ${sep} \a-w${Cursor.Name}\ax=${cinfo}${Ini[${INILoot},${Cursor.Name.Left[1]},${Cursor.Name}]}\ax
		/if (${Cursor.ID} && ${_type.Equal[Destroy]}) /destroy

	} 
	
	/if (${swMQClear}) /mqclear
	/for _count 1 to ${lstTypes${_bind}.Count[|]}
		/varset _listout ${_listout} ${If[${_sep},${sep},]} ${cinfo}${lstTypes${_bind}.Arg[${_count},|].Lower}\ax		
		/if (${lstTypes${_bind}.Arg[${_count},|].Equal[auto]}) /varset _listout ${_listout}:${If[${swAutoCursor},${oon},${ooff}]}	
		/varset _sep TRUE
	/next _count
	VOUT /${_bind.Lower} [${_listout}]
/return



|***
 *	DES: set rez controls
 *	USE: /rez []
 *  NOTE: people should really stop dying
 ***| 	
#bind rez /rez
sub Bind_rez(string _type, string _verbage, bool _silent)
	/declare _count int local 0
	/declare _bind string local Rez
	/declare _table string local character
	/declare _listout string local
	/declare _sep bool local
	/varset _type ${list_isoloate[${lstTypes${_bind}}, ${_type}]}
	
	| set rez range 	
	/if (${_type.Equal[range]}) {
		/if (${Range.Between[1,110:${Int[${_verbage}]}]}) {
			/call sql_update FALSE ${_table} setMaxRezRange ${_verbage}
		} else {
			VOUT /${_bind.Lower} range(${cnum}${setMaxRezRange}\ax) allowable range is ${cinfo}1 - 110\ax
			/return
		}	
	
	| set rez accept percent	
	} else /if (${_type.Equal[pct]}) {
		/if (${Range.Between[10,96:${Int[${_verbage}]}]}) {
			/if (${lstTypesRezPct.Find[${_verbage}]}) /call sql_update FALSE ${_table} setMinRezPct ${_verbage}
		}
		/varset _sep FALSE
		/for _count 1 to ${lstTypesRezPct.Count[|]}
			/varset _listout ${_listout} ${If[${_sep},${sep},]} ${If[${setMinRezPct} == ${lstTypesRezPct.Arg[${_count},|]},${cgood},${cinfo}]}${lstTypesRezPct.Arg[${_count},|].Lower}\ax   
			/varset _sep TRUE
		/next _count
		VOUT /${_bind.Lower} pct [${_listout}]
		/return
		
	| all the others				
	} else /if (${lstTypes${_bind}.Find[${_type}]}) {
		/call set_switch ${_table} sw${_bind}${_type} ${_verbage}

	}

	/if (${_silent}) /return
	/varset _sep FALSE
	/if (${swMQClear}) /mqclear
	/for _count 1 to ${lstTypes${_bind}.Count[|]}
		/varset _listout ${_listout} ${If[${_sep},${sep},]} ${cinfo}${lstTypes${_bind}.Arg[${_count},|].Lower}\ax		
		/if (${lstTypes${_bind}.Arg[${_count},|].Equal[pct]}) /varset _listout ${_listout}:${cnum}${setMinRezPct}\ax
		/if (${lstTypes${_bind}.Arg[${_count},|].Equal[range]}) /varset _listout ${_listout}:${cnum}${setMaxRezRange}\ax
		/if (${lstTypes${_bind}.Arg[${_count},|].Equal[combat]}) /varset _listout ${_listout}:${If[${swRezCombat},${oon},${ooff}]}
		/if (${lstTypes${_bind}.Arg[${_count},|].Equal[everyone]}) /varset _listout ${_listout}:${If[${swRezEveryone},${oon},${ooff}]}
		/if (${lstTypes${_bind}.Arg[${_count},|].Equal[give]}) /varset _listout ${_listout}:${If[${swRezGive},${oon},${ooff}]}
		/if (${lstTypes${_bind}.Arg[${_count},|].Equal[take]}) /varset _listout ${_listout}:${If[${swRezTake},${oon},${ooff}]}
		/if (${lstTypes${_bind}.Arg[${_count},|].Equal[token]}) /varset _listout ${_listout}:${If[${swRezToken},${oon},${ooff}]}
		/varset _sep TRUE
	/next _count
	VOUT /${_bind.Lower} [${_listout}]	
/return



|***
 *	DES: list control
 *	USE: /lc []
 *  NOTE: 
 ***| 
#bind listControl /lc
sub Bind_listControl(string _type, string _verbage, bool _silent)
	/declare _properType string 	local
	/declare _count int local 0
	/declare _bind string local LC
	/declare _listout string local
	/declare _sep bool local	
	
	/varset _type ${list_isoloate[${lstTypes${_bind}}, ${_type}]}

	| make sure were on/in the right zone
	/call validate_zone FALSE FALSE
	
	| safezone toggle
	/if (${_type.Equal[SafeZone]}) {
		/if (${SafeZone} || ${_verbage.Equal[FALSE]}) {
			/sqlite ${dbCore} dummy UPDATE zone SET SafeZone="FALSE" WHERE ShortName="${Zone.ShortName.Lower}"
			/varset SafeZone FALSE
		} else /if (!${SafeZone} || ${_verbage.Equal[TRUE]}) {
			/sqlite ${dbCore} dummy UPDATE zone SET SafeZone="TRUE" WHERE ShortName="${Zone.ShortName.Lower}"
			/varset SafeZone TRUE
		}
		VSQL \awzone\ax::\aw${Zone.ShortName.Lower}\ax::\awSafeZone\ax::${If[${SafeZone},${oon},${ooff}]}
		/dgaexecute /admin zonereset
		/return TRUE
	
	| disable all commands but safezone while in a safezone
	} else /if (${SafeZone} && ${_type.NotEqual[SafeZone]}) {
		VOUT ${onotice} ${sep} /lc is disabled while in a Safe Zone.
		/return FALSE
	

	| remove a mob from the current zone
	} else /if (${_type.Equal[remove]}) {
		VOUT /${_bind.Lower} ${_type.Lower} ${sep} ${ctar}${Target.DisplayName}\ax

		/declare _mobName 			string local |${Target.DisplayName}
		/declare _listName 			string local	
		/declare _INIlist				string local ${ZoneFileList}
		/declare _a 						int local
		/declare _b 						int local
		/declare _strLeft 			string local
		/declare _strRight 			string local
		/declare _found					bool local
		
		/for _count 1 to ${ZoneFileList.Count[|]}
			/if (${ZoneFileList.Arg[${_count},|].Equal[SafeZone]}) /continue
			/varset _strLeft FALSE
			/varset _strRight FALSE
			
			/if (${${ZoneFileList.Arg[${_count},|]}.Find[${_mobName}]}) {
				/varset _found TRUE
				VSQL \awzone\ax::\aw${Zone.ShortName.Lower}\ax::\aw${ZoneFileList.Arg[${_count},|]}\ax::\ar${Target.DisplayName}\ax
				/varcalc _a ${${ZoneFileList.Arg[${_count},|]}.Find[${_mobName}]}-1
				/varcalc _b ${${ZoneFileList.Arg[${_count},|]}.Length}-${_mobName.Length}-${_a}
				/if (${_a} > 0) /varset _strLeft ${${ZoneFileList.Arg[${_count},|]}.Left[${_a}]}
				/if (${_a} >= 0) /varset _strRight ${If[${Select[${_b},NULL,FALSE,""]},,${${ZoneFileList.Arg[${_count},|]}.Right[${_b}]}]} 
				/sqlite ${dbCore} dummy UPDATE zone SET ${ZoneFileList.Arg[${_count},|]}="${If[${Bool[${_strLeft}]},${_strLeft},]}${If[${Bool[${_strRight}]},${_strRight},]}" WHERE ShortName="${Zone.ShortName.Lower}"
			}
		/next _count	
		/if (${_found}) {
			/call Bind_admin zonereset
			/dgexecute /admin zonereset
		}
		/return TRUE
		
	} else /if (${lstTypes${_bind}.Find[${_type}]}) {
		
		/sql ${dbCore} dummy SELECT * FROM zone WHERE ShortName="${Zone.ShortName.Lower}"

		/if (${sqlite.Result[dummy 1 ${_type}List].Find[${Target.DisplayName}]}) {
			VSQL \awzone\ax::\aw${Zone.ShortName.Lower}\ax::\aw${_type}List\ax::\at${Target.DisplayName}\ax
			/return TRUE

		} else /if (!${sqlite.Result[dummy 1 ${_type}List].Find[${Target.DisplayName}]}) {

			| add to list
			/if (${Bool[${sqlite.Result[dummy 1 ${_type}List]}]}) {
				/sqlite ${dbCore} dummy UPDATE zone SET ${_type}List="${${_type}List}|${Target.DisplayName}" WHERE ShortName="${Zone.ShortName.Lower}"

			| create the list
			} else /if (!${Bool[${sqlite.Result[dummy 1 ${_type}List]}]}) {
				/sqlite ${dbCore} dummy UPDATE zone SET ${_type}List="|${Target.DisplayName}" WHERE ShortName="${Zone.ShortName.Lower}"
			}

			VSQL \awzone\ax::\aw${Zone.ShortName.Lower}\ax::\aw${_type}List\ax::\at${Target.DisplayName}\ax
			/dgaexecute /admin zonereset
			/return TRUE
		}


	} 
	
	
	/if (${_silent}) /return
	/if (${swMQClear}) /mqclear
	
	| require a target. one that is not a PC
	/if (!${Target.ID} && ${_type.NotEqual[SafeZone]}) {
		/call echos needtarget 0 0 0 0
	}	
	
	/for _count 1 to ${lstTypes${_bind}.Count[|]}
		/varset _listout ${_listout} ${If[${_sep},${sep},]} ${cinfo}${lstTypes${_bind}.Arg[${_count},|].Lower}\ax		
		/varset _sep TRUE
	/next _count
	VOUT /${_bind.Lower} [${_listout}]

/return



|***
 *	DES: pet control
 *	USE: /pc []
 *  NOTE:
 ***|
#bind petControl /pc
sub Bind_petControl(string _type, string _verbage, bool _silent)
	/declare _count int local 0
	/declare _bind string local PC
	/declare _table string local pet
	/declare _listout string local
	/declare _sep bool local
	/varset _type ${list_isoloate[${lstTypes${_bind}}, ${_type}]}


	| build and buff
	/if (${Bool[${_type.Find[build]}]}) {
		/if (!${validate_class[TRUE, ${lstClassPet}]}) /return
		/call set_switch ${_table} swPet TRUE
		/call check_pet
		/if (${Me.Pet.ID}) /call check_buffs_pet
		/return

	| pet/engage#
	} else /if (${_type.Equal[pet]}) {
		/if (!${validate_class[TRUE, ${lstClassPet}]}) /return
		/if (!${Bool[${_verbage}]}) {
			/call set_switch ${_table} swPet ${_verbage}
			/if (!${swPet} && ${Pet.ID}) {
				/if (${Me.AltAbilityReady[Suspended Minion]}) {
					/if (${validate_cast[FALSE, alt, "Suspended Minion", ${Me.ID}]}) /call core_cast2 "Suspended Minion" alt 0 FALSE
				} else /if (${Me.AltAbilityReady[Companion's Suspension]}) {
					/if (${validate_cast[FALSE, alt, "Companion's Suspension", ${Me.ID}]}) /call core_cast2 "Companion's Suspension" alt 0 FALSE
				} else {
					/pet leave
				}
			}
		} else /if (${Range.Between[1,99:${Int[${_verbage}]}]}) {
			/call sql_update FALSE ${_table} setPetEngagePct ${_verbage}
		} else /if (!${Bool[${_pcNum}]} || !${Range.Between[1,99:${Int[${_verbage}]}]}) {
			VOUT /${_bind.Lower} pet(${cnum}${setPetEngagePct}\ax) valid range ${cinfo}1 - 99\ax
			/return
		}
		
	| swarm/engage#
	} else /if (${_type.Equal[swarm]}) {
		/if (!${Bool[${_verbage}]}) {
			/call set_switch ${_table} swSwarm ${_verbage}
		} else /if (${Bool[${_verbage}]} && ${Range.Between[1,99:${Int[${_verbage}]}]}) {			
			/call sql_update FALSE ${_table} setSwarmEngagePct ${_verbage}
		} else /if (!${Bool[${_verbage}]} || !${Range.Between[1,99:${Int[${_verbage}]}]}) {
			VOUT /${_bind.Lower} swarm(${cnum}${setSwarmEngagePct}\ax) valid range ${cinfo}1 - 99\ax
			/return
		}
		
	| pc tank  This is not working atm. sorry...
	} else /if (${_type.Equal[tank]}) {
		/return
		/if (!${validate_class[TRUE, ${lstClassPet}]}) /return
		/call set_switch ${_table} swPettank ${_verbage}
		/call echos switch "Pet Tank" swPettank	0 0
		/return
		
	| pc type
	} else /if (${_type.Equal[type]}) {
		/if (!${validate_class[TRUE, ${lstClassPet}]}) /return
		/call sql_update TRUE pet setPetType "${_verbage}"
		
	| pc bag ##
	} else /if (${_type.Equal[bag]}) {
		/if (!${validate_class[TRUE, ${lstClassPet}]}) /return
		/if (${Range.Between[1,10:${Int[${_verbage}]}]}) {
			/call sql_update FALSE ${_table} setPetBagSlot ${_verbage}
		} else {
			VOUT /${_bind.Lower} bag(${cnum}${setPetBagSlot}\ax] valid range range ${cinfo}1 - 10\ax
			/return
		}
		
	| swarm pets on a DS mob	
	} else /if (${_type.Equal[dsswarm]}) {
		/call set_switch ${_table} swNoSwarmonDS ${_verbage}
				
		

	}
	
	/if (${_silent}) /return
	/if (${swMQClear}) /mqclear
	/for _count 1 to ${lstTypes${_bind}.Count[|]}
		/varset _listout ${_listout} ${If[${_sep},${sep},]} ${cinfo}${lstTypes${_bind}.Arg[${_count},|].Lower}\ax		
		/if (${lstTypes${_bind}.Arg[${_count},|].Equal[dsswarm]}) /varset _listout ${_listout}:${If[${swNoSwarmonDS},${oon},${ooff}]}
		/if (${lstTypes${_bind}.Arg[${_count},|].Equal[pet]}) /varset _listout ${_listout}:${If[${swPet},${cnum}${setPetEngagePct}\ax,${cbad}${setPetEngagePct}\ax]}
		/if (${lstTypes${_bind}.Arg[${_count},|].Equal[swarm]}) /varset _listout ${_listout}:${If[${swSwarm},${cnum}${setSwarmEngagePct}\ax,${cbad}${setSwarmEngagePct}\ax]}
		/if (${lstTypes${_bind}.Arg[${_count},|].Equal[type]}) /varset _listout ${_listout}:${If[${Bool[${setPetType}]},${cnum}${setPetType}\ax,${ooff}]}
		/if (${Me.Class.PetClass} || ${Select[${Me.Class.ShortName},ENC,DRU,SHM,SHD,NEC]}) {
			/if (${lstTypes${_bind}.Arg[${_count},|].Equal[bag]}) /varset _listout ${_listout}:${If[${Bool[${setPetBagSlot}]},${cnum}${setPetBagSlot}\ax,${ooff}]}
		}
		/varset _sep TRUE
	/next _count
	VOUT /${_bind.Lower} [${_listout}]
/return



|***
 *	DES: sets controlls for /tie
 *	USE: /tc [name|time|distance]
 *  NOTE:
 ***| 
#bind setTieControl /tc
sub Bind_setTieControl(string _type, string _verbage, bool _silent)
	/declare _count int local 0
	/declare _bind string local TC
	/declare _table string local movement
	/declare _listout string local
	/declare _sep bool local

	| set time to toon name
	/if (${_type.Equal[name]}) {
		/if (${setTieToon.NotEqual[${_verbage}]} && ${_verbage.NotEqual[${Me.DisplayName}]}) {	
			/call sql_update FALSE ${_table} setTieToon ${_verbage}
		}

	| forced tied
	| we like it rough
	} else /if (${_type.Equal[lock]}) {
		/call set_switch ${_table} swTieLock ${_verbage}
		/if (${swTieLock}) /call set_switch ${_table} setTieMode Stick

	| set tie time
	} else /if (${_type.Equal[time]}) {
		/if (${Range.Between[5,50:${Int[${_verbage}]}]}) {
			/call sql_update FALSE timer tCheck_Tie ${_verbage} 
		} else {
			VOUT /${_bind.Lower} time(${cnum}${tCheck_Tie}\ax) allowable range is ${cinfo}5 - 50\ax
			/return
		}
		
	| set tie mode
	} else /if (${_type.Equal[mode]}) {
		/if (${_verbage.Equal[stick]}) {
			/call sql_update FALSE ${_table} setTieMode Stick
		} else /if (${_verbage.Equal[nav]}) {
			/call sql_update FALSE ${_table} setTieMode Nav
		} 
		/declare _tcModeType string local |stick|nav
		/for _count 1 to ${_tcModeType.Count[|]}
			/varset _listout ${_listout} ${If[${_sep},${sep},]}
			/if (${_tcModeType.Arg[${_count},|].Equal[stick]}) /varset _listout ${_listout}${If[${setTieMode.Equal[stick]},${cgood}stick\ax,${cinfo}stick\ax]}
			/if (${_tcModeType.Arg[${_count},|].Equal[nav]}) /varset _listout ${_listout}${If[${setTieMode.Equal[nav]},${cgood}nav\ax,${cinfo}nav\ax]}
			/varset _sep TRUE
		/next _count
		VOUT /${_bind.Lower} mode [${_listout}]
		/return
		
	| set tie distance		
	} else /if (${_type.Equal[distance]}) {
		/if (${Range.Between[1,99:${Int[${_verbage}]}]}) {
			/call sql_update FALSE ${_table} setTieDistance ${_verbage} 
		} else {
			VOUT /${_bind.Lower} distance(${cnum}${setTieDistance}\ax) allowable range is ${cinfo}1 - 99\ax
			/return
		}
	}

	/if (${_silent}) /return
	/if (${swMQClear}) /mqclear
	/for _count 1 to ${lstTypes${_bind}.Count[|]}
		/varset _listout ${_listout} ${If[${_sep},${sep},]} ${cinfo}${lstTypes${_bind}.Arg[${_count},|].Lower}\ax		
		/if (${lstTypes${_bind}.Arg[${_count},|].Equal[name]}) /varset _listout ${_listout}:${cnum}${If[${Bool[${setTieToon}]},${setTieToon},${ooff}]}\ax
		/if (${lstTypes${_bind}.Arg[${_count},|].Equal[time]}) /varset _listout ${_listout}:${cnum}${tCheck_Tie}\ax
		/if (${lstTypes${_bind}.Arg[${_count},|].Equal[distance]}) /varset _listout ${_listout}:${cnum}${setTieDistance}\ax
		/if (${lstTypes${_bind}.Arg[${_count},|].Equal[lock]}) /varset _listout ${_listout}:${If[${swTieLock},${oon},${ooff}]}
		/if (${lstTypes${_bind}.Arg[${_count},|].Equal[mode]}) /varset _listout ${_listout}:${cnum}${setTieMode}\ax
		/varset _sep TRUE
	/next _count
	VOUT /${_bind.Lower} [${_listout}]
/return TRUE



|***
 *	DES: enable/disable Tie
 *	USE: /tie [switch|on|off]
 *  NOTE:
 ***| 	
#bind setTie /tie
sub Bind_setTie(string _type)

	| tie toon not in zone?
	/if (!${Bool[${Spawn[pc ${setTieToon}].ID}]}) {
		/varset swSetTie FALSE 
		/return FALSE
	}

	/if (${_type.Equal[on]}) {
		/if (${swSetTie}) /return
		/if (${Bool[${Spawn[pc ${setTieToon}].ID}]}) /varset swSetTie TRUE
	} else /if (${_type.Equal[off]}) {
		/if (!${swSetTie}) /return
		/if (${Bool[${Spawn[pc ${setTieToon}].ID}]}) /varset swSetTie FALSE
	} else {
		/if (${Bool[${Spawn[pc ${setTieToon}].ID}]}) /varset swSetTie ${If[${swSetTie},FALSE,TRUE]}  
	}

	VOUT /tie ${sep} ${If[${swSetTie},${oenabled},${odisabled}]} ${If[${swTieLock},${sep} (${cgood}Locked\ax),]}
/return TRUE



|***
 *	DES: Burn control
 *	USE: /burn []
 *  NOTE:
 ***| 	
#bind burn /burn
sub Bind_burn(string _type, string _verbage, bool _silent)
	
	/declare _count int local 0
	/declare _bind string local Burn
	/declare _listout string local
	/declare _sep bool local FALSE

	| Burn Automatic on/off
	/if (${Bool[${_type.Find[auto]}]}) {
		/call set_switch character swBurnAuto ${_verbage}  

	| Burn in Raids
	} else /if (${Bool[${_type.Find[raid]}]}) {
		/call set_switch character swBurnRaid ${_verbage}  

	| Mob count #
	} else /if (${_type.Equal[count]}) {
	 	/if (${Range.Between[1,99:${Int[${_verbage}]}]}) {
			/call sql_update FALSE character setBurnCount ${_verbage} 
		} else {
			VOUT /${_bind.Lower} count(${cnum}${setBurnCount}\ax) # of mobs in range ${cinfo}1 - 99\ax
			/return
		}

	| Burn Engage at %
	} else /if (${_type.Equal[engage]}) {
	 	/if (${Range.Between[1,99:${Int[${_verbage}]}]}) {
			/call sql_update FALSE character setBurnPct ${_verbage} 
		} else {
			VOUT /${_bind.Lower} engage(${cnum}${setBurnPct}\ax] % of mobs health to engage burn ${cinfo}1 - 99\ax
			/return
		}

	| force constant burning
	} else /if (${_type.Equal[force]}) {
		/call set_switch character swBurnForce ${_verbage}

	} 
	
	/if (${_silent}) /return
	/if (${swMQClear}) /mqclear
	/for _count 1 to ${lstTypes${_bind}.Count[|]}
		/varset _listout ${_listout} ${If[${_sep},${sep},]} ${cinfo}${lstTypes${_bind}.Arg[${_count},|]}\ax
		/if (${lstTypes${_bind}.Arg[${_count},|].Equal[auto]}) /varset _listout ${_listout}:${cnum}${If[${Bool[${swBurnAuto}]},${oon},${ooff}]}\ax
		/if (${lstTypes${_bind}.Arg[${_count},|].Equal[force]}) /varset _listout ${_listout}:${cnum}${If[${Bool[${swBurnForce}]},${oon},${ooff}]}\ax
		/if (${lstTypes${_bind}.Arg[${_count},|].Equal[engage]}) /varset _listout ${_listout}:${cnum}${setBurnPct}\ax
		/if (${lstTypes${_bind}.Arg[${_count},|].Equal[count]}) /varset _listout ${_listout}:${cnum}${setBurnCount}\ax
		/if (${lstTypes${_bind}.Arg[${_count},|].Equal[raid]}) /varset _listout ${_listout}:${cnum}${If[${Bool[${swBurnRaid}]},${oon},${ooff}]}\ax
		/varset _sep TRUE
	/next _count
	VOUT /${_bind.Lower} [${_listout}]
/return



|***
 *	DES: core controls
 *	USE: /core [] [TEXT]
 *  NOTE:
 ***|
#bind Core /core
sub Bind_Core(string _type, string _verbage, string _verbage2, bool _silent)

	/declare _count int local 0
	/declare _bind string local Core
	/declare _listout string local
	/declare _sep bool local FALSE

	| core auto/manual
	/if (${_type.Equal[auto]}) {
		/call set_switch character coreAuto ${_verbage}
		/if (!${coreAuto}) { 
			/call set_stop NOECHO
			/call clear_combat
			/if (${swHomeSet}) {
				/call Bind_setMovementControls home FALSE
			}
		}
		VOUT /${_bind.Lower} auto ${sep} ${If[${coreAuto},${oenabled},${odisabled}]}
		/return
		
	| enable wasiting fireworks for those bastards with too much AA on their hands
	} else /if (${_type.Equal[fireworks]}) {
		/call set_switch character swAAFireworks ${_verbage}  
		/if (!${swAAFireworks}) /call timer_update timer_Waste_Fireworks 1
		
	| take raid invites
	} else /if (${_type.Equal[takeraidinvite]}) {
		/call set_switch character swTakeRaidInvite ${_verbage} 

	| take group invites
	} else /if (${_type.Equal[takegroupinvite]}) {
		/call set_switch character swTakeGroupInvite ${_verbage} 
	
	| invite people to your raid
	} else /if (${_type.Equal[giveraidinvite]}) {
		/call set_switch character swGiveRaidInvite ${_verbage} 

	| exp max level
	} else /if (${_type.Equal[expmaintain]}) {
		/if (${Range.Between[1,99:${Int[${_verbage}]}]}) {
			/call sql_update FALSE character setLvlPct ${_verbage} 
		} else {
			VOUT /${_bind.Lower} expmaintain(${cnum}${setLvlPct}\ax) allowable range ${cinfo}1 - 99\ax
			/return
		}	

	| exp maintain level
	} else /if (${_type.Equal[explevel]}) {
		/if (${Range.Between[1,110:${Int[${_verbage}]}]}) {
			/call sql_update FALSE character setLvlMax ${_verbage} 
		} else {
			VOUT /${_bind.Lower} explevel(${cnum}${setLvlMax}\ax) allowable range ${cinfo}1 - 110\ax
			/return
		}	

	| exp auto adjust
 	} else /if (${_type.Equal[expadjust]}) {
		/call set_switch character swCorrectEXPAA ${_verbage} 

	| check in game cash
	} else /if (${_type.Equal[checkcash]}) {
		/if (${Window[MKPW_ClaimWindow].Child[MKPW_ClaimClickHereBtn].Text.EqualCS[Claimed]}) {
			VOUT No cash to collect, current: ${cnum}${Window[MarketplaceWnd].Child[MKPW_AvailableFundsUpper].Text}\ax
			/return
		} else {
			/call check_free_ingame_cash
		}		

	| enable/disable using events.inc file
	} else /if (${_type.Equal[events]}) {
		/call set_switch character swEventsInc ${_verbage}


				
	}

	/if (${_silent}) /return
	/if (${swMQClear}) /mqclear	
	/for _count 1 to ${lstTypes${_bind}.Count[|]}
		/varset _listout ${_listout} ${If[${_sep},${sep},]} ${cinfo}${lstTypes${_bind}.Arg[${_count},|]}\ax
		/if (${lstTypes${_bind}.Arg[${_count},|].Equal[auto]}) /varset _listout ${_listout}:${If[${coreAuto},${oon},${ooff}]}
		/if (${lstTypes${_bind}.Arg[${_count},|].Equal[events]}) /varset _listout ${_listout}:${If[${swEventsInc},${oon},${ooff}]}
		/if (${lstTypes${_bind}.Arg[${_count},|].Equal[expadjust]}) /varset _listout ${_listout}:${If[${swCorrectEXPAA},${oon},${ooff}]}
		/if (${lstTypes${_bind}.Arg[${_count},|].Equal[explevel]}) /varset _listout ${_listout}:${cnum}${setLvlMax}\ax
		/if (${lstTypes${_bind}.Arg[${_count},|].Equal[expmaintain]}) /varset _listout ${_listout}:${cnum}${setLvlPct}\ax
		/if (${lstTypes${_bind}.Arg[${_count},|].Equal[fireworks]}) /varset _listout ${_listout}:${If[${swAAFireworks},${oon},${ooff}]}
		/if (${lstTypes${_bind}.Arg[${_count},|].Equal[giveraidinvite]}) /varset _listout ${_listout}:${If[${swGiveRaidInvite},${oon},${ooff}]}
		/if (${lstTypes${_bind}.Arg[${_count},|].Equal[takegroupinvite]}) /varset _listout ${_listout}:${If[${swTakeGroupInvite},${oon},${ooff}]}
		/if (${lstTypes${_bind}.Arg[${_count},|].Equal[takeraidinvite]}) /varset _listout ${_listout}:${If[${swTakeRaidInvite},${oon},${ooff}]}
		/varset _sep TRUE
	/next _count
	VOUT /${_bind.Lower} [${_listout}]
/return




|***
 *	DES: set on/off switches
 *	USE: /onoff []
 *  NOTE: enable/disable many switches
 ***|
#bind onoff /onoff
sub Bind_onoff(string _type, string _verbage, bool _silent)
	
	/declare _count int local 0
	/declare _listout string local
	/declare _sep bool local FALSE
	/declare _bind string local OnOff
	
	/if (${lstTypes${_bind}.Find[${_type}]}) {
		/if (${Defined[switch${_type.Left[1].Upper}${_type.Right[-1].Lower}]}) {
			/call set_switch item switch${_type.Left[1].Upper}${_type.Right[-1].Lower} ${_verbage}  
		} 
	}
	
	/if (${_silent}) /return
	/if (${swMQClear}) /mqclear
	/for _count 1 to ${lstTypes${_bind}.Count[|]}
		/varset _listout ${_listout} ${If[${_sep},${sep},]} ${cinfo}${lstTypes${_bind}.Arg[${_count},|]}\ax
		/if (${lstTypes${_bind}.Arg[${_count},|].Equal[horn]}) /varset _listout ${_listout}:${If[${switchHorn},${oon},${ooff}]}	
		/if (${lstTypes${_bind}.Arg[${_count},|].Equal[shrink]}) /varset _listout ${_listout}:${If[${switchShrink},${oon},${ooff}]}

		/varset _sep TRUE
	/next _count
	VOUT /${_bind.Lower} [${_listout}]
/return  



|***
 *	DES: tradeskill controls
 *	USE: /ts [movebank|movehouse]
 *  NOTE:
 ***|
#bind ts /ts
sub Bind_ts(string _tsType)

	/declare _listout string local
	/declare _sep bool local
	/declare _count int local
	/declare _bind string local ts

	| tradeskill commands
	/if (${_tsType.Equal[movebank]}) {
		/declare _slot 				int local
		/declare _bagslot 		int local
		/for _slot 23 to 26
			/if (${InvSlot[${_slot}].Item.Container}) {
				/for _bagslot 1 to ${InvSlot[${_slot}].Item.Container}
					/if (${Me.Inventory[${_slot}].Item[${_bagslot}].StackSize} > 100 && ${Window[BigBankWnd].Open}) {
						VOUT  Moving ${sep} ${cinfo}${Me.Inventory[${_slot}].Item[${_bagslot}].Name}\ax ${sep} [${ctar}${FindItemCount[${Me.Inventory[${_slot}].Item[${_bagslot}].Name}]}\ax]
						/nomodkey /shift /itemnotify in Pack${Math.Calc[${FindItem[=${Me.Inventory[${_slot}].Item[${_bagslot}].Name}].ItemSlot}-22]} ${Math.Calc[${FindItem[=${Me.Inventory[${_slot}].Item[${_bagslot}].Name}].ItemSlot2}+1]} leftmouseup
						/delay 5 ${Cursor.ID}
						/notify BigBankWnd bigb_autobutton leftmouseup
						/delay 5 !${Cursor.ID}
					}
				/next _bagslot
			}
		/next _slot

	} else /if (${_tsType.Equal[movehouse]}) {
		/if (${Window[RealEstateItemsWnd].Open}) {
			/declare _slot 				int local
			/declare _bagslot 		int local
			/for _slot 23 to 26
				/if (${InvSlot[${_slot}].Item.Container}) {
					/for _bagslot 1 to ${InvSlot[${_slot}].Item.Container}
						/nomodkey /itemnotify "${Me.Inventory[${_slot}].Name}" rightmouseup
						/if (${Me.Inventory[${_slot}].Item[${_bagslot}].StackSize} > 100 && ${Window[RealEstateItemsWnd].Open}) {
							VOUT  Moving ${sep} ${cinfo}${Me.Inventory[${_slot}].Item[${_bagslot}].Name}\ax ${sep} [${ctar}${FindItemCount[${Me.Inventory[${_slot}].Item[${_bagslot}].Name}]}\ax]
							| /nomodkey /itemnotify in Pack${Math.Calc[${FindItem[=${Me.Inventory[${_slot}].Item[${_bagslot}].Name}].ItemSlot}-22]} ${Math.Calc[${FindItem[=${Me.Inventory[${_slot}].Item[${_bagslot}].Name}].ItemSlot2}+1]} 
							/click left ${Me.Inventory[${_slot}].Item[${_bagslot}].Name} 

							/nomodkey /shift /notify RealEstateItemsWnd REIW_Move_Closet_Button leftmouseup
							
						}
					/next _bagslot
				}
			/next _slot
		} else {
			VOUT Open Realestate Items Window
		}

	} else {
		/if (${swMQClear}) /mqclear

		/for _count 1 to ${${_bind}Type.Count[|]}
			/varset _listout ${_listout} ${If[${_sep},${sep},]} ${cinfo}${${_bind}Type.Arg[${_count},|]}\ax		
			/varset _sep TRUE
		/next _count
		VOUT /${_bind.Lower} [${_listout}]
	} 
/return



|***
 *	DES: sets the assist
 *	USE: /setassist [G|1|2|3|F|MA1|MA2]
 *  NOTE: 
 ***| 	
#bind setAssist /setassist
sub Bind_setAssist(string _verbage, string _name)
	/call validate_assist FALSE ${_verbage} ${If[${Bool[${_name}]},${_name},]}
/return



|***
 *	DES: combat controls
 *	USE: /cc []
 *  NOTE:
 ***|
#bind setCombatControl /cc
sub Bind_setCombatControl(string _type, string _verbage, bool _silent)
	
	/declare _count int local 0
	/declare _bind string local CC
	/declare _listout string local
	/declare _sep bool local FALSE

	| enable / disable forced assist
	/if  (${_type.Equal[forceassist]}) {
		/call validate_assist FALSE force
		VOUT /${_bind.Lower} forceassist ${sep} ${If[${swAssistF},${oenabled},${odisabled}]}

	| force manual combat routines
	} else /if (${_type.Equal[force]}) {
		/if (${Bool[${_verbage}]}) {
			/if (${Target.ID} && (${Target.ID} != ${Me.ID}) && (${Target.ID} != ${Me.Pet.ID})) {

				| engage base combat
				/if (${_verbage.Equal[combat]}) {
					/if (${Me.Invis}) /makemevisible
					/varset isValidTarget TRUE
					/varset combat COMBAT
					/varset Target2Kill ${Target.ID}
					OUT /${_bind.Lower} force combat ${sep} [${ctar}${Target.DisplayName}\ax]

				| force assist for other peer toons
				} else /if (${_verbage.Equal[assist]}) {
					/dgexecute /varset Target2Kill ${Target.ID}						
					OUT /${_bind.Lower} force assist ${sep} [${ctar}${Target.DisplayName}\ax]
				}
			}
			 
			| echo options
			} else {
				/declare _tmpVariable string local |combat
				/for _count 1 to ${_tmpVariable.Count[|]}
					/varset _listout ${_listout} ${If[${_sep},${sep},]} ${cinfo}${_tmpVariable.Arg[${_count},|]}\ax
					/varset _sep TRUE
				/next _count
				VOUT /${_bind.Lower} force [${_listout}]	
			}
			/return

	| set rest %		
	} else /if (${_type.Equal[rest]}) {
		/if (${Range.Between[1,99:${Int[${_verbage}]}]}) {
			/call sql_update FALSE character setRestPct ${_verbage}  
		} else /if (${_verbage.Equal[0]}) {
			/call sql_update FALSE character setRestPct 0  
		}
		
	| set bandolier swapping
	} else /if (${_type.Equal[bandolier]}) {
		/call set_switch character swBandolier ${_verbage}
		VOUT /${_bind.Lower} bandolier ${sep} ${cinfo}${combat}\ax
		/return

	| set stop nuke %
	} else /if (${_type.Equal[stopnuke]}) {
		/if (${Range.Between[0,99:${Int[${_verbage}]}]}) {
			/call sql_update FALSE character setStopNukePct ${_verbage} 
		} else {
			VOUT /${_bind.Lower} stopnuke(${cnum}${setStopNukePct}\ax) allowable range is ${cinfo}0 - 99\ax
		} 		
		/return
		
	| set circle of to be used
	} else /if (${_type.Equal[circle]}) {
		/if (${_verbage.Equal[power]} || ${_verbage.Equal[life]} || ${_verbage.Equal[mana]}) {
			/call sql_update FALSE item useCircle ${_verbage.Left[1].Upper}${_verbage.Right[-1].Lower}
			VOUT /${_bind.Lower} circle ${sep} ${cinfo}${_verbage.Left[1].Upper}${_verbage.Right[-1].Lower}\ax
		} else /if (!${Bool[${_verbage}]}) {
			VOUT /${_bind.Lower} circle allowable options [${cinfo}power, life, mana\ax] 
		} 
		/return

	| set Pull
	} else /if (${_type.Equal[pull]}) {
		/call set_switch character swPull ${_verbage}
		VOUT /${_bind.Lower} pull ${sep} ${If[${swPull},${oenabled},${odisabled}]}
		/if (${swPull}) /call validate_assist NOECHO x
		/return
		
	| set Off Tank
	} else /if (${_type.Equal[offtank]}) {
		/call set_switch character swOffTank ${_verbage}
		VOUT /${_bind.Lower} offtank ${sep} ${If[${swOffTank},${oenabled},${odisabled}]}
		/return
		
	| delay in nuking	
	} else /if (${_type.Equal[nukedelay]}) {
		/if (${Range.Between[1,100:${Int[${_verbage}]}]}) {
			/call sql_update FALSE ${Me.Class.ShortName} switchNukeDelay ${_verbage}
			VOUT /${_bind.Lower} nukedelay ${sep} ${cinfo}${_verbage}\ax
		} else {
			VOUT /${_bind.Lower} nukedelay(${cnum}${tNuke_Delay}\ax) allowable range is ${cinfo}1 - 100\ax
		} 	
		/return
		
	| force fade to rest
	} else /if (${_type.Equal[faderest]}) {
		/if (!${validate_class[TRUE, |WAR|PAL|SHD|ROG|BER|RNG|MON|BST]}) /return
		/call set_switch character swForceFadeRest ${_verbage}

	| agro switch/agro range ##
	} else /if (${_type.Equal[agro]}) {
		/if (!${Bool[${_verbage}]} || ${_verbage.Equal[on]} || ${_verbage.Equal[off]}) {
			/call set_switch character swAgro ${_verbage} 
			VOUT /${_bind.Lower} agro ${sep} ${If[${swAgro},${oenabled},${odisabled}]}
		} else /if (${Range.Between[1,400:${Int[${_verbage}]}]}) {
			/call sql_update FALSE character setMobAgro ${_verbage} 
			VOUT /${_bind.Lower} agro ${sep} ${cinfo}${_verbage}\ax
		} else /if (${Bool[${_verbage}]} && !${Range.Between[1,400:${Int[${_verbage}]}]})  {
			VOUT /${_bind.Lower} agro(${cnum}${setMobAgro}\ax) allowable range is ${cinfo}1 - 400\ax
			/return
		}
		/call check_AA_agro		
		/return

	| setMA1/2 engage at %
	} else /if (${_type.Equal[engage]}) {
		/if (${Range.Between[1,100:${Int[${_verbage}]}]}) {
			/call sql_update FALSE character setEngageMAPct ${_verbage}
			VOUT /${_bind.Lower} engage ${sep} ${cinfo}${setEngageMAPct}\ax
		} else {
			VOUT /${_bind.Lower} engage(${cnum}${setEngageMAPct}\ax) allowable range ${cinfo}1 - 99\ax
		}
		/return

	| Smart Assist engage at %
	} else /if (${_type.Equal[smartengage]}) {
		/if (!${validate_expansion[TRUE, |16]}) /return
		/if (${Range.Between[1,100:${Int[${_verbage}]}]}) {
			/call sql_update FALSE character setEngagePct ${_verbage}
			VOUT /${_bind.Lower} smartengage ${sep} ${cinfo}${setEngagePct}\ax
		} else {
			VOUT /${_bind.Lower} smartengage(${cnum}${setEngagePct}\ax) allowable range ${cinfo}1 - 99\ax
		}
		/return

	| set melee combat distance to target
	} else /if (${_type.Equal[combatdistance]}) {
		/if (${Range.Between[1,50:${Int[${_verbage}]}]}) {
			/call sql_update FALSE character setCombatDistance ${_verbage}
			VOUT /${_bind.Lower} combatdistance ${sep} ${cinfo}${_verbage}\ax
		} else {
			VOUT /${_bind.Lower} combatdistance(${cnum}${setCombatDistance}\ax] minimum range to melee mob ${cinfo}1 - 50\ax
		}
		/return

	| set fake safe radius
	} else /if (${_type.Equal[saferadius]}) {
		/if (${Range.Between[1,50:${Int[${_verbage}]}]}) {
			/call sql_update FALSE character setCombatRadius ${_verbage}

			VOUT /${_bind.Lower} saferadius ${sep} ${cinfo}${setCombatRadius}\ax
		} else {
			VOUT /${_bind.Lower} saferadius(${cnum}${setCombatRadius}\ax] allowable radius ${cinfo}1 - 50\ax
		}
		/return
	
	| set Z radius
	} else /if (${_type.Equal[zradius]}) {
		/if (${Range.Between[1,200:${Int[${_verbage}]}]}) {
			/call sql_update FALSE character setzradius ${_verbage} 	

		} else {
			VOUT /${_bind.Lower} zradius(${cnum}${setzradius}\ax) allowable range ${cinfo}1 - 200\ax
			/return
		}	
		
	| alliance on or off... yada yada		
	} else /if (${_type.Equal[alliance]}) {
		/call set_switch character swAlliance ${_verbage}
		
		|**
		/declare _alliancetype string local |off|group|raid
		/if (${_alliancetype.Find[${_verbage}]}) {
			/if (${_verbage.Equal[off]}) {
				/call sql_update FALSE character setAlliance FALSE
			} else /if (${_verbage.Equal[group]} || ${_verbage.Equal[raid]}) {
				/call sql_update FALSE character setAlliance ${_verbage.Lower}
			}
		}

		/for _count 1 to ${_alliancetype.Count[|]}
			/varset _listout ${_listout} ${If[${_sep},${sep},]} ${If[${setAlliance.Equal[${_alliancetype.Arg[${_count},|]}]},${cgood},${cinfo}]}${_alliancetype.Arg[${_count},|]}\ax   
			/varset _sep TRUE
		/next _count
		VOUT /${_bind.Lower} alliance [${_listout}]	
		/return
		**|

	| enable combat: melee, range, nuke, dot, heal
	} else /if (${Select[${_type},melee,range,nuke,dot,heal]}) {

		| enable combat: melee
		/if (${_type.Equal[melee]}) {
			/call set_switch character swCombatMelee ${_verbage}

		| enable combat: range
		} else /if (${_type.Equal[range]}) {
			/call set_switch character swCombatRange ${_verbage}

		| enable combat: nuke
		} else /if (${_type.Equal[nuke]}) {
			/call set_switch character swCombatNuke ${_verbage}

		| enable combat: dot
		} else /if (${_type.Equal[dot]}) {
			/call set_switch character swCombatDoT ${_verbage}

		| enable combat: HEal
		} else /if (${_type.Equal[heal]}) {
			/call set_switch character swCombatHeal ${_verbage}

		}

		/for _count 1 to ${lstTypes${_bind}.Count[|]}
			/if (${lstTypes${_bind}.Arg[${_count},|].Equal[melee]}) /varset _listout ${_listout} ${cinfo}melee\ax:${If[${swCombatMelee},${oon},${ooff}]}	
			/if (${lstTypes${_bind}.Arg[${_count},|].Equal[range]}) /varset _listout ${_listout} ${If[${_sep},${sep},]} ${cinfo}range\ax:${If[${swCombatRange},${oon},${ooff}]}	
			/if (${lstTypes${_bind}.Arg[${_count},|].Equal[nuke]}) /varset _listout ${_listout} ${If[${_sep},${sep},]} ${cinfo}nuke\ax:${If[${swCombatNuke},${oon},${ooff}]}	
			/if (${lstTypes${_bind}.Arg[${_count},|].Equal[dot]}) /varset _listout ${_listout} ${If[${_sep},${sep},]} ${cinfo}dot\ax:${If[${swCombatDoT},${oon},${ooff}]}	
			/if (${lstTypes${_bind}.Arg[${_count},|].Equal[heal]}) /varset _listout ${_listout} ${If[${_sep},${sep},]} ${cinfo}heal\ax:${If[${swCombatHeal},${oon},${ooff}]}	
			/varset _sep TRUE
		/next _count
		VOUT /${_bind.Lower} [${_listout}]
		/return
		
	| melee discs
	} else /if (${_type.Equal[meleedisc]}) {
		/if (${_verbage.Equal[kick]} && ${Me.Skill[kick]}) /call set_switch ${Me.Class.ShortName} kick ${If[${kick},FALSE,TRUE]}
		/if (${_verbage.Equal[bash]} && ${Me.Skill[bash]}) /call set_switch ${Me.Class.ShortName} bash ${If[${bash},FALSE,TRUE]}
		/if (${_verbage.Equal[taunt]} && ${Me.Skill[taunt]}) /call set_switch ${Me.Class.ShortName} taunt ${If[${taunt},FALSE,TRUE]}
		/if (${_verbage.Equal[disarm]} && ${Me.Skill[disarm]}) /call set_switch ${Me.Class.ShortName} disarm ${If[${disarm},FALSE,TRUE]}
		/if (${_verbage.Equal[intimidation]} && ${Me.Skill[intimidation]}) /call set_switch ${Me.Class.ShortName} intimidation ${If[${intimidation},FALSE,TRUE]}
		/if (${_verbage.Equal[backstab]} && ${Me.Skill[backstab]}) /call set_switch ${Me.Class.ShortName} backstab ${If[${backstab},FALSE,TRUE]}
		/if (${_verbage.Equal[frenzy]} && ${Me.Skill[frenzy]}) /call set_switch ${Me.Class.ShortName} frenzy ${If[${frenzy},FALSE,TRUE]}
		/if (${_verbage.Equal[dragonpunch]} && ${Me.Skill[dragon punch]}) /call set_switch ${Me.Class.ShortName} dragonpunch ${If[${dragonpunch},FALSE,TRUE]}
		/if (${_verbage.Equal[eaglestrike]} && ${Me.Skill[eagle strike]}) /call set_switch ${Me.Class.ShortName} eaglestrike ${If[${eaglestrike},FALSE,TRUE]}
		/if (${_verbage.Equal[tigerclaw]} && ${Me.Skill[tiger claw]}) /call set_switch ${Me.Class.ShortName} tigerclaw ${If[${tigerclaw},FALSE,TRUE]}
		/if (${_verbage.Equal[flyingkick]} && ${Me.Skill[flying kick]}) /call set_switch ${Me.Class.ShortName} flyingkick ${If[${flyingkick},FALSE,TRUE]}
		/if (${_verbage.Equal[roundkick]} && ${Me.Skill[round kick]}) /call set_switch ${Me.Class.ShortName} roundkick ${If[${roundkick},FALSE,TRUE]}
		/if (${_verbage.Equal[slam]} && ${Me.Skill[slam]}) /call set_switch ${Me.Class.ShortName} slam ${If[${slam},FALSE,TRUE]}


		/declare _meleeskill string local |backstab|bash|disarm|dragonpunch|eaglestrike|flyingkick|frenzy|intimidation|kick|roundkick|slam|taunt|tigerclaw
		/for _count 1 to ${_meleeskill.Count[|]}
			/varset _listout ${_listout} ${If[${_sep},${sep},]} ${cinfo}${_meleeskill.Arg[${_count},|].Lower}\ax		
			/if (${_meleeskill.Arg[${_count},|].Equal[kick]} && ${Me.Skill[kick]}) /varset _listout ${_listout}:${If[${kick},${oon},${ooff}]}	
			/if (${_meleeskill.Arg[${_count},|].Equal[bash]} && ${Me.Skill[bash]}) /varset _listout ${_listout}:${If[${bash},${oon},${ooff}]}	
			/if (${_meleeskill.Arg[${_count},|].Equal[taunt]} && ${Me.Skill[taunt]}) /varset _listout ${_listout}:${If[${taunt},${oon},${ooff}]}	
			/if (${_meleeskill.Arg[${_count},|].Equal[disarm]} && ${Me.Skill[disarm]}) /varset _listout ${_listout}:${If[${disarm},${oon},${ooff}]}	
			/if (${_meleeskill.Arg[${_count},|].Equal[intimidation]} && ${Me.Skill[intimidation]}) /varset _listout ${_listout}:${If[${intimidation},${oon},${ooff}]}	
			/if (${_meleeskill.Arg[${_count},|].Equal[frenzy]} && ${Me.Skill[frenzy]}) /varset _listout ${_listout}:${If[${frenzy},${oon},${ooff}]}	
			/if (${_meleeskill.Arg[${_count},|].Equal[backstab]} && ${Me.Skill[backstab]}) /varset _listout ${_listout}:${If[${backstab},${oon},${ooff}]}	
			/if (${_meleeskill.Arg[${_count},|].Equal[dragonpunch]} && ${Me.Skill[dragon punch]}) /varset _listout ${_listout}:${If[${dragonpunch},${oon},${ooff}]}	
			/if (${_meleeskill.Arg[${_count},|].Equal[eaglestrike]} && ${Me.Skill[eagle strike]}) /varset _listout ${_listout}:${If[${eaglestrike},${oon},${ooff}]}	
			/if (${_meleeskill.Arg[${_count},|].Equal[tigerclaw]} && ${Me.Skill[tiger claw]}) /varset _listout ${_listout}:${If[${tigerclaw},${oon},${ooff}]}	
			/if (${_meleeskill.Arg[${_count},|].Equal[flyingkick]} && ${Me.Skill[flyin gkick]}) /varset _listout ${_listout}:${If[${flyingkick},${oon},${ooff}]}	
			/if (${_meleeskill.Arg[${_count},|].Equal[roundkick]} && ${Me.Skill[round kick]}) /varset _listout ${_listout}:${If[${roundkick},${oon},${ooff}]}	
			/if (${_meleeskill.Arg[${_count},|].Equal[slam]} && ${Me.Skill[slam]}) /varset _listout ${_listout}:${If[${slam},${oon},${ooff}]}	
			/varset _sep TRUE
		/next _count
		VOUT /${_bind.Lower} meleedisc [${_listout}]
		/return
	}

	/if (${swMQClear}) /mqclear
	/for _count 1 to ${lstTypes${_bind}.Count[|]}
		/varset _listout ${_listout} ${If[${_sep},${sep},]} ${cinfo}${lstTypes${_bind}.Arg[${_count},|].Lower}\ax		
		/if (${lstTypes${_bind}.Arg[${_count},|].Equal[zradius]}) /varset _listout ${_listout}:${cnum}${setzradius}\ax
		/if (${lstTypes${_bind}.Arg[${_count},|].Equal[smartengage]}) /varset _listout ${_listout}:${cnum}${setEngagePct}\ax
		/if (${lstTypes${_bind}.Arg[${_count},|].Equal[engage]}) /varset _listout ${_listout}:${cnum}${setEngageMAPct}\ax
		/if (${lstTypes${_bind}.Arg[${_count},|].Equal[nukedelay]}) /varset _listout ${_listout}:${cnum}${tNuke_Delay}\ax
		/if (${lstTypes${_bind}.Arg[${_count},|].Equal[agro]}) /varset _listout ${_listout}:${If[${swAgro},${cnum}${setMobAgro}\ax,${cbad}${setMobAgro}\ax]}
		/if (${lstTypes${_bind}.Arg[${_count},|].Equal[combatdistance]}) /varset _listout ${_listout}:${cnum}${setCombatDistance}\ax
		/if (${lstTypes${_bind}.Arg[${_count},|].Equal[saferadius]}) /varset _listout ${_listout}:${cnum}${setCombatRadius}\ax
		/if (${lstTypes${_bind}.Arg[${_count},|].Equal[stopnuke]}) /varset _listout ${_listout}:${cnum}${setStopNukePct}\ax
		/if (${lstTypes${_bind}.Arg[${_count},|].Equal[circle]}) /varset _listout ${_listout}:${cnum}${useCircle}\ax
		/if (${lstTypes${_bind}.Arg[${_count},|].Equal[alliance]}) /varset _listout ${_listout}:${If[${swAlliance},${oon}\ax,${ooff}]}
		/if (${lstTypes${_bind}.Arg[${_count},|].Equal[pull]}) /varset _listout ${_listout}:${If[${swPull},${oon},${ooff}]}
		/if (${lstTypes${_bind}.Arg[${_count},|].Equal[rest]}) /varset _listout ${_listout}:${If[${setRestPct},${cnum}${setRestPct}\ax,${ooff}]}
		/if (${lstTypes${_bind}.Arg[${_count},|].Equal[setMA1]}) /varset _listout ${_listout}:${If[${Bool[${setMA1}]},${cnum}${setMA1}\ax,${ooff}]}
		/if (${lstTypes${_bind}.Arg[${_count},|].Equal[setMA2]}) /varset _listout ${_listout}:${If[${Bool[${setMA2}]},${cnum}${setMA2}\ax,${ooff}]}
		/if (${lstTypes${_bind}.Arg[${_count},|].Equal[faderest]}) /varset _listout ${_listout}:${If[${swForceFadeRest},${oon},${ooff}]}
		/if (${lstTypes${_bind}.Arg[${_count},|].Equal[melee]}) /varset _listout ${_listout}:${If[${swCombatMelee},${oon},${ooff}]}
		/if (${lstTypes${_bind}.Arg[${_count},|].Equal[range]}) /varset _listout ${_listout}:${If[${swCombatRange},${oon},${ooff}]}
		/if (${lstTypes${_bind}.Arg[${_count},|].Equal[nuke]}) /varset _listout ${_listout}:${If[${swCombatNuke},${oon},${ooff}]}
		/if (${lstTypes${_bind}.Arg[${_count},|].Equal[dot]}) /varset _listout ${_listout}:${If[${swCombatDoT},${oon},${ooff}]}
		/if (${lstTypes${_bind}.Arg[${_count},|].Equal[heal]}) /varset _listout ${_listout}:${If[${swCombatHeal},${oon},${ooff}]}
		/if (${lstTypes${_bind}.Arg[${_count},|].Equal[offtank]}) /varset _listout ${_listout}:${If[${swOffTank},${oon},${ooff}]}
		/varset _sep TRUE
	/next _count
	VOUT /${_bind.Lower} [${_listout}]
/return	
	


|***
 *	DES: shortcut for the agro command
 *	USE: /agro [##]
 *  NOTE:
 ***| 	
#bind setMobAgro /agro
sub Bind_setMobAgro(int _setMobAgro)
	/call Bind_setCombatControl agro ${_setMobAgro}
/return



|***
 *	DES: movement controls
 *	USE: /movement []
 *  NOTE:
 ***|
#bind setMovementControls /movement
sub Bind_setMovementControls(string _type, string _verbage, bool _silent)
	/declare _count int local 0
	/declare _bind string local Movement
	/declare _listout string local
	/declare _sep bool local FALSE

	| set home on or off
	/if (${_type.Equal[home]}) {
		/call set_switch movement swHomeSet ${_verbage}  
		/call set_home
		/return
		
	| navigate to the Target or NAME		
	} else /if (${_type.Equal[goto]}) {
		/if (!${Target.ID} && !${Bool[${_verbage}]}) /return
		/if (${Bool[${_verbage}]}) {
			/squelch /nav spawn ID ${_verbage}
		} else /if (${Target.ID}) {
			/squelch /nav spawn ${Target.DisplayName}
		}
		/return		

	| set stick behind
	} else /if (${_type.Equal[behind]}) {
		/call set_switch movement swStickBehind ${_verbage}  

		| disable mezzing with stickbehind enabled.
		/if (${swdeBuffMezz} && ${swStickBehind}) {
			/if (${Defined[swdeBuffMezz]}) /call sql swdeBuffMezz Buffs FALSE  
			/VOUT /${_bind.Lower} behind ${sep} ${If[${swdeBuffMezz},${oenabled},${odisabled}]}
		}

	| set face fast
	} else /if (${_type.Equal[fast]}) {
		/call set_switch movement swFaceFast ${_verbage}  

	| set home radius
	} else /if (${_type.Equal[radius]}) {
		/if (${Range.Between[1,99:${Int[${_verbage}]}]}) {
			| /if (${_verbage} < ${setHomeLeash}) {
			| 	VOUT ${onotice} ${sep} Radius must be greater then leash(${cnum}${setHomeLeash}\ax)
			| 	/return
			| }
			/call sql_update FALSE movement setHomeRadius ${_verbage} 
		} else {
			VOUT /${_bind.Lower} radius(${cnum}${setHomeRadius}\ax) allowable range ${cinfo}1 - 99\ax
			/return
		}
		
		
	| gather your crew
	} else /if (${_type.Equal[gather]}) {

		/sqlite ${dbCore} dummy SELECT lstCrew${_verbage} FROM environment WHERE currentBuild="${currentBuild}"
		/declare _clist string local ${sqlite.Result[dummy 1 lstCrew${_verbage}]}	

		/for _count 1 to ${_clist.Count[|]}
			/if (${_clist.Arg[${_count},|].Equal[${Me.DisplayName}]}) /continue
			/if (${Bool[${Spawn[pc ${_clist.Arg[${_count},|]}]}]}) {
				VOUT Gathering ${sep} ${cinfo}${_clist.Arg[${_count},|]}\ax
				/dexecute ${_clist.Arg[${_count},|]} /nav ID ${Me.ID}
				
			}

		/next _count		
		/return
		
	| set leash range
	} else /if (${_type.Equal[leash]}) {
		/if (${Range.Between[1,99:${Int[${_verbage}]}]}) {
			| /if (${setHomeRadius} > ${_verbage}) {
			| 	VOUT ${onotice} ${sep} Leash must be less then radius(${cnum}${setHomeRadius}\ax)
			| 	/return
			| }
			/call sql_update FALSE movement setHomeLeash ${_verbage} 
		} else {
			VOUT /${_bind.Lower} leash(${cnum}${setHomeLeash}\ax) allowable range ${cinfo}1 - 99\ax
			/return
		}	

	| set facing arc
	} else /if (${_type.Equal[arc]}) {
		/if (${Range.Between[1,45:${Int[${_verbage}]}]}) {
			/call sql_update FALSE movement setFaceArc ${_verbage} 
		} else {
			VOUT /${_bind.Lower} arc(${cnum}${setFaceArc}\ax) allowable range ${cinfo}1 - 45\ax
			/return
		}	
		
	| i need mounts because i like to annoy people in the raid
	} else /if (${_type.Equal[mount]}) {
		/if (!${Bool[${_verbage}]}) {
			VOUT /${_bind.Lower} setMount [${cinfo}on\ax ${sep} ${cinfo}off\ax]
		} else /if (${Bool[${_verbage.Equal[off]}]}) {
			/if (!${Bool[${Me.Mount.ID}]}) {
				/return
			} else /if (${Bool[${Me.Mount.ID}]}) {
				/dismount
				/return
			}
		} else /if (${_verbage.Equal[on]}) {
			/if (${Bool[${Me.Mount.ID}]}) /return
			/if (${Zone.Indoor}) {
				VOUT Go outside..
				/return FALSE
			} else /if (${Me.FeetWet}) {
				VOUT WTF.. really?!
				/return FALSE
			}
			/if (${validate_cast[FALSE, item, "${setMount}", ${Me.ID}]}) /call core_cast2 "${setMount}" item 0 FALSE
		}
		
	}

	/if (${swMQClear}) /mqclear
	/for _count 1 to ${lstTypes${_bind}.Count[|]}
		/varset _listout ${_listout} ${If[${_sep},${sep},]} ${cinfo}${lstTypes${_bind}.Arg[${_count},|].Lower}\ax		
		/if (${lstTypes${_bind}.Arg[${_count},|].Equal[fast]}) /varset _listout ${_listout}:${If[${swFaceFast},${oon},${ooff}]}
		/if (${lstTypes${_bind}.Arg[${_count},|].Equal[behind]}) /varset _listout ${_listout}:${If[${swStickBehind},${oon},${ooff}]}
		/if (${lstTypes${_bind}.Arg[${_count},|].Equal[home]}) /varset _listout ${_listout}:${If[${swHomeSet},${oon},${ooff}]}
		/if (${lstTypes${_bind}.Arg[${_count},|].Equal[arc]}) /varset _listout ${_listout}:${cnum}${setFaceArc}\ax
		/if (${lstTypes${_bind}.Arg[${_count},|].Equal[leash]}) /varset _listout ${_listout}:${cnum}${setHomeLeash}\ax
		/if (${lstTypes${_bind}.Arg[${_count},|].Equal[radius]}) /varset _listout ${_listout}:${cnum}${setHomeRadius}\ax
		/varset _sep TRUE
	/next _count
	VOUT /${_bind.Lower} [${_listout}]
/return



|***
 *	DES: builds your group
 *	USE: /crew [crew1|crew2]
 *  NOTE:
 *	/grouproles set [NAME] [#]
 *	1=MainTank .. 					${GroupTank}
 *	2=MainAssist ..		 			${GroupAssist}
 *	3=Puller .. 						${GroupPuller}
 *	4=MarkNpc .. 						${GroupMark}
 *	5=MasterLooter .. 			${GroupLooter}
 *	/makeleader [NAME] .. 	${GroupLeader}
 ***|
#bind crew /crew
sub Bind_crew(int _crew)
	| bail if now crey type listed
	/if (!${Bool[${_crew}]}) /return
	
	/declare _count int local 0
	/sqlite ${dbCore} dummy SELECT lstCrew${_crew} FROM environment WHERE currentBuild="${currentBuild}"
	/declare _clist string local ${sqlite.Result[dummy 1 lstCrew${_crew}]}
	
	| disband raid if exists
	/if (${Raid.Members}) {
		/for _count 1 to ${_clist.Count[|]}
			/if (${Me.Name.Equal[${_clist.Arg[${_count},|]}]}) /continue
			/dexecute ${_clist.Arg[${_count},|]} /raiddisband
			/delay 2
		/next _count	
		/if (${Raid.Members}) /raiddisband
	}
	
	| drop group. we dont care if you are in one or not.
	/for _count 1 to ${_clist.Count[|]}
		/dexecute ${_clist.Arg[${_count},|]} /disband
		/delay 1
	/next _count			
	
	/delay 2s

	| invite the members in the list
	/for _count 1 to ${_clist.Count[|]}
		/if (!${Bool[${Group.Member[${_clist.Arg[${_count},|]}]}]}) {
			VOUT Inviting ${sep} ${cpc}${_clist.Arg[${_count},|]}\ax
			/invite ${_clist.Arg[${_count},|]}
		}	
	/next _count
		
	/delay 1s
	/delay 12s ${Group.GroupSize} == ${_clist.Count[|]}
	/if (${Group.GroupSize} != ${_clist.Count[|]}) {
		VOUT ${owarning} ${sep} Group failed to form properly.
		/return
	} else {
		VOUT Group formed.
	}
	
	| set Group Main Assist
	/if (${Bool[${setCrew${_crew}Assist}]} && !${Group.Member[${setCrew${_crew}Assist}].Offline}) {
		/delay 5
		| /if (${Group.Member[${setCrew${_crew}Assist}].Index}) /grouproles set ${setCrew${_crew}Assist} 2
		/grouproles set ${setCrew${_crew}Assist} 2
	}

	| set Group Master Looter
	/if (${Bool[${setCrew${_crew}Looter}]} && !${Group.Member[${setCrew${_crew}Looter}].Offline}) {
		/delay 5
		| /if (${Group.Member[${setCrew${_crew}Looter}].Index}) /grouproles set ${setCrew${_crew}Looter} 5
		/grouproles set ${setCrew${_crew}Looter} 5
	}

	| set Group leader ... last to make sure all roles are set
	/if (${Bool[${setCrew${_crew}Leader}]} && !${Group.Member[${setCrew${_crew}Leader}].Offline}) {
		/delay 5
		| /if (${Group.Member[${Group.Leader}].Index} != ${Group.Member[${setCrew${_crew}Leader}].Index}) /makeleader ${setCrew${_crew}Leader}
		/makeleader ${setCrew${_crew}Leader}
	}
	
	VOUT Leader:${cinfo}${setCrew${_crew}Leader}\ax ${sep} Assist:${cinfo}${setCrew${_crew}Assist}\ax ${sep} Looter:${cinfo}${setCrew${_crew}Looter}\ax
/return



|***
 *	DES: random insult
 *	USE: /oof
 *  NOTE: not documented. why should i share everything!?!
 ***|
#bind oof /oof
sub Bind_oof()
 	/declare _pronoun 			string 	local FALSE
 	
 	/if (${Select[${Me.Gender},male]}) {
 		/varset _pronoun his
 	} else /if (${Select[${Me.Gender},female]}) {
 		/varset _pronoun her
 	} else /if (${Select[${Me.Gender},neuter]}) {
 		/varset _pronoun its'
 	}
 	
 	/em looks around for the biggest fucking rock to hit ${_pronoun} head against.
	/say ${bitch${Math.Rand[${bitchcount}]}}
/return



|***
 *	DES: lesson of the devoted
 *	USE: /lesson
 *  NOTE:
 ***|
#bind lesson /lesson
sub Bind_lesson()
  /if (${validate_cast[FALSE, alt, "Lesson of the Devoted", ${Me.ID}]}) /call core_cast2 "Lesson of the Devoted" alt 0 FALSE
/return



|***
 *	DES: grabs a ground spawn.
 *	USE: /grab NAME
 *  NOTE: max range of 20
 ***|
#bind grab /grab
sub Bind_grab(string _name)

	| typed something wrong
	/if (${_name.Equal[NULL]} || !${Bool[${_name}]} || ${_name.Equal[info]} ) {
		VOUT /grab ${cinfo}NAME\ax of mapfilter spawn name you want to pick up.
		/return
	}
	
	| trash pick up.
	/if (${Ground[${_name}].Distance3D} > 20) {
		VOUT ${cinfo}${_name}\ax is ${Ground[${_name}].Distance3D}. Needs to be within 20.
		/return
	} else /if (${Ground[${_name}].Distance3D} <= 20) {
		/invoke ${Ground[${_name}].Grab}
	}
/return



|***
 *	DES: navigates to the name/target
 *	USE: /navto [NAME]
 *  NOTE: 
 ***|
#bind nav_to_target /navto
sub Bind_nav_to_target(string _name)
	/call Bind_setMovementControls goto ${_name}
/return



|***
 *	DES: clicks yes on a window option for some form of travel
 *	USE: /take
 *  NOTE: 
 ***|
#bind broadcast_take /take
sub Bind_broadcast_take()
	/if (${swBCTake}) {
		/dgexecute /click_window_option
	}
	/call Bind_click_window_option
/return

|***
 *	DES: take window option sub
 ***|
#bind click_window_option /click_window_option
sub Bind_click_window_option()

	| take ports
	/if (${Window[ConfirmationDialogbox].Child[CD_TextOutPut].Text.Find[${Spawn[${Me}].Guild}]} || ${Window[ConfirmationDialogbox].Child[CD_TextOutPut].Text.Find["Guild Hall"]}) /nomodkey /notify ConfirmationDialogBox Yes_Button leftmouseup
	/if (${Window[ConfirmationDialogbox].Child[CD_TextOutPut].Text.Find["be translocated by"]}) /nomodkey /notify ConfirmationDialogBox Yes_Button leftmouseup
	/if (${Window[LargeDialogWindow].Child[LDW_TextBox].Text.Find["Would you like to travel"]}) /nomodkey /notify LargeDialogWindow LDW_YesButton leftmouseup 

	| take taskadds/missions/HA
	/if (${Window[LargeDialogWindow].Child[LDW_TextBox].Text.Find["Would you like to take on the challenge"]}) /nomodkey /notify LargeDialogWindow LDW_YesButton leftmouseup 
	/if (${Window[ConfirmationDialogBox].Child[CD_TextOutPut].Text.Find["Join the expedition"]}) /nomodkey /notify ConfirmationDialogBox Yes_Button leftmouseup 
	/if (${Window[LargeDialogWindow].Child[LDW_TextBox].Text.Find["You have a heroic adventure"]}) /nomodkey /notify LargeDialogWindow LDW_YesButton leftmouseup
	/if (${Window[ConfirmationDialogbox].Child[CD_TextOutPut].Text.Find["has asked you to join the shared task"]}) /nomodkey /notify ConfirmationDialogBox Yes_Button leftmouseup
	
	| trade window
	/if (${Window[Tradewnd]}) /nomodkey /notify Tradewnd TRDW_Trade_Button leftmouseup

	| give trade (inc NPC)
	/if (${Window[GiveWnd]}) /squelch /notify GiveWnd GVW_Give_Button leftmouseup
	
/return




| are these useful ?
/squelch /notify ConfirmationDialogBox OK_Button leftmouseup
/squelch /notify TradeWND TRDW_Trade_Button leftmouseup
/squelch /notify TaskTemplateSelectWnd TaskTemplateSelectAcceptButton leftmouseup
/squelch /notify ProgressionTemplateSelectWnd ProgressionTemplateSelectAcceptButton leftmouseup







|***
 *	DES: stack invis
 *	USE: /invis
 *  NOTE: 
 ***|
#bind broadcast_invis /invis
sub Bind_broadcast_invis()
	/dgexecute /cast_AA_invis
	/call Bind_cast_AA_invis
/return

|***
 *	DES: sub for invis command
 ***| 
#bind cast_AA_invis /cast_AA_invis
sub Bind_cast_AA_invis()

	| fucking bards
	/if (${Me.AltAbilityReady[Shauri's Sonorous Clouding]} && ${Select[${Me.Class.ShortName},BRD]}) {
		/call pulse_stop
		/call MQ2Cast "Shauri's Sonorous Clouding" alt -targetid|${Me.ID}
		ECHOCHANNEL AA: Shauri's Sonorous Clouding
		/return
		
	| Invis vs Normies  casters	
	} else /if (${Me.AltAbilityReady[Group Perfected Invisibility]} && ${Select[${Me.Class.ShortName},WIZ,MAG,ENC]}) {
		/call MQ2Cast "Group Perfected Invisibility" alt
		ECHOCHANNEL AA: Group Perfected Invisibility
		/return
	
	| Invis Vs undead casters
	} else /if (${Me.AltAbilityReady[Group Perfected Invisibility to Undead]} && ${Select[${Me.Class.ShortName},CLR,SHD,NEC,PAL]}) {
		/delay 3
		/call MQ2Cast "Group Perfected Invisibility to Undead" alt
		ECHOCHANNEL AA: Group Perfected Invisibility to Undead
		/return
		
	| shaman	
	} else /if (${Me.AltAbilityReady[Group Silent Presence]} && ${Select[${Me.Class.ShortName},SHM]}) {
		/delay 5
		/call MQ2Cast "Group Silent Presence" alt -targetid|${Me.ID}
		ECHOCHANNEL AA: Group Silent Presence
		/return
	
	| rangers and druids
	}	else /if (${Me.AltAbilityReady[Shared Camouflage]} && ${Select[${Me.Class.ShortName},RNG,DRU]}) {
		/return
		/delay 15
		/call MQ2Cast "Shared Camouflage" alt -targetid|${Me.ID}
		ECHOCHANNEL AA: Shared Camouflage
		/return
	}
/return



|***
 *	DES: clicks a door or lever to open or zone
 *	USE: /enter
 *  NOTE: 
 ***|
#bind broadcast_door /enter
sub Bind_broadcast_door()
	/if (${swBCEnter}) {
		/dgexecute /click_door
	}
	OUT ${obroadcasted} ${sep} /enter
	/call Bind_click_door
/return

|***
 *	DES: sub for door/lever clicking
 ***|
#bind click_door /click_door
sub Bind_click_door()

	/call timer_update timer_try_door 4s

	:retrydoortarget
	/if (!${timer_try_door}) /return
	/doortarget 
	/delay 1
	/if (!${Switch.ID}) /goto :retrydoortarget

	/if (!${timer_try_door}) /return

	/if (!${Switch.Open}) {
		/click left door
		/delay 1s
		/doevents flush
	}
/return



|***
 *	DES: deal with corpses
 *	USE: /cls
 *  NOTE: 
 ***|
#bind cls /cls
sub Bind_cls()
	/call Bind_clear_corpse_sub
	/if (${swBCClean}) {
		/dgexecute /clear_corpse_sub
	} 
	/call Bind_clear_corpse_sub
/return

|***
 *	DES: sub for dealing with corpses
 ***|
#bind  clear_corpse_sub /clear_corpse_sub
sub Bind_clear_corpse_sub()
	/if (${Cursor.ID}) /call sort_cursor TRUE
	/hidecorpse all
/return


	
|***
 *	DES: will set your group to combat ready with 1 key.
 *	USE:   
 *  NOTE: this is designed for a fast group set due to agro while traveling. This will not affect the sender of the command
 ***|
#bind broadcast_here /here
sub Bind_broadcast_here()
	/call Bind_broadcast_on
	/dgexecute /set_combat_here
	OUT ${obroadcasted} ${sep} /here
/return

|***
 *	DES: sub for /here command
 ***|
#bind set_combat_here /set_combat_here 
sub Bind_set_combat_here()
 	AMIDEAD
	UPDATEHUD "Combat Now"

	VOUT ${owarning} ${sep} COMBAT ${sep} Right NOW!
	
	| shut the bard up..
	/if (${Select[${Me.Class.ShortName},BRD]} && ${Me.Casting.ID}) /call pulse_stop

	/if (${Me.Casting.ID}) /stopcast
	/doevents flush

	| clear all other modes
	/call switch_clear_mode
	
	| reset timers for safety
	/if (${swBuffItem}) /call timer_update timer_check_buffs_item ${defaultTimerDelay}
	/if (${swBuffSelf}) /call timer_update timer_check_buffs_self ${defaultTimerDelay}
	/if (${swBuffRaid}) /call timer_update timer_check_buffs_raid ${defaultTimerDelay}
	/if (${swBuffBC}) /call timer_update timer_check_buffs_BC ${defaultTimerDelay}
	/if (${swBuffPet}) /call timer_update timer_check_buffs_pet ${defaultTimerDelay}
	/if (${swBuffMerc}) /call timer_update timer_check_buffs_merc ${defaultTimerDelay}
	/if (${swBuffOther}) /call timer_update timer_check_buffs_other ${defaultTimerDelay}
	/if (${Bool[${Aura1Buff}]} || ${Bool[${Aura2Buff}]}) /call timer_update timer_check_aura ${defaultTimerDelay}
	/if (${swBuffTell}) /call timer_update timer_check_buffs_tell ${defaultTimerDelay}
	
	/if (${Select[${Me.Class.ShortName},CLR]}) {
		/call timer_update timer_heal_ward ${tHealWard}
	}

	AMIDEAD
	| stop sticking to stuff
	/if (${swsnapstuck}) /varset swsnapstuck FALSE
	/if (${swSetTie}) /varset swSetTie FALSE
	
	/if (${AdvPath.Following}) /squelch /afollow off

	/if (${Stick.Active}) /squelch /stick off

	| stop mq2nav
	/if (${Navigation.Active}) /squelch /nav stop

	| stop moving
	/if (${Me.Moving}) {
		/keypress forward
		/keypress back
	}

	| fix zone
	/call control_zone FALSE
 	
	| /call clear_combat
	/if (${swPet} && ${Me.Pet.ID} && ${Bool[${Me.Pet.Target}]}) /pet back off

	| make us visible 
	/if (${makeVisible} && ${Me.Invis}) /makemevisible
	
	| get shit out of our hands
	/call sort_cursor TRUE
	
	| clear the current target
	/if (${clearTarget} && ${Target.ID}) /squelch /target clear
	
	| wake the Main Assist up from its nap, set him in place and group assists
	/if (${Group.MainAssist.Name.Equal[${Me.DisplayName}]}) {
		/if (!${swHomeSet}) {
			/call set_switch character swHomeSet TRUE
			/call set_home
		}
		| cut the agro way down for safety
		/if (${SpawnCount[npc ${If[${swOverrideLOS},,los]} radius ${setMobAgro}]} > 2) {
			/if (${baseAgroRange} && ${setMobAgro} > ${baseAgroRange}) /agro ${baseAgroRange}
		}	
		/if (!${swAgro}) /call set_switch character swAgro TRUE
		/if (!${swPull} && !${swAssistF}) /call set_switch character swPull TRUE
		/if (${setzradius} != ${hereZRadius}) /cc setzradius ${hereZRadius}	
	}

	UPDATEHUD FALSE	
/return



|***
 *	DES: sets all toons to override all engaeg HP% and attack as soon as there is a vlaid target in range
 *	USE: /killmob
 *  NOTE: this is just a simple broadcast of .override setEngageMAPct. for like when your in a hurry and shit.
 ***|
#bind killmob /killmob
sub Bind_killmob()
	/call Bind_override engagepct
	/if (${swBCKillMob}) {
		/dgexecute /override engagepct
	} 	
	OUT ${obroadcasted} ${sep} /killmob
/return



|***
 *	DES: look for undefined variables
 *	USE: /checkvar
 *  NOTE: this is a utility for finding errors in code. nothing more
 ***|
#bind checkvar /checkvar
sub Bind_checkvar()
	/mqclear
	/invoke ${Macro.Undeclared}
/return



|***
 *	DES: make adjustments for the drivetoon
 *	USE: /imincharge
 *  NOTE: This is a work in progress.
 ***|
#bind imincharge /imincharge
sub Bind_imincharge()

	/declare _count int local 0
	/declare _toonName string local FALSE
	
	| cycle all the toons you are boxing
	/for _count 1 to ${DanNet.PeerCount}
		/varset _toonName ${DanNet.Peers.Arg[${_count},|].Left[1].Upper}${DanNet.Peers.Arg[${_count},|].Right[-1]}
		
		| if i'm issuing the command. turn off tie and move along
		/if (${_toonName.Equal[${Me.DisplayName}]}) {
			/call sql_update FALSE movement setTieToon FALSE
			/continue
		}
		
		| if the toon is not in this zone, move along
		/if (!${Spawn[pc ${_toonName}].ID}) /continue
		
		| update the toons DB setting
		/sqlite ${dbCore} onoff UPDATE character SET movement="${Me.DisplayName}" WHERE chrID="${MacroQuest.Server}_${Spawn[pc ${_toonName}].Class.ShortName}_${_toonName}"
		
		| reset the variable in memory
		/dexecute ${_toonName} /squelch /target clear
		/dexecute ${_toonName} /varset setTieToon ${Me.DisplayName}
	/next _count




	| set my tie name off
	

	| set everyone elses /tie defaults
	| /dgexecute /tc name ${Me.DisplayName}
	OUT ${obroadcasted} ${sep} I'm the damn boss!
/return
	
	
	
|***
 *	DES: have everyone get your target and repeate what you say.
 *	USE: /bcsay VERBAGE
 *  NOTE:
 ***|
#bind bcsay /bcsay
sub Bind_bcsay(string _verbage, string _sender, int _tmpID)
	| SDEBUG bcsay v:"${_verbage}" s:${_sender} id:${_tmpID}
	
	/if (${Me.Invis}) /makemevisible
	
	| sender
	/if (!${Bool[${_sender}]}) {
		| need a target
		/if (!${Target.ID}) /return
		
		| speak you peace
		/say ${_verbage}
		
		| tell everyone else to speak up
		/dgexecute /bcsay "${_verbage}" ${Me.DisplayName} ${Target.ID}
		/return
	}

	| everyone else
	/if (${_sender.NotEqual[${Me.DisplayName}]}) {
		/if (${Bool[${Spawn[pc ${_sender} radius 60].ID}]}) {
			/squelch /target ID ${_tmpID}
			/delay 10s ${Target.ID} == ${_tmpID}
			/if (${Target.ID} == ${_tmpID}) {
				/delay ${Int[${Math.Calc[${Math.Rand[20]}+10]}]}
				/say ${_verbage}
				/delay 1s
			}
		}
	}
	
/return



|***
 *	DES: for when you really gotta go..
 *	USE: /setGTFO
 *  NOTE:
 ***|
#bind GTFO /GTFO
sub Bind_GTFO()
	/dgexecute /get_the_fuck_out
	OUT ${obroadcasted} ${sep} Get The Fuck Out
	/call Bind_get_the_fuck_out
/return

#bind get_the_fuck_out /get_the_fuck_out
sub Bind_get_the_fuck_out()
	/call sort_cursor TRUE
	/call set_stop NOECHO
	/call clear_combat
	/docommand /makemevisible

	/if (${Me.ItemReady[${setGTFO}]}) {
		/if (${validate_cast[FALSE, item, "${setGTFO}", ${Me.ID}]}) /call core_cast2 "${setGTFO}" item 0 FALSE
	} else /if (${Me.SpellReady[${Spell[${setGTFO}].RankName}]}) {
		/if (${validate_cast[FALSE, spell, "${setGTFO}", ${Me.ID}]}) /call core_cast2 "${setGTFO}" 0 0 FALSE
	} else /if (${Me.AltAbility[${setGTFO}].ID}) {
		/if (${validate_cast[FALSE, alt, "${setGTFO}", ${Me.ID}]}) /call core_cast2 "${setGTFO}" alt 0 FALSE
	}
/return



|***
 *	DES: fellowship campfire controls
 *	USE: /campfire  
 *  NOTE: 
 ***|
#bind campfire /campfire
sub Bind_campfire(string _verbage)
	/declare _count int local 0
	/declare _bind string local campfire
	/declare _listout string local
	/declare _sep bool local FALSE
	/declare campfireType string local |drop|port

	| drop a campfire
	/if (${_verbage.Equal[drop]}) {
		
		| need 3 people close
		/if (${SpawnCount[fellowship radius 50]} < 3) /return
		
		/squelch /windowstate FellowshipWnd open
		/delay 10 !${Window[FellowshipWnd].Open}
		/nomodkey /notify FellowshipWnd FP_Subwindows tabselect 2
		
		| skip this if we dont have a fire
		/if (${Me.Fellowship.Campfire}) {
			/nomodkey /notify FellowshipWnd FP_DestroyCampsite leftmouseup
			/delay 5s ${Window[ConfirmationDialogBox].Open}
			/if (${Window[ConfirmationDialogBox].Open}) /nomodkey /notify ConfirmationDialogBox Yes_Button leftmouseup
			/delay 5s !${Me.Fellowship.Campfire}
		}
		/nomodkey /notify FellowshipWnd FP_RefreshList leftmouseup
		/delay 8
		/nomodkey /notify FellowshipWnd FP_CampsiteKitList listselect 1
		/delay 8
		/nomodkey /notify FellowshipWnd FP_CreateCampsite leftmouseup
		/delay 5s ${Me.Fellowship.Campfire}
		/if (${Window[FellowshipWnd].Open}) /squelch /windowstate FellowshipWnd close
		
	| click campfire insignia	
	} else /if (${_verbage.Equal[port]}) {
		/if (${Me.Invis}) /makemevisible
		/delay 1s !${Me.Invis}
  	/call set_stop NOECHO
		/delay 1s !${Me.Casting.ID}
		/if (${validate_cast[FALSE, item, "Fellowship Registration Insignia", ${Me.ID}]}) /call core_cast2 "Fellowship Registration Insignia" item 0 FALSE
		/return
	}
	
	/if (${swMQClear}) /mqclear
	/for _count 1 to ${${_bind}Type.Count[|]}
		/varset _listout ${_listout} ${If[${_sep},${sep},]} ${cinfo}${${_bind}Type.Arg[${_count},|]}\ax		
		/varset _sep TRUE
	/next _count
	VOUT /${_bind.Lower} [${_listout}]	
	
/return



|***
 *	DES: Request Call of the Hero [aa|group]
 *	USE: /coh [group]
 *  NOTE: 
 ***|
#bind request_coh /coh
sub Bind_request_coh(string _verbage)
	/if (${_verbage.Equal[group]}) {
		/dex ${get_resident[mage, 99999]} /call_of_the_hero "${Me.DisplayName}" "${_verbage}"
		/return
	} else {
		/dex ${get_resident[mage, 99999]} /call_of_the_hero "${Me.DisplayName}"
		/return
	}
/return



|***
 *	DES: spellbar gem controls
 *	USE: /gc 
 *  NOTE: this will store the current spell line up in core.ini. it is sorted by class, not level.
 ***|
#bind gem_control /gc
sub Bind_gem_control(string _verbage, string _name)

	/if (${Select[${Me.Class.ShortName},ROG,WAR,MON]}) /return FALSE

	/declare _count int local 0
	/declare _section string local ${Me.Class.ShortName}
	/declare _bind string local gc
	/declare _listout string local
	/declare _sep bool local FALSE
	/declare _controls string local |list|load|save
	/declare _tmpName string local

	| do listings before we assign a default name
	/if (${_verbage.Equal[list]}) {
		
		| list all for class
		/if (!${Ini[${INIGemControl},${_section}].Find[${_name}]}) {
			VOUT [${cnum}${Me.Class.ShortName}\ax] ${sep} ${Ini[${INIGemControl},${_section}]}
		
		| list the specific
		} else {
		
			VOUT List ${sep} ${cinfo}${_name}\ax
			| parse the list
			/for _count 1 to ${Ini[${INIGemControl},${_section},${_name}].Count[|]}
				VOUT gem${cnum}${Ini[${INIGemControl},${_section},${_name}].Arg[${_count},|]}\ax ${sep} ${cinfo}${rank_add[FALSE, ${Ini[${INIGemControl},${_section},${_name}].Arg[${Math.Calc[${_count}+1]},|]}]}\ax	
				/varset _count ${Math.Calc[${_count}+1]}	
			/next _count		
		}
		
		/return
	}

	| default _name to YYYMMDD if no name entered
	/if (!${Bool[${_name}]}) {
		/varset _name ${Time.Year}${Time.Month}${Time.Day}
	}

	| save the spell set
	/if (${_verbage.Equal[save]}) {
		/if (!${Bool[${_name}]}) /return FALSE
		/declare _value string local
	
		| cycle the current memed spells
		/for _count 1 to ${Me.NumGems}
		
			| dont keep track of whats in the buffslot if the user is not holding the gemslot
			/if (!${swBuffGemHold} && ${_count} == ${fix_gem_id[${buffGem}]}) /continue

			| remove the damn rank names for storage
			/varset _tmpName ${rank_remove[FALSE, ${Me.Gem[${_count}]}]}

			| build the entry
			/varset _value ${_value}|${_count}|${_tmpName}
		/next _count

		| write the entry
		/ini ${INIGemControl} ${_section} "${_name}" "${_value}"
		VOUT /${_bind.Lower} save ${cinfo}${_name}\ax
		
		/return	
		
	| load a spell set
	} else /if (${_verbage.Equal[load]}) {

		| bad name
		/if (!${Ini[${INIGemControl},${_section}].Find[${_name}]}) {
			VOUT ${onotice} ${sep} Invalid Name
			VOUT /gc [${cnum}load\ax|${cnum}save\ax] [${cnum}name\ax]
			VOUT ${sep} ${Ini[${INIGemControl},${_section}]}
			/return FALSE
		}

		| iterate valid ini entry
		/for _count 1 to ${Ini[${INIGemControl},${_section},${_name}].Count[|]}
			
			| add the rank back to the spells by what you have in your book
			/varset _tmpName ${rank_add[FALSE, ${Ini[${INIGemControl},${_section},${_name}].Arg[${Math.Calc[${_count}+1]},|]}]}
			
			| mem any missing spells
			/call mem_spell TRUE "${_tmpName}" ${Ini[${INIGemControl},${_section},${_name}].Arg[${_count},|]} FALSE
			
			| just keep swimming, just keep swimming
			/varset _count ${Math.Calc[${_count}+1]}	
		/next _count
		
		/return
				
	}

	/if (${swMQClear}) /mqclear
	/for _count 1 to ${_controls.Count[|]}
		/varset _listout ${_listout} ${If[${_sep},${sep},]} ${cinfo}${_controls.Arg[${_count},|].Lower}\ax
		/varset _sep TRUE
	/next _count
	VOUT /${_bind.Lower} [${_listout}]

/return



|***
 *	DES: user controls
 *	USE: RTFM.. seriously
 *  NOTE: hey look squirrels...
 ***|
#bind sql_edit /edit
sub Bind_sql_edit(string _variable, string _value, string _add_delete)
	| SDEBUG Bind_sql_edit(variable:\at${_variable}\ax value:\at${_value}\ax a/d:\at${_add_delete}\ax)
	/declare _count int local 0
	/declare _countArray int local 
	/declare _table string local
	/declare _whereField string local
	/declare _whereValue string local
	/declare _found bool local FALSE
	
	| error instructions
	/if (!${Bool[${_variable}]} || (!${Bool[${_value}]} && ${_value.NotEqual[FALSE]})) {
		OUT ${cinfo}/edit\ax [${cnum}${If[${Bool[${_variable}]},${_variable},variable]}\ax] [${cnum}"value"\ax] [${cnum}add\ax|${cnum}delete\ax](for lists)
		/return FALSE
	} 

	| find the table that holds the data we need to adjust
	/for _countArray 1 to ${sqlTables.Size}
		/for _count 1 to ${sqlTables[${_countArray}].Count[|]}
			| SDEBUG ${sqlTables[${_countArray}].Arg[${_count},|]}

			| prep sql info needed so it does not have to be done inline
			/varset _table ${sqlTables[${_countArray}].Arg[${_count},|]}
			
			/if (${_countArray} == 1) {
				/varset _whereField ${sql_where.Arg[1,|]}
				/varset _whereValue ${sql_where.Arg[2,|]}
			} else /if (${_countArray} == 2) {
				/varset _whereField ${sql_where.Arg[3,|]}
				/varset _whereValue ${sql_where.Arg[4,|]}
			}
			
			| SDEBUG Looking in: \at${_table}\ax WHERE \aw${_whereField}\ax = \aw"${_whereValue}"\ax
				
			/sqlite ${dbCore} sqlFindTable SELECT ${_variable} FROM ${_table} WHERE ${_whereField}="${_whereValue}"
			/if (!${sqlite.Rows[sqlFindTable]}) /continue
			/varset _found TRUE
			| SDEBUG FOUND ${_variable} ${_table} WHERE ${_whereField}="${_whereValue}"
				
			| if we add or delete data, prep the _value first before entering it
			/declare _varData string local ${sql_return[${_variable}, dummy]}
			
			| add to an existig or default value
			/if (${_add_delete.Equal[add]}) {
				| dont waste time adding same shit. we dont want dupes anyway
				/if (${_varData.Find[${_value}]}) /return TRUE
				
				| clear any false/.default data on new entries
				/if (${_varData.Find[|FALSE]} || ${_varData.Find[FALSE]} ) /varset _varData
				
				| adjust the variable
				/varset _value ${_varData}|${_value}

			| delete an existing value
			} else /if (${_add_delete.Equal[delete]}) {
				
				| dont try and delete anything that is not there
				/if (!${_varData.Find[${_value}]}) /return TRUE
	
				/varset _value |${_value}
				/declare _a int local
				/declare _b int local
				/declare _strLeft string local
				/declare _strRight string local
				
				| split the data along the | point of the list to be removed.
				/varcalc _a ${_varData.Find[${_value}]}-1
				/varcalc _b ${_varData.Length}-${_value.Length}-${_a}
				/if (${_a} >= 0) /varset _strLeft ${If[${Select[${_a},0,NULL,FALSE,""]},,${_varData.Left[${_a}]}]}
				/if (${_a} >= 0) /varset _strRight ${If[${Select[${_b},0,NULL,FALSE,""]},,${_varData.Right[${_b}]}]} 
				
				| reconnect the two variables w/o the data to be removed
				/varset _value ${_strLeft}${_strRight}	
				
				| if last entity deleted, reset to factory false
				/if (${_value.Equal[NULL]} || !${Bool[${_value}]}) /varset _value |FALSE
				
			}
	
			| update the _value into the DB						
			/sqlite ${dbCore} dummy UPDATE ${_table} SET ${_variable}="${_value}" WHERE ${_whereField}="${_whereValue}"
			/sqlite ${dbCore} dummy SELECT ${_variable} FROM ${_table} WHERE ${_whereField}="${_whereValue}"


			| repopulate the variable
			/varset ${_variable} ${sqlite.Result[dummy 1 ${_variable}]}

			| results!
			VSQL /edit ${sep} ${cinfo}${_table}\ax::${cinfo}${_variable}\ax::${If[${sqlite.Status[dummy].Equal[Success]},${cgood}${_value}\ax,${cbad}${${_variable}}\ax]}\ax
			/call sql_clear FALSE dummy
			/call sql_clear FALSE sqlFindTable
			/return TRUE

		/next _count		
	/next _countArray	 
	
	| out error if we never found the variable the user entered
	/if (!${_found}) VSQL \arSQLite Error\ax in ${sep} ::\aw${_variable}\ax

/return TRUE



|***
 *	DES: i dunno... information maybe?
 *	USE: /info
 *  NOTE: list all switches for help
 ***|
#bind core_info /info
sub Bind_core_info(string _type, string _verbage)

	| force clear screen shit takes up space
	/mqclear

	/declare _listout string local
	/declare _sep bool local
	/declare _count int local 0
	/declare _lstCMDS string local |admin|buff|burn|campfire|cc|core|cursor|debuff|hc|heal|lc|mode|movement|onoff|override|pc|rez|setassist|tc|ts
	
	| default commands
	/if (!${Bool[${_type}]}) {

		/if (${swDispInfo}) {
			/call echos wiki 0 0 0 0
			VOUT /info ${cinfo}command\ax for details
		}

		/for _count 1 to ${_lstCMDS.Count[|]}
			/varset _listout ${_listout} ${If[${_sep},${sep},]} ${_lstCMDS.Arg[${_count},|].Lower}
			/varset _sep TRUE
		/next _count
		VOUT ${_listout} ${If[${_sep},${sep},]} ${Me.Class.ShortName.Lower}
		/return

	| class help
	} else /if (${_type.Equal[${Me.Class.ShortName}]}) {
		
		/for _count 1 to ${Math.Calc[${Ini[${INIInfo},${Me.Class.ShortName}].Count[|]}-1]}
			VOUT /${Me.Class.ShortName.Lower} ${cinfo}${Ini[${INIInfo},${Me.Class.ShortName}].Arg[${_count},|]}\ax ${sep} ${Ini[${INIInfo},${Me.Class.ShortName},${Ini[${INIInfo},${Me.Class.ShortName}].Arg[${_count},|]}]}
		/next _count
		/return			

	| command list
	} else /if (${_lstCMDS.Find[${_type}]}) {
		/for _count 1 to ${Math.Calc[${Ini[${INIInfo},${_type}].Count[|]}-1]}
			VOUT /${_type.Lower} ${cinfo}${Ini[${INIInfo},${_type}].Arg[${_count},|].Lower}\ax ${sep} ${Ini[${INIInfo},${_type.Lower},${Ini[${INIInfo},${_type.Lower}].Arg[${_count},|]}]}
		/next _count
		/return
	}
	
/return 



|***
 *	DES: environment settings
 *	USE: /environment []
 *  NOTE: 
 ***|
#bind Core_environment /environment
sub Bind_Core_environment(string _type, string _verbage, bool _silent)
	/declare _count int local 0
	/declare _countArray int local 0
	/declare _bind string local Environment
	/declare _listout string local
	/declare _sep bool local FALSE

	| set mob minimum lvl for attacking
	/if (${_type.Equal[moblvlmin]}) {
		/call sql_update FALSE ${_bind.Lower} setMobLvlMin ${_verbage}

	| set mob maximum level for attacking
	} else /if (${_type.Equal[moblvmax]}) {
		/call sql_update FALSE ${_bind.Lower} setMobLvlMax ${_verbage}

	| use sarcastic remarks
	} else /if (${_type.Equal[bitch]}) {
		/call set_switch ${_bind.Lower} swBitch ${_verbage}

	| force a nuke loop till all are casted before exiting
	} else /if (${_type.Equal[nukeloop]}) {
		/call set_switch ${_bind.Lower} swNukeLoop ${_verbage}

	| display /info
	} else /if (${_type.Equal[dispinfo]}) {
		/call set_switch ${_bind.Lower} swDispInfo ${_verbage}

	| display build ##
	} else /if (${_type.Equal[dispbuild]}) {
		/call set_switch ${_bind.Lower} swDispBuild ${_verbage}

	} else /if (${_type.Equal[dispexp]}) {
		/call set_switch ${_bind.Lower} swDispEXP ${_verbage}

	} else /if (${_type.Equal[manualaadiscnow]}) {
		/call set_switch ${_bind.Lower} swManualAADiscNow ${_verbage}

	} else /if (${_type.Equal[delayzoning]}) {
		/call set_switch ${_bind.Lower} swDelayZoning ${_verbage}

	} else /if (${_type.Equal[restfull]}) {
		/call set_switch ${_bind.Lower} swRestFull ${_verbage}

	} else /if (${_type.Equal[advloot]}) {
		/call set_switch ${_bind.Lower} swADVLoot ${_verbage}

	} else /if (${_type.Equal[glyphoutsideraid]}) {
		/call set_switch ${_bind.Lower} swGlyphOutsideRaid ${_verbage}

	} else /if (${_type.Equal[mqclear]}) {
		/call set_switch ${_bind.Lower} swMQClear ${_verbage}

	} else /if (${_type.Equal[safelistguild]}) {
		/call set_switch ${_bind.Lower} swSafelistGuild ${_verbage}

	} else /if (${_type.Equal[eqcash]}) {
		/call set_switch ${_bind.Lower} swEQCash ${_verbage}

	} else /if (${_type.Equal[modepersistant]}) {
		/call set_switch ${_bind.Lower} swModePersistent ${_verbage}

	} else /if (${_type.Equal[combattie]}) {
		/call set_switch ${_bind.Lower} swCombatTie ${_verbage}

	} else /if (${_type.Equal[buffgemhold]}) {
		/call set_switch ${_bind.Lower} swBuffGemHold ${_verbage}

	} else /if (${_type.Equal[mqfontsize]}) {
		/call sql_update FALSE ${_bind.Lower} setMQFontSize ${_verbage}

	} else /if (${_type.Equal[mqcaptions]}) {
		/call set_switch ${_bind.Lower} swMQCaptions ${_verbage}

	} else /if (${_type.Equal[maxbuffcount]}) {
		/call sql_update FALSE ${_bind.Lower} setMaxBuffCount ${_verbage}

	} else /if (${_type.Equal[mealinmanual]}) {
		/call set_switch ${_bind.Lower} swMealinManual ${_verbage}

	} else /if (${_type.Equal[bczonereset]}) {
		/call set_switch ${_bind.Lower} swBCZonereset ${_verbage}

	} else /if (${_type.Equal[bcstop]}) {
		/call set_switch ${_bind.Lower} swBCStop ${_verbage}

	} else /if (${_type.Equal[bctake]}) {
		/call set_switch ${_bind.Lower} swBCTake ${_verbage}

	} else /if (${_type.Equal[bcenter]}) {
		/call set_switch ${_bind.Lower} swBCEnter ${_verbage}

	} else /if (${_type.Equal[bcinvis]}) {
		/call set_switch ${_bind.Lower} swBCInvis ${_verbage}

	} else /if (${_type.Equal[bckillmob]}) {
		/call set_switch ${_bind.Lower} swBCKillMob ${_verbage}

	} else /if (${_type.Equal[bcclean]}) {
		/call set_switch ${_bind.Lower} swBCClean ${_verbage}

	} else /if (${_type.Equal[bcanchor]}) {
		/call set_switch ${_bind.Lower} swBCAnchor ${_verbage}

	} else /if (${_type.Equal[echoalt]}) {
		/call set_switch ${_bind.Lower}swEchosAlt ${_verbage}

	} else /if (${_type.Equal[echospell]}) {
		/call set_switch ${_bind.Lower} swEchosSpell ${_verbage}

	} else /if (${_type.Equal[echodisc]}) {
		/call set_switch ${_bind.Lower} swEchosDisc ${_verbage}

	} else /if (${_type.Equal[echoitem]}) {
		/call set_switch ${_bind.Lower} swEchosItem ${_verbage}

	} else /if (${_type.Equal[echomelee]}) {
		/call set_switch ${_bind.Lower} swEchosMelee ${_verbage}

	} else /if (${_type.Equal[echopulse]}) {
		/call set_switch ${_bind.Lower} swEchosPulse ${_verbage}

	} else /if (${_type.Equal[echobc]}) {
		/call set_switch ${_bind.Lower} swEchosBC ${_verbage}

	} else /if (${_type.Equal[echoverbose]}) {
		/call set_switch ${_bind.Lower} swEchosVerbose ${_verbage}

	} else /if (${_type.Equal[echosql]}) {
		/call set_switch ${_bind.Lower} swEchosSQL ${_verbage}

	}

	/if (${swMQClear}) /mqclear
	| VOUT /${_bind.Lower} 
	/for _countArray 1 to ${lstTypes${_bind}.Size}
		/for _count 1 to ${lstTypes${_bind}[${_countArray}].Count[|]}
			/varset _listout ${_listout} ${If[${_sep},${sep},]} ${cinfo}${lstTypes${_bind}[${_countArray}].Arg[${_count},|].Lower}\ax		
			/if (${lstTypes${_bind}[${_countArray}].Arg[${_count},|].Equal[moblvlmin]}) /varset _listout ${_listout}:${If[${setMobLvlMin},${cnum}${setMobLvlMin}\ax,${ooff}]}
			/if (${lstTypes${_bind}[${_countArray}].Arg[${_count},|].Equal[moblvmax]}) /varset _listout ${_listout}:${If[${setMobLvlMax},${cnum}${setMobLvlMax}\ax,${ooff}]}
			/if (${lstTypes${_bind}[${_countArray}].Arg[${_count},|].Equal[bitch]}) /varset _listout ${_listout}:${If[${swBitch},${oon},${ooff}]}
			/if (${lstTypes${_bind}[${_countArray}].Arg[${_count},|].Equal[nukeloop]}) /varset _listout ${_listout}:${If[${swNukeLoop},${oon},${ooff}]}
			/if (${lstTypes${_bind}[${_countArray}].Arg[${_count},|].Equal[dispinfo]}) /varset _listout ${_listout}:${If[${swDispInfo},${oon},${ooff}]}
			/if (${lstTypes${_bind}[${_countArray}].Arg[${_count},|].Equal[dispbuild]}) /varset _listout ${_listout}:${If[${swDispBuild},${oon},${ooff}]}
			/if (${lstTypes${_bind}[${_countArray}].Arg[${_count},|].Equal[dispexp]}) /varset _listout ${_listout}:${If[${swDispEXP},${oon},${ooff}]}
			/if (${lstTypes${_bind}[${_countArray}].Arg[${_count},|].Equal[manualaadiscnow]}) /varset _listout ${_listout}:${If[${swManualAADiscNow},${oon},${ooff}]}
			/if (${lstTypes${_bind}[${_countArray}].Arg[${_count},|].Equal[delayzoning]}) /varset _listout ${_listout}:${If[${swDelayZoning},${oon},${ooff}]}
			/if (${lstTypes${_bind}[${_countArray}].Arg[${_count},|].Equal[restfull]}) /varset _listout ${_listout}:${If[${swRestFull},${oon},${ooff}]}
			/if (${lstTypes${_bind}[${_countArray}].Arg[${_count},|].Equal[advloot]}) /varset _listout ${_listout}:${If[${swADVLoot},${oon},${ooff}]}
			/if (${lstTypes${_bind}[${_countArray}].Arg[${_count},|].Equal[glyphoutsideraid]}) /varset _listout ${_listout}:${If[${swGlyphOutsideRaid},${oon},${ooff}]}
			/if (${lstTypes${_bind}[${_countArray}].Arg[${_count},|].Equal[mqclear]}) /varset _listout ${_listout}:${If[${swMQClear},${oon},${ooff}]}
			/if (${lstTypes${_bind}[${_countArray}].Arg[${_count},|].Equal[safelistguild]}) /varset _listout ${_listout}:${If[${swSafelistGuild},${oon},${ooff}]}
			/if (${lstTypes${_bind}[${_countArray}].Arg[${_count},|].Equal[eqcash]}) /varset _listout ${_listout}:${If[${swEQCash},${oon},${ooff}]}
			/if (${lstTypes${_bind}[${_countArray}].Arg[${_count},|].Equal[modepersistant]}) /varset _listout ${_listout}:${If[${swModePersistent},${oon},${ooff}]}
			/if (${lstTypes${_bind}[${_countArray}].Arg[${_count},|].Equal[combattie]}) /varset _listout ${_listout}:${If[${swCombatTie},${oon},${ooff}]}
			/if (${lstTypes${_bind}[${_countArray}].Arg[${_count},|].Equal[buffgemhold]}) /varset _listout ${_listout}:${If[${swBuffGemHold},${oon},${ooff}]}
			/if (${lstTypes${_bind}[${_countArray}].Arg[${_count},|].Equal[mqcaptions]}) /varset _listout ${_listout}:${If[${swMQCaptions},${oon},${ooff}]}
			/if (${lstTypes${_bind}[${_countArray}].Arg[${_count},|].Equal[mealinmanual]}) /varset _listout ${_listout}:${If[${swMealinManual},${oon},${ooff}]}
			/if (${lstTypes${_bind}[${_countArray}].Arg[${_count},|].Equal[bczonereset]}) /varset _listout ${_listout}:${If[${swBCZonereset},${oon},${ooff}]}
			/if (${lstTypes${_bind}[${_countArray}].Arg[${_count},|].Equal[bcstop]}) /varset _listout ${_listout}:${If[${swBCStop},${oon},${ooff}]}
			/if (${lstTypes${_bind}[${_countArray}].Arg[${_count},|].Equal[bctake]}) /varset _listout ${_listout}:${If[${swBCTake},${oon},${ooff}]}
			/if (${lstTypes${_bind}[${_countArray}].Arg[${_count},|].Equal[bcenter]}) /varset _listout ${_listout}:${If[${swBCEnter},${oon},${ooff}]}
			/if (${lstTypes${_bind}[${_countArray}].Arg[${_count},|].Equal[bcinvis]}) /varset _listout ${_listout}:${If[${swBCInvis},${oon},${ooff}]}
			/if (${lstTypes${_bind}[${_countArray}].Arg[${_count},|].Equal[bckillmob]}) /varset _listout ${_listout}:${If[${swBCKillMob},${oon},${ooff}]}
			/if (${lstTypes${_bind}[${_countArray}].Arg[${_count},|].Equal[bcclean]}) /varset _listout ${_listout}:${If[${swBCClean},${oon},${ooff}]}
			/if (${lstTypes${_bind}[${_countArray}].Arg[${_count},|].Equal[bcanchor]}) /varset _listout ${_listout}:${If[${swBCAnchor},${oon},${ooff}]}
			/if (${lstTypes${_bind}[${_countArray}].Arg[${_count},|].Equal[echoalt]}) /varset _listout ${_listout}:${If[${swEchosAlt},${oon},${ooff}]}
			/if (${lstTypes${_bind}[${_countArray}].Arg[${_count},|].Equal[echospell]}) /varset _listout ${_listout}:${If[${swEchosSpell},${oon},${ooff}]}
			/if (${lstTypes${_bind}[${_countArray}].Arg[${_count},|].Equal[echodisc]}) /varset _listout ${_listout}:${If[${swEchosDisc},${oon},${ooff}]}
			/if (${lstTypes${_bind}[${_countArray}].Arg[${_count},|].Equal[echoitem]}) /varset _listout ${_listout}:${If[${swEchosItem},${oon},${ooff}]}
			/if (${lstTypes${_bind}[${_countArray}].Arg[${_count},|].Equal[echomelee]}) /varset _listout ${_listout}:${If[${swEchosMelee},${oon},${ooff}]}
			/if (${lstTypes${_bind}[${_countArray}].Arg[${_count},|].Equal[echopulse]}) /varset _listout ${_listout}:${If[${swEchosPulse},${oon},${ooff}]}
			/if (${lstTypes${_bind}[${_countArray}].Arg[${_count},|].Equal[echobc]}) /varset _listout ${_listout}:${If[${swEchosBC},${oon},${ooff}]}
			/if (${lstTypes${_bind}[${_countArray}].Arg[${_count},|].Equal[echoverbose]}) /varset _listout ${_listout}:${If[${swEchosVerbose},${oon},${ooff}]}
			/if (${lstTypes${_bind}[${_countArray}].Arg[${_count},|].Equal[echosql]}) /varset _listout ${_listout}:${If[${swEchosSQL},${oon},${ooff}]}
			/if (${lstTypes${_bind}[${_countArray}].Arg[${_count},|].Equal[mqfontsize]}) /varset _listout ${_listout}:${If[${setMQFontSize},${cnum}${setMQFontSize}\ax,${ooff}]}
			/if (${lstTypes${_bind}[${_countArray}].Arg[${_count},|].Equal[maxbuffcount]}) /varset _listout ${_listout}:${If[${setMaxBuffCount},${cnum}${setMaxBuffCount}\ax,${ooff}]}
			/varset _sep TRUE
		/next _count
		VOUT ${If[${_countArray}==1,/${_bind.Lower} [,]}${_listout}${If[${_countArray}==${lstTypes${_bind}.Size},],]}
		/varset _listout
		/varset _sep FALSE
	/next _countArray
	
/return



|***
 *	DES: Controlls for the HUD
 *	USE: /hc [time|update|delete]
 *  NOTE:
 ***| 
#bind hud /hc
sub Bind_hud(string _type, string _verbage, bool _silent)

	/declare _sep bool local
	/declare _listout string local
	/declare _count int local 0
	/declare _bind string local HUD	

	| adjust hud clock
	/if (${_type.Equal[time]}) {
		/varset swHUDTime ${If[${swHUDTime},FALSE,TRUE]}
		/if (${swHUDTime}) VOUT HUD Time ${sep} ${cinfo}IRL\ax
		/if (!${swHUDTime}) VOUT HUD Time ${sep} ${cinfo}Game\ax


	| reset the MQhud ini file
	} else /if (${_type.Equal[update]}) {
		/if (${Bool[${_verbage}]} ) {
			/call hud_control ${_verbage}
		} else {
			/varset _verbage Core
		 	/call hud_control ${_verbage}
		}

	| delete the hud
	} else /if (${_type.Equal[delete]}) {

		/if (${Bool[${_verbage}]} ) {
			/ini ".\..\MQ2HUD.ini" "${_verbage}" NULL NULL
		} else {
			/varset _verbage Core
			/ini ".\..\MQ2HUD.ini" "core" NULL NULL
		}
		VOUT Deleting MQ2HUD.INI ${sep} ${cinfo}${_verbage.Left[1].Upper}${_verbage.Right[-1].Lower}\ax
		
	} 
	
	/if (${_silent}) /return

	/for _count 1 to ${lstTypes${_bind}.Count[|]}
		/varset _listout ${_listout} ${If[${_sep},${sep},]} ${cinfo}${lstTypes${_bind}.Arg[${_count},|]}\ax
		/varset _sep TRUE
	/next _count
	VOUT /${_bind.Lower} [${_listout}]
/return




|***
 *	DES: admin controls
 *	USE: dont
 *  NOTE: seriously..
 ***| 
#bind admin /admin
sub Bind_admin(string _type, string _verbage, string _verbage2, bool _silent)
	/declare _sep bool local
	/declare _listout string local
	/declare _count int local 0
	/declare _bind string local Admin	

	| echo the notice
	/if (${_type.Equal[notice]}) {
		/mqclear
		/call echo_notice
		/return

	| xtarget control
	} else /if (${_type.Equal[xt]}) {
		/if (${_verbage.Equal[build]}) {
			/declare _tempxtsort ${swHealXTarget}
			
			/if (!${_tempxtsort}) /varset swHealXTarget TRUE
			/call set_xtarget
			/if (!${_tempxtsort}) {
				/varset swHealXTarget FALSE
				/varset timer_set_xtarget 0
			}
		}
		
		/return
		
		
	| place in maintenance mode
	} else /if (${_type.Equal[maintenance]}) {
		/call set_switch character swMaintenance ${_verbage}

	| reset core
	} else /if (${_type.Equal[reset]}) {
		/call initialize_core ${_verbage}
		/return
		
	| migrate old INIS
 	} else /if (${_type.Equal[migrate]}) {
		/call ini_migration ${_type} ${_verbage}
		/return

	| runs the whitewash routine to clear all aliases
	} else /if (${_type.Equal[whitewash]}) {
		/call whitewash
		/return

	| reset current zone
	} else /if (${_type.Equal[zonereset]}) {	
		/call control_zone FALSE
		VOUT Zone ${sep} \awzone\ax::\at${Zone.Name}\ax 
		/return


	| edit kill code
	} else /if (${_type.Equal[killkey]}) {
		/call sql_update FALSE environment remoteKillKey "${_verbage}"

	| return a single variable
	} else /if (${_type.Equal[get]}) {
		/if (!${Bool[${_verbage}]}) {
			VOUT /${_bind.Lower} get [${cnum}variable name\ax]
			/return FALSE
		}
		OUT ${_verbage} ${sep} ${cnum}${sql_return[${_verbage}, dummy]}\ax
		/return

	| list the tables for viewing
	} else /if (${_type.Equal[table]}) {
		/if (!${Bool[${_verbage}]}) {
			/for _count 1 to ${sqlTables[1].Count[|]}
				/varset _listout ${_listout} ${If[${_sep},${sep},]} ${cinfo}${sqlTables[1].Arg[${_count},|]}\ax		
				/varset _sep TRUE
			/next _count
			VOUT /${_bind.Lower} table [${_listout} ${sep} ${cinfo}environment\ax]	
			/return
		}
		/mqclear
		OUT List Table ${sep} ${cnum}${_verbage.Lower}\ax
		/call sql_getcolums FALSE ${_verbage.Lower}
		/return

	| return a single variable
	} else /if (${_type.Equal[list]}) {
		/if (!${Bool[${_verbage}]}) {
			/declare _lstList string local |unity|nuke|dot|buff|gom|clickitem|clickbuff|itemburn|itemswarm|now|odditem|bp|zone|familiar
			/for _count 1 to ${_lstList.Count[|]}
				/varset _listout ${_listout} ${If[${_sep},${sep},]} ${cinfo}${_lstList.Arg[${_count},|]}\ax		
				/varset _sep TRUE
			/next _count
			VOUT /${_bind.Lower} list [${_listout}]	
			/return
		}

		| list buffs	
		/if (${_verbage.Equal[buff]}) {
			/mqclear
			OUT Buff List ${sep} ${If[${Bool[${_verbage2}]},#${Int[${_verbage2}]},]}
			/for _count 1 to 10
				/if (${Bool[${_verbage2}]} && ${_verbage2.NotEqual[${_count}]}) /continue
				OUT Buff${cinfo}${_count}\ax ${sep} ${cnum}${sql_return[Buff${_count}, dummy]}\ax
				OUT Buff${cinfo}${_count}\axself ${sep} ${cnum}${sql_return[Buff${_count}self, dummy]}\ax
				OUT Buff${cinfo}${_count}\axselfCondition ${sep} ${cnum}${sql_return[Buff${_count}selfCondition, dummy]}\ax
				OUT Buff${cinfo}${_count}\axAlias ${sep} ${cnum}${sql_return[Buff${_count}Alias, dummy]}\ax
				OUT BCBuff${cinfo}${_count}\axClass ${sep} ${cnum}${sql_return[BCBuff${_count}Class, dummy]}\ax
				OUT RaidBuff${cinfo}${_count}\axClass ${sep} ${cnum}${sql_return[RaidBuff${_count}Class, dummy]}\ax
				OUT ManualBuff${cinfo}${_count}\axClass ${sep} ${cnum}${sql_return[ManualBuff${_count}Class, dummy]}\ax
				OUT ManualBuff${cinfo}${_count}\axCheck ${sep} ${cnum}${sql_return[ManualBuff${_count}Check, dummy]}\ax
				OUT Buff${cinfo}${_count}\axnoBCinRaid ${sep} ${cnum}${sql_return[Buff${_count}noBCinRaid, dummy]}\ax
			/next _count
			
		| list unity buffs
		} else /if (${_verbage.Equal[unity]}) {
			/mqclear
			OUT AA Unity List ${sep}
			/for _count 1 to 7
				OUT BuffAAUnity${cinfo}${_count}\ax ${sep} ${cnum}${sql_return[BuffAAUnity${_count}, dummy]}\ax
			/next _count
			
		| list nukes
		} else /if (${_verbage.Equal[nuke]}) {
			/mqclear
			OUT Nuke List ${sep} ${If[${Bool[${_verbage2}]},#${Int[${_verbage2}]},]}
			/for _count 1 to 8
				/if (${Bool[${_verbage2}]} && ${_verbage2.NotEqual[${_count}]}) /continue
				OUT Nuke${cinfo}${_count}\ax ${sep} ${cnum}${sql_return[Nuke${_count}, dummy]}\ax
				OUT Nuke${cinfo}${_count}\axCondition ${sep} ${cnum}${sql_return[Nuke${_count}Condition, dummy]}\ax
			/next _count

		| list DoTs
		} else /if (${_verbage.Equal[dot]}) {
			/mqclear
			OUT DoT List ${sep} ${If[${Bool[${_verbage2}]},#${Int[${_verbage2}]},]}
			/for _count 1 to 8
				/if (${Bool[${_verbage2}]} && ${_verbage2.NotEqual[${_count}]}) /continue
				OUT DoT${cinfo}${_count}\ax ${sep} ${cnum}${sql_return[DoT${_count}, dummy]}\ax
				OUT DoT${cinfo}${_count}\axchk ${sep} ${cnum}${sql_return[DoT${_count}chk, dummy]}\ax
				OUT DoT${cinfo}${_count}\axCondition ${sep} ${cnum}${sql_return[DoT${_count}Condition, dummy]}\ax
			/next _count

		| list GoM
		} else /if (${_verbage.Equal[gom]}) {
			/mqclear
			OUT GoM List ${sep}
			OUT GoMNuke ${sep} ${cnum}${sql_return[GoMNuke, dummy]}\ax
			OUT GoMNukeCondition ${sep} ${cnum}${sql_return[GoMNukeCondition, dummy]}\ax
			OUT GoMNuke2 ${sep} ${cnum}${sql_return[GoMNuke2, dummy]}\ax
			OUT GoMNuke2Condition ${sep} ${cnum}${sql_return[GoMNuke2Condition, dummy]}\ax

		| click DPS items
		} else /if (${_verbage.Equal[clickitem]}) {
			/mqclear
			OUT DPS Click List ${sep} ${If[${Bool[${_verbage2}]},#${Int[${_verbage2}]},]}
			/for _count 1 to 6
				/if (${Bool[${_verbage2}]} && ${_verbage2.NotEqual[${_count}]}) /continue
				OUT clickItem${cinfo}${_count}\ax ${sep} ${cnum}${sql_return[clickItem${_count}, dummy]}\ax
			/next _count

		| click DPS items
		} else /if (${_verbage.Equal[clickbuff]}) {
			/mqclear
			OUT Click Buff Item List ${sep} ${If[${Bool[${_verbage2}]},#${Int[${_verbage2}]},]}
			/for _count 1 to 6
				/if (${Bool[${_verbage2}]} && ${_verbage2.NotEqual[${_count}]}) /continue
				OUT clickItemBuff${cinfo}${_count}\ax ${sep} ${cnum}${sql_return[clickItemBuff${_count}, dummy]}\ax
			/next _count
		
		| click BURN DPS items
		} else /if (${_verbage.Equal[itemburn]}) {
			/mqclear
			OUT Burn Click List ${sep} ${If[${Bool[${_verbage2}]},#${Int[${_verbage2}]},]}
			/for _count 1 to 6
				/if (${Bool[${_verbage2}]} && ${_verbage2.NotEqual[${_count}]}) /continue
				OUT clickItemBurn${cinfo}${_count}\ax ${sep} ${cnum}${sql_return[clickItemBurn${_count}, dummy]}\ax
			/next _count

		| click swarm DPS items
		} else /if (${_verbage.Equal[itemswarm]}) {
			/mqclear
			OUT Swarm Click List ${sep} ${If[${Bool[${_verbage2}]},#${Int[${_verbage2}]},]}
			/for _count 1 to 6
				/if (${Bool[${_verbage2}]} && ${_verbage2.NotEqual[${_count}]}) /continue
				OUT clickItemSwarm${cinfo}${_count}\ax ${sep} ${cnum}${sql_return[clickItemSwarm${_count}, dummy]}\ax
			/next _count

		| AA/Disc Now
		} else /if (${_verbage.Equal[now]}) {
			/mqclear
			OUT AA/Disc Now List ${sep} ${If[${Bool[${_verbage2}]},#${Int[${_verbage2}]},]}
			/for _count 1 to 2
				/if (${Bool[${_verbage2}]} && ${_verbage2.NotEqual[${_count}]}) /continue
				OUT AANow${cinfo}${_count}\ax ${sep} ${cnum}${sql_return[AANow${_count}, dummy]}\ax
			/next _count
			/for _count 1 to 2
				/if (${Bool[${_verbage2}]} && ${_verbage2.NotEqual[${_count}]}) /continue
				OUT DiscNow${cinfo}${_count}\ax ${sep} ${cnum}${sql_return[DiscNow${_count}, dummy]}\ax
			/next _count
				
		| odds and ends
		} else /if (${_verbage.Equal[odditem]}) {
			/mqclear
			OUT Odd Items List ${sep} ${If[${Bool[${_verbage2}]},#${Int[${_verbage2}]},]}
			/for _count 1 to 4
				/if (${Bool[${_verbage2}]} && ${_verbage2.NotEqual[${_count}]}) /continue
				OUT clickOddItem${cinfo}${_count}\ax ${sep} ${cnum}${sql_return[clickOddItem${_count}, dummy]}\ax
			/next _count

		| breastplate stacking
		} else /if (${_verbage.Equal[bp]}) {
			/mqclear
			OUT Breastplates List ${sep} ${If[${Bool[${_verbage2}]},#${Int[${_verbage2}]},]}
			/for _count 1 to 2
				/if (${Bool[${_verbage2}]} && ${_verbage2.NotEqual[${_count}]}) /continue
				OUT Breastplate${cinfo}${_count}\ax ${sep} ${cnum}${sql_return[Breastplate${_count}, dummy]}\ax
				OUT Breastplate${cinfo}${_count}\axCondition ${sep} ${cnum}${sql_return[Breastplate${_count}Condition, dummy]}\ax
				OUT Breastplate${cinfo}${_count}\axeffect ${sep} ${cnum}${sql_return[Breastplate${_count}effect, dummy]}\ax
			/next _count

		| breastplate stacking
		} else /if (${_verbage.Equal[familiar]}) {
			/mqclear
			OUT Breastplates List ${sep} ${If[${Bool[${_verbage2}]},#${Int[${_verbage2}]},]}
			/for _count 1 to 3
			/if (${Bool[${_verbage2}]} && ${_verbage2.NotEqual[${_count}]}) /continue
				OUT setFam${cinfo}${_count}\ax ${sep} ${cnum}${sql_return[setFam${_count}, dummy]}\ax
				OUT setFam${cinfo}${_count}\axBuff ${sep} ${cnum}${sql_return[setFam${_count}Buff, dummy]}\ax
				OUT swFam${cinfo}${_count}\axLeave ${sep} ${cnum}${sql_return[swFam${_count}Leave, dummy]}\ax
			/next _count
	

		| current zone list
		} else /if (${_verbage.Equal[zone]}) {
			/mqclear
			/sql ${dbCore} dummy SELECT * FROM zone WHERE ShortName="${Zone.ShortName.Lower}"
			
			OUT \awzone\ax::\aw${Zone.ShortName.Lower}\ax
			OUT ::\awSafeZone\ax ${sep} ${cnum}${SafeZone}\ax
			OUT ::\awNameList\ax ${sep} ${cnum}${NameList}\ax
			OUT ::\awExcludeList\ax ${sep} ${cnum}${ExcludeList}\ax
			OUT ::\awNoMezzList\ax ${sep} ${cnum}${NoMezzList}\ax
			OUT ::\awRangeList\ax ${sep} ${cnum}${RangeList}\ax
			OUT ::\awNoCastList\ax ${sep} ${cnum}${NoCastList}\ax
			OUT ::\awNoMeleeList\ax ${sep} ${cnum}${NoMeleeList}\ax
			OUT ::\awHuntMobList\ax ${sep} ${cnum}${HuntMobList}\ax
			OUT ::\awCharmMobList\ax ${sep} ${cnum}${CharmMobList}\ax
			OUT ::\awNoFireList\ax ${sep} ${cnum}${NoFireList}\ax
			OUT ::\awNoPoisonList\ax ${sep} ${cnum}${NoPoisonList}\ax
			OUT ::\awNoMagicList\ax ${sep} ${cnum}${NoCorruptionList}\ax
			OUT ::\awNoCorruptionList\ax ${sep} ${cnum}${NoCorruptionList}\ax
			OUT ::\awNoColdList\ax ${sep} ${cnum}${NoColdList}\ax
			OUT ::\awNoDiseaseList\ax ${sep} ${cnum}${NoDiseaseList}\ax
			OUT ::\awHarvestList\ax ${sep} ${cnum}${HarvestList}\ax
			
		}

		| end of /admin list commands			
		/return


	}

	/if (${_silent}) /return
	/if (${swMQClear}) /mqclear	
	/for _count 1 to ${lstTypes${_bind}.Count[|]}
		/varset _listout ${_listout} ${If[${_sep},${sep},]} ${cinfo}${lstTypes${_bind}.Arg[${_count},|]}\ax
		/if (${lstTypes${_bind}.Arg[${_count},|].Equal[maintenance]}) /varset _listout ${_listout}:${If[${swMaintenance},${oon},${ooff}]}
		/varset _sep TRUE
	/next _count
	VOUT /${_bind.Lower} [${_listout}]
/return



|***
 *	DES: displays the port and builds the command to the porting class
 *	USE: /port 
 *  NOTE: expect sarcasim if you cannot spell
 ***| 
#bind port_list /port
sub Bind_port_list(string _class, string _location, string _sublocation)
	/declare _listout 				string 	local
	/declare _listouttype			string 	local
	/declare _sep 						bool 		local FALSE
	/declare _country 				int 		local 0
	/declare _count 					int 		local 0
	/declare _state 					int 		local 0
	/declare _portclass 			string 	local |druid|wizard|item|guild
	| /declare _porttype	 			string 	local |tl|group
	/declare _continent 			string 	local |alaris|antonica|discord|faydwer|kunark|luclin|odus|serpentsspine|taelosia|planes|other
	/declare _tell						bool		local FALSE

	/declare _tmpItem string local FALSE
	/declare _tmpLocation string local FALSE


	/if (${_class.Equal[guild]}) {
		
		| The Burning Lands fire disc thingie		
		/if (${_location.Equal[TBL]}) {
			/varset _tmpItem Stratos Fire Platform
			/varset _tmpLocation Teleport to Stratos

		| The Broken Mirror
		} else /if (${_location.Equal[TBM]}) {
			/varset _tmpItem Cynosure of Health
			/varset _tmpLocation Teleport to the Plane of Health
			
		| Empires of Kunark Statue
		} else /if (${_location.Equal[EOK]}) {
			/varset _tmpItem Ancient Iksar Translocator Statue
			/varset _tmpLocation Teleport to Frontier Mountains
			
		| Halas Plackard
		} else /if (${_location.Equal[Halas]}) {
			/varset _tmpItem Placard: Halas
			/varset _tmpLocation Teleport to Frontier Mountains
			
		| surefall Glades Ruby pillar thingie
		} else /if (${_location.Equal[Surefall]}) {
			/varset _tmpItem Brazier: The Everburning Ruby
			/varset _tmpLocation Teleport to Surefall Glades
			
		| neriak commons painting
		} else /if (${_location.Equal[neriak1]}) {
			/varset _tmpItem Painting: Toadstool Tavern
			/varset _tmpLocation Teleport to Neriak - Commons
			
		| neriak commons painting
		} else /if (${_location.Equal[neriak2]}) {
			/varset _tmpItem Painting: The Blind Fish
			/varset _tmpLocation Teleport to Neriak - Commons
			
		| neriak 3rd gate bar
		} else /if (${_location.Equal[neriak3]}) {
			/varset _tmpItem Painting: Maiden's Fancy
			/varset _tmpLocation Teleport to Neriak - Third Gate

		| neriak foreign quarter
		} else /if (${_location.Equal[neriakf]}) {
			/varset _tmpItem Painting: Slugs Tavern
			/varset _tmpLocation Teleport to Neriak - Foreign Quarter

		| PoM ugly assed face picture
		} else /if (${_location.Equal[PoM]}) {
			/varset _tmpItem Painting: Haunting Laughter
			/varset _tmpLocation Teleport to Stare Into Madness
			
		| High Hold Snake Tapestry
		} else /if (${_location.Equal[HighHold]}) {
			/varset _tmpItem Banner: The Knotted Serpent
			/varset _tmpLocation Teleport to High Keep
			
		| qeynos tapestry
		} else /if (${_location.Equal[qeynos]}) {
			/varset _tmpItem Banner: Traveler's Tapestry
			/varset _tmpLocation Teleport to North Qeynos

		| Shabby Lobby Door
		} else /if (${_location.Equal[lobby]}) {
			/varset _tmpItem Shabby Lobby Door
			/varset _tmpLocation Open the Door to the Lobby

		| Gorowyn Lantern
		} else /if (${_location.Equal[RoS]}) {
			/varset _tmpItem Gorowyn Translocator Lantern
			/varset _tmpLocation Teleport to Skyfire Mountains
			
		}		
			
		| found an item
		/if (${Bool[${_tmpItem}]}) {	
			/squelch /itemtarget "${_tmpItem}"
			/squelch /click right item
			/delay 1s
			/squelch /notify "${_tmpLocation}" menuselect
			/return
		}		

		
		/if (${swMQClear}) /mqclear
		/for _count 1 to ${lstTypesPortGuildHall.Count[|]}
			/varset _listout ${_listout} ${If[${_sep},${sep},]} ${cinfo}${lstTypesPortGuildHall.Arg[${_count},|].Lower}\ax
			/varset _sep TRUE
		/next _count
		VOUT /port guild [${_listout}]		
		/return

	} else /if (${_class.Equal[item]}) {
		
		| primary anchor		
		/if (${_location.Equal[primary]}) {
			/varset _tmpItem Primary Anchor Transport Device
			
		| secondary anchor
		} else /if (${_location.Equal[secondary]}) {
			/varset _tmpItem Secondary Anchor Transport Device
			
		| magician summoned door
		} else /if (${_location.Equal[door]}) {
			/varset _tmpItem Bulwark of Many Portals
		
		| PoP port click
		} else /if (${_location.Equal[binden]}) {
			/if (${Me.ItemReady[The Binden Concerrentia]}) {
				/varset _tmpItem The Binden Concerrentia
			} else /if (${Me.ItemReady[The Fabled Binden Concerrentia]}) {
				/varset _tmpItem The Fabled Binden Concerrentia
			} 			
			
			/if (!${FindItemCount[Quintessence of Knowledge]}) {
				VOUT ${owarning} NO ${sep} \atQuintessence of Knowledge\ax
				/return FALSE
			}

		| drunkards stein
		} else /if (${_location.Equal[stein]}) {
			/varset _tmpItem Drunkard's Stein
			
		| TBM xpac mirror
		} else /if (${_location.Equal[mirror]}) {
			/varset _tmpItem Mirror Fragment of Anashti Sul
			
		| EoK port clickie
		} else /if (${_location.Equal[iksarstone]}) {
			/varset _tmpItem Ancient Iksar Translocator Stone

		| TSS necklace portal
		} else /if (${_location.Equal[warportal]}) {
			/varset _tmpItem Necklace of Warportal Fragments
			/if (!${FindItemCount[Small Portal Fragments]})	{
				VOUT ${owarning} NO ${sep} \atSmall Portal Fragments\ax
				/return FALSE
			}
			
		| PoK port brick
		} else /if (${_location.Equal[pokbrick]}) {
			/varset _tmpItem Brick of Knowledge

		| fellowship clickie for those that have friends
		} else /if (${_location.Equal[fellowship]}) {
			/varset _tmpItem Fellowship Registration Insignia

		} else /if (${_location.Equal[brells]}) {
			/varset _tmpItem Mark of Brell

		} else /if (${_location.Equal[scepter]}) {
			/varset _tmpItem Scepter of Draconic Calling

		| TBL port lamp
		} else /if (${_location.Equal[lamp]}) {
			
			/if (!${Bool[${_sublocation}]}) {
				/declare _sublist string local |zephyr|palace|stone
				/declare _sublistout string local

				/for _count 1 to ${_sublist.Count[|]}
					/varset _sublistout ${_sublistout} ${If[${_sep},${sep},]} ${cinfo}${_sublist.Arg[${_count},|]}\ax
					/varset _sep TRUE
				/next _count
				VOUT /port item slide [${_sublistout}]
				/return FALSE
			}			
			
			/if (${_sublocation.Equal[Zephyr]}) {
				/while (!${FindItem[Wishing Lamp: Zephyr's Flight].ID}) {
					/convertitem Wishing Lamp:
					/delay 1s					
				}
				/varset _tmpItem Wishing Lamp: Zephyr's Flight
			} else /if (${_sublocation.Equal[Palace]}) {
				/while (!${FindItem[Wishing Lamp: Palace of Embers].ID}) {
					/convertitem Wishing Lamp:
					/delay 1s					
				}
				/varset _tmpItem Wishing Lamp: Palace of Embers
			} else /if (${_sublocation.Equal[Stone]}) {
				/while (!${FindItem[Wishing Lamp: The Stone Demesne].ID}) {
					/convertitem Wishing Lamp:
					/delay 1s					
				}
				/varset _tmpItem Wishing Lamp: The Stone Demesne				
			}

		| signet of the crystal circle
		} else /if (${_location.Equal[rofsignet]}) {
			/varset _tmpItem Signet of the Crystal Circle

		| jaggedpine ring from epic ornament quests
		} else /if (${_location.Equal[jaggedpine]}) {
			/varset _tmpItem Ring of Jaggedpine

		| tolans Ranger BP
		} else /if (${_location.Equal[tolans]}) {
			/varset _tmpItem Tolan's Darkwood Breastplate

		| TBM port clickie
		} else /if (${_location.Equal[touchstone]}) {
			/varset _tmpItem Touchstone of Health

		| ROS port clickie
		} else /if (${_location.Equal[gorowyn]}) {
			/varset _tmpItem Gorowyn Translocator Beacon

		| zueria slide
		} else /if (${_location.Equal[slide]}) {

			/if (!${Bool[${_sublocation}]}) {
				/declare _sublist string local |stonebrunt|greatdivide|ro|nektulos|dreadlands|skyfire
				/declare _sublistout string local

				/for _count 1 to ${_sublist.Count[|]}
					/varset _sublistout ${_sublistout} ${If[${_sep},${sep},]} ${cinfo}${_sublist.Arg[${_count},|]}\ax
					/varset _sep TRUE
				/next _count
				VOUT /port item slide [${_sublistout}]
				/return FALSE
			}
					
			/if (${_sublocation.Equal[Stonebrunt]}) {
				/while (!${FindItem[Zueria Slide: Stonebrunt].ID}) {
					/convertitem Zueria Slide:
					/delay 1s					
				}
				/varset _tmpItem Zueria Slide: Stonebrunt
			
			} else /if (${_sublocation.Equal[GreatDivide]}) {
				/while (!${FindItem[Zueria Slide: Great Divide].ID}) {
					/convertitem Zueria Slide:
					/delay 1s					
				}
				/varset _tmpItem Zueria Slide: Great Divide
			
			} else /if (${_sublocation.Equal[Ro]}) {
				/while (!${FindItem[Zueria Slide: North Ro].ID}) {
					/convertitem Zueria Slide:
					/delay 1s					
				}
				/varset _tmpItem Zueria Slide: North Ro
			
			} else /if (${_sublocation.Equal[Nektulos]}) {
				/while (!${FindItem[Zueria Slide: Nektulos].ID}) {
					/convertitem Zueria Slide:
					/delay 1s					
				}
				/varset _tmpItem Zueria Slide: Nektulos
			
			} else /if (${_sublocation.Equal[Dreadlands]}) {
				/while (!${FindItem[Zueria Slide: Dreadlands].ID}) {
					/convertitem Zueria Slide:
					/delay 1s					
				}
				/varset _tmpItem Zueria Slide: Dreadlands
			
			} else /if (${_sublocation.Equal[Skyfire]}) {
				/while (!${FindItem[Zueria Slide: Skyfire].ID}) {
					/convertitem Zueria Slide:
					/delay 1s					
				}
				/varset _tmpItem Zueria Slide: Skyfire
			
			}

		| arx key
		} else /if (${_location.Equal[arx]}) {
			/varset _tmpItem Arx Key

		} else /if (${_location.Equal[stratos]}) {
			/varset _tmpItem Stratos Air Platform

		} else /if (${_location.Equal[faithstone]}) {
			/declare _myDiety ${Me.Diety}
			/declare _myRace ${Me.Race}
			/if (${_myDiety.Equal[Bertoxxulous]}) /varset _tmpItem Faithstone of Decay
			/if (${_myDiety.Equal[Brell Serilis]}) /varset _tmpItem Faithstone of Underfoot
			/if (${_myDiety.Equal[Bristlebane]}) /varset _tmpItem Faithstone of Mischief
			/if (${_myDiety.Equal[Cazic-Thule]}) /varset _tmpItem Faithstone of Fear
			/if (${_myDiety.Equal[Innoruuk]} && ${_myRace.Equal[Human]}) /varset _tmpItem Faithstone of Spite
			/if (${_myDiety.Equal[Innoruuk]} && ${_myRace.Equal[Dark Elf]}) /varset _tmpItem Faithstone of Hate
			/if (${_myDiety.Equal[Karana]}) /varset _tmpItem Faithstone of Thunder
			/if (${_myDiety.Equal[Mithaniel Marr]}) /varset _tmpItem Faithstone of Courage
			/if (${_myDiety.Equal[Prexus]}) /varset _tmpItem Faithstone of the Learned
			/if (${_myDiety.Equal[Rallos Zek]}) /varset _tmpItem Spiritstone of the Feerott
			/if (${_myDiety.Equal[Rodcet Nife]}) /varset _tmpItem Faithstone of Life
			/if (${_myDiety.Equal[The Tribunal]}) /varset _tmpItem Faithstone of Deep Musing
			/if (${_myDiety.Equal[Tunaree]}) /varset _tmpItem Faithstone of Nature
			/if (${_myDiety.Equal[Veeshan]}) /varset _tmpItem Faithstone of the Dedicated

		| gyrospear instant click item
		} else /if (${_location.Equal[gyrospire]}) {
			/varset _tmpItem Gyrospire Relocation Device
				
		}
			
		| found an item
		/if (${Bool[${_tmpItem}]}) {	
			/if (!${Me.ItemReady[${_tmpItem}]}) /return FALSE
			/if (${Me.Invis}) /makemevisible
			/if (!${swModePersistent}) /call switch_clear_mode
			/call set_stop NOECHO
			/call clear_combat
			/delay 1s !${Me.Casting.ID}
			/if (${validate_cast[FALSE, item, "${_tmpItem}", ${Me.ID}]}) /call core_cast2 "${_tmpItem}" item 0 FALSE
			/return
		}
		
		/if (${swMQClear}) /mqclear
		/for _count 1 to ${lstTypesPortItem.Count[|]}
			/varset _listout ${_listout} ${If[${_sep},${sep},]} ${cinfo}${lstTypesPortItem.Arg[${_count},|].Lower}\ax
			/varset _sep TRUE
		/next _count
		VOUT /port item [${_listout}]		
		/return
	}


	| druid port list
	/declare _portDruidAlaris 											string local |beast|pillars|shardslanding
	/declare _portDruidAntonica  										string local |commonlands|feerrott|karana|lavastorm|misty|ro|surefall|undershore|westkarna
	/declare _portDruidDiscord 											string local |bloodfields|wallofslaughter
	/declare _portDruidFaydwer 											string local |butcherblock|looping|steamfront
	/declare _portDruidKunark 											string local |dreadlands|lceanium
	/declare _portDruidLuclin 											string local |dawnshroud|grimling|nexus|twilight
	/declare _portDruidOdus 												string local |buriedsea|stonebrunt|tempesttemple|tox
	/declare _portDruidSerpentsspine							 	string local |blightfire|direwind|steppes
	/declare _portDruidTaelosia 										string local |barindu|natimbi
	/declare _portDruidPlanes 											string local |grounds|potime|brells|arcstone|knowledge|esianti
	/declare _portDruidVelious 											string local |cobaltscar|wakening|greatdivide|iceclad
	/declare _portDruidOther 												string local |primarypush|secondarypush|primary|secondary

	| wizard port list
	/declare _portWizardAlaris 											string local |pillars|sarith 
	/declare _portWizardAntonica 										string local |northkarana|commonlands|nektulos|qeyenos|northro|cazic|undershore|westkarana
	/declare _portWizardDiscord 										string local |wallofslaughter|bloodfields
	/declare _portWizardFaydwer 										string local |fay|dragonscale
	/declare _portWizardKunark 											string local |lceanium|dreadlands|skyfire
	/declare _portWizardLuclin 											string local |dawnshroud|twilight|grimling
	/declare _portWizardOdus 												string local |tempesttemple|katta
	/declare _portWizardSerpentsspine							 	string local |tox|stonebrunt|icefall|sunderock|blightfire
	/declare _portWizardTaelosia 										string local |barindu|natimbi
	/declare _portWizardPlanes 											string local |grounds|potime|brells|arcstone|pok|esianti
	/declare _portWizardVelious 										string local |cobaltscar|wakening|greatdivide|iceclad
	/declare _portWizardOther 											string local |bind|nexus|primary|secondary|primarypush|secondarypush
			
	| sort the class
	/if (${Bool[${_class.Find[druid]}]}) {
		/varset _class Druid
	} else /if (${Bool[${_class.Find[wizard]}]}) {
		/varset _class Wizard
	} else {
		| build class types
		/for _count 1 to ${_portclass.Count[|]}
			/varset _listout ${_listout} ${If[${_sep},${sep},]} ${cinfo}${_portclass.Arg[${_count},|]}\ax
			/varset _sep TRUE
		/next _count
		/varset _sep FALSE

		| VOUT /port [${_listout}] [${cinfo}location\ax] [${_listouttype}]
		VOUT /port [${_listout}] [${cinfo}location\ax]
		/return
	}
	
  | echo locations if class correct
	/if (${Bool[${_class}]} && !${Bool[${_location}]}) {
		/for _country 1 to ${_continent.Count[|]}
			/for _state 1 to ${_port${_class}${_continent.Arg[${_country},|].Left[1].Upper}${_continent.Arg[${_country},|].Right[-1].Lower}.Count[|]}
				/varset _listout ${_listout} ${If[${_sep},${sep},]} ${cinfo}${_port${_class}${_continent.Arg[${_country},|].Left[1].Upper}${_continent.Arg[${_country},|].Right[-1].Lower}.Arg[${_state},|]}\ax
				/varset _sep TRUE
			/next _state
			VOUT ${_continent.Arg[${_country},|].Left[1].Upper}${_continent.Arg[${_country},|].Right[-1].Lower} [${_listout}] 
			/varset _listout
			/varset _sep
		/next _country

	| request port when class and location are present
	} else /if (${Bool[${_class}]} && ${Bool[${_location}]}) {
		/dex ${get_resident[${_class}, 100]} /taxi_service "${Me.DisplayName}" "${_location}"
	}

/return


|***
 *	DES: make the toon stop its shit and splash at its feet
 *	USE: /dex NAME /splash | /splash
 *  NOTE: 
 ***|
#bind ae_splash /splash
sub Bind_ae_splash()
	/if (!${validate_class[TRUE, |PAL|CLR]}) /return FALSE

	| splash
	VOUT Splashing these dumb asses ${sep}
	
	| paladin
	/if (${Me.Class.ShortName.Equal[PAL]}) {
		/if (!${Me.Gem[${Spell[${splashCure}].RankName}]}) {
			/call mem_spell TRUE "${splashCure}" ${buffGem} FALSE
			/return
		}
		/if (${validate_cast[FALSE, spell, "${splashCure}", ${Me.ID}]}) /cast "${Spell[${splashCure}].RankName}" loc ${Me.X} ${Me.Y} ${Me.Z}
	}
	
	| cleric
	/if (${Me.Class.ShortName.Equal[CLR]}) {
		/if (!${Me.Gem[${Spell[${healSplash}].RankName}]}) {
			/call mem_spell TRUE "${healSplash}" ${buffGem} FALSE
			/return
		}
		/if (${validate_cast[FALSE, spell, "${healSplash}", ${Me.ID}]}) /cast "${Spell[${healSplash}].RankName}" loc ${Me.X} ${Me.Y} ${Me.Z}
	}
	
/return TRUE

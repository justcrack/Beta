|***
 *	custom.inc
 *	this is a blank structure file
 *	feel free to add/create your own commands whatever
 *	use the other files as a templates if you need examples
 *  the set_custom_declares() will be loded on mac start if you need to add your own variables
 *
 ***|





|***
 *	DES: custom declares
 *	USE: /call set_declares_custom
 *  NOTE: This function is initialized for you in the macs base prep routine.
 ***|
sub set_declares_custom()

/return




|***custom builds for raid vs group play
 *	USE: /build [raid|group]
 *  NOTE: this is for me, not you ;)
 ***|
#bind build_swap /build
sub Bind_build_swap(string _build)


	/if (!${Bool[${_build}]}) {
		VOUT ${owarning} ${sep} \aw/build\ax [${cnum}name\ax] needs a build name
	}


	/if (${_build.Equal[raid]}) {
		
		OUT Set Raid Build
		
		/call Bind_setCLR retort off TRUE
		/call Bind_setCLR grouphot off TRUE
		/call Bind_setCLR arbitrate off TRUE
		
		/call Bind_buff self off TRUE
		/call Bind_buff bc off TRUE
		/call Bind_buff tell off TRUE
		
		/call Bind_rez take off TRUE
		/call Bind_rez give off TRUE
		
		/call Bind_burn raid off TRUE
		/call Bind_burn auto off TRUE
		/call Bind_burn count 99 TRUE
		
		/showspelleffects off
		
	} else /if (${_build.Equal[group]}) {
		
		OUT Set Group Build
	
		/call Bind_setCLR retort on TRUE
		/call Bind_setCLR grouphot on TRUE
		/call Bind_setCLR arbitrate on TRUE
		
		/call Bind_buff self on TRUE
		/call Bind_buff bc on TRUE
		/call Bind_buff tell on TRUE
		
		/call Bind_rez take on TRUE
		/call Bind_rez give on TRUE
		
		/call Bind_burn raid on TRUE
		/call Bind_burn auto on TRUE
		/call Bind_burn count 3 TRUE

		/showspelleffects on

		
	}

/return






#bind build_backup /backup
sub Bind_build_backup(string _build)

	/declare _stampDate 		string local ${Time.Year}${If[${Time.Month}<10,0${Time.Month},${Time.Month}]}${If[${Time.Day}<10,0${Time.Day},${Time.Day}]}
	/declare _stampTime 		string local ${If[${Time.Hour}<10,0${Time.Hour},${Time.Hour}]}${If[${Time.Minute}<10,0${Time.Minute},${Time.Minute}]}${If[${Time.Second}<10,0${Time.Second},${Time.Second}]}
	/declare _dbCoreBackup 	string local macros\tc\data\dbCore-${_stampDate}${_stampTime}.db

	| /echo backup::${_dbCoreBackup}


| /echo ${_stampDate}${_stampTime}
	| /sqlite ${_dbCoreBackup} dummy CREATE TABLE environment (currentBuild TEXT UNIQUE)
	| /sqlite ${dbCore} dummy INSERT INTO ${_dbCoreBackup}.environment SELECT * FROM environment;



	| /if (${sqlite.Status[dummy].NotEqual[SUCCESS]}) {
	| 	VSQL \arWARNING\ax something is wrong \at..\ax \awdbCore.db\ax -> \aw${_dbCoreBackup}.db\ax
	| } 

| /exec copy .\..\tc\data\dbCore.db dbCore${_stampDate}${_stampTime}.db


| /exec copy "e:\dbCore.db e:\dbCore12.db" bg



/sqlite ${dbCore} query_journal_mode PRAGMA journal_mode
/echo ${sqlite.result[query_journal_mode 1 journal_mode]}


/return

	/sqlite ${_dbCore} dummy INSERT INTO ${_dbCoreBackup}.environment SELECT * FROM environment


${sqlite.Rows[dummy]}

/sqlite DB query PRAGMA journal_mode=WAL

Knightly: /sqlite DB query PRAGMA journal_mode
[10:40 PM] Knightly: ${sqlite.result[query 1 journal_mode]}
[10:40 PM] Knightly: Will tell you the journal mode






sub Pull()
	/if (${DMZ} && ${Me.InInstance}==FALSE) /return
	/if (${Select[${Role},puller,pullertank,hunter,hunterpettank,pullerpettank]}==0) /return
	/if (!${Pulling}) /return
	/if (${DPSPaused}) /return
	/if (${DebugPull}) /echo \atDEBUGPULL Pull Enter \agLine#: ${Macro.CurLine}
	/declare PullAttempts int local 0 
	/declare StuckCount int local 0 
	/declare PullDist float local ${PullRange}
	/declare WasTwistingPull bool local ${Twist}
	/declare AdvpathPaused int local 0
	/declare WPCurrent int local 0
	/declare AdvpathDirection string local
	/declare AutoFireOff int local ${AutoFireOn}
	/declare X1 int local
	/declare Y1 int local
	/declare X2 int local
	/declare Y2 int local
	/declare AdjustHeadingTimer timer local 0
	/declare WasInRange int local 0
	/varset Pulled 0
	/varset PullTooFar 0
	/varset PullTimer 50
	/varset CantHit 0
	/varset ToClose 0

	SDEBUG \atDEBUGPULL Pull: PullWith:${PullWith} PullDist: ${PullDist} \agLine#: ${Macro.CurLine}
	| Set autofire setting off during pulls if not using ranged item to pull
	/if (${AutoFireOff}) /varset AutoFireOn 0
	/if (${IAmABard} && !${PullTwistOn} && ${WasTwistingPull}) { 
		/while (${Me.BardSongPlaying}) {
			/if (${Twist}) /squelch /twist off
			/stopsong
			/delay 5
		}
	}
	
	/if (!${Me.Mount.ID} && ${Me.Sitting}) /stand  
      
	if (${Select[${Role},pullerpettank]} && ${PullRoleToggle} && ${Group.Puller.ID}!=${Me.ID}) {
		/if (${Math.Distance[${Spawn[${MyTargetID}].Y},${Spawn[${MyTargetID}].X},${Spawn[${MyTargetID}].Z}:${CampYLoc},${CampXLoc},${CampZLoc}]}>${CampRadius}) /call PullModeToggle TurnOn
	}

	/varset BeginMobID ${Me.XTarget[${XTSlot}].ID}

	:PullAgain
	
	/doevents 
	/if (!${DragCorpse} && ${SpawnCount[pccorpse ${Me} radius ${MaxRadius}]}) {
		/call GrabCorpse
		/if (${DragCorpse}) {
			/goto :DonePulling
		}
	}
	
	/if (${DPSPaused}) /return  
|

 vars used to determine if we are stuck
/varset X1 ${Int[${Me.X}]}
/varset Y1 ${Int[${Me.Y}]}            
/if (${PullAggroTargetID}) {
/if (${DebugPull}) /echo Pulling 1.1 \agLine#: ${Macro.CurLine}
/varset Pulled 1
/varset MyTargetID ${AggroTargetID}
/varset MyTargetName ${Spawn[${AggroTargetID}].CleanName}
/call StopMoving
/if (${DebugPull}) /echo \atDEBUGPULL Pull Aggro detected \agLine#: ${Macro.CurLine}
/goto :DonePulling
}
| Exit pull and reset if timed out or wandered too far from camp
/if (${DebugPull}) /echo Pulling 1.2 PullTimer: ${PullTimer} Distance: ${Math.Distance[${CampYLoc},${CampXLoc}]} Distance3D: ${Math.Distance[${CampYLoc},${CampXLoc},${CampZLoc}]} MaxRadius: ${Math.Calc[${MaxRadius}*.90]} PullAttempts: ${PullAttempts} MyTargetID: ${Spawn[${MyTargetID}].ID}  Target LOS: ${Spawn[${MyTargetID}].LineOfSight} \agLine#: ${Macro.CurLine}
/if (${PullTimer}==0 || (${Math.Distance[${CampYLoc},${CampXLoc},${CampZLoc}]} >= ${Math.Calc[${MaxRadius}*.90]}) || (${PullAttempts}>=7 && !${Spawn[${MyTargetID}].LineOfSight} && !${Select[${Role},hunter,hunterpettank]}) || !${Spawn[${MyTargetID}].ID} || ${PullAttempts}>=100) {
/if (${MyTargetID}) /squelch /alert add 1 id ${MyTargetID}
/if (${DebugPull}) /echo \atDEBUGPULL Pull: /echo Adding ${Spawn[${MyTargetID}].CleanName} ID: ${MyTargetID} to temp ignore list \agLine#: ${Macro.CurLine}
/if (${DebugPull}) /echo \atDEBUGPULL Pull: Done Pulling-Timer expired, Mob unreachable or exceeded max pull radius. \agLine#: ${Macro.CurLine}
/call BackToCampReset
/if (${Macro.Return}) /return
}
/if (((${AggroTargetID} && !${ChainPull}) || (${Me.XTarget[${XTSlot2}].ID} && ${ChainPull})) && ${Math.Distance[${CampYLoc},${CampXLoc}]} < ${CampRadius}) {
/echo Looks like mobs in camp aborting pull.
/call PullReset
/if (!${ChainPull}) {
/if (${DPSOn} || ${MeleeOn}) {
/call CheckForCombat 0 Pull1
} else {
/call CheckForCombat 1 Pull2
}
}
/if (${DebugPull}) /echo \atDEBUGPULL Pull Mobs in camp detected \agLine#: ${Macro.CurLine}
/return
}
/if (${PullWith.Equal[Ranged]}) {
/call PullWithRanged ${PullDist} 0
}
| - Filter to prevent pulling until AA/Disc/Spell/Item is ready.
/if (${DebugPull}) /echo \atDEBUGPULL Check Ability: ${AggroTargetID} ${PullAggroTargetID} ${ChainPull} ${Select[${PullWith},Melee,Pet,Ranged]} ${Me.SpellReady[${PullWith}]} ${Me.AltAbilityReady[${PullWith}]} ${Me.CombatAbilityReady[${PullWith}]} ${Me.ItemReady[${PullWith}]} ${PullWith} ${PullWith.Equal[Ranged]} ${Me.RangedReady} \agLine#: ${Macro.CurLine}
/if (${PullTimer} && (!${AggroTargetID} && !${ChainPull}) && ((!${Select[${PullWith},Melee,Pet,Ranged]} && !${Select[TRUE,${Me.SpellReady[${PullWith}]},${Me.AltAbilityReady[${PullWith}]},${Me.CombatAbilityReady[${PullWith}]},${Me.ItemReady[${PullWith}]}]}) || (${PullWith.Equal[Ranged]} && !${Me.RangedReady}))) /goto :PullAgain
/if (${DebugPull}) /echo \atDEBUGPULL Pull Starting \agLine#: ${Macro.CurLine}
/varset PullAttempts 0
| Set group role puller to adjust for merc running up while pulling if soloing
/if (${Group}==1 && !${Group.Puller.Name.Equal[${Me}]} && ${Select[${Role},puller]} && ${Spawn[=${MainAssist}].Type.Equal[Mercenary]}) /call AssignGroupRole set "${Me.CleanName}" 3
|------------------------------------------------------------------------Pull Begin Move-------------------------------------------------------------------------------------------|            
/if ((${Spawn[${MyTargetID}].Distance3D}>${PullDist} || !${Spawn[${MyTargetID}].LineOfSight}) && ${Math.Distance[${Spawn[${MyTargetID}].Y},${Spawn[${MyTargetID}].X},${Spawn[${MyTargetID}].Z}:${CampYLoc},${CampXLoc},${CampZLoc}]}<${MaxRadius}) {
:WeThereYet
|------------------------------------------------------------------------MQ2AdvPath-------------------------------------------------------------------------------------------|                
| Advpath pull does not use max pull radius. It uses pullwith radius
/if (${PullMoveUse.Equal[advpath]}) {
/call PullUsingAdvPath ${MyTargetID} ${PullDist}
/if (!${MyTargetID}) /return
|------------------------------------------------------------------------MQ2Nav-------------------------------------------------------------------------------------------|
} else /if (${PullMoveUse.Equal[nav]}) {
/call PullUsingNav ${MyTargetID} ${PullDist} 
/if (!${MyTargetID}) /return
|------------------------------------------------------------------------LOS-------------------------------------------------------------------------------------------|
| We are pulling by LOS.  
} else /if (${Spawn[${MyTargetID}].LineOfSight}) {
/if (${Me.FeetWet}) {
/if (${X2}==0) /varcalc PullDist ${PullDist}-(${Spawn[${MyTargetID}].Distance3D}-${Spawn[${MyTargetID}].Distance})
/moveto id ${MyTargetID} mdist ${PullDist} uw
} else {
/moveto id ${MyTargetID} mdist ${PullDist}
}
/delay 10
/varset X2 ${Int[${Me.X}]}
/varset Y2 ${Int[${Me.Y}]}            
/if (${DebugPull}) /echo \atDEBUGPULL Pull LOS ${Spawn[${MyTargetID}].LineOfSight} ${PullDist} \agLine#: ${Macro.CurLine}
|------------------------------------------------------------------------Mob OOR Return to Camp-------------------------------------------------------------------------------------------|                    
} else {
/if (${X2}!=0) { 
/if (${Math.Distance[${Me.Y},${Me.X}:${Y2},${X2}]}>200 || ${PullTimer}==0) {
/echo Mob is no Longer in LOS. Returning to Camp.
/if (${MyTargetID}) /squelch /alert add 1 id ${MyTargetID}
/if (${MoveTo.Moving}) /moveto off
/call BackToCampReset
/if (${Macro.Return}) /return
} else /if ((!${Me.Moving} || !${MoveTo.Moving}) && ${PullDist}<${Spawn[${MyTargetID}].Distance}) {
/if (${Me.FeetWet}) {
/moveto id ${MyTargetID} mdist ${PullDist} uw
} else {
/moveto id ${MyTargetID} mdist ${PullDist}
}
/delay 10
}
}
/if (${DebugPull}) /echo \atDEBUGPULL Pull NO-LOS ${Spawn[${MyTargetID}].LineOfSight} ${Spawn[${MyTargetID}].Distance3D} ${PullDist} \agLine#: ${Macro.CurLine}
}
}
|------------------------------------------------------------------------End Pull Move-------------------------------------------------------------------------------------------|            
/if (${Spawn[${MyTargetID}].Distance3D}<=${PullDist} && ${Target.FeetWet}==${Me.FeetWet}) /varset WasInRange 1
/if (${Math.Distance[${Spawn[${MyTargetID}].Y},${Spawn[${MyTargetID}].X},${Spawn[${MyTargetID}].Z}:${CampYLoc},${CampXLoc},${CampZLoc}]}>${MaxRadius}) {
/if (!${PullIgnore1.Find[|${MyTargetID}]} && ${MyTargetID}>0) {
/squelch /alert add 1 id ${MyTargetID}
/varset PullIgnore1 ${PullIgnore1}|${MyTargetID}
/echo Added ${MyTargetID} to ignor pull list. ${PullIgnore1}
}
}
/varset CantSee 0
/doevents
/if (${Pulled}) /goto :DonePulling
|Checking to see if you are far from camp to try and fix the exceed to far from camp check.
/if ((${Select[${PullMoveUse},los,nav]}>=1 && ${Math.Distance[${CampYLoc},${CampXLoc},${CampZLoc}]}>=${Math.Calc[${MaxRadius}*.90]})) /goto :PullAgain
/if (${DPSPaused}) /return 
| Extending PullTimer if moving closer or target moving && !${Select[${Role},hunter]}
/if (${PullMoveUse.Equal[nav]}) {
/if ((${Navigation.Active} || ${MoveTo.Moving} || (${Spawn[${MyTargetID}].Speed}>25 && ${Spawn[${MyTargetID}].Distance3D}>${PullRange}))) /varcalc PullTimer ${PullTimer}+50
} else /if (${PullMoveUse.Equal[los]} && !${Select[${Role},hunter,hunterpettank]}) {
/if ((${Me.Moving} || ${MoveTo.Moving} || (${Spawn[${MyTargetID}].Speed}>25 && ${Spawn[${MyTargetID}].Distance3D}>${PullRange}))) /varcalc PullTimer ${PullTimer}+50
}
| Pull counter used to in conjunction with 1s delay for timing
/varcalc PullAttempts ${PullAttempts}+1
| Try and pull again after 7 seconds or 3 seconds if target is moving
/if (${Select[${PullMoveUse},los,nav]}>=1) {
/if (${PullAttempts}>=7) {
| Make range smaller to creep closer to mob if not los
/if (${Spawn[${MyTargetID}].Distance3D}<=${PullDist} && !${Spawn[${MyTargetID}].LineOfSight}) /varcalc PullDist ${PullDist}*.6
/if (${DebugPull}) /echo \atDEBUGPULL Pull PullDist: ${PullDist} - PullLoops: ${PullAttempts} LineOfSight: ${Spawn[${MyTargetID}].LineOfSight} \agLine#: ${Macro.CurLine}
/goto :PullAgain
} else /if (${PullAttempts}>=3 && ${Spawn[${MyTargetID}].Speed}>25 && ${WasInRange} && ${Spawn[${MyTargetID}].Distance3D}>${PullDist}) {
| Make range smaller to creep closer to mob if Mob is Moving
/varcalc PullDist ${PullDist}*.6
/varset WasInRange 0
/if (${DebugPull}) /echo \atDEBUGPULL Pull PullDist: ${PullDist}*.6 - PullLoops: ${PullAttempts} \agLine#: ${Macro.CurLine}
/goto :PullAgain
}
}
| 1s timer used in conjunction with PullAttempts to control pulling
/delay 10
|------------------------------------------------------------------------Are We Stuck-------------------------------------------------------------------------------------------|                       
| - Check to see if we are stuck
/if ((${Int[${Me.X}]}==${X1}) && (${Int[${Me.Y}]}==${Y1})) {
/varcalc StuckCount (${StuckCount})+1
/if (${StuckCount}>=2) {
/if (${IAmDead} || ${Me.Hovering}) {
/call Stopmoving
/return
} else {
/call Stuck pull
} 
}
/if (${StuckCount}>=7 && !${PullAggroTargetID}) {
/echo I am stuck aborting pull
/if (${MyTargetID}) /squelch /alert add 1 id ${MyTargetID}
/call StopMoving
/call BackToCampReset
/if (${Macro.Return}) /return            
}
}
|------------------------------------------------------------------------Are We Stuck End-------------------------------------------------------------------------------------------|                        
/if (${DebugPull})  /echo \atDEBUGPULL Pull Loop Count: ${PullAttempts} ${Me.Moving} ${MoveTo.Moving} ${Me.Speed} ${Spawn[${MyTargetID}].Distance3D} ${PullRange} \agLine#: ${Macro.CurLine}
| Not using advpath
/if (${Select[${PullMoveUse},los,nav]}>=1) {
| Distance loop check until mob in range to pull
/if ((${Spawn[${MyTargetID}].Distance3D}>${PullRange} || !${Spawn[${MyTargetID}].LineOfSight} || ${Target.FeetWet}!=${Me.FeetWet}) && !${PullAggroTargetID}) {
/if (!${Me.Moving} || ${WasInRange}) /varcalc PullDist ${PullDist}*.8 
/goto :WeThereYet
}
| If mob moves out of line of sight during pull try moveto mob again
/if (!${Spawn[${MyTargetID}].LineOfSight} && !${Select[${Role},hunter,hunterpettank]} && ${PullTimer}) {
/varcalc PullDist ${PullDist}*.8
/if (${DebugPull}) /echo \atDEBUGPULL Pull Decrease pull distance to ${PullDist} \agLine#: ${Macro.CurLine}
/goto :PullAgain
}
/if (${PullAggroTargetID}) /goto :PullAgain
}
/if (${Spawn[${MyTargetID}].ID} && ${Target.FeetWet}==${Me.FeetWet} && !${PullAggroTargetID} && (${Spawn[${MyTargetID}].Distance3D}<${PullRange} || (${PullWith.Equal[Melee]} && ${Spawn[${MyTargetID}].Distance3D}<${Math.Calc[${PullRange}*2]}))) {
/if (${PullMoveUse.Equal[advpath]}) /varset WasInRange 1
/call StopMoving
| Target mob before Aggroing
/target id ${MyTargetID}
/delay 20 ${Target.ID}==${MyTargetID}
| Validate target one more time before pulling
/call ValidateTarget
/if (${ValidTarget}==0) {
/if (${Target.ID}) /squelch /alert add 1 id ${Target.ID}
/squelch /target clear
/echo Aborting Pull! Target invalid now! Reason:${Macro.Return}
/call StopMoving
/call BackToCampReset
/if (${Macro.Return}) /return
}
/if (${PullWith.Equal[Ranged]} && ${Spawn[${MyTargetID}].Distance3D}<30) /varset ToClose 1
|------------------------------------------------------------------------Pull with Melee-------------------------------------------------------------------------------------------|            
| Handle pulling with Melee setting
/if (${PullWith.Equal[Melee]} || (${PullWithAlt.Equal[Melee]} && ${ToClose} && !${Select[${Role},hunter,hunterpettank]})) {
/call PullWithMelee
|------------------------------------------------------------------------Pull with Ranged-------------------------------------------------------------------------------------------|                         
| Pull with ranged
} else /if (${PullWith.Equal[Ranged]} && !${ToClose}) {
/call PullWithRanged ${PullDist} 1
/if (${Macro.Return}>=1) /varset PullDist ${Int[${Macro.Return}]}
|------------------------------------------------------------------------Pull with Pet-------------------------------------------------------------------------------------------|                                 
| Pull with pet
} else /if (${PullWith.Equal[Pet]}) {                
/call PullWithPet
|------------------------------------------------------------------------Pull with Cast-------------------------------------------------------------------------------------------|                          
| Pull with cast
} else {
/call PullWithCast ${PullDist}
/if (${Macro.Return}>=1) {
/varset PullDist ${Int[${Macro.Return}]}
/goto :PullAgain
}
}
|------------------------------------------------------------------------Pull with End-------------------------------------------------------------------------------------------|                      
|- Toggle puller mode off if option enabled.
/if (${Select[${Role},pullerpettank]} && ${PullRoleToggle} && ${Group.Puller.ID}==${Me.ID}) /call PullModeToggle TurnOff            
} else /if (${PullAggroTargetID}) {
| The else /if fixes when puller stalls because puller grabs aggro with out getting to pull.
/varset Pulled 1
}
| If pull failed start over while timer > 0
/if (${Select[${PullMoveUse},los,nav]}>=1 && !${Pulled}) /goto :PullAgain
:DonePulling
| reset mq2moveutils dist back to 10 from pull distance to ensure correct movement 
/moveto dist 10
/if (${PullWith.Equal[Ranged]}) {
/call PullWithRanged ${PullDist} 2
}
/varset Pulling 0
| Turn autofire back on
/if (${AutoFireOff}) {
/if (${DebugPull}) /echo \atDEBUGPULL Pull AutoFire on \agLine#: ${Macro.CurLine}
/varset AutoFireOff 0
/varset AutoFireOn 1
}
/if (${Select[${Role},hunter,hunterpettank]} && ${MyTargetID}) {
/call PullHunter
/return
}
/if (${DebugPull}) /echo \atDEBUGPULL Pull Done Pulling ${ReturnToCamp} ${Pulled} \agLine#: ${Macro.CurLine}
/if (${ReturnToCamp}) {
/if (${Pulled}) {
/call WaitForMob
/varset Pulled 0
} else {
/call DoWeMove 1 pull
}
}
/if (${DebugPull}) /echo \atDEBUGPULL Pull Leave Mob ID:${Spawn[${MyTargetID}].ID} \agLine#: ${Macro.CurLine}
/return



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



|===================================================================================================================================================================|

cutsom sub designed to use observers to maintain a group "presence" over a designated list of item. starting with debuffs in order to remove the MQ2Debuff plugin.
every character needs to be aware of the debuffs present on every other character.


/plugin MQ2debuffs unload



Changed ${Me} and ${Buff} TLO's
  Added DiseaseCounters, PoisonCounters, CurseCounters, CorruptionCounters
  Changed Counters to TotalCounters

| bad
${Me.DiseaseCounters}
${Me.PoisonCounters}
${Me.CurseCounters}
${Me.CorruptionCounters}
${Me.TotalCounters}

${Me.Diseased.CounterNumber}
${Me.Poisoned.CounterNumber}
${Me.Cursed.CounterNumber}
${Me.Corrupted.CounterNumber}
${Me.TotalCounters}


| Good 
||||

${Me.CountersDisease}
${Me.CountersCurse}
${Me.CountersPoison}
${Me.CountersCorruption}
${Me.TotalCounters


${DanNet.PeerCount[${DanNetMacroChannel}]}

${DanNet.Peers[${DanNetMacroChannel}]}


establish:
/dobserve <name> -q <query> [-o <result>]

read:
${DanNet[<name>].Observe[<query>]} 
${DanNet[<name>].O[<query>]}





#bind observer /observer
sub Bind_observer(bool _debug)


	/declare _countT 					int 		local 0
	/declare _countI 					int 		local 0
	/declare _countE					int 		local 0
	/declare _toonName 				string 	local FALSE
	/declare _element					string	local FALSE
	/declare _TLO							string 	local FALSE
	/declare _observe[1] 			string 	local
	/declare totalstart				int			local 0
	/declare totalend					int			local 0

	
	| established debuffs
	/varset _observe[1] |Me|TotalCounters|CountersDisease|CountersCurse|CountersPoison|CountersCorruption
	| /varset _observe[2] |Me|Snared|Mezed|Maloed|Tashed

	/varset totalstart ${MacroQuest.Running}

	| cycle the toons inside the dan group
	/for _countT 1 to ${DanNet.PeerCount[${DanNetMacroChannel}]}
		/varset _toonName ${DanNet.Peers[${DanNetMacroChannel}].Arg[${_countT},|]}

		| skip myself
		/if (${_toonName.Equal[${Me.DisplayName}]}) /continue
		/echo Setting Observers\aw::\ax\a-t${_toonName}\ax

		| go through each array
		/for _countI 1 to ${_observe.Size}
			
			| set the TLO
			/echo _observer[${_countI}]::_TLO::${_observe[${_countI}].Arg[1,|]}
			/varset _TLO ${_observe[${_countI}].Arg[1,|]}
			
			| loop the array 
			/for _countE 2 to ${_observe[${_countI}].Count[|]}
				/echo _observer[${_countI}]::_element[${_countE}]::${_observe[${_countI}].Arg[${_countE},|]}
				| set the member
				/varset _element ${_observe[${_countI}].Arg[${_countE},|]}

				| make the variable
				/if (!${Defined[${_toonName}_${_element}]}) /declare ${_toonName}_${_element} string outer
				| set observer
				/dobserve ${_toonName} -q "${_TLO}.${_element}" -o ${_toonName}_${_element}

			/next _countE
		/next _countI

	/next _countT
	/varset totalend ${MacroQuest.Running}
	
	/echo Setting Observers\aw::\ax\a-tDone\ax ${sep} \aw${Math.Calc[((${totalend}-${totalstart}) / 10) / 60]}\ax secoonds
/return










	| this is an example of a cycled display check for the query.
	| /for _countT 1 to ${DanNet.PeerCount[${DanNetMacroChannel}]}
	| 	/varset _toonName ${DanNet.Peers[${DanNetMacroChannel}].Arg[${_countT},|]}


| /if (!${Defined[emra_CountersPoison]}) /declare emra_CountersPoison string outer
/dobserve emra -q "Me.CountersPoison" -o emra_CountersPoison -t 1s
/delay 1s			
/echo emra ${sep} ${emra_TotalCounters}



	| /next _countT
		


/dquery emra -q "Me.CountersPoison" -o _DNqout -t 1s 


/dobserve emra -q "Me.CountersPoison" -o _DNqout -t 1s
/echo ${DanNet[emra].Observe[emra_CountersPoison]}


TotalCounters


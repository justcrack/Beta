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


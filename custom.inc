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





|==========================================================================================================================================
create a DB backup..
why the fuck doesnt this work???



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




|==========================================================================================================================================

templating

a simple. plain english flat file that can be filled out and share. something that can import into core and make a 100% ready character.
will need maps from all entries to existing DB values.







; Core Macro
; exspes.core.com
; template.ini
;
; this provides a template to import settings to creat a character inside a core database
; any field not filled out hree will be imported as FALSE
;
;
; My advice. 
; This is to be use as intended for Cores design. if you try and do stupid shit with it, don't tell me.
; seriously. just fill in the blanks. if your class doesnt use spells. dont put them in there. its not difficult.

; Notes
; thre will NOT be any conditions imported anywhere. dont ask, not doing it.





; chracter server
Server=

; character first name
Name=

; character class
Class=














; == auras (2) =======================================================
; there are 2 usable auras. if your class only uses one aura, then....

; name of the spell, song or disc you wish to use asn an aura (NOT ITEM)
Aura1Spell=

; name of the aura as it appears in your aura window
Aura1Buff=

Aura2Spell=
Aura2Buff=


; == AA Now (2) ======================================================
; name of the AA you always want running on your character

AANow1=
AANow2=


; == Disc Now (2) ====================================================
; name of the Disc you always want running on your character

DiscNow1=
DiscNow2=


; == Item Now (2) ====================================================
; name of the item you always want to have an effect on you, like a geomantra item

ItemNow1=
ItemNow2=

















; == buffs 1-10 ======================================================
; i'm only doing 1 here as an example. you can copy and paste the other 9 if you like. just change the #
; dont overthink it

; name of the buff you want casted
Buff1=

; is this a self buff TRUE/FALSE
Buff1self=

; aliase people /t you to get the buff cased on them. like |HP or |symbol
Buff1Alias=

; list of classes you box you want to have this buff eg:|CLR|RNG|WAR|SHD|PAL
BCBuff1Class=

; list of classes in a raid to use this buff one. eg: |ROG|RNG|BRD|MNK
RaidBuff1Class=

; list of classes you dont box to use this on (Mercs and other RL players)
ManualBuff1Class=





















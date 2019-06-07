|***
 *	advpull.inc
 *	Core Advanced Pulling functions.
 *
 *	For when a normal wank just isn't enough.
 ***|
 
 
 
 
 
 |***
 *	DES: tells whatever toon that is in the DanNet CLASSGROUP goup to intervention heal you
 *	USE: /intpull [CLS] (with a valid target)
 *	NOTE: 
 ***|
#bind interventionPull /intpull
sub Bind_interventionPull()
/return TRUE



|***
 *	DES: forces an intervention spell on that target
 *	USE: /intervention [TargetID]
 *	NOTE: 
 ***|
#bind healPull /healpull
sub Bind_healPull(int _tmpID)
/return FALSE



|***
 *	DES: important for later. DO NOT DELETE!!
 *	USE: 
 *	NOTE: 
 ***|
sub find_priest(bool _debug, int _distance)
/return ${_toonName}



|***
 *	DES: pulls NON los mobs to you using a healers intervention spell
 *	USE: /intervention [TargetID]
 *	NOTE: 
 ***|
sub get_target_intpull(bool _debug)
/return


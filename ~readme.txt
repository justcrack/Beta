


how to update/migrate to the SQLite version


instructions are in order. 
do them in order
if you cannot do them in order, i cannot help you
no one can.....
-------------------


- log out all of your toons. log out of MQ. get alcohol

- back yo shit up. the entire TC folder

- now that you have backed it up. rename it

- make a new TC folder

- this one should be empty. so you are not erasing your old shit

- double check the first few steps

- go into your new TC folder

- unzip the contents of the beta####.##.zip file into your EMPTY TC folder

- make a folder called data inside the tc folder

- you should now have shit in your NEW TC folder. some files and a couple directories

- log in a toon. start core up as you would normally. the initial database will generate, you will see lots of data fly by.. thats intended

- set yourself up and go kill things, pregerably gnomes and asslings


now, if you want to start fresh, you are done. if you want to migrate any existing data, continue on
-------------------

- loot and zones
- from your BACKUP. copy the following file into the NEW data folder:
  -- core.loot.ini
  
- from your BACKUP. copy the following file into the TC folder:  
  -- core.zone.ini  

- issue /admin migrate zone
- this will import all of your existing zone file


- copy your character INI files (the ones you want to keep data from) back to the NEW TC folder.

- log in the character you want to migrate the data from
- issue /admin migrate character
- this will migrate a majority of your characters data. IT IS NOT 100% complete, but the important things get there
- wash-rinse-repeate for remaing toons. once imported, you can remove the character INI files, they will not be used

- you can delete all but the core.loot.ini file that you copoed. they are no longer needed.

- changelog.txt will have the rest of the info you need and new commands for editing the database


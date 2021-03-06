#!/usr/local/bin/bash



    ./stop

			[ -f ./src.build/infokruncher ] &&  rm ./src.build/infokruncher
			[ -f ./src.build/db/kruncherdb ] && rm ./src.build/db/kruncherdb 

	./Build.ksh



	if [ 0 == 1 ]; then
		./src.build/infokruncher -d
		./src.build/kruncherdb -d
	else
		if [ -f ./src.build/infokruncher ] && [ -f ./src.build/db/kruncherdb ]; then

			./src.build/infokruncher& 
			./src.build/db/kruncherdb& 


			[ -f /var/log/syslog ] && sudo tail -f /var/log/syslog | grep -i --line-buffered "jmt" | grep "favicon.ico" | cut -d: -f4-20&

		else
			echo -ne "\033[31mBuild failed\033[0m\n"
			read ok
		fi
	fi



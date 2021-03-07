#!/usr/local/bin/bash
target=${1-all}


#[ -f /var/log/syslog ] && sudo tail -f /var/log/syslog | grep -i --line-buffered "jmt" | grep "favicon.ico" | cut -d: -f4-20&

./stop $@
./Build.ksh $@

echo -ne "\033[44m\033[36mRun target:${target}\033[0m\n"

shift

if [ "${target}" == "all" ] || [ "${target}" == "infokruncher" ]; then
	echo -ne "\033[35mStarting infokruncher\033[0m\n"
	./src.build/infokruncher& 
fi

if [ "${target}" == "all" ] || [ "${target}" == "kruncherdb" ]; then
	echo -ne "\033[35mStarting kruncherdb\033[0m\n"
	./src.build/db/kruncherdb& 
fi

#	if [ 0 == 1 ]; then
#		./src.build/infokruncher -d
#		./src.build/db/kruncherdb -d
#	else
#		if [ -f ./src.build/infokruncher ] && [ -f ./src.build/db/kruncherdb ]; then
#
#			./src.build/infokruncher& 
#			./src.build/db/kruncherdb& 
#
#			#[ -f /var/log/syslog ] && sudo tail -f /var/log/syslog | grep -i --line-buffered "jmt" | grep "favicon.ico" | cut -d: -f4-20&
#
#		else
#			echo -ne "\033[31mBuild failed\033[0m\n"
#			read ok
#		fi
#	fi
#


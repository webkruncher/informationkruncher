#!/usr/local/bin/bash
target=${1-all}


#[ -f /var/log/syslog ] && sudo tail -f /var/log/syslog | grep -i --line-buffered "jmt" | grep "favicon.ico" | cut -d: -f4-20&

./stop $@
./build $@

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

#krunchdbunittester
if [ "${target}" == "all" ] || [ "${target}" == "dbut" ]; then
	echo -ne "\033[35mStarting krunchdbunittester\033[0m\n"
	./src.build/db/krunchdbunittester& 
fi

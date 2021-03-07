#!/usr/bin/env bash
target=${1-all}
echo -ne "\033[44m\033[36mBuild target:${target}\033[0m\n"

                            
if [ "${target}" == "all" ] || [ "${target}" == "infokruncher" ]; then
	echo -ne "\033[32mBuilding infokruncher\033[0m\n"
	#[ -f src.build/informationkruncher ] && rm src.build/informationkruncher
	cmake  -S src/ -B src.build/  --target infokruncher
	cmake  --build src.build/ --target infokruncher
fi

if [ "${target}" == "all" ] || [ "${target}" == "kruncherdb" ]; then
	echo -ne "\033[32mBuilding kruncherdb\033[0m\n"
	#[ -f src.build/db/kruncherdb ] && rm src.build/db/kruncherdb
	cmake  -S src/ -B src.build/  --target kruncherdb
	cmake  --build src.build/ --target kruncherdb
fi

if [ "${target}" == "all" ] || [ "${target}" == "dbut" ]; then
	echo -ne "\033[32mBuilding krunchdbunittester\033[0m\n"
	#[ -f src.build/db/kruncherdb ] && rm src.build/db/krunchdbunittester
	cmake  -S src/ -B src.build/  --target krunchdbunittester
	cmake  --build src.build/ --target krunchdbunittester
fi


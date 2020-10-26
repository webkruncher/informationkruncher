
pkill tail

	#touch src/*
	pid=`ps aux | grep "infokruncher" | sed '/grep/d' | tr -s ' ' | cut -d ' ' -f2`
	if [ "${pid}" == "" ]; then
		echo "not running yet"
	else
		echo "killing ${pid}"
		kill -9 ${pid}			
		wget -O- localhost 2>&1 >> /dev/null
	fi


	./Build.ksh
	if [ ! -f ./src.build/infokruncher ] ; then
		echo -ne "\033[31mBuild failed:\033[0m"
		read ok
	fi
	#sleep 2
	#echo "Ready to start infokruncher?"
	#read ok



	if [ 1 == 1 ]; then
		./src.build/infokruncher -d
	else
		if [ -f ./src.build/infokruncher ] ; then

			./src.build/infokruncher& 


			[ -f /var/log/syslog ] && sudo tail -f /var/log/syslog | grep -i --line-buffered "jmt" | grep "favicon.ico" | cut -d: -f4-20&

		else
			echo -ne "\033[31mBuild failed\033[0m"
			read ok
		fi
	fi



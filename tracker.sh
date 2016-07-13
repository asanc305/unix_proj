#! /bin/bash

if [ $1 == "-start" ]; then 
	EVENT=$(grep -E 'Handlers|EV=' /proc/bus/input/devices | grep -B1 'EV=120013' | grep -Eo 'event[0-9]+')
	$(./tracker /dev/input/$EVENT)&
elif [ $1 == "-end" ]; then
	PID=$(ps -e | grep 'tracker' | awk '{print $1}')
	for I in $PID;
	do
		$(kill -2 $I)
	done  
else
  echo "error"
fi

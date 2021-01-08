#!/bin/bash
acounter=0
icounter=0
actives=""
inactives=""
ps > temps
for direc in $( ls $1)
do
	filename="$1/""$direc""/_client_read"
	if [ -e $filename ];then
		filename="$1/""$direc""/_client_write"
		if [ -e $filename ];then
			filename="$1/""$direc""/_others_read"
			if [ -e $filename ];then
				filename="$1/""$direc""/_others_write"
				if [ -e $filename ];then
					filename="$1/""$direc""/_pid"
					if [ -e $filename ];then
						while read -r line || [[ -n "$line" ]]
						#prevents the last line from being ignored if it doesn't end with a \n
						do
							if [ "$line" -gt "0" ];then
								tempstr=$(grep -F "$line" "temps")
								if [ "$tempstr" != "" ];then
									actives="$actives""\n""$1""/$direc"
									acounter=`expr $acounter + 1`
								else
									inactives="$inactives""\n""$1""/$direc"
									icounter=`expr $icounter + 1`
								fi
							fi
						done < "$filename"
					fi
				fi
			fi
		fi
	fi
done
rm -f temps;
echo -e "\nActive Boards $acounter $actives\n"
echo -e "Inactive Boards (on disk but no server running) $icounter $inactives\n"

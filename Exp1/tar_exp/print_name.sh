#!/bin/bash
		count=2
		while [ $count -le 50 ]
		do
			if [ $count -lt 10 ]
				then
					echo COE18B00$count
			else 
				echo COE18B0$count
			fi
			((count++))
		done


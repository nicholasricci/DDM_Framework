#    This file is part of DDMTestbed, a free Data Distribution Management benchmark
#    Copyright (C) 2013 Francesco Bedini
#
#    This program is free software: you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published by
#    the Free Software Foundation, either version 3 of the License, or
#    (at your option) any later version.
#
#    This program is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.
#
#    You should have received a copy of the GNU General Public License
#    along with this program.  If not, see <http://www.gnu.org/licenses/>.

#!/bin/bash 

function print_help {
	echo "Command unrecognised"
	echo "Syntax: $0 [-purge]"
	exit 
}

if [ $# -gt 1 ]
then
	print_help
fi

#Check that there's exactly one parameter
if [ $# -eq 1 ]
then
	if [ $1 = "-purge" ]
	then
		cd TestsInstances
		  #List instance names here
		  rm -R "TINIEST";
		  rm -R "TINY";
		  rm -R "TINY50";
		  rm -R "TINY50U";
		  rm -R "TINY50S";

		  rm -R "SMALLEST";
		  rm -R "SMALL";
		  rm -R "SMALL30";
		  rm -R "SMALL30U";
		  rm -R "SMALL30S";

		  rm -R "SPARSE";
		  rm -R "AVERAGE";
		  rm -R "MEDIUM";
		  rm -R "MEDIUM20";
		  rm -R "MEDIUM20U";
		  rm -R "MEDIUM20S";

		  rm -R "GRANDE";
		  rm -R "BIG";
		  rm -R "BIG10";
		  rm -R "BIG10U";
		  rm -R "BIG10S";

		  rm -R "HUGE";
		  rm -R "HUGER";
		  rm -R "CLUTTERED";
		cd ..
	else
		print_help
	fi
fi

#Compile DDMInstanceMaker
./launcher.sh build

T="$(date +%s)"
#Generate them, for each instance you add here a "rm -R <instance_name>" must be added above to make the purge operator work properly
./utils/DDMInstanceMaker -d 1 -u 5 -s 3 -r 21312 -n "TINIEST" -l 1 -a "Francesco Bedini" -k -v 1;
./utils/DDMInstanceMaker -d 2 -u 10 -s 15 -r 1111 -n "TINY" -l 1 -a "Francesco Bedini" -k -v 1;
./utils/DDMInstanceMaker -d 2 -u 10 -s 15 -r 2222 -n "TINY50" -l 50 -a "Francesco Bedini" -k -v 1;
./utils/DDMInstanceMaker -d 2 -u 10 -s 15 -r 3333 -n "TINY50U" -l 50 -a "Francesco Bedini" -k -v 1 -R 1;
./utils/DDMInstanceMaker -d 2 -u 10 -s 15 -r 3333 -n "TINY50S" -l 50 -a "Francesco Bedini" -k -v 1 -R 2;
./utils/DDMInstanceMaker -d 3 -u 1000 -s 1000 -r 21312 -n "SMALLEST" -l 1 -a "Francesco Bedini" -k -v 1;
./utils/DDMInstanceMaker -d 3 -u 1700 -s 1800 -r 1111 -n "SMALL" -l 1 -a "Francesco Bedini" -k -v 1;
./utils/DDMInstanceMaker -d 3 -u 1700 -s 1800 -r 2222 -n "SMALL30" -l 30 -a "Francesco Bedini" -k -v 1;
./utils/DDMInstanceMaker -d 3 -u 1700 -s 1800 -r 3333 -n "SMALL30U" -l 30 -a "Francesco Bedini" -k -v 1 -R 1;
./utils/DDMInstanceMaker -d 3 -u 1700 -s 1800 -r 3333 -n "SMALL30S" -l 30 -a "Francesco Bedini" -k -v 1 -R 2;
./utils/DDMInstanceMaker -d 2 -u 4000 -s 3500 -r 21312 -n "AVERAGE" -l 1 -a "Francesco Bedini" -k -v 1;
./utils/DDMInstanceMaker -d 2 -u 1000 -s 5000 -r 91234 -n "SPARSE" -l 1 -S 10 -a "Francesco Bedini" -k -v 1;
./utils/DDMInstanceMaker -d 2 -u 5000 -s 6000 -r 1111 -n "MEDIUM" -l 1 -a "Francesco Bedini" -k -v 1;
./utils/DDMInstanceMaker -d 2 -u 5000 -s 6000 -r 2222 -n "MEDIUM20" -l 20 -a "Francesco Bedini" -k -v 1;
./utils/DDMInstanceMaker -d 2 -u 5000 -s 6000 -r 3333 -n "MEDIUM20U" -l 20 -a "Francesco Bedini" -k -v 1 -R 1;
./utils/DDMInstanceMaker -d 2 -u 5000 -s 6000 -r 3333 -n "MEDIUM20S" -l 20 -a "Francesco Bedini" -k -v 1 -R 2;
./utils/DDMInstanceMaker -d 2 -u 9000 -s 8500 -r 21312 -n "GRANDE" -l 1 -a "Francesco Bedini" -k -v 1;
./utils/DDMInstanceMaker -d 2 -u 10000 -s 11000 -r 1111 -n "BIG" -l 1 -a "Francesco Bedini" -k -v 1;
./utils/DDMInstanceMaker -d 2 -u 10000 -s 11000 -r 2222 -n "BIG10" -l 10 -a "Francesco Bedini" -k -v 1;
./utils/DDMInstanceMaker -d 2 -u 10000 -s 11000 -r 3333 -n "BIG10U" -l 10 -a "Francesco Bedini" -k -v 1 -R 1;
./utils/DDMInstanceMaker -d 2 -u 10000 -s 11000 -r 3333 -n "BIG10S" -l 10 -a "Francesco Bedini" -k -v 1 -R 2;
./utils/DDMInstanceMaker -d 2 -u 14500 -s 15000 -r 1234 -n "HUGE" -l 1 -a "Francesco Bedini" -k -v 1 -R 2;
./utils/DDMInstanceMaker -d 2 -u 20000 -s 30000 -r 4444 -n "HUGER" -l 1 -a "Francesco Bedini" -k -v 1 -R 2;
./utils/DDMInstanceMaker -d 2 -u 50000 -s 45000 -r 3333 -n "CLUTTERED" -l 1 -S 100000000000000000000 -a "Francesco Bedini" -k -v 1 -R 2;

T="$(($(date +%s)-T))"
echo -e "\nElapsed time: ${T} seconds"


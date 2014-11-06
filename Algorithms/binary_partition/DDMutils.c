/*
    This file is part of DDMTestbed, a free Data Distribution Management benchmark
    Copyright (C) 2013 Francesco Bedini

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#include "DDMUtils.h"


bool ReadNextString(char * string, FILE * fp)
{
	while(!feof(fp)){
		if(fgets(string, LINE_MAX_LENGTH, fp) == NULL)
            return false;
        if(*string == '#')//If this line is a comment, skip it
			continue;
		return true;
		}
	return false;
}

void WriteLogLine(char * string)
{
    FILE * fp;
    time_t now;
    fp = fopen(LOG_FILENAME,"a+"); //Opens output file in append mode.
	if(fp == NULL)
	{
        fprintf(stderr,"*** Error ***\nCannot append on log file file %s.\n", LOG_FILENAME);
        exit(EXIT_FAILURE);
	}
	time(&now);
	fprintf(fp,"%s: ", ctime(&now));
	fprintf(fp, "%s", string);
	fprintf(fp, "\n----------------------------\n");
    fclose(fp);
}

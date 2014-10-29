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


#include "ExtentMaker.h"

_ERR_CODE generate_random_extent_set(MatchingInstance ** out, const uint32_t subscriptions, const uint32_t updates, const uint_fast8_t dimensions, const uint64_t avg_ext_size){
	return generate_fixed_extent_set(out, (unsigned int)time(NULL), subscriptions, updates, dimensions, avg_ext_size);
}

_ERR_CODE generate_fixed_extent_set(MatchingInstance ** out, const unsigned int seed, const uint32_t subscriptions, const uint32_t updates, const uint_fast8_t dimensions, const uint64_t avg_ext_size){
	uint32_t i, k;
	_ERR_CODE error;
	SPACE_TYPE temp1, temp2;
	if((error = generate_empty_extent_set(out, subscriptions, updates, dimensions)) != err_none){
		return error;
	}
	srand(seed);
	for(i=0; i<subscriptions; i++)
	{
		(*out)->extents[i].id=i;
		(*out)->extents[i].dirtyBit = false;
		for(k=0; k<dimensions; k++){
			temp1 = BIGRANDOM(SPACE_TYPE_MIN, SPACE_TYPE_MAX);
			if(avg_ext_size == 0) //FUTURE IMPROVEMENT As avg_ext_size is costant inside this loop, take this if out of the for cicle
			{
				//i.e. Random size
				temp2 = BIGRANDOM(SPACE_TYPE_MIN, SPACE_TYPE_MAX);
				(*out)->extents[i].bounds[k].lower = MIN(temp1,temp2);
				(*out)->extents[i].bounds[k].upper = MAX(temp1,temp2);
			}
			else {
				temp2 = temp1 + (rand() % ((2 * avg_ext_size)) + 1);
				if(temp2 < temp1)
                {
                    //Overflow detected
                    temp2 = SPACE_TYPE_MAX;
                }
				(*out)->extents[i].bounds[k].lower = temp1;
				(*out)->extents[i].bounds[k].upper = temp2;
			}
		}
	}
	for(i=subscriptions; i<subscriptions+updates; i++)
	{
		(*out)->extents[i].id=i-subscriptions;
		(*out)->extents[i].dirtyBit = true;
		for(k=0; k<dimensions; k++){
			temp1 = BIGRANDOM(SPACE_TYPE_MIN, SPACE_TYPE_MAX);
						if(avg_ext_size == 0) //FUTURE IMPROVEMENT As avg_ext_size is costant inside this loop, take this if out of the for cicle
			{
				//i.e. Random size
				temp2 = BIGRANDOM(SPACE_TYPE_MIN, SPACE_TYPE_MAX);
				(*out)->extents[i].bounds[k].lower = MIN(temp1,temp2);
				(*out)->extents[i].bounds[k].upper = MAX(temp1,temp2);
			}
			else {
				temp2 = temp1 + (rand() % ((2 * avg_ext_size) + 1));
                if(temp2 < temp1)
                {
                    //Overflow detected
                    temp2 = SPACE_TYPE_MAX;
                }
				(*out)->extents[i].bounds[k].lower = temp1;
				(*out)->extents[i].bounds[k].upper = temp2;
			}
		}
	}
	return err_none;
}

_ERR_CODE generate_sample_extent_set(MatchingInstance ** out){
#define SUBSCRIPTIONS 15
#define UPDATES 20
#define DIMENSIONS 1
#define SIZE 4
	uint32_t i, k;
	_ERR_CODE error;
		if((error= generate_empty_extent_set(out, SUBSCRIPTIONS, UPDATES, DIMENSIONS)) != err_none)
		{
			return set_error(error,__FILE__, __FUNCTION__,__LINE__);
		}
	(*out)->dimensions=DIMENSIONS;
	for(i=0; i<SUBSCRIPTIONS; i++)
		{
			(*out)->extents[i].id=i;
			(*out)->extents[i].dirtyBit=false;
			for(k=0; k<DIMENSIONS; k++)
			{
				(*out)->extents[i].bounds[k].lower=i*i;
				(*out)->extents[i].bounds[k].upper=(*out)->extents[i].bounds[k].lower+SIZE;
			}
		}
	for(i=SUBSCRIPTIONS; i<SUBSCRIPTIONS+UPDATES; i++)
		{
			(*out)->extents[i].id=i-SUBSCRIPTIONS;
			(*out)->extents[i].dirtyBit=false;
			for(k=0; k<DIMENSIONS; k++)
			{
				(*out)->extents[i].bounds[k].lower=(i*i-10*i);
				(*out)->extents[i].bounds[k].upper=(*out)->extents[i].bounds[k].lower+SIZE;
			}
		}
	return err_none;
}

_ERR_CODE generate_empty_extent_set(MatchingInstance ** out, const uint32_t subscriptions, const uint32_t updates, const uint_fast8_t dimensions){
	if(dimensions>MAX_DIMENSIONS)
		return set_error(err_too_many_dim,__FILE__, __FUNCTION__, __LINE__);
	(*out) = (MatchingInstance*)  malloc(sizeof(MatchingInstance));
	if((*out) == NULL)
		return set_error(err_alloc,__FILE__, __FUNCTION__, __LINE__);
	(*out)->dimensions=dimensions;
	(*out)->updates=updates;
	(*out)->subscriptions=subscriptions;
	(*out)->extent_number=(uint64_t)updates+(uint64_t)subscriptions;
	(*out)->extents=(extent*) malloc((*out)->extent_number*sizeof(extent));
	if((*out)->extents==NULL)
		return set_error(err_alloc,__FILE__, __FUNCTION__, __LINE__);
    (*out)->matching_result = malloc(sizeof(bitmatrix));
	if((*out)->matching_result == NULL)
		return set_error(err_alloc,__FILE__, __FUNCTION__, __LINE__);
    if(init_bit_matrix(((*out)->matching_result), updates, subscriptions, zero) != err_none)
	{
		print_error_string();
		exit(EXIT_FAILURE);
	}
	return err_none;
}

void free_matching_instance(MatchingInstance ** instance, bool discardResult)
{
	if(discardResult)
		free_bit_matrix((*instance)->matching_result);
	free((*instance)->extents);
	free(*instance);
	*instance=NULL;
}
_ERR_CODE save_instance_info_on_file(const char * file_path, const char * instance_name, const char * author_name, const char *version_number, const bool override_file, const unsigned int random_seed, const uint32_t sequence_length, const uint_fast8_t dimensions, uint32_t subscriptions, uint32_t updates)
{
	FILE * fp;
	char confirm='1';
	if(!override_file){
		fp=fopen(file_path, "r");
		if(fp!=NULL)
		{//File already exists
			fclose(fp);
			printf("File %s already exists, would you like to overwrite it? (y|n)\n", file_path);
			do{
				confirm = getchar();
			}while(confirm!='y' && confirm !='n');
			if(confirm =='n')
				return err_file;
		}
	}
	fp = fopen (file_path, "w");
	if(fp == NULL)
		return set_error(err_file, __FILE__, __FUNCTION__, __LINE__);

	fprintf(fp, "#Instance name: %s\n",instance_name);
	fprintf(fp, "#Instance version: %s\n",version_number);
	fprintf(fp, "#Created on %s\n", __TIMESTAMP__);
	fprintf(fp, "#Author: %s\n", author_name);
	fprintf(fp, "#Random Seed: %d\n", random_seed);
    fprintf(fp, "#Sequence length:\n%d\n", sequence_length);
	fprintf(fp, "#Dimensions\n%"PRIuFAST8"\n", dimensions);
	fprintf(fp, "#Subscription Regions\n%"PRIu32"\n", subscriptions);
	fprintf(fp, "#Update Regions\n%"PRIu32"\n", updates);

	fclose(fp); //Closes file

	return err_none;
}

_ERR_CODE save_to_file (MatchingInstance * m, const char * file_path, const bool override_file)
{
	unsigned int i,k;
	FILE * fp;
	char confirm='1';
	if(!override_file){
		fp=fopen(file_path, "r");
		if(fp!=NULL)
		{//File already exists
			fclose(fp);
			printf("File %s already exists, would you like to overwrite it? (y|n)\n", file_path);
			do{
				confirm = getchar();
			}while(confirm!='y' && confirm !='n');
			if(confirm =='n')
				return err_file;
		}
	}
	fp = fopen (file_path, "w");
	if(fp == NULL)
		return set_error(err_file, __FILE__, __FUNCTION__, __LINE__);

	fputs("#Subscriptions <id> <D1 edges> [<D2 edges>]...\n", fp);
	for(i=0; i<m->subscriptions; i++)
	{
		fprintf(fp, "%"PRIu64, m->extents[i].id);
		for(k=0; k<m->dimensions; k++)
		{
			fprintf(fp, " %"PRId64" %"PRId64, m->extents[i].bounds[k].lower, m->extents[i].bounds[k].upper);
		}
		fprintf(fp,"\n");
	}
	fputs("#Updates <id> <D1 edges> [<D2 edges>]...\n", fp);
	for(i=m->subscriptions; i<m->extent_number; i++)
	{
		fprintf(fp, "%"PRIu64, m->extents[i].id);
		for(k=0; k<m->dimensions; k++)
		{
			fprintf(fp, " %"PRId64" %"PRId64, m->extents[i].bounds[k].lower,m->extents[i].bounds[k].upper);
		}
		fprintf(fp,"\n");
	}
	fclose(fp);
	return err_none;
}

_ERR_CODE move_extents_random_waypoints(MatchingInstance * m, uint32_t iteration_number, movement_restriction restriction, void (*instance_solver)(MatchingInstance *,bitmatrix *), char * output_dir, SPACE_TYPE velocity, SPACE_TYPE acceleration)
{
    uint64_t i, j, g;
    uint32_t r;
    uint_fast16_t k;
    waypoint * waypoints;
    FILE * fp;
    SPACE_TYPE temp;
    uint64_t extent_width;
    char filename[MAX_FILE_PATH_LENGHT];
    extent * ext;
    uint64_t waypoint_count = 0;

   //Allocating waypoints
    switch(restriction)
    {
        case RESTR_NONE: //move all extents
            waypoint_count = m->subscriptions+m->updates;
            ext = m->extents;
            break;
        case RESTR_LOCK_SUBSCRIPTION:
            waypoint_count = m->updates;
            ext = &(m->extents[m->subscriptions]);
            break;
        case RESTR_LOCK_UPDATES:
            waypoint_count = m->subscriptions;
            ext = m->extents;
            break;
        default:
            return set_error(err_invalid_input, __FILE__,__FUNCTION__,__LINE__);
    }
    //Alloc waypoints data structure
    waypoints = (waypoint*) malloc(sizeof(waypoint)*(waypoint_count));
    if(waypoints == NULL)
        return set_error(err_alloc,__FILE__,__FUNCTION__,__LINE__);

    for(i=0; i<waypoint_count; i++)
    {
        waypoints[i].destination = (SPACE_TYPE*) malloc(sizeof(SPACE_TYPE) * m->dimensions);
        if(waypoints[i].destination == NULL)
        {
            free(waypoints);
            return set_error(err_alloc, __FILE__,__FUNCTION__,__LINE__);
        }
        waypoints[i].reached = true;
    }
    for(r = 1; r < iteration_number; r++)
    {
        //Move extents and set dirty bit if moved
        bool reached;
        for(j=0; j<waypoint_count; j++)
        {
            if(waypoints[j].reached)
            {
                //Generate new random waypoint
                if(rand() % 10 == 0)
                {
                //printf("%"PRIu64" starts moving again on iteration %"PRIu64"\n", ext[j].id, i);
                    for(k=0; k<m->dimensions; k++)
                    {
                        waypoints[j].destination[k] = BIGRANDOM(SPACE_TYPE_MIN, SPACE_TYPE_MAX);
                    }
                    waypoints[j].reached = false;
                }
                else
                {
                    continue;
                }
            }
            reached = true;
            for(k=0; k<m->dimensions; k++)
            {
                if(ext[j].bounds[k].lower != waypoints[j].destination[k])
                {
                    reached = false;
                    if(ext[j].bounds[k].lower > waypoints[j].destination[k])
                    {
                        extent_width = ext[j].bounds[k].upper - ext[j].bounds[k].lower;
                        temp = ext[j].bounds[k].lower;
                        ext[j].bounds[k].lower -= velocity;
                        if(temp < ext[j].bounds[k].lower)
                        {
                            //Overflow detected
                            ext[j].bounds[k].lower = SPACE_TYPE_MIN;
                            ext[j].bounds[k].upper = SPACE_TYPE_MIN + extent_width;
                        }
                        else
                        {
                            //Move upper bound too, this operation can't overflow
                           ext[j].bounds[k].upper -= velocity;
                        }

                        if(ext[j].bounds[k].lower <= waypoints[j].destination[k])
                        {
                            //If the extent overtook the destination, move it back
                            ext[j].bounds[k].upper += (waypoints[j].destination[k] - ext[j].bounds[k].lower);
                            ext[j].bounds[k].lower = waypoints[j].destination[k];
                        }
                    }
                    else
                    {
                        extent_width = ext[j].bounds[k].upper - ext[j].bounds[k].lower;
                        temp = ext[j].bounds[k].upper;
                        ext[j].bounds[k].upper += velocity;
                        if(temp > ext[j].bounds[k].lower)
                        {
                            //Overflow detected
                            ext[j].bounds[k].upper = SPACE_TYPE_MAX;
                            ext[j].bounds[k].lower = SPACE_TYPE_MAX - extent_width;
                        }
                        else
                        { //Move the lower bound too, this operation can't overflow
                           ext[j].bounds[k].lower += velocity;
                        }

                        if(ext[j].bounds[k].lower > waypoints[j].destination[k])
                        {
                             //If the extent overtook the destination, move it back
                            ext[j].bounds[k].upper -= (ext[j].bounds[k].lower - waypoints[j].destination[k]);
                            ext[j].bounds[k].lower = waypoints[j].destination[k];
                        }
                    }
                    /*Another possible way to handle the overflow of the extent out of the bound SPACE_TYPE_MIN AND SPACE_TYPE_MAX
                    if(ext[j].bounds[k].upper < ext[j].bounds[k].lower)
                    {//One possible solution: Swap them
                        temp = ext[j].bounds[k].upper;
                        ext[j].bounds[k].upper = ext[j].bounds[k].lower;
                        ext[j].bounds[k].lower = temp;
                    }*/
                }
            }
            ext[j].dirtyBit = true;
            if(reached == true)
            {
                //printf("Extent %"PRIu64" reached its waypoint on iteration: %"PRIu64".\n", ext[j].id, i);
                waypoints[j].reached = true;
            }
        }

        //Solve this instance
        reset_whole_bit_mat(*(m->matching_result), m->updates, m->subscriptions);
        (*instance_solver)(m, m->matching_result);

        //Print output file as changelog (call function)

        sprintf(filename, "%s/"INPUT_FILE_NAME,output_dir, r);
        fp = fopen(filename,"w");
        if(fp == NULL)
            return set_error(err_file, __FILE__,__FUNCTION__, __LINE__);
            bool firstextent = true;
            switch (restriction)
            {
                case RESTR_NONE:

                //Note that this case don't have a break; as we want to fall in the cases below

                case RESTR_LOCK_UPDATES:
                for(g=0; g<m->subscriptions; g++)
                {
                    if(m->extents[g].dirtyBit)
                    {
                        if(firstextent)
                        {//Print header
                        fprintf(fp, "#Subscriptions <id> <D1 edges> [<D2 edges>]...\ns\n");
                        firstextent=false;
                        }
                        m->extents[g].dirtyBit = false;
                        fprintf(fp, "%"PRIu64" ", m->extents[g].id);
                        for(k=0; k<m->dimensions; k++)
                        {
                            fprintf(fp, " %"PRId64" ""%"PRId64, m->extents[g].bounds[k].lower, m->extents[g].bounds[k].upper);
                        }
                        fputs("\n", fp);
                    }
                }
                if(restriction == RESTR_LOCK_UPDATES)
                    break;
                //else if it was RESTR_NONE print update extents too.

                case RESTR_LOCK_SUBSCRIPTION:
                    firstextent=true;
                for(g=m->subscriptions; g<m->extent_number; g++)
                {
                    if(m->extents[g].dirtyBit)
                    {
                    if(firstextent)
                    {   //Print header
                        fprintf(fp, "#Updates <id> <D1 edges> [<D2 edges>]...\nu\n");
                        firstextent=false;
                    }
                        m->extents[g].dirtyBit = false;
                        fprintf(fp, "%"PRIu64, m->extents[g].id);
                        for(k=0; k<m->dimensions; k++)
                        {
                            fprintf(fp, " %"PRId64" %"PRId64, m->extents[g].bounds[k].lower, m->extents[g].bounds[k].upper);
                        }
                        fputs("\n", fp);
                    }
                }
                break;
            }
        fclose(fp);

        //print bitmatrix result
        snprintf(filename, MAX_FILE_PATH_LENGHT, "%s/"OUTPUT_FILE_NAME, output_dir, r); //N.B. OUTPUT_FILE_NAME cointains a placeholder
        print_bit_matrix(*(m->matching_result), filename, true, m->updates, m->subscriptions);

    }//end for
    free(waypoints);
    return err_none;
}

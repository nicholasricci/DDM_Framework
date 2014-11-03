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


/**
*   \brief This file contains the DDMInstanceMarker main function
*   \author Francesco Bedini
*   \file InstanceMakerMain.c
*/

#include "defines.h"
#include "types.h"
#include "error.h"
#include "ExtentMaker.h"
#include "bitmatrix.h"

#include <stdio.h>
#include <inttypes.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>
#include <omp.h>

/**
*	\brief Prints help on standard output.
	 * This function prints the program help on standard output.
	 * It is called when input parameters are incorrect or when is requested by user via -h flag.
	 */
void print_help(void){
		printf("\n\nSYNOPSIS:\n");
		printf("-d <number of dimensions as unsigned int>\n"
		"-u <number of update extents  as unsigned int>\n"
		"-s <number of subscription extents as unsigned int>\n"
		"-n \"Instance Name\"\n"
		"[-r <random seed as unsigned int>]\n"
		"[-l sequence lenght]\n"
		"[-a \"Author Name\"]\n[-R 0|1|2]\n"
		"[-v \"Version number\"]\n"
		"[-S <extent average size>]\n"
		"[-k]\n"
		"\n-l number of instances that are generated. Must be greater than 0. Default value is 1.\n"
		"-R handles extents movement resitrictions:\n"
		"\t0: no restrictions,\n"
		"\t1: lock update extents,\n"
		"\t2: lock subscription extents.\n"
		"-k allows you to sKip the confirmation prompt.\n\n");
}

/**
*	\brief Solves an instance using the brute force algorithm.
	 * This procedure solves a given instance using the brute force approach.
	 * \param instance A MatchingInstance to solve
	 * \param result A bitmatrix where to store the result on
	 */
void brute_force_solver(MatchingInstance * instance, bitmatrix * result)
{
uint64_t i, j;
uint_fast16_t k;
//int parallel = false;
//#pragma omp parallel if (parallel)
{
//#pragma omp single
	//printf("\nBRUTE FORCE SOLVER: Created %d threads\n", omp_get_num_threads()); //TODO remove
//#pragma omp for private (i, j, k) if (parallel)
    for(i=0; i<instance->subscriptions; i++) //For each subscription extent
	{
		for(j=instance->subscriptions; j< instance->extent_number; j++) // For each update extent
		{
			for(k=0; k<instance->dimensions; k++) //for each dimension
			{//TODO just one if with multiple "OR" connected conditions may be more efficient
				if(instance->extents[j].bounds[k].lower<= instance->extents[i].bounds[k].lower
					&& instance->extents[i].bounds[k].lower <= instance->extents[j].bounds[k].upper) //if u.lower <= s.lower <= u.upper
				{
					continue;
				}
				else if(instance->extents[j].bounds[k].lower <= instance->extents[i].bounds[k].upper
					&& instance->extents[i].bounds[k].upper <= instance->extents[j].bounds[k].upper) //if u.lower <= s.upper <= u.upper
				{
					continue;
				}
				else if(instance->extents[i].bounds[k].lower <= instance->extents[j].bounds[k].lower
				&& instance->extents[j].bounds[k].lower <= instance->extents[i].bounds[k].upper) //if s.lower <= u.lower <= s.upper
				{
                    continue;
				}
				else if(instance->extents[i].bounds[k].lower <= instance->extents[j].bounds[k].upper
				&& instance->extents[j].bounds[k].upper <= instance->extents[i].bounds[k].upper)//if s.lower <= u.upper <= s.upper
				{
                    continue;
				}
				else
				{
                    k = instance->dimensions + 1; //Exit from this for and use k as a flag
                }
			}
			if(k == instance->dimensions) //Exit has not been forced
			{
				//If they are matched, set matching_result entry using their IDs
				//printf("Subscr %"PRIu64" overlapped to update %"PRIu64"\n", instance->extents[i].id, instance->extents[j].id);
				set_bit_mat(*(instance->matching_result), instance->extents[j].id , instance->extents[i].id); //set(update, subscr)
			}
		}//end for updates
	}//end for subscriptions
} //end parallel
}

/**
 * \brief The main entry point for DDMInstanceMaker
 * This is the main entry point for DDMInstanceMaker, code execution starts from here.
 * \param argc the number of parameters received +1 (the command name, the first parameter)
 * \param argv a vector of string that cointains the parameters (trimmed by blank space).
 */
int main (const int argc, const char * argv[]){
	//Variable declaration
	int i;
	bool confirm;
	MatchingInstance * instance;
	uint_fast16_t dimensions;
	uint32_t sequence_length;
	movement_restriction mov_restriction;
	uint64_t updates;
	uint64_t subscriptions;
#ifdef __DEBUG
	double startTime, endTime;
	clock_t start, end;
	double elapsed_cpu_time;
#endif
	bool SkipConfirmation;
	int AuthorName; //The index of argv where author name is stored
	int Version;	//The index of argv where version string is stored
	int instanceName; //The index of argv where the istance name is stored
	uint64_t extent_average_size;
	char WorkingDirectory [MAX_FILENAME_LENGTH];
	char FilenameBuffer [MAX_FILE_PATH_LENGHT];
	unsigned int RandomSeed;

	//Initializizing omp, setting number of thread = number of cores
	omp_set_num_threads(omp_get_num_procs());
	//This inizializations are used to check for input error
	instance = NULL;
	updates = subscriptions = RandomSeed = dimensions = extent_average_size = 0;
	AuthorName = Version = instanceName = 0;
	sequence_length = 1; //i.e. Produce just the initial file
	mov_restriction = RESTR_NONE;
	SkipConfirmation = false;
	WorkingDirectory[0] = '\0';

    RandomSeed = (unsigned int)time(NULL);

	//Checking user input
	printf("\n--------------------------------------------------------\n");
	if (argc < 5 || (argc>1 && strcmp(argv[1], "--help") == 0))
	{
		print_help();
		return 0;
	}
	for(i=1; i<argc; i++)
	{
		if(argv[i][0] == '-')
		{
			switch (argv[i][1])
			{
			case 'd': //Set dimensions
				dimensions = atoi(argv[++i]);
				if(dimensions == 0)
				{
					printf("Bad dimensions number format. Required: -d <unsigned int>\n");
					return -1;
				}
				break;
			case 'u': //Set updates number
				updates = atoi(argv[++i]);
				if(updates == 0)
				{
					printf("Bad updates number format. Required: -u <unsigned int>\n");
					return -1;
				}
				break;
			case 's': //Set subscriptions number
				subscriptions = atoi(argv[++i]);
				if(subscriptions == 0)
				{
					printf("Bad subscriptions number format. Required: -s <unsigned int>\n");
					return -1;
				}
				break;
			case 'a': //Set author name
				AuthorName = ++i;
				break;
			case 'n': //Set instance name
					//TODO controllare che sia più corto della define.
				instanceName = ++i;
				break;
            case 'R': //Set
                mov_restriction = atoi(argv[++i]);
				break;
			case 'S': //Set extent Size
				extent_average_size = atoll(argv[++i]);
				break;
            case 'l':
                sequence_length = atoi(argv[++i]);
                if(sequence_length < 1)
                    print_runtime_error_string("-l parameter must be greater than 0", true);
                break;
			case 'v': //Set version string
				Version=++i;
				break;
			case 'r': //Set random seed
				RandomSeed = atoi(argv[++i]);
				if(RandomSeed == 0 && argv[i][1] != '\0')
				{
                    print_runtime_error_string("Bad Random Seed format. Required: -r <unsigned int>", true);
				}
				break;
			case 'k': //Don't ask for confirmation before making instance
				SkipConfirmation = true;
				break;
			case 'h': //Print help
				print_help();
				return 0;
			default: //Unknown parameter, execution aborted
				printf("\nParameter '%s' unrecognised\n", argv[i]);
				print_help();
				return 0;
				break;
			}
		}
	}
	//If a compulsory parameter was not provided, abort execution
	if(subscriptions == 0 || dimensions == 0 || updates == 0 || instanceName == 0)
	{
		printf("\nA required parameter was not provided or it's invalid (please check wheter -d, -u, -s and -n are correctly provided)\n");
		print_help();
		return EXIT_FAILURE;
	}


	//Print summary
	printf("\nCreating a DDM Instance with following parameters:\n\n");
	printf("Dimension(s):\t\t %"PRIuFAST16"\nUpdate Extents:\t\t %"PRIu64"\nSubscription Extents:\t %"PRIu64"\n", dimensions, updates, subscriptions);
	printf("Instance name:\t\t %s\n", argv[instanceName]);
	if(Version>0)
		printf("Version number:\t\t %s\n", argv[Version]);
	if(AuthorName>0)
		printf("Author name:\t\t %s\n", argv[AuthorName]);
	printf("Extent average size:\t");
	if(extent_average_size == 0)
		printf("Random\n");
	else
		printf("%"PRIu64"\n", extent_average_size);
	printf("Random seed: %u (Randomly generated if not provided)\n", RandomSeed);
    printf("Sequence length: %"PRIu32"\n", sequence_length);
    if(sequence_length > 1){
    printf("Movement restriction: ");
    switch(mov_restriction)
    {
    case RESTR_NONE:
        printf("none\n");
        break;
    case RESTR_LOCK_UPDATES:
        printf("Update extents don't change position.\n");
    break;
    case RESTR_LOCK_SUBSCRIPTION:
        printf("Subscription extents don't change position.\n");
    break;
    }
    }
    	if(!SkipConfirmation){
		do{
			printf("\nContinue? (y|n): ");
			confirm = getchar();
			getchar(); //to skip \n next time;
		}
		while(confirm != 'y' && confirm !='n');
		if(confirm=='n')
		{
			printf("Excecution aborted\n");
			return 0;
		}
	}
	//Create istance folder
	strcpy(WorkingDirectory, argv[instanceName]);
	if(mkdir(WorkingDirectory,  S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) != 0)
	{
		//This directory already exists, execution aborted
		printf("Error: This directory already exists, delete it or use another instance name.\n");
		return EXIT_FAILURE;
	}

	//Create the subfolder results inside the instance folder
	sprintf(FilenameBuffer,"cd ./%s && mkdir results && cd ..", WorkingDirectory);
	system(FilenameBuffer);

	//Creating instance using provided parameters
	if(generate_fixed_extent_set(&instance, RandomSeed, subscriptions, updates, dimensions, extent_average_size) != err_none)
	{
		//An error occurred, aborting.
		print_error_string();
		exit(EXIT_FAILURE);
	}

    //Save instance infos on file
	sprintf(FilenameBuffer, "%s/"INFO_FILE_NAME, WorkingDirectory);

	if(save_instance_info_on_file(FilenameBuffer, argv[instanceName], argv[AuthorName],
		Version != 0 ? argv[Version]:"1.0", true,  RandomSeed, sequence_length, instance->dimensions, instance->subscriptions, instance->updates) != err_none)
	{
		//An error occurred, aborting.
		print_error_string();
		return EXIT_FAILURE;
	}

    //Saving first instance to file
	sprintf(FilenameBuffer, "%s/"INPUT_FILE_NAME, WorkingDirectory, 0); //N.B. INPUT_FILE_NAME cointains a %d
	if(save_to_file(instance, FilenameBuffer, true) != err_none)
	{
		//An error occurred, aborting.
		print_error_string();
		return EXIT_FAILURE;
	}
	#ifdef __DEBUG
	printf("\nInput instance successfully generated.\n");
    #endif
	//Computing optimal solution

	//Start timers (for debugging purpose only)
#ifdef __DEBUG
	start = clock();
	startTime= omp_get_wtime();
#endif //__DEBUG
	//Solving it with "Brute force" parallel algorithm.
	brute_force_solver(instance, instance->matching_result);
#ifdef __DEBUG
	end = clock();
	endTime = omp_get_wtime();
	elapsed_cpu_time=(double)(end-start)/CLOCKS_PER_SEC;
	printf("\nTime elapsed: %.3fs. Total CPU time:  %.3fs, saving file\n", (endTime - startTime) , elapsed_cpu_time);
#endif //__DEBUG

	//Saving optimal solution on output file
	snprintf(FilenameBuffer, MAX_FILE_PATH_LENGHT, "%s/"OUTPUT_FILE_NAME, WorkingDirectory, 0);
	print_bit_matrix(*(instance->matching_result), FilenameBuffer, true, updates, subscriptions); //TODO check if fails

    //generate other sequence_length-1 instances that
    if(move_extents_random_waypoints(instance, sequence_length, mov_restriction, &brute_force_solver, WorkingDirectory, 1000000000000000, 1) != err_none)
        {
            print_error_string();
            return EXIT_FAILURE;
        }

	//Free matrix
	free_matching_instance(&instance, true);

	printf("\nInstance generation completed\n");
	return EXIT_SUCCESS;
}

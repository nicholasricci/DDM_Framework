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

/** \file DDMBench.c
\brief It cointains DDMBench main

The file DDMBench.c cointains the main function of DDMBench
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>		//remember to use -lm (link math) to compile this
#include <string.h>
#include <time.h>
//#include <unistd.h>		//used to get process id (to get peak memory size)
#include <omp.h>

#include "defines.h"
#include "error.h"
#include "bitmatrix.h"
#include "ExtentMaker.h"
#include "Algorithm.h"
#include "execution_time.h"
#include "DDM_input_output.h"


/*DDM variable */
DDM_Input *ddm_input;

/**
*	\brief Prints help on standard output.
	 * This function prints the program help on standard output.
	 * It is called when input parameters are incorrect or when is requested by user via -h flag.
	 */
void printHelp(void){
		printf("\n\nSYNOPSIS:\n");
		printf("-i <instance name>\n");
		printf("-v prints version number and exits\n");
		printf("-c prints copyright notice and exits\n");
		printf("-h shows the help and exits\n\n");
}

uint64_t computeScore(double avgTime, double avgNormDistance, int memoryPeak)
{
    return (uint64_t)(((avgTime) * 10000.0) + avgNormDistance*(100000.0) + (memoryPeak/1024.0) * 0.50);
}

/**
 * \brief Loads optimal solution from file.
 * This procedure loads the optimal solution of the given instance.
 * \param out a bitmatrix * where the bitmatrix loaded from file will be stored.
 * \param instancename a string that cointains the name of the instance whose output must be retrieved.
 * \param iteration the iteration step whose bitmatrix has to be loaded
 * \return an error code if reading fails for any reason (file might not exist...).
 */
_ERR_CODE loadOpitmalSolution(bitmatrix * out, char * instancename, int iteration)
{
	char app [FILENAME_MAX];
	sprintf(app, "%s/"OUTPUT_FILE_NAME, instancename, iteration); //N.B. OUTPUT_FILE_NAME cointains a %d
	uint64_t rows, columns;
	if(read_bit_matrix(out, app, &rows, &columns) != err_none)
	{
		return print_error_string();
	}
	return err_none;
}

/**
 * \brief This function checks if solution is optimal or suboptimal.
 * This function checks that extents that overlaps in the optimal solution overlaps on the algorithm result too (otherwise solution is wrong).
 * \param optimal a bitmatrix pointer to the optimal solution
 * \param algorithm_result a bitmatrix pointer to the computed solution
 * \param updates, the number of update extents (that gives us the number of rows of the bitmatrix).
 * \param subscr, the number of subscr extents (that gives us the number of columns of the bitmatrix).
 * \return a boolean value which is true if the computed solution is optimal or suboptimal, false otherwise.
 */
bool checkSolution(bitmatrix * optimal, bitmatrix * algorithm_result, const uint64_t updates, const uint64_t subscr)
{
	return checkSet (*optimal, *algorithm_result, updates, subscr);
}

/**
 * \brief The main entry point for DDMBench
 * This is the main entry point for DDMBench, code execution starts from here.
 * \param argc the number of parameters received +1 (the command name, the first parameter)
 * \param argv a vector of string that cointains the parameters (trimmed by blank space).
 */
int main(int argc, char * argv[]) {

	//Variable declaration
	int instanceName;
	int * distances;
	double * norm_distances;
	int peakMemory = 0;
	FILE * fp;
	char buffer [BUFFER_SIZE];
    char line [LINE_MAX_LENGTH];
    uint64_t score;

	bitmatrix * output = NULL;
	bitmatrix * optimal_solution = NULL;
	optimal_solution = (bitmatrix *) malloc(sizeof(bitmatrix));
	int i;
	uint32_t sequence_lenght;
	uint64_t subscriptions, updates;

	uint_fast8_t dimensions;
	bool getMemoryPeakOnly;
	time_t now;
	double avg_elapsed_time;
	double std_deviation;
	double avg_distance;
	double avg_distance_normalized;

	sequence_lenght = 0;
	subscriptions = updates = 0;
	dimensions = 0;
	avg_distance = avg_distance_normalized = 0.0;
	instanceName = 0;
	getMemoryPeakOnly = true;
	
	ddm_input = DDM_Initialize_Input(argc, argv);
	updates = DDM_Get_Updates(*ddm_input);
	subscriptions = DDM_Get_Subscriptions(*ddm_input);
	dimensions = DDM_Get_Dimensions(*ddm_input);
	
	sequence_lenght = 1;
	//TODO/*Insert the read of dimensions, subscriptions and updates*/

	//printf("Updates %d Subscription %d Dimensions %"PRIuFAST8" Sequence %d\n", updates, subscriptions, dimensions, sequence_lenght);
    if(updates == 0 || subscriptions == 0 || dimensions == 0 || sequence_lenght == 0)
    {
        //print error and exit
        print_runtime_error_string("Error: Instance file is not correct\n", true);
    }

    distances = (int *) malloc(sequence_lenght * sizeof(int));
    norm_distances = (double *)  malloc(sequence_lenght * sizeof(double));
    if(distances == NULL || norm_distances == NULL || time_init(sequence_lenght) != err_none)
        {
            set_error(err_alloc, __FILE__,__FUNCTION__, __LINE__);
            print_error_string();
        }

    printf("Running %s on %s with the following parameters:\n", getAlgorithmName(), argv[instanceName]);
    printf("Updates: %"PRIu64"\nSubscription: %"PRIu64"\nDimensions: %"PRIuFAST8"\nSequence length: %"PRIu32"\n", updates, subscriptions, dimensions, sequence_lenght);


        //if this instance is only used to compute peak memory exit
		if(getMemoryPeakOnly)
		{
            if (ExecuteAlgorithm(argv[instanceName], dimensions, subscriptions, updates, 0, &output) != err_none)
            {
                //Error code received, execution aborted
                print_error_string();
                exit(EXIT_FAILURE);
                //return EXIT_FAILURE;
            }

			free_bit_matrix (output);
            exit(EXIT_SUCCESS);
		}

	//Execute the proposed algorithm
	/*for(i=0; i < sequence_lenght; i++){

		//Execure Algorithm
		if (ExecuteAlgorithm(argv[instanceName], dimensions, subscriptions, updates, i, &output) != err_none)
		{
			//Error code received, execution aborted
			print_error_string();
			exit(EXIT_FAILURE);
			//return EXIT_FAILURE;
		}
        //Solution Check

		//Load optimal solution in memory
		if(loadOpitmalSolution(optimal_solution, argv[instanceName], i) != err_none)
		{
			print_error_string();
			exit(EXIT_FAILURE);
		}


		//Check if solution is optimal or suboptimal
		if(!checkSolution(optimal_solution, output, updates, subscriptions))
		{
			print_bit_matrix(*output, "wrong_solution.dat", true, updates, subscriptions);
            printf("Detected incorrect solution on step %d. Wrong solution has been saved in \"wrong_solution.dat\"\n", i);
            print_runtime_error_string("Solution is not correct, at least one match was not detected.", true);
		}

        //Solution evaluation (how many pairs are wrongly matched)
        //TODO CONTROLLARE CON DEBUG?
		distances[i] = compareBitmatix(*optimal_solution, *output, updates, subscriptions);
		norm_distances[i] = distances[i] == 0 ? 0.0 : (distances[i] / ((double)((subscriptions * updates) - count_ones(*optimal_solution, updates, subscriptions))));
		avg_distance += distances[i];
		avg_distance_normalized += norm_distances[i];
        printf("Iteration %d/%d done. Time taken: %gs, Distance: %g\n", (i+1) , sequence_lenght, get_time(i), norm_distances[i]);
	}
	printf("Total time: %g, Total Distance: %g\n", get_totalTime(), avg_distance);
	avg_elapsed_time = get_totalTime()/sequence_lenght;
	std_deviation = sqrt((get_totalTime2()/sequence_lenght)-(avg_elapsed_time*avg_elapsed_time));
	avg_distance /= sequence_lenght;
	//Get memory peak from /proc virtual filesystem
/*	sprintf(buffer,"grep VmPeak /proc/%d/status|awk '{print $2}'", (int)pid); //N.B. 'self' used instead of 'pid' would retrieve shell memory
	fp = popen(buffer, "r");
	if(fp == NULL)
	{
		printf("Couldn't retrieve memory max size");
		return EXIT_FAILURE;
	}
	if(fgets(buffer, sizeof(buffer)-1, fp) == NULL)
		{
		printf("Couldn't retrieve memory peak");
		return EXIT_FAILURE;
	}
	peakMemory = atoi(buffer);
	pclose(fp);
	printf("\nMemory peak: %d kB\n", peakMemory);//*/

		//Free memory
		//free_bit_matrix(&optimal_solution);
	/*	free_bit_matrix(output);


		//Output on standard output
		printf("The proposed algorithm (%s) took an average time of %.3fs to solve %s (std. dev: %.3fs).\n"
		"Average distance from optimal solution is %.4f (%.2f%%)",getAlgorithmName() , avg_elapsed_time, argv[instanceName], std_deviation, avg_distance, avg_distance_normalized);
		if (avg_distance == 0.0)
		{
			printf(": solutions computed are optimal.\n");
		}
		puts("\n");

		//Retrieve peak memory launching another time this instance with parameter p
        printf("Retrieving memory peak launching another instance\n");
		int res = snprintf(buffer,BUFFER_SIZE, "valgrind --tool=massif --pages-as-heap=yes --massif-out-file=massif.out %s -i %s -p 1> /dev/null 2> /dev/null; cat massif.out | grep mem_heap_B | sed -e 's/mem_heap_B=\\(.*\\)/\\1/' | sort -g | tail -n 1 && rm massif.out", argv[0],argv[2]);
        if(res >= BUFFER_SIZE)
        {
            set_error(err_unhandled, __FILE__, __FUNCTION__, __LINE__);
            print_error_string();
        }
		fp = popen(buffer, "r");
		if(fp == NULL)
		{
			print_runtime_error_string("FATAL ERROR: couldn't retrieve memory max size. Did you installed Valgrind? (Run 'sudo apt-get install valgrind')", true);
		}
		if(fgets(buffer, sizeof(buffer)-1, fp) == NULL)
			{
			print_runtime_error_string("FATAL ERROR: couldn't retrieve memory max size. Did you installed Valgrind? (Run 'sudo apt-get install valgrind')", true);
		}
		peakMemory = atoi(buffer);
		pclose(fp);
		if(peakMemory == 0)
		{
			print_runtime_error_string("Couldn't retrieve memory max size", true);
		}
		printf("\nMemory peak: %d B, %g kB, %g MB, %g GB\n", peakMemory, peakMemory/1024.0, peakMemory/1048576.0, peakMemory/1073741824.0);

    //Compute score
    score = computeScore(avg_elapsed_time, avg_distance_normalized, peakMemory);
	printf("'%s' avg. time: %gs (std. dev. %gs)\navg. distance: %g (%g%%)\nmemory peak: %dB\nScore: %"PRIu64"\n-----------------------\n",
	getAlgorithmName(), avg_elapsed_time, std_deviation, avg_distance,avg_distance_normalized, peakMemory, score);
	//Append results on output file (instancename/ALGORITHM_NAME.txt)
	snprintf(buffer, BUFFER_SIZE,"%s/results/%s.txt",argv[instanceName], getAlgorithmName());
	printf("Printing out results on output file \"%s\"\n", buffer);
	fp = fopen(buffer,"a+"); //Opens output file in append mode.
	if(fp == NULL)
	{
        fprintf(stderr,"*** Error ***\nCannot append on file %s.\n", buffer);
        exit(EXIT_FAILURE);
	}
	time(&now); //Get current time
	fprintf(fp, "-----Matching Bench v. "DDM_BENCH_VERSION " %s\n", ctime(&now));
	fprintf(fp, "Average completion time: %.3fs std. dev: %.3fs\n", avg_elapsed_time, std_deviation);
	fprintf(fp, "Memory Peak: %dB\n", peakMemory);
	fprintf(fp, "Distance from optimal solution: %.4f (%.2f%%)\n", avg_distance, avg_distance_normalized);
	fprintf(fp, "Total score: %"PRIu64"\n", score);
	fputs("\n---Detailed view-------------\n",fp);
	fputs("Iteration\tElapsed Time\t\tDistance\n",fp);
	for(i=0; i<sequence_lenght; i++)
	{
        fprintf(fp,"%9d\t%12.3f\t\t%8d\n", i , get_time(i), distances[i]);
	}
	fputs ("------------------------------------------------------------\n\n",fp);
	fclose(fp);
	printf("Result file saved.\n");

	//Print the result on the rank file
	snprintf(buffer, BUFFER_SIZE,"%s/rank.txt",argv[instanceName]);
    printf("Printing rank file in %s.\n", buffer);
    fp = fopen(buffer, "a+");
    if(fp == NULL)
    {
        fprintf(stderr,"*** Error ***\nCannot append on file %s.\n", buffer);
        exit(EXIT_FAILURE);
    }
    fprintf(fp,"%"PRIu64" %s (t: %gs, d: %g (%g%%), m: %gkB) on %s",score, getAlgorithmName(), avg_elapsed_time, avg_distance, avg_distance_normalized, peakMemory/1024.0, ctime(&now));
    fclose(fp);

    //Ordering it
    snprintf(buffer, BUFFER_SIZE, "sort %s/rank.txt -n -o  %s/rank.txt", argv[instanceName],argv[instanceName]);

    int systemRet = system(buffer);
    if(systemRet == -1){
      // The system method failed
        fprintf(stderr,"*** Error ***\nCannot order rank file %s.\n", buffer);
        exit(EXIT_FAILURE);
    }
	printf("Rank file saved and ordered.\n");


	//Saving summary result on log file
	printf("\nSaving summary result on \"%s\" file\n", LOG_FILENAME);
    snprintf(buffer,BUFFER_SIZE,"'%s' executed instance '%s' on %s\navg. time: %.3fs (std. dev. %.3f)\navg. distance: %.4f\nmemory peak: %dB\nScore: %"PRIu64"\n-----------------------\n",
	getAlgorithmName(), argv[instanceName], ctime(&now), avg_elapsed_time, std_deviation, avg_distance, peakMemory, score);
	WriteLogLine(buffer);
    printf("Updating csv file...\n");
    fp = fopen(CSV_FILENAME,"r");
    if (fp == NULL)
    {
        printf("Csv file does not exits, trying to create it\n");
        fp = fopen(CSV_FILENAME, "w");
        if(fp == NULL)
        {
            fprintf(stderr, "CSV result file can't be created in "CSV_FILENAME"\n");
            exit(EXIT_FAILURE);
        }
        //Print header
        fprintf(fp,"Method,Instance,Score,\"Average Time(s)\",\"Average Distance\",\"Normalized Distance\",\"Memory Peak(B)\",Timestamp\n");
    }
    else
    {
        fclose(fp);
        fp = fopen(CSV_FILENAME, "a");
        if(fp == NULL)
        {
            fprintf(stderr, "Can't append on csv result file: "CSV_FILENAME"\n");
            exit(EXIT_FAILURE);
        }
    }
    //Here we have an open file in fp
    fprintf(fp, "\"%s\",\"%s\",%"PRIu64",%.6f,%.6f,%.6f,%d,%s", getAlgorithmName(), argv[instanceName], score, avg_elapsed_time, avg_distance, avg_distance_normalized, peakMemory, ctime(&now));
    fclose(fp);
    printf("CSV result file saved\n");
    free(distances);
    free(norm_distances);
    time_free();
    printf("\nAll done!\n");*/
	return EXIT_SUCCESS;
}

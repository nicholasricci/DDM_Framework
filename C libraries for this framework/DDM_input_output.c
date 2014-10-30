/**
 *This library is reccomended for who want to use your algorithms inside this DDM framework.
 *To use this library needs to copy this files .c and .h into your folder and linked in your make file.
 *In this library contains the function for the input and the output.
 *The output: it creates a file .txt with executable name of the program and leave the file in the bin folder.
 *The input: check if parameters contain correct value and return extents, alfa and dimension.
 */

/**
 * INCLUDE
 */

#include "DDM_input_output.h"

/**
 * INPUT
 */

DDM_Input* Initialize_DDM_Input(int argc, char* argv[]){
  DIR *dir;
  FILE *file_input;
  char test[1000];
  DDM_Input *ddm_input = (DDM_Input *) malloc(sizeof(DDM_Input));
 
  if (argc == 5){ 
    sprintf(ddm_input->type_test, "%s", argv[1]);
    if (strncmp(argv[1], "alfa", 4) == 0){
      ddm_input->extents = atoi(argv[2]);
      ddm_input->dimensions = atoi(argv[3]);
      ddm_input->alfa = atof(argv[4]);
      ddm_input->updates = (size_t) (ddm_input->extents / 2);
      ddm_input->subscriptions = (size_t) (ddm_input-extents / 2);
      ddm_input->list_updates = NULL;
      ddm_input->list_subscriptions = NULL;
    }else{
      ddm_input->dimensions = atoi(argv[2]);
      ddm_input->updates = atoi(argv[3]);
      ddm_input->subscriptions = atoi(argv[4]);
      ddm_input->extents = ddm_input->updates + ddm_input->subscriptions;
      ddm_input->alfa = 0.0;
      sprintf(test, FOLDER_CHECK, argv[1]);
      if (dir = opendir(test)){
	//Directory Exists
	//Check if file input-0 exists
	sprintf(test, FILE_CHECK, argv[1], TEST_INPUT);
        if (file_input = fopen(test, "r")){
            printf("\nFile %s exists!\n", TEST_INPUT);
            ddm_input->list_subscriptions = (DDM_Extent *) malloc(sizeof(DDM_Extent) * ddm_input->subscriptions);
            ddm_input->list_updates = (DDM_Extent *) malloc(sizeof(DDM_Extent) * ddm_input->updates);
            read = getline(&line, &len, file_input);
            for (i = 0; i < ddm_input->subscriptions; ++i){
                read = getline(&line, &len, file_input);
                sscanf(line, "%d %n", &(ddm_input->list_subscriptions[i].id), &nchar);
                line = line + nchar;
                for (j = 0; j < ddm_input->dimensions; ++j){
                    sscanf(line, "%lf %lf %n", &(ddm_input->list_subscriptions[i].lower[j]), &(ddm_input->list_subscriptions[i].upper[j]), &nchar);
                    line = line + nchar;
                }
            }
            read = getline(&line, &len, file_input);
            for (i = 0; i < ddm_input->updates; ++i){
                read = getline(&line, &len, file_input);
                sscanf(line, "%d %n", &(ddm_input->list_updates[i].id), &nchar);
                line = line + nchar;
                for (j = 0; j < ddm_input->dimensions; ++j){
                    sscanf(line, "%lf %lf %n", &(ddm_input->list_updates[i].lower[j]), &(ddm_input->list_updates[i].upper[j]), &nchar);
                    line = line + nchar;
                }
            }

            //TEST
            /*for (i = 0; i < ddm_input->subscriptions; ++i){
                sprintf(test, "%d ", ddm_input->list_subscriptions[i].id);
                for (j = 0; j < ddm_input->dimensions; ++j){
                    sprintf(test, "%s %lf %lf ", test, ddm_input->list_subscriptions[i].lower[j], ddm_input->list_subscriptions[i].upper[j]);
                }
                printf("%s\n", test);
            }
            for (i = 0; i < ddm_input->updates; ++i){
                sprintf(test, "%d ", ddm_input->list_updates[i].id);
                for (j = 0; j < ddm_input->dimensions; ++j){
                    sprintf(test, "%s %lf %lf ", test, ddm_input->list_updates[i].lower[j], ddm_input->list_updates[i].upper[j]);
                }
                printf("%s\n", test);
            }*/
        }else{
            printf("\nFile info or input doesn't exist!\n");
	    free(ddm_input);
	    ddm_input = NULL;
        }
        //Close directory
        closedir(dir);
      }else if (ENOENT == errno){
	printf("\nDirectory doesn't exists!\n");
	free(ddm_input);
	ddm_input = NULL;
      }else{
	printf("\nSome error to open directory!\n");
	free(ddm_input);
	ddm_input = NULL;
      }
    }
  }
  
  return ddm_input;
}

size_t DDM_Get_Extents(int argc, char* argv[]){
 
  if (argc == 5 && strncmp(argv[1], "alfa", 4) == 0)
    return atoi(argv[2]);
  
  return 0;
}

size_t DDM_Get_Dimension(int argc, char* argv[]){
  
  if (argc == 5 && strncmp(argv[1], "alfa", 4) == 0)
    return atoi(argv[3]);
  return 0;
}

float DDM_Get_Alfa(int argc, char* argv[]){
  
  if (argc == 5 && strncmp(argv[1], "alfa", 4) == 0)
    return atof(argv[4]);
  return 0;
}

size_t DDM_Get_Updates(int argc, char* argv[]){
  
  if (argc == 5 && strncmp(argv[1], "alfa", 4) == 0)
    return (size_t) (atoi(argv[2]) / 2);
  return 0;
}

size_t DDM_Get_Subscriptions(int argc, char* argv[]){
  
  if (argc == 5 && strncmp(argv[1], "alfa", 4) == 0)
    return (size_t) (atoi(argv[2]) / 2);
  return 0;
}

/**
 * OUTPUT
 */

void DDM_Write_Result(char* argv[], double total_time){
  /* write results */
  char str[100];
  strcpy(str, argv[0]);
  strcat(str, ".txt");
  FILE* fout = fopen(str, "a");
  if ( fout == NULL ) {
      printf("Error creating file %s\n", str);
      exit(-1);
  }

  fprintf(fout, "%f\n", total_time);
  fclose(fout);
}

/**
 * TIMER
 */

void DDM_Start_Timer(DDM_Timer *ddm_timer){
  ddm_timer->start = clock();
  ddm_timer->total = 0;
}

void DDM_Stop_Timer(DDM_Timer *ddm_timer){
  ddm_timer->end = clock();
  ddm_timer->total = ((float)(ddm_timer->end - ddm_timer->start)) / CLOCKS_PER_SEC;
}

float DDM_Get_Total_Time(DDM_Timer ddm_timer){
  return ddm_timer.total;
}
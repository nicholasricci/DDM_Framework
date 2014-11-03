/**
 *This library is reccomended for who want to use your algorithms inside this DDM framework.
 *To use this library needs to copy this files .c and .h into your folder and linked in your make file.
 *In this library contains the function for the input and the output.
 *The output: it creates a file .txt with executable name of the program and leave the file in the bin folder.
 *The input: check if parameters contain correct value and return extents, alfa and dimension.
 */

/******************************************
 **************** INCLUDE *****************
 ******************************************/

#include "DDM_input_output.h"

/******************************************
 **************** SUPPORT *****************
 ******************************************/

/*char** str_split(char* a_str, const char a_delim)
{
    char** result    = 0;
    size_t count     = 0;
    char* tmp        = a_str;
    char* last_comma = 0;
    char delim[2];
    delim[0] = a_delim;
    delim[1] = 0;

    // Count how many elements will be extracted. /
    while (*tmp)
    {
        if (a_delim == *tmp)
        {
            count++;
            last_comma = tmp;
        }
        tmp++;
    }

    // Add space for trailing token. /
    count += last_comma < (a_str + strlen(a_str) - 1);

    // Add space for terminating null string so caller
    //  knows where the list of returned strings ends. /
    count++;

    result = malloc(sizeof(char*) * count);

    if (result)
    {
        size_t idx  = 0;
        char* token = strtok(a_str, delim);

        while (token)
        {
            assert(idx < count);
            *(result + idx++) = strdup(token);
            token = strtok(0, delim);
        }
        assert(idx == count - 1);
        *(result + idx) = 0;
    }

    return result;
}*/

/******************************************
 ***************** INPUT ******************
 ******************************************/

DDM_Input* DDM_Initialize_Input(int argc, char* argv[]){
  DIR *dir;
  FILE *file_input;
  char test[1000];
  DDM_Input *ddm_input = (DDM_Input *) malloc(sizeof(DDM_Input));
  int i, j, nchar;
  size_t len;
  char line[LINE_MAX_LENGTH];
  char *addr_line;
  //char **tokens;
 
  if (argc == 5){ 
    sprintf(ddm_input->type_test, "%s", argv[1]);
    if (strncmp(argv[1], "alfa", 4) == 0){
      ddm_input->extents = atoi(argv[2]);
      ddm_input->dimensions = atoi(argv[3]);
      ddm_input->alfa = atof(argv[4]);
      ddm_input->updates = (size_t) (ddm_input->extents / 2);
      ddm_input->subscriptions = (size_t) (ddm_input->extents / 2);
      ddm_input->list_updates = NULL;
      ddm_input->list_subscriptions = NULL;
    }else{
      ddm_input->dimensions = atoi(argv[2]);
      ddm_input->updates = atoi(argv[3]);
      ddm_input->subscriptions = atoi(argv[4]);
      ddm_input->extents = ddm_input->updates + ddm_input->subscriptions;
      ddm_input->alfa = 0.0;
      sprintf(test, FOLDER_CHECK, argv[1]);
      if ((dir = opendir(test)) != NULL){
	//Directory Exists
	//Check if file input-0 exists
	sprintf(test, FILE_CHECK, argv[1], TEST_INPUT);
        if ((file_input = fopen(test, "r")) != NULL){
            printf("\nFile %s exists!\n", TEST_INPUT);
            ddm_input->list_subscriptions = (DDM_Extent *) malloc(sizeof(DDM_Extent) * ddm_input->subscriptions);
            ddm_input->list_updates = (DDM_Extent *) malloc(sizeof(DDM_Extent) * ddm_input->updates);
            fgets(line, sizeof line, file_input);
	    for (i = 0; i < ddm_input->subscriptions; ++i){
		fgets(line, sizeof line, file_input);
                sscanf(line, "%zu%n", &(ddm_input->list_subscriptions[i].id), &nchar);
		addr_line = line + nchar;		
                for (j = 0; j < ddm_input->dimensions; ++j){
                    sscanf(addr_line, " %lf %lf %n", &(ddm_input->list_subscriptions[i].lower[j]), &(ddm_input->list_subscriptions[i].upper[j]), &nchar);
		    addr_line = line + nchar;
                }
                //printf("%zu\n", ddm_input->list_subscriptions[i].id);
            }
            
            fgets(line, sizeof line, file_input);
	    for (i = 0; i < ddm_input->updates; ++i){
		fgets(line, sizeof line, file_input);
                sscanf(line, "%zu%n", &(ddm_input->list_updates[i].id), &nchar);
		addr_line = line + nchar;
                for (j = 0; j < ddm_input->dimensions; ++j){
                    sscanf(addr_line, " %lf %lf %n", &(ddm_input->list_updates[i].lower[j]), &(ddm_input->list_updates[i].upper[j]), &nchar);
		    addr_line = line + nchar;
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

int DDM_Is_Alfa_Test(DDM_Input ddm_input){
  
  if ((strncmp(DDM_Get_Test_Type(ddm_input), "alfa", 4)) == 0)
    return 1;
  return 0;
}

char* DDM_Get_Test_Type(DDM_Input ddm_input){
  char *tmp = (char *) malloc(sizeof(char) * 100);
  if (ddm_input.type_test != NULL){
    sprintf(tmp, "%s", ddm_input.type_test);
    return tmp;  
  }
  return NULL;
}

size_t DDM_Get_Extents(DDM_Input ddm_input){
 
  if (ddm_input.extents != 0)
    return ddm_input.extents;
  return -1;
}

size_t DDM_Get_Dimensions(DDM_Input ddm_input){
  
  if (ddm_input.dimensions != 0)
    return ddm_input.dimensions;
  return -1;
}

float DDM_Get_Alfa(DDM_Input ddm_input){
  
  if (ddm_input.alfa != 0)
    return ddm_input.alfa;
  return -1;
}

size_t DDM_Get_Updates(DDM_Input ddm_input){
  
  if (ddm_input.updates != 0)
    return ddm_input.updates;
  return -1;
}

size_t DDM_Get_Subscriptions(DDM_Input ddm_input){
  
  if (ddm_input.subscriptions != 0)
    return ddm_input.subscriptions;
  return -1;
}

DDM_Extent* DDM_Get_Updates_List(DDM_Input ddm_input){
  if (ddm_input.list_updates != NULL)
    return ddm_input.list_updates;
  return NULL;
}

DDM_Extent* DDM_Get_Subscriptions_List(DDM_Input ddm_input){
  if (ddm_input.list_subscriptions != NULL)
    return ddm_input.list_subscriptions;
  return NULL;
}

/******************************************
 ***************** OUTPUT *****************
 ******************************************/

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

/******************************************
 ***************** TIMER ******************
 ******************************************/

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
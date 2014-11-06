#include "Algorithm.h"
#include "DDM_input_output.h"

/**
* \brief getAlgorithmName
  \brief Insert here your algorithm name.
  \return a char* containing your approach' name
	 * This will be used as the name of your result file. All your result will be appended there.
*/
static MatchingInstance * instance;

extern DDM_Input *ddm_input;

const float Lmax = 1.0e6; /* dimension length */

char * getAlgorithmName(void)
{
    return "Binary Partition Based";
}

typedef struct extents_edges{
uint64_t id;
bool isUpdateExtent;
bool isLower;
SPACE_TYPE position;
} extent_edge;


void Write_Result(double total_time){
  FILE* fout = fopen("binary_partition.txt", "a");
  if ( fout == NULL ) {
      printf("Error creating file binary_partition_based.txt\n");
      exit(-1);
  }

  fprintf(fout, "%f\n", total_time);
  fclose(fout);
}

_ERR_CODE readEntireInput(MatchingInstance ** instance, const uint_fast8_t dimensions, const uint32_t subscriptions, const uint32_t updates, const char * filename)
{
	FILE * fp = NULL;
	uint64_t i;
	char line [LINE_MAX_LENGTH];
	int j;
	int character_read;
	char * buffer;
	DDM_Extent *list_updates, *list_subscriptions;
	float alfa, l;
	
	generate_empty_extent_set(instance,subscriptions, updates, dimensions);
	
	  list_updates = DDM_Get_Updates_List(*ddm_input);
	  list_subscriptions = DDM_Get_Subscriptions_List(*ddm_input);
	  
		//fgets(rigan, LINE_MAX_LENGTH, fp); //Skip #line //TODO Solve
		for(i=0; i<subscriptions; i++)
		{
			//Reading subscription extents
            /*ReadNextString(line, fp);
			if((sscanf(line,"%"PRIu64"%n",&((*instance)->extents[i].id),&character_read)) != 1)
                return set_error(err_invalid_input, __FILE__, __FUNCTION__, __LINE__);*/
			(*instance)->extents[i].id = (_INT)list_subscriptions[i].id;
			(*instance)->extents[i].dirtyBit = false;
            //buffer=line+ character_read;
			for(j=0; j<dimensions; j++){
				//if(sscanf(buffer, "%"PRId64" %"PRId64"%n",&((*instance)->extents[i].bounds[j].lower),&((*instance)->extents[i].bounds[j].upper), &character_read)!=2)
                    //return set_error(err_invalid_input, __FILE__, __FUNCTION__, __LINE__);
                //buffer+= character_read;
			  (*instance)->extents[i].bounds[j].lower = (SPACE_TYPE)list_subscriptions[i].lower[j];
			  (*instance)->extents[i].bounds[j].upper = (SPACE_TYPE)list_subscriptions[i].upper[j];
				//printf("\nLETTO: %"PRId64" %"PRId64"\n",(*instance)->extents[i].bounds[j].lower, (*instance)->extents[i].bounds[j].upper);
			}
		}
		//fgetc(fp); //Skips \n
		//fgets(rigan, LINE_MAX_LENGTH, fp); //Skip #line
		for(i = subscriptions; i < (subscriptions + updates); i++)
		{
            /*ReadNextString(line, fp);
			//Reading update extents
			if(sscanf(line,"%"PRIu64"%n",&((*instance)->extents[i].id), &character_read) != 1)
                return set_error(err_invalid_input, __FILE__, __FUNCTION__, __LINE__);*/
				(*instance)->extents[i].id = (_INT)list_updates[i - subscriptions].id; //TODO control if is right
				(*instance)->extents[i].dirtyBit = false;
				//buffer=line+character_read;
				for(j=0; j<dimensions; j++)
				{
                    /*if (sscanf(buffer, "%"PRId64" %"PRId64"%n",&((*instance)->extents[i].bounds[j].lower),&((*instance)->extents[i].bounds[j].upper), &character_read) != 2)
                        return set_error(err_invalid_input, __FILE__, __FUNCTION__, __LINE__);
                    buffer += character_read;*/
				  (*instance)->extents[i].bounds[j].lower = (SPACE_TYPE)list_updates[i - subscriptions].lower[j];
				  (*instance)->extents[i].bounds[j].upper = (SPACE_TYPE)list_updates[i - subscriptions].upper[j];
				}
		}
	
	
	return err_none;
}

_ERR_CODE readChangelogInput(MatchingInstance * instance, const uint_fast8_t dimensions, const uint32_t subscriptions, const uint32_t updates, const char * filename)
{
	FILE * fp = NULL;
	uint64_t id;
	uint64_t offset;
	char line [LINE_MAX_LENGTH];
	char * buffer;
    int character_read;
    int j;

	fp = fopen(filename, "r");
	if(fp != NULL)
	{
		//fgets(rigan, LINE_MAX_LENGTH, fp); //Skip #line //TODO Solve
		ReadNextString(line, fp);
		if(line[0]=='s') //Reading Subscription extent
		{
            offset = 0;
		}
		else if (line[0] == 'u') //Reading update extent, subscription extent didn't move
		{
            offset = subscriptions;
		}
		else
		{
		    if(!feof(fp))
                return set_error(err_invalid_input, __FILE__,__FUNCTION__, __LINE__);
            //Else file is empty, no extents has been moved
		}
		if(ReadNextString(line, fp))
		{
            do{
                if(sscanf(line,"%"PRIu64"%n",&id, &character_read) != 1)
                    return set_error(err_invalid_input, __FILE__,__FUNCTION__, __LINE__);
                instance->extents[offset+id].dirtyBit = true;
                buffer = line + character_read;
                for(j=0; j<dimensions; j++)
                {
                    //printf("ID: %"PRIu64"\n",id);
                    if (sscanf(buffer, "%"PRId64" %"PRId64"%n",&(instance->extents[offset + id].bounds[j].lower),&(instance->extents[offset+id].bounds[j].upper), &character_read)!=2)
                        return set_error(err_invalid_input, __FILE__,__FUNCTION__, __LINE__);
                    buffer += character_read;
                }
            if(!ReadNextString(line, fp))
                break;
            }while(line[0]!='s' && line[0]!='u');
		}

		if(line[0]=='u')
		{
            offset = subscriptions;
            do
            {
                if(ReadNextString(line, fp))
                {
                    if (sscanf(line,"%"PRIu64"%n",&id, &character_read)!=1)
                        return set_error(err_invalid_input, __FILE__,__FUNCTION__, __LINE__);
                    instance->extents[offset+id].dirtyBit = true;
                    buffer = line + character_read;
                    for(j=0; j<dimensions; j++)
                    {

                        if (sscanf(buffer, "%"PRId64" %"PRId64"%n",&(instance->extents[offset+id].bounds[j].lower),&(instance->extents[offset+id].bounds[j].upper), &character_read)!=2)
                            return set_error(err_invalid_input, __FILE__,__FUNCTION__, __LINE__);
                        buffer += character_read;
                    }
                }
                else
                    break;
            }while(line[0]!='s' && line[0]!='u');
		}
		fclose(fp);
	}
	else
	{
		perror("fopen");
		return set_error(err_file, __FILE__, __FUNCTION__, __LINE__);
	}
	return err_none;
}

void check_matched (list S, list U, uint_fast8_t current_dim, bitmatrix * partition_result)
{
    if(S != NULL && U != NULL)
    {
        list_element * tempS;
        list_element * tempU;
        tempS = S;
        endpoints * UpdateExtent;
        endpoints * SubscrExtent;
        while(tempS != NULL)
        {
            tempU = U;
            while(tempU != NULL)
            {
                UpdateExtent = &(instance->extents[instance->subscriptions+tempU->id].bounds[current_dim]);
                SubscrExtent = &(instance->extents[tempS->id].bounds[current_dim]);
                if (((UpdateExtent->lower <= SubscrExtent->lower) &&  (SubscrExtent->lower <= UpdateExtent->upper))
                || ((UpdateExtent->lower <= SubscrExtent->upper) &&  (SubscrExtent->upper <= UpdateExtent->upper))
                || ((SubscrExtent->lower <= UpdateExtent->upper) &&  (UpdateExtent->upper <= SubscrExtent->upper))
                || ((SubscrExtent->lower <= UpdateExtent->lower) &&  (UpdateExtent->lower <= SubscrExtent->upper))){
                  //printf("\nu_id: %d, lower: %d, upper: %d - s_id: %d, lower: %d, upper: %d\n", tempU->id, UpdateExtent->lower, UpdateExtent->upper, tempS->id, SubscrExtent->lower, SubscrExtent->upper);  
		  set_bit_mat(*partition_result, tempU->id, tempS->id);
		}
                tempU = tempU->next;
            }
            tempS = tempS->next;
        }
    }
}

int compar (const void* p1, const void* p2)
{
    extent_edge e1 = *(extent_edge*)p1;
    extent_edge e2 = *(extent_edge*)p2;
    if(e1.position < e2.position)
    return -1;
    if(e1.position == e2.position)
    return 0;
    else
    return 1;
}

void Partition (extent_edge * edges, const uint64_t edges_count, const double lower_Edge, const double upper_edge, uint_fast8_t current_dim, bitmatrix * partition_result)
{
    //Recursion termination test
    if(edges_count <= 2)
    {
        //printf("Recursion done! (Edges count = %"PRIu64"\n",edges_count);
                return;
    }
    //else
    //{
        //static int step = 0;
        //printf("Passo %d\n. Edges_count= %"PRIu64"\n da %g a %g\n", step++, edges_count, lower_Edge, upper_edge);
   // }

    list Sl, Sp, Sr, Ul, Up, Ur;
    list_init(&Sl);
    list_init(&Sp);
    list_init(&Sr);
    list_init(&Ul);
    list_init(&Up);
    list_init(&Ur);

    uint64_t Csl, Csr, Cul, Cur;
    Csl = Csr = Cul = Cur = 0;

    double pivot = lower_Edge + (upper_edge - lower_Edge)/2.0;
    //printf("Pivot value: %g\n",pivot);
    uint64_t i;
    for(i=0; i< edges_count; i++) //For each edge
    {
        if(edges[i].position < pivot)
        {
            if(! edges[i].isLower) //If this extents finishes before the pivot value
            {
                if(edges[i].isUpdateExtent) //If it is an update extent
                {
                    //printf("Adding %"PRIu64" to list Ul\n", edges[i].id);
                    list_add_by_id(&Ul,edges[i].id);
                    Cul++;
                }
                else //Is a Subscriber
                {
                    //printf("Adding %"PRIu64" to list Sl\n", edges[i].id);
                    list_add_by_id(&Sl, edges[i].id);
                    Csl++;
                }
            } //Else if is upper nobody cares...
        }
        else //If this elements is greater or equal to the pivot value
        {
            if(edges[i].isLower) //If this edge is the lower bound
            {
                //This extents begin after the pivot
                if(edges[i].isUpdateExtent) //If it is an update extent
                {
                    //printf("Adding %"PRIu64" to list Ur\n", edges[i].id);
                    list_add_by_id(&Ur, edges[i].id);
                    Cur++;
                }
                else //Is a Subscriber
                {
                    //printf("Adding %"PRIu64" to list Sr\n", edges[i].id);
                    list_add_by_id(&Sr, edges[i].id);
                    Csr++;
                }
            }
            else
            {
                if(edges[i].isUpdateExtent) //If it is an update extent
                {
                    //If this extent has not begun after the pivot
                    if(is_in_list(Ur, edges[i].id) == NULL)
                    {
                        //printf("Adding %"PRIu64" to list Up\n", edges[i].id);
                        list_add_by_id(&Up,edges[i].id);
                    }
                }
                else //Is a Subscriber
                {
                    //If this extent has not begun after the pivot
                    if(is_in_list(Sr, edges[i].id) == NULL)
                    {
                        //printf("Adding %"PRIu64" to list Sp\n", edges[i].id);
                        list_add_by_id(&Sp,edges[i].id);
                    }
                }
            }
        }
    } //Partition completed
    //Set Matched the elements in Sp and Up
    list_element * tempS;
    list_element * tempU;
    tempS = Sp;
    if(Sp != NULL && Up != NULL)
    {
        while(tempS != NULL)
        {
            tempU = Up;
            while(tempU != NULL)
            {
                set_bit_mat(*partition_result, tempU->id, tempS->id);
                tempU = tempU->next;
            }
            tempS = tempS->next;
        }
    }
    //check for matching:
    check_matched(Sr, Up, current_dim, partition_result);
    check_matched(Sl, Up, current_dim, partition_result);
    check_matched(Sp, Ur, current_dim, partition_result);
    check_matched(Sp, Ul, current_dim, partition_result);

    //printf("Ul %"PRIu64", Sl %"PRIu64", Ur %"PRIu64", Sr %"PRIu64"\n", Cul, Csl, Cur, Csr);
    //Continue recursion
    //Create new Edges vector
    extent_edge * EdgesRight = (extent_edge *) malloc(sizeof(extent_edge)*(Cur+Csr)*2);
    extent_edge * EdgesLeft = (extent_edge *) malloc(sizeof(extent_edge)*(Cul+Csl)*2);
    if(EdgesRight == NULL || EdgesLeft == NULL)
    {
        fprintf(stderr, "Partition failed allocating memory");
        exit(EXIT_FAILURE);
    }
    //Fill edges vector
    //Fill EdgesLeft with Ul + Sl
    tempU = Ul;
    for(i=0; i<Cul*2; i+=2)
    {
        EdgesLeft[i].id = tempU->id;
        EdgesLeft[i].isLower = true;
        EdgesLeft[i].isUpdateExtent = true;
        EdgesLeft[i].position = instance->extents[instance->subscriptions+tempU->id].bounds[current_dim].lower;
        EdgesLeft[i+1].id = tempU->id;
        EdgesLeft[i+1].isLower = false;
        EdgesLeft[i+1].isUpdateExtent = true;
        EdgesLeft[i+1].position = instance->extents[instance->subscriptions+tempU->id].bounds[current_dim].upper;
        tempU = tempU->next;
    }
    tempS = Sl;
    for(i=Cul*2; i<(Csl+Cul)*2; i+=2)
    {
        EdgesLeft[i].id = tempS->id;
        EdgesLeft[i].isLower = true;
        EdgesLeft[i].isUpdateExtent = false;
        EdgesLeft[i].position = instance->extents[tempS->id].bounds[current_dim].lower;
        EdgesLeft[i+1].id = tempS->id;
        EdgesLeft[i+1].isLower = false;
        EdgesLeft[i+1].isUpdateExtent = false;
        EdgesLeft[i+1].position = instance->extents[tempS->id].bounds[current_dim].upper;
        tempS = tempS->next;
    }

    //Fill Edgesright with Ur + Sr
    tempU = Ur;
    for(i=0; i<Cur*2; i+=2)
    {
        EdgesRight[i].id = tempU->id;
        EdgesRight[i].isLower = true;
        EdgesRight[i].isUpdateExtent = true;
        EdgesRight[i].position = instance->extents[instance->subscriptions+tempU->id].bounds[current_dim].lower;
        EdgesRight[i+1].id = tempU->id;
        EdgesRight[i+1].isLower = false;
        EdgesRight[i+1].isUpdateExtent = true;
        EdgesRight[i+1].position = instance->extents[instance->subscriptions+tempU->id].bounds[current_dim].upper;
        tempU = tempU->next;
    }
    tempS = Sr;
    for(i=Cur*2; i<(Csr+Cur)*2; i+=2)
    {
        EdgesRight[i].id = tempS->id;
        EdgesRight[i].isLower = true;
        EdgesRight[i].isUpdateExtent = false;
        EdgesRight[i].position = instance->extents[tempS->id].bounds[current_dim].lower;
        EdgesRight[i+1].id = tempS->id;
        EdgesRight[i+1].isLower = false;
        EdgesRight[i+1].isUpdateExtent = false;
        EdgesRight[i+1].position = instance->extents[tempS->id].bounds[current_dim].upper;
        tempS = tempS->next;
    }

    //Order the two vectors:
    //TODO Error here something goes wrong
    printf("\n%d\n", edges_count);
    qsort (EdgesRight, (Cur+Csr)*2, sizeof(extent_edge), &compar);
    qsort (EdgesLeft, (Cul+Csl)*2, sizeof(extent_edge), &compar);

    //Recursive calls

    if(Cul != 0 && Csl != 0)
    {
           //printf("Calling recursive function on LEFT (%"PRIu64")\n",(Cul+Csl)*2);
           Partition(EdgesLeft, (Cul+Csl)*2,lower_Edge,pivot, current_dim, partition_result);
    }
    else
    {
            //printf("NOT Calling recursive function on LEFT (%"PRIu64")\n",(Cul+Csl)*2);
    }
    if(Cur != 0 && Csr != 0)
    {
        //printf("Calling recursive function on RIGHT (%"PRIu64")\n",(Cur+Csr)*2);
        Partition(EdgesRight, (Cur+Csr)*2, pivot, upper_edge, current_dim, partition_result);
    }
    else
    {
         //printf("NOT calling recursive function on RIGHT (%"PRIu64")\n",(Cur+Csr)*2);
    }
    free(EdgesLeft);
    free(EdgesRight);
    list_remove_all(&Sl);
    list_remove_all(&Sp);
    list_remove_all(&Sr);
    list_remove_all(&Ul);
    list_remove_all(&Up);
    list_remove_all(&Ur);
}

_ERR_CODE ExecuteAlgorithm(const char * InstanceName, const uint_fast8_t dimensions, const uint32_t subscriptions, const uint32_t updates, const uint32_t iteration,  bitmatrix ** result)
{
	//Declare vars
	uint64_t i;
	uint64_t edge_count=0;
	uint_fast8_t k;
	char filename [1000];

    static extent_edge ** edges;
    DDM_Timer ddm_timer;

	sprintf(filename,"%s/"INPUT_FILE_NAME, InstanceName, iteration); //N.B. INPUT_FILE_NAME cointains a %d

	if(iteration == 0)
	{ //Initialization of static variables
		if(readEntireInput(&instance, dimensions, subscriptions, updates, filename) != err_none)
		{
		    return print_error_string();
		}
		//"List" initialization
        edges = (extent_edge**) malloc(sizeof(extent_edge*) * dimensions);
        if(edges == NULL)
            return set_error(err_alloc, __FILE__,__FUNCTION__, __LINE__);
		for(k = 0; k< dimensions; k++)
		{
            edges[k] = (extent_edge*) malloc(sizeof(extent_edge) * (subscriptions + updates) * 2);
            if(edges[k] == NULL)
                return set_error(err_alloc, __FILE__,__FUNCTION__, __LINE__);
		}
		set_whole_bit_mat(*(instance->matching_result), updates, subscriptions);
    }
	else
	{
		if(readChangelogInput(instance, dimensions, subscriptions, updates, filename) != err_none)
		{
		    return print_error_string();
		}
		//Remember to clear the result bitmatrix!
		set_whole_bit_mat(*(instance->matching_result),updates, subscriptions);
	}

    //Start the timer
    start_time(iteration);
    DDM_Start_Timer(&ddm_timer);
       for(i=0; i<subscriptions; i++) //For each subscription extent
        {
            for(k=0; k<dimensions; k++)
            {
                edges[k][edge_count].id = instance->extents[i].id;
                edges[k][edge_count + 1].id = instance->extents[i].id;
                edges[k][edge_count].isUpdateExtent=false;
                edges[k][edge_count + 1].isUpdateExtent=false;
                edges[k][edge_count].isLower=true;
                edges[k][edge_count + 1].isLower=false;
                edges[k][edge_count].position= instance->extents[i].bounds[k].lower;
                edges[k][edge_count + 1].position= instance->extents[i].bounds[k].upper;
            }
            edge_count+=2;
        }
        for(i = instance->subscriptions; i< instance->extent_number; i++) // For each update extent
        {
            for(k=0; k<instance->dimensions; k++)
            {
                edges[k][edge_count].id = instance->extents[i].id;
                edges[k][edge_count + 1].id = instance->extents[i].id;
                edges[k][edge_count].isUpdateExtent=true;
                edges[k][edge_count + 1].isUpdateExtent=true;
                edges[k][edge_count].isLower=true;
                edges[k][edge_count + 1].isLower=false;
                edges[k][edge_count].position= instance->extents[i].bounds[k].lower;
                edges[k][edge_count + 1].position= instance->extents[i].bounds[k].upper;
            }
            edge_count+=2;
        }

        //edge_count = (subscriptions + updates) * 2;

        //Edges are sorted
        for(k=0; k<instance->dimensions; k++)
        {
            qsort (edges[k], edge_count, sizeof(extent_edge), &compar);
        }
	/* Prints the ordered edges
	for(k=0; k<instance->dimensions; k++)
	{
        printf("Dimensione %d\n", k);
		for(i=0; i<edge_count; i++)
		{
			printf("ID: %"PRIu64" %s pos: %"PRId64"\n",edges[k][i].id, edges[k][i].isUpdateExtent?"U":"S", edges[k][i].position);
		}
	}*/

       /* static bitvector * Sl;
        static bitvector * Sp;
        static bitvector * Sr;
        static bitvector * Ul;
        static bitvector * Up;
        static bitvector * Ur;
        Sl = (bitvector*) init_bitvector(subscriptions);
        Sp = (bitvector*) init_bitvector(subscriptions);
        Sr = (bitvector*) init_bitvector(subscriptions);
        Ul = (bitvector*) init_bitvector(updates);
        Up = (bitvector*) init_bitvector(updates);
        Ur = (bitvector*) init_bitvector(updates);

        if(Sl == NULL || Sp == NULL || Sr == NULL || Ul == NULL || Up == NULL || Ur == NULL)
        {
            return set_error(err_alloc, __FILE__, __FUNCTION__, __LINE__);
        }*/

        //Order edges in order to have Update extent before subscription extents TODO.
        extent_edge app;
        for(k=0; k<dimensions; k++)
        {
            for(i = edge_count-1; i+1 != 0; i-=2)
            {
           // printf("i: %"PRIu64"\n", i);
                if(edges[k][i].position == edges[k][i-1].position)
                {
                //printf("\a\a\a\aDue edge sono nella stessa posizione!\n");
                    if(edges[k][i].isLower == edges[k][i-1].isLower)
                    {
                        if(edges[k][i].isUpdateExtent != edges[k][i-1].isUpdateExtent)
                        {
                            if(edges[k][i].isLower)
                            {
                                //S goes before U
                                if(edges[k][i-1].isUpdateExtent)
                                {
                                    //Swap them
                                    app = edges[k][i-1];
                                    edges[k][i-1] = edges[k][i];
                                    edges[k][i] = app;
                                }
                            }
                            else
                            {
                                //U goes before S
                                if(edges[k][i].isUpdateExtent)
                                {//Swap them
                                    app = edges[k][i-1];
                                    edges[k][i-1] = edges[k][i];
                                    edges[k][i] = app;
                                }
                            }
                        }//else continue
                    }
                    else
                    {
                        if(edges[k][i].isUpdateExtent != edges[k][i-1].isUpdateExtent)
                        {
                            if(edges[k][i-1].isUpdateExtent && !(edges[k][i-1].isLower))
                            {
                                    //Swap them
                                    app = edges[k][i-1];
                                    edges[k][i-1] = edges[k][i];
                                    edges[k][i] = app;
                            }
                            else if (edges[k][i].isUpdateExtent && edges[k][i].isLower)
                            {
                                    //Swap them
                                    app = edges[k][i-1];
                                    edges[k][i-1] = edges[k][i];
                                    edges[k][i] = app;
                            }
                        }//else continue
                    }
                }
            }
        }
        bitmatrix temp_result;
	//Initialize the bitmatrix
        init_bit_matrix(&temp_result, updates, subscriptions, uninitialized);
        for(k=0; k< instance->dimensions; k++)
        {
            reset_whole_bit_mat(temp_result, updates, subscriptions);
	//Do the first call
            Partition(edges[k], edge_count, SPACE_TYPE_MIN, SPACE_TYPE_MAX, k, &temp_result);
            //AND tra matching result e temp_result
            for(i=0; i<updates; i++)
            {
                vector_bitwise_and((*(instance->matching_result))[i],temp_result[i], BIT_VEC_WIDTH(subscriptions));
            }
            //print_readable_bitmatrix(*(instance->matching_result),k,updates, subscriptions);
        }

    *result = instance->matching_result;
    DDM_Stop_Timer(&ddm_timer);
    stop_time(iteration);
    
    /*instead DDM_Write_Result(argv, total_time);*/
    Write_Result(DDM_Get_Total_Time(ddm_timer));
	//free_matching_instance(&instance,false);

	//Return err_none if everything went well
	return err_none;
}

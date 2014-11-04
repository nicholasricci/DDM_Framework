#include <stdio.h>
#include <stdlib.h>

#include "DDM_input_output.h"

const float Lmax = 1.0e6; /* dimension length */

int main(int argc, char *argv[]){
  size_t dimensions;
  size_t updates;
  size_t subscriptions;
  DDM_Extent *list_updates;
  DDM_Extent *list_subscriptions;
  DDM_Input *ddm_input;
  DDM_Timer ddm_timer;
  /*Index variables*/
  size_t i, j, k;
  /*Result variable*/
  size_t nmatch;
  int isValid;
  
  ddm_input = DDM_Initialize_Input(argc, argv);
  dimensions = DDM_Get_Dimensions(*ddm_input);
  updates = DDM_Get_Updates(*ddm_input);
  subscriptions = DDM_Get_Subscriptions(*ddm_input);
  list_updates = DDM_Get_Updates_List(*ddm_input);
  list_subscriptions = DDM_Get_Subscriptions_List(*ddm_input);
  
  nmatch = 0;
  DDM_Start_Timer(&ddm_timer);
  
  //Execute algorithm
  for (i = 0; i < updates; ++i){
#pragma omp parallel for private(j, isValid, k) reduction(+:nmatch)
    for (j = 0; j < subscriptions; ++j){
      isValid = 1;
      for (k = 0; k < dimensions; ++k){
	/*if ((list_updates[i].upper[k] < list_subscriptions[j].lower[k]) || (list_updates[i].lower[k] > list_subscriptions[j].upper[k])){
	  isValid = 0;
	  break;
	}*/
	//if ((list_updates[i].lower[k] < list_subscriptions[j].upper[k]) && (list_updates[i].upper[k] > list_subscriptions[j].lower[k])){
	if (
	  (list_updates[i].lower[k] <= list_subscriptions[j].lower[k] 
	  && list_subscriptions[j].lower[k] <= list_updates[i].upper[k])
	  ||
	  (list_updates[i].lower[k] <= list_subscriptions[j].upper[k]
	  && list_subscriptions[j].upper[k] <= list_updates[i].upper[k])
	  ||
	  (list_subscriptions[j].lower[k] <= list_updates[i].lower[k]
	  && list_updates[i].lower[k] <= list_subscriptions[j].upper[k])
	  ||
	  (list_subscriptions[j].lower[k] <= list_updates[i].upper[k]
	  && list_updates[i].upper[k] <= list_subscriptions[j].upper[k])
	  ){
	  continue;
	}else{
	  isValid = 0;
	}
	//printf("%zu: %lf %lf\n", j, list_updates[i].lower[k], list_subscriptions[j].upper[k]);
      }
      if (isValid)
	nmatch++;
    }
  }
  
  /*for (i = 0; i < updates; ++i){
    for (j = 0; j < subscriptions; ++j){
      isValid = 1;
      for (k = 0; k < dimensions; ++k){
	if ((list_updates[i].lower[k] <= list_subscriptions[j].upper[k]
	    &&
	    list_updates[i].upper[k] >= list_subscriptions[j].lower[k])
	    ||
	    (list_subscriptions[j].lower[k] <= list_updates[i].upper[k]
	    &&
	    list_subscriptions[j].upper[k] >= list_updates[i].lower[k]))
	  continue;
	else
	  isValid = 0;
      }
      if (isValid)
	nmatch++;
    }
  }*/
  
  DDM_Stop_Timer(&ddm_timer);
  
  //Write result on file
  DDM_Write_Result(argv, DDM_Get_Total_Time(ddm_timer));

  //write on screen nmatches and total time
  printf("%zu matches  %fs\n", nmatch, DDM_Get_Total_Time(ddm_timer) );
  
  
  return 0;
}

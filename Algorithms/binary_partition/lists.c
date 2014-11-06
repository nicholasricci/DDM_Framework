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


#include "lists.h"

void list_init(list * l)
{
    (*l) = NULL;
}

void list_add_by_id(list * l, const size_t element_id)
{
    list_element * new_element;
    new_element = (list_element*) malloc(sizeof(list_element));
    if(new_element == NULL)
    {
        fprintf(stderr, "List element alloc fail.\n");
        exit(EXIT_FAILURE);
    }
    new_element->id= element_id;
    new_element->next = *l;
    *l = new_element;
}

void list_add_by_id_ordered(const list l, const size_t element_id)
{
    printf("TODO ADD BY ID ORDERED\n");
    exit(EXIT_FAILURE);
}

void list_add_element(list * l, const list_element * element)
{
    printf("TODO LIST_ADD_ELEMENT\n");
    exit(EXIT_FAILURE);
}

int list_remove_by_id(list * l, const size_t element_id)
{
    list_element * temp;
    list_element * prev = NULL;
    temp = *l;
    while(temp != NULL && temp->id != element_id)
    {
        prev = temp;
        temp = (*temp).next;
    }
    if(temp != NULL)
    {   //Element found
        if(prev==NULL)
        {
            //We are removing the first element
            *l = temp->next;
        }
        else
        {
            prev->next = temp->next;
        }
        free(temp);
        return 1;
    }
    return 0;
}

list_element * is_in_list(const list l, const size_t element_id)
{
    list_element * temp;
    temp = l;
    while(temp != NULL)
    {
        if(temp->id == element_id)
            return temp;
        temp = temp->next;
    }
    return NULL;
}

list_element * is_in_ordered_list(const list l, const size_t element_id)
{
    printf("TODO IS IN ORDERED LIST\n");
    exit(EXIT_FAILURE);
}

void list_remove_all(list * l)
{
    list_element *temp;

    while (*l != NULL)
    {
        temp = *l;
        *l = (*l)->next;
        free(temp);
    }
}
void list_print(list l, char * title)
{
        list_element * temp;
        temp = l;
        printf("Printing list %s:\n", title);
        if(temp==NULL)
        {
            printf("List is empty!\n");
            return;
        }
        while(temp != NULL)
        {
            printf("%zu", temp->id);
            temp = temp->next;
        }
        printf("\n");
}

/* This is the prototype for ASSIGNMENT 1 FOR COMP20007
   Student Number: 828472
   Modules List, Queues and Stack make use of code provided by Matt Farrugia
   */

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <limits.h>

#include "traverse.h"
#include "list.h"

//==========================PART 1=============================================
//implemented with a stack
void
print_dfs(Graph* graph, int source_id){
	
	//DECLARATION AND INITIALISATION
	int stack_top;
	List *tmp_stk;
	int* vlist = (int*)calloc(graph->maxn,((graph->maxn)*sizeof(int)));
	Edge* curr_edge;
	
	//creating a temporary stack
	tmp_stk = new_list();
	
	//pushing the vertex's index into stack 
	list_add_start(tmp_stk, source_id);
	
	//looking through stack while its not empty
	while(!(list_is_empty(tmp_stk))){
		//pop stack and remember index of the top vertex
		stack_top = list_remove_start(tmp_stk);
		
		//search through all edges if stack if vertex not visited
		if(!vlist[stack_top]){
			printf("%s\n", graph->vertices[stack_top]->label);//print names
			vlist[stack_top] = 1; //mark as visited
			
			//look at first edge of vertice
			curr_edge = graph->vertices[stack_top]->first_edge;
			
			//while multiple edges exist
			while(curr_edge){
				//if already visited that vertex, look at next edge
				if(!vlist[curr_edge->v]){	
					list_add_start(tmp_stk, curr_edge->v);//pushstack	 		
					break;
				}
				curr_edge = curr_edge->next_edge;//look at next edge
			}
		}
	}	
	free_list(tmp_stk); //free the stack to prevent memory leaks
	free(vlist); //free the visited list
}



//==========================PART 2=============================================

void print_bfs(Graph* graph, int source_id){
	
	//DECLARATION AND INITIALISATION
	//qfront is index at queue front, target is vertex that we are looking at
	int i, qfront, target;
	List *tmp_q;
	int* vlist = (int*)calloc(graph->maxn,((graph->maxn)*sizeof(int)));
	Edge* curr_edge;
	
	//creating a temporary queue
	tmp_q = new_list();
	
	//creating list of visited places, marking them to unvisited as default
	for(i=0;i<(graph->maxn);i++){
		vlist[i]= 0;
	}
	
	//enqueueing the vertex's index into queue and mark as visited and print
	list_add_end(tmp_q, source_id);
	vlist[source_id] = 1;
	printf("%s\n", graph->vertices[source_id]->label);
	
	//looking through queue while it is not empty
	while(!(list_is_empty(tmp_q))){
		
		//dequeue and remember the head of queue
		qfront = list_remove_start(tmp_q);
		
		//look at first edge of vertice
		curr_edge = graph->vertices[qfront]->first_edge;
		
		//while there are edges from node at front of queue
		while(curr_edge->next_edge){			
			//if already visited that vertex, look at next edge
			if(vlist[curr_edge->v]){				
				curr_edge = curr_edge->next_edge;
			}
			else{
				target = curr_edge->v; //target would be destination
				vlist[target] = 1; //mark target as visited
				list_add_end(tmp_q, target);//enqueue breath vertices
				printf("%s\n", graph->vertices[target]->label);//print visited
			}
			
		}
		//if only one edge left
		if(!(curr_edge->next_edge)&& !(vlist[curr_edge->v])){ 
			//enqueue only edge's destination
			target = curr_edge->v;
			vlist[target] = 1; //mark target as visited
			list_add_end(tmp_q, target); 	
			printf("%s\n", graph->vertices[target]->label);//print visited
		}
	}
	free_list(tmp_q); //free the queue to prevent memory leaks
	free(vlist); //free the visited list
}

//==========================PART 3=============================================

void detailed_path(Graph* graph, int source_id, int destination_id) {
		
	//DECLARATION AND INITIALISATION
	int stack_top, total_dist=0;
	List *tmp_stk;
	int* vlist = (int*)calloc(graph->maxn,((graph->maxn)*sizeof(int)));
	Edge* curr_edge;
	
	//checking source_id != destination_id
	if(source_id == destination_id){
		printf("%s (0km)", graph->vertices[source_id]->label);
		return;
	}
	
	//creating a temporary stack
	tmp_stk = new_list();

	//pushing the vertex's index into stack 
	list_add_start(tmp_stk, source_id);
	
	//looking through stack while its not empty
	while(!(list_is_empty(tmp_stk))&& !(vlist[destination_id])){
		//pop stack and remember index of the top vertex
		stack_top = list_remove_start(tmp_stk);
		
		//search through all edges if stack if vertex not visited
		if(!vlist[stack_top]){
			
			//print names and distance
			printf("%s (%dkm)\n", graph->vertices[stack_top]->label, total_dist);
			vlist[stack_top] = 1; //mark as visited
			
			//look at first edge of vertice
			curr_edge = graph->vertices[stack_top]->first_edge;
			
			//while multiple edges exist
			while(curr_edge){
				//if already visited that vertex, look at next edge
				if(!vlist[curr_edge->v]){	
					//add the distance onto the cumulative distance
					total_dist = total_dist + (curr_edge->weight);
					list_add_start(tmp_stk, curr_edge->v); //pushstack			
					break;
				}
				curr_edge = curr_edge->next_edge;
			}

		}
	}	
	free_list(tmp_stk); //free the stack to prevent memory leaks
	free(vlist); //free the visited list
}


//==========================PART 4=============================================

//function to print out current path before moving on to next possibility
void
print_all(Graph* graph, List* print_q, int destination_id){
	int i, town_index, len = list_size(print_q);
	Node* node = print_q->head;//assign temp node to use for printing stack

	//pop stack and print while assigning to tmp array
	for(i=0;i<(len-1);i++){
		town_index = node->data;
		printf("%s, ", graph->vertices[town_index]->label);
		node = node->next;
	}
	//print destination and new line to signal end of this path
	printf("%s\n", graph->vertices[destination_id]->label);

}
//recursive function to map out all possible paths
void
route_all(Graph* graph, int current, int destination_id, int* vlist, 
          List* stack){

	Edge* curr_edge;	  
	
	//mark current node as visited and add to path list
	vlist[current]= 1;
	list_add_end(stack, current);
	
	//look at first edge of vertex
	curr_edge = graph->vertices[current]->first_edge;
	
	//if destination reached, print the current path 
	if(current == destination_id){
		print_all(graph, stack, destination_id);
	}
	
	//not reached so keep looping through recursion
	else{
		
		//looking through edges if it exists
		while(curr_edge){
			if(!vlist[curr_edge->v]){//recurse if not visited that node already
				route_all(graph, (curr_edge->v), destination_id, vlist, stack);
			}
			//look at next edge if visited already	
			curr_edge = curr_edge->next_edge;
		}
	}
	//change paths once the path is printed and reset the visit status
	vlist[current] = 0; //reset to unvisited
	current = list_remove_end(stack);
}
//main function to call the recursive function
void 
all_paths(Graph* graph, int source_id, int destination_id){
	
	int* vlist = (int*)calloc(graph->maxn,((graph->maxn)*sizeof(int)));
	//int* curr_path = (int*)malloc((graph->maxn)*sizeof(int));
	List* stack;
	stack = new_list();
	
	//checking source_id != destination_id
	if(source_id == destination_id){
		printf("%s", graph->vertices[source_id]->label);
		return;
	}
	
	//call recursive function to route the paths
	route_all(graph, source_id, destination_id, vlist, stack);
	//free the partner stack used
	free_list(stack);
	//free the visited list array
	free(vlist);
}


//==========================PART 5=============================================

//function to compare the differences in distance and set shortest path
//using brute force 
void
compare(int* shortest, int *s_distance, int* short_n, List* stack,
		int *c_distance){	
	int i;
	Node* node= stack->head;
	//if current distance is shorter than shorter distance
	if((*c_distance)< (*s_distance)){
		(*s_distance) = (*c_distance);//reassign shorter distance
		*short_n = list_size(stack);//reassign shortest list size
		
	
		//loop through the stack and replace array with stack
		for(i=0;i<(*short_n);i++){
			shortest[i] = node->data; //appending node index in
			node = node->next; //look at nextt node	
		}
	}
}

//function to print the array of shortest and its distance
void
print_shortest(Graph* graph, int* shortest, int short_n, int distance,
			   int destination_id){
	int i;
	//for every vertex in shortest path, print label
	for(i=0;i<short_n-1;i++){
		printf("%s, ", graph->vertices[(shortest[i])]->label);
	}
	//print last one and label
	printf("%s (%dkm)\n", graph->vertices[destination_id]->label, distance);
}

//main recursive function to find all and shortest path
int
route_short(Graph* graph, int current, int destination_id, int* vlist, 
          List* stack, int *shortest, int *short_n, 
		  int *s_distance, int *c_distance, Edge* prev_edge){

	Edge *curr_edge;	  
	
	//mark current node as visited and add to path list
	vlist[current]= 1;
	list_add_end(stack, current);
	//look at first edge of vertex
	curr_edge = graph->vertices[current]->first_edge;
	
	//if destination reached, compare current path with shortest
	if(current == destination_id){
		compare(shortest, s_distance,short_n, stack, c_distance);
	}
	//not reached so keep looping through recursion
	else{
		//looking through edges if it exists
		while(curr_edge){
			if(!vlist[curr_edge->v]){

				//add to distance since traversed edge
				*c_distance = (*c_distance) + (curr_edge->weight);
				prev_edge = curr_edge;
				
				//recurse if not visited that node already
				//only minus current distance if prev exists and backtracked
				if(prev_edge && route_short(graph, (curr_edge->v), 
				   destination_id, vlist,stack, shortest, short_n, s_distance, 
				   c_distance, prev_edge)){

					*c_distance = (*c_distance) - (prev_edge->weight);
				}
			}
			//look at next edge if visited already	
			curr_edge = curr_edge->next_edge;
		}
	}
	
	//change paths once the path is compared and reset the visit status
	vlist[current] = 0; //reset to unvisited
	current = list_remove_end(stack);//assign a new current vertex, backtracking
	//return if backtrack
	return 1;
}

//main function to call upon the recursive and print function
void shortest_path(Graph* graph, int source_id, int destination_id) {
	//index for shortest path and shortest and current distances
	int short_n, s_distance= INT_MAX , c_distance= 0; 
	Edge* prev_edge=NULL;
	
	int* vlist = (int*)calloc(graph->maxn,((graph->maxn)*sizeof(int)));
	int* shortest = (int*)malloc((graph->maxn)*sizeof(int));//array for shortest
	
	//creating a stack to mediate the paths
	List* stack;
	stack = new_list();
	
	//checking source_id != destination_id
	if(source_id == destination_id){
		printf("%s (0km)", graph->vertices[source_id]->label);
		return;
	}
	
	//call recursive function to route the paths
	route_short(graph, source_id, destination_id, vlist, stack, 
			  shortest, &short_n, &s_distance, &c_distance, prev_edge);

	//print shortest path
	print_shortest(graph, shortest, short_n, s_distance, destination_id);
	
	//free the partner stack used
	free_list(stack);
	//free the visited list array and shortest path
	free(vlist);
	free(shortest);
}


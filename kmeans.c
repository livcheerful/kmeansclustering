#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "kmeans.h"

double x_min = -1.0;
double x_max = 3.5;
double y_min = -1;
double y_max = 3;

list_node* list_create(){    
    list_node* head = malloc(sizeof(list_node));
    head->id = -1;
    head->next = NULL;
    return head;
}

void list_insert(list_node* list, int id){
    if(list->id ==-1){
       list->id = id;
       return;
    }
    list_node* tail = list->next;
    while(tail){
       list = list->next;
       tail = list->next;
    }
    tail = malloc(sizeof(list_node));
    list->next = tail;
    tail->id = id;
    tail->next = NULL;
}

void list_free(list_node* list){
   list_node * tail = list;
   while(list){
      tail = list->next;
      free(list);
      list = tail;
   }
}

double distance(double num_x1, double num_y1, double num_x2, double num_y2){

   double x = num_x1 - num_x2;
   double y = num_y1 - num_y2;
	
   return sqrt(x*x+y*y);
}
coordinate* get_coordinates(int len, FILE* x, FILE* y){
   coordinate* coords = malloc(sizeof(coordinate)*len); 
   for(int i = 0; i < len; i++){
      char * temp = malloc(4);
      size_t size = 4;
      getline(&temp, &size, x);
      coords[i].x = atof(temp);
      getline(&temp, &size, y);
      coords[i].y = atof(temp);
      free(temp);
   }
   return coords;
}

void print_coordinates(int len, coordinate*centers){
   for(int i = 0; i<len; i++){
      printf("%dth coordinate: [%.1f,%.1f]\n", i, centers[i].x, centers[i].y);
   }
}

void find_closest(int num_points, int k, coordinate*points, coordinate*centers, list_node** groups){
   for(int i = 0; i < num_points; i++){
      int best = 0;
      int shortest = distance(points[i].x,points[i].y,centers[0].x, centers[0].y);
      for(int j = 1; j < k; j++){
         if(distance(points[i].x,points[i].y,centers[j].x,centers[j].y)<shortest){
            shortest = distance(points[i].x,points[i].y, centers[j].x,centers[j].y);
            best = j;
         } 
      }
      list_insert(groups[best], i);
      //printf("[%.1f,%.1f] belongs to %d:[%.1f, %.1f]\n", points[i].x, points[i].y,best+1, centers[best].x, centers[best].y);
   }

}

double cluster_to_point_total(list_node*cluster, coordinate*points, double x, double y){
   double total = 0.;
   while(cluster){
      int point_id = cluster->id;
      double temp = distance(x, y, points[point_id].x, points[point_id].y);
      total += temp*temp;
      cluster = cluster->next;
   }
   return total;
}

int calculate_new_centers(double x_min, double x_max, double y_min, double y_max, int k, list_node** groups, coordinate*points, coordinate* center){
   int changed = 0;
   for(int i = 0; i < k; i ++){
      double new_x = center[i].x;
      double new_y = center[i].y;
      double smallest_dist = cluster_to_point_total(groups[i], points, center[i].x, center[i].y);
      for(double x = x_min; x < x_max; x+=.001){
         for(double y = y_min; y < y_max; y+=.001){
            double distance = cluster_to_point_total(groups[i], points, x, y);
            if(distance<smallest_dist){
               new_x = x;
               new_y = y;
               smallest_dist = distance;
            }
         }
      }
      printf("The best center for group %d is [%.3f, %.3f]\n", i+1, new_x, new_y);
      if(center[i].x!=new_x||center[i].y!=new_y){
         center[i].x = new_x;
         center[i].y = new_y;
         changed = 1;
      }
   }
   return changed;
}

void print_groups(int k, list_node** groups){
   for(int i = 0; i < k; i++){
      printf("Group %d: ", i+1);
      list_node* list = groups[i];
      while(list){
          printf("%d ", list->id);
          list = list->next;
      }
      printf("\n");
   }
}

void print_graph(int num_points, int k, coordinate* points, coordinate*centers){
   char** graph = malloc(sizeof(char*)*10);
   for(int i = 0; i < 10; i++){
      graph[i] = malloc(sizeof(char)*9);
      for(int j = 0; j < 9; j++){
         graph[i][j] = ' ';
      }
   }
   for(int i = 0; i < num_points; i++){
      int x= (points[i].x+1.)*2;
      int y= (points[i].y+1.)*2;
      graph[x][y] = 'o';
   }
   for(int i = 0; i < k; i++)
   {
      int x= (centers[i].x+1.)*2;
      int y= (centers[i].y+1.)*2;
      if(graph[x][y]==' '){
         graph[x][y] = (char)((int)'1' + i);
      }
      else{
         graph[x][y] = '*';
      }
   }

   for(int j = 9; j>=0; j--){
      for(int i = 0; i < 10; i++){
         printf("%c ", graph[i][j]);
      }
      printf("\n");
   }
}

int main(int argc, char * argv[]){
   if(argc!= 5){
      fprintf(stderr,"ERROR [usage]: ./kmeans [input x points] [input y points] [beginning x clusters] [beginning y clusters]\n");
      return -1;
   }
   FILE * x_begin = fopen(argv[3], "r");
   FILE * y_begin = fopen(argv[4], "r");
   FILE * x_points = fopen(argv[1], "r");
   FILE * y_points = fopen(argv[2], "r");
 
   if(!x_begin|!y_begin|!x_points|!y_points){
      fprintf(stderr, "ERROR [opening files]\n");
   } 
   coordinate*centers = get_coordinates(3, x_begin, y_begin);
   coordinate* points = get_coordinates(10, x_points, y_points);
   int changed = 1;
 
   int iteration = 1;
   while(changed){
      printf("-----------ITERATION %d-----------\n",iteration);
      iteration++;
      list_node**groups = malloc(sizeof(list_node*)*3);
      for(int i = 0; i < 3; i++){
         groups[i] = list_create();
      }
      find_closest(10, 3, points, centers, groups);
      print_groups(3, groups);
   //   print_graph(10,3,points,centers);
       changed = calculate_new_centers(x_min, x_max, y_min, y_max, 3, groups, points, centers);
      for(int i = 0; i < 3; i++){
         list_free(groups[i]);
      }
   }

   free(centers);
   fclose(x_begin);
   fclose(y_begin);
   fclose(x_points);
   fclose(y_points);
   return 0;
}

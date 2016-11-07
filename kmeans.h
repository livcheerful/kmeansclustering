#ifndef KMEANS_H 
#define KMEANS_H 

typedef struct list_node list_node; 

struct list_node{
    int id; //the value in the array of points
    list_node * next; //the next value in the array
};

typedef struct coordinate{
   double x;
   double y;
}coordinate;

list_node* list_create();

void list_insert(list_node * list, int id);

void list_free(list_node* list);

double distance(double num_x1, double num_y1, double num_x2, double num_y2); //find the distance between two points

coordinate * get_coordinates(int len, FILE* x, FILE* y); //from two x y files, parse coordinates together

void print_coordinates(int len, coordinate* centers); //print the list of coordinates

void find_closest(int num_points, int k, coordinate * points, coordinate* centers, list_node** groups); //for every point, find the center that it is closest to and add it to the list

int calculate_new_centers(double x_min, double x_max, double y_min, double y_max, int k, list_node** groups, coordinate*points, coordinate* center); //for every group, find the new center of the points.

void print_groups(int k, list_node** groups);//print what the points got grouped into

void print_graph(int num_points, int k, coordinate* points, coordinate* centers); //print all points and clusters in a graph


#endif

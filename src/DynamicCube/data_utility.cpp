//==========================================================================================
//Copyright 2018 ©, 2018 Minh Do, Hiep phan, Quang, Nguyen, Matthew Rinker, Ashwin Lall
//
//This file is a part of the RegretOperator project.
//
//The RegretOperator project is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    The RegretOperator project is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//   GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//along with this program. If not, see <http://www.gnu.org/licenses/>.
//==========================================================================================
//#include "stdafx.h"
#include "data_utility.h"


/*
 *  Allocate memory for a point in dim-dimensional space
 */
point_t* alloc_point( int dim)
{
  point_t* point_v;

  point_v = ( point_t*)malloc( sizeof( point_t));
  memset( point_v, 0, sizeof( point_t));

  point_v->dim = dim;

  point_v->coord = ( COORD_TYPE*)malloc( dim * sizeof( COORD_TYPE));
  memset( point_v->coord, 0, dim * sizeof( COORD_TYPE));

  point_v->id = -1;

  return point_v;
}

/*
* Allocate memory for a point in a dim-dimensional space with a id
*/
point_t* alloc_point(int dim, int id)
{
  point_t* point_v = alloc_point(dim);

  point_v->id = id;

  return point_v;
}

/*
 *  Release memory for a point
 */
void release_point( point_t* &point_v)
{
  if(point_v == NULL)
    return;

  if(point_v->coord != NULL)
  {
    free( point_v->coord);
    point_v->coord = NULL;
  }

  free( point_v);
  point_v = NULL;
}

/*
 *  Allocate memory for numberOfPoints number of points
 */
point_set_t* alloc_point_set(int numberOfPoints)
{
  point_set_t* point_set_v;

  point_set_v = ( point_set_t*)malloc( sizeof( point_set_t));
  memset( point_set_v, 0, sizeof( point_set_t));

  point_set_v->numberOfPoints = numberOfPoints;

  point_set_v->points = (point_t **) malloc(numberOfPoints * sizeof(point_t*));
  memset(point_set_v->points, 0, numberOfPoints * sizeof(point_t*));

  return point_set_v;
}

/*
 *  Release memory for numberOfPoints number of points
 */
void release_point_set(point_set_t* &point_set_v, bool clear)
{
  if(point_set_v == NULL)
    return;

  if(point_set_v->points != NULL)
  {
    if(clear)
    {
      for(int i = 0; i < point_set_v->numberOfPoints; i++)
        release_point( point_set_v->points[i]);
    }

    free(point_set_v->points);
    point_set_v->points = NULL;
  }

  free(point_set_v);
  point_set_v = NULL;
}

/*
 *  Print the coordinates for a given point
 */
void print_point(point_t* point_v)
{
  //printf("printing point:\n");
  //printf("%d ", point_v->id);
  for (int i = 0; i < point_v->dim; i++)
    printf("%f ", point_v->coord[i]);


  //DIST_TYPE diff;

  //diff = 0;

  //int dim = point_v->dim;
  //for (int i = 0; i<dim; i++)
  //{
  //  diff += point_v->coord[i] * point_v->coord[i];
  //}

  //printf("%lf", (DIST_TYPE)sqrt(diff));

  printf("\n");
}

/*
 *  For debug purose, print the contents of a given point set
 */
void print_point_set(point_set_t* point_set_v)
{
  for(int i = 0; i < point_set_v->numberOfPoints; i++)
    printf("%i ", point_set_v->points[i]->id);
    //print_point(point_set_v->points[i]);
  printf("\n");
}

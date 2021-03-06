#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/time.h>

#include "point.h"
#include "cube.h"
#include "LIB_Util.h"
#include "sphere.h"
#include "data_utility.h"
#include "operation.h"

int main(int argc, char** argv)
{
	int i, j, N, D, K, db, LIB;

	int REPEATS = 10000; // number of trials to evaluate funcit  on
	int DATAREPEATS = 10;  // number of data sets used
  int VERBOSE = 0;
  //Cube , Cube LIB, Sphere , Sphere LIB
	int NUM_METHODS = 6;
  struct timeval start, end;
  double M;

  int* maxIndex;
  char* input;
  struct point* p;
  struct point* pPrime;
  point_set_t* P;
  point_set_t* skyline;
  point_set_t *S;
	FILE* fp;
	FILE* log;


	double rr[NUM_METHODS];
	char filename[1024];
	char *methods[] = {"Cube        ", "Cube LIB       ","Sphere      ", "Sphere LIB     ", "ImpGreedy      ", "ImpGreedy LIB       ",
                      };
	if (argc < 3)
	{
		printf ("usage: ./vary-db file-prefix K \n");
		return 0;
	}

	if (argc >= 3)
  {
		K = atoi(argv[2]);
  }

  N = 10000;
	sprintf(filename, "plot/varydb-%s-k%d", argv[1], K);
	fp = fopen(filename, "w");

	sprintf(filename, "plot/varydb-%s-k%d.log", argv[1], K);
  log = fopen(filename, "w");

	for(db = 1; db <= 5; db++)
	{
		fprintf(fp, "+%d", db);
		M = db/10.0;
		for(i = 0; i < NUM_METHODS; ++i)
		{
			rr[i] = 0.0;
		}
		for(j = 0; j < DATAREPEATS; ++j)
		{
      p = new struct point[N];
      maxIndex = new int[K];
			sprintf(filename, "Data/%s-%d-%d.dat", argv[1], N, j + 1);
      cout << filename << endl;

      p = readPoints(filename);
      D = p[0].d;
      p = scale(D,p,N);


			// Cube algorithm
      cout<<"------------------------Transformed Cube-----------------------"<<j+1<<endl;
      pPrime = transformforth(db,D,p,N);
			cube(D, N, K, pPrime, maxIndex);

      i = 0;
      LIB = 0;
			rr[i] += evaluateRandomLIB(D, N, K, pPrime, maxIndex, REPEATS, LIB, db, VERBOSE, M);

			fprintf(log, "%s,db=%d, M=%lf, %lf\n", methods[i],db, M, rr[i]/(j+1));

      //Cube LIB
      cout<<"-------------------------Transformedback Cube-------------------------"<<j+1<<endl;
      i = 1;
      LIB = 1;
      rr[i] += evaluateRandomLIB(D, N, K, p, maxIndex, REPEATS, LIB, D, VERBOSE, M);

      printf("%s,db=%d, M=%lf, %lf\n",  methods[i],db, M,  rr[i]/(j+1));
      fprintf(log, "%s,db=%d, M=%lf, %lf\n", methods[i],M, N, rr[i]/(j+1));

      // Sphere algorithm
      cout<<"--------------------Transformed Sphere------------------------"<<j+1<<endl;
      P = Cube2Sphere(D,pPrime,N);
      skyline = skyline_point(P);

      gettimeofday(&start, NULL);
      S = sphereWSImpLP(skyline,K);
			gettimeofday(&end, NULL);

      i = 2;
      LIB = 0;
      for (int x = 0; x < K; x++){
        maxIndex[x] = S->points[x]->id;
      }
			rr[i] += evaluateRandomLIB(D, N, K, pPrime, maxIndex, REPEATS, LIB, db, VERBOSE, M);

			printf("%s,db=%d, M=%lf, %lf\n" , methods[i], db, M, rr[i]/(j+1));
			fprintf(log, "%s,db=%d, M=%lf, %lf\n", methods[i], db, M, rr[i]/(j+1));

      //Sphere LIB
      cout<<"---------------------Transformeback Sphere------------------------"<<j+1<<endl;
      i= 3;
      LIB = 1;
      rr[i] += evaluateRandomLIB(D, N, K, p, maxIndex, REPEATS, LIB, db, VERBOSE, M);
      printf("%s,db=%d, M=%lf, %lf\n", methods[i], db, M, rr[i]/(j+1));
      fprintf(log, "%s,db=%d, M=%lf , %lf\n", methods[i], db, M, rr[i]/(j+1));


      // Uniform direction solution
      cout<<"--------------------Transformed ImpGreedy-------------------"<<j+1<<endl;
      P = Cube2Sphere(D, pPrime, N);
			gettimeofday(&start, NULL);
			S = ImpGreedy(K, P);
			gettimeofday(&end, NULL);
      i = 4;
      LIB= 0;

      for (int x = 0; x < K; x++){
        maxIndex[x] = S->points[x]->id;
      }

			rr[i] += evaluateRandomLIB(D, N, K, pPrime, maxIndex, REPEATS, LIB, db, VERBOSE, M);
			printf("%s,db=%d, M=%lf, %lf\n" , methods[i], db, M, rr[i]/(j+1));
			fprintf(log, "%s,db=%d, M=%lf, %lf\n" , methods[i], db, M, rr[i]/(j+1));

      // Uniform direction solution
      cout<<"--------------------Transformeback ImpGreedy----------------------"<<j+1<<endl;
      i = 5;
      LIB= 1;
			rr[i] += evaluateRandomLIB(D, N, K, p, maxIndex, REPEATS, LIB, db, VERBOSE, M);

			printf("%s,db=%d, M=%lf, %lf\n" , methods[i],db, M, rr[i]/(j+1));
			fprintf(log, "%s,db=%lf, M=%d, %lf\n", methods[i],db, M, rr[i]/(j+1));
      delete[] p;
      delete[] maxIndex;
      delete[] pPrime;
		}
		printf("\n");
		for(i = 0; i < NUM_METHODS; ++i)
			fprintf(fp, "\t%lf", rr[i]/DATAREPEATS);
		fprintf(fp, "\n");
		fflush(fp);
	}

	// free up memory
  cout << "END" << endl;
	fclose(fp); fclose(log);

	return 0;
}

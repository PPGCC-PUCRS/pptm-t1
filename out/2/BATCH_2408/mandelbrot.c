#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h> 


struct complex{
  double real;
  double imag;
};


int main(int argc, char *argv[]){
  int i, j, iter, numoutside = 0, total, init, end;
  double area, error, ztemp;
  double start, finish;
  struct complex z, c;
  // MPI information variables;
  int id, procnum, size;
  
  /*
   *   
   *
   *     Outer loops run over npoints, initialise z=c
   *
   *     Inner loop has the iteration z=z*z+c, and threshold test
   */
  MPI_Status status;
  MPI_Init(&argc,&argv);
  MPI_Comm_size(MPI_COMM_WORLD, &procnum);
  MPI_Comm_rank(MPI_COMM_WORLD, &id);

  if(id == 0) { // MASTER
    MPI_Barrier(MPI_COMM_WORLD);
    start = MPI_Wtime();  
  
   // for (i=0; i<NPOINTS; i++) {
   //   for (j=0; j<NPOINTS; j++) {
   //     c.real = -2.0+2.5*(double)(i)/(double)(NPOINTS)+1.0e-7;
   //     c.imag = 1.125*(double)(j)/(double)(NPOINTS)+1.0e-7;
   //     z=c;
   //     for (iter=0; iter<MAXITER; iter++){
   //       ztemp=(z.real*z.real)-(z.imag*z.imag)+c.real;
   //       z.imag=z.real*z.imag*2+c.imag; 
   //       z.real=ztemp; 
   //       if ((z.real*z.real+z.imag*z.imag)>4.0e0) {
   //         numoutside++; 
   //         break;
   //       }
   //     }
   //   }
   // }
  
    int init = 0;
    for (i=1; i < procnum; i++) {
      int end = (i+1) * NPOINTS / procnum;
      MPI_Send(&init,1,MPI_INT,i,0,MPI_COMM_WORLD);
      MPI_Send(&end,1,MPI_INT,i,0,MPI_COMM_WORLD);
      init = end;
    }

    MPI_Reduce(&numoutside,&total,1,MPI_INT,MPI_SUM,0,MPI_COMM_WORLD);

    /*
     *  Calculate area and error and output the results
     */

    finish = MPI_Wtime();  
    area=2.0*2.5*1.125*(double)(NPOINTS*NPOINTS-total)/(double)(NPOINTS*NPOINTS);
    error=area/(double)NPOINTS;
    fprintf(stderr, "%d\t%lf\n",NPOINTS,finish-start);
    printf("Area of Mandlebrot set = %12.8f +/- %12.8f\n",area,error);

  // --------------------------------------------------
  } else { //SLAVE
    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Recv(&init,1,MPI_INT,MPI_ANY_SOURCE,0,MPI_COMM_WORLD,&status);
    MPI_Recv(&end,1,MPI_INT,status.MPI_SOURCE,0,MPI_COMM_WORLD,&status);
  
    for (;init < end; init++) {
      for (j=0; j<NPOINTS; j++) {
        c.real = -2.0+2.5*(double)(init)/(double)(NPOINTS)+1.0e-7;
        c.imag = 1.125*(double)(j)/(double)(NPOINTS)+1.0e-7;
        z=c;
        for (iter=0; iter<MAXITER; iter++) {
          ztemp=(z.real*z.real)-(z.imag*z.imag)+c.real;
          z.imag=z.real*z.imag*2+c.imag; 
          z.real=ztemp; 
          if ((z.real*z.real+z.imag*z.imag)>4.0e0) {
            numoutside++;
            break;
          }
        }
      }
    }


    MPI_Reduce(&numoutside,&total,1,MPI_INT,MPI_SUM,0,MPI_COMM_WORLD);
  } 

  MPI_Finalize();
  return 0;
}

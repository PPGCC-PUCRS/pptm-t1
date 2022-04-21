#include <stdio.h>
#include <omp.h>

int main () {
  int i;
  double x, pi, soma = 0.0, tempo, passo;
  tempo = -omp_get_wtime();
  passo = 1.0/NUM_PASSOS;
  #pragma omp parallel for private(x)
  for (i=0;i< NUM_PASSOS; i++){
      x = (i + 0.5) * passo;           // Largura do retangulo
      #pragma omp atomic
      soma = soma + 4.0 / (1.0 + x*x); // soma += Area do retangulo
  }
  pi = passo * soma;
  tempo += omp_get_wtime();
  fprintf(stderr,"%d %lf\n",NUM_PASSOS,tempo);
  printf("PASSOS=%d PI=%.20lf\n",NUM_PASSOS,pi);
  return 0;
}

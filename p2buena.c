#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include <stdlib.h>
#include <math.h>
#define UMBRAL 1

double microsegundos();
void inicializar_semilla();
void generar_aleatorio(int v[], int n);
void generar_ascendente(int v[], int n);
void generar_descendente(int v[], int n);
void mostar_vector(int v[], int n);
void ord_ins(int v[], int n);
void test_ord_insercion();
int random_num(int n, int m);
void intercambiar(int v[], int a, int b);
void ordenarAux(int v[], int izq, int der);
void ordenacionRapida(int v[],int n);
void test_ord_rapida();
void calc_tiempos(void (*tipovector)(int[],int),void (*algoritmo)(int[],int),
                  float cota_sub, float cota_ajust, float cota_sobre);
void mostrar_tablas();

int main() {
    inicializar_semilla();
    test_ord_insercion();
    test_ord_rapida();
    mostrar_tablas();
    return 0;
}

double microsegundos() {
    struct timeval t;
    if (gettimeofday(&t, NULL) < 0 )
        return 0.0;
    return (t.tv_usec + t.tv_sec * 1000000.0);
}

void inicializar_semilla() {
    srand(time(NULL));
}

void generar_aleatorio(int v[], int n) {
    int i, m=2*n+1;
    for (i = 0; i < n; i++) {
        v[i]= (rand()%m) - n;
    }
}
void generar_ascendente(int v[], int n) {
    int i;
    for (i = 0; i < n; i++) {
        v[i] = i;
    }
}
void generar_descendente(int v[], int n) {
    int i;
    for (i = n; i >= 0; i--) {
        v[i] = n-i;
    }
}

void mostar_vector(int v[], int n) {
    int i;
    for(i = 0; i < n; i++) {
        printf("%d, ", v[i]);
    }
    printf("\n");
}

void ord_ins(int v[], int n) {
    int x, j, i;
    for (i = 1; i < n; i++) {
        x = v[i];
        j = i-1;
        while ((j >= 0) && (v[j] > x)) {
            v[j+1] = v[j];
            j = j-1;
        }
        v[j+1] = x;
    }
}

void test_ord_insercion() {
    int n = 10; int v[n];
    printf("-----TEST ORDENACION POR INSERCION-----\n");
    printf("Ordenacion por insercion con inicializacion aleatoria:\n");
    generar_aleatorio(v,10); 
    mostar_vector(v,n);
    printf("ordenando...\n"); ord_ins(v,n);
    mostar_vector(v,n);
    printf("Ordenacion por insercion con inicializacion ascendente:\n");
    generar_ascendente(v,n);
    mostar_vector(v,n);
    printf("ordenando...\n"); ord_ins(v,n);
    mostar_vector(v,n);
    printf("Ordenacion por insercion con inicializacion descendente:\n");
    generar_descendente(v,n);
    mostar_vector(v,n);
    printf("ordenando...\n"); ord_ins(v,n);
    mostar_vector(v,n);
}

int random_num(int n, int m) { //esta funcion nos devuelve un numero aleatorio entre dos numeros introducidos
    int num;
    num = rand() % (m-n+1) + n;
    return num;
}

void intercambiar(int v[], int a, int b) {
    int aux;
    aux = v[a];
    v[a] = v[b];
    v[b] = aux;
}

void ordenarAux(int v[], int izq, int der) {
    int pivote, i, j, x;
    if ((izq+UMBRAL)<=der) {
        x = random_num(izq,der);
        pivote = v[x];
        intercambiar(v,izq,x);
        i = izq + 1;
        j = der;
        while (i <= j) {
            while ((i<=der)&&(v[i]<pivote)) {
                i = i+1;
            }
            while (v[j] > pivote) {
                j = j-1;
            }
            if (i <= j) {
                intercambiar(v,i,j);
                i = i+1;
                j = j-1;
            }
        }
        intercambiar(v,izq,j);
        ordenarAux(v,izq,j-1);
        ordenarAux(v,j+1,der);
    }
}

void ordenacionRapida(int v[],int n) {
    ordenarAux(v,0,n-1);
    if (UMBRAL>1) {
        ord_ins(v,n);
    }        
}

void test_ord_rapida() {
    int n = 10; int v[n];
    printf("-----TEST ORDENACION RAPIDA-----\n");
    printf("Ordenacion por insercion con inicializacion aleatoria:\n");
    generar_aleatorio(v,10); 
    mostar_vector(v,n);
    printf("ordenando...\n"); ordenacionRapida(v,n);
    mostar_vector(v,n);
    printf("Ordenacion por insercion con inicializacion ascendente:\n");
    generar_ascendente(v,n);
    mostar_vector(v,n);
    printf("ordenando...\n"); ordenacionRapida(v,n);
    mostar_vector(v,n);
    printf("Ordenacion por insercion con inicializacion descendente:\n");
    generar_descendente(v,n);
    mostar_vector(v,n);
    printf("ordenando...\n"); ordenacionRapida(v,n);
    mostar_vector(v,n);
}

void calc_tiempos(void (*tipovector)(int[],int),void (*algoritmo)(int[],int),
                  float cota_sub, float cota_ajust, float cota_sobre) {
    double t,t500,ta,tb,x,y,z; int v[64000], n, i;
    printf("\t n\t  t(n)\t     t(n)/f(n)\t   t(n)/g(n)\tt(n)/h(n)\n");
    for (n = 500; n <= 32000; n=n*2) {
        tipovector(v,n);
        ta = microsegundos();
        algoritmo(v,n);
        tb = microsegundos();
        t = tb - ta;
        if (t < 500) {
            ta = microsegundos();
            for(i = 0; i < 1000; i++) {
                tipovector(v,n);
                algoritmo(v,n);
            }
            tb = microsegundos();
            t = tb - ta;
            ta = microsegundos();
            for(i = 0; i < 1000; i++) {
                tipovector(v,n);
            }
            tb = microsegundos();
            t500 = tb - ta;
            t = (t - t500) / 1000;
            x = t / pow(n,cota_sub);
            y = t / pow(n,cota_ajust);
            z = t / pow(n,cota_sobre);
            printf("(*)%7d%12.3f%15.5f%14.5f%14.7f\n",n,t,x,y,z);
        } else {
            x = t / pow(n,cota_sub);
            y = t / pow(n,cota_ajust);
            z = t / pow(n,cota_sobre);
            printf("%10d%12.3f%15.5f%14.5f%14.7f\n",n,t,x,y,z);
        }
    }
}

void mostrar_tablas() {
    printf("\n----- TIEMPOS ORD POR INSERCION VECTOR ASCENDENTE -----\n\n");
    calc_tiempos(generar_ascendente,ord_ins,0.8,1.0,1.2);
    printf("\n----- TIEMPOS ORD POR INSERCION VECTOR DESCENDENTE -----\n\n");
    calc_tiempos(generar_descendente,ord_ins,1.7,2.0,2.4);
    printf("\n----- TIEMPOS ORD POR INSERCION VECTOR ALEATORIO -----\n\n");
    calc_tiempos(generar_aleatorio,ord_ins,1.75,2.0,2.4);
    printf("\n----- TIEMPOS ORD RAPIDA VECTOR ASCENDENTE -----\n\n");
    calc_tiempos(generar_ascendente,ordenacionRapida,0.9,1.1,1.35);
    printf("\n----- TIEMPOS ORD RAPIDA VECTOR DESCENDENTE -----\n\n");
    calc_tiempos(generar_descendente,ordenacionRapida,0.9,1.1,1.35);
    printf("\n----- TIEMPOS ORD RAPIDA VECTOR ALEATORIO -----\n\n");
    calc_tiempos(generar_aleatorio,ordenacionRapida,0.88,1.08,1.35);
}
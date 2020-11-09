#include "intersectii.h"
#define MAX 20

Interval* construct_interval(int st, int dr) {
    Interval *interval = malloc(sizeof(Interval));
    if (interval == NULL) {
        exit(1);
    }
    interval->capat_stanga = st;
    interval->capat_dreapta = dr;
    return interval;
}

// functie ajutatoare
void afiseaza_lista_intervale(ListaIntervale2D *lista) {
    int i;
    for (i = 0; i < lista->dimensiune; i++) {
        Interval2D *interv = lista->intervale[i];
        printf("{punct stanga x: %d, punct stanga y: %d, punct dreapta x: %d, punct dreapta y: %d}\n",
                    interv->punct_stanga->x, interv->punct_stanga->y,
                    interv->punct_dreapta->x, interv->punct_dreapta->y);
    }
}


ListaIntervale2D* citeste_intrare(char *nume_fisier_intrare) {
    FILE* input = fopen(nume_fisier_intrare, "r");
    ListaIntervale2D* lista = (ListaIntervale2D*) malloc(sizeof(ListaIntervale2D));
    if (!lista) return NULL;

    fscanf(input, "%d", &lista->dimensiune);

    lista->intervale = (Interval2D**)malloc(lista->dimensiune * sizeof(Interval2D*));

    for (int i = 0; i <= lista->dimensiune; i++) {
        lista->intervale[i] = (Interval2D*)malloc(sizeof(Interval2D));
        lista->intervale[i]->punct_stanga = (Punct2D*)malloc(sizeof(Punct2D));
        lista->intervale[i]->punct_dreapta = (Punct2D*)malloc(sizeof(Punct2D));

        fscanf(input, "%d %d %d %d", &lista->intervale[i]->punct_stanga->x, &lista->intervale[i]->punct_stanga->y,
               &lista->intervale[i]->punct_dreapta->x, &lista->intervale[i]->punct_dreapta->y);
    }

    return lista;
}

// ! Functie pentru actualizarea in arbore
// ! O veti da ca parametru cand initializati arborele
void actualizare_cu_delta(Nod *nod, int v2) {
    nod->info += v2;
}

// ! Functie pentru combinarea raspunsurilor in arbore
// ! O veti da ca parametru cand initializati arborele
int suma_raspunsurilor(int r1, int r2) {
    return r1 + r2;
}

void swap(Interval2D** a, Interval2D** b) {
    Interval2D* temp = *a;
    *a = *b;
    *b = temp;
}

int calculeaza_numar_intersectii(ListaIntervale2D *lista) {
    int nr_interesectii = 0;

    // creez arborele de intervale
    int y_max = -1;
    for (int i = 0; i < lista->dimensiune; i++) {
        if (lista->intervale[i]->punct_dreapta->y > y_max) {
            y_max = lista->intervale[i]->punct_dreapta->y;
        }
    }
    ArboreDeIntervale* arbore = construieste_arbore(0, y_max, 0, actualizare_cu_delta, suma_raspunsurilor);

    // sortez lista dupa x al capatului din stanga
    for (int i = 0; i < lista->dimensiune - 1; i++) {
        for (int j = 0; j < lista->dimensiune - i - 1; j++) {
            if (lista->intervale[j]->punct_stanga->x > lista->intervale[j+1]->punct_stanga->x) {
                swap(&lista->intervale[j], &lista->intervale[j+1]);
            }
        }
    }

    int x = 0; // coordonata x a dreptei verticale imaginare
    for (int i = 0; i < lista->dimensiune; i++) {
        while (x < lista->intervale[i]->punct_stanga->x) {
            // verificare segment orizontal, capat dreapta
            for (int j = 0; j < i; j++) {
                if (lista->intervale[i]->punct_stanga->y == lista->intervale[i]->punct_dreapta->y &&
                    lista->intervale[j]->punct_dreapta->x == x) {
                    Interval* interval = construct_interval(lista->intervale[j]->punct_stanga->y, 
                                                    lista->intervale[j]->punct_dreapta->y);
                    actualizare_interval_in_arbore(arbore, interval, -1);
                }
            }
            x++;
        }

        Interval* interval = construct_interval(lista->intervale[i]->punct_stanga->y, 
                                                lista->intervale[i]->punct_dreapta->y);

        // verificare segment orizontal, capat stanga
        if (lista->intervale[i]->punct_stanga->y == lista->intervale[i]->punct_dreapta->y) {
            actualizare_interval_in_arbore(arbore, interval, 1);
        }   
        // altfel, este segment vertical si calculam intersectiile
        else { 
            nr_interesectii += interogare_interval_in_arbore(arbore, interval);
        }
    }
    return nr_interesectii;
}


int calculeaza_numar_intersectii_trivial(ListaIntervale2D *lista) {
    int v_oriz[MAX]; // vector caracteristic pentru segmentele orizontale
    int v_vert[MAX]; // vector caracteristic pentru segmentele verticale
    int nr_oriz = 0, nr_vert = 0, nr_intersectii = 0;

    for (int i = 0; i < lista->dimensiune; i++) {
        if (lista->intervale[i]->punct_stanga->y == lista->intervale[i]->punct_dreapta->y) {
            v_oriz[nr_oriz] = i;
            nr_oriz++;
        } else {
            v_vert[nr_vert] = i;
            nr_vert++;
        }
    }

    for (int i = 0; i < nr_oriz; i++) {
        for (int j = 0; j < nr_vert; j++) {
            if (lista->intervale[v_vert[j]]->punct_stanga->y <= lista->intervale[v_oriz[i]]->punct_stanga->y &&
                lista->intervale[v_vert[j]]->punct_dreapta->y >= lista->intervale[v_oriz[i]]->punct_stanga->y &&
                lista->intervale[v_vert[j]]->punct_stanga->x <= lista->intervale[v_oriz[i]]->punct_dreapta->x &&
                lista->intervale[v_vert[j]]->punct_stanga->x >= lista->intervale[v_oriz[i]]->punct_stanga->x) {
                    nr_intersectii++;
            }
        }
    }
    
    return nr_intersectii;
}

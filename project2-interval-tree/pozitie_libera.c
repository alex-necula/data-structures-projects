#include "pozitie_libera.h"


DateIntrare* citeste_fisier_in(char *nume_fisier_intrare) {
    FILE* input = fopen(nume_fisier_intrare, "r");
    DateIntrare* lista = (DateIntrare*) malloc(sizeof(DateIntrare));
    if (!lista) return NULL;

    fscanf(input, "%d %d", &lista->numarul_maxim_din_intrari, &lista->numar_intrari);

    lista->intrari = (int*)malloc(lista->numar_intrari * sizeof(int));

    for (int i = 0; i < lista->numar_intrari; i++) {
        fscanf(input, "%d", &lista->intrari[i]);
    }

    return lista;
}

void actualizare(Nod* nod, int valoare) {
    if (nod->copil_dreapta && nod->copil_dreapta->info != -1) {
        nod->info = nod->copil_dreapta->info;
    } else if (nod->copil_stanga && nod->copil_stanga->info != -1) {
        nod->info = nod->copil_stanga->info;
    } else {
        nod->info = -1;
    }
}

int maxim(int r1, int r2) {
    return (r1 > r2) ? r1 : r2;
}

Interval* construct_interval(int st, int dr) {
    Interval *interval = malloc(sizeof(Interval));
    if (interval == NULL) {
        exit(1);
    }
    interval->capat_stanga = st;
    interval->capat_dreapta = dr;
    return interval;
}

int* calculeaza_vector_raspuns(DateIntrare *date_intrare) {
    ArboreDeIntervale* arbore = construieste_arbore(0, date_intrare->numarul_maxim_din_intrari, -1, 
                                                    actualizare, maxim);
    seteaza_info_in_arbore_la_valoare_capat_dreapta(arbore);

    int* v = (int*) malloc((date_intrare->numarul_maxim_din_intrari + 1) * sizeof(int));

    for (int i = 0; i < date_intrare->numar_intrari; i++) {
        int index = date_intrare->intrari[i];

        Interval* interval = construct_interval(0, index);
        index = interogare_interval_in_arbore(arbore, interval);

        if (index != -1) {
            v[index] = date_intrare->intrari[i];
            interval->capat_stanga = index;
            interval->capat_dreapta = index;
            actualizare_interval_in_arbore(arbore, interval, -1);
        }
    }

    return v;
}


int* calculeaza_vector_raspuns_trivial(DateIntrare *date_intrare) {
    int* v = (int*) malloc((date_intrare->numarul_maxim_din_intrari + 1) * sizeof(int));

    for (int i = 0; i <= date_intrare->numarul_maxim_din_intrari; i++) {
        v[i] = -1; // initializare pentru a verifica daca exista element introdus
    }

    for (int i = 0; i < date_intrare->numar_intrari; i++) {
        int index = date_intrare->intrari[i];

        while (index >= 0 && v[index] != -1) {
            index--;
        }

        if (index != -1) {
            v[index] = date_intrare->intrari[i];
        }
    }
    
    return v;
}

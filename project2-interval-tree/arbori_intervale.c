#include "arbori_intervale.h"

Nod* creare_nod(int st, int dr) {
    Nod* nod = (Nod*)malloc(sizeof(Nod));
    if (!nod) return NULL;

    nod->interval = (Interval*)malloc(sizeof(Interval));
    if (!nod->interval) {
        free(nod);
        return NULL;
    }

    nod->interval->capat_dreapta = dr;
    nod->interval->capat_stanga = st;
    nod->info = 0;

    if (st < dr) {
        int mijloc = (st + dr) / 2;
        nod->copil_stanga = creare_nod(st, mijloc);
        nod->copil_dreapta = creare_nod(mijloc + 1, dr);
    }

    return nod;
}

ArboreDeIntervale *construieste_arbore(int capat_stanga, int capat_dreapta,
        int valoare_predifinita_raspuns_copil,
        void (*f_actualizare)(Nod *nod, int v_actualizare),
        int (*f_combinare_raspunsuri_copii)(int raspuns_stanga, int raspuns_dreapta)) {

    ArboreDeIntervale* arbore = (ArboreDeIntervale*)malloc(sizeof(ArboreDeIntervale));
    if (!arbore) return NULL;

    arbore->radacina = creare_nod(capat_stanga, capat_dreapta);
    arbore->dimensiune = (capat_dreapta - capat_stanga) * 2 + 1;
    arbore->valoare_predifinita_raspuns_copil = valoare_predifinita_raspuns_copil;
    arbore->f_actualizare = f_actualizare;
    arbore->f_combinare_raspunsuri_copii = f_combinare_raspunsuri_copii;

    return arbore;
}


void actualizare_interval_pentru_nod(ArboreDeIntervale *arbore, Nod *nod,
                                    Interval *interval, int v_actualizare) {
    if (interval->capat_stanga <= nod->interval->capat_stanga && 
        nod->interval->capat_dreapta <= interval->capat_dreapta) {
        arbore->f_actualizare(nod, v_actualizare);
    } else {
        int mijloc = (nod->interval->capat_stanga + nod->interval->capat_dreapta) / 2;

        if (interval->capat_stanga <= mijloc) {
            actualizare_interval_pentru_nod(arbore, nod->copil_stanga, interval, v_actualizare);
        }
        if (mijloc < interval->capat_dreapta) {
            actualizare_interval_pentru_nod(arbore, nod->copil_dreapta, interval, v_actualizare);
        }

        arbore->f_actualizare(nod, v_actualizare);
    }

}


// Functia este deja implementata, se cheama functia de mai sus cu radacina arborelui
void actualizare_interval_in_arbore(ArboreDeIntervale *arbore, 
                                        Interval *interval, int v_actualizare) {
    actualizare_interval_pentru_nod(arbore, arbore->radacina, interval, v_actualizare);
}

int interogare_interval_pentru_nod(ArboreDeIntervale *arbore, Nod *nod, Interval *interval) {
    int leftVal, rightVal;
    leftVal = rightVal = arbore->valoare_predifinita_raspuns_copil;

    if (interval->capat_stanga <= nod->interval->capat_stanga && 
        nod->interval->capat_dreapta <= interval->capat_dreapta) {
        return nod->info;
    } else {
        int mijloc = (nod->interval->capat_stanga + nod->interval->capat_dreapta) / 2;

        if (interval->capat_stanga <= mijloc) {
            leftVal = interogare_interval_pentru_nod(arbore, nod->copil_stanga, interval);
        }
        if (mijloc < interval->capat_dreapta) {
            rightVal = interogare_interval_pentru_nod(arbore, nod->copil_dreapta, interval);
        }
        
        return arbore->f_combinare_raspunsuri_copii(leftVal, rightVal);
    }
}


// Functia este deja implementata, se cheama functia de mai sus cu radacina arborelui
int interogare_interval_in_arbore(ArboreDeIntervale *arbore, Interval *interval) {
    return interogare_interval_pentru_nod(arbore, arbore->radacina, interval);
}


// ----- DOAR pentru bonus si DOAR daca considerati ca e necesara ----- //
void seteaza_info_in_nod_la_valoare_capat_dreapta(Nod* nod) {
    nod->info = nod->interval->capat_dreapta;
    if (nod->copil_stanga) seteaza_info_in_nod_la_valoare_capat_dreapta(nod->copil_stanga);
    if (nod->copil_dreapta) seteaza_info_in_nod_la_valoare_capat_dreapta(nod->copil_dreapta);
}


void seteaza_info_in_arbore_la_valoare_capat_dreapta(ArboreDeIntervale* arbore) {
    seteaza_info_in_nod_la_valoare_capat_dreapta(arbore->radacina);
}

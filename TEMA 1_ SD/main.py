from bubble_sort import bubbleSort
from count_sort import countSort
from merge_sort import mergeSort
from quick_sort import quickSort
from selection_sort import selectionSort
from radix_sort import radixSort
from default_sort import defaultSort

import time


def verificare(v):                  # functia verificare controleaza ca vectorul sa fie intradevar sortat
    for i in range(len(v)-1):
        if v[i] > v[i+1]:
            return False
    return True


def cmp(t):
    return t[1]         # de functia comparator cmp ma folosesc pentru a sorta tupluri dupa timpul de executie


sortari = (bubbleSort, countSort, mergeSort, quickSort, selectionSort, radixSort, defaultSort)      #voi parcurge aceasta lista de sortari

print('\n')         # acest '/n' are doar un rol estetic

for nr in range(1, 10):
    f = open("test" + str(nr) + ".txt")     # citesc pe rand toate datele din fiecare test

    descriere = f.readline()            # la inceputul fiecarui test se afla un rand in care sunt descrise numerele testului respectiv
    print(descriere)

    v = f.readline()
    v = [int(i) for i in v.split()]     # in v voi introduce toate valorile citite din fisier pentru fiecare test

    timpi_executie = []

    for s in sortari:
        A = v.copy()        # sortarile sunt facute in asa fel incat ele doar modifica vectorul dat ca parametru,
                            # asa ca acesta trebuie copiat inaintea oricarei sortari
        okay = True         # de okay ma folosesc pentru a controla daca sortarea are rost sa fie facuta sau nu
        tic = False         # daca nu fac sortarea, aceasta nu va avea un timp la care incepe


        # CELE 4 IF-URI CARE URMEAZA, TRATEAZA CAZURI PARTICULARE

        # LA NUMERE PREA MARI, COUNT SORT ESTE INEFICIENT
        if max(A) > 1000000 and sortari.index(s) == 1:
            print("numerele de sortat sunt prea mari, countSort-ul este ineficient")
            okay = False

        # LA PREA MULTE NUMERE, BUBBLE SORT ESTE INEFICIENT
        if len(A) > 10000 and sortari.index(s) == 0:
            print("numerele de sortat sunt prea multe, bubbleSort-ul este ineficient")
            okay = False

        # LA PREA MULTE NUMERE, SELECTION SORT ESTE INEFICIENT
        if len(A) > 10000 and sortari.index(s) == 4:
            print("numerele de sortat sunt prea multe, selectionSort-ul este ineficient")
            okay = False

        # ATUNCI CAND AM PREA MULTE NUMERE SAU NUMERE PREA MARI, RADIX SORT-UL IL FAC IN BAZA MAI MARE
        if (len(A) > 10000 or max(A) > 1000000) and sortari.index(s) == 5:
            okay = False
            tic = time.perf_counter()
            s(A, 2048)
            toc = time.perf_counter()
            print(f"numere multe sau prea mari, radixSort in baza 2048 efectuata in {toc - tic:0.9f} secunde")

        if okay:
            tic = time.perf_counter()       # in tic retin clipa inaintea inceputului sortarii
            s(A)                            # s va lua pe rand fiecare sortare din lista _sortari_
            toc = time.perf_counter()       # in toc retin clipa in care se termina sortarea
            if verificare(A):
                print(str(s.__name__) + f" efectuata in {toc - tic:0.9f} secunde")
            else:
                print("sortare incorecta")


        timpi_executie.append((s.__name__, toc - tic if tic else 100000))
        # in timpi_executie introduc tupluri astfel: pe prima pozitie a tuplului se afla numele sortarii,
        # pe a doua pozitie a tuplului inserez diferenta de timp pe care o am de la inceputul la finalul
        # sortarii, sau inserez 100000 daca nu se efectueaza sortarea

    timpi_executie.sort(key=cmp)        # sortez timpii de executie dupa timp

    print("\nPentru setul de date de intrare dat, sortarile in ordinea eficientei sunt:")
    for i in range(len(timpi_executie)):
        print(str(i + 1) + " " + timpi_executie[i][0])      # afisez sortarile dupa timpul de executie

    for i in range(0, 123):
        print("_", end='_')        # afisez o linie pentru a se distinge mai bine seturile de intrare intre ele
    print('\n')

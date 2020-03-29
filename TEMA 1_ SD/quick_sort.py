def pivotMediana(l):

    if len(l) <= 5:
        return sorted(l)[len(l) // 2]

    subliste = [sorted(l[i: i + 5]) for i in range(0, len(l), 5)]
    mediane = [sl[len(sl) // 2] for sl in subliste]

    return pivotMediana(mediane)


def quickSort(v):               #Am observat un lucru interesant la quicksort:
                                #este mai rapida sortarea daca folosesc ca pivot ultima valoare din vector
    if len(v) < 2:              #cu toate ca in primul semestru am fosti invatati sa folosim mediana medianelor pentru a gasi pivotul
        return v                #si atunci ni s-a spus ca este cea mai eficienta metoda

    pivot = pivotMediana(v)
    L = []
    E = []
    G = []

    for a in v:
        if a < pivot:
            L.append(a)
        elif a == pivot:
            E.append(a)
        else:
            G.append(a)

    quickSort(L)
    quickSort(G)

    v.clear()

    v.extend(L)
    v.extend(E)
    v.extend(G)


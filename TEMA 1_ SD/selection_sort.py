def selectionSort(v):
    for i in range(len(v)-1):
        k = min(v[i+1:])
        x = v.index(k, i+1)

        if v[i] > v[x]:
            v[i], v[x] = v[x], v[i]         # I <3 python pentru simplitatea interschimbarii

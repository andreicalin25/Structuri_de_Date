def radixSort(v, baza=10):          #radix sortul se face implicit in baza 10, chiar daca in unele cazuri, apelez functia
    maxi = max(v)                   #din main astfel incat sortarea sa se faca in baza 2048
    zecimala_crt = 1

    while maxi:
        maxi /= baza
        sub = [[] for i in range(baza)]

        for element in v:
            zecimala = element // zecimala_crt % baza
            sub[zecimala].append(element)

        v.clear()
        zecimala_crt = zecimala_crt * baza

        for aceeasi_zecimala in sub:
            v.extend(aceeasi_zecimala)

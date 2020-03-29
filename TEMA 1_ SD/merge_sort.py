def mergeSort(v):
    if len(v) > 1:
        mij = len(v)//2
        lstanga = v[:mij]
        ldreapta = v[mij:]

        mergeSort(lstanga)
        mergeSort(ldreapta)

        i = j = k = 0

        while i < len(lstanga) and j < len(ldreapta):

            if lstanga[i] < ldreapta[j]:
                v[k] = lstanga[i]
                i += 1
                k += 1
            else:
                v[k] = ldreapta[j]
                j += 1
                k += 1

        while i < len(lstanga):
            v[k] = lstanga[i]
            i += 1
            k += 1

        while j < len(ldreapta):
            v[k] = ldreapta[j]
            j += 1
            k += 1

def countSort(v):

    maxim = max(v)
    aparitii = [0 for i in range (0, maxim+1)]

    for i in v:
        aparitii[i] += 1

    j = 0
    for i in range(0, maxim+1):
        while aparitii[i]:
            v[j] = i
            j += 1
            aparitii[i] -= 1

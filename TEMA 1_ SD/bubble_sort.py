def bubbleSort(v):
    n = len(v)

    for i in range(n):
        sortat = True

        for j in range(0, n-i-1):

            if v[j] > v[j+1]:

                v[j], v[j+1] = v[j+1], v[j]
                sortat = False

        if sortat:
            break

from enum import Enum


class Operation(Enum):
    """Operations"""

    DELETED = 1
    INSERTED = 2
    SUBSTITUTED = 3

    def __str__(self):
        return str(self.name.lower())


def distances(a, b):

    m = len(a)
    n = len(b)

    #data structures
    matrix1 = [[(0, None) for x in range(0,n+1)] for y in range(0,m+1)]
    insCost = [[0 for x in range(0,n+1)] for y in range(0,m+1)]
    delCost = [[0 for x in range(0,n+1)] for y in range(0,m+1)]
    subCost = [[0 for x in range(0,n+1)] for y in range(0,m+1)]

    # fill outer edges
    for i in range(1, m+1):
        matrix1[i][0] = (i, Operation.DELETED)
    for j in range(1, n+1):
        matrix1[0][j] = (j, Operation.INSERTED)

    for i in range(1, m+1):
        for j in range(1, n+1):
            delCost[i][j] = matrix1[i - 1][j][0] + 1
            insCost[i][j] = matrix1[i][j - 1][0] + 1

            if a[i - 1] == b[j - 1]:
                subCost[i][j] = matrix1[i - 1][j - 1][0]
            else:
                subCost[i][j] = matrix1[i - 1][j - 1][0] + 1

            if delCost[i][j] <= subCost[i][j] and delCost[i][j] <= insCost[i][j]:
                matrix1[i][j] = (delCost[i][j], Operation.DELETED)
            elif insCost[i][j] <= delCost[i][j] and insCost[i][j] <= subCost[i][j]:
                matrix1[i][j] = (insCost[i][j], Operation.INSERTED)
            else:
                matrix1[i][j] = (subCost[i][j], Operation.SUBSTITUTED)

    return matrix1
def id_generator(n):
    while True:
        yield n
        n += 1


class Piece:

    pid = id_generator(1)

    def __init__(self, up, right, down, left):
        self.id = next(self.__class__.pid)
        self.up = up
        self.right = right
        self.down = down
        self.left = left
        self.empty = True
        self.grass = False

    def __repr__(self):
        return 'P(id:%d%s)' % (self.id, ' empty' if self.empty else ' grass' if self.grass else ' %d,%d,%d,%d' % (
            self.up, self.right, self.down, self.left))



def read_file(input_file):
    with open(input_file, 'r') as file:
        lineas = [i.strip() for i in file.readlines()]
        rows, cols, grass = [int(i) for i in lineas[:3]]
        grasses = [[int(j) for j in i.split()] for i in lineas[3:grass + 3]]
        k = lineas[grass + 3]
        pieces = [[int(j) for j in i.split()] for i in lineas[grass + 4:]]
    return rows, cols, grass, k, grasses, pieces


def place_candidate(grid, piece, i, j):
    # print(grid[i][j], piece, i, j)
    if not piece[4]:
        return False
    if i + 1 < len(grid):
        # print(grid[i + 1][j].up)
        if not grid[i + 1][j].up == piece[2] and not grid[i + 1][j].empty:
            return False
    if i - 1 > -1:
        # print(grid[i - 1][j].down)
        if not grid[i - 1][j].down == piece[0] and not grid[i - 1][j].empty:
            return False
    if j + 1 < len(grid[0]):
        # print(grid[i][j + 1].left)
        if not grid[i][j + 1].left == piece[1] and not grid[i][j + 1].empty:
            return False
    if j - 1 > -1:
        # print(grid[i][j - 1].right)
        if not grid[i][j - 1].right == piece[3] and not grid[i][j - 1].empty:
            return False
    grid[i][j].up, grid[i][j].right, grid[i][j].down, grid[i][j].left, = piece[:4]
    grid[i][j].empty = False
    return True


def unplace(grid, i, j):
    grid[i][j].up, grid[i][j].right, grid[i][j].down, grid[i][j].left, = [0] * 4
    grid[i][j].empty = True


def rotations(piece):
    return 4


def rotate(piece):
    piece[0], piece[1], piece[2], piece[3] = piece[3], piece[0], piece[1], piece[2]
    # return [piece[-2]] + piece[:-2] + [piece[-1]]


def es_sol(grid, pieces):
    for fila in grid:
        for elemento in fila:
            if elemento.empty:
                return False
    return True


def empty_slots(grid):
    empty = []
    for i in range(len(grid)):
        for j in range(len(grid[0])):
            if grid[i][j].empty:
                empty.append([i, j])
    return empty  # [[i, j] for j in range(len(grid[i])) for i in range(len(grid)) if grid[i][j].empty]


def backtracking(grid, pieces):
    if es_sol(grid, pieces):
        return True
    # pieces.sort(key=lambda x: x[4])
    for i in range(len(grid)):
        for j in range(len(grid[0])):
            piece = grid[i][j]
            if piece.empty:
                for candidate in pieces:
                    for k in range(rotations(candidate)):
                        if place_candidate(grid, candidate, i, j):
                            candidate[4] -= 1
                            if backtracking(grid, pieces):
                                return True
                            candidate[4] += 1
                            unplace(grid, i, j)
                        candidate = rotate(candidate)
    return False

import time
def backtracking2(grid, empty, pieces):
    time.sleep(0.1)
    print(grid)
    if es_sol(grid, pieces):
        return True
    for i, j in empty:
        for k in range(len(pieces)):
            candidate = pieces[k]
            if candidate[4] < 1:
                continue
            for l in range(rotations(candidate)):
                if place_candidate(grid, candidate, i, j):
                    candidate[4] -= 1
                    new_empty = [x for x in empty if x != [i, j]]
                    if backtracking2(grid, new_empty, pieces):
                        return True
                    candidate[4] += 1
                    unplace(grid, i, j)
                rotate(candidate)
    return False




if __name__ == '__main__':
    level = 'easy'
    route = '../../tests/%s/%s4.txt' % (level, level)
    rows, cols, grass, k, grasses, pieces = read_file(route)
    grid = [[Piece(1, 1, 1, 1) for j in range(cols)] for i in range(rows)]
    for i, j in grasses:
        grid[i][j].grass = True
        grid[i][j].empty = False
    # print(rotate(rotate([1,2,3,4,5])))
    empty = empty_slots(grid)
    print(empty)
    backtracking2(grid, empty, pieces)

    for fila in grid:
        print(' '.join((str(i).ljust(15) for i in fila)))














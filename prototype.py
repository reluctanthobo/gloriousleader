import random
import sys

class Tile(object):
    def __init__(self, x, y):
        self.x = x
        self.y = y
        self.monster = None
        self.level = 1
    def add_monster(self, monster):
        self.monster = monster
    def remove_monster(self, monster):
        self.monster = None

    def __str__(self):
        if self.monster:
            return str(self.monster) 
        if self.level == 1:
            return "--"
        elif self.level == 2:
            return "+="
        elif self.level == 3:
            return "++"

class Board(object):
    def __init__(self, w, h):
        self.w = w
        self.h = h
        self.tiles = []

        for i in range(w):
            self.tiles.append(Tile(w-1-i,0))
        for i in range(h-1):
            self.tiles.append(Tile(0, i+1))
        for i in range(w-1):
            self.tiles.append(Tile(i+1, h-1))
        for i in range(h-2):
            self.tiles.append(Tile(w-1, h-i-2))

    def first_tile(self):
        return self.tiles[0]

    def random_tile(self):
        return random.choice(self.tiles)

    def advance(self, tile, count):
        return self.tiles[(self.tiles.index(tile) + count) % len(self.tiles)]

    def __str__(self):
        grid = []
        for i in range(self.h):
            grid.append([])
            for j in range(self.w):
                grid[i].append(None)
        for tile in self.tiles:
            grid[tile.y][tile.x] = tile
        res = ""
        for i in range(self.h):
            res += "\n"
            y = self.h - i - 1
            for x in range(self.w):
                if grid[y][x] is not None:
                    res += " " + str(grid[y][x]) + " "
                else:
                    res += "    "
            res += "\n"
        return res

class Monster(object):
    def __init__(self, name, tile):
        self.tile = tile
        self.name = name
        tile.add_monster(self)
    def set_tile(self, tile):
        self.tile.remove_monster(self)
        self.tile = tile
        self.tile.add_monster(self)

    def __str__(self):
        return self.name

if __name__ == "__main__":
    b = Board(10,10)

    max_advance = 6

    p = Monster("PP", b.first_tile())
    a = Monster("AA", b.advance(p.tile, -4))

    turn = 1

    while True:
        print "Turn: %d" % turn
        print str(b)
        new_turn = False

        sys.stdout.write("> ")
        cmd = sys.stdin.readline()
        cmd = cmd.lower().split()
        if len(cmd) == 0:
            continue
        if cmd[0] == "advance":
            arg = max(1,min(int(cmd[1]), max_advance))
            p.set_tile(b.advance(p.tile, arg))
            new_turn = True

        if new_turn:
            a.set_tile(b.advance(a.tile, random.randint(1,4)))
            if a.tile == p.tile:
                print "The alien got you"
                break
            if p.tile.level < 3:
                p.tile.level += 1
            if a.tile.level > 1:
                a.tile.level -= 1
                
            turn += 1

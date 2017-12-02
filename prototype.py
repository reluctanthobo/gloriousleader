import random
import sys

colors = ["red", "red", "red", "red", "red", "red", "red", "grn", "grn", "blu"]

def random_tile():
    if random.uniform(0,1) < 0.9:
        color = random.choice(colors)
        quality = random.randint(1,5)
        return Tile(color, quality)
    return filler_tile()

def filler_tile():
    return Tile("prk", 0)

def random_survivor(tile, board, index):
    return Survivor(random.choice(colors), tile, board, index)

def random_monster(tile, board):
    return Monster(board.tiles[board.get_index(tile) - 20], board)

class Tile(object):
    def __init__(self, color, quality):
        self.color = color
        self.quality = quality
        self.counters = []
        self.hotels = []

    def add_counter(self, counter):
        self.counters.append(counter)
    def use_counter(self, counter):
        self.counters.remove(counter)
        self.hotels.append(counter)
    def remove_counter(self, counter):
        self.counters.remove(counter)
    def remove_hotel(self, counter):
        self.hotels.remove(counter)

    def __str__(self):
        hstring = ""
        if len(self.hotels) > 0:
            hstring = " [%s]" % ", ".join(["%s" % h for h in self.hotels])
        qstring = " %d" % self.quality if self.quality > 0 else "  "
        return "%s%s: %s%s" % (
            self.color, 
            qstring,
            ", ".join(["%s" % c for c in self.counters]),
            hstring
        )

class Board(object):
    def __init__(self):
        self.tiles = []
    def add_tile(self, tile):
        self.tiles.append(tile)
    def advance(self, tile):
        index = self.tiles.index(tile)
        res = self.tiles[min(index+1, len(self.tiles)-1)]
        return res
    def reverse(self, tile):
        index = self.tiles.index(tile)
        res = self.tiles[max(index-1, 0)]
        return res
    def get_index(self, tile):
        return self.tiles.index(tile)
    def tile_window(self, tile, w=5):
        index = self.tiles.index(tile)
        n = len(self.tiles)
        return [(i,self.tiles[i]) for i in range(max(index-w/2,0), min(index+w/2+1,n))]
    def first_tile(self):
        return self.tiles[0]

class GameBit(object):
    def __init__(self, color, tile, board):
        self.tile = tile
        self.color = color
        self.board = board
        self.frozen = False
        tile.add_counter(self)
    def advance(self, n=1):
        for i in range(n):
            self.set_tile(self.board.advance(self.tile))
    def reverse(self, n=1):
        for i in range(n):
            self.set_tile(self.board.reverse(self.tile))
    def set_tile(self, tile):
        self.tile.remove_counter(self)
        self.tile = tile
        self.tile.add_counter(self)

    def freeze(self):
        self.frozen = True
        self.tile.use_counter(self)
    def die(self):
        if self.frozen:
            self.tile.remove_hotel(self)
        else:
            self.tile.remove_counter(self)

    def __str__(self):
        return self.color

class Monster(GameBit):
    def __init__(self, tile, board):
        super(Monster,self).__init__("monster", tile, board)
        self.charge = 0

class Survivor(GameBit):
    def __init__(self, color, tile, board, index):
        super(Survivor,self).__init__(color, tile, board)
        self.index = index
    def __str__(self):
        return "%s" % (self.color)

if __name__ == "__main__":
    b = Board()
    for i in range(20):
        b.add_tile(filler_tile())

    for i in range(100):
        b.add_tile(random_tile())

    p = GameBit("player", b.tiles[20], b)
    monsters = [random_monster(p.tile, b)]
    survivors = []
    total_survivors = 0
    power = 0
    bullets = 2

    turn = 1

    while True:
        print "Turn: %d Bullets: %d Power: %d" % (turn,bullets,power)

        for index, tile in b.tile_window(p.tile,11):
            print "%d: %s" % (index, str(tile))

        new_turn = False

        sys.stdout.write("> ")
        cmd = sys.stdin.readline()
        cmd = cmd.lower().split()
        new_survivor = None
        if len(cmd) == 0:
            continue
        if cmd[0] == "a":
            p.advance()
            new_turn = True
        elif cmd[0] == "b":
            p.reverse()
            new_turn = True
        elif cmd[0] == "f":
            teleport = 0
            for s in survivors:
                if not s.frozen and s.tile == p.tile and s.color == s.tile.color:
                    if s.tile.color == "blu":
                        s.freeze()
                        teleport += s.tile.quality + 1
                        s.tile.quality = 0
                        print "The machine propels you forward"
                    elif s.tile.color == "grn":
                        s.freeze()
                        power += 3*(s.tile.quality + 1)
                        print "The power station springs to life"
                        s.tile.quality = 0
                    elif s.tile.color == "red":
                        s.freeze()
                        s.tile.quality += 1
                    new_turn = True
            if teleport > 0:
                last = 0
                start = b.get_index(p.tile)
                index = 1
                while teleport > 0 and start+index < len(b.tiles):
                    if b.tiles[start+index].color == "blu":
                        teleport -= 1
                        last = index
                        if teleport == 0:
                            b.tiles[start+index].quality = 0
                            break
                    index += 1
                if start+last < len(b.tiles):
                    p.advance(last)
                    for t in survivors:
                        if t.tile == p.tile and not t.frozen:
                            t.advance(last)

        elif cmd[0] == "w":
            new_turn = True

        if new_turn:
            if random.uniform(0,1) < max(25.0,min(total_survivors,50))/100.0:
                new_survivor = random_survivor(p.tile, p.board, total_survivors)
                total_survivors += 1

            for s in survivors:
                if not s.frozen and random.uniform(0,1) < 0.75:
                    s.advance()
            if new_survivor:
                survivors.append(new_survivor)

            if len(survivors) > len(monsters)*5 and random.uniform(0,1) < 0.5:
                new_monster = random_monster(p.tile, b)
                monsters.append(new_monster)
                print "A new alien joins the hunt"

            monster_speed = 1 if power > 0 else 2
            dead_monsters = []
            for m in monsters:
                monster_pos  = b.get_index(m.tile) + monster_speed
                lowest_eaten = []
                lowest_index = None
                player_pos = b.get_index(p.tile)
                for s in survivors:
                    index = b.get_index(s.tile)
                    if index <= player_pos and index <= monster_pos:
                        if lowest_index is None or index < lowest_index:
                            lowest_index = index
                            lowest_eaten = [s]
                        elif lowest_index is not None and index == lowest_index:
                            lowest_eaten.append(s)
                if lowest_index is not None:
                    monster_pos = lowest_index
                    for s in lowest_eaten:
                        if s.frozen and  s.tile.quality > 0:
                            s.tile.quality -= 1
                            print "You hear fighting in the distance"
                            break
                    else:
                        print "You hear screams in the distance"
                        for s in lowest_eaten:
                            survivors.remove(s)
                            s.die()
                else:
                    if b.get_index(p.tile) <= monster_pos:
                        if bullets == 0 :
                            print "An alien got you"
                            sys.exit(1)
                        else:
                            print "You shoot one of the aliens"
                            dead_monsters.append(m)
                            bullets -= 1
                m.set_tile(b.tiles[monster_pos])
            for m in dead_monsters:
                m.die()
                monsters.remove(m)

            if p.tile == b.tiles[-1]:
                print "You escaped"
                sys.exit(0)

            if power > 0:
                power -= 1
            turn += 1

import fileinput
import random

def weighted_choice(items): # [(key, count)]
    total = sum(x[1] for x in items)
    index = random.randint(0, total - 1)
    offset = 0
    for key, count in items:
        offset += count
        if index < offset:
            return key

class Node(object):
    def __init__(self):
        self.children = {}
        self.white = 0
        self.black = 0
        self.draw = 0
        self.total = 0
    def add_result(self, result):
        self.total += 1
        if result == '1-0':
            self.white += 1
        elif result == '0-1':
            self.black += 1
        elif result == '1/2-1/2':
            self.draw += 1
        else:
            raise ValueError(result)
    def do_move(self, move):
        if move not in self.children:
            self.children[move] = Node()
        return self.children[move]
    def output(self, depth=0):
        padding = '  ' * depth
        items = self.children.items()
        items.sort(key=lambda x: x[1].total, reverse=True)
        total = sum(x[1].total for x in items)
        for key, value in items:
            if value.total < 2:
                continue
            pct = 100.0 * value.total / total
            white = 100.0 * value.white / value.total
            draw = 100.0 * value.draw / value.total
            black = 100.0 * value.black / value.total
            print '%s%s W=%.1f%% D=%.1f%% B=%.1f%% [%.1f%% %d]' % (
                padding, key, white, draw, black, pct, value.total)
            value.output(depth + 1)
    def visit(self, wtm):
        items = self.children.items()
        items.sort(key=lambda x: x[1].total, reverse=True)
        total = sum(x[1].total for x in items)
        for key, value in items:
            pct = 100.0 * value.total / total
            white = 100.0 * value.white / value.total
            draw = 100.0 * value.draw / value.total
            black = 100.0 * value.black / value.total
            print '%8s [%3d%% %3d%% %3d%%] %3d%% %d' % (
                key, white, draw, black, pct, value.total)
        print
        if wtm:
            move = raw_input('WHITE > ')
        else:
            move = raw_input('BLACK > ')
        if move in self.children:
            self.children[move].visit(not wtm)
    def random(self):
        items = self.children.items()
        total = sum(x[1].total for x in items)
        if total < 10000:
            return
        pcts = []
        for key, value in items:
            pct = int(round(100.0 * value.total / total))
            if pct >= 20:
                pcts.append((key, pct))
        if not pcts:
            return
        move = weighted_choice(pcts)
        print move,
        if move in self.children:
            self.children[move].random()

def main():
    results = {
        '1-0': 'W',
        '0-1': 'B',
        '1/2-1/2': 'D'
    }
    root = Node()
    for line in fileinput.input():
        line = line.strip()
        if not line.startswith('1.'):
            continue
        result = line[line.index('}')+1:].strip()
        moves = line[:line.index('{')].strip().split()
        del moves[::3]
        print results[result], ' '.join(moves)
        continue
        node = root
        node.add_result(result)
        for move in moves[:10]:
            node = node.do_move(move)
            node.add_result(result)
        print root.total, root.white, root.draw, root.black
    # while True:
    #     root.visit(True)
        # root.random()
        # print

if __name__ == '__main__':
    main()

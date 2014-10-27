from data import DATA
from itertools import groupby

def group(data):
    data = sorted(data)
    def grouper(x):
        return x[:3]
    result = []
    for (key, src, dst), group in groupby(data, grouper):
        total = sum(x[-1] for x in group)
        result.append((key, src, dst, total))
    return result

def process(data):
    def grouper(x):
        return x[0]
    for key, group in groupby(data, grouper):
        group = list(group)
        total = sum(x[-1] for x in group)
        for item in group:
            pct = 100.0 * item[-1] / total
            if pct >= 10:
                print '{0x%016xL, %2d, %2d, %7d},' % item

if __name__ == '__main__':
    data = DATA
    data = group(data)
    process(data)

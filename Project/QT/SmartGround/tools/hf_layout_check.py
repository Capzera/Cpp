import random


def build(weights):
    nodes = [{'w': w, 'orig': True, 'l': None, 'r': None, 'id': i, 'parent': None} for i, w in enumerate(weights)]
    forest = list(range(len(nodes)))
    plan = []
    rows = {i: 0 for i in range(len(nodes))}

    def key(i):
        n = nodes[i]
        return (n['w'], 0 if n['orig'] else 1, n['id'])

    def shift(i, d):
        rows[i] += d
        for c in (nodes[i]['l'], nodes[i]['r']):
            if c is not None:
                shift(c, d)

    nxt = len(nodes)
    while len(forest) > 1:
        forest.sort(key=key)
        a, b = forest[0], forest[1]
        ra, rb = rows[a], rows[b]
        lift = None
        if ra != rb:
            if ra < rb:
                shift(a, rb - ra)
                lift = (a, rb - ra)
            else:
                shift(b, ra - rb)
                lift = (b, ra - rb)
        parent = {'w': nodes[a]['w'] + nodes[b]['w'], 'orig': False, 'l': a, 'r': b, 'id': nxt, 'parent': None}
        nodes.append(parent)
        nxt += 1
        nodes[a]['parent'] = parent['id']
        nodes[b]['parent'] = parent['id']
        rows[parent['id']] = max(rows[a], rows[b]) + 1
        plan.append({'a': a, 'b': b, 'p': parent['id'], 'lift': lift})
        forest.pop(0)
        forest.pop(0)
        forest.append(parent['id'])
    return nodes, plan, rows, forest[0]


def inorder_slots(nodes, root):
    slots = {}
    cnt = [0]

    def walk(i):
        n = nodes[i]
        if n['l'] is None:
            slots[i] = cnt[0]
            cnt[0] += 1
            return
        walk(n['l'])
        walk(n['r'])
    walk(root)
    return slots


def check(weights, S=58.0, LEVEL=48.0, BOTTOM=574.0, CENTER=296.0):
    nodes, plan, rows, root = build(weights)
    n = len(weights)
    slots = inorder_slots(nodes, root)
    left = CENTER - (n - 1) * S / 2.0
    xs = {}

    def setx(i):
        nd = nodes[i]
        if nd['l'] is None:
            xs[i] = left + slots[i] * S
        else:
            setx(nd['l'])
            setx(nd['r'])
            xs[i] = (xs[nd['l']] + xs[nd['r']]) / 2.0
    setx(root)

    def pos(i):
        return (xs[i], BOTTOM - rows[i] * LEVEL)

    problems = []
    for nd in nodes:
        if nd['l'] is None:
            continue
        mid = (xs[nd['l']] + xs[nd['r']]) / 2.0
        if abs(mid - xs[nd['id']]) > 1e-9:
            problems.append('bisector node %d' % nd['id'])

    worst = 1e18
    worstpair = None
    # final tree
    ids = list(range(len(nodes)))
    for i in range(len(ids)):
        for j in range(i + 1, len(ids)):
            p, q = pos(ids[i]), pos(ids[j])
            d = ((p[0] - q[0]) ** 2 + (p[1] - q[1]) ** 2) ** 0.5
            if d < worst:
                worst = d
                worstpair = (ids[i], ids[j])
    # intermediate forest states
    live = set(range(n))
    for st in plan:
        live.add(st['p'])
        live.discard(st['a'])
        live.discard(st['b'])
        comp = [i for i in live]
        for i in range(len(comp)):
            for j in range(i + 1, len(comp)):
                p, q = pos(comp[i]), pos(comp[j])
                d = ((p[0] - q[0]) ** 2 + (p[1] - q[1]) ** 2) ** 0.5
                if d < worst:
                    worst = d
                    worstpair = (comp[i], comp[j])
    return worst, worstpair, problems, rows


tests = [[45, 13, 20, 16, 9, 12, 5], [5, 9, 12, 13, 16, 45], [1, 2, 3, 4, 5, 6],
         [5, 5, 5, 5], [7, 9], [40, 8, 15, 22, 3, 12, 7, 18], [1, 2, 3, 5, 8, 13, 21, 34],
         [10, 20, 30, 40, 50], [0, 0, 0, 0], [1, 1, 2, 2, 3, 3, 4, 4]]
random.seed(7)
for _ in range(500):
    k = random.randint(2, 8)
    tests.append([random.randint(0, 60) for _ in range(k)])

overall = 1e18
bad = 0
bis = 0
for t in tests:
    w, pair, pr, rows = check(t)
    if pr:
        bis += 1
        if bis < 4:
            print('BISECTOR FAIL', t, pr[:3])
    if w < 44.0:
        bad += 1
        if bad < 8:
            print('TOO CLOSE', t, round(w, 2), pair, rows)
    overall = min(overall, w)
print('min separation', round(overall, 2), 'close cases', bad, 'bisector fails', bis, 'of', len(tests))

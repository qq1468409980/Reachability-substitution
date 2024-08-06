# %%
import copy
import numpy as np


def create_seq(u, r_rev):
    seq = []
    # early_stop = 20
    while len(seq) < 2*n:
        removed_a = set()
        # if exist a reachable to all rest B
        # pick all such a
        for item in u.items():
            if not item[1]:
                a = item[0]
                seq.append(a)
                removed_a.add(a)
                # print(a)
            else:
                break
        for a in removed_a:
            u.pop(a)
        # print(u)

        # update reachablity
        if removed_a:
            for value in r_rev.values():
                value -= set(removed_a)
            # print(r_rev)
            r_rev = dict(sorted(r_rev.items(), key=lambda item: len(item[1])))

        # if does not exist a reachable to all rest B
        # pick b from B that has the least reachable to rest A
        for item in r_rev.items():
            b = item[0]
            seq.append(b)
            r_rev.pop(b)
            # print(b)
            # update reachablity
            for value in u.values():
                value -= {b}
            # print(u)
            u = dict(sorted(u.items(), key=lambda item: len(item[1])))
            break

    return seq


def check_valid(seq, unreachablity):
    for i in range(len(seq)):
        for j in range(i+1, len(seq)):
            if seq[i] in unreachablity and seq[j] in unreachablity[seq[i]]:
                return False
    return True


def check_lost_reachablity(seq, reverse_reachablity):
    lost_reachablity = []
    for i in range(len(seq)):
        for j in range(i):
            if seq[j] in reverse_reachablity and seq[i] in reverse_reachablity[seq[j]]:
                lost_reachablity.append((seq[i], seq[j]))
    return lost_reachablity


def evaluate_seq(seq, reverse_reachablity):
    num_arc = 0
    prev_aux_status = None
    for i in range(len(seq)):
        if seq[i] < n:
            for j in range(i+1, len(seq)):
                if seq[j] in reverse_reachablity and seq[i] in reverse_reachablity[seq[j]]:
                    num_arc += 1
                    if prev_aux_status == "out":
                        num_arc += 1
                    prev_aux_status = "in"
                    break
        else:
            for j in range(i):
                if seq[i] in reverse_reachablity and seq[j] in reverse_reachablity[seq[i]]:
                    num_arc += 1
                    prev_aux_status = "out"
                    break
    num_arc += len(check_lost_reachablity(seq, reverse_reachablity))
    return num_arc


# %%
n = 50
p_threshold = 0.1
for p_threshold in np.arange(0.1, 1, 0.1):
    p_reachable = np.random.rand(n, n) + np.eye(n)
    unreachablity = {
        i: {j+n for j in range(n) if p_reachable[i, j] <= p_threshold} for i in range(n)}
    reverse_reachablity = {
        j+n: {i for i in range(n) if p_reachable[i, j] > p_threshold} for j in range(n)}
    u = copy.deepcopy(unreachablity)
    u = dict(sorted(u.items(), key = lambda item: len(item[1])))
    r_rev = copy.deepcopy(reverse_reachablity)
    r_rev = dict(sorted(r_rev.items(), key=lambda item: len(item[1])))

    seq = create_seq(u, r_rev)
    print(f"threshold: {p_threshold}")
    print(f"Legality: {check_valid(seq, unreachablity)}")
    print(f"num arcs created by heuristic: {evaluate_seq(seq, reverse_reachablity)}")
    print(f"reachability size: {(p_reachable > p_threshold).sum()-n}")

from typing import List, Any, Tuple
from math import sqrt
from copy import deepcopy
from numpy.linalg import norm
import numpy as np
from algorithms.compare_point import compare_point, unique_point_list, get_non_dominated


def raw_topsis(data: List[List[Any]], targets: List[str]) -> Tuple[List[List[Any]], List[int]]:
    """
        Pomocniczy algorytm optymalizacji metodą topsis
    """
    nzd = get_non_dominated(data, targets)
    norm_data = deepcopy(nzd)
    norm_factor = [0 for i in range(len(data) - 1)]
    weights = []
    best = []
    worst = []

    # Wyznaczanie wag
    factor = 1 / sum([i for i in range(1, len(data[0]))])
    for i in range(1, len(data[0])):
        weights.append(factor * i)
    weights.reverse()

    # Wyznaczanie wektora normalizacji
    for i in range(len(data)):
        for j in range(1, len(data[0])):
            norm_factor[j-1] += data[i][j] ** 2
    norm_factor = [sqrt(i) for i in norm_factor]

    # Normalizacja danych wejściowych
    for i in range(len(norm_data)):
        for j in range(1, len(norm_data[0])):
            w = weights[j-1]
            fac = norm_factor[j-1]
            norm_data[i][j] = (norm_data[i][j] * w) / fac
    
    # Znajdywanie ograniczeń
    for i in range(1, len(norm_data[0])):
        if targets[i-1] == 'min':
            best.append(min([j[i] for j in norm_data]))
            worst.append(max([j[i] for j in norm_data]))
        else:
            best.append(max([j[i] for j in norm_data]))
            worst.append(min([j[i] for j in norm_data]))
    
    # Obliczanie odległości
    rank = []
    for i in norm_data:
        state = i[0]
        p = np.array(i[1:])
        b_dist = norm(np.array(best) - p)
        w_dist = norm(np.array(worst) - p)
        score = w_dist / (b_dist + w_dist)
        rank.append([state, score])
    rank.sort(key=lambda elem : elem[1])

    return (rank, weights)


def topsis(data: List[List[Any]], targets: List[str]) -> Tuple[List[str], str]:
    """
        Algorytm optymalizacji metodą topsis
    """
    (rank, weights) = raw_topsis(data, targets)

    # Informacja do wyświetlenia
    info = f"""
    Wagi wyznaczane zgodnie z szeregiem
    w_i = (1/s) * (i_max-i)
    s = sum(n=1, i)[n(n+1)/2]
    W tym wypadku: {weights}
    """
    
    # Wyznaczanie punktu idealnego i antyidealnego
    return ([i[0] for i in rank], info)


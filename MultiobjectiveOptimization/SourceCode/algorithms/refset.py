from typing import List, Any, Tuple
from algorithms.compare_point import compare_point, unique_point_list
from algorithms.topsis import raw_topsis


def refset(data: List[List[Any]], targets: List[str]) -> Tuple[List[str], str]:
    """
        Algorytm optymalizacji metodą refset
    """
    # Podział danych
    classes = 3
    divider = round(len(data) / classes)
    groups = [data[:divider], data[divider:2*divider], data[2*divider:]]

    # Rozwiązanie grup
    result = []
    for i in range(classes):
        result.append(raw_topsis(groups[i], targets)[0])
    
    # Update wag
    result2 = []
    for i in range(classes):
        elem = result[i]
        for j in range(len(elem)):
            elem[j][1] += 1
        result2.extend(elem)
    
    result2.sort(key=lambda elem : elem[1])
    return ([i[0] for i in result2], "")

from typing import List, Any, Tuple
from algorithms.compare_point import compare_point, unique_point_list


def naive_owd(data: List[List[Any]], targets: List[str]) -> Tuple[List[str], str]:
    """
        Algorytm optymalizacji naiwnym OWD z drugiego tematy tych labów
    """
    zd = [];
    nzd = [];
    elements_to_check = data;
    while len(elements_to_check) > 0:
        # Inicjalizacja pętli głównej
        active_element = elements_to_check[0]
        incomparable = []
        fl = False

        # Wykonanie kroku optymalizacji
        for i in range(1, len(elements_to_check)):
            flag = compare_point(active_element, elements_to_check[i], targets)
            if flag == -1:
                zd.append(active_element)
                active_element = elements_to_check[i]
                fl = True
            elif flag == 0:
                incomparable.append(elements_to_check[i])
            else:
                zd.append(elements_to_check[i])
        
        # Operacje zakończenia pętli głównej
        nzd.append(active_element)
        nzd = unique_point_list(nzd)
        elements_to_check = incomparable
    
    states = [i[0] for i in nzd]
    return (states, "")
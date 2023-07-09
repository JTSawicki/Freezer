from typing import List, Any


def compare_point(point1: List[Any], point2: List[Any], targets: List[str]) -> int:
    """
        Funckcja porównuje dwa punkty danych o tej samej długości
        Args:
            point1(List[Any]): Punkt danych [stan(str), parametr1(float), parameter2(float), parameter3(float), ...]
            point2(List[Any]): Punkt danych [stan(str), parametr1(float), parameter2(float), parameter3(float), ...]
            targets(List[str]): Lista celów optymalizacji dla kolejnych parametrów 'min' lub 'max'
        Returns:
            int: -1 dla point1 gorszy, 1 point1 lepszy, 0 nie da się jednoznacznie porównać lub równe
    """
    result = 0
    for i in range(1, len(point1)):
        target = True if targets[i-1] == 'min' else False
        p1 = point1[i]
        p2 = point2[i]
        flag = p1 < p2
        diff = flag == target
        if (point1[i] < point2[i]) == target:
            if result == -1:
                return 0
            result = 1
        elif point1[i] == point2[i]:
            pass
        else:
            if result == 1:
                return 0
            result = -1
    return result


def point_equal(point1: List[Any], point2: List[Any]) -> bool:
    """
        Funckcja sprawdza czy dwa punkty danych o tej samej długości są sobie równe
        Args:
            point1(List[Any]): Punkt danych [stan(str), parametr1(float), parameter2(float), parameter3(float), ...]
            point2(List[Any]): Punkt danych [stan(str), parametr1(float), parameter2(float), parameter3(float), ...]
        Returns:
            bool: Czy punkty równe sobie
    """
    for i in range(len(point1)):
        if point1[i] != point2[i]:
            return False
    return True


def unique_point_list(point_list: List[List[Any]]) -> List[List[Any]]:
    """
        Funckcja zwraca listę punktów o niepowtarzających się wartościach
        Args:
            point_list(List[List[Any]]): Lista punktów danych. Punkt danych [stan(str), parametr1(float), parameter2(float), parameter3(float), ...]
        Returns:
            List[List[Any]]: Lista punktów o niepowtarzających się wartościach
    """
    result = []
    for elem in point_list:
        flag = False
        for result_elem in result:
            if point_equal(elem, result_elem):
                flag = True
        if not flag:
            result.append(elem)
    return result


def get_non_dominated(point_list: List[List[Any]], targets: List[str]) -> List[List[Any]]:
    """
        Funckcja zwraca listę punktów o niezdominowanych
        Args:
            point_list(List[List[Any]]): Lista punktów danych. Punkt danych [stan(str), parametr1(float), parameter2(float), parameter3(float), ...]
            targets(List[str]): Lista celów optymalizacji dla kolejnych parametrów 'min' lub 'max'
        Returns:
            List[List[Any]]: Lista punktów o niepowtarzających się wartościach
    """
    result = []
    for i in range(len(point_list)):
        point = point_list[i]
        is_dominated = False
        for j in range(len(point_list)):
            point_2 = point_list[j]
            compare_flag = compare_point(point, point_2, targets)
            if compare_flag == -1:
                is_dominated = True
                break
        if is_dominated == False:
            result.append(point)
    return result

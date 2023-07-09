'''
Plik zawiera informacje o algorytmach optymalizacji

W celu napisania algorytmu należy zrobić nowy plik o nazwie <nazwa_algorytmu>.py w folderze algorithms i napisać w nim funkcję <nazwa_algorytmu>
Następnie należy dodać ten algorytm do dołownika algorithms w tym pliku

Jeden przykładowy algorytm pokazujący wszystko wraz z opisem zmiennych został zamieszczony poniżej
'''
from typing import List, Any, Tuple
from algorithms.topsis import topsis
from algorithms.naive_owd import naive_owd
from algorithms.refset import refset


# Przykład implementacji działającego algorytmu
def ShowAll(data: List[List[Any]], targets: List[str]) -> Tuple[List[str], str]:
    """
    Zwraca wszystkie dane jako optymalne
    Minimalna ilość prametrów to 2
    Args:
        data(List[List[Any]]): Lista danych pobieranych przez algorytm optymalizacji. Wiersze mają postać [stan(str), parametr1(float), parameter2(float), parameter3(float), ...]
        targets(List[str]): Lista celów optymalizacji dla kolejnych parametrów 'min' lub 'max'
    Returns:
        Tuple[List[str], str]: (Lista stanów, Informacje dodatkowe do wyświetlenia)
    """
    result = []
    for i in range(len(data)):
        result.append(data[i][0])
    return (result, "")


algorithms = {
    'Pokaż wszystkie': ShowAll,
    'Naiwny OWD': naive_owd,
    'TOPSIS': topsis,
    'REFSET': refset
}

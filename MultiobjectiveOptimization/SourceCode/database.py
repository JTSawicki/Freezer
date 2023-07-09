import pandas as pd
import pickle
import numpy as np
from pathlib import Path
from typing import List, Dict

path_to_file = "preprocessed_data.pkl"
data = None

keys_to_names = {
    'STATE': 'Stan',
    'POPULATION': 'Populacja',
    'WHITE': 'Ludność rasy białej',
    'BLACK': 'Ludność rasy czarnej',
    'AMERI_ES': 'Ludność rasy indiańskiej',
    'ASIAN': 'Ludność rasy żółtej',
    'HAWN_PI': 'Ludność rasy havańskiej',
    'HISPANIC': 'Ludność rasy hispano',
    'OTHER': 'Lodność rasy innej',
    'MULT_RACE': 'Lodność rasy mieszanej',
    'MALES': 'Męszczyźni',
    'FEMALES': 'Kobiety',
    'AGE_UNDER5': 'Ludzie w wieku < 5',
    'AGE_5_9': 'Ludzie w wieku (5, 9)',
    'AGE_10_14': 'Ludzie w wieku (10, 14)',
    'AGE_15_19': 'Ludzie w wieku (15, 19)',
    'AGE_20_24': 'Ludzie w wieku (20, 24)',
    'AGE_25_34': 'Ludzie w wieku (25, 34)',
    'AGE_35_44': 'Ludzie w wieku (35, 44)',
    'AGE_45_54': 'Ludzie w wieku (45, 54)',
    'AGE_55_64': 'Ludzie w wieku (55, 64)',
    'AGE_65_74': 'Ludzie w wieku (65, 74)',
    'AGE_75_84': 'Ludzie w wieku (75, 84)',
    'AGE_85_UP': 'Ludzie w wieku > 85',
    'Public_Elementary_Schools': 'Publiczne szkoły podstawowe',
    'Public_MIDDLE_Schools': 'Publiczne szkoły średnie',
    'Public_HIGH_Schools': 'Publiczne szkoły wyższe',
    'Private_Elementary_Schools': 'Prywatne szkoły podstawowe',
    'Private_MIDDLE_Schools': 'Prywatne szkoły średnie',
    'Private_HIGH_Schools': 'Prywatne szkoły wyższe',
    'Total_Schools': 'Szkoły łącznie',
    'total_hospitals': 'Szpitale',
    'mean_salary': 'Średnia płaca',
    'median_salary': 'Mediana płacy',
    'crime_property_per100k': 'Kradzierze na 100 tys. mieszkańców',
    'crime_violent_per100k': 'Napady na 100 tys. mieszkańców',
    'rent_1_bedroom': 'Cena wynajmu 1 pokojowego mieszkania',
    'rent_2_bedroom': 'Cena wynajmu 2 pokojowego mieszkania',
    'rent_3_bedroom': 'Cena wynajmu 3 pokojowego mieszkania'
}
names_to_keys = {v: k for k, v in keys_to_names.items()}

state_name_to_code = {
    "Alabama": "AL",
    "Alaska": "AK",
    "Arizona": "AZ",
    "Arkansas": "AR",
    "California": "CA",
    "Colorado": "CO",
    "Connecticut": "CT",
    "Delaware": "DE",
    "District of Columbia": "DC",
    "Florida": "FL",
    "Georgia": "GA",
    "Hawaii": "HI",
    "Idaho": "ID",
    "Illinois": "IL",
    "Indiana": "IN",
    "Iowa": "IA",
    "Kansas": "KS",
    "Kentucky": "KY",
    "Louisiana": "LA",
    "Maine": "ME",
    "Maryland": "MD",
    "Massachusetts": "MA",
    "Michigan": "MI",
    "Minnesota": "MN",
    "Mississippi": "MS",
    "Missouri": "MO",
    "Montana": "MT",
    "Nebraska": "NE",
    "Nevada": "NV",
    "New Hampshire": "NH",
    "New Jersey": "NJ",
    "New Mexico": "NM",
    "New York": "NY",
    "North Carolina": "NC",
    "North Dakota": "ND",
    "Ohio": "OH",
    "Oklahoma": "OK",
    "Oregon": "OR",
    "Pennsylvania": "PA",
    # "Puerto Rico": "PR", -> terytorium nieinkorporowane
    "Rhode Island": "RI",
    "South Carolina": "SC",
    "South Dakota": "SD",
    "Tennessee": "TN",
    "Texas": "TX",
    "Utah": "UT",
    "Vermont": "VT",
    "Virginia": "VA",
    # "Virgin Islands": "VI", -> terytorium nieinkorporowane
    "Washington": "WA",
    "West Virginia": "WV",
    "Wisconsin": "WI",
    "Wyoming": "WY",
}
state_code_to_name = {v: k for k, v in state_name_to_code.items()}

def initDatabase():
    """
        Inicjalizuje zbiór danych
    """
    global data
    print(Path.cwd())
    with open(path_to_file, 'rb') as f:
        data = pickle.load(f)


def GetListOfColumns() -> List[str]:
    """
        Zwraca listę kolumn
    """
    global data
    return list(data.columns)


def GetListOfParams() -> List[str]:
    """
        Zwraca listę kolumn bez kolumny 'STATE'
    """
    global data
    result = list(data.columns)
    result.remove('STATE')
    return result


def GetData(params: List[str]) -> List[List[str]]:
    """
        Zwraca listę danych dla podanych parametrów
        Args:
            params(List[str]): Lista parametrów optymalizacji
        Returns:
            List[List[str]]: Lista danych pobieranych przez algorytm optymalizacji. Wiersze mają postać [stan, parametr1, parameter2, parameter3]
    """
    global data
    result = data[['STATE'] + params]
    return result.values.tolist()


def GetDataByStates(params: List[str], states: List[str]) -> List[List[str]]:
    """
        Zwraca listę danych dla podanych parametrów i stanów
        Args:
            params(List[str]): Lista parametrów optymalizacji
            states(List[str]): Lista wybranych stanów
        Returns:
            List[List[str]]: Lista danych pobieranych przez algorytm optymalizacji. Wiersze mają postać [stan, parametr1, parameter2, parameter3]
    """
    global data
    result = data[['STATE'] + params]
    result = result[result['STATE'].isin(states)]
    result = result.values.tolist()

    # Sortowanie względem kolejności wejścia
    result2 = []
    for i in states:
        for j in result:
            if j[0] == i:
                result2.append(j)
                break

    return result2


def ConvertDataByKey(data: List[str], map: Dict[str, str]) -> List[str]:
    """
        Funkcja zmienia zawartość listy względem podanej mapy
    """
    return [map[elem] for elem in data]

from typing import List, Tuple
from tkinter import BaseWidget
from tkfactory.TkFactory import TkFactory
from tkinter import X, Y, BOTH, YES, LEFT, RIGHT, TOP, BOTTOM, END, W, E, N, S, EW


class ChoiceBox():
    """
    Box wyboru parametru optymalizacji.
    W celu załadowania do GUI po utworzeniu należy wywołać funkcję pakującą na elemencie self.Frame
    """

    def __init__(self, info: str, choices: List[str], targets: List[str], factory: TkFactory, master: BaseWidget):
        """
        Funkcja inicjalizuje box wyboru parametru optymalizacji
        Args:
            info(str): Informacja o parametrze
            choices(List[str]): Lista pól do wyboru w combobox
            targets(List[str]): Lista celów optymalizacji do wyboru w drugim combobox
            factory(TkFactory): Fabryka elementów GUI
            master(BaseWidget): Nadrzędny element GUI
        """
        self.info = info
        self.choices = choices
        self.targets = targets
        self.factory = factory

        self.widgets = {}
        self.Frame = factory.getFrame(master)

        self.build()
    
    def build(self):
        """
            Funkcja konstruje gui
        """
        self.Frame.grid_columnconfigure(0, weight=1)
        self.Frame.grid_columnconfigure(1, weight=1)
        self.Frame.grid_columnconfigure(2, weight=1)
        self.widgets["label"] = self.factory.getLabel(self.Frame, self.info)
        self.widgets["label"].grid(column=0, row=0, sticky=EW)
        self.widgets["choice_combo"] = self.factory.getCombobox(self.Frame, self.choices)
        self.widgets["choice_combo"].current(0)
        self.widgets["choice_combo"].get
        self.widgets["choice_combo"].grid(column=1, row=0, sticky=EW)
        self.widgets["target_combo"] = self.factory.getCombobox(self.Frame, self.targets)
        self.widgets["target_combo"].current(0)
        self.widgets["target_combo"].grid(column=2, row=0, sticky=EW)
    
    def get_selected(self) -> Tuple[str, str]:
        """
            Funckja zwraca obecnie wybrane wartości
            Returns:
                (Tuple[str, str]): Krotka [parametr, cel]
        """
        return tuple([self.widgets["choice_combo"].get(), self.widgets["target_combo"].get()])

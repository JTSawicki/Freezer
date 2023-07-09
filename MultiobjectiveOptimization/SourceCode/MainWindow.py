import tkinter as tk
import tkinter.ttk as ttk
from tkinter import X, Y, BOTH, YES, LEFT, RIGHT, TOP, BOTTOM, END, E, W, N, S
from tkfactory.TkFactory import TkFactory
from tkfactory.MyFactory import MyFactory
from tkinter import messagebox
from tkinter import filedialog
from ChoiceBox import ChoiceBox
from algorithms.info import algorithms
import database

factory = TkFactory.getFactory()
mfactory = MyFactory.getFactory()

ParameterCount = 10


class MainWindow(tk.Tk):
    """
        Główne okno programu
    """

    def __init__(self):
        """
            Inicjacja głównego okna
            Zmienne:
                widgets (dic): Słownik zawierający łącza do widgetów
        """
        tk.Tk.__init__(self)
        self.widgets = {}
        self.choice_boxes = []
        self.build()
        self.initActions()
        self.refreshGui()

    def build(self):
        """
            Funkcja konstruje gui okna
        """
        # Inicjalizacja ramki głównej programu
        self.widgets["mainFrame"] = factory.getFrame(self)
        self.widgets["mainFrame"].pack(fill=BOTH, expand=YES)

        # Tworzenie menu symulacji
        self.widgets["choiceFrame"] = factory.getFrame(self.widgets["mainFrame"])
        self.widgets["choiceFrame"].pack(side=LEFT, fill=Y, padx=5, pady=5)
        self.widgets["choiceFrame"].config(borderwidth=2)
        self.widgets["paramLabel"] = factory.getLabel(self.widgets["choiceFrame"], "Parametry optymalizacji")
        self.widgets["paramLabel"].pack(side=TOP, fill=X)

        # Dodawanie pól wyboru parametrów
        choices = database.ConvertDataByKey(database.GetListOfParams(), database.keys_to_names)
        choices = ['brak'] + choices
        for i in range(ParameterCount):
            combo = ChoiceBox('Parametr %02d' % (i+1,), choices, ['min', 'max'], factory, self.widgets["choiceFrame"])
            combo.Frame.pack(side=TOP, fill=X, padx=5, pady=5)
            self.choice_boxes.append(combo)
        
        # Wybór algorytmu
        self.widgets["algoLabel"] = factory.getLabel(self.widgets["choiceFrame"], "Algorytm optymalizacji")
        self.widgets["algoLabel"].pack(side=TOP, fill=X)
        self.widgets["algoCombo"] = factory.getCombobox(self.widgets["choiceFrame"], list(algorithms.keys()))
        self.widgets["algoCombo"].current(0)
        self.widgets["algoCombo"].pack(side=TOP, fill=X, padx=5, pady=5)

        # Informacje wyjściowe z algorytmu
        self.widgets["algInfo"] = factory.getLabel(self.widgets["choiceFrame"], "")
        self.widgets["algInfo"].pack(side=TOP, anchor="w")

        # Przycisk wywołania symulacji
        self.widgets["runButton"] = factory.getButton(self.widgets["choiceFrame"], "Optymalizuj", self.optimize_command)
        self.widgets["runButton"].pack(side=BOTTOM, fill=X)

        # Informacja o ilości wyników
        self.widgets["resultCountLabel"] = factory.getLabel(self.widgets["choiceFrame"], "Ilość wyników: brak")
        self.widgets["resultCountLabel"].pack(side=BOTTOM, anchor="w")

        # Widok znalezionych elementów
        style = ttk.Style(self.widgets["mainFrame"])
        style.configure('Treeview', rowheight=60)
        self.widgets["mTree"] = factory.getTreeview(self.widgets["mainFrame"])
        self.widgets["mTree"].heading('#0', text='Stan')
        self.widgets["mTree"]["columns"] = tuple([str(i) for i in range(ParameterCount)])
        self.widgets["mTree"].column("#0", width=100, anchor='c')
        for i in range(ParameterCount):
            self.widgets["mTree"].column(str(i), width=100, anchor='c')
            self.widgets["mTree"].heading(str(i), text='')
        self.widgets["mTree"].pack(side=LEFT, fill=BOTH, expand=YES)

        # Scrollbar dla widoku elementów
        self.widgets["mTreeScrollbar"] = ttk.Scrollbar(self.widgets["mainFrame"], orient="vertical", command=self.widgets["mTree"].xview)
        self.widgets["mTreeScrollbar"].pack(side=RIGHT, fill=Y)
        self.widgets["mTree"].configure(yscrollcommand = self.widgets["mTreeScrollbar"].set)

    def initActions(self):
        """
            Funkcja inicjuje domyślne zachowania gui takie jak reakcja na zmianę pola
        """
        pass
        # self.widgets["mcTypeCombo"].bind("<<ComboboxSelected>>", lambda x: self.loadTable(self.widgets["mcTypeCombo"].get()))

    def refreshGui(self):
        """
            Funkcja odświeża elementy gui takie jak wartości pól combobox
        """
        pass

    def optimize_command(self):
        """
            Komenda wywołująca działanie optymalizacji
        """
        selected_parameters = []
        selected_targets = []
        selected_algorithm = self.widgets["algoCombo"].get()
        for i in range(ParameterCount):
            selected = self.choice_boxes[i].get_selected()
            if selected[0] != 'brak':
                selected_parameters.append(selected[0])
                selected_targets.append(selected[1])

        if len(selected_parameters) < 2 and selected_algorithm != 'Pokaż wszystkie':
            messagebox.showinfo("Parametry optymalizacji", "Wybrano za mało parametrów optymalizacji\nMinimum to 2")
            return
        
        if len(set(selected_parameters)) != len(selected_parameters):
            messagebox.showinfo("Parametry optymalizacji", "Nie można wybrać dwa razy tego samego parametru optymalizacji")
            return
        
        raw_selected_parameters = selected_parameters
        selected_parameters = database.ConvertDataByKey(selected_parameters, database.names_to_keys)

        data = database.GetData(selected_parameters)
        (states, info) = algorithms[selected_algorithm](data, selected_targets)
        data_to_show = database.GetDataByStates(selected_parameters, states)

        # Wyświetlanie informacji
        if info != "":
            self.widgets["algInfo"].config(text = f"Wybrany algorytm: {selected_algorithm}\n{info}")
        else:
            self.widgets["algInfo"].config(text = "")

        # Wyświetlanie informacji o ilości wyników
        self.widgets["resultCountLabel"].config(text = f"Ilość wyników: {len(states)}")

        # Wyświetlanie wyników w tabeli
        # Usuwanie starych elemntów z tabeli
        for child in self.widgets["mTree"].get_children():
            self.widgets["mTree"].delete(child)
        # Dodawanie nowych elementów do drzewa
        for elem in data_to_show:
            self.widgets["mTree"].insert("", "end", text=database.state_code_to_name[elem[0]], values=elem[1:])
        # Zmienianie tytułów kolumn
        for i in range(ParameterCount):
            if i < len(raw_selected_parameters):
                self.widgets["mTree"].heading(str(i), text=raw_selected_parameters[i])
            else:
                self.widgets["mTree"].heading(str(i), text='')

        
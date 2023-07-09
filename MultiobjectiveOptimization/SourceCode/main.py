import database
from MainWindow import MainWindow
from tkfactory.TkFactory import TkFactory
from tkinter import font
from tkinter import messagebox

# Inicjowanie niezbędnych klasy bazy danych i fabryki abstrakcyjnej
database.initDatabase()

factory = TkFactory.getFactory()

# Tworzenie głównego okna
root = MainWindow()
factory.addExternalWidget(root, "root")

# Ustawianie ikony programu
# root.iconbitmap('databasefolder/programicon.ico')

# Zmienia nie czcionek
# default_font = font.nametofont("TkDefaultFont")
# default_font.configure(family="Georgia")

# Uruchamianie programu
root.mainloop()
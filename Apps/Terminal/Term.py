from tkinter import *
import os

def terminal():
    os.system("lxterminal")

window = Tk()

window.geometry("95x35")
window.minsize(95, 35)
window.maxsize(95, 35)

button = Button(window, text="Terminal", command=terminal)
button.pack()

window.mainloop()
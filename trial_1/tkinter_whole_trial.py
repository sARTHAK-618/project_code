import tkinter as tk
from tkinter import messagebox
window = tk.Tk()
window.title("Hello world")
window.config(bg = "grey")
window.geometry("500x500")

font_style = ("Arial", 20)

def Veg_Selection(event):
    data_text = f"{clicked.get()} data processing..."
    msg_text = f"""This will start the process of cultivating the plant with its respective need...
                 plant selected -->{clicked.get()} """
    if clicked.get() == "Spinach":
        vege_label = tk.Label(window, text = data_text, bg = "grey", font = font_style).pack()
    elif clicked.get() == "Mint":
        vege_label = tk.Label(window, text = data_text, bg = "grey", font = font_style).pack()
    elif clicked.get() == "Corriander":
        vege_label = tk.Label(window, text = data_text, bg = "grey", font = font_style).pack()
    elif clicked.get() == "Fenugreek":
        vege_label = tk.Label(window, text = data_text, bg = "grey", font = font_style).pack()
    messagebox.showinfo("about", msg_text)

clicked = tk.StringVar()
clicked.set("Vegetables")

options = ["Spinach", "Mint", "Corriander", "Fenugreek"]

# Create OptionMenu widget
vegies = tk.OptionMenu(window, clicked, *options, command = Veg_Selection)
vegies.config(font = font_style, bg = "grey")
vegies.pack()

vegies_names = window.nametowidget(vegies.menuname)
vegies_names.config(font = font_style, bg = "grey")



window.mainloop()

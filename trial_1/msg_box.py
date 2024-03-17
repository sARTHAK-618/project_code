import tkinter as tk
from tkinter import ttk
from tkinter import messagebox
window = tk.Tk()
window.geometry("500x500")
font_style = ("Arial", 20)
window.config(bg = "grey")

def test():
    answer  = messagebox.showinfo("about", "")
    print(answer)
random = tk.Button(window, text = "Click me", command = test)
random.pack()

window.mainloop()
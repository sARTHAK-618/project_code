import matplotlib.pyplot as plt


def spinach_window():
    spin_window = tk.Toplevel()
    spin_window.title("Spinach Data and Visualization")
    spin_window.geometry("700x700")
    spin_window.state("zoomed")

    upper_frame = tk.Frame(spin_window)
    upper_frame.pack(fill = "both", expand = True)
    canvas1 = FigureCanvasTkAgg(matplotlib_normal_test, upper_frame)
    canvas1.draw()
    canvas1.get_tk_widget().pack(side="left", fill="both", expand=True)
    
plt.style.use("fivethirtyeight")
x_vals = [1, 2, 3, 4, 5]
y_vals = [1, 3, 2, 3, 4]

plt.plot(x_vals, y_vals)
plt.show()

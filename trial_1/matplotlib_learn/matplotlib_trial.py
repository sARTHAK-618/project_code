import random
from itertools import count
import pandas as pd
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation


plt.style.use("fivethirtyeight")

x_vals = []
y_vals = []

index = count()

def animate(i):
    x_vals.append(next(index))
    y_vals.append(random.randint(0, 5))
    plt.plot(x_vals, y_vals)
ani = FuncAnimation(plt.gcf(), animate, interval=100)




def show_plot():
    ani = FuncAnimation(plt.gcf(), animate, interval=100)
    plt.tight_layout()
    plt.show()

show_plot()
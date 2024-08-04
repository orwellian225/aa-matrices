import sys
import math as m

import numpy as np
import polars as pl
import seaborn as sns
import matplotlib.pyplot as plt
from matplotlib.ticker import FuncFormatter

max_n = 32
step = 8

if len(sys.argv) == 3:
    max_n = int(sys.argv[1])
    step = int(sys.argv[2])

mem_data_schema = { "n": pl.UInt32, "Algorithms": pl.String, "Memory Usage (B)": pl.UInt64 }
mem_data = { "n": [], "Algorithms": [], "Memory Usage (B)": [] }

for n in range(step, max_n + 1, step):
    i = n // step - 1
    padded_n: int  = 2**(m.ceil(m.log2(n)))
    tree_layers = m.log2(padded_n)
    tree_layer_nodes = 2**tree_layers - 1

    mem_data["Algorithms"].append("smm")
    mem_data["Algorithms"].append("smmr")
    mem_data["Algorithms"].append("strassen")

    mem_data["n"].append(n)
    mem_data["n"].append(n)
    mem_data["n"].append(n)

    mem_data["Memory Usage (B)"].append(n * n * 4 * 3) # 3 n^2 matrices of single precision floats (4 bytes)
    mem_data["Memory Usage (B)"].append(padded_n * padded_n * 4 * 3) # 3 n^2 matrices of single precision floats (4 bytes)
    mem_data["Memory Usage (B)"].append(padded_n * padded_n * 4 * 3 + 4 * 16 * padded_n * padded_n - 4 * 16 * padded_n) # 3 n^2 + each function calls allocates 8 sub matrices

mem_data = pl.DataFrame(data=mem_data, schema=mem_data_schema)

_ = sns.set_style("whitegrid")
ax = sns.lineplot(data=mem_data.select(
    pl.col("n"),
    (pl.col("Memory Usage (B)") / 1024 / 1024).alias("Memory Usage (mB)"),
    pl.col("Algorithms")
), x="n", y="Memory Usage (mB)", hue="Algorithms", palette="Set2")

ax.set_title("Algorithm Memory Usage")
ax.set_xscale("log", base=2)
ax.xaxis.set_major_formatter(FuncFormatter(lambda x, pos: str(int(x))))
plt.savefig("data/memusage.pdf")

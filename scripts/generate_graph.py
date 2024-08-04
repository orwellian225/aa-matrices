import sys

import seaborn as sns
import polars as pl
import matplotlib.pyplot as plt
from matplotlib.ticker import FuncFormatter

if len(sys.argv) == 2:
    csv_path = sys.argv[1]
else:
    csv_path = input("Enter csv filepath: ")

data = pl.read_csv(source=csv_path, schema={"n": pl.Int32, "algorithm": pl.String, "duration_s": pl.Float32, "correct": pl.UInt8})

_ = sns.set_style("whitegrid")
ax = sns.lineplot(data=data.select(
    pl.col("n"),
    pl.col("algorithm").alias("Algorithms"),
    pl.col("duration_s").alias("Duration (s)"),
), x="n", y="Duration (s)", hue="Algorithms", palette="Set2")

ax.set_title("Algorithm Runtimes")
ax.set_xscale('log', base=2)
ax.xaxis.set_major_formatter(FuncFormatter(lambda x, pos: str(int(x))))
plt.savefig("data/timeduration.pdf")

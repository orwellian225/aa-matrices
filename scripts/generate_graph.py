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
).filter( pl.col("n").is_between(0, 1024, closed="both") ), x="n", y="Duration (s)", hue="Algorithms", palette="Set2", sort=True)

ax.set_title("Algorithm Runtimes at small $n$")
ax.set_xscale('log', base=2)
ax.xaxis.set_major_formatter(FuncFormatter(lambda x, pos: str(int(x))))
plt.savefig("data/timeduration_short.pdf")
plt.close()

ax = sns.lineplot(data=data.select(
    pl.col("n"),
    pl.col("algorithm").alias("Algorithms"),
    pl.col("duration_s").alias("Duration (s)"),
).filter( pl.col("n").is_between(1024, 8192, closed="both") ), x="n", y="Duration (s)", hue="Algorithms", palette="Set2", sort=True)

ax.set_title("Algorithm Runtimes at large $n$")
ax.set_xscale('log', base=2)
ax.xaxis.set_major_formatter(FuncFormatter(lambda x, pos: str(int(x))))
plt.savefig("data/timeduration_long.pdf")
plt.close()

temp = data.pivot("algorithm", values="duration_s", index="n", aggregate_function="mean")
ratios = temp.select(
	pl.col("n"),
	(pl.col("smmr") / pl.col("smm") ).alias("SMMR"),
	(pl.col("strassen") / pl.col("smm") ).alias("Strassen"),
).unpivot(index="n", on=("SMMR", "Strassen"), variable_name="ratio name", value_name="ratio")

ax = sns.lineplot(data=ratios.filter(pl.col("n").is_between(1024, 8192, closed="both")), x="n", y="ratio", hue="ratio name", palette="Set2", sort=True)

ax.set_title("Algorithm Ratios at large $n$")
ax.set_xscale('log', base=2)
ax.xaxis.set_major_formatter(FuncFormatter(lambda x, pos: str(int(x))))
plt.savefig("data/timeratio_long.pdf")
plt.close()

ax = sns.lineplot(data=ratios.filter(pl.col("n").is_between(0, 1024, closed="both")), x="n", y="ratio", hue="ratio name", palette="Set2", sort=True)

ax.set_title("Algorithm Ratios at small $n$")
ax.set_xscale('log', base=2)
ax.xaxis.set_major_formatter(FuncFormatter(lambda x, pos: str(int(x))))
plt.savefig("data/timeratio_short.pdf")
plt.close()

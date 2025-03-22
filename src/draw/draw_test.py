import matplotlib.pyplot as plt
from matplotlib.backends.backend_pdf import PdfPages
import json

test_methods = dict(json.load(open("../../test_methods.json")))


dipazone_for_count = test_methods["dipazone_for_count"]
methods = list(test_methods.keys())

pdf = PdfPages("test_methods.pdf")
for t in range(len(methods)):
    if(methods[t] == "dipazone_for_count"):
        continue
    x = [i*dipazone_for_count for i in range(len(test_methods[methods[t]]))]
    y = test_methods[methods[t]]
    figure = plt.figure()
    axes = figure.subplots()
    axes.plot(x, y)
    axes.set_title(methods[t])
    axes.set_xlabel("euclidean distance")
    axes.set_ylabel("count per dipazone")
    pdf.savefig(figure)
    plt.close()
pdf.close()

test_steps = dict(json.load(open("../../test_steps.json")))
step_start = test_steps["step start"]
step_end = test_steps["step end"] 
step_step = test_steps["step step"]
ideal_step = test_steps["ideal step"]
not_methods = ["step start", "step end", "step step", "ideal step"]
methods = [i for i in list(test_steps.keys()) if i not in not_methods]
dipazone = step_end - step_start
iterations = round(dipazone // step_step)

pdf = PdfPages("test_steps.pdf")
x = [i*step_step for i in range(iterations)]
for t in range(len(methods)):
    y = test_steps[methods[t]]["max distance per step"]
    figure = plt.figure()
    axes = figure.subplots()

    axes.set_title(methods[t])
    axes.set_xlabel("h")
    axes.set_ylabel("max distance per step")
    axes.plot(x, y, label=f"ideal step = {ideal_step}")
    axes.legend()
    pdf.savefig(figure)
    plt.close()
pdf.close()
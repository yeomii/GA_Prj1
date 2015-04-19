import matplotlib.pyplot as plt
from matplotlib.path import Path
import matplotlib.patches as patches

def __main__():
    fin = open("cycle.in", "r")
    solin = open("cycle.out", "w")

    verts = []
    size = fin.readline()
    for i in range(size):
        t = fin.readline().split(' ')
        verts += (float(t[0]), float(t[1]))
    verts += (0,0)
    codes = [Path.MOVETO] + [Path.LINETO]*(size - 1) + [Path.CLOSEPOLY]

    path = Path(verts, codes)

    fig = plt.figure()
    ax = fig.add_subplot(111)
    patch = patches.PathPatch(path, facecolor='orange', lw=2)
    ax.add_patch(patch)
    ax.set_xlim(-2,2)
    ax.set_ylim(-2,2)
    plt.show()
    plt.savefig("simple_path.png")

__main__()

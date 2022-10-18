
import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D

if __name__== "__main__":
    numVars = 3
    D = np.fromfile("./state_snapshots.bin")
    numTimeSteps = int(np.size(D)/numVars)
    print(numTimeSteps)
    #D  = np.reshape(data, (numTimeSteps, numVars))

    fig = plt.figure()
    ax = fig.add_subplot(1, 1, 1, projection='3d')
    ax.plot(D[0::3], D[1::3], D[2::3])
    fig.savefig("plot.png", format="png", bbox_inches='tight', dpi=300)
    plt.show()

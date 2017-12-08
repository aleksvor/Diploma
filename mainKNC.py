# -*- coding: utf-8 -*-
import numpy as np
import pandas as pd
from sklearn import model_selection
from sklearn.neighbors import KNeighborsClassifier
from sklearn.preprocessing import scale

data = pd.read_table('objectMatrix.txt', sep=',', header = None)

y = data[18]
x = data[list(range(0,18))]

print(y)
print(x)

x = pd.DataFrame(scale(x))

# кросс-валидация и поиск наилучшего k
kf = model_selection.KFold(n_splits=5, shuffle=True, random_state=42)

result = pd.Series([])

for k in range(1,101):
    neigh = KNeighborsClassifier(n_neighbors=k, weights='distance', n_jobs=-1)
    scores = model_selection.cross_val_score(neigh, x, y, cv=kf, scoring='neg_log_loss')
    for i in range(0, scores.shape[0]):
        scores[i] = abs(scores[i])
    result[k] = np.mean(scores)

best = result.idxmin()
print(best)
print(result[best])

neigh = KNeighborsClassifier(n_neighbors=best, weights='distance')
neigh.fit(x, y)

data = pd.read_table('objectMatrixTest.txt', sep=',', header=None)

xt = data[list(range(1, 19))]

print(xt)

xt = pd.DataFrame(scale(xt))

yt = neigh.predict_proba(xt)

lab = data[0]

with open("SubmissionKNC.csv", "w") as f:
    f.write('Id,Prediction1,Prediction2,Prediction3,Prediction4,Prediction5,Prediction6,Prediction7,Prediction8,Prediction9\n')
    for i in range(0, yt.shape[0]):
        f.write(lab[i])
        f.write(",")
        for j in range(0, 8):
            f.write(str(yt[i][j]))
            f.write(",")
        f.write(str(yt[i][8]))
        f.write("\n")

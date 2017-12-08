# -*- coding: utf-8 -*-
import numpy as np
import pandas as pd
from sklearn import model_selection
from sklearn.neural_network import MLPClassifier
from sklearn.preprocessing import scale

data = pd.read_table('objectMatrix.txt', sep=',', header = None)

y = data[18]
x = data[list(range(0, 18))]

x = pd.DataFrame(scale(x))

# кросс-валидация
kf = model_selection.KFold(n_splits=5, shuffle=True, random_state=42)

mlp = MLPClassifier(solver='adam', shuffle=True, random_state=42)

scores = model_selection.cross_val_score(mlp, x, y, cv=kf, scoring="neg_log_loss")
print(np.mean(scores))

mlp.fit(x, y);

data = pd.read_table('objectMatrixTest.txt', sep=',', header=None)

xt = data[list(range(1, 19))]

xt = pd.DataFrame(scale(xt))

yt = mlp.predict_proba(xt)

# вывод

lab = data[0]

with open("SubmissionNN.csv", "w") as f:
    f.write('Id,Prediction1,Prediction2,Prediction3,Prediction4,Prediction5,Prediction6,Prediction7,Prediction8,Prediction9\n')
    for i in range(0, yt.shape[0]):
        f.write(lab[i])
        f.write(",")
        for j in range(0, 8):
            f.write(str(yt[i][j]))
            f.write(",")
        f.write(str(yt[i][8]))
        f.write("\n")

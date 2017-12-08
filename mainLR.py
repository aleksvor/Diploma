# -*- coding: utf-8 -*-
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
from sklearn import model_selection
from sklearn.preprocessing import scale
from sklearn.linear_model import LogisticRegression

data = pd.read_table('objectMatrix.txt', sep=',', header=None)

y = data[18]
x = data[list(range(0, 18))]

x = pd.DataFrame(scale(x))

# кросс-валидация
kf = model_selection.KFold(n_splits=5, shuffle=True, random_state=42)

resc = pd.Series([])
resScore = pd.Series([])

for c in range(1000, 2001):

    lr = LogisticRegression(penalty='l2', C=c, solver='newton-cg', multi_class='multinomial', n_jobs=-1)

    scores = model_selection.cross_val_score(lr, x, y, cv=kf, scoring='neg_log_loss')

    resc[c-999] = c
    resScore[c-999] = np.mean(scores)


plt.plot(resc, resScore)
plt.savefig('plot.png', format='png')
plt.show()

lr.fit(x, y)

data = pd.read_table('objectMatrixTest.txt', sep=',', header = None)

xt = data[list(range(1, 19))]

xt = pd.DataFrame(scale(xt))

yt = lr.predict_proba(xt)

lab = data[0]

with open("SubmissionLR.csv", "w") as f:
    f.write('Id,Prediction1,Prediction2,Prediction3,Prediction4,Prediction5,Prediction6,Prediction7,Prediction8,Prediction9\n')
    for i in range(0, yt.shape[0]):
        f.write(lab[i])
        f.write(",")
        for j in range(0, 8):
            f.write(str(yt[i][j]))
            f.write(",")
        f.write(str(yt[i][8]))
        f.write("\n")

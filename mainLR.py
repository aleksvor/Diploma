# -*- coding: utf-8 -*-
import numpy as np
import pandas as pd
from sklearn import model_selection
from sklearn.preprocessing import scale
from sklearn.linear_model import LogisticRegression

data = pd.read_table('objectMatrix.txt', sep=',', header=None)

y = data[18]
x = data[list(range(0, 18))]

x = pd.DataFrame(scale(x))

# кросс-валидация и поиск наилучшего k
kf = model_selection.KFold(n_splits=5, shuffle=True, random_state=42)

c = 1500.0

lr = LogisticRegression(penalty='l2', C=c, solver='newton-cg', multi_class='multinomial', n_jobs=-1)

scores = model_selection.cross_val_score(lr, x, y, cv=kf, scoring='neg_log_loss')

print(np.mean(scores))
print(c)

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

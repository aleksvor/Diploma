# -*- coding: utf-8 -*-
import numpy as np
import pandas
from sklearn import cross_validation
from sklearn.neighbors import KNeighborsClassifier
from sklearn.tree import DecisionTreeClassifier
from sklearn.preprocessing import scale
from sklearn.linear_model import LogisticRegression

data = pandas.read_table('D:\Microsoft Malware Classification Challenge\objectMatrix.txt', sep=',', header = None)

y = data[15]
x = data[list(range(0, 15))]

x = pandas.DataFrame(scale(x))

# кросс-валидация и поиск наилучшего k
kf = cross_validation.KFold(data.shape[0], n_folds = 5, shuffle = True, random_state = 42)

#result = pandas.Series([])
c = 500.0
'''
while True:
'''
lr = LogisticRegression(penalty = 'l2', C = c, solver = 'newton-cg', multi_class = 'multinomial')

scores = cross_validation.cross_val_score(lr, x, y, cv = kf, scoring = 'log_loss')

for i in range(0, scores.shape[0]):  
  scores[i] = abs(scores[i])

res = np.mean(scores)

print(res)
'''
  if res <= 0.3:
      break;
  else:
      c = c + 1.0;

print(np.mean(scores))
print(c)

for k in range(1,101):
    neigh = KNeighborsClassifier(n_neighbors = k, weights = 'distance', n_jobs = -1)
    scores = cross_validation.cross_val_score(neigh, x, y, cv = kf, scoring = 'log_loss')
    for i in range(0, scores.shape[0]):
        scores[i] = abs(scores[i])
    result[k] = np.mean(scores)

best = result.idxmin()
print(best)
print(result[best])

neigh = KNeighborsClassifier(n_neighbors = best, weights = 'distance')
neigh.fit(x, y)

data = pandas.read_table('D:\Microsoft Malware Classification Challenge\objectMatrixTest.txt', sep=',', header = None)

xt = data[list(range(1, 11))]

xt = pandas.DataFrame(scale(xt))

yt = neigh.predict_proba(xt)

lab = data[0]

with open("D:/Microsoft Malware Classification Challenge/Submission.csv", "w") as f:
    f.write('Id,Prediction1,Prediction2,Prediction3,Prediction4,Prediction5,Prediction6,Prediction7,Prediction8,Prediction9\n')
    for i in range(0, yt.shape[0]):
        f.write(lab[i])
        f.write(",")
        for j in range(0, 8):
            f.write(str(yt[i][j]))
            f.write(",")
        f.write(str(yt[i][8]))
        f.write("\n")
'''
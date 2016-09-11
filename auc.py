import sys
import sklearn
from sklearn.metrics import roc_curve, auc

label = []
pctr = []
f = open('ctr.txt', 'r')
for line in f:
    br = line.strip().split('\t')
    l = int(br[2])
    label.append(l)
    ctr = float(br[0])
    pctr.append(ctr)

f.close()

fpr, tpr, _ = roc_curve(label, pctr)
roc_auc = auc(fpr, tpr)
print roc_auc


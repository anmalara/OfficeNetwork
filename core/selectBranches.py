from root_numpy import root2array
from root_numpy import rec2array
import numpy as np
import time
import os.path
import sys

from Utils import *


def selectBranches(file_name, tree_name, obj, selection_cuts, output_name):
  array = root2array(filenames=file_name, treename=tree_name, branches=branch_name_list[obj], selection=selection_cuts)
  # it needs 2 steps to a proper coversion into numpy.ndarray
  array = rec2array(array)
  firstEvent = True
  print "start loop "
  for i in range(0,len(array)):
    firstBranch = True
    for j in range(0,len(branch_name_list[obj])):
      try:
        temp = array[i,j].reshape(1,array[i,j].shape[0])
      except:
        # this is happening if a single values instead of an array (even if empty) is stored
        temp = np.zeros((1, int(array[i,branch_name_list[obj].index(obj+"N")])))
        try:
          temp[0,0] = array[i,j]
        except:
          # this is if the numbers of objects is equal to 0: i.e. objects do not exist
          pass
      if temp.shape[1] < 1: temp = np.zeros((1, nobjects[obj]))
      # convert from float32 to float64
      temp = temp.astype(float)
      if firstBranch:
        event = temp
        firstBranch = False
      else:
        event = np.concatenate((event, temp))
    # event.shape = (len(branch_name_list[obj]), array[i,j].shape[0])
    if event.shape[1] >= nobjects[obj]:
      event = event[:,:nobjects[obj]]
    else:
      event = np.hstack((event, np.zeros((len(branch_name_list[obj]),nobjects[obj]-event.shape[1]))))
    event = event.reshape(1, len(branch_name_list[obj]), nobjects[obj])
    if firstEvent:
      allevents = event
      firstEvent = False
    else:
      allevents = np.concatenate((allevents, event))
    # print allevents.shape
    if i%50000 == 0:
      print "At ", float(i)/len(array)*100, "%"
    if i%50000 == 0 and i > 0:
      postfix = str(i/50000).zfill(4)
      np.save(output_name+"_"+postfix+".npy", allevents)
      del allevents
      firstEvent = True
  # return numpy.ndarray whose shape is (n_events,branches.size(), nobjects[obj])



path = "/nfs/dust/cms/user/amalara/OfficeNetwork/nTuples/"
tree_name = "AnalysisTree"

output_path = "/nfs/dust/cms/user/amalara/OfficeNetwork/preSelectedVariables/"
if not os.path.exists(output_path):
    os.makedirs(output_path)


processes = []
processes.append("QCDPt15to30")
processes.append("QCDPt30to50")
processes.append("QCDPt50to80")
processes.append("QCDPt80to120")
processes.append("QCDPt120to170")
processes.append("QCDPt170to300")
processes.append("QCDPt300to470")
processes.append("QCDPt470to600")
processes.append("QCDPt600to800")
processes.append("QCDPt800to1000")
processes.append("QCDPt1000to1400")
processes.append("QCDPt1400to1800")
processes.append("QCDPt1800to2400")
processes.append("QCDPt2400to3200")
processes.append("QCDPt3200toInf")
processes.append("DY1JetsToLL")
processes.append("DY2JetsToLL")
processes.append("DY3JetsToLL")
processes.append("DY4JetsToLL")

from Utils import *

for obj in branch_name_list:
  for bkg in processes:
    print obj, bkg
    output_name = output_path+bkg+"_"+branch_outputnames[obj]
    file_name = path+"uhh2.AnalysisModuleRunner.MC."+bkg+".root"
    output = selectBranches(file_name, tree_name, obj, selection_cuts, output_name)

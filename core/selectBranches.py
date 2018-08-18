from root_numpy import root2array
from root_numpy import rec2array
import numpy as np
import time
import os.path
import sys

from Utils import *



def selectBranches(file_name, tree_name, branch_names, selection_cuts):
    array = root2array(filenames=file_name, treename=tree_name, branches=branch_names, selection=selection_cuts)
    # it needs 2 steps to a proper coversion into numpy.ndarray
    array = rec2array(array)
    for i in range(0,len(array)):
        for j in range(0,len(branch_names)):
            try:
                temp = array[i,j].reshape(1,array[i,j].shape[0])
            except:
                temp = np.zeros((1, int(array[i,branch_names.index(objects[branch_name_list.index(branch_names)]+"N")])))
                try:
                    temp[0,0] = array[i,j]
                except:
                    pass
            if temp.shape[1] < 1:
                temp = np.zeros((1, njets))
            # convert from float32 to float64
            temp = temp.astype(float)
            if j == 0:
                event = temp
            else:
                event = np.concatenate((event, temp))
        # event.shape = (len(branch_names), array[i,j].shape[0])
        if event.shape[1] >= njets:
            event = event[:,:njets]
        else:
            event = np.hstack((event, np.zeros((len(branch_names),njets-event.shape[1]))))
        event = event.reshape(1, len(branch_names), njets)
        if i==0:
            allevents = event
        else:
            allevents = np.concatenate((allevents, event))
    # return numpy.ndarray whose shape is (n_events,branches.size(), njets)
    print allevents.shape
    return allevents


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

for index, branch_names in enumerate(branch_name_list):
    for bkg in processes:
        output_name = output_path+bkg+"_"+branch_outputnames[objects[index]]+".npy"
        print output_name
        file_name = path+"uhh2.AnalysisModuleRunner.MC."+bkg+".root"
        output = selectBranches(file_name, tree_name, branch_names, selection_cuts)
        np.save(output_name, output)

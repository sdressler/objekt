#!/usr/bin/python

import os, time, subprocess
import progressbar

### USER VARIABLES BELOW ###

llvm   = '/opt/llvm/3.2/'
lib    = 'LLVMGraph.so'
switch = "-LLVMGraph"

cases = [
    'examples/synthetic/vector',
    'examples/synthetic/strvec',
    'examples/synthetic/nestvec',
    'examples/synthetic/neststrvec',
    'examples/synthetic/map',
    'examples/tf'
]

### DO NOT TOUCH FROM HERE ###

llvm_opt    = llvm + '/bin/opt'
base_path   = os.getcwd()
plugin_path = os.path.join(base_path, lib)

#bar = progressbar.ProgressBar(
#          maxval=len(cases) * 4,
#          widgets=[
#              progressbar.Bar('=', '[', ']'),
#              ' ',
#              progressbar.Percentage()
#          ]
#      )

text = ""

class Label():
    def update(self, pbar):
        return text
    
widgets=[
    Label(),
    ' ',         
    progressbar.Percentage(),
    ' ',
    progressbar.Bar('=', '[', ']'),
    ' '
]

progress = progressbar.ProgressBar(maxval=(len(cases)), widgets=widgets).start()

N = 0
for case in cases:
    #text = case
    #print "Running: " + case
    case_text = case.split('/')[-1]
    text = case_text
    
    os.chdir(os.path.join(base_path, case))
    
    # Cleanup step
    p = subprocess.Popen(["make", "clean"], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    p.wait()
    
    # Build step
    p = subprocess.Popen(["make"], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    p.wait()
    
    if p.returncode != 0:
        #N += 1
        #progress.update(N)
        #print " Build OK.",
        
    #else:
        print "Build Failed. Output below."
        print p.stderr.read()
        exit()
    
    # llvm/opt step
    for file in os.listdir("."):
        if file.endswith(".bc"):
            with open(os.path.splitext(file)[0] + '.tf.bc', "wb") as out:
                p = subprocess.Popen(
                    [llvm_opt, "-load", plugin_path, switch, file],
                    stdout=out, stderr=subprocess.PIPE
                )
                p.wait()
                
                if p.returncode != 0:
                    #N += 1
                    #progress.update(N)
                    #print " Pass OK.",
                    
                #else:
                    print " Pass Failed. Output below."
                    print p.stderr.read()
                    exit()
                    
    # Rebuild step
    p = subprocess.Popen(["make", "tf"], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    p.wait()
    
    if p.returncode != 0:
        #N += 1
        #progress.update(N)
        #print " Rebuild OK.",
    #   
    #else:
        print " Rebuild Failed. Output below."
        print p.stderr.read()
        exit()
        
    # Run step
    for file in os.listdir("."):
        if file.endswith(".x"):
            p = subprocess.Popen(os.path.join(os.getcwd(), file), stderr=subprocess.PIPE)
            p.wait()
            
#            if p.returncode == 0:
#                
#                est = None
#                meas = None
#                
#                for line in iter(p.stdout.readline,''):
#                    
#                    try:
#                        key   = line.split(' ')[0]
#                        value = line.split(' ')[1]
#                        
#                        if key == "ESTIMATED:":
#                            #print value + " Byte"
#                            est = int(value)
#                            
#                        if key == "MEASURED:":
#                            #print value + " Byte"
#                            meas = int(value)
#                            
#                    except IndexError:
#                        continue
#                    
#                if est != None and meas != None:
                    #print est - meas
#                    if (est - meas) == 0:
#                        text = case_text + "  [OK] "
#                        
#                    else:
#                        text = case_text + " [FAIL]"
#                            
#                else:
#                    text = case_text + " [FAIL]"
#                
#                print ""
                
            N += 1
            progress.update(N)
            
            
#            else:
#                print " Run Failed. Output below."
#                print p.stderr.read()
#                exit()
        
#print "Done."

progress.finish()
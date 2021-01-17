# -*- coding: utf-8 -*-
"""
Created on Sat Sep  5 17:09:42 2020

@author: Nebojsa.Vojinovic
"""

from dann5.d5o import Qvar, Qequation, Qint
from dimod import ExactSolver
from dwave.system import DWaveSampler, EmbeddingComposite
from dwave.cloud.exceptions import SolverNotFoundError
from dwave.system.samplers import LeapHybridSampler

a = Qvar(3, "a")
b = Qvar(3, "b")
c = Qvar(2, "c")
d = Qvar(2, "d")
r_addAbcd = Qequation(Qvar("R", 15))
r_addAbcd.assign( a + b)# + c + d )

print(r_addAbcd.toString(False))
print(r_addAbcd.toString(True))

qT = r_addAbcd.qubo(False)
print("Vectors")
print(qT)

Q = r_addAbcd.qubo(True)

print(Q)


try:
    qpu_advantage = DWaveSampler(solver={'topology__type': 'pegasus', 'qpu': True})
    qpu_2000q = DWaveSampler(solver={'topology__type': 'chimera', 'qpu': True})
    
    qpus = {'Advantage': qpu_advantage, 'DW-2000Q': qpu_2000q}

    print("Connected to Advantage {} and 2000Q {}.".format(qpu_advantage.solver.id, qpu_2000q.solver.id))
except SolverNotFoundError:
    print("Currently a pair of solvers are unavailable for sections comparing QPU technologies. Try those examples later.")

    
# EmbeddingComposite maps problem elementss to a structured Chimera sampler node adresses
embedingSampler = ExactSolver()                   # local
#embedingSampler = EmbeddingComposite(qpu_advantage) 
#embedingSampler = EmbeddingComposite(qpu_2000q) 
#embedingSampler = EmbeddingComposite(DWaveSampler())   # DW_2000Q_6
#embedingSampler = LeapHybridSampler()

# Return num_reads solutions (responses are in the D-Wave's graph of indexed qubits)
kwargs = {}
if 'num_reads' in embedingSampler.parameters:
    kwargs['num_reads'] = 100
if 'answer_mode' in embedingSampler.parameters:
    kwargs['answer_mode'] = 'histogram'
if 'chain_strength' in embedingSampler.parameters:
    # strength 10 for R 16, 15, 6, works for both, pegasus and chimera
    kwargs['chain_strength'] = 6   

sampleset = embedingSampler.sample_qubo(Q, **kwargs)
#sampleset = embedingSampler.sample_qubo(Q)
#sampleset = embedingSampler.sample_qubo(Q, num_reads=5000)

samples = [dict(sample) for sample in sampleset.lowest().samples()]
r_addAbcd.set(samples)
print(r_addAbcd.solutions())
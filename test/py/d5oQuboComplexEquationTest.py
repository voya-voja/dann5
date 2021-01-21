# -*- coding: utf-8 -*-
"""
Created on Wed Jan 20 20:23:19 2021

@author: Nebojsa.Vojinovic
"""

from dann5.d5o import Qvar, Qequation
from dimod import ExactSolver

from dwave.system import DWaveSampler, EmbeddingComposite
from dwave.cloud.exceptions import SolverNotFoundError
from dwave.system.samplers import LeapHybridSampler

k = Qvar(3, 'k')
l = Qvar(2, 'l')
m = Qvar(3, 'n')
_2 = Qvar("2", 2)    # constant
eZ = Qequation(Qvar('Z', 13))
eZ.assign(_2 * k + l * m)

print(eZ.toString(False, -1))

Q = eZ.qubo(True, -1)

try:
    qpu_advantage = DWaveSampler(solver={'topology__type': 'pegasus', 'qpu': True})
    qpu_2000q = DWaveSampler(solver={'topology__type': 'chimera', 'qpu': True})
    
    qpus = {'Advantage': qpu_advantage, 'DW-2000Q': qpu_2000q}

    print("Connected to Advantage {} and 2000Q {}.".format(qpu_advantage.solver.id, qpu_2000q.solver.id))
except SolverNotFoundError:
    print("Currently a pair of solvers are unavailable for sections comparing QPU technologies. Try those examples later.")

    
# EmbeddingComposite maps problem elementss to a structured Chimera sampler node adresses
#embedingSampler = ExactSolver()                   # local
embedingSampler = EmbeddingComposite(qpu_advantage) 
#embedingSampler = EmbeddingComposite(qpu_2000q) 
#embedingSampler = EmbeddingComposite(DWaveSampler())   # DW_2000Q_6
#embedingSampler = LeapHybridSampler()

# Return num_reads solutions (responses are in the D-Wave's graph of indexed qubits)
kwargs = {}
if 'num_reads' in embedingSampler.parameters:
    kwargs['num_reads'] = 5000
if 'answer_mode' in embedingSampler.parameters:
    kwargs['answer_mode'] = 'histogram'
if 'chain_strength' in embedingSampler.parameters:
    # strength 5 for R 16, 15, 6, 12 works for both, pegasus and chimera
    kwargs['chain_strength'] = 5   

sampleset = embedingSampler.sample_qubo(Q, **kwargs)
#sampleset = embedingSampler.sample_qubo(Q)
#sampleset = embedingSampler.sample_qubo(Q, num_reads=5000)

samples = [dict(sample) for sample in sampleset.lowest().samples()]
eZ.set(samples)
print(eZ.solutions())
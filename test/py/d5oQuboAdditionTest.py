# -*- coding: utf-8 -*-
"""
Created on Sat Sep  5 17:09:42 2020

@author: Nebojsa.Vojinovic
"""

from lib.d5o import Qvar, Qequation, Qint
from dimod import ExactSolver
from dwave.system import DWaveSampler, EmbeddingComposite
from dwave.cloud.exceptions import SolverNotFoundError
from dwave.system.samplers import LeapHybridSampler

a = Qvar(3, "a")
b = Qvar(3, "b")
c = Qvar(3, "c")
d = Qvar(3, "d")
r_addAbcd = Qequation(Qvar("R", 15))
r_addAbcd.assign( a + b + c + d )

print(r_addAbcd.toString(True))

d = r_addAbcd.bqm(False).dictionary()
print("Vectors")
print(d)

Q = r_addAbcd.bqm(True).dictionary()

print(Q)


try:
    qpu_advantage = DWaveSampler(solver={'topology__type': 'pegasus', 'qpu': True})
    qpu_2000q = DWaveSampler(solver={'topology__type': 'chimera', 'qpu': True})
    
    qpus = {'Advantage': qpu_advantage, 'DW-2000Q': qpu_2000q}

    print("Connected to Advantage {} and 2000Q {}.".format(qpu_advantage.solver.id, qpu_2000q.solver.id))
except SolverNotFoundError:
    print("Currently a pair of solvers are unavailable for sections comparing QPU technologies. Try those examples later.")

    
# EmbeddingComposite maps problem elementss to a structured Chimera sampler node adresses
#embedingSampler = ExactSolver()                   # local
#embedingSampler = EmbeddingComposite(qpu_advantage) 
#embedingSampler = EmbeddingComposite(qpu_2000q) 
embedingSampler = EmbeddingComposite(DWaveSampler())   # DW_2000Q_6
#embedingSampler = LeapHybridSampler()

# Return num_reads solutions (responses are in the D-Wave's graph of indexed qubits)
kwargs = {}
if 'num_reads' in embedingSampler.parameters:
    kwargs['num_reads'] = 100
if 'answer_mode' in embedingSampler.parameters:
    kwargs['answer_mode'] = 'histogram'
if 'chain_strength' in embedingSampler.parameters:
    # strength 10 for R 16, 15, 6, works for both, pegasus and chimera
    kwargs['chain_strength'] = 10   

sampleset = embedingSampler.sample_qubo(Q, **kwargs)
#sampleset = embedingSampler.sample_qubo(Q)
#sampleset = embedingSampler.sample_qubo(Q, num_reads=5000)


print('   a  b  c  d\n')
i = 0
ai = Qint(3)
bi = Qint(3)
ci = Qint(3)
di = Qint(3)
ci.push(0).push(0).push(0)
di.push(0).push(0).push(0)
for sample in sampleset.lowest().samples():
    i = i + 1
    ai.push(sample['a0']).push(sample['a1']).push(sample['a2'])
    bi.push(sample['b0']).push(sample['b1']).push(sample['b2'])
    ci.push(sample['c0']).push(sample['c1']).push(sample['c2'])
    di.push(sample['d0']).push(sample['d1']).push(sample['d2'])
    print('{}. {} {} {} {}'.format(i, ai.value(), bi.value(), ci.value(), di.value()))
#print(sampleset)


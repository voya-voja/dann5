# -*- coding: utf-8 -*-
"""
Created on Mon Jul 27 14:06:44 2020

@author: Nebojsa.Vojinovic
"""
#from dwave.system import DWaveSampler, EmbeddingComposite
import dimod
import dann5.d5o as d5o
 
print("\nEQ gate")
eqQ = d5o.EqQT()
Q = eqQ.qubo()
print(Q)
sampler = dimod.ExactSolver()                   # local
sampleset = sampler.sample_qubo(Q)
#sampler = EmbeddingComposite(DWaveSampler())   # DW_2000Q_6
#sampleset = sampler.sample_qubo(Q, num_reads=5000)   # DW_2000Q_6
print(sampleset)

print("\nNOT gate")
neQ = d5o.NotQT()
Q = neQ.qubo()
print(Q)
sampler = dimod.ExactSolver()                   # local
sampleset = sampler.sample_qubo(Q)
#sampler = EmbeddingComposite(DWaveSampler())   # DW_2000Q_6
#sampleset = sampler.sample_qubo(Q, num_reads=5000)   # DW_2000Q_6
print(sampleset)

print("\nLT gate")
ltQ = d5o.LtQT()
Q = ltQ.qubo()
print(Q)
sampler = dimod.ExactSolver()                   # local
sampleset = sampler.sample_qubo(Q)
#sampler = EmbeddingComposite(DWaveSampler())   # DW_2000Q_6
#sampleset = sampler.sample_qubo(Q, num_reads=5000)   # DW_2000Q_6
print(sampleset)

print("\nLE gate")
leQ = d5o.LeQT()
Q = leQ.qubo()
print(Q)
sampler = dimod.ExactSolver()                   # local
sampleset = sampler.sample_qubo(Q)
#sampler = EmbeddingComposite(DWaveSampler())   # DW_2000Q_6
#sampleset = sampler.sample_qubo(Q, num_reads=5000)   # DW_2000Q_6
print(sampleset)

print("\nGT gate")
gtQ = d5o.GtQT()
Q = gtQ.qubo()
print(Q)
sampler = dimod.ExactSolver()                   # local
sampleset = sampler.sample_qubo(Q)
#sampler = EmbeddingComposite(DWaveSampler())   # DW_2000Q_6
#sampleset = sampler.sample_qubo(Q, num_reads=5000)   # DW_2000Q_6
print(sampleset)

print("\nGE gate")
geQ = d5o.GeQT()
Q = geQ.qubo()
print(Q)
sampler = dimod.ExactSolver()                   # local
sampleset = sampler.sample_qubo(Q)
#sampler = EmbeddingComposite(DWaveSampler())   # DW_2000Q_6
#sampleset = sampler.sample_qubo(Q, num_reads=5000)   # DW_2000Q_6
print(sampleset)

print("\nAND gate")
andQ = d5o.AndQubo()
Q = andQ.bqm().dictionary()
print(Q)
sampler = dimod.ExactSolver()                   # local
sampleset = sampler.sample_qubo(Q)
print(sampleset.lowest())

print("\nN-AND gate")
nandQ = d5o.NandQubo()
Q = nandQ.bqm().dictionary()
print(Q)
sampler = dimod.ExactSolver()                   # local
sampleset = sampler.sample_qubo(Q)
print(sampleset.lowest())

print("\nOR gate")
orQ = d5o.OrQubo()
Q = orQ.bqm().dictionary()
print(Q)
sampler = dimod.ExactSolver()                   # local
sampleset = sampler.sample_qubo(Q)
print(sampleset.lowest())

print("\nN-OR gate")
norQ = d5o.NorQubo()
Q = norQ.bqm().dictionary()
print(Q)
sampler = dimod.ExactSolver()                   # local
sampleset = sampler.sample_qubo(Q)
print(sampleset.lowest())

print("\nNOT(left) OR right gate")
nLorRQ = d5o.NotLeftOrRightQubo()
Q = nLorRQ.bqm().dictionary()
print(Q)
sampler = dimod.ExactSolver()                   # local
sampleset = sampler.sample_qubo(Q)
print(sampleset.lowest())

print("\nDWave NOT(left) OR right gate")
dWnLorRQ = d5o.DwNotLeftOrRightQubo()
Q = dWnLorRQ.bqm().dictionary()
print(Q)
sampler = dimod.ExactSolver()                   # local
sampleset = sampler.sample_qubo(Q)
print(sampleset.lowest())

print("\nNXOR gate")
nxorQ = d5o.NxorQubo()
Q = nxorQ.bqm().dictionary()
print(Q)
sampler = dimod.ExactSolver()                   # local
sampleset = sampler.sample_qubo(Q)
print(sampleset.lowest())

print("\nXOR gate")
xorQ = d5o.XorQubo()
Q = xorQ.bqm().dictionary()
print(Q)
sampler = dimod.ExactSolver()                   # local
sampleset = sampler.sample_qubo(Q)
print(sampleset.lowest())

print("\nH-ADDER gate")
hAdder = d5o.HalfAdderQubo()
Q = hAdder.bqm().dictionary()
print(Q)
sampler = dimod.ExactSolver()                   # local
sampleset = sampler.sample_qubo(Q)
print(sampleset.lowest())

print("\nF-ADDER gate")
fAdder = d5o.FullAdderQubo()
Q = fAdder.bqm().dictionary()
print(Q)
sampler = dimod.ExactSolver()                   # local
sampleset = sampler.sample_qubo(Q)
print(sampleset.lowest())
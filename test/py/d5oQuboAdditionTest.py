# -*- coding: utf-8 -*-
"""
Created on Sat Sep  5 17:09:42 2020

@author: Nebojsa.Vojinovic
"""

import dimod
import lib.d5o as d5o

import dwavebinarycsp as dbc
from helpers import embedding, solvers, convert

a = d5o.Qdef(3, "a")
b = d5o.Qdef(3, "b")
c = d5o.Qdef(3, "c")
addAbc = a + b  + c

vR = d5o.Qvar("R", 6)
r_addAbc = d5o.Qequation(vR, addAbc)
r_addAbc.push(d5o.Qvar(a)).push(d5o.Qvar(b)).push(d5o.Qvar(c))

d = r_addAbc.bqm(False).dictionary()
print("Vectors")
print(d)

Q = r_addAbc.bqm(True).dictionary()

print(Q)

sampler = dimod.ExactSolver()                   # local
sampleset = sampler.sample_qubo(Q)
#sampler = EmbeddingComposite(DWaveSampler())   # DW_2000Q_6
#sampleset = sampler.sample_qubo(Q, num_reads=5000)   # DW_2000Q_6
print('a0 = ', sampleset.lowest().samples()[0]['a0'])
print('a1 = ', sampleset.lowest().samples()[0]['a1'])
print('a2 = ', sampleset.lowest().samples()[0]['a2'])
#print('a3 = ', sampleset.lowest().samples()[0]['a3'])
print('b0 = ', sampleset.lowest().samples()[0]['b0'])
print('b1 = ', sampleset.lowest().samples()[0]['b1'])
print('b2 = ', sampleset.lowest().samples()[0]['b2'])
#print('b3 = ', sampleset.lowest().samples()[0]['b3'])

print(sampleset.lowest())

print('a0 = ', sampleset.lowest().samples()[1]['a0'])
print('a1 = ', sampleset.lowest().samples()[1]['a1'])
print('a2 = ', sampleset.lowest().samples()[1]['a2'])
#print('a3 = ', sampleset.lowest().samples()[1]['a3'])
print('b0 = ', sampleset.lowest().samples()[1]['b0'])
print('b1 = ', sampleset.lowest().samples()[1]['b1'])
print('b2 = ', sampleset.lowest().samples()[1]['b2'])
#print('b3 = ', sampleset.lowest().samples()[1]['b3'])

#print(sampleset)
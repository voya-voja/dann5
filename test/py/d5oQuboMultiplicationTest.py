# -*- coding: utf-8 -*-
"""
Created on Sat Sep  5 17:09:42 2020

@author: Nebojsa.Vojinovic
"""

import dimod
import lib.d5o as d5o

import dwavebinarycsp as dbc
from helpers import embedding, solvers, convert

sv_a = d5o.Qdef(3, "a")
sv_b = d5o.Qdef(3, "b")
ev_r = sv_a * sv_b

qv_p = d5o.Qvar("p", 21)
qe = d5o.Qequation(qv_p, ev_r)
qe.push(d5o.QuboVector(sv_a)).push(d5o.QuboVector(sv_b))

d = qe.bqm(False).dictionary()
print("Vectors")
print(d)

csp = dbc.factories.multiplication_circuit(3)
print("\nCSP")
print(csp.constraints)
# Convert the CSP into BQM bqm
bqm = dbc.stitch(csp, min_classical_gap=.1)

convert.adjustBQM(bqm)

print("\nBQM")
print(bqm)

Q = qe.bqm(True).dictionary()

print(Q)

sampler = dimod.ExactSolver()                   # local
sampleset = sampler.sample_qubo(Q)
#sampler = EmbeddingComposite(DWaveSampler())   # DW_2000Q_6
#sampleset = sampler.sample_qubo(Q, num_reads=5000)   # DW_2000Q_6
print('a0 = ', sampleset.lowest().samples()[0]['a0'])
print('a1 = ', sampleset.lowest().samples()[0]['a1'])
print('a2 = ', sampleset.lowest().samples()[0]['a2'])
print('b0 = ', sampleset.lowest().samples()[0]['b0'])
print('b1 = ', sampleset.lowest().samples()[0]['b1'])
print('b2 = ', sampleset.lowest().samples()[0]['b2'])

print(sampleset.lowest())

print('a0 = ', sampleset.lowest().samples()[1]['a0'])
print('a1 = ', sampleset.lowest().samples()[1]['a1'])
print('a2 = ', sampleset.lowest().samples()[1]['a2'])
print('b0 = ', sampleset.lowest().samples()[1]['b0'])
print('b1 = ', sampleset.lowest().samples()[1]['b1'])
print('b2 = ', sampleset.lowest().samples()[1]['b2'])

# -*- coding: utf-8 -*-
"""
Created on Mon Sep  7 14:36:28 2020

@author: Nebojsa.Vojinovic
"""


import penaltymodel.core as pm
from dwave.system import DWaveSampler, EmbeddingComposite

import dwavebinarycsp as dbc
import dwavebinarycsp.factories.constraint.gates as gates
import operator

import dimod
import networkx as nx
import matplotlib.pyplot as ploter

from helpers import embedding, solvers, convert

import lib.d5o as d5o

print('\nEQ - NE operators:')
csp = dbc.ConstraintSatisfactionProblem(dbc.BINARY)
csp.add_constraint(operator.ne, ['a', 'na'])  # a != na
csp.add_constraint(operator.ne, ['b', 'nb'])  # b != nb
csp.add_constraint(gates.or_gate(['a', 'b', 'x1']))  # a or b
csp.add_constraint(gates.or_gate(['na', 'nb', 'x2']))  # na or nb
csp.add_constraint(gates.and_gate(['x1', 'x2', 'r']))  # na or nb
bqm = dbc.stitch(csp)
convert.adjustBQM(bqm)
sampler = dimod.ExactSolver()                   # local
sampleset = sampler.sample(bqm)
#sampler = EmbeddingComposite(DWaveSampler())   # DW_2000Q_6
#sampleset = sampler.sample_qubo(Q, num_reads=5000)   # DW_2000Q_6
print(sampleset)        # doctest: +SKIP

print("\nAnd gate")
csp1 = dbc.ConstraintSatisfactionProblem(dbc.BINARY)
csp1.add_constraint(gates.and_gate(['a', 'b', 'r']))  # a or b
bqm = dbc.stitch(csp1)
convert.adjustBQM(bqm)

andQ = d5o.AndQubo()
Q = andQ.bqm().dictionary()
print(Q)


print("\nOrgate")
csp1 = dbc.ConstraintSatisfactionProblem(dbc.BINARY)
csp1.add_constraint(gates.or_gate(['a', 'b', 'r']))  # a or b
bqm = dbc.stitch(csp1)
convert.adjustBQM(bqm)

orQ = d5o.OrQubo()
Q = orQ.bqm().dictionary()
print(Q)

print("\nXor gate")
csp1 = dbc.ConstraintSatisfactionProblem(dbc.BINARY)
csp1.add_constraint(gates.xor_gate(['a', 'b', 'r']))  # a or b
bqm = dbc.stitch(csp1)
convert.adjustBQM(bqm)

sampler = dimod.ExactSolver()                   # local
sampleset = sampler.sample(bqm)
#sampler = EmbeddingComposite(DWaveSampler())   # DW_2000Q_6
#sampleset = sampler.sample_qubo(Q, num_reads=5000)   # DW_2000Q_6
print(sampleset.lowest())        # doctest: +SKIP

xorQ = d5o.XorQubo()
Q = xorQ.bqm().dictionary()
print(Q)
sampler = dimod.ExactSolver()                   # local
sampleset = sampler.sample_qubo(Q)
print(sampleset.lowest())

print("\nH-aader gate")
csp1 = dbc.ConstraintSatisfactionProblem(dbc.BINARY)
csp1.add_constraint(gates.halfadder_gate(['a', 'b', 'r', 'x']))  # a or b
bqm = dbc.stitch(csp1)
convert.adjustBQM(bqm)

faQ = d5o.HalfAdderQubo()
Q = faQ.bqm().dictionary()
print(Q)


print("\nF-aader gate")
csp1 = dbc.ConstraintSatisfactionProblem(dbc.BINARY)
csp1.add_constraint(gates.fulladder_gate(['a', 'b', 'c', 'r', 'x']))  # a or b
bqm = dbc.stitch(csp1)
convert.adjustBQM(bqm)

faQ = d5o.FullAdderQubo()
Q = faQ.bqm().dictionary()
print(Q)

sampler = dimod.ExactSolver()                   # local
sampleset = sampler.sample(bqm)
#sampler = EmbeddingComposite(DWaveSampler())   # DW_2000Q_6
#sampleset = sampler.sample_qubo(Q, num_reads=5000)   # DW_2000Q_6
print(sampleset.lowest())        # doctest: +SKIP
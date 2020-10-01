#    Copyright 2018 D-Wave Systems Inc.

def to_base_ten(sample):
    a = b = 0
    
    # we know that multiplication_circuit() has created these variables
    a_vars = ['a0', 'a1', 'a2']
    b_vars = ['b0', 'b1', 'b2']
    
    for lbl in reversed(a_vars):
        a = (a << 1) | sample[lbl]
    for lbl in reversed(b_vars):
        b = (b << 1) | sample[lbl] 
        
    return a,b


def real_to_base_ten(sample):
    a = b = 0
    
    # we know that multiplication_circuit() has created these variables
    a_vars = ['a0', 'a1', 'a2']
    b_vars = ['b0', 'b1', 'b2']
    
    for lbl in reversed(a_vars):
        a = (a << 1) | int(sample[lbl])
    for lbl in reversed(b_vars):
        b = (b << 1) | int(sample[lbl])
        
    return a,b
def prime_to_base_ten(sample):
    a = p = 0
    
    # we know that multiplication_circuit() has created these variables
    a_vars = ['a0', 'a1', 'a2']
    p_vars = ['p0', 'p1', 'p2', 'p3', 'p4', 'p5']
    
    for lbl in reversed(a_vars):
        a = (a << 1) | sample[lbl]
    for lpl in reversed(p_vars):
        p = (p << 1) | sample[lpl] 
        
    return a,p

from collections import OrderedDict

# Function for converting the response to a dict of integer values
def response_to_dict(response):
    results_dict = OrderedDict()
    accum_res_dict = OrderedDict()
    count_dict = OrderedDict()
    avrg_rests_dict = OrderedDict()
    for sample, energy in response.data(['sample', 'energy']):
        # Convert A and B from binary to decimal
        a, b = to_base_ten(sample)
        # Aggregate results by unique A and B values (ignoring internal circuit variables)
        if (a, b) not in results_dict:
            results_dict[(a, b)] = energy
            accum_res_dict[(a, b)] = energy
            count_dict[(a, b)] = 1
        else:
            accum_res_dict[(a, b)] += energy
            count_dict[(a, b)] += 1
    
    for key in accum_res_dict.keys():
        avrg_rests_dict[key] = accum_res_dict[key] / count_dict[key]
            
    return results_dict, avrg_rests_dict, count_dict

# Function for converting the response to a dict of integer values
def real_response_to_dict(response):
    results_dict = OrderedDict()
    for sample, energy in response.data(['sample', 'energy']):
        # Convert A and B from binary to decimal
        a, b = real_to_base_ten(sample)
        # Aggregate results by unique A and B values (ignoring internal circuit variables)
        if (a, b) not in results_dict:
            results_dict[(a, b)] = energy
            
    return results_dict

def adjustBQM(bqm):
    atBqmLinear = iter(bqm.linear)
    while True:
        variable = next(atBqmLinear, '_bqm_lin_end_')
        if(variable == '_bqm_lin_end_'):
            break
        bqm.linear[variable] = round(bqm.linear[variable], 1)
        print( variable, ":", bqm.linear[variable])
        
    atBqmQuadratic = iter(bqm.quadratic)
    while True:
        variable = next(atBqmQuadratic, '_bqm_quad_end_')
        if(variable == '_bqm_quad_end_'):
            break
        bqm.quadratic[variable] = round(bqm.quadratic[variable], 1)
        print( variable, ":", bqm.quadratic[variable])
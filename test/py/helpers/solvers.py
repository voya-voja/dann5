#    Copyright 2018 D-Wave Systems Inc.

import os
from dwave.cloud import Client
from dwave.cloud.exceptions import *
from IPython.display import Markdown, display

def print_markdown(string):
    display(Markdown(string))

def default_solver():
    with Client.from_config() as client:
        try:
            my_default_solver = client.get_solver(qpu=True).id
            ds = "Solver: " + my_default_solver
            print(ds)
        except SolverNotFoundError:
            my_default_solver = " "
            print_markdown("<span style='color:red;font-weight:bold'>No D-Wave solver found.</span>")
            print("Please check available solvers on the <span style='font-weight:bold'>Leap dashboard</span>.")

    #my_default_token  = os.getenv('DWAVE_API_TOKEN')
    my_default_token  = "DEV-f5b44e6085a17ce89f478c2334f873df4e6b080f"
    if not my_default_token or my_default_token == "None":
       print_markdown("<span style='color:red;font-weight:bold'>No default API token.</span>")
       print("An API token is not set for this environment.")
       print_markdown("You can find your API token on the <span style='font-weight:bold'>Leap dashboard</span>.")
       print("Please uncomment the \"sampler =\" line in the next cell and paste your token there.")
    else:
       dt = "API Token: " + my_default_token[:10] + "***" + my_default_token[-5:]
       print(dt)
    return(my_default_solver, my_default_token)




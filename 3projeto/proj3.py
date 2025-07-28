"""
ASA projeto 3 2023/2024
Grupo: al039
Alunos: Sofia Lopes (106198), Raquel Rodrigues (106322)

Programa que calcula o lucro máximo que se pode obter diariamente da produção e
venda de brinquedos de natal na empresa UbiquityInc.
"""

from pulp import *

firstLine = input().split()
numToys = int(firstLine[0])
numPackages = int(firstLine[1])
maxProduction = int(firstLine[2])

problem = LpProblem("Toys_Problem", LpMaximize)

toys = {}
varsToys = [None]*numToys
toy_packages = {}
for i in range(1, numToys + 1):
    toyInfo = input().split()
    toys[i] = [int(toyInfo[0]), int(toyInfo[1])] # (profit, production capacity) for each toy
    toy_packages[i] = []
    # define toys variables
    varsName = f"T_{i}"
    varsToys[i-1] = LpVariable(varsName, 0, toys[i][1], LpInteger)   

packages = {}
varsPackages = [None]*numPackages
for i in range(1, numPackages + 1):
    packageInfo = input().split()
    # (toy1, toy2, toy3, profit) for each package
    toy1 = int(packageInfo[0])
    toy2 = int(packageInfo[1])
    toy3 = int(packageInfo[2])
    packages[i] = (toy1, toy2, toy3, int(packageInfo[3])) 
    # define packages variables
    varsName = f"P_{i}"
    packageMaxProduction = min(toys[toy1][1], toys[toy2][1], toys[toy3][1])
    vars = LpVariable(varsName, 0, packageMaxProduction, LpInteger)
    varsPackages[i-1] = vars
    # registers the package for each toy it contains 
    toy_packages[toy1].append(vars)
    toy_packages[toy2].append(vars)
    toy_packages[toy3].append(vars)

# Objective function 
problem +=  lpSum(toys[i][0] * varsToys[i-1] for i in range(1, numToys + 1)) + \
            lpSum(packages[p][3] * varsPackages[p-1] for p in range(1, numPackages + 1))

# Constraints for each toy (production capacity)
for i in range(1, numToys +1):
    problem += varsToys[i-1] + lpSum(toy_packages[i]) <= toys[i][1]

# Constraint of maximum capacity for overall production
problem += lpSum(varsToys) + 3*lpSum(varsPackages) <= maxProduction

problem.solve(GLPK(msg=0))

print(int(problem.objective.value()))
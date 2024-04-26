epigenomics_branch_parameter = int(input("epigenomics_branch_parameter : "))

no_of_tasks = 4 * epigenomics_branch_parameter + 4

a = 0
b = 0
c = 0
count = 1

for i in range(1, no_of_tasks + 1):
    # print(no_of_tasks- 3)
    if i == 1:
        a = 1
        b = epigenomics_branch_parameter
        c = no_of_tasks - (a+b)
    elif i <= 3 * epigenomics_branch_parameter + 1:
        a = a + b
        b = 1
        c = no_of_tasks - (a+b)
    elif i > 3 * epigenomics_branch_parameter + 1 and i <= no_of_tasks - 3:
        a =  a
        b = 1
        c = no_of_tasks - (a+b)
    elif i > (no_of_tasks - 3) and i < no_of_tasks:
        a = a +1
        b = 1
        c = no_of_tasks - (a+b)
    else:
        a = 0 
        b = 0
        c = no_of_tasks
    
    print( '0 '* a + '1 ' * b + '0 '* c)
    if i == 3 * epigenomics_branch_parameter + 1:
        a = a + 1
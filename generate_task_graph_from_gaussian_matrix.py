gaussian_matrix_size = int(input("Gaussian matrix size : "))

val = gaussian_matrix_size - 1
no_of_tasks = int((pow(gaussian_matrix_size, 2) + gaussian_matrix_size - 2)/2)

a = 1
b = val
c = no_of_tasks - (a + b)
count = 0

for i in range(1, no_of_tasks + 1):
    if i == 1:
        pass
    elif i == no_of_tasks:
        a = no_of_tasks
        b = 0
        c = 0
    elif count == val + 1 :
        count = 0
        val = val - 1
        b = val
        c = c
        a = no_of_tasks - (b+c)
    else :
        a = a + b 
        b = 1
        c = no_of_tasks -(a+b)
    print('0 '* a + '1 ' * b + '0 '* c)
    count = count + 1
    


    


def main():
    a = 10
    b = 20
    
    swap(a, b)
    print("a is: " + str(a)) 
    print("b is: " + str(b))

def swap(a, b):
    temp = a
    a = b
    b = temp

main()

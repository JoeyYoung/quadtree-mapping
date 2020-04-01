from ctypes import *

add_plus = CDLL('./test.so')
sum = add_plus.add_integer_plus(4,5)
print("sum: " + str(sum))

# 使用float类型变量作为函数参数
a = c_float(5.5)
b = c_float(4.1)
add_float =  add_plus.add_float_plus
add_float.restype = c_float
sum_float = add_plus.add_float_plus(a,b)

print(sum_float)
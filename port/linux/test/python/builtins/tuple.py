t = tuple()
assert type(t) == tuple

l = [1, 2, 3, 4, 5]
t = tuple(l)
assert type(t) == tuple
assert t[0] == 1
assert t[1] == 2
assert t[2] == 3
assert t[3] == 4
assert t[4] == 5

t = tuple('test')
assert type(t) == tuple
assert t[0] == 't'
assert t[1] == 'e'
assert t[2] == 's'
assert t[3] == 't'

assert (1,2,3) == (1,2,3)
assert (1,2,3) != (1,2,4)

assert (1,2,3) + (4,5,6) == (1,2,3,4,5,6)
assert (1,2,3) + (4,5,6) + (7,8,9) == (1,2,3,4,5,6,7,8,9)

assert (1,2,3) + (4,5,6) != (1,2,3,1,2,3)

assert 2 * (1,2,3) == (1,2,3,1,2,3)
assert (1,2,3) * 2 == (1,2,3,1,2,3)
assert 3 * (1,2,3) == (1,2,3,1,2,3,1,2,3)
assert (1,2,3) * 3 == (1,2,3,1,2,3,1,2,3)

print('PASS')

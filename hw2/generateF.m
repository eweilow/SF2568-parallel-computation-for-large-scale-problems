syms x r(x);

u = x*(x-1);

ubis = diff(u, x, 2);

% r = -1 * x - 1

f = ubis - r*u

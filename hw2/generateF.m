clear
clc
syms x r(x);

u = -10*x*(x-1).*(x-0.5);

u


uprim = diff(u, x, 1)
ubis = diff(u, x, 2)

f = ubis + r*u

R = -sin(pi*x)
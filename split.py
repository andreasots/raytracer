#!/usr/bin/env python
# -*- coding: utf-8 -*-
#
#  untitled.py
#  
#  Copyright 2012 Andreas Ots <andreasots@gmail.com>
#  
#  This program is free software; you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation; either version 2 of the License, or
#  (at your option) any later version.
#  
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#  
#  You should have received a copy of the GNU General Public License
#  along with this program; if not, write to the Free Software
#  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
#  MA 02110-1301, USA.
#  
#  

from visual import *

def aabox(bounds):
	curve(pos=[bounds[0], vector(bounds[0].x, bounds[0].y, bounds[1].z),
	vector(bounds[0].x, bounds[1].y, bounds[1].z),
	vector(bounds[0].x, bounds[1].y, bounds[0].z),
	bounds[0], vector(bounds[1].x, bounds[0].y, bounds[0].z),
	vector(bounds[1].x, bounds[0].y, bounds[1].z), bounds[1],
	vector(bounds[1].x, bounds[1].y, bounds[0].z),
	vector(bounds[1].x, bounds[0].y, bounds[0].z)])
	curve(pos=[bounds[1], vector(bounds[0].x, bounds[1].y, bounds[1].z)])
	curve(pos=[vector(bounds[0].x, bounds[0].y, bounds[1].z),
	vector(bounds[1].x, bounds[0].y, bounds[1].z)])
	curve(pos=[vector(bounds[1].x, bounds[1].y, bounds[0].z),
	vector(bounds[0].x, bounds[1].y, bounds[0].z)])

def spheresplit(c, r, pos, axis):
	sphere(pos=c, radius=r)
	if axis == 'x':
		n = vector(1, 0, 0)
	elif axis == 'y':
		n = vector(0, 1, 0)
	elif axis == 'z':
		n = vector(0, 0, 1)

	d = pos - c.dot(n)
	print(d)
	if abs(d) >= r:
		aabox([c-vector(r, r, r), c+vector(r, r, r)])
		return
	
	if d > 0:
		left = [c-vector(r, r, r), c+vector(r, r, r)]
		if axis == 'x':
			left[1].x = pos
		elif axis == 'y':
			left[1].y = pos
		elif axis == 'z':
			left[1].z = pos
		aabox(left)
		
		right = [c+n*d, c+vector(r, r, r)]
		R = r*(1-(d/r)**2)**0.5
		if axis == 'x':
			right[0].y -= R
			right[0].z -= R
			right[1].y -= r-R
			right[1].z -= r-R
		elif axis == 'y':
			right[0].x -= R
			right[0].z -= R
			right[1].x -= r-R
			right[1].z -= r-R
		elif axis == 'z':
			right[0].x -= R
			right[0].y -= R
			right[1].x -= r-R
			right[1].y -= r-R
		aabox(right)
	elif d < 0:
		right = [c-vector(r, r, r), c+vector(r, r, r)]
		if axis == 'x':
			right[0].x = pos
		elif axis == 'y':
			right[0].y = pos
		elif axis == 'z':
			right[0].z = pos
		aabox(right)
		
		left = [c-vector(r, r, r), c+n*d]
		R = r*(1-(d/r)**2)**0.5
		if axis == 'x':
			left[1].y += R
			left[1].z += R
			left[0].y += r-R
			left[0].z += r-R
		elif axis == 'y':
			left[1].x += R
			left[1].z += R
			left[0].x += r-R
			left[0].z += r-R
		elif axis == 'z':
			left[1].x += R
			left[1].y += R
			left[0].x += r-R
			left[0].y += r-R
		aabox(left)
	else:
		right = [c-vector(r, r, r), c+vector(r, r, r)]
		left = [c-vector(r, r, r), c+vector(r, r, r)]
		if axis == 'x':
			right[0].x = c.x
			left[1].x = c.x
		elif axis == 'y':
			right[0].y = c.y
			left[1].y = c.y
		elif axis == 'z':
			right[0].z = c.z
			left[1].z = c.z
		aabox(left)
		aabox(right)
	
def ngonsplit(l, pos, axis):
	curve(pos = l+[l[0]], color=color.green)
	
	if axis == 'x':
		n = vector(1, 0, 0)
	elif axis == 'y':
		n = vector(0, 1, 0)
	elif axis == 'z':
		n = vector(0, 0, 1)

	left = []
	right = []

	for p in l:
		d = pos - p.dot(n)
		if d <= 0:
			left += [p]
		if d >= 0:
			right += [p]

	for i in range(len(l)):
		j = (i+1) % len(l)
		D = l[j]-l[i]
		d = (pos - l[i].dot(n))/D.dot(n)
		if d >= 0 and d <= 1:
			P = l[i] + d*D
			left += [P]
			right += [P]
			
	points(pos=left, color=color.red)
	points(pos=right, color=color.blue)
	
	try:
		l = [vector(left[0].x, left[0].y, left[0].z), vector(left[0].x, left[0].y, left[0].z)]
		for i in left:
			l[0].x = min(i.x, l[0].x)
			l[0].y = min(i.y, l[0].y)
			l[0].z = min(i.z, l[0].z)
			l[1].x = max(i.x, l[1].x)
			l[1].y = max(i.y, l[1].y)
			l[1].z = max(i.z, l[1].z)
		aabox(l)
	except:
		pass
	
	try:
		r = [vector(right[0].x, right[0].y, right[0].z), vector(right[0].x, right[0].y, right[0].z)]
		for i in right:
			r[0].x = min(i.x, r[0].x)
			r[0].y = min(i.y, r[0].y)
			r[0].z = min(i.z, r[0].z)
			r[1].x = max(i.x, r[1].x)
			r[1].y = max(i.y, r[1].y)
			r[1].z = max(i.z, r[1].z)
		aabox(r)
	except:
		pass

def cylindersplit(a, b, r, pos, axis):
	cylinder(pos=a, axis=(b-a), radius=r, opacity=0.5)
	
	n = norm(b-a)
	R = vector(r*(1-n.x**2)**0.5, r*(1-n.y**2)**0.5, r*(1-n.z**2)**0.5)
	box = [vector(min(a.x, b.x), min(a.y, b.y), min(a.z, b.z)) - R, vector(max(a.x, b.x), max(a.y, b.y), max(a.z, b.z)) + R]
	
	if axis == 'x':
		N = vector(1, 0, 0)
	elif axis == 'y':
		N = vector(0, 1, 0)
	elif axis == 'z':
		N = vector(0, 0, 1)
	
	d_a = pos - a.dot(N)
	d_b = pos - b.dot(N)
	
	if d_a/d_b > 0:
		aabox(box)
		return
	
	n_abs = [abs(n.x), abs(n.y), abs(n.z)]
	n_abs.sort()
	
	if abs(n.x) == n_abs[2]:
		u = vector(0, 0, 1)
	elif abs(n.y) == n_abs[2]:
		u = vector(1, 0, 0)
	elif abs(n.z) == n_abs[2]:
		u = vector(0, 1, 0)
	else:
		return
	arrow(pos=a, axis=u, color=color.blue)
	v = n.cross(u)
	v = v.norm()
	u = v.cross(n)
    
	arrow(pos=a, axis=v, color=color.red)
	arrow(pos=a, axis=u, color=color.red)
	arrow(pos=a, axis=n, color=color.red)
	
	A = [a + r*v, a-r*v , a + r*u, a - r*u]
	B = [b + r*v, b-r*v , b + r*u, b - r*u]
	P = []
	
	D = b - a 
	det = 1/N.dot(D)
	for p in A:
		d = (pos - p.dot(N))*det
		print d
		if d >= 0 and d <= 1:
			P += [p + d*D]
	
	points(pos=A+B+P)
		
	try:
		l = [vector(A[0].x, A[0].y, A[0].z), vector(A[0].x, A[0].y, A[0].z)]
		for i in A+P:
			l[0].x = min(i.x, l[0].x)
			l[0].y = min(i.y, l[0].y)
			l[0].z = min(i.z, l[0].z)
			l[1].x = max(i.x, l[1].x)
			l[1].y = max(i.y, l[1].y)
			l[1].z = max(i.z, l[1].z)
		aabox(l)
	except:
		pass
	
	try:
		r = [vector(B[0].x, B[0].y, B[0].z), vector(B[0].x, B[0].y, B[0].z)]
		for i in B+P:
			r[0].x = min(i.x, r[0].x)
			r[0].y = min(i.y, r[0].y)
			r[0].z = min(i.z, r[0].z)
			r[1].x = max(i.x, r[1].x)
			r[1].y = max(i.y, r[1].y)
			r[1].z = max(i.z, r[1].z)
		aabox(r)
	except:
		pass
	
def main():
	cylindersplit(vector(-1, -1, -2), vector(1, 1, 2), 1.0, 0.0, 'x')
	return 0

if __name__ == '__main__':
	main()


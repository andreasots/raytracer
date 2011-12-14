#!/usr/bin/env python
# -*- coding: utf-8 -*-
#
#  untitled.py
#  
#  Copyright 2011 Andreas Ots <andreasots@gmail.com>
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
import sys
Ctraversal = 1
Cintersect = 1

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

def copy_vector(v):
	return vector(v.x, v.y, v.z)

def splitplane(bounds, axis, pos):
	bounds = [copy_vector(bounds[0]), copy_vector(bounds[1])]
	bounds[0][axis] = pos
	points = []
	for i in range(5):
		points += [vector(bounds[0].x, bounds[0].y, bounds[0].z)]
	if axis == 0: # x = 1
		points[1][1] = bounds[1][1]
		points[2][1] = bounds[1][1]
	else: # x = 0
		points[1][0] = bounds[1][0]
		points[2][0] = bounds[1][0]
	if axis == 2: # y = 1
		points[2][1] = bounds[1][1]
		points[3][1] = bounds[1][1]
	else: # y = 2
		points[2][2] = bounds[1][2]
		points[3][2] = bounds[1][2]
	points[4] = points[0]
	curve(pos=points)

def boxsplit(bounds, axis, pos, left):
	bounds = [copy_vector(bounds[0]), copy_vector(bounds[1])]
	if left:
		bounds[1][axis] = pos
	else:
		bounds[0][axis] = pos
	return bounds

def intersect(box1, box2):
	for i in range(3):
		if box1[0][i] > box2[1][i] or box1[1][i] < box2[0][i]:
			return False
	return True

def primcount(bounds, boxes, axis, pos):
	left = 0
	right = 0
	l = boxsplit(bounds, axis, pos, True)
	r = boxsplit(bounds, axis, pos, False)
	
	for box in boxes:
		if intersect(l, box):
			left += 1
		if intersect(r, box):
			right += 1
	
	return left, right

def bestsplit(bounds, boxes, splits, D):
	width = bounds[1][0] - bounds[0][0]
	height = bounds[1][1] - bounds[0][1]
	depth = bounds[1][2] - bounds[0][2]
	area = 2*(width*height + height*depth + width*depth)
	mincost = Ctraversal + area * len(boxes) * Cintersect
	minpos = 0
	minaxis = -1

	for axis in range(3):
		for pos in splits[axis]:
			prims = primcount(bounds, boxes, axis, pos)
			left = boxsplit(bounds, axis, pos, True)
			right = boxsplit(bounds, axis, pos, False)
			width = left[1][0] - left[0][0]
			height = left[1][1] - left[0][1]
			depth = left[1][2] - left[0][2]
			leftarea = 2*(width*height + height*depth + width*depth)
			width = right[1][0] - right[0][0]
			height = right[1][1] - right[0][1]
			depth = right[1][2] - right[0][2]
			rightarea = 2*(width*height + height*depth + width*depth)
			leftcost = Ctraversal + leftarea * prims[0] * Cintersect
			rightcost = Ctraversal + rightarea * prims[1] * Cintersect
			if leftcost + rightcost < mincost:
				mincost = leftcost + rightcost
				minpos = pos
				minaxis = axis
	if minaxis >= 0:
		splitplane(bounds, minaxis, minpos)
		if D < 10:
			left = boxsplit(bounds, minaxis, minpos, True)
			right = boxsplit(bounds, minaxis, minpos, False)
			leftboxes = []
			rightboxes = []
			for box in boxes:
				if intersect(left, box):
					leftboxes += [box]
				if intersect(right, box):
					rightboxes += [box]
			leftsplits = [[],[],[]]
			rightsplits = [[],[],[]]
			for box in leftboxes:
				for i in range(2):
					for axis in range(3):
						leftsplits[axis] += [box[i][axis]]
			for axis in range(3):
				leftsplits[axis] = {}.fromkeys(leftsplits[axis]).keys()
				leftsplits[axis].sort()
			for box in rightboxes:
				for i in range(2):
					for axis in range(3):
						rightsplits[axis] += [box[i][axis]]
			for axis in range(3):
				rightsplits[axis] = {}.fromkeys(rightsplits[axis]).keys()
				rightsplits[axis].sort()
			bestsplit(left, leftboxes, leftsplits, D+1)
			bestsplit(right, rightboxes, rightsplits, D+1)

def main(argc, argv):
	if argc == 1:
		return 0
	screen = display(title="Raytracer", width=1024, height=768)
	bounds = None
	boxes = []
	with open(argv[1]) as f:
		for line in f:
			line = line.strip().split()
			box = None
			if len(line) == 0:
				continue
			if line[0] == "sphere":
				pos = (float(line[2]), float(line[3]), float(line[4]))
				radius = float(line[5])
				sphere(pos = pos, radius = radius, color = color.green)
				box = [vector(pos)-vector(radius, radius, radius), vector(pos)+vector(radius, radius, radius)]
				boxes += [box]
			elif line[0] == "triangle":
				A = vector(float(line[2]), float(line[3]), float(line[4]))
				B = vector(float(line[5]), float(line[6]), float(line[7]))
				C = vector(float(line[8]), float(line[9]), float(line[10]))
				box = [vector(min(A.x, B.x, C.x), min(A.y, B.y, C.y), min(A.z, B.z, C.z)),
				vector(max(A.x, B.x, C.x), max(A.y, B.y, C.y), max(A.z, B.z, C.z))]
				boxes += [box]
				curve(pos = [A, B, C, A], color=color.green)
			elif line[0] == "camera":
				if line[1] == "lookat":
					eye = vector(float(line[2]), float(line[3]), float(line[4]))
					center = vector(float(line[5]), float(line[6]), float(line[7]))
					up = vector(float(line[8]), float(line[9]), float(line[10]))
					f = center-eye
					f = f.norm()
					up = up.norm()
					s = f.cross(up)
					up = s.cross(f)
				sphere(pos=eye, radius = 0.1, color = color.red)	
				arrow(pos=eye, axis=f, color = color.red, shaftwidth=0.125)
				arrow(pos=eye, axis=up, color = color.blue, shaftwidth=0.125)
				screen.up = up
			if bounds == None:
				if box != None:
					bounds = [copy_vector(box[0]), copy_vector(box[1])]
			else:
				bounds[0].x = min(bounds[0].x, box[0].x)
				bounds[0].y = min(bounds[0].y, box[0].y)
				bounds[0].z = min(bounds[0].z, box[0].z)
				bounds[1].x = max(bounds[1].x, box[1].x)
				bounds[1].y = max(bounds[1].y, box[1].y)
				bounds[1].z = max(bounds[1].z, box[1].z)
	aabox(bounds)
	screen.center = (bounds[0]+bounds[1])/2
	splits = [[], [], []]
	for box in boxes:
		for i in range(2):
			for axis in range(3):
				splits[axis] += [box[i][axis]]
	for axis in range(3):
		splits[axis] = {}.fromkeys(splits[axis]).keys()
		splits[axis].sort()
	
	print "Building kD-tree"
	#bestsplit(bounds, boxes, splits, 0)
	print "Done"
	
	return 0

if __name__ == '__main__':
	main(len(sys.argv), sys.argv)


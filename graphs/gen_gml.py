#!/usr/bin/env python

# Name: gen_gml.py
# Description: Generates random graph in .gml format
# Author: Milan Munzar, xmunza00@stud.fit.vutbr.cz

import os
import sys
import argparse
import itertools
import random
import xml.dom.minidom as xml

# parsing command line arguments
def isPositiveInt(value):
    value = int(value)
    if value < 0:
         raise argparse.ArgumentTypeError("%s is an invalid positive int value" % value)
    return value

def isPositiveFloat(value):
    value = float(value)
    if value < 0:
         raise argparse.ArgumentTypeError("%s is an invalid positive int value" % value)
    return value

parser = argparse.ArgumentParser(description='Generates random undirected graph in .gml format.')
parser.add_argument('-v', type = isPositiveInt, required=True, help = 'number of vertices')
parser.add_argument('-e', type = isPositiveFloat, required=True, help = 'number of edges (or portion of maximum if <1)')
args = parser.parse_args()

# generates random edges in undirected graph
possible_combinations = list(itertools.combinations(range(args.v),2))
if args.e < 1:
    args.e = len(possible_combinations)*args.e
args.e = int(args.e)

if len(possible_combinations) < args.e:
	print "Error: Number of edges exceeds possible combinations in undirected graph!"
	exit(0)

used_combinations = []
for i in range(args.e):
	used_combinations.append(possible_combinations.pop(random.randint(0,
		len(possible_combinations) - 1)))

# generates .gml
impl = xml.getDOMImplementation()
document = impl.createDocument(None, "graphml", None)
top_elem = document.documentElement

elem_graph = document.createElement("graph")
elem_graph.setAttribute("id", "G")
elem_graph.setAttribute("edgedefault", "undirected")
top_elem.appendChild(elem_graph)

for i in xrange(args.v):
	elem_node = document.createElement("node")
	elem_node.setAttribute("id", str(i))
	elem_graph.appendChild(elem_node)


while used_combinations:
	edge =  used_combinations.pop()
	elem_edge = document.createElement("edge")
	elem_edge.setAttribute("source", str(edge[0]))
	elem_edge.setAttribute("target", str(edge[1]))	
	elem_graph.appendChild(elem_edge)

# creates file
file_number = 1
file_name = str(args.e) + "_" + str(args.v) + "_" + str(file_number) + ".gml"
while os.path.exists(file_name):
	file_number += 1
	file_name = str(args.e) + "_" + str(args.v) + "_" + str(file_number) + ".gml"

gml_file = open(file_name, "w")
gml_file.write(document.toprettyxml())
gml_file.close()



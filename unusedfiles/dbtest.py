#!/usr/bin/python
#-*-coding:utf-8-*-
#
#
# Author : TT_last
#
#
# This is a mongodb Test for db

from pymongo import Connection 

con = Connection("172.22.27.122")

#get user code
#
db = con.gzhu_db
solutions = db.solutions
one_solution = solutions.find_one({'result':'0'});

if one_solution:
	print one_solution["code"]
else:
	pass

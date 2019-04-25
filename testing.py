#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Mon Apr 22 03:32:50 2019

@author: anshumansingh
"""

database = pymysql.connect('localhost','root','mysql@3006','restaurant') #CONNECTION PARAMETERES for database
cursor = database.cursor()

def execute_query(statement):
    cursor.execute(statement)
    return cursor.fetchall()

available = execute_query(f"select ITEM_ID from restaurant.config where PAGE_NO = 1 and ITEM_NO = 1")
print(available[0][0])
#print(available)
for item in available:
    item_id = item[0]
    #print(execute_query(f"select count(*) from restaurant.temp where TABLE_ID = 1 and ITEM_ID = {item_id}")[0][0])

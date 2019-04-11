#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Thu Apr 11 00:53:38 2019

@author: anshumansingh
"""

import pymysql # module to connect with database
database = pymysql.connect('localhost','root','mysql@3006','') #CONNECTION PARAMETERES for database
cursor = database.cursor()


statement = f"show databases"
cursor.execute(statement)
print(cursor.fetchall())

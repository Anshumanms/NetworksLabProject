#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Thu Apr 11 00:53:38 2019

@author: anshumansingh
"""

import pymysql # module to connect with database
database = pymysql.connect('localhost','root','mysql@3006','restaurant') #CONNECTION PARAMETERES for database
cursor = database.cursor()

def execute_query(statement):
    cursor.execute(statement)
    return cursor.fetchall()

table_id = 1
content = "bill"
request = ["T1,P1,I1","T1,P2,I1"]

if content == "reset":
    request = []
if content == "confirm":
    for item in request:
        order = item.split(",")
        #print(order)
        table_id = order[0].replace("T","")
        page_id = order[1].replace("P","")
        item_id = order[2].replace("I","")
        #print("Table ID : ",table_id+" Page ID : "+page_id+" Item ID : "+item_id)
        available = execute_query(f"select QUANTITY_AVAILABLE,ITEM_NAME,PRICE_PER_ITEM from available where ITEM_ID = '{item_id}'")
        #print(type(available[0][0]))
        if (available[0][0] > 0):
            execute_query(f"update available set QUANTITY_AVAILABLE = '{available[0][0]-1}' where ITEM_ID = '{item_id}'")
            execute_query(f"insert into current_orders (TABLE_ID, ITEM_ID,ITEM_NAME,QUANTITY,PRICE) values ('{table_id}','{item_id}','{available[0][1]}','{1}','{available[0][2]}')")
            database.commit()
        else:
            print("Item unavailable")
if content == "bill":
    bill_amount = execute_query(f"select sum(price) from current_orders where TABLE_ID = '{table_id}'")
    print("Bill Amount : ",bill_amount)
    execute_query(f"insert into past_orders select * from current_orders where TABLE_ID = '{table_id}'")
    execute_query(f"delete from current_orders where TABLE_ID = '{table_id}'")
    database.commit()
print("The End")            
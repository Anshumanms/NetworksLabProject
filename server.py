import socket
import pymysql
maxConnections = 4
database = pymysql.connect('localhost','root','mysql@3006','restaurant') #CONNECTION PARAMETERES for database
cursor = database.cursor()

def execute_query(statement):
    cursor.execute(statement)
    return cursor.fetchall()
    

try: 
    s = socket.socket(socket.AF_INET,socket.SOCK_STREAM)
    s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)         
     
    s.bind(('10.208.67.44', 8090))
    s.listen(maxConnections)                 
    request = []
    
    while True:
     
        client, addr = s.accept()
        #print("client : ",client)
        #print("addr : ",addr)
        
        
        
        while True:
            #print("Waiting")
            content = client.recv(32)
            
            #if not content:
            #    break
            #print("Waiting")
            if len(content) ==0:
               break
     
            else:
                content  = content.decode()
                #print(content)
                if (not ("bill" in content) and not ("reset" in content) and not ("confirm" in content) and not ("getstatus") in content):
                    #request.append(content)
                    order = content.split(",")
                    table_id = order[0].replace("T","")
                    page_id = order[1].replace("P","")
                    item_no = order[2].replace("N","")
                    #print(table_id,page_id,item_no)
                    item_id = execute_query(f"select ITEM_ID from restaurant.config where PAGE_NO = '{page_id}' and ITEM_NO = '{item_no}'")[0][0]
                    available = execute_query(f"select QUANTITY_AVAILABLE,ITEM_NAME,PRICE_PER_ITEM from available where ITEM_ID = '{item_id}'")
                    if (available != () and available[0][0]> 0 ):
                        client.send(f"{available[0][1]} : Available\f".encode())
                        execute_query(f"insert into temp (TABLE_ID, ITEM_ID,ITEM_NAME,PRICE) values ('{table_id}','{item_id}','{available[0][1]}','{available[0][2]}')")
                        #database.commit()
                    else:
                        print(f"{available[0][1]}: Not Available", item_id)
                        client.send(f"{available[0][1]}:Not Available\f".encode())
                    database.commit()
                else:
                    if "getstatus" in content:
                        table_id = content.split(",")[0].replace("T","")
                        totalItems = execute_query(f"select count(*) from restaurant.temp where TABLE_ID = {table_id}")[0][0]
                        totalAmount = execute_query(f"select sum(PRICE) from restaurant.temp where TABLE_ID = {table_id}")[0][0]
                        client.send(f"TI:{totalItems} TA:{totalAmount} \f".encode())
                        #client.send(f"S:{totalAmount}\f".encode())
                    if "reset" in content:
                        table_id = content.split(",")[0].replace("T","")
                        available = execute_query(f"delete from restaurant.temp where TABLE_ID = '{table_id}'")
                        print("Order Reset!")
                        client.send("Selection Cleared.\f".encode());
                    if "confirm" in content:
                        table_id = content.split(",")[0].replace("T","")
                        result = execute_query(f"select distinct ITEM_ID from restaurant.temp where TABLE_ID = {table_id}")
                        for item in result:
                                item_id = item[0]
                                no_of_items = execute_query(f"select count(*) from restaurant.temp where TABLE_ID = 1 and ITEM_ID = {item_id}")[0][0]
                                available = execute_query(f"select QUANTITY_AVAILABLE,ITEM_NAME,PRICE_PER_ITEM from available where ITEM_ID = '{item_id}'")
                                if (available != ()):
                                    if (available[0][0] >= no_of_items):
                                        execute_query(f"update available set QUANTITY_AVAILABLE = '{available[0][0]-no_of_items}' where ITEM_ID = '{item_id}'")
                                        #execute_query(f"delete from temp where TABLE_ID = '{table_id}' and ITEM_ID = {item_id}")
                                        execute_query(f"insert into current_orders (TABLE_ID, ITEM_ID,ITEM_NAME,QUANTITY,PRICE) values ('{table_id}','{item_id}','{available[0][1]}','{no_of_items}','{available[0][2]*no_of_items}')")
                                        
                                    else:
                                        print(f"Ordered {available[0][1]}:{no_of_items} Available:{available[0][0]}")
                                        #execute_query(f"delete from temp where TABLE_ID = '{table_id}' and ITEM_ID = {item_id}")
                                        client.send(f"{available[0][1]} Available : {available[0][0]}\f".encode());
                                    execute_query(f"delete from temp where TABLE_ID = '{table_id}' and ITEM_ID = {item_id}")
                                    #database.commit()
                                else:
                                    print("Item unavailable: ", available[0][1])
                                    client.send("Item unavailable:\f".encode());
                        print("Order Confirmed!")
                        client.send("Order Confirmed.\f".encode());
                        request = []
                    if "bill" in content:
                        table_id = content.split(",")[0].replace("T","")
                        bill_amount = execute_query(f"select sum(PRICE) from current_orders where TABLE_ID = '{table_id}'")
                        print("Bill Amount : ",bill_amount[0][0])
                        client.send(f"Bill Amount :{bill_amount[0][0]}\f".encode())
                        execute_query(f"insert into past_orders select * from current_orders where TABLE_ID = '{table_id}'")
                        execute_query(f"delete from current_orders where TABLE_ID = '{table_id}'")
                        #database.commit()
                    database.commit()
                
        #print("Closing connection")
        client.close()
finally:
    client.close()
    s.close()
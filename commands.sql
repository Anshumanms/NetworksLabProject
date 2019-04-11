create table restaurant;
create table available (ITEM_ID int, ITEM_NAME varchar(255) not null,QUANTITY int not null, PRICE_PER_ITEM int not null,PRIMARY KEY ITEM_ID  );
create table current_orders (ORDER_ID int PRIMARY KEY, TABLE_ID int not null, ITEM_ID int, ITEM_NAME varchar(255) );


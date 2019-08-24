create table restaurant;
create table available (ITEM_ID int, ITEM_NAME varchar(255) not null,QUANTITY int not null, PRICE_PER_ITEM decimal(7,2) not null,PRIMARY KEY ITEM_ID  );
create table current_orders (ORDER_ID int PRIMARY KEY, TABLE_ID int not null, ITEM_ID int not null, ITEM_NAME varchar(255) not null,QUANTITY int not null,PRICE decimal(8,2));
create table config (ITEM_NO int not null,PAGE_NO int not null, ITEM_ID int PRIMARY KEY);

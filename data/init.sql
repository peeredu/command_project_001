CREATE DATABASE shop;
USE shop;

CREATE TABLE Orders (
   OrderId INT NOT NULL AUTO_INCREMENT PRIMARY KEY, 
   OrderDate DATETIME
);

CREATE TABLE Products (
  ProductId INT NOT NULL AUTO_INCREMENT PRIMARY KEY, 
  ProductName VARCHAR(50) NOT NULL,
  UnitPrice NUMERIC(10,2), 
  Quantity INT,
  Active BOOLEAN DEFAULT 1  
);

CREATE TABLE OrderDetails (
  OrderDetailId INT NOT NULL AUTO_INCREMENT PRIMARY KEY, 
  OrderId INT,
  ProductId INT,
  UnitPrice NUMERIC(10,2),
  Quantity INT,
  CONSTRAINT FK_OrderDetails_Orders
  FOREIGN KEY (OrderId) REFERENCES Orders (OrderId),
  CONSTRAINT FK_OrderDetails_Products
  FOREIGN KEY (ProductId) REFERENCES Products (ProductId)
);

INSERT INTO Products (ProductName, UnitPrice, Quantity) VALUES ('banana', 120, 451); 
INSERT INTO Products (ProductName, UnitPrice, Quantity) VALUES ('orange', 154.12, 324);
INSERT INTO Products (ProductName, UnitPrice, Quantity) VALUES ('apple', 91.25, 150);
INSERT INTO Products (ProductName, UnitPrice, Quantity) VALUES ('grapefruit', 150.25, 130);
INSERT INTO Products (ProductName, UnitPrice, Quantity) VALUES ('kiwi', 175.14, 15);
INSERT INTO Products (ProductName, UnitPrice, Quantity) VALUES ('grapes', 275, 45);
INSERT INTO Products (ProductName, UnitPrice, Quantity) VALUES ('lemon', 105.24, 64);
INSERT INTO Products (ProductName, UnitPrice, Quantity, Active) VALUES ('peach', 256.68, 0, 0);

INSERT INTO Orders (OrderDate) VALUES ('2024-02-27T12:34:56');
INSERT INTO Orders (OrderDate) VALUES ('2024-03-02T09:45:11');
INSERT INTO Orders (OrderDate) VALUES ('2024-03-04T14:01:47');

INSERT INTO OrderDetails (OrderId, ProductId, UnitPrice, Quantity) VALUES (1, 1, 120, 5);
INSERT INTO OrderDetails (OrderId, ProductId, UnitPrice, Quantity) VALUES (1, 3, 91.25, 4);
INSERT INTO OrderDetails (OrderId, ProductId, UnitPrice, Quantity) VALUES (1, 7, 105.24, 1);

INSERT INTO OrderDetails (OrderId, ProductId, UnitPrice, Quantity) VALUES (2, 1, 120, 11);
INSERT INTO OrderDetails (OrderId, ProductId, UnitPrice, Quantity) VALUES (2, 2, 154.12, 27);
INSERT INTO OrderDetails (OrderId, ProductId, UnitPrice, Quantity) VALUES (2, 3, 91.25, 7);
INSERT INTO OrderDetails (OrderId, ProductId, UnitPrice, Quantity) VALUES (2, 4, 150.25, 17);
INSERT INTO OrderDetails (OrderId, ProductId, UnitPrice, Quantity) VALUES (2, 6, 275, 3);

INSERT INTO OrderDetails (OrderId, ProductId, UnitPrice, Quantity) VALUES (3, 5, 175, 2);

-- Далее будут приводиться примеры необходимых запросов, которые можно будет копировать

SELECT * FROM Products WHERE ProductId=2;
SET @id:=1; SELECT * FROM Products WHERE ProductId=@id;

SET @name='test3';
SET @price=66.66;
SET @quantity=6;
SET @active=1;
INSERT INTO Products (ProductName, UnitPrice, Quantity, Active) VALUES (@product_name, @unit_price, @quantity, @active);


SELECT * FROM Products;
SELECT * FROM Orders;
SELECT * FROM OrderDetails;

DROP TABLE OrderDetails;
DROP TABLE Orders;
DROP TABLE Products;
DROP DATABASE shop;

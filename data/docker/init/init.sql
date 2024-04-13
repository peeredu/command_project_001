CREATE DATABASE IF NOT EXISTS shop;
USE shop;

CREATE TABLE IF NOT EXISTS Orders (
   OrderId INT NOT NULL AUTO_INCREMENT PRIMARY KEY, 
   OrderDate DATETIME
);

CREATE TABLE IF NOT EXISTS Products (
  ProductId INT NOT NULL AUTO_INCREMENT PRIMARY KEY, 
  ProductName VARCHAR(50) NOT NULL,
  UnitPrice INT, 
  Quantity INT,
  Active BOOLEAN DEFAULT 1  
);

CREATE TABLE IF NOT EXISTS OrderDetails (
  OrderDetailId INT NOT NULL AUTO_INCREMENT PRIMARY KEY, 
  OrderId INT,
  ProductId INT,
  UnitPrice INT,
  Quantity INT,
  CONSTRAINT FK_OrderDetails_Orders
  FOREIGN KEY (OrderId) REFERENCES Orders (OrderId),
  CONSTRAINT FK_OrderDetails_Products
  FOREIGN KEY (ProductId) REFERENCES Products (ProductId)
);

DELIMITER //
CREATE PROCEDURE init_Products()
BEGIN
  IF NOT EXISTS (SELECT * FROM Products LIMIT 1) THEN
    INSERT INTO Products (ProductName, UnitPrice, Quantity) VALUES ('banana', 12000, 451); 
    INSERT INTO Products (ProductName, UnitPrice, Quantity) VALUES ('orange', 15412, 324);
    INSERT INTO Products (ProductName, UnitPrice, Quantity) VALUES ('apple', 9125, 150);
    INSERT INTO Products (ProductName, UnitPrice, Quantity) VALUES ('grapefruit', 15025, 130);
    INSERT INTO Products (ProductName, UnitPrice, Quantity) VALUES ('kiwi', 17514, 15);
    INSERT INTO Products (ProductName, UnitPrice, Quantity) VALUES ('grapes', 27500, 45);
    INSERT INTO Products (ProductName, UnitPrice, Quantity) VALUES ('lemon', 10524, 64);
    INSERT INTO Products (ProductName, UnitPrice, Quantity, Active) VALUES ('peach', 25668, 0, 0);
  END IF;
END//
DELIMITER ;

DELIMITER //
CREATE PROCEDURE init_Orders()
BEGIN
  IF NOT EXISTS (SELECT * FROM Orders LIMIT 1) THEN
    INSERT INTO Orders (OrderDate) VALUES ('2024-02-27T12:34:56');
    INSERT INTO Orders (OrderDate) VALUES ('2024-03-02T09:45:11');
    INSERT INTO Orders (OrderDate) VALUES ('2024-03-04T14:01:47');
  END IF;
END//
DELIMITER ;

DELIMITER //
CREATE PROCEDURE init_OrderDetails()
BEGIN
  IF NOT EXISTS (SELECT * FROM Orders LIMIT 1) THEN
    INSERT INTO OrderDetails (OrderId, ProductId, UnitPrice, Quantity) VALUES (1, 1, 12000, 5);
    INSERT INTO OrderDetails (OrderId, ProductId, UnitPrice, Quantity) VALUES (1, 3, 9125, 4);
    INSERT INTO OrderDetails (OrderId, ProductId, UnitPrice, Quantity) VALUES (1, 7, 10524, 1);
    INSERT INTO OrderDetails (OrderId, ProductId, UnitPrice, Quantity) VALUES (2, 1, 12000, 11);
    INSERT INTO OrderDetails (OrderId, ProductId, UnitPrice, Quantity) VALUES (2, 2, 15412, 27);
    INSERT INTO OrderDetails (OrderId, ProductId, UnitPrice, Quantity) VALUES (2, 3, 9125, 7);
    INSERT INTO OrderDetails (OrderId, ProductId, UnitPrice, Quantity) VALUES (2, 4, 15025, 17);
    INSERT INTO OrderDetails (OrderId, ProductId, UnitPrice, Quantity) VALUES (2, 6, 27500, 3);
    INSERT INTO OrderDetails (OrderId, ProductId, UnitPrice, Quantity) VALUES (3, 5, 17500, 2);
  END IF;
END//
DELIMITER ;

CALL init_Products;
CALL init_Orders;
CALL init_OrderDetails;

DROP PROCEDURE IF EXISTS init_Products;
DROP PROCEDURE IF EXISTS init_Orders;
DROP PROCEDURE IF EXISTS init_OrderDetails;

-- Далее будут приводиться примеры необходимых запросов, которые можно будет копировать

-- INSERT INTO Products (ProductName, UnitPrice, Quantity, Active) VALUES ('test', 11.22, 33, 1);
-- INSERT INTO Products (ProductName, UnitPrice, Quantity, Active) VALUES (?, ?, ?, ?);

-- SELECT * FROM Products WHERE ProductId=2;
-- SELECT * FROM Products WHERE ProductId=?;

-- UPDATE Products SET Active=0 WHERE ProductId=2;
-- UPDATE Products SET Active=0 WHERE ProductId=?;

-- DELETE FROM Products WHERE ProductId=2;
-- DELETE FROM Products WHERE ProductId=?;

-- SET @id:=1; SELECT * FROM Products WHERE ProductId=@id;

-- SET @name='test3';
-- SET @price=66.66;
-- SET @quantity=6;
-- SET @active=1;
-- INSERT INTO Products (ProductName, UnitPrice, Quantity, Active) VALUES (@product_name, @unit_price, @quantity, @active);

-- SELECT * FROM Orders O CROSS JOIN OrderDetails OD WHERE O.OrderId=OD.OrderId AND O.OrderId=2;
-- SELECT * FROM Orders O CROSS JOIN OrderDetails OD WHERE O.OrderId=OD.OrderId AND O.OrderId=?;

-- SELECT * FROM Products;
-- SELECT * FROM Orders;
-- SELECT * FROM OrderDetails;

-- DROP TABLE OrderDetails;
-- DROP TABLE Orders;
-- DROP TABLE Products;
-- DROP DATABASE shop;

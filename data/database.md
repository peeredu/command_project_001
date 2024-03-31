# Структура базы данных shop

## Таблица 1: Orders
(таблица хранит заказы)
#### Поля:
+ OrderId int (pk) (auto-generate)
+ OrderDate datetime %Y.%m.%d %H:%M:%S

## Таблица 2: Products
(таблица хранит различные продукты)
#### Поля:
+ ProductId int (pk) (auto-generate)
+ ProductName str
+ UnitPrice money
+ Quantity int
+ Active boolean

## Таблица 3: OrderDetails
(таблица хранит детали заказа)
#### Поля:
+ OrderDetailId (pk) (auto-generate)
+ OrderId (fk)
+ ProductId (fk)
+ UnitPrice money
+ Quantity int


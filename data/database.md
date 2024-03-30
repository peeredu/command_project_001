# Структура базы данных shop

## Таблица 1: Orders
(таблица хранит заказы)
#### Поля:
+ OrderId int (pk) (auto-generate)
+ OrderDate datetime %Y.%m.%d %H:%M:%S

## Таблица 2: Products
(таблица хранит различные продукты)
#### Поля:
+ ProductID int (pk) (auto-generate)
+ ProductName str
+ UnitPrice money
+ Quantity int

## Таблица 3: OrderDetails
(таблица хранит детали заказа)
#### Поля:
+ OrderDetailsId (pk) (auto-generate)
+ OrderId
+ ProductID
+ UnitPrice
+ Quantity


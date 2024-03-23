# Структура базы данных

## Таблица 1: ShopItem
#### Поля:
+ id int (pk) (auto-generate)
+ name str
+ price int
+ quantity int
+ in_stock bool

## Таблица 2: Order
#### Поля:
+ id (pk) (auto-generate)
+ items_ids list {ShopItem.id}
+ price int
+ date (auto-generate) datetime %Y.%m.%d %H:%M:%S


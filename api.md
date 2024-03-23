# Схема api
### **GET** запросы
**/items/{id}** - показать информацию об отдельном товаре

- ответ сервера 200: {"name": str, "price": int, "quantity": int, "in_stock": bool}
- ответ в случае любой ошибки: {"name": NULL, "price": 0, "quantity": 0, "in_stock": NULL}

**/orders/{id}** - показать информацию об отдельном заказе

- ответ сервера 200: {"items_ids": [{id}], "price": int, "date": datetime}
- ответ в случае любой ошибки: {"items_ids": [], "price": 0, "date": NULL}

**/items** - показать информацию о всех товарах

- ответ сервера 200: {"items": [{"name": str, "price": int, "quantity": int, "in_stock": bool}]}
- ответ в случае любой ошибки: {"items": []}

**/orders** - показать информацию о всех заказах

- ответ сервера 200: {"orders": [{"items_ids": [{id}], "price": int, "date": datetime}]}
- ответ в случае любой ошибки: {"orders": []}

**/orders/{date}** - показать информацию о всех заказах за определенную дату

- ответ сервера 200: {"orders": [{"items_ids": [{id}], "price": int, "date": datetime}]}
- ответ в случае любой ошибки: {"orders": []}

**/orders/max** - показать информацию о заказе с максимальной стоимостью

- ответ сервера 200: {"items_ids": [{id}], "price": int, "date": datetime}
- ответ в случае любой ошибки: {"items_ids": [], "price": 0, "date": NULL}

**/orders/min** - показать информацию о заказе с минимальной стомостью

- ответ сервера 200: {"items_ids": [{id}], "price": int, "date": datetime}
- ответ в случае любой ошибки: {"items_ids": [], "price": 0, "date": NULL}

### **POST** запросы

**/orders** - добавить новый заказ

- ответ сервера 201: {"items_ids": [{id}], "price": int, "date": datetime}
- ответ в случае любой ошибки: {"items_ids": [], "price": 0, "date": NULL}

**/items** - добавить новый товар

- ответ сервера 201: {"name": str, "price": int, "quantity": int, "in_stock": bool}
- ответ в случае любой ошибки: {"name": NULL, "price": 0, "quantity": 0, "in_stock": NULL}

### **PUT** запросы

**/items/{id}** - отредактировать товар

- ответ сервера 200: {"name": str, "price": int, "quantity": int, "in_stock": bool}
- ответ в случае любой ошибки: {"name": NULL, "price": 0, "quantity": 0, "in_stock": NULL}

### **DELETE** запросы

**/items/{id}** - удалить товар товар

- ответ сервера 200: {"name": str, "price": int, "quantity": int, "in_stock": bool}
- ответ в случае любой ошибки: {"name": NULL, "price": 0, "quantity": 0, "in_stock": NULL}

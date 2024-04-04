<template>
    <div class="FilterForm">
        <input class="input"
        type="text"
         v:bind:value="id"
         placeholder="Введите номер товара"
         @input="id = $event.target.value">
        <button class="btn" @click="getItem">Найти</button>
    </div>
    <div class='ItemCard'>
        <p class="ItemCardLine">Название: {{ item['name'] }}</p>
        <p class="ItemCardLine">Cтоимость: {{ item['price'] }} р.</p>
        <p class="ItemCardLine">Количество: {{ item['quantity'] }}</p>
    </div>
</template>

<script>
import axios from 'axios'
    export default {
        data() {
            return {
                id: 0,
                item: {
                    id: 0,
                    name: '',
                    price: 0,
                    quantity: 0,
                    active: false,
                },
            }
        },
        methods: {
        async getItem() {
            axios.get("http://localhost:2000/items/" + this.id).then((response) => {
                this.item = response.data
            })
        },
    }
    }
</script>

<style scoped>
.FilterForm {
    padding: 15px;
    width: 600px;
    margin-left: auto;
    margin-right: auto;
    display: flex;
}

.input {
    width: 80%;
    padding: 10px 15px;
    border: 1px solid rgb(23, 255, 197);
    font-size: medium;
    border-radius: 8px;
}

.btn {
  display: inline-block;
  border: 1px solid lightseagreen;
  width: 75px;
  height: 50px;
  background-color: rgb(216, 255, 242);
  cursor: pointer;
  line-height: 25px;
  text-align: center;
  border-radius: 8px;
}

.ItemCard {
    padding: 15px;
    width: 600px;
    margin-left: auto;
    margin-right: auto;
    display: flex;
    flex-direction: column;
    width: 250px;
    height: 300px;
}

.ItemCardLine {
    margin-bottom: 10px;
}

</style>
<template>
  <div сlass="AddForm">
    <input v-bind:value="item.name" type="text" placeholder="Название товара" />
    <input v-bind:value="item.price" type="text" placeholder="Цена товара" />
    <input v-bind:value="item.quantity" type="text" placeholder="Количество товара" />
    <form-button v-bind:service="button_data" @click="AddItem"></form-button>
  </div>
</template>

<script>
import FormButton from "@/components/FormButton";
export default {
  data() {
    return {
        item: {
            name = '',
            price = 0,
            quantity = 0,
        },
      button_data: {
        text: "Добавить товар",
      },
    };
  },
  components: {
    FormButton,
  },
  methods: {
    async AddItem() {
        this.item = 
    }
  }
};
</script>

<style scoped></style>

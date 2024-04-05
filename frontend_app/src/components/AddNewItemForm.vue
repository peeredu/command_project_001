<template>
  <form @submit.prevent>
    <div class="AddForm">
      <input class="InputForm"
        v-bind:value="item.name"
        type="text"
        placeholder="Название товара"
        @input="item.name = $event.target.value"
      />
      <input class="InputForm"
        v-bind:value="item.price"
        type="text"
        placeholder="Цена товара"
        @input="item.price = $event.target.value"
      />
      <input class="InputForm"
        v-bind:value="item.quantity"
        type="text"
        placeholder="Количество товара"
        @input="item.quantity = $event.target.value"
      />
      <div class="btns">
        <form-button class="Btn" v-bind:service="button_data" @click="AddItem"></form-button>
        <link-button v-bind:service="button_link_data"></link-button>
      </div>
    </div>
  </form>
</template>

<script>
import FormButton from "@/components/FormButton";
import LinkButton from "@/components/LinkButton";
import axios from "axios";
export default {
  data() {
    return {
      item: {
        name: "",
        price: "",
        quantity: "",
      },
      button_data: {
        text: "Добавить товар",
      },
      button_link_data: {
        title: "Назад",
        link: "/",
      }
    };
  },
  components: {
    FormButton,
    LinkButton,
  },
  methods: {
    async AddItem() {
      // TODO Task #49: Валидация данных из формы.
      const NewItem = {
        name: this.item.name,
        price: this.item.price,
        quantity: this.item.quantity,
      };
      axios
        .post("http://localhost:2000/items", NewItem)
        .then(function (response) {
          // TODO: здесь надо выводить на экран что товар успешно добавлен
          console.log(response);
        })
        .catch(function (error) {
          // TODO: здесь надо выводить на экран сообщение об ошибке
          console.log(error);
        });
      this.item.name = ''
      this.item.price = ''
      this.item.quantity = ''
    },
  },
};
</script>

<style scoped>
.AddForm {
  padding: 20px;
  display: flex;
  flex-direction: column;
}

.InputForm {
    height: 40px;
    width: 600px;
    margin-left: auto;
    margin-right: auto;
    margin-bottom: 10px;
    border: 1px solid rgb(32, 73, 178);
    padding: 3px;
    font-size: medium;
}

.btns {
    margin-left: auto;
    margin-right: auto;
}

.Btn {
    align-self: flex-end;
    margin-left: auto;
    margin-right: 150px;
}
</style>

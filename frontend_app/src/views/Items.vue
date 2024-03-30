<template>
  <item-list v-bind:items="items"></item-list>
  <link-button class='btn' v-bind:service="button_data"></link-button>
</template>

<script>
import axios from "axios";
import LinkButton from "@/components/LinkButton";
import ItemList from "@/components/ItemList";
export default {
  components: {
    LinkButton,
    ItemList,
  },
  data() {
    return {
      items: [
        { name: "Картофель", price: 40, quantity: 80, in_stock: true },
        {
          name: "Сырок глазированный",
          price: 80,
          quantity: 13,
          in_stock: true,
        },
        { name: "Кока кола", price: 240, quantity: 0, in_stock: false },
      ],
      button_data: {
        title: "Назад",
        link: "/",
      },
    };
  },
  // methods: {
  //     async getItems() {
  //         axios
  //             .get('/items')
  //             .then((response) => {
  //                 this.items = response.data;
  //             })
  //     }
  // },
};
</script>

<style>
.btn {
    margin-right: auto;
    margin-left: auto;
}
</style>

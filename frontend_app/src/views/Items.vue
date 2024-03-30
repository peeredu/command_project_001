<template>
  <item-list v-bind:items="items"></item-list>
  <div class="btn">
    <link-button v-bind:service="button_data"></link-button>
  </div>
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
      items: [],
      button_data: {
        title: "Назад",
        link: "/",
      },
    };
  },
  methods: {
    async getItems() {
      axios.get("http://localhost:2000/items").then((response) => {
        this.items = response.data;
      });
    },
  },
  beforeMount() {
    this.getItems();
  },
};
</script>

<style>
.btn {
  width: 600px;
  display: flex;
  margin-right: auto;
  margin-left: auto;
}
</style>

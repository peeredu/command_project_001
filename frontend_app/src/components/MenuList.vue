<template>
  <div class="menuList">
    <link-button v-for="service in services" v-bind:service="service" class="menu_item">
    </link-button>
  </div>
</template>

<script>
import LinkButton from "@/components/LinkButton";
export default {
  data() {
    return {
      services: [
        { title: "Заказы", link: "/orders" },
        { title: "Товары", link: "/items" },
        { title: "Добавить заказ", link: "#" },
        { title: "Добавить товар", link: "/new_item" },
        { title: "Найти товар", link: "/find_item" },
        { title: "Найти заказ", link: "/find_order" },
      ],
    };
  },
  components: {
    LinkButton,
  },
};
</script>

<style scoped>
.menuList {
  display: flex;
  flex-direction: column;
  align-items: center;
}
</style>

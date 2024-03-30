<template>
  <router-link class='menu_item' :to="service.link">
    {{ service.title }}
  </router-link>
</template>

<script>
export default {
  props: {
    service: {
    },
  },
};
</script>

<style scoped>
.menu_item {
  display: inline-block;
  border: 1px solid lightseagreen;
  margin-bottom: 15px;
  width: 150px;
  height: 50px;
  background-color: rgb(216, 255, 242);
  cursor: pointer;
  line-height: 50px;
  text-align: center;
  border-radius: 8px;
}
</style>

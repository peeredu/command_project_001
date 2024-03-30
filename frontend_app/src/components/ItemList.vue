<template>
  <div class="headers">
    <p class="headersLine"><strong>Наименование</strong></p>
    <p class="headersLine"><strong>Цена</strong></p>
    <p class="headersLine"><strong>Количество</strong></p>
  </div>
  <div class="itemList" v-for="item in items">
    <p class="itemListLine" v-for="prop in item">
      {{ prop }}
    </p>
  </div>
</template>


<script>
export default {
  props: {
    items: {
      type: Array,
    },
  },
};
</script>

<style scoped>

.headers {
  display: flex;
  width: 600px;
  margin-right: auto;
  margin-left: auto;
}

.headersLine {
  display: flex;
  width: 150px;
  margin: 2px;
}

.itemList {
  display: flex;
  width: 600px;
  margin-right: auto;
  margin-left: auto;
}

.itemListLine {
  display: flex;
  width: 150px;
  margin: 2px;
}
</style>
